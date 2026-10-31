#include "ds_div.h"
#include "uptr.h"
// mwcc-version: 1.2/base
// mwcc-flags: -str reuse
// TU02 of ov003 (player free functions, 0x02204ce8-0x02212830): 25 unit files merged. Every original file is a namespace
// (the file-local Obj/V3/Rec/Msg typedefs and the extern "C" declarations differ per file); functions are emitted in
// descending address order, each in its own namespace block.
#include "types.h"
#include "sys/StackPad.h"
#include "gfx/VecFx32.h"
#include "game/SegmentHitResult.h"
#include "Unk_020d8c7c.h"
#include "actor/Actor.h"
#include "actor/Character.h"
#include "talk/TalkMsgRequest.h"
#include "talk/TalkWindowState.h"
#include "talk/MsgRequest.h"
#include "player/PlayerActor.h"
#include "actor/NpcActor.h"
#include "game/GroundInfoBase.h"
#include "net/CommManager.h"
#include "field/FishShadow.h"
#include "actor/ActorCollider.h"
#include "game/GroundInfo.h"
#include "game/FxVec3.h"
#include "gfx/AnimFrameCtrl.h"
#include "gfx/TwoLayerAnimModel.h"
#include "item/ItemName.h"
#include "player/FishBobber.h"
#include "player/PlayerActionRequest.h"
#include "player/PlayerData.h"
#include "player/PlayerFaceTexRef.h"
#include "player/Unk_02007694.h"
#include "snd/BgmManager.h"
#include "snd/BgmVolumeMixer.h"
#include "ui/HudController.h"
#include "proto/main/unk_020b4828.h"
#include "proto/main/unk_020039ec.h"
#include "proto/main/unk_0204cc1c.h"
#include "proto/main/unk_02004558.h"
#include "proto/main/unk_02034438.h"
#include "proto/main/unk_0203c23c.h"
#include "proto/main/unk_0203ecec.h"
#include "proto/main/unk_020411dc.h"
#include "gfx/ScreenTransition.h"
#include "proto/main/unk_0205d7d8.h"
#include "proto/main/unk_0205dfa4.h"
#include "proto/main/unk_02063904.h"
#include "proto/main/unk_0206e54c.h"
#include "proto/main/unk_020742f4.h"
#include "proto/main/unk_020815f8.h"
#include "proto/main/unk_020850e0.h"
#include "proto/main/unk_02087e70.h"
#include "proto/main/unk_02089fbc.h"
#include "proto/main/unk_02090268.h"
#include "proto/main/unk_020943dc.h"
#include "proto/main/unk_02094810.h"
#include "proto/main/unk_02097444.h"
#include "proto/main/unk_02097d1c.h"
#include "proto/main/unk_02098e90.h"
#include "proto/main/unk_020b0e60.h"
#include "proto/ov003/unk_ov003_02204ce8.h"
#include "proto/ov003/unk_ov003_022187fc.h"
#include "proto/ov003/unk_ov003_022192b4.h"
#include "proto/ov003/unk_ov003_0221ffb8.h"
#include "proto/ov003/unk_ov003_02225800.h"
#include "proto/ov003/unk_ov003_0222e734.h"
#include "gfx/Mtx43.h"
#include "field/FieldUnitPos.h"

// defined elsewhere (their definitions' types; the generated headers of those files don't fit here, or leave them out:
// include/proto/.excluded)
extern "C" {
void GroundInfo_Destruct(void *obj);
BOOL Ground_IsWaterAt(VecFx32 *pos);
s32 Ground_GetHeightAt(VecFx32 *p, u32 *out, u32 flags0);
BOOL Camera_SetModeDefault(void);
void Camera_SetMode4(void);
BOOL TalkRequest_FinishPlayerMessage(void);
s32 FieldAction_FindDropUnit(u32 kind, Vec2 *p, u8 *out);
s16 Math_Atan2(s32 x, s32 y);
s32 Vec_Equal(VecFx32 *a, VecFx32 *b);
s32 Vec_DistXZ(VecFx32 *a, VecFx32 *b);
void Tree_RequestShake(s32 o, FieldUnitPos pos, s32 mode);
void FieldItemFx_StartStrikeResult(FieldUnitPos pos);
void MTX_MultVec43(const VecFx32 *pSrc, const Mtx43 *pMtx, VecFx32 *pDst);  // (MtxFx43 there: the same layout)
void PendingUnit_ApplyAt(Vec2 *pos, s32 flag);
void PlayerActor_GetFrontPoint(VecFx32 *out, PlayerActor *o);
void PlayerActor_GetFrontUnitCenter(VecFx32 *a, PlayerActor *b);
void PlayerActor_GetHeldItem(u16 *out, void *x);
void PlayerActor_StepTowardPose(PlayerActor *o, s32 a, s32 b, s32 c);
s32 PlayerActor_TurnAngle(s16 *p, s32 target);
}

// calls into other modules (the old extern "C" declarations keep their local signatures; the call compiles like the method call)

// the real message class (main 0x0200e2c0 func/ctor 0x0200e2e0/dtor 0x0200e2d0). The 21 files that use it each saw it with a different layout of the
// payload at +0x0c: those views (hoisted below with their member types) are members of one union.
struct Unk_ov003_02205e58_Pair {
    u8 a, b;
};
struct PlayerBuryItemArgs {
    u16 h;
    u8 b2;
    u8 b3;
    u8 b4;
    u8 pad_05[7];
};
struct PlayerDigUpItemArgs {
    VecFx32Copy v;
    u16 a;
    u8 b;
};
struct Unk_ov003_0220bc84_Pair {
    u8 a, b;
};
struct Unk_ov003_0220c4ac_Rec {
    u8 a, b, c;
};
struct Unk_ov003_0220e970_Rec {
    u8 b0, b1, b2, b3;
};
struct PlayerStowItemArgs {
    s32 st;
    s32 a;
    s32 b;
    s16 h;
    u8 bits;
    u8 pad_0f;
};
struct DoorEnterWork {
    s32 a, b;
    s16 c;
    u8 d, e;
};


namespace ns_02204d90 {


extern "C" {
extern BlockMap *gSceneBlockMap;
BOOL Sky_IsShootingStarVisible();
}


// forward declarations (functions are emitted in descending address order)
}

namespace ns_022052f4 {

struct Unk_ov003_022052f4_V2 {
    s32 x, y;
    Unk_ov003_022052f4_V2() {}
    Unk_ov003_022052f4_V2(const Unk_ov003_022052f4_V2 &o) { x = o.x; y = o.y; }
};

struct Unk_ov003_022052f4_Date {
    u16 a : 7;
    u16 b : 4;
    u16 c : 5;
};


typedef PlayerActor Obj;
typedef VecFx32 V3;
typedef Unk_ov003_022052f4_V2 V2;
typedef Unk_ov003_022052f4_Date Date;

extern "C" {
extern BlockMap *gSceneBlockMap;

void *PlayerActor_GetPlayerData(void *p);
void Clock_GetDateTime(void *p);
s32 PlayerActor_CompareLastPlayDateNow(Obj *o);
s32 Weather_GetCurrent();
s32 PlayerActor_GetTan(Obj *o);
void PlayerActor_SetLastPlayDate(Obj *o, PlayerData *a, void *d);
s32 FieldAction_RequestPitfall(s32 a, V2 *p, s32 k);
s32 Flower_Trample(V2 *p);

}

static inline BOOL Unk_ov003_022052f4_Rng(u16 *p, u32 lo, u32 hi) {
    BOOL r = FALSE;
    if (*p >= lo && *p <= hi) r = TRUE;
    return r;
}

static inline BOOL Unk_ov003_022052f4_RngV(volatile u16 *p, u32 lo, u32 hi) {
    BOOL r = FALSE;
    u32 a = *p;
    u32 b = *p;
    if (b >= lo && a <= hi) r = TRUE;
    return r;
}

static inline BOOL Unk_ov003_02205744_Chk(u16 *p) {
    BOOL f8 = TRUE, f7 = TRUE, f6 = TRUE, f5 = TRUE, f4 = TRUE, f3 = TRUE, f2 = TRUE, f1 = TRUE, f0 = FALSE;
    u32 v = *p;
    if (v >= 0x26 && v <= 0x2a) f0 = TRUE;
    if (!f0) { if (v < 0x5d || v > 0x61) f1 = FALSE; }
    if (!f1) { if (v < 0x2f || v > 0x56) f2 = FALSE; }
    if (!f2) { if (v < 0x57 || v > 0x5b) f3 = FALSE; }
    if (!f3) { if (v < 0x66 || v > 0x68) f4 = FALSE; }
    if (!f4) { if (v != 0x69) f5 = FALSE; }
    if (!f5) { if (v < 0x6a || v > 0x6c) f6 = FALSE; }
    if (!f6) { if (v != 0x6d) f7 = FALSE; }
    if (!f7) { if (v < 0xc8 || v > 0xcf) f8 = FALSE; }
    return f8;
}

// forward declarations (functions are emitted in descending address order)
}

namespace ns_02205c28 {


struct Unk_ov003_02205c90_Bits {
    u32 a : 12;
    u32 f : 16;
    u32 b : 4;
};

static inline void Unk_ov003_02205e58_Set(Unk_ov003_02205e58_Pair *q, u32 a, u32 b) {
    q->a = a;
    q->b = b;
}


typedef PlayerActor Obj;
typedef VecFx32 V3;
typedef PlayerActionRequest Msg;

extern "C" {
extern CommManager *gCommManager;
extern BgmManager *data_021c1b3c;

void PlayerActor_ApproachAngle(void *p, s32 a, s32 b, s32 c, s32 d);

V3 *FishCatch_GetPos(s32 a);
V3 *BottleThrow_GetPos(s32 a);

}

// forward declarations (functions are emitted in descending address order)
}

namespace ns_02206574 {


struct PitfallStruggleWork {
    /* 0x00 */ s32 f0;
    /* 0x04 */ s32 f4;
    /* 0x08 */ s32 f8;
    /* 0x0c */ s32 fc;
    /* 0x10 */ s32 f10;
    /* 0x14 */ u16 f14;
    /* 0x16 */ u16 pad_16;
    /* 0x18 */ s32 f18;
};

enum Unk_ov003_02206a84_Three { Unk_ov003_02206a84_THREE = 3 };


typedef PlayerActor Obj;
typedef VecFx32 V3;
typedef PlayerActionRequest Msg;

extern "C" {
extern CommManager *gCommManager;
extern s16 data_02135f44[];
struct Unk_ov003_02206c04_Keys { u16 a; u16 b; s16 c; };
extern Unk_ov003_02206c04_Keys gPad;

s32 func_01ffcb0c(s32 a, s32 b);
s32 FX_Div(s32 a, s32 b);
s32 VEC_Mag(V3 *v);
void Vec_Sub(V3 *out, V3 *a, V3 *b);

}


// forward declarations (functions are emitted in descending address order)
}

namespace ns_02206e94 {
struct Unk_ov003_02206e94_Bits {
    u32 lo : 12;
    u32 mid : 16;
    u32 hi : 4;
};

struct TripPitfallWork {
    s32 unk_00;
    union {
        s32 unk_04;
        u8 unk_04_b;
    };
    s32 unk_08;
    s32 unk_0c;
    s32 unk_10;
    s16 inputAngle;
    s32 effectHandle;
};


typedef PlayerActor Obj;
typedef VecFx32 V3;
typedef PlayerActionRequest Msg;
typedef TripPitfallWork Rec;


extern "C" {
extern CommManager *gCommManager;

s32 PlayerActor_DecreaseClamped(s32 a, s32 b, s32 c);
s32 Sky_WishOnShootingStar();


}


static inline BOOL Eq2(V3 *p, volatile V3 *v) {
    s32 z = p->z;
    s32 x = p->x;
    return x == v->x && z == v->z;
}

struct Unk_ov003_022072b8_P {
    u8 a, b;
};


static inline BOOL Range(volatile u16 *p, u32 lo, u32 hi) {
    u32 a = *p;
    u32 b = *p;
    BOOL r = FALSE;
    if (b >= lo && a <= hi) r = TRUE;
    return r;
}

// forward declarations (functions are emitted in descending address order)
}

namespace ns_022077c8 {

struct Unk_ov003_022077c8_Bits {
    u32 lo : 12;
    u32 mid : 16;
    u32 hi : 4;
};


struct FaintWork {
    s32 effect;
    u8 faintKind;
    u8 pad_05[3];
    s32 unk_08;
};

typedef PlayerActor Obj;
typedef VecFx32 V3;
typedef Vec2 Pair;
typedef PlayerActionRequest Msg;

extern "C" {
extern CommManager *gCommManager;
extern BgmManager *data_021c1b3c;

s32 TalkRequest_FinishSceneEntry();
void TalkRequest_AddLeaveRoom();
s32 Item_GetFurnitureIndex(u16 *p);
s32 Item_IsFurniture(u16 *p);

}


static inline BOOL Unk_ov003_022078e0_IsTwo(u8 v) {
    if (v == 2) return TRUE;
    return FALSE;
}


// forward declarations (functions are emitted in descending address order)
}

namespace ns_02208108 {

struct Unk_ov003_02208190_Pair {
    s32 a, b;
    Unk_ov003_02208190_Pair() {}
    Unk_ov003_02208190_Pair(const Unk_ov003_02208190_Pair &o) { a = o.a; b = o.b; }
};

struct TreeShakeWork {
    s32 treePosX;
    s32 treePosZ;
    u16 faceAngle;
    u8 shakeFrames;
    u8 unk_0b;
    u8 doRelease;
    u8 unk_0d;
    u8 unk_0e;
    u8 unk_0f;
};

struct Act66Work {
    s32 unk_00;
    s32 unk_04;
    s32 unk_08;
    u16 angle;
    u8 waitFrames;
    u8 answer;
};

// action 0x67's view of the same work area: the facing angle and the hole's position
struct Act67Work {
    u16 angle;
    u8 pad_02[2];
    VecFx32 pos;
};

struct Unk_ov003_02208190_Bits {
    u32 lo : 12;
    u32 mid : 16;
    u32 hi : 4;
};

struct Unk_ov003_02208190_V3D : VecFx32Copy {
    Unk_ov003_02208190_V3D(s32 a, s32 b, s32 c) : VecFx32Copy(a, b, c) {}
    ~Unk_ov003_02208190_V3D() {}
};

struct Unk_ov003_02208190_VV {
    volatile s32 x, y, z;
    Unk_ov003_02208190_VV(s32 a, s32 b, s32 c) { x = a; y = b; z = c; }
};

struct Unk_ov003_02208190_VD : VecFx32Copy {
    Unk_ov003_02208190_VD() {}
    ~Unk_ov003_02208190_VD() {}
};

typedef VecFx32Copy V3;
typedef Unk_ov003_02208190_VV VV;
typedef Unk_ov003_02208190_VD VD;
typedef Unk_ov003_02208190_V3D V3D;
typedef Unk_ov003_02208190_Pair Pair;
typedef TreeShakeWork RecA;
typedef Act66Work RecC;


typedef PlayerActionRequest Msg;
typedef PlayerActor Obj;

extern "C" {
extern CommManager *gCommManager;



}

// forward declarations (functions are emitted in descending address order)
}

namespace ns_02208a58 {
struct Unk_ov003_02208a58_Bits {
    u32 lo : 12;
    u32 mid : 16;
    u32 hi : 4;
};

struct FillHoleWork {
    s32 unk_00;
    VecFx32 holePos;
    s16 faceAngle;
    u8 fillMode;
    u8 pad_13;
};


typedef PlayerActor Obj;
typedef VecFx32 V3;
typedef ns_02208108::Unk_ov003_02208190_Pair Pair;
typedef FillHoleWork Rec;
typedef PlayerActionRequest Msg;

extern "C" {
extern CommManager *gCommManager;
extern BlockMap *gSceneBlockMap;

s32 TalkRequest_AddSlingshot();
void Sky_OnSlingshotFired(s32 h);

}

// forward declarations (functions are emitted in descending address order)
}

namespace ns_022093bc {

// view A of the 0x7d0 record
struct BuryItemWork {
    u16 buryItem;
    s16 faceAngle;
    u8 fillMode;
    u8 holeUnitX;
    u8 holeUnitZ;
    u8 answerOk;
};

// view B
struct DugItemStoreWork {
    u8 digUnitX;
    u8 digUnitZ;
    u8 step;
};

// view C
struct DigUpItemMsgWork {
    VecFx32 digPos;
    u8 talkStep;
};


typedef VecFx32 V3;
typedef Vec2 Pair;
typedef PlayerActionRequest Msg;
typedef PlayerBuryItemArgs Pay;

typedef PlayerActor Obj;

extern "C" {
extern CommManager *gCommManager;

void func_02007c08_dummy();
s32 TalkRequest_AddPlayerMessage();
s32 FieldAction_RequestToolAtPendingForAid(u32 a, s32 b, s32 c, u32 d);

}

static inline BOOL Unk_ov003_022099d0_R(u16 *p, u32 lo, u32 hi) {
    BOOL r = FALSE;
    if (*p >= lo && *p <= hi) r = TRUE;
    return r;
}

// forward declarations (functions are emitted in descending address order)
}

namespace ns_02209d50 {
struct Unk_ov003_02209d50_V3D : VecFx32Copy {
    Unk_ov003_02209d50_V3D() {}
    ~Unk_ov003_02209d50_V3D() {}
};

struct Unk_ov003_02209d50_V2 {
    s32 x, y;
    Unk_ov003_02209d50_V2() {}
    Unk_ov003_02209d50_V2(const Unk_ov003_02209d50_V2 &o) { x = o.x; y = o.y; }
};

struct Unk_ov003_02209d50_Bits {
    u32 lo : 12;
    u32 mid : 16;
    u32 hi : 4;
};

struct DigUpItemWork {
    VecFx32Copy digPos;
    u8 talkStep;
    u8 holeKind;
};

struct DigWork {
    s32 unk_00;
    VecFx32Copy digPos;
};


typedef PlayerActor Obj;
typedef VecFx32Copy V3;
typedef Unk_ov003_02209d50_V2 V2;
typedef Unk_ov003_02209d50_V3D V3D;
typedef Mtx43 Blk;
typedef Unk_ov003_02209d50_Bits Bits;
typedef DigUpItemWork Rec;
typedef DigWork Rec2;
typedef PlayerActionRequest Msg;
typedef PlayerDigUpItemArgs Pay;

extern "C" {
extern CommManager *gCommManager;
extern BlockMap *gSceneBlockMap;

void Collision_AddDigHole(s32 a, s32 b);
BOOL Item_IsFurniture(u16 *p);
s32 Item_GetFurnitureIndex(u16 *p);
void VillagerTrend_OnItemDug(V3 *v);

}

static inline BOOL Unk_ov003_02209fc8_Eq(u16 *p) {
    if (Item_IsFurniture(p)) {
        u16 w = 0xfff1;
        if (Item_GetFurnitureIndex(p) == Item_GetFurnitureIndex(&w)) return TRUE;
        return FALSE;
    }
    if (*p == 0xfff1) return TRUE;
    return FALSE;
}

static inline BOOL Unk_ov003_02209fc8_Rng(u16 *p, u32 lo, u32 hi) {
    BOOL r = FALSE;
    if (*p >= lo && *p <= hi) r = TRUE;
    return r;
}


static inline BOOL Unk_ov003_02209fc8_IsNone(u16 *p) {
    return Unk_ov003_02209fc8_Eq(p);
}
// An object with an (empty) inline constructor and destructor keeps its stack slot although no code touches it;
// the original frame has such an unused u16 slot between loc and the first inline temporary.
struct DigUpItemStackPad { u16 v; DigUpItemStackPad() {} ~DigUpItemStackPad() {} };

// forward declarations (functions are emitted in descending address order)
}

namespace ns_0220a680 {

struct Unk_ov003_0220a684_Pair {
    s32 a, b;
    Unk_ov003_0220a684_Pair() {}
    Unk_ov003_0220a684_Pair(s32 x, s32 y) { a = x; b = y; }
    Unk_ov003_0220a684_Pair(const Unk_ov003_0220a684_Pair &o) { a = o.a; b = o.b; }
};

struct ShovelStrikeWork {
    u8 a, b, c, pad_03;
    VecFx32CopyDtor pos;
    u8 pad_10[0x1c - 0x10];
};

struct PlayerDigArgs {
    u8 id;
    u8 pad_01[3];
    VecFx32CopyDtor pos;
    VecFx32CopyDtor GetPos() { return pos; }
};

struct Unk_ov003_0220a684_B3 {
    u8 a, b, c;
};


typedef PlayerActor Obj;
typedef VecFx32CopyDtor V3;
typedef Unk_ov003_0220a684_Pair Pair;
typedef ShovelStrikeWork Rec;
typedef PlayerDigArgs P3;
typedef Unk_ov003_0220a684_B3 B3;
typedef PlayerActionRequest Msg;

extern "C" {
extern CommManager *gCommManager;
extern BlockMap *gSceneBlockMap;
extern s16 data_02135f44[];

s32 PlayerActor_Decelerate(s32 a, s32 b);
s32 PlayerActor_DecreaseClamped(s32 a, s32 b, s32 c);
s32 func_01ffcb0c(s32 a, s32 b);

s32 Snowball_Break(void *p, s32 a);

}

static inline BOOL Unk_ov003_0220a7fc_Chk(u16 *p) {
    BOOL f9 = TRUE, f8 = TRUE, f7 = TRUE, f6 = TRUE, f5 = TRUE, f4 = TRUE, f3 = TRUE, f2 = TRUE, f1 = FALSE;
    u32 v = *p;
    if (v >= 0x26 && v <= 0x2a) f1 = TRUE;
    if (!f1) {
        if (!(v >= 0x5d && v <= 0x61)) f2 = FALSE;
    }
    if (!f2) {
        if (!(v >= 0x2f && v <= 0x56)) f3 = FALSE;
    }
    if (!f3) {
        if (!(v >= 0x57 && v <= 0x5b)) f4 = FALSE;
    }
    if (!f4) {
        if (!(v >= 0x66 && v <= 0x68)) f5 = FALSE;
    }
    if (!f5) {
        if (v != 0x69) f6 = FALSE;
    }
    if (!f6) {
        if (!(v >= 0x6a && v <= 0x6c)) f7 = FALSE;
    }
    if (!f7) {
        if (v != 0x6d) f8 = FALSE;
    }
    if (!f8) {
        if (!(v >= 0xc8 && v <= 0xcf)) f9 = FALSE;
    }
    return f9;
}

// forward declarations (functions are emitted in descending address order)
}

namespace ns_0220b0f0 {

struct Unk_ov003_0220b0f0_Pair {
    s32 a, b;
    Unk_ov003_0220b0f0_Pair() {}
    Unk_ov003_0220b0f0_Pair(const Unk_ov003_0220b0f0_Pair &o) {
        a = o.a;
        b = o.b;
    }
};

struct Unk_ov003_0220b0f0_V3C : VecFx32 {
    Unk_ov003_0220b0f0_V3C() {}
    Unk_ov003_0220b0f0_V3C(const Unk_ov003_0220b0f0_V3C &o) {
        x = o.x;
        y = o.y;
        z = o.z;
    }
};

struct Unk_ov003_0220b0f0_B3 {
    u8 a, b, c;
};

struct Unk_ov003_0220b0f0_Bits {
    u32 lo : 12;
    u32 mid : 16;
    u32 hi : 4;
};

struct ShovelReadyWork {
    VecFx32 targetPos;
    s16 targetAngle;
    u8 hasTarget;
    u8 unk_0f;
};


typedef PlayerActor Obj;
typedef VecFx32 V3;
typedef Unk_ov003_0220b0f0_V3C V3C;
typedef Unk_ov003_0220b0f0_Pair Pair;
typedef ShovelReadyWork Rec;
typedef PlayerActionRequest Msg;
typedef Mtx43 S30;
typedef TalkMsgRequest Sec;

extern "C" {
extern CommManager *gCommManager;

s32 PlayerActor_DecreaseClamped(s32 a, s32 b, s32 c);
void PlayerActor_ApproachAngle(void *p, s32 a, s32 b, s32 c, s32 d);

V3 *HeldInsect_GetPos(u8 id);

}

static inline BOOL Unk_ov003_0220b330_IsZero(s32 v) {
    return v == 0 ? TRUE : FALSE;
}

// forward declarations (functions are emitted in descending address order)
}

namespace ns_0220ba90 {


struct Unk_ov003_0220bc84_Bits {
    u32 a : 12;
    u32 f : 16;
    u32 b : 4;
};

struct InsectShowCatchWork {
    /* 0x0 */ s32 effect;
    /* 0x4 */ u16 unk_04;
    /* 0x6 */ u8 showMode;
    /* 0x7 */ u8 insectSlot;
    /* 0x8 */ u8 insectKind;
    /* 0x9 */ u8 step;
    /* 0xa */ u8 catalogWasFull;
};


typedef PlayerActor Obj;
typedef VecFx32 V3;
typedef Mtx43 T48;
typedef PlayerActionRequest Msg;
typedef InsectShowCatchWork State;

extern "C" {
extern CommManager *gCommManager;

void PlayerActor_ApproachAngle(void *p, s32 a, s32 b, s32 c, s32 d);
s32 TalkRequest_AddPlayerMessage();
void VillagerTrend_OnInsectCaught(void *p);

V3 *HeldInsect_GetPos(u8 a);

}

// forward declarations (functions are emitted in descending address order)
}

namespace ns_0220c448 {

struct Unk_ov003_0220c448_Bits {
    u32 lo : 12;
    u32 mid : 16;
    u32 hi : 4;
};



typedef PlayerActor Obj;
typedef VecFx32Ctor V3;
typedef PlayerActionRequest Msg;
typedef Unk_ov003_0220c4ac_Rec Rec;

extern "C" {
extern CommManager *gCommManager;
extern BlockMap *gSceneBlockMap;
extern s16 data_02135f44[];

s32 PlayerActor_DecreaseClamped(s32 a, s32 b, s32 c);
void CollisionTag_Construct(void *p);
void CollisionTag_Destruct(void *p);
s32 Collision_TestSegment(void *p, V3 *a, V3 *b, s32 c);
void Flower_PlayTrampleFx(void *p);
s32 Math_AngleDiffAbs(s32 a, s32 b);
s32 func_01ffcb0c(s32 a, s32 b);


}

// forward declarations (functions are emitted in descending address order)
}

namespace ns_0220cd4c {

struct Unk_ov003_0220cd4c_Bits {
    u32 lo : 12;
    u32 mid : 16;
    u32 hi : 4;
};

struct BugNetSwingWork {
    u8 swingHit;
    u8 pad_01;
    s16 targetAngle;
    u8 caughtInsect;
};


typedef PlayerActor Obj;
typedef VecFx32Ctor V3;
typedef Mtx43 Blk;
typedef PlayerActionRequest Msg;
typedef FishShadow Act;

extern "C" {
extern CommManager *gCommManager;
// 0x022349e6 is byte 0x12 of the record table sFishSizeClassParams (unit unk_ov003_0221ffb8)
extern u8 sFishSizeClassParams[];
#define data_ov003_022349e6 (sFishSizeClassParams + 0x12)

s32 Math_AngleDiffAbs(s32 a, s32 b);
void FishCatch_SetDisplayPosScale(Act *a, V3 *b, V3 *c);
void Fish_GetDisplayScale(V3 *v, s32 a);
s32 FieldAction_RequestDropForAid(u32 a, u32 b);

}

static inline BOOL Rng(volatile u16 *p, u32 lo, u32 hi) {
    BOOL r = FALSE;
    u32 b = *p;
    u32 a = *p;
    if (a >= lo && b <= hi) r = TRUE;
    return r;
}

// forward declarations (functions are emitted in descending address order)
}

namespace ns_0220d6f4 {

struct Unk_ov003_0220d6f4_Bits {
    u32 lo : 12;
    u32 mid : 16;
    u32 hi : 4;
};

struct FishShowCatchWork {
    s16 unk_00;
    u8 step;
    u8 fishId;
    u8 showMode;
    u8 catalogWasFull;
};


typedef PlayerActor Obj;
typedef VecFx32Ctor V3;
typedef Mtx43 Blk;
typedef PlayerActionRequest Msg;
typedef FishShadow Act;
typedef TalkMsgRequest Sec;
typedef FishShowCatchWork Rec;

extern "C" {
extern CommManager *gCommManager;

void Fish_GetDisplayScale(V3 *v, s32 a);
void FishCatch_SetDisplayPosScale(Act *a, V3 *b, V3 *c);
s32 TalkRequest_AddPlayerMessage();
s32 FieldAction_RequestDropForAid(u32 a, u32 b);
void VillagerTrend_OnFishCaught(void *p);
s32 PlayerActor_DecreaseClamped(s32 a, s32 b, s32 c);


}

static inline BOOL Rng(volatile u16 *p, u32 lo, u32 hi) {
    BOOL r = FALSE;
    u32 a = *p;
    u32 b = *p;
    if (a >= lo && b <= hi) r = TRUE;
    return r;
}

// forward declarations (functions are emitted in descending address order)
}

namespace ns_0220e030 {


typedef PlayerActor Obj;
typedef VecFx32Ctor V3;
typedef PlayerActionRequest Msg;
typedef VecXZ P2;

extern "C" {
extern CommManager *gCommManager;
extern BgmManager *data_021c1b3c;

void FishShadow_OnRodPulled(FishShadow *o);  // (src/ov003/unk_ov003_0221ffb8.cpp)
BOOL TouchPick_GetGroundPos(TouchPickResult *r, VecFx32 *out);
s32 PlayerActor_DecreaseClamped(s32 a, s32 b, s32 c);
s32 Camera_SetMode5();

}

// forward declarations (functions are emitted in descending address order)
}

namespace ns_0220e970 {

struct Unk_ov003_0220e970_Pv {
    s32 x, z;
    Unk_ov003_0220e970_Pv() {}
    Unk_ov003_0220e970_Pv(s32 a, s32 b) {
        x = a;
        z = b;
    }
    Unk_ov003_0220e970_Pv(const Unk_ov003_0220e970_Pv &o) {
        x = o.x;
        z = o.z;
    }
};

struct Unk_ov003_0220e970_Bits {
    u32 lo : 12;
    u32 mid : 16;
    u32 hi : 4;
};


typedef PlayerActor Obj;
typedef VecFx32 V3;
typedef PlayerActionRequest Msg;
typedef VecXZ P2;
typedef Unk_ov003_0220e970_Pv Pv;
typedef Unk_ov003_0220e970_Rec Rec;
typedef Mtx43 Mtx;
typedef TalkMsgRequest Sec;

extern "C" {
extern CommManager *gCommManager;
extern s16 data_02135f44[];

s32 TalkRequest_AddPlayerMessage();
s32 func_01ffcb0c(s32 a, s32 b);
s32 PlayerActor_Decelerate(s32 a, s32 b);
s32 Tree_RequestChop(u32 a, P2 *p, s32 b);
s32 Snowball_Break(void *q, s32 a);

}

// forward declarations (functions are emitted in descending address order)
}

namespace ns_0220f314 {

struct PlayerAxeSwingArgs {
    s32 x, z;
    u8 flag;
};


struct AxeSwingWork {
    VecFx32Ctor v;
    s16 h;
};


typedef PlayerActor Obj;
typedef VecFx32Ctor V3;
typedef PlayerActionRequest Msg;
typedef PlayerAxeSwingArgs P2;
typedef Vec2 Pair;

extern "C" {
extern CommManager *gCommManager;

s32 PlayerActor_Decelerate(s32 a, s32 b);
s32 PlayerActor_DecreaseClamped(s32 a, s32 b, s32 c);

}

static inline BOOL Unk_ov003_0220fa70_Ge(s32 v) {
    if (v >= 0x1000) {
        return TRUE;
    }
    return FALSE;
}

// forward declarations (functions are emitted in descending address order)
}

namespace ns_0220fc88 {

struct Unk_ov003_0220fc88_Bits {
    u32 lo : 12;
    u32 mid : 16;
    u32 hi : 4;
};

struct Unk_ov003_0220fc88_Bf {
    u8 a : 2;
    u8 b : 2;
    u8 c : 4;
};

struct StowItemWork {
    s32 st;
    s32 a;
    s32 b;
    s32 timer;
    s16 h;
    u8 bits;
    u8 pad_13;
};

struct Unk_ov003_0220fc88_H {
    u16 a;
    u16 b;
};


typedef PlayerActor Obj;
typedef VecFx32Ctor V3;
typedef PlayerStowItemArgs RecA;
typedef StowItemWork RecB;
typedef PlayerActionRequest Msg;
typedef Unk_ov003_0220fc88_H H;

extern "C" {
extern CommManager *gCommManager;

s32 TalkRequest_FinishSceneEntry();
void *PlayerActor_GetPlayerData(void *p);

}

// forward declarations (functions are emitted in descending address order)
}

namespace ns_022105bc {
struct Unk_ov003_022105bc_Bits {
    u32 lo : 12;
    u32 mid : 16;
    u32 hi : 4;
};

struct Unk_ov003_02210bdc_Rec {
    u8 a;
    u8 b;
};


typedef PlayerActor Obj;
typedef Unk_ov003_022105bc_Bits Bits;
typedef PlayerActionRequest Msg;
typedef PlayerStowItemArgs Pay;
typedef ns_0220fc88::StowItemWork Rec;
typedef Unk_ov003_02210bdc_Rec Rec2;
typedef DoorEnterWork Rec3;

extern "C" {
extern CommManager *gCommManager;

s32 Item_GetFurnitureIndex(u16 *p);
s32 Item_IsFurniture(u16 *p);
s32 TalkRequest_FinishSceneEntry();
s32 TownBbs_UpdateDaily();
s32 Scene_GetCurrent();
s32 Scene_GetPrevious();
s32 SceneId_IsHouseRoom(s32 a);

}


static inline BOOL Unk_ov003_022107fc_Eq(u16 *p, u16 *k) {
    if (Item_IsFurniture(p)) {
        *k = 0xfff1;
        s32 a = Item_GetFurnitureIndex(p);
        if (a == Item_GetFurnitureIndex(k)) return TRUE;
        return FALSE;
    }
    if (*p == 0xfff1) return TRUE;
    return FALSE;
}

// forward declarations (functions are emitted in descending address order)
}

namespace ns_02210ef0 {

struct Unk_ov003_02210ef0_Bits {
    u32 lo : 12;
    u32 mid : 16;
    u32 hi : 4;
};

struct Unk_ov003_02210ef0_Rec2 {
    u8 a, b;
};


typedef PlayerActor Obj;
typedef VecFx32Ctor V3;
typedef PlayerActionRequest Msg;
typedef DoorEnterWork Rec;
typedef Vec2 Pair;

extern "C" {
extern CommManager *gCommManager;
extern BlockMap *gSceneBlockMap;

void VEC_Add(V3 *a, V3 *b, V3 *c);
s32 Flower_SpawnPetalFxAt(u16 *a, Pair *b, s32 c, s32 d);

}

// forward declarations (functions are emitted in descending address order)
}

namespace ns_02211818 {

struct Unk_ov003_02211818_Bits {
    u32 lo : 12;
    u32 mid : 16;
    u32 hi : 4;
};

struct ReleaseCreatureWork {
    u16 item;
    u8 pad_02[2];
    s32 effect;
    void *fishDisplay;
    u8 isFish;
    u8 step;
};


typedef PlayerActor Obj;
typedef VecFx32Ctor V3;
typedef Mtx43 Blk;
typedef PlayerActionRequest Msg;
typedef ReleaseCreatureWork Rec;
typedef Vec2 Pair;

struct ReleaseCreatureModelLocals {
    u16 item;
    s16 scaleX, scaleY, scaleZ;
};

extern "C" {
extern CommManager *gCommManager;

void Fish_GetDisplayScale(V3 *v, s32 a);
s32 FishDisplay_SetEntry(void *a, s32 b, V3 *c, V3 *d, s32 e, s32 f, s32 g, s32 h, s32 i, s32 j);
void FishDisplay_Release(void *a);
void *FishDisplay_Acquire();
s32 func_02133150(s32 a, s32 b);
s32 func_02132a4c(s32 a);
float func_02132594(float a, float b);
s32 func_01ffcb0c(s32 a, s32 b);

}

static inline s32 Idx(volatile u16 *p, u32 lo, u32 hi) {
    BOOL r = FALSE;
    u32 b = *p;
    u32 a = *p;
    if (a >= lo && b <= hi) r = TRUE;
    if (r) return b - lo;
    return -1;
}

// forward declarations (functions are emitted in descending address order)
}

namespace ns_02212140 {

struct Unk_ov003_02212140_Bits {
    u32 lo : 12;
    u32 mid : 16;
    u32 hi : 4;
};


struct Unk_ov003_02212140_V3D : VecFx32Ctor {
    Unk_ov003_02212140_V3D() {}
    ~Unk_ov003_02212140_V3D() {}
};

typedef PlayerActor Obj;
typedef VecFx32Ctor V3;
typedef Unk_ov003_02212140_V3D V3D;
typedef Vec2 Pair;

enum Unk_ov003_0221227c_Limit { UNK_ov003_0221227c_5 = 5 };

extern "C" {
extern CommManager *gCommManager;

void TalkRequest_FinishSlingshot();
s32 TalkRequest_IsSaveMenuRunning();
s32 TalkRequest_AddPlayerMessage();
}

static inline u16 Unk_ov003_022125ac_A(u32 x) {
    if (x < 0x38) {
        return x + 0x12e8;
    }
    return 0x12e8;
}

static inline u16 Unk_ov003_022125ac_B(u32 x) {
    if (x < 0x38) {
        return x + 0x12b0;
    }
    return 0x12b0;
}

static inline BOOL Unk_ov003_022125ac_R(u16 *p, u32 lo, u32 hi) {
    BOOL r = FALSE;
    if (*p >= lo && *p <= hi) {
        r = TRUE;
    }
    return r;
}


// forward declarations (functions are emitted in descending address order)
}

// the functions defined in this file, with their definitions' types (the bodies below are in descending address order)
extern "C" s32 PlayerActor_TestSlotFlag9(s32 a);
extern "C" s32 FieldPos_IsSameUnit(VecFx32Ctor * a, VecFx32Ctor * b);
extern "C" s32 PlayerActor_GetDigCountdownAt(VecFx32Ctor * a, s32 b);
extern "C" s32 PlayerActor_GetStrikeCountdownAt(VecFx32Ctor * a, s32 b);
extern "C" s32 PlayerActor_IsLocalAct67HitAt(VecFx32Ctor * a);
extern "C" s32 PlayerActor_IsLocalReleaseWaiting();
extern "C" u16 PlayerActor_GetLocalShownItem();
extern "C" s32 PlayerActor_LocalRequestBuryItem(VecFx32Ctor * p, u16 * b);
extern "C" s32 PlayerActor_LocalReleaseCatch(u8 a);
extern "C" s32 PlayerActor_LocalRequestDoorApproach(s32 * a, s32 * b, s16 * c);
extern "C" s32 PlayerActor_LocalRequestDoorEnter(u32 a, s32 * b, s32 * c, s32 d);
extern "C" s32 PlayerActor_LocalRequestDoorExit();
extern "C" s32 PlayerActor_LocalRequestMailboxOpen(VecFx32Ctor * p);
extern "C" s32 PlayerActor_LocalRequestMailboxWaitOrClose(s32 a);
extern "C" s32 PlayerActor_LocalFaint(s32 a);
extern "C" s32 PlayerActor_LocalRequestAct77From(VecFx32Ctor * p);
extern "C" s32 PlayerActor_LocalBeeSting();
extern "C" s32 PlayerActor_LocalRequestAct11();
extern "C" s32 PlayerActor_LocalRequestThrowBottle();
extern "C" s32 PlayerActor_SetWatchMode(s32 a, s32 b);
extern "C" s32 PlayerActor_LocalEndWatch();
extern "C" s32 PlayerActor_LocalRequestReleaseCreature(u32 a);
extern "C" s32 PlayerActor_ConfirmReleaseCreature();
extern "C" VecFx32 * PlayerActor_GetTrackTarget(u32 a);
extern "C" void PlayerActor_ApplyHoldOffset(Mtx43 * b, VecFx32 * v);
extern "C" void PlayerActor_LocalPlayAnim99();
extern "C" s32 PlayerActor_LocalHoldsNet();
extern "C" s32 PlayerActor_RequestReleaseCreature(PlayerActor * o, u32 a, s32 b, s16 c);
extern "C" void PlayerActor_ReleaseCreatureSetNetItem(u8 * p, s32 a);
extern "C" void PlayerActor_ReleaseCreatureGetNetItem(u8 * p, u16 * out);
extern "C" void PlayerActor_ReleaseCreatureSetNetState(u8 * p, u32 v);
extern "C" u8 PlayerActor_ReleaseCreatureGetNetState(u8 * p);
extern "C" void PlayerActor_SetupReleaseCreature(PlayerActor * o, u8 * m);
extern "C" s32 PlayerActor_NetReleaseCreature(PlayerActor * o, s16 a);
extern "C" void PlayerActor_EndReleaseCreature(PlayerActor * o);
extern "C" void PlayerActor_ReleaseCreatureUpdateModel(PlayerActor * o);
extern "C" void PlayerActor_ReleaseCreatureUpdate(PlayerActor * o);
extern "C" void PlayerActor_MainReleaseCreature(PlayerActor * o);
extern "C" void PlayerActor_Act11SetArgs(u8 * p, u32 v);
extern "C" s32 PlayerActor_RequestAct11(PlayerActor * o, u32 p, s32 a, s16 b);
extern "C" void PlayerActor_SetupAct11(PlayerActor * o, u8 * m);
extern "C" s32 PlayerActor_NetAct11(PlayerActor * o, s16 a);
extern "C" void PlayerActor_Act11Update(PlayerActor * o);
extern "C" void PlayerActor_MainAct11(PlayerActor * o);
extern "C" void PlayerActor_PluckReachSetArgs(u8 * p, Vec2 * s);
extern "C" s32 PlayerActor_RequestPluckReach(PlayerActor * o, Vec2 * p, s32 a, s16 b);
extern "C" void PlayerActor_PluckReachSetNetData(u8 * p, Vec2 * s);
extern "C" void PlayerActor_PluckReachGetNetData(u8 * p, Vec2 * o);
extern "C" void PlayerActor_SetupPluckReach(PlayerActor * o, u8 * m);
extern "C" void PlayerActor_NetPluckReach(PlayerActor * o, s32 x);
extern "C" void PlayerActor_PluckReachUpdate(PlayerActor * o);
extern "C" void PlayerActor_PluckReachCheckEnd(PlayerActor * o);
extern "C" void PlayerActor_MainPluckReach(PlayerActor * o);
extern "C" s32 PlayerActor_RequestPluck(PlayerActor * o, s32 * p, s32 a, s32 b);
extern "C" void PlayerActor_SetupPluck(PlayerActor * o, PlayerActionRequest * m);
extern "C" void PlayerActor_NetPluck();
extern "C" void PlayerActor_EndPluck(PlayerActor * o);
extern "C" void PlayerActor_PluckUpdate(PlayerActor * o);
extern "C" void PlayerActor_PluckApply(PlayerActor * o);
extern "C" void PlayerActor_PluckCheckEnd(PlayerActor * o);
extern "C" void PlayerActor_MainPluck(PlayerActor * o);
extern "C" void PlayerActor_DoorApproachSetArgs(DoorEnterWork * p, s32 a, s32 b, s16 c);
extern "C" s32 PlayerActor_RequestDoorApproach(PlayerActor * o, s32 a, s32 b, s16 c, s32 d, s32 e);
extern "C" void PlayerActor_DoorApproachSetWork(DoorEnterWork * p, s32 a, s32 b, s16 c);
extern "C" void PlayerActor_DoorApproachSetNetData(u8 * p, s32 a, s32 b, s16 c);
extern "C" void PlayerActor_DoorApproachGetNetData(u8 * p, s32 * a, s32 * b, s16 * c);
extern "C" void PlayerActor_SetupDoorApproach(PlayerActor * o, PlayerActionRequest * m);
extern "C" void PlayerActor_NetDoorApproach(PlayerActor * o, s32 x);
extern "C" void PlayerActor_DoorApproachMove(PlayerActor * o);
extern "C" void PlayerActor_DoorApproachUpdate(PlayerActor * o);
extern "C" void PlayerActor_DoorApproachCheckEnd(PlayerActor * o);
extern "C" void PlayerActor_MainDoorApproach(PlayerActor * o);
extern "C" void PlayerActor_DoorEnterSetArgs(DoorEnterWork * p, u32 a, s32 b, s32 c, s16 d);
extern "C" s32 PlayerActor_RequestDoorEnter(PlayerActor * o, u8 a, s32 b, s32 c, s16 d, s32 e, s32 f);
extern "C" void PlayerActor_DoorEnterSetWork(DoorEnterWork * p, u32 a, s32 b, s32 c, s16 d, u8 e);
extern "C" void PlayerActor_DoorEnterSetNetData(u8 * p, u32 a, s32 b, s32 c, s16 e);
extern "C" void PlayerActor_DoorEnterGetNetData(u8 * p, u8 * a, s32 * b, s32 * c, u16 * e);
extern "C" void PlayerActor_SetupDoorEnter(PlayerActor * o, PlayerActionRequest * m);
extern "C" void PlayerActor_NetDoorEnter(PlayerActor * o, s32 x);
extern "C" void PlayerActor_DoorEnterMove(PlayerActor * o);
extern "C" void PlayerActor_DoorEnterUpdate(PlayerActor * o);
extern "C" void PlayerActor_DoorEnterCheckEnd(PlayerActor * o);
extern "C" void PlayerActor_MainDoorEnter(PlayerActor * o);
extern "C" s32 PlayerActor_RequestDoorEntered(PlayerActor * o, s32 a, s32 b);
extern "C" void PlayerActor_SetupDoorEntered(PlayerActor * o);
extern "C" s32 PlayerActor_NetDoorEntered(PlayerActor * o, s32 a);
extern "C" void PlayerActor_MainDoorEntered();
extern "C" s32 PlayerActor_RequestDoorExit(PlayerActor * o, s32 a, s32 b);
extern "C" s32 PlayerActor_RequestDoorExitWith(PlayerActor * o, u32 a, u32 b, s32 c, s16 d);
extern "C" void PlayerActor_DoorExitSetNetData(u8 * p, u32 a, u32 b);
extern "C" void PlayerActor_DoorExitGetNetData(u8 * p, u8 * a, u8 * b);
extern "C" s32 PlayerActor_SetupDoorExit(PlayerActor * o, PlayerActionRequest * m);
extern "C" void PlayerActor_NetDoorExit(PlayerActor * o, s32 a);
extern "C" void PlayerActor_DoorExitUpdate(PlayerActor * o);
extern "C" void PlayerActor_DoorExitCheckEnd(PlayerActor * o);
extern "C" void PlayerActor_MainDoorExit(PlayerActor * o);
extern "C" s32 PlayerActor_RequestAct3C(PlayerActor * o, s32 a, s32 b);
extern "C" void PlayerActor_SetupAct3C(PlayerActor * o);
extern "C" s32 PlayerActor_NetAct3C(PlayerActor * o, s32 a);
extern "C" void PlayerActor_EndAct3C(PlayerActor * o, s32 a);
extern "C" void PlayerActor_MainAct3C();
extern "C" void PlayerActor_StowItemSetArgs(PlayerStowItemArgs * p, s32 a, u32 b, s32 c, s32 d, s16 e);
extern "C" s32 PlayerActor_RequestStowItem(PlayerActor * o, s32 a, s32 b, s32 c, s32 d, s16 e, s32 f, s16 g);
extern "C" void PlayerActor_StowItemSetWork(ns_0220fc88::StowItemWork * p, s32 a, u32 b, s32 c, s32 d, s16 e, s32 f);
extern "C" void PlayerActor_StowItemSetNetData(u8 * p, u32 a, u32 b, u32 c, s16 d);
extern "C" void PlayerActor_StowItemGetNetData(u8 * p, u8 * a, s32 * b, s32 * c, s16 * d);
extern "C" void PlayerActor_SetupStowItem(PlayerActor * o, PlayerActionRequest * a);
extern "C" s32 PlayerActor_NetStowItem(PlayerActor * o, s32 a);
extern "C" void PlayerActor_EndStowItem(PlayerActor * o);
extern "C" void PlayerActor_StowItemUpdate(PlayerActor * o);
extern "C" void PlayerActor_MainStowItem(PlayerActor * o);
extern "C" void PlayerActor_StowUmbrellaSetArgs(PlayerStowItemArgs * p, s32 st, u32 bits, s32 x, s32 y, s32 h);
extern "C" s32 PlayerActor_RequestStowUmbrella(PlayerActor * o, s32 id, u8 low, s32 x, s32 y, s16 h, s32 a, s32 b);
extern "C" void PlayerActor_StowUmbrellaSetWork(PlayerStowItemArgs * p, s32 st, u32 bits, s32 x, s32 y, s32 h);
extern "C" void PlayerActor_StowUmbrellaSetNetData(u8 * p, u32 id, s32 x, s32 y, s32 h);
extern "C" void PlayerActor_StowUmbrellaGetNetData(u8 * p, u8 * a, s32 * b, s32 * c, s16 * d);
extern "C" void PlayerActor_SetupStowUmbrella(PlayerActor * o, PlayerActionRequest * a);
extern "C" s32 PlayerActor_NetStowUmbrella(PlayerActor * o, s32 a);
extern "C" void PlayerActor_StowUmbrellaUpdate(PlayerActor * o);
extern "C" void PlayerActor_StowUmbrellaCheckEnd(PlayerActor * o);
extern "C" void PlayerActor_MainStowUmbrella(PlayerActor * o);
extern "C" s32 PlayerActor_RequestUmbrellaSpin(PlayerActor * o, s32 a, s16 b);
extern "C" void PlayerActor_UmbrellaSpinInitWork(u8 * p);
extern "C" void PlayerActor_SetupUmbrellaSpin(PlayerActor * o);
extern "C" s32 PlayerActor_NetUmbrellaSpin(PlayerActor * o, s16 b);
extern "C" void PlayerActor_EndUmbrellaSpin(PlayerActor * o);
extern "C" void PlayerActor_UmbrellaSpinEffect(PlayerActor * o);
extern "C" void PlayerActor_UmbrellaSpinCheckEnd(PlayerActor * o);
extern "C" void PlayerActor_MainUmbrellaSpin(PlayerActor * o);
extern "C" void PlayerActor_AxeSwingSetArgs(s32 * p, s32 a, s32 b, u32 c);
extern "C" s32 PlayerActor_RequestAxeSwing(PlayerActor * o, s32 a, s16 b);
extern "C" s32 PlayerActor_RequestAxeSwingAt(PlayerActor * o, s32 * a, s32 * b, u8 * c, s32 d, s32 e);
extern "C" void PlayerActor_AxeSwingSetWork(u8 * d, VecFx32Ctor * v, s32 a);
extern "C" void PlayerActor_AxeSwingSetNetData(u8 * p, s32 a, s32 b, u8 c);
extern "C" void PlayerActor_AxeSwingGetNetData(u8 * p, s32 * a, s32 * b, u8 * c);
extern "C" void PlayerActor_SetupAxeSwing(PlayerActor * o, PlayerActionRequest * a);
extern "C" s32 PlayerActor_NetAxeSwing(PlayerActor * o, s32 a);
extern "C" void PlayerActor_AxeSwingTurn(PlayerActor * o);
extern "C" void PlayerActor_AxeSwingTrackTarget(PlayerActor * o);
extern "C" void PlayerActor_AxeSwingCheckEnd(PlayerActor * o);
extern "C" void PlayerActor_AxeSwingCheckEndRemote(PlayerActor * o);
extern "C" void PlayerActor_MainAxeSwing(PlayerActor * o);
extern "C" s32 PlayerActor_RequestAxeFollowThrough(PlayerActor * o, s32 a, s16 b);
extern "C" void PlayerActor_SetupAxeFollowThrough(PlayerActor * o);
extern "C" s32 PlayerActor_NetAxeFollowThrough(PlayerActor * o, s16 b);
extern "C" void PlayerActor_AxeFollowThroughTrackTarget(PlayerActor * o);
extern "C" s32 PlayerActor_AxeDispatch(PlayerActor * o, u8 a, ...);
extern "C" void PlayerActor_MainAxeFollowThrough(PlayerActor * o);
extern "C" s32 PlayerActor_RequestAct48(PlayerActor * o, s32 a, s16 b);
extern "C" void PlayerActor_SetupAct48(PlayerActor * o);
extern "C" s32 PlayerActor_NetAct48(PlayerActor * o, s16 b);
extern "C" void PlayerActor_Act48Update(PlayerActor * o);
extern "C" void PlayerActor_Act48CheckEnd(PlayerActor * o);
extern "C" void PlayerActor_MainAct48(PlayerActor * o);
extern "C" void PlayerActor_AxeStrikeSetArgs(u8 * d, u32 x, u32 y, s32 * s);
extern "C" s32 PlayerActor_RequestAxeStrike(PlayerActor * o, u32 a, u32 b, s32 * c, s32 d, s32 e);
extern "C" void PlayerActor_AxeStrikeSetWork(u8 * d, u32 x, u32 y, s32 * s);
extern "C" void PlayerActor_AxeStrikeSetNetData(u8 * d, u8 a, u8 b, u8 c, u8 e, u8 f);
extern "C" void PlayerActor_AxeStrikeGetNetData(u8 * s, u8 * a, u8 * b, u8 * c, u8 * d, u8 * e);
extern "C" void PlayerActor_SetupAxeStrike(PlayerActor * o, PlayerActionRequest * a);
extern "C" void PlayerActor_NetAxeStrike(PlayerActor * o, s32 a);
extern "C" void PlayerActor_EndAxeStrike(PlayerActor * o);
extern "C" void PlayerActor_AxeStrikeUpdate(PlayerActor * o);
extern "C" void PlayerActor_AxeStrikeHit(PlayerActor * o);
extern "C" void PlayerActor_AxeStrikeCheckEnd(PlayerActor * o);
extern "C" void PlayerActor_MainAxeStrike(PlayerActor * o);
extern "C" void PlayerActor_AxeChopSetArgs(Unk_ov003_0220e970_Rec * r, u32 a, u32 b, ns_0220e970::Unk_ov003_0220e970_Pv p);
extern "C" s32 PlayerActor_RequestAxeChop(PlayerActor * o, u32 a, u32 b, ns_0220e970::Unk_ov003_0220e970_Pv p, s32 c, s16 d);
extern "C" void PlayerActor_AxeChopSetWork(Unk_ov003_0220e970_Rec * r, u32 a, u32 b, ns_0220e970::Unk_ov003_0220e970_Pv p);
extern "C" void PlayerActor_SetupAxeChop(PlayerActor * o, PlayerActionRequest * a);
extern "C" void PlayerActor_NetAxeChop(PlayerActor * o);
extern "C" void PlayerActor_EndAxeChop(PlayerActor * o);
extern "C" void PlayerActor_AxeChopUpdate(PlayerActor * o);
extern "C" void PlayerActor_AxeChopHit(PlayerActor * o);
extern "C" void PlayerActor_AxeChopCheckEnd(PlayerActor * o);
extern "C" void PlayerActor_MainAxeChop(PlayerActor * o);
extern "C" void PlayerActor_AxeBreakSetArgs(Unk_ov003_0220e970_Rec * r, u32 c, ns_0220e970::Unk_ov003_0220e970_Pv p);
extern "C" s32 PlayerActor_RequestAxeBreak(PlayerActor * o, u32 a, ns_0220e970::Unk_ov003_0220e970_Pv p, s32 c, s16 e);
extern "C" void PlayerActor_AxeBreakSetWork(Unk_ov003_0220e970_Rec * r, u32 c, ns_0220e970::Unk_ov003_0220e970_Pv p);
extern "C" void PlayerActor_AxeBreakSetNetData(u8 * p, u32 v);
extern "C" void PlayerActor_AxeBreakGetNetData(u8 * src, u8 * dst);
extern "C" void PlayerActor_SetupAxeBreak(PlayerActor * o, PlayerActionRequest * a);
extern "C" void PlayerActor_NetAxeBreak(PlayerActor * o, s16 a);
extern "C" void PlayerActor_EndAxeBreak(PlayerActor * o);
extern "C" void PlayerActor_AxeBreakUpdate(PlayerActor * o);
extern "C" void PlayerActor_AxeBreakCheckEnd(PlayerActor * o);
extern "C" void PlayerActor_MainAxeBreak(PlayerActor * o);
extern "C" s32 PlayerActor_RequestAxeBrokenMessage(PlayerActor * o, s32 a, s16 b);
extern "C" void PlayerActor_AxeBrokenMessageInitWork(Unk_ov003_0220e970_Rec * r);
extern "C" void PlayerActor_SetupAxeBrokenMessage(PlayerActor * o);
extern "C" s32 PlayerActor_NetAxeBrokenMessage(PlayerActor * o, s16 a);
extern "C" void PlayerActor_AxeBrokenMessageUpdate(PlayerActor * o);
extern "C" void PlayerActor_EndAxeBrokenMessage(PlayerActor * o);
extern "C" void PlayerActor_MainAxeBrokenMessage(PlayerActor * o);
extern "C" void PlayerActor_FishCastSetArgs(VecXZ * d, VecFx32Ctor * s);
extern "C" s32 PlayerActor_RequestFishCast(PlayerActor * o, VecFx32 * v, s32 a, s16 b);
extern "C" void PlayerActor_FishCastSetNetData(u8 * p, s32 a, s32 b);
extern "C" void PlayerActor_FishCastGetNetData(u8 * p, s32 * a, s32 * b);
extern "C" void PlayerActor_SetupFishCast(PlayerActor * o, PlayerActionRequest * a);
extern "C" s32 PlayerActor_NetFishCast(PlayerActor * o, s16 a);
extern "C" void PlayerActor_FishCastUpdate(PlayerActor * o);
extern "C" void PlayerActor_FishCastTurn(PlayerActor * o);
extern "C" void PlayerActor_FishCastCheckEnd(PlayerActor * o);
extern "C" void PlayerActor_MainFishCast(PlayerActor * o);
extern "C" s32 PlayerActor_RequestFishCastFail(PlayerActor * o, s32 a, s16 b);
extern "C" void PlayerActor_SetupFishCastFail(PlayerActor * o);
extern "C" s32 PlayerActor_NetFishCastFail(PlayerActor * o, s16 a);
extern "C" void PlayerActor_FishCastFailUpdate(PlayerActor * o);
extern "C" void PlayerActor_FishCastFailCheckEnd(PlayerActor * o);
extern "C" void PlayerActor_MainFishCastFail(PlayerActor * o);
extern "C" s32 PlayerActor_RequestFishWait(PlayerActor * o, s32 a, s16 b);
extern "C" void PlayerActor_FishWaitSetNetData(u8 * p, VecFx32Ctor * v, u8 c);
extern "C" void PlayerActor_FishWaitGetNetData(u8 * p, VecFx32Ctor * v, u8 * out);
extern "C" void PlayerActor_SetupFishWait(PlayerActor * o);
extern "C" void PlayerActor_NetFishWait(PlayerActor * o, s16 a);
extern "C" void PlayerActor_EndFishWait(PlayerActor * o, s32 a);
extern "C" void PlayerActor_FishWaitSyncBobber(PlayerActor * o);
extern "C" void PlayerActor_FishWaitFaceBobber(PlayerActor * o);
extern "C" void PlayerActor_FishWaitOnBobberLand(PlayerActor * o);
extern "C" void PlayerActor_FishWaitCheckInput(PlayerActor * o);
extern "C" void PlayerActor_MainFishWait(PlayerActor * o);
extern "C" s32 PlayerActor_RequestFishHook(PlayerActor * o, s32 a, s16 b);
extern "C" void PlayerActor_SetupFishHook(PlayerActor * o);
extern "C" s32 PlayerActor_NetFishHook(PlayerActor * o, s16 a);
extern "C" void PlayerActor_FishHookCheckResult(PlayerActor * o);
extern "C" void PlayerActor_MainFishHook(PlayerActor * o);
extern "C" void PlayerActor_FishReelInSetArgs(u8 * p, u32 v);
extern "C" s32 PlayerActor_RequestFishReelIn(PlayerActor * o, u32 a, s32 b, s16 c);
extern "C" void PlayerActor_FishReelInSetWork(u8 * p, u32 v);
extern "C" void PlayerActor_FishReelInSetNetData(u8 * p, u32 v);
extern "C" void PlayerActor_FishReelInGetNetData(u8 * p, u8 * out);
extern "C" void PlayerActor_SetupFishReelIn(PlayerActor * o, u8 * p);
extern "C" void PlayerActor_NetFishReelIn(PlayerActor * o, s16 a);
extern "C" void PlayerActor_FishReelInCheckEnd(PlayerActor * o);
extern "C" void PlayerActor_MainFishReelIn(PlayerActor * o);
extern "C" s32 PlayerActor_RequestFishEscape(PlayerActor * o, s32 a, s16 b);
extern "C" void PlayerActor_SetupFishEscape(PlayerActor * o);
extern "C" s32 PlayerActor_NetFishEscape(PlayerActor * o, s16 a);
extern "C" void PlayerActor_FishEscapeCheckEnd(PlayerActor * o);
extern "C" void PlayerActor_MainFishEscape(PlayerActor * o);
extern "C" s32 PlayerActor_RequestFishLand(PlayerActor * o, s32 a, s16 b);
extern "C" void PlayerActor_SetupFishLand(PlayerActor * o);
extern "C" s32 PlayerActor_NetFishLand(PlayerActor * o, s16 a);
extern "C" void PlayerActor_FishLandCheckEnd(PlayerActor * o);
extern "C" void PlayerActor_MainFishLand(PlayerActor * o);
extern "C" s32 PlayerActor_RequestFishShowCatch(PlayerActor * o, u32 a, s32 b, s16 c);
extern "C" void PlayerActor_FishShowCatchSetNetData(u8 * p, u32 v);
extern "C" void PlayerActor_FishShowCatchGetNetData(u8 * p, u8 * out);
extern "C" void PlayerActor_SetupFishShowCatch(PlayerActor * o, u8 * p);
extern "C" void PlayerActor_NetFishShowCatch(PlayerActor * o, s16 a);
extern "C" void PlayerActor_FishShowCatchUpdate(PlayerActor * o);
extern "C" void PlayerActor_MainFishShowCatch(PlayerActor * o);
extern "C" s32 PlayerActor_RequestFishStore(PlayerActor * o, u32 a, s32 b, s32 c);
extern "C" void PlayerActor_FishStoreSetNetState(u8 * p, u32 v);
extern "C" u32 PlayerActor_FishStoreGetNetState(u8 * p);
extern "C" void PlayerActor_SetupFishStore(PlayerActor * o, u8 * b);
extern "C" s32 PlayerActor_NetFishStore(PlayerActor * o, s16 a);
extern "C" void PlayerActor_FishStoreUpdate(PlayerActor * o);
extern "C" void PlayerActor_FishStoreCheckEndRemote(PlayerActor * o);
extern "C" void PlayerActor_MainFishStore(PlayerActor * o);
extern "C" s32 PlayerActor_RequestBugNetSwing(PlayerActor * o, s32 a, s32 b);
extern "C" void PlayerActor_SetupBugNetSwing(PlayerActor * o, u8 * b);
extern "C" s32 PlayerActor_NetBugNetSwing(PlayerActor * o, s16 b);
extern "C" void PlayerActor_BugNetSwingAdvanceAnim(PlayerActor * o);
extern "C" void PlayerActor_BugNetSwingTurn(PlayerActor * o);
extern "C" void PlayerActor_BugNetSwingTrackTarget(PlayerActor * o);
extern "C" void PlayerActor_BugNetSwingUpdate(PlayerActor * o);
extern "C" void PlayerActor_BugNetSwingGetSweep(PlayerActor * o, VecFx32Ctor * a, VecFx32Ctor * b, VecFx32Ctor * out);
extern "C" s32 PlayerActor_BugNetSwingCheckHit(PlayerActor * o, u8 * a, VecFx32Ctor * b, u8 * c);
extern "C" s32 PlayerActor_BugNetSwingCanReach(PlayerActor * o, VecFx32Ctor * a, VecFx32Ctor * b, u8 * c);
extern "C" s32 PlayerActor_BugNetSwingCheckWall(PlayerActor * o);
extern "C" s32 PlayerActor_BugNetSwingCheckGround(PlayerActor * o, VecFx32Ctor * a, VecFx32Ctor * b, VecFx32Ctor * c);
extern "C" void PlayerActor_BugNetSwingUpdateRemote(PlayerActor * o);
extern "C" void PlayerActor_BugNetSwingDecelerate(PlayerActor * o);
extern "C" void PlayerActor_BugNetSwingCheckEnd(PlayerActor * o);
extern "C" void PlayerActor_MainBugNetSwing(PlayerActor * o);
extern "C" s32 PlayerActor_RequestInsectShowCatch(PlayerActor * o, u32 a, u32 b, u32 c, s32 d, s16 e);
extern "C" void PlayerActor_InsectShowCatchSetNetData(u8 * p, u32 a, u32 b, u32 c, u8 d, s16 e);
extern "C" void PlayerActor_InsectShowCatchSetNetState(u8 * p, u32 a);
extern "C" void PlayerActor_InsectShowCatchSetNetAngle(void * p, s32 a);
extern "C" void PlayerActor_InsectShowCatchGetNetData(u8 * p, u8 * a, u8 * b, u8 * c, u8 * d, u16 * e);
extern "C" u32 PlayerActor_InsectShowCatchGetNetState(u8 * p);
extern "C" u32 PlayerActor_InsectShowCatchGetNetAngle(void * p);
extern "C" void PlayerActor_SetupInsectShowCatch(PlayerActor * o, PlayerActionRequest * r);
extern "C" void PlayerActor_NetInsectShowCatch(PlayerActor * o, s32 a);
extern "C" void PlayerActor_EndInsectShowCatch(PlayerActor * o);
extern "C" void PlayerActor_InsectShowCatchUpdate(PlayerActor * o);
extern "C" void PlayerActor_MainInsectShowCatch(PlayerActor * o);
extern "C" s32 PlayerActor_RequestInsectStore(PlayerActor * o, u32 a, u32 b, s32 id, s32 e);
extern "C" void PlayerActor_InsectStoreSetNetData(u8 * p, u32 a, u32 b, u32 c);
extern "C" void PlayerActor_InsectStoreSetNetState(u8 * p, u32 a);
extern "C" void PlayerActor_InsectStoreSetNetAngle(u8 * p, u32 a);
extern "C" void PlayerActor_InsectStoreGetNetData(u8 * p, u8 * a, u8 * b, u16 * c);
extern "C" u32 PlayerActor_InsectStoreGetNetState(u8 * p);
extern "C" u32 PlayerActor_InsectStoreGetNetAngle(u8 * p);
extern "C" void PlayerActor_SetupInsectStore(PlayerActor * o, PlayerActionRequest * r);
extern "C" s32 PlayerActor_NetInsectStore(PlayerActor * o, s32 a);
extern "C" void PlayerActor_EndInsectStore(PlayerActor * o);
extern "C" void PlayerActor_InsectStoreUpdate(PlayerActor * o);
extern "C" void PlayerActor_MainInsectStore(PlayerActor * o);
extern "C" void PlayerActor_ShovelReadySetArgs(u8 * p, u8 a, u8 b, u32 c);
extern "C" s32 PlayerActor_RequestShovelReady(PlayerActor * o, s32 a, s16 b);
extern "C" s32 PlayerActor_RequestShovelReadyAt(PlayerActor * o, u8 * a, u8 * b, u8 * c, s32 id, s32 e);
extern "C" void PlayerActor_ShovelReadySetWork(ns_0220b0f0::ShovelReadyWork * r, ns_0220b0f0::Unk_ov003_0220b0f0_V3C v, s32 c, u32 d);
extern "C" void PlayerActor_ShovelReadySetNetData(u8 * p, u8 a, u8 b, u32 c);
extern "C" void PlayerActor_ShovelReadyGetNetData(u8 * src, u8 * a, u8 * b, u8 * c);
extern "C" void PlayerActor_SetupShovelReady(PlayerActor * o, PlayerActionRequest * r);
extern "C" s32 PlayerActor_NetShovelReady(PlayerActor * o, s32 a);
extern "C" void PlayerActor_ShovelReadyTurn(PlayerActor * o);
extern "C" void PlayerActor_ShovelReadyTrackTarget(PlayerActor * o);
extern "C" void PlayerActor_ShovelReadyCheckEnd(PlayerActor * o);
extern "C" void PlayerActor_ShovelReadyCheckEndRemote(PlayerActor * o);
extern "C" void PlayerActor_MainShovelReady(PlayerActor * o);
extern "C" void PlayerActor_ShovelWaitSetArgs(u8 * d, ns_0220b0f0::Unk_ov003_0220b0f0_Pair v);
extern "C" s32 PlayerActor_RequestShovelWait(PlayerActor * o, ns_0220b0f0::Unk_ov003_0220b0f0_Pair p, s32 id, s16 e);
extern "C" void PlayerActor_ShovelWaitSetWork(u8 * p, ns_0220b0f0::Unk_ov003_0220b0f0_Pair * v);
extern "C" void PlayerActor_ShovelWaitSetNetData(u8 * p, ns_0220b0f0::Unk_ov003_0220b0f0_Pair * v);
extern "C" void PlayerActor_ShovelWaitGetNetData(u8 * p, ns_0220b0f0::Unk_ov003_0220b0f0_Pair * out);
extern "C" void PlayerActor_SetupShovelWait(PlayerActor * o, PlayerActionRequest * r);
extern "C" s32 PlayerActor_NetShovelWait(PlayerActor * o, s16 a);
extern "C" void PlayerActor_ShovelWaitTrackTarget(PlayerActor * o);
extern "C" s32 PlayerActor_ShovelDispatch(PlayerActor * o, VecFx32 * v, u8 f);
extern "C" void PlayerActor_MainShovelWait(PlayerActor * o);
extern "C" s32 PlayerActor_RequestAct5B(PlayerActor * o, u32 a, s16 b);
extern "C" void PlayerActor_SetupAct5B(PlayerActor * o);
extern "C" s32 PlayerActor_NetAct5B(PlayerActor * o, s16 x);
extern "C" void PlayerActor_Act5BUpdate(PlayerActor * o);
extern "C" void PlayerActor_Act5BCheckEnd(PlayerActor * o);
extern "C" void PlayerActor_MainAct5B(PlayerActor * o);
extern "C" s32 PlayerActor_RequestAct5C(PlayerActor * o, u32 a, s16 b);
extern "C" void PlayerActor_SetupAct5C(PlayerActor * o);
extern "C" s32 PlayerActor_NetAct5C(PlayerActor * o, s16 x);
extern "C" void PlayerActor_Act5CCheckEnd(PlayerActor * o);
extern "C" void PlayerActor_MainAct5C(PlayerActor * o);
extern "C" void PlayerActor_ShovelStrikeSetArgs(ns_0220a680::Unk_ov003_0220a684_B3 * d, ns_0220a680::Unk_ov003_0220a684_Pair p, u32 c);
extern "C" s32 PlayerActor_RequestShovelStrike(PlayerActor * o, u32 id, ns_0220a680::Unk_ov003_0220a684_Pair p, s32 b, s32 c);
extern "C" void PlayerActor_ShovelStrikeSetWork(ns_0220a680::ShovelStrikeWork * r, ns_0220a680::Unk_ov003_0220a684_Pair p, u8 c);
extern "C" void PlayerActor_ShovelStrikeSetNetData(u8 * d, u8 a, u8 b, u8 c, u8 e);
extern "C" void PlayerActor_ShovelStrikeGetNetData(u8 * s, u8 * a, u8 * b, u8 * c, u8 * d);
extern "C" void PlayerActor_SetupShovelStrike(PlayerActor * o, PlayerActionRequest * m);
extern "C" void PlayerActor_NetShovelStrike(PlayerActor * o, s32 x);
extern "C" void PlayerActor_EndShovelStrike(PlayerActor * o);
extern "C" void PlayerActor_ShovelStrikeUpdate(PlayerActor * o);
extern "C" void PlayerActor_ShovelStrikeCheckEnd(PlayerActor * o);
extern "C" void PlayerActor_MainShovelStrike(PlayerActor * o);
extern "C" void PlayerActor_DigSetArgs(ns_0220a680::PlayerDigArgs * p, u32 id, VecFx32CopyDtor v);
extern "C" s32 PlayerActor_RequestDig(PlayerActor * o, u32 id, VecFx32CopyDtor v, s32 b, s16 c);
extern "C" void PlayerActor_DigSetWork(ns_0220a680::ShovelStrikeWork * r, u32 id, VecFx32CopyDtor v);
extern "C" void PlayerActor_SetupDig(PlayerActor * o, PlayerActionRequest * m);
extern "C" void PlayerActor_NetDig(PlayerActor * o);
extern "C" void PlayerActor_EndDig(PlayerActor * o);
extern "C" void PlayerActor_DigUpdate(PlayerActor * o);
extern "C" void PlayerActor_DigCheckEnd(PlayerActor * o);
extern "C" void PlayerActor_MainDig(PlayerActor * o);
extern "C" void PlayerActor_DigUpItemSetArgs(PlayerDigUpItemArgs * d, VecFx32Copy v, u16 a, u8 b);
extern "C" s32 PlayerActor_RequestDigUpItem(PlayerActor * o, VecFx32Copy v, u8 x, s32 c, s32 e);
extern "C" s32 PlayerActor_RequestDigUpItemWith(PlayerActor * o, VecFx32Copy v, u16 a, u8 b, s32 c, s32 d);
extern "C" void PlayerActor_DigUpItemSetNetData(u8 * p, ns_02209d50::Unk_ov003_02209d50_V2 v, u16 a, u8 b);
extern "C" void PlayerActor_DigUpItemGetNetData(u8 * p, s32 * xy, u16 * a, u8 * b);
extern "C" void PlayerActor_SetupDigUpItem(PlayerActor * o, PlayerActionRequest * m);
extern "C" void PlayerActor_NetDigUpItem(PlayerActor * o, s32 a);
extern "C" void PlayerActor_EndDigUpItem(PlayerActor * o, s32 p);
extern "C" void PlayerActor_DigUpItemUpdate(PlayerActor * o);
extern "C" void PlayerActor_DigUpItemMessage(PlayerActor * o);
extern "C" void PlayerActor_MainDigUpItem(PlayerActor * o);
extern "C" s32 PlayerActor_RequestDugItemStore(PlayerActor * o, Vec2 * p, u32 c, u32 d, s32 e);
extern "C" void PlayerActor_DugItemStoreSetNetData(u8 * s, u8 a, u8 b, u8 c);
extern "C" void PlayerActor_DugItemStoreGetNetData(u8 * s, u8 * a, u8 * b, u8 * c);
extern "C" void PlayerActor_SetupDugItemStore(PlayerActor * o, PlayerActionRequest * m);
extern "C" void PlayerActor_NetDugItemStore(PlayerActor * o, s32 a);
extern "C" void PlayerActor_DugItemStoreShrink(PlayerActor * o);
extern "C" void PlayerActor_DugItemStoreUpdate(PlayerActor * o);
extern "C" void PlayerActor_MainDugItemStore(PlayerActor * o);
extern "C" s32 PlayerActor_RequestBuryItem(PlayerActor * o, u32 k, Vec2 * p, u32 v, s32 a, s16 b);
extern "C" void PlayerActor_BuryItemSetNetData(u8 * s, u32 v, u8 a, u8 b, u8 c);
extern "C" void PlayerActor_SetupBuryItem(PlayerActor * o, PlayerActionRequest * m);
extern "C" void PlayerActor_NetBuryItem();
extern "C" void PlayerActor_BuryItemTurn(PlayerActor * o);
extern "C" void PlayerActor_BuryItemCheckEnd(PlayerActor * o);
extern "C" void PlayerActor_MainBuryItem(PlayerActor * o);
extern "C" s32 PlayerActor_RequestFillHole(PlayerActor * o, u32 k, Vec2 * p, u32 v, s32 a, s16 b);
extern "C" void PlayerActor_SetupFillHole(PlayerActor * o, PlayerActionRequest * a);
extern "C" void PlayerActor_NetFillHole();
extern "C" void PlayerActor_EndFillHole(PlayerActor * o);
extern "C" void PlayerActor_FillHoleTurn(PlayerActor * o);
extern "C" void PlayerActor_FillHoleUpdate(PlayerActor * o);
extern "C" void PlayerActor_FillHoleCheckEnd(PlayerActor * o);
extern "C" void PlayerActor_MainFillHole(PlayerActor * o);
extern "C" s32 PlayerActor_RequestWateringCan(PlayerActor * o, s32 a, s16 b);
extern "C" void PlayerActor_WateringCanSetNetData(u8 * p, u32 v);
extern "C" void PlayerActor_WateringCanGetNetData(u8 * p, u8 * out);
extern "C" void PlayerActor_SetupWateringCan(PlayerActor * o);
extern "C" s32 PlayerActor_NetWateringCan(PlayerActor * o, s16 b);
extern "C" void PlayerActor_EndWateringCan(PlayerActor * o);
extern "C" void PlayerActor_WateringCanUpdate(PlayerActor * o);
extern "C" void PlayerActor_WateringCanCheckEnd(PlayerActor * o);
extern "C" void PlayerActor_WateringCanCheckEndRemote(PlayerActor * o);
extern "C" void PlayerActor_MainWateringCan(PlayerActor * o);
extern "C" s32 PlayerActor_RequestSlingshot(PlayerActor * o, s32 a, s16 b);
extern "C" void PlayerActor_SetupSlingshot(PlayerActor * o);
extern "C" s32 PlayerActor_NetSlingshot(PlayerActor * o, s16 b);
extern "C" void PlayerActor_SlingshotUpdate(PlayerActor * o);
extern "C" void PlayerActor_SlingshotCheckEnd(PlayerActor * o);
extern "C" void PlayerActor_MainSlingshot(PlayerActor * o);
extern "C" s32 PlayerActor_RequestSlingshotWatch(PlayerActor * o, s32 a, s16 b);
extern "C" void PlayerActor_SetupSlingshotWatch(PlayerActor * o);
extern "C" void PlayerActor_NetSlingshotWatch();
extern "C" void PlayerActor_MainSlingshotWatch(PlayerActor * o);
extern "C" void PlayerActor_Act66SetArgs(u8 * d, ns_02208108::Unk_ov003_02208190_Pair v, u32 c);
extern "C" s32 PlayerActor_RequestAct66(PlayerActor * o, ns_02208108::Unk_ov003_02208190_Pair * p, u32 c, s32 b, s16 d);
extern "C" void PlayerActor_Act66SetWork(ns_02208108::Act66Work * r, u32 a, u32 b, VecFx32 * v);
extern "C" void PlayerActor_Act66SetNetData(u8 * p, ns_02208108::Unk_ov003_02208190_Pair * v);
extern "C" void PlayerActor_Act66GetNetData(u8 * p, ns_02208108::Unk_ov003_02208190_Pair * out);
extern "C" void PlayerActor_SetupAct66(PlayerActor * o, u8 * m);
extern "C" void PlayerActor_NetAct66(PlayerActor * o, s32 a);
extern "C" void PlayerActor_EndAct66(PlayerActor * o, s32 a);
extern "C" void PlayerActor_Act66Turn(PlayerActor * o);
extern "C" void PlayerActor_Act66CheckEnd(PlayerActor * o);
extern "C" void PlayerActor_MainAct66(PlayerActor * o);
extern "C" void PlayerActor_Act67SetArgs(VecFx32Copy * d, VecFx32Copy v);
extern "C" s32 PlayerActor_RequestAct67(PlayerActor * o, VecFx32Copy * v, s32 a, s16 b);
extern "C" void PlayerActor_Act67SetWork(ns_02208108::Act67Work * r, s32 a, VecFx32Copy v);
extern "C" void PlayerActor_SetupAct67(PlayerActor * o, u8 * m);
extern "C" void PlayerActor_NetAct67(PlayerActor * o);
extern "C" void PlayerActor_Act67Effect(PlayerActor * o);
extern "C" void PlayerActor_Act67Turn(PlayerActor * o);
extern "C" void PlayerActor_Act67CheckEnd(PlayerActor * o);
extern "C" void PlayerActor_Act67CheckEndRemote(PlayerActor * o);
extern "C" void PlayerActor_MainAct67(PlayerActor * o);
extern "C" void PlayerActor_TreeShakeSetArgs(void * p, s32 a, s32 b, u8 c, u8 d);
extern "C" s32 PlayerActor_RequestTreeShake(PlayerActor * o, s32 x, s32 z, bool a, bool b, s32 c, s32 d);
extern "C" void PlayerActor_TreeShakeSetWork(ns_02208108::TreeShakeWork * r, s32 a, s32 x, s32 z, u8 b, u8 c);
extern "C" void PlayerActor_TreeShakeSetNetData(u8 * p, s32 a, s32 b, u8 c);
extern "C" void PlayerActor_TreeShakeGetNetData(u8 * p, s32 * a, s32 * b, u8 * c);
extern "C" void PlayerActor_SetupTreeShake(PlayerActor * o, u8 * m);
extern "C" void PlayerActor_NetTreeShake(PlayerActor * o, s32 a);
extern "C" void PlayerActor_EndTreeShake(PlayerActor * o);
extern "C" void PlayerActor_TreeShakeTurn(PlayerActor * o);
extern "C" void PlayerActor_TreeShakeCheckEnd(PlayerActor * o);
extern "C" void PlayerActor_TreeShakeCheckEndRemote(PlayerActor * o);
extern "C" void PlayerActor_MainTreeShake(PlayerActor * o);
extern "C" void PlayerActor_TreeShakeReleaseSetArgs(void * p, s32 a, s32 b, u8 c, u8 d);
extern "C" s32 PlayerActor_RequestTreeShakeRelease(PlayerActor * o, s32 x, s32 z, u8 a, u8 b, s32 c, s16 d);
extern "C" void PlayerActor_TreeShakeReleaseSetWork(void * p, s16 a, s32 b, s32 c, u8 d, u8 e);
extern "C" void PlayerActor_SetupTreeShakeRelease(PlayerActor * o, PlayerActionRequest * m);
extern "C" void PlayerActor_NetTreeShakeRelease();
extern "C" void PlayerActor_TreeShakeReleaseUpdate(PlayerActor * o);
extern "C" void PlayerActor_TreeShakeReleaseTurn(PlayerActor * o);
extern "C" void PlayerActor_TreeShakeReleaseCheckEnd(PlayerActor * o);
extern "C" void PlayerActor_MainTreeShakeRelease(PlayerActor * o);
extern "C" void PlayerActor_MailboxOpenSetArgs(VecFx32 * d, VecFx32 v);
extern "C" s32 PlayerActor_RequestMailboxOpen(PlayerActor * o, VecFx32 v, s32 a, s16 b);
extern "C" void PlayerActor_MailboxOpenSetWork(VecFx32 * d, VecFx32 v);
extern "C" void PlayerActor_MailboxOpenSetNetData(u8 * a, VecFx32 * v);
extern "C" void PlayerActor_MailboxOpenGetNetData(u8 * a, VecFx32 * v);
extern "C" void PlayerActor_SetupMailboxOpen(PlayerActor * o, PlayerActionRequest * m);
extern "C" void PlayerActor_NetMailboxOpen(PlayerActor * o, s16 a);
extern "C" void PlayerActor_MailboxOpenMove(PlayerActor * o);
extern "C" void PlayerActor_MailboxOpenCheckEnd(PlayerActor * o);
extern "C" void PlayerActor_MainMailboxOpen(PlayerActor * o);
extern "C" s32 PlayerActor_RequestMailboxWait(PlayerActor * o, s32 a, s16 b);
extern "C" void PlayerActor_SetupMailboxWait(PlayerActor * o);
extern "C" s32 PlayerActor_NetMailboxWait(PlayerActor * o, s16 a);
extern "C" void PlayerActor_MainMailboxWait(PlayerActor * o);
extern "C" s32 PlayerActor_RequestMailboxClose(PlayerActor * o, s32 a, s16 b);
extern "C" void PlayerActor_SetupMailboxClose(PlayerActor * o);
extern "C" s32 PlayerActor_NetMailboxClose(PlayerActor * o, s16 a);
extern "C" void PlayerActor_MailboxCloseUpdate(PlayerActor * o);
extern "C" void PlayerActor_MainMailboxClose(PlayerActor * o);
extern "C" s32 PlayerActor_RequestFaint(PlayerActor * o, u32 a, s32 b, s16 c);
extern "C" void PlayerActor_SetupFaint(PlayerActor * o, PlayerActionRequest * m);
extern "C" s32 PlayerActor_NetFaint(PlayerActor * o, s16 a);
extern "C" void PlayerActor_FaintEffect(PlayerActor * o);
extern "C" void PlayerActor_FaintWarp(PlayerActor * o);
extern "C" void PlayerActor_MainFaint(PlayerActor * o);
extern "C" s32 PlayerActor_RequestAct6E(PlayerActor * o, s32 a, s16 b);
extern "C" void PlayerActor_SetupAct6E(PlayerActor * o);
extern "C" s32 PlayerActor_NetAct6E(PlayerActor * o, s16 a);
extern "C" void PlayerActor_Act6EUpdateAnim(PlayerActor * o);
extern "C" void PlayerActor_Act6ECheckEnd(PlayerActor * o);
extern "C" void PlayerActor_MainAct6E(PlayerActor * o);
extern "C" s32 PlayerActor_RequestTrip(PlayerActor * o, s32 a, s32 b);
extern "C" void PlayerActor_SetupTrip(PlayerActor * o);
extern "C" s32 PlayerActor_NetTrip(PlayerActor * o, s32 a);
extern "C" void PlayerActor_TripEffects(PlayerActor * o);
extern "C" void PlayerActor_TripCheckEnd(PlayerActor * o);
extern "C" void PlayerActor_TripUpdate(PlayerActor * o);
extern "C" void PlayerActor_MainTrip(PlayerActor * o);
extern "C" s32 PlayerActor_RequestAct72(PlayerActor * o, s32 a, s32 b);
extern "C" void PlayerActor_SetupAct72(PlayerActor * o);
extern "C" s32 PlayerActor_NetAct72(PlayerActor * o, s32 a);
extern "C" void PlayerActor_Act72Decelerate(PlayerActor * o);
extern "C" void PlayerActor_Act72CheckEnd(PlayerActor * o);
extern "C" void PlayerActor_MainAct72(PlayerActor * o);
extern "C" s32 PlayerActor_RequestPitfallFall(PlayerActor * o, s32 * p, s32 b, s32 c);
extern "C" void PlayerActor_PitfallFallSetNetData(u8 * p, u8 a, u8 b);
extern "C" void PlayerActor_PitfallFallGetNetData(u8 * s, u8 * a, u8 * b);
extern "C" void PlayerActor_SetupPitfallFall(PlayerActor * o, u8 * a);
extern "C" s32 PlayerActor_NetPitfallFall(PlayerActor * o, s32 a);
extern "C" void PlayerActor_EndPitfallFall(PlayerActor * o, s32 a);
extern "C" void PlayerActor_PitfallFallEffect(PlayerActor * o);
extern "C" void PlayerActor_PitfallFallUpdate(PlayerActor * o);
extern "C" void PlayerActor_MainPitfallFall(PlayerActor * o);
extern "C" s32 PlayerActor_RequestPitfallStruggle(PlayerActor * o, s32 a, s32 b, s32 c);
extern "C" void PlayerActor_SetupPitfallStruggle(PlayerActor * o, s32 * a);
extern "C" void PlayerActor_NetPitfallStruggle(PlayerActor * o, s32 a);
extern "C" void PlayerActor_EndPitfallStruggle(PlayerActor * o, s32 a);
extern "C" void PlayerActor_PitfallStruggleUpdate(PlayerActor * o);
extern "C" void PlayerActor_PitfallStruggleInput(PlayerActor * o);
extern "C" void PlayerActor_MainPitfallStruggle(PlayerActor * o);
extern "C" s32 PlayerActor_RequestPitfallClimbOut(PlayerActor * o, s32 a, s32 b);
extern "C" void PlayerActor_SetupPitfallClimbOut(PlayerActor * o);
extern "C" s32 PlayerActor_NetPitfallClimbOut(PlayerActor * o, s32 a);
extern "C" void PlayerActor_PitfallClimbOutCheckEnd(PlayerActor * o);
extern "C" void PlayerActor_MainPitfallClimbOut(PlayerActor * o);
extern "C" s32 PlayerActor_RequestBeeSting(PlayerActor * o, s32 a, s32 b);
extern "C" void PlayerActor_SetupBeeSting(PlayerActor * o);
extern "C" void PlayerActor_NetBeeSting();
extern "C" void PlayerActor_BeeStingUpdate(PlayerActor * o);
extern "C" void PlayerActor_MainBeeSting(PlayerActor * o);
extern "C" s32 PlayerActor_RequestAct80(PlayerActor * o, s32 a, s32 b);
extern "C" void PlayerActor_SetupAct80(PlayerActor * o);
extern "C" s32 PlayerActor_NetAct80(PlayerActor * o, s32 a);
extern "C" void PlayerActor_EndAct80(PlayerActor * o);
extern "C" void PlayerActor_Act80Update(PlayerActor * o);
extern "C" void PlayerActor_Act80CheckEnd(PlayerActor * o);
extern "C" void PlayerActor_MainAct80(PlayerActor * o);
extern "C" s32 PlayerActor_RequestFirework(PlayerActor * o, s32 a, s32 b);
extern "C" void PlayerActor_SetupFirework(PlayerActor * o);
extern "C" s32 PlayerActor_NetFirework(PlayerActor * o, s32 a);
extern "C" void PlayerActor_EndFirework(PlayerActor * o);
extern "C" void PlayerActor_FireworkUpdate(PlayerActor * o);
extern "C" void PlayerActor_FireworkCheckEnd(PlayerActor * o);
extern "C" void PlayerActor_MainFirework(PlayerActor * o);
extern "C" s32 PlayerActor_RequestAct82(PlayerActor * o, s32 a, s32 b);
extern "C" void PlayerActor_SetupAct82(PlayerActor * o);
extern "C" s32 PlayerActor_NetAct82(PlayerActor * o, s32 a);
extern "C" void PlayerActor_EndAct82(PlayerActor * o);
extern "C" void PlayerActor_Act82Update(PlayerActor * o);
extern "C" void PlayerActor_Act82CheckEnd(PlayerActor * o);
extern "C" void PlayerActor_MainAct82(PlayerActor * o);
extern "C" s32 PlayerActor_RequestThrowBottle(PlayerActor * o, s32 a, s32 b);
extern "C" void PlayerActor_SetupThrowBottle(PlayerActor * o);
extern "C" s32 PlayerActor_NetThrowBottle(PlayerActor * o, s32 a);
extern "C" void PlayerActor_ThrowBottleShowItem(PlayerActor * o);
extern "C" void PlayerActor_ThrowBottleLook(PlayerActor * o);
extern "C" void PlayerActor_MainThrowBottle(PlayerActor * o);
extern "C" s32 PlayerActor_RequestFishRelease(PlayerActor * o, u8 * p, u32 c, s32 id, s32 e);
extern "C" void PlayerActor_FishReleaseSetNetData(u8 * p, u32 a, u32 b);
extern "C" void PlayerActor_FishReleaseGetNetData(u8 * src, u8 * a, u8 * b);
extern "C" void PlayerActor_SetupFishRelease(PlayerActor * o, PlayerActionRequest * r);
extern "C" s32 PlayerActor_NetFishRelease(PlayerActor * o, s32 a);
extern "C" void PlayerActor_FishReleaseLook(PlayerActor * o);
extern "C" void PlayerActor_MainFishRelease(PlayerActor * o);
extern "C" s32 PlayerActor_ResumeWalkOrIdle(PlayerActor * o);
extern "C" s32 PlayerActor_FieldInteractFront(PlayerActor * o, s32 * pa);
extern "C" s32 PlayerActor_AxeClassifyTarget(PlayerActor * o, u8 * pa, u8 * pb, s32 * out);
extern "C" s32 PlayerActor_ShovelClassifyTarget(PlayerActor * o, VecFx32 * p, u8 * f);
extern "C" s32 PlayerActor_FishFindCastTarget(PlayerActor * o, VecFx32 * out, VecFx32 * tgt);
extern "C" void PlayerActor_FieldCheckUnitAhead(PlayerActor * o);
extern "C" void PlayerActor_FieldRunStep(PlayerActor * o);
extern "C" void PlayerActor_FieldCheckStepUnit(PlayerActor * o);
extern "C" void PlayerActor_FieldPollPitfall(PlayerActor * o);
extern "C" void PlayerActor_FieldUpdateTan(PlayerActor * o);
extern "C" s32 PlayerActor_CheckToolHitActor(PlayerActor * o);
extern "C" s32 PlayerActor_FieldUseTool(PlayerActor * self);
extern "C" s32 PlayerActor_FieldStartToolAction(PlayerActor * self, s32 mode, VecFx32 * pos);
extern "C" s32 PlayerActor_FieldInteractAt(PlayerActor * self, s32 a);

namespace ns_02212140 {
extern "C" BOOL PlayerActor_TestSlotFlag9(s32 a) {
    return PlayerActor_TestSlotFlag(9, a);
}
}

namespace ns_02212140 {
extern "C" s32 FieldPos_IsSameUnit(V3 *a, V3 *b) {
    s32 p[2], q[2];
    p[0] = 0;
    p[1] = 0;
    q[0] = 0;
    q[1] = 0;
    FieldPos_ToUnit(&p[0], &p[1], b);
    FieldPos_ToUnit(&q[0], &q[1], a);
    if (p[0] == q[0] && p[1] == q[1]) {
        return TRUE;
    }
    return FALSE;
}
}

namespace ns_02212140 {
extern "C" s32 PlayerActor_GetDigCountdownAt(V3 *a, s32 b) {
    Obj *o = PlayerActor_Get(b);
    if (o) {
        u32 t = ((Unk_ov003_02212140_Bits *)&o->bodyModel.curFrame)->mid;
        s32 r = 0;
        V3D v;
        v.x = r;
        v.y = r;
        v.z = r;
        if (o->action == 0x5e) {
            s32 *p = (s32 *)(o->actionWorkRaw);
            v.x = p[1];
            v.y = p[2];
            v.z = p[3];
            s32 st = o->animId;
            if (st == 0x4e) {
                r = 0x16 - t;
            } else if (st == 0x4b) {
                r = 9 - t;
            }
        }
        if (r > 0) {
            V3D w;
            w.x = v.x;
            w.y = v.y;
            w.z = v.z;
            if (FieldPos_IsSameUnit(a, &w)) {
                return r;
            }
        }
    }
    return 0;
}
}

namespace ns_02212140 {
extern "C" s32 PlayerActor_GetStrikeCountdownAt(V3 *a, s32 b) {
    Obj *o = PlayerActor_Get(b);
    if (o) {
        u32 t = ((Unk_ov003_02212140_Bits *)&o->bodyModel.curFrame)->mid;
        s32 r = 0;
        V3D v;
        v.x = r;
        v.y = r;
        v.z = r;
        s32 st = o->action;
        if (st == 0x49) {
            u8 *q = o->actionWorkRaw;
            FieldPos_FromUnitCenter(&v, q[0], q[1]);
            r = 8 - t;
        } else if (st == 0x5d) {
            u8 *q = o->actionWorkRaw;
            FieldPos_FromUnitCenter(&v, q[0], q[1]);
            r = 5 - t;
        }
        if (r > 0) {
            V3D w;
            w.x = v.x;
            w.y = v.y;
            w.z = v.z;
            if (FieldPos_IsSameUnit(a, &w)) {
                return r;
            }
        }
    }
    return 0;
}
}

namespace ns_02212140 {
extern "C" s32 PlayerActor_IsLocalAct67HitAt(V3 *a) {
    Obj *o = PlayerActor_Get(4);
    if (o) {
        u32 t = ((Unk_ov003_02212140_Bits *)&o->bodyModel.curFrame)->mid;
        s32 st = o->animId;
        if ((st == 0x67 && t == 5) || (st == 0x44 && t == 8)) {
            V3D v;
            PlayerActor_GetFrontUnitCenter(&v, o);
            return FieldPos_IsSameUnit(a, &v);
        }
    }
    return 0;
}
}

namespace ns_02212140 {
extern "C" s32 PlayerActor_IsLocalReleaseWaiting() {
    Obj *o = PlayerActor_Get(4);
    if (o) {
        if (o->action == 6) {
            // (the release's step: ReleaseCreatureWork::step - 0xd on the DS, after a 64-bit pointer here; as a fixed
            // 0xd it was never 1, and the pocket menu's "Show it off!" waited for it for good)
            if (((ns_02211818::ReleaseCreatureWork *)&o->actionWork)->step == 1) {
                return TRUE;
            }
        }
    }
    return FALSE;
}
}

namespace ns_02212140 {
extern "C" u16 PlayerActor_GetLocalShownItem() {
    Obj *o = PlayerActor_Get(4);
    u16 r = 0xfff1;
    if (o) {
        switch (o->action) {
        case 0x57: {
            u8 *q = o->actionWorkRaw;
            if (q[6] == 0) {
                r = Unk_ov003_022125ac_B(q[8]);
            }
            break;
        }
        case 0x54: {
            u8 *q = o->actionWorkRaw;
            if (q[4] == 0) {
                u32 x = q[3];
                if (x < 0x38) {
                    r = Unk_ov003_022125ac_A(x);
                }
            }
            break;
        }
        case 0x5f:
            if (Unk_ov003_022125ac_R(&o->actionItem, 0x1549, 0x1549)) {
                r = o->actionItem;
            }
            break;
        }
    }
    return r;
}
}

namespace ns_02212140 {
extern "C" s32 PlayerActor_LocalRequestBuryItem(V3 *p, u16 *b) {
    Obj *o = PlayerActor_Get(4);
    if (o) {
        s32 x = 0;
        s32 y = 0;
        FieldPos_ToUnit(&x, &y, p);
        Pair pr;
        u32 t = *b;
        pr.x = x;
        pr.y = y;
        return PlayerActor_RequestBuryItem(o, 1, &pr, t, 6, -1);
    }
    return 0;
}
}

namespace ns_02212140 {
extern "C" s32 PlayerActor_LocalReleaseCatch(u8 a) {
    Obj *o = PlayerActor_Get(4);
    if (o) {
        if (a) {
            return PlayerActor_RequestFishRelease(o, &a, o->action == 5 ? 1 : 0, 6, -1);
        }
        return PlayerActor_RequestInsectShowCatch(o, 1, 0, 0, 6, -1);
    }
    return 0;
}
}

namespace ns_02212140 {
extern "C" s32 PlayerActor_LocalRequestDoorApproach(s32 *a, s32 *b, s16 *c) {
    Obj *o = PlayerActor_Get(4);
    if (o) {
        if ((o)->getHeldHoldableIndex()) {
            return PlayerActor_RequestStowItem(o, 0x38, 0, *a, *b, *c, 6, -1);
        }
        (o)->setActionFlag(1);
        return PlayerActor_RequestDoorApproach(o, *a, *b, *c, 6, -1);
    }
    return 0;
}
}

namespace ns_02212140 {
extern "C" s32 PlayerActor_LocalRequestDoorEnter(u32 a, s32 *b, s32 *c, s32 d) {
    Obj *o = PlayerActor_Get(4);
    if (o) {
        if ((o)->getHeldHoldableIndex()) {
            return PlayerActor_RequestStowItem(o, 0x39, ((u32)a << 26) >> 24, *b, *c, d, 6, -1);
        }
        (o)->setActionFlag(1);
        return PlayerActor_RequestDoorEnter(o, a, *b, *c, d, 6, -1);
    }
    return 0;
}
}

namespace ns_02212140 {
extern "C" s32 PlayerActor_LocalRequestDoorExit() {
    Obj *o = PlayerActor_Get(4);
    if (o) {
        return PlayerActor_RequestDoorExit(o, 6, -1);
    }
    return 0;
}
}

namespace ns_02212140 {
extern "C" s32 PlayerActor_LocalRequestMailboxOpen(V3 *p) {
    Obj *o = PlayerActor_Get(4);
    if (o) {
        return PlayerActor_RequestMailboxOpen(o, *p, 6, -1);
    }
    return 0;
}
}

namespace ns_02212140 {
extern "C" s32 PlayerActor_LocalRequestMailboxWaitOrClose(s32 a) {
    Obj *o = PlayerActor_Get(4);
    if (o) {
        switch (a) {
        case 1:
            return PlayerActor_RequestMailboxWait(o, 6, -1);
        case 2:
            return PlayerActor_RequestMailboxClose(o, 6, -1);
        default:
            return 0;
        }
    }
    return 0;
}
}

namespace ns_02212140 {
extern "C" s32 PlayerActor_LocalFaint(s32 a) {
    Obj *o = PlayerActor_Get(4);
    if (o) {
        Unk_ov003_0221227c_Limit k = UNK_ov003_0221227c_5;
        if (k <= (o)->getRequiredPriority()) {
            return 0;
        }
        if (o->action == 0x78) {
            return 0;
        }
        if (PlayerActor_RequestFaint(o, a + 2, 7, -1)) {
            if ((o)->testActionFlag(0x1a)) {
                (o)->clearActionFlag(0x1a);
                Bgm_Release(0x3f);
            }
            return 1;
        }
    }
    return 0;
}
}

namespace ns_02212140 {
extern "C" s32 PlayerActor_LocalRequestAct77From(V3 *p) {
    Obj *o = PlayerActor_Get(4);
    if (o) {
        return o->requestAct77(Math_Atan2(p->x - o->position.x, p->z - o->position.z), 6, -1);
    }
    return 0;
}
}

namespace ns_02212140 {
extern "C" s32 PlayerActor_LocalBeeSting() {
    if (TalkRequest_IsSaveMenuRunning()) {
        return 0;
    }
    Obj *o = PlayerActor_Get(4);
    if (o) {
        if ((o)->testActionFlag(0xb)) {
            return 0;
        }
        Unk_ov003_0221227c_Limit k = UNK_ov003_0221227c_5;
        if (k <= (o)->getRequiredPriority()) {
            s32 st = o->action;
            if (st != 0x4f) {
                return 0;
            }
            if (st == 0x4f) {
                PlayerActor_RequestFishReelIn(o, 0, 6, -1);
                return 0;
            }
        }
        if (!TalkRequest_AddPlayerMessage()) {
            return 0;
        }
        return PlayerActor_RequestBeeSting(o, 5, -1);
    }
    return 0;
}
}

namespace ns_02212140 {
extern "C" s32 PlayerActor_LocalRequestAct11() {
    Obj *o = PlayerActor_Get(4);
    if (o) {
        o->actionPriority = ((Unk_02007694 *)(void *)(o))->getActionDonePriority(o->action);
        return PlayerActor_RequestAct11(o, 3, 5, -1);
    }
    return 0;
}
}

namespace ns_02212140 {
extern "C" s32 PlayerActor_LocalRequestThrowBottle() {
    Obj *o = PlayerActor_Get(4);
    if (o) {
        if (o->action == 0x89) {
            return 0;
        }
        return PlayerActor_RequestThrowBottle(o, 6, -1);
    }
    return 0;
}
}

namespace ns_02212140 {
extern "C" s32 PlayerActor_SetWatchMode(s32 a, s32 b) {
    Obj *o = PlayerActor_Get(b);
    if (b == 4) {
        b = gCommManager->localSlot;
    }
    if (o) {
        if (gCommManager->isLocalSlot(b)) {
            o->actionPriority = ((Unk_02007694 *)(void *)(o))->getActionDonePriority(o->action);
            switch (a) {
            case 0:
                TalkRequest_FinishSlingshot();
                (o)->requestWait(3, 1, -1);
                break;
            case 1:
                PlayerActor_RequestSlingshotWatch(o, 6, -1);
                break;
            }
        }
        return TRUE;
    }
    return FALSE;
}
}

namespace ns_02212140 {
// ---------------------------------------------------------------- free functions on the big player object
extern "C" s32 PlayerActor_LocalEndWatch() {
    Obj *o = PlayerActor_Get(4);
    if (o) {
        TalkRequest_FinishSlingshot();
        if (o->action != 0x77) {
            o->actionPriority = ((Unk_02007694 *)(void *)(o))->getActionDonePriority(o->action);
            (o)->requestWait(3, 1, -1);
        }
        PlayerActor_SetHeadTilt(0, 0, 4);
        return TRUE;
    }
    return FALSE;
}
}

namespace ns_02211818 {
extern "C" s32 PlayerActor_LocalRequestReleaseCreature(u32 a) {
    Obj *p = PlayerActor_Get(4);
    if (p) {
        return PlayerActor_RequestReleaseCreature(p, a, 5, -1);
    }
    return 0;
}
}

namespace ns_02211818 {
extern "C" BOOL PlayerActor_ConfirmReleaseCreature() {
    Obj *p = PlayerActor_Get(4);
    if (p) {
        if (p->action == 6) {
            Rec *r = (Rec *)&p->actionWork;
            if (r->step == 1) {
                r->step = 2;
                return TRUE;
            }
        }
    }
    return FALSE;
}
}

namespace ns_02211818 {
extern "C" VecFx32 *PlayerActor_GetTrackTarget(u32 a) {
    Obj *p = PlayerActor_Get(a);
    if (p != 0 && (p)->getHeldToolKind() == 3) {
        FishBobber *q = &p->heldItemModel.bobber;
        if (q->curState > 1) return &q->pos;
    }
    return PlayerActor_GetBodyPos(a);
}
}

namespace ns_02211818 {
extern "C" void PlayerActor_ApplyHoldOffset(Blk *b, VecFx32 *v) {
    s32 r7 = b->m[9];
    s32 s0 = b->m[10];
    s32 s4 = b->m[11];
    Blk c;
    V3 vv;
    V3 out;
    b->m[9] = b->m[10] = b->m[11] = 0;
    c = *b;
    if (v == 0) {
        vv.x = 0x4cd;
        vv.y = 0;
        vv.z = 0;
    } else {
        vv.x = v->x;
        vv.y = v->y;
        vv.z = v->z;
    }
    MTX_MultVec43(&vv, &c, &out);
    b->m[9] = out.x + r7;
    b->m[10] = out.y + s0;
    b->m[11] = out.z + s4;
}
}

namespace ns_02211818 {
extern "C" void PlayerActor_LocalPlayAnim99() {
    Obj *o = PlayerActor_Get(4);
    if (o) {
        (o)->startAnimOnce(0x99, 3, 0);
    }
}
}

namespace ns_02211818 {
extern "C" BOOL PlayerActor_LocalHoldsNet() {
    u16 t;
    PlayerActor_GetSlotHeldItem(&t, 4);
    BOOL f = FALSE;
    volatile u16 *p = &t;
    u32 b = *p;
    u32 a = *p;
    if (a >= 0x1376 && b <= 0x1376) f = TRUE;
    if (f || (b >= 0x1377 && b <= 0x1377)) return TRUE;
    return FALSE;
}
}

namespace ns_02211818 {
extern "C" s32 PlayerActor_RequestReleaseCreature(Obj *o, u32 a, s32 b, s16 c) {
    Msg m;
    (&m)->assign(6, b, c);
    *(u16 *)((u8 *)&m.unk_0c) = a;
    s32 r = (o)->pushRequest(&m);
    return r;
}
}

namespace ns_02211818 {
extern "C" void PlayerActor_ReleaseCreatureSetNetItem(u8 *p, s32 a) {
    NetBuf_WriteU16(p, a);
}
}

namespace ns_02211818 {
extern "C" void PlayerActor_ReleaseCreatureGetNetItem(u8 *p, u16 *out) {
    *out = NetBuf_ReadU16(p);
}
}

namespace ns_02211818 {
extern "C" void PlayerActor_ReleaseCreatureSetNetState(u8 *p, u32 v) {
    p[2] = v;
}
}

namespace ns_02211818 {
extern "C" u8 PlayerActor_ReleaseCreatureGetNetState(u8 *p) {
    return p[2];
}
}

namespace ns_02211818 {
extern "C" void PlayerActor_SetupReleaseCreature(Obj *o, u8 *m) {
    Rec *r4 = (Rec *)&o->actionWork;
    volatile u16 t;
    u16 r7 = *(u16 *)(((PlayerActionRequest *)(void *)m)->unk_0c_b);
    u32 id;
    u8 *r6;
    t = r7;
    r4->item = r7;
    r4->effect = -1;
    r4->fishDisplay = 0;
    {
        BOOL z = FALSE;
        volatile u16 *pt = &t;
        u32 b = *pt;
        u32 a = *pt;
        if (a >= 0x12e8 && b <= 0x131f) z = TRUE;
        r4->isFish = z;
    }
    r4->step = 0;
    if (r4->isFish == 0) {
        s32 i = Idx(&t, 0x12b0, 0x12e7);
        HeldInsect_Start(i, o->sessionSlot);
        id = 0x9f;
    } else {
        r4->fishDisplay = FishDisplay_Acquire();
        id = 0x9e;
    }
    (o)->startAnimOnce(id, 3, 3);
    (o)->playSe(0x4f);
    r6 = o->netData.bytes;
    if (gCommManager->isLocalSlot(o->sessionSlot)) {
        PlayerActor_ReleaseCreatureSetNetItem(r6, r7);
        PlayerActor_ReleaseCreatureSetNetState(r6, 0);
    } else if (PlayerActor_ReleaseCreatureGetNetState(r6) >= 1) {
        r4->step = 1;
        *(u32 *)(Unk_ov003_02211818_Bits *)&o->bodyModel.curFrame = ((u32)(((Unk_ov003_02211818_Bits *)&o->bodyModel.numFrames)->mid - 1) << 16) >> 4;
    }
}
}

namespace ns_02211818 {
extern "C" s32 PlayerActor_NetReleaseCreature(Obj *o, s16 a) {
    u16 t;
    PlayerActor_ReleaseCreatureGetNetItem(o->netData.bytes, &t);
    return PlayerActor_RequestReleaseCreature(o, t, 5, a);
}
}

namespace ns_02211818 {
extern "C" void PlayerActor_EndReleaseCreature(Obj *o) {
    Rec *r4 = (Rec *)&o->actionWork;
    if (r4->effect > -1) {
        Effect_End(r4->effect);
    }
    if (r4->isFish == 0) {
        HeldInsect_Remove(o->sessionSlot, 1);
    } else {
        FishDisplay_Release(r4->fishDisplay);
    }
}
}

namespace ns_02211818 {
extern "C" void PlayerActor_ReleaseCreatureUpdateModel(Obj *o) {
    Rec *r6 = (Rec *)&o->actionWork;
    ReleaseCreatureModelLocals t;
    Blk b;
    V3 v50;
    V3 v5c;
    V3 v68;
    s32 *r7;
    s16 sc;
    s32 m;
    t.item = r6->item;
    r7 = &r6->effect;
    (o)->turnToCamera(0x400);
    b = o->itemHandMtx;
    v50.x = 0x4cd;
    v50.y = 0;
    v50.z = 0;
    m = ((Unk_ov003_02211818_Bits *)&o->bodyModel.curFrame)->mid;
    if (o->animId != 0xa0 && (s32)m >= 0x10) {
        s32 q = (m - 15) * 0x19a / 12;
        v50.x = v50.x + q * 2;
        v50.y = v50.y + q;
        v50.z = v50.z - q;
    }
    PlayerActor_ApplyHoldOffset(&b, &v50);
    if (*r7 > -1) {
        v5c.x = b.m[9];
        v5c.y = b.m[10];
        v5c.z = b.m[11];
        WorldCurve_FromCurved(&v5c, &v5c);
        Effect_SetPosition(*r7, (VecFx32 *)&v5c, NULL, NULL);
    } else if (*r7 == -1) {
        *r7 = -2;
        if (r6->isFish == 0) {
            s32 i = Idx(&t.item, 0x12b0, 0x12e7);
            if (i == 0x18 || i == 0x30 || (u32)(i - 0x32) <= 1) {
                v5c.x = b.m[9];
                v5c.y = b.m[10];
                v5c.z = b.m[11];
                WorldCurve_FromCurved(&v5c, &v5c);
                *r7 = Effect_CreateById(0x5a, &v5c, 0, 0);
            }
        }
    }
    if (o->animId != 0xa0) {
        if (m <= 10) {
            sc = 0;
        } else if (m < 0x10) {
            sc = (s16)((m - 10) * 0x11);
        } else {
            sc = 0x64;
        }
    } else {
        if (m < 6) {
            sc = (s16)(0x64 - m * 0x11);
        } else {
            sc = 0;
        }
    }
    if (r6->isFish == 0) {
        t.scaleX = sc;
        t.scaleY = sc;
        t.scaleZ = sc;
        HeldInsect_SetHandMatrix(o->sessionSlot, &t.scaleX, &b, 0);
    } else {
        float f = (float)sc / 100.0f;
        float g;
        if (f > 0) {
            g = 0.5f + 4096.0f * f;
        } else {
            g = 4096.0f * f - 0.5f;
        }
        s32 k = (s32)g;
        s32 i = Idx(&t.item, 0x12e8, 0x131f);
        Fish_GetDisplayScale(&v68, i);
        v68.x = func_01ffcb0c(v68.x, k);
        v68.y = func_01ffcb0c(v68.y, k);
        v68.z = func_01ffcb0c(v68.z, k);
        v5c.x = b.m[9];
        v5c.y = b.m[10];
        v5c.z = b.m[11];
        WorldCurve_FromCurved(&v5c, &v5c);
        i = Idx(&t.item, 0x12e8, 0x131f);
        FishDisplay_SetEntry(r6->fishDisplay, i, &v5c, &v68, 0, 0, 0, 1, 1, 0x1f);
    }
}
}

namespace ns_02211818 {
extern "C" void PlayerActor_ReleaseCreatureUpdate(Obj *o) {
    Rec *r0 = (Rec *)&o->actionWork;
    u8 *r6 = o->netData.bytes;
    u8 *r5 = &r0->step;
    switch (*r5) {
    case 0:
        if (((AnimFrameCtrl *)((AnimFrameCtrl *)&o->bodyModel))->hasPassedFrame(10)) {
            (o)->playSe(0x63);
        } else if (((AnimFrameCtrl *)((AnimFrameCtrl *)&o->bodyModel))->hasPassedFrame(0x19)) {
            (o)->playSe(0x7f2);
        }
        if (((AnimFrameCtrl *)((AnimFrameCtrl *)&o->bodyModel))->isFinished()) {
            *r5 = 1;
            PlayerActor_ReleaseCreatureSetNetState(r6, 1);
        }
        break;
    case 1:
        if (gCommManager->isLocalSlot(o->sessionSlot) == 0) {
            if (PlayerActor_ReleaseCreatureGetNetState(r6) >= 2) {
                *r5 = 2;
            }
        }
        break;
    case 2:
        if (r0->effect > -1) {
            Effect_End(r0->effect);
        }
        if (gCommManager->isLocalSlot(o->sessionSlot)) {
            PlayerActor_ReleaseCreatureSetNetState(r6, 2);
        }
        (o)->startAnimOnce(0xa0, 3, 0);
        (o)->playSe(0x7f1);
        *r5 = 3;
        break;
    case 3:
        if (((AnimFrameCtrl *)((AnimFrameCtrl *)&o->bodyModel))->isFinished()) {
            ((Unk_02007694 *)(void *)(o))->requestAct05(3, 5, -1);
        }
        break;
    }
}
}

namespace ns_02211818 {
extern "C" void PlayerActor_MainReleaseCreature(Obj *o) {
    (o)->advanceAnim();
    volatile V3 v;
    V3 *pv = (V3 *)&o->drawPos;
    v.x = o->drawPos.x;
    v.y = pv->y;
    v.z = pv->z;
    s32 d0 = (s16)o->drawTilt;
    Blk b1 = o->bodyModel.mtx;
    Blk b2 = o->itemHandMtx;
    (o)->calcHandMtx();
    PlayerActor_ReleaseCreatureUpdateModel(o);
    o->drawPos.x = v.x;
    o->drawPos.y = v.y;
    o->drawPos.z = v.z;
    o->drawTilt = d0;
    o->bodyModel.mtx = b1;
    o->itemHandMtx = b2;
    if ((o)->netFollowTransform()) {
        (o)->moveWithCollision();
    }
    (o)->updateBodyCollider();
    PlayerActor_ReleaseCreatureUpdate(o);
}
}

namespace ns_02211818 {
extern "C" void PlayerActor_Act11SetArgs(u8 *p, u32 v) {
    *(u16 *)p = v;
}
}

namespace ns_02211818 {
extern "C" s32 PlayerActor_RequestAct11(Obj *o, u32 p, s32 a, s16 b) {
    Msg m;
    (&m)->assign(0x11, a, b);
    PlayerActor_Act11SetArgs(m.unk_0c_b, p);
    s32 r = (o)->pushRequest(&m);
    return r;
}
}

namespace ns_02211818 {
extern "C" void PlayerActor_SetupAct11(Obj *o, u8 *m) {
    (o)->startAnimOnce(0x9a, *(u16 *)(((PlayerActionRequest *)(void *)m)->unk_0c_b), 0);
}
}

namespace ns_02211818 {
extern "C" s32 PlayerActor_NetAct11(Obj *o, s16 a) {
    return PlayerActor_RequestAct11(o, 3, 5, a);
}
}

namespace ns_02211818 {
extern "C" void PlayerActor_Act11Update(Obj *o) {
    (o)->advanceAnim();
    if (((AnimFrameCtrl *)((AnimFrameCtrl *)&o->bodyModel))->isFinished()) {
        (o)->startAnim(0, 3, 0);
    }
}
}

namespace ns_02211818 {
extern "C" void PlayerActor_MainAct11(Obj *o) {
    PlayerActor_Act11Update(o);
    if (gCommManager->isLocalSlot(o->sessionSlot)) {
        (o)->act10FaceTalkTarget();
    } else {
        (o)->netFollowTransform();
    }
    (o)->updateBodyCollider();
}
}

namespace ns_02211818 {
extern "C" void PlayerActor_PluckReachSetArgs(u8 *p, Pair *s) {
    p[0] = s->x;
    p[1] = s->y;
}
}

namespace ns_02211818 {
extern "C" s32 PlayerActor_RequestPluckReach(Obj *o, Pair *p, s32 a, s16 b) {
    Pair t;
    Msg m;
    (&m)->assign(0x16, a, b);
    t.x = p->x;
    t.y = p->y;
    PlayerActor_PluckReachSetArgs(m.unk_0c_b, &t);
    s32 r = (o)->pushRequest(&m);
    return r;
}
}

namespace ns_02211818 {
extern "C" void PlayerActor_PluckReachSetNetData(u8 *p, Pair *s) {
    p[0] = s->x;
    p[1] = s->y;
}
}

namespace ns_02211818 {
extern "C" void PlayerActor_PluckReachGetNetData(u8 *p, Pair *o) {
    o->x = p[0];
    o->y = p[1];
}
}

namespace ns_02211818 {
extern "C" void PlayerActor_SetupPluckReach(Obj *o, u8 *m) {
    Pair s;
    u8 *q = ((PlayerActionRequest *)(void *)m)->unk_0c_b;
    u32 a = q[0];
    u32 b = q[1];
    u8 *r = (u8 *)&o->actionWork;
    r[0] = a;
    r[1] = b;
    r[2] = 0;
    s.x = a;
    s.y = b;
    PlayerActor_PluckReachSetNetData(o->netData.bytes, &s);
    (o)->startAnim(0x15, 3, 0);
    if ((o)->getHeldToolKind() == 4) {
        HeldItemModel_PlayAnim(&o->heldItemModel, 0xb, 3, 0);
    }
}
}

namespace ns_02210ef0 {
extern "C" void PlayerActor_NetPluckReach(Obj *o, s32 x) {
    if (o->action == 0x16) {
        o->netSeq = x;
    } else {
        Pair p = {0, 0};
        PlayerActor_PluckReachGetNetData(o->netData.bytes, &p);
        Pair q = p;
        PlayerActor_RequestPluckReach(o, &q, 6, x);
    }
}
}

namespace ns_02210ef0 {
extern "C" void PlayerActor_PluckReachUpdate(Obj *o) {
    u8 *r = (u8 *)&o->actionWork;
    if (r[2] == 0) {
        s32 t = o->fieldAnswer;
        if (t == 2) {
            r[2] = 2;
        } else if (t == 1) {
            r[2] = 1;
        }
    }
    (o)->advanceAnim();
}
}

namespace ns_02210ef0 {
extern "C" void PlayerActor_PluckReachCheckEnd(Obj *o) {
    if (gCommManager->isLocalSlot(o->sessionSlot) == 0) {
        o->actionPriority = ((Unk_02007694 *)(void *)(o))->getActionDonePriority(o->action);
    }
    u8 *r = (u8 *)&o->actionWork;
    switch (r[2]) {
    case 0:
        break;
    case 1: {
        s32 y = r[1];
        s32 x = r[0];
        Pair q = {x, y};
        PlayerActor_RequestPluck(o, (s32 *)&q, 6, -1);
        break;
    }
    case 2:
        o->actionPriority = ((Unk_02007694 *)(void *)(o))->getActionDonePriority(o->action);
        (o)->requestWait(3, 1, -1);
        break;
    }
}
}

namespace ns_02210ef0 {
extern "C" void PlayerActor_MainPluckReach(Obj *o) {
    if (gCommManager->isLocalSlot(o->sessionSlot)) {
        PlayerActor_PluckReachUpdate(o);
    } else {
        (o)->advanceAnim();
        u8 *r = (u8 *)&o->actionWork;
        s32 y = r[1];
        s32 x = r[0];
        Pair q = {x, y};
        (o)->netSyncNearUnit((s32 *)(void *)(&q));
    }
    (o)->updateBodyCollider();
    PlayerActor_PluckReachCheckEnd(o);
}
}

namespace ns_02210ef0 {
extern "C" s32 PlayerActor_RequestPluck(Obj *o, s32 *p, s32 a, s32 b) {
    Msg m;
    Unk_ov003_02210ef0_Rec2 &q = *(Unk_ov003_02210ef0_Rec2 *)m.unk_0c_b;
    (&m)->assign(0x17, a, b);
    q.a = p[0];
    q.b = p[1];
    s32 r = (o)->pushRequest(&m);
    return r;
}
}

namespace ns_02210ef0 {
extern "C" void PlayerActor_SetupPluck(Obj *o, Msg *m) {
    u8 *p = m->unk_0c_b;
    V3 v;
    FieldPos_FromUnitCenter(&v, p[0], p[1]);
    V3 *pv = (V3 *)&o->actionWork;
    *pv = v;
    ((u8 *)pv)[12] = 0;
    (o)->startAnimOnce(0x14, 3, 0);
    if ((o)->getHeldToolKind() == 4) {
        HeldItemModel_PlayAnim(&o->heldItemModel, 0xe, 3, 0);
    }
    u16 *g = BlockMap_GetItemPtrAtPos(gSceneBlockMap, &v, 0);
    o->shownItem = *g;
    o->actionItem = o->shownItem;
}
}

namespace ns_02210ef0 {
extern "C" void PlayerActor_NetPluck() {
}
}

namespace ns_02210ef0 {
extern "C" void PlayerActor_EndPluck(Obj *o) {
    if (gCommManager->isLocalSlot(o->sessionSlot) == 0) {
        if ((o)->testActionFlag(0x1c)) {
            PlayerActor_PluckApply(o);
        }
    }
}
}

namespace ns_02210ef0 {
extern "C" void PlayerActor_PluckUpdate(Obj *o) {
    (o)->advanceAnim();
    if (((AnimFrameCtrl *)((AnimFrameCtrl *)&o->bodyModel))->hasPassedFrame(0xa)) {
        PlayerActor_PluckApply(o);
    }
}
}

namespace ns_02210ef0 {
extern "C" void PlayerActor_PluckApply(Obj *o) {
    s16 ang;
    u16 t2;
    Pair ab;
    V3 *pv = (V3 *)&o->actionWork;
    V3 v = *pv;
    ab.x = 0;
    ab.y = 0;
    FieldPos_ToUnit(&ab.x, &ab.y, &v);
    ang = o->rotY;
    switch (o->actionItem) {
    case 0x1f:
        Effect_Create(0x2e, &v, &ang, 0);
        break;
    case 0x20:
        Effect_Create(0x2f, &v, &ang, 0);
        break;
    case 0x21: {
        V3 t(0, 0, -0x800);
        VEC_Add(&v, &t, &v);
        Effect_Create(0x2d, &v, &ang, 0);
        break;
    }
    case 0x22: {
        V3 t(-0x800, 0, 0);
        VEC_Add(&v, &t, &v);
        Effect_Create(0x2d, &v, &ang, 0);
        break;
    }
    case 0x23: {
        V3 t(0x800, 0, 0xc00);
        VEC_Add(&v, &t, &v);
        Effect_Create(0x2d, &v, &ang, 0);
        break;
    }
    case 0x24: {
        V3 t(-0x800, 0, 0);
        VEC_Add(&v, &t, &v);
        Effect_Create(0x2d, &v, &ang, 0);
        break;
    }
    default: {
        u32 w = o->actionItem;
        if ((w >= 0x6e && w <= 0x73) || (w >= 0x74 && w <= 0x79) || (w >= 0x7a && w <= 0x7f) ||
            (w >= 0x80 && w <= 0x87) || w == 0x88 || w == 0x89 || (w >= 0x8a && w <= 0x8f) ||
            (w >= 0x90 && w <= 0x95) || (w >= 0x96 && w <= 0x9b) || (w >= 0x9c && w <= 0xa3)) {
            goto hit;
        }
        if (w != 0xa5 && w != 0xa4) {
            break;
        }
    hit:
        Effect_PlayById2(0x54, &v, 0, 0);
        t2 = o->actionItem;
        Pair c = {ab.x, ab.y};
        Flower_SpawnPetalFxAt(&t2, &c, o->rotY, 2);
        (o)->playSe(0x7d8);
        break;
    }
    }
    switch (o->actionItem) {
    case 0x1f:
    case 0x20:
        (o)->playSe(0x7d8);
        break;
    case 0x21:
    case 0x22:
    case 0x23:
    case 0x24:
        (o)->playSe(0x7ec);
        break;
    }
    Pair d = {ab.x, ab.y};
    PendingUnit_ApplyAt(&d, 0);
    o->clearActionFlag(0x1c);  // (void: nothing reads this function's result)
}
}

namespace ns_02210ef0 {
extern "C" void PlayerActor_PluckCheckEnd(Obj *o) {
    if (((AnimFrameCtrl *)((AnimFrameCtrl *)&o->bodyModel))->isFinished()) {
        o->actionPriority = ((Unk_02007694 *)(void *)(o))->getActionDonePriority(o->action);
        (o)->requestWait(3, 1, -1);
    }
    if (((Unk_ov003_02210ef0_Bits *)&o->bodyModel.curFrame)->mid >= 0xe) {
        if (gCommManager->isLocalSlot(o->sessionSlot)) {
            PlayerActor_ResumeWalkOrIdle(o);
        } else {
            (o)->clearActionFlag(0x12);
        }
    }
}
}

namespace ns_02210ef0 {
extern "C" void PlayerActor_MainPluck(Obj *o) {
    PlayerActor_PluckUpdate(o);
    V3 *pv = (V3 *)&o->actionWork;
    V3 v = *pv;
    (o)->netSyncNearPoint((VecFx32 *)(void *)(&v));
    (o)->updateBodyCollider();
    (o)->submitSceneCollider();
    PlayerActor_PluckCheckEnd(o);
}
}

namespace ns_02210ef0 {
extern "C" void PlayerActor_DoorApproachSetArgs(Rec *p, s32 a, s32 b, s16 c) {
    p->a = a;
    p->b = b;
    p->c = c;
}
}

namespace ns_02210ef0 {
extern "C" s32 PlayerActor_RequestDoorApproach(Obj *o, s32 a, s32 b, s16 c, s32 d, s32 e) {
    Msg m;
    (&m)->assign(0x38, d, *(s16 *)&e);
    PlayerActor_DoorApproachSetArgs((DoorEnterWork *)m.unk_0c_b, a, b, c);
    s32 r = (o)->pushRequest(&m);
    return r;
}
}

namespace ns_02210ef0 {
extern "C" void PlayerActor_DoorApproachSetWork(Rec *p, s32 a, s32 b, s16 c) {
    p->a = a;
    p->b = b;
    p->c = c;
}
}

namespace ns_02210ef0 {
extern "C" void PlayerActor_DoorApproachSetNetData(u8 *p, s32 a, s32 b, s16 c) {
    NetBuf_PackPair20(p, a, b);
    NetBuf_WriteS16B(p + 5, c);
}
}

namespace ns_02210ef0 {
extern "C" void PlayerActor_DoorApproachGetNetData(u8 *p, s32 *a, s32 *b, s16 *c) {
    NetBuf_UnpackPair20(p, (u32 *)a, (u32 *)b);
    *c = NetBuf_ReadS16B(p + 5);
}
}

namespace ns_02210ef0 {
extern "C" void PlayerActor_SetupDoorApproach(Obj *o, Msg *m) {
    u8 *p = m->unk_0c_b;
    (o)->startAnimOnce(0x37, 3, 0);
    s32 a = *(s32 *)p;
    s32 b = *(s32 *)(p + 4);
    s16 c = *(s16 *)(p + 8);
    PlayerActor_DoorApproachSetWork((Rec *)&o->actionWork, a, b, c);
    PlayerActor_DoorApproachSetNetData(o->netData.bytes, a, b, c);
}
}

namespace ns_02210ef0 {
extern "C" void PlayerActor_NetDoorApproach(Obj *o, s32 x) {
    s16 ang;
    V3 pos;
    PlayerActor_DoorApproachGetNetData(o->netData.bytes, &pos.x, &pos.z, &ang);
    pos.y = o->position.y;
    V3 *pv = (V3 *)&o->position;
    *pv = pos;
    o->rotY = ang;
    PlayerActor_RequestDoorApproach(o, pos.x, pos.z, ang, 6, x);
}
}

namespace ns_02210ef0 {
extern "C" void PlayerActor_DoorApproachMove(Obj *o) {
    Rec *r = (Rec *)&o->actionWork;
    PlayerActor_StepTowardPose(o, r->a, r->b, r->c);
}
}

namespace ns_02210ef0 {
extern "C" void PlayerActor_DoorApproachUpdate(Obj *o) {
    (o)->advanceAnim();
    if (((AnimFrameCtrl *)((AnimFrameCtrl *)&o->bodyModel))->hasPassedFrame(8) || ((AnimFrameCtrl *)((AnimFrameCtrl *)&o->bodyModel))->hasPassedFrame(0xd)) {
        (o)->playSe(0x7d5);
    }
}
}

namespace ns_02210ef0 {
extern "C" void PlayerActor_DoorApproachCheckEnd(Obj *o) {
    if (((AnimFrameCtrl *)((AnimFrameCtrl *)&o->bodyModel))->isFinished()) {
        Rec *r = (Rec *)&o->actionWork;
        PlayerActor_RequestDoorEnter(o, 1, r->a, r->b, r->c, 6, -1);
    }
}
}

namespace ns_02210ef0 {
extern "C" void PlayerActor_MainDoorApproach(Obj *o) {
    PlayerActor_DoorApproachMove(o);
    PlayerActor_DoorApproachUpdate(o);
    (o)->updateBodyCollider();
    PlayerActor_DoorApproachCheckEnd(o);
}
}

namespace ns_02210ef0 {
extern "C" void PlayerActor_DoorEnterSetArgs(Rec *p, u32 a, s32 b, s32 c, s16 d) {
    p->d = a;
    p->a = b;
    p->b = c;
    p->c = d;
}
}

namespace ns_02210ef0 {
extern "C" s32 PlayerActor_RequestDoorEnter(Obj *o, u8 a, s32 b, s32 c, s16 d, s32 e, s32 f) {
    Msg m;
    (&m)->assign(0x39, e, *(s16 *)&f);
    PlayerActor_DoorEnterSetArgs((DoorEnterWork *)m.unk_0c_b, a, b, c, d);
    s32 r = (o)->pushRequest(&m);
    return r;
}
}

namespace ns_02210ef0 {
extern "C" void PlayerActor_DoorEnterSetWork(Rec *p, u32 a, s32 b, s32 c, s16 d, u8 e) {
    p->d = a;
    p->a = b;
    p->b = c;
    p->c = d;
    p->e = e;
}
}

namespace ns_02210ef0 {
extern "C" void PlayerActor_DoorEnterSetNetData(u8 *p, u32 a, s32 b, s32 c, s16 e) {
    p[7] = a;
    NetBuf_PackPair20(p, b, c);
    NetBuf_WriteS16B(p + 5, e);
}
}

namespace ns_02210ef0 {
extern "C" void PlayerActor_DoorEnterGetNetData(u8 *p, u8 *a, s32 *b, s32 *c, u16 *e) {
    *a = p[7];
    NetBuf_UnpackPair20(p, (u32 *)b, (u32 *)c);
    *e = NetBuf_ReadS16B(p + 5);
}
}

namespace ns_02210ef0 {
extern "C" void PlayerActor_SetupDoorEnter(Obj *o, Msg *m) {
    (o)->setActionFlag(4);
    u8 *p = m->unk_0c_b;
    Rec *r = (Rec *)&o->actionWork;
    s32 z = 0;
    s32 k = z;
    switch (p[0xa]) {
    case 0:
        (o)->startAnimOnce(0x33, 3, 0);
        break;
    case 1:
        (o)->startAnimOnce(0x32, 3, 0);
        break;
    case 2:
        k = 0xc;
        (o)->startAnim(z, 3, z);
        break;
    }
    u32 id = p[0xa];
    Pair t = *(Pair *)p;
    s16 ang = *(s16 *)(p + 8);
    PlayerActor_DoorEnterSetWork(r, id, t.x, t.y, ang, k);
    PlayerActor_DoorEnterSetNetData(o->netData.bytes, id, t.x, t.y, ang);
    Building_OpenDoorForEntryAt(&o->position);
}
}

namespace ns_02210ef0 {
extern "C" void PlayerActor_NetDoorEnter(Obj *o, s32 x) {
    if ((u32)(o->action - 0x38) <= 2) {
        o->netSeq = x;
    } else {
        u8 a;
        u8 f;
        s16 ang;
        s32 o1, o2;
        V3 pos;
        PlayerActor_DoorEnterGetNetData(o->netData.bytes, &a, &pos.x, &pos.z, (u16 *)&ang);
        pos.y = o->position.y;
        if (PlayerActor_GetSlotPosXZ(&f, &o1, &o2, -1, o->sessionSlot)) {
            o->position.x = o1;
            o->position.z = o2;
        }
        o->rotY = ang;
        PlayerActor_RequestDoorEnter(o, a, pos.x, pos.z, ang, 6, x);
    }
}
}

namespace ns_022105bc {
extern "C" void PlayerActor_DoorEnterMove(Obj *o) {
    Rec3 *r = (Rec3 *)&o->actionWorkRaw[0];
    u8 *p = &r->e;
    if (r->e) {
        *p = r->e - 1;
        if (*p == 0) {
            (o)->startAnimOnce(0x33, 3, 0);
        }
    } else {
        PlayerActor_StepTowardPose(o, r->a, r->b, r->c);
    }
}
}

namespace ns_022105bc {
extern "C" void PlayerActor_DoorEnterUpdate(Obj *o) {
    (o)->advanceAnim();
    if (o->animId == 0x32) {
        s32 m = ((Bits *)&o->bodyModel.curFrame)->mid;
        if (m > 0x16) goto r_hi;
        if (m >= 0x16) goto hit;
        if (m > 0xc) goto done;
        if (m < 8) goto done;
        switch (m) {
        case 8: goto hit;
        case 0xc: goto hit;
        }
        goto done;
    r_hi:
        if (m > 0x1b) goto r34;
        switch (m) {
        case 0x1b: goto hit;
        }
        goto done;
    r34:
        if (m != 0x22) goto done;
    hit:
        (o)->playFootstepSe();
    done:;
    } else if (o->animId == 0x33) {
        switch ((s32)((Bits *)&o->bodyModel.curFrame)->mid) {
        case 9:
        case 0x11:
        case 0x1a:
            (o)->playFootstepSe();
            break;
        }
    }
}
}

namespace ns_022105bc {
extern "C" void PlayerActor_DoorEnterCheckEnd(Obj *o) {
    if (((AnimFrameCtrl *)((AnimFrameCtrl *)&o->bodyModel))->isFinished()) {
        PlayerActor_RequestDoorEntered(o, 6, -1);
    }
}
}

namespace ns_022105bc {
extern "C" void PlayerActor_MainDoorEnter(Obj *o) {
    PlayerActor_DoorEnterMove(o);
    PlayerActor_DoorEnterUpdate(o);
    (o)->updateBodyCollider();
    PlayerActor_DoorEnterCheckEnd(o);
}
}

namespace ns_022105bc {
extern "C" s32 PlayerActor_RequestDoorEntered(Obj *o, s32 a, s32 b) {
    Msg m;
    (&m)->assign(0x3a, a, b);
    s32 r = (o)->pushRequest(&m);
    return r;
}
}

namespace ns_022105bc {
extern "C" void PlayerActor_SetupDoorEntered(Obj *o) {
    (o)->clearActionFlag(0);
    o->drawStep = 0;
}
}

namespace ns_022105bc {
extern "C" s32 PlayerActor_NetDoorEntered(Obj *o, s32 a) {
    return PlayerActor_RequestDoorEntered(o, 6, a);
}
}

namespace ns_022105bc {
extern "C" void PlayerActor_MainDoorEntered() {}
}

namespace ns_022105bc {
extern "C" s32 PlayerActor_RequestDoorExit(Obj *o, s32 a, s32 b) {
    Msg m;
    Rec2 &q = *(Rec2 *)m.unk_0c_b;
    (&m)->assign(0x3b, a, b);
    q.a = 0;
    q.b = 0;
    s32 r = (o)->pushRequest(&m);
    return r;
}
}

namespace ns_022105bc {
extern "C" s32 PlayerActor_RequestDoorExitWith(Obj *o, u32 a, u32 b, s32 c, s16 d) {
    Msg m;
    (&m)->assign(0x3b, c, d);
    Rec2 &q = *(Rec2 *)m.unk_0c_b;
    q.a = a;
    q.b = b;
    s32 r = (o)->pushRequest(&m);
    return r;
}
}

namespace ns_022105bc {
extern "C" void PlayerActor_DoorExitSetNetData(u8 *p, u32 a, u32 b) {
    p[0] = a;
    p[1] = b;
}
}

namespace ns_022105bc {
extern "C" void PlayerActor_DoorExitGetNetData(u8 *p, u8 *a, u8 *b) {
    *a = p[0];
    *b = p[1];
}
}

namespace ns_022105bc {
extern "C" s32 PlayerActor_SetupDoorExit(Obj *o, Msg *m) {
    (o)->clearActionFlag(0);
    u8 *p = m->unk_0c_b;
    Rec2 *rec = (Rec2 *)&o->actionWorkRaw[0];
    s32 t = 0;
    CommManager *g = gCommManager;
    s32 n;
    if (g->isLocalSlot(o->sessionSlot)) {
        n = Field_GetDoorExitMode();
    } else {
        n = p[0];
    }
    if (g->isLocalSlot(o->sessionSlot)) {
        void *q = PlayerData_GetCurrent();
        if (Scene_GetCurrent() == 0 && q != 0 && ((PlayerData *)q)->testFlag(0x23) != 0 &&
            (SceneId_IsHouseRoom(Scene_GetPrevious()) != 0 || Scene_GetPrevious() == 6)) {
            t = 1;
        } else if (TownSessionState_TestFlag(TownSessionState_Get(), 0) != 0) {
            t = 2;
        } else if (TownSessionState_TestFlag(TownSessionState_Get(), 5) != 0) {
            t = 3;
        }
    } else {
        t = p[1];
    }
    u32 u;
    if (n != 0) {
        (o)->startAnimOnce(0x34, 0, 0);
        u = 0;
        o->drawStep = 1;
    } else {
        u = (u8)Building_GetDoorAnimParamAt(&o->position);
        if (u <= 1) {
            (o)->startAnimOnce(0x35, 0, 0);
        } else {
            (o)->startAnim(0x36, 0, 0);
        }
        o->drawStep = 1;
    }
    rec->b = u;
    rec->a = t;
    PlayerActor_DoorExitSetNetData(o->netData.bytes, n, t);
    return Building_OpenDoorForExitAt(&o->position);
}
}

namespace ns_022105bc {
extern "C" void PlayerActor_NetDoorExit(Obj *o, s32 a) {
    if (o->action == 0x3b) {
        o->netSeq = a;
    } else {
        Rec2 r;
        PlayerActor_DoorExitGetNetData(o->netData.bytes, &r.a, &r.b);
        PlayerActor_RequestDoorExitWith(o, r.a, r.b, 6, a);
    }
}
}

namespace ns_022105bc {
extern "C" void PlayerActor_DoorExitUpdate(Obj *o) {
    u8 *p = &o->actionWorkRaw[1]; // Rec2.b: frames to wait before the exit walk
    (o)->advanceAnim();
    u32 c = o->actionWorkRaw[1];
    if (c) {
        *p = c - 1;
        if (*p == 0) {
            if (o->animId == 0x36) {
                (o)->startAnimOnce(0x35, 5, 0);
            }
        }
    } else {
        if (o->animId == 0x34) {
            s32 m = ((Bits *)&o->bodyModel.curFrame)->mid;
            if (m > 0x1c) goto r_hi;
            if (m >= 0x1c) goto hit;
            if (m > 0x11) goto l23;
            if (m < 0xd) goto done;
            switch (m) {
            case 0xd: goto hit;
            case 0x11: goto hit;
            }
            goto done;
        l23:
            switch (m) {
            case 0x17: goto hit;
            }
            goto done;
        r_hi:
            if (m > 0x25) goto r40;
            switch (m) {
            case 0x25: goto hit;
            }
            goto done;
        r40:
            if (m != 0x28) goto done;
        hit:
            (o)->playFootstepSe();
        done:;
        } else if (o->animId == 0x35) {
            switch ((s32)((Bits *)&o->bodyModel.curFrame)->mid) {
            case 4:
            case 9:
            case 0xe:
            case 0x14:
                (o)->playFootstepSe();
                break;
            }
        }
    }
}
}

namespace ns_022105bc {
extern "C" void PlayerActor_DoorExitCheckEnd(Obj *o) {
    u16 v[8];
    if (!((AnimFrameCtrl *)((AnimFrameCtrl *)&o->bodyModel))->isFinished()) return;
    (o)->nudgeForward();
    o->actionPriority = ((Unk_02007694 *)(void *)(o))->getActionDonePriority(o->action);
    u8 *pst = &o->actionWorkRaw[0];
    u8 st = *pst;
    PlayerActor_GetHeldItem(v, o);
    if (st <= 1) {
        BOOL r = FALSE;
        volatile u16 *pv = &v[0];
        u32 x = *pv;
        u32 y = *pv;
        if (y < 0x1380 || x > 0x139f) {
        } else {
            r = TRUE;
        }
        if (r != 0 || (x >= 0x13a0 && x <= 0x13a7)) {
            (o)->clearActionFlag(0x1d);
        }
    }
    switch (*pst) {
    case 0: {
        BOOL r = Unk_ov003_022107fc_Eq(v, &v[5]);
        if (r == 0 && Taxi_IsLeaving() == 0 && (o)->testActionFlag(0x1d) == 0) {
            PlayerActor_RequestStowItem(o, 2, 2, 0, 0, 0, 6, -1);
            return;
        }
        if (gCommManager->isLocalSlot(o->sessionSlot)) {
            if (Unk_ov003_022107fc_Eq(v, &v[2])) {
                (o)->setActionFlag(0);
            }
        }
    }
    case 1:
        if (Unk_ov003_022107fc_Eq(v, &v[3])) {
            (o)->setActionFlag(0);
        }
        TalkRequest_FinishSceneEntry();
        (o)->clearActionFlag(0x1d);
        (o)->requestWait(3, 1, -1);
        break;
    case 2: {
        CommManager *g = gCommManager;
        if (g->isLocalSlot(o->sessionSlot)) {
            TownSessionState_ClearFlag(TownSessionState_Get(), 0);
        }
        if (Taxi_IsLeaving() == 0) {
            (o)->requestAct76(2, 0, 0, 6, -1);
            return;
        }
        TalkRequest_FinishSceneEntry();
        if (g->isLocalSlot(o->sessionSlot)) {
            goto x;
        } else {
            PlayerActor_GetHeldItem(&v[1], o);
            if (Unk_ov003_022107fc_Eq(&v[1], &v[4])) {
            x:
                (o)->setActionFlag(0);
            }
        }
        (o)->clearActionFlag(0x1d);
        (o)->requestWait(3, 1, -1);
        break;
    }
    case 3:
        if (gCommManager->isLocalSlot(o->sessionSlot)) {
            TownSessionState_ClearFlag(TownSessionState_Get(), 5);
            PlayerData_GetCurrent()->clearFlag(1);
            TownBbs_UpdateDaily();
        }
        (o)->requestAct76(3, 0, 0, 6, -1);
        break;
    }
}
}

namespace ns_022105bc {
extern "C" void PlayerActor_MainDoorExit(Obj *o) {
    PlayerActor_DoorExitUpdate(o);
    (o)->updateBodyCollider();
    PlayerActor_DoorExitCheckEnd(o);
}
}

namespace ns_022105bc {
extern "C" s32 PlayerActor_RequestAct3C(Obj *o, s32 a, s32 b) {
    Msg m;
    (&m)->assign(0x3c, a, b);
    s32 r = (o)->pushRequest(&m);
    return r;
}
}

namespace ns_022105bc {
extern "C" void PlayerActor_SetupAct3C(Obj *o) {
    (o)->clearActionFlag(0);
    if (Field_GetDoorExitMode()) {
        (o)->startAnimOnce(0x34, 0, 0);
    } else {
        (o)->startAnimOnce(0x35, 0, 0);
    }
    o->drawStep = 0;
    if (TownSessionState_TestFlag(TownSessionState_Get(), 0) != 0 || TownSessionState_TestFlag(TownSessionState_Get(), 5) != 0) {
        if (gCommManager->isLocalSlot(o->sessionSlot)) {
            Bgm_RequestSilence(0x12, 0xf, 0);
        }
    }
}
}

namespace ns_022105bc {
extern "C" s32 PlayerActor_NetAct3C(Obj *o, s32 a) {
    return PlayerActor_RequestAct3C(o, 1, a);
}
}

namespace ns_022105bc {
extern "C" void PlayerActor_EndAct3C(Obj *o, s32 a) {
    if (a != 0x3b && a != 0x3d) {
        (o)->setActionFlag(0);
    }
}
}

namespace ns_022105bc {
extern "C" void PlayerActor_MainAct3C() {}
}

namespace ns_022105bc {
extern "C" void PlayerActor_StowItemSetArgs(Pay *p, s32 a, u32 b, s32 c, s32 d, s16 e) {
    p->st = a;
    p->bits = b;
    p->a = c;
    p->b = d;
    p->h = e;
}
}

namespace ns_022105bc {
extern "C" s32 PlayerActor_RequestStowItem(Obj *o, s32 a, s32 b, s32 c, s32 d, s16 e, s32 f, s16 g) {
    u16 v[2];
    PlayerActor_GetHeldItem(v, o);
    {
        BOOL r = FALSE;
        volatile u16 *pv = &v[0];
        u32 x = *pv;
        u32 y = *pv;
        if (y < 0x1380 || x > 0x139f) {
        } else {
            r = TRUE;
        }
        if (r != 0 || (x >= 0x13a0 && x <= 0x13a7)) {
            return PlayerActor_RequestStowUmbrella(o, a, b, c, d, e, f, g);
        }
    }
    Msg m;
    (&m)->assign(0x3d, f, g);
    PlayerActor_StowItemSetArgs((Pay *)m.unk_0c_b, a, b, c, d, e);
    (o)->clearActionFlag(1);
    s32 r = (o)->pushRequest(&m);
    return r;
}
}

namespace ns_022105bc {
extern "C" void PlayerActor_StowItemSetWork(Rec *p, s32 a, u32 b, s32 c, s32 d, s16 e, s32 f) {
    p->st = a;
    p->bits = b;
    p->a = c;
    p->b = d;
    p->h = e;
    p->timer = f;
}
}

namespace ns_022105bc {
extern "C" void PlayerActor_StowItemSetNetData(u8 *p, u32 a, u32 b, u32 c, s16 d) {
    p[7] = a;
    NetBuf_PackPair20(p, b, c);
    NetBuf_WriteS16B(p + 5, d);
}
}

namespace ns_022105bc {
extern "C" void PlayerActor_StowItemGetNetData(u8 *p, u8 *a, s32 *b, s32 *c, s16 *d) {
    *a = p[7];
    NetBuf_UnpackPair20(p, (u32 *)b, (u32 *)c);
    *d = NetBuf_ReadS16B(p + 5);
}
}

namespace ns_0220fc88 {
extern "C" void PlayerActor_SetupStowItem(Obj *o, Msg *a) {
    RecA *r = (RecA *)a->unk_0c_b;
    H hh;
    StowItemWork *p7d0 = (StowItemWork *)&o->actionWork;
    PlayerActor_GetHeldItem(&hh.a, o);
    PlayerData *q = static_cast<PlayerData *>(PlayerActor_GetPlayerData(o));
    hh.b = 0xfff1;
    q->setHeldItem(&hh.b);
    (o)->startAnimOnce(0x6b, 3, 0);
    q->setHeldItem(&hh.a);
    s32 v;
    if ((r->bits & 3) == 2) {
        (o)->setActionFlag(0);
        v = 0;
        u32 mid = ((Unk_ov003_0220fc88_Bits *)&o->bodyModel.numFrames)->mid;
        ((AnimFrameCtrl *)((AnimFrameCtrl *)&o->bodyModel))->setup(mid, 3, 0x1000, (u16)(mid - 1));
    } else {
        v = 0x1000;
    }
    s32 st = r->st;
    u8 bits = r->bits;
    s32 x = r->a;
    s32 y = r->b;
    s32 h2 = r->h;
    PlayerActor_StowItemSetWork(p7d0, st, bits, x, y, h2, v);
    u8 *p8 = o->netData.bytes;
    switch (st) {
    case 0x38:
        break;
    case 0x39:
        bits |= 0x10;
        break;
    case 0x10:
        bits |= 0x20;
        break;
    case 2:
        bits |= 0x30;
        break;
    }
    PlayerActor_StowItemSetNetData(p8, bits, x, y, h2);
    o->playSe(0x4f);  // (void: nothing reads this function's result)
}
}

namespace ns_0220fc88 {
extern "C" s32 PlayerActor_NetStowItem(Obj *o, s32 a) {
    u8 b;
    s16 h;
    s32 x, y;
    PlayerActor_StowItemGetNetData(o->netData.bytes, &b, &x, &y, &h);
    s32 id;
    switch (b & 0xf0) {
    case 0:
        id = 0x38;
        break;
    case 0x10:
        id = 0x39;
        break;
    case 0x20:
        id = 0x10;
        break;
    default:
        id = 2;
        break;
    }
    return PlayerActor_RequestStowItem(o, id, (u8)(b & 0xf), x, y, h, 6, a);
}
}

namespace ns_0220fc88 {
extern "C" void PlayerActor_EndStowItem(Obj *o) {
    if ((((RecB *)&o->actionWork)->bits & 3) == 2) {
        o->heldItemModel.scale = 0x1000;
    } else {
        o->heldItemModel.scale = 0;
        (o)->clearActionFlag(0);
    }
}
}

namespace ns_0220fc88 {
extern "C" void PlayerActor_StowItemUpdate(Obj *o) {
    RecB *r = (RecB *)&o->actionWork;
    s32 *t = &r->timer;
    s32 target;
    if ((r->bits & 3) == 2) {
        target = 0x1000;
        if (((Unk_ov003_0220fc88_Bits *)&o->bodyModel.curFrame)->mid < 8) {
            if (((AnimFrameCtrl *)((AnimFrameCtrl *)&o->bodyModel))->hasPassedFrame(7)) {
                (o)->applyHeldItemPose(0, 7);
            }
            if (*t != 0x1000) {
                *t += 0x249;
                if (*t > 0x1000) *t = 0x1000;
            }
        }
    } else {
        target = 0;
        if (*t != 0) {
            *t -= 0x249;
            if (*t < 0) *t = target;
        }
    }
    o->heldItemModel.scale = r->timer;
    if (((AnimFrameCtrl *)((AnimFrameCtrl *)&o->bodyModel))->isFinished() && target == *t) {
        (o)->setActionFlag(1);
        switch (r->st) {
        case 0x38:
            (o)->clearActionFlag(0);
            PlayerActor_RequestDoorApproach(o, r->a, r->b, r->h, 6, -1);
            break;
        case 0x39:
            (o)->clearActionFlag(0);
            PlayerActor_RequestDoorEnter(o, (u8)((r->bits & 0xc) >> 2), r->a, r->b, r->h, 6, -1);
            break;
        case 0x10:
            (o)->clearActionFlag(0);
            o->actionPriority = ((Unk_02007694 *)(void *)(o))->getActionDonePriority(o->action);
            (o)->requestAct10(3, 5, -1);
            break;
        case 2:
            TalkRequest_FinishSceneEntry();
            o->actionPriority = ((Unk_02007694 *)(void *)(o))->getActionDonePriority(o->action);
            (o)->requestWait(3, 1, -1);
            break;
        }
    }
}
}

namespace ns_0220fc88 {
extern "C" void PlayerActor_MainStowItem(Obj *o) {
    (o)->advanceAnim();
    (o)->updateBodyCollider();
    PlayerActor_StowItemUpdate(o);
}
}

namespace ns_0220fc88 {
extern "C" void PlayerActor_StowUmbrellaSetArgs(RecA *p, s32 st, u32 bits, s32 x, s32 y, s32 h) {
    p->st = st;
    p->bits = bits;
    p->a = x;
    p->b = y;
    p->h = *(s16 *)&h;
}
}

namespace ns_0220fc88 {
extern "C" s32 PlayerActor_RequestStowUmbrella(Obj *o, s32 id, u8 low, s32 x, s32 y, s16 h, s32 a, s32 b) {
    Msg m;
    (&m)->assign(0x3e, a, *(s16 *)&b);
    PlayerActor_StowUmbrellaSetArgs((PlayerStowItemArgs *)m.unk_0c_b, id, low, x, y, h);
    (o)->clearActionFlag(1);
    s32 r = (o)->pushRequest(&m);
    return r;
}
}

namespace ns_0220fc88 {
extern "C" void PlayerActor_StowUmbrellaSetWork(RecA *p, s32 st, u32 bits, s32 x, s32 y, s32 h) {
    p->st = st;
    p->bits = bits;
    p->a = x;
    p->b = y;
    p->h = *(s16 *)&h;
}
}

namespace ns_0220fc88 {
extern "C" void PlayerActor_StowUmbrellaSetNetData(u8 *p, u32 id, s32 x, s32 y, s32 h) {
    p[7] = id;
    NetBuf_PackPair20(p, x, y);
    NetBuf_WriteS16B(p + 5, *(s16 *)&h);
}
}

namespace ns_0220fc88 {
extern "C" void PlayerActor_StowUmbrellaGetNetData(u8 *p, u8 *a, s32 *b, s32 *c, s16 *d) {
    *a = p[7];
    NetBuf_UnpackPair20(p, (u32 *)b, (u32 *)c);
    *d = NetBuf_ReadS16B(p + 5);
}
}

namespace ns_0220fc88 {
extern "C" void PlayerActor_SetupStowUmbrella(Obj *o, Msg *a) {
    RecA *r = (RecA *)a->unk_0c_b;
    u8 bits = r->bits;
    RecA *p7d0 = (RecA *)&o->actionWork;
    HeldItemModel_PlayAnim(&o->heldItemModel, 0x24, 3, 1);
    s32 x, y, st;
    s16 h;
    if ((bits & 3) != 2) {
        (o)->startAnimOnce(0x62, 3, 0);
        u32 mid = ((Unk_ov003_0220fc88_Bits *)&o->bodyModel.numFrames)->mid;
        ((AnimFrameCtrl *)((AnimFrameCtrl *)&o->bodyModel))->setup(mid, 3, 0x1000, (u16)(mid - 1));
        BlendAnimModel *s = (BlendAnimModel *)HeldItemModel_GetModel((u8 *)&o->heldItemModel);
        u32 mid2 = ((Unk_ov003_0220fc88_Bits *)&s->numFrames)->mid;
        ((AnimFrameCtrl *)(&static_cast<AnimFrameCtrl &>(*s)))->setup(mid2, 3, 0x1000, (u16)(mid2 - 1));
        (o)->playSe(0x858);
    } else {
        H hh;
        PlayerActor_GetHeldItem(&hh.a, o);
        PlayerData *q = static_cast<PlayerData *>(PlayerActor_GetPlayerData(o));
        hh.b = 0xfff1;
        q->setHeldItem(&hh.b);
        (o)->startAnimOnce(0x62, 3, 0);
        q->setHeldItem(&hh.a);
        (o)->clearActionFlag(0);
    }
    st = r->st;
    x = r->a;
    y = r->b;
    h = r->h;
    PlayerActor_StowUmbrellaSetWork(p7d0, st, bits, x, y, h);
    u8 *p8 = o->netData.bytes;
    switch (st) {
    case 0x38:
        break;
    case 0x39:
        bits |= 0x10;
        break;
    case 0x10:
        bits |= 0x20;
        break;
    case 2:
        bits |= 0x30;
        break;
    }
    PlayerActor_StowUmbrellaSetNetData(p8, bits, x, y, h);
    o->playSe(0x4f);  // (void: nothing reads this function's result)
}
}

namespace ns_0220fc88 {
extern "C" s32 PlayerActor_NetStowUmbrella(Obj *o, s32 a) {
    u8 b;
    s16 h;
    s32 x, y;
    PlayerActor_StowUmbrellaGetNetData(o->netData.bytes, &b, &x, &y, &h);
    s32 id;
    switch (b & 0xf0) {
    case 0:
        id = 0x38;
        break;
    case 0x10:
        id = 0x39;
        break;
    case 0x20:
        id = 0x10;
        break;
    default:
        id = 2;
        break;
    }
    return PlayerActor_RequestStowUmbrella(o, id, (u8)(b & 0xf), x, y, h, 6, a);
}
}

namespace ns_0220fc88 {
extern "C" void PlayerActor_StowUmbrellaUpdate(Obj *o) {
    (o)->advanceAnim();
    RecA *r = (RecA *)&o->actionWork;
    if ((r->bits & 3) != 2) {
        if (((Unk_ov003_0220fc88_Bits *)&o->bodyModel.curFrame)->mid < 6) {
            if (((AnimFrameCtrl *)((AnimFrameCtrl *)&o->bodyModel))->hasPassedFrame(5)) {
                (o)->clearActionFlag(0);
            }
        }
    } else {
        if (((Unk_ov003_0220fc88_Bits *)&o->bodyModel.curFrame)->mid < 8) {
            if (((AnimFrameCtrl *)((AnimFrameCtrl *)&o->bodyModel))->hasPassedFrame(7)) {
                (o)->setActionFlag(0);
                (o)->playSe(0x857);
            }
        }
    }
}
}

namespace ns_0220fc88 {
extern "C" void PlayerActor_StowUmbrellaCheckEnd(Obj *o) {
    RecA *r = (RecA *)&o->actionWork;
    if (((AnimFrameCtrl *)((AnimFrameCtrl *)&o->bodyModel))->isFinished()) {
        (o)->setActionFlag(1);
        switch (r->st) {
        case 0x38:
            (o)->clearActionFlag(0);
            PlayerActor_RequestDoorApproach(o, r->a, r->b, r->h, 6, -1);
            break;
        case 0x39:
            (o)->clearActionFlag(0);
            PlayerActor_RequestDoorEnter(o, (u8)((r->bits & 0xc) >> 2), r->a, r->b, r->h, 6, -1);
            break;
        case 0x10:
            (o)->clearActionFlag(0);
            o->actionPriority = ((Unk_02007694 *)(void *)(o))->getActionDonePriority(o->action);
            (o)->requestAct10(3, 5, -1);
            break;
        case 2:
            TalkRequest_FinishSceneEntry();
            o->actionPriority = ((Unk_02007694 *)(void *)(o))->getActionDonePriority(o->action);
            (o)->requestWait(3, 1, -1);
            break;
        }
    }
}
}

namespace ns_0220fc88 {
extern "C" void PlayerActor_MainStowUmbrella(Obj *o) {
    PlayerActor_StowUmbrellaUpdate(o);
    (o)->updateBodyCollider();
    PlayerActor_StowUmbrellaCheckEnd(o);
}
}

namespace ns_0220fc88 {
extern "C" s32 PlayerActor_RequestUmbrellaSpin(Obj *o, s32 a, s16 b) {
    Msg m;
    (&m)->assign(0x45, a, b);
    s32 r = (o)->pushRequest(&m);
    return r;
}
}

namespace ns_0220fc88 {
extern "C" void PlayerActor_UmbrellaSpinInitWork(u8 *p) {
    *(u16 *)p = 0;
    *(u16 *)(p + 2) = 0x20;
    *(s32 *)(p + 4) = -1;
}
}

namespace ns_0220fc88 {
extern "C" void PlayerActor_SetupUmbrellaSpin(Obj *o) {
    (o)->startAnimOnce(0x63, 3, 0);
    PlayerActor_UmbrellaSpinInitWork((u8 *)&o->actionWork);
    (o)->playSe(0x856);
    o->setActionFlag(9);  // (void: nothing reads this function's result)
}
}

namespace ns_0220fc88 {
extern "C" s32 PlayerActor_NetUmbrellaSpin(Obj *o, s16 b) {
    return PlayerActor_RequestUmbrellaSpin(o, 6, b);
}
}

namespace ns_0220fc88 {
extern "C" void PlayerActor_EndUmbrellaSpin(Obj *o) {
    s32 h = ((RecB *)&o->actionWork)->a;
    if (h != -1) {
        Effect_End(h);
    }
}
}

namespace ns_0220fc88 {
extern "C" void PlayerActor_UmbrellaSpinEffect(Obj *o) {
    (o)->advanceAnim();
    s32 *p = (s32 *)&o->actionWorkRaw[4]; // RecB.a
    V3 v(o->heldItemJointMtx.m[9], o->heldItemJointMtx.m[10], o->heldItemJointMtx.m[11]);
    WorldCurve_FromCurved(&v, &v);
    if (((RecB *)&o->actionWork)->a == -1) {
        *p = Effect_Create(0x33, &v, &o->rotY, 0);
    } else {
        Effect_SetPosition(((RecB *)&o->actionWork)->a, (VecFx32 *)&v, &o->rotY, NULL);
    }
}
}

namespace ns_0220fc88 {
extern "C" void PlayerActor_UmbrellaSpinCheckEnd(Obj *o) {
    if (((AnimFrameCtrl *)((AnimFrameCtrl *)&o->bodyModel))->isFinished()) {
        o->actionPriority = ((Unk_02007694 *)(void *)(o))->getActionDonePriority(o->action);
        (o)->requestWait(3, 1, -1);
    }
    u32 m = ((Unk_ov003_0220fc88_Bits *)&o->bodyModel.curFrame)->mid;
    if (m >= 0xe) {
        if (gCommManager->isLocalSlot(o->sessionSlot)) {
            PlayerActor_ResumeWalkOrIdle(o);
        }
    }
}
}

namespace ns_0220fc88 {
extern "C" void PlayerActor_MainUmbrellaSpin(Obj *o) {
    PlayerActor_UmbrellaSpinEffect(o);
    if (gCommManager->isLocalSlot(o->sessionSlot)) {
        (o)->moveWithCollision();
    } else {
        (o)->netFollowTransform();
    }
    (o)->updateBodyCollider();
    (o)->submitSceneCollider();
    PlayerActor_UmbrellaSpinCheckEnd(o);
}
}

namespace ns_0220fc88 {
extern "C" void PlayerActor_AxeSwingSetArgs(s32 *p, s32 a, s32 b, u32 c) {
    p[0] = a;
    p[1] = b;
    *(u8 *)&p[2] = c;
}
}

namespace ns_0220f314 {
extern "C" s32 PlayerActor_RequestAxeSwing(Obj *o, s32 a, s16 b) {
    Msg m;
    s32 *p = m.unk_0c.v;
    (&m)->assign(0x46, a, b);
    if (o->hasTargetPos) {
        PlayerActor_AxeSwingSetArgs(p, o->targetPos.x, o->targetPos.z, 1);
    } else {
        PlayerActor_AxeSwingSetArgs(p, o->toolTargetPos.x, o->toolTargetPos.z, o->toolTargetKind != 0 ? 1 : 0);
    }
    s32 r = (o)->pushRequest(&m);
    return r;
}
}

namespace ns_0220f314 {
extern "C" s32 PlayerActor_RequestAxeSwingAt(Obj *o, s32 *a, s32 *b, u8 *c, s32 d, s32 e) {
    Msg m;
    (&m)->assign(0x46, d, *(s16 *)&e);
    PlayerActor_AxeSwingSetArgs(m.unk_0c.v, *a, *b, *c);
    s32 r = (o)->pushRequest(&m);
    return r;
}
}

namespace ns_0220f314 {
extern "C" void PlayerActor_AxeSwingSetWork(u8 *d, V3 *v, s32 a) {
    AxeSwingWork *r = (AxeSwingWork *)d;
    r->v.x = v->x;
    r->v.y = v->y;
    r->v.z = v->z;
    r->h = a;
}
}

namespace ns_0220f314 {
extern "C" void PlayerActor_AxeSwingSetNetData(u8 *p, s32 a, s32 b, u8 c) {
    NetBuf_PackPair20(p, a, b);
    p[5] = c;
}
}

namespace ns_0220f314 {
extern "C" void PlayerActor_AxeSwingGetNetData(u8 *p, s32 *a, s32 *b, u8 *c) {
    NetBuf_UnpackPair20(p, (u32 *)a, (u32 *)b);
    u32 t = p[5];
    *c = t;
}
}

namespace ns_0220f314 {
extern "C" void PlayerActor_SetupAxeSwing(Obj *o, Msg *a) {
    struct {
        u32 out;
        V3 v, A, B, C, D;
    } l;
    P2 *q = (P2 *)a->unk_0c_b;
    u8 *rec = (u8 *)&o->actionWork;
    s32 x = ((P2 *)a->unk_0c_b)->x;
    s32 z = q->z;
    u8 flag = q->flag;
    PlayerActor_AxeSwingSetNetData(o->netData.bytes, x, z, flag);
    (o)->startAnimOnce(0x42, 3, 0);
    if (flag != 0) {
        l.v.x = x;
        l.v.z = z;
    } else {
        PlayerActor_GetFrontPoint(&l.A, o);
        if (Unk_ov003_0220fa70_Ge(Ground_GetHeightAt(&l.A, &l.out, 0x19))) {
            PlayerActor_GetFrontUnitCenter(&l.B, o);
            l.v.x = l.B.x;
            l.v.y = l.B.y;
            l.v.z = l.B.z;
        } else {
            PlayerActor_GetFrontPoint(&l.C, o);
            l.v.x = l.C.x;
            l.v.y = l.C.y;
            l.v.z = l.C.z;
        }
    }
    s32 ang = Math_Atan2(l.v.x - o->bodyPos.x, l.v.z - o->bodyPos.z);
    l.D.x = l.v.x;
    l.D.y = l.v.y;
    l.D.z = l.v.z;
    PlayerActor_AxeSwingSetWork(rec, &l.D, ang);
    if (gCommManager->isLocalSlot(o->sessionSlot)) {
        (o)->clearActionFlag(0x14);
        if ((o)->getHeldHoldableIndex() != 0xb) {
            if (!Random_GlobalBelow(8)) {
                (o)->setActionFlag(0x14);
            }
        }
    }
}
}

namespace ns_0220f314 {
extern "C" s32 PlayerActor_NetAxeSwing(Obj *o, s32 a) {
    u8 c;
    s32 x, z;
    PlayerActor_AxeSwingGetNetData(o->netData.bytes, &x, &z, &c);
    return PlayerActor_RequestAxeSwingAt(o, &x, &z, &c, 6, a);
}
}

namespace ns_0220f314 {
extern "C" void PlayerActor_AxeSwingTurn(Obj *o) {
    o->turnToward(((AxeSwingWork *)&o->actionWork)->h);  // (void: nothing reads this function's result)
}
}

namespace ns_0220f314 {
extern "C" void PlayerActor_AxeSwingTrackTarget(Obj *o) {
    V3 t;
    V3 *pv = &((AxeSwingWork *)&o->actionWork)->v;
    t.x = pv->x;
    t.y = pv->y;
    t.z = pv->z;
    (o)->setSubCollider(&t, 0xf33, 0x1000);
    ((ActorCollider *)((u8 *)&o->subCollider))->submit();
}
}

namespace ns_0220f314 {
extern "C" void PlayerActor_AxeSwingCheckEnd(Obj *o) {
    if (((AnimFrameCtrl *)((AnimFrameCtrl *)&o->bodyModel))->isFinished()) {
        if (PlayerActor_AxeDispatch(o, 1) == 0) {
            PlayerActor_RequestAxeFollowThrough(o, 6, -1);
        }
    }
}
}

namespace ns_0220f314 {
extern "C" void PlayerActor_AxeSwingCheckEndRemote(Obj *o) {
    o->actionPriority = ((Unk_02007694 *)(void *)(o))->getActionDonePriority(o->action);
    if (((AnimFrameCtrl *)((AnimFrameCtrl *)&o->bodyModel))->isFinished()) {
        PlayerActor_RequestAxeFollowThrough(o, 6, -1);
    }
}
}

namespace ns_0220f314 {
extern "C" void PlayerActor_MainAxeSwing(Obj *o) {
    s32 old = o->speed;
    if (gCommManager->isLocalSlot(o->sessionSlot)) {
        s32 t;
        PlayerActor_AxeSwingTurn(o);
        (o)->advanceAnim();
        (o)->moveWithCollision();
        (o)->updateBodyCollider();
        t = PlayerActor_DecreaseClamped(o->speed, 0, 0x171);
        (o)->setSpeed((u32 *)(void *)(&t));
        PlayerActor_AxeSwingCheckEnd(o);
        PlayerActor_AxeSwingTrackTarget(o);
    } else {
        s32 t2;
        (o)->advanceAnim();
        if ((o)->netFollowTransform()) {
            (o)->moveWithCollision();
        }
        (o)->updateBodyCollider();
        t2 = PlayerActor_DecreaseClamped(o->speed, 0, 0x171);
        (o)->setSpeed((u32 *)(void *)(&t2));
        PlayerActor_AxeSwingCheckEndRemote(o);
    }
    if (old != 0 && o->speed == 0) {
        Effect_Create(0x2c, &o->position, &o->rotY, 0);
    }
}
}

namespace ns_0220f314 {
extern "C" s32 PlayerActor_RequestAxeFollowThrough(Obj *o, s32 a, s16 b) {
    Msg m;
    (&m)->assign(0x47, a, b);
    s32 r = (o)->pushRequest(&m);
    return r;
}
}

namespace ns_0220f314 {
extern "C" void PlayerActor_SetupAxeFollowThrough(Obj *o) {
    o->startAnim(0x43, 3, 0);  // (void: nothing reads this function's result)
}
}

namespace ns_0220f314 {
extern "C" s32 PlayerActor_NetAxeFollowThrough(Obj *o, s16 b) {
    return PlayerActor_RequestAxeFollowThrough(o, 6, b);
}
}

namespace ns_0220f314 {
extern "C" void PlayerActor_AxeFollowThroughTrackTarget(Obj *o) {
    V3 v;
    PlayerActor_GetFrontPoint(&v, o);
    (o)->setSubCollider(&v, 0xf33, 0x1000);
    ((ActorCollider *)((u8 *)&o->subCollider))->submit();
}
}

namespace ns_0220f314 {
extern "C" s32 PlayerActor_AxeDispatch(Obj *o, u8 a, ...) {
    u8 k;
    s32 v[2];
    s32 r;
    Pair t3, t5;
    if (o->speed != 0) {
        return -1;
    }
    r = (o)->getHeldHoldableIndex();
    k = 0;
    if (r != 0xb) {
        if ((o)->testActionFlag(0x14)) {
            if (r != 0xa) {
                k = 1;
            } else {
                k = 2;
            }
        }
    }
    v[0] = 0;
    v[1] = 0;
    r = PlayerActor_AxeClassifyTarget(o, &a, &k, v);
    if (!gCommManager->isLocalSlot(o->sessionSlot)) {
        k = 0;
    }
    switch (r) {
    case 1:
        PlayerActor_RequestAct48(o, 6, -1);
        break;
    case 2:
    case 3:
        if (k == 2) {
            PlayerActor_RequestAxeBreak(o, 1, ns_0220e970::Pv(v[0], v[1]), 6, -1);
        } else {
            BOOL f1 = k ? 1 : 0;
            BOOL f2 = r == 3 ? 1 : 0;
            PlayerActor_RequestAxeChop(o, f1, f2, ns_0220e970::Pv(v[0], v[1]), 6, -1);
        }
        break;
    case 4:
        if (k == 2) {
            PlayerActor_RequestAxeBreak(o, 0, ns_0220e970::Pv(v[0], v[1]), 6, -1);
        } else {
            t3.x = v[0];
            t3.y = v[1];
            PlayerActor_RequestAxeStrike(o, k ? 1 : 0, 0, &t3.x, 6, -1);
        }
        break;
    case 5:
        o->actionPriority = ((Unk_02007694 *)(void *)(o))->getActionDonePriority(o->action);
        (o)->requestWait(3, 1, -1);
        break;
    case 6:
        if (k == 2) {
            PlayerActor_RequestAxeBreak(o, 1, ns_0220e970::Pv(v[0], v[1]), 6, -1);
        } else {
            t5.x = v[0];
            t5.y = v[1];
            PlayerActor_RequestAxeStrike(o, k ? 1 : 0, 1, &t5.x, 6, -1);
        }
        break;
    }
    return r;
}
}

namespace ns_0220f314 {
extern "C" void PlayerActor_MainAxeFollowThrough(Obj *o) {
    (o)->advanceAnim();
    if (gCommManager->isLocalSlot(o->sessionSlot)) {
        s32 t;
        (o)->moveWithCollision();
        (o)->updateBodyCollider();
        t = PlayerActor_Decelerate(o->speed, 0);
        (o)->setSpeed((u32 *)(void *)(&t));
        PlayerActor_AxeDispatch(o, 0);
    } else {
        s32 t2;
        if ((o)->netFollowTransform()) {
            (o)->moveWithCollision();
        }
        (o)->updateBodyCollider();
        t2 = PlayerActor_Decelerate(o->speed, 0);
        (o)->setSpeed((u32 *)(void *)(&t2));
        o->actionPriority = ((Unk_02007694 *)(void *)(o))->getActionDonePriority(o->action);
    }
    PlayerActor_AxeFollowThroughTrackTarget(o);
}
}

namespace ns_0220f314 {
extern "C" s32 PlayerActor_RequestAct48(Obj *o, s32 a, s16 b) {
    Msg m;
    (&m)->assign(0x48, a, b);
    s32 r = (o)->pushRequest(&m);
    return r;
}
}

namespace ns_0220f314 {
extern "C" void PlayerActor_SetupAct48(Obj *o) {
    o->startAnimOnce(0x46, 3, 0);  // (void: nothing reads this function's result)
}
}

namespace ns_0220f314 {
extern "C" s32 PlayerActor_NetAct48(Obj *o, s16 b) {
    return PlayerActor_RequestAct48(o, 6, b);
}
}

namespace ns_0220f314 {
extern "C" void PlayerActor_Act48Update(Obj *o) {
    if (((AnimFrameCtrl *)((AnimFrameCtrl *)&o->bodyModel))->hasPassedFrame(6)) {
        (o)->playSe(0x83d);
    }
    o->advanceAnim();  // (void: nothing reads this function's result)
}
}

namespace ns_0220f314 {
extern "C" void PlayerActor_Act48CheckEnd(Obj *o) {
    if (((AnimFrameCtrl *)((AnimFrameCtrl *)&o->bodyModel))->isFinished()) {
        o->actionPriority = ((Unk_02007694 *)(void *)(o))->getActionDonePriority(o->action);
        (o)->requestWait(3, 1, -1);
    }
}
}

namespace ns_0220f314 {
extern "C" void PlayerActor_MainAct48(Obj *o) {
    PlayerActor_Act48Update(o);
    (o)->updateBodyCollider();
    PlayerActor_Act48CheckEnd(o);
}
}

namespace ns_0220f314 {
extern "C" void PlayerActor_AxeStrikeSetArgs(u8 *d, u32 x, u32 y, s32 *s) {
    d[2] = x;
    d[3] = y;
    d[0] = s[0];
    d[1] = s[1];
}
}

namespace ns_0220f314 {
extern "C" s32 PlayerActor_RequestAxeStrike(Obj *o, u32 a, u32 b, s32 *c, s32 d, s32 e) {
    Msg m;
    Pair t;
    (&m)->assign(0x49, d, *(s16 *)&e);
    t.x = c[0];
    t.y = c[1];
    PlayerActor_AxeStrikeSetArgs(m.unk_0c_b, a, b, &t.x);
    s32 r = (o)->pushRequest(&m);
    return r;
}
}

namespace ns_0220f314 {
extern "C" void PlayerActor_AxeStrikeSetWork(u8 *d, u32 x, u32 y, s32 *s) {
    d[2] = x;
    d[3] = y;
    d[0] = s[0];
    d[1] = s[1];
}
}

namespace ns_0220f314 {
extern "C" void PlayerActor_AxeStrikeSetNetData(u8 *d, u8 a, u8 b, u8 c, u8 e, u8 f) {
    d[0] = a;
    d[1] = b;
    d[2] = c;
    d[3] = e;
    d[4] = f;
}
}

namespace ns_0220f314 {
extern "C" void PlayerActor_AxeStrikeGetNetData(u8 *s, u8 *a, u8 *b, u8 *c, u8 *d, u8 *e) {
    *a = s[0];
    *b = s[1];
    *c = s[2];
    *d = s[3];
    *e = s[4];
}
}

namespace ns_0220f314 {
extern "C" void PlayerActor_SetupAxeStrike(Obj *o, Msg *a) {
    u8 *q = a->unk_0c_b;
    u8 b2 = q[2];
    u8 b3 = q[3];
    u32 b0 = a->unk_0c_b[0];
    u32 b1 = q[1];
    Pair t;
    t.x = b0;
    t.y = b1;
    PlayerActor_AxeStrikeSetWork((u8 *)&o->actionWork, b2, b3, &t.x);
    PlayerActor_AxeStrikeSetNetData(o->netData.bytes, b2, b3, o->toolHitKind, b0, b1);
    (o)->startAnimOnce(0x45, 3, 0);
    if (b3) {
        (o)->setActionFlag(0x1c);
    }
}
}

namespace ns_0220f314 {
extern "C" void PlayerActor_NetAxeStrike(Obj *o, s32 a) {
    struct {
        u8 a, b, c, d, e;
    } l;
    Pair t;
    PlayerActor_AxeStrikeGetNetData(o->netData.bytes, &l.a, &l.b, &l.c, &l.d, &l.e);
    o->toolHitKind = l.c;
    if (l.b == 0) {
        t.x = l.d;
        t.y = l.e;
        PlayerActor_RequestAxeStrike(o, l.a, 0, &t.x, 6, a);
    }
}
}

namespace ns_0220f314 {
extern "C" void PlayerActor_EndAxeStrike(Obj *o) {
    if (!gCommManager->isLocalSlot(o->sessionSlot)) {
        if ((o)->testActionFlag(0x1c)) {
            PlayerActor_AxeStrikeHit(o);
        }
    }
}
}

namespace ns_0220f314 {
extern "C" void PlayerActor_AxeStrikeUpdate(Obj *o) {
    (o)->advanceAnim();
    if (((AnimFrameCtrl *)((AnimFrameCtrl *)&o->bodyModel))->hasPassedFrame(8)) {
        PlayerActor_AxeStrikeHit(o);
    }
}
}

namespace ns_0220e970 {
extern "C" void PlayerActor_AxeStrikeHit(Obj *o) {
    s16 ang;
    u16 buf;
    s32 t = -0x333;
    (o)->setSpeed((u32 *)(void *)(&t));
    Rec *r = (Rec *)&o->actionWork;
    s32 n = (o)->getHeldHoldableIndex();
    if (r->b2) {
        n++;
        if (n == 7) {
            (o)->playSe(0x83e);
        } else if (n == 9) {
            (o)->playSe(0x83f);
        }
    }
    PlayerActor_SetHoldableItem(n, o->sessionSlot);
    (o)->setActionFlag(9);
    if (r->b3) {
        FieldUnitPos q;
        s32 y = r->b1;
        s32 x = r->b0;
        q.x = x;
        q.z = y;
        FieldItemFx_StartStrikeResult(q);
        (o)->clearActionFlag(0x12);
        (o)->clearActionFlag(0x1c);
    }
    switch (o->toolHitKind) {
    case 0: {
        V3 v;
        V3 *pv = (V3 *)&o->position;
        v.x = pv->x;
        v.y = pv->y;
        v.z = pv->z;
        ang = o->rotY;
        s32 idx = ((u16)ang >> 4) * 2;
        s32 s, dz, c, dx;
        c = data_02135f44[idx];
        s = data_02135f44[idx + 1];
        dz = func_01ffcb0c(s, 0xccd) - func_01ffcb0c(c, -0x800);
        dx = func_01ffcb0c(c, 0xccd) + func_01ffcb0c(s, -0x800);
        v.x += dx;
        v.z += dz;
        v.y += 0xccd;
        ang += 0x2000;
        Effect_Create(4, &v, &ang, 0);
        (o)->playSe(0x83c);
        break;
    }
    case 1:
        (o)->playSe(0x7df);
        break;
    case 2:
        (o)->playSe(0x7df);
        if (o->toolHitActor) {
            PlayerActor_GetHeldItem(&buf, o);
            ((NpcActor *)o->toolHitActor)->onToolHit(&buf);
        }
        break;
    case 3:
        if (gCommManager->isLocalSlot(o->sessionSlot)) {
            if (Snowball_Break((void *)o->toolHitActor, 1) == 0) {
                (o)->playSe(0x7df);
            }
        } else {
            (o)->playSe(0x7df);
        }
        break;
    }
    o->toolHitActor = 0;
    o->toolHitKind = 0;
}
}

namespace ns_0220e970 {
extern "C" void PlayerActor_AxeStrikeCheckEnd(Obj *o) {
    if (((AnimFrameCtrl *)((AnimFrameCtrl *)&o->bodyModel))->isFinished()) {
        o->actionPriority = ((Unk_02007694 *)(void *)(o))->getActionDonePriority(o->action);
        (o)->requestWait(3, 1, -1);
    }
}
}

namespace ns_0220e970 {
extern "C" void PlayerActor_MainAxeStrike(Obj *o) {
    PlayerActor_AxeStrikeUpdate(o);
    (o)->moveWithCollision();
    (o)->updateBodyCollider();
    s32 *p = &o->speed;
    s32 v = *p;
    if (v < 0) v = -v;
    s32 t = PlayerActor_Decelerate(v, 0) * -1;
    (o)->setSpeed((u32 *)(void *)(&t));
    PlayerActor_AxeStrikeCheckEnd(o);
}
}

namespace ns_0220e970 {
extern "C" void PlayerActor_AxeChopSetArgs(Rec *r, u32 a, u32 b, Pv p) {
    r->b2 = a;
    r->b3 = b;
    r->b0 = p.x;
    r->b1 = p.z;
}
}

namespace ns_0220e970 {
extern "C" s32 PlayerActor_RequestAxeChop(Obj *o, u32 a, u32 b, Pv p, s32 c, s16 d) {
    Msg m;
    (&m)->assign(0x4a, c, d);
    PlayerActor_AxeChopSetArgs((Unk_ov003_0220e970_Rec *)m.unk_0c_b, a, b, p);
    s32 r = (o)->pushRequest(&m);
    return r;
}
}

namespace ns_0220e970 {
extern "C" void PlayerActor_AxeChopSetWork(Rec *r, u32 a, u32 b, Pv p) {
    r->b2 = a;
    r->b3 = b;
    r->b0 = p.x;
    r->b1 = p.z;
}
}

namespace ns_0220e970 {
extern "C" void PlayerActor_SetupAxeChop(Obj *o, Msg *a) {
    Rec &r = *(Rec *)a->unk_0c_b;
    u32 c = r.b2;
    u32 d = r.b3;
    PlayerActor_AxeChopSetWork((Rec *)&o->actionWork, c, d, Pv(r.b0, r.b1));
    (o)->startAnimOnce(0x44, 3, 0);
    o->setActionFlag(0x1c);  // (void: nothing reads this function's result)
}
}

namespace ns_0220e970 {
extern "C" void PlayerActor_NetAxeChop(Obj *o) {
}
}

namespace ns_0220e970 {
extern "C" void PlayerActor_EndAxeChop(Obj *o) {
    if (!gCommManager->isLocalSlot(o->sessionSlot)) {
        if ((o)->testActionFlag(0x1c)) {
            PlayerActor_AxeChopHit(o);
        }
    }
}
}

namespace ns_0220e970 {
extern "C" void PlayerActor_AxeChopUpdate(Obj *o) {
    (o)->advanceAnim();
    if (((AnimFrameCtrl *)((AnimFrameCtrl *)&o->bodyModel))->hasPassedFrame(8)) {
        PlayerActor_AxeChopHit(o);
    }
}
}

namespace ns_0220e970 {
extern "C" void PlayerActor_AxeChopHit(Obj *o) {
    Rec *r = (Rec *)&o->actionWork;
    s32 n = (o)->getHeldHoldableIndex();
    if (r->b2) {
        n++;
        if (n == 7) {
            (o)->playSe(0x83e);
        } else if (n == 9) {
            (o)->playSe(0x83f);
        }
    }
    PlayerActor_SetHoldableItem(n, o->sessionSlot);
    (o)->setActionFlag(9);
    (o)->playSe(0x83b);
    u32 x = r->b0;
    u32 y = r->b1;
    s16 v;
    V3 pos;
    FieldPos_FromUnitCenter(&pos, r->b0, r->b1);
    v = o->rotY + 0x2000;
    Effect_Create(2, &pos, &v, 0);
    if (pos.x >= o->position.x) {
        P2 a;
        a.x = x;
        a.z = y;
        Tree_RequestChop(o->sessionSlot, &a, 5);
    } else {
        P2 b;
        b.x = x;
        b.z = y;
        Tree_RequestChop(o->sessionSlot, &b, 4);
    }
    o->clearActionFlag(0x1c);  // (void: nothing reads this function's result)
}
}

namespace ns_0220e970 {
extern "C" void PlayerActor_AxeChopCheckEnd(Obj *o) {
    u32 b3 = ((Rec *)&o->actionWork)->b3;
    if (((AnimFrameCtrl *)((AnimFrameCtrl *)&o->bodyModel))->isFinished()) {
        o->actionPriority = ((Unk_02007694 *)(void *)(o))->getActionDonePriority(o->action);
        if (b3) {
            (o)->requestAct77(o->rotY, 6, -1);
        } else {
            (o)->requestWait(3, 1, -1);
        }
    }
    if (b3 == 0) {
        if (gCommManager->isLocalSlot(o->sessionSlot)) {
            if (((Unk_ov003_0220e970_Bits *)&o->bodyModel.curFrame)->mid > 8) {
                PlayerActor_ResumeWalkOrIdle(o);
            }
        }
    }
}
}

namespace ns_0220e970 {
extern "C" void PlayerActor_MainAxeChop(Obj *o) {
    PlayerActor_AxeChopUpdate(o);
    (o)->updateBodyCollider();
    (o)->submitSceneCollider();
    PlayerActor_AxeChopCheckEnd(o);
}
}

namespace ns_0220e970 {
extern "C" void PlayerActor_AxeBreakSetArgs(Rec *r, u32 c, Pv p) {
    r->b2 = c;
    r->b0 = p.x;
    r->b1 = p.z;
}
}

namespace ns_0220e970 {
extern "C" s32 PlayerActor_RequestAxeBreak(Obj *o, u32 a, Pv p, s32 c, s16 e) {
    Msg m;
    (&m)->assign(0x4b, c, e);
    PlayerActor_AxeBreakSetArgs((Unk_ov003_0220e970_Rec *)m.unk_0c_b, a, p);
    s32 r = (o)->pushRequest(&m);
    return r;
}
}

namespace ns_0220e970 {
extern "C" void PlayerActor_AxeBreakSetWork(Rec *r, u32 c, Pv p) {
    r->b2 = c;
    r->b0 = p.x;
    r->b1 = p.z;
}
}

namespace ns_0220e970 {
extern "C" void PlayerActor_AxeBreakSetNetData(u8 *p, u32 v) {
    *p = v;
}
}

namespace ns_0220e970 {
extern "C" void PlayerActor_AxeBreakGetNetData(u8 *src, u8 *dst) {
    *dst = *src;
}
}

namespace ns_0220e970 {
extern "C" void PlayerActor_SetupAxeBreak(Obj *o, Msg *a) {
    Rec *r = (Rec *)a->unk_0c_b;
    u32 c = r->b2;
    PlayerActor_AxeBreakSetWork((Rec *)&o->actionWork, c, Pv(r->b0, r->b1));
    PlayerActor_AxeBreakSetNetData(o->netData.bytes, c);
    (o)->startAnimOnce(0x47, 3, 0);
    if (gCommManager->isLocalSlot(o->sessionSlot)) {
        Bgm_RequestSilence(0x12, 0xf, 0);
    }
}
}

namespace ns_0220e970 {
extern "C" void PlayerActor_NetAxeBreak(Obj *o, s16 a) {
    u8 t;
    PlayerActor_AxeBreakGetNetData(o->netData.bytes, &t);
    if (t == 0) {
        PlayerActor_RequestAxeBreak(o, 0, Pv(0, 0), 6, a);
    }
}
}

namespace ns_0220e970 {
extern "C" void PlayerActor_EndAxeBreak(Obj *o) {
    if (gCommManager->isLocalSlot(o->sessionSlot)) {
        u16 *r = PlayerData_GetBySessionSlot(o->sessionSlot)->getHeldItem();
        if (r) {
            (o)->netSendClothesChange(3, *r);
        }
    } else {
        Rec *r = (Rec *)&o->actionWork;
        if (r->b2) {
            Vec2 q;
            s32 y = r->b1;
            s32 x = r->b0;
            q.x = x;
            q.y = y;
            PendingUnit_ApplyAt(&q, 0);
            (o)->clearActionFlag(0x12);
        }
    }
}
}

namespace ns_0220e970 {
extern "C" void PlayerActor_AxeBreakUpdate(Obj *o) {
    (o)->advanceAnim();
    if (((Unk_ov003_0220e970_Bits *)&o->bodyModel.curFrame)->mid >= 8) {
        if (((AnimFrameCtrl *)((AnimFrameCtrl *)&o->bodyModel))->hasPassedFrame(8)) {
            Rec *r = (Rec *)&o->actionWork;
            if (r->b2) {
                Vec2 q;
                s32 y = r->b1;
                s32 x = r->b0;
                q.x = x;
                q.y = y;
                PendingUnit_ApplyAt(&q, 0);
                (o)->clearActionFlag(0x12);
                r->b2 = 0;
            }
            (o)->playSe(0x840);
            (o)->clearActionFlag(0);
            (o)->setActionFlag(9);
            Mtx t = o->toolHandMtx;
            V3 pos;
            pos.x = t.m[9];
            pos.y = t.m[10];
            pos.z = t.m[11];
            WorldCurve_FromCurved(&pos, &pos);
            Effect_PlayById(0x8c, &pos, 0, 0);
            pos.x = t.m[9];
            pos.y = t.m[10];
            pos.z = t.m[11];
            t.m[9] = t.m[10] = t.m[11] = 0;
            Mtx t2 = t;
            V3 v;
            V3 out;
            v.x = v.y = 0;
            v.z = 0xab8;
            MTX_MultVec43(&v, &t2, &out);
            pos.x = pos.x + out.x;
            pos.y = pos.y + out.y;
            pos.z = pos.z + out.z;
            WorldCurve_FromCurved(&pos, &pos);
            Effect_PlayById(0x8b, &pos, 0, 0);
        }
        (o)->turnToCamera(0x200);
    }
}
}

namespace ns_0220e970 {
extern "C" void PlayerActor_AxeBreakCheckEnd(Obj *o) {
    if (o->rotY == 0) {
        if (((AnimFrameCtrl *)((AnimFrameCtrl *)&o->bodyModel))->isFinished()) {
            if (!gCommManager->isLocalSlot(o->sessionSlot) || TalkRequest_AddPlayerMessage()) {
                PlayerActor_RequestAxeBrokenMessage(o, 6, -1);
            }
        }
    }
}
}

namespace ns_0220e970 {
extern "C" void PlayerActor_MainAxeBreak(Obj *o) {
    PlayerActor_AxeBreakUpdate(o);
    (o)->moveWithCollision();
    (o)->updateBodyCollider();
    PlayerActor_AxeBreakCheckEnd(o);
}
}

namespace ns_0220e970 {
extern "C" s32 PlayerActor_RequestAxeBrokenMessage(Obj *o, s32 a, s16 b) {
    Msg m;
    (&m)->assign(0x4c, a, b);
    s32 r = (o)->pushRequest(&m);
    return r;
}
}

namespace ns_0220e970 {
extern "C" void PlayerActor_AxeBrokenMessageInitWork(Rec *r) {
    r->b0 = 0;
}
}

namespace ns_0220e970 {
extern "C" void PlayerActor_SetupAxeBrokenMessage(Obj *o) {
    struct Pad { s32 v[2]; Pad() {} ~Pad() {} } pad;
    (o)->startAnim(0x48, 3, 0);
    PlayerActor_AxeBrokenMessageInitWork((Rec *)&o->actionWork);
    if (gCommManager->isLocalSlot(o->sessionSlot)) {
        ((Character *)(void *)(o))->attachTalkRequest(o);
        Sec &s = *o;
        ((MsgRequest *)(void *)(&s))->setFileName((const char *)((void *)"obj_etc_player"));
        o->msgIndex = 10;
        o->window->nextState = 1;
        Camera_SetMode4();
        Bgm_ReleasePriority(0x12);
        Bgm_RequestSilence(0xc, 0, 1);
        Bgm_Request(0xd, 0x42, 0x7f, 1);
    }
}
}

namespace ns_0220e970 {
extern "C" s32 PlayerActor_NetAxeBrokenMessage(Obj *o, s16 a) {
    return PlayerActor_RequestAxeBrokenMessage(o, 6, a);
}
}

namespace ns_0220e970 {
extern "C" void PlayerActor_AxeBrokenMessageUpdate(Obj *o) {
    Rec *p = (Rec *)&o->actionWork;
    switch (p->b0) {
    case 0:
        if (o->window) {
            if (o->window->state) {
                p->b0 = 1;
                o->msgStep = 12;
            }
        }
        break;
    case 1:
        if (o->window) {
            if (!o->window->state) {
                ((Character *)(void *)(o))->detachTalkRequest(o);
                (o)->clearActionFlag(0x11);
                TalkRequest_FinishPlayerMessage();
                o->actionPriority = ((Unk_02007694 *)(void *)(o))->getActionDonePriority(o->action);
                PlayerActor_SetHoldableItem(0, o->sessionSlot);
                (o)->setActionFlag(0);
                (o)->requestWait(3, 1, -1);
                Camera_SetModeDefault();
            }
        }
        break;
    }
}
}

namespace ns_0220e970 {
extern "C" void PlayerActor_EndAxeBrokenMessage(Obj *o) {
    if (!gCommManager->isLocalSlot(o->sessionSlot)) {
        PlayerActor_SetHoldableItem(0, o->sessionSlot);
        (o)->setActionFlag(0);
    }
}
}

namespace ns_0220e030 {
extern "C" void PlayerActor_MainAxeBrokenMessage(Obj *o) {
    (o)->advanceAnim();
    (o)->updateBodyCollider();
    if (gCommManager->isLocalSlot(o->sessionSlot)) {
        PlayerActor_AxeBrokenMessageUpdate(o);
    } else {
        o->actionPriority = ((Unk_02007694 *)(void *)(o))->getActionDonePriority(o->action);
    }
}
}

namespace ns_0220e030 {
extern "C" void PlayerActor_FishCastSetArgs(P2 *d, V3 *s) {
    d->x = s->x;
    d->z = s->z;
}
}

namespace ns_0220e030 {
extern "C" s32 PlayerActor_RequestFishCast(Obj *o, VecFx32 *v, s32 a, s16 b) {
    Msg m;
    V3 t;
    (&m)->assign(0x4d, a, b);
    t.x = v->x;
    t.y = v->y;
    t.z = v->z;
    PlayerActor_FishCastSetArgs((VecXZ *)m.unk_0c_b, &t);
    s32 r = (o)->pushRequest(&m);
    return r;
}
}

namespace ns_0220e030 {
extern "C" void PlayerActor_FishCastSetNetData(u8 *p, s32 a, s32 b) {
    NetBuf_PackPair20(p, a, b);
}
}

namespace ns_0220e030 {
extern "C" void PlayerActor_FishCastGetNetData(u8 *p, s32 *a, s32 *b) {
    NetBuf_UnpackPair20(p, (u32 *)a, (u32 *)b);
}
}

namespace ns_0220e030 {
extern "C" void PlayerActor_SetupFishCast(Obj *o, Msg *a) {
    (o)->startAnimOnce(0x52, 3, 0);
    HeldItemModel_PlayAnim(&o->heldItemModel, 0x15, 3, 0);
    P2 &q = *(P2 *)a->unk_0c_b;
    struct { V3 t1, t2; } l;
    s32 z = q.z;
    s32 x = q.x;
    l.t1.x = x; l.t1.y = 0; l.t1.z = z;
    l.t2.x = x; l.t2.y = 0; l.t2.z = z;
    ((FishBobber *)((u8 *)&o->heldItemModel.bobber))->setTargetPos((VecFx32 *)(void *)(&l.t2));
    PlayerActor_FishCastSetNetData(o->netData.bytes, l.t1.x, l.t1.z);
    (*(s16 *)&o->actionWork) = Math_Atan2(l.t1.x - o->position.x, l.t1.z - o->position.z);
}
}

namespace ns_0220e030 {
extern "C" s32 PlayerActor_NetFishCast(Obj *o, s16 a) {
    s32 x, z;
    struct { V3 v1, v2; } l;
    PlayerActor_FishCastGetNetData(o->netData.bytes, &x, &z);
    s32 y = o->position.y;
    l.v1.x = x;
    l.v1.y = y;
    l.v1.z = z;
    l.v2.x = x;
    l.v2.y = y;
    l.v2.z = z;
    return PlayerActor_RequestFishCast(o, &l.v2, 6, a);
}
}

namespace ns_0220e030 {
extern "C" void PlayerActor_FishCastUpdate(Obj *o) {
    (o)->advanceAnim();
    if (((AnimFrameCtrl *)((AnimFrameCtrl *)&o->bodyModel))->hasPassedFrame(0x11)) {
        (o)->playSe(0x849);
    }
}
}

namespace ns_0220e030 {
extern "C" void PlayerActor_FishCastTurn(Obj *o) {
    o->turnToward((*(s16 *)&o->actionWork));  // (void: nothing reads this function's result)
}
}

namespace ns_0220e030 {
extern "C" void PlayerActor_FishCastCheckEnd(Obj *o) {
    if (((AnimFrameCtrl *)((AnimFrameCtrl *)&o->bodyModel))->isFinished()) {
        PlayerActor_RequestFishWait(o, 6, -1);
        if (gCommManager->isLocalSlot(o->sessionSlot)) {
            Camera_SetMode5();
        }
    }
}
}

namespace ns_0220e030 {
extern "C" void PlayerActor_MainFishCast(Obj *o) {
    if (gCommManager->isLocalSlot(o->sessionSlot)) {
        s32 t;
        PlayerActor_FishCastTurn(o);
        PlayerActor_FishCastUpdate(o);
        (o)->moveWithCollision();
        (o)->updateBodyCollider();
        t = PlayerActor_DecreaseClamped(o->speed, 0, 0x171);
        (o)->setSpeed((u32 *)(void *)(&t));
        PlayerActor_FishCastCheckEnd(o);
    } else {
        s32 t2;
        PlayerActor_FishCastUpdate(o);
        if ((o)->netFollowTransform()) {
            (o)->moveWithCollision();
        }
        (o)->updateBodyCollider();
        t2 = PlayerActor_DecreaseClamped(o->speed, 0, 0x171);
        (o)->setSpeed((u32 *)(void *)(&t2));
        PlayerActor_FishCastCheckEnd(o);
    }
}
}

namespace ns_0220e030 {
extern "C" s32 PlayerActor_RequestFishCastFail(Obj *o, s32 a, s16 b) {
    Msg m;
    (&m)->assign(0x4e, a, b);
    s32 r = (o)->pushRequest(&m);
    return r;
}
}

namespace ns_0220e030 {
extern "C" void PlayerActor_SetupFishCastFail(Obj *o) {
    (o)->startAnimOnce(0x53, 3, 0);
    HeldItemModel_PlayAnim(&o->heldItemModel, 0x16, 3, 0);
}
}

namespace ns_0220e030 {
extern "C" s32 PlayerActor_NetFishCastFail(Obj *o, s16 a) {
    return PlayerActor_RequestFishCastFail(o, 6, a);
}
}

namespace ns_0220e030 {
extern "C" void PlayerActor_FishCastFailUpdate(Obj *o) {
    (o)->advanceAnim();
    if (((AnimFrameCtrl *)((AnimFrameCtrl *)&o->bodyModel))->hasPassedFrame(0xc)) {
        (o)->playSe(0x84a);
    }
}
}

namespace ns_0220e030 {
extern "C" void PlayerActor_FishCastFailCheckEnd(Obj *o) {
    if (((AnimFrameCtrl *)((AnimFrameCtrl *)&o->bodyModel))->isFinished()) {
        o->actionPriority = ((Unk_02007694 *)(void *)(o))->getActionDonePriority(o->action);
        (o)->requestWait(3, 1, -1);
    }
}
}

namespace ns_0220e030 {
extern "C" void PlayerActor_MainFishCastFail(Obj *o) {
    PlayerActor_FishCastFailUpdate(o);
    if ((o)->netFollowTransform()) {
        (o)->moveWithCollision();
    }
    (o)->updateBodyCollider();
    s32 t = PlayerActor_DecreaseClamped(o->speed, 0, 0x171);
    (o)->setSpeed((u32 *)(void *)(&t));
    PlayerActor_FishCastFailCheckEnd(o);
}
}

namespace ns_0220e030 {
extern "C" s32 PlayerActor_RequestFishWait(Obj *o, s32 a, s16 b) {
    Msg m;
    (&m)->assign(0x4f, a, b);
    s32 r = (o)->pushRequest(&m);
    return r;
}
}

namespace ns_0220e030 {
extern "C" void PlayerActor_FishWaitSetNetData(u8 *p, V3 *v, u8 c) {
    NetBuf_PackTriple20(p, v->x, v->y, v->z, c);
}
}

namespace ns_0220e030 {
extern "C" void PlayerActor_FishWaitGetNetData(u8 *p, V3 *v, u8 *out) {
    NetBuf_UnpackTriple20(p, (u32 *)&v->x, &v->y, (u32 *)&v->z, out);
}
}

namespace ns_0220e030 {
extern "C" void PlayerActor_SetupFishWait(Obj *o) {
    u8 kind;
    V3 pos;
    V3 cur;
    u8 *p = o->netData.bytes;
    (o)->startAnim(0x54, 3, 0);
    CommManager *g = gCommManager;
    if (!g->isLocalSlot(o->sessionSlot)) {
        PlayerActor_FishWaitGetNetData(p, &pos, &kind);
        if (kind == 3) {
            cur.x = pos.x;
            cur.y = pos.y;
            cur.z = pos.z;
            ((FishBobber *)((u8 *)&o->heldItemModel.bobber))->setPos((VecFx32 *)(void *)(&cur));
        }
    }
    HeldItemModel_PlayAnim(&o->heldItemModel, 0x17, 3, 0);
    if (g->isLocalSlot(o->sessionSlot)) {
        PlayerActor_FishWaitSetNetData(p, (V3 *)&o->heldItemModel.bobber.pos, o->heldItemModel.bobber.curState);
    }
}
}

namespace ns_0220e030 {
extern "C" void PlayerActor_NetFishWait(Obj *o, s16 a) {
    s32 st = o->action;
    if (st != 0x4d) {
        if (st == 0x4f) {
            o->netSeq = a;
        } else {
            PlayerActor_RequestFishWait(o, 6, a);
        }
    }
}
}

namespace ns_0220e030 {
extern "C" void PlayerActor_EndFishWait(Obj *o, s32 a) {
    if (gCommManager->isLocalSlot(o->sessionSlot)) {
        data_021c1b3c->mixer.endFishDuck();
    } else if (a != 0x50 && a != 0x51) {
        ((FishBobber *)((u8 *)&o->heldItemModel.bobber))->setState(1);
    }
}
}

namespace ns_0220e030 {
extern "C" void PlayerActor_FishWaitSyncBobber(Obj *o) {
    u8 kind;
    V3 pos;
    struct { V3 cur, t2, t3; } l;
    u8 *p = o->netData.bytes;
    if (gCommManager->isLocalSlot(o->sessionSlot)) {
        PlayerActor_FishWaitSetNetData(p, (V3 *)&o->heldItemModel.bobber.pos, o->heldItemModel.bobber.curState);
    } else {
        PlayerActor_FishWaitGetNetData(p, &pos, &kind);
        u8 k = kind;
        s32 st = o->heldItemModel.bobber.curState;
        if (st == 4) goto case4;
        if (st == 3) goto end;
        if (k == 4) {
            ((FishBobber *)((u8 *)&o->heldItemModel.bobber))->setState(k);
            l.t2 = pos;
            ((FishBobber *)((u8 *)&o->heldItemModel.bobber))->setPos((VecFx32 *)(void *)(&l.t2));
        } else if (st >= 6) {
            ((FishBobber *)((u8 *)&o->heldItemModel.bobber))->setState(0);
        }
        goto end;
    case4: {
        V3 *pc = (V3 *)&o->heldItemModel.bobber.pos;
        l.cur = *pc;
        s32 dx = l.cur.x - pos.x;
        s32 dz = l.cur.z - pos.z;
        if (dx * dx + dz * dz >= 0x400000) {
            l.t3 = pos;
            ((FishBobber *)((u8 *)&o->heldItemModel.bobber))->setPos((VecFx32 *)(void *)(&l.t3));
        }
        if (k == 5) {
            ((FishBobber *)((u8 *)&o->heldItemModel.bobber))->setState(k);
            (o)->playSe(0x84f);
            o->sePosX = pos.x;
            o->sePosY = pos.y;
            o->sePosZ = pos.z;
        }
    }
    end:;
    }
}
}

namespace ns_0220e030 {
extern "C" void PlayerActor_FishWaitFaceBobber(Obj *o) {
    s16 t;
    V3 *p = (V3 *)&o->heldItemModel.bobber.pos;
    s32 a = Math_Atan2(p->x - o->position.x, p->z - o->position.z);
    t = o->rotY;
    PlayerActor_TurnAngle(&t, a);
    o->setAngleY(&t);  // (void: nothing reads this function's result)
}
}

namespace ns_0220e030 {
extern "C" void PlayerActor_FishWaitOnBobberLand(Obj *o) {
    if (HeldItemModel_IsBobberLanded(&o->heldItemModel) && o->heldItemModel.bobber.justLanded) {
        (o)->playSe(0x84b);
        V3 *p = (V3 *)&o->heldItemModel.bobber.pos;
        o->sePosX = p->x;
        o->sePosY = p->y;
        o->sePosZ = p->z;
        if (gCommManager->isLocalSlot(o->sessionSlot)) {
            data_021c1b3c->mixer.startFishDuck();
        }
    }
}
}

namespace ns_0220e030 {
extern "C" void PlayerActor_FishWaitCheckInput(Obj *o) {
    PlayerActor_FishWaitOnBobberLand(o);
    if (HeldItemModel_IsBobberLanded(&o->heldItemModel)) {
        if ((o)->testActionFlag(0xb)) {
            FishShadow *fish = (FishShadow *)o->heldItemModel.bobber.getFish();
            if (fish) {
                FishShadow_OnRodPulled(fish);
            } else {
                FieldFish_ScareAround((V3 *)&o->heldItemModel.bobber.pos, 0x1000);
            }
            PlayerActor_RequestFishReelIn(o, 0, 6, -1);
        } else {
            if (o->actionPressed != 0 || TouchPick_GetGroundPos(Scene_GetTouchPicker(), NULL)) {
                if (((FishBobber *)((u8 *)&o->heldItemModel.bobber))->tryHook()) {
                    PlayerActor_RequestFishHook(o, 6, -1);
                } else {
                    PlayerActor_RequestFishReelIn(o, 0, 6, -1);
                }
            }
        }
    }
}
}

namespace ns_0220e030 {
extern "C" void PlayerActor_MainFishWait(Obj *o) {
    (o)->advanceAnim();
    PlayerActor_FishWaitSyncBobber(o);
    (o)->netFollowTransform();
    PlayerActor_FishWaitFaceBobber(o);
    (o)->updateBodyCollider();
    if (gCommManager->isLocalSlot(o->sessionSlot)) {
        PlayerActor_FishWaitCheckInput(o);
    } else {
        o->actionPriority = ((Unk_02007694 *)(void *)(o))->getActionDonePriority(o->action);
    }
}
}

namespace ns_0220e030 {
extern "C" s32 PlayerActor_RequestFishHook(Obj *o, s32 a, s16 b) {
    Msg m;
    (&m)->assign(0x50, a, b);
    s32 r = (o)->pushRequest(&m);
    return r;
}
}

namespace ns_0220e030 {
extern "C" void PlayerActor_SetupFishHook(Obj *o) {
    (o)->startAnim(0x55, 3, 0);
    HeldItemModel_PlayAnim(&o->heldItemModel, 0x18, 3, 0);
    if (gCommManager->isLocalSlot(o->sessionSlot)) {
        Bgm_RequestSilence(0x12, 0, 0);
    }
}
}

namespace ns_0220e030 {
extern "C" s32 PlayerActor_NetFishHook(Obj *o, s16 a) {
    return PlayerActor_RequestFishHook(o, 6, a);
}
}

namespace ns_0220e030 {
extern "C" void PlayerActor_FishHookCheckResult(Obj *o) {
    V3 *p = (V3 *)&o->heldItemModel.bobber.pos;
    volatile s32 pad;
    if (gCommManager->isLocalSlot(o->sessionSlot)) {
        switch (((FishBobber *)((u8 *)&o->heldItemModel.bobber))->checkReelResult()) {
        case 0:
            (o)->playSeAt(0x7f6, (VecFx32 *)(p));
            break;
        case 1:
            PlayerActor_RequestFishEscape(o, 6, -1);
            Bgm_ReleasePriority(0x12);
            break;
        case 2:
            PlayerActor_RequestFishLand(o, 6, -1);
            break;
        }
    } else {
        (o)->playSeAt(0x7f6, (VecFx32 *)(p));
    }
}
}

namespace ns_0220e030 {
extern "C" void PlayerActor_MainFishHook(Obj *o) {
    (o)->advanceAnim();
    (o)->netFollowTransform();
    (o)->updateBodyCollider();
    PlayerActor_FishHookCheckResult(o);
}
}

namespace ns_0220e030 {
extern "C" void PlayerActor_FishReelInSetArgs(u8 *p, u32 v) {
    *p = v;
}
}

namespace ns_0220d6f4 {
extern "C" s32 PlayerActor_RequestFishReelIn(Obj *o, u32 a, s32 b, s16 c) {
    Msg m;
    (&m)->assign(0x51, b, c);
    PlayerActor_FishReelInSetArgs(m.unk_0c_b, a);
    s32 r = (o)->pushRequest(&m);
    return r;
}
}

namespace ns_0220d6f4 {
extern "C" void PlayerActor_FishReelInSetWork(u8 *p, u32 v) {
    *p = v;
}
}

namespace ns_0220d6f4 {
extern "C" void PlayerActor_FishReelInSetNetData(u8 *p, u32 v) {
    *p = v;
}
}

namespace ns_0220d6f4 {
extern "C" void PlayerActor_FishReelInGetNetData(u8 *p, u8 *out) {
    *out = *p;
}
}

namespace ns_0220d6f4 {
extern "C" void PlayerActor_SetupFishReelIn(Obj *o, u8 *p) {
    u32 b = ((PlayerActionRequest *)(void *)p)->unk_0c_b[0];
    PlayerActor_FishReelInSetWork((u8 *)&o->actionWork, b);
    PlayerActor_FishReelInSetNetData(o->netData.bytes, b);
    (o)->startAnimOnce(0x56, 3, 0);
    HeldItemModel_PlayAnim(&o->heldItemModel, 0x19, 3, 0);
    (o)->playSe(0x84c);
    (o)->playSe(0x84e);
    s32 *q = (s32 *)&o->heldItemModel.bobber.pos;
    o->sePosX = q[0];
    o->sePosY = q[1];
    o->sePosZ = q[2];
    if (gCommManager->isLocalSlot(o->sessionSlot)) Camera_SetModeDefault();
}
}

namespace ns_0220d6f4 {
extern "C" void PlayerActor_NetFishReelIn(Obj *o, s16 a) {
    u8 t;
    PlayerActor_FishReelInGetNetData(o->netData.bytes, &t);
    PlayerActor_RequestFishReelIn(o, t, 6, a);
}
}

namespace ns_0220d6f4 {
extern "C" void PlayerActor_FishReelInCheckEnd(Obj *o) {
    if (((AnimFrameCtrl *)((AnimFrameCtrl *)&o->bodyModel))->isFinished()) {
        o->actionPriority = ((Unk_02007694 *)(void *)(o))->getActionDonePriority(o->action);
        if (*(u8 *)&o->actionWork) {
            (o)->requestAct79(5, -1);
        } else {
            (o)->requestWait(3, 1, -1);
        }
    }
}
}

namespace ns_0220d6f4 {
extern "C" void PlayerActor_MainFishReelIn(Obj *o) {
    (o)->advanceAnim();
    (o)->netFollowTransform();
    (o)->updateBodyCollider();
    PlayerActor_FishReelInCheckEnd(o);
}
}

namespace ns_0220d6f4 {
extern "C" s32 PlayerActor_RequestFishEscape(Obj *o, s32 a, s16 b) {
    Msg m;
    (&m)->assign(0x52, a, b);
    s32 r = (o)->pushRequest(&m);
    return r;
}
}

namespace ns_0220d6f4 {
extern "C" void PlayerActor_SetupFishEscape(Obj *o) {
    (o)->setSpeed((u32 *)&sFishEscapeSpeed);
    (o)->startAnimOnce(0x57, 3, 0);
    HeldItemModel_PlayAnim(&o->heldItemModel, 0x1a, 3, 0);
}
}

namespace ns_0220d6f4 {
extern "C" s32 PlayerActor_NetFishEscape(Obj *o, s16 a) {
    return PlayerActor_RequestFishEscape(o, 6, a);
}
}

namespace ns_0220d6f4 {
extern "C" void PlayerActor_FishEscapeCheckEnd(Obj *o) {
    if (((AnimFrameCtrl *)((AnimFrameCtrl *)&o->bodyModel))->isFinished()) {
        if (gCommManager->isLocalSlot(o->sessionSlot)) Camera_SetModeDefault();
        o->actionPriority = ((Unk_02007694 *)(void *)(o))->getActionDonePriority(o->action);
        (o)->requestWait(3, 1, -1);
        HeldItemModel_PlayAnim(&o->heldItemModel, 0x13, 3, 0);
    }
}
}

namespace ns_0220d6f4 {
extern "C" void PlayerActor_MainFishEscape(Obj *o) {
    s32 t;
    (o)->advanceAnim();
    if ((o)->netFollowTransform()) (o)->moveWithCollision();
    (o)->updateBodyCollider();
    s32 v = o->speed;
    if (v < 0) v = -v;
    t = PlayerActor_DecreaseClamped(v, 0, 0xc5) * -1;
    (o)->setSpeed((u32 *)(void *)(&t));
    PlayerActor_FishEscapeCheckEnd(o);
}
}

namespace ns_0220d6f4 {
extern "C" s32 PlayerActor_RequestFishLand(Obj *o, s32 a, s16 b) {
    Msg m;
    (&m)->assign(0x53, a, b);
    s32 r = (o)->pushRequest(&m);
    return r;
}
}

namespace ns_0220d6f4 {
extern "C" void PlayerActor_SetupFishLand(Obj *o) {
    (o)->startAnimOnce(0x58, 3, 0);
    HeldItemModel_PlayAnim(&o->heldItemModel, 0x1b, 3, 1);
    (o)->playSe(0x84c);
    (o)->playSe(0x84e);
    s32 *p = (s32 *)&o->heldItemModel.bobber.pos;
    o->sePosX = p[0];
    o->sePosY = p[1];
    o->sePosZ = p[2];
}
}

namespace ns_0220d6f4 {
extern "C" s32 PlayerActor_NetFishLand(Obj *o, s16 a) {
    return PlayerActor_RequestFishLand(o, 6, a);
}
}

namespace ns_0220d6f4 {
extern "C" void PlayerActor_FishLandCheckEnd(Obj *o) {
    if (((AnimFrameCtrl *)((AnimFrameCtrl *)&o->bodyModel))->isFinished()) {
        if (((FishBobber *)((u8 *)&o->heldItemModel.bobber))->isCatchLanded()) PlayerActor_RequestFishShowCatch(o, 0, 6, -1);
    }
}
}

namespace ns_0220d6f4 {
extern "C" void PlayerActor_MainFishLand(Obj *o) {
    (o)->advanceAnim();
    (o)->netFollowTransform();
    (o)->updateBodyCollider();
    if (gCommManager->isLocalSlot(o->sessionSlot)) PlayerActor_FishLandCheckEnd(o);
}
}

namespace ns_0220d6f4 {
extern "C" s32 PlayerActor_RequestFishShowCatch(Obj *o, u32 a, s32 b, s16 c) {
    Msg m;
    (&m)->assign(0x54, b, c);
    m.unk_0c_b[0] = a;
    s32 r = (o)->pushRequest(&m);
    return r;
}
}

namespace ns_0220d6f4 {
extern "C" void PlayerActor_FishShowCatchSetNetData(u8 *p, u32 v) {
    *p = v;
}
}

namespace ns_0220d6f4 {
extern "C" void PlayerActor_FishShowCatchGetNetData(u8 *p, u8 *out) {
    *out = *p;
}
}

namespace ns_0220d6f4 {
extern "C" void PlayerActor_SetupFishShowCatch(Obj *o, u8 *p) {
    u32 b = ((PlayerActionRequest *)(void *)p)->unk_0c_b[0];
    Rec *r = (Rec *)&o->actionWork;
    r->showMode = b;
    r->unk_00 = 0x39;
    r->catalogWasFull = Catalog_HasAllFish();
    if (b == 0) {
        if (gCommManager->isLocalSlot(o->sessionSlot)) {
            Act *a = (Act *)o->heldItemModel.bobber.getFish();
            s8 v = a->fishId;
            r->fishId = v;
            if (v < 0x38) {
                VillagerTrend_OnFishCaught(ACTOR_POS_AT(o));
                ((HudCountdown *)(Hud_GetCountdown()))->incCountA();
            }
        }
        r->step = 0;
        (o)->startAnimOnce(0x59, 3, 0);
        HeldItemModel_PlayAnim(&o->heldItemModel, 0x1c, 3, 1);
    } else {
        r->step = 2;
        r->unk_00 = 0x3a;
        o->msgStep = 8;
    }
    PlayerActor_FishShowCatchSetNetData(o->netData.bytes, b);
}
}

namespace ns_0220d6f4 {
extern "C" void PlayerActor_NetFishShowCatch(Obj *o, s16 a) {
    u8 t;
    PlayerActor_FishShowCatchGetNetData(o->netData.bytes, &t);
    PlayerActor_RequestFishShowCatch(o, t, 6, a);
}
}

namespace ns_0220d6f4 {
extern "C" void PlayerActor_FishShowCatchUpdate(Obj *o) {
    Rec *r6;
    u8 *r5;
    Act *a;
    struct {
        u16 pad0;
        u16 t;
        u32 pad1;
    } tmp;
    Vec2 buf;
    Blk b;
    V3 v48;
    V3 v54;
    V3 v60;
    V3 v6c;
    V3 v78;
    (o)->turnToCamera(0x400);
    r6 = (Rec *)&o->actionWork;
    a = (Act *)o->heldItemModel.bobber.getFish();
    if (a) {
        b = o->itemHandMtx;
        Fish_GetDisplayScale(&v54, a->fishId);
        v48.x = 0x4cd;
        v48.y = 0;
        v48.z = 0;
        u32 m = ((Unk_ov003_0220d6f4_Bits *)&o->bodyModel.curFrame)->mid;
        if (r6->showMode != 0) {
            v48.x = 0x800;
            v48.y = 0x19a;
            v48.z = -0x19a;
        } else if ((s32)m >= 0x10) {
            s32 q = DS_DIV((s32)((m - 15) * 0x19a), (s32)(((Unk_ov003_0220d6f4_Bits *)&o->bodyModel.numFrames)->mid - 0x10));
            v48.x = v48.x + q * 2;
            v48.y = v48.y + q;
            v48.z = v48.z - q;
        }
        PlayerActor_ApplyHoldOffset(&b, &v48);
        v60.x = b.m[9];
        v60.y = b.m[10];
        v60.z = b.m[11];
        WorldCurve_FromCurved(&v60, &v60);
        v6c.x = v60.x;
        v6c.y = v60.y;
        v6c.z = v60.z;
        v78.x = v54.x;
        v78.y = v54.y;
        v78.z = v54.z;
        FishCatch_SetDisplayPosScale(a, &v6c, &v78);
    }
    r5 = &r6->step;
    if (o->animId == 0x59) {
        if (((AnimFrameCtrl *)((AnimFrameCtrl *)&o->bodyModel))->hasPassedFrame(3)) Camera_SetMode4();
    }
    if (((Unk_ov003_0220d6f4_Bits *)&o->bodyModel.curFrame)->mid < 0x11 && *r5 < 5 && r6->showMode == 0) return;
    switch (*r5) {
    case 0:
        if (TalkRequest_AddPlayerMessage()) {
            *r5 = 1;
            ((Character *)(void *)(o))->attachTalkRequest(o);
            (o)->setActionFlag(0x11);
            Sec &s = *o;
            ((MsgRequest *)(void *)(&s))->setFileName((const char *)((void *)"obj_etc_getfish"));
            u32 v = r6->fishId;
            o->msgIndex = v;
            tmp.t = v + 0x12e8;
            Catalog_AddCreature(&tmp.t);
            o->actionItem = o->shownItem = tmp.t;
            o->window->nextState = 1;
            Bgm_ReleasePriority(0x12);
            Bgm_RequestSilence(0xc, 0, 4);
            Bgm_Request(0xd, 0x39, 0x7f, 1);
        }
    case 1:
        if (o->window == 0) return;
        if (o->window->state == 0) return;
        if (Catalog_HasAllFish() && r6->catalogWasFull == 0) {
            *r5 = 5;
            o->msgStep = 0xe;
        } else {
            *r5 = 2;
            o->msgStep = 8;
        }
        return;
    case 2: {
        s32 c = Pocket_FindEmpty();
        if (c == -1) {
            buf.x = 0;
            buf.y = 0;
            if (FieldAction_FindDropUnit(0x10, &buf, 0)) {
                *r5 = 3;
                BOOL in = FALSE;
                u32 h = o->actionItem;
                if (h >= 0x1320 && h <= 0x1322) in = TRUE;
                if (in) {
                    o->msgStep = 4;
                } else {
                    o->msgStep = 0;
                }
            } else {
                *r5 = 4;
                o->msgStep = 1;
            }
        } else {
            if (o->window == 0) return;
            if (o->window->state != 0) return;
            ((Character *)(void *)(o))->detachTalkRequest(o);
            (o)->clearActionFlag(0x11);
            TalkRequest_FinishPlayerMessage();
            PlayerActor_RequestFishStore(o, 0, 6, -1);
            Camera_SetModeDefault();
        }
        return;
    }
    case 3:
        if (o->msgStep >= 0xf) {
            BOOL in = FALSE;
            u32 h = o->actionItem;
            if (h >= 0x1320 && h <= 0x1322) in = TRUE;
            if (in) {
                *r5 = 6;
                if (o->dropQuery != -1) return;
                o->dropQuery = FieldAction_RequestDropForAid(o->sessionSlot, o->actionItem);
                if (o->dropQuery == -1) return;
                *r5 = 4;
            } else {
                *r5 = 4;
            }
        } else {
            if (o->window == 0) return;
            if (o->window->state != 0) return;
            ((Character *)(void *)(o))->detachTalkRequest(o);
            (o)->clearActionFlag(0x11);
            PlayerActor_RequestFishStore(o, 1, 6, -1);
            Camera_SetModeDefault();
        }
        return;
    case 4:
        if (o->window == 0) return;
        if (o->window->state != 0) return;
        ((FishBobber *)((u8 *)&o->heldItemModel.bobber))->startCatchLift();
        ((Character *)(void *)(o))->detachTalkRequest(o);
        (o)->clearActionFlag(0x11);
        TalkRequest_FinishPlayerMessage();
        o->actionPriority = ((Unk_02007694 *)(void *)(o))->getActionDonePriority(o->action);
        PlayerActor_RequestFishRelease(o, (u8 *)&data_ov003_02230ac0, 0, 6, -1);
        Camera_SetModeDefault();
        return;
    case 5:
        if (o->window == 0) return;
        if (o->window->state != 0) return;
        ((Character *)(void *)(o))->detachTalkRequest(o);
        (o)->clearActionFlag(0x11);
        TalkRequest_FinishPlayerMessage();
        (o)->requestAct76(1, r6->fishId, 0, 6, -1);
        return;
    case 6:
        if (o->dropQuery != -1) return;
        o->dropQuery = FieldAction_RequestDropForAid(o->sessionSlot, o->actionItem);
        if (o->dropQuery == -1) return;
        *r5 = 4;
        return;
    }
}
}

namespace ns_0220cd4c {
extern "C" void PlayerActor_MainFishShowCatch(Obj *o) {
    (o)->advanceAnim();
    volatile VecFx32 sv;
    V3 *pv = (V3 *)&o->drawPos;
    sv.x = pv->x;
    sv.y = pv->y;
    sv.z = pv->z;
    s16 h = o->drawTilt;
    Blk b0 = o->bodyModel.mtx;
    Blk b1 = o->itemHandMtx;
    (o)->calcHandMtx();
    (o)->netFollowTransform();
    (o)->updateBodyCollider();
    if (gCommManager->isLocalSlot(o->sessionSlot)) {
        PlayerActor_FishShowCatchUpdate(o);
    } else {
        o->actionPriority = ((Unk_02007694 *)(void *)(o))->getActionDonePriority(o->action);
        (o)->turnToCamera(0x400);
    }
    o->drawPos.x = sv.x;
    o->drawPos.y = sv.y;
    o->drawPos.z = sv.z;
    o->drawTilt = h;
    o->bodyModel.mtx = b0;
    o->itemHandMtx = b1;
}
}

namespace ns_0220cd4c {
extern "C" s32 PlayerActor_RequestFishStore(Obj *o, u32 a, s32 b, s32 c) {
    Msg m;
    (&m)->assign(0x55, b, c);
    *(u8 *)((u8 *)&m.unk_0c) = a;
    s32 r = (o)->pushRequest(&m);
    return r;
}
}

namespace ns_0220cd4c {
extern "C" void PlayerActor_FishStoreSetNetState(u8 *p, u32 v) {
    *p = v;
}
}

namespace ns_0220cd4c {
extern "C" u32 PlayerActor_FishStoreGetNetState(u8 *p) {
    return *p;
}
}

namespace ns_0220cd4c {
extern "C" void PlayerActor_SetupFishStore(Obj *o, u8 *b) {
    u8 v = ((PlayerActionRequest *)(void *)b)->unk_0c_b[0];
    ((BugNetSwingWork *)&o->actionWork)->swingHit = v;
    PlayerActor_FishStoreSetNetState(o->netData.bytes, v);
    (o)->startAnimOnce(0x5a, 3, 3);
    (o)->playSe(0x4f);
}
}

namespace ns_0220cd4c {
extern "C" s32 PlayerActor_NetFishStore(Obj *o, s16 a) {
    return PlayerActor_RequestFishStore(o, PlayerActor_FishStoreGetNetState(o->netData.bytes), 6, a);
}
}

namespace ns_0220cd4c {
extern "C" void PlayerActor_FishStoreUpdate(Obj *o) {
    u16 v[4];
    V3 vf;
    Blk bl;
    V3 v48;
    Act *p5 = (Act *)o->heldItemModel.bobber.getFish();
    s32 f;
    if (o->animId == 0x5a) {
        f = *(s32 *)(Unk_ov003_0220cd4c_Bits *)&o->bodyModel.curFrame >> 12;
        if ((u16)f < 6) {
            if (p5) {
                s32 k = (s32)((u32)data_ov003_022349e6[p5->sizeClass * 20] << 12) / 100;
                f = (s16)(k - k * (u16)f / 6);
            } else {
                f = (s16)(0x1000 - (u16)f * 0x2ab);
            }
        } else {
            f = 0;
        }
    } else {
        f = 0;
    }
    vf.x = f;
    vf.y = f;
    vf.z = f;
    bl = o->itemHandMtx;
    PlayerActor_ApplyHoldOffset(&bl, 0);
    v48.x = ((V3 *)&bl.m[9])->x;
    v48.y = ((V3 *)&bl.m[9])->y;
    v48.z = ((V3 *)&bl.m[9])->z;
    v[0] = 0xfff1;
    if (p5) {
        V3 a, b;
        WorldCurve_FromCurved(&v48, &v48);
        a.x = v48.x;
        a.y = v48.y;
        a.z = v48.z;
        b.x = vf.x;
        b.y = vf.y;
        b.z = vf.z;
        FishCatch_SetDisplayPosScale(p5, &a, &b);
        v[0] = p5->fishId + 0x12e8;
        if (f == 0) FishCatch_NetSendStored(o->sessionSlot);
    }
    u8 *r6 = &((BugNetSwingWork *)&o->actionWork)->swingHit;
    u8 *r7 = o->netData.bytes;
    switch (*r6) {
    case 0:
        if (!((AnimFrameCtrl *)((AnimFrameCtrl *)&o->bodyModel))->isFinished()) return;
        o->actionPriority = ((Unk_02007694 *)(void *)(o))->getActionDonePriority(o->action);
        (o)->requestWait(6, 1, -1);
        if (gCommManager->isLocalSlot(o->sessionSlot)) {
            v[3] = v[0];
            Pocket_AddFoundItem(&v[3]);
        }
        ((FishBobber *)((u8 *)&o->heldItemModel.bobber))->endCatch();
        break;
    case 1:
        if (o->animId == 0x5a) {
            if (!((AnimFrameCtrl *)((AnimFrameCtrl *)&o->bodyModel))->isFinished()) return;
            (o)->startAnim(0x6c, 6, 6);
            HeldItemModel_PlayAnim(&o->heldItemModel, 0x13, 6, 0);
        }
        if (gCommManager->isLocalSlot(o->sessionSlot)) {
            if (o->msgStep == 5) {
                if (!MenuCtrl_OpenPocketsFullFish(v[0], p5)) return;
                o->msgStep = 6;
            } else if (o->msgStep == 6) {
                if (!MenuCtrl_IsFinished()) return;
                o->msgStep = 0xf;
                if (MenuCtrl_IsResultOk()) {
                    ((FishBobber *)((u8 *)&o->heldItemModel.bobber))->setFish((void *)(0));
                    if ((o)->testActionFlag(0x11)) {
                        (o)->clearActionFlag(0x11);
                        ((Character *)(void *)(o))->detachTalkRequest(o);
                    }
                    TalkRequest_FinishPlayerMessage();
                    v[1] = MenuCtrl_GetPocketsFullItem();
                    o->actionPriority = ((Unk_02007694 *)(void *)(o))->getActionDonePriority(o->action);
                    if (Rng(&v[1], 0x12e8, 0x131f)) {
                        PlayerActor_RequestFishRelease(o, (u8 *)&data_ov003_02230ac4, 0, 6, -1);
                    } else {
                        (o)->requestWait(6, 1, -1);
                        *r6 = 2;
                    }
                    return;
                }
                goto l378;
            } else if (o->msgStep >= 0xf) {
            l378:
                if (Rng(&v[0], 0x1320, 0x1322)) {
                    *r6 = 3;
                    if (o->dropQuery == -1) {
                        o->dropQuery = FieldAction_RequestDropForAid(o->sessionSlot, v[0]);
                        if (o->dropQuery != -1) *r6 = 2;
                    }
                } else {
                    if (p5) {
                        V3 a, b;
                        Fish_GetDisplayScale(&vf, p5->fishId);
                        a.x = v48.x;
                        a.y = v48.y;
                        a.z = v48.z;
                        b.x = vf.x;
                        b.y = vf.y;
                        b.z = vf.z;
                        FishCatch_SetDisplayPosScale(p5, &a, &b);
                    }
                    PlayerActor_RequestFishRelease(o, (u8 *)&data_ov003_02230ac8, 0, 6, -1);
                    PlayerActor_FishStoreSetNetState(r7, 2);
                }
                ((FishBobber *)((u8 *)&o->heldItemModel.bobber))->startCatchLift();
                if ((o)->testActionFlag(0x11)) {
                    (o)->clearActionFlag(0x11);
                    ((Character *)(void *)(o))->detachTalkRequest(o);
                }
                TalkRequest_FinishPlayerMessage();
            }
        } else {
            o->actionPriority = ((Unk_02007694 *)(void *)(o))->getActionDonePriority(o->action);
            if (PlayerActor_FishStoreGetNetState(r7) == 2) {
                if (p5) {
                    V3 a, b;
                    Fish_GetDisplayScale(&vf, p5->fishId);
                    a.x = v48.x;
                    a.y = v48.y;
                    a.z = v48.z;
                    b.x = vf.x;
                    b.y = vf.y;
                    b.z = vf.z;
                    FishCatch_SetDisplayPosScale(p5, &a, &b);
                }
                ((FishBobber *)((u8 *)&o->heldItemModel.bobber))->startCatchLift();
                *r6 = 2;
                (o)->startAnim(0, 6, 6);
            }
        }
        break;
    case 2:
        o->actionPriority = ((Unk_02007694 *)(void *)(o))->getActionDonePriority(o->action);
        (o)->requestWait(6, 1, -1);
        break;
    case 3:
        if (o->dropQuery == -1) {
            o->dropQuery = FieldAction_RequestDropForAid(o->sessionSlot, v[0]);
            if (o->dropQuery != -1) *r6 = 2;
        }
        break;
    }
}
}

namespace ns_0220cd4c {
extern "C" void PlayerActor_FishStoreCheckEndRemote(Obj *o) {
    if (((AnimFrameCtrl *)((AnimFrameCtrl *)&o->bodyModel))->isFinished()) {
        o->actionPriority = ((Unk_02007694 *)(void *)(o))->getActionDonePriority(o->action);
        (o)->startAnim(0x6c, 6, 6);
        HeldItemModel_PlayAnim(&o->heldItemModel, 0x13, 6, 0);
    }
}
}

namespace ns_0220cd4c {
extern "C" void PlayerActor_MainFishStore(Obj *o) {
    (o)->advanceAnim();
    (o)->netFollowTransform();
    (o)->updateBodyCollider();
    if (gCommManager->isLocalSlot(o->sessionSlot)) {
        PlayerActor_FishStoreUpdate(o);
    } else {
        PlayerActor_FishStoreCheckEndRemote(o);
    }
}
}

namespace ns_0220cd4c {
extern "C" s32 PlayerActor_RequestBugNetSwing(Obj *o, s32 a, s32 b) {
    Msg m;
    u16 *p = (u16 *)m.unk_0c_b;
    (&m)->assign(0x56, a, b);
    if (o->toolTargetKind == 2) {
        *p = Math_Atan2(o->toolTargetPos.x - o->bodyPos.x, o->toolTargetPos.z - o->bodyPos.z);
    } else {
        *p = o->rotY;
    }
    s32 r = (o)->pushRequest(&m);
    return r;
}
}

namespace ns_0220cd4c {
extern "C" void PlayerActor_SetupBugNetSwing(Obj *o, u8 *b) {
    BugNetSwingWork *r = (BugNetSwingWork *)&o->actionWork;
    u32 z = 0;
    r->swingHit = z;
    r->targetAngle = *(s16 *)(((PlayerActionRequest *)(void *)b)->unk_0c_b);
    r->caughtInsect = 0xff;
    (o)->startAnimOnce(0x5b, 3, z);
    HeldItemModel_PlayAnim(&o->heldItemModel, 3, 3, 0);
}
}

namespace ns_0220cd4c {
extern "C" s32 PlayerActor_NetBugNetSwing(Obj *o, s16 b) {
    return PlayerActor_RequestBugNetSwing(o, 6, b);
}
}

namespace ns_0220cd4c {
extern "C" void PlayerActor_BugNetSwingAdvanceAnim(Obj *o) {
    BOOL r;
    if (((AnimFrameCtrl *)((AnimFrameCtrl *)&o->bodyModel))->isFinished())
        r = TRUE;
    else
        r = FALSE;
    (o)->advanceAnim();
    if (r == 0) {
        if (((AnimFrameCtrl *)((AnimFrameCtrl *)&o->bodyModel))->isFinished()) (o)->playSe(0x848);
    }
}
}

namespace ns_0220cd4c {
extern "C" void PlayerActor_BugNetSwingTurn(Obj *o) {
    (o)->turnToward(*(s16 *)&o->actionWorkRaw[2]);
}
}

namespace ns_0220cd4c {
extern "C" void PlayerActor_BugNetSwingTrackTarget(Obj *o) {
    V3 v;
    PlayerActor_GetFrontPoint(&v, o);
    (o)->setSubCollider(&v, 0x99a, 0x1000);
    o->subCollider.submit();
}
}

namespace ns_0220cd4c {
extern "C" void PlayerActor_BugNetSwingUpdate(Obj *o) {
    struct {
        u8 a, b, c, pad;
        volatile u16 w;
    } st;
    V3 v0c, v18, v24, v30, v3c;
    volatile VecFx32 v48;
    V3 v54;
    BugNetSwingWork *r5;
    if (o->bodyModel.frameStep == 0) return;
    if (((AnimFrameCtrl *)((AnimFrameCtrl *)&o->bodyModel))->isFinished()) return;
    if (((Unk_ov003_0220cd4c_Bits *)&o->bodyModel.curFrame)->mid < 3) return;
    st.a = 0;
    st.b = 0;
    v0c = V3(o->toolHandMtx.m[9], o->toolHandMtx.m[10], o->toolHandMtx.m[11]);
    WorldCurve_FromCurved(&v0c, &v0c);
    v18.x = v0c.x;
    v18.y = v0c.y;
    v18.z = v0c.z;
    PlayerActor_BugNetSwingGetSweep(o, &v18, &v24, &v30);
    r5 = (BugNetSwingWork *)&o->actionWork;
    if (r5->swingHit == 0 && ((Unk_ov003_0220cd4c_Bits *)&o->bodyModel.curFrame)->mid < 6) {
        PlayerActor_GetHeldItem((u16 *)&st.w, o);
        s32 id;
        if (Rng(&st.w, 0x1376, 0x1376))
            id = 0xa00;
        else
            id = 0xd1f;
        if (BugNet_HitTest(&v18, &v24, id, &st.c)) {
            st.a = 1;
            FieldInsect_GetPosAndKind(&v3c, st.c);
            st.b = FieldInsect_IsTreeKind(st.c);
            if (st.b != 0) v3c.z += 0x100;
        }
    }
    switch (PlayerActor_BugNetSwingCheckHit(o, &st.a, &v3c, &st.b)) {
    case 1:
        if (Insect_TryCatch(st.c) == 0) return;
        r5->swingHit = 1;
        r5->caughtInsect = st.c;
        (o)->playSe(0x847);
        return;
    case 2:
        return;
    case 3:
        if (r5->swingHit != 0) return;
        r5->swingHit = 1;
        r5->caughtInsect = 0xff;
        (o)->playSe(0x847);
        return;
    case 0:
    default:
        break;
    }
    v54.x = v0c.x;
    v54.y = v0c.y;
    v54.z = v0c.z;
    s32 r = PlayerActor_BugNetSwingCheckGround(o, &v54, &v24, &v30);
    if (st.a == 0) return;
    s32 ang;
    if (r) {
        s32 dx = v3c.x - v30.x;
        v48.x = dx;
        s32 dz = v3c.z - v30.z;
        v48.z = dz;
        ang = Math_Atan2(dx, dz);
    } else {
        ang = 0x7fff;
    }
    if (Math_AngleDiffAbs(ang, 0) > 0x4000) {
        if (Insect_TryCatch(st.c) == 0) return;
        r5->swingHit = 1;
        r5->caughtInsect = st.c;
        (o)->playSe(0x847);
    }
}
}

namespace ns_0220c448 {
extern "C" void PlayerActor_BugNetSwingGetSweep(Obj *o, V3 *a, V3 *b, V3 *out) {
    V3 *pos = (V3 *)&o->position;
    b->x = pos->x;
    b->y = pos->y;
    b->z = pos->z;
    s32 idx = ((u16)o->rotY >> 4) * 2;
    s32 sn = data_02135f44[idx];
    s32 cs = data_02135f44[idx + 1];
    switch (((Unk_ov003_0220c448_Bits *)&o->bodyModel.curFrame)->mid) {
    case 3:
        (o)->playSe(0x845);
        a->y += 0x1dd3;
        b->x += func_01ffcb0c(sn, 0x1e66);
        b->y += 0x24cd;
        b->z += func_01ffcb0c(cs, 0x1e66);
        break;
    case 4:
        a->x = pos->x;
        a->y = pos->y;
        a->z = pos->z;
        a->x += func_01ffcb0c(sn, 0x1e66);
        a->y += 0x24cd;
        a->z += func_01ffcb0c(cs, 0x1e66);
        b->x += func_01ffcb0c(sn, 0x2a66);
        b->z += func_01ffcb0c(cs, 0x2a66);
        break;
    case 5:
        a->x = pos->x;
        a->y = pos->y;
        a->z = pos->z;
        a->x += func_01ffcb0c(sn, 0x2a66);
        a->z += func_01ffcb0c(cs, 0x2a66);
        b->x += func_01ffcb0c(sn, 0x2800);
        b->y -= 0x666;
        b->z += func_01ffcb0c(cs, 0x2800);
        break;
    }
    Mtx43 l = o->heldItemJointMtx;
    V3 t(l.m[9], l.m[10], l.m[11]);
    WorldCurve_FromCurved(&t, &t);
    *out = t;
}
}

namespace ns_0220c448 {
extern "C" s32 PlayerActor_BugNetSwingCheckHit(Obj *o, u8 *a, V3 *b, u8 *c) {
    V3 v;
    u32 t;
    if (((AnimFrameCtrl *)((AnimFrameCtrl *)&o->bodyModel))->hasPassedFrame(4)) {
        if (PlayerActor_CheckToolHitActor(o)) {
            (o)->setActionFlag(9);
            (o)->playSe(0x846);
            o->bodyModel.frameStep = 0;
            HeldItemModel_PlayAnim(&o->heldItemModel, 4, 3, 1);
            if (o->toolHitKind == 4) {
                return 3;
            }
            if (a[0] != 0) {
                return 1;
            }
            return 2;
        }
        PlayerActor_OffsetByAngle(&v, o, &o->position, &o->rotY, &data_ov003_02230adc);
        FieldPos_SnapToUnitCenter(&v, &v);
        if (Ground_GetHeightAt(&v, &t, 0x19) >= 0x400) {
            (o)->setActionFlag(9);
            (o)->playSe(0x846);
            o->bodyModel.frameStep = 0;
            HeldItemModel_PlayAnim(&o->heldItemModel, 4, 3, 1);
            if (a[0] != 0) {
                if (PlayerActor_BugNetSwingCanReach(o, &v, b, c)) {
                    return 1;
                }
                return 2;
            }
            return 2;
        }
    } else if (((AnimFrameCtrl *)((AnimFrameCtrl *)&o->bodyModel))->hasPassedFrame(3)) {
        if (a[0] != 0) {
            V3 w;
            u32 u;
            PlayerActor_OffsetByAngle(&w, o, &o->position, &o->rotY, &data_ov003_02230ae0);
            FieldPos_SnapToUnitCenter(&w, &w);
            if (Ground_GetHeightAt(&w, &u, 0x19) >= 0x400) {
                if (PlayerActor_BugNetSwingCanReach(o, &w, b, c)) {
                    return 1;
                }
                return 2;
            }
        }
    }
    return 0;
}
}

namespace ns_0220c448 {
extern "C" s32 PlayerActor_BugNetSwingCanReach(Obj *o, V3 *a, V3 *b, u8 *c) {
    volatile V3 base;
    V3 *pv = (V3 *)&o->position;
    base.x = pv->x;
    base.y = pv->y;
    base.z = pv->z;
    u16 *cell = BlockMap_GetItemPtrAtPos(gSceneBlockMap, a, 0);
    if (cell) {
        if (*cell == 0x1b || *cell == 0x89) {
            return 1;
        }
    }
    if (c[0] == 0) {
        s32 a4 = Math_Atan2(b->x - base.x, b->z - base.z);
        s32 a5 = Math_Atan2(a->x - base.x, a->z - base.z);
        s32 t1 = (s16)Math_AngleDiffAbs(a4, 0);
        s32 t2 = (s16)Math_AngleDiffAbs(a5, 0);
        if (t1 == t2) {
            s32 z = b->z;
            s32 bz = base.z;
            if ((bz >= z && z > a->z) || (bz <= z && z < a->z)) {
                return 1;
            }
            return 0;
        }
        if ((t1 >= 0x4000 && t2 > t1) || (t2 < 0x4000 && t2 <= t1)) {
            return 1;
        }
        s32 dx = a->x - base.x;
        s32 dz = a->z - base.z;
        s32 d2 = dx * dx + dz * dz;
        s16 k = data_02135f44[(((u16)(s16)(t1 - t2)) >> 4) * 2 + 1];
        s32 q = func_01ffcb0c(d2, k);
        q = func_01ffcb0c(q, k);
        if (d2 - q < 0x900000) {
            return 0;
        }
        return 1;
    } else {
        s32 z = b->z;
        s32 bz = base.z;
        if ((bz >= z && z > a->z) || (bz <= z && z < a->z)) {
            return 1;
        }
        return 0;
    }
}
}

namespace ns_0220c448 {
extern "C" s32 PlayerActor_BugNetSwingCheckWall(Obj *o) {
    V3 v;
    u32 t;
    if (((AnimFrameCtrl *)((AnimFrameCtrl *)&o->bodyModel))->hasPassedFrame(4)) {
        if (PlayerActor_CheckToolHitActor(o)) {
            (o)->setActionFlag(9);
            (o)->playSe(0x846);
            o->bodyModel.frameStep = 0;
            HeldItemModel_PlayAnim(&o->heldItemModel, 4, 3, 1);
            return 1;
        }
        PlayerActor_OffsetByAngle(&v, o, &o->position, &o->rotY, &data_ov003_02230ad4);
        if (Ground_GetHeightAt(&v, &t, 0x19) >= 0x400) {
            (o)->setActionFlag(9);
            (o)->playSe(0x846);
            o->bodyModel.frameStep = 0;
            HeldItemModel_PlayAnim(&o->heldItemModel, 4, 3, 1);
            return 1;
        }
    }
    return 0;
}
}

namespace ns_0220c448 {
extern "C" s32 PlayerActor_BugNetSwingCheckGround(Obj *o, V3 *a, V3 *b, V3 *c) {
    SegmentHitResult l;
    CollisionTag_Construct(&l);
    s32 x, y;
    if (Collision_TestSegment(&l, c, a, 0x1b)) {
    L_ok:
        (o)->setActionFlag(9);
        (o)->playSe(0x846);
        o->bodyModel.frameStep = 0;
        HeldItemModel_PlayAnim(&o->heldItemModel, 4, 3, 1);
        if (((AnimFrameCtrl *)((AnimFrameCtrl *)&o->bodyModel))->hasPassedFrame(5)) {
            Vec2 p;
            x = 0;
            y = 0;
            FieldPos_ToUnit(&x, &y, c);
            p.x = x;
            p.y = y;
            Flower_PlayTrampleFx(&p);
        }
        if (((Unk_ov003_0220c448_Bits *)&o->bodyModel.curFrame)->mid <= 4) {
            CollisionTag_Destruct(&l);
            return 1;
        }
    } else {
        if (((AnimFrameCtrl *)((AnimFrameCtrl *)&o->bodyModel))->hasPassedFrame(5) == 0) {
            goto end0;
        }
        if (gCommManager->isLocalSlot(o->sessionSlot) == 0) {
            GroundInfoBase q;
            ((GroundInfo *)(void *)(&q))->initAtPos((VecFx32 *)(b), 0, 0);
            if (q.waterKind == 0) {
                *c = *b;
                GroundInfo_Destruct(&q);
                goto L_ok;
            }
            GroundInfo_Destruct(&q);
        }
        Effect_Create(0xb, b, 0, 0);
        (o)->setActionFlag(9);
        HeldItemModel_PlayAnim(&o->heldItemModel, 4, 3, 1);
    }
end0:
    CollisionTag_Destruct(&l);
    return 0;
}
}

namespace ns_0220c448 {
extern "C" void PlayerActor_BugNetSwingUpdateRemote(Obj *o) {
    if (o->bodyModel.frameStep != 0 && ((Unk_ov003_0220c448_Bits *)&o->bodyModel.curFrame)->mid >= 3) {
        if (PlayerActor_BugNetSwingCheckWall(o) == 0) {
            V3 a(o->toolHandMtx.m[9], o->toolHandMtx.m[10], o->toolHandMtx.m[11]);
            V3 b, c, d, e;
            WorldCurve_FromCurved(&a, &a);
            b = a;
            PlayerActor_BugNetSwingGetSweep(o, &b, &c, &d);
            e = a;
            PlayerActor_BugNetSwingCheckGround(o, &e, &c, &d);
        }
    }
}
}

namespace ns_0220c448 {
extern "C" void PlayerActor_BugNetSwingDecelerate(Obj *o) {
    if (o->speed != 0) {
        s32 t = PlayerActor_DecreaseClamped(o->speed, 0, 0x171);
        (o)->setSpeed((u32 *)(void *)(&t));
        if (o->speed == 0) {
            Effect_Create(0x2b, (V3 *)&o->position, &o->rotY, 0);
        }
    }
}
}

namespace ns_0220c448 {
extern "C" void PlayerActor_BugNetSwingCheckEnd(Obj *o) {
    if (((AnimFrameCtrl *)((AnimFrameCtrl *)&o->bodyModel))->isFinished() == 0 && o->bodyModel.frameStep != 0) {
        return;
    }
    u8 *p = &o->actionWorkRaw[0];
    if (p[0] != 0) {
        u32 id = p[4];
        if (id == 0xff) {
            PlayerActor_RequestInsectShowCatch(o, 0, id, 0x30, 6, -1);
        } else {
            s32 r = Insect_GetCatchResult(id);
            if (r == 0) {
                u32 t = (u8)Insect_FinishCatch(id);
                PlayerActor_RequestInsectShowCatch(o, 0, id, t, 6, -1);
            } else if (r == 1) {
                p[0] = 0;
            }
        }
    } else {
        if (((AnimFrameCtrl *)(BlendAnimModel *)HeldItemModel_GetModel((u8 *)&o->heldItemModel))->isFinished()) {
            o->actionPriority = ((Unk_02007694 *)(void *)(o))->getActionDonePriority(o->action);
            if (gCommManager->isLocalSlot(o->sessionSlot)) {
                (o)->requestWait(9, 1, -1);
            }
        }
    }
}
}

namespace ns_0220c448 {
extern "C" void PlayerActor_MainBugNetSwing(Obj *o) {
    if (gCommManager->isLocalSlot(o->sessionSlot)) {
        PlayerActor_BugNetSwingTurn(o);
        PlayerActor_BugNetSwingUpdate(o);
        PlayerActor_BugNetSwingAdvanceAnim(o);
        (o)->moveWithCollision();
        (o)->updateBodyCollider();
        PlayerActor_BugNetSwingDecelerate(o);
        PlayerActor_BugNetSwingCheckEnd(o);
        PlayerActor_BugNetSwingTrackTarget(o);
    } else {
        PlayerActor_BugNetSwingUpdateRemote(o);
        PlayerActor_BugNetSwingAdvanceAnim(o);
        if ((o)->netFollowTransform()) {
            (o)->moveWithCollision();
        }
        (o)->updateBodyCollider();
        PlayerActor_BugNetSwingDecelerate(o);
        PlayerActor_BugNetSwingCheckEnd(o);
        PlayerActor_BugNetSwingTrackTarget(o);
    }
    u8 *p = &o->actionWorkRaw[0];
    V3 a, b(o->heldItemJointMtx.m[9], o->heldItemJointMtx.m[10], o->heldItemJointMtx.m[11]);
    WorldCurve_FromCurved(&a, &b);
    *(V3 *)(p + 8) = a;
}
}

namespace ns_0220c448 {
extern "C" s32 PlayerActor_RequestInsectShowCatch(Obj *o, u32 a, u32 b, u32 c, s32 d, s16 e) {
    Msg m;
    (&m)->assign(0x57, d, e);
    Rec &q = (*(Unk_ov003_0220c4ac_Rec *)m.unk_0c_b);
    q.c = a;
    q.a = b;
    q.b = c;
    s32 r = (o)->pushRequest(&m);
    return r;
}
}

namespace ns_0220c448 {
extern "C" void PlayerActor_InsectShowCatchSetNetData(u8 *p, u32 a, u32 b, u32 c, u8 d, s16 e) {
    p[0] = a;
    p[1] = b;
    p[2] = c;
    p[3] = d;
    PlayerActor_InsectShowCatchSetNetAngle(p, e);
}
}

namespace ns_0220c448 {
extern "C" void PlayerActor_InsectShowCatchSetNetState(u8 *p, u32 a) {
    p[0] = a;
}
}

namespace ns_0220c448 {
extern "C" void PlayerActor_InsectShowCatchSetNetAngle(void *p, s32 a) {
    NetBuf_WriteS16B((u8 *)p + 4, a);
}
}

namespace ns_0220c448 {
extern "C" void PlayerActor_InsectShowCatchGetNetData(u8 *p, u8 *a, u8 *b, u8 *c, u8 *d, u16 *e) {
    *a = p[0];
    *b = p[1];
    *c = p[2];
    *d = p[3];
    *e = PlayerActor_InsectShowCatchGetNetAngle(p);
}
}

namespace ns_0220c448 {
extern "C" u32 PlayerActor_InsectShowCatchGetNetState(u8 *p) {
    return p[0];
}
}

namespace ns_0220c448 {
extern "C" u32 PlayerActor_InsectShowCatchGetNetAngle(void *p) {
    return NetBuf_ReadS16B((u8 *)p + 4);
}
}

namespace ns_0220ba90 {
extern "C" void PlayerActor_SetupInsectShowCatch(Obj *o, Msg *r) {
    u8 *q = &((Unk_ov003_0220bc84_Pair *)r->unk_0c_b)->a;
    u32 a = q[0];
    u32 b = q[1];
    u32 c = q[2];
    if (c == 0) {
        (o)->startAnimOnce(0x5c, 3, 0);
        HeldItemModel_PlayAnim(&o->heldItemModel, 5, 3, 1);
    }
    State *rec = (State *)&o->actionWork;
    rec->insectSlot = a;
    rec->insectKind = b;
    rec->showMode = c;
    rec->unk_04 = 0x39;
    rec->catalogWasFull = Catalog_HasAllInsects();
    if (c != 0) {
        if (c == 3) {
            rec->step = 3;
            rec->unk_04 = 0x3b;
            o->msgStep = 8;
        } else {
            if (o->animId == 0x6c) {
                rec->showMode = 2;
                (o)->startAnim(0, 3, 3);
            }
            rec->step = 7;
        }
    } else {
        if (gCommManager->isLocalSlot(o->sessionSlot)) {
            Bgm_RequestSilence(0x12, 0, 0);
            ((HudCountdown *)(Hud_GetCountdown()))->incCountB();
            VillagerTrend_OnInsectCaught(&o->position);
        }
        rec->step = 0;
    }
    rec->effect = -1;
    if (gCommManager->isLocalSlot(o->sessionSlot)) {
        PlayerActor_InsectShowCatchSetNetData(o->netData.bytes, rec->step, a, b, c, 0);
    }
}
}

namespace ns_0220ba90 {
extern "C" void PlayerActor_NetInsectShowCatch(Obj *o, s32 a) {
    u8 buf[4];
    u16 e;
    PlayerActor_InsectShowCatchGetNetData(o->netData.bytes, &buf[0], &buf[1], &buf[2], &buf[3], &e);
    PlayerActor_RequestInsectShowCatch(o, buf[3], buf[1], buf[2], 6, a);
}
}

namespace ns_0220ba90 {
extern "C" void PlayerActor_EndInsectShowCatch(Obj *o) {
    if (((State *)&o->actionWork)->effect != -1) {
        Effect_End(((State *)&o->actionWork)->effect);
    }
    o->headPitch = 0;
    o->headYaw = 0;
}
}

namespace ns_0220ba90 {
extern "C" void PlayerActor_InsectShowCatchUpdate(Obj *o) {
    struct {
        s16 unk0;
        u16 item;
        s16 scale[3];
        Vec2 pr;
    } L2;
    T48 t;
    V3 vv;
    T48 t2;
    V3 d;
    V3 w;
    V3 pos;
    State *rec;
    u8 *sub;
    u32 mode;
    u8 *p7;
    u8 b8;
    CommManager *g;
    rec = (State *)&o->actionWork;
    sub = o->netData.bytes;
    mode = rec->showMode;
    u8 *st = &rec->step;
    p7 = &rec->insectSlot;
    b8 = rec->insectKind;
    if (mode == 0) {
        (o)->turnToCamera(0x400);
    }
    s32 r7 = o->sessionSlot;
    if (*st == 0) {
        if (gCommManager->isLocalSlot(r7)) {
            *st = 1;
        } else {
            *st = 9;
            if (*p7 != 0xff) {
                Insect_FinishCatch(*p7);
            }
        }
        u32 bt = b8;
        if (bt == 0x18 || bt == 0x30 || (u8)(bt + 0xce) <= 1) {
            t = o->itemHandMtx;
            PlayerActor_ApplyHoldOffset(&t, 0);
            vv.x = ((V3 *)((u8 *)&t + 0x24))->x;
            vv.y = ((V3 *)((u8 *)&t + 0x24))->y;
            vv.z = ((V3 *)((u8 *)&t + 0x24))->z;
            WorldCurve_FromCurved(&vv, &vv);
            rec->effect = Effect_Create(0x25, &vv, 0, 0);
        }
        HeldInsect_Start(b8, (u8)r7);
    }
    t2 = o->itemHandMtx;
    d.x = 0x4cd;
    d.y = 0;
    d.z = 0;
    if (o->animId != 0x5c) {
        d.x = d.x + 0x333;
        d.y = d.y + 0x19a;
        d.z = d.z - 0x19a;
    } else {
        s32 f = ((Unk_ov003_0220bc84_Bits *)&o->bodyModel.curFrame)->f;
        if (f >= 0x24) {
            s32 num = (f - 0x23) * 0x19a;
            s32 den = ((Unk_ov003_0220bc84_Bits *)&o->bodyModel.numFrames)->f - 0x24;
            s32 e = DS_DIV(num, den);
            d.x = d.x + e * 2;
            d.y = d.y + e;
            d.z = d.z - e;
        }
    }
    PlayerActor_ApplyHoldOffset(&t2, &d);
    if (rec->effect != -1) {
        w.x = ((V3 *)((u8 *)&t2 + 0x24))->x;
        w.y = ((V3 *)((u8 *)&t2 + 0x24))->y;
        w.z = ((V3 *)((u8 *)&t2 + 0x24))->z;
        WorldCurve_FromCurved(&w, &w);
        Effect_SetPosition(rec->effect, &w, 0, 0);
    }
    L2.scale[0] = 0;
    L2.scale[1] = 0;
    L2.scale[2] = 0;
    u32 fr = ((Unk_ov003_0220bc84_Bits *)&o->bodyModel.curFrame)->f;
    if (fr < 0x17 && o->animId == 0x5c) {
        goto tail;
    }
    g = gCommManager;
    if (!g->isLocalSlot(o->sessionSlot)) {
        o->actionPriority = ((Unk_02007694 *)(void *)(o))->getActionDonePriority(o->action);
        if (*st < 6 || *st == 9) {
            if (PlayerActor_InsectShowCatchGetNetState(sub) == 6) {
                HeldInsect_Release((u8)r7, PlayerActor_InsectShowCatchGetNetAngle(sub));
                (o)->startAnim(0, 3, 3);
                *st = 7;
            }
        }
    }
    L2.scale[0] = 0x64;
    L2.scale[1] = 0x64;
    L2.scale[2] = 0x64;
    if (o->animId == 0x5c) {
        u32 f2 = ((Unk_ov003_0220bc84_Bits *)&o->bodyModel.curFrame)->f;
        if (f2 <= 0x21) {
            s16 t = (f2 - 0x17) * 10;
            if (t >= 0x64) {
                t = 0x64;
            }
            L2.scale[0] = t;
            L2.scale[1] = t;
            L2.scale[2] = t;
        }
    }
    switch (*st) {
    case 0:
        break;
    case 1: {
        if (TalkRequest_AddPlayerMessage() == 0) {
            break;
        }
        *st = 2;
        TalkMsgRequest *sec = o;
        ((Character *)(void *)(o))->attachTalkRequest(sec);
        (o)->setActionFlag(0x11);
        MsgRequest &sr = *o;
        ((MsgRequest *)(&sr))->setFileName((const char *)((void *)"obj_etc_getinsect"));
        o->msgIndex = b8;
        L2.item = b8 + 0x12b0;
        Catalog_AddCreature(&L2.item);
        o->window->nextState = 1;
        Camera_SetMode4();
        if ((o)->testActionFlag(0x1a)) {
            Bgm_Release(0x3f);
            (o)->clearActionFlag(0x1a);
        }
        Bgm_ReleasePriority(0x12);
        Bgm_RequestSilence(0xc, 0, 0xe);
        Bgm_Request(0xd, 0x39, 0x7f, 1);
        break;
    }
    case 2: {
        TalkWindowState *h = o->window;
        if (h == 0) {
            break;
        }
        if (h->state == 0) {
            break;
        }
        if (Catalog_HasAllInsects() != 0 && rec->catalogWasFull == 0) {
            *st = 8;
            o->msgStep = 0xd;
            break;
        }
        *st = 3;
        o->msgStep = 8;
        break;
    }
    case 3: {
        if (Pocket_FindEmpty() == -1) {
            L2.pr.x = 0;
            L2.pr.y = 0;
            if (FieldAction_FindDropUnit(0x10, &L2.pr, 0)) {
                *st = 4;
                o->msgStep = 0;
            } else {
                *st = 5;
                o->msgStep = 1;
            }
            return;
        }
        TalkWindowState *h = o->window;
        if (h == 0) {
            break;
        }
        if (h->state != 0) {
            break;
        }
        TalkMsgRequest *sec = o;
        ((Character *)(void *)(o))->detachTalkRequest(sec);
        (o)->clearActionFlag(0x11);
        TalkRequest_FinishPlayerMessage();
        PlayerActor_RequestInsectStore(o, b8, 0, 6, -1);
        Camera_SetModeDefault();
        break;
    }
    case 4: {
        if (o->msgStep >= 0xf) {
            *st = 5;
            break;
        }
        TalkWindowState *h = o->window;
        if (h == 0) {
            break;
        }
        if (h->state != 0) {
            break;
        }
        TalkMsgRequest *sec = o;
        ((Character *)(void *)(o))->detachTalkRequest(sec);
        (o)->clearActionFlag(0x11);
        PlayerActor_RequestInsectStore(o, b8, 1, 6, -1);
        Camera_SetModeDefault();
        break;
    }
    case 5: {
        TalkWindowState *h = o->window;
        if (h == 0) {
            break;
        }
        if (h->state != 0) {
            break;
        }
        if (rec->effect != -1) {
            Effect_End(rec->effect);
        }
        s16 ang = Random_GlobalBelow(0x2aaa) - 0x1555;
        HeldInsect_Release((u8)r7, ang);
        PlayerActor_InsectShowCatchSetNetAngle(sub, ang);
        TalkMsgRequest *sec = o;
        ((Character *)(void *)(o))->detachTalkRequest(sec);
        (o)->clearActionFlag(0x11);
        TalkRequest_FinishPlayerMessage();
        (o)->startAnim(0, 3, 3);
        *st = 6;
        PlayerActor_InsectShowCatchSetNetState(sub, 6);
        Camera_SetModeDefault();
        return;
    }
    case 6: {
        if (HeldInsect_GetStage((u8)r7) != 3) {
            if (o->headPitch == 0 && o->headYaw == 0) {
                o->actionPriority = ((Unk_02007694 *)(void *)(o))->getActionDonePriority(o->action);
                switch (mode) {
                case 2:
                    if (g->isLocalSlot(o->sessionSlot)) {
                        if ((o)->testActionFlag(0x11)) {
                            (o)->clearActionFlag(0x11);
                            TalkMsgRequest *sec = o;
                            ((Character *)(void *)(o))->detachTalkRequest(sec);
                        }
                        TalkRequest_FinishPlayerMessage();
                    }
                case 0:
                case 3:
                    (o)->requestWait(3, 1, -1);
                    return;
                case 1:
                    ((Unk_02007694 *)(void *)(o))->requestAct05(3, 5, -1);
                    return;
                }
                return;
            }
            PlayerActor_ApproachAngle(&o->headPitch, 0, 0x400, 0x1770000, 0xc0000);
            PlayerActor_ApproachAngle(&o->headYaw, 0, 0x400, 0x1770000, 0xc0000);
            return;
        } else {
            V3 *q = HeldInsect_GetPos(r7);
            V3 *pv = (V3 *)&o->position;
            pos.x = o->position.x;
            pos.y = pv->y;
            pos.z = pv->z;
            pos.y = pos.y + 0x1b33;
            s16 yaw = Math_Atan2(q->x - pos.x, q->z - pos.z);
            s32 h = Math_Atan2(q->y - pos.y, Vec_DistXZ(q, &pos));
            if (h >= 0x1800) {
                h = 0x1800;
            }
            yaw -= o->rotY;
            PlayerActor_ApproachAngle(&o->headPitch, h, 0x400, 0x1770000, 0xc0000);
            PlayerActor_ApproachAngle(&o->headYaw, yaw, 0x400, 0x1770000, 0xc0000);
            return;
        }
    }
    case 7: {
        s32 r = HeldInsect_GetStage((u8)r7);
        if (r == 3 || r == 0) {
            *st = 6;
        }
        return;
    }
    case 8: {
        TalkWindowState *h = o->window;
        if (h == 0) {
            break;
        }
        if (h->state != 0) {
            break;
        }
        TalkMsgRequest *sec = o;
        ((Character *)(void *)(o))->detachTalkRequest(sec);
        (o)->clearActionFlag(0x11);
        TalkRequest_FinishPlayerMessage();
        (o)->requestAct76(0, b8, *p7, 6, -1);
        return;
    }
    }
tail:
    HeldInsect_SetHandMatrix((u8)r7, L2.scale, &t2, 0);
}
}

namespace ns_0220ba90 {
extern "C" void PlayerActor_MainInsectShowCatch(Obj *o) {
    (o)->advanceAnim();
    volatile V3 sv;
    V3 *pv = (V3 *)&o->drawPos;
    sv.x = o->drawPos.x;
    sv.y = pv->y;
    sv.z = pv->z;
    s16 d0 = o->drawTilt;
    T48 a = o->bodyModel.mtx;
    T48 b = o->itemHandMtx;
    (o)->calcHandMtx();
    (o)->netFollowTransform();
    (o)->updateBodyCollider();
    PlayerActor_InsectShowCatchUpdate(o);
    o->drawPos.x = sv.x;
    o->drawPos.y = sv.y;
    o->drawPos.z = sv.z;
    o->drawTilt = d0;
    o->bodyModel.mtx = a;
    o->itemHandMtx = b;
}
}

namespace ns_0220ba90 {
extern "C" s32 PlayerActor_RequestInsectStore(Obj *o, u32 a, u32 b, s32 id, s32 e) {
    Msg m;
    (&m)->assign(0x58, id, *(s16 *)&e);
    Unk_ov003_0220bc84_Pair &q = (*(Unk_ov003_0220bc84_Pair *)m.unk_0c_b);
    q.a = a;
    q.b = b;
    s32 r = (o)->pushRequest(&m);
    return r;
}
}

namespace ns_0220ba90 {
extern "C" void PlayerActor_InsectStoreSetNetData(u8 *p, u32 a, u32 b, u32 c) {
    p[0] = a;
    PlayerActor_InsectStoreSetNetState(p, b);
    PlayerActor_InsectStoreSetNetAngle(p, c);
}
}

namespace ns_0220ba90 {
extern "C" void PlayerActor_InsectStoreSetNetState(u8 *p, u32 a) {
    p[1] = a;
}
}

namespace ns_0220ba90 {
extern "C" void PlayerActor_InsectStoreSetNetAngle(u8 *p, u32 a) {
    NetBuf_WriteS16B(p + 2, a);
}
}

namespace ns_0220ba90 {
extern "C" void PlayerActor_InsectStoreGetNetData(u8 *p, u8 *a, u8 *b, u16 *c) {
    *a = p[0];
    *b = PlayerActor_InsectStoreGetNetState(p);
    *c = PlayerActor_InsectStoreGetNetAngle(p);
}
}

namespace ns_0220ba90 {
extern "C" u32 PlayerActor_InsectStoreGetNetState(u8 *p) {
    return p[1];
}
}

namespace ns_0220ba90 {
extern "C" u32 PlayerActor_InsectStoreGetNetAngle(u8 *p) {
    return NetBuf_ReadS16B(p + 2);
}
}

namespace ns_0220ba90 {
extern "C" void PlayerActor_SetupInsectStore(Obj *o, Msg *r) {
    u8 x, y;
    Unk_ov003_0220bc84_Pair *q = (Unk_ov003_0220bc84_Pair *)r->unk_0c_b;
    u8 *p = (u8 *)&o->actionWork;
    x = q->a;
    y = q->b;
    p[0] = x;
    p[1] = y;
    PlayerActor_InsectStoreSetNetData(o->netData.bytes, x, y, 0);
    (o)->startAnimOnce(0x5d, 3, 0);
    HeldItemModel_PlayAnim(&o->heldItemModel, 6, 3, 1);
    (o)->playSe(0x4f);
}
}

namespace ns_0220ba90 {
extern "C" s32 PlayerActor_NetInsectStore(Obj *o, s32 a) {
    u8 buf[4];
    PlayerActor_InsectStoreGetNetData(o->netData.bytes, &buf[0], &buf[1], (u16 *)&buf[2]);
    return PlayerActor_RequestInsectStore(o, buf[0], buf[1], 6, a);
}
}

namespace ns_0220ba90 {
extern "C" void PlayerActor_EndInsectStore(Obj *o) {
    o->headPitch = 0;
    o->headYaw = 0;
}
}

namespace ns_0220b0f0 {
extern "C" void PlayerActor_InsectStoreUpdate(Obj *o) {
    s32 flag = 0;
    struct {
        u16 item;
        s16 scale[3];
    } w;
    V3 pos;
    S30 cp;
    u32 code;
    s32 v;
    s16 h;
    u32 id;
    V3 *pv = (V3 *)&o->position;
    pos.x = pv->x;
    pos.y = pv->y;
    pos.z = pv->z;
    id = o->sessionSlot;
    if (o->animId == 0x5d) {
        u32 m = ((Unk_ov003_0220b0f0_Bits *)&o->bodyModel.curFrame)->mid;
        if (m < 6) {
            h = 0x64 - m * 16;
        } else {
            h = 0;
        }
    } else {
        h = 0;
    }
    w.scale[0] = h;
    w.scale[1] = h;
    w.scale[2] = h;
    cp = o->itemHandMtx;
    PlayerActor_ApplyHoldOffset(&cp, 0);
    u8 *rec = (u8 *)&o->actionWork;
    u8 *p6 = o->netData.bytes;
    u8 *st = rec + 1;
    code = (u16)(rec[0] + 0x12b0);
    switch (rec[1]) {
    case 0:
        if (((AnimFrameCtrl *)((AnimFrameCtrl *)&o->bodyModel))->isFinished() == 0) {
            goto tail;
        }
        HeldInsect_Remove(o->sessionSlot, 1);
        o->actionPriority = ((Unk_02007694 *)(void *)(o))->getActionDonePriority(o->action);
        (o)->requestWait(6, 1, -1);
        if (gCommManager->isLocalSlot(o->sessionSlot)) {
            w.item = code;
            Pocket_AddFoundItem(&w.item);
        }
        flag = 1;
        goto tail;
    case 1:
        if (o->animId == 0x5d) {
            if (((AnimFrameCtrl *)((AnimFrameCtrl *)&o->bodyModel))->isFinished() == 0) {
                goto tail;
            }
            (o)->startAnim(0x6c, 3, 3);
            HeldItemModel_PlayAnim(&o->heldItemModel, 0, 3, 0);
        }
        pos.x -= 0x400;
        pos.y += 0x1b34;
        pos.z += 0x1400;
        WorldCurve_ToCurved(&pos, &pos);
        cp.m[9] = pos.x;
        cp.m[10] = pos.y;
        cp.m[11] = pos.z;
        if (gCommManager->isLocalSlot(o->sessionSlot)) {
            s32 s = o->msgStep;
            if (s == 5) {
                if (MenuCtrl_OpenPocketsFullInsect(code) == 0) {
                    goto tail;
                }
                o->msgStep = 6;
                goto tail;
            } else if (s == 6) {
                if (MenuCtrl_IsFinished() == 0) {
                    goto tail;
                }
                o->msgStep = 0xf;
                if (MenuCtrl_IsResultOk() != 0) {
                    if ((o)->testActionFlag(0x11)) {
                        (o)->clearActionFlag(0x11);
                        ((Character *)(void *)(o))->detachTalkRequest(o);
                    }
                    TalkRequest_FinishPlayerMessage();
                    o->actionPriority = ((Unk_02007694 *)(void *)(o))->getActionDonePriority(o->action);
                    (o)->requestWait(6, 1, -1);
                    return;
                }
            } else if (s < 0xf) {
                goto tail;
            }
            v = (s16)(Random_GlobalBelow(0x2aaa) - 0x1555);
            HeldInsect_Release(id, v);
            PlayerActor_InsectStoreSetNetAngle(p6, v);
            if ((o)->testActionFlag(0x11)) {
                (o)->clearActionFlag(0x11);
                ((Character *)(void *)(o))->detachTalkRequest(o);
            }
            TalkRequest_FinishPlayerMessage();
            *st = 2;
            PlayerActor_InsectStoreSetNetState(p6, 2);
            (o)->startAnim(0, 4, 4);
            return;
        } else {
            o->actionPriority = ((Unk_02007694 *)(void *)(o))->getActionDonePriority(o->action);
            if (PlayerActor_InsectStoreGetNetState(p6) == 2) {
                HeldInsect_Release(id, PlayerActor_InsectStoreGetNetAngle(p6));
                *st = 2;
                (o)->startAnim(0, 4, 4);
            }
            return;
        }
    case 2:
        if (HeldInsect_GetStage(id) != 3) {
            if (o->headPitch == 0 && o->headYaw == 0) {
                o->actionPriority = ((Unk_02007694 *)(void *)(o))->getActionDonePriority(o->action);
                (o)->requestWait(6, 1, -1);
            } else {
                PlayerActor_ApproachAngle(&o->headPitch, 0, 0x400, 0x1770000, 0xc0000);
                PlayerActor_ApproachAngle(&o->headYaw, 0, 0x400, 0x1770000, 0xc0000);
            }
            return;
        } else {
            V3 *q = HeldInsect_GetPos(id);
            V3 p3;
            V3 *pv2 = (V3 *)&o->position;
            p3.x = pv2->x;
            p3.y = pv2->y;
            p3.z = pv2->z;
            p3.y += 0x1b33;
            s32 yaw = Math_Atan2(q->x - p3.x, q->z - p3.z);
            s32 hh = Math_Atan2(q->y - p3.y, Vec_DistXZ(q, &p3));
            o->headYaw = yaw;
            if (hh >= 0x1800) {
                hh = 0x1800;
            }
            o->headPitch = hh;
            return;
        }
    default:
    tail:
        HeldInsect_SetHandMatrix(id, w.scale, &cp, flag);
    }
}
}

namespace ns_0220b0f0 {
extern "C" void PlayerActor_MainInsectStore(Obj *o) {
    (o)->advanceAnim();
    (o)->netFollowTransform();
    (o)->updateBodyCollider();
    PlayerActor_InsectStoreUpdate(o);
}
}

namespace ns_0220b0f0 {
extern "C" void PlayerActor_ShovelReadySetArgs(u8 *p, u8 a, u8 b, u32 c) {
    p[0] = a;
    p[1] = b;
    p[2] = c;
}
}

namespace ns_0220b0f0 {
extern "C" s32 PlayerActor_RequestShovelReady(Obj *o, s32 a, s16 b) {
    Msg m;
    u8 *d = m.unk_0c_b;
    V3 v;
    s32 f;
    (&m)->assign(0x59, a, b);
    if (o->hasTargetPos != 0) {
        s32 z = o->targetPos.z;
        v.x = o->targetPos.x;
        v.y = 0;
        v.z = z;
        f = 1;
    } else {
        s32 z = o->toolTargetPos.z;
        v.x = o->toolTargetPos.x;
        f = 0;
        v.y = 0;
        v.z = z;
        if (o->toolTargetKind != 0) {
            f = 1;
        }
    }
    s32 x = 0;
    s32 y = 0;
    FieldPos_ToUnit(&x, &y, &v);
    PlayerActor_ShovelReadySetArgs(d, x, y, f);
    s32 r = (o)->pushRequest(&m);
    return r;
}
}

namespace ns_0220b0f0 {
extern "C" s32 PlayerActor_RequestShovelReadyAt(Obj *o, u8 *a, u8 *b, u8 *c, s32 id, s32 e) {
    Msg m;
    (&m)->assign(0x59, id, *(s16 *)&e);
    PlayerActor_ShovelReadySetArgs(m.unk_0c_b, *a, *b, *c);
    s32 r = (o)->pushRequest(&m);
    return r;
}
}

namespace ns_0220b0f0 {
extern "C" void PlayerActor_ShovelReadySetWork(Rec *r, V3C v, s32 c, u32 d) {
    r->targetPos.x = v.x;
    r->targetPos.y = v.y;
    r->targetPos.z = v.z;
    r->targetAngle = c;
    r->hasTarget = d;
    r->unk_0f = 0;
}
}

namespace ns_0220b0f0 {
extern "C" void PlayerActor_ShovelReadySetNetData(u8 *p, u8 a, u8 b, u32 c) {
    p[0] = a;
    p[1] = b;
    p[2] = c;
}
}

namespace ns_0220b0f0 {
extern "C" void PlayerActor_ShovelReadyGetNetData(u8 *src, u8 *a, u8 *b, u8 *c) {
    *a = src[0];
    *b = src[1];
    *c = src[2];
}
}

namespace ns_0220b0f0 {
extern "C" void PlayerActor_SetupShovelReady(Obj *o, Msg *r) {
    Unk_ov003_0220b0f0_B3 *q = (Unk_ov003_0220b0f0_B3 *)r->unk_0c_b;
    Rec *rp = (Rec *)&o->actionWork;
    u32 a = q->a;
    u32 b = q->b;
    u32 c = q->c;
    V3 pos;
    V3 out;
    V3C t;
    s32 acc;
    PlayerActor_ShovelReadySetNetData(o->netData.bytes, a, b, c);
    (o)->startAnimOnce(0x49, 3, 0);
    FieldPos_FromUnitCenter(&pos, a, b);
    if (c == 0) {
        s32 d = o->speed;
        acc = 0x2000;
        while (d != 0) {
            d -= 0x171;
            if (d > 0) {
                acc += d;
            } else {
                d = 0;
            }
        }
        PlayerActor_OffsetByAngle(&out, o, &o->position, &o->rotY, &acc);
        pos.x = out.x;
        pos.y = out.y;
        pos.z = out.z;
        FieldPos_SnapToUnitCenter(&pos, &pos);
    }
    s32 h = Math_Atan2(pos.x - o->bodyPos.x, pos.z - o->bodyPos.z);
    t.x = pos.x;
    t.y = pos.y;
    t.z = pos.z;
    PlayerActor_ShovelReadySetWork(rp, t, h, c);
}
}

namespace ns_0220b0f0 {
extern "C" s32 PlayerActor_NetShovelReady(Obj *o, s32 a) {
    u8 b[3];
    PlayerActor_ShovelReadyGetNetData(o->netData.bytes, &b[0], &b[1], &b[2]);
    return PlayerActor_RequestShovelReadyAt(o, &b[0], &b[1], &b[2], 6, a);
}
}

namespace ns_0220b0f0 {
extern "C" void PlayerActor_ShovelReadyTurn(Obj *o) {
    Rec *r4 = (Rec *)&o->actionWork;
    s32 h;
    if (o->speed == 0 && r4->hasTarget == 0) {
        V3 t;
        PlayerActor_GetFrontUnitCenter(&t, o);
        h = Math_Atan2(t.x - o->bodyPos.x, t.z - o->bodyPos.z);
        r4->targetAngle = h;
        r4->hasTarget = 1;
        r4->targetPos.x = t.x;
        r4->targetPos.y = t.y;
        r4->targetPos.z = t.z;
    } else {
        h = r4->targetAngle;
    }
    (o)->turnToward(h);
}
}

namespace ns_0220b0f0 {
extern "C" void PlayerActor_ShovelReadyTrackTarget(Obj *o) {
    V3 v;
    Rec *r = (Rec *)&o->actionWork;
    v.x = r->targetPos.x;
    v.y = r->targetPos.y;
    v.z = r->targetPos.z;
    (o)->setSubCollider(&v, 0xf33, 0x1000);
    o->subCollider.submit();
}
}

namespace ns_0220b0f0 {
extern "C" void PlayerActor_ShovelReadyCheckEnd(Obj *o) {
    Rec *r4 = (Rec *)&o->actionWork;
    if (((AnimFrameCtrl *)((AnimFrameCtrl *)&o->bodyModel))->isFinished() != 0 || o->animId == 0x4a) {
        if (o->animId == 0x49) {
            (o)->startAnim(0x4a, 3, 0);
        }
        if (o->speed == 0) {
            if (r4->targetAngle == o->rotY) {
                V3C a;
                a.x = r4->targetPos.x;
                a.y = r4->targetPos.y;
                a.z = r4->targetPos.z;
                if (Unk_ov003_0220b330_IsZero(PlayerActor_ShovelDispatch(o, &a, 1))) {
                    s32 x = 0;
                    s32 y = 0;
                    Pair w;
                    FieldPos_ToUnit(&x, &y, &a);
                    w.a = x;
                    w.b = y;
                    PlayerActor_RequestShovelWait(o, w, 6, -1);
                }
            }
        }
    }
}
}

namespace ns_0220b0f0 {
extern "C" void PlayerActor_ShovelReadyCheckEndRemote(Obj *o) {
    o->actionPriority = ((Unk_02007694 *)(void *)(o))->getActionDonePriority(o->action);
    if (((AnimFrameCtrl *)((AnimFrameCtrl *)&o->bodyModel))->isFinished()) {
        (o)->startAnim(0x4a, 3, 0);
    }
}
}

namespace ns_0220b0f0 {
extern "C" void PlayerActor_MainShovelReady(Obj *o) {
    s32 r4 = o->speed;
    s32 t = PlayerActor_DecreaseClamped(r4, 0, 0x171);
    (o)->setSpeed((u32 *)(void *)(&t));
    if (gCommManager->isLocalSlot(o->sessionSlot)) {
        PlayerActor_ShovelReadyTurn(o);
        (o)->advanceAnim();
        (o)->moveWithCollision();
        (o)->updateBodyCollider();
        PlayerActor_ShovelReadyCheckEnd(o);
        PlayerActor_ShovelReadyTrackTarget(o);
    } else {
        (o)->advanceAnim();
        if ((o)->netFollowTransform()) {
            (o)->moveWithCollision();
        }
        (o)->updateBodyCollider();
        PlayerActor_ShovelReadyCheckEndRemote(o);
    }
    if (r4 != 0 && o->speed == 0) {
        Effect_Create(0x2a, (V3 *)&o->position, &o->rotY, 0);
    }
}
}

namespace ns_0220b0f0 {
extern "C" void PlayerActor_ShovelWaitSetArgs(u8 *d, Pair v) {
    d[0] = v.a;
    d[1] = v.b;
}
}

namespace ns_0220b0f0 {
extern "C" s32 PlayerActor_RequestShovelWait(Obj *o, Pair p, s32 id, s16 e) {
    Msg m;
    (&m)->assign(0x5a, id, e);
    PlayerActor_ShovelWaitSetArgs(m.unk_0c_b, p);
    s32 r = (o)->pushRequest(&m);
    return r;
}
}

namespace ns_0220b0f0 {
extern "C" void PlayerActor_ShovelWaitSetWork(u8 *p, Pair *v) {
    p[0] = v->a;
    p[1] = v->b;
}
}

namespace ns_0220b0f0 {
extern "C" void PlayerActor_ShovelWaitSetNetData(u8 *p, Pair *v) {
    p[0] = v->a;
    p[1] = v->b;
}
}

namespace ns_0220b0f0 {
extern "C" void PlayerActor_ShovelWaitGetNetData(u8 *p, Pair *out) {
    out->a = p[0];
    out->b = p[1];
}
}

namespace ns_0220b0f0 {
extern "C" void PlayerActor_SetupShovelWait(Obj *o, Msg *r) {
    Pair w;
    Pair w2;
    Unk_ov003_0220b0f0_B3 *q = (Unk_ov003_0220b0f0_B3 *)r->unk_0c_b;
    u32 b = q->b;
    u8 *p = (u8 *)&o->actionWork;
    u32 a = q->a;
    w.a = a;
    w.b = b;
    PlayerActor_ShovelWaitSetWork(p, &w);
    if (o->fieldQuery == -1) {
        p[2] = 0;
    } else {
        p[2] = 1;
    }
    w2.a = a;
    w2.b = b;
    PlayerActor_ShovelWaitSetNetData(o->netData.bytes, &w2);
    if (o->animId == 0x4a) {
        (o)->startAnim(0x4a, 3, 0);
    }
}
}

namespace ns_0220b0f0 {
extern "C" s32 PlayerActor_NetShovelWait(Obj *o, s16 a) {
    Pair v;
    v.a = 0;
    v.b = 0;
    PlayerActor_ShovelWaitGetNetData(o->netData.bytes, &v);
    return PlayerActor_RequestShovelWait(o, v, 6, a);
}
}

namespace ns_0220b0f0 {
extern "C" void PlayerActor_ShovelWaitTrackTarget(Obj *o) {
    V3 v;
    u8 *p = (u8 *)&o->actionWork;
    FieldPos_FromUnitCenter(&v, p[0], p[1]);
    (o)->setSubCollider(&v, 0xf33, 0x1000);
    o->subCollider.submit();
}
}

namespace ns_0220a680 {
extern "C" s32 PlayerActor_ShovelDispatch(Obj *o, VecFx32 *v, u8 f) {
    Pair xy;
    V3 pos;
    s32 r6;
    u32 r7;
    Rec *r5;
    if (o->speed != 0) return -1;
    r7 = 0;
    r5 = (Rec *)&o->actionWork;
    if (f != 0) {
        pos.x = v->x;
        pos.y = v->y;
        pos.z = v->z;
    } else {
        FieldPos_FromUnitCenter(&pos, r5->a, r5->b);
        if (r5->c == 0) {
            r7 = 1;
            f = 1;
        }
    }
    r6 = PlayerActor_ShovelClassifyTarget(o, &pos, &f);
    xy.a = 0;
    xy.b = 0;
    FieldPos_ToUnit(&xy.a, &xy.b, &pos);
    if (r7 != 0) {
        if (o->fieldQuery != -1) r5->c = 1;
    }
    switch (r6) {
    case 0:
        break;
    case 1:
        PlayerActor_RequestAct5B(o, 6, -1);
        break;
    case 2:
        PlayerActor_RequestShovelStrike(o, 0, xy, 6, -1);
        break;
    case 3:
        PlayerActor_RequestDig(o, 0, pos, 6, -1);
        break;
    case 4:
        PlayerActor_RequestDig(o, 1, pos, 6, -1);
        break;
    case 5:
        PlayerActor_RequestDigUpItem(o, VecFx32Copy(pos.x, pos.y, pos.z), 0, 6, -1);
        break;
    case 6: {
        Vec2 xy2;
        xy2.x = 0;
        xy2.y = 0;
        FieldPos_ToUnit(&xy2.x, &xy2.y, &pos);
        PlayerActor_RequestFillHole(o, 0, &xy2, 0xfff1, 6, -1);
        break;
    }
    case 7:
        PlayerActor_RequestAct5C(o, 6, -1);
        break;
    case 8:
        PlayerActor_RequestShovelStrike(o, 1, xy, 6, -1);
        break;
    case 9:
        PlayerActor_RequestDigUpItem(o, VecFx32Copy(pos.x, pos.y, pos.z), 1, 6, -1);
        break;
    }
    return r6;
}
}

namespace ns_0220a680 {
extern "C" void PlayerActor_MainShovelWait(Obj *o) {
    s32 r4 = o->speed;
    s32 t = PlayerActor_Decelerate(r4, 0);
    (o)->setSpeed((u32 *)(void *)(&t));
    (o)->advanceAnim();
    if (gCommManager->isLocalSlot(o->sessionSlot)) {
        V3 v;
        (o)->moveWithCollision();
        (o)->updateBodyCollider();
        v.x = 0;
        v.y = 0;
        v.z = 0;
        PlayerActor_ShovelDispatch(o, &v, 0);
    } else {
        if ((o)->netFollowTransform()) (o)->moveWithCollision();
        (o)->updateBodyCollider();
        o->actionPriority = ((Unk_02007694 *)(void *)(o))->getActionDonePriority(o->action);
    }
    PlayerActor_ShovelWaitTrackTarget(o);
    if (r4 != 0) {
        if (o->speed == 0) {
            Effect_Create(0x2a, (VecFx32CopyDtor *)&o->position, &o->rotY, 0);
        }
    }
}
}

namespace ns_0220a680 {
extern "C" s32 PlayerActor_RequestAct5B(Obj *o, u32 a, s16 b) {
    Msg m;
    (&m)->assign(0x5b, a, b);
    s32 r = (o)->pushRequest(&m);
    return r;
}
}

namespace ns_0220a680 {
extern "C" void PlayerActor_SetupAct5B(Obj *o) {
    (o)->startAnimOnce(0x4d, 3, 0);
}
}

namespace ns_0220a680 {
extern "C" s32 PlayerActor_NetAct5B(Obj *o, s16 x) {
    return PlayerActor_RequestAct5B(o, 6, x);
}
}

namespace ns_0220a680 {
extern "C" void PlayerActor_Act5BUpdate(Obj *o) {
    if (((AnimFrameCtrl *)((AnimFrameCtrl *)&o->bodyModel))->hasPassedFrame(5)) {
        (o)->playSe(0x842);
    }
    (o)->advanceAnim();
}
}

namespace ns_0220a680 {
extern "C" void PlayerActor_Act5BCheckEnd(Obj *o) {
    if (((AnimFrameCtrl *)((AnimFrameCtrl *)&o->bodyModel))->isFinished()) {
        o->actionPriority = ((Unk_02007694 *)(void *)(o))->getActionDonePriority(o->action);
        (o)->requestWait(3, 1, -1);
    }
}
}

namespace ns_0220a680 {
extern "C" void PlayerActor_MainAct5B(Obj *o) {
    PlayerActor_Act5BUpdate(o);
    (o)->updateBodyCollider();
    PlayerActor_Act5BCheckEnd(o);
}
}

namespace ns_0220a680 {
extern "C" s32 PlayerActor_RequestAct5C(Obj *o, u32 a, s16 b) {
    Msg m;
    (&m)->assign(0x5c, a, b);
    s32 r = (o)->pushRequest(&m);
    return r;
}
}

namespace ns_0220a680 {
extern "C" void PlayerActor_SetupAct5C(Obj *o) {
    (o)->startAnimOnce(0x5e, 3, 0);
}
}

namespace ns_0220a680 {
extern "C" s32 PlayerActor_NetAct5C(Obj *o, s16 x) {
    return PlayerActor_RequestAct5C(o, 6, x);
}
}

namespace ns_0220a680 {
extern "C" void PlayerActor_Act5CCheckEnd(Obj *o) {
    if (((AnimFrameCtrl *)((AnimFrameCtrl *)&o->bodyModel))->isFinished()) {
        o->actionPriority = ((Unk_02007694 *)(void *)(o))->getActionDonePriority(o->action);
        (o)->requestWait(3, 1, -1);
    }
}
}

namespace ns_0220a680 {
extern "C" void PlayerActor_MainAct5C(Obj *o) {
    (o)->advanceAnim();
    (o)->moveWithCollision();
    (o)->updateBodyCollider();
    s32 t = PlayerActor_DecreaseClamped(o->speed, 0, 0x171);
    (o)->setSpeed((u32 *)(void *)(&t));
    PlayerActor_Act5CCheckEnd(o);
}
}

namespace ns_0220a680 {
extern "C" void PlayerActor_ShovelStrikeSetArgs(B3 *d, Pair p, u32 c) {
    d->a = p.a;
    d->b = p.b;
    d->c = c;
}
}

namespace ns_0220a680 {
extern "C" s32 PlayerActor_RequestShovelStrike(Obj *o, u32 id, Pair p, s32 b, s32 c) {
    Msg m;
    (&m)->assign(0x5d, b, *(s16 *)&c);
    PlayerActor_ShovelStrikeSetArgs((B3 *)m.unk_0c_b, p, id);
    s32 r = (o)->pushRequest(&m);
    return r;
}
}

namespace ns_0220a680 {
extern "C" void PlayerActor_ShovelStrikeSetWork(Rec *r, Pair p, u8 c) {
    r->a = p.a;
    r->b = p.b;
    r->c = c;
}
}

namespace ns_0220a680 {
extern "C" void PlayerActor_ShovelStrikeSetNetData(u8 *d, u8 a, u8 b, u8 c, u8 e) {
    d[0] = a;
    d[1] = b;
    d[2] = c;
    d[3] = e;
}
}

namespace ns_0220a680 {
extern "C" void PlayerActor_ShovelStrikeGetNetData(u8 *s, u8 *a, u8 *b, u8 *c, u8 *d) {
    *a = s[0];
    *b = s[1];
    *c = s[2];
    *d = s[3];
}
}

namespace ns_0220a680 {
extern "C" void PlayerActor_SetupShovelStrike(Obj *o, Msg *m) {
    B3 *q = (B3 *)m->unk_0c_b;
    u8 c = q->c;
    s32 a = q->a;
    s32 b = q->b;
    Pair p;
    p.a = a;
    p.b = b;
    PlayerActor_ShovelStrikeSetWork((Rec *)&o->actionWork, p, c);
    PlayerActor_ShovelStrikeSetNetData(o->netData.bytes, c, a, b, o->toolHitKind);
    (o)->startAnimOnce(0x4c, 3, 0);
    if (c != 0) {
        (o)->setActionFlag(0x1c);
    }
}
}

namespace ns_0220a680 {
extern "C" void PlayerActor_NetShovelStrike(Obj *o, s32 x) {
    u8 b[4];
    PlayerActor_ShovelStrikeGetNetData(o->netData.bytes, &b[0], &b[1], &b[2], &b[3]);
    o->toolHitKind = b[3];
    if (b[0] == 0) {
        Pair p;
        p.a = b[1];
        p.b = b[2];
        PlayerActor_RequestShovelStrike(o, 0, p, 6, x);
    }
}
}

namespace ns_0220a680 {
extern "C" void PlayerActor_EndShovelStrike(Obj *o) {
    if (gCommManager->isLocalSlot(o->sessionSlot) == 0) {
        if ((o)->testActionFlag(0x1c)) {
            Rec *r = (Rec *)&o->actionWork;
            if (r->c != 0) {
                FieldUnitPos p;
                u32 b = r->b;
                u32 a = r->a;
                p.x = a;
                p.z = b;
                FieldItemFx_StartStrikeResult(p);
                (o)->clearActionFlag(0x12);
                (o)->clearActionFlag(0x1c);
            }
        }
    }
}
}

namespace ns_0220a680 {
extern "C" void PlayerActor_ShovelStrikeUpdate(Obj *o) {
    s16 h[2];
    s32 xy[2];
    V3 pos;
    u16 *p;
    s32 k;
    (o)->advanceAnim();
    if (((AnimFrameCtrl *)((AnimFrameCtrl *)&o->bodyModel))->hasPassedFrame(5)) {
        Rec *rc = (Rec *)&o->actionWork;
        if (rc->c != 0) {
            FieldUnitPos pa;
            u32 b = rc->b;
            u32 a = rc->a;
            pa.x = a;
            pa.z = b;
            FieldItemFx_StartStrikeResult(pa);
            (o)->clearActionFlag(0x12);
            (o)->clearActionFlag(0x1c);
        }
        s32 t = -0x333;
        (o)->setSpeed((u32 *)(void *)(&t));
        PlayerActor_GetFrontUnitCenter(&pos, o);
        xy[0] = 0;
        xy[1] = 0;
        FieldPos_ToUnit(&xy[0], &xy[1], &pos);
        s32 x, y, hx, hy;
        x = *(volatile s32 *)&xy[0];
        y = *(volatile s32 *)&xy[1];
        hx = x >> 4;
        hy = y >> 4;
        p = BlockMap_GetItemPtr(gSceneBlockMap, hx, hy, x - (hx << 4), y - (hy << 4), 0);
        if (p != NULL) {
            if (Unk_ov003_0220a7fc_Chk(p)) {
                Tree_RequestShake(o->sessionSlot, FieldUnitPos(xy[0], xy[1]), 1);
            }
        }
        (o)->setActionFlag(9);
        k = 0xf;
        if (p != NULL) k = (s32)(*p & 0xf000) >> 12;
        switch (o->toolHitKind) {
        case 0:
            if (p != NULL) {
                if (k <= 4) {
                    BOOL r3 = TRUE;
                    BOOL r2 = FALSE;
                    u32 v = *p;
                    if (v >= 0xe3 && v <= 0xe7) r2 = r3;
                    if (!r2) {
                        if (!(v >= 0xe8 && v <= 0xfb)) r3 = FALSE;
                    }
                    if (!r3) {
                        if (v >= 0x26 && v <= 0x2a) goto l_a9dc;
                        if (v >= 0x5d && v <= 0x61) goto l_a9dc;
                        if (v >= 0x2f && v <= 0x56) goto l_a9dc;
                        if (v >= 0x57 && v <= 0x5b) goto l_a9dc;
                        if (v >= 0x66 && v <= 0x68) goto l_a9dc;
                        if (v == 0x69) goto l_a9dc;
                        if (v >= 0x6a && v <= 0x6c) goto l_a9dc;
                        if (v == 0x6d) goto l_a9dc;
                        if (!(v >= 0xc8 && v <= 0xcf)) goto l_a9e6;
                    }
                }
            l_a9dc:
                if (k == 0xa) goto l_a9e6;
                if ((u32)(k - 0xd) > 1) goto l_a9f0;
            l_a9e6:
                (o)->playSe(0x7df);
                break;
            }
        l_a9f0:
            (o)->playSe(0x844);
            {
                V3 *pv = (V3 *)&o->position;
                pos.x = pv->x;
                pos.y = pv->y;
                pos.z = pv->z;
                *(s16 *)&h[0] = o->rotY;
                s32 r5, r6, r7, dz, dx;
                s32 idx = ((u16)h[0] >> 4) * 2;
                s32 sn = data_02135f44[idx];
                s32 cs = data_02135f44[idx + 1];
                r5 = func_01ffcb0c(cs, 0xccd);
                r5 -= func_01ffcb0c(sn, 0x800);
                r6 = func_01ffcb0c(sn, 0xccd);
                dx = r6 + func_01ffcb0c(cs, 0x800);
                pos.x = pos.x + dx;
                pos.z += r5;
                h[0] = (s16)(h[0] - 0x2000);
                Effect_Create(5, &pos, &h[0], 0);
            }
            break;
        case 1:
            (o)->playSe(0x7df);
            break;
        case 2:
            (o)->playSe(0x7df);
            if (o->toolHitActor != NULL) {
                PlayerActor_GetHeldItem((u16 *)&h[1], o);
                ((NpcActor *)o->toolHitActor)->onToolHit((u16 *)&h[1]);
            }
            break;
        case 3:
            if (gCommManager->isLocalSlot(o->sessionSlot)) {
                if (Snowball_Break((void *)o->toolHitActor, 1) == 0) {
                    (o)->playSe(0x7df);
                }
            } else {
                (o)->playSe(0x7df);
            }
            break;
        }
        o->toolHitActor = 0;
        o->toolHitKind = 0;
    }
}
}

namespace ns_0220a680 {
extern "C" void PlayerActor_ShovelStrikeCheckEnd(Obj *o) {
    if (((AnimFrameCtrl *)((AnimFrameCtrl *)&o->bodyModel))->isFinished()) {
        o->actionPriority = ((Unk_02007694 *)(void *)(o))->getActionDonePriority(o->action);
        (o)->requestWait(3, 1, -1);
    }
}
}

namespace ns_0220a680 {
extern "C" void PlayerActor_MainShovelStrike(Obj *o) {
    PlayerActor_ShovelStrikeUpdate(o);
    (o)->moveWithCollision();
    (o)->updateBodyCollider();
    s32 v = o->speed;
    if (v < 0) v = -v;
    s32 t = PlayerActor_Decelerate(v, 0) * -1;
    (o)->setSpeed((u32 *)(void *)(&t));
    PlayerActor_ShovelStrikeCheckEnd(o);
}
}

namespace ns_0220a680 {
extern "C" void PlayerActor_DigSetArgs(P3 *p, u32 id, V3 v) {
    p->id = id;
    p->pos.x = v.x;
    p->pos.y = v.y;
    p->pos.z = v.z;
}
}

namespace ns_0220a680 {
extern "C" s32 PlayerActor_RequestDig(Obj *o, u32 id, V3 v, s32 b, s16 c) {
    Msg m;
    (&m)->assign(0x5e, b, c);
    PlayerActor_DigSetArgs((P3 *)m.unk_0c_b, id, v);
    s32 r = (o)->pushRequest(&m);
    return r;
}
}

namespace ns_0220a680 {
extern "C" void PlayerActor_DigSetWork(Rec *r, u32 id, V3 v) {
    r->a = id;
    r->pos.x = v.x;
    r->pos.y = v.y;
    r->pos.z = v.z;
}
}

namespace ns_0220a680 {
extern "C" void PlayerActor_SetupDig(Obj *o, Msg *m) {
    P3 *p = (P3 *)m->unk_0c_b;
    u32 id = p->id;
    V3 t = p->pos;
    Rec *r = (Rec *)&o->actionWork;
    PlayerActor_DigSetWork(r, id, t);
    if ((u32)(o->animId - 0x49) <= 1) {
        if (r->a != 0) {
            (o)->startAnimOnce(0x4e, 3, 0);
        } else {
            (o)->startAnimOnce(0x4b, 3, 0);
        }
    } else {
        (o)->startAnimOnce(0x49, 3, 0);
    }
    o->setActionFlag(0x1c);  // (void: nothing reads this function's result)
}
}

namespace ns_0220a680 {
extern "C" void PlayerActor_NetDig(Obj *o) {
}
}

namespace ns_02209d50 {
extern "C" void PlayerActor_EndDig(Obj *o) {
    if (!gCommManager->isLocalSlot(o->sessionSlot)) {
        if ((o)->testActionFlag(0x1c)) {
            Rec2 *r = (Rec2 *)&o->actionWork;
            V3D p;
            p.x = r->digPos.x;
            p.y = r->digPos.y;
            p.z = r->digPos.z;
            if (o->animId == 0x4b) {
                FieldItemFx_StartDigHole(o->sessionSlot, &p, 0);
            } else {
                FieldItemFx_StartDigUpTree(o->sessionSlot, &p, o->rotY);
                FieldItemFx_StartDigHole(o->sessionSlot, &p, 0);
            }
            (o)->clearActionFlag(0x1c);
        }
    }
}
}

namespace ns_02209d50 {
extern "C" void PlayerActor_DigUpdate(Obj *o) {
    s16 h;
    V3D p;
    s32 a, b;
    if (o->animId != 0x49) {
        Rec2 *r = (Rec2 *)&o->actionWork;
        p.x = r->digPos.x;
        p.y = r->digPos.y;
        p.z = r->digPos.z;
        h = o->rotY;
        if (o->animId == 0x4b) {
            switch (((Bits *)&o->bodyModel.curFrame)->mid) {
            case 2:
                Effect_Create(6, &p, 0, 0);
                break;
            case 5:
                (o)->playSe(0x841);
                (o)->setActionFlag(9);
                break;
            case 7: {
                Effect_Create(7, &p, &h, 0);
                FieldItemFx_StartDigHole(o->sessionSlot, &p, 0);
                (o)->clearActionFlag(0x1c);
                FieldPos_ToUnit(&a, &b, &p);
                Collision_AddDigHole(a, b);
                break;
            }
            }
        } else {
            switch ((s32)((Bits *)&o->bodyModel.curFrame)->mid) {
            case 5:
                (o)->playSe(0x85b);
                (o)->setActionFlag(9);
                break;
            case 12:
                Effect_Create(6, &p, 0, 0);
                break;
            case 18:
                Effect_Create(8, &p, &h, 0);
                break;
            case 21: {
                FieldItemFx_StartDigUpTree(o->sessionSlot, &p, o->rotY);
                FieldItemFx_StartDigHole(o->sessionSlot, &p, 0);
                (o)->playSe(0x85c);
                (o)->clearActionFlag(0x1c);
                break;
            }
            }
        }
    }
    (o)->advanceAnim();
}
}

namespace ns_02209d50 {
extern "C" void PlayerActor_DigCheckEnd(Obj *o) {
    if (((AnimFrameCtrl *)((AnimFrameCtrl *)&o->bodyModel))->isFinished()) {
        if (o->animId == 0x49) {
            if (*(u8 *)&o->actionWork != 0) {
                (o)->startAnimOnce(0x4e, 3, 0);
            } else {
                (o)->startAnimOnce(0x4b, 3, 0);
            }
        } else {
            o->actionPriority = ((Unk_02007694 *)(void *)(o))->getActionDonePriority(o->action);
            (o)->requestWait(3, 1, -1);
        }
    }
}
}

namespace ns_02209d50 {
extern "C" void PlayerActor_MainDig(Obj *o) {
    V3 v;
    PlayerActor_DigUpdate(o);
    if (gCommManager->isLocalSlot(o->sessionSlot)) {
        (o)->moveWithCollision();
    } else {
        Rec2 *r = (Rec2 *)&o->actionWork;
        v = r->digPos;
        (o)->netSyncNearPoint((VecFx32 *)(void *)(&v));
    }
    (o)->updateBodyCollider();
    PlayerActor_DigCheckEnd(o);
}
}

namespace ns_02209d50 {
extern "C" void PlayerActor_DigUpItemSetArgs(Pay *d, V3 v, u16 a, u8 b) {
    d->v.x = v.x;
    d->v.y = v.y;
    d->v.z = v.z;
    d->a = a;
    d->b = b;
}
}

namespace ns_02209d50 {
extern "C" s32 PlayerActor_RequestDigUpItem(Obj *o, V3 v, u8 x, s32 c, s32 e) {
    Msg m;
    (&m)->assign(0x5f, c, *(s16 *)&e);
    PlayerActor_DigUpItemSetArgs((PlayerDigUpItemArgs *)m.unk_0c_b, v, 0xfff1, x);
    s32 r = (o)->pushRequest(&m);
    return r;
}
}

namespace ns_02209d50 {
extern "C" s32 PlayerActor_RequestDigUpItemWith(Obj *o, V3 v, u16 a, u8 b, s32 c, s32 d) {
    Msg m;
    (&m)->assign(0x5f, c, *(s16 *)&d);
    PlayerActor_DigUpItemSetArgs((PlayerDigUpItemArgs *)m.unk_0c_b, v, a, b);
    if (!gCommManager->isLocalSlot(o->sessionSlot)) {
        o->actionItem = o->shownItem = 0xfff1;
    }
    s32 r = (o)->pushRequest(&m);
    return r;
}
}

namespace ns_02209d50 {
extern "C" void PlayerActor_DigUpItemSetNetData(u8 *p, V2 v, u16 a, u8 b) {
    p[2] = v.x;
    p[3] = v.y;
    NetBuf_WriteU16(p, a);
    p[4] = b;
}
}

namespace ns_02209d50 {
extern "C" void PlayerActor_DigUpItemGetNetData(u8 *p, s32 *xy, u16 *a, u8 *b) {
    xy[0] = p[2];
    xy[1] = p[3];
    *a = NetBuf_ReadU16(p);
    *b = p[4];
}
}

namespace ns_02209d50 {
extern "C" void PlayerActor_SetupDigUpItem(Obj *o, Msg *m) {
    Pay *pl = (PlayerDigUpItemArgs *)m->unk_0c_b;
    V3 v = pl->v;
    u16 loc = pl->a;
    u8 b = pl->b;
    DigUpItemStackPad dx;

    Rec *r = (Rec *)&o->actionWork;
    r->talkStep = 0;
    r->digPos = v;
    r->holeKind = b;
    s32 st = o->animId;
    if (st == 0x49 || st == 0x4a || !Unk_ov003_02209fc8_IsNone(&loc)) {
        (o)->startAnimOnce(0x4f, 3, 0);
    } else {
        (o)->startAnimOnce(0x49, 3, 0);
    }
    s32 z2 = o->heldItemJointMtx.m[11];
    s32 y2 = o->heldItemJointMtx.m[10];
    s32 x2 = o->heldItemJointMtx.m[9];
    o->shownItemPos.x = x2;
    o->shownItemPos.y = y2;
    o->shownItemPos.z = z2;
    WorldCurve_FromCurved(&o->shownItemPos, &o->shownItemPos);
    o->shownItemScaleX = 0x1000;
    o->shownItemScaleY = 0x1000;
    o->shownItemScaleZ = 0x1000;
    (o)->clearActionFlag(0xd);
    s32 xy[2];
    xy[0] = 0;
    xy[1] = 0;
    FieldPos_ToUnit(&xy[0], &xy[1], &v);
    u16 t = o->actionItem;
    V2 pr;
    pr.x = xy[0];
    pr.y = xy[1];
    PlayerActor_DigUpItemSetNetData(o->netData.bytes, pr, t, b);
    s32 px = *(volatile s32 *)&xy[0];
    s32 py = *(volatile s32 *)&xy[1];
    s32 hx = px >> 4;
    s32 hy = py >> 4;
    u16 *cell = BlockMap_GetItemPtr(gSceneBlockMap, hx, hy, px - (hx << 4), py - (hy << 4), 0);
    if (!Unk_ov003_02209fc8_IsNone(&loc)) {
        o->actionItem = o->shownItem = loc;
        if (Unk_ov003_02209fc8_Rng(cell, 0xfc, 0xfd)) {
            *(u32 *)(Bits *)&o->bodyModel.curFrame = (u32)((((Bits *)&o->bodyModel.numFrames)->mid - 1) << 16) >> 4;
            (o)->setActionFlag(0xd);
        }
    } else {
        if (Unk_ov003_02209fc8_Eq(&o->actionItem) || !gCommManager->isLocalSlot(o->sessionSlot)) {
            if (cell == 0) {
                o->actionItem = o->shownItem = 0xfff1;
            } else {
                o->actionItem = o->shownItem = *cell;
            }
        }
    }
    if (gCommManager->isLocalSlot(o->sessionSlot)) {
        Bgm_RequestSilence(0x12, 0xf, 0);
        if (Unk_ov003_02209fc8_Rng(&o->actionItem, 0x1549, 0x1549)) VillagerTrend_OnItemDug((V3 *)&o->position);
    }
    o->msgStep = 7;
    (o)->setActionFlag(0x1c);
}
}

namespace ns_02209d50 {
extern "C" void PlayerActor_NetDigUpItem(Obj *o, s32 a) {
    s32 xy[2];
    V3 v;
    u16 h;
    u8 b;
    if (o->action == 0) {
        xy[0] = 0;
        xy[1] = 0;
        PlayerActor_DigUpItemGetNetData(o->netData.bytes, xy, &h, &b);
        FieldPos_FromUnitCenter(&v, xy[0], xy[1]);
        V3 w = v;
        PlayerActor_RequestDigUpItemWith(o, w, h, b, 6, a);
    }
}
}

namespace ns_02209d50 {
extern "C" void PlayerActor_EndDigUpItem(Obj *o, s32 p) {
    if (!gCommManager->isLocalSlot(o->sessionSlot)) {
        if ((o)->testActionFlag(0x1c)) {
            Rec *r = (Rec *)&o->actionWork;
            V3 v;
            v = r->digPos;
            FieldItemFx_StartDigHole(o->sessionSlot, &v, r->holeKind);
            (o)->clearActionFlag(0x1c);
            (o)->clearActionFlag(9);
        }
        if (p != 0x60 && p != 0x61) (o)->clearActionFlag(0xd);
    }
}
}

namespace ns_02209d50 {
extern "C" void PlayerActor_DigUpItemUpdate(Obj *o) {
    s32 a, b;
    V3 p;
    volatile VecFx32 sv;
    Blk b0;
    Blk b1;
    (o)->advanceAnim();
    if (o->animId != 0x49) {
        Rec *r = (Rec *)&o->actionWork;
        p = r->digPos;
        switch (((Bits *)&o->bodyModel.curFrame)->mid) {
        case 5:
            Effect_Create(6, &p, 0, 0);
            break;
        case 6:
            (o)->playSe(0x841);
            (o)->setActionFlag(9);
            break;
        case 12: {
            Effect_Create(8, &p, 0, 0);
            FieldPos_ToUnit(&a, &b, &p);
            Collision_AddDigHole(a, b);
            FieldItemFx_StartDigHole(o->sessionSlot, &p, r->holeKind);
            (o)->setActionFlag(0xd);
            (o)->clearActionFlag(0x1c);
            break;
        }
        }
        if (((Bits *)&o->bodyModel.curFrame)->mid >= 0x18) {
            (o)->clearActionFlag(9);
            V3 *pv = (V3 *)&o->drawPos;
            sv.x = pv->x;
            sv.y = pv->y;
            sv.z = pv->z;
            s16 h = o->drawTilt;
            b0 = o->bodyModel.mtx;
            b1 = o->itemHandMtx;
            (o)->calcHandMtx();
            (o)->updateShownItemPos(0x1000);
            o->drawPos.x = sv.x;
            o->drawPos.y = sv.y;
            o->drawPos.z = sv.z;
            o->drawTilt = h;
            o->bodyModel.mtx = b0;
            o->itemHandMtx = b1;
        } else {
            s32 z2 = o->heldItemJointMtx.m[11];
            s32 y2 = o->heldItemJointMtx.m[10];
            s32 x2 = o->heldItemJointMtx.m[9];
            o->shownItemPos.x = x2;
            o->shownItemPos.y = y2;
            o->shownItemPos.z = z2;
            WorldCurve_FromCurved(&o->shownItemPos, &o->shownItemPos);
        }
    }
}
}

namespace ns_022093bc {
extern "C" void PlayerActor_DigUpItemMessage(Obj *o) {
    CommManager *l;
    struct {
        u32 pad;
        s32 a0, b0, a1, b1, a2, b2;
        Pair p0, p1, p2;
    } ab;
    V3 v0, v1, v2;
    if (o->animId == 0x49) {
        if (((AnimFrameCtrl *)((AnimFrameCtrl *)&o->bodyModel))->isFinished() != 0) (o)->startAnimOnce(0x4f, 3, 0);
        return;
    }
    DigUpItemMsgWork *r5 = (DigUpItemMsgWork *)&o->actionWork;
    u8 *r6 = &r5->talkStep;
    s32 lvl = (u32)(o->bodyModel.curFrame << 4) >> 16;
    l = gCommManager;
    if (l->isLocalSlot(o->sessionSlot) != 0) {
        if (((AnimFrameCtrl *)((AnimFrameCtrl *)&o->bodyModel))->hasPassedFrame(0x13) != 0) Camera_SetMode4();
    }
    if (lvl < 0x19) return;
    if ((o)->turnToCamera(0x400) == 0) return;
    if (((AnimFrameCtrl *)((AnimFrameCtrl *)&o->bodyModel))->isFinished() == 0) return;
    if (l->isLocalSlot(o->sessionSlot) == 0) {
        (o)->clearActionFlag(0x12);
        o->actionPriority = ((Unk_02007694 *)(void *)(o))->getActionDonePriority(o->action);
        return;
    }
    switch (*r6) {
    case 0: {
        if (TalkRequest_AddPlayerMessage() == 0) break;
        *r6 = 1;
        ((Character *)(void *)(o))->attachTalkRequest(o);
        (o)->setActionFlag(0x11);
        void *q = PlayerData_GetCurrent();
        MsgRequest &s = *o;
        ((MsgRequest *)(&s))->setFileName((const char *)((void *)"obj_etc_player"));
        if (Unk_ov003_022099d0_R(&o->actionItem, 0x136a, 0x136a)) {
            if (((PlayerData *)(q))->testFlag(0x27) == 0) {
                o->msgIndex = 0x21;
                ((PlayerData *)(q))->setFlag(0x27);
                goto l54;
            }
        }
        if (Unk_ov003_022099d0_R(&o->actionItem, 0x137b, 0x137b)) {
            if (((PlayerData *)(q))->testFlag(0x28) == 0) {
                o->msgIndex = 0x23;
                ((PlayerData *)(q))->setFlag(0x28);
                goto l54;
            }
        }
        o->msgIndex = 1;
        {
            ItemName name((u16 *)(&o->actionItem));
            ((TalkWindowState *)(o->window))->setNamedSlot(0, &name, 7);
        }
    l54:
        o->window->nextState = 1;
        Bgm_ReleasePriority(0x12);
        Bgm_RequestSilence(0xc, 0, 1);
        Bgm_Request(0xd, 0x39, 0x7f, 1);
        o->msgStep = 0xa;
        break;
    }
    case 1:
        if (o->window != 0) {
            if (o->window->state != 0) *r6 = 2;
        }
        break;
    case 2:
        if (Pocket_FindEmpty() == -1) {
            *r6 = 3;
            o->msgStep = 2;
            break;
        }
        if (o->window == 0) break;
        if (o->window->state != 0) break;
        ((Character *)(void *)(o))->detachTalkRequest(o);
        (o)->clearActionFlag(0x11);
        TalkRequest_FinishPlayerMessage();
        ab.a0 = 0;
        ab.b0 = 0;
        v0.x = r5->digPos.x;
        v0.y = r5->digPos.y;
        v0.z = r5->digPos.z;
        FieldPos_ToUnit(&ab.a0, &ab.b0, &v0);
        ab.p0.x = ab.a0;
        ab.p0.y = ab.b0;
        PlayerActor_RequestDugItemStore(o, &ab.p0, 0, 6, -1);
        Camera_SetModeDefault();
        break;
    case 3:
        if (o->msgStep >= 0xf) {
            *r6 = 4;
            break;
        }
        if (o->window == 0) break;
        if (o->window->state != 0) break;
        ((Character *)(void *)(o))->detachTalkRequest(o);
        (o)->clearActionFlag(0x11);
        ab.a1 = 0;
        ab.b1 = 0;
        v1.x = r5->digPos.x;
        v1.y = r5->digPos.y;
        v1.z = r5->digPos.z;
        FieldPos_ToUnit(&ab.a1, &ab.b1, &v1);
        ab.p1.x = ab.a1;
        ab.p1.y = ab.b1;
        PlayerActor_RequestDugItemStore(o, &ab.p1, 1, 6, -1);
        Camera_SetModeDefault();
        break;
    case 4:
        if (o->window == 0) break;
        if (o->window->state != 0) break;
        if (MenuCtrl_IsFinished() == 0) break;
        ((Character *)(void *)(o))->detachTalkRequest(o);
        (o)->clearActionFlag(0x11);
        TalkRequest_FinishPlayerMessage();
        *r6 = 5;
        break;
    case 5:
        if (o->fieldQuery != -1) break;
        ab.a2 = 0;
        ab.b2 = 0;
        v2.x = r5->digPos.x;
        v2.y = r5->digPos.y;
        v2.z = r5->digPos.z;
        FieldPos_ToUnit(&ab.a2, &ab.b2, &v2);
        u32 hh = o->actionItem;
        ab.p2.x = ab.a2;
        ab.p2.y = ab.b2;
        PlayerActor_RequestBuryItem(o, 2, &ab.p2, hh, 6, -1);
        Camera_SetModeDefault();
        break;
    }
}
}

namespace ns_022093bc {
extern "C" void PlayerActor_MainDigUpItem(Obj *o) {
    PlayerActor_DigUpItemUpdate(o);
    if (gCommManager->isLocalSlot(o->sessionSlot) != 0) {
        if ((u32)(o->bodyModel.curFrame << 4) >> 16 >= 0x19) (o)->moveWithCollision();
    } else {
        V3 v;
        V3 *pv = &((DigUpItemMsgWork *)&o->actionWork)->digPos;
        v.x = pv->x;
        v.y = pv->y;
        v.z = pv->z;
        (o)->netSyncNearPoint(&v);
    }
    (o)->updateBodyCollider();
    PlayerActor_DigUpItemMessage(o);
}
}

namespace ns_022093bc {
extern "C" s32 PlayerActor_RequestDugItemStore(Obj *o, Pair *p, u32 c, u32 d, s32 e) {
    Msg m;
    u8 *pl = (u8 *)m.unk_0c_b;
    (&m)->assign(0x60, d, *(s16 *)&e);
    pl[0] = p->x;
    pl[1] = p->y;
    pl[2] = c;
    s32 r = (o)->pushRequest(&m);
    return r;
}
}

namespace ns_022093bc {
extern "C" void PlayerActor_DugItemStoreSetNetData(u8 *s, u8 a, u8 b, u8 c) {
    s[0] = a;
    s[1] = b;
    s[2] = c;
}
}

namespace ns_022093bc {
extern "C" void PlayerActor_DugItemStoreGetNetData(u8 *s, u8 *a, u8 *b, u8 *c) {
    *a = s[0];
    *b = s[1];
    *c = s[2];
}
}

namespace ns_022093bc {
extern "C" void PlayerActor_SetupDugItemStore(Obj *o, Msg *m) {
    u8 *q = m->unk_0c_b;
    DugItemStoreWork *r = (DugItemStoreWork *)&o->actionWork;
    u8 t2 = q[2];
    u8 t0 = m->unk_0c_b[0];
    u8 t1 = q[1];
    r->step = t2;
    r->digUnitX = t0;
    r->digUnitZ = t1;
    PlayerActor_DugItemStoreSetNetData(o->netData.bytes, t0, t1, t2);
    (o)->startAnimOnce(0x50, 3, 0);
    (o)->playSe(0x4f);
}
}

namespace ns_022093bc {
extern "C" void PlayerActor_NetDugItemStore(Obj *o, s32 a) {
    u8 b[3];
    PlayerActor_DugItemStoreGetNetData(o->netData.bytes, &b[0], &b[1], &b[2]);
    Pair p;
    p.x = b[0];
    p.y = b[1];
    PlayerActor_RequestDugItemStore(o, &p, b[2], 6, a);
}
}

namespace ns_022093bc {
extern "C" void PlayerActor_DugItemStoreShrink(Obj *o) {
    s32 r4;
    (o)->advanceAnim();
    if (o->animId == 0x50) {
        s32 t = (s32)(((u32)(o->bodyModel.curFrame >> 12) << 16) >> 4);
        r4 = 0x1000 - t / 6;
        if (r4 < 0) {
            r4 = 0;
            (o)->clearActionFlag(0xd);
        }
    } else {
        r4 = 0;
    }
    o->shownItemScaleX = r4;
    o->shownItemScaleY = r4;
    o->shownItemScaleZ = r4;
    (o)->updateShownItemPos(r4);
}
}

namespace ns_022093bc {
extern "C" void PlayerActor_DugItemStoreUpdate(Obj *o) {
    DugItemStoreWork *g = (DugItemStoreWork *)&o->actionWork;
    u8 *st = &g->step;
    switch (g->step) {
    case 0:
        if (((AnimFrameCtrl *)((AnimFrameCtrl *)&o->bodyModel))->isFinished() == 0) break;
        o->actionPriority = ((Unk_02007694 *)(void *)(o))->getActionDonePriority(o->action);
        (o)->requestWait(3, 1, -1);
        if (gCommManager->isLocalSlot(o->sessionSlot) == 0) break;
        Pocket_AddFoundItem(&o->actionItem);
        break;
    case 1:
        if (o->animId == 0x50) {
            if (((AnimFrameCtrl *)((AnimFrameCtrl *)&o->bodyModel))->isFinished() == 0) break;
            (o)->startAnim(0x6c, 3, 3);
        }
        if (gCommManager->isLocalSlot(o->sessionSlot) != 0) {
            s32 s = o->msgStep;
            if (s == 5) {
                if (MenuCtrl_OpenPocketsFullDug(o->actionItem) != 0) o->msgStep = 6;
            } else if (s == 6) {
                if (MenuCtrl_IsFinished() == 0) break;
                o->msgStep = 0xf;
                if (MenuCtrl_IsResultOk() == 0) goto l76c;
                (o)->clearActionFlag(0xd);
                if ((o)->testActionFlag(0x11) != 0) {
                    (o)->clearActionFlag(0x11);
                    ((Character *)(void *)(o))->detachTalkRequest(o);
                }
                TalkRequest_FinishPlayerMessage();
                o->actionPriority = ((Unk_02007694 *)(void *)(o))->getActionDonePriority(o->action);
                (o)->requestWait(3, 1, -1);
            } else if (s >= 0xf) {
            l76c:
                if ((o)->testActionFlag(0x11) != 0) {
                    (o)->clearActionFlag(0x11);
                    ((Character *)(void *)(o))->detachTalkRequest(o);
                }
                TalkRequest_FinishPlayerMessage();
                *st = *st + 1;
            }
        } else {
            o->actionPriority = ((Unk_02007694 *)(void *)(o))->getActionDonePriority(o->action);
            (o)->clearActionFlag(0xd);
        }
        break;
    case 2:
        if (o->fieldQuery != -1) break;
        Pair p;
        u32 pb = g->digUnitZ;
        u32 v = o->actionItem;
        u32 pa = g->digUnitX;
        p.x = pa;
        p.y = pb;
        PlayerActor_RequestBuryItem(o, 2, &p, v, 6, -1);
        (o)->clearActionFlag(0xd);
        break;
    }
}
}

namespace ns_022093bc {
extern "C" void PlayerActor_MainDugItemStore(Obj *o) {
    PlayerActor_DugItemStoreShrink(o);
    (o)->updateBodyCollider();
    PlayerActor_DugItemStoreUpdate(o);
}
}

namespace ns_022093bc {
extern "C" s32 PlayerActor_RequestBuryItem(Obj *o, u32 k, Pair *p, u32 v, s32 a, s16 b) {
    Msg m;
    Pay *pl = (PlayerBuryItemArgs *)m.unk_0c_b;
    (&m)->assign(0x61, a, b);
    pl->h = v;
    pl->b4 = k;
    pl->b2 = p->x;
    pl->b3 = p->y;
    s32 r = (o)->pushRequest(&m);
    return r;
}
}

namespace ns_022093bc {
extern "C" void PlayerActor_BuryItemSetNetData(u8 *s, u32 v, u8 a, u8 b, u8 c) {
    NetBuf_WriteU16(s, v);
    s[2] = a;
    s[3] = b;
    s[4] = c;
}
}

namespace ns_022093bc {
extern "C" void PlayerActor_SetupBuryItem(Obj *o, Msg *m) {
    u32 h;
    Pay *pl = (PlayerBuryItemArgs *)m->unk_0c_b;
    BuryItemWork *r = (BuryItemWork *)&o->actionWork;
    u8 k = pl->b4;
    u32 x = pl->b2;
    u32 z = pl->b3;
    V3 v;
    FieldPos_FromUnitCenter(&v, x, z);
    h = (*(PlayerBuryItemArgs *)m->unk_0c_b).h;
    if (k == 2) {
        if (gCommManager->isLocalSlot(o->sessionSlot) != 0) {
            o->fieldQuery = FieldAction_RequestToolAtPendingForAid(o->sessionSlot, 2, 0, o->actionItem);
        }
    } else if (o->animId == 0x6c) {
        k = 3;
    }
    r->buryItem = h;
    r->holeUnitX = x;
    r->holeUnitZ = z;
    r->fillMode = k;
    r->answerOk = 0;
    r->faceAngle = Math_Atan2(v.x - o->position.x, v.z - o->position.z);
    PlayerActor_BuryItemSetNetData(o->netData.bytes, h, (u8)x, (u8)z, k);
    (o)->startAnimOnce(0x49, 7, 0);
    (o)->clearActionFlag(0xd);
}
}

namespace ns_022093bc {
extern "C" void PlayerActor_NetBuryItem() {
}
}

namespace ns_022093bc {
extern "C" void PlayerActor_BuryItemTurn(Obj *o) {
    (o)->turnToward(((BuryItemWork *)&o->actionWork)->faceAngle);
}
}

namespace ns_022093bc {
extern "C" void PlayerActor_BuryItemCheckEnd(Obj *o) {
    BuryItemWork *r = (BuryItemWork *)&o->actionWork;
    if (gCommManager->isLocalSlot(o->sessionSlot) == 0) {
        o->actionPriority = ((Unk_02007694 *)(void *)(o))->getActionDonePriority(o->action);
        return;
    }
    if (o->fieldAnswer == 1) r->answerOk = 1;
    if (((AnimFrameCtrl *)((AnimFrameCtrl *)&o->bodyModel))->isFinished() != 0 || o->animId == 0x4a) {
        if (((AnimFrameCtrl *)((AnimFrameCtrl *)&o->bodyModel))->isFinished() != 0) (o)->startAnim(0x4a, 3, 0);
        if (r->fillMode == 2 && r->answerOk == 0) return;
        o->actionPriority = ((Unk_02007694 *)(void *)(o))->getActionDonePriority(o->action);
        Pair p;
        u32 pb = r->holeUnitZ;
        u32 pa = r->holeUnitX;
        p.x = pa;
        p.y = pb;
        PlayerActor_RequestFillHole(o, r->fillMode, &p, r->buryItem, 6, -1);
    }
}
}

namespace ns_022093bc {
extern "C" void PlayerActor_MainBuryItem(Obj *o) {
    PlayerActor_BuryItemTurn(o);
    (o)->advanceAnim();
    (o)->updateBodyCollider();
    PlayerActor_BuryItemCheckEnd(o);
}
}

namespace ns_022093bc {
extern "C" s32 PlayerActor_RequestFillHole(Obj *o, u32 k, Pair *p, u32 v, s32 a, s16 b) {
    Msg m;
    Pay *pl = (PlayerBuryItemArgs *)m.unk_0c_b;
    (&m)->assign(0x62, a, b);
    pl->b4 = k;
    pl->b2 = p->x;
    pl->b3 = p->y;
    pl->h = v;
    s32 r = (o)->pushRequest(&m);
    return r;
}
}

namespace ns_02208a58 {
extern "C" void PlayerActor_SetupFillHole(Obj *o, Msg *a) {
    V3 v;
    u8 *q = (u8 *)a->unk_0c_b;
    Rec *r4 = (Rec *)&o->actionWork;
    u32 b = q[4];
    FieldPos_FromUnitCenter(&v, q[2], q[3]);
    *(u16 *)&((Rec *)&o->actionWork)->unk_00 = a->unk_0c_h;
    r4->fillMode = b;
    r4->holePos.x = v.x;
    r4->holePos.y = v.y;
    r4->holePos.z = v.z;
    r4->faceAngle = Math_Atan2(v.x - o->position.x, v.z - o->position.z);
    switch (o->animId) {
    case 0x4a:
        (o)->startAnimOnce(0x51, 3, 0);
        break;
    case 0x49:
        if (((AnimFrameCtrl *)((AnimFrameCtrl *)&o->bodyModel))->isFinished()) {
            (o)->startAnimOnce(0x51, 3, 0);
        }
        break;
    default:
        (o)->startAnimOnce(0x49, 3, 0);
        break;
    }
    (o)->clearActionFlag(0xd);
    (o)->setActionFlag(0x1c);
}
}

namespace ns_02208a58 {
extern "C" void PlayerActor_NetFillHole() {}
}

namespace ns_02208a58 {
extern "C" void PlayerActor_EndFillHole(Obj *o) {
    volatile V3 v;
    V3 w;
    V3 x;
    CommManager *g = gCommManager;
    if (!g->isLocalSlot(o->sessionSlot)) {
        if ((o)->testActionFlag(0x1c)) {
            Rec *r = (Rec *)&o->actionWork;
            v.x = r->holePos.x;
            v.y = r->holePos.y;
            v.z = r->holePos.z;
            if (g->isLocalSlot(o->sessionSlot)) {
                w.x = v.x;
                w.y = v.y;
                w.z = v.z;
                FieldItemFx_StartFillHole(o->sessionSlot, &w);
            } else {
                x.x = v.x;
                x.y = v.y;
                x.z = v.z;
                FieldItemFx_StartFillHoleWithItem(o->sessionSlot, &x);
            }
            (o)->clearActionFlag(0x1c);
        }
    }
}
}

namespace ns_02208a58 {
extern "C" void PlayerActor_FillHoleTurn(Obj *o) {
    (o)->turnToward(((Rec *)&o->actionWork)->faceAngle);
}
}

namespace ns_02208a58 {
extern "C" void PlayerActor_FillHoleUpdate(Obj *o) {
    V3 v;
    V3 w;
    V3 x;
    s16 h;
    if (o->animId != 0x49) {
        Rec *r = (Rec *)&o->actionWork;
        v.x = r->holePos.x;
        v.y = r->holePos.y;
        v.z = r->holePos.z;
        h = o->rotY;
        s32 k = ((Unk_ov003_02208a58_Bits *)&o->bodyModel.curFrame)->mid;
        switch (k) {
        case 1:
            Effect_Create(9, &v, &h, 0);
            break;
        case 5:
            if (gCommManager->isLocalSlot(o->sessionSlot)) {
                w.x = v.x;
                w.y = v.y;
                w.z = v.z;
                FieldItemFx_StartFillHole(o->sessionSlot, &w);
            } else {
                u16 *p = BlockMap_GetItemPtrAtPos(gSceneBlockMap, &v, 0);
                BOOL f = FALSE;
                if (*p >= 0xfc && *p <= 0xfd) f = TRUE;
                if (f) {
                    x.x = v.x;
                    x.y = v.y;
                    x.z = v.z;
                    FieldItemFx_StartFillHoleWithItem(o->sessionSlot, &x);
                }
            }
            (o)->clearActionFlag(0x1c);
            break;
        case 7:
            (o)->playSe(0x843);
            (o)->setActionFlag(9);
            break;
        case 0x15:
            Effect_Create(0xa, &v, 0, 0);
            break;
        }
    }
    (o)->advanceAnim();
}
}

namespace ns_02208a58 {
extern "C" void PlayerActor_FillHoleCheckEnd(Obj *o) {
    if (((AnimFrameCtrl *)((AnimFrameCtrl *)&o->bodyModel))->isFinished()) {
        Rec *r5 = (Rec *)&o->actionWork;
        if (o->animId == 0x49) {
            (o)->startAnimOnce(0x51, 3, 0);
        } else {
            o->actionPriority = ((Unk_02007694 *)(void *)(o))->getActionDonePriority(o->action);
            u32 b = r5->fillMode;
            if (b == 1) {
                ((Unk_02007694 *)(void *)(o))->requestAct05(3, 5, -1);
            } else if (gCommManager->isLocalSlot(o->sessionSlot) && b == 3) {
                if ((o)->testActionFlag(0x11)) {
                    (o)->clearActionFlag(0x11);
                    ((Character *)(void *)(o))->detachTalkRequest(o);
                }
                ((Unk_02007694 *)(void *)(o))->requestWaitMenu(3, 1, -1);
            } else {
                (o)->requestWait(3, 1, -1);
            }
        }
    }
}
}

namespace ns_02208a58 {
extern "C" void PlayerActor_MainFillHole(Obj *o) {
    V3 v;
    PlayerActor_FillHoleTurn(o);
    PlayerActor_FillHoleUpdate(o);
    Rec *r = (Rec *)&o->actionWork;
    v.x = r->holePos.x;
    v.y = r->holePos.y;
    v.z = r->holePos.z;
    (o)->netSyncNearPoint(&v);
    (o)->updateBodyCollider();
    PlayerActor_FillHoleCheckEnd(o);
}
}

namespace ns_02208a58 {
extern "C" s32 PlayerActor_RequestWateringCan(Obj *o, s32 a, s16 b) {
    Msg m;
    (&m)->assign(0x63, a, b);
    s32 r = (o)->pushRequest(&m);
    return r;
}
}

namespace ns_02208a58 {
extern "C" void PlayerActor_WateringCanSetNetData(u8 *p, u32 v) {
    *p = v;
}
}

namespace ns_02208a58 {
extern "C" void PlayerActor_WateringCanGetNetData(u8 *p, u8 *out) {
    *out = *p;
}
}

namespace ns_02208a58 {
extern "C" void PlayerActor_SetupWateringCan(Obj *o) {
    (o)->startAnimOnce(0x61, 3, 0);
    ((Rec *)&o->actionWork)->unk_00 = -1;
}
}

namespace ns_02208a58 {
extern "C" s32 PlayerActor_NetWateringCan(Obj *o, s16 b) {
    return PlayerActor_RequestWateringCan(o, 6, b);
}
}

namespace ns_02208a58 {
extern "C" void PlayerActor_EndWateringCan(Obj *o) {
    V3 t;
    if (((Rec *)&o->actionWork)->unk_00 != -1) {
        Effect_End(((Rec *)&o->actionWork)->unk_00);
    }
    PlayerActor_GetFrontPoint(&t, o);
    sWateringActive = 0;
    sWateringPos.x = t.x;
    sWateringPos.y = t.y;
    sWateringPos.z = t.z;
}
}

namespace ns_02208a58 {
extern "C" void PlayerActor_WateringCanUpdate(Obj *o) {
    V3 v;
    V3 t;
    V3 t2;
    (o)->advanceAnim();
    s32 k = ((Unk_ov003_02208a58_Bits *)&o->bodyModel.curFrame)->mid;
    s32 *h = &((Rec *)&o->actionWork)->unk_00;
    if (k >= 5 && k <= 0x23) {
        s32 z = o->heldItemJointMtx.m[11];
        s32 y = o->heldItemJointMtx.m[10];
        v.x = o->heldItemJointMtx.m[9];
        v.y = y;
        v.z = z;
        WorldCurve_FromCurved(&v, &v);
        if (*h == -1) {
            *h = Effect_Create(0x26, &v, &o->rotY, 0);
            (o)->playSe(0x854);
            PlayerActor_GetFrontPoint(&t, o);
            sWateringActive = 1;
            sWateringPos.x = t.x;
            sWateringPos.y = t.y;
            sWateringPos.z = t.z;
        } else {
            Effect_SetPosition(*h, &v, 0, 0);
        }
        switch (k) {
        case 0x14:
            PlayerActor_GetFrontPoint(&t2, o);
            v.x = t2.x;
            v.y = t2.y;
            v.z = t2.z;
            (o)->startFieldQuery(&v, 1, 5);
            break;
        case 0x23:
            if (*h != -1) {
                Effect_End(*h);
                *h = -1;
            }
            break;
        }
    }
}
}

namespace ns_02208a58 {
extern "C" void PlayerActor_WateringCanCheckEnd(Obj *o) {
    if (((AnimFrameCtrl *)((AnimFrameCtrl *)&o->bodyModel))->isFinished()) {
        o->actionPriority = ((Unk_02007694 *)(void *)(o))->getActionDonePriority(o->action);
        (o)->requestWait(3, 1, -1);
    }
    u8 *p6 = o->netData.bytes;
    PlayerActor_WateringCanSetNetData(p6, 0);
    if (((Unk_ov003_02208a58_Bits *)&o->bodyModel.curFrame)->mid >= 0x21 && PlayerActor_ResumeWalkOrIdle(o)) {
        s32 *p4 = &((Rec *)&o->actionWork)->unk_00;
        PlayerActor_WateringCanSetNetData(p6, 1);
        if (((Rec *)&o->actionWork)->unk_00 != -1) {
            Effect_End(((Rec *)&o->actionWork)->unk_00);
            *p4 = -1;
        }
    }
}
}

namespace ns_02208a58 {
extern "C" void PlayerActor_WateringCanCheckEndRemote(Obj *o) {
    u8 b;
    u32 out;
    o->actionPriority = ((Unk_02007694 *)(void *)(o))->getActionDonePriority(o->action);
    u8 *p6 = o->netData.bytes;
    PlayerActor_WateringCanGetNetData(p6, &b);
    if (b != 0 && PlayerActor_GetSlotAction(&out, -1, o->sessionSlot) && out == 0x63) {
        PlayerActor_RequestWateringCan(o, 6, -1);
        s32 *p4 = &((Rec *)&o->actionWork)->unk_00;
        PlayerActor_WateringCanSetNetData(p6, 1);
        if (((Rec *)&o->actionWork)->unk_00 != -1) {
            Effect_End(((Rec *)&o->actionWork)->unk_00);
            *p4 = -1;
        }
    } else {
        if (((AnimFrameCtrl *)((AnimFrameCtrl *)&o->bodyModel))->isFinished()) {
            (o)->requestWait(3, 1, -1);
        }
    }
}
}

namespace ns_02208a58 {
extern "C" void PlayerActor_MainWateringCan(Obj *o) {
    PlayerActor_WateringCanUpdate(o);
    (o)->updateBodyCollider();
    (o)->submitSceneCollider();
    if (gCommManager->isLocalSlot(o->sessionSlot)) {
        PlayerActor_WateringCanCheckEnd(o);
    } else {
        PlayerActor_WateringCanCheckEndRemote(o);
    }
}
}

namespace ns_02208a58 {
extern "C" s32 PlayerActor_RequestSlingshot(Obj *o, s32 a, s16 b) {
    Msg m;
    (&m)->assign(0x64, a, b);
    s32 r = (o)->pushRequest(&m);
    return r;
}
}

namespace ns_02208a58 {
extern "C" void PlayerActor_SetupSlingshot(Obj *o) {
    if (gCommManager->isLocalSlot(o->sessionSlot) && !TalkRequest_AddSlingshot()) {
        o->actionPriority = ((Unk_02007694 *)(void *)(o))->getActionDonePriority(o->action);
        (o)->requestWait(3, 1, -1);
    } else {
        Sky_OnSlingshotFired(o->sessionSlot);
        (o)->startAnimOnce(0x5f, 3, 0);
        HeldItemModel_PlayAnim(&o->heldItemModel, 0x22, 3, 1);
    }
}
}

namespace ns_02208a58 {
extern "C" s32 PlayerActor_NetSlingshot(Obj *o, s16 b) {
    return PlayerActor_RequestSlingshot(o, 6, b);
}
}

namespace ns_02208a58 {
extern "C" void PlayerActor_SlingshotUpdate(Obj *o) {
    V3 v;
    V3 w;
    (o)->turnAwayFromCamera(0x400);
    (o)->advanceAnim();
    if (o->animId == 0x5f) {
        v.x = o->bodyPos.x;
        v.y = o->bodyPos.y;
        v.z = o->bodyPos.z;
        v.y += 0xb00;
        v.z += 0x300;
        if (((Unk_ov003_02208a58_Bits *)&o->bodyModel.curFrame)->mid < 0xb) {
            (o)->playSeAt(0x851, &v);
        } else if (((AnimFrameCtrl *)((AnimFrameCtrl *)&o->bodyModel))->hasPassedFrame(0xb)) {
            ((FishBobber *)((u8 *)&o->heldItemModel.bobber))->setState(9);
            w.x = v.x;
            w.y = v.y;
            w.z = v.z;
            ((FishBobber *)((u8 *)&o->heldItemModel.bobber))->setPos(&w);
            (o)->playSe(0x852);
            (o)->playSe(0x853);
            (o)->setActionFlag(9);
        }
    }
}
}

namespace ns_02208a58 {
extern "C" void PlayerActor_SlingshotCheckEnd(Obj *o) {
    if (((AnimFrameCtrl *)((AnimFrameCtrl *)&o->bodyModel))->isFinished()) {
        (o)->startAnim(0x60, 3, 3);
        if (!gCommManager->isLocalSlot(o->sessionSlot)) {
            o->actionPriority = ((Unk_02007694 *)(void *)(o))->getActionDonePriority(o->action);
        }
    }
}
}

namespace ns_02208a58 {
extern "C" void PlayerActor_MainSlingshot(Obj *o) {
    PlayerActor_SlingshotUpdate(o);
    (o)->updateBodyCollider();
    PlayerActor_SlingshotCheckEnd(o);
}
}

namespace ns_02208a58 {
extern "C" s32 PlayerActor_RequestSlingshotWatch(Obj *o, s32 a, s16 b) {
    Msg m;
    (&m)->assign(0x65, a, b);
    s32 r = (o)->pushRequest(&m);
    return r;
}
}

namespace ns_02208a58 {
extern "C" void PlayerActor_SetupSlingshotWatch(Obj *o) {
    (o)->startAnim(0, 3, 0);
}
}

namespace ns_02208a58 {
extern "C" void PlayerActor_NetSlingshotWatch() {}
}

namespace ns_02208a58 {
extern "C" void PlayerActor_MainSlingshotWatch(Obj *o) {
    (o)->advanceAnim();
    (o)->updateBodyCollider();
    (o)->submitSceneCollider();
}
}

namespace ns_02208a58 {
extern "C" void PlayerActor_Act66SetArgs(u8 *d, Pair v, u32 c) {
    d[0] = v.a;
    d[1] = v.b;
    d[2] = c;
}
}

namespace ns_02208a58 {
extern "C" s32 PlayerActor_RequestAct66(Obj *o, Pair *p, u32 c, s32 b, s16 d) {
    Msg m;
    (&m)->assign(0x66, b, d);
    PlayerActor_Act66SetArgs((u8 *)m.unk_0c_b, *p, c);
    s32 r = (o)->pushRequest(&m);
    return r;
}
}

namespace ns_02208a58 {
extern "C" void PlayerActor_Act66SetWork(ns_02208108::Act66Work *r, u32 a, u32 b, V3 *v) {
    r->answer = a;
    r->waitFrames = 0;
    r->angle = b;
    r->unk_00 = v->x;
    r->unk_04 = v->y;
    r->unk_08 = v->z;
}
}

namespace ns_02208a58 {
extern "C" void PlayerActor_Act66SetNetData(u8 *p, Pair *v) {
    p[0] = v->a;
    p[1] = v->b;
}
}

namespace ns_02208a58 {
extern "C" void PlayerActor_Act66GetNetData(u8 *p, Pair *out) {
    out->a = p[0];
    out->b = p[1];
}
}

namespace ns_02208108 {
extern "C" void PlayerActor_SetupAct66(Obj *o, u8 *m) {
    u8 *q = ((PlayerActionRequest *)(void *)m)->unk_0c_b;
    RecC *r = (RecC *)o->actionWorkRaw;
    s32 a = ((PlayerActionRequest *)(void *)m)->unk_0c_b[0];
    s32 b = q[1];
    s32 c = q[2];
    Pair p;
    VD d;
    V3 pos;
    FieldPos_FromUnitCenter(&pos, a, b);
    d.x = pos.x - o->position.x;
    d.z = pos.z - o->position.z;
    s32 ang = Math_Atan2(d.x, d.z);
    PlayerActor_Act66SetWork(r, c, ang, &pos);
    p.a = a;
    p.b = b;
    PlayerActor_Act66SetNetData(o->netData.bytes, &p);
    (o)->startAnim(0x64, 3, 0);
    if (r->answer == 1) {
        (o)->setActionFlag(0x1c);
    }
}
}

namespace ns_02208108 {
extern "C" void PlayerActor_NetAct66(Obj *o, s32 a) {
    if (o->action == 0x66) {
        o->netSeq = a;
    } else {
        Pair p;
        p.a = 0;
        p.b = 0;
        PlayerActor_Act66GetNetData(o->netData.bytes, &p);
        PlayerActor_RequestAct66(o, &p, 0, 6, a);
    }
}
}

namespace ns_02208108 {
extern "C" void PlayerActor_EndAct66(Obj *o, s32 a) {
    if (gCommManager->isLocalSlot(o->sessionSlot) == 0) {
        if (a == 2) {
            (o)->clearActionFlag(0x1c);
        } else if (a != 0x67) {
            if ((o)->testActionFlag(0x1c)) {
                V3 *p = (V3 *)o->actionWorkRaw;
                FieldItemFx_StartHoleShrink(o->sessionSlot, p);
                (o)->clearActionFlag(0x1c);
            }
        } else {
            (o)->clearActionFlag(0x1c);
        }
    } else {
        (o)->clearActionFlag(0x1c);
    }
}
}

namespace ns_02208108 {
extern "C" void PlayerActor_Act66Turn(Obj *o) {
    s16 h = o->rotY;
    PlayerActor_TurnAngle(&h, *(s16 *)&o->actionWorkRaw[0xc]);
    (o)->setAngleY(&h);
}
}

namespace ns_02208108 {
extern "C" void PlayerActor_Act66CheckEnd(Obj *o) {
    RecC *r = (RecC *)o->actionWorkRaw;
    CommManager *g = gCommManager;
    if (g->isLocalSlot(o->sessionSlot) != 0) {
        if (r->answer == 0) {
            if (o->fieldAnswer == 1) {
                r->answer = 1;
            } else if (o->fieldAnswer == 2) {
                r->answer = 2;
            }
        }
    }
    if (r->waitFrames < 2) {
        r->waitFrames++;
    } else {
        if (g->isLocalSlot(o->sessionSlot) == 0) {
            o->actionPriority = ((Unk_02007694 *)(void *)(o))->getActionDonePriority(o->action);
        }
        switch (r->answer) {
        case 0:
            break;
        case 1: {
            V3 v;
            v.x = r->unk_00;
            v.y = r->unk_04;
            v.z = r->unk_08;
            PlayerActor_RequestAct67(o, &v, 6, -1);
            break;
        }
        case 2:
            o->actionPriority = ((Unk_02007694 *)(void *)(o))->getActionDonePriority(o->action);
            (o)->requestWait(3, 1, -1);
            break;
        }
    }
}
}

namespace ns_02208108 {
extern "C" void PlayerActor_MainAct66(Obj *o) {
    RecC *r = (RecC *)o->actionWorkRaw;
    (o)->advanceAnim();
    V3 v;
    v.x = o->actionWork;
    v.y = r->unk_04;
    v.z = r->unk_08;
    if ((o)->netSyncNearPoint((VecFx32 *)(void *)(&v))) {
        PlayerActor_Act66Turn(o);
    }
    (o)->updateBodyCollider();
    PlayerActor_Act66CheckEnd(o);
}
}

namespace ns_02208108 {
extern "C" void PlayerActor_Act67SetArgs(V3 *d, V3 v) {
    *d = v;
}
}

namespace ns_02208108 {
extern "C" s32 PlayerActor_RequestAct67(Obj *o, V3 *v, s32 a, s16 b) {
    Msg m;
    (&m)->assign(0x67, a, b);
    V3 t;
    FieldPos_SnapToUnitCenter(&t, v);
    PlayerActor_Act67SetArgs((V3 *)m.unk_0c_b, t);
    return (o)->pushRequest(&m);
}
}

namespace ns_02208108 {
extern "C" void PlayerActor_Act67SetWork(Act67Work *r, s32 a, V3 v) {
    r->angle = a;
    r->pos = v;
}
}

namespace ns_02208108 {
extern "C" void PlayerActor_SetupAct67(Obj *o, u8 *m) {
    VD d;
    u8 *q = ((PlayerActionRequest *)(void *)m)->unk_0c_b;
    V3D l(*(s32 *)(((PlayerActionRequest *)(void *)m)->unk_0c_b), *(s32 *)(q + 4), *(s32 *)(q + 8));
    d.x = l.x - o->position.x;
    d.z = l.z - o->position.z;
    s32 ang = Math_Atan2(d.x, d.z);
    PlayerActor_Act67SetWork((Act67Work *)o->actionWorkRaw, ang, l);
    (o)->startAnimOnce(0x65, 3, 0);
    FieldItemFx_StartHoleShrink(o->sessionSlot, &l);
    (o)->clearActionFlag(0x1c);
    (o)->playSe(0x7da);
    (o)->setActionFlag(9);
}
}

namespace ns_02208108 {
extern "C" void PlayerActor_NetAct67(Obj *o) {
}
}

namespace ns_02208108 {
extern "C" void PlayerActor_Act67Effect(Obj *o) {
    (o)->advanceAnim();
    if (o->animId == 0x65) {
        if (((AnimFrameCtrl *)((AnimFrameCtrl *)&o->bodyModel))->hasPassedFrame(3) || ((AnimFrameCtrl *)((AnimFrameCtrl *)&o->bodyModel))->hasPassedFrame(0xd)) {
            Effect_Create(0x23, (V3 *)&o->position, &o->rotY, 0);
        }
    }
}
}

namespace ns_02208108 {
extern "C" void PlayerActor_Act67Turn(Obj *o) {
    s16 h = o->rotY;
    PlayerActor_TurnAngle(&h, *(s16 *)&o->actionWorkRaw[0]);
    (o)->setAngleY(&h);
}
}

namespace ns_02208108 {
extern "C" void PlayerActor_Act67CheckEnd(Obj *o) {
    if (((AnimFrameCtrl *)((AnimFrameCtrl *)&o->bodyModel))->isFinished()) {
        o->actionPriority = ((Unk_02007694 *)(void *)(o))->getActionDonePriority(o->action);
        (o)->requestWait(3, 1, -1);
    }
    if (((Unk_ov003_02208190_Bits *)&o->bodyModel.curFrame)->mid >= 0x16) {
        PlayerActor_ResumeWalkOrIdle(o);
    }
}
}

namespace ns_02208108 {
extern "C" void PlayerActor_Act67CheckEndRemote(Obj *o) {
    o->actionPriority = ((Unk_02007694 *)(void *)(o))->getActionDonePriority(o->action);
    if (((AnimFrameCtrl *)((AnimFrameCtrl *)&o->bodyModel))->isFinished()) {
        (o)->requestWait(3, 1, -1);
    }
}
}

namespace ns_02208108 {
extern "C" void PlayerActor_MainAct67(Obj *o) {
    Act67Work *r = (Act67Work *)o->actionWorkRaw;
    PlayerActor_Act67Effect(o);
    V3 v;
    v.x = r->pos.x;
    v.y = r->pos.y;
    v.z = r->pos.z;
    if ((o)->netSyncNearPoint((VecFx32 *)(void *)(&v))) {
        PlayerActor_Act67Turn(o);
    }
    (o)->updateBodyCollider();
    if (gCommManager->isLocalSlot(o->sessionSlot)) {
        PlayerActor_Act67CheckEnd(o);
    } else {
        PlayerActor_Act67CheckEndRemote(o);
    }
}
}

namespace ns_02208108 {
extern "C" void PlayerActor_TreeShakeSetArgs(void *p, s32 a, s32 b, u8 c, u8 d) {
    u8 *q = (u8 *)p;
    *(s32 *)q = a;
    *(s32 *)(q + 4) = b;
    q[8] = c;
    q[9] = d;
}
}

namespace ns_02208108 {
extern "C" s32 PlayerActor_RequestTreeShake(Obj *o, s32 x, s32 z, bool a, bool b, s32 c, s32 d) {
    Msg m;
    (&m)->assign(0x68, c, *(s16 *)&d);
    PlayerActor_TreeShakeSetArgs(m.unk_0c_b, x, z, a, b);
    return (o)->pushRequest(&m);
}
}

namespace ns_02208108 {
extern "C" void PlayerActor_TreeShakeSetWork(RecA *r, s32 a, s32 x, s32 z, u8 b, u8 c) {
    r->faceAngle = a;
    r->treePosX = x;
    r->treePosZ = z;
    r->unk_0b = b;
    r->doRelease = c;
    r->shakeFrames = 0;
}
}

namespace ns_02208108 {
extern "C" void PlayerActor_TreeShakeSetNetData(u8 *p, s32 a, s32 b, u8 c) {
    NetBuf_PackPair20(p, a, b);
    p[5] = c;
}
}

namespace ns_02208108 {
extern "C" void PlayerActor_TreeShakeGetNetData(u8 *p, s32 *a, s32 *b, u8 *c) {
    NetBuf_UnpackPair20(p, (u32 *)a, (u32 *)b);
    *c = p[5];
}
}

namespace ns_02208108 {
extern "C" void PlayerActor_SetupTreeShake(Obj *o, u8 *m) {
    u8 *q = ((PlayerActionRequest *)(void *)m)->unk_0c_b;
    u8 b0 = q[8];
    u8 b1 = q[9];
    VD d;
    V3D v(*(s32 *)(((PlayerActionRequest *)(void *)m)->unk_0c_b), o->position.y, *(s32 *)(q + 4));
    d.x = v.x - o->position.x;
    d.z = v.z - o->position.z;
    s32 ang = Math_Atan2(d.x, d.z);
    PlayerActor_TreeShakeSetWork((RecA *)o->actionWorkRaw, ang, v.x, v.z, b0, b1);
    PlayerActor_TreeShakeSetNetData(o->netData.bytes, v.x, v.z, b0 | (b1 << 4));
    if (b1 != 0) {
        (o)->startAnimOnce(0x66, 3, 0);
    } else {
        (o)->startAnim(0x66, 3, 0);
    }
    (o)->setActionFlag(9);
}
}

namespace ns_02208108 {
extern "C" void PlayerActor_NetTreeShake(Obj *o, s32 a) {
    if (o->action == 0x68) {
        o->netSeq = a;
    } else {
        s32 x, z;
        u8 f;
        PlayerActor_TreeShakeGetNetData(o->netData.bytes, &x, &z, &f);
        bool t0 = f != 0;
        bool t1 = (f >> 4) != 0;
        PlayerActor_RequestTreeShake(o, x, z, t0, t1, 6, a);
    }
}
}

namespace ns_02208108 {
extern "C" void PlayerActor_EndTreeShake(Obj *o) {
    if (gCommManager->isLocalSlot(o->sessionSlot) == 0) {
        RecA *r = (RecA *)o->actionWorkRaw;
        if (r->shakeFrames < 5) {
            V3 v(r->treePosX, 0, r->treePosZ);
            Pair p;
            p.a = 0;
            p.b = 0;
            FieldPos_ToUnit(&p.a, &p.b, &v);
            Tree_RequestShake(o->sessionSlot, FieldUnitPos(p.a, p.b), 2);
        }
    }
}
}

namespace ns_02208108 {
extern "C" void PlayerActor_TreeShakeTurn(Obj *o) {
    s16 h = o->rotY;
    PlayerActor_TurnAngle(&h, *(s16 *)&o->actionWorkRaw[8]);
    (o)->setAngleY(&h);
}
}

namespace ns_02208108 {
extern "C" void PlayerActor_TreeShakeCheckEnd(Obj *o) {
    RecA *r = (RecA *)o->actionWorkRaw;
    if (r->doRelease != 0) {
        if (((AnimFrameCtrl *)((AnimFrameCtrl *)&o->bodyModel))->isFinished()) {
            PlayerActor_RequestTreeShakeRelease(o, r->treePosX, r->treePosZ, r->unk_0b, 1, 6, -1);
        }
    } else {
        s32 t = o->fieldAnswer;
        if (t == 1) {
            r->doRelease = 1;
            o->bodyModel.playMode = 1;
        } else if (t == 2) {
            o->actionPriority = ((Unk_02007694 *)(void *)(o))->getActionDonePriority(o->action);
            (o)->requestWait(3, 1, -1);
        }
    }
}
}

namespace ns_02208108 {
extern "C" void PlayerActor_TreeShakeCheckEndRemote(Obj *o) {
    RecA *r = (RecA *)o->actionWorkRaw;
    o->actionPriority = ((Unk_02007694 *)(void *)(o))->getActionDonePriority(o->action);
    if (r->doRelease != 0) {
        if (((AnimFrameCtrl *)((AnimFrameCtrl *)&o->bodyModel))->isFinished()) {
            PlayerActor_RequestTreeShakeRelease(o, r->treePosX, r->treePosZ, r->unk_0b, 1, 6, -1);
        }
    }
}
}

namespace ns_02208108 {
extern "C" void PlayerActor_MainTreeShake(Obj *o) {
    RecA *r = (RecA *)o->actionWorkRaw;
    V3 v(r->treePosX, 0, r->treePosZ);
    Pair p;
    p.a = 0;
    p.b = 0;
    FieldPos_ToUnit(&p.a, &p.b, &v);
    if (r->shakeFrames > 5) {
        Tree_KeepShaking(o->sessionSlot, &p.a); // (unit x, z)
    } else {
        r->shakeFrames++;
        if (r->shakeFrames == 5) {
            Tree_RequestShake(o->sessionSlot, FieldUnitPos(p.a, p.b), 2);
        }
    }
    (o)->advanceAnim();
    if (gCommManager->isLocalSlot(o->sessionSlot)) {
        PlayerActor_TreeShakeTurn(o);
        (o)->updateBodyCollider();
        PlayerActor_TreeShakeCheckEnd(o);
    } else {
        if ((o)->netFollowTransform()) {
            PlayerActor_TreeShakeTurn(o);
        }
        (o)->updateBodyCollider();
        PlayerActor_TreeShakeCheckEndRemote(o);
    }
}
}

namespace ns_02208108 {
extern "C" void PlayerActor_TreeShakeReleaseSetArgs(void *p, s32 a, s32 b, u8 c, u8 d) {
    u8 *q = (u8 *)p;
    *(s32 *)q = a;
    *(s32 *)(q + 4) = b;
    q[8] = c;
    q[9] = d;
}
}

namespace ns_02208108 {
extern "C" s32 PlayerActor_RequestTreeShakeRelease(Obj *o, s32 x, s32 z, u8 a, u8 b, s32 c, s16 d) {
    Msg m;
    (&m)->assign(0x69, c, d);
    PlayerActor_TreeShakeReleaseSetArgs(m.unk_0c_b, x, z, a, b);
    return (o)->pushRequest(&m);
}
}

namespace ns_02208108 {
extern "C" void PlayerActor_TreeShakeReleaseSetWork(void *p, s16 a, s32 b, s32 c, u8 d, u8 e) {
    u8 *q = (u8 *)p;
    q[0] = 0;
    *(s16 *)(q + 2) = a;
    *(s32 *)(q + 4) = b;
    *(s32 *)(q + 8) = c;
    q[0xc] = d;
    q[0xd] = e;
}
}

namespace ns_022077c8 {
extern "C" void PlayerActor_SetupTreeShakeRelease(Obj *o, Msg *m) {
    s32 o1, o2;
    VecFx32CtorDtor d;
    V3 t;
    V3 *r0 = (VecFx32 *)m->unk_0c_b;
    u8 r4 = ((u8 *)&r0->z)[0];
    u8 r6 = ((u8 *)&r0->z)[1];
    s32 r2 = r0->y;
    s32 y = o->position.y;
    t.x = (*(VecFx32 *)m->unk_0c_b).x;
    t.y = y;
    t.z = r2;
    d.x = t.x - o->position.x;
    d.z = r2 - o->position.z;
    s32 r7 = Math_Atan2(d.x, d.z);
    o1 = 0;
    o2 = 0;
    FieldPos_ToUnit(&o1, &o2, &t);
    PlayerActor_TreeShakeReleaseSetWork(o->actionWorkRaw, r7, o1, o2, r4, r6);
    (o)->startAnimOnce(0x67, 3, 0);
}
}

namespace ns_022077c8 {
extern "C" void PlayerActor_NetTreeShakeRelease() {
}
}

namespace ns_022077c8 {
extern "C" void PlayerActor_TreeShakeReleaseUpdate(Obj *o) {
    FieldUnitPos p;
    (o)->advanceAnim();
    if (((AnimFrameCtrl *)((AnimFrameCtrl *)&o->bodyModel))->hasPassedFrame(5)) {
        s32 *r0 = &o->actionWork;
        s32 y = r0[2];
        s32 x = r0[1];
        p.x = x;
        p.z = y;
        Tree_RequestShake(o->sessionSlot, p, 3);
        (o)->clearActionFlag(0x12);
        (o)->setActionFlag(9);
    }
}
}

namespace ns_022077c8 {
extern "C" void PlayerActor_TreeShakeReleaseTurn(Obj *o) {
    s16 v = o->rotY;
    PlayerActor_TurnAngle(&v, *(s16 *)&o->actionWorkRaw[2]);
    (o)->setAngleY(&v);
}
}

namespace ns_022077c8 {
extern "C" void PlayerActor_TreeShakeReleaseCheckEnd(Obj *o) {
    u8 r4 = o->actionWorkRaw[0xc];
    if (((AnimFrameCtrl *)((AnimFrameCtrl *)&o->bodyModel))->isFinished()) {
        o->actionPriority = ((Unk_02007694 *)(void *)(o))->getActionDonePriority(o->action);
        if (r4 != 0) {
            (o)->requestAct77(o->rotY, 6, -1);
        } else {
            (o)->requestWait(3, 1, -1);
        }
    }
}
}

namespace ns_022077c8 {
extern "C" void PlayerActor_MainTreeShakeRelease(Obj *o) {
    Pair p;
    s32 *r4 = &o->actionWork;
    PlayerActor_TreeShakeReleaseUpdate(o);
    if (gCommManager->isLocalSlot(o->sessionSlot)) {
        PlayerActor_TreeShakeReleaseTurn(o);
        (o)->updateBodyCollider();
    } else {
        s32 y = r4[2];
        s32 x = r4[1];
        p.x = x;
        p.y = y;
        if ((o)->netSyncNearUnit((s32 *)(void *)(&p))) {
            PlayerActor_TreeShakeReleaseTurn(o);
        }
        (o)->updateBodyCollider();
    }
    PlayerActor_TreeShakeReleaseCheckEnd(o);
}
}

namespace ns_022077c8 {
extern "C" void PlayerActor_MailboxOpenSetArgs(V3 *d, V3 v) {
    d->x = v.x;
    d->y = v.y;
    d->z = v.z;
}
}

namespace ns_022077c8 {
extern "C" s32 PlayerActor_RequestMailboxOpen(Obj *o, V3 v, s32 a, s16 b) {
    Msg m;
    (&m)->assign(0x6a, a, b);
    PlayerActor_MailboxOpenSetArgs((VecFx32 *)m.unk_0c_b, v);
    s32 r = (o)->pushRequest(&m);
    return r;
}
}

namespace ns_022077c8 {
extern "C" void PlayerActor_MailboxOpenSetWork(V3 *d, V3 v) {
    d->x = v.x;
    d->y = v.y;
    d->z = v.z;
}
}

namespace ns_022077c8 {
extern "C" void PlayerActor_MailboxOpenSetNetData(u8 *a, V3 *v) {
    NetBuf_PackTriple20(a, v->x, v->y, v->z, 0);
}
}

namespace ns_022077c8 {
extern "C" void PlayerActor_MailboxOpenGetNetData(u8 *a, V3 *v) {
    u8 t;
    NetBuf_UnpackTriple20(a, (u32 *)&v->x, &v->y, (u32 *)&v->z, &t);
}
}

namespace ns_022077c8 {
extern "C" void PlayerActor_SetupMailboxOpen(Obj *o, Msg *m) {
    V3 v;
    (o)->startAnimOnce(0x7a, 3, 0);
    V3 *pv = (VecFx32 *)m->unk_0c_b;
    v.x = pv->x;
    v.y = pv->y;
    v.z = pv->z;
    PlayerActor_MailboxOpenSetWork((V3 *)&o->actionWork, v);
    PlayerActor_MailboxOpenSetNetData(o->netData.bytes, &v);
}
}

namespace ns_022077c8 {
extern "C" void PlayerActor_NetMailboxOpen(Obj *o, s16 a) {
    V3 v;
    PlayerActor_MailboxOpenGetNetData(o->netData.bytes, &v);
    PlayerActor_RequestMailboxOpen(o, v, 6, a);
}
}

namespace ns_022077c8 {
extern "C" void PlayerActor_MailboxOpenMove(Obj *o) {
    s32 *r2 = &o->actionWork;
    PlayerActor_StepTowardPose(o, r2[0], r2[2], -0x8000);
}
}

namespace ns_022077c8 {
extern "C" void PlayerActor_MailboxOpenCheckEnd(Obj *o) {
    if (((AnimFrameCtrl *)((AnimFrameCtrl *)&o->bodyModel))->isFinished()) {
        PlayerActor_RequestMailboxWait(o, 6, -1);
    }
}
}

namespace ns_022077c8 {
extern "C" void PlayerActor_MainMailboxOpen(Obj *o) {
    PlayerActor_MailboxOpenMove(o);
    (o)->advanceAnim();
    (o)->updateBodyCollider();
    PlayerActor_MailboxOpenCheckEnd(o);
}
}

namespace ns_022077c8 {
extern "C" s32 PlayerActor_RequestMailboxWait(Obj *o, s32 a, s16 b) {
    Msg m;
    (&m)->assign(0x6b, a, b);
    s32 r = (o)->pushRequest(&m);
    return r;
}
}

namespace ns_022077c8 {
extern "C" void PlayerActor_SetupMailboxWait(Obj *o) {
    if (o->animId != 0x7a) {
        (o)->startAnimOnce(0x38, 3, 0);
    }
    u32 t = ((Unk_ov003_022077c8_Bits *)&o->bodyModel.numFrames)->mid - 1;
    *(u32 *)(Unk_ov003_022077c8_Bits *)&o->bodyModel.curFrame = (t << 16) >> 4;
}
}

namespace ns_022077c8 {
extern "C" s32 PlayerActor_NetMailboxWait(Obj *o, s16 a) {
    return PlayerActor_RequestMailboxWait(o, 6, a);
}
}

namespace ns_022077c8 {
extern "C" void PlayerActor_MainMailboxWait(Obj *o) {
    (o)->advanceAnim();
    (o)->updateBodyCollider();
}
}

namespace ns_022077c8 {
extern "C" s32 PlayerActor_RequestMailboxClose(Obj *o, s32 a, s16 b) {
    Msg m;
    (&m)->assign(0x6c, a, b);
    s32 r = (o)->pushRequest(&m);
    return r;
}
}

namespace ns_022077c8 {
extern "C" void PlayerActor_SetupMailboxClose(Obj *o) {
    if (o->animId != 0x7a) {
        (o)->startAnimOnce(0x7a, 3, 0);
    }
    u32 v = ((Unk_ov003_022077c8_Bits *)&o->bodyModel.numFrames)->mid;
    ((AnimFrameCtrl *)((AnimFrameCtrl *)&o->bodyModel))->setup(v, 3, 0x1000, (u16)(v - 1));
}
}

namespace ns_022077c8 {
extern "C" s32 PlayerActor_NetMailboxClose(Obj *o, s16 a) {
    return PlayerActor_RequestMailboxClose(o, 6, a);
}
}

namespace ns_022077c8 {
extern "C" void PlayerActor_MailboxCloseUpdate(Obj *o) {
    (o)->advanceAnim();
    if (((AnimFrameCtrl *)((AnimFrameCtrl *)&o->bodyModel))->isFinished()) {
        (o)->startAnim(0, 3, 3);
        if (gCommManager->isLocalSlot(o->sessionSlot) == 0) {
            o->actionPriority = ((Unk_02007694 *)(void *)(o))->getActionDonePriority(o->action);
        }
    }
}
}

namespace ns_022077c8 {
extern "C" void PlayerActor_MainMailboxClose(Obj *o) {
    PlayerActor_MailboxCloseUpdate(o);
    (o)->updateBodyCollider();
}
}

namespace ns_022077c8 {
extern "C" s32 PlayerActor_RequestFaint(Obj *o, u32 a, s32 b, s16 c) {
    Msg m;
    (&m)->assign(0x6d, b, c);
    *(u8 *)m.unk_0c_b = a;
    s32 r = (o)->pushRequest(&m);
    return r;
}
}

namespace ns_022077c8 {
extern "C" void PlayerActor_SetupFaint(Obj *o, Msg *m) {
    u8 b = *(u8 *)m->unk_0c_b;
    s32 t = (o)->getHeldToolKind();
    if (t == 3 || t == 4 || t == 10) {
        (o)->startAnimOnce(0x69, 3, 6);
        if (t == 3) {
            HeldItemModel_PlayAnim(&o->heldItemModel, 0x14, 3, 1);
        } else if (t == 4) {
            HeldItemModel_PlayAnim(&o->heldItemModel, 2, 9, 1);
        }
    } else {
        (o)->startAnimOnce(0x68, 3, 6);
    }
    TalkRequest_AddLeaveRoom();
    s32 *r4 = &o->actionWork;
    *r4 = Effect_Create(0x34, &o->headTopPos, 0, 0);
    ((u8 *)r4)[4] = b;
    if (b >= 2) {
        Snd_PlaySe(0x51);
        if (b == 2) {
            (o)->playSe(0x839);
        } else {
            (o)->playSe(0x838);
        }
        Bgm_RequestSilence(0xc, 0, 0);
    }
    (o)->playSe(0x816);
}
}

namespace ns_022077c8 {
extern "C" s32 PlayerActor_NetFaint(Obj *o, s16 a) {
    return PlayerActor_RequestFaint(o, 0, 7, a);
}
}

namespace ns_022077c8 {
extern "C" void PlayerActor_FaintEffect(Obj *o) {
    s32 *r4;
    (o)->advanceAnim();
    r4 = &o->actionWork;
    if (*r4 == -1) {
        *r4 = Effect_Create(0x34, &o->headTopPos, 0, 0);
    } else {
        Effect_SetPosition(*r4, &o->headTopPos, NULL, NULL);
    }
}
}

namespace ns_022077c8 {
extern "C" void PlayerActor_FaintWarp(Obj *o) {
    FaintWork *r4 = (FaintWork *)o->actionWorkRaw;
    V3 v;
    BlockMap *r6;
    if (((AnimFrameCtrl *)((AnimFrameCtrl *)&o->bodyModel))->hasPassedFrame(0x2e)) {
        if (r4->faintKind >= 2) {
            Bgm_ReleasePriority(0xc);
            data_021c1b3c->faintBgm.play();
        }
    } else if (((AnimFrameCtrl *)((AnimFrameCtrl *)&o->bodyModel))->hasPassedFrame(0x2f)) {
        r6 = TownBlockMap_Get();
        if (gCommManager->isLocalSlot(o->sessionSlot)) {
            if (r4->faintKind == 0) {
                if (Town_FindPlayerHouse(r6, &v, NULL, NULL)) {
                    SceneWarp_RequestAt(Scene_GetWarpRequest(), 0, (V3 *)&o->position, 0x1b800000, 0, 2, 2);
                }
            } else if (o->sessionSlot == 0) {
                if (Town_FindPlayerHouse(r6, &v, NULL, NULL)) {
                    v.z += 0x2000;
                    SceneWarp_RequestAt(Scene_GetWarpRequest(), 0, &v, 0x1b800000, 0, 2, 2);
                }
            } else {
                if (Town_FindGateHouse(r6, &v, NULL, NULL)) {
                    SceneWarp_RequestAt(Scene_GetWarpRequest(), 0, &v, 0x1b800000, 0, 2, 2);
                }
            }
        }
    }
}
}

namespace ns_022077c8 {
extern "C" void PlayerActor_MainFaint(Obj *o) {
    PlayerActor_FaintEffect(o);
    (o)->updateBodyCollider();
    PlayerActor_FaintWarp(o);
}
}

namespace ns_022077c8 {
extern "C" s32 PlayerActor_RequestAct6E(Obj *o, s32 a, s16 b) {
    Msg m;
    (&m)->assign(0x6e, a, b);
    s32 r = (o)->pushRequest(&m);
    return r;
}
}

namespace ns_022077c8 {
extern "C" void PlayerActor_SetupAct6E(Obj *o) {
    s16 v;
    *(u8 *)&o->actionWork = 0;
    v = -0x8000;
    (o)->setAngleY(&v);
    (o)->clearActionFlag(0);
    (o)->startAnimOnce(0x6a, 0, 0);
    (o)->playSe(0x818);
}
}

namespace ns_022077c8 {
extern "C" s32 PlayerActor_NetAct6E(Obj *o, s16 a) {
    return PlayerActor_RequestAct6E(o, 7, a);
}
}

namespace ns_022077c8 {
extern "C" void PlayerActor_Act6EUpdateAnim(Obj *o) {
    if (Unk_ov003_022078e0_IsTwo(gScreenTransition.state)) {
        (o)->advanceAnim();
        if (o->animId == 0x6a) {
            u32 m = ((Unk_ov003_022077c8_Bits *)&o->bodyModel.curFrame)->mid;
            if (m == 0x13 || m == 0x19 || m == 0x1e) {
                (o)->playFootstepSe();
            }
        } else {
            (o)->updateFootstepFx();
        }
    }
}
}

namespace ns_022077c8 {
extern "C" void PlayerActor_Act6ECheckEnd(Obj *o) {
    u8 *r4 = o->actionWorkRaw;
    u16 v[2];
    s32 r;
    if (((AnimFrameCtrl *)((AnimFrameCtrl *)&o->bodyModel))->isFinished()) {
        *r4 = 1;
        (o)->startAnim(1, 3, 0);
    }
    if (*r4 != 0) {
        if ((o)->turnToCamera(0x59a)) {
            data_021c1b3c->faintBgm.fadeOut();
            (o)->nudgeForward();
            TalkRequest_FinishSceneEntry();
            o->actionPriority = ((Unk_02007694 *)(void *)(o))->getActionDonePriority(o->action);
            PlayerActor_GetHeldItem(v, o);
            if (Item_IsFurniture(v)) {
                v[1] = 0xfff1;
                s32 a = Item_GetFurnitureIndex(v);
                if (a == Item_GetFurnitureIndex(&v[1])) {
                    r = 1;
                } else {
                    r = 0;
                }
            } else {
                if (v[0] == 0xfff1) {
                    r = 1;
                } else {
                    r = 0;
                }
            }
            if (r == 0) {
                PlayerActor_RequestStowItem(o, 2, 2, 0, 0, 0, 6, -1);
            } else {
                (o)->requestWait(3, 1, -1);
                (o)->setActionFlag(0);
            }
        }
    }
}
}

namespace ns_022077c8 {
extern "C" void PlayerActor_MainAct6E(Obj *o) {
    PlayerActor_Act6EUpdateAnim(o);
    (o)->updateBodyCollider();
    PlayerActor_Act6ECheckEnd(o);
}
}

namespace ns_02206e94 {
extern "C" s32 PlayerActor_RequestTrip(Obj *o, s32 a, s32 b) {
    Msg m;
    (&m)->assign(0x71, a, b);
    s32 r = (o)->pushRequest(&m);
    return r;
}
}

namespace ns_02206e94 {
extern "C" void PlayerActor_SetupTrip(Obj *o) {
    Rec *r = (Rec *)&o->actionWork;
    s32 st = (o)->getHeldToolKind();
    ((Rec *)&o->actionWork)->unk_00 = st;
    switch (st) {
    case 4:
        HeldItemModel_PlayAnim(&o->heldItemModel, 7, 2, 1);
        (o)->startAnimOnce(0x8d, 2, 0);
        break;
    case 3:
        HeldItemModel_PlayAnim(&o->heldItemModel, 0x1d, 2, 1);
        (o)->startAnimOnce(0x8d, 2, 0);
        break;
    case 10:
        (o)->startAnimOnce(0x8d, 2, 0);
        break;
    default:
        (o)->startAnimOnce(0x8b, 2, 0);
        break;
    }
    r->unk_04_b = 0;
    (o)->setActionFlag(9);
    (o)->playSe(0x7db);
}
}

namespace ns_02206e94 {
extern "C" s32 PlayerActor_NetTrip(Obj *o, s32 a) {
    return PlayerActor_RequestTrip(o, 6, a);
}
}

namespace ns_02206e94 {
extern "C" void PlayerActor_TripEffects(Obj *o) {
    u16 hh[2];
    V3 vb;
    V3 va;
    V3 out1;
    V3 out2;
    ((TwoLayerAnimModel *)((u8 *)&o->bodyModel))->updateLayers();
    hh[0] = o->rotY;
    if (((Rec *)&o->actionWork)->unk_04_b == 0) {
        if (((AnimFrameCtrl *)((AnimFrameCtrl *)&o->bodyModel))->hasPassedFrame(0xa) != 0) {
            PlayerActor_GetHeldItem(&hh[1], o);
            BOOL ok = FALSE;
            u32 ra = ((volatile u16 *)hh)[1];
            u32 rb = ((volatile u16 *)hh)[1];
            if (rb < 0x137c || ra > 0x137c) {
            } else {
                ok = TRUE;
            }
            if (ok) {
                s32 t = o->toolHandMtx.m[9];
                if (t == 0 && o->toolHandMtx.m[10] == 0 && o->toolHandMtx.m[11] == 0) {
                    V3 *pv = (V3 *)&o->position;
                    va.x = o->position.x;
                    va.y = pv->y;
                    va.z = pv->z;
                } else {
                    va.x = t;
                    va.y = o->toolHandMtx.m[10];
                    va.z = o->toolHandMtx.m[11];
                    WorldCurve_FromCurved(&va, &va);
                }
                Effect_Create(0x32, &va, 0, 0);
                PlayerActor_SetHoldableItem(0, o->sessionSlot);
            }
        }
        if (((AnimFrameCtrl *)((AnimFrameCtrl *)&o->bodyModel))->hasPassedFrame(7) != 0) {
            PlayerActor_OffsetByAngle(&out1, o, &o->position, (s16 *)&hh[0], &data_ov003_02230ad0);
            vb.x = out1.x;
            vb.y = out1.y;
            vb.z = out1.z;
            Effect_Create(0x39, &vb, (s16 *)&hh[0], 0);
        }
        if (((AnimFrameCtrl *)((AnimFrameCtrl *)&o->bodyModel))->hasPassedFrame(9) != 0) {
            PlayerActor_OffsetByAngle(&out2, o, &o->position, (s16 *)&hh[0], &data_ov003_02230ad8);
            vb.x = out2.x;
            vb.y = out2.y;
            vb.z = out2.z;
            Effect_Create(0x37, &vb, (s16 *)&hh[0], 0);
        }
    }
}
}

namespace ns_02206e94 {
extern "C" void PlayerActor_TripCheckEnd(Obj *o) {
    if (((Rec *)&o->actionWork)->unk_04_b == 1) {
        if (((AnimFrameCtrl *)((AnimFrameCtrl *)&o->bodyModel))->isFinished() != 0) {
            o->actionPriority = ((Unk_02007694 *)(void *)(o))->getActionDonePriority(o->action);
            (o)->requestWait(3, 1, -1);
            o->tripCooldown = 0x3c;
        }
    }
}
}

namespace ns_02206e94 {
extern "C" void PlayerActor_TripUpdate(Obj *o) {
    Rec *r = (Rec *)&o->actionWork;
    if (r->unk_04_b == 0) {
        s32 t = PlayerActor_DecreaseClamped(o->speed, 0, 0x7b);
        (o)->setSpeed((u32 *)(void *)(&t));
        if (o->speed == 0) {
            if (((AnimFrameCtrl *)((AnimFrameCtrl *)&o->bodyModel))->isFinished() != 0) {
                switch (r->unk_00) {
                case 4:
                    HeldItemModel_PlayAnim(&o->heldItemModel, 8, 3, 0);
                    (o)->startAnimOnce(0x8e, 3, 0);
                    break;
                case 3:
                    HeldItemModel_PlayAnim(&o->heldItemModel, 0x1e, 3, 0);
                    (o)->startAnimOnce(0x8e, 3, 0);
                    break;
                case 10:
                    (o)->startAnimOnce(0x8e, 3, 0);
                    break;
                default:
                    (o)->startAnimOnce(0x8c, 3, 0);
                    break;
                }
                r->unk_04_b = 1;
                if (gCommManager->isLocalSlot(o->sessionSlot) != 0) {
                    void *p = PlayerData_GetCurrent();
                    if (p != 0) {
                        if (((PlayerData *)(p))->testFlag(0x17) != 0) {
                            ((PlayerData *)(p))->setFlag(0x1a);
                        }
                    }
                }
            }
        }
    }
}
}

namespace ns_02206e94 {
extern "C" void PlayerActor_MainTrip(Obj *o) {
    PlayerActor_TripUpdate(o);
    PlayerActor_TripEffects(o);
    (o)->moveWithCollision();
    (o)->updateBodyCollider();
    PlayerActor_TripCheckEnd(o);
}
}

namespace ns_02206e94 {
extern "C" s32 PlayerActor_RequestAct72(Obj *o, s32 a, s32 b) {
    Msg m;
    (&m)->assign(0x72, a, b);
    s32 r = (o)->pushRequest(&m);
    return r;
}
}

namespace ns_02206e94 {
extern "C" void PlayerActor_SetupAct72(Obj *o) {
    (o)->startAnimOnce(0x9c, 3, 0);
    if (gCommManager->isLocalSlot(o->sessionSlot) != 0) {
        Sky_WishOnShootingStar();
    }
}
}

namespace ns_02206e94 {
extern "C" s32 PlayerActor_NetAct72(Obj *o, s32 a) {
    return PlayerActor_RequestAct72(o, 6, a);
}
}

namespace ns_02206e94 {
extern "C" void PlayerActor_Act72Decelerate(Obj *o) {
    if (o->speed != 0) {
        s32 t = PlayerActor_DecreaseClamped(o->speed, 0, 0x171);
        (o)->setSpeed((u32 *)(void *)(&t));
    }
}
}

namespace ns_02206e94 {
extern "C" void PlayerActor_Act72CheckEnd(Obj *o) {
    (o)->turnToCamera(0x59a);
    if (((AnimFrameCtrl *)((AnimFrameCtrl *)&o->bodyModel))->isFinished() != 0) {
        o->actionPriority = ((Unk_02007694 *)(void *)(o))->getActionDonePriority(o->action);
        (o)->requestWait(3, 1, -1);
    }
    if (gCommManager->isLocalSlot(o->sessionSlot) != 0) {
        if (((Unk_ov003_02206e94_Bits *)&o->bodyModel.curFrame)->mid >= 0x1a) {
            PlayerActor_ResumeWalkOrIdle(o);
        }
    }
}
}

namespace ns_02206e94 {
extern "C" void PlayerActor_MainAct72(Obj *o) {
    (o)->advanceAnim();
    (o)->moveWithCollision();
    (o)->updateBodyCollider();
    PlayerActor_Act72Decelerate(o);
    PlayerActor_Act72CheckEnd(o);
}
}

namespace ns_02206e94 {
extern "C" s32 PlayerActor_RequestPitfallFall(Obj *o, s32 *p, s32 b, s32 c) {
    Msg m;
    Unk_ov003_022072b8_P &q = *(Unk_ov003_022072b8_P *)m.unk_0c_b;
    (&m)->assign(0x73, b, c);
    q.a = p[0];
    q.b = p[1];
    s32 r = (o)->pushRequest(&m);
    return r;
}
}

namespace ns_02206e94 {
extern "C" void PlayerActor_PitfallFallSetNetData(u8 *p, u8 a, u8 b) {
    p[0] = a;
    p[1] = b;
}
}

namespace ns_02206e94 {
extern "C" void PlayerActor_PitfallFallGetNetData(u8 *s, u8 *a, u8 *b) {
    *a = s[0];
    *b = s[1];
}
}

namespace ns_02206e94 {
extern "C" void PlayerActor_SetupPitfallFall(Obj *o, u8 *a) {
    Unk_ov003_022072b8_P *q = (Unk_ov003_022072b8_P *)(((PlayerActionRequest *)(void *)a)->unk_0c_b);
    V3 v;
    Rec *r = (Rec *)&o->actionWork;
    s32 c0 = q->a;
    s32 c1 = q->b;
    FieldPos_FromUnitCenter(&v, c0, c1);
    ((Rec *)&o->actionWork)->unk_00 = v.x;
    r->unk_04 = v.z;
    r->unk_08 = -1;
    (o)->startAnim(0x90, 3, 0);
    if ((o)->getHeldToolKind() == 4) {
        HeldItemModel_PlayAnim(&o->heldItemModel, 0xf, 3, 0);
    }
    PlayerActor_PitfallFallSetNetData(o->netData.bytes, c0, c1);
    o->playSe(0x7ee);  // (void: nothing reads this function's result)
}
}

namespace ns_02206e94 {
extern "C" s32 PlayerActor_NetPitfallFall(Obj *o, s32 a) {
    u8 b[2];
    s32 p[2];
    PlayerActor_PitfallFallGetNetData(o->netData.bytes, &b[0], &b[1]);
    p[0] = b[0];
    p[1] = b[1];
    return PlayerActor_RequestPitfallFall(o, p, 6, a);
}
}

namespace ns_02206e94 {
extern "C" void PlayerActor_EndPitfallFall(Obj *o, s32 a) {
    Rec *r = (Rec *)&o->actionWork;
    VecFx32CtorDtor v;
    s32 z = r->unk_04;
    s32 y = o->position.y;
    s32 x = r->unk_00;
    v.x = x;
    v.y = y;
    v.z = z;
    V3 *pw = (V3 *)&o->position;
    pw->x = v.x;
    pw->y = v.y;
    pw->z = v.z;
    if ((o)->testActionFlag(0x12) != 0) {
        if (a == 0x74 || a == 0x75) {
            V3 t;
            V3 *pv = (V3 *)&o->position;
            t.x = o->position.x;
            t.y = pv->y;
            t.z = pv->z;
            FieldItemFx_StartPitfallHole(o->sessionSlot, &t);
        }
        (o)->clearActionFlag(0x12);
    }
    if (a != 0x74 && a != 0x75) {
        V3 t;
        V3 *pv = (V3 *)&o->position;
        t.x = o->position.x;
        t.y = pv->y;
        t.z = pv->z;
        FieldItemFx_StartPitfallClose(o->sessionSlot, &t);
    }
    if (a != 0x74) {
        s32 h = r->unk_08;
        if (h != -1) {
            Effect_End(h);
        }
    }
}
}

namespace ns_02206e94 {
extern "C" void PlayerActor_PitfallFallEffect(Obj *o) {
    (o)->advanceAnim();
    s32 *p = (s32 *)&o->actionWorkRaw[8]; // TripPitfallWork.unk_08
    s32 z = 0;
    if (*p == -1) {
        *p = Effect_Create(0x4c, &o->headTopPos, NULL, NULL);
    } else {
        Effect_SetPosition(*p, &o->headTopPos, NULL, NULL);
    }
    o->playSeAt(0x8a, (V3 *)&o->position);  // (void: nothing reads this function's result)
}
}

namespace ns_02206e94 {
extern "C" void PlayerActor_PitfallFallUpdate(Obj *o) {
    Rec *r = (Rec *)&o->actionWork;
    if (o->animId == 0x90) {
        volatile V3 v;
        s32 z = r->unk_04;
        s32 x = r->unk_00;
        v.x = x;
        v.y = 0;
        v.z = z;
        PlayerActor_StepTowardPose(o, x, z, o->rotY);
        V3 &pv = (*(V3 *)&o->position);
        s32 pz = pv.z;
        s32 px = pv.x;
        if (px == v.x && pz == v.z) {
            o->bodyModel.playMode = 1;
        } else {
            goto end;
        }
    }
    if (((AnimFrameCtrl *)((AnimFrameCtrl *)&o->bodyModel))->isFinished() != 0) {
        if (o->animId == 0x91) {
            PlayerActor_RequestPitfallStruggle(o, r->unk_08, 6, -1);
            (o)->setActionFlag(9);
        } else {
            struct { V3 pad; V3 b; } l;
            (o)->startAnimOnce(0x91, 3, 0);
            V3 *pv = (V3 *)&o->position;
            l.b.x = o->position.x;
            l.b.y = pv->y;
            l.b.z = pv->z;
            GroundInfoBase x;
            ((GroundInfo *)(void *)(&x))->initAtPos(&l.b, 0, 0);
            V3 c;
            if (x.attr == 0x13) {
                Effect_PlayById(0x8f, &l.b, 0, 0);
            } else {
                Effect_PlayById(0x8e, &l.b, 0, 0);
            }
            (o)->playSe(0x7e8);
            (o)->setActionFlag(9);
            c.x = l.b.x;
            c.y = l.b.y;
            c.z = l.b.z;
            FieldItemFx_StartPitfallHole(o->sessionSlot, &c);
            (o)->clearActionFlag(0x12);
            if ((o)->getHeldToolKind() == 4) {
                HeldItemModel_PlayAnim(&o->heldItemModel, 0x10, 3, 1);
            }
            GroundInfo_Destruct(&x);
        }
    }
end:;
}
}

namespace ns_02206e94 {
extern "C" void PlayerActor_MainPitfallFall(Obj *o) {
    PlayerActor_PitfallFallEffect(o);
    PlayerActor_PitfallFallUpdate(o);
}
}

namespace ns_02206e94 {
extern "C" s32 PlayerActor_RequestPitfallStruggle(Obj *o, s32 a, s32 b, s32 c) {
    Msg m;
    (&m)->assign(0x74, b, c);
    (*(VecFx32 *)m.unk_0c_b).x = a;
    s32 r = (o)->pushRequest(&m);
    return r;
}
}

namespace ns_02206e94 {
extern "C" void PlayerActor_SetupPitfallStruggle(Obj *o, s32 *a) {
    s32 t;
    (o)->startAnim(0x92, 3, 0);
    o->bodyModel.frameStep = 0x800;
    t = *(s32 *)((PlayerActionRequest *)(void *)a)->unk_0c_b;  // (the payload: 0xc on the DS)
    Rec *r = (Rec *)&o->actionWork;
    r->unk_04 = 0x800;
    r->unk_08 = 0x800;
    r->unk_0c = 0;
    r->unk_10 = 0;
    ((Rec *)&o->actionWork)->unk_00 = (o)->getInputMagnitude();
    r->inputAngle = (o)->getInputAngleRaw();
    r->effectHandle = t;
    (o)->clearActionFlag(9);
    if ((o)->getHeldToolKind() == 4) {
        HeldItemModel_PlayAnim(&o->heldItemModel, 0x11, 3, 0);
        HeldItemModel_SetAnimSpeed(&o->heldItemModel, 0x800);
    }
}
}

namespace ns_02206e94 {
extern "C" void PlayerActor_NetPitfallStruggle(Obj *o, s32 a) {
    s32 st = o->action;
    if (st != 0x73) {
        if (st == 0x74) {
            o->netSeq = a;
        } else {
            PlayerActor_RequestPitfallStruggle(o, -1, 6, a);
        }
    }
}
}

namespace ns_02206e94 {
extern "C" void PlayerActor_EndPitfallStruggle(Obj *o, s32 a) {
    if (((Rec *)&o->actionWork)->effectHandle != -1) {
        Effect_End(((Rec *)&o->actionWork)->effectHandle);
    }
    if (a != 0x75) {
        V3 v;
        V3 *pv = (V3 *)&o->position;
        v.x = o->position.x;
        v.y = pv->y;
        v.z = pv->z;
        FieldItemFx_StartPitfallClose(o->sessionSlot, &v);
    }
}
}

namespace ns_02206574 {
extern "C" void PlayerActor_PitfallStruggleUpdate(Obj *o) {
    PitfallStruggleWork *r6 = (PitfallStruggleWork *)&o->actionWork;
    s32 *r4 = &r6->f4;
    if (r6->fc >= 10) {
        *r4 = *r4 + 0x1333;
    } else {
        *r4 = *r4 - 0x4cd;
    }
    s32 v = *r4;
    if (v > 0x2400) {
        *r4 = 0x2400;
    } else if (v < r6->f8) {
        *r4 = r6->f8;
    }
    o->bodyModel.frameStep = *r4;
    if ((o)->getHeldToolKind() == 4) {
        HeldItemModel_SetAnimSpeed(&o->heldItemModel, *r4);
    }
    (o)->advanceAnim();
    s32 *r4b = &r6->f18;
    s32 z = 0;
    if (r6->f18 == -1) {
        *r4b = Effect_Create(0x4c, &o->headTopPos, NULL, NULL);
    } else {
        Effect_SetPosition(r6->f18, &o->headTopPos, NULL, NULL);
    }
    (o)->playSeAt(0x8a, (VecFx32 *)(&o->position));
}
}

namespace ns_02206574 {
extern "C" void PlayerActor_PitfallStruggleInput(Obj *o) {
    if (gCommManager->isLocalSlot(o->sessionSlot)) {
        PitfallStruggleWork *p = (PitfallStruggleWork *)&o->actionWork;
        s32 *q = &p->f10;
        s32 r4 = p->f0;
        s32 h = p->f14;
        s32 sp10;
        V3 a, b, c, d;
        b.y = 0;
        a.y = 0;
        s32 idx = (h >> 4) * 2;
        a.x = func_01ffcb0c(data_02135f44[idx], r4);
        a.z = func_01ffcb0c(data_02135f44[idx + 1], r4);
        s32 s4 = (o)->getInputMagnitude();
        s32 s8 = (o)->getInputAngleRaw();
        s32 i2 = ((u16)s8 >> 4) * 2;
        b.x = func_01ffcb0c(data_02135f44[i2], s4);
        b.z = func_01ffcb0c(data_02135f44[i2 + 1], s4);
        ((PitfallStruggleWork *)&o->actionWork)->f0 = s4;
        p->f14 = s8;
        r4 = 0;
        Vec_Sub(&d, &a, &b);
        c.x = d.x;
        c.y = d.y;
        c.z = d.z;
        if (VEC_Mag(&c) >= 0x59a) {
            r4 = FX_Div(func_01ffcb0c(VEC_Mag(&c), 0x6400), 0x59a);
        }
        s32 t = 0;
        u32 k = gPad.b;
        if (k & 1) t += 0x14;
        if (k & 2) t += 0x14;
        if (k & 0x400) t += 0x14;
        if (k & 0x800) t += 0x14;
        if (k & 0x200) t += 0x14;
        if (k & 0x100) t += 0x14;
        if (k & 8) t += 0x14;
        if (k & 4) t += 0x14;
        r4 += t << 12;
        if (r4 == 0) {
            r4 += 0x2000;
        }
        *q = *q + r4;
        p->fc = r4 >> 12;
        if (*q < 0) {
            *q = 0;
        }
        if (r4 != 0) {
            if (s4 != 0 || t != 0) {
                (o)->playSe(0x7ef);
            }
            p->f8 = FX_Div(func_01ffcb0c(func_01ffcb0c(0x1c00, *q), 0xa66), 0x190000) + 0x800;
        }
        if ((o)->testActionFlag(0xb) != 0 || p->f10 >= 0x190000) {
            PlayerActor_RequestPitfallClimbOut(o, 6, -1);
        }
    }
}
}

namespace ns_02206574 {
extern "C" void PlayerActor_MainPitfallStruggle(Obj *o) {
    PlayerActor_PitfallStruggleUpdate(o);
    (o)->updateBodyCollider();
    PlayerActor_PitfallStruggleInput(o);
}
}

namespace ns_02206574 {
extern "C" s32 PlayerActor_RequestPitfallClimbOut(Obj *o, s32 a, s32 b) {
    Msg m;
    (&m)->assign(0x75, a, b);
    s32 r = (o)->pushRequest(&m);
    return r;
}
}

namespace ns_02206574 {
extern "C" void PlayerActor_SetupPitfallClimbOut(Obj *o) {
    V3 pos;
    u32 buf[0x10];
    V3 pos2;
    s32 st = (o)->getHeldToolKind();
    if (st == 10 || (u32)(st - 3) <= 1) {
        (o)->startAnimOnce(0x94, 3, 0);
        if ((o)->getHeldToolKind() == 4) {
            HeldItemModel_PlayAnim(&o->heldItemModel, 0x12, 3, 1);
        }
    } else {
        (o)->startAnimOnce(0x93, 3, 0);
    }
    V3 *pv = (V3 *)&o->position;
    pos.x = o->position.x;
    pos.y = pv->y;
    pos.z = pv->z;
    ((GroundInfo *)(buf))->initAtPos(&pos, 0, 0);
    if (buf[0x34 / 4] == 0x13) {
        Effect_PlayById(0x91, &pos, 0, 0);
    } else {
        Effect_PlayById(0x90, &pos, 0, 0);
    }
    (o)->setActionFlag(9);
    pv = (V3 *)&o->position;
    pos2.x = o->position.x;
    pos2.y = pv->y;
    pos2.z = pv->z;
    FieldItemFx_StartPitfallClose(o->sessionSlot, &pos2);
    (o)->playSe(0x7e9);
    GroundInfo_Destruct(buf);
}
}

namespace ns_02206574 {
extern "C" s32 PlayerActor_NetPitfallClimbOut(Obj *o, s32 a) {
    return PlayerActor_RequestPitfallClimbOut(o, 6, a);
}
}

namespace ns_02206574 {
extern "C" void PlayerActor_PitfallClimbOutCheckEnd(Obj *o) {
    if (((AnimFrameCtrl *)((AnimFrameCtrl *)&o->bodyModel))->isFinished()) {
        o->actionPriority = ((Unk_02007694 *)(void *)(o))->getActionDonePriority(o->action);
        u16 k = 3;
        if ((o)->getHeldToolKind() == 4) {
            k *= (u32)k;
        }
        (o)->requestWait(k, 1, -1);
    }
}
}

namespace ns_02206574 {
extern "C" void PlayerActor_MainPitfallClimbOut(Obj *o) {
    (o)->advanceAnim();
    (o)->updateBodyCollider();
    PlayerActor_PitfallClimbOutCheckEnd(o);
}
}

namespace ns_02206574 {
extern "C" s32 PlayerActor_RequestBeeSting(Obj *o, s32 a, s32 b) {
    Msg m;
    (&m)->assign(0x78, a, b);
    if ((o)->pushRequest(&m)) {
        (o)->setActionFlag(7);
        return TRUE;
    }
    return FALSE;
}
}

namespace ns_02206574 {
extern "C" void PlayerActor_SetupBeeSting(Obj *o) {
    (o)->startAnimOnce(0x71, 3, 0);
    *(u8 *)&o->actionWork = 0;
    if ((o)->getHeldToolKind() == 3) {
        HeldItemModel_PlayAnim(&o->heldItemModel, 0x13, 3, 0);
    }
    (o)->clearActionFlag(0x1a);
    Bgm_Release(0x3f);
    Bgm_RequestSilence(0x19, 0xf, 0);
}
}

namespace ns_02206574 {
extern "C" void PlayerActor_NetBeeSting() {
}
}

extern "C" s32 data_ov003_02230af0 = 0xccd;
extern "C" s32 sFishEscapeSpeed = -0x419;
extern "C" s32 data_ov003_02230ae8 = 0x10cd;
extern "C" s32 sFishCastFarDist = 0x6000;
extern "C" s32 data_ov003_02230ae0 = 0x2000;
extern "C" s32 data_ov003_02230adc = 0x2d9a;
extern "C" s32 data_ov003_02230ad8 = 0x800;
extern "C" s32 data_ov003_02230ad4 = 0x2d9a;
extern "C" s32 data_ov003_02230ad0 = 0x1000;
extern "C" s32 sFishCastNearDist = 0x5000;
extern "C" s32 data_ov003_02230ac8 = 1;
extern "C" s32 data_ov003_02230ac4 = 1;
extern "C" s32 data_ov003_02230ac0 = 1;
extern "C" const s32 sFieldFrontDist = 0x2000;

namespace ns_02206574 {
extern "C" void PlayerActor_BeeStingUpdate(Obj *o) {
    StackPad8 pad;

    switch (o->animId) {
    case 0x71:
        if (((AnimFrameCtrl *)((AnimFrameCtrl *)&o->bodyModel))->isFinished()) {
            (o)->startAnimOnce(0x72, 0, 0);
        }
        break;
    case 0x72:
        (o)->turnAwayFromCamera(0x3ae);
        if (((AnimFrameCtrl *)((AnimFrameCtrl *)&o->bodyModel))->isFinished()) {
            (o)->startAnimOnce(0x73, 0, 0);
        }
        break;
    case 0x73:
        if (((AnimFrameCtrl *)((AnimFrameCtrl *)&o->bodyModel))->isFinished()) {
            (o)->playSe(0x817);
            (o)->startAnimOnce(0x74, 0, 0);
            PlayerData *r5 = PlayerData_GetCurrent();
            if (r5) {
                PlayerData_SetStungFace(r5, 1);
                u32 q = r5->getFaceType() + 0x10;
                o->faceTex.load(q);
                if (r5->testFlag(0x17)) {
                    r5->setFlag(0x19);
                }
            }
        }
        break;
    case 0x74:
        if (((AnimFrameCtrl *)((AnimFrameCtrl *)&o->bodyModel))->isFinished()) {
            (o)->startAnimOnce(0x75, 0, 0);
        }
        break;
    case 0x75: {
        u8 *p = (u8 *)&o->actionWork;
        s16 prev = o->rotY;
        (o)->turnToCamera(0x59a);
        if (prev != 0) {
            if (o->rotY == 0) {
                Bgm_ReleasePriority(0x19);
                Bgm_Request(0xd, 0x40, 0x7f, 1);
            }
        }
        switch (*p) {
        case 0:
            *p = 1;
            ((Character *)(o))->attachTalkRequest(o);
            (o)->setActionFlag(0x11);
            ((MsgRequest *)(&(TalkMsgRequest &)*o))->setFileName("obj_etc_player");
            o->msgIndex = 0x14;
            o->window->nextState = 1;
            Camera_SetMode4();
        case 1:
            if (o->window) {
                if (o->window->state) {
                    *p = 2;
                }
            }
            break;
        case 2:
            if (o->window) {
                if (o->window->state == 0) {
                    ((Character *)(o))->detachTalkRequest(o);
                    (o)->clearActionFlag(0x11);
                    TalkRequest_FinishPlayerMessage();
                    o->actionPriority = ((Unk_02007694 *)(void *)(o))->getActionDonePriority(o->action);
                    (o)->requestWait(3, 1, -1);
                    (o)->clearActionFlag(7);
                    Camera_SetModeDefault();
                    Bgm_Release(0x40);
                    Bgm_RequestSilence(0xc, 0xf, 5);
                }
            }
            break;
        }
        break;
    }
    }
}
}

namespace ns_02206574 {
extern "C" void PlayerActor_MainBeeSting(Obj *o) {
    (o)->advanceAnim();
    (o)->updateBodyCollider();
    PlayerActor_BeeStingUpdate(o);
}
}

namespace ns_02206574 {
extern "C" s32 PlayerActor_RequestAct80(Obj *o, s32 a, s32 b) {
    Msg m;
    (&m)->assign(0x80, a, b);
    s32 r = (o)->pushRequest(&m);
    return r;
}
}

namespace ns_02206574 {
extern "C" void PlayerActor_SetupAct80(Obj *o) {
    (o)->startAnimOnce(0x7b, 3, 0);
}
}

namespace ns_02206574 {
extern "C" s32 PlayerActor_NetAct80(Obj *o, s32 a) {
    return PlayerActor_RequestAct80(o, 6, a);
}
}

namespace ns_02206574 {
extern "C" void PlayerActor_EndAct80(Obj *o) {
    if (gCommManager->isLocalSlot(o->sessionSlot)) {
        u16 *r = PlayerData_GetBySessionSlot(o->sessionSlot)->getHeldItem();
        if (r) {
            (o)->netSendClothesChange(3, *r);
        }
    }
}
}

namespace ns_02206574 {
extern "C" void PlayerActor_Act80Update(Obj *o) {
    (o)->advanceAnim();
    if (((AnimFrameCtrl *)((AnimFrameCtrl *)&o->bodyModel))->hasPassedFrame(0xd)) {
        u16 t[2];
        PlayerData *r = PlayerData_GetBySessionSlot(o->sessionSlot);
        t[0] = 0xfff1;
        r->setHeldItem(&t[0]);
        t[1] = 0xfff1;
        HeldItemModel_SetItem(&o->heldItemModel, &t[1], 0);
        (o)->playSe(0x850);
        V3 v;
        s32 tz = o->toolHandMtx.m[11];
        s32 ty = o->toolHandMtx.m[10];
        s32 tx = o->toolHandMtx.m[9];
        v.x = tx;
        v.y = ty;
        v.z = tz;
        WorldCurve_FromCurved(&v, &v);
        Effect_Create(0x24, &v, &o->rotY, 0);
        (o)->setActionFlag(9);
    }
}
}

namespace ns_02206574 {
extern "C" void PlayerActor_Act80CheckEnd(Obj *o) {
    if (((AnimFrameCtrl *)((AnimFrameCtrl *)&o->bodyModel))->isFinished()) {
        u16 t = 0x137d;
        o->actionPriority = ((Unk_02007694 *)(void *)(o))->getActionDonePriority(o->action);
        if (gCommManager->isLocalSlot(o->sessionSlot)) {
            s32 r4 = Pocket_FindItem(&t);
            if (r4 != -1) {
                (o)->clearActionFlag(0);
                PlayerActor_SetHoldableItem(0x15, o->sessionSlot);
                Pocket_RemoveItem(r4);
                PlayerActor_RequestStowItem(o, 2, 2, 0, 0, 0, 6, -1);
                return;
            }
        }
        (o)->requestWait(3, 1, -1);
    }
}
}

namespace ns_02206574 {
extern "C" void PlayerActor_MainAct80(Obj *o) {
    PlayerActor_Act80Update(o);
    (o)->moveWithCollision();
    (o)->updateBodyCollider();
    PlayerActor_Act80CheckEnd(o);
}
}

namespace ns_02206574 {
extern "C" s32 PlayerActor_RequestFirework(Obj *o, s32 a, s32 b) {
    Msg m;
    (&m)->assign(0x81, a, b);
    s32 r = (o)->pushRequest(&m);
    return r;
}
}

namespace ns_02205c28 {
extern "C" void PlayerActor_SetupFirework(Obj *o) {
    s32 *r2 = &o->actionWork;
    *r2 = -1;
    ((u8 *)r2)[4] = 0;
    ((u8 *)r2)[5] = 0x14;
    (o)->startAnim(0x7c, 9, 0);
    HeldItemModel_PlayAnim(&o->heldItemModel, 0x28, 0, 1);
    if (gCommManager->isLocalSlot(o->sessionSlot)) {
        Camera_SetMode4();
        data_021c1b3c->mixer.startFireworkDuck();
    }
}
}

namespace ns_02205c28 {
extern "C" s32 PlayerActor_NetFirework(Obj *o, s32 a) {
    return PlayerActor_RequestFirework(o, 6, a);
}
}

namespace ns_02205c28 {
extern "C" void PlayerActor_EndFirework(Obj *o) {
    if (gCommManager->isLocalSlot(o->sessionSlot)) {
        u16 *r = PlayerData_GetBySessionSlot(o->sessionSlot)->getHeldItem();
        if (r) {
            (o)->netSendClothesChange(3, *r);
        }
        Camera_SetModeDefault();
        data_021c1b3c->mixer.endFireworkDuck();
    }
    if (o->actionWork != -1) {
        Effect_End(o->actionWork);
    }
    V3 v;
    PlayerActor_GetFrontPoint(&v, o);
    sWateringActive = 0;
    sWateringPos.x = v.x;
    sWateringPos.y = v.y;
    sWateringPos.z = v.z;
}
}

namespace ns_02205c28 {
extern "C" void PlayerActor_FireworkUpdate(Obj *o) {
    (o)->advanceAnim();
    s32 *r6 = &o->actionWork;
    u8 *r4 = (u8 *)r6 + 4;
    VecFx32Ctor v(o->heldItemJointMtx.m[9], o->heldItemJointMtx.m[10], o->heldItemJointMtx.m[11]);
    WorldCurve_FromCurved(&v, &v);
    s32 z = 0;
    s32 m1 = ~z;
    if (o->actionWork == m1) {
        u16 t[2];
        (*r4)++;
        s32 cnt = *r4;
        if (cnt > 9) {
            if (cnt == 10) {
                V3 w;
                PlayerActor_GetHeldItem(&t[0], o);
                if (t[0] == 0x137f) {
                    (o)->playSe(0x85a);
                } else {
                    (o)->playSe(0x859);
                }
                PlayerActor_GetFrontPoint(&w, o);
                sWateringActive = 1;
                sWateringPos.x = w.x;
                sWateringPos.y = w.y;
                sWateringPos.z = w.z;
            }
            PlayerActor_GetHeldItem(&t[1], o);
            if (t[1] == 0x137f) {
                *r6 = Effect_Create(0x35, (V3 *)&v, &o->rotY, NULL);
            } else {
                *r6 = Effect_Create(0x36, (V3 *)&v, &o->rotY, NULL);
            }
        }
    } else {
        Effect_SetPosition(o->actionWork, (V3 *)&v, &o->rotY, 0);
    }
}
}

namespace ns_02205c28 {
extern "C" void PlayerActor_FireworkCheckEnd(Obj *o) {
    if (((AnimFrameCtrl *)(BlendAnimModel *)HeldItemModel_GetModel((u8 *)&o->heldItemModel))->isFinished()) {
        u8 *q = (u8 *)&o->actionWork;
        u8 *r1 = q + 4;
        if (*r1 != 0) {
            *r1 = *r1 - 1;
        }
        if (*r1 == 0) {
            u8 *r2 = q + 5;
            if (*r2 != 0) {
                *r2 = *r2 - 1;
            }
            if (*r2 == 0) {
                u16 t[2];
                PlayerData *r = PlayerData_GetBySessionSlot(o->sessionSlot);
                t[0] = 0xfff1;
                r->setHeldItem(&t[0]);
                t[1] = 0xfff1;
                HeldItemModel_SetItem(&o->heldItemModel, &t[1], 0);
                o->actionPriority = ((Unk_02007694 *)(void *)(o))->getActionDonePriority(o->action);
                (o)->requestWait(9, 1, -1);
            }
        }
    }
}
}

namespace ns_02205c28 {
extern "C" void PlayerActor_MainFirework(Obj *o) {
    PlayerActor_FireworkUpdate(o);
    (o)->moveWithCollision();
    (o)->updateBodyCollider();
    PlayerActor_FireworkCheckEnd(o);
}
}

namespace ns_02205c28 {
extern "C" s32 PlayerActor_RequestAct82(Obj *o, s32 a, s32 b) {
    Msg m;
    (&m)->assign(0x82, a, b);
    s32 r = (o)->pushRequest(&m);
    return r;
}
}

namespace ns_02205c28 {
extern "C" void PlayerActor_SetupAct82(Obj *o) {
    o->actionWork = -1;
    (o)->startAnimOnce(0x7d, 3, 0);
    HeldItemModel_PlayAnim(&o->heldItemModel, 0x2a, 0, 0);
    (o)->playSe(0x85d);
}
}

namespace ns_02205c28 {
extern "C" s32 PlayerActor_NetAct82(Obj *o, s32 a) {
    return PlayerActor_RequestAct82(o, 6, a);
}
}

namespace ns_02205c28 {
extern "C" void PlayerActor_EndAct82(Obj *o) {
    if (gCommManager->isLocalSlot(o->sessionSlot)) {
        u16 *r = PlayerData_GetBySessionSlot(o->sessionSlot)->getHeldItem();
        if (r) {
            (o)->netSendClothesChange(3, *r);
        }
    }
    if (o->actionWork != -1) {
        Effect_End(o->actionWork);
    }
}
}

namespace ns_02205c28 {
extern "C" void PlayerActor_Act82Update(Obj *o) {
    (o)->advanceAnim();
    s32 f = ((Unk_ov003_02205c90_Bits *)&o->bodyModel.curFrame)->f;
    if (f >= 0xe && f <= 0x18) {
        s32 *r4 = &o->actionWork;
        VecFx32Ctor v(o->toolHandMtx.m[9], o->toolHandMtx.m[10], o->toolHandMtx.m[11]);
        WorldCurve_FromCurved(&v, &v);
        if (o->actionWork == -1) {
            *r4 = Effect_Create(0x31, (V3 *)&v, &o->rotY, 0);
        } else {
            Effect_SetPosition(o->actionWork, (V3 *)&v, &o->rotY, 0);
        }
    }
    if (((AnimFrameCtrl *)((AnimFrameCtrl *)&o->bodyModel))->hasPassedFrame(0x18)) {
        u16 t[2];
        PlayerData *r = PlayerData_GetBySessionSlot(o->sessionSlot);
        t[0] = 0xfff1;
        r->setHeldItem(&t[0]);
        t[1] = 0xfff1;
        HeldItemModel_SetItem(&o->heldItemModel, &t[1], 0);
    }
}
}

namespace ns_02205c28 {
extern "C" void PlayerActor_Act82CheckEnd(Obj *o) {
    if (((AnimFrameCtrl *)((AnimFrameCtrl *)&o->bodyModel))->isFinished()) {
        o->actionPriority = ((Unk_02007694 *)(void *)(o))->getActionDonePriority(o->action);
        (o)->requestWait(3, 1, -1);
    }
    if (gCommManager->isLocalSlot(o->sessionSlot)) {
        u32 f = ((Unk_ov003_02205c90_Bits *)&o->bodyModel.curFrame)->f;
        if (f >= 0x18) {
            PlayerActor_ResumeWalkOrIdle(o);
        }
    }
}
}

namespace ns_02205c28 {
extern "C" void PlayerActor_MainAct82(Obj *o) {
    PlayerActor_Act82Update(o);
    (o)->moveWithCollision();
    (o)->updateBodyCollider();
    PlayerActor_Act82CheckEnd(o);
}
}

namespace ns_02205c28 {
extern "C" s32 PlayerActor_RequestThrowBottle(Obj *o, s32 a, s32 b) {
    Msg m;
    (&m)->assign(0x89, a, b);
    s32 r = (o)->pushRequest(&m);
    return r;
}
}

namespace ns_02205c28 {
extern "C" void PlayerActor_SetupThrowBottle(Obj *o) {
    (o)->startAnimOnce(0x8f, 0, 0);
    (o)->playSe(0x4f);
}
}

namespace ns_02205c28 {
extern "C" s32 PlayerActor_NetThrowBottle(Obj *o, s32 a) {
    return PlayerActor_RequestThrowBottle(o, 6, a);
}
}

namespace ns_02205c28 {
extern "C" void PlayerActor_ThrowBottleShowItem(Obj *o) {
    (o)->advanceAnim();
    if (o->animId == 0x8f) {
        s32 f = ((Unk_ov003_02205c90_Bits *)&o->bodyModel.curFrame)->f;
        if (f >= 6 && f <= 0x19) {
            (o)->setActionFlag(0xd);
            o->getHeldItemPos((VecFx32 *)(u8 *)&o->shownItemPos.x);
            o->shownItem = 0x1520;
            s32 v = (f - 6) * 0x333;
            if (f >= 0xb) {
                v = 0x1000;
            }
            o->shownItemScaleX = v;
            o->shownItemScaleY = v;
            o->shownItemScaleZ = v;
            if (f == 0x19) {
                (o)->clearActionFlag(0xd);
                BottleThrow_Start(o->sessionSlot);
            }
        }
    }
}
}

namespace ns_02205c28 {
extern "C" void PlayerActor_ThrowBottleLook(Obj *o) {
    if (((AnimFrameCtrl *)((AnimFrameCtrl *)&o->bodyModel))->isFinished()) {
        (o)->startAnim(0, 3, 3);
    } else if (o->animId == 0) {
        if (BottleThrow_IsActive(o->sessionSlot) == 0) {
            PlayerActor_SetHeadTilt(0, 0, 4);
            o->actionPriority = ((Unk_02007694 *)(void *)(o))->getActionDonePriority(o->action);
            ((Unk_02007694 *)(void *)(o))->requestAct05(3, 5, -1);
        } else {
            V3 a, b;
            V3 *q = BottleThrow_GetPos(o->sessionSlot);
            a.x = q->x;
            a.y = q->y;
            a.z = q->z;
            V3 *pv = (V3 *)&o->position;
            b.x = pv->x;
            b.y = pv->y;
            b.z = pv->z;
            s32 yaw = Math_Atan2(a.x - b.x, a.z - b.z);
            s32 h = Math_Atan2(a.y - b.y, Vec_DistXZ(&a, &b));
            if (h > 0) {
                h = 0;
            }
            PlayerActor_SetHeadTilt(h, 0, 4);
            s16 tmp = o->rotY;
            PlayerActor_TurnAngle(&tmp, yaw);
            (o)->setAngleY(&tmp);
        }
    }
}
}

namespace ns_02205c28 {
extern "C" void PlayerActor_MainThrowBottle(Obj *o) {
    PlayerActor_ThrowBottleShowItem(o);
    (o)->updateBodyCollider();
    PlayerActor_ThrowBottleLook(o);
}
}

namespace ns_02205c28 {
extern "C" s32 PlayerActor_RequestFishRelease(Obj *o, u8 *p, u32 c, s32 id, s32 e) {
    Msg m;
    (&m)->assign(0x8f, id, *(s16 *)&e);
    u8 v0 = *(volatile u8 *)p;
    Unk_ov003_02205e58_Pair &q = (*(Unk_ov003_02205e58_Pair *)m.unk_0c_b);
    q.a = v0;
    q.b = c;
    s32 r = (o)->pushRequest(&m);
    return r;
}
}

namespace ns_02205c28 {
extern "C" void PlayerActor_FishReleaseSetNetData(u8 *p, u32 a, u32 b) {
    p[0] = a;
    p[1] = b;
}
}

namespace ns_02205c28 {
extern "C" void PlayerActor_FishReleaseGetNetData(u8 *src, u8 *a, u8 *b) {
    *a = src[0];
    *b = src[1];
}
}

namespace ns_02205c28 {
extern "C" void PlayerActor_SetupFishRelease(Obj *o, Msg *r) {
    u8 x, y;
    u8 *p;
    Unk_ov003_02205e58_Pair *q = (Unk_ov003_02205e58_Pair *)r->unk_0c_b;
    p = (u8 *)&o->actionWork;
    x = q->a;
    y = q->b;
    if (y == 0) {
        (o)->startAnim(0, 3, 3);
    }
    p[0] = x;
    p[1] = y;
    PlayerActor_FishReleaseSetNetData(o->netData.bytes, x, y);
}
}

namespace ns_02205c28 {
extern "C" s32 PlayerActor_NetFishRelease(Obj *o, s32 a) {
    u8 buf[2];
    PlayerActor_FishReleaseGetNetData(o->netData.bytes, &buf[0], &buf[1]);
    return PlayerActor_RequestFishRelease(o, &buf[0], buf[1], 6, a);
}
}

namespace ns_02205c28 {
extern "C" void PlayerActor_FishReleaseLook(Obj *o) {
    u8 *p = (u8 *)&o->actionWork;
    if (p[0] != 0) {
        s32 t = o->sessionSlot;
        if (FishCatch_IsActive(t)) {
            V3 a, b;
            V3 *q = FishCatch_GetPos(t);
            a.x = q->x;
            a.y = q->y;
            a.z = q->z;
            V3 *pv = (V3 *)&o->position;
            b.x = pv->x;
            b.y = pv->y;
            b.z = pv->z;
            b.y = b.y + 0x1b33;
            s32 yaw = Math_Atan2(a.x - b.x, a.z - b.z);
            s32 h = Math_Atan2(a.y - b.y, Vec_DistXZ(&a, &b));
            if (h >= 0x1800) {
                h = 0x1800;
            } else if (h <= -0x1000) {
                h = -0x1000;
            }
            s32 d = (s16)(yaw - o->rotY);
            if ((u16)(d + 0x2aaa) >= 0x5554) {
                PlayerActor_SetHeadTilt(0, 0, o->sessionSlot);
            } else {
                PlayerActor_ApproachAngle(&o->headPitch, h, 0x400, 0x1770000, 0xc0000);
                PlayerActor_ApproachAngle(&o->headYaw, d, 0x400, 0x1770000, 0xc0000);
            }
        } else {
            PlayerActor_SetHeadTilt(0, 0, o->sessionSlot);
            o->actionPriority = ((Unk_02007694 *)(void *)(o))->getActionDonePriority(o->action);
            if (p[1] != 0) {
                ((Unk_02007694 *)(void *)(o))->requestAct05(3, 5, -1);
            } else {
                (o)->requestWait(3, 1, -1);
            }
        }
    }
}
}

namespace ns_02205c28 {
extern "C" void PlayerActor_MainFishRelease(Obj *o) {
    (o)->advanceAnim();
    if ((o)->netFollowTransform()) {
        (o)->moveWithCollision();
    }
    (o)->updateBodyCollider();
    PlayerActor_FishReleaseLook(o);
}
}

namespace ns_02205c28 {
extern "C" s32 PlayerActor_ResumeWalkOrIdle(Obj *o) {
    if ((o)->getTargetWalkSpeed() > 0) {
        o->actionPriority = ((Unk_02007694 *)(void *)(o))->getActionDonePriority(o->action);
        return (o)->requestWalk(0, 1, -1);
    } else {
        return (o)->tryInteract();
    }
}
}

namespace ns_022052f4 {
extern "C" BOOL PlayerActor_FieldInteractFront(Obj *o, s32 *pa) {
    V3 v;
    if (o->inputMode == 1) {
        V3 t;
        PlayerActor_OffsetByAngle(&t, o, &o->position, &o->rotY, &sFieldFrontDist);
        v.x = t.x;
        v.y = t.y;
        v.z = t.z;
        if (o->isPosInReach(&v.x, 0xd) && o->startFieldQuery(&v, 2, *pa) && o->requestByFieldAnswer(&v, 0)) return TRUE;  // (the methods take the position as s32: PlayerActor.h)
    } else {
        v.x = o->targetPos.x;
        v.y = o->targetPos.y;
        v.z = o->targetPos.z;
        if (o->isPosInReach(&v.x, 0xd) && o->startFieldQuery(&v, 2, *pa) && o->requestByFieldAnswer(&v, 0)) return TRUE;  // (the methods take the position as s32: PlayerActor.h)
    }
    return FALSE;
}
}

namespace ns_022052f4 {
extern "C" s32 PlayerActor_AxeClassifyTarget(Obj *o, u8 *pa, u8 *pb, s32 *out) {
    s32 r = 1;
    s32 a, b;
    u32 t;
    V3 pos;
    if (*pa != 0 && PlayerActor_CheckToolHitActor(o)) return 4;
    PlayerActor_GetFrontPoint(&pos, o);
    FieldPos_ToUnit(&a, &b, &pos);
    if (out != NULL) {
        out[0] = a;
        out[1] = b;
    }
    s32 res = Ground_GetHeightAt(&pos, &t, 0x19);
    if (t == 4) goto aac;
    if (t != 6) goto b42;
    if (gSceneBlockMap != NULL) {
        u16 *p = BlockMap_GetItemPtrAtPos(gSceneBlockMap, &pos, 0);
        BOOL f3 = TRUE, f2 = TRUE, f1 = TRUE, f0 = FALSE;
        u32 v = *p;
        if (v >= 0x2b && v <= 0x2e) f0 = TRUE;
        if (!f0) { if (v < 0xff || v > 0x102) f1 = FALSE; }
        if (!f1) { if (v < 0x62 || v > 0x65) f2 = FALSE; }
        if (!f2) { if (v < 0xd0 || v > 0xd3) f3 = FALSE; }
        if (f3) return 1;
    }
aac:
    if (*pa != 0) {
        if (res < 0x1000) return 1;
        V2 q;
        q.x = 0;
        q.y = 0;
        q.x = a;
        q.y = b;
        switch ((o)->startUnitItemQuery((Unk_0200f6d4_V2 *)(void *)(&q), 1, *pb)) {
        case 0: return 5;
        case 1: return 0;
        case 2: return 4;
        }
    }
    if (o->fieldAnswer == 1) {
        switch (o->fieldAnswerKind) {
        case 6: r = 2; break;
        case 7: r = 3; break;
        case 12:
        case 13: r = 4; break;
        case 14: r = 6; break;
        }
    } else if (o->fieldAnswer == 2) {
        r = 5;
    } else {
        r = 0;
    }
    goto done;
b42:
    if (res >= 0x1000) r = 4;
done:
    return r;
}
}

namespace ns_022052f4 {
extern "C" s32 PlayerActor_ShovelClassifyTarget(Obj *o, V3 *p, u8 *f) {
    s32 a, b;
    FieldPos_ToUnit(&a, &b, p);
    if (*f != 0) {
        if (PlayerActor_CheckToolHitActor(o)) return 2;
        V2 q;
        q.x = 0;
        q.y = 0;
        q.x = a;
        q.y = b;
        switch ((o)->startUnitItemQuery((Unk_0200f6d4_V2 *)(void *)(&q), 2, 3)) {
        case 0: return 7;
        case 1: return 0;
        case 2: return 2;
        }
    }
    if (o->fieldAnswer == 2) return 7;
    if (o->fieldAnswer == 0) return 0;
    switch (o->fieldAnswerKind) {
    case 23: return 1;
    case 12:
    case 13: return 2;
    case 14: return 8;
    case 19:
    case 20: return 6;
    case 9:
    case 21: return 5;
    case 10:
    case 22: return 9;
    case 11: return 4;
    case 8:
    default: return 3;
    }
}
}

namespace ns_022052f4 {
extern "C" BOOL PlayerActor_FishFindCastTarget(Obj *o, V3 *out, V3 *tgt) {
    s16 ang;
    V3 t1, t0;
    if (Vec_Equal(tgt, (V3 *)&o->position)) {
        ang = o->rotY;
    } else {
        ang = Math_Atan2(tgt->x - o->position.x, tgt->z - o->position.z);
    }
    PlayerActor_OffsetByAngle(&t0, o, &o->position, &ang, &sFishCastFarDist);
    out->x = t0.x;
    out->y = t0.y;
    out->z = t0.z;
    BOOL r4 = Ground_IsWaterAt(out);
    PlayerActor_OffsetByAngle(&t1, o, &o->position, &ang, &sFishCastNearDist);
    if (r4 && Ground_IsWaterAt(&t1)) return TRUE;
    return FALSE;
}
}

namespace ns_022052f4 {
extern "C" void PlayerActor_FieldCheckUnitAhead(Obj *o) {
    V2 p;
    V3 out;
    p.x = 0;
    p.y = 0;
    PlayerActor_OffsetByAngle(&out, o, &o->position, &o->rotY, &data_ov003_02230ae8);
    FieldPos_ToUnit(&p.x, &p.y, &out);
    BOOL same = FALSE;
    s32 x = p.x;
    if (*(volatile s32 *)&p.x == o->aheadUnitX && p.y == o->aheadUnitZ) same = TRUE;
    if (same) {
        o->aheadUnitX = x;
        o->aheadUnitZ = p.y;
    } else {
        o->aheadUnitX = x;
        o->aheadUnitZ = p.y;
        s32 x0 = *(volatile s32 *)&p.x, y = p.y, hx = x0 >> 4, hy = y >> 4;
        u16 *c = BlockMap_GetItemPtr(gSceneBlockMap, hx, hy, x0 - (hx << 4), y - (hy << 4), 0);
        if (c != NULL) {
            if (Unk_ov003_02205744_Chk(c)) {
                FieldUnitPos q;
                q.x = p.x;
                q.z = p.y;
                Tree_RequestShake(o->sessionSlot, q, 1);
            }
        }
    }
}
}

namespace ns_022052f4 {
extern "C" void PlayerActor_FieldRunStep(Obj *o) {
    V2 p;
    p.x = 0;
    p.y = 0;
    if (o->speed > 0x53f) {
        FieldPos_ToUnit(&p.x, &p.y, (V3 *)&o->position);
        BOOL same = FALSE;
        s32 x = p.x;
        if (*(volatile s32 *)&p.x == o->runUnitX && p.y == o->runUnitZ) same = TRUE;
        if (same) {
            o->runUnitX = x;
            o->runUnitZ = p.y;
        } else {
            o->runUnitX = x;
            o->runUnitZ = p.y;
            V2 q;
            q.x = p.x;
            q.y = p.y;
            Flower_Trample(&q);
        }
    }
}
}

namespace ns_022052f4 {
extern "C" void PlayerActor_FieldCheckStepUnit(Obj *o) {
    if ((o)->testActionFlag(0xb)) return;
    if ((o)->testActionFlag(0x16)) return;
    if (!(o)->isLocomotionAction(o->action)) return;
    if (o->fieldQuery != -1) return;
    V2 p;
    p.x = 0;
    p.y = 0;
    FieldPos_ToUnit(&p.x, &p.y, (V3 *)&o->position);
    s32 t = (o)->getHeldToolKind();
    s32 k = 0;
    switch (t) {
    case 1: k = 2; break;
    case 2: k = 1; break;
    case 4: k = 3; break;
    case 5: k = 4; break;
    }
    V2 q;
    q.x = p.x;
    q.y = p.y;
    o->fieldQuery = FieldAction_RequestPitfall(o->sessionSlot, &q, k);
    if (o->fieldQuery != -1) {
        if ((o)->getFieldAnswerKind() == 0x19) {
            (o)->clearRequests();
            (o)->setActionFlag(0x16);
            (o)->setActionFlag(0x13);
            o->pitfallUnitX = p.x;
            o->pitfallUnitZ = p.y;
        }
    }
}
}

namespace ns_022052f4 {
extern "C" void PlayerActor_FieldPollPitfall(Obj *o) {
    if ((o)->testActionFlag(0x16)) {
        switch (o->fieldAnswer) {
        case 1: {
            s32 v[2];
            v[0] = o->pitfallUnitX;
            v[1] = o->pitfallUnitZ;
            PlayerActor_RequestPitfallFall(o, v, 6, -1);
            (o)->clearActionFlag(0x16);
            (o)->clearActionFlag(0x13);
            break;
        }
        case 2:
            (o)->clearActionFlag(0x16);
            (o)->clearActionFlag(0x13);
            break;
        }
    }
}
}

namespace ns_022052f4 {
extern "C" void PlayerActor_FieldUpdateTan(Obj *o) {
    struct L {
        u16 w0;
        u16 s2;
        u16 s4;
        u16 s6;
        u32 t[2];
    } l;
    BOOL night;
    u16 *cnt;
    u8 *fl;
    PlayerData *tm;
    u32 A, B, C;
    if (o->drawStep == 0) return;
    night = FALSE;
    if ((o)->isGuestInSession()) night = TRUE;
    cnt = PlayerSession_GetTanTimer();
    if (*cnt == 0) *cnt = 1;
    fl = PlayerSession_GetSessionFlags();
    tm = static_cast<PlayerData *>(PlayerActor_GetPlayerData(o));
    l.t[0] = 0;
    l.t[1] = 0;
    Clock_GetDateTime(l.t);
    if ((*fl & 1) != 0) {
        if (PlayerActor_CompareLastPlayDateNow(o) != 1) return;
        if (night) {
            *fl = *fl & 0xfe;
        } else {
            *fl = *fl & 0xee;
        }
    }
    u8 *tb = (u8 *)&l;
    A = tb[0xd];
    B = tb[0xc];
    C = tb[0xb];
    if (B != 8) {
        if (B == 7 && C >= 0x10) {
        } else if (B != 9 || C > 0xf) {
            return;
        }
    }
    u8 hh = ((u8 *)&l)[0xa];
    if (hh < 0xa || hh >= 0x11) return;
    if (Weather_GetCurrent() >= 3) return;
    PlayerActor_GetHeldItem(&l.s2, o);
    if (Unk_ov003_022052f4_RngV(&l.s2, 0x1380, 0x139f)) return;
    PlayerActor_GetHeldItem(&l.s4, o);
    if (Unk_ov003_022052f4_RngV(&l.s4, 0x13a0, 0x13a7)) return;
    *cnt = *cnt - 1;
    if (*cnt != 0) return;
    l.w0 = (l.w0 & ~0x7f) | (A &= 0x7f);
    l.w0 = (l.w0 & ~0x780) | ((B &= 0xf) << 7);
    l.w0 = (l.w0 & ~0xf800) | ((C &= 0x1f) << 11);
    u32 n = (u8)(PlayerActor_GetTan(o) + 1);
    if (n > 7) n = 7;
    tm->setTan(n);
    PlayerActor_SetLastPlayDate(o, tm, &l);
    *cnt = 0x4650;
    *fl = *fl & 0xfb;
    *fl = *fl | 3;
    (o)->netSendTan();
}
}

namespace ns_022052f4 {
extern "C" BOOL PlayerActor_CheckToolHitActor(Obj *o) {
    u16 buf[2];
    s32 k;
    u32 id;
    o->toolHitKind = 0;
    if (o->subCollider.isHit == 0) return FALSE;
    o->toolHitKind = 1;
    k = o->subCollider.hitTargetKind;
    id = o->subCollider.hitTargetIndex;
    PlayerActor_GetHeldItem(buf, o);
    switch (k) {
    case 2:
    case 3: {
        NpcActor *it = NpcRegistry_FindByKind(k, id);
        if (it != NULL) {
            BOOL f = Unk_ov003_022052f4_Rng(buf, 0x1376, 0x1376);
            if (f || Unk_ov003_022052f4_Rng(buf, 0x1377, 0x1377)) {
                if (it->onToolHit(buf) == 1) {
                    o->toolHitKind = 4;
                    break;
                }
            } else {
                o->toolHitActor = it;
            }
            o->toolHitKind = 2;
        }
        break;
    }
    case 0x11:
    case 0x12: {
        void *it = Snowball_FindByParam(id);
        if (it != NULL) {
            BOOL f = Unk_ov003_022052f4_Rng(buf, 0x1376, 0x1376);
            if (!f && !Unk_ov003_022052f4_Rng(buf, 0x1377, 0x1377)) o->toolHitActor = (Actor *)it;
            o->toolHitKind = 3;
        }
        break;
    }
    }
    return TRUE;
}
}

namespace ns_02204d90 {
extern "C" BOOL PlayerActor_FieldUseTool(PlayerActor *self) {
    s32 mode = (self)->getHeldToolKind();
    s16 ang;
    s32 xy[2];
    struct {
        StackPad12 pad;
        VecFx32 t, s, z, va, vb, vc;
    } l;
    if (self->actionPressed) {
        l.z.x = 0;
        l.z.y = 0;
        l.z.z = 0;
        if (PlayerActor_FieldStartToolAction(self, mode, &l.z)) {
            return TRUE;
        }
    }
    if (!(self)->isPosInReach(&self->targetPos.x, mode)) {
        if (self->toolTargetKind == 1 || self->hasTargetPos) {
            if ((self)->interactAt(mode)) {
                return TRUE;
            }
        }
        goto fail;
    }
    switch (self->toolTargetKind) {
    case 1:
        if (self->toolTargetKind != 0) {
            if (PlayerActor_FieldInteractFront(self, &mode)) {
                return TRUE;
            }
        }
        if (mode == 2 || mode == 1 || mode == 10 || mode == 0) {
            if (self->toolTargetKind != 0) {
                if (mode == 10) {
                    l.va.x = self->targetPos.x;
                    l.va.y = self->targetPos.y;
                    l.va.z = self->targetPos.z;
                    if (PlayerActor_FieldStartToolAction(self, 0, &l.va)) {
                        return TRUE;
                    }
                } else {
                    l.vb.x = self->targetPos.x;
                    l.vb.y = self->targetPos.y;
                    l.vb.z = self->targetPos.z;
                    if (PlayerActor_FieldStartToolAction(self, mode, &l.vb)) {
                        return TRUE;
                    }
                }
            }
        }
        break;
    case 2:
        if (mode == 4) {
            l.z.x = 0;
            l.z.y = 0;
            l.z.z = 0;
            if (PlayerActor_FieldStartToolAction(self, mode, &l.z)) {
                return TRUE;
            }
        }
        break;
    }
    if (self->inputMode == 2 && mode == 5) {
        FieldPos_ToUnit(&xy[0], &xy[1], &self->targetPos);
        s32 hx, hy, x, y;
        x = *(volatile s32 *)&xy[0];
        y = *(volatile s32 *)&xy[1];
        hx = x >> 4;
        hy = y >> 4;
        u16 *cell = BlockMap_GetItemPtr(gSceneBlockMap, hx, hy, x - (hx << 4), y - (hy << 4), 0);
        if (cell) {
            BOOL f8 = TRUE, f7 = TRUE, f6 = TRUE, f5 = TRUE, f4 = TRUE, f3 = TRUE, f2 = TRUE, f1 = FALSE;
            u32 v = *cell;
            if (v <= 5) {
                f1 = TRUE;
            }
            if (!f1) {
                if (v < 6 || v > 0xb) {
                    f2 = FALSE;
                }
            }
            if (!f2) {
                if (v < 0xc || v > 0x11) {
                    f3 = FALSE;
                }
            }
            if (!f3) {
                if (!((v >= 0x12 && v <= 0x19) || v == 0x1c)) {
                    f4 = FALSE;
                }
            }
            if (!f4) {
                if (!((v >= 0x8a && v <= 0x8f) || (v >= 0x90 && v <= 0x95) || (v >= 0x96 && v <= 0x9b) || (v >= 0x9c && v <= 0xa3) || v == 0xa5)) {
                    f5 = FALSE;
                }
            }
            if (!f5) {
                if (v != 0x1a) {
                    f6 = FALSE;
                }
            }
            if (!f6) {
                if (v != 0xa4) {
                    f7 = FALSE;
                }
            }
            if (!f7) {
                if (v != 0x1d) {
                    f8 = FALSE;
                }
            }
            if (!f8) {
                if (!((v >= 0x6e && v <= 0x73) || (v >= 0x74 && v <= 0x79) || (v >= 0x7a && v <= 0x7f) || (v >= 0x80 && v <= 0x87) || v == 0x88 || v == 0x89)) {
                    goto after_tile;
                }
            }
            l.z.x = 0;
            l.z.y = 0;
            l.z.z = 0;
            if (PlayerActor_FieldStartToolAction(self, mode, &l.z)) {
                return TRUE;
            }
        }
    }
after_tile:
    if (self->hasTargetPos) {
        if ((self)->interactAt(mode)) {
            return TRUE;
        }
    }
    if (self->inputMode == 2) {
        switch (mode) {
        case 1: {
            ang = Math_Atan2(self->targetPos.x - self->bodyPos.x, self->targetPos.z - self->bodyPos.z);
            PlayerActor_OffsetByAngle(&l.t, self, &self->position, &ang, &sFieldFrontDist);
            if ((self)->isPosInReach(&l.t.x, mode)) {
                s32 *pa = &self->targetPos.x;
                s32 *pb = &self->targetPos.y;
                s32 *pc = &self->targetPos.z;
                l.s.x = *pa;
                l.s.y = *pb;
                l.s.z = *pc;
                *pa = l.t.x;
                *pb = l.t.y;
                *pc = l.t.z;
                l.z.x = 0;
                l.z.y = 0;
                l.z.z = 0;
                if (PlayerActor_FieldStartToolAction(self, mode, &l.z)) {
                    self->targetPos.x = l.s.x;
                    self->targetPos.y = l.s.y;
                    self->targetPos.z = l.s.z;
                    return TRUE;
                }
                self->targetPos.x = l.s.x;
                self->targetPos.y = l.s.y;
                self->targetPos.z = l.s.z;
            }
            break;
        }
        case 5:
            l.z.x = 0;
            l.z.y = 0;
            l.z.z = 0;
            if (PlayerActor_FieldStartToolAction(self, mode, &l.z)) {
                return TRUE;
            }
            break;
        case 3:
            if (Ground_IsWaterAt(&self->targetPos)) {
                l.vc.x = self->targetPos.x;
                l.vc.y = self->targetPos.y;
                l.vc.z = self->targetPos.z;
                if (PlayerActor_FieldStartToolAction(self, mode, &l.vc)) {
                    return TRUE;
                }
            }
            break;
        }
    }
fail:
    if (mode == 0) {
        if (self->actionPressed) {
            if (Sky_IsShootingStarVisible()) {
                PlayerActor_RequestAct72(self, 6, -1);
                return TRUE;
            }
        }
    }
    return FALSE;
}
}

namespace ns_02204d90 {
extern "C" BOOL PlayerActor_FieldStartToolAction(PlayerActor *self, s32 mode, VecFx32 *pos) {
    VecFx32 a, z, c, z2, t1, t2;
    switch (mode) {
    case 2:
        PlayerActor_RequestAxeSwing(self, 6, -1);
        return TRUE;
    case 1:
        PlayerActor_RequestShovelReady(self, 6, -1);
        return TRUE;
    case 4:
        PlayerActor_RequestBugNetSwing(self, 6, -1);
        return TRUE;
    case 3:
        z.x = 0;
        z.y = 0;
        z.z = 0;
        if (Vec_Equal(pos, &z)) {
            VecFx32 *pv = &self->position;
            pos->x = pv->x;
            pos->y = pv->y;
            pos->z = pv->z;
        }
        if (PlayerActor_FishFindCastTarget(self, &a, pos)) {
            c.x = a.x;
            c.y = a.y;
            c.z = a.z;
            PlayerActor_RequestFishCast(self, &c, 6, -1);
        } else {
            PlayerActor_RequestFishCastFail(self, 6, -1);
        }
        return TRUE;
    case 5:
        PlayerActor_RequestWateringCan(self, 6, -1);
        return TRUE;
    case 6:
        PlayerActor_RequestSlingshot(self, 6, -1);
        return TRUE;
    case 7:
        PlayerActor_RequestAct82(self, 6, -1);
        return TRUE;
    case 8:
        PlayerActor_RequestAct80(self, 6, -1);
        return TRUE;
    case 9:
        PlayerActor_RequestFirework(self, 6, -1);
        return TRUE;
    case 0:
    case 10:
        z2.x = 0;
        z2.y = 0;
        z2.z = 0;
        if (Vec_Equal(pos, &z2)) {
            PlayerActor_GetFrontUnitCenter(&t1, self);
            pos->x = t1.x;
            pos->y = t1.y;
            pos->z = t1.z;
        }
        PlayerActor_GetFrontUnitCenter(&t2, self);
        a.x = t2.x;
        a.y = t2.y;
        a.z = t2.z;
        if (Vec_DistXZ(&self->position, pos) < 0x2334) {
            if ((self)->startFieldQuery(&a, 1, 0)) {
                BOOL f;
                if ((self)->getFieldAnswerKind() == 1) {
                    f = FALSE;
                } else {
                    f = TRUE;
                }
                s32 zero = 0;
                PlayerActor_RequestTreeShake(self, a.x, a.z, f, zero, 6, ~zero);
                return TRUE;
            }
        }
        if (mode == 10) {
            PlayerActor_RequestUmbrellaSpin(self, 6, -1);
            return TRUE;
        }
    }
    return FALSE;
}
}

namespace ns_02204d90 {
extern "C" BOOL PlayerActor_FieldInteractAt(PlayerActor *self, s32 a) {
    VecFx32 v;
    if (PlayerActor_FieldInteractFront(self, &a)) {
        return TRUE;
    }
    if (self->inputMode == 1) {
        VecFx32 t;
        PlayerActor_OffsetByAngle(&t, self, &self->position, &self->rotY, &data_ov003_02230af0);
        v.x = t.x;
        v.y = t.y;
        v.z = t.z;
    } else {
        v.x = self->targetPos.x;
        v.y = self->targetPos.y;
        v.z = self->targetPos.z;
        if (!(self)->isPosInReach(&v.x, 0xc)) {
            return FALSE;
        }
    }
    if ((self)->startFieldQuery(&v, 0, a)) {
        if ((self)->requestByFieldAnswer(&v, 0)) {
            return TRUE;
        }
    }
    return FALSE;
}
}

// ---- overlay variables (generated: tools/host_overlay_vars.py) ----
#include "host_overlay_vars.h"

HOST_OVERLAY_VARS(unk_ov003_02204ce8) {
    HOST_OVERLAY_VAR(data_ov003_02230af0),
    HOST_OVERLAY_VAR(sFishEscapeSpeed),
    HOST_OVERLAY_VAR(data_ov003_02230ae8),
    HOST_OVERLAY_VAR(sFishCastFarDist),
    HOST_OVERLAY_VAR(data_ov003_02230ae0),
    HOST_OVERLAY_VAR(data_ov003_02230adc),
    HOST_OVERLAY_VAR(data_ov003_02230ad8),
    HOST_OVERLAY_VAR(data_ov003_02230ad4),
    HOST_OVERLAY_VAR(data_ov003_02230ad0),
    HOST_OVERLAY_VAR(sFishCastNearDist),
    HOST_OVERLAY_VAR(data_ov003_02230ac8),
    HOST_OVERLAY_VAR(data_ov003_02230ac4),
    HOST_OVERLAY_VAR(data_ov003_02230ac0),
    HOST_OVERLAY_VARS_END
};

#ifdef HOST64
#include "field/FishShadow.h"
// (modern/host/frontend.c's ACWW_FISH_TEST: the fish at the player's bobber, its AI mode and bite state; -1: none)
extern "C" int PlayerActor_HostBobberFishState(void *obj) {
    PlayerActor *o = (PlayerActor *)obj;
    FishShadow *f = o ? (FishShadow *)o->heldItemModel.bobber.getFish() : NULL;
    if (f == NULL) return -1;
    return f->aiMode << 8 | f->biteState;
}
extern "C" int PlayerActor_HostStartFishing(void *obj) {
    VecFx32 z = {0, 0, 0};
    return PlayerActor_FieldStartToolAction((PlayerActor *)obj, 3, &z);
}
#endif
