#ifndef SYS_INPUTREPLAY_H
#define SYS_INPUTREPLAY_H

// (port) Buttons and touch at the same time: on the DS a menu in touch mode takes the first button press only to
// change to button mode (and the other way round), with a sound; the press itself does nothing. The menus' switch
// checks (MenuProc::checkSwitchToButtons / checkSwitchToTouch, the resident select) now hand that press back for the
// next frame - Pad_Update adds the keys to gPad.trig, TouchPanel_Update makes a still-held touch a new one - so the
// press both changes the mode and does what it does in the new mode. The choice lists and the talk window take the
// press in the same frame (their checkDeviceSwitch callers go on).
#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

void Input_ReplayKeys(u16 keys);  // gPad.trig of the next frame gets these too
void Input_ReplayTouch(void);     // the next frame's touch, if still held, counts as just touched

#ifdef __cplusplus
}
#endif

#endif
