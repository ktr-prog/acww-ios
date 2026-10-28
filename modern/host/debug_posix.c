// Host build: debugging aids built on POSIX signals (HOST_DEBUG_POSIX, host.h). None of them is needed to run the
// game; elsewhere the platform's debugger does these jobs.
// - Crash report (SIGSEGV, SIGBUS, SIGILL, SIGFPE): where, with a frame-pointer walk from the faulting context.
//   SIGFPE is fatal too: the game's divisions go through include/ds_div.h, so a hardware divide error is a bug.
// - SIGINT: the last register accesses, every game thread's backtrace, the exit-time reports (ACWW_IOHIST,
//   ACWW_PROFILE); the sound file and the frontend closed.
// - ACWW_PROFILE=1: samples the interrupted program counter (SIGPROF), written to acww_profile.txt at exit.
// - ACWW_WATCH=addr:len (x86 Linux): write-protects the page and logs the writes in the range (single-stepping the
//   writing instruction with the trap flag).
// The interrupted context's registers are read from the ucontext only where its layout is known (x86 Linux);
// elsewhere the report uses host_plat_print_backtrace() (plat.h).

#define _GNU_SOURCE
#include "host.h"

#if HOST_DEBUG_POSIX

#if defined(__APPLE__)
#include <dlfcn.h>
#include <mach-o/dyld.h>
#else
#include <link.h>
#endif
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/time.h>
#if defined(__APPLE__)
#include <sys/ucontext.h>  // (the types only: <ucontext.h> wants _XOPEN_SOURCE there)
#else
#include <ucontext.h>
#endif
#include <unistd.h>

#include "plat.h"

#if defined(__linux__) && defined(__x86_64__)
#define HAVE_UC_REGS 1
#define UC_PC(uc) ((uintptr_t)(uc)->uc_mcontext.gregs[REG_RIP])
#define UC_SP(uc) ((uintptr_t)(uc)->uc_mcontext.gregs[REG_RSP])
#define UC_BP(uc) ((uintptr_t)(uc)->uc_mcontext.gregs[REG_RBP])
#elif defined(__linux__) && defined(__i386__)
#define HAVE_UC_REGS 1
#define UC_PC(uc) ((uintptr_t)(uc)->uc_mcontext.gregs[REG_EIP])
#define UC_SP(uc) ((uintptr_t)(uc)->uc_mcontext.gregs[REG_ESP])
#define UC_BP(uc) ((uintptr_t)(uc)->uc_mcontext.gregs[REG_EBP])
#elif defined(__linux__) && defined(__aarch64__)
#define HAVE_UC_REGS 1
#define UC_PC(uc) ((uintptr_t)(uc)->uc_mcontext.pc)
#define UC_SP(uc) ((uintptr_t)(uc)->uc_mcontext.sp)
#define UC_BP(uc) ((uintptr_t)(uc)->uc_mcontext.regs[29])
#elif defined(__APPLE__) && defined(__aarch64__)
#define HAVE_UC_REGS 1
#define UC_PC(uc) ((uintptr_t)(uc)->uc_mcontext->__ss.__pc)
#define UC_SP(uc) ((uintptr_t)(uc)->uc_mcontext->__ss.__sp)
#define UC_BP(uc) ((uintptr_t)(uc)->uc_mcontext->__ss.__fp)
#elif defined(__APPLE__) && defined(__x86_64__)
#define HAVE_UC_REGS 1
#define UC_PC(uc) ((uintptr_t)(uc)->uc_mcontext->__ss.__rip)
#define UC_SP(uc) ((uintptr_t)(uc)->uc_mcontext->__ss.__rsp)
#define UC_BP(uc) ((uintptr_t)(uc)->uc_mcontext->__ss.__rbp)
#else
#define HAVE_UC_REGS 0
#endif
// the x86 trap flag (EFLAGS bit 8) for ACWW_WATCH's single step
#if HAVE_UC_REGS && (defined(__x86_64__) || defined(__i386__))
#define HAVE_UC_TRAPFLAG 1
#else
#define HAVE_UC_TRAPFLAG 0
#endif

#define PAGE 0x1000u

// the program's load bias (a PIE: where it was loaded; 0 for a fixed-address program), read at start-up: the reports
// print each code address also less it, the address addr2line and gdb take for the program file
static uintptr_t sLoadBias;
#if defined(__APPLE__)
// (Mach-O: image 0 is the program; the slide is its distance from the file's addresses, which start at 4 GB)
static void find_load_bias(void) { sLoadBias = (uintptr_t)_dyld_get_image_vmaddr_slide(0); }
#else
static int first_object(struct dl_phdr_info *info, size_t size, void *data) {
    (void)size;
    *(uintptr_t *)data = (uintptr_t)info->dlpi_addr;
    return 1;  // (the program itself is the first)
}
static void find_load_bias(void) { dl_iterate_phdr(first_object, &sLoadBias); }
#endif
static void print_code_address(const char *what, uintptr_t a) {
    if (sLoadBias && a >= sLoadBias) fprintf(stderr, "%s%08lx (%lx)", what, (unsigned long)a, (unsigned long)(a - sLoadBias));
    else fprintf(stderr, "%s%08lx", what, (unsigned long)a);
#if defined(__APPLE__)
    // (the nearest symbol the program exports: its functions, on Apple, where there is no addr2line on the device)
    Dl_info info;
    if (dladdr((const void *)a, &info) && info.dli_sname)
        fprintf(stderr, " %s+%#lx", info.dli_sname, (unsigned long)(a - (uintptr_t)info.dli_saddr));
#endif
}

// frame-pointer walk, staying within 1 MB above the stack pointer (host stacks are 512 KB, the main one is the
// process stack)
static void host_backtrace(uintptr_t *bp, uintptr_t sp) {
    for (int i = 0; i < 24; i++) {
        if ((uintptr_t)bp < sp || (uintptr_t)bp > sp + 0x100000 || ((uintptr_t)bp & (sizeof *bp - 1))) break;
        print_code_address("  from ", bp[1]);
        fprintf(stderr, "\n");
        sp = (uintptr_t)bp;
        bp = (uintptr_t *)bp[0];
    }
}

static void context_report(void *ctx) {
#if HAVE_UC_REGS
    ucontext_t *uc = ctx;
    host_backtrace((uintptr_t *)UC_BP(uc), UC_SP(uc));
#else
    (void)ctx;
#endif
    host_plat_print_backtrace();
}

static uintptr_t context_pc(void *ctx) {
#if HAVE_UC_REGS
    return UC_PC((ucontext_t *)ctx);
#else
    (void)ctx;
    return 0;
#endif
}

// ---- ACWW_WATCH=addr:len (x86 Linux: the trap flag)
#if HAVE_UC_TRAPFLAG
static uintptr_t sWatchLo, sWatchHi, sWatchPage, sWatchPc;
static int sWatchStep;
static u8 sWatchOld[64];
static void watch_init(void) {
    const char *w = getenv("ACWW_WATCH");
    if (!w) return;
    unsigned long a, n;
    if (sscanf(w, "%lx:%lx", &a, &n) != 2) return;
    sWatchLo = a;
    sWatchHi = a + (n > 64 ? 64 : n);
    sWatchPage = a & ~(uintptr_t)(PAGE - 1);
    mprotect((void *)sWatchPage, PAGE, PROT_READ);
}
// a write to the watched page: let the instruction run (page opened, trap flag) and compare after it
static int watch_fault(ucontext_t *uc, uintptr_t a) {
    if (!sWatchPage || (a & ~(uintptr_t)(PAGE - 1)) != sWatchPage) return 0;
    memcpy(sWatchOld, (void *)sWatchLo, sWatchHi - sWatchLo);
    sWatchPc = UC_PC(uc);
    sWatchStep = 1;
    mprotect((void *)sWatchPage, PAGE, PROT_READ | PROT_WRITE);
    uc->uc_mcontext.gregs[REG_EFL] |= 0x100;
    return 1;
}
static void on_trap(int sig, siginfo_t *si, void *ctx) {
    (void)sig; (void)si;
    ucontext_t *uc = ctx;
    uc->uc_mcontext.gregs[REG_EFL] &= ~0x100;
    if (!sWatchStep) return;
    sWatchStep = 0;
    if (memcmp(sWatchOld, (void *)sWatchLo, sWatchHi - sWatchLo)) {
        fprintf(stderr, "watch: frame %llu pc %08lx:", (unsigned long long)host_frame(), (unsigned long)sWatchPc);
        for (uintptr_t i = 0; i < sWatchHi - sWatchLo; i += 4) fprintf(stderr, " %08x", *(u32 *)(sWatchLo + i));
        fprintf(stderr, "\n");
        host_backtrace((uintptr_t *)UC_BP(uc), UC_SP(uc));
    }
    mprotect((void *)sWatchPage, PAGE, PROT_READ);
}
#else
static void watch_init(void) {
    if (getenv("ACWW_WATCH")) fprintf(stderr, "host: ACWW_WATCH needs x86 Linux\n");
}
#endif

// ---- ACWW_PROFILE=1: program counter samples (SIGPROF)
#define PROF_SLOTS 65536
static uintptr_t sProfAddr[PROF_SLOTS];
static u32 sProfCount[PROF_SLOTS];
static void on_prof(int sig, siginfo_t *si, void *ctx) {
    (void)sig; (void)si;
    uintptr_t pc = context_pc(ctx);
    u32 h = (u32)(pc >> 2) * 2654435761u >> 16;
    for (int i = 0; i < 16; i++, h = (h + 1) & (PROF_SLOTS - 1)) {
        if (sProfAddr[h] == pc || !sProfAddr[h]) { sProfAddr[h] = pc; sProfCount[h]++; return; }
    }
}
void host_debug_exit_dumps(void) {
    if (!getenv("ACWW_PROFILE")) return;
    FILE *f = fopen("acww_profile.txt", "w");
    if (!f) return;
    // (where this function is now: with its address in the binary (nm), the load offset of a position-independent
    // executable)
    fprintf(f, "# host_debug_exit_dumps %08lx\n", (unsigned long)(void *)host_debug_exit_dumps);
    for (int i = 0; i < PROF_SLOTS; i++)
        if (sProfCount[i]) fprintf(f, "%u %08lx\n", sProfCount[i], (unsigned long)sProfAddr[i]);
    fclose(f);
    fprintf(stderr, "host: profile in acww_profile.txt\n");
}

// ---- SIGINT: what the game was doing
static void on_int(int sig, siginfo_t *si, void *ctx) {
    (void)sig; (void)si;
    host_io_ring_dump();
    host_exit_dumps();
    extern void snd7_output_close(void);
    snd7_output_close();
    extern void frontend_shutdown(void);
    frontend_shutdown();
    fprintf(stderr, "host: interrupted at frame %llu, ", (unsigned long long)host_frame());
    print_code_address("pc ", context_pc(ctx));
    fprintf(stderr, "\n");
    context_report(ctx);
    extern void host_dump_threads(void (*)(uintptr_t *, uintptr_t));
    host_dump_threads(host_backtrace);
    _exit(3);
}

// ---- a crash: where
static void on_fatal_signal(int sig, siginfo_t *si, void *ctx) {
#if HAVE_UC_TRAPFLAG
    if (sig == SIGSEGV && watch_fault(ctx, (uintptr_t)si->si_addr)) return;
#endif
    fprintf(stderr, "host: signal %d (%s) at frame %llu, ", sig, strsignal(sig), (unsigned long long)host_frame());
    print_code_address("pc ", context_pc(ctx));
    fprintf(stderr, ", address %p\n", si->si_addr);
    if (sig == SIGFPE)
        fprintf(stderr, "host: (an integer division the DS wouldn't trap on: it wants DS_DIV / DS_MOD, include/ds_div.h)\n");
    context_report(ctx);
    // (every game thread's stack too: the one that crashed may have been called from another's request)
    extern void host_dump_threads(void (*)(uintptr_t *, uintptr_t));
    host_dump_threads(host_backtrace);
    fflush(stderr);
    extern void frontend_shutdown(void);
    frontend_shutdown();
    _exit(5);
}

// the crash handlers; again after melon_init: melonDS's JIT installs a SIGSEGV handler of its own (for its fast
// memory, unused here) that doesn't hand other faults on to this one
void host_debug_crash_handlers(void) {
    // (on a stack of their own: a stack overflow's fault can't run on the stack that overflowed)
    static int altstack;
    if (!altstack) {
        enum { ALT = 256 * 1024 };
        stack_t ss;
        memset(&ss, 0, sizeof ss);
        ss.ss_sp = mmap(0, ALT, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
        ss.ss_size = ALT;
        if (ss.ss_sp != MAP_FAILED && sigaltstack(&ss, 0) == 0) altstack = 1;
    }
    struct sigaction sa;
    memset(&sa, 0, sizeof sa);
    sa.sa_flags = SA_SIGINFO | SA_NODEFER | (altstack ? SA_ONSTACK : 0);
    sa.sa_sigaction = on_fatal_signal;
    sigaction(SIGSEGV, &sa, 0);
    sigaction(SIGBUS, &sa, 0);
    sigaction(SIGILL, &sa, 0);
    sigaction(SIGFPE, &sa, 0);
}

void host_debug_init(void) {
    find_load_bias();
    struct sigaction sa;
    memset(&sa, 0, sizeof sa);
    sa.sa_flags = SA_SIGINFO | SA_NODEFER;
    host_debug_crash_handlers();
#if HAVE_UC_TRAPFLAG
    sa.sa_sigaction = on_trap;
    sigaction(SIGTRAP, &sa, 0);
#endif
    sa.sa_sigaction = on_int;
    sigaction(SIGINT, &sa, 0);
    if (getenv("ACWW_PROFILE")) {
        if (!HAVE_UC_REGS) fprintf(stderr, "host: ACWW_PROFILE: no program counter on this host (all samples at 0)\n");
        sa.sa_sigaction = on_prof;
        sa.sa_flags = SA_SIGINFO | SA_RESTART;
        sigaction(SIGPROF, &sa, 0);
        struct itimerval pt = {{0, 1000}, {0, 1000}};
        setitimer(ITIMER_PROF, &pt, 0);
    }
    watch_init();
}

#else

void host_debug_init(void) {}
void host_debug_crash_handlers(void) {}
void host_debug_exit_dumps(void) {}

#endif
