# Optional features

Additions to the game. Every one starts **off**, so the game plays as it does on the DS. Each has a switch in the
settings screen: on iPhone and iPad, the **MENU** button; on a computer, **F2**; with a controller, **Back + Start**.
The switches are kept in `acww-settings.txt` in the app's folder on iOS, and in `settings.txt` next to the key
bindings elsewhere (`modern/host/features.c`).

How to add one: a `FEAT_` entry in `modern/host/host_features.h`, its key, name and help lines in
`modern/host/features.c`, a row in the table below, and the change itself behind `feature_on(FEAT_...)` (the
frontend's side) or a value the frontend hands the game (the game's side, as the circle pad does through
`AcwwShm.stick`).

| # | Feature | Setting | What it does | State |
|---|---------|---------|--------------|-------|
| 1 | Circle pad | `circle_pad` | Analog walking. The touch D-pad becomes a circle pad, and a controller's left stick works the same way. How far you push sets how fast you walk; B still runs. Pushed far enough, it also presses the D-pad's directions, as a 3DS's circle pad does in DS games, so menus work as before. | Done |

## Circle pad: how it works

The 3DS circle pad code for this game (Action Replay, USA 1.1) hooks `PlayerActor::readInput` at 0x0200DD16, just
after `inputRun` is stored. Its ARM7 part reads the 3DS circle pad into 0x027FFDF0. When the stick is pushed, the hook
sets the player's input like this:

- magnitude = `Math_Sqrt64(x² + y²) << 12 / 105`, at most 0x1000 (1.0). When not running, it is scaled to the D-pad's
  walking speed (`* 0xC32 >> 12`).
- angle = `Math_Atan2(x, -y)`, the same as the stylus's angle.

The native version does the same in `src/main/unk_02004558_extra.cpp`, at the same place. The game reads the stick
from the frontend (`host_circle_pad`, `include/host_input.h`; `AcwwShm.stick`). The touch circle pad
(`modern/host/ios/touch_controls.c`) and a controller's left stick (`modern/host/frontend_sdl.c`) both give positions
from -1 to 1, which are scaled so that 105 is a full push. Both have a dead zone of 12–15%. Past 40% the circle pad
also holds the D-pad's directions (eight directions of 45°).

## Not features (always on)

- **Save states** (SAVE / LOAD; F5 / F9 on a computer). One slot, kept in `acww-state.bin` in the app's folder, so a
  state loads after the app is closed too. It only loads in the same build of the app.
- **Wi-Fi (DS-to-DS local wireless).** Copies of the app find each other over UDP (`modern/host/wm7.c`). On iOS, the
  automatic search on the local network needs a permission from Apple that sideloaded apps don't get. Instead, give
  the other device's address in `acww-options.txt`: `ACWW_NET_PEER=192.168.1.23` (or a host name; `host:port` for
  another port).

## Wanted later

- **Nintendo Wi-Fi Connection** (online play, Tom Nook's Wi-Fi features): the port has no network stack for the DS's
  online library (`ov065`), and Nintendo's servers are gone. A replacement server (e.g. Wiimmfi) would also be needed.
