// Host build: the optional features - additions to the game, each off by default (the game as the
// DS has it) and switched on in the settings screen (frontend_ui.c). Kept in a small text file, one "key=0|1" a line:
// acww-settings.txt (iOS: in the app's folder).
#ifndef HOST_FEATURES_H
#define HOST_FEATURES_H

#ifdef __cplusplus
extern "C" {
#endif

enum {
    FEAT_CIRCLE_PAD,  // analog walking: the touch D-pad becomes a circle pad, a controller's left stick is analog
    FEAT_FPS,         // the game's own frame rate (its main loop's turns a second) over the top screen's corner
    FEAT_NO_RESETTI,  // Resetti never comes: starting after a session ended without saving counts no reset
                      // (unk_0209dc94.cpp; the Action Replay code 221cd774 00000000's effect)
    FEAT_COUNT
};

// the game's main loop's turns so far (frontend.c, after each turn's tasks): FEAT_FPS counts them
extern volatile unsigned long long gHostGameFrames;

void features_init(const char *path);  // the file's settings (missing: all off)
int feature_on(int f);
void feature_set(int f, int on);       // (saved at once)
const char *feature_name(int f);       // capitals, for the settings screen
// (not switches: both always there, chosen at the gate as on the DS)
// local play (DS-to-DS, wm7.c; iOS: the copies found with Bonjour): on (ACWW_LOCAL_PLAY=0: off, for tests)
int host_local_play_on(void);
// Nintendo Wi-Fi Connection through Wiimmfi (wm7.c, net_nat.c, pxi7.c): on (ACWW_WIIMMFI=0: off, for tests)
int host_wiimmfi_on(void);

#ifdef __cplusplus
}
#endif

#endif
