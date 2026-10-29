// Host build: loading an overlay resets its variables, as the DS does.
//
// On the DS, loading an overlay (FS_LoadOverlayImage + FS_StartOverlay) reads a fresh copy of its code and
// initialised data from the ROM, clears its bss and runs its static initialisers. The host has every overlay linked
// into the program. Each overlay unit lists its variables (HOST_OVERLAY_VARS, generated at the end of the unit by
// tools/host_overlay_vars.py) and may have an explicit initialiser, StaticInit_<unit>, for what the DS's __sinit did
// that isn't constant data; modern/gen/overlay_table.c collects them per overlay. Loading overlay N copies each of
// N's variables back from a copy taken at start-up (HostOverlay_Init, from main()), then runs N's units'
// StaticInit_<unit> in link order. Overlays that share an address range on the DS each keep their own variables here;
// each one is reset when it is loaded. ACWW_OVLOG=1 lists the loads.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef HOST_ASAN
#include <sanitizer/asan_interface.h>
#endif

#include "host.h"
#include "overlay.h"
#include "savestate.h"

static unsigned char **sPristine;  // per unit: its variables' bytes as linked, one after the other
static unsigned char *sKeep;       // per overlay id: not reset (ACWW_OVRESET)
HOST_STATE_KEEP(sPristine)
HOST_STATE_KEEP(sKeep)
static unsigned sMaxId;
static int sLog;

// ACWW_OVRESET=0 (for comparisons): the overlays' static initialisers run once at start-up, nothing is reset on
// loads; ACWW_OVRESET=-5,9: that for the overlays listed only
static void parse_keep(void) {
    const char *e = getenv("ACWW_OVRESET");
    for (unsigned i = 0; i < gHostOverlayUnitCount; i++) {
        if (gHostOverlayUnits[i].id > sMaxId) sMaxId = gHostOverlayUnits[i].id;
    }
    sKeep = calloc(sMaxId + 1, 1);
    if (!sKeep) host_fatal("overlay data copy");
    if (!e) return;
    if (!strcmp(e, "0")) {
        memset(sKeep, 1, sMaxId + 1);
        return;
    }
    if (*e != '-') return;
    for (const char *p = e + 1; *p;) {
        char *q;
        unsigned long id = strtoul(p, &q, 10);
        if (q == p) break;
        if (id <= sMaxId) sKeep[id] = 1;
        p = *q == ',' ? q + 1 : q;
    }
}

// From main(), before the game starts: after the static constructors, which construct some overlay objects (those
// whose classes have default constructors; StaticInit_<unit> constructs them again at every load) and touch no other
// overlay variable.
void HostOverlay_Init(void) {
    sLog = getenv("ACWW_OVLOG") != NULL;
    parse_keep();
    sPristine = calloc(gHostOverlayUnitCount, sizeof *sPristine);
    if (!sPristine) host_fatal("overlay data copy");
    size_t total = 0, nvars = 0;
    for (unsigned i = 0; i < gHostOverlayUnitCount; i++) {
        const HostOverlayVar *v = gHostOverlayUnits[i].vars;
        if (!v) continue;
        size_t n = 0;
        for (; v->addr; v++, nvars++) n += v->size;
        total += n;
        sPristine[i] = malloc(n ? n : 1);
        if (!sPristine[i]) host_fatal("overlay data copy");
        unsigned char *p = sPristine[i];
        for (v = gHostOverlayUnits[i].vars; v->addr; v++) {
            memcpy(p, v->addr, v->size);
            p += v->size;
        }
    }
    if (sLog) fprintf(stderr, "overlays: %u units, %zu variables, %zu bytes kept\n", gHostOverlayUnitCount, nvars, total);
    for (unsigned i = 0; i < gHostOverlayUnitCount; i++) {
        const HostOverlayUnit *u = &gHostOverlayUnits[i];
        if (sKeep[u->id] && u->staticInit) u->staticInit();
    }
}

// ACWW_OVLOG: what the reset undoes. Variables that are all zero as linked (the bss: GCC and clang put the
// zero-initialised ones there) count separately from the others (data), as the DS's two parts of an overlay.
typedef struct {
    size_t bytes, vars, changed;
    const char *first;
    size_t firstOff;
} ResetStats;

static void log_unit(const HostOverlayUnit *u, const unsigned char *pristine, ResetStats *data, ResetStats *bss) {
    for (const HostOverlayVar *v = u->vars; v->addr; v++) {
        int zero = 1;
        for (size_t k = 0; k < v->size && zero; k++) zero = pristine[k] == 0;
        ResetStats *s = zero ? bss : data;
        s->bytes += v->size;
        s->vars++;
        const unsigned char *cur = v->addr;
        for (size_t k = 0; k < v->size; k++) {
            if (cur[k] != pristine[k] && !s->changed++) {
                s->first = v->name;
                s->firstOff = k;
            }
        }
        pristine += v->size;
    }
}

void HostOverlay_Load(unsigned id) {
    if (id > sMaxId || sKeep[id]) return;
    ResetStats data = {0, 0, 0, "-", 0}, bss = {0, 0, 0, "-", 0};
    unsigned inits = 0;
    for (unsigned i = 0; i < gHostOverlayUnitCount; i++) {
        const HostOverlayUnit *u = &gHostOverlayUnits[i];
        if (u->id != id || !u->vars) continue;
        if (sLog) log_unit(u, sPristine[i], &data, &bss);
        const unsigned char *p = sPristine[i];
        for (const HostOverlayVar *v = u->vars; v->addr; v++) {
#ifdef HOST_ASAN
            // (the sanitizer variant: a variable that held a heap has its free space poisoned; the reset ends the heap)
            __asan_unpoison_memory_region(v->addr, v->size);
#endif
            memcpy(v->addr, p, v->size);
            p += v->size;
        }
    }
    // then the units' static initialisers, in link order (as the DS's .sinit list)
    for (unsigned i = 0; i < gHostOverlayUnitCount; i++) {
        const HostOverlayUnit *u = &gHostOverlayUnits[i];
        if (u->id != id || !u->staticInit) continue;
        u->staticInit();
        inits++;
    }
    if (sLog) {
        fprintf(stderr, "frame %llu: overlay %u loaded: data %zu bytes in %zu variables (%zu bytes changed, first in "
                "%s+%#zx), bss %zu bytes in %zu variables (%zu bytes not zero, first in %s+%#zx), %u constructors\n",
                (unsigned long long)host_frame(), id, data.bytes, data.vars, data.changed, data.first, data.firstOff,
                bss.bytes, bss.vars, bss.changed, bss.first, bss.firstOff, inits);
    }
}

