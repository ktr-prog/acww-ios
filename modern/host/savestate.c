// Host build: save states for testing (savestate.h). The game's whole state is memory of this process: the program's
// data and bss from the game's first variable to the last of the runtime's DS (modern/link/game_begin.c ..
// state_end.c: tools/configure_host.py links the frontend, SDL and this file outside that range), the page blocks
// the game's memory lives in (the main RAM, the heaps, VRAM: host_plat_pages_alloc) and the game threads' stacks
// (host_plat_stack_alloc). The game is held at a frame's end, inside frontend_backend_frame, so its coroutine's stack
// pointer is the rest of its state. A save copies all of that; a load puts it back and resumes the game where the
// save was taken. The blocks are never freed, so a saved block is still there when its state is loaded.
// (Everything is on the main thread: the game's coroutines and the frontend. The audio thread reads only the
// frontend's ring.)
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "host.h"
#include "host_mem.h"
#include "plat.h"
#include "savestate.h"

extern char gHostStateDataEnd[1], gHostStateBssEnd[1];
char *host_state_static_bss_end(void);  // (modern/link/state_end.c)

typedef struct { unsigned char *p; size_t n; } Region;
enum { MAX_REGIONS = 512, MAX_KEEP = 16 };
static Region sRegions[MAX_REGIONS];
static int sNRegions, sOverflow;
static Region sKeep[MAX_KEEP];
static int sNKeep;

// the state: the regions it covers (the program's ranges first) and their bytes, one after the other
static Region sSaved[MAX_REGIONS + 3];
static int sNSaved;
static unsigned char *sSnap;
static size_t sSnapCap;
static void *sSnapSp;
static int sChecked;  // 1 the ranges are fine, -1 they aren't (no save states)

void host_state_track(void *p, size_t n) {
    if (!p || !n) return;
    if (sNRegions == MAX_REGIONS) { sOverflow = 1; return; }
    sRegions[sNRegions++] = (Region){p, n};
}

void host_state_untrack(void *p) {
    for (int i = 0; i < sNRegions; i++)
        if (sRegions[i].p == p) { sRegions[i] = sRegions[--sNRegions]; return; }
}

void host_state_keep(void *p, size_t n) {
    for (int i = 0; i < sNKeep; i++)
        if (sKeep[i].p == p) return;
    if (sNKeep == MAX_KEEP) { sOverflow = 1; return; }
    sKeep[sNKeep++] = (Region){p, n};
}

static int in(const void *p, const char *lo, const char *hi) {
    return (uintptr_t)p >= (uintptr_t)lo && (uintptr_t)p < (uintptr_t)hi;
}

// the program's ranges: its data, its zero-initialised variables and (Mach-O, where they're apart: __common and
// __bss) its static zero-initialised ones; overlapping ones merged (ELF's one .bss). The count.
static int program_ranges(Region out[3]) {
    const char *lo[3] = {gHostGameDataBegin, gHostGameBssBegin, host_game_static_bss_begin()};
    const char *hi[3] = {gHostStateDataEnd, gHostStateBssEnd, host_state_static_bss_end()};
    for (int i = 0; i < 3; i++)  // (by start)
        for (int j = i + 1; j < 3; j++)
            if (lo[j] < lo[i]) {
                const char *t = lo[i]; lo[i] = lo[j], lo[j] = t;
                t = hi[i]; hi[i] = hi[j], hi[j] = t;
            }
    int n = 0;
    for (int i = 0; i < 3; i++) {
        if (n && lo[i] <= (const char *)out[n - 1].p + out[n - 1].n) {
            const char *end = (const char *)out[n - 1].p + out[n - 1].n;
            if (hi[i] > end) out[n - 1].n = (size_t)(hi[i] - (const char *)out[n - 1].p);
        } else {
            out[n++] = (Region){(unsigned char *)lo[i], (size_t)(hi[i] - lo[i])};
        }
    }
    return n;
}

// the link put the state's variables where they should be: each range the right way round, the game's and the
// runtime's variables inside, this file's outside
static int ranges_fine(void) {
    if (sChecked) return sChecked > 0;
    const char *lo[3] = {gHostGameDataBegin, gHostGameBssBegin, host_game_static_bss_begin()};
    const char *hi[3] = {gHostStateDataEnd, gHostStateBssEnd, host_state_static_bss_end()};
    const char *gend[3] = {gHostGameDataEnd, gHostGameBssEnd, host_game_static_bss_end()};
    const char *why = 0;
    const char *env = getenv("ACWW_SAVESTATES");
    if (env && atoi(env) == 0) why = "ACWW_SAVESTATES=0 (on iOS: the build's check found the ranges wrong)";
    for (int i = 0; i < 3 && !why; i++) {
        if (!(lo[i] < hi[i])) why = "a range is the wrong way round";
        else if (!in(gend[i], lo[i], hi[i])) why = "the game's variables aren't inside";
        if (!why && (in(sRegions, lo[i], hi[i]) || in(&sChecked, lo[i], hi[i]) || in(&sSnap, lo[i], hi[i])))
            why = "the save states' own variables are inside";
    }
    if (!why && !in(&gHostSysWork, lo[1], hi[1]) && !in(&gHostSysWork, lo[0], hi[0]))
        why = "the runtime's variables aren't inside";
    if (!why && sOverflow) why = "too many memory blocks";
    sChecked = why ? -1 : 1;
    if (why)
        fprintf(stderr, "savestate: off: %s (data %p-%p, bss %p-%p, static bss %p-%p)\n", why, (const void *)lo[0],
                (const void *)hi[0], (const void *)lo[1], (const void *)hi[1], (const void *)lo[2], (const void *)hi[2]);
    return sChecked > 0;
}

int host_state_have(void) { return sSnap != 0; }

int host_state_save(void *const *gameSp) {
    if (!ranges_fine()) return 0;
    uint64_t t0 = host_plat_time_ns();
    Region list[MAX_REGIONS + 3];
    int n = program_ranges(list);
    for (int i = 0; i < sNRegions; i++) list[n++] = sRegions[i];
    size_t total = 0;
    for (int i = 0; i < n; i++) total += list[i].n;
    if (total > sSnapCap) {
        unsigned char *b = realloc(sSnap, total);
        if (!b) {
            fprintf(stderr, "savestate: no memory for %zu MB\n", total >> 20);
            return 0;
        }
        sSnap = b;
        sSnapCap = total;
    }
    size_t at = 0;
    for (int i = 0; i < n; i++) {
        memcpy(sSnap + at, list[i].p, list[i].n);
        at += list[i].n;
    }
    memcpy(sSaved, list, (size_t)n * sizeof *list);
    sNSaved = n;
    sSnapSp = *gameSp;
    fprintf(stderr, "savestate: saved at frame %llu: %zu KB in %d blocks, %.1f ms\n", (unsigned long long)host_frame(),
            total >> 10, n, (double)(host_plat_time_ns() - t0) / 1e6);
    return 1;
}

int host_state_load(void **gameSp) {
    if (!sSnap || !ranges_fine()) return 0;
    uint64_t t0 = host_plat_time_ns();
    unsigned long long frame = host_frame();
    // the variables a load leaves alone: kept aside, put back after
    size_t keepN = 0;
    for (int i = 0; i < sNKeep; i++) keepN += sKeep[i].n;
    unsigned char *keep = malloc(keepN ? keepN : 1);
    if (!keep) return 0;
    size_t k = 0;
    for (int i = 0; i < sNKeep; i++) { memcpy(keep + k, sKeep[i].p, sKeep[i].n); k += sKeep[i].n; }
    size_t at = 0;
    for (int i = 0; i < sNSaved; i++) {
        memcpy(sSaved[i].p, sSnap + at, sSaved[i].n);
        at += sSaved[i].n;
    }
    k = 0;
    for (int i = 0; i < sNKeep; i++) { memcpy(sKeep[i].p, keep + k, sKeep[i].n); k += sKeep[i].n; }
    free(keep);
    *gameSp = sSnapSp;
    // (the frame pacing starts again from now: the saved state's next frame time is in the past)
    host_set_paused(1);
    host_set_paused(0);
    fprintf(stderr, "savestate: loaded at frame %llu: back to frame %llu, %.1f ms\n", frame,
            (unsigned long long)host_frame(), (double)(host_plat_time_ns() - t0) / 1e6);
    return 1;
}
