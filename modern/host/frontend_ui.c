// Host build: the settings screen (frontend_ui.h): the optional features (host_features.h), a line each, switched on
// and off by a tap (or Enter / a controller's A), then CLOSE. Drawn into an overlay of the screens' size (256x384) with
// a 5x7 font of its own, which frontend_sdl.c shows over the frame; the game waits while it is open.
//   iOS: the MENU button      F2 or a controller's Back + Start: open / close      Escape (or B): close
// The DS buttons' keys and controller buttons are fixed (kActions).
#if HOST_FRONTEND_SDL

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "host_features.h"
#include "frontend_ui.h"

enum { W = 256, H = 384, NACT = 12, SLINES = FEAT_COUNT + 1 };
static const struct { const char *name; uint32_t bit; SDL_Scancode key; SDL_GameControllerButton pad; } kActions[NACT] = {
    {"A", 1u << 0, SDL_SCANCODE_X, SDL_CONTROLLER_BUTTON_B},  // (the face buttons by position: right = A)
    {"B", 1u << 1, SDL_SCANCODE_Z, SDL_CONTROLLER_BUTTON_A},
    {"X", 1u << 10, SDL_SCANCODE_S, SDL_CONTROLLER_BUTTON_Y},
    {"Y", 1u << 11, SDL_SCANCODE_A, SDL_CONTROLLER_BUTTON_X},
    {"L", 1u << 9, SDL_SCANCODE_Q, SDL_CONTROLLER_BUTTON_LEFTSHOULDER},
    {"R", 1u << 8, SDL_SCANCODE_W, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER},
    {"START", 1u << 3, SDL_SCANCODE_RETURN, SDL_CONTROLLER_BUTTON_START},
    {"SELECT", 1u << 2, SDL_SCANCODE_RSHIFT, SDL_CONTROLLER_BUTTON_BACK},
    {"UP", 1u << 6, SDL_SCANCODE_UP, SDL_CONTROLLER_BUTTON_DPAD_UP},
    {"DOWN", 1u << 7, SDL_SCANCODE_DOWN, SDL_CONTROLLER_BUTTON_DPAD_DOWN},
    {"LEFT", 1u << 5, SDL_SCANCODE_LEFT, SDL_CONTROLLER_BUTTON_DPAD_LEFT},
    {"RIGHT", 1u << 4, SDL_SCANCODE_RIGHT, SDL_CONTROLLER_BUTTON_DPAD_RIGHT},
};
static int sOpen, sLine, sDirty = 1;

void ui_init(void) {}

uint32_t ui_keyboard_keys(const Uint8 *keyboard) {
    uint32_t k = 0;
    for (int i = 0; i < NACT; i++)
        if (keyboard[kActions[i].key]) k |= kActions[i].bit;
    return k;
}
uint32_t ui_pad_keys(SDL_GameController *pad) {
    uint32_t k = 0;
    for (int i = 0; i < NACT; i++)
        if (SDL_GameControllerGetButton(pad, kActions[i].pad)) k |= kActions[i].bit;
    return k;
}
int ui_key_bound(SDL_Scancode sc) {
    for (int i = 0; i < NACT; i++)
        if (kActions[i].key == sc) return 1;
    return 0;
}
int ui_is_open(void) { return sOpen; }

// ---- the screen's controls
static void set_open(int open) {
    sOpen = open;
    if (open) sLine = 0;
    sDirty = 1;
}
void ui_open_settings(void) { set_open(1); }
static void choose_line(void) {  // a feature on / off, or close
    if (sLine < FEAT_COUNT) feature_set(sLine, !feature_on(sLine));
    else set_open(0);
    sDirty = 1;
}
// the lines' places (the overlay's pixels; 28 high: touch sized)
static int line_y(int i) { return 44 + i * 32 + (i >= FEAT_COUNT ? 12 : 0); }
int ui_tap(int x, int y) {
    (void)x;
    if (!sOpen) return 0;
    for (int i = 0; i < SLINES; i++) {
        int ly = line_y(i) - 7;
        if (y < ly || y >= ly + 28) continue;
        sLine = i;
        choose_line();
        return 1;
    }
    return 1;
}
int ui_event(const SDL_Event *e) {
    if (e->type == SDL_KEYDOWN && !e->key.repeat && e->key.keysym.scancode == SDL_SCANCODE_F2) {
        set_open(!sOpen);
        return 1;
    }
    if (e->type == SDL_CONTROLLERBUTTONDOWN) {  // Back + Start: open / close
        SDL_GameController *c = SDL_GameControllerFromInstanceID(e->cbutton.which);
        int b = e->cbutton.button;
        if (c && ((b == SDL_CONTROLLER_BUTTON_BACK && SDL_GameControllerGetButton(c, SDL_CONTROLLER_BUTTON_START)) ||
                  (b == SDL_CONTROLLER_BUTTON_START && SDL_GameControllerGetButton(c, SDL_CONTROLLER_BUTTON_BACK)))) {
            set_open(!sOpen);
            return 1;
        }
    }
    if (!sOpen) return 0;
    sDirty = 1;
    if (e->type == SDL_KEYDOWN && !e->key.repeat) {
        SDL_Scancode sc = e->key.keysym.scancode;
        if (sc == SDL_SCANCODE_UP) sLine = (sLine + SLINES - 1) % SLINES;
        else if (sc == SDL_SCANCODE_DOWN) sLine = (sLine + 1) % SLINES;
        else if (sc == SDL_SCANCODE_RETURN || sc == SDL_SCANCODE_KP_ENTER) choose_line();
        else if (sc == SDL_SCANCODE_ESCAPE) set_open(0);
    } else if (e->type == SDL_CONTROLLERBUTTONDOWN) {
        SDL_GameControllerButton b = (SDL_GameControllerButton)e->cbutton.button;
        if (b == SDL_CONTROLLER_BUTTON_DPAD_UP) sLine = (sLine + SLINES - 1) % SLINES;
        else if (b == SDL_CONTROLLER_BUTTON_DPAD_DOWN) sLine = (sLine + 1) % SLINES;
        else if (b == SDL_CONTROLLER_BUTTON_A) choose_line();
        else if (b == SDL_CONTROLLER_BUTTON_B) set_open(0);
    }
    return 1;  // (everything else while open: the game is paused)
}

// ---- drawing: a 5x7 font (rows of 5 bits, the top row first; capitals only)
static const struct { char c; const char *rows; } kFont[] = {
    {'A', ".###.#...##...#######...##...##...#"}, {'B', "####.#...##...#####.#...##...#####."},
    {'C', ".###.#...##....#....#....#...#.###."}, {'D', "####.#...##...##...##...##...#####."},
    {'E', "######....#....####.#....#....#####"}, {'F', "######....#....####.#....#....#...."},
    {'G', ".###.#...##....#.####...##...#.####"}, {'H', "#...##...##...#######...##...##...#"},
    {'I', ".###...#....#....#....#....#...###."}, {'J', "..###...#....#....#....#.#..#..##.."},
    {'K', "#...##..#.#.#..##...#.#..#..#.#...#"}, {'L', "#....#....#....#....#....#....#####"},
    {'M', "#...###.###.#.##.#.##...##...##...#"}, {'N', "#...##...###..##.#.##..###...##...#"},
    {'O', ".###.#...##...##...##...##...#.###."}, {'P', "####.#...##...#####.#....#....#...."},
    {'Q', ".###.#...##...##...##.#.##..#..##.#"}, {'R', "####.#...##...#####.#.#..#..#.#...#"},
    {'S', ".#####....#.....###.....#....#####."}, {'T', "#####..#....#....#....#....#....#.."},
    {'U', "#...##...##...##...##...##...#.###."}, {'V', "#...##...##...##...##...#.#.#...#.."},
    {'W', "#...##...##...##.#.##.#.##.#.#.#.#."}, {'X', "#...##...#.#.#...#...#.#.#...##...#"},
    {'Y', "#...##...#.#.#...#....#....#....#.."}, {'Z', "#####....#...#...#...#...#....#####"},
    {'0', ".###.#...##..###.#.###..##...#.###."}, {'1', "..#...##....#....#....#....#...###."},
    {'2', ".###.#...#....#...#...#...#...#####"}, {'3', "####.....#....#.###.....#....#####."},
    {'4', "...#...##..#.#.#..#.#####...#....#."}, {'5', "######....####.....#....##...#.###."},
    {'6', ".###.#....#....####.#...##...#.###."}, {'7', "#####....#...#...#...#....#....#..."},
    {'8', ".###.#...##...#.###.#...##...#.###."}, {'9', ".###.#...##...#.####....#....#.###."},
    {':', "......#....#.........#....#........"}, {'-', "...............#####..............."},
    {'(', "...#...#...#....#....#.....#.....#."}, {')', ".#.....#.....#....#....#...#...#..."},
    {'/', "....#....#...#...#...#...#....#...."}, {'>', ".#.....#.....#.....#...#...#...#..."},
    {'<', "...#...#...#...#.....#.....#.....#."}, {'.', ".........................##...##..."},
    {',', "....................##....#...#...."}, {'\'', "..#....#...#......................."},
    {'=', "..........#####.....#####.........."}, {'+', "......#....#..#####..#....#........"},
    {'?', ".###.#...#....#...#...#.........#.."}, {'!', "..#....#....#....#....#.........#.."},
    {'_', "..............................#####"}, {'[', ".###..#....#....#....#....#....###."},
    {']', ".###....#....#....#....#....#..###."}, {'&', ".##..#..#.#.#...#...#.#.##..#..##.#"},
    {';', "......#....#.........#....#...#...."}, {'"', ".#.#..#.#.........................."},
    {'#', ".#.#..#.#.#####.#.#.#####.#.#..#.#."}, {'%', "##..###..#...#...#...#...#..###..##"},
};
static uint32_t sPix[W * H];

static void fill_rect(int x, int y, int w, int h, uint32_t argb) {
    for (int j = y; j < y + h && j < H; j++)
        for (int i = x; i < x + w && i < W; i++)
            if (i >= 0 && j >= 0) sPix[j * W + i] = argb;
}
// text at (x, y), capitals; 6 pixels a character; at most `max` characters (0: all)
static void text(int x, int y, const char *s, uint32_t argb, int max) {
    for (int n = 0; *s && (!max || n < max); s++, n++, x += 6) {
        char c = *s >= 'a' && *s <= 'z' ? (char)(*s - 32) : *s;
        if (c == ' ') continue;
        const char *rows = NULL;
        for (unsigned g = 0; g < sizeof kFont / sizeof kFont[0]; g++)
            if (kFont[g].c == c) rows = kFont[g].rows;
        if (!rows) rows = kFont[48].rows;  // ('?')
        for (int r = 0; r < 7; r++)
            for (int b = 0; b < 5; b++)
                if (rows[r * 5 + b] == '#' && x + b < W && y + r < H) sPix[(y + r) * W + x + b] = argb;
    }
}
// text at twice the size (12 pixels a character)
static void text2(int x, int y, const char *s, uint32_t argb) {
    for (; *s; s++, x += 12) {
        char c = *s >= 'a' && *s <= 'z' ? (char)(*s - 32) : *s;
        if (c == ' ') continue;
        const char *rows = NULL;
        for (unsigned g = 0; g < sizeof kFont / sizeof kFont[0]; g++)
            if (kFont[g].c == c) rows = kFont[g].rows;
        if (!rows) continue;
        for (int r = 0; r < 7; r++)
            for (int b = 0; b < 5; b++)
                if (rows[r * 5 + b] == '#') fill_rect(x + 2 * b, y + 2 * r, 2, 2, argb);
    }
}
static void draw_settings(void) {
    const uint32_t kBack = 0xf0101820, kBar = 0xff30508a, kText = 0xffffffff, kDim = 0xff98a8c0, kHi = 0xffffe070,
                   kOn = 0xff60e080;
    fill_rect(0, 0, W, H, kBack);
    text2(8, 8, "SETTINGS", kHi);
    for (int i = 0; i < SLINES; i++) {
        int y = line_y(i);
        if (i == sLine) fill_rect(4, y - 7, W - 8, 28, kBar);
        if (i < FEAT_COUNT) {
            text2(10, y, feature_name(i), kText);
            text2(W - 10 - 36, y, feature_on(i) ? " ON" : "OFF", feature_on(i) ? kOn : kDim);
        } else {
            text2(10, y, "CLOSE", kText);
        }
    }
    const char *build = getenv("ACWW_BUILD_LABEL");  // (iOS: the app's build number and commit, ios_glue.m)
    if (build) text(8, H - 14, build, kDim, 0);
}

// a short message over the bottom of the top screen for a few seconds (the game goes on)
static char sToast[48];
static int sToastFrames;
void ui_toast(const char *msg) {
    snprintf(sToast, sizeof sToast, "%s", msg ? msg : "");
    sToastFrames = sToast[0] ? 150 : 0;
    sDirty = 1;
}
static const uint32_t *toast_overlay(int *changed) {
    if (!sToastFrames) return NULL;
    if (--sToastFrames == 0) { *changed = 1; return NULL; }
    if (*changed) {
        memset(sPix, 0, sizeof sPix);
        int n = (int)strlen(sToast), w = n * 6 + 12;
        fill_rect((W - w) / 2, 172, w, 15, 0xe0101820);
        text((W - n * 6) / 2, 176, sToast, 0xffffe070, 0);
    }
    return sPix;
}

const uint32_t *ui_overlay(int *changed) {
    *changed = sDirty;
    if (!sOpen) { sDirty = 0; return toast_overlay(changed); }
    if (sDirty) draw_settings();
    sDirty = 0;
    return sPix;
}

#endif
