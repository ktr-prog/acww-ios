// Host build: the game's side of the frontend (frontend.h). At each VBlank the frame goes to the AcwwShm block and the
// input comes from it, through the backend: frontend_sdl.c (in-process SDL2, the 64-bit builds) or frontend_viewer.c
// (the 64-bit viewer process, the i386 build). With ACWW_HEADLESS there is no backend at all: no window, no sound
// device, input only from ACWW_KEYS / ACWW_TOUCH.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "frontend.h"
#include "host.h"
#include "host_mem.h"
#include "savestate.h"

static AcwwShm *sShm;
HOST_STATE_KEEP(sShm)  // (this launch's block, not a loaded state's: savestate.c)

// the circle pad (include/host_input.h): from the frontend's block (frontend_sdl.c fills it)
int host_circle_pad(int *x, int *y) {
    uint32_t w = sShm ? sShm->stick : 0;
    *x = (int8_t)(w & 0xff);
    *y = (int8_t)(w >> 8 & 0xff);
    return (w >> 16 & 1) && (*x || *y);
}

#include "render.h"
#include "wgpu_display.h"

void frontend_init(const char *argv0) {
    if (getenv("ACWW_HEADLESS")) return;
    sShm = frontend_backend_open(argv0);
}

AcwwShm *frontend_shm(void) { return sShm; }

// scripted keys for headless runs: ACWW_KEYS="frame:frames:mask;..." (mask: the bits of AcwwShm.keys, hex)
static const char *sKeySpec;  // (the environment's string: this launch's, not a loaded state's)
HOST_STATE_KEEP(sKeySpec)
static u32 scripted_keys(void) {
    const char *spec = sKeySpec;
    if (!spec) spec = sKeySpec = getenv("ACWW_KEYS") ? getenv("ACWW_KEYS") : "";
    u32 f = (u32)host_frame(), k = 0;
    for (const char *p = spec; *p;) {
        unsigned fr, n, m;
        if (sscanf(p, "%u:%u:%x", &fr, &n, &m) == 3 && f >= fr && f < fr + n) k |= m;
        p = strchr(p, ';');
        if (!p) break;
        p++;
    }
    return k;
}

// ---- test runs: a map of the town and walking to places by themselves (docs/host.md, "Test scripts")
// ACWW_TOWNDUMP=<frame>:<file>: the scene's unit grid at that frame - '.' walkable, '#' not, and the structures (item
// codes 0x5000-0x50ff: houses, shops, the gate...) with their units. ACWW_GOTO="<frame>:x,z[;x,z...]": from that frame
// the D-pad steers the player to each unit (x, z; fractions allowed) in turn ("goto:" lines say when one is reached
// or the walk is stuck). Positions in units, as `ACWW_POSLOG`'s values / 2.
typedef struct { void *blocks; s32 width, height; } HostBlockMapView;  // (include/town/BlockMap.h's first members)
extern HostBlockMapView *gSceneBlockMap;
extern int TownMap_IsUnitWalkable(s32 x, s32 y, void *grid);
extern u16 *BlockMap_GetItemPtr(void *g, s32 hx, s32 hy, s32 lx, s32 ly, u8 layer);
extern void *PlayerActor_GetBodyPos(u32 idx);
static const char *sTownDump, *sGoto;
HOST_STATE_KEEP(sTownDump)
HOST_STATE_KEEP(sGoto)
static void town_dump(void) {
    if (!sTownDump) sTownDump = getenv("ACWW_TOWNDUMP") ? getenv("ACWW_TOWNDUMP") : "";
    unsigned at;
    char path[512];
    if (sscanf(sTownDump, "%u:%511s", &at, path) != 2 || at != (unsigned)host_frame()) return;
    HostBlockMapView *g = gSceneBlockMap;
    FILE *f = g ? fopen(path, "w") : 0;
    if (!f) return;
    fprintf(f, "blocks %d x %d\n", g->width, g->height);
    for (s32 z = 0; z < g->height * 16; z++) {
        for (s32 x = 0; x < g->width * 16; x++) fputc(TownMap_IsUnitWalkable(x, z, 0) ? '.' : '#', f);
        fputc('\n', f);
    }
    for (s32 z = 0; z < g->height * 16; z++)
        for (s32 x = 0; x < g->width * 16; x++) {
            u16 *c = BlockMap_GetItemPtr(g, x >> 4, z >> 4, x & 15, z & 15, 0);
            if (c && (*c & 0xff00) == 0x5000) fprintf(f, "structure %04x at %d %d\n", *c, x, z);
        }
    fclose(f);
}
static u32 goto_keys(void) {
    if (!sGoto) sGoto = getenv("ACWW_GOTO") ? getenv("ACWW_GOTO") : "";
    static int wp = 0, done = 0;
    static float bestD = 1e9f;
    static u64 bestAt = 0;
    unsigned start;
    int n = 0;
    if (done || sscanf(sGoto, "%u:%n", &start, &n) != 1 || host_frame() < start) return 0;
    const char *p = sGoto + n;
    for (int i = 0; i < wp && p; i++) { p = strchr(p, ';'); if (p) p++; }
    float tx, tz;
    if (!p || sscanf(p, "%f,%f", &tx, &tz) != 2) { done = 1; return 0; }
    const s32 *pos = PlayerActor_GetBodyPos(4);
    if (!pos) return 0;
    float x = pos[0] / 8192.0f, z = pos[2] / 8192.0f, dx = tx - x, dz = tz - z, d = dx * dx + dz * dz;
    if (d < 0.3f * 0.3f) {
        fprintf(stderr, "goto: at %.2f %.2f (waypoint %d) frame %llu\n", x, z, wp, (unsigned long long)host_frame());
        wp++, bestD = 1e9f, bestAt = host_frame();
        return 0;
    }
    if (d < bestD - 0.01f) bestD = d, bestAt = host_frame();
    else if (host_frame() - bestAt == 180) {  // (stuck: on to the next waypoint)
        fprintf(stderr, "goto: stuck at %.2f %.2f on the way to %.2f %.2f, frame %llu\n", x, z, tx, tz,
                (unsigned long long)host_frame());
        wp++, bestD = 1e9f, bestAt = host_frame();
        return 0;
    }
    u32 k = 0;
    float ax = dx < 0 ? -dx : dx, az = dz < 0 ? -dz : dz;
    if (ax > 0.15f && ax > az * 0.4f) k |= dx > 0 ? 0x10 : 0x20;  // Right / Left
    if (az > 0.15f && az > ax * 0.4f) k |= dz > 0 ? 0x80 : 0x40;  // Down / Up
    return k;
}

// at each VBlank: publish the frame, take the input
void frontend_frame(void) {
    u16 keyinput = 0x3ff;  // KEYINPUT: released bits are 1
    // the ARM7's button word (HW_BUTTON_XY_BUF, the system work area), OR-ed into KEYINPUT by PAD_Read: X bit 10, Y bit 11, debug bit 13
    // (released = 1); bit 15 clear = not folded
    u16 xy = 0x2c00;
    if (sShm) {
        // (not when the GPU shows them itself: they would be read back from it)
        if (!wdisp_presenting())
            for (int s = 0; s < 2; s++) memcpy(sShm->pixels + s * 256 * 192, render_screen(s), 256 * 192 * 4);
        __atomic_thread_fence(__ATOMIC_SEQ_CST);
        sShm->frame++;
        frontend_backend_frame(sShm);
        if (sShm->quit) host_quit();
        u32 k = sShm->keys;
        keyinput = (u16)(~k & 0x3ff);
        xy = (u16)(0x2000 | (~k & 0x0c00));
    }
    town_dump();
    u32 sk = scripted_keys() | goto_keys();
    keyinput &= (u16)~(sk & 0x3ff);
    xy &= (u16)~(sk & 0x0c00);
    *(u16 *)host_io(0x04000130) = keyinput;
    *(volatile u16 *)&gHostSysWork.buttonXY = xy;
}

// for the touch panel service (pxi7.c): the mouse or a finger on the bottom screen
int frontend_touch(int *x, int *y) {
    if (!sShm || !sShm->touch_down) return 0;
    *x = sShm->touch_x;
    *y = sShm->touch_y;
    return 1;
}

void frontend_shutdown(void) {
    if (sShm) frontend_backend_close();
}
