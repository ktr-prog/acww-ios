# iPhone and iPad

The same native program as on the PC (no emulator), built for iPhones and iPads with iOS 14 or later (so iOS 18
too). The on-screen layout is checked for every size from the iPhone SE to the iPad Pro. You need your own dump of
the game:
**Animal Crossing: Wild World, USA (Rev 1)**, SHA-1 `77fde3e30e1e6068395d1f96ea63be569b61c351`.

## 1. Build the app on GitHub

You don't need a Mac. The repository's workflow `.github/workflows/ios.yml` builds the app on GitHub's macOS
machines.

1. Push this repository to GitHub. The **iOS** workflow starts by itself (or: the **Actions** tab > **iOS** >
   **Run workflow**).
2. Wait for it to finish (about 5-10 minutes; green tick). Open the run and download the artifact **ACWW-iOS** at the
   bottom of its page. It is a `.zip` with **`ACWW.ipa`** inside: unzip it.

Notes:
- GitHub keeps artifacts for 90 days; run the workflow again for a new one.
- macOS minutes are free for public repositories. In a private repository they count against your account's free
  Actions minutes at ten times the rate of Linux minutes.
- No game data is used for the build, and none is ever needed on GitHub. Don't upload your game there.

**Your own icon (optional):** add a picture as `ios/icon.png` to your repository (any size; the middle square of it
is used) and the next build uses it as the app's icon. Without it the app has a plain icon drawn by the build. Only
add pictures you have the right to use, especially if the repository is public.

## 2. Install it on the device

`ACWW.ipa` isn't signed for the App Store. Install it with a sideloading tool, which signs it with your own Apple ID:
for example AltStore, SideStore or Sideloadly (follow the tool's own instructions to install an `.ipa`).

With a free Apple ID:
- iOS 16 and later need **Developer Mode**: Settings > Privacy & Security > Developer Mode (the phone restarts). It
  shows up once a sideloaded app is installed.
- You may need to trust your Apple ID's developer certificate once: Settings > General > VPN & Device Management.
- Apps signed with a free Apple ID stop opening after **7 days** until the tool refreshes (re-signs) them. Your game
  and save file aren't lost when that happens: they are in the app's folder.

## 3. Add your game

1. Open the app once. It says that no game was found, and creates its folder.
2. Open the **Files** app > **On My iPhone** (or **On My iPad**) > **AC:WW**, and copy your `.nds` file there (from
   iCloud Drive, a USB stick, AirDrop, or a Mac/PC: Finder or iTunes file sharing, the device's Files tab).
3. Go back to the app and tap **I've added it - check again**.

If the file isn't the supported dump (another region, revision, or a modified or trimmed file), the app says so and
you can still try it.

**Saves** are written next to the game, in the same folder: `<game name>.sav`, the DS's own save format. The game
writes it when you save in the game. Copy it somewhere safe from time to time. The PC version uses the same file:
put it next to the `.nds` there, with the same name. Deleting the app deletes the folder, and the save with it.

## Playing

**Touch:** the bottom screen is the touch screen (the stylus). The buttons below the screens (upright) or beside them
(sideways) are the DS's: the D-pad (diagonals too), A, B, X, Y, L, R, Start and Select. Several buttons can be held at
once, and a finger can slide from one button to another. Turn the device any way you like.

**Save states:** SAVE stores the game exactly as it is now; LOAD goes back to that moment (it's dimmed until something
is saved). There is one slot, kept in `acww-state.bin` in the app's folder, so it's still there after the app was
closed. A new version of the app can't load an older version's state. A state saved between about 1 and 3 minutes
after starting the app loads only until the app is closed (a message says so). Loading a state doesn't touch the
game's save file.

**MENU** opens the settings: the optional features, each off until you switch it on (the game as on the DS).

**Game controllers** (Xbox, PlayStation, MFi and others that iOS supports): connect one in Settings > Bluetooth. The
on-screen buttons then hide and the screens grow. A touch next to the screens brings the buttons back. The face
buttons are by position, as on the DS (right = A, bottom = B, top = X, left = Y). The left stick also works as the
D-pad, and the triggers as L and R. Back + Start opens the settings (the game pauses while they're open).

**Keyboard** (iPad): the PC's keys (README.md).

The game pauses while the app is in the background and continues when you come back.

## If something goes wrong: the log

The app writes **`acww-log.txt`** into its folder (Files app > On My iPhone > AC:WW): the sound device, the screen
layout, every touch on the bottom screen, the frame rate and the sound queue every 10 seconds, and, if the app
crashes, where (with the function names and every game thread's stack). The previous run's log is kept as
`acww-log-previous.txt`, so after a crash, open the app again and that file has the report. Send these files when
you report a problem.

**`acww-options.txt`** in the same folder sets the runtime's options (the environment variables of the PC version):
one `NAME=value` per line. It is created with a few useful ones commented out (`#`). Close the app fully (swipe it
away) and open it again after changing it.

## What's different from the PC version

- **The picture** is drawn by the CPU renderers at the DS's resolution, then scaled up sharply. There is no
  higher-resolution 3D (the PC's WebGPU renderer) on iOS yet.
- **No DS-to-DS play** (local wireless, the gate): the iOS version doesn't use the local network at all. Online play
  is to go through Wiimmfi (not there yet).
- **The game's own reset** (after a broken wireless connection) closes the app with a message. Open it again.
- There are no environment variables on iOS, so the PC's options (README.md, docs/host.md) keep their defaults.

## Building on a Mac yourself

What the workflow does (Xcode, CMake and Ninja: `brew install cmake ninja`):

```
git clone --depth 1 --branch release-2.32.10 https://github.com/libsdl-org/SDL.git build/SDL2-src
cmake -S build/SDL2-src -B build/SDL2-build -G Ninja -DCMAKE_SYSTEM_NAME=iOS -DCMAKE_OSX_ARCHITECTURES=arm64 \
  -DCMAKE_OSX_DEPLOYMENT_TARGET=14.0 -DCMAKE_BUILD_TYPE=Release -DSDL_SHARED=OFF -DSDL_STATIC=ON -DSDL_TEST=OFF \
  -DCMAKE_INSTALL_PREFIX="$PWD/build/SDL2-ios"
cmake --build build/SDL2-build && cmake --install build/SDL2-build
python3 tools/configure_host.py --os ios --sdl2 "$PWD/build/SDL2-ios"
ninja -f build_host_ios.ninja
python3 tools/ios_check.py build/hostios_usa/acww
python3 tools/ios_package.py build/hostios_usa/acww build/ios/ACWW.ipa --sign-adhoc
```

## How the iOS build works

- `tools/configure_host.py --os ios`: the macOS build's setup (Apple's clang, Mach-O, arm64) with the iPhoneOS SDK,
  `-target arm64-apple-ios<min>`, SDL2 linked statically, and the iOS frontend parts. The renderer is the CPU one
  (no `--wgpu`). `build_host_ios.ninja`, `build/hostios_usa/acww`.
- `modern/host/ios/ios_glue.m`: the app's `main` (UIKit through `SDL_UIKitRunApp`), finding the game in the
  Documents folder (and checking its SHA-1), the safe area, message boxes. The runtime's `main` (main.c) is
  `host_main` there.
- `modern/host/ios/touch_controls.c`: plain C, no SDL. It lays out the screens and the buttons in the safe area,
  maps fingers to buttons and the stylus, and draws the buttons. `modern/host/ios/tests/touch_controls_test.c` checks
  it on any machine (the workflow runs it): 8 devices in both orientations (shape, safe area, no overlaps, touch
  sizes), and the touch rules.
- `modern/host/frontend_sdl.c` (`HOST_IOS`): a full-screen window that turns with the device; the layout recomputed
  each frame; the screens drawn at a whole multiple of the DS's pixels into a render target, then smoothed to their
  size. Fingers and an iPad's pointer go to the touch controls. Controllers hide the buttons. The game pauses in the
  background (iOS doesn't allow drawing there). The frontend runs on the main thread's own stack: main.c switches to
  the game's stack and the game switches back at each frame (`frontend_backend_main_loop`), so UIKit, SDL and Metal
  never run on a coroutine stack.
- Save states (`modern/host/savestate.c`): the game's whole state is memory of the process: the game's variables, the
  runtime's DS (memory, hardware, OS, sound, renderers), the page blocks the game's memory lives in and the game
  threads' stacks. configure_host.py links the game's objects and those runtime objects between
  `modern/link/game_begin.c` and `state_end.c`, and the frontend, SDL and the save states themselves outside them. A
  save copies all of it while the game is held at a frame's end, plus where its coroutine stopped; a load puts it back
  and resumes there. `tools/ios_check.py` checks the ranges in the link map (`build/hostios_usa/acww.map`): every
  variable of those objects inside, nobody else's. The save memory (and its file) is left alone by a load. Each save
  is also written to `acww-state.bin`: the game's blocks come from an arena at a fixed address (plat_posix.c), and a
  load in another launch moves the pointers into the program (it is elsewhere every launch), the ROM image and, if
  its place was taken, the arena.
- `modern/host/early.c`: the DS memory is mapped before the game's static constructors run. ELF has constructor
  priorities for that. Mach-O doesn't (its initialisers run in the link's object order), so configure_host.py links
  `early.o` first on macOS and iOS. `tools/ios_check.py` checks the result in the program: `__mod_init_func` (plain
  or chained fixups) or `__init_offsets`.
- `tools/apple_abi_cc.py`: every game and runtime unit is compiled through it on Apple targets (iOS and macOS).
  Apple's arm64 ABI has the caller extend char/short arguments and the callee extend char/short return values, and
  each side relies on the other. The decompiled game has many calls whose declaration doesn't match the definition
  (an int passed to a u8 or s16 parameter, function pointer casts, the generated shared-call dispatchers). Elsewhere
  the value is cut to its type where it's received, as on the DS; on Apple it would arrive uncut and break paths,
  angles, collision and sound. The tool compiles the unit to LLVM IR, removes those promises (zeroext/signext on a
  function's own parameters and on returned values at calls), then optimises and compiles it as usual. Calls whose
  variadic-ness differs from the definition (Apple passes variadic arguments on the stack) are fixed in the sources
  instead: `FX_Sqrt` in the collision code, `OS_SPrintf`, the Wi-Fi callbacks.
- `tools/ios_package.py`: the app bundle (Info.plist with the Files-app sharing keys and a launch screen, so the app
  isn't letterboxed; icons drawn by the script) and the `.ipa`.
