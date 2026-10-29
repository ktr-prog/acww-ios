// Host build: the in-process SDL2 frontend backend (frontend.h; HOST_FRONTEND_SDL, the 64-bit builds;
// docs/portability.md 4.1). A window with both screens, keyboard and game controller input, the mouse or a finger on
// the bottom screen as the touch panel, the sound through an SDL audio callback that reads the AcwwShm ring.
//
// Keys (the defaults; F1 opens the key binding screen, frontend_ui.c): arrows = D-pad, X = A, Z = B, S = X, A = Y,
// Q = L, W = R, Enter = Start, Right Shift = Select (by position: the same keys on other layouts). F11: window size
// 1x/2x/3x; Alt+Enter: full screen; M: mute (unless a DS button has it).
// Game controllers (SDL's database): D-pad or left stick, the face buttons by position as on the DS (right = A,
// bottom = B, top = X, left = Y), shoulders or triggers = L / R, Start, Back = Select; Back + Start: the binding
// screen.
//
// The game's thread calls this at each VBlank, on whichever game stack is running: SDL (and the video driver under
// it) runs on a stack of its own instead (8 MB, a coroutine: frontend_backend_frame switches to it and back), so the
// game's 512 KB thread stacks needn't fit a GL driver's needs.
//
// With the screens on the GPU (ACWW_RENDERER3D=wgpu: render_on_gpu) the window is drawn by wgpu_display.c, through
// its native handles (X11 or Wayland; on macOS a Metal view's CAMetalLayer), at the window's resolution; else by an SDL renderer from the AcwwShm pixels.
//
// Pacing stays the runtime's (hw.c, 60 Hz on the host clock); ACWW_VSYNC=1 also waits for the display's vsync.
// Testing without a display: SDL_VIDEODRIVER=dummy (or offscreen), SDL_AUDIODRIVER=dummy (or disk, with
// SDL_DISKAUDIOFILE), and ACWW_WINSHOT="frame:file.ppm;..." writes what the window shows (its whole drawable, at
// its size) at those frames.
#if HOST_FRONTEND_SDL

#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_syswm.h>
#if defined(__APPLE__)
#include <SDL_metal.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "coro.h"
#include "frontend.h"
#include "frontend_ui.h"
#include "host.h"
#include "plat.h"
#include "render.h"
#include "savestate.h"
#include "wgpu_display.h"

// iOS (HOST_IOS; docs/ios.md): the window fills the screen and turns with the device; the screens and the on-screen
// controls (modern/host/ios/touch_controls.c) are laid out in the safe area each frame and drawn by SDL's renderer
// (Metal); fingers are the stylus and the buttons; the buttons hide while a game controller is connected (a touch
// beside the screens shows them again); the game pauses while the app is in the background; the frontend runs on
// the main thread's own stack (frontend_backend_main_loop), not a coroutine
#if HOST_IOS
#include "ios/ios.h"
#include "ios/touch_controls.h"
static TcLayout sLayout;
static TcState sTouch;
static int sShowControls = 1;
static SDL_Texture *sButtonTex[TC_COUNT][16];  // per control: [pressed] (the D-pad: [the directions held >> 4])
static int sButtonW[TC_COUNT], sButtonH[TC_COUNT];
static SDL_Texture *sSharp;  // the screens at a whole multiple of their size, smoothed down from there
static int sSharpK;
static volatile int sInBackground;
static float sPt = 1;  // the window's pixels per point
#endif

static AcwwShm *sShm;
static SDL_Window *sWin;
static SDL_Renderer *sRen;
static SDL_Texture *sTex, *sOverlayTex;
static int sGpu;  // the window drawn by wgpu_display.c
static SDL_AudioDeviceID sDev;
static int sScale = 2, sMute;
static SDL_GameController *sPads[4];

// ---- sound: the game makes it in step with its virtual time (paced to the host clock), the sound card plays it on
// its own clock. The queue between them is held near AUDIO_TARGET by playing up to 0.5% faster or slower (linear
// interpolation; not audible), which absorbs the clocks' drift and the game's frame-sized bursts. After a stall
// (the queue ran dry) it waits for half the target again; when far too much is queued it skips ahead. (The same as
// the viewer's, modern/host/viewer/viewer.c.)
#define AUDIO_TARGET 2200.0  // frames (67 ms), on top of what the device asks for at a time
static unsigned sDry, sSkips, sCalls;  // ACWW_AUDIO_INFO: printed at the end
static double sLevelSum;
static void audio_callback(void *user, Uint8 *stream, int len) {
    AcwwShm *shm = user;
    int16_t *out = (int16_t *)stream;
    uint32_t want = (uint32_t)len / 4, w = __atomic_load_n(&shm->audio_write, __ATOMIC_ACQUIRE);
    static uint32_t r;
    static double frac;
    static int primed, init;
    if (!init) { init = 1; r = w; }
    double target = AUDIO_TARGET + want;  // a margin above what one call takes
    uint32_t level = w - r;
    if (level > ACWW_AUDIO_FRAMES / 2 || level > 4 * target) {
        if (primed) sSkips++;
        r = w - (uint32_t)target;
        level = (uint32_t)target;
    }
    sLevelSum += level;
    sCalls++;
    if (!primed && level >= target / 2 + want) primed = 1;
    double err = (level - target) / target;
    double ratio = 1.0 + 0.005 * (err < -1 ? -1 : err > 1 ? 1 : err);
    int mute = __atomic_load_n(&sMute, __ATOMIC_RELAXED);
    uint32_t i = 0;
    for (; primed && i < want; i++) {
        if (w - r < 2) { primed = 0; sDry++; break; }  // ran dry
        const int16_t *a = &shm->audio[(r % ACWW_AUDIO_FRAMES) * 2], *b = &shm->audio[((r + 1) % ACWW_AUDIO_FRAMES) * 2];
        out[i * 2] = mute ? 0 : (int16_t)(a[0] + (b[0] - a[0]) * frac);
        out[i * 2 + 1] = mute ? 0 : (int16_t)(a[1] + (b[1] - a[1]) * frac);
        frac += ratio;
        while (frac >= 1.0) { frac -= 1.0; r++; }
    }
    memset(out + i * 2, 0, (want - i) * 4);
    __atomic_store_n(&shm->audio_read, r, __ATOMIC_RELEASE);
}

#if defined(__APPLE__)
static SDL_MetalView sMetalView;  // (the window's Metal view, wgpu_display.c's surface)
#endif

static void sdl_quit(void) {
    if (getenv("ACWW_AUDIO_INFO") && sCalls)
        fprintf(stderr, "frontend: sound: %u underruns, %u skips, mean queue %.0f frames\n", sDry, sSkips,
                sLevelSum / sCalls);
    if (sDev) SDL_CloseAudioDevice(sDev);
    sDev = 0;
#if defined(__APPLE__)
    if (sMetalView) SDL_Metal_DestroyView(sMetalView);
    sMetalView = 0;
#endif
    SDL_Quit();
}

static void open_audio(void) {
    if (getenv("ACWW_MUTE") || SDL_InitSubSystem(SDL_INIT_AUDIO) != 0) return;
    SDL_AudioSpec want, have;
    memset(&want, 0, sizeof want);
    want.freq = ACWW_AUDIO_RATE;
    want.format = AUDIO_S16SYS;
    want.channels = 2;
    want.samples = getenv("ACWW_AUDIO_SAMPLES") ? (Uint16)atoi(getenv("ACWW_AUDIO_SAMPLES")) : 512;
    want.callback = audio_callback;
    want.userdata = sShm;
    sDev = SDL_OpenAudioDevice(NULL, 0, &want, &have, 0);  // SDL converts to the device's rate and format
    if (!sDev) { fprintf(stderr, "frontend: no sound: %s\n", SDL_GetError()); return; }
#if HOST_IOS
    if (1)  // (always: into the log, acww-log.txt)
#else
    if (getenv("ACWW_AUDIO_INFO"))
#endif
        fprintf(stderr, "frontend: sound %d Hz, format %04x, %d channels, %d frames (%s)\n", have.freq, have.format,
                have.channels, have.samples, SDL_GetCurrentAudioDriver());
    SDL_PauseAudioDevice(sDev, 0);
}

// ---- the frontend's own stack
enum { FE_STACK = 8 * 1024 * 1024 };
static void *sGameSp;
static void frame_body(void);
#if !HOST_IOS
static void *sFeSp;
static void fe_main(void *arg) {
    (void)arg;
    for (;;) {
        frame_body();
        host_coro_switch(&sFeSp, sGameSp);
    }
}
#else
// iOS: the main thread's own stack is the frontend's (main.c switches to the game from it and calls this when the
// game switches back the first time)
extern void *gHostMainSp;
void frontend_backend_main_loop(void) {
    for (;;) {
        frame_body();
        host_coro_switch(&gHostMainSp, sGameSp);
    }
}

// the app going to the background and coming back (UIKit's notifications, which SDL passes on while it delivers
// them: before the app is suspended)
static int SDLCALL app_events(void *user, SDL_Event *e) {
    (void)user;
    if (e->type == SDL_APP_WILLENTERBACKGROUND || e->type == SDL_APP_DIDENTERBACKGROUND) sInBackground = 1;
    if (e->type == SDL_APP_DIDENTERFOREGROUND) sInBackground = 0;
    return 1;
}
#endif

AcwwShm *frontend_backend_open(const char *argv0) {
    (void)argv0;
    // SDL's SIGINT/SIGTERM handlers would replace the runtime's (debug_posix.c); the face buttons by position
    SDL_SetHint(SDL_HINT_NO_SIGNAL_HANDLERS, "1");
    SDL_SetHint(SDL_HINT_GAMECONTROLLER_USE_BUTTON_LABELS, "0");
    SDL_SetHint(SDL_HINT_APP_NAME, "Animal Crossing: Wild World");
#if HOST_IOS
    // every orientation (the layout follows); the home indicator hidden after a few seconds without a touch near it;
    // the screen kept on (a controller's buttons don't count as touches); no accelerometer as a joystick (it would
    // count as a controller); controllers through Apple's GameController framework only (SDL's own HID drivers would
    // ask for Bluetooth access); fingers and the mouse kept apart (touch_controls.c takes both)
    SDL_SetHint(SDL_HINT_ORIENTATIONS, "LandscapeLeft LandscapeRight Portrait PortraitUpsideDown");
    SDL_SetHint(SDL_HINT_IOS_HIDE_HOME_INDICATOR, "1");
    SDL_SetHint(SDL_HINT_IDLE_TIMER_DISABLED, "1");
    SDL_SetHint(SDL_HINT_ACCELEROMETER_AS_JOYSTICK, "0");
    SDL_SetHint(SDL_HINT_JOYSTICK_HIDAPI, "0");
    SDL_SetHint(SDL_HINT_TOUCH_MOUSE_EVENTS, "0");
    SDL_SetHint(SDL_HINT_MOUSE_TOUCH_EVENTS, "0");
#endif
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) != 0) {
        fprintf(stderr, "frontend: no window (%s): running headless\n", SDL_GetError());
        return 0;
    }
    sShm = calloc(1, sizeof *sShm);
#if HOST_IOS
    void *stack = sShm;  // (no stack of its own: frontend_backend_main_loop)
    SDL_SetEventFilter(app_events, NULL);
    tc_reset(&sTouch);
#else
    void *stack = host_plat_stack_alloc(FE_STACK, 0);
    host_state_untrack(stack);  // (the frontend's own stack: not the game's state)
#endif
    if (!sShm || !stack) { fprintf(stderr, "frontend: no memory\n"); SDL_Quit(); free(sShm); return sShm = 0; }
    sShm->magic = ACWW_SHM_MAGIC;
#if !HOST_IOS
    sFeSp = host_coro_prepare(stack, FE_STACK, fe_main, 0);
#endif
    sWin = SDL_CreateWindow("Animal Crossing: Wild World (native)", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                            256 * sScale, 384 * sScale, SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI
#if HOST_IOS
                            | SDL_WINDOW_FULLSCREEN | SDL_WINDOW_BORDERLESS
#elif defined(__APPLE__)
                            | (render_on_gpu() ? SDL_WINDOW_METAL : 0)
#endif
                            );
    int vsync = getenv("ACWW_VSYNC") && atoi(getenv("ACWW_VSYNC"));
    if (sWin && render_on_gpu()) {  // the GPU draws into the window
#if defined(__APPLE__)
        // (a Metal view over the window's content: its layer is what wgpu draws into, at the drawable's pixels)
        if ((sMetalView = SDL_Metal_CreateView(sWin)))
            sGpu = wdisp_attach(WDISP_METAL, SDL_Metal_GetLayer(sMetalView), 0, vsync);
#endif
        SDL_SysWMinfo wm;
        SDL_VERSION(&wm.version);
        if (!sGpu && SDL_GetWindowWMInfo(sWin, &wm)) {
#if defined(SDL_VIDEO_DRIVER_X11)
            if (wm.subsystem == SDL_SYSWM_X11)
                sGpu = wdisp_attach(WDISP_X11, wm.info.x11.display, wm.info.x11.window, vsync);
#endif
#if defined(SDL_VIDEO_DRIVER_WAYLAND)
            if (wm.subsystem == SDL_SYSWM_WAYLAND)
                sGpu = wdisp_attach(WDISP_WAYLAND, wm.info.wl.display, (uintptr_t)wm.info.wl.surface, vsync);
#endif
        }
        if (!sGpu) fprintf(stderr, "frontend: the GPU can't draw into the window: the screens are read back\n");
    }
    if (sWin && !sGpu) {
        sRen = SDL_CreateRenderer(sWin, -1, vsync ? SDL_RENDERER_PRESENTVSYNC : 0);
        if (!sRen) sRen = SDL_CreateRenderer(sWin, -1, SDL_RENDERER_SOFTWARE);
    }
    if (sRen) {
#if HOST_IOS
        // (no logical size: the layout places the screens and the controls in the window's pixels, ios_show)
        sTex = SDL_CreateTexture(sRen, SDL_PIXELFORMAT_XRGB8888, SDL_TEXTUREACCESS_STREAMING, 256, 384);
        if (sTex) SDL_SetTextureScaleMode(sTex, SDL_ScaleModeNearest);
#else
        SDL_RenderSetLogicalSize(sRen, 256, 384);  // letterboxed, the mouse in logical coordinates
        sTex = SDL_CreateTexture(sRen, SDL_PIXELFORMAT_XRGB8888, SDL_TEXTUREACCESS_STREAMING, 256, 384);
#endif
    }
    if (!sTex && !sGpu) {
        fprintf(stderr, "frontend: no window (%s): running headless\n", SDL_GetError());
        SDL_Quit();
        free(sShm);
        return sShm = 0;
    }
    ui_init();
    open_audio();
    atexit(sdl_quit);
    return sShm;
}

void frontend_backend_frame(AcwwShm *shm) {
    (void)shm;
#if HOST_IOS
    host_coro_switch(&sGameSp, gHostMainSp);
#else
    host_coro_switch(&sGameSp, sFeSp);
#endif
}

// (also called from the crash and SIGINT handlers: nothing here that isn't safe there; SDL_Quit runs at exit)
void frontend_backend_close(void) {}

// ---- input
static uint32_t keyboard_keys(void) {
    static Uint8 st[SDL_NUM_SCANCODES];
    int n;
    const Uint8 *now = SDL_GetKeyboardState(&n);
    memcpy(st, now, (size_t)(n < SDL_NUM_SCANCODES ? n : SDL_NUM_SCANCODES));
    // (Alt+Enter is the full-screen toggle, not a DS button)
    if (SDL_GetModState() & KMOD_ALT) st[SDL_SCANCODE_RETURN] = 0;
    return ui_keyboard_keys(st);
}

static uint32_t pad_keys(void) {
    enum { DEAD = 16000 };
    uint32_t k = 0;
    for (int p = 0; p < 4; p++) {
        SDL_GameController *c = sPads[p];
        if (!c) continue;
        k |= ui_pad_keys(c);
        int x = SDL_GameControllerGetAxis(c, SDL_CONTROLLER_AXIS_LEFTX), y = SDL_GameControllerGetAxis(c, SDL_CONTROLLER_AXIS_LEFTY);
        if (x > DEAD) k |= 1u << 4;
        if (x < -DEAD) k |= 1u << 5;
        if (y < -DEAD) k |= 1u << 6;
        if (y > DEAD) k |= 1u << 7;
        if (SDL_GameControllerGetAxis(c, SDL_CONTROLLER_AXIS_TRIGGERRIGHT) > DEAD) k |= 1u << 8;
        if (SDL_GameControllerGetAxis(c, SDL_CONTROLLER_AXIS_TRIGGERLEFT) > DEAD) k |= 1u << 9;
    }
    return k;
}

static void pad_added(int index) {
    for (int p = 0; p < 4; p++) {
        if (sPads[p]) continue;
        sPads[p] = SDL_GameControllerOpen(index);
        if (sPads[p])
            fprintf(stderr, "frontend: controller %s\n", SDL_GameControllerName(sPads[p]));
#if HOST_IOS
        if (sPads[p]) sShowControls = 0;  // (a touch beside the screens shows them again)
#endif
        return;
    }
}

static void pad_removed(SDL_JoystickID id) {
    for (int p = 0; p < 4; p++) {
        if (sPads[p] && SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(sPads[p])) == id) {
            SDL_GameControllerClose(sPads[p]);
            sPads[p] = 0;
        }
    }
#if HOST_IOS
    int any = 0;
    for (int p = 0; p < 4; p++) any |= sPads[p] != 0;
    if (!any) sShowControls = 1;
#endif
}

// the touch panel: the left mouse button or the first finger, in the window's logical coordinates (0..255,
// 0..383); the bottom screen is y 192..383
static struct { int held, x, y; } sMouse, sFinger;
static SDL_FingerID sFingerId;
// a point of the window (its coordinates) in the screens' (256x384); the SDL renderer's mouse events have them already
static void window_to_logical(int wx, int wy, int *x, int *y) {
    if (!sGpu) { *x = wx, *y = wy; return; }
    int w, h;
    float r[4];
    SDL_GetWindowSize(sWin, &w, &h);
    wdisp_letterbox(w, h, r);
    *x = (int)SDL_floorf(((float)wx - r[0]) * 256.0f / r[2]);
    *y = (int)SDL_floorf(((float)wy - r[1]) * 384.0f / r[3]);
}
static void finger_to_logical(float fx, float fy, int *x, int *y) {
    int w, h;
    SDL_GetWindowSize(sWin, &w, &h);
    if (sGpu) { window_to_logical((int)(fx * w), (int)(fy * h), x, y); return; }
    float lx, ly;
    SDL_RenderWindowToLogical(sRen, (int)(fx * w), (int)(fy * h), &lx, &ly);
    *x = (int)lx;
    *y = (int)ly;
}
static void update_touch(void) {
#if HOST_IOS
    int tx, ty, was = sShm->touch_down != 0;
    if (tc_stylus(&sTouch, &tx, &ty)) {
        sShm->touch_x = tx;
        sShm->touch_y = ty;
        sShm->touch_down = 1;
    } else {
        sShm->touch_down = 0;
    }
    // (the log, acww-log.txt: the stylus going down and up, the first 200 times; ACWW_TOUCH_LOG=0: never)
    static int logged, logOn = -1;
    if (logOn < 0) logOn = !getenv("ACWW_TOUCH_LOG") || atoi(getenv("ACWW_TOUCH_LOG")) != 0;
    if (logOn && was != (sShm->touch_down != 0) && logged < 200) {
        logged++;
        if (sShm->touch_down) fprintf(stderr, "touch: down at %d,%d (frame %llu)\n", tx, ty, (unsigned long long)host_frame());
        else fprintf(stderr, "touch: up (frame %llu)\n", (unsigned long long)host_frame());
    }
    return;
#endif
    int held = sFinger.held || sMouse.held;
    int x = sFinger.held ? sFinger.x : sMouse.x, y = sFinger.held ? sFinger.y : sMouse.y;
    if (held && y >= 192 && y < 384 && x >= 0 && x < 256) {
        sShm->touch_x = x;
        sShm->touch_y = y - 192;
        sShm->touch_down = 1;
    } else if (!held || y < 192) {
        sShm->touch_down = 0;  // (dragged off the side: held at the last point, as the viewer does)
    }
}

static void handle(const SDL_Event *e) {
    // (the binding screen takes the rest while it is open)
    if (e->type == SDL_QUIT) { sShm->quit = 1; return; }
    if (e->type == SDL_CONTROLLERDEVICEADDED) { pad_added(e->cdevice.which); return; }
    if (e->type == SDL_CONTROLLERDEVICEREMOVED) { pad_removed(e->cdevice.which); return; }
    if (ui_event(e)) {
        sMouse.held = sFinger.held = 0;
#if HOST_IOS
        tc_reset(&sTouch);
#endif
        return;
    }
#if HOST_IOS
    // fingers (normalised to the window) and the mouse (an iPad's pointer: in points) to the touch controls, in the
    // window's pixels; a touch beside the screens while the buttons are hidden shows them
    switch (e->type) {
    case SDL_FINGERDOWN:
    case SDL_FINGERMOTION:
    case SDL_FINGERUP: {
        int ev = e->type == SDL_FINGERDOWN ? TC_DOWN : e->type == SDL_FINGERUP ? TC_UP : TC_MOVE;
        if (tc_pointer(&sTouch, &sLayout, (int64_t)e->tfinger.fingerId, ev, e->tfinger.x * (float)sLayout.w,
                       e->tfinger.y * (float)sLayout.h))
            sShowControls = 1;
        return;
    }
    case SDL_MOUSEBUTTONDOWN:
    case SDL_MOUSEBUTTONUP:
        if (e->button.which == SDL_TOUCH_MOUSEID || e->button.button != SDL_BUTTON_LEFT) return;
        if (tc_pointer(&sTouch, &sLayout, -1000, e->type == SDL_MOUSEBUTTONDOWN ? TC_DOWN : TC_UP,
                       (float)e->button.x * sPt, (float)e->button.y * sPt))
            sShowControls = 1;
        return;
    case SDL_MOUSEMOTION:
        if (e->motion.which == SDL_TOUCH_MOUSEID) return;
        tc_pointer(&sTouch, &sLayout, -1000, TC_MOVE, (float)e->motion.x * sPt, (float)e->motion.y * sPt);
        return;
    default: break;
    }
#endif
    switch (e->type) {
    case SDL_KEYDOWN:
        if (e->key.repeat) break;
        if (e->key.keysym.scancode == SDL_SCANCODE_F11) {
            sScale = sScale % 3 + 1;
            SDL_SetWindowFullscreen(sWin, 0);
            SDL_SetWindowSize(sWin, 256 * sScale, 384 * sScale);
        } else if (e->key.keysym.scancode == SDL_SCANCODE_RETURN && (e->key.keysym.mod & KMOD_ALT)) {
            int full = (SDL_GetWindowFlags(sWin) & SDL_WINDOW_FULLSCREEN_DESKTOP) != 0;
            SDL_SetWindowFullscreen(sWin, full ? 0 : SDL_WINDOW_FULLSCREEN_DESKTOP);
        } else if (e->key.keysym.scancode == SDL_SCANCODE_M && !ui_key_bound(SDL_SCANCODE_M)) {
            __atomic_store_n(&sMute, !sMute, __ATOMIC_RELAXED);
        }
        break;
    case SDL_MOUSEBUTTONDOWN:
    case SDL_MOUSEBUTTONUP:
        if (e->button.which == SDL_TOUCH_MOUSEID || e->button.button != SDL_BUTTON_LEFT) break;  // (fingers below)
        sMouse.held = e->type == SDL_MOUSEBUTTONDOWN;
        window_to_logical(e->button.x, e->button.y, &sMouse.x, &sMouse.y);
        break;
    case SDL_MOUSEMOTION:
        if (e->motion.which == SDL_TOUCH_MOUSEID) break;
        sMouse.held = (e->motion.state & SDL_BUTTON_LMASK) != 0;
        window_to_logical(e->motion.x, e->motion.y, &sMouse.x, &sMouse.y);
        break;
    case SDL_FINGERDOWN:
        if (sFinger.held) break;
        sFingerId = e->tfinger.fingerId;
        sFinger.held = 1;
        finger_to_logical(e->tfinger.x, e->tfinger.y, &sFinger.x, &sFinger.y);
        break;
    case SDL_FINGERMOTION:
        if (sFinger.held && e->tfinger.fingerId == sFingerId)
            finger_to_logical(e->tfinger.x, e->tfinger.y, &sFinger.x, &sFinger.y);
        break;
    case SDL_FINGERUP:
        if (sFinger.held && e->tfinger.fingerId == sFingerId) sFinger.held = 0;
        break;
    default: break;
    }
}

// ---- ACWW_WINSHOT="frame:path.ppm;...": the window's drawable as presented (before the present: the back buffer
// is undefined after it)
static void write_ppm(const char *path, const uint32_t *px, int w, int h) {
    FILE *o = fopen(path, "wb");
    if (!o) return;
    fprintf(o, "P6\n%d %d\n255\n", w, h);
    for (int i = 0; i < w * h; i++) {
        unsigned char rgb[3] = {(unsigned char)(px[i] >> 16), (unsigned char)(px[i] >> 8), (unsigned char)px[i]};
        fwrite(rgb, 1, 3, o);
    }
    fclose(o);
}
// what the window shows into a file (before the present)
static void shot(const char *path) {
    int w, h;
    if (sGpu) {
        SDL_GetWindowSizeInPixels(sWin, &w, &h);
        uint32_t *px = malloc((size_t)w * h * 4);
        if (px && wdisp_read_window(w, h, px)) write_ppm(path, px, w, h);
        free(px);
        return;
    }
    SDL_GetRendererOutputSize(sRen, &w, &h);
    uint32_t *px = malloc((size_t)w * h * 4);
    // (the whole output: with the logical size the viewport is only the letterboxed picture)
    SDL_RenderSetLogicalSize(sRen, 0, 0);
    SDL_RenderSetViewport(sRen, 0);
    if (px && SDL_RenderReadPixels(sRen, 0, SDL_PIXELFORMAT_XRGB8888, px, w * 4) == 0) write_ppm(path, px, w, h);
    else fprintf(stderr, "frontend: ACWW_WINSHOT: %s\n", SDL_GetError());
#if !HOST_IOS
    SDL_RenderSetLogicalSize(sRen, 256, 384);
#endif
    free(px);
}
static void window_shot(void) {
    static const char *spec;
    if (!spec) spec = getenv("ACWW_WINSHOT") ? getenv("ACWW_WINSHOT") : "";
    unsigned f = (unsigned)host_frame();
    for (const char *p = spec; *p;) {
        unsigned fr;
        char path[512];
        if (sscanf(p, "%u:%511[^;]", &fr, path) == 2 && fr == f) shot(path);
        p = strchr(p, ';');
        if (!p) break;
        p++;
    }
}

// ---- ACWW_UI_SCRIPT="frame:item,item,...;...": scripted input for testing the binding screen - from that frame on,
// one item at a time (a frame each, or a step of the paused binding screen): a key (SDL's scancode name, e.g. F1,
// Down, Return: pressed and released), pad:<button> (SDL's name, e.g. pad:a: a controller button), shot:<file> (what
// the window shows, as ACWW_WINSHOT), wait:<ms> (real time), quit (the window closed)
static char sPendingShot[512];  // (taken by show, before the present)
static void script_step(void) {
    static const char *spec, *next;
    if (!spec) spec = getenv("ACWW_UI_SCRIPT") ? getenv("ACWW_UI_SCRIPT") : "";
    if (!next) {
        unsigned f = (unsigned)host_frame();
        for (const char *p = spec; *p;) {
            unsigned fr;
            int n = 0;
            if (sscanf(p, "%u:%n", &fr, &n) == 1 && n && fr == f) next = p + n;
            p = strchr(p, ';');
            if (!p) break;
            p++;
        }
        if (!next) return;
    }
    char item[512];
    size_t len = strcspn(next, ",;");
    snprintf(item, sizeof item, "%.*s", (int)(len < sizeof item ? len : sizeof item - 1), next);
    next = next[len] == ',' ? next + len + 1 : NULL;
    SDL_Event e;
    memset(&e, 0, sizeof e);
    if (!strncmp(item, "wait:", 5)) {
        SDL_Delay((Uint32)atoi(item + 5));
    } else if (!strcmp(item, "quit")) {
        e.type = SDL_QUIT;
        handle(&e);
    } else if (!strncmp(item, "shot:", 5)) {
        snprintf(sPendingShot, sizeof sPendingShot, "%s", item + 5);
    } else if (!strncmp(item, "pad:", 4)) {
        e.type = SDL_CONTROLLERBUTTONDOWN;
        e.cbutton.which = -1;
        e.cbutton.button = (Uint8)SDL_GameControllerGetButtonFromString(item + 4);
        handle(&e);
        e.type = SDL_CONTROLLERBUTTONUP;
        handle(&e);
    } else {
        e.type = SDL_KEYDOWN;
        e.key.keysym.scancode = SDL_GetScancodeFromName(item);
        handle(&e);
        e.type = SDL_KEYUP;
        handle(&e);
    }
}

#if HOST_IOS
// the layout for the window as it is now (turned, the safe area, the buttons shown or not); a change of it ends what
// the fingers held (none keeps a meaning it had in the old layout)
static void ios_layout(void) {
    int ow = 0, oh = 0, ww = 0, wh = 0;
    if (SDL_GetRendererOutputSize(sRen, &ow, &oh) != 0 || ow <= 0 || oh <= 0) return;
    SDL_GetWindowSize(sWin, &ww, &wh);
    sPt = ww > 0 ? (float)ow / (float)ww : 1;
    float in[4];
    ios_safe_area(sWin, in);
    for (int i = 0; i < 4; i++) in[i] *= sPt;
    int changed = ow != sLayout.w || oh != sLayout.h || sShowControls != sLayout.controls;
    if (changed) tc_reset(&sTouch);
    tc_layout(&sLayout, ow, oh, sPt, in, sShowControls);
    if (changed)
        fprintf(stderr, "frontend: window %dx%d px (%.0fx scale), safe area %.0f,%.0f,%.0f,%.0f px, screens %.0f,%.0f %.0fx%.0f, "
                "buttons %s\n", ow, oh, sPt, in[0], in[1], in[2], in[3], sLayout.screens.x, sLayout.screens.y,
                sLayout.screens.w, sLayout.screens.h, sLayout.controls ? "on" : "off");
}

// every 10 s of play, into the log (acww-log.txt; ACWW_STATS=0: not): the frame rate (the DS's is 59.83), the sound
// queue's dry runs and skips (the sound card's buffer emptied: a crackle; too far behind: a jump) and its mean fill
static void ios_stats(void) {
    static int on = -1;
    static uint64_t t0;
    static unsigned frames, dry0, skips0, calls0;
    static double level0;
    if (on < 0) on = !getenv("ACWW_STATS") || atoi(getenv("ACWW_STATS")) != 0;
    if (!on) return;
    uint64_t now = host_plat_time_ns();
    if (!t0) { t0 = now; dry0 = sDry, skips0 = sSkips, calls0 = sCalls, level0 = sLevelSum; return; }
    if (++frames < 600) return;
    double secs = (double)(now - t0) / 1e9;
    unsigned calls = sCalls - calls0;
    int pads = 0;
    for (int p = 0; p < 4; p++) pads += sPads[p] != 0;
    fprintf(stderr, "stats: frame %llu: %.1f frames/s, sound %u dry, %u skips, queue %.0f frames; %s, %d controller(s)\n",
            (unsigned long long)host_frame(), frames / secs, sDry - dry0, sSkips - skips0,
            calls ? (sLevelSum - level0) / calls : 0.0, sLayout.controls ? "buttons" : "no buttons", pads);
    t0 = now, frames = 0, dry0 = sDry, skips0 = sSkips, calls0 = sCalls, level0 = sLevelSum;
}

// the controls, each from a texture made once per size and look (touch_controls.c draws them)
static void ios_draw_controls(void) {
    uint32_t held = tc_held(&sTouch), dirs = tc_dpad_dirs(&sTouch);
    for (int b = 0; b < TC_COUNT; b++) {
        TcRect r = sLayout.button[b];
        int w = (int)(r.w + 0.5f), h = (int)(r.h + 0.5f);
        if (w <= 0 || h <= 0) continue;
        if (w != sButtonW[b] || h != sButtonH[b]) {
            for (int v = 0; v < 16; v++) {
                if (sButtonTex[b][v]) SDL_DestroyTexture(sButtonTex[b][v]);
                sButtonTex[b][v] = 0;
            }
            sButtonW[b] = w, sButtonH[b] = h;
        }
        int v = b == TC_DPAD ? (int)(dirs >> 4 & 15) : (int)(held >> b & 1);
        SDL_Texture **t = &sButtonTex[b][v];
        if (!*t) {
            uint32_t *px = malloc((size_t)w * (size_t)h * 4);
            if (!px) continue;
            tc_draw(px, w, h, b, b == TC_DPAD ? 0 : v, b == TC_DPAD ? (uint32_t)v << 4 : 0);
            *t = SDL_CreateTexture(sRen, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, w, h);
            if (*t) {
                SDL_UpdateTexture(*t, 0, px, w * 4);
                SDL_SetTextureBlendMode(*t, SDL_BLENDMODE_BLEND);
                SDL_SetTextureScaleMode(*t, SDL_ScaleModeNearest);
            }
            free(px);
            if (!*t) continue;
        }
        SDL_Rect dst = {(int)(r.x + 0.5f), (int)(r.y + 0.5f), w, h};
        // (LOAD dimmed while there's no state to load)
        SDL_SetTextureAlphaMod(*t, b == TC_LOAD && !host_state_have() ? 96 : 255);
        SDL_RenderCopy(sRen, *t, 0, &dst);
    }
}

// the SAVE and LOAD buttons: a save state of the game as it is now, or back to the one saved (savestate.c; this
// launch only). Here the game is held at the end of its frame (sGameSp).
static void ios_states(void) {
    static uint32_t was;
    uint32_t held = tc_held(&sTouch), pressed = held & ~was;
    was = held;
    if (pressed & 1u << TC_SAVE && !host_state_save(&sGameSp)) fprintf(stderr, "frontend: state not saved\n");
    if (pressed & 1u << TC_LOAD && host_state_have() && !host_state_load(&sGameSp))
        fprintf(stderr, "frontend: state not loaded\n");
}

// the frame: the screens in their place - drawn at a whole multiple of the DS's pixels (sharp), then smoothed to the
// size they have (no uneven pixels) - the binding screen over them while it is open, the controls around them
static void ios_show(const uint32_t *overlay, int changed) {
    SDL_UpdateTexture(sTex, 0, sShm->pixels, 256 * 4);
    if (overlay) {
        if (!sOverlayTex) {
            sOverlayTex = SDL_CreateTexture(sRen, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, 256, 384);
            if (sOverlayTex) SDL_SetTextureBlendMode(sOverlayTex, SDL_BLENDMODE_BLEND);
            changed = 1;
        }
        if (sOverlayTex && changed) SDL_UpdateTexture(sOverlayTex, 0, overlay, 256 * 4);
    }
    SDL_SetRenderTarget(sRen, 0);
    SDL_SetRenderDrawColor(sRen, 0, 0, 0, 255);
    SDL_RenderClear(sRen);
    TcRect sc = sLayout.screens;
    SDL_FRect dst = {sc.x, sc.y, sc.w, sc.h};
    int k = (int)SDL_ceilf(sc.w / 256.0f - 0.01f);
    k = k < 1 ? 1 : k > 8 ? 8 : k;
    if (k != sSharpK) {
        if (sSharp) SDL_DestroyTexture(sSharp);
        sSharp = SDL_CreateTexture(sRen, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, 256 * k, 384 * k);
        if (sSharp) SDL_SetTextureScaleMode(sSharp, SDL_ScaleModeLinear);
        sSharpK = k;
    }
    if (sSharp && SDL_SetRenderTarget(sRen, sSharp) == 0) {
        SDL_RenderCopy(sRen, sTex, 0, 0);
        if (overlay && sOverlayTex) SDL_RenderCopy(sRen, sOverlayTex, 0, 0);
        SDL_SetRenderTarget(sRen, 0);
        SDL_RenderCopyF(sRen, sSharp, 0, &dst);
    } else {  // (no render targets: the DS's pixels straight to the window)
        SDL_SetRenderTarget(sRen, 0);
        SDL_RenderCopyF(sRen, sTex, 0, &dst);
        if (overlay && sOverlayTex) SDL_RenderCopyF(sRen, sOverlayTex, 0, &dst);
    }
    if (sLayout.controls) ios_draw_controls();
}

// the app in the background: the game holds (no drawing - iOS doesn't allow it there - and no sound) until it is back
static void ios_background(void) {
    host_set_paused(1);
    if (sDev) SDL_PauseAudioDevice(sDev, 1);
    tc_reset(&sTouch);
    SDL_Event e;
    while (sInBackground && !sShm->quit) {
        if (SDL_WaitEventTimeout(&e, 250)) handle(&e);
    }
    tc_reset(&sTouch);
    if (sDev) SDL_PauseAudioDevice(sDev, 0);
    host_set_paused(0);
}
#endif

// the frame and, while it is open, the binding screen over it
static void show(void) {
    int changed;
    const uint32_t *overlay = ui_overlay(&changed);
#if HOST_IOS
    ios_show(overlay, changed);
    window_shot();
    if (sPendingShot[0]) shot(sPendingShot), sPendingShot[0] = 0;
    SDL_RenderPresent(sRen);
    return;
#endif
    if (sGpu) {
        int w, h;
        SDL_GetWindowSizeInPixels(sWin, &w, &h);
        wdisp_overlay(overlay, changed);
        window_shot();
        if (sPendingShot[0]) shot(sPendingShot), sPendingShot[0] = 0;
        wdisp_present(w, h);
        return;
    }
    SDL_UpdateTexture(sTex, 0, sShm->pixels, 256 * 4);
    SDL_SetRenderDrawColor(sRen, 0, 0, 0, 255);
    SDL_RenderClear(sRen);
    SDL_RenderCopy(sRen, sTex, 0, 0);
    if (overlay) {
        if (!sOverlayTex) {
            sOverlayTex = SDL_CreateTexture(sRen, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, 256, 384);
            SDL_SetTextureBlendMode(sOverlayTex, SDL_BLENDMODE_BLEND);
            changed = 1;
        }
        if (changed) SDL_UpdateTexture(sOverlayTex, 0, overlay, 256 * 4);
        SDL_RenderCopy(sRen, sOverlayTex, 0, 0);
    }
    window_shot();
    if (sPendingShot[0]) shot(sPendingShot), sPendingShot[0] = 0;
    SDL_RenderPresent(sRen);
}

// one frame (on the frontend's stack): events, input, then the picture; while the binding screen is open the game
// waits here
static void frame_body(void) {
    SDL_Event e;
#if HOST_IOS
    if (sInBackground) ios_background();
    ios_layout();
#endif
    while (SDL_PollEvent(&e)) handle(&e);
    script_step();
    if (ui_is_open()) host_set_paused(1);
    while (ui_is_open() && !sShm->quit) {
#if HOST_IOS
        if (sInBackground) ios_background();
        ios_layout();
#endif
        show();
        if (SDL_WaitEventTimeout(&e, 50)) {
            handle(&e);
            while (SDL_PollEvent(&e)) handle(&e);
        }
        script_step();
    }
    host_set_paused(0);
#if HOST_IOS
    if (sInBackground) return;  // (went to the background just now: nothing drawn; the next frame waits)
    ios_layout();               // (the buttons shown or hidden by this frame's events)
    ios_stats();
    ios_states();
    sShm->keys = keyboard_keys() | pad_keys() | tc_keys(&sTouch);
#else
    sShm->keys = keyboard_keys() | pad_keys();
#endif
    update_touch();
    show();
}

#endif
