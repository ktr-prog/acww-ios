// Host build: the SDL frontend's key binding screen (frontend_ui.c; frontend_sdl.c uses it). Hidden until F1 (or a
// controller's Back + Start) opens it; the game pauses while it is open. The bindings are kept in a text file:
// ACWW_BINDINGS, else bindings.txt in SDL's preference directory (~/.local/share/acww/ on Linux).
#ifndef HOST_FRONTEND_UI_H
#define HOST_FRONTEND_UI_H

#include <SDL.h>
#include <stdint.h>

void ui_init(void);                                 // the bindings (from the file, else the defaults)
uint32_t ui_keyboard_keys(const Uint8 *keyboard);   // the DS buttons held (AcwwShm.keys bits) on the keyboard
uint32_t ui_pad_keys(SDL_GameController *pad);      // ... and on a controller (its buttons; not the sticks)
int ui_key_bound(SDL_Scancode sc);                  // a DS button's key (frontend_sdl.c's own keys yield to it)
// an event: 1 if the binding screen took it (it is open, or the event opened it)
int ui_event(const SDL_Event *e);
int ui_is_open(void);
// the binding screen's picture over the screens (256x384, 0xAARRGGBB, straight alpha), or NULL while hidden;
// `changed` set when it differs from the last call's
const uint32_t *ui_overlay(int *changed);
// the settings screen (the optional features, host_features.h): open it (iOS: the MENU button); a tap at (x, y) of the
// overlay (256x384) while a screen is open
void ui_open_settings(void);
void ui_toast(const char *msg);  // a short message (capitals) shown for a few seconds over the screens
int ui_tap(int x, int y);

#endif
