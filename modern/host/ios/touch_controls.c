// Host build, iOS: the on-screen controls (touch_controls.h): layout, touch and pictures. Plain C, no SDL.
#include "touch_controls.h"

#include <math.h>
#include <string.h>

static float clampf(float v, float lo, float hi) { return v < lo ? lo : v > hi ? hi : v; }
static float minf(float a, float b) { return a < b ? a : b; }
static float maxf(float a, float b) { return a > b ? a : b; }
static TcRect rect(float x, float y, float w, float h) { TcRect r = {x, y, w, h}; return r; }
static TcRect square_at(float cx, float cy, float d) { return rect(cx - d / 2, cy - d / 2, d, d); }
static int inside(TcRect r, float x, float y) { return x >= r.x && x < r.x + r.w && y >= r.y && y < r.y + r.h; }

// the screens (2:3) as large as they fit in r, centred
static TcRect fit_screens(TcRect r) {
    float w = r.w, h = w * 1.5f;
    if (h > r.h) h = r.h, w = h / 1.5f;
    return rect(r.x + (r.w - w) / 2, r.y + (r.h - h) / 2, w, h);
}

// A/B/X/Y in a K x K box centred at (cx, cy): the DS's diamond (A right, B bottom, X top, Y left)
static void face_buttons(TcLayout *l, float cx, float cy, float k) {
    float d = k * 0.38f, o = k * 0.31f;
    l->button[TC_A] = square_at(cx + o, cy, d);
    l->button[TC_B] = square_at(cx, cy + o, d);
    l->button[TC_X] = square_at(cx, cy - o, d);
    l->button[TC_Y] = square_at(cx - o, cy, d);
}

void tc_layout(TcLayout *l, int w, int h, float pt, const float safe[4], int controls) {
    memset(l, 0, sizeof *l);
    l->w = w, l->h = h, l->controls = controls;
    if (pt <= 0) pt = 1;
    float sx = safe[0], sy = safe[1], sw = (float)w - safe[0] - safe[2], sh = (float)h - safe[1] - safe[3];
    if (sw < 16 || sh < 16) sx = 0, sy = 0, sw = (float)w, sh = (float)h;  // (nonsense insets: ignored)
    TcRect safeRect = rect(sx, sy, sw, sh);
    if (!controls) {
        l->screens = fit_screens(safeRect);
        return;
    }
    const float m = 8 * pt;  // the margin around the controls
    if (h >= w) {
        // upright: the screens at the top, the controls below them
        float ctrl = clampf(sh * 0.30f, 190 * pt, 270 * pt);
        if (ctrl > sh * 0.45f) ctrl = sh * 0.45f;
        float sch = sh - ctrl, scw = sch / 1.5f;
        if (scw > sw) scw = sw, sch = scw * 1.5f;
        l->screens = rect(sx + (sw - scw) / 2, sy, scw, sch);
        float top = sy + sch, bottom = sy + sh, ch = bottom - top;
        // the row: L, Select, Start, R
        float rh = clampf(ch * 0.17f, 30 * pt, 44 * pt);
        float rowY = top + m * 0.75f;
        float lrw = clampf(sw * 0.24f, 64 * pt, 140 * pt);
        l->button[TC_L] = rect(sx + m, rowY, lrw, rh);
        l->button[TC_R] = rect(sx + sw - m - lrw, rowY, lrw, rh);
        float gap = 10 * pt, ssh = rh * 0.8f;
        float room = l->button[TC_R].x - (l->button[TC_L].x + lrw) - 2 * gap;  // between L and R
        float ssw = minf(clampf(sw * 0.17f, 52 * pt, 110 * pt), (room - gap) / 2);
        float x0 = sx + sw / 2 - (2 * ssw + gap) / 2;
        l->button[TC_SELECT] = rect(x0, rowY + (rh - ssh) / 2, ssw, ssh);
        l->button[TC_START] = rect(x0 + ssw + gap, rowY + (rh - ssh) / 2, ssw, ssh);
        // the D-pad and the face buttons below the row
        float aTop = rowY + rh + m * 0.5f, aBottom = bottom - m * 0.5f, ah = aBottom - aTop;
        float k = minf(minf(ah, sw * 0.44f), 230 * pt);
        float cy = aTop + ah / 2;
        float lcx = maxf(sx + sw * 0.25f, sx + m + k / 2), rcx = minf(sx + sw * 0.75f, sx + sw - m - k / 2);
        l->button[TC_DPAD] = square_at(lcx, cy, k * 0.92f);
        face_buttons(l, rcx, cy, k);
    } else {
        // on its side: the screens in the middle, the controls left and right of them
        const float minSide = 150 * pt;
        float sch = sh, scw = sch / 1.5f, side = (sw - scw) / 2;
        if (side < minSide) {
            side = minSide;
            scw = maxf(sw - 2 * side, sw * 0.3f);
            sch = minf(scw * 1.5f, sh);
            scw = sch / 1.5f;
            side = (sw - scw) / 2;
        }
        l->screens = rect(sx + (sw - scw) / 2, sy + (sh - sch) / 2, scw, sch);
        TcRect panel[2] = {rect(sx, sy, l->screens.x - sx, sh),
                           rect(l->screens.x + scw, sy, sx + sw - (l->screens.x + scw), sh)};
        for (int s = 0; s < 2; s++) {
            TcRect p = panel[s];
            float pw = p.w - 2 * m;
            float rh = clampf(sh * 0.11f, 30 * pt, 44 * pt), lrw = minf(clampf(pw * 0.6f, 64 * pt, 150 * pt), pw);
            float ssh = rh * 0.8f, ssw = minf(clampf(pw * 0.4f, 52 * pt, 110 * pt), pw);
            float cx = p.x + p.w / 2;
            TcRect shoulder = rect(cx - lrw / 2, p.y + m, lrw, rh);
            TcRect small = rect(cx - ssw / 2, p.y + p.h - m - ssh, ssw, ssh);
            float aTop = shoulder.y + rh + m, aBottom = small.y - m, ah = aBottom - aTop;
            float k = minf(minf(ah, pw), 260 * pt);
            float cy = aTop + ah / 2;
            if (s == 0) {
                l->button[TC_L] = shoulder, l->button[TC_SELECT] = small;
                l->button[TC_DPAD] = square_at(cx, cy, k * 0.92f);
            } else {
                l->button[TC_R] = shoulder, l->button[TC_START] = small;
                face_buttons(l, cx, cy, k);
            }
        }
    }
}

// ---- touch

static const uint32_t kBit[TC_COUNT] = {
    [TC_A] = 1u << 0, [TC_B] = 1u << 1, [TC_X] = 1u << 10, [TC_Y] = 1u << 11, [TC_L] = 1u << 9, [TC_R] = 1u << 8,
    [TC_START] = 1u << 3, [TC_SELECT] = 1u << 2, [TC_DPAD] = 0,
};
enum { WHAT_STYLUS = -1, WHAT_NOTHING = -2, WHAT_SLIDING = TC_COUNT };  // (sliding: a button finger between buttons)
enum { DIR_RIGHT = 1u << 4, DIR_LEFT = 1u << 5, DIR_UP = 1u << 6, DIR_DOWN = 1u << 7 };

void tc_reset(TcState *s) { memset(s, 0, sizeof *s); }

// the D-pad's directions for a point (also outside it: a finger that slid off keeps steering); cardinal directions
// get 55 degrees each, the diagonals 35
static uint32_t dpad_dirs(const TcLayout *l, float x, float y) {
    TcRect r = l->button[TC_DPAD];
    float dx = x - (r.x + r.w / 2), dy = (r.y + r.h / 2) - y;
    if (dx * dx + dy * dy < (r.w * 0.12f) * (r.w * 0.12f)) return 0;
    float a = atan2f(dy, dx) * (180.0f / 3.14159265f);  // -180..180, 0 = right, 90 = up
    uint32_t k = 0;
    if (a > -62.5f && a < 62.5f) k |= DIR_RIGHT;
    if (a > 27.5f && a < 152.5f) k |= DIR_UP;
    if (a > 117.5f || a < -117.5f) k |= DIR_LEFT;
    if (a > -152.5f && a < -27.5f) k |= DIR_DOWN;
    return k;
}

// the button (not the D-pad) under a point, with some room around each: the nearest one whose area holds it
static int button_at(const TcLayout *l, float x, float y) {
    float best = 1e30f;
    int found = -1;
    for (int b = 0; b < TC_COUNT; b++) {
        if (b == TC_DPAD) continue;
        TcRect r = l->button[b];
        if (r.w <= 0) continue;
        float cx = r.x + r.w / 2, cy = r.y + r.h / 2, d2;
        if (b <= TC_Y) {  // round: 20% beyond the edge
            float rad = r.w / 2 * 1.2f;
            d2 = (x - cx) * (x - cx) + (y - cy) * (y - cy);
            if (d2 > rad * rad) continue;
        } else {  // pills: a quarter of their height beyond the edge
            float g = r.h * 0.25f;
            if (x < r.x - g || x >= r.x + r.w + g || y < r.y - g || y >= r.y + r.h + g) continue;
            d2 = (x - cx) * (x - cx) + (y - cy) * (y - cy);
        }
        if (d2 < best) best = d2, found = b;
    }
    return found;
}
static int dpad_at(const TcLayout *l, float x, float y) {
    TcRect r = l->button[TC_DPAD];
    float g = r.w * 0.08f;
    return r.w > 0 && x >= r.x - g && x < r.x + r.w + g && y >= r.y - g && y < r.y + r.h + g;
}

static void stylus_point(TcState *s, const TcLayout *l, float x, float y) {
    TcRect sc = l->screens;
    float half = sc.h / 2;
    int px = (int)floorf((x - sc.x) * 256.0f / sc.w), py = (int)floorf((y - (sc.y + half)) * 192.0f / half);
    s->stylus_x = px < 0 ? 0 : px > 255 ? 255 : px;
    s->stylus_y = py < 0 ? 0 : py > 191 ? 191 : py;
}
static int stylus_taken(const TcState *s, int except) {
    for (int i = 0; i < TC_MAX_POINTERS; i++)
        if (i != except && s->p[i].used && s->p[i].what == WHAT_STYLUS) return 1;
    return 0;
}

int tc_pointer(TcState *s, const TcLayout *l, int64_t id, int event, float x, float y) {
    int i = 0;
    while (i < TC_MAX_POINTERS && !(s->p[i].used && s->p[i].id == id)) i++;
    if (event == TC_UP) {
        if (i < TC_MAX_POINTERS) memset(&s->p[i], 0, sizeof s->p[i]);
        return 0;
    }
    if (i == TC_MAX_POINTERS) {
        if (event != TC_DOWN) return 0;  // (a move of a pointer we never saw go down)
        for (i = 0; i < TC_MAX_POINTERS && s->p[i].used; i++) {}
        if (i == TC_MAX_POINTERS) return 0;  // (more fingers than that: ignored)
        s->p[i].used = 1, s->p[i].id = id, s->p[i].keys = 0;
        TcRect sc = l->screens;
        if (inside(sc, x, y)) {
            int bottom = y >= sc.y + sc.h / 2;
            s->p[i].what = bottom && !stylus_taken(s, i) ? WHAT_STYLUS : WHAT_NOTHING;
        } else if (l->controls && dpad_at(l, x, y)) {
            s->p[i].what = TC_DPAD;
        } else if (l->controls) {
            int b = button_at(l, x, y);
            s->p[i].what = b >= 0 ? b : WHAT_NOTHING;
        } else {
            s->p[i].what = WHAT_NOTHING;
            return 1;  // (outside the screens with the controls hidden: the frontend shows them)
        }
    }
    int w = s->p[i].what;
    if (w == WHAT_STYLUS) {
        stylus_point(s, l, x, y);
    } else if (w == TC_DPAD) {
        s->p[i].keys = dpad_dirs(l, x, y);
    } else if (w >= 0) {  // a button finger: whichever button it is on now
        int b = button_at(l, x, y);
        s->p[i].what = b >= 0 ? b : WHAT_SLIDING;
        s->p[i].keys = b >= 0 ? kBit[b] : 0;
    }
    return 0;
}

uint32_t tc_keys(const TcState *s) {
    uint32_t k = 0;
    for (int i = 0; i < TC_MAX_POINTERS; i++)
        if (s->p[i].used) k |= s->p[i].keys;
    return k;
}

int tc_stylus(const TcState *s, int *x, int *y) {
    if (!stylus_taken(s, -1)) return 0;
    *x = s->stylus_x, *y = s->stylus_y;
    return 1;
}

uint32_t tc_held(const TcState *s) {
    uint32_t h = 0;
    for (int i = 0; i < TC_MAX_POINTERS; i++) {
        if (!s->p[i].used || s->p[i].what < 0 || s->p[i].what >= TC_COUNT) continue;
        if (s->p[i].what != TC_DPAD || s->p[i].keys) h |= 1u << s->p[i].what;
    }
    return h;
}

uint32_t tc_dpad_dirs(const TcState *s) {
    uint32_t k = 0;
    for (int i = 0; i < TC_MAX_POINTERS; i++)
        if (s->p[i].used && s->p[i].what == TC_DPAD) k |= s->p[i].keys;
    return k;
}

// ---- pictures: white shapes, translucent, over the game's black surround; signed distances for smooth edges

// a 5x7 font (the binding screen's, frontend_ui.c): the letters the labels use
static const struct { char c; const char *rows; } kFont[] = {
    {'A', ".###.#...##...#######...##...##...#"}, {'B', "####.#...##...#####.#...##...#####."},
    {'C', ".###.#...##....#....#....#...#.###."}, {'E', "######....#....####.#....#....#####"},
    {'L', "#....#....#....#....#....#....#####"}, {'R', "####.#...##...#####.#.#..#..#.#...#"},
    {'S', ".#####....#.....###.....#....#####."}, {'T', "#####..#....#....#....#....#....#.."},
    {'X', "#...##...#.#.#...#...#.#.#...##...#"}, {'Y', "#...##...#.#.#...#....#....#....#.."},
};

// src over dst, both straight alpha (0xAARRGGBB); a: src's alpha (0..1) times its colour's
static void blend(uint32_t *d, uint32_t rgb, float a) {
    if (a <= 0) return;
    if (a > 1) a = 1;
    float da = (float)(*d >> 24) / 255.0f, oa = a + da * (1 - a);
    if (oa <= 0) return;
    uint32_t out = 0;
    for (int sh = 0; sh < 24; sh += 8) {
        float sc = (float)(rgb >> sh & 255), dc = (float)(*d >> sh & 255);
        float c = (sc * a + dc * da * (1 - a)) / oa;
        out |= (uint32_t)(c + 0.5f) << sh;
    }
    *d = out | (uint32_t)(oa * 255.0f + 0.5f) << 24;
}

static float len2(float x, float y) { return sqrtf(x * x + y * y); }
// a rounded box's signed distance (negative inside): centre, half size, corner radius
static float sd_box(float px, float py, float cx, float cy, float hw, float hh, float r) {
    float qx = fabsf(px - cx) - (hw - r), qy = fabsf(py - cy) - (hh - r);
    return len2(maxf(qx, 0), maxf(qy, 0)) + minf(maxf(qx, qy), 0) - r;
}

// the shape at a point: its signed distance, and which D-pad arm it is in (0 none / centre, else a DIR_ bit)
static float shape_sd(int which, int w, int h, float x, float y, uint32_t *arm) {
    float cx = w / 2.0f, cy = h / 2.0f;
    *arm = 0;
    if (which <= TC_Y) return len2(x - cx, y - cy) - (minf((float)w, (float)h) / 2 - 1);
    if (which == TC_DPAD) {
        float half = minf((float)w, (float)h) / 2 - 1, aw = half * 0.36f, r = aw * 0.35f;
        float d = minf(sd_box(x, y, cx, cy, half, aw, r), sd_box(x, y, cx, cy, aw, half, r));
        float dx = x - cx, dy = y - cy;
        if (fabsf(dx) > aw || fabsf(dy) > aw) {
            if (fabsf(dx) >= fabsf(dy)) *arm = dx > 0 ? DIR_RIGHT : DIR_LEFT;
            else *arm = dy > 0 ? DIR_DOWN : DIR_UP;
        }
        return d;
    }
    float hh = h / 2.0f - 1;  // pills
    return sd_box(x, y, cx, cy, w / 2.0f - 1, hh, hh);
}

// an arrow in a D-pad arm (a triangle pointing outwards), 4x4 samples a pixel
static float arrow_cover(int w, int h, float px, float py, uint32_t dir) {
    float cx = w / 2.0f, cy = h / 2.0f, half = minf((float)w, (float)h) / 2 - 1;
    float at = half * 0.68f, s = half * 0.17f;  // the arrow's centre from the middle, its half size
    float n = 0;
    for (int j = 0; j < 4; j++) {
        for (int i = 0; i < 4; i++) {
            float x = px + (i + 0.5f) / 4 - cx, y = py + (j + 0.5f) / 4 - cy;
            // in the arm's own frame: u outwards, v across
            float u = dir == DIR_RIGHT ? x : dir == DIR_LEFT ? -x : dir == DIR_DOWN ? y : -y;
            float v = dir == DIR_RIGHT || dir == DIR_LEFT ? y : x;
            u -= at;
            // the triangle: tip at u = +s, base at u = -s, half width s at the base
            if (u >= -s && u <= s && fabsf(v) <= (s - u) / 2) n += 1;
        }
    }
    return n / 16;
}

static void label(uint32_t *px, int w, int h, const char *text, float alpha) {
    int n = (int)strlen(text);
    float fit = minf(h * 0.42f / 7.0f, w * 0.78f / (6.0f * n - 1));
    int s = (int)fit;
    if (s < 1) s = 1;
    int tw = (6 * n - 1) * s, th = 7 * s, x0 = (w - tw) / 2, y0 = (h - th) / 2;
    for (int k = 0; k < n; k++) {
        const char *rows = 0;
        for (unsigned g = 0; g < sizeof kFont / sizeof kFont[0]; g++)
            if (kFont[g].c == text[k]) rows = kFont[g].rows;
        if (!rows) continue;
        for (int r = 0; r < 7; r++)
            for (int b = 0; b < 5; b++) {
                if (rows[r * 5 + b] != '#') continue;
                for (int yy = 0; yy < s; yy++)
                    for (int xx = 0; xx < s; xx++) {
                        int X = x0 + (k * 6 + b) * s + xx, Y = y0 + r * s + yy;
                        if (X >= 0 && X < w && Y >= 0 && Y < h) blend(&px[Y * w + X], 0xffffff, alpha);
                    }
            }
    }
}

void tc_draw(uint32_t *px, int w, int h, int which, int pressed, uint32_t dirs) {
    static const char *const kLabel[TC_COUNT] = {"A", "B", "X", "Y", "L", "R", "START", "SELECT", ""};
    memset(px, 0, (size_t)w * h * 4);
    if (w <= 2 || h <= 2) return;
    float rim = maxf(1.5f, minf((float)w, (float)h) * (which == TC_DPAD ? 0.025f : 0.05f));
    const float kFill = 0.22f, kFillPressed = 0.55f, kRim = 0.62f;
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            uint32_t arm;
            float d = shape_sd(which, w, h, x + 0.5f, y + 0.5f, &arm);
            float cover = clampf(0.5f - d, 0, 1);
            if (cover <= 0) continue;
            int lit = which == TC_DPAD ? (arm && (dirs & arm)) : pressed;
            float a = lit ? kFillPressed : kFill;
            float rimAmount = clampf(d + rim + 0.5f, 0, 1);  // 1 within `rim` of the edge
            a = a + (kRim - a) * rimAmount;
            blend(&px[y * w + x], 0xffffff, a * cover);
            if (which == TC_DPAD && arm) {
                float c = arrow_cover(w, h, (float)x, (float)y, arm);
                if (c > 0) blend(&px[y * w + x], 0xffffff, 0.75f * c);
            }
        }
    }
    if (which != TC_DPAD) label(px, w, h, kLabel[which], pressed ? 1.0f : 0.85f);
}
