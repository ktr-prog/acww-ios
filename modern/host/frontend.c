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
#include "sys/OSThread.h"

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

// ACWW_WFC_TEST=<frame>: from that frame (in the field), what the gate's Copper does to connect to Nintendo WFC
// (SpNpcCopperTalk::startWifiLogin, waitWifiLogin: src/ov048), with "wfc:" lines on how it goes. Called from the game's
// main loop after the frame's tasks (Main_PostTaskStub).
#if defined(__APPLE__)
#define CXX_NAME(n) "_" n
#else
#define CXX_NAME(n) n
#endif
extern void NetOverlay_LoadWifi(void);
extern void NetOverlay_AssertAny(void) __asm__(CXX_NAME("_Z20NetOverlay_AssertAnyv"));  // (C++ functions)
extern void NetOverlay_AssertWifi(void) __asm__(CXX_NAME("_Z21NetOverlay_AssertWifiv"));
extern void Comm_Start(s32 a, u32 b, u32 c);
extern int Net_PollConnected(void);
extern u32 Net_GetError(void);
extern u8 *Net_GetWifiFriendList(void);
extern int GameStats_Upload(void), GameStats_PollUpload(void), GameStats_Download(void), GameStats_PollDownload(void);
extern int AxMail_DownloadMail(void), AxMail_PollMail(void), AxMail_DownloadBbs(void), AxMail_PollBbs(void);
extern void Net_ClearWifiError(void);
extern int Net_WifiShutdownStepExt(void), Comm_End(void);
extern void NetOverlay_Restore(void);
extern void *PlayerActor_Get(s32 idx);
extern s32 PlayerActor_RequestPitfallFall(void *o, s32 *unit, s32 b, s32 c);
static const char *sWfcTest, *sPitfallTest;
HOST_STATE_KEEP(sWfcTest)
HOST_STATE_KEEP(sPitfallTest)
// ACWW_PITFALL_TEST=<frame>: the player falls into a pitfall where they stand
static void pitfall_test(void) {
    if (!sPitfallTest) sPitfallTest = getenv("ACWW_PITFALL_TEST") ? getenv("ACWW_PITFALL_TEST") : "";
    static int done;
    unsigned at;
    if (done || !*sPitfallTest || sscanf(sPitfallTest, "%u", &at) != 1 || host_frame() < at) return;
    done = 1;
    const s32 *pos = PlayerActor_GetBodyPos(4);
    void *o = PlayerActor_Get(4);
    if (!pos || !o) return;
    s32 unit[2] = {pos[0] / 8192, pos[2] / 8192};
    fprintf(stderr, "pitfall: at %d,%d: %d\n", unit[0], unit[1], PlayerActor_RequestPitfallFall(o, unit, 6, -1));
}
// ACWW_EMOTION_TEST=<frame>:<first>[:<last>]: the local player does the emotions first..last (as the chat menu's
// emotion keys do), one after the other: each logged (its action, body animations, play modes and effects), and
// any that hasn't ended after 20 seconds, or whose effects are still alive after it ended, reported as broken
extern s32 PlayerActor_RequestEmotion(u8 *p);
extern int PlayerActor_HostEmotionState(void *o, s32 *action, u32 *anim, u32 *mode, s32 *effects);
static const char *sEmotionTest;
static void emotion_test(void) {
    if (!sEmotionTest) sEmotionTest = getenv("ACWW_EMOTION_TEST") ? getenv("ACWW_EMOTION_TEST") : "";
    static unsigned at, id, last;
    static int n = -2;
    static unsigned long long started;
    static int seen, bad, anims[8], nAnims, modes[8], maxFx;
    if (n == -2) {
        n = sscanf(sEmotionTest, "%u:%u:%u", &at, &id, &last);
        if (n == 2) last = id;
    }
    if (n < 2 || id == 0 || host_frame() < at || id > last) return;
    void *o = PlayerActor_Get(4);
    s32 action, fx[4];
    u32 anim, mode;
    if (!o || !PlayerActor_HostEmotionState(o, &action, &anim, &mode, fx)) return;
    int alive = (fx[0] != -1) + (fx[1] != -1) + (fx[2] != -1) + (fx[3] != -1);
    if (!started) {
        if (action != 5 && action != 1 && action != 2 && action != 16) return;  // (idle first)
        u8 b = (u8)id;
        s32 r = PlayerActor_RequestEmotion(&b);
        started = host_frame(), seen = 0, nAnims = 0, maxFx = 0;
        if (!r) fprintf(stderr, "emotion %u: not taken (action %d)\n", id, action), started = 0, id++;
        return;
    }
    if (action == 20) {
        seen = 1;
        if (alive > maxFx) maxFx = alive;
        if (!nAnims || anims[nAnims - 1] != (int)anim) {
            if (nAnims < 8) anims[nAnims] = (int)anim, modes[nAnims] = (int)mode, nAnims++;
        }
    }
    unsigned long long t = host_frame() - started;
    if ((seen && action != 20) || t > 1200 || (!seen && t > 60)) {
        char a[120] = "";
        for (int i = 0; i < nAnims; i++) snprintf(a + strlen(a), sizeof a - strlen(a), " %d/%d", anims[i], modes[i]);
        const char *why = !seen ? "never started" : t > 1200 ? "never ended" : alive ? "effects left alive" : "";
        if (*why) bad++;
        fprintf(stderr, "emotion %u: %llu frames, animations/modes%s, effects at most %d, then action %d%s%s\n", id, t, a,
                maxFx, action, *why ? ": BROKEN, " : "", why);
        started = 0, id++;
        if (id > last) fprintf(stderr, "emotion: done, %d broken\n", bad);
    }
}
// ACWW_LIE_TEST=<frame>: the local player lies down where it stands, as in a bed (the action PlayerActor_SetupLieInBed
// starts), for a look at the pose (ACWW_SHOT)
extern s32 PlayerActor_RequestLieInBed(void *o, u32 a, u32 b, u32 c);
static void lie_test(void) {
    static const char *spec;
    static int done;
    unsigned at;
    if (!spec) spec = getenv("ACWW_LIE_TEST") ? getenv("ACWW_LIE_TEST") : "";
    if (done || sscanf(spec, "%u", &at) != 1 || host_frame() < at) return;
    done = 1;
    void *o = PlayerActor_Get(4);
    if (o) fprintf(stderr, "lie: %d (frame %llu)\n", PlayerActor_RequestLieInBed(o, 1, 6, ~0u), (unsigned long long)host_frame());
}
// ACWW_LIGHTS_TEST=<frame>:<item>: that building's lights (BuildingLights) on at that frame, by day too - the gate's
// come on when it opens
extern int Building_HostSetLit(u32 item);
static void lights_test(void) {
    static const char *spec;
    unsigned at, item;
    if (!spec) spec = getenv("ACWW_LIGHTS_TEST") ? getenv("ACWW_LIGHTS_TEST") : "";
    if (sscanf(spec, "%u:%x", &at, &item) != 2 || host_frame() < at) return;
    static int done;
    if (!done) done = 1, fprintf(stderr, "lights: %x: %d\n", item, Building_HostSetLit(item));
}
// ACWW_TEST_CA=<the modulus, 256 hex digits>: the "Nintendo CA" root the SSL client trusts gets that key (a test
// server's chain, for the connection tests: tools/wfc_test_server.py)
extern u8 data_ov065_0228c0b8[128];
static const char *sTestCa;
HOST_STATE_KEEP(sTestCa)
static void test_ca(void) {
    if (!sTestCa) sTestCa = getenv("ACWW_TEST_CA") ? getenv("ACWW_TEST_CA") : "";
    if (strlen(sTestCa) < 256) return;
    for (int i = 0; i < 128; i++) {
        unsigned v;
        if (sscanf(sTestCa + i * 2, "%2x", &v) == 1) data_ov065_0228c0b8[i] = (u8)v;
    }
}
volatile unsigned long long gHostGameFrames;  // (host_features.h)
// the OS's thread list, checked after each turn: a broken entry (a thread's memory freed and used again while the
// thread was still listed) is reported once, with the frame, before it brings the game down (acww-log.txt)
extern OSThreadInfo data_021fcc2c;
static void thread_list_check(void) {
    static int reported;
    if (reported) return;
    const OSThread *th = data_021fcc2c.list;
    for (int n = 0; th && n < 64; n++) {
        if (th->state > 2 || th->priority > 32 || th->id > 1000) {
            fprintf(stderr, "host: the thread list is broken at frame %llu: thread %p (id %u, state %u, priority %u)\n",
                    (unsigned long long)host_frame(), (const void *)th, th->id, th->state, th->priority);
            reported = 1;
            return;
        }
        th = th->next;
    }
}
// The network heap's blocks, while the game is online (Nintendo WFC): each block given out and taken back is checked
// against the blocks out, and the first time a block given out overlaps one still out, or a block not out is taken
// back, both are reported with who asked (acww-log.txt) - memory used twice breaks the game soon after.
extern void *gNetHeap;
extern void Heap_HostTrace(void *h, void (*a)(void *, void *, u32, s32), void (*f)(void *, void *));
void host_print_code_address(const char *what, uintptr_t a);
enum { NET_BLOCKS = 4096 };
static struct NetBlock { uintptr_t p; u32 n; uintptr_t by[3]; } sNetBlocks[NET_BLOCKS];
static int sNetBlockCount, sNetReports;
static void *sNetTraced;
static void net_who(const uintptr_t *by) {
    for (int i = 0; i < 3 && by[i]; i++) host_print_code_address(i ? " <- " : " by ", by[i]);
    fputc('\n', stderr);
}
// the callers past the hook and the heap's own two frames (the frame pointer chain: x86-64 and arm64 alike keep the
// caller's frame pointer and the return address at the frame pointer), at most 3, checked to stay on this stack
static void net_callers(uintptr_t by[3]) {
    by[0] = by[1] = by[2] = 0;
    uintptr_t *fp = (uintptr_t *)__builtin_frame_address(0), lo = (uintptr_t)fp;
    for (int i = 0, k = 0; i < 6 && k < 3; i++) {
        if (!fp || ((uintptr_t)fp & 7) || (uintptr_t)fp < lo || (uintptr_t)fp > lo + (1u << 20)) break;
        uintptr_t ret = fp[1];
        if (i >= 2) by[k++] = ret;  // (0: the hook's caller, Heap::alloc/free; 1: its caller, Heap_Alloc...)
        fp = (uintptr_t *)fp[0];
    }
}
#define NET_CALLERS(by) uintptr_t by[3]; net_callers(by);
static int sNetLogged;  // (every block given out and taken back, the first 600: who and where, acww-log.txt)
// the network heap's block lists checked at each of its allocations and frees and each frame (the NNS expanded heap's
// structs, as laid out on 64-bit hosts): the first broken block reported - something wrote past its own block
// (the reports go to the log, the game keeps on)
typedef struct NetHeapBlock { u16 sig, attr; u32 size; struct NetHeapBlock *prev, *next; } NetHeapBlock;
typedef struct NetHeapHead {
    u32 sig; void *lp, *ln; void *ch, *ct; u16 cn, co; uintptr_t start, end; u32 attr;
    NetHeapBlock *freeHead, *freeTail, *usedHead, *usedTail;
} NetHeapHead;
static int sHeapBad;
static void heap_check(const char *where) {
    if (sHeapBad || !gNetHeap) return;
    extern void *Heap_HostHandle(void *h);
    NetHeapHead *h = (NetHeapHead *)Heap_HostHandle(gNetHeap);
    for (int l = 0; l < 2; l++) {
        NetHeapBlock *prev = 0;
        int n = 0;
        for (NetHeapBlock *b = l ? h->usedHead : h->freeHead; b; prev = b, b = b->next, n++) {
            const char *bad = 0;
            if ((uintptr_t)b < h->start || (uintptr_t)b >= h->end) bad = "outside";
            else if (b->sig != (l ? 0x5544 : 0x4652)) bad = "signature";
            else if (b->prev != prev) bad = "prev link";
            else if ((uintptr_t)(b + 1) + b->size > h->end) bad = "size";
            if (bad || n > 100000) {
                fprintf(stderr, "net heap: broken (%s list, block %d at %p: %s; sig %#x size %#x prev %p next %p) at %s, frame %llu\n",
                        l ? "used" : "free", n, (void *)b, bad ? bad : "loop", bad && !strcmp(bad, "outside") ? 0 : b->sig,
                        bad && !strcmp(bad, "outside") ? 0 : b->size, (void *)prev, bad && !strcmp(bad, "outside") ? 0 : (void *)b->next,
                        where, (unsigned long long)host_frame());
                sHeapBad = 1;
                return;
            }
        }
    }
}
static void net_alloc_hook(void *heap, void *p, u32 n, s32 align) {
    (void)align;
    if (heap != gNetHeap || !p) return;
    heap_check("alloc");
    NET_CALLERS(by)
    if (sNetLogged < 600) {
        sNetLogged++;
        fprintf(stderr, "net heap: + %p %u (frame %llu)", p, n, (unsigned long long)host_frame());
        net_who(by);
    }
    uintptr_t a = (uintptr_t)p;
    for (int i = 0; i < sNetBlockCount && sNetReports < 20; i++)
        if (a < sNetBlocks[i].p + sNetBlocks[i].n && sNetBlocks[i].p < a + n) {
            fprintf(stderr, "net heap: block %p (%u bytes) given out over block %p (%u bytes) still out, frame %llu:\n  new",
                    p, n, (void *)sNetBlocks[i].p, sNetBlocks[i].n, (unsigned long long)host_frame());
            net_who(by);
            fprintf(stderr, "  old");
            net_who(sNetBlocks[i].by);
            sNetReports++;
        }
    if (sNetBlockCount < NET_BLOCKS) {
        struct NetBlock *b = &sNetBlocks[sNetBlockCount++];
        b->p = a, b->n = n, memcpy(b->by, by, sizeof by);
    }
}
static void net_free_hook(void *heap, void *p) {
    if (heap != gNetHeap || !p) return;
    heap_check("free");
    if (sNetLogged < 600) {
        NET_CALLERS(by)
        sNetLogged++;
        fprintf(stderr, "net heap: - %p (frame %llu)", p, (unsigned long long)host_frame());
        net_who(by);
    }
    for (int i = 0; i < sNetBlockCount; i++)
        if (sNetBlocks[i].p == (uintptr_t)p) { sNetBlocks[i] = sNetBlocks[--sNetBlockCount]; return; }
    if (sNetReports < 20) {
        NET_CALLERS(by)
        fprintf(stderr, "net heap: block %p taken back but not out, frame %llu:\n  ", p, (unsigned long long)host_frame());
        net_who(by);
        sNetReports++;
    }
}
static void net_heap_trace(void) {
    if (gNetHeap == sNetTraced) return;
    sNetTraced = gNetHeap, sNetBlockCount = 0;  // (a new network heap: its blocks from now; earlier ones unknown)
    if (gNetHeap) {
        Heap_HostTrace(gNetHeap, net_alloc_hook, net_free_hook);
        fprintf(stderr, "net heap: tracing %p (frame %llu)\n", gNetHeap, (unsigned long long)host_frame());
    }
}

void host_test_post_task(void) {
    gHostGameFrames++;
    frontend_backend_turn();
    net_heap_trace();
    heap_check("frame");
    if (getenv("ACWW_NET_FREE_LOG") && gNetHeap) {
        extern void Heap_HostFreeSpace(void *h, u32 *all, u32 *largest);
        static u32 last;
        u32 all, big;
        Heap_HostFreeSpace(gNetHeap, &all, &big);
        if (all != last) fprintf(stderr, "net heap: %u bytes free, %u in one block (frame %llu)\n", all, big, (unsigned long long)host_frame()), last = all;
    }
    thread_list_check();
    pitfall_test();
    emotion_test();
    lie_test();
    lights_test();
    test_ca();
    if (!sWfcTest) sWfcTest = getenv("ACWW_WFC_TEST") ? getenv("ACWW_WFC_TEST") : "";
    static int step;
    static u32 lastErr = ~0u;
    unsigned at;
    if (!*sWfcTest || sscanf(sWfcTest, "%u", &at) != 1 || host_frame() < at || step == 3) return;
    if (step == 0) {
        extern void Heap_HostFreeSpace(void *h, u32 *all, u32 *largest);
        u32 all = 0, big = 0;
        if (gNetHeap) Heap_HostFreeSpace(gNetHeap, &all, &big);
        fprintf(stderr, "wfc: connecting (frame %llu; the network heap has %u bytes free, %u in one block)\n",
                (unsigned long long)host_frame(), all, big);
        NetOverlay_LoadWifi();
        Comm_Start(4, 4, 2);
        step = 1;
        return;
    }
    NetOverlay_AssertAny();
    int c = Net_PollConnected();
    u32 e = Net_GetError();
    if (e != lastErr) fprintf(stderr, "wfc: error %#x (frame %llu)\n", e, (unsigned long long)host_frame()), lastErr = e;
    if (e && step < 4) { step = 3; return; }
    NetOverlay_AssertWifi();
    if (step == 1 && c) fprintf(stderr, "wfc: connected (frame %llu)\n", (unsigned long long)host_frame()), step = 2;
    if (step < 4 && Net_GetWifiFriendList()) {
        fprintf(stderr, "wfc: logged in, friend list there (frame %llu)\n", (unsigned long long)host_frame());
        step = 4;
        {   // (a GameSpy HTTP post made and freed: what the stats upload builds first)
            extern void *ghttpNewPost(void);
            extern void ghiFreePost(void *p);
            void *p = ghttpNewPost();
            fprintf(stderr, "wfc: a post: %p\n", p);
            if (p) ghiFreePost(p);
        }
    }
    if (step >= 4) {  // then what Copper does next (SpNpcCopperTalk's scripts 0xb-0x12): each start, then its poll
        static int (*const kStart[4])(void) = {GameStats_Download, GameStats_Upload, AxMail_DownloadMail, AxMail_DownloadBbs};
        static int (*const kPoll[4])(void) = {GameStats_PollDownload, GameStats_PollUpload, AxMail_PollMail, AxMail_PollBbs};
        static const char *const kName[4] = {"stats download", "stats upload", "mail", "bbs"};
        static int k, started;
        if (k >= 4) {
            // then off as Copper goes off (shutdownWifi, endComm), and again from the start: ACWW_WFC_TEST=<frame>:<times>
            NetOverlay_AssertWifi();
            if (!Net_WifiShutdownStepExt()) return;
            if (Comm_End()) NetOverlay_Restore();
            fprintf(stderr, "wfc: all done, off (frame %llu)\n", (unsigned long long)host_frame());
            unsigned times = 1;
            const char *c = strchr(sWfcTest, ':');
            if (c) times = (unsigned)atoi(c + 1);
            static unsigned done;
            if (++done < times) step = 0, k = 0, started = 0, lastErr = ~0u;
            else step = 3;
            return;
        }
        if (!started) {
            started = 1;
            int ok = kStart[k]();
            fprintf(stderr, "wfc: %s started: %d (frame %llu)\n", kName[k], ok, (unsigned long long)host_frame());
            if (!ok) k++, started = 0;
            return;
        }
        if (Net_GetError()) {
            fprintf(stderr, "wfc: %s: error %#x\n", kName[k], Net_GetError());
            Net_ClearWifiError();
            k++, started = 0;
        } else if (kPoll[k]()) {
            fprintf(stderr, "wfc: %s done (frame %llu)\n", kName[k], (unsigned long long)host_frame());
            k++, started = 0;
        }
    }
}

// a press that arrived at this screen refresh (a new touch, a button newly down): the game's main loop starts its next
// turn now (Main_VBlankCallback) - each press read at once. Taken once.
static int sInputWantsTurn;
static u32 sLastTouchDown, sLastKeys;
int host_input_wants_turn(void) {
    int r = sInputWantsTurn;
    sInputWantsTurn = 0;
    return r;
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
        if (sShm->touch_down && !sLastTouchDown) sInputWantsTurn = 1;
        sLastTouchDown = sShm->touch_down;
        keyinput = (u16)(~k & 0x3ff);
        xy = (u16)(0x2000 | (~k & 0x0c00));
    }
    town_dump();
    u32 sk = scripted_keys() | goto_keys();
    keyinput &= (u16)~(sk & 0x3ff);
    xy &= (u16)~(sk & 0x0c00);
    {   // (a button newly down: the window's or a test script's)
        u32 held = (~keyinput & 0x3ff) | (u32)(~xy & 0x0c00);
        if (held & ~sLastKeys) sInputWantsTurn = 1;
        sLastKeys = held;
    }
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
