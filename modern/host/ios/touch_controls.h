// Host build, iOS: the on-screen controls (touch_controls.c). Plain C, no SDL: the layout of the screens and the
// buttons in a window of w x h pixels, the fingers on them, and the buttons' pictures. frontend_sdl.c (HOST_IOS)
// draws and feeds them; modern/host/ios/tests/ checks them on any machine.
//
// The layout: the DS's two screens (256x384 together) as large as they fit with the buttons beside them - below them
// when the window is upright (L, Save, Start, Select, Load, R in a row, then the D-pad on the left and A/B/X/Y on the
// right), left and right of them when it lies on its side (L, the D-pad, Save and Start on the left; R, A/B/X/Y,
// Select and Load on the right). Without the buttons (a game controller is connected) the screens fill the safe area.
// Touch: a finger that lands on the bottom screen is the stylus (one at a time); one that lands on a button holds it
// and may slide to another (the D-pad: the direction from its centre, diagonals too); the top screen does nothing.
#ifndef HOST_IOS_TOUCH_CONTROLS_H
#define HOST_IOS_TOUCH_CONTROLS_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// the controls; each is drawn from its own picture (tc_draw)
// (SAVE / LOAD: the frontend's save state, not DS buttons: no key bits)
enum { TC_A, TC_B, TC_X, TC_Y, TC_L, TC_R, TC_START, TC_SELECT, TC_SAVE, TC_LOAD, TC_DPAD, TC_COUNT };

typedef struct { float x, y, w, h; } TcRect;

typedef struct {
    int w, h;                // the window's drawable, in pixels
    TcRect screens;          // where the screens' picture (256x384) goes
    int controls;            // the buttons are shown
    TcRect button[TC_COUNT]; // each control's square or pill (pixels)
} TcLayout;

// the layout for a w x h pixel window; pt: pixels per point (the screen's scale, 2 or 3); safe: the insets of the
// safe area in pixels (left, top, right, bottom); controls: show the buttons
void tc_layout(TcLayout *l, int w, int h, float pt, const float safe[4], int controls);

// pointers (fingers, or the mouse): what they hold
enum { TC_MAX_POINTERS = 12 };
typedef struct {
    struct {
        int64_t id;
        int used;
        int what;   // -1: the stylus, -2: nothing (the top screen, or outside), else a TC_ button
        uint32_t keys;
    } p[TC_MAX_POINTERS];
    int stylus_x, stylus_y;  // the stylus's point on the bottom screen (0..255, 0..191)
} TcState;

void tc_reset(TcState *s);
// a pointer went down, moved or went up at (x, y) pixels; returns 1 if it went down outside the screens and the
// buttons while they are hidden (the frontend shows them again then)
enum { TC_DOWN, TC_MOVE, TC_UP };
int tc_pointer(TcState *s, const TcLayout *l, int64_t id, int event, float x, float y);
// the DS buttons held (AcwwShm.keys bits: 0 A, 1 B, 2 Select, 3 Start, 4 Right, 5 Left, 6 Up, 7 Down, 8 R, 9 L,
// 10 X, 11 Y)
uint32_t tc_keys(const TcState *s);
// the stylus: 1 and its point (0..255, 0..191) while a finger is on the bottom screen
int tc_stylus(const TcState *s, int *x, int *y);
// the controls held now (bit per TC_ control: the D-pad's bit while any direction is held), for drawing them pressed
uint32_t tc_held(const TcState *s);
// the D-pad's directions held (the keys' bits 4-7)
uint32_t tc_dpad_dirs(const TcState *s);

// a control's picture, w x h pixels (its layout rect's size), 0xAARRGGBB with straight alpha; dirs: for the D-pad,
// the directions to draw pressed (the keys' bits 4-7); pressed: the other controls
void tc_draw(uint32_t *px, int w, int h, int which, int pressed, uint32_t dirs);

#ifdef __cplusplus
}
#endif

#endif
