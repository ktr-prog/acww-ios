// mwcc-flags: -nothumb -O4,p
// I004b: itcm 0x01ffcc60-0x01ffcd50 (3 ARM functions): the vblank work callback, the window-edge animation step and an empty
// function. mwcc 1.2/base, C++, ARM, -O4,p. The two H-blank handlers behind it (0x01ffcd50, 0x01ffceb8) are UNMATCHED, see notes.txt.
#include "types.h"
#include "host_mem.h"
#include "nitro/io_access.h"
#include "proto/autoload_2/unk_02113088.h"
#include "proto/dtcm/unk_027e0434.h"
#include "proto/itcm/unk_01ffcc60.h"
#include "proto/main/unk_02060b7c.h"

extern "C" {
extern s32 sVBlankCount;
extern u16 gMainWaitingFrame;
extern OSThreadQueue gFrameWaitQueue;
extern OSThreadQueue gVBlankQueue;

void VramQueueTex_Run(void);
void HBlank_RunVBlank(void);
}

extern "C" void Sky_HBlankNone(void) {
}

// WIN1H (0x04000042) animation: sMenuWipeLine counts 0..47, sMenuWipeEdge is a 24-entry table walked up and down
extern "C" void MenuScreen_WipeHBlank(void) {
    u32 i = sMenuWipeLine;
    u32 j = (i >= 24) ? 47 - i : i;
    u32 w = sMenuWipeEdge[j];
    vu16 *reg = (vu16 *)0x04000000;
    u32 v = ((w << 8) & 0xff00) | 0xff;
    if (IO_R(&reg[2]) & 2) {
        IO_W(&reg[0x21], v);
    }
    i++;
    if (i >= 48) {
        i -= 48;
    }
    sMenuWipeLine = (u8)i;
}

// vblank work
#ifdef HOST
// (modern/host/frontend.c: a press - a tap, a button - arrived at this screen refresh; already published and sampled
// when this handler runs. The game's turn then starts now instead of at the end of its 2 or 3 refreshes: it reads
// the press at once - quicker than on a DS, where a press waited for the next turn)
extern "C" int host_input_wants_turn(void);
#define HOST_INPUT_WANTS_TURN() host_input_wants_turn()
#else
#define HOST_INPUT_WANTS_TURN() 0
#endif
extern "C" void Main_VBlankCallback(void) {
    sVBlankCount++;
    if (sVBlankCount >= gVBlanksPerFrame || (gMainWaitingFrame != 0 && HOST_INPUT_WANTS_TURN())) {
        if (gMainWaitingFrame != 0) {
            OS_WakeupThread(&gFrameWaitQueue);
            sVBlankCount = 0;
            VramQueueTex_Run();
        }
    }
    HBlank_RunVBlank();
    OS_WakeupThread(&gVBlankQueue);
    *HW_INTR_CHECK_BUF |= 1;  // (DTCM + 0x3ff8): the vblank interrupt was handled
}

