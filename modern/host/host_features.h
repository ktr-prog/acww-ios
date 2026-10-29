// Host build: the optional features (docs/features.md) - additions to the game, each off by default (the game as the
// DS has it) and switched on in the settings screen (frontend_ui.c). Kept in a small text file, one "key=0|1" a line:
// on iOS acww-settings.txt in the app's folder, elsewhere settings.txt next to the key bindings.
#ifndef HOST_FEATURES_H
#define HOST_FEATURES_H

#ifdef __cplusplus
extern "C" {
#endif

enum {
    FEAT_CIRCLE_PAD,  // analog walking: the touch D-pad becomes a circle pad, a controller's left stick is analog
    FEAT_COUNT
};

void features_init(const char *path);  // the file's settings (missing: all off)
int feature_on(int f);
void feature_set(int f, int on);       // (saved at once)
const char *feature_name(int f);       // capitals, for the settings screen
const char *feature_help(int f, int line);  // up to 3 lines of capitals ("" past the end)

#ifdef __cplusplus
}
#endif

#endif
