// Host build: the key binding screen (frontend_ui.h). Each DS button has a key and a controller button (either may
// be none); the controller's left stick and triggers stay the D-pad and L / R besides. The screen is drawn into an
// overlay of the screens' size (256x384) with a 5x7 font of its own, which frontend_sdl.c shows over the frame.
//   F1 or Back + Start: open / close        Up / Down (or the D-pad): choose a line
//   Enter (or A): change it - the next key or controller button pressed becomes its (Escape: keep the old one)
//   Delete (or Y): clear it                  Escape, F1 (or B): close (the bindings are saved then)
// F1 and F11 can't be bound (the frontend's own keys); M mutes only while no DS button has it.
#if HOST_FRONTEND_SDL

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "host_features.h"
#include "frontend_ui.h"

enum { W = 256, H = 384, NACT = 12, NLINES = NACT + 2 };
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
static SDL_Scancode sKey[NACT];
static SDL_GameControllerButton sPad[NACT];
static int sOpen, sLine, sCapture, sDirty = 1;
// the page: the key bindings, or the settings (the optional features, host_features.h: a line each, then the key
// bindings and close)
enum { PAGE_BINDINGS, PAGE_SETTINGS };
static int sPage;
enum { SLINES = FEAT_COUNT + 2 };
static char sPath[1024];

// ---- the bindings and their file: a line per DS button, "<button>\t<key>\t<controller button>" (SDL's names; "-":
// none)
static void defaults(void) {
    for (int i = 0; i < NACT; i++) sKey[i] = kActions[i].key, sPad[i] = kActions[i].pad;
}
static void save(void) {
    FILE *f = sPath[0] ? fopen(sPath, "w") : NULL;
    if (!f) { fprintf(stderr, "frontend: key bindings: can't write %s\n", sPath); return; }
    fprintf(f, "# Animal Crossing: Wild World (native): key bindings - DS button, key, controller button (- none)\n");
    for (int i = 0; i < NACT; i++) {
        const char *k = sKey[i] != SDL_SCANCODE_UNKNOWN ? SDL_GetScancodeName(sKey[i]) : "-";
        const char *p = sPad[i] != SDL_CONTROLLER_BUTTON_INVALID ? SDL_GameControllerGetStringForButton(sPad[i]) : "-";
        fprintf(f, "%s\t%s\t%s\n", kActions[i].name, k && *k ? k : "-", p ? p : "-");
    }
    fclose(f);
}
static void load(void) {
    FILE *f = fopen(sPath, "r");
    if (!f) return;
    char line[256];
    while (fgets(line, sizeof line, f)) {
        if (line[0] == '#') continue;
        line[strcspn(line, "\r\n")] = 0;
        char *k = strchr(line, '\t'), *p = k ? strchr(k + 1, '\t') : NULL;
        if (!p) continue;
        *k++ = 0, *p++ = 0;
        for (int i = 0; i < NACT; i++) {
            if (strcmp(line, kActions[i].name)) continue;
            sKey[i] = strcmp(k, "-") ? SDL_GetScancodeFromName(k) : SDL_SCANCODE_UNKNOWN;
            sPad[i] = strcmp(p, "-") ? SDL_GameControllerGetButtonFromString(p) : SDL_CONTROLLER_BUTTON_INVALID;
        }
    }
    fclose(f);
}
void ui_init(void) {
    defaults();
    if (getenv("ACWW_BINDINGS")) {
        snprintf(sPath, sizeof sPath, "%s", getenv("ACWW_BINDINGS"));
    } else {
        char *dir = SDL_GetPrefPath("", "acww");
        if (dir) snprintf(sPath, sizeof sPath, "%sbindings.txt", dir);
        SDL_free(dir);
    }
    load();
}

uint32_t ui_keyboard_keys(const Uint8 *keyboard) {
    uint32_t k = 0;
    for (int i = 0; i < NACT; i++)
        if (sKey[i] != SDL_SCANCODE_UNKNOWN && keyboard[sKey[i]]) k |= kActions[i].bit;
    return k;
}
uint32_t ui_pad_keys(SDL_GameController *pad) {
    uint32_t k = 0;
    for (int i = 0; i < NACT; i++)
        if (sPad[i] != SDL_CONTROLLER_BUTTON_INVALID && SDL_GameControllerGetButton(pad, sPad[i])) k |= kActions[i].bit;
    return k;
}
int ui_key_bound(SDL_Scancode sc) {
    for (int i = 0; i < NACT; i++)
        if (sKey[i] == sc) return 1;
    return 0;
}
int ui_is_open(void) { return sOpen; }

// ---- the screen's controls
static void choose_line(void);
static void set_open(int open) {
    if (sOpen && !open && sPage == PAGE_BINDINGS) save();
    sOpen = open;
    sCapture = 0;
    sDirty = 1;
}
static void open_page(int page) {
    if (sOpen && sPage == PAGE_BINDINGS && page != PAGE_BINDINGS) save();
    sPage = page;
    sLine = 0;
    set_open(1);
}
void ui_open_settings(void) { open_page(PAGE_SETTINGS); }
// the settings page's line: a feature on / off, the key bindings, close
static void settings_choose(void) {
    if (sLine < FEAT_COUNT) feature_set(sLine, !feature_on(sLine));
    else if (sLine == FEAT_COUNT) open_page(PAGE_BINDINGS);
    else set_open(0);
    sDirty = 1;
}
// the lines' places (the overlay's pixels): the bindings page's 11 high, the settings page's 30 (touch sized)
static int line_y(int i) {
    if (sPage == PAGE_SETTINGS) return 44 + i * 32 + (i >= FEAT_COUNT ? 12 : 0);
    return 36 + i * 11 + (i >= NACT ? 6 : 0);
}
int ui_tap(int x, int y) {
    (void)x;
    if (!sOpen) return 0;
    int n = sPage == PAGE_SETTINGS ? SLINES : NLINES, h = sPage == PAGE_SETTINGS ? 28 : 11;
    for (int i = 0; i < n; i++) {
        int ly = line_y(i) - (sPage == PAGE_SETTINGS ? 7 : 2);
        if (y < ly || y >= ly + h) continue;
        sLine = i;
        sDirty = 1;
        if (sPage == PAGE_SETTINGS) settings_choose();
        else if (i >= NACT) choose_line();  // (reset, close: a binding needs a key or a controller button)
        return 1;
    }
    return 1;
}
static void bind_key(SDL_Scancode sc) {
    for (int i = 0; i < NACT; i++)
        if (sKey[i] == sc) sKey[i] = SDL_SCANCODE_UNKNOWN;  // (a key does one thing)
    sKey[sLine] = sc;
}
static void bind_pad(SDL_GameControllerButton b) {
    for (int i = 0; i < NACT; i++)
        if (sPad[i] == b) sPad[i] = SDL_CONTROLLER_BUTTON_INVALID;
    sPad[sLine] = b;
}
static void choose_line(void) {  // Enter / A on the current line
    if (sPage == PAGE_SETTINGS) { settings_choose(); return; }
    if (sLine < NACT) sCapture = 1;
    else if (sLine == NACT) defaults();
    else set_open(0);
}
static int nlines(void) { return sPage == PAGE_SETTINGS ? SLINES : NLINES; }
int ui_event(const SDL_Event *e) {
    if (e->type == SDL_KEYDOWN && !e->key.repeat && e->key.keysym.scancode == SDL_SCANCODE_F1) {
        if (sOpen && sPage == PAGE_BINDINGS) set_open(0);
        else open_page(PAGE_BINDINGS);
        return 1;
    }
    if (e->type == SDL_KEYDOWN && !e->key.repeat && e->key.keysym.scancode == SDL_SCANCODE_F2) {
        if (sOpen && sPage == PAGE_SETTINGS) set_open(0);
        else open_page(PAGE_SETTINGS);
        return 1;
    }
    if (e->type == SDL_CONTROLLERBUTTONDOWN && !sCapture) {  // Back + Start: open / close
        SDL_GameController *c = SDL_GameControllerFromInstanceID(e->cbutton.which);
        int b = e->cbutton.button;
        if (c && ((b == SDL_CONTROLLER_BUTTON_BACK && SDL_GameControllerGetButton(c, SDL_CONTROLLER_BUTTON_START)) ||
                  (b == SDL_CONTROLLER_BUTTON_START && SDL_GameControllerGetButton(c, SDL_CONTROLLER_BUTTON_BACK)))) {
            if (sOpen) set_open(0);
            else open_page(PAGE_SETTINGS);
            return 1;
        }
    }
    if (!sOpen) return 0;
    sDirty = 1;
    if (e->type == SDL_KEYDOWN && !e->key.repeat) {
        SDL_Scancode sc = e->key.keysym.scancode;
        if (sCapture) {
            if (sc != SDL_SCANCODE_ESCAPE && sc != SDL_SCANCODE_F11) bind_key(sc);
            sCapture = 0;
        } else if (sc == SDL_SCANCODE_UP) {
            sLine = (sLine + nlines() - 1) % nlines();
        } else if (sc == SDL_SCANCODE_DOWN) {
            sLine = (sLine + 1) % nlines();
        } else if (sc == SDL_SCANCODE_RETURN || sc == SDL_SCANCODE_KP_ENTER) {
            choose_line();
        } else if ((sc == SDL_SCANCODE_DELETE || sc == SDL_SCANCODE_BACKSPACE) && sLine < NACT &&
                   sPage == PAGE_BINDINGS) {
            sKey[sLine] = SDL_SCANCODE_UNKNOWN;
            sPad[sLine] = SDL_CONTROLLER_BUTTON_INVALID;
        } else if (sc == SDL_SCANCODE_ESCAPE) {
            set_open(0);
        }
    } else if (e->type == SDL_CONTROLLERBUTTONDOWN) {
        SDL_GameControllerButton b = (SDL_GameControllerButton)e->cbutton.button;
        if (sCapture) {
            bind_pad(b);
            sCapture = 0;
        } else if (b == SDL_CONTROLLER_BUTTON_DPAD_UP) {
            sLine = (sLine + nlines() - 1) % nlines();
        } else if (b == SDL_CONTROLLER_BUTTON_DPAD_DOWN) {
            sLine = (sLine + 1) % nlines();
        } else if (b == SDL_CONTROLLER_BUTTON_A) {
            choose_line();
        } else if (b == SDL_CONTROLLER_BUTTON_Y && sLine < NACT && sPage == PAGE_BINDINGS) {
            sKey[sLine] = SDL_SCANCODE_UNKNOWN;
            sPad[sLine] = SDL_CONTROLLER_BUTTON_INVALID;
        } else if (b == SDL_CONTROLLER_BUTTON_B) {
            set_open(0);
        }
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
static const char *pad_name(SDL_GameControllerButton b) {
    static const char *const kNames[] = {"A", "B", "X", "Y", "BACK", "GUIDE", "START", "L-STICK", "R-STICK", "LB",
                                         "RB", "D-PAD UP", "D-PAD DOWN", "D-PAD LEFT", "D-PAD RIGHT"};
    if (b == SDL_CONTROLLER_BUTTON_INVALID) return "-";
    if ((unsigned)b < sizeof kNames / sizeof kNames[0]) return kNames[b];
    const char *s = SDL_GameControllerGetStringForButton(b);
    return s ? s : "?";
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
            text2(10, y, i == FEAT_COUNT ? "KEY BINDINGS" : "CLOSE", kText);
        }
    }
    int y = line_y(SLINES - 1) + 36;
    if (sLine < FEAT_COUNT)
        for (int k = 0; k < 3; k++) text(8, y + k * 11, feature_help(sLine, k), kDim, 0);
    static const char *const kHelp[] = {
        "TAP A LINE TO SWITCH IT. EACH FEATURE",
        "STARTS OFF: THE GAME AS ON THE DS.",
        "THE GAME IS PAUSED WHILE THIS IS OPEN.",
        "",
        "KEYBOARD: F2 OPENS THIS, F1 THE KEY",
        "  BINDINGS. CONTROLLER: BACK+START.",
    };
    for (unsigned i = 0; i < sizeof kHelp / sizeof kHelp[0]; i++) text(8, H - 8 - 11 * (6 - (int)i), kHelp[i], kDim, 0);
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
    if (!sDirty) return sPix;
    sDirty = 0;
    if (sPage == PAGE_SETTINGS) { draw_settings(); return sPix; }
    const uint32_t kBack = 0xf0101820, kBar = 0xff30508a, kText = 0xffffffff, kDim = 0xff98a8c0, kHi = 0xffffe070;
    fill_rect(0, 0, W, H, kBack);
    text(8, 6, "KEY BINDINGS", kHi, 0);
    text(256 - 8 - 6 * 9, 6, "F1: CLOSE", kDim, 0);
    text(8, 22, "DS", kDim, 0);
    text(62, 22, "KEY", kDim, 0);
    text(158, 22, "CONTROLLER", kDim, 0);
    for (int i = 0; i < NLINES; i++) {
        int y = line_y(i);
        if (i == sLine) fill_rect(4, y - 2, W - 8, 11, kBar);
        if (i == NACT) { text(8, y, "RESET TO DEFAULTS", kText, 0); continue; }
        if (i == NACT + 1) { text(8, y, "CLOSE", kText, 0); continue; }
        text(8, y, kActions[i].name, kText, 0);
        if (sCapture && i == sLine) {
            text(62, y, "PRESS A KEY OR BUTTON", kHi, 0);
            continue;
        }
        text(62, y, sKey[i] != SDL_SCANCODE_UNKNOWN ? SDL_GetScancodeName(sKey[i]) : "-", kText, 15);
        text(158, y, pad_name(sPad[i]), kText, 16);
    }
    static const char *const kHelp[] = {
        "UP/DOWN: CHOOSE",
        "ENTER: CHANGE - THEN PRESS A KEY OR",
        "  A CONTROLLER BUTTON (ESC: KEEP IT)",
        "DELETE: CLEAR",
        "ESC OR F1: CLOSE AND SAVE",
        "",
        "CONTROLLER: D-PAD CHOOSES, A CHANGES,",
        "  Y CLEARS, B CLOSES. F2: SETTINGS.",
        "  ITS LEFT STICK IS ALSO THE",
        "  D-PAD, ITS TRIGGERS L AND R.",
        "",
        "THE GAME IS PAUSED WHILE THIS IS OPEN.",
        "",
        "OTHER KEYS: F11 WINDOW SIZE,",
        "  ALT+ENTER FULL SCREEN, M MUTE",
    };
    for (unsigned i = 0; i < sizeof kHelp / sizeof kHelp[0]; i++) text(8, 204 + (int)i * 11, kHelp[i], kDim, 0);
    return sPix;
}

#endif
