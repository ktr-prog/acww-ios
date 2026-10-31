#ifndef PLAYER_UNK_0200B908_OBJ_H
#define PLAYER_UNK_0200B908_OBJ_H

#include "types.h"

// Emotion table entry (Emotion_GetEntry)
// (src/main/unk_02004558.cpp; also used by src/main/unk_02004558_extra.cpp).

// (the player's view of an NpcEmotionEntry, src/main/unk_020119cc.cpp: phase 0's animation, phase 1's, the intro's
// play mode. On 64-bit hosts a phase holds a pointer and is 0x18 bytes, not 0xc: the fields follow the phases there)
#ifdef HOST64
#include "npc/NpcEmotionFx.h"
struct Unk_0200b908_Obj {
    u32 animId;
    u8 pad_04[sizeof(NpcEmotionPhase) - 4];
    volatile u32 nextAnimId;
    u8 pad_10[sizeof(NpcEmotionPhase) - 4];
    u8 animPlayMode;
};
#else
struct Unk_0200b908_Obj {
    /* 0x00 */ u32 animId;
    /* 0x04 */ u8 pad_04[8];
    /* 0x0c */ volatile u32 nextAnimId;
    /* 0x10 */ u8 pad_10[8];
    /* 0x18 */ u8 animPlayMode;
};
#endif

#endif
