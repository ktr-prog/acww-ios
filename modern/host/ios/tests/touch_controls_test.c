// Host build, iOS: checks of the on-screen controls (modern/host/ios/touch_controls.c) on any machine:
//   cc -O1 -Wall -Imodern/host/ios modern/host/ios/tests/touch_controls_test.c modern/host/ios/touch_controls.c -lm
//   ./a.out [preview-dir]
// For each device below, upright and on its side, with and without the buttons: the screens keep the DS's shape and
// stay in the safe area, the buttons too, nothing overlaps, everything is big enough to touch; then the touch rules
// (buttons, sliding, the D-pad's directions, the stylus, several fingers). With a directory it also writes a picture
// of each layout (PPM) there.
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "touch_controls.h"

static int sFails;
#define CHECK(c, ...) do { if (!(c)) { sFails++; printf("FAIL %s:%d: ", __FILE__, __LINE__); printf(__VA_ARGS__); printf("\n"); } } while (0)

typedef struct {
    const char *name;
    int w, h;          // points, upright
    float scale;       // pixels per point
    float up[4];       // safe insets upright (l, t, r, b), points
    float side[4];     // on its side
} Device;

static const Device kDevices[] = {
    {"iPhone SE (3rd gen)", 375, 667, 2, {0, 20, 0, 0}, {0, 0, 0, 0}},
    {"iPhone 13 mini", 375, 812, 3, {0, 50, 0, 34}, {50, 0, 50, 21}},
    {"iPhone 16", 393, 852, 3, {0, 59, 0, 34}, {59, 0, 59, 21}},
    {"iPhone 16 Pro Max", 440, 956, 3, {0, 62, 0, 34}, {62, 0, 62, 21}},
    {"iPhone 11", 414, 896, 2, {0, 48, 0, 34}, {48, 0, 48, 21}},
    {"iPad mini (6th gen)", 744, 1133, 2, {0, 24, 0, 20}, {0, 24, 0, 20}},
    {"iPad (10th gen)", 820, 1180, 2, {0, 24, 0, 20}, {0, 24, 0, 20}},
    {"iPad Pro 13-inch", 1032, 1376, 2, {0, 24, 0, 20}, {0, 24, 0, 20}},
};

static int contains(TcRect outer, TcRect r, float slack) {
    return r.x >= outer.x - slack && r.y >= outer.y - slack && r.x + r.w <= outer.x + outer.w + slack &&
           r.y + r.h <= outer.y + outer.h + slack;
}
static int overlap(TcRect a, TcRect b) {
    return a.x < b.x + b.w && b.x < a.x + a.w && a.y < b.y + b.h && b.y < a.y + a.h;
}
// the round buttons overlap only if their circles do
static int overlap_controls(int i, int j, TcRect a, TcRect b) {
    if (i <= TC_Y && j <= TC_Y) {
        float dx = (a.x + a.w / 2) - (b.x + b.w / 2), dy = (a.y + a.h / 2) - (b.y + b.h / 2);
        return sqrtf(dx * dx + dy * dy) < (a.w + b.w) / 2;
    }
    return overlap(a, b);
}
static float cx(TcRect r) { return r.x + r.w / 2; }
static float cy(TcRect r) { return r.y + r.h / 2; }

static const char *kName[TC_COUNT] = {"A", "B", "X", "Y", "L", "R", "START", "SELECT", "DPAD"};

static void write_preview(const char *dir, const char *dev, const char *orient, int controls, const TcLayout *l,
                          float pt, const float safe[4]) {
    int W = l->w, H = l->h;
    uint32_t *img = calloc((size_t)W * H, 4);
    // the unsafe area a little lighter, to see it; the screens as two test patterns
    for (int y = 0; y < H; y++)
        for (int x = 0; x < W; x++) {
            int unsafe = x < safe[0] || y < safe[1] || x >= W - safe[2] || y >= H - safe[3];
            img[y * W + x] = unsafe ? 0x202020 : 0x000000;
        }
    for (int y = 0; y < (int)l->screens.h; y++)
        for (int x = 0; x < (int)l->screens.w; x++) {
            int X = (int)l->screens.x + x, Y = (int)l->screens.y + y;
            int top = y < l->screens.h / 2;
            int u = x * 256 / (int)l->screens.w, v = (y % (int)(l->screens.h / 2)) * 192 / (int)(l->screens.h / 2);
            uint32_t c = top ? (0x3a7d44u + ((u / 32 + v / 32) % 2) * 0x101010u) : (0x6b8fc2u + ((u / 32 + v / 32) % 2) * 0x101010u);
            if (X >= 0 && X < W && Y >= 0 && Y < H) img[Y * W + X] = c;
        }
    if (controls) {
        for (int b = 0; b < TC_COUNT; b++) {
            TcRect r = l->button[b];
            int bw = (int)lroundf(r.w), bh = (int)lroundf(r.h), bx = (int)lroundf(r.x), by = (int)lroundf(r.y);
            uint32_t *px = malloc((size_t)bw * bh * 4);
            // (A and the D-pad's up-right drawn pressed: to see both looks)
            tc_draw(px, bw, bh, b, b == TC_A, b == TC_DPAD ? (1u << 4 | 1u << 6) : 0);
            for (int y = 0; y < bh; y++)
                for (int x = 0; x < bw; x++) {
                    int X = bx + x, Y = by + y;
                    if (X < 0 || X >= W || Y < 0 || Y >= H) continue;
                    uint32_t s = px[y * bw + x], d = img[Y * W + X];
                    float a = (float)(s >> 24) / 255.0f;
                    uint32_t o = 0;
                    for (int sh = 0; sh < 24; sh += 8)
                        o |= (uint32_t)((float)(s >> sh & 255) * a + (float)(d >> sh & 255) * (1 - a) + 0.5f) << sh;
                    img[Y * W + X] = o;
                }
            free(px);
        }
    }
    char path[1024], name[256];
    snprintf(name, sizeof name, "%s_%s_%s", dev, orient, controls ? "buttons" : "controller");
    for (char *p = name; *p; p++) if (*p == ' ' || *p == '(' || *p == ')') *p = '_';
    snprintf(path, sizeof path, "%s/%s.ppm", dir, name);
    FILE *f = fopen(path, "wb");
    if (f) {
        fprintf(f, "P6\n%d %d\n255\n", W, H);
        for (int i = 0; i < W * H; i++) { unsigned char rgb[3] = {img[i] >> 16, img[i] >> 8, img[i]}; fwrite(rgb, 1, 3, f); }
        fclose(f);
    }
    free(img);
    (void)pt;
}

static void check_layout(const Device *d, int side, int controls, const char *dir) {
    float pt = d->scale;
    int w = (int)((side ? d->h : d->w) * pt), h = (int)((side ? d->w : d->h) * pt);
    const float *in = side ? d->side : d->up;
    float safe[4] = {in[0] * pt, in[1] * pt, in[2] * pt, in[3] * pt};
    TcLayout l;
    tc_layout(&l, w, h, pt, safe, controls);
    TcRect sr = {safe[0], safe[1], w - safe[0] - safe[2], h - safe[1] - safe[3]};
    const char *o = side ? "side" : "upright";
    CHECK(contains(sr, l.screens, 0.5f), "%s %s: screens outside the safe area", d->name, o);
    CHECK(fabsf(l.screens.h - l.screens.w * 1.5f) < 1.0f, "%s %s: screens not 2:3 (%.1f x %.1f)", d->name, o,
          l.screens.w, l.screens.h);
    float sw = l.screens.w / pt;
    if (controls) {
        CHECK(sw >= 230, "%s %s: screens only %.0f pt wide", d->name, o, sw);
        for (int i = 0; i < TC_COUNT; i++) {
            TcRect a = l.button[i];
            CHECK(a.w > 0 && a.h > 0, "%s %s: %s has no size", d->name, o, kName[i]);
            CHECK(contains(sr, a, 0.5f), "%s %s: %s outside the safe area", d->name, o, kName[i]);
            CHECK(!overlap(a, l.screens), "%s %s: %s over the screens", d->name, o, kName[i]);
            for (int j = i + 1; j < TC_COUNT; j++)
                CHECK(!overlap_controls(i, j, a, l.button[j]), "%s %s: %s and %s overlap", d->name, o, kName[i], kName[j]);
            float minSide = (i <= TC_Y ? 44 : i == TC_DPAD ? 120 : 26) * pt;
            CHECK(a.h >= minSide - 0.5f && a.w >= minSide - 0.5f, "%s %s: %s too small (%.0f x %.0f pt)", d->name, o,
                  kName[i], a.w / pt, a.h / pt);
        }
        printf("  %-22s %-7s screens %3.0fx%3.0f pt (DS x%.2f), A/B/X/Y %2.0f pt, D-pad %3.0f pt, L/R %3.0fx%2.0f pt\n",
               d->name, o, l.screens.w / pt, l.screens.h / pt, l.screens.w / 256.0f, l.button[TC_A].w / pt,
               l.button[TC_DPAD].w / pt, l.button[TC_L].w / pt, l.button[TC_L].h / pt);
    } else {
        // as large as the safe area allows
        CHECK(fabsf(l.screens.w - sr.w) < 1 || fabsf(l.screens.h - sr.h) < 1, "%s %s: screens not as large as they fit",
              d->name, o);
    }
    if (dir) write_preview(dir, d->name, o, controls, &l, pt, safe);
}

static void check_touch(void) {
    const Device *d = &kDevices[2];  // iPhone 16
    float pt = d->scale, safe[4] = {0, d->up[1] * pt, 0, d->up[3] * pt};
    int w = (int)(d->w * pt), h = (int)(d->h * pt);
    TcLayout l;
    tc_layout(&l, w, h, pt, safe, 1);
    TcState s;
    tc_reset(&s);
    static const uint32_t kBits[] = {1u << 0, 1u << 1, 1u << 10, 1u << 11, 1u << 9, 1u << 8, 1u << 3, 1u << 2};
    for (int b = 0; b < TC_DPAD; b++) {
        tc_pointer(&s, &l, 7, TC_DOWN, cx(l.button[b]), cy(l.button[b]));
        CHECK(tc_keys(&s) == kBits[b], "button %s gives keys %#x", kName[b], tc_keys(&s));
        CHECK(tc_held(&s) == 1u << b, "button %s held %#x", kName[b], tc_held(&s));
        tc_pointer(&s, &l, 7, TC_UP, cx(l.button[b]), cy(l.button[b]));
        CHECK(tc_keys(&s) == 0, "button %s not released", kName[b]);
    }
    // sliding from A to B, then off every button, then back to A
    tc_pointer(&s, &l, 1, TC_DOWN, cx(l.button[TC_A]), cy(l.button[TC_A]));
    tc_pointer(&s, &l, 1, TC_MOVE, cx(l.button[TC_B]), cy(l.button[TC_B]));
    CHECK(tc_keys(&s) == (1u << 1), "slide A -> B: keys %#x", tc_keys(&s));
    tc_pointer(&s, &l, 1, TC_MOVE, l.button[TC_B].x + l.button[TC_B].w * 3, (float)h - 1);
    CHECK(tc_keys(&s) == 0, "slid off: keys %#x", tc_keys(&s));
    tc_pointer(&s, &l, 1, TC_MOVE, cx(l.button[TC_A]), cy(l.button[TC_A]));
    CHECK(tc_keys(&s) == 1, "slid back onto A: keys %#x", tc_keys(&s));
    // the D-pad at the same time as A: each direction and the diagonals
    TcRect dp = l.button[TC_DPAD];
    struct { float a; uint32_t k; } dirs[] = {
        {0, 1u << 4}, {90, 1u << 6}, {180, 1u << 5}, {270, 1u << 7},
        {45, 1u << 4 | 1u << 6}, {135, 1u << 5 | 1u << 6}, {225, 1u << 5 | 1u << 7}, {315, 1u << 4 | 1u << 7},
        {20, 1u << 4}, {70, 1u << 6},
    };
    for (unsigned i = 0; i < sizeof dirs / sizeof dirs[0]; i++) {
        float r = dp.w * 0.35f, a = dirs[i].a * 3.14159265f / 180;
        float x = cx(dp) + r * cosf(a), y = cy(dp) - r * sinf(a);
        tc_pointer(&s, &l, 2, i == 0 ? TC_DOWN : TC_MOVE, x, y);
        CHECK(tc_keys(&s) == (dirs[i].k | 1u), "D-pad at %.0f degrees (with A): keys %#x, want %#x", dirs[i].a,
              tc_keys(&s), dirs[i].k | 1u);
        CHECK(tc_dpad_dirs(&s) == dirs[i].k, "D-pad dirs at %.0f", dirs[i].a);
    }
    tc_pointer(&s, &l, 2, TC_MOVE, cx(dp), cy(dp));
    CHECK(tc_keys(&s) == 1u, "D-pad centre: keys %#x", tc_keys(&s));
    // (a finger that slides far off the D-pad keeps steering)
    tc_pointer(&s, &l, 2, TC_MOVE, cx(dp), cy(dp) - dp.h * 2);
    CHECK(tc_keys(&s) == (1u | 1u << 6), "far above the D-pad: keys %#x", tc_keys(&s));
    tc_pointer(&s, &l, 2, TC_UP, 0, 0);
    tc_pointer(&s, &l, 1, TC_UP, 0, 0);
    CHECK(tc_keys(&s) == 0, "all released: keys %#x", tc_keys(&s));
    // the stylus: the bottom screen's corners, the top screen, a second finger
    TcRect sc = l.screens;
    int x, y;
    tc_pointer(&s, &l, 3, TC_DOWN, sc.x + 0.1f, sc.y + sc.h / 2 + 0.1f);
    CHECK(tc_stylus(&s, &x, &y) && x == 0 && y == 0, "stylus top-left: %d,%d", x, y);
    tc_pointer(&s, &l, 3, TC_MOVE, sc.x + sc.w - 0.1f, sc.y + sc.h - 0.1f);
    CHECK(tc_stylus(&s, &x, &y) && x == 255 && y == 191, "stylus bottom-right: %d,%d", x, y);
    tc_pointer(&s, &l, 3, TC_MOVE, sc.x + sc.w / 2, sc.y + sc.h * 0.75f);
    CHECK(tc_stylus(&s, &x, &y) && x == 128 && y == 96, "stylus middle: %d,%d", x, y);
    tc_pointer(&s, &l, 3, TC_MOVE, sc.x - 50, sc.y);  // dragged off: held at the nearest edge
    CHECK(tc_stylus(&s, &x, &y) && x == 0 && y == 0, "stylus dragged off: %d,%d", x, y);
    tc_pointer(&s, &l, 4, TC_DOWN, sc.x + sc.w / 4, sc.y + sc.h * 0.9f);  // a second finger: not the stylus
    tc_pointer(&s, &l, 3, TC_MOVE, sc.x + sc.w / 2, sc.y + sc.h * 0.75f);
    CHECK(tc_stylus(&s, &x, &y) && x == 128 && y == 96, "the first finger stays the stylus: %d,%d", x, y);
    tc_pointer(&s, &l, 3, TC_UP, 0, 0);
    CHECK(!tc_stylus(&s, &x, &y), "stylus lifted (the second finger isn't it)");
    tc_pointer(&s, &l, 4, TC_UP, 0, 0);
    tc_pointer(&s, &l, 5, TC_DOWN, sc.x + sc.w / 2, sc.y + sc.h / 4);  // the top screen
    CHECK(!tc_stylus(&s, &x, &y) && tc_keys(&s) == 0, "the top screen does nothing");
    tc_pointer(&s, &l, 5, TC_UP, 0, 0);
    CHECK(tc_pointer(&s, &l, 6, TC_DOWN, cx(l.button[TC_A]), cy(l.button[TC_A])) == 0, "a button: not 'show'");
    tc_pointer(&s, &l, 6, TC_UP, 0, 0);
    // the controls hidden: a touch beside the screens asks for them, the bottom screen still works
    TcLayout hidden;
    tc_layout(&hidden, w, h, pt, safe, 0);
    CHECK(tc_pointer(&s, &hidden, 8, TC_DOWN, 2, (float)h - safe[3] - 2) == 1, "hidden: a touch below asks for the buttons");
    tc_pointer(&s, &hidden, 8, TC_UP, 0, 0);
    CHECK(tc_pointer(&s, &hidden, 9, TC_DOWN, cx(hidden.screens), hidden.screens.y + hidden.screens.h * 0.75f) == 0 &&
          tc_stylus(&s, &x, &y), "hidden: the stylus works");
    tc_pointer(&s, &hidden, 9, TC_UP, 0, 0);
    // moves and ups of pointers never seen, more fingers than slots
    tc_pointer(&s, &l, 99, TC_MOVE, 10, 10);
    tc_pointer(&s, &l, 99, TC_UP, 10, 10);
    CHECK(tc_keys(&s) == 0, "unknown pointers ignored");
    for (int i = 0; i < 20; i++) tc_pointer(&s, &l, 100 + i, TC_DOWN, cx(l.button[TC_B]), cy(l.button[TC_B]));
    CHECK(tc_keys(&s) == 2, "many fingers on B: keys %#x", tc_keys(&s));
    for (int i = 0; i < 20; i++) tc_pointer(&s, &l, 100 + i, TC_UP, 0, 0);
    CHECK(tc_keys(&s) == 0, "many fingers released: keys %#x", tc_keys(&s));
}

int main(int argc, char **argv) {
    const char *dir = argc > 1 ? argv[1] : NULL;
    for (unsigned i = 0; i < sizeof kDevices / sizeof kDevices[0]; i++)
        for (int side = 0; side < 2; side++)
            for (int controls = 1; controls >= 0; controls--) check_layout(&kDevices[i], side, controls, dir);
    check_touch();
    printf(sFails ? "touch_controls_test: %d FAILED\n" : "touch_controls_test: all passed\n", sFails);
    return sFails != 0;
}
