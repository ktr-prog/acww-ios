// Host build, iOS: what the app's Objective-C side (ios_glue.m) gives the C runtime. The app's main() is there: it
// starts UIKit through SDL, finds the game (a .nds file in the app's Documents folder, which the Files app shows as
// "On My iPhone / On My iPad > ACWW") and calls the runtime's own main (main.c, host_main on iOS) with its path.
#ifndef HOST_IOS_IOS_H
#define HOST_IOS_IOS_H

#include <SDL.h>

#ifdef __cplusplus
extern "C" {
#endif

// the window's safe area insets in points: left, top, right, bottom (0 where unknown)
void ios_safe_area(SDL_Window *window, float out[4]);
// a message the player must see before the app ends (a fatal error, the game's own reset)
void ios_fatal_message(const char *text);
// the runtime's main (main.c)
int host_main(int argc, char **argv);

#ifdef __cplusplus
}
#endif

#endif
