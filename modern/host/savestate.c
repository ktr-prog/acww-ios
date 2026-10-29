// Host build: save states (savestate.h). The game's whole state is memory of this process: the program's data from
// the game's first variable to the last of the runtime's DS (modern/link/game_begin.c .. state_end.c:
// tools/configure_host.py links the frontend, SDL and this file outside that range), the page blocks the game's
// memory lives in (the main RAM, the heaps, VRAM: host_plat_pages_alloc) and the game threads' stacks
// (host_plat_stack_alloc). The game is held at a frame's end, inside frontend_backend_frame, so its coroutine's stack
// pointer is the rest of its state. A save copies all of that; a load puts it back and resumes the game where the
// save was taken. The blocks are never freed, so a saved block is still there when its state is loaded.
//
// A state is also written to a file (STATE_FILE, in the working directory: on iOS the app's folder), so it can be
// loaded after the app was closed. The arena (plat.h) is at a fixed place, the same in every launch; the program is
// elsewhere in each (ASLR): the file keeps where it was, and a load moves every 8-byte value of the state (at any 4-byte boundary) that points
// into the old program by how far it moved (and into the old arena, in the rare launch that had it elsewhere). A word
// that only looks like such a pointer would be moved too: in data, a word pointing into the code that isn't a
// function's start is left alone (Mach-O: function_starts); the virtual clock's values (cycles and OS ticks, 64-bit)
// are near those addresses only around certain moments of play, and a state taken then loads only in the launch that
// took it (refused after). The file is only for this build (its id is in it), the HostPtr32 table travels with it,
// and the variables that hold this launch's own things (the frontend's block, the ROM image, open files:
// host_state_keep) keep theirs. The program is built without the stack protector: its canaries in the saved stacks'
// frames are per launch (tools/configure_host.py).
// (Everything is on the main thread: the game's coroutines and the frontend. The audio thread reads only the
// frontend's ring.)
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#if defined(__APPLE__)
#include <mach-o/dyld.h>
#include <mach-o/loader.h>
#endif

#include "host.h"
#include "host_mem.h"
#include "plat.h"
#include "savestate.h"

#define STATE_FILE "acww-state.bin"
#define STATE_MAGIC "ACWWST03"

extern char gHostStateDataEnd[1], gHostStateBssEnd[1];
char *host_state_static_bss_end(void);  // (modern/link/state_end.c)
unsigned int HostPtr32_Export(const void *const **ptrs);  // (ptr32.c)
void HostPtr32_Import(const void *const *ptrs, unsigned int n);

typedef struct { unsigned char *p; size_t n; int stack; } Region;
enum { MAX_REGIONS = 512, MAX_KEEP = 32, MAX_FOREIGN = 4 };
static Region sRegions[MAX_REGIONS];
static int sNRegions, sOverflow;
static Region sKeep[MAX_KEEP];
static int sNKeep;
static Region sForeign[MAX_FOREIGN];  // host_state_foreign: pointed into, not saved (the ROM image)
static int sNForeign;

// the state in memory: the regions it covers (the program's ranges first) and their bytes, one after the other
static Region sSaved[MAX_REGIONS + 3];
static int sNSaved;
static unsigned char *sSnap;
static size_t sSnapCap, sSnapSize;
static void *sSnapSp;
static int sChecked;     // 1 the ranges are fine, -1 they aren't (no save states)
static int sFileHave = -1;  // a state file is there (-1: not looked yet)
static char sMessage[96];

const char *host_state_message(void) { return sMessage; }
static void message(const char *m) { snprintf(sMessage, sizeof sMessage, "%s", m); }

static void track(void *p, size_t n, int stack) {
    if (!p || !n) return;
    if (sNRegions == MAX_REGIONS) { sOverflow = 1; return; }
    sRegions[sNRegions++] = (Region){p, n, stack};
}
void host_state_track(void *p, size_t n) { track(p, n, 0); }
void host_state_track_stack(void *p, size_t n) { track(p, n, 1); }

void host_state_untrack(void *p) {
    for (int i = 0; i < sNRegions; i++)
        if (sRegions[i].p == p) {
            memmove(&sRegions[i], &sRegions[i + 1], (size_t)(sNRegions - i - 1) * sizeof *sRegions);
            sNRegions--;
            return;
        }
}

void host_state_keep(void *p, size_t n) {
    for (int i = 0; i < sNKeep; i++)
        if (sKeep[i].p == p) return;
    if (sNKeep == MAX_KEEP) { sOverflow = 1; return; }
    sKeep[sNKeep++] = (Region){p, n};
}

void host_state_foreign(const void *p, size_t n) {
    if (!p || !n || sNForeign == MAX_FOREIGN) return;
    sForeign[sNForeign++] = (Region){(unsigned char *)p, n};
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

// ---- this build's id: Mach-O's UUID; elsewhere a hash of the program's size and of where the state's ranges and a
// few functions are in it (a rebuild that changes the layout changes it)
static uint64_t fnv(uint64_t h, const void *p, size_t n) {
    const unsigned char *b = p;
    for (size_t i = 0; i < n; i++) h = (h ^ b[i]) * 0x100000001b3ull;
    return h;
}
static uint64_t build_id(void) {
    uint64_t h = 0xcbf29ce484222325ull;
    uintptr_t lo = 0, hi = 0;
    host_plat_image_bounds(&lo, &hi);
    uintptr_t v[8] = {hi - lo, (uintptr_t)gHostGameDataBegin - lo, (uintptr_t)gHostStateDataEnd - lo,
                      (uintptr_t)gHostGameBssBegin - lo, (uintptr_t)gHostStateBssEnd - lo,
                      (uintptr_t)&host_state_save - lo, (uintptr_t)&host_frame - lo, sizeof(Region)};
    h = fnv(h, v, sizeof v);
#if defined(__APPLE__)
    const struct mach_header_64 *mh = (const struct mach_header_64 *)_dyld_get_image_header(0);
    if (mh && mh->magic == MH_MAGIC_64) {
        const unsigned char *c = (const unsigned char *)(mh + 1);
        for (uint32_t i = 0; i < mh->ncmds; i++) {
            const struct load_command *lc = (const struct load_command *)c;
            if (lc->cmd == LC_UUID) h = fnv(h, ((const struct uuid_command *)c)->uuid, 16);
            c += lc->cmdsize;
        }
    }
#endif
    return h;
}

// the virtual clock near [lo, hi)? Its cycles or its OS ticks (cycles / 64) within about a minute of it either side
// (a 64-bit time near a moved range's addresses could be taken for a pointer into it)
static int clock_near(uint64_t cycles, uint64_t lo, uint64_t hi) {
    for (int k = 0; k <= 6; k += 6) {
        uint64_t v = cycles >> k, margin = (1ull << 31) >> k;
        if (v + margin >= lo && v <= hi + margin) return 1;
    }
    return 0;
}

int host_state_have(void) {
    if (sSnap) return 1;
    if (sFileHave < 0) {
        FILE *f = fopen(STATE_FILE, "rb");
        sFileHave = f != 0;
        if (f) fclose(f);
    }
    return sFileHave;
}

// ---- the file: a header, the regions' places and sizes, the foreign ranges, the HostPtr32 table, then each region's
// bytes as pages (4 KB) with a map of the non-zero ones (the zero ones aren't written), then a checksum
typedef struct {
    char magic[8];
    uint64_t build, imageLo, imageHi, arenaBase, arenaUsed, gameSp, cycles;
    uint32_t nRegions, nForeign, nPtrs, reserved;
} FileHead;
enum { PAGE = 4096 };

static int zero_page(const unsigned char *p, size_t n) {
    for (size_t i = 0; i < n; i++)
        if (p[i]) return 0;
    return 1;
}

static void write_file(void) {
    uintptr_t lo = 0, hi = 0, abase = 0;
    size_t aused = 0;
    if (!host_plat_image_bounds(&lo, &hi)) { fprintf(stderr, "savestate: no program bounds: no file\n"); return; }
    host_plat_arena(&abase, &aused);
    const void *const *ptrs;
    unsigned nptrs = HostPtr32_Export(&ptrs);
    FileHead h;
    memset(&h, 0, sizeof h);
    memcpy(h.magic, STATE_MAGIC, 8);
    h.build = build_id(), h.imageLo = lo, h.imageHi = hi, h.arenaBase = abase, h.arenaUsed = aused;
    h.gameSp = (uintptr_t)sSnapSp, h.cycles = host_cycles();
    h.nRegions = (uint32_t)sNSaved, h.nForeign = (uint32_t)sNForeign, h.nPtrs = nptrs;
    FILE *f = fopen(STATE_FILE ".tmp", "wb");
    if (!f) { fprintf(stderr, "savestate: can't write %s.tmp\n", STATE_FILE); return; }
    uint64_t sum = 0xcbf29ce484222325ull;
#define PUT(p, n) do { fwrite((p), 1, (n), f); sum = fnv(sum, (p), (n)); } while (0)
    PUT(&h, sizeof h);
    for (int i = 0; i < sNSaved; i++) {
        uint64_t r[2] = {(uintptr_t)sSaved[i].p, sSaved[i].n | (uint64_t)(sSaved[i].stack != 0) << 63};
        PUT(r, sizeof r);
    }
    for (int i = 0; i < sNForeign; i++) {
        uint64_t r[2] = {(uintptr_t)sForeign[i].p, sForeign[i].n};
        PUT(r, sizeof r);
    }
    for (unsigned i = 0; i < nptrs; i++) {
        uint64_t p = (uintptr_t)ptrs[i];
        PUT(&p, sizeof p);
    }
    size_t at = 0, written = 0;
    unsigned char *map = 0;
    for (int i = 0; i < sNSaved; i++) {
        size_t n = sSaved[i].n, pages = (n + PAGE - 1) / PAGE;
        unsigned char *m = realloc(map, (pages + 7) / 8 + 1);
        if (!m) break;
        map = m;
        memset(map, 0, (pages + 7) / 8 + 1);
        for (size_t p = 0; p < pages; p++) {
            size_t len = p + 1 < pages ? PAGE : n - p * PAGE;
            if (!zero_page(sSnap + at + p * PAGE, len)) map[p / 8] |= (unsigned char)(1u << (p % 8));
        }
        PUT(map, (pages + 7) / 8);
        for (size_t p = 0; p < pages; p++) {
            if (!(map[p / 8] >> (p % 8) & 1)) continue;
            size_t len = p + 1 < pages ? PAGE : n - p * PAGE;
            PUT(sSnap + at + p * PAGE, len);
            written += len;
        }
        at += n;
    }
#undef PUT
    free(map);
    fwrite(&sum, 1, sizeof sum, f);
    int ok = !ferror(f);
    if (fclose(f) != 0) ok = 0;
    if (!ok || rename(STATE_FILE ".tmp", STATE_FILE) != 0) {
        fprintf(stderr, "savestate: can't write %s\n", STATE_FILE);
        remove(STATE_FILE ".tmp");
        return;
    }
    sFileHave = 1;
    fprintf(stderr, "savestate: %s written (%zu KB of state, %zu KB not zero)\n", STATE_FILE, at >> 10, written >> 10);
}

int host_state_save(void *const *gameSp) {
    if (!ranges_fine()) { message("SAVE STATES ARE OFF IN THIS BUILD"); return 0; }
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
            message("NO MEMORY FOR A STATE");
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
    sSnapSize = total;
    sSnapSp = *gameSp;
    write_file();
    fprintf(stderr, "savestate: saved at frame %llu: %zu KB in %d blocks, %.1f ms\n", (unsigned long long)host_frame(),
            total >> 10, n, (double)(host_plat_time_ns() - t0) / 1e6);
    message("STATE SAVED");
    return 1;
}

// put the state in sSaved / sSnap back (the keep list's variables keep theirs)
static int restore(void **gameSp) {
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
    return 1;
}

// ---- reading a file: into sSaved / sSnap at this launch's places, its pointers moved
typedef struct { uint64_t lo, hi; int64_t d; int image; } Move;

// Mach-O: the program's functions' starts (LC_FUNCTION_STARTS) and its code section, at this launch's places. A word
// of the state that points into the code is a function pointer (data) or a return address (a stack): in data, one
// that isn't a function's start is a number that only looks like a pointer (iOS has the program at 0x1xxxxxxxx: a
// pair of 32-bit values, the second 1, can look like one) and stays as it is.
static uintptr_t *sFuncs;
static size_t sNFuncs;
static uintptr_t sTextLo, sTextHi;
static void function_starts(void) {
#if defined(__APPLE__)
    static int done;
    if (done) return;
    done = 1;
    const struct mach_header_64 *mh = (const struct mach_header_64 *)_dyld_get_image_header(0);
    intptr_t slide = _dyld_get_image_vmaddr_slide(0);
    if (!mh || mh->magic != MH_MAGIC_64) return;
    const unsigned char *c = (const unsigned char *)(mh + 1);
    uint64_t textVm = 0, leVm = 0, leOff = 0;
    uint32_t fsOff = 0, fsSize = 0;
    int haveText = 0, haveLe = 0;
    for (uint32_t i = 0; i < mh->ncmds; i++) {
        const struct load_command *lc = (const struct load_command *)c;
        if (lc->cmd == LC_SEGMENT_64) {
            const struct segment_command_64 *sg = (const struct segment_command_64 *)c;
            if (!strcmp(sg->segname, "__TEXT")) {
                textVm = sg->vmaddr, haveText = 1;
                const struct section_64 *sec = (const struct section_64 *)(sg + 1);
                for (uint32_t k = 0; k < sg->nsects; k++)
                    if (!strncmp(sec[k].sectname, "__text", 16)) {
                        sTextLo = (uintptr_t)(sec[k].addr + slide);
                        sTextHi = (uintptr_t)(sec[k].addr + sec[k].size + slide);
                    }
            } else if (!strcmp(sg->segname, "__LINKEDIT")) {
                leVm = sg->vmaddr, leOff = sg->fileoff, haveLe = 1;
            }
        } else if (lc->cmd == LC_FUNCTION_STARTS) {
            const struct linkedit_data_command *ld = (const struct linkedit_data_command *)c;
            fsOff = ld->dataoff, fsSize = ld->datasize;
        }
        c += lc->cmdsize;
    }
    if (!haveText || !haveLe || !fsSize || sTextLo >= sTextHi) { sTextLo = sTextHi = 0; return; }
    const unsigned char *d = (const unsigned char *)(uintptr_t)(leVm + slide + (fsOff - leOff)), *e = d + fsSize;
    size_t cap = 0;
    uint64_t addr = textVm + (uint64_t)slide;
    while (d < e) {
        uint64_t delta = 0;
        int shift = 0;
        while (d < e) {
            unsigned char b = *d++;
            delta |= (uint64_t)(b & 0x7f) << shift;
            shift += 7;
            if (!(b & 0x80)) break;
        }
        if (!delta) break;
        addr += delta;
        if (sNFuncs == cap) {
            uintptr_t *f = realloc(sFuncs, (cap = cap ? cap * 2 : 65536) * sizeof *sFuncs);
            if (!f) { free(sFuncs); sFuncs = 0, sNFuncs = 0; sTextLo = sTextHi = 0; return; }
            sFuncs = f;
        }
        sFuncs[sNFuncs++] = (uintptr_t)addr;  // (in order: the deltas are positive)
    }
    if (!sNFuncs) sTextLo = sTextHi = 0;
#endif
}
static int function_start(uintptr_t a) {
    size_t lo = 0, hi = sNFuncs;
    while (lo < hi) {
        size_t mid = (lo + hi) / 2;
        if (sFuncs[mid] < a) lo = mid + 1;
        else hi = mid;
    }
    return lo < sNFuncs && sFuncs[lo] == a;
}

// a's place in this launch, if it points into a range that moved (inStack: a word of a stack)
static int moved(uint64_t a, const Move *m, int nm, int inStack, uint64_t *out) {
    for (int i = 0; i < nm; i++)
        if (a >= m[i].lo && a < m[i].hi) {
            uint64_t w = a + (uint64_t)m[i].d;
            if (m[i].image && !inStack && w >= sTextLo && w < sTextHi && !function_start((uintptr_t)w)) return 0;
            *out = w;
            return 1;
        }
    return 0;
}
// (every 4 bytes: the game's heaps align their blocks to 4 bytes, as on the DS, so a pointer in an object there is
// often at an address that is 4 mod 8)
static void relocate(unsigned char *buf, size_t n, uint64_t addr, int inStack, const Move *m, int nm) {
    size_t i = (size_t)((4 - (addr & 3)) & 3);
    while (i + 8 <= n) {
        uint64_t v, w;
        memcpy(&v, buf + i, 8);
        if (v && moved(v, m, nm, inStack, &w)) {
            memcpy(buf + i, &w, 8);
            i += 8;
        } else {
            i += 4;
        }
    }
}

static int load_file(void) {
    FILE *f = fopen(STATE_FILE, "rb");
    if (!f) { message("NO STATE SAVED"); return 0; }
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);
    unsigned char *file = size > (long)sizeof(FileHead) + 8 ? malloc((size_t)size) : 0;
    int ok = file && fread(file, 1, (size_t)size, f) == (size_t)size;
    fclose(f);
    FileHead h;
    if (ok) memcpy(&h, file, sizeof h);
    uint64_t sum;
    if (ok) memcpy(&sum, file + size - 8, 8);
    if (!ok || memcmp(h.magic, STATE_MAGIC, 8) || fnv(0xcbf29ce484222325ull, file, (size_t)size - 8) != sum) {
        free(file);
        fprintf(stderr, "savestate: %s unreadable or damaged\n", STATE_FILE);
        message("THE STATE FILE IS DAMAGED");
        return 0;
    }
    if (h.build != build_id()) {
        free(file);
        fprintf(stderr, "savestate: %s is from another build of the app\n", STATE_FILE);
        message("THE STATE IS FROM ANOTHER VERSION");
        return 0;
    }
    uintptr_t lo = 0, hi = 0, abase = 0;
    size_t aused = 0;
    host_plat_image_bounds(&lo, &hi);
    int haveArena = host_plat_arena(&abase, &aused);
    Move mv[2 + MAX_FOREIGN];
    int nm = 0;
    int64_t slide = (int64_t)(lo - h.imageLo), adelta = (int64_t)(abase - h.arenaBase);
    if (slide) mv[nm++] = (Move){h.imageLo, h.imageHi, slide, 1};
    if (adelta && h.arenaUsed) mv[nm++] = (Move){h.arenaBase, h.arenaBase + h.arenaUsed, adelta, 0};
    if (slide) function_starts();
    // (a state taken when the clock was near a range that moved: refused, see the top)
    if ((slide && clock_near(h.cycles, h.imageLo, h.imageHi)) ||
        (adelta && h.arenaUsed && clock_near(h.cycles, h.arenaBase, h.arenaBase + h.arenaUsed))) {
        free(file);
        fprintf(stderr, "savestate: the state was saved at a moment that can't be moved to this launch\n");
        message("THIS STATE LOADS ONLY BEFORE A RESTART");
        return 0;
    }
    const unsigned char *p = file + sizeof h, *end = file + size - 8;
    const uint64_t *regs = (const uint64_t *)p;
    p += (size_t)h.nRegions * 16;
    const uint64_t *foreign = (const uint64_t *)p;
    p += (size_t)h.nForeign * 16;
    const uint64_t *fptrs = (const uint64_t *)p;
    p += (size_t)h.nPtrs * 8;
    if (h.nRegions > MAX_REGIONS + 3 || p > end) { free(file); message("THE STATE FILE IS DAMAGED"); return 0; }
    for (uint32_t i = 0; i < h.nForeign && i < (uint32_t)sNForeign; i++) {
        int64_t d = (int64_t)((uintptr_t)sForeign[i].p - foreign[2 * i]);
        if (d && foreign[2 * i + 1] == sForeign[i].n)
            mv[nm++] = (Move){foreign[2 * i], foreign[2 * i] + foreign[2 * i + 1], d, 0};
    }
    // the regions at this launch's places: the program's ranges with the program, the rest in the arena
    Region list[MAX_REGIONS + 3];
    size_t total = 0;
    for (uint32_t i = 0; i < h.nRegions; i++) {
        uint64_t a = regs[2 * i], n = regs[2 * i + 1] & ~(1ull << 63), na;
        int stack = (int)(regs[2 * i + 1] >> 63);
        if (a >= h.imageLo && a < h.imageHi) na = a + (uint64_t)slide;
        else if (haveArena && a >= h.arenaBase && a + n <= h.arenaBase + h.arenaUsed) na = a + (uint64_t)adelta;
        else { free(file); message("THE STATE CAN'T BE LOADED HERE"); fprintf(stderr, "savestate: a block outside the arena\n"); return 0; }
        list[i] = (Region){(unsigned char *)(uintptr_t)na, (size_t)n, stack};
        total += (size_t)n;
    }
    // the program's ranges must be this build's
    Region prog[3];
    int np = program_ranges(prog);
    for (int i = 0; i < np; i++)
        if (i >= (int)h.nRegions || list[i].p != prog[i].p || list[i].n != prog[i].n) {
            free(file);
            message("THE STATE IS FROM ANOTHER VERSION");
            return 0;
        }
    unsigned char *snap = malloc(total ? total : 1);
    if (!snap) { free(file); message("NO MEMORY FOR A STATE"); return 0; }
    size_t at = 0;
    for (uint32_t i = 0; i < h.nRegions && ok; i++) {
        size_t n = list[i].n, pages = (n + PAGE - 1) / PAGE;
        const unsigned char *map = p;
        p += (pages + 7) / 8;
        if (p > end) { ok = 0; break; }
        for (size_t q = 0; q < pages; q++) {
            size_t len = q + 1 < pages ? PAGE : n - q * PAGE;
            if (map[q / 8] >> (q % 8) & 1) {
                if (p + len > end) { ok = 0; break; }
                memcpy(snap + at + q * PAGE, p, len);
                p += len;
            } else {
                memset(snap + at + q * PAGE, 0, len);
            }
        }
        if (nm) relocate(snap + at, n, regs[2 * i], list[i].stack, mv, nm);
        at += n;
    }
    if (!ok) { free(snap), free(file); message("THE STATE FILE IS DAMAGED"); return 0; }
    // ACWW_STATE_CHECK: words (at any alignment) still pointing into the old program after the moving - none expected
    if (getenv("ACWW_STATE_CHECK")) {
        size_t a2 = 0; int shown = 0;
        for (uint32_t i = 0; i < h.nRegions; i++) {
            for (size_t o = 0; o + 8 <= list[i].n; o += 1) {
                uint64_t v; memcpy(&v, snap + a2 + o, 8);
                if (v >= h.imageLo && v < h.imageHi && shown++ < 60)
                    fprintf(stderr, "savestate: stale: block %u (%p) offset %zx%s: %llx (program + %llx)\n", i, (void *)list[i].p, o,
                            list[i].stack ? " (a stack)" : "", (unsigned long long)v, (unsigned long long)(v - h.imageLo));
            }
            a2 += list[i].n;
        }
        fprintf(stderr, "savestate: %d stale words\n", shown);
    }
    // the arena as the state had it (its blocks accessible: this launch may not have handed them all out yet)
    if (haveArena) {
        if (h.arenaUsed > aused && !host_plat_arena_set_used((size_t)h.arenaUsed)) {
            free(snap), free(file);
            message("THE STATE CAN'T BE LOADED HERE");
            return 0;
        }
        for (uint32_t i = (uint32_t)np; i < h.nRegions; i++) host_plat_commit(list[i].p, list[i].n);
    }
    // the HostPtr32 table, moved
    const void **ptrs = malloc((h.nPtrs ? h.nPtrs : 1) * sizeof *ptrs);
    if (!ptrs) { free(snap), free(file); message("NO MEMORY FOR A STATE"); return 0; }
    for (uint32_t i = 0; i < h.nPtrs; i++) {
        uint64_t v = fptrs[i], w;
        ptrs[i] = (const void *)(uintptr_t)(moved(v, mv, nm, 0, &w) ? w : v);
    }
    HostPtr32_Import(ptrs, h.nPtrs);
    free(ptrs);
    // the in-memory state is now this one; the game's blocks are the file's (later ones of this launch: let go)
    free(sSnap);
    sSnap = snap, sSnapCap = sSnapSize = total;
    memcpy(sSaved, list, h.nRegions * sizeof *list);
    sNSaved = (int)h.nRegions;
    uint64_t sp = h.gameSp, nsp;
    sSnapSp = (void *)(uintptr_t)(moved(sp, mv, nm, 1, &nsp) ? nsp : sp);
    sNRegions = 0;
    for (uint32_t i = (uint32_t)np; i < h.nRegions; i++) track(list[i].p, list[i].n, list[i].stack);
    free(file);
    fprintf(stderr, "savestate: %s read (program moved by %lld, arena by %lld)\n", STATE_FILE, (long long)slide,
            (long long)adelta);
    return 1;
}

int host_state_load(void **gameSp) {
    if (!ranges_fine()) { message("SAVE STATES ARE OFF IN THIS BUILD"); return 0; }
    uint64_t t0 = host_plat_time_ns();
    unsigned long long frame = host_frame();
    if (!sSnap && !load_file()) return 0;
    if (!restore(gameSp)) { message("NO MEMORY FOR A STATE"); return 0; }
    fprintf(stderr, "savestate: loaded at frame %llu: back to frame %llu, %.1f ms\n", frame,
            (unsigned long long)host_frame(), (double)(host_plat_time_ns() - t0) / 1e6);
    message("STATE LOADED");
    return 1;
}
