// Host build runtime: platform services for POSIX systems (Linux, macOS). See plat.h.
#ifndef _WIN32

#define _GNU_SOURCE
#include <errno.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <time.h>
#include <unistd.h>
#if defined(__GLIBC__) || defined(__APPLE__)
#include <execinfo.h>
#define HAVE_EXECINFO 1
#endif
#if defined(__APPLE__)
#include <mach-o/dyld.h>
#include <mach-o/loader.h>
#endif

#include "plat.h"
#include "savestate.h"

uint64_t host_plat_time_ns(void) {
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return (uint64_t)t.tv_sec * 1000000000u + (uint64_t)t.tv_nsec;
}

void host_plat_sleep_until_ns(uint64_t t) {
#if defined(__linux__)
    // an absolute deadline: no drift from the time spent between computing it and sleeping
    struct timespec ts = {(time_t)(t / 1000000000u), (long)(t % 1000000000u)};
    while (clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &ts, 0) == EINTR) {}
#else
    // macOS has no clock_nanosleep: relative sleeps until the deadline has passed
    for (;;) {
        uint64_t now = host_plat_time_ns();
        if (now >= t) return;
        uint64_t d = t - now;
        struct timespec ts = {(time_t)(d / 1000000000u), (long)(d % 1000000000u)};
        nanosleep(&ts, 0);
    }
#endif
}

void host_plat_sleep_ms(unsigned ms) { host_plat_sleep_until_ns(host_plat_time_ns() + (uint64_t)ms * 1000000u); }

int host_plat_localtime(struct tm *out) {
    time_t now = time(0);
    return localtime_r(&now, out) != 0;
}

struct Start { void (*fn)(void *); void *arg; };
static void *thread_main(void *p) {
    struct Start s = *(struct Start *)p;
    free(p);
    s.fn(s.arg);
    return 0;
}
void host_plat_restart(char **argv) {
#ifdef __linux__
    execv("/proc/self/exe", argv);
#endif
    execvp(argv[0], argv);
}

int host_plat_thread_start(void (*fn)(void *), void *arg) {
    struct Start *s = malloc(sizeof *s);
    if (!s) return 0;
    s->fn = fn;
    s->arg = arg;
    // no signals for the new thread (it inherits this mask): the debugging handlers (debug_posix.c) look at the
    // game's thread
    sigset_t all, old;
    sigfillset(&all);
    pthread_sigmask(SIG_SETMASK, &all, &old);
    pthread_t t;
    int err = pthread_create(&t, 0, thread_main, s);
    pthread_sigmask(SIG_SETMASK, &old, 0);
    if (err) { free(s); return 0; }
    pthread_detach(t);
    return 1;
}

static size_t page_size(void) {
    static size_t page;
    if (!page) page = (size_t)sysconf(_SC_PAGESIZE);
    return page;
}

// ---- the arena (plat.h): one reservation at a fixed address, its pages made accessible as they are handed out
#if UINTPTR_MAX > 0xffffffffu
#define ARENA_AT ((uintptr_t)0x400000000ull)  // (16 GB: inside what iOS gives an app)
#define ARENA_SIZE ((size_t)512 << 20)
static unsigned char *sArena;
static size_t sArenaUsed;
static int sArenaTried;
static unsigned char *arena_take(size_t n) {
    if (!sArenaTried) {
        sArenaTried = 1;
        void *p = mmap((void *)ARENA_AT, ARENA_SIZE, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_NORESERVE, -1, 0);
        if (p == MAP_FAILED) p = 0;
        sArena = p;
        if (p && (uintptr_t)p != ARENA_AT)
            fprintf(stderr, "host: the memory arena is at %p, not %p (save states from other launches: moved)\n", p,
                    (void *)ARENA_AT);
    }
    if (!sArena || n > ARENA_SIZE - sArenaUsed) return 0;
    unsigned char *p = sArena + sArenaUsed;
    if (mprotect(p, n, PROT_READ | PROT_WRITE) != 0) return 0;
    sArenaUsed += n;
    return p;
}
int host_plat_arena(uintptr_t *base, size_t *used) {
    if (!sArenaTried) arena_take(0);
    if (!sArena) return 0;
    *base = (uintptr_t)sArena, *used = sArenaUsed;
    return 1;
}
int host_plat_arena_set_used(size_t used) {
    if (!sArena || used > ARENA_SIZE) return 0;
    if (used > sArenaUsed && mprotect(sArena + sArenaUsed, used - sArenaUsed, PROT_READ | PROT_WRITE) != 0) return 0;
    sArenaUsed = used;
    return 1;
}
int host_plat_commit(void *p, size_t n) {
    size_t page = page_size();
    uintptr_t a = (uintptr_t)p & ~(page - 1), e = ((uintptr_t)p + n + page - 1) & ~(page - 1);
    if (!sArena || a < (uintptr_t)sArena || e > (uintptr_t)sArena + ARENA_SIZE) return 0;
    return mprotect((void *)a, e - a, PROT_READ | PROT_WRITE) == 0;
}
#else
static unsigned char *arena_take(size_t n) { (void)n; return 0; }
int host_plat_arena(uintptr_t *base, size_t *used) { (void)base; (void)used; return 0; }
int host_plat_arena_set_used(size_t used) { (void)used; return 0; }
int host_plat_commit(void *p, size_t n) { (void)p; (void)n; return 0; }
#endif

// the stack's pages with one more below them that faults (an overflow is a crash, not a corruption)
void *host_plat_stack_alloc(size_t n, int flags) {
    size_t page = page_size();
    n = (n + page - 1) & ~(page - 1);
    (void)flags;
    unsigned char *p = arena_take(n + page);
    if (!p) p = mmap(0, n + page, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (p == MAP_FAILED || !p) return 0;
    mprotect(p, page, PROT_NONE);
    host_state_track(p + page, n);  // (a game thread's stack: savestate.c)
    return p + page;
}

void host_plat_stack_free(void *stack, size_t n) {
    if (!stack) return;
    host_state_untrack(stack);
#if UINTPTR_MAX > 0xffffffffu
    if (sArena && (unsigned char *)stack >= sArena && (unsigned char *)stack < sArena + ARENA_SIZE) return;  // (kept)
#endif
    size_t page = page_size();
    n = (n + page - 1) & ~(page - 1);
    munmap((unsigned char *)stack - page, n + page);
}

void *host_plat_pages_alloc(size_t n) {
    size_t page = page_size();
    void *p = arena_take((n + page - 1) & ~(page - 1));
    if (!p) p = mmap(0, n, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (p == MAP_FAILED || !p) return 0;
    host_state_track(p, n);  // (the game's memory: savestate.c)
    return p;
}

int host_plat_image_bounds(uintptr_t *lo, uintptr_t *hi) {
#if defined(__APPLE__)
    // the program's segments as loaded (__PAGEZERO and __LINKEDIT left out)
    const struct mach_header_64 *mh = (const struct mach_header_64 *)_dyld_get_image_header(0);
    intptr_t slide = _dyld_get_image_vmaddr_slide(0);
    if (!mh || mh->magic != MH_MAGIC_64) return 0;
    const unsigned char *c = (const unsigned char *)(mh + 1);
    uintptr_t l = UINTPTR_MAX, h = 0;
    for (uint32_t i = 0; i < mh->ncmds; i++) {
        const struct load_command *lc = (const struct load_command *)c;
        if (lc->cmd == LC_SEGMENT_64) {
            const struct segment_command_64 *sg = (const struct segment_command_64 *)c;
            if (strcmp(sg->segname, "__PAGEZERO") && strcmp(sg->segname, "__LINKEDIT") && sg->vmsize) {
                if (sg->vmaddr + slide < l) l = sg->vmaddr + slide;
                if (sg->vmaddr + slide + sg->vmsize > h) h = sg->vmaddr + slide + sg->vmsize;
            }
        }
        c += lc->cmdsize;
    }
    if (l >= h) return 0;
    *lo = l, *hi = h;
    return 1;
#elif defined(__ELF__)
    extern char __executable_start[], _end[];  // (the linkers' symbols: GNU ld, lld, gold)
    *lo = (uintptr_t)__executable_start;
    *hi = (uintptr_t)_end;
    return 1;
#else
    (void)lo; (void)hi;
    return 0;
#endif
}

int host_plat_backtrace(void **pcs, int n) {
#if HAVE_EXECINFO
    return backtrace(pcs, n);
#else
    (void)pcs; (void)n;
    return 0;
#endif
}

void host_plat_print_backtrace(void) {
#if HAVE_EXECINFO
    void *bt[32];
    int n = backtrace(bt, 32);
    backtrace_symbols_fd(bt, n, 2);
#endif
}

#endif
