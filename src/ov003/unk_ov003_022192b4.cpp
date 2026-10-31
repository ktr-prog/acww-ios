#include "ds_div.h"
#include "host_overlay_vars.h"
#include "snd/SndSeEmitter.h"
#include "sys/ProcProfile.h"
#include "uptr.h"
#include "gfx/AnimModel.h"
#include "town/BlockMap.h"
#include "town/TownBlockMap.h"
#include "field/FieldUnitPos.h"
// the field objects' model heap (0x21400 bytes on the DS; the model objects are larger on 64-bit hosts)
#ifdef HOST64
#define FIELDOBJ_MODEL_HEAP_SIZE 0x30000
#else
#define FIELDOBJ_MODEL_HEAP_SIZE 0x21400
#endif
// the tree / cedar / palm loaders' file buffer slots: u32 arrays (the loaders store the buffers as u32); on the DS
// the free functions see them as void * (the same 4 bytes), on 64-bit hosts they keep the u32 view
typedef u32 FileBufSlot;
// mwcc-version: 1.2/base
// ov003 TU22 (field objects, .text 0x02219294-0x0221ffb8): 11 unit files merged; every old file is a namespace
#include "types.h"
#include "sys/StackPad.h"
#include "gfx/VecFx32.h"
#include "gfx/Mtx43.h"
#include "Unk_020d8c7c.h"
#include "net/CommManager.h"
#include "gfx/ModelSet.h"
#include "game/UnitShapeQueryX.h"
#include "gfx/CachedModel.h"
#include "field/FieldObjectShapeQuery.h"
#include "gfx/SPLResource.h"
#include "gfx/EffectSplEmitter.h"
#include "gfx/NNSG3dRS.h"
#include "field/FieldAction.h"
#include "actor/Actor.h"
#include "gfx/SPLResource.h"
#include "game/GroundInfoBase.h"
#include "game/GroundInfo.h"
#include "gfx/AnimFrameCtrl.h"
#include "gfx/Model.h"
#include "gfx/TwoLayerAnimModel.h"
#include "player/PlayerData.h"
#include "sys/Heap.h"
#include "proto/autoload_2/unk_020e8558.h"
#include "proto/itcm/unk_01ffcb2c.h"
#include "proto/main/unk_020039ec.h"
#include "proto/main/unk_0203ecec.h"
#include "proto/main/unk_02063904.h"
#include "proto/main/unk_02063f3c.h"
#include "proto/main/unk_0208f268.h"
#include "proto/main/unk_02094810.h"
#include "proto/main/unk_02097444.h"
#include "proto/ov003/unk_ov003_022192b4.h"
#include "proto/ov003/unk_ov003_0221ffb8.h"
#include "proto/itcm/unk_01ffcfc0.h"
#include "proto/main/unk_020abea8.h"
#include "proto/main/unk_0204cc1c.h"
#include "gfx/Mtx43.h"

// defined elsewhere (their definitions' types; the generated headers of those files don't fit here, or leave them out:
// include/proto/.excluded)
extern "C" {
extern Mtx43 data_021f47e0;  // work matrix of main's model code (MtxFx43 there: the same layout)
void Mtx43_SetTranslate(Mtx43 *m, s32 x, s32 y, s32 z);
void Mtx43_RotateX(Mtx43 *m, s32 angle);
s32 Item_GetInfoUnk07(u16 *p);
BOOL Item_IsTreeGrown(u16 *p);
void PendingUnit_ApplyAt(Vec2 *pos, s32 flag);
void PendingUnit_ApplyAtIfAid(Vec2 *pos, u32 v, s32 flag);
s32 PendingUnit_Find(Vec2 *pos, s32 flag);
s32 PendingUnit_FindBySlot(u32 a, u32 b);
u32 TownState_GetSeasonPeriod();
u8 *func_021065dc(u8 *p);
u8 *func_021065f8(u8 *p, u32 i);
}

// ================================================================ other modules' real names
#define func_02133150 _s32_div_f
#define func_02135714 __cxa_vec_ctor
#define func_021355f0 __cxa_vec_cleanup


// ================================================================ shared (global) classes


extern "C" {
extern CommManager *gCommManager;
PendingUnit *PendingUnit_Get(u32 i);
// (src/ov003/unk_ov003_02225800.cpp; its header brings game/FxVec3.h, whose out-of-line constructor this unit
// does not use: see FxVec3 below)
void Insect_SpawnBeeSwarm(VecFx32Ctor *v);
}

// One falling-leaf / seasonal particle effect started by a shaken tree (TreeLeafFx_SetRecord); treeType 3 = free.
struct TreeLeafFxEntry {
    /* 0x00 */ s32 treeType;
    /* 0x04 */ s32 isLeaves;
    /* 0x08 */ s32 motionType;
    /* 0x0c */ u8 treeStage;
    /* 0x0d */ s8 lifeTimer;
    /* 0x0e */ s8 frame;
    /* 0x0f */ u8 animKind;
    /* 0x10 */ s32 tintVariant;
    /* 0x14 */ s32 posX;
    /* 0x18 */ s32 posY;
    /* 0x1c */ s32 posZ;
};

// sTreeLeafFx: the two leaf effect records (TreeLeafFx_*).
struct TreeLeafFx {
    /* 0x00 */ TreeLeafFxEntry records[2];
    /* 0x40 */ s32 spawnIndex;
};

// Item category -> its field object shape records (3 bytes each: x, z, size; FieldObj_GetShapeRecord), ended by a
// NULL entry (data_ov003_0222f4b8).
struct FieldObjShapeCategory {
    const u8 *shapes;
    u8 category;
    u8 unk_05;
    u16 shapeCount;
};

// Per tree type / motion / stage / leaves table entry (TreeLeafFx_GetParams); the last three values go to the SPL
// emitter (TreeLeafFx_ApplyParams: +0x68, +0x58, +0x50).
struct TreeLeafFxParams {
    /* 0x00 */ s32 lifeTime;
    /* 0x04 */ u16 genInterval;
    /* 0x06 */ u16 particleLife;
    /* 0x08 */ s16 initVelMagAxis;
};

// Effect callbacks of the two emitters of a leaf effect (data_ov003_02232b48/b78, EffectSpl_CreateTracked).
struct TreeLeafFxCbs {
    /* 0x00 */ EffectEmitterCbs emitters[2];
};


// fx32 animation frame as bitfields (A01 copy set of AnimFrameCtrl::curFrame views)
struct Unk_ov003_0221b8bc_Bits {
    u32 a : 12;
    u32 b : 16;
    u32 c : 4;
};

// 0x14c-byte animated tree (shake / chop animation of one tree model, TreeAnim_*). The AnimModel and the
// SndSeEmitter are built by TreeAnim_Construct and torn down by TreeAnim_Destruct, as the vector constructor of the
// TreeAnimSet calls them (raw storage here; the AnimModel members the TreeAnim code reads directly are named in the
// union, at their places in the AnimModel: Model::resMdl / resTex / mtx, the AnimFrameCtrl base, its curFrame and
// playMode).
struct TreeAnim {
    /* 0x000 */ s32 active;
    /* 0x004 */ s32 sessionSlot;
#define TA_AFC ((unsigned long)__builtin_offsetof(AnimModel, numFrames) - (unsigned long)__builtin_offsetof(AnimFrameCtrl, numFrames))
    union {
        alignas(AnimModel) u8 animModel[sizeof(AnimModel)];
        struct {
            u8 hm0[__builtin_offsetof(AnimModel, resMdl)];
            void *resMdl;
            u32 resTex;
            u8 hm1[__builtin_offsetof(AnimModel, mtx) - __builtin_offsetof(AnimModel, resTex) - 4];
            Mtx43 baseMatrix;
        };
        struct {
            u8 hm2[TA_AFC];
            u8 frameCtrl[sizeof(AnimFrameCtrl)];
        };
        struct {
            u8 hm3[TA_AFC + __builtin_offsetof(AnimFrameCtrl, curFrame)];
            Unk_ov003_0221b8bc_Bits animFrame;
        };
        struct {
            u8 hm4[TA_AFC + __builtin_offsetof(AnimFrameCtrl, playMode)];
            u8 playMode;
        };
    };
#undef TA_AFC
    /* 0x0c0 */ s32 posX;
    /* 0x0c4 */ s32 posY;
    /* 0x0c8 */ s32 posZ;
    /* 0x0cc */ VecXZ unit;
    /* 0x0d4 */ s32 animKind;
    /* 0x0d8 */ s32 alpha;
    /* 0x0dc */ s32 leafSpawnCount;
    /* 0x0e0 */ alignas(SndSeEmitter) u8 seEmitter[sizeof(SndSeEmitter)];
    /* 0x120 */ s32 unk_120[6];
    /* 0x138 */ s32 unk_138[3];
    /* 0x144 */ TreeLeafFxEntry *leafRecord;
    /* 0x148 */ TreeLeafFxEntry *seasonalRecord;
};

// Queued tree animation (TreeAnimSet_Request -> TreeAnimSet_ProcessRequest / TreeAnimRequest_Resolve).
struct TreeAnimRequest {
    /* 0x00 */ s32 active;
    /* 0x04 */ s32 sessionSlot;
    /* 0x08 */ VecXZ unit;
    /* 0x10 */ s32 animKind;
    /* 0x14 */ s32 isChop;
};

// the sub-object at +0x4b20: constructed by the explicit call in the scene constructor (TreeAnimSet_Construct),
// destroyed by the implicit member destructor, which is the function at 0x0221c4e4
class TreeAnimSet {
public:
    ~TreeAnimSet();
    /* 0x0000 */ TreeAnim treeAnims[3][4];
    /* 0x0f90 */ TreeAnim cedarAnims[4];
    /* 0x14c0 */ TreeAnim litCedarAnims[3];
    /* 0x18a4 */ TreeAnim palmAnims[4];
    /* 0x1dd4 */ TreeAnimRequest requests[5];
    /* 0x1e4c */ void *animFiles[8][4];   // TreeAnimSet_LoadAnims: loaded .nsbca files
    /* 0x1ecc */ void *anims[8][4];       // ... and their first animation
};


extern "C" TreeAnimSet *TreeAnimSet_Construct(TreeAnimSet *self);

class FieldObjectManager : public GameProc {
public:
    inline FieldObjectManager()
    {
        TreeAnimSet_Construct(&treeAnimSet);
    }
    virtual BOOL onDelete();
    virtual BOOL onCreate();
    virtual BOOL onExecute();
    virtual BOOL onDraw();

    /* 0x050 */ CachedModel *iconModels[0x49];
    /* 0x174 */ CachedModel treeModels[3][6];
    /* 0xc6c */ CachedModel cedarModels[6];
    /* 0x1014 */ CachedModel litCedarModels[3];
    /* 0x11e8 */ CachedModel palmModels[6];
    /* 0x1590 */ CachedModel flowerModels[4][10];
    /* 0x2df0 */ CachedModel specialFlowerModels[2][2];
    /* 0x3060 */ CachedModel *dandelionModels[2];
    /* 0x3068 */ CachedModel designModels[0x20];
    /* 0x43e8 */ CachedModel grassModels[5];
    /* 0x46f4 */ CachedModel *stoneModels[5];
    /* 0x4708 */ CachedModel holeModels[2];
    /* 0x4840 */ CachedModel crackModels[2];
    /* 0x4978 */ CachedModel *stumpModels[12];
    /* 0x49a8 */ CachedModel turnipModels[2];
    /* 0x4ae0 */ ModelSet iconModelSet;
    /* 0x4af0 */ ModelSet dandelionModelSet;
    /* 0x4b00 */ ModelSet stoneModelSet;
    /* 0x4b10 */ ModelSet stumpModelSet;
    /* 0x4b20 */ TreeAnimSet treeAnimSet;
    /* 0x6a6c */ s32 frameCounter;
};

// 0xa4-byte field item effect (sFieldItemFxTable: an item tossed, dropped from a tree, popping out of the ground, a
// hole opening or closing ... ; FieldItemFx_* / FieldItemFxTable_*), its sound emitter built by its constructor.
struct FieldItemFx {
    /* 0x00 */ s32 sessionSlot;
    /* 0x04 */ s32 active;
    /* 0x08 */ u16 item;
    /* 0x0a */ u16 pendingItem;
    /* 0x0c */ s32 kind;
    /* 0x10 */ VecXZ unit;
    /* 0x18 */ VecFx32 pos;
    /* 0x24 */ VecFx32 offset;
    /* 0x30 */ s32 velocityX;
    /* 0x34 */ s32 velocityY;
    /* 0x38 */ s32 velocityZ;
    /* 0x3c */ VecFx32 scale;
    /* 0x48 */ s16 startArg;
    /* 0x4a */ s16 unk_4a;
    /* 0x4c */ s16 step;
    /* 0x4e */ u16 frame;
    /* 0x50 */ s16 timer;
    /* 0x52 */ s16 alpha;
    /* 0x54 */ VecFx32 origin;
    /* 0x60 */ SndSeEmitter seEmitter;
    /* 0xa0 */ u8 pendingApply;
    /* 0xa1 */ u8 pendingCommit;
    FieldItemFx();
    ~FieldItemFx();
};

struct FieldItemFxTable {
    FieldItemFx entries[0x14];
    ~FieldItemFxTable();
};

// zero-initialised 3-word object whose (empty) destructor lives in main
struct FxVec3 : VecFx32Copy {
    FxVec3() {}
};


// ================================================================ per-file declarations
namespace ns_0221927c {
typedef FieldUnitPos P2;
typedef VecFx32Copy V3;
extern "C" {


s32 func_02133150(s32 a, s32 b);

}
// ---- prototypes of this file's functions
}

namespace ns_02219b84 {
typedef FieldUnitPos P2;
typedef VecFx32Copy V3;
typedef Mtx43 Blk;
typedef FieldItemFx Ent;
typedef FieldItemFxTable Tbl;
static inline Tbl *FxTable() { return &sFieldItemFxTable; }
extern "C" {


void Mtx43_Scale(Blk *m, s32 x, s32 y, s32 z);


static inline BOOL Unk_ov003_02219e7c_Chk2(u32 v) {
    BOOL f4 = TRUE, f3 = TRUE, f2 = TRUE, f1 = FALSE;
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
        if ((v < 0x12 || v > 0x19) && v != 0x1c) {
            f4 = FALSE;
        }
    }
    return f4;
}

static inline BOOL Unk_ov003_02219e7c_Chk1(volatile u16 *q, volatile u16 *p, u16 &a) {
    BOOL f = FALSE;
    a = *q;
    u32 v = *p;
    if (v >= 0x21 && a <= 0x24) {
        f = TRUE;
    }
    return f;
}

static inline BOOL Unk_ov003_02219e7c_Chk4(u32 a) {
    BOOL f = FALSE;
    u32 d = (u16)(a + 0xffe6);
    if (d <= 4) {
        if ((1 << d) & 0x1b) {
            f = TRUE;
        }
    }
    return f;
}
}
// ---- prototypes of this file's functions
}

namespace ns_0221a4a0 {
extern "C" {

extern s16 data_02135f44[];

void GroundInfo_Destruct(GroundInfoBase *b);
void VEC_Add(void *a, void *b, void *out);
void Sky_PlayBalloonDropSe(s32 a);
void Sky_EndBalloonDrop(void);
void PlayerActor_LocalRequestAct77From(void *p);
s32 func_02133150(s32 a, s32 b);
void Town_SetBeesReleased(void);
}
struct Unk_ov003_0221a664_Obj {
    s32 pad_00;
    VecFx32 pos;
};
static inline s32 Unk_ov003_0221aabc_Idx(s32 a) { return ((u16)a >> 4) * 2; }
static inline BOOL Unk_ov003_0221ad84_Chk(volatile u16 *p, u32 &vr)
{
    BOOL f8 = TRUE, f7 = TRUE, f6 = TRUE, f5 = TRUE, f4 = TRUE, f3 = TRUE, f2 = TRUE;
    s32 f1 = 0;
    u32 v = *p;
    u32 w = *p;
    vr = v;
    if (w <= 5) {
        f1 = TRUE;
    }
    if (!f1) {
        if (v < 6 || v > 0xb) f2 = FALSE;
    }
    if (!f2) {
        if (v < 0xc || v > 0x11) f3 = FALSE;
    }
    if (!f3) {
        if ((v < 0x12 || v > 0x19) && v != 0x1c) f4 = FALSE;
    }
    if (!f4) {
        if ((v < 0x8a || v > 0x8f) && (v < 0x90 || v > 0x95) && (v < 0x96 || v > 0x9b) &&
            (v < 0x9c || v > 0xa3) && v != 0xa5) f5 = FALSE;
    }
    if (!f5) {
        if (v != 0x1a) f6 = FALSE;
    }
    if (!f6) {
        if (v != 0xa4) f7 = FALSE;
    }
    if (!f7) {
        if (v != 0x1d) f8 = FALSE;
    }
    return f8;
}
// ---- prototypes of this file's functions
}

namespace ns_0221aed4 {
typedef FieldUnitPos P2;
typedef VecFx32Copy V3;
extern "C" {


void VEC_Add(void *a, void *b, void *c);
s32 Item_IsFlower(void *c);
s32 Flower_SpawnPetalFx(void *c, void *p, s32 a, s32 b);
s32 Weed_SpawnPullFx(void *c, void *p);
s32 DeadTurnip_SpawnDigFx(void *p);
s32 func_02133150(s32 a, s32 b);
s32 PendingUnit_CommitAt(void *p, s32 z);
s32 Town_CanReleaseBees();


}
// ---- prototypes of this file's functions
}

namespace ns_0221b8bc {
struct Unk_ov003_0221b8bc_V3D : VecFx32 {
    Unk_ov003_0221b8bc_V3D() {}
    ~Unk_ov003_0221b8bc_V3D() {}
};
struct Unk_ov003_0221b8bc_Col {
    u16 a, b, c;
};
struct Unk_ov003_0221b8bc_Col2 {
    u16 a, b;
};
extern "C" {


s32 Item_GetFruitTreeFruit(u16 *p);
s32 Field_AidOrLocal(s32 aid);
u16 SceneLights_GetRoomColor(void);
void NNS_G3dMdlSetMdlEmi(void *p, s32 a, s32 b);
void NNS_G3dMdlSetMdlAlphaAll(void *p, s32 a);
void NNS_G3dMdlSetMdlPolygonIDAll(void *p, s32 a);
s32 Item_GetStumpSize(u16 *p);
s32 Item_GetTreeStage(u16 *p);
s32 Town_CanReleaseBees(void);

}
// ---- prototypes of this file's functions
}

namespace ns_0221c220 {
typedef TreeAnimSet Big;
typedef TreeLeafFxEntry Rec;
typedef VecXZ Pos;
typedef TreeLeafFx Set;
typedef EffectSplEmitter Tgt;
typedef TreeLeafFxParams PRec;
typedef TreeAnim Elem2;
extern "C" {

void *func_02135714(void *p, u32 n, u32 size, void *ctor, void *dtor);
void *func_021355f0(void *p, u32 n, u32 size, void *dtor);
s32 func_02133150(s32 a, s32 b);
s32 Scene_InTown();

extern s16 data_02135f44[];
extern BlockMap *gSceneBlockMap;

}
// ---- prototypes of this file's functions
}

namespace ns_0221cb54 {
struct Unk_ov003_0221cb54_Col;
struct Unk_ov003_0221d118_V3 {
    s32 x, y, z;
    Unk_ov003_0221d118_V3(const VecFx32Copy &o) { x = o.x; y = o.y; z = o.z; }
    ~Unk_ov003_0221d118_V3() {}
};
typedef Unk_ov003_0221d118_V3 V3d;
struct Unk_ov003_0221d118_K : VecFx32 {
    Unk_ov003_0221d118_K() {}
    Unk_ov003_0221d118_K(const VecFx32Copy &o) { x = o.x; y = o.y; z = o.z; }
    ~Unk_ov003_0221d118_K() {}
};
typedef Unk_ov003_0221d118_K V3k;
struct Unk_ov003_0221cb54_Col {
    volatile u16 a, b, c;
};
typedef Mtx43 Blk;
typedef FieldObjectManager O;
typedef FieldUnitPos P2;
typedef VecFx32Copy V3;
typedef VecXZ R2;
typedef Unk_ov003_0221cb54_Col Col;
extern "C" {

extern BlockMap *gSceneBlockMap;
extern u8 *gCamera;

s32 PendingUnit_FindForAid(s32 aid, P2 p, s32 a);
s32 Town_IsSaplingBlocker(u16 *c);
s32 Item_IsTreeStage0(u16 *c);
s32 Camera_IsBlockingFocusView(V3 *v, s32 a, s32 b);
s32 Field_AidOrZero(s32 aid);
void PendingUnit_Apply(PendingUnit *e);
s32 Town_GetRafflesiaPos(V3 *o, s32 z);
void Vec_Sub(VecFx32 *out, VecFx32 *a, VecFx32 *b);
s32 Flower_GetSpecies(u16 *c);
void ModelSet_Release(void *p);
void PendingUnits_Flush();
s32 func_02133150(s32 a, s32 b);
extern V3 gCameraLookAt;
}
static inline BOOL Chk_0221cd80(u16 *p) {
    BOOL f9 = TRUE, f8 = TRUE, f7 = TRUE, f6 = TRUE, f5 = TRUE, f4 = TRUE, f3 = TRUE, f2 = TRUE, f1 = FALSE;
    u32 v = *p;
    if (v >= 0x26 && v <= 0x2a) f1 = TRUE;
    if (!f1) {
        if (v < 0x5d || v > 0x61) f2 = FALSE;
    }
    if (!f2) {
        if (v < 0x2f || v > 0x56) f3 = FALSE;
    }
    if (!f3) {
        if (v < 0x57 || v > 0x5b) f4 = FALSE;
    }
    if (!f4) {
        if (v < 0x66 || v > 0x68) f5 = FALSE;
    }
    if (!f5) {
        if (v != 0x69) f6 = FALSE;
    }
    if (!f6) {
        if (v < 0x6a || v > 0x6c) f7 = FALSE;
    }
    if (!f7) {
        if (v != 0x6d) f8 = FALSE;
    }
    if (!f8) {
        if (v < 0xc8 || v > 0xcf) f9 = FALSE;
    }
    return f9;
}
static inline BOOL Chk_0221d118(u16 *p) {
    BOOL f8 = TRUE, f7 = TRUE, f6 = TRUE, f5 = TRUE, f4 = TRUE, f3 = TRUE, f2 = TRUE, f1 = FALSE;
    u32 v = *p;
    if (v <= 5) f1 = TRUE;
    if (!f1) {
        if (v < 6 || v > 0xb) f2 = FALSE;
    }
    if (!f2) {
        if (v < 0xc || v > 0x11) f3 = FALSE;
    }
    if (!f3) {
        if ((v < 0x12 || v > 0x19) && v != 0x1c) f4 = FALSE;
    }
    if (!f4) {
        if ((v < 0x8a || v > 0x8f) && (v < 0x90 || v > 0x95) && (v < 0x96 || v > 0x9b) && (v < 0x9c || v > 0xa3) && v != 0xa5) f5 = FALSE;
    }
    if (!f5) {
        if (v != 0x1a) f6 = FALSE;
    }
    if (!f6) {
        if (v != 0xa4) f7 = FALSE;
    }
    if (!f7) {
        if (v != 0x1d) f8 = FALSE;
    }
    return f8;
}
static inline BOOL Chk_0221d37c(u16 *p) {
    BOOL f9 = TRUE, f8 = TRUE, f7 = TRUE, f6 = TRUE, f5 = TRUE, f4 = TRUE, f3 = TRUE, f2 = TRUE, f1 = FALSE;
    u32 v = *p;
    if (v >= 0x26 && v <= 0x2a) f1 = TRUE;
    if (!f1) {
        if (v < 0x5d || v > 0x61) f2 = FALSE;
    }
    if (!f2) {
        if (v < 0x2f || v > 0x56) f3 = FALSE;
    }
    if (!f3) {
        if (v < 0x57 || v > 0x5b) f4 = FALSE;
    }
    if (!f4) {
        if (v < 0x66 || v > 0x68) f5 = FALSE;
    }
    if (!f5) {
        if (v != 0x69) f6 = FALSE;
    }
    if (!f6) {
        if (v < 0x6a || v > 0x6c) f7 = FALSE;
    }
    if (!f7) {
        if (v != 0x6d) f8 = FALSE;
    }
    if (!f8) {
        if (v < 0xc8 || v > 0xcf) f9 = FALSE;
    }
    return f9;
}
// ---- prototypes of this file's functions
}

namespace ns_0221db54 {
typedef VecFx32Copy V3;
typedef Mtx43 Blk;
typedef ::FieldObjectManager O;
typedef CachedModel M;
extern "C" {


u16 SceneLights_GetRoomColor();
void NNS_G3dMdlSetMdlEmiAll(NNSG3dResMdl *mdl, u32 col);   // (src/autoload_2/unk_02105d50.c: u8 *, void *)
s32 Flower_GetSpecies(u16 *p);
s32 Flower_GetColor(u16 *p);
s32 Item_GetTreeStage(u16 *p);
s32 Item_GetFruitTreeFruit(u16 *p);
s32 LitCedarList_Find(s32 a, s32 b);
s32 func_02133150(s32 a, s32 b);

}
// ---- prototypes of this file's functions
}

namespace ns_0221e4d4 {
typedef VecFx32 V3;
typedef Mtx43 Blk;
typedef CachedModel Elem;
typedef FieldObjectManager Obj;
typedef BOOL (*Fn)();
extern "C" {

extern void *data_021f482c_v;

void Mtx43_RotateXYZ(Blk *m, s32 x, s32 y, s32 z);
void Mtx43_Scale(Blk *m, s32 x, s32 y, s32 z);
void NNS_G3dMdlSetMdlAlphaAll(void *p, s32 a);
void Field_OnEnter();


static inline BOOL Unk_ov003_0221e4d4_Chk1(volatile u16 *p) {
    BOOL r = TRUE;
    BOOL f = FALSE;
    u32 a = *p;
    u32 v = *p;
    if (v >= 0xd4 && a <= 0xda) {
        f = TRUE;
    }
    if (!f) {
        if (a < 0xdb || a > 0xe1) {
            r = FALSE;
        }
    }
    return r;
}

static inline s32 Unk_ov003_0221e4d4_Kind(volatile u16 *p) {
    u32 v = *p;
    return (v & 0xf000) >> 12;
}

static inline BOOL Unk_ov003_0221e4d4_Chk4(u32 a) {
    BOOL f = FALSE;
    u16 x = a + 0xffe6;
    if (x <= 4) {
        if ((1 << x) & 0x1b) {
            f = TRUE;
        }
    }
    return f;
}

static inline BOOL Unk_ov003_0221e4d4_Chk9(u32 a) {
    BOOL f9 = TRUE, f8 = TRUE, f7 = TRUE, f6 = TRUE, f5 = TRUE, f4 = TRUE, f3 = TRUE, f2 = FALSE;
    if (a <= 5) {
        f2 = TRUE;
    }
    if (!f2) {
        if (a < 6 || a > 0xb) {
            f3 = FALSE;
        }
    }
    if (!f3) {
        if (a < 0xc || a > 0x11) {
            f4 = FALSE;
        }
    }
    if (!f4) {
        if ((a < 0x12 || a > 0x19) && a != 0x1c) {
            f5 = FALSE;
        }
    }
    if (!f5) {
        if ((a < 0x8a || a > 0x8f) && (a < 0x90 || a > 0x95) && (a < 0x96 || a > 0x9b) && (a < 0x9c || a > 0xa3) && a != 0xa5) {
            f6 = FALSE;
        }
    }
    if (!f6) {
        if (a != 0x1a) {
            f7 = FALSE;
        }
    }
    if (!f7) {
        if (a != 0xa4) {
            f8 = FALSE;
        }
    }
    if (!f8) {
        if (a != 0x1d) {
            f9 = FALSE;
        }
    }
    return f9;
}
}
// ---- prototypes of this file's functions
}

namespace ns_0221ede8 {
typedef CachedModel Elem;
typedef FieldObjectManager Obj;
extern "C" {


s32 ModelSet_Load(void *t, void *file, void *heap);
void *ModelSet_Find(void *t, void *name);


}
// ---- prototypes of this file's functions
}

namespace ns_0221f798 {
extern "C" {

extern BlockMap *gSceneBlockMap;

BOOL ModelSet_Load(void *t, void *file, void *heap);
void *ModelSet_Find(void *t, void *name);
void Gfx3d_LoadPltt(void *a, u32 b);
void Gfx3d_LoadTex(void *a, u32 b);
const char *Item_GetIconModelName(s32 a, s32 b);
void *NNS_G3dGetTex(void *file);
s32 func_02106654();
s32 func_02106670(s32 a, s32 b);

}
// ---- prototypes of this file's functions
}

// ================================================================ data declarations
extern "C" char data_ov003_02232eb4[0x1c];
extern "C" const u32 data_ov003_0222f178[3];
extern "C" char data_ov003_022333c0[0x20];
extern "C" const u32 data_ov003_0222f160[3];
extern "C" char data_ov003_02233790[0x24];
extern "C" char data_ov003_02234328[0x28];
extern "C" const u32 data_ov003_0222f0d0[3];
extern "C" char data_ov003_02232c7c[0x18];
extern "C" char data_ov003_02233400[0x20];
extern "C" char data_ov003_022345d4[0x30];
extern "C" char data_ov003_02233288[0x1c];
extern "C" char data_ov003_02232ed0[0x1c];
extern "C" char data_ov003_02232874[0xc];
extern "C" char data_ov003_02232f08[0x1c];
extern "C" char data_ov003_02232c64[0x18];
extern "C" const u32 data_ov003_0222f148[3];
extern "C" char data_ov003_022337fc[0x24];
extern "C" char data_ov003_02232cac[0x18];
extern "C" char data_ov003_022329b8[0x10];
extern "C" const u32 data_ov003_0222f1c0[3];
extern "C" char data_ov003_02233868[0x24];
extern "C" char data_ov003_02233420[0x20];
extern "C" const TreeLeafFxParams *data_ov003_022326a8[2];
extern "C" const u32 data_ov003_0222f25c[3];
extern "C" char data_ov003_022329c8[0x10];
extern "C" const u32 data_ov003_0222f3b8[3];
extern "C" char *data_ov003_02232dcc[6];
extern "C" char data_ov003_02232cc4[0x18];
extern "C" const TreeLeafFxParams *data_ov003_022327b0[2];
extern "C" char data_ov003_022338b0[0x24];
extern "C" char data_ov003_02232f24[0x1c];
extern "C" char data_ov003_022338d4[0x24];
extern "C" char data_ov003_0223391c[0x24];
extern "C" char data_ov003_02232988[0x10];
extern "C" char data_ov003_02233820[0x24];
extern "C" const u32 data_ov003_0222f3c4[3];
extern "C" const u32 data_ov003_0222f358[3];
extern "C" char data_ov003_02232f5c[0x1c];
extern "C" char data_ov003_02233988[0x24];
extern "C" char data_ov003_022339ac[0x24];
extern "C" const u32 data_ov003_0222f418[3];
extern "C" const TreeLeafFxParams *data_ov003_022327f0[2];
extern "C" const TreeLeafFxParams *data_ov003_022326c0[2];
extern "C" char data_ov003_022343a0[0x28];
extern "C" const u32 data_ov003_0222f46c[3];
extern "C" const u32 data_ov003_0222f1d8[3];
extern "C" const TreeLeafFxParams *data_ov003_02232800[2];
extern "C" char data_ov003_02233a18[0x24];
extern "C" const u32 data_ov003_0222f2b0[3];
extern "C" const TreeLeafFxParams *data_ov003_022327f8[2];
extern "C" char data_ov003_02233480[0x20];
extern "C" char data_ov003_02233a84[0x24];
extern "C" const u32 data_ov003_0222f250[3];
extern "C" char data_ov003_02232cdc[0x18];
extern "C" const u32 data_ov003_0222f2c8[3];
extern "C" char data_ov003_02233aa8[0x24];
extern "C" char *data_ov003_0223263c[1];
extern "C" const u32 data_ov003_0222f220[3];
extern "C" char data_ov003_02232f78[0x1c];
extern "C" char data_ov003_02233af0[0x24];
extern "C" const u32 data_ov003_0222f2ec[3];
extern "C" char data_ov003_02233b14[0x24];
extern "C" char *data_ov003_02232a28[4];
extern "C" char *data_ov003_02232b18[4];
extern "C" char data_ov003_0223288c[0xc];
extern "C" const TreeLeafFxParams *data_ov003_02232660[2];
extern "C" const TreeLeafFxParams *data_ov003_02232718[2];
extern "C" const u32 data_ov003_0222f22c[3];
extern "C" char data_ov003_022334a0[0x20];
extern "C" char data_ov003_02233b80[0x24];
extern "C" char data_ov003_02233bec[0x24];
extern "C" char data_ov003_02232f94[0x1c];
extern "C" char data_ov003_02233c10[0x24];
extern "C" const TreeLeafFxParams *data_ov003_022326b0[2];
extern "C" const TreeLeafFxParams *data_ov003_02232768[2];
extern "C" const TreeLeafFxParams *data_ov003_022327d8[2];
extern "C" char *data_ov003_02232630[1];
extern "C" const TreeLeafFxParams *data_ov003_02232770[2];
extern "C" const TreeLeafFxParams **data_ov003_02232a58[4];
extern "C" const u32 data_ov003_0222f6fc[23];
extern "C" const u32 data_ov003_0222f10c[3];
extern "C" const u32 data_ov003_0222f118[3];
extern "C" char *sTreeAnimPaths[32];
extern "C" char data_ov003_02233c7c[0x24];
extern "C" const TreeLeafFxParams **data_ov003_02232a78[4];
extern "C" char data_ov003_022329f8[0x10];
extern "C" char data_ov003_02233004[0x1c];
extern "C" char *data_ov003_02232bec[5];
extern "C" char **data_ov003_02232910[3];
extern "C" char data_ov003_02233020[0x1c];
extern "C" char data_ov003_0223303c[0x1c];
extern "C" const TreeLeafFxParams **data_ov003_02232a88[4];
extern "C" const u32 data_ov003_0222f238[3];
extern "C" char *data_ov003_02232d3c[6];
extern "C" const TreeLeafFxParams *data_ov003_02232760[2];
extern "C" char *data_ov003_02232654[1];
extern "C" char data_ov003_022343f0[0x28];
extern "C" const u32 data_ov003_0222f0a0[3];
extern "C" char data_ov003_02233ce8[0x24];
extern "C" char data_ov003_02233d0c[0x24];
extern "C" char data_ov003_02233d30[0x24];
extern "C" char data_ov003_02233090[0x1c];
extern "C" char data_ov003_02233500[0x20];
extern "C" char data_ov003_02234418[0x28];
extern "C" const u32 data_ov003_0222f328[3];
extern "C" const u32 data_ov003_0222f244[3];
extern "C" char *data_ov003_02232d84[6];
extern "C" char data_ov003_02233100[0x1c];
extern "C" char *data_ov003_02232d9c[6];
extern "C" const TreeLeafFxParams *data_ov003_022327b8[2];
extern "C" char data_ov003_02233138[0x1c];
extern "C" char data_ov003_02234440[0x28];
extern "C" char data_ov003_02232998[0x10];
extern "C" char data_ov003_02234468[0x28];
extern "C" const TreeLeafFxParams **data_ov003_02232a98[4];
extern "C" char *data_ov003_02232db4[6];
extern "C" char *data_ov003_02232aa8[4];
extern "C" char **data_ov003_0223294c[3];
extern "C" const TreeLeafFxParams ***data_ov003_02232c3c[5];
extern "C" char data_ov003_02233520[0x20];
extern "C" char data_ov003_02233e2c[0x24];
extern "C" char data_ov003_02233e50[0x24];
extern "C" char *data_ov003_0223264c[1];
extern "C" char *data_ov003_02232ac8[4];
extern "C" char data_ov003_02233e74[0x24];
extern "C" const u32 data_ov003_0222f214[3];
extern "C" char data_ov003_02233e98[0x24];
extern "C" const TreeLeafFxParams *data_ov003_022326e0[2];
extern "C" char data_ov003_02233ebc[0x24];
extern "C" char data_ov003_02233f04[0x24];
extern "C" char data_ov003_02233f28[0x24];
extern "C" char data_ov003_02232898[0xc];
extern "C" char data_ov003_02233540[0x20];
extern "C" char data_ov003_0223285c[0xc];
extern "C" char data_ov003_02233f70[0x24];
extern "C" char *data_ov003_02232a18[4];
extern "C" char *data_ov003_02232b28[4];
extern "C" const u32 data_ov003_0222f394[3];
extern "C" const u32 data_ov003_0222f37c[3];
extern "C" char data_ov003_022331e0[0x1c];
extern "C" char *data_ov003_02232dfc[6];
extern "C" char data_ov003_02233fb8[0x24];
extern "C" const u32 data_ov003_0222f2a4[3];
extern "C" char data_ov003_02234024[0x24];
extern "C" const u32 data_ov003_0222f268[3];
extern "C" char data_ov003_02233218[0x1c];
extern "C" const TreeLeafFxParams *data_ov003_02232708[2];
extern "C" char data_ov003_02233234[0x1c];
extern "C" char data_ov003_02234048[0x24];
extern "C" const u32 data_ov003_0222f364[3];
extern "C" const u32 data_ov003_0222f28c[3];
extern "C" char data_ov003_02233250[0x1c];
extern "C" char data_ov003_022344e0[0x28];
extern "C" char data_ov003_022340d8[0x24];
extern "C" char *data_ov003_02232634[1];
extern "C" char data_ov003_0223326c[0x1c];
extern "C" char data_ov003_02233560[0x20];
extern "C" char data_ov003_022345a8[0x2c];
extern "C" const u32 data_ov003_0222f2f8[3];
extern "C" char data_ov003_022340fc[0x24];
extern "C" const u32 data_ov003_0222f304[3];
extern "C" char data_ov003_02234508[0x28];
extern "C" char *data_ov003_02232648[1];
extern "C" const TreeLeafFxParams *data_ov003_02232798[2];
extern "C" const u32 data_ov003_0222f31c[3];
extern "C" const TreeLeafFxParams **data_ov003_02232b88[4];
extern "C" const u32 data_ov003_0222f3a0[3];
extern "C" char data_ov003_022332c0[0x1c];
extern "C" const TreeLeafFxParams *data_ov003_022326d0[2];
extern "C" char data_ov003_022332dc[0x1c];
extern "C" char data_ov003_02234120[0x24];
extern "C" char data_ov003_022332f8[0x1c];
extern "C" const u32 data_ov003_0222f4e0[6];
extern "C" char data_ov003_02234530[0x28];
extern "C" const TreeLeafFxParams *data_ov003_02232738[2];
extern "C" char *data_ov003_02232638[1];
extern "C" char data_ov003_02234558[0x28];
extern "C" const u32 data_ov003_0222f3e8[3];
extern "C" char **data_ov003_022328bc[3];
extern "C" const TreeLeafFxParams ****data_ov003_02232928[3];
extern "C" char data_ov003_0223418c[0x24];
extern "C" const TreeLeafFxParams *data_ov003_02232728[2];
extern "C" const u32 data_ov003_0222f40c[3];
extern "C" const TreeLeafFxParams *data_ov003_02232688[2];
extern "C" const u32 data_ov003_0222f490[3];
extern "C" u32 data_ov003_022335c0[8];
extern "C" char data_ov003_022341f8[0x24];
extern "C" const u32 data_ov003_0222f4a8[4];
extern "C" char data_ov003_02233384[0x1c];
extern "C" char data_ov003_0223421c[0x24];
extern "C" char *data_ov003_02232788[2];
extern "C" BOOL (*sFieldObjectLoaders[9])();
extern "C" const u32 data_ov003_0222f1cc[3];
extern "C" const TreeLeafFxParams *data_ov003_022326f8[2];
extern "C" char data_ov003_02234288[0x28];
extern "C" char **data_ov003_022328d4[3];
extern "C" char **data_ov003_02232904[3];
extern "C" char data_ov003_022335e0[0x24];
extern "C" char data_ov003_02233604[0x24];
extern "C" char data_ov003_02232978[0x10];
extern "C" char data_ov003_02232868[0xc];
extern "C" char data_ov003_02233628[0x24];
extern "C" const u32 data_ov003_0222f13c[3];
extern "C" const u32 data_ov003_0222f430[3];
extern "C" const u32 data_ov003_0222f130[3];
extern "C" char data_ov003_02233694[0x24];
extern "C" char data_ov003_02232850[0xc];
extern "C" char data_ov003_02232e44[0x1c];
extern "C" char data_ov003_022336b8[0x24];
extern "C" char data_ov003_02232e60[0x1c];
extern "C" char data_ov003_02234300[0x28];
extern "C" char data_ov003_02232e7c[0x1c];
extern "C" const TreeLeafFxParams *data_ov003_02232680[2];
extern "C" const u32 data_ov003_0222f49c[3];
extern "C" char data_ov003_02233724[0x24];
extern "C" const u32 data_ov003_0222f2e0[3];
extern "C" char data_ov003_02233748[0x24];
extern "C" const TreeLeafFxParams *data_ov003_022327c8[2];
extern "C" const TreeLeafFxParams ***data_ov003_02232c28[5];
extern "C" const u32 data_ov003_0222f340[3];
extern "C" char data_ov003_022337b4[0x24];
extern "C" char *data_ov003_02232a48[4];
extern "C" char data_ov003_022337d8[0x24];
extern "C" char *data_ov003_02232650[1];
extern "C" const u32 data_ov003_0222f3ac[3];
extern "C" const u32 data_ov003_0222f1e4[3];
extern "C" char data_ov003_02234350[0x28];
extern "C" char data_ov003_02233440[0x20];
extern "C" const u32 data_ov003_0222f04c[3];
extern "C" char data_ov003_0223388c[0x24];
extern "C" const TreeLeafFxParams *data_ov003_02232808[2];
extern "C" char data_ov003_02232f40[0x1c];
extern "C" const u32 data_ov003_0222f07c[3];
extern "C" char data_ov003_02233940[0x24];
extern "C" const u32 data_ov003_0222f1a8[3];
extern "C" char data_ov003_02233368[0x1c];
extern "C" char **data_ov003_02232934[3];
extern "C" char data_ov003_022329d8[0x10];
extern "C" char data_ov003_02233460[0x20];
extern "C" const TreeLeafFxParams *data_ov003_022326d8[2];
extern "C" char data_ov003_02233a60[0x24];
extern "C" const TreeLeafFxParams *data_ov003_02232740[2];
extern "C" const u32 data_ov003_0222f2d4[3];
extern "C" char data_ov003_02232a08[0x10];
extern "C" const u32 data_ov003_0222f388[3];
extern "C" char data_ov003_02233acc[0x24];
extern "C" const u32 data_ov003_0222f208[3];
extern "C" char *data_ov003_02232ab8[4];
extern "C" const TreeLeafFxParams **data_ov003_02232a38[4];
extern "C" char *data_ov003_02232940[3];
extern "C" char *data_ov003_02232d0c[6];
extern "C" char data_ov003_02233ba4[0x24];
extern "C" const TreeLeafFxParams *data_ov003_022327c0[2];
extern "C" char data_ov003_022334e0[0x20];
extern "C" char data_ov003_02233c34[0x24];
extern "C" char *data_ov003_02232720[2];
extern "C" const u32 data_ov003_0222f100[3];
extern "C" const u32 data_ov003_0222f484[3];
extern "C" const TreeLeafFxParams **data_ov003_02232a68[4];
extern "C" const TreeLeafFxParams *data_ov003_02232758[2];
extern "C" const u32 data_ov003_0222f040[3];
extern "C" char data_ov003_02233ca0[0x24];
extern "C" char data_ov003_022343c8[0x28];
extern "C" const u32 data_ov003_0222f124[3];
extern "C" const u32 data_ov003_0222f088[3];
extern "C" char data_ov003_02233074[0x1c];
extern "C" const u32 data_ov003_0222f19c[3];
extern "C" const u32 data_ov003_0222f094[3];
extern "C" char *data_ov003_02232d54[6];
extern "C" char **data_ov003_022328ec[3];
extern "C" char data_ov003_022330ac[0x1c];
extern "C" char data_ov003_02233d54[0x24];
extern "C" char data_ov003_022330e4[0x1c];
extern "C" const u32 data_ov003_0222f0dc[3];
extern "C" char data_ov003_02233d9c[0x24];
extern "C" const u32 data_ov003_0222f1b4[3];
extern "C" char data_ov003_02234490[0x28];
extern "C" char data_ov003_02232820[0xc];
extern "C" char data_ov003_0223318c[0x1c];
extern "C" const u32 data_ov003_0222f424[3];
extern "C" char *data_ov003_022326e8[2];
extern "C" char data_ov003_022331a8[0x1c];
extern "C" char data_ov003_022344b8[0x28];
extern "C" char data_ov003_02233ee0[0x24];
extern "C" char data_ov003_02233f4c[0x24];
extern "C" const u32 data_ov003_0222f0c4[3];
extern "C" const TreeLeafFxParams **data_ov003_02232b08[4];
extern "C" const u32 data_ov003_0222f43c[3];
extern "C" char data_ov003_02233f94[0x24];
extern "C" const u32 data_ov003_0222f34c[3];
extern "C" TreeLeafFxCbs data_ov003_02232b48;
extern "C" char data_ov003_02234000[0x24];
extern "C" const TreeLeafFxParams *data_ov003_022327d0[2];
extern "C" char *data_ov003_02232778[2];
extern "C" char data_ov003_0223406c[0x24];
extern "C" char *data_ov003_02232e14[6];
extern "C" const TreeLeafFxParams *data_ov003_02232818[2];
extern "C" const u32 data_ov003_0222f758[46];
extern "C" const u32 data_ov003_0222f2bc[3];
extern "C" const TreeLeafFxParams *data_ov003_02232810[2];
extern "C" char data_ov003_022332a4[0x1c];
extern "C" const TreeLeafFxParams **data_ov003_02232b68[4];
extern "C" char *data_ov003_02232e2c[6];
extern "C" const TreeLeafFxParams **data_ov003_02232b98[4];
extern "C" char data_ov003_02234144[0x24];
extern "C" char *data_ov003_022328b0[3];
extern "C" const TreeLeafFxParams **data_ov003_02232bb8[4];
extern "C" char data_ov003_022335a0[0x20];
extern "C" const u32 data_ov003_0222f3d0[3];
extern "C" char *data_ov003_02232bc8[4];
extern "C" const TreeLeafFxParams *data_ov003_022326b8[2];
extern "C" char data_ov003_022341b0[0x24];
extern "C" char data_ov003_022341d4[0x24];
extern "C" const TreeLeafFxParams ***data_ov003_02232c50[5];
extern "C" char data_ov003_02234580[0x28];
extern "C" const u32 data_ov003_0222f460[3];
extern "C" char *data_ov003_02232bd8[5];
extern "C" char data_ov003_02232958[0x10];
extern "C" char data_ov003_02232968[0x10];
extern "C" char data_ov003_022342b0[0x28];
extern "C" char data_ov003_0223364c[0x24];
extern "C" char data_ov003_02233670[0x24];
extern "C" const u32 data_ov003_0222f190[3];
extern "C" VecFx32 *data_ov003_0223291c[3];
extern "C" const u32 data_ov003_0222f6a0[23];
extern "C" const TreeLeafFxParams *data_ov003_022327a8[2];
extern "C" char data_ov003_02232e98[0x1c];
extern "C" const u32 data_ov003_0222f058[3];
extern "C" const u32 data_ov003_0222f070[3];
extern "C" char data_ov003_02232838[0xc];
extern "C" char data_ov003_02232eec[0x1c];
extern "C" char **data_ov003_022328a4[3];
extern "C" TreeLeafFxCbs data_ov003_02232b78;
extern "C" char *data_ov003_02232c14[5];
extern "C" const u32 data_ov003_0222f454[3];
extern "C" const TreeLeafFxParams *data_ov003_022326a0[2];
extern "C" char data_ov003_02233964[0x24];
extern "C" const u32 data_ov003_0222f3f4[3];
extern "C" char data_ov003_022339f4[0x24];
extern "C" char **data_ov003_022328f8[3];
extern "C" const u32 data_ov003_0222f274[3];
extern "C" const u32 data_ov003_0222f0b8[3];
extern "C" char *data_ov003_02232af8[4];
extern "C" char data_ov003_02233b38[0x24];
extern "C" char *data_ov003_02232cf4[6];
extern "C" char data_ov003_022330c8[0x1c];
extern "C" const u32 data_ov003_0222f184[3];
extern "C" char data_ov003_02232fb0[0x1c];
extern "C" char data_ov003_02232fcc[0x1c];
extern "C" char data_ov003_02232fe8[0x1c];
extern "C" char *data_ov003_02232d24[6];
extern "C" const u32 data_ov003_0222f064[3];
extern "C" char data_ov003_02233cc4[0x24];
extern "C" const u32 data_ov003_0222f3dc[3];
extern "C" const u32 data_ov003_0222f1f0[3];
extern "C" char *data_ov003_02232d6c[6];
extern "C" const u32 data_ov003_0222f0f4[3];
extern "C" char data_ov003_02233dc0[0x24];
extern "C" char data_ov003_02233154[0x1c];
extern "C" char data_ov003_02233e08[0x24];
extern "C" const u32 data_ov003_0222f1fc[3];
extern "C" const TreeLeafFxParams *data_ov003_022327e8[2];
extern "C" const u32 sFieldObjShapeTable[197];
extern "C" char *data_ov003_02232de4[6];
extern "C" const TreeLeafFxParams **data_ov003_02232b38[4];
extern "C" char data_ov003_02233fdc[0x24];
extern "C" char data_ov003_022331fc[0x1c];
extern "C" char data_ov003_02234090[0x24];
extern "C" const TreeLeafFxParams *data_ov003_02232790[2];
extern "C" const TreeLeafFxParams *data_ov003_02232780[2];
extern "C" char data_ov003_02233580[0x20];
extern "C" const TreeLeafFxParams *data_ov003_022327a0[2];
extern "C" const u32 data_ov003_0222f370[3];
extern "C" const u32 data_ov003_0222f0e8[3];
extern "C" char data_ov003_02233314[0x1c];
extern "C" const u32 data_ov003_0222f400[3];
extern "C" char data_ov003_0223334c[0x1c];
extern "C" const u32 data_ov003_0222f448[3];
extern "C" const FieldObjShapeCategory data_ov003_0222f4b8[2];
extern "C" const u32 data_ov003_0222f16c[3];
extern "C" char data_ov003_02232880[0xc];
extern "C" char data_ov003_02232844[0xc];
extern "C" char data_ov003_022336dc[0x24];
extern "C" const TreeLeafFxParams *data_ov003_022326f0[2];
extern "C" char data_ov003_0223376c[0x24];
extern "C" const u32 data_ov003_0222f310[3];
extern "C" char *data_ov003_022328c8[3];
extern "C" char data_ov003_02233844[0x24];
extern "C" char data_ov003_022338f8[0x24];
extern "C" char data_ov003_022339d0[0x24];
extern "C" const u32 data_ov003_0222f0ac[3];
extern "C" char **data_ov003_022328e0[3];
extern "C" char data_ov003_02233b5c[0x24];
extern "C" const TreeLeafFxParams *data_ov003_02232710[2];
extern "C" char data_ov003_02232690[0x8];
extern "C" char data_ov003_02233c58[0x24];
extern "C" char data_ov003_02233058[0x1c];
extern "C" const u32 data_ov003_0222f154[3];
extern "C" char *data_ov003_02232644[1];
extern "C" char data_ov003_0223311c[0x1c];
extern "C" char data_ov003_02233de4[0x24];
extern "C" const TreeLeafFxParams *data_ov003_02232678[2];
extern "C" const TreeLeafFxParams **data_ov003_02232ae8[4];
extern "C" const u32 data_ov003_0222f478[3];
extern "C" const TreeLeafFxParams *data_ov003_02232698[2];
extern "C" const TreeLeafFxParams *data_ov003_02232700[2];
extern "C" const TreeLeafFxParams *data_ov003_02232730[2];
extern "C" const TreeLeafFxParams *data_ov003_02232658[2];
extern "C" const TreeLeafFxParams *data_ov003_022326c8[2];
extern "C" char data_ov003_02233330[0x1c];
extern "C" char data_ov003_02234240[0x24];
extern "C" char data_ov003_022342d8[0x28];
extern "C" char data_ov003_02233700[0x24];
extern "C" char data_ov003_022329a8[0x10];
extern "C" char data_ov003_02232c94[0x18];
extern "C" char data_ov003_02233170[0x1c];
extern "C" char data_ov003_022329e8[0x10];
extern "C" const u32 data_ov003_0222f280[3];
extern "C" char data_ov003_02233bc8[0x24];
extern "C" const TreeLeafFxParams *data_ov003_02232748[2];
extern "C" char data_ov003_02233d78[0x24];
extern "C" char data_ov003_0223282c[0xc];
extern "C" char data_ov003_022331c4[0x1c];
extern "C" char *data_ov003_02232b58[4];
extern "C" char data_ov003_022340b4[0x24];
extern "C" char data_ov003_02234168[0x24];
extern "C" const TreeLeafFxParams *data_ov003_02232670[2];
extern "C" char data_ov003_022333a0[0x20];
extern "C" const TreeLeafFxParams *data_ov003_022327e0[2];
extern "C" char data_ov003_02234378[0x28];
extern "C" char data_ov003_022334c0[0x20];
extern "C" const u32 data_ov003_0222f334[3];
extern "C" const TreeLeafFxParams **data_ov003_02232ad8[4];
extern "C" const TreeLeafFxParams **data_ov003_02232ba8[4];
extern "C" char *data_ov003_02234604[12];
extern "C" char data_ov003_02233a3c[0x24];
extern "C" char *data_ov003_02232640[1];
extern "C" const TreeLeafFxParams *data_ov003_02232750[2];
extern "C" char data_ov003_022333e0[0x20];
// ================================================================ data (creation order)
extern "C" char data_ov003_02232eb4[0x1c] = "/fg/flower/rose/rose.nsbmd";
extern "C" const u32 data_ov003_0222f178[3] = {
    3, 0x1a0001, 0x59a,
};
extern "C" char data_ov003_022333c0[0x20] = "/fg/flower/tulip/tulip.nsbmd";
extern "C" const u32 data_ov003_0222f160[3] = {
    2, 0x120001, 0x171,
};
extern "C" char data_ov003_02233790[0x24] = "/fg/tree/tree_mdl/tree_02s.nsbmd";
extern "C" char data_ov003_02234328[0x28] = "/fg/tree/cedar_mdl/cedar_babys.nsbmd";
extern "C" const u32 data_ov003_0222f0d0[3] = {
    0x12, 0xe0008, 0xa4,
};
extern "C" char data_ov003_02232c7c[0x18] = "/fg/grass/grassB.nsbmd";
extern "C" char data_ov003_02233400[0x20] = "/fg/eff_tree/shakeS_03.nsbca";
extern "C" char data_ov003_022345d4[0x30] = "/fg/flower/rafflesia/rafflesia_palette.nsbtx";
extern "C" char data_ov003_02233288[0x1c] = "/fg/eff_tree/anm_p04w.nsbmd";
extern "C" char data_ov003_02232ed0[0x1c] = "/fg/eff_tree/cutR_04.nsbca";
extern "C" char data_ov003_02232874[0xc] = "obj_stnDw";
extern "C" alignas(4) const u8 sUnitSearchOffsets81[84] = {
    0x88, 0x87, 0x89, 0x78, 0x98, 0x77, 0x99, 0x79, 0x97, 0x8a, 0x86, 0xa8, 0x68, 0x9a, 0x76, 0x7a,
    0x96, 0x69, 0xa7, 0x67, 0xa9, 0x6a, 0xa6, 0xaa, 0x66, 0x8b, 0x85, 0xb8, 0x58, 0x7b, 0x95, 0x9b,
    0x75, 0x57, 0xb9, 0x59, 0xb7, 0x6b, 0xa5, 0xab, 0x65, 0x5a, 0xb6, 0xba, 0x56, 0x5b, 0xb5, 0xbb,
    0x55, 0x8c, 0x84, 0xc8, 0x48, 0x7c, 0x94, 0x9c, 0x74, 0x47, 0xc9, 0x49, 0xc7, 0x6c, 0xa4, 0xac,
    0x64, 0x46, 0xca, 0x4a, 0xc6, 0x5c, 0xb4, 0xbc, 0x54, 0x4b, 0xc5, 0x45, 0xcb, 0x4c, 0xc4, 0xcc,
    0x44, 0x00, 0x00, 0x00,
};
extern "C" char data_ov003_02232f08[0x1c] = "/fg/eff_tree/cutL_01.nsbca";
extern "C" char data_ov003_02232c64[0x18] = "/fg/grass/grassA.nsbmd";
extern "C" const u32 data_ov003_0222f148[3] = {
    0x12, 0xa000a, 0xe1,
};
extern "C" char data_ov003_022337fc[0x24] = "/fg/eff_tree/cutL_04_1unit.nsbca";
TreeLeafFx sTreeLeafFx;
extern "C" char data_ov003_02232cac[0x18] = "/fg/grass/grassD.nsbmd";
extern "C" char data_ov003_022329b8[0x10] = "tree03_stamp";
extern "C" const u32 data_ov003_0222f1c0[3] = {
    0x13, 0xe0004, 0x2e1,
};
extern "C" char data_ov003_02233868[0x24] = "/fg/tree/palm_mdl/palm_02w.nsbmd";
extern "C" char data_ov003_02233420[0x20] = "/fg/eff_tree/shakeS_04.nsbca";
extern "C" const TreeLeafFxParams *data_ov003_022326a8[2] = {
    (const TreeLeafFxParams *)data_ov003_0222f43c, (const TreeLeafFxParams *)data_ov003_0222f088,
};
extern "C" const u32 data_ov003_0222f25c[3] = {
    0x1a, 0x10001b, 0x8f,
};
extern "C" char data_ov003_022329c8[0x10] = "tree01_stamp";
extern "C" const u32 data_ov003_0222f3b8[3] = {
    0x1a, 0x200008, 0xa4,
};
extern "C" char *data_ov003_02232dcc[6] = {
    data_ov003_02234048, data_ov003_02233aa8, data_ov003_02233acc, data_ov003_02233af0,
    data_ov003_02233b14, data_ov003_0223406c,
};
extern "C" char data_ov003_02232cc4[0x18] = "/fg/grass/clover.nsbmd";
extern "C" const TreeLeafFxParams *data_ov003_022327b0[2] = {
    (const TreeLeafFxParams *)data_ov003_0222f2c8, (const TreeLeafFxParams *)data_ov003_0222f1e4,
};
extern "C" char data_ov003_022338b0[0x24] = "/fg/tree/palm_mdl/palm_04w.nsbmd";
extern "C" char data_ov003_02232f24[0x1c] = "/fg/eff_tree/cutL_02.nsbca";
extern "C" char data_ov003_022338d4[0x24] = "/fg/tree/palm_mdl/palm_01s.nsbmd";
extern "C" char data_ov003_0223391c[0x24] = "/fg/tree/palm_mdl/palm_03s.nsbmd";
extern "C" char data_ov003_02232988[0x10] = "palm03_stamp";
extern "C" char data_ov003_02233820[0x24] = "/fg/eff_tree/cutR_02_1unit.nsbca";
extern "C" const u32 data_ov003_0222f3c4[3] = {
    3, 0x230001, 0x19a,
};
extern "C" const u32 data_ov003_0222f358[3] = {
    0x13, 0x1a000a, 0x28f,
};
extern "C" char data_ov003_02232f5c[0x1c] = "/fg/eff_tree/cutL_04.nsbca";
extern "C" char data_ov003_02233988[0x24] = "/fg/tree/tree_tex/tree_01w.nsbtx";
extern "C" char data_ov003_022339ac[0x24] = "/fg/tree/tree_tex/tree_02w.nsbtx";
extern "C" const u32 data_ov003_0222f418[3] = {
    3, 0xe0001, 0x2cd,
};
extern "C" const TreeLeafFxParams *data_ov003_022327f0[2] = {
    (const TreeLeafFxParams *)data_ov003_0222f250, (const TreeLeafFxParams *)data_ov003_0222f478,
};
extern "C" const TreeLeafFxParams *data_ov003_022326c0[2] = {
    (const TreeLeafFxParams *)data_ov003_0222f04c, 0,
};
extern "C" char data_ov003_022343a0[0x28] = "/fg/tree/cedar_tex/cedar_deadw.nsbtx";
extern "C" const u32 data_ov003_0222f46c[3] = {
    0x13, 0xa000a, 0x28f,
};
extern "C" const u32 data_ov003_0222f1d8[3] = {
    0x12, 0xa000a, 0xe1,
};
extern "C" const TreeLeafFxParams *data_ov003_02232800[2] = {
    (const TreeLeafFxParams *)data_ov003_0222f430, (const TreeLeafFxParams *)data_ov003_0222f424,
};
extern "C" char data_ov003_02233a18[0x24] = "/fg/tree/tree_tex/tree_01f.nsbtx";
extern "C" const u32 data_ov003_0222f2b0[3] = {
    0x13, 0xb0006, 0x28f,
};
extern "C" const TreeLeafFxParams *data_ov003_022327f8[2] = {
    (const TreeLeafFxParams *)data_ov003_0222f0ac, (const TreeLeafFxParams *)data_ov003_0222f190,
};
extern "C" char data_ov003_02233480[0x20] = "/fg/eff_tree/shakeS_01.nsbca";
extern "C" alignas(4) const u8 data_ov003_0222f534[48] = {
    0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};
extern "C" char data_ov003_02233a84[0x24] = "/fg/tree/tree_tex/tree_04f.nsbtx";
extern "C" const u32 data_ov003_0222f250[3] = {
    4, 0x230001, 0x70a,
};
extern "C" char data_ov003_02232cdc[0x18] = "/fg/hole/obj_hole.nsbmd";
extern "C" const u32 data_ov003_0222f2c8[3] = {
    3, 0xe0001, 0x23d,
};
extern "C" char data_ov003_02233aa8[0x24] = "/fg/tree/tree_tex/tree_01s.nsbtx";
extern "C" char *data_ov003_0223263c[1] = {
    data_ov003_02234580,
};
extern "C" const u32 data_ov003_0222f220[3] = {
    2, 0xa0001, 0x19a,
};
extern "C" char data_ov003_02232f78[0x1c] = "/fg/eff_tree/cutR_01.nsbca";
extern "C" char data_ov003_02233af0[0x24] = "/fg/tree/tree_tex/tree_03s.nsbtx";
extern "C" const u32 data_ov003_0222f2ec[3] = {
    0x12, 0x7001b, 0xa4,
};
extern "C" char data_ov003_02233b14[0x24] = "/fg/tree/tree_tex/tree_04s.nsbtx";
extern "C" char *data_ov003_02232a28[4] = {
    data_ov003_022333c0, data_ov003_022333e0, data_ov003_022334c0, data_ov003_02232eb4,
};
extern "C" char *data_ov003_02232b18[4] = {
    data_ov003_02233170, data_ov003_0223318c, data_ov003_022331a8, data_ov003_022331c4,
};
extern "C" char data_ov003_0223288c[0xc] = "obj_stnCw";
extern "C" const s32 data_ov003_0222f564[3][4] = {
    {0x1800, 0x2000, 0x2e66, 0x3c00},
    {0x1666, 0x2333, 0x2e66, 0x3c00},
    {0x1400, 0x1c00, 0x299a, 0x3333},
};
extern "C" const TreeLeafFxParams *data_ov003_02232660[2] = {
    (const TreeLeafFxParams *)data_ov003_0222f37c, (const TreeLeafFxParams *)data_ov003_0222f460,
};
extern "C" const TreeLeafFxParams *data_ov003_02232718[2] = {
    (const TreeLeafFxParams *)data_ov003_0222f2f8, 0,
};
extern "C" const u32 data_ov003_0222f22c[3] = {
    0x12, 0x8000f, 0x8f,
};
extern "C" char data_ov003_022334a0[0x20] = "/fg/grass/grass_palette.nsbtx";
extern "C" char data_ov003_02233b80[0x24] = "/fg/tree/palm_tex/palm_02w.nsbtx";
extern "C" char data_ov003_02233bec[0x24] = "/fg/tree/tree_mdl/tree_01f.nsbmd";
extern "C" char data_ov003_02232f94[0x1c] = "/fg/eff_tree/cutR_02.nsbca";
extern "C" char data_ov003_02233c10[0x24] = "/fg/tree/palm_tex/palm_01s.nsbtx";
extern "C" const TreeLeafFxParams *data_ov003_022326b0[2] = {
    (const TreeLeafFxParams *)data_ov003_0222f238, (const TreeLeafFxParams *)data_ov003_0222f124,
};
extern "C" alignas(4) const u8 sLandingUnitOffsets[12] = {
    0x88, 0x89, 0x87, 0x78, 0x98, 0x79, 0x99, 0x77, 0x97, 0x00, 0x00, 0x00,
};
extern "C" const TreeLeafFxParams *data_ov003_02232768[2] = {
    (const TreeLeafFxParams *)data_ov003_0222f3a0, 0,
};
extern "C" const TreeLeafFxParams *data_ov003_022327d8[2] = {
    (const TreeLeafFxParams *)data_ov003_0222f040, (const TreeLeafFxParams *)data_ov003_0222f484,
};
extern "C" alignas(4) const u16 sTreeFruitIcons[6] = {
    0x1518, 0x1519, 0x151a, 0x151b, 0x151c, 0x0,
};
extern "C" char *data_ov003_02232630[1] = {
    data_ov003_02233004,
};
extern "C" const TreeLeafFxParams *data_ov003_02232770[2] = {
    (const TreeLeafFxParams *)data_ov003_0222f1f0, (const TreeLeafFxParams *)data_ov003_0222f2d4,
};
extern "C" const TreeLeafFxParams **data_ov003_02232a58[4] = {
    data_ov003_02232800, data_ov003_022327d8, data_ov003_022327a8, data_ov003_022326a8,
};
extern "C" const u32 data_ov003_0222f6fc[23] = {
    3, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 2, 2, 3,
};
extern "C" alignas(4) const u16 sTreeFruitItems[6] = {
    0x1518, 0x1519, 0x151a, 0x151b, 0x151c, 0x0,
};
extern "C" const u32 data_ov003_0222f10c[3] = {
    0x1a, 0x14000f, 0x8f,
};
extern "C" const u32 data_ov003_0222f118[3] = {
    0x12, 0x7001b, 0x29,
};
extern "C" char *sTreeAnimPaths[32] = {
    data_ov003_02233330, data_ov003_0223334c, data_ov003_02233368, data_ov003_02233384,
    data_ov003_02233480, data_ov003_022333a0, data_ov003_02233400, data_ov003_02233420,
    data_ov003_02233330, data_ov003_022334e0, data_ov003_02233500, data_ov003_02233520,
    data_ov003_02233330, data_ov003_02233580, data_ov003_02233540, data_ov003_02233560,
    data_ov003_02232f08, data_ov003_02232f24, data_ov003_02232f40, data_ov003_02232f5c,
    data_ov003_02232f78, data_ov003_02232f94, data_ov003_02232fcc, data_ov003_02232ed0,
    data_ov003_02232f08, data_ov003_02233628, data_ov003_022336dc, data_ov003_022337fc,
    data_ov003_02232f78, data_ov003_02233820, data_ov003_02233964, data_ov003_02233b38,
};
extern "C" char data_ov003_02233c7c[0x24] = "/fg/tree/palm_tex/palm_04s.nsbtx";
extern "C" const TreeLeafFxParams **data_ov003_02232a78[4] = {
    data_ov003_02232658, data_ov003_022326f8, data_ov003_02232738, data_ov003_02232798,
};
extern "C" char data_ov003_022329f8[0x10] = "cedar02_stamp";
extern "C" char data_ov003_02233004[0x1c] = "/fg/hole/obj_crack_pl.nsbtx";
extern "C" char *data_ov003_02232bec[5] = {
    data_ov003_02232898, data_ov003_02232850, data_ov003_02232838, data_ov003_02232820,
    data_ov003_02232868,
};
extern "C" char **data_ov003_02232910[3] = {
    data_ov003_02232d3c, data_ov003_02232d54, data_ov003_02232d54,
};
extern "C" char data_ov003_02233020[0x1c] = "/fg/grass/redTurnip_s.nsbmd";
extern "C" char data_ov003_0223303c[0x1c] = "/fg/eff_tree/anm_t01w.nsbmd";
extern "C" const TreeLeafFxParams **data_ov003_02232a88[4] = {
    data_ov003_02232740, data_ov003_02232758, data_ov003_02232688, data_ov003_022326d8,
};
extern "C" const u32 data_ov003_0222f238[3] = {
    2, 0xc0001, 0x571,
};
extern "C" char *data_ov003_02232d3c[6] = {
    data_ov003_022342d8, data_ov003_02233e08, data_ov003_02233e2c, data_ov003_02233e50,
    data_ov003_02233e74, data_ov003_02234300,
};
extern "C" const TreeLeafFxParams *data_ov003_02232760[2] = {
    (const TreeLeafFxParams *)data_ov003_0222f418, (const TreeLeafFxParams *)data_ov003_0222f2bc,
};
extern "C" char *data_ov003_02232654[1] = {
    data_ov003_02232eec,
};
extern "C" char data_ov003_022343f0[0x28] = "/fg/tree/cedar_tex/cedar_deads.nsbtx";
extern "C" const u32 data_ov003_0222f0a0[3] = {
    4, 0x1e0001, 0x63d,
};
extern "C" char data_ov003_02233ce8[0x24] = "/fg/tree/tree_pal/tree_setC.nsbtx";
// (registration node 0x223593c is created with the object below)
FieldItemFxTable sFieldItemFxTable;   // __sinit: vector ctor + registered destructor
extern "C" char data_ov003_02233d0c[0x24] = "/fg/flower/rose/rose_palette.nsbtx";
extern "C" char data_ov003_02233d30[0x24] = "/fg/tree/tree_mdl/tree_babyw.nsbmd";
Heap *sFieldObjectModelHeap;
extern "C" char data_ov003_02233090[0x1c] = "/fg/eff_tree/anm_t01f.nsbmd";
extern "C" char data_ov003_02233500[0x20] = "/fg/eff_tree/shakeL_03_1.nsbca";
extern "C" char data_ov003_02234418[0x28] = "/fg/tree/cedar_tex/obj_x_cedar.nsbtx";
extern "C" const u32 data_ov003_0222f328[3] = {
    0x1a, 0x1c000a, 0xe1,
};
extern "C" const u32 data_ov003_0222f244[3] = {
    0x13, 0x210006, 0x28f,
};
extern "C" char *data_ov003_02232d84[6] = {
    data_ov003_02233f70, data_ov003_022338d4, data_ov003_022338f8, data_ov003_0223391c,
    data_ov003_02233940, data_ov003_02233f94,
};
extern "C" char data_ov003_02233100[0x1c] = "/fg/eff_tree/anm_t02s.nsbmd";
extern "C" char *data_ov003_02232d9c[6] = {
    data_ov003_02233fb8, data_ov003_02233988, data_ov003_022339ac, data_ov003_022339d0,
    data_ov003_022339f4, data_ov003_02233fdc,
};
extern "C" const TreeLeafFxParams *data_ov003_022327b8[2] = {
    (const TreeLeafFxParams *)data_ov003_0222f34c, 0,
};
// (a ProcProfile: the create function is a pointer, larger on 64-bit hosts)
ProcProfile sFieldObjectManagerProfile = {(void *(*)(void))FieldObjectManager_Create, 0x8a, 0xb};
extern "C" char data_ov003_02233138[0x1c] = "/fg/eff_tree/anm_t04s.nsbmd";
extern "C" char data_ov003_02234440[0x28] = "/fg/tree/cedar_mdl/obj_x_cedar.nsbmd";
extern "C" char data_ov003_02232998[0x10] = "palm04_stamp";
extern "C" char data_ov003_02234468[0x28] = "/fg/flower/tulip/tulip_palette.nsbtx";
extern "C" const TreeLeafFxParams **data_ov003_02232a98[4] = {
    0, data_ov003_02232808, data_ov003_022327e0, data_ov003_02232768,
};
extern "C" char *data_ov003_02232db4[6] = {
    data_ov003_02234000, data_ov003_02233a18, data_ov003_02233a3c, data_ov003_02233a60,
    data_ov003_02233a84, data_ov003_02234024,
};
extern "C" char *data_ov003_02232aa8[4] = {
    data_ov003_0223303c, data_ov003_02233058, data_ov003_02233314, data_ov003_02233074,
};
extern "C" char **data_ov003_0223294c[3] = {
    data_ov003_02232aa8, data_ov003_02232ab8, data_ov003_02232ac8,
};
extern "C" const TreeLeafFxParams ***data_ov003_02232c3c[5] = {
    data_ov003_02232ad8, data_ov003_02232b38, data_ov003_02232a88, data_ov003_02232ba8,
    data_ov003_02232a58,
};
extern "C" char data_ov003_02233520[0x20] = "/fg/eff_tree/shakeL_04_1.nsbca";
extern "C" char data_ov003_02233e2c[0x24] = "/fg/tree/cedar_mdl/cedar_02w.nsbmd";
extern "C" char data_ov003_02233e50[0x24] = "/fg/tree/cedar_mdl/cedar_03w.nsbmd";
extern "C" char *data_ov003_0223264c[1] = {
    data_ov003_02234440,
};
extern "C" char *data_ov003_02232ac8[4] = {
    data_ov003_02232fe8, data_ov003_02233100, data_ov003_0223311c, data_ov003_02233138,
};
extern "C" char data_ov003_02233e74[0x24] = "/fg/tree/cedar_mdl/cedar_04w.nsbmd";
extern "C" const u32 data_ov003_0222f214[3] = {
    0x13, 0xe0004, 0x2e1,
};
extern "C" char data_ov003_02233e98[0x24] = "/fg/tree/cedar_mdl/cedar_01s.nsbmd";
extern "C" const TreeLeafFxParams *data_ov003_022326e0[2] = {
    (const TreeLeafFxParams *)data_ov003_0222f2b0, (const TreeLeafFxParams *)data_ov003_0222f244,
};
extern "C" char data_ov003_02233ebc[0x24] = "/fg/tree/cedar_mdl/cedar_02s.nsbmd";
extern "C" char data_ov003_02233f04[0x24] = "/fg/tree/cedar_mdl/cedar_04s.nsbmd";
extern "C" char data_ov003_02233f28[0x24] = "/fg/tree/palm_mdl/palm_babyw.nsbmd";
extern "C" char data_ov003_02232898[0xc] = "obj_stnAs";
extern "C" char data_ov003_02233540[0x20] = "/fg/eff_tree/shakeL_03_2.nsbca";
extern "C" char data_ov003_0223285c[0xc] = "tanpopo_w";
extern "C" char data_ov003_02233f70[0x24] = "/fg/tree/palm_mdl/palm_babys.nsbmd";
extern "C" char *data_ov003_02232a18[4] = {
    data_ov003_02234468, data_ov003_02234490, data_ov003_022344b8, data_ov003_02233d0c,
};
extern "C" char *data_ov003_02232b28[4] = {
    data_ov003_02233250, data_ov003_02233154, data_ov003_0223326c, data_ov003_02233288,
};
extern "C" const u32 data_ov003_0222f394[3] = {
    0x1a, 0x280008, 0x15c,
};
extern "C" const u32 data_ov003_0222f37c[3] = {
    2, 0xb0001, 0x200,
};
extern "C" char data_ov003_022331e0[0x1c] = "/fg/eff_tree/anm_c01s.nsbmd";
extern "C" char *data_ov003_02232dfc[6] = {
    data_ov003_022343c8, data_ov003_02234120, data_ov003_02234144, data_ov003_02234168,
    data_ov003_0223418c, data_ov003_022343f0,
};
extern "C" char data_ov003_02233fb8[0x24] = "/fg/tree/tree_tex/tree_babyw.nsbtx";
FieldObjectManager *gFieldObjectManager;
extern "C" const u32 data_ov003_0222f2a4[3] = {
    3, 0xe0001, 0x19a,
};
extern "C" const s32 data_ov003_0222f594[3][4] = {
    {0x733, 0xc00, 0x119a, 0x1666},
    {0x666, 0x99a, 0xccd, 0x1333},
    {0x733, 0xb85, 0xe66, 0x1333},
};
extern "C" char data_ov003_02234024[0x24] = "/fg/tree/tree_tex/tree_deadf.nsbtx";
extern "C" const u32 data_ov003_0222f268[3] = {
    2, 0xb0001, 0x171,
};
extern "C" char data_ov003_02233218[0x1c] = "/fg/eff_tree/anm_c03s.nsbmd";
extern "C" const TreeLeafFxParams *data_ov003_02232708[2] = {
    (const TreeLeafFxParams *)data_ov003_0222f064, (const TreeLeafFxParams *)data_ov003_0222f19c,
};
extern "C" char data_ov003_02233234[0x1c] = "/fg/eff_tree/anm_c04s.nsbmd";
extern "C" char data_ov003_02234048[0x24] = "/fg/tree/tree_tex/tree_babys.nsbtx";
extern "C" const u32 data_ov003_0222f364[3] = {
    8, 0x80005, 0xe1,
};
extern "C" const u32 data_ov003_0222f28c[3] = {
    2, 0x140001, 0x19a,
};
extern "C" char data_ov003_02233250[0x1c] = "/fg/eff_tree/anm_p01w.nsbmd";
extern "C" char data_ov003_022344e0[0x28] = "/fg/tree/tree_pal/tree_baby_setA.nsbtx";
extern "C" char data_ov003_022340d8[0x24] = "/fg/tree/cedar_tex/cedar_03w.nsbtx";
extern "C" char *data_ov003_02232634[1] = {
    data_ov003_02232cdc,
};
extern "C" char data_ov003_0223326c[0x1c] = "/fg/eff_tree/anm_p03w.nsbmd";
extern "C" char data_ov003_02233560[0x20] = "/fg/eff_tree/shakeL_04_2.nsbca";
extern "C" char data_ov003_022345a8[0x2c] = "/fg/flower/suzuran/suzuran_palette.nsbtx";
extern "C" const u32 data_ov003_0222f2f8[3] = {
    8, 0xe0005, 0x15c,
};
extern "C" char data_ov003_022340fc[0x24] = "/fg/tree/cedar_tex/cedar_04w.nsbtx";
extern "C" const u32 data_ov003_0222f304[3] = {
    0x1a, 0x18000a, 0x8f,
};
extern "C" char data_ov003_02234508[0x28] = "/fg/tree/tree_pal/tree_baby_setB.nsbtx";
extern "C" char *data_ov003_02232648[1] = {
    data_ov003_02232e7c,
};
extern "C" const TreeLeafFxParams *data_ov003_02232798[2] = {
    (const TreeLeafFxParams *)data_ov003_0222f184, (const TreeLeafFxParams *)data_ov003_0222f394,
};
extern "C" const u32 data_ov003_0222f31c[3] = {
    0x12, 0x8000f, 0xa4,
};
extern "C" const TreeLeafFxParams **data_ov003_02232b88[4] = {
    0, data_ov003_022326c0, data_ov003_02232810, data_ov003_02232718,
};
// (registration node 0x2235954 is created with the object below)
FxVec3 sBalloonSplashPos;   // __sinit: registered destructor
extern "C" const u32 data_ov003_0222f3a0[3] = {
    8, 0xe0005, 0x15c,
};
extern "C" char data_ov003_022332c0[0x1c] = "/fg/eff_tree/anm_p02s.nsbmd";
extern "C" const TreeLeafFxParams *data_ov003_022326d0[2] = {
    (const TreeLeafFxParams *)data_ov003_0222f454, (const TreeLeafFxParams *)data_ov003_0222f40c,
};
extern "C" char data_ov003_022332dc[0x1c] = "/fg/eff_tree/anm_p03s.nsbmd";
extern "C" char data_ov003_02234120[0x24] = "/fg/tree/cedar_tex/cedar_01s.nsbtx";
extern "C" char data_ov003_022332f8[0x1c] = "/fg/eff_tree/anm_p04s.nsbmd";
extern "C" const u32 data_ov003_0222f4e0[6] = {
    0xffffef00, 0x2800, 0xffffff00, 0x1100, 0x3000, 0xfffff800,
};
extern "C" char data_ov003_02234530[0x28] = "/fg/tree/tree_pal/tree_baby_setC.nsbtx";
extern "C" const TreeLeafFxParams *data_ov003_02232738[2] = {
    (const TreeLeafFxParams *)data_ov003_0222f1d8, (const TreeLeafFxParams *)data_ov003_0222f448,
};
extern "C" char *data_ov003_02232638[1] = {
    data_ov003_02234240,
};
u8 sFieldObjectModelHeapBuf[FIELDOBJ_MODEL_HEAP_SIZE] HOST_ALIGN8;
extern "C" char data_ov003_02234558[0x28] = "/fg/tree/cedar_pal/cedar_baby_set.nsbtx";
extern "C" const u32 data_ov003_0222f3e8[3] = {
    2, 0x1c0001, 0x266,
};
extern "C" char **data_ov003_022328bc[3] = {
    data_ov003_02232b28, data_ov003_02232b58, data_ov003_02232b58,
};
extern "C" const TreeLeafFxParams ****data_ov003_02232928[3] = {
    data_ov003_02232c50, data_ov003_02232c28, data_ov003_02232c3c,
};
extern "C" char data_ov003_0223418c[0x24] = "/fg/tree/cedar_tex/cedar_04s.nsbtx";
extern "C" const TreeLeafFxParams *data_ov003_02232728[2] = {
    (const TreeLeafFxParams *)data_ov003_0222f148, (const TreeLeafFxParams *)data_ov003_0222f328,
};
extern "C" const u32 data_ov003_0222f40c[3] = {
    0x13, 0x230004, 0x214,
};
extern "C" const TreeLeafFxParams *data_ov003_02232688[2] = {
    (const TreeLeafFxParams *)data_ov003_0222f274, (const TreeLeafFxParams *)data_ov003_0222f304,
};
extern "C" const u32 data_ov003_0222f490[3] = {
    0x13, 0x1c0006, 0x28f,
};
extern "C" u32 data_ov003_022335c0[8] = {
    1, 0, 2, 2, 3, 3, 3, 3,
};
extern "C" char data_ov003_022341f8[0x24] = "/fg/tree/palm_tex/palm_babys.nsbtx";
extern "C" const u32 data_ov003_0222f4a8[4] = {
    7, 7, 7, 0xa,
};
extern "C" char data_ov003_02233384[0x1c] = "/fg/eff_tree/shake_04.nsbca";
extern "C" char data_ov003_0223421c[0x24] = "/fg/tree/palm_tex/palm_deads.nsbtx";
extern "C" char *data_ov003_02232788[2] = {
    data_ov003_02232e98, data_ov003_02233020,
};
extern "C" BOOL (*sFieldObjectLoaders[9])() = {
    FieldObj_LoadTrees, FieldObj_LoadFlowers, FieldObj_LoadGrass, FieldObj_LoadStones,
    FieldObj_LoadHoles, FieldObj_LoadCracks, FieldObj_LoadStumps, FieldObj_LoadTurnips,
    FieldObj_LoadDesigns,
};
extern "C" const VecFx32 data_ov003_0222f4f8[2] = {
    {-0x700, 0x2633, -0x1cd},
    {0x480, 0x20b3, -0x80},
};
extern "C" const u32 data_ov003_0222f1cc[3] = {
    1, 0x80001, 0x8f,
};
extern "C" const TreeLeafFxParams *data_ov003_022326f8[2] = {
    (const TreeLeafFxParams *)data_ov003_0222f22c, (const TreeLeafFxParams *)data_ov003_0222f10c,
};
extern "C" char data_ov003_02234288[0x28] = "/fg/flower/rafflesia/rafflesia.nsbtx";
extern "C" char **data_ov003_022328d4[3] = {
    data_ov003_02232de4, data_ov003_02232dfc, data_ov003_02232dfc,
};
extern "C" char **data_ov003_02232904[3] = {
    data_ov003_02232d9c, data_ov003_02232db4, data_ov003_02232dcc,
};
extern "C" char data_ov003_022335e0[0x24] = "/fg/flower/suzuran/suzuran.nsbtx";
extern "C" char data_ov003_02233604[0x24] = "/fg/flower/suzuran/suzuran.nsbmd";
extern "C" char data_ov003_02232978[0x10] = "palm02_stamp";
extern "C" char data_ov003_02232868[0xc] = "obj_stnEs";
extern "C" char data_ov003_02233628[0x24] = "/fg/eff_tree/cutL_02_1unit.nsbca";
extern "C" const u32 data_ov003_0222f13c[3] = {
    2, 0x1a0001, 0x200,
};
extern "C" const u32 data_ov003_0222f430[3] = {
    2, 0xc0001, 0x51f,
};
extern "C" const u32 data_ov003_0222f130[3] = {
    5, 0xe0001, 0x733,
};
extern "C" char data_ov003_02233694[0x24] = "/fg/tree/tree_mdl/tree_03w.nsbmd";
extern "C" char data_ov003_02232850[0xc] = "obj_stnBs";
extern "C" char data_ov003_02232e44[0x1c] = "/fg/stone/obj_stnw.nsbmd";
extern "C" char data_ov003_022336b8[0x24] = "/fg/tree/tree_mdl/tree_04w.nsbmd";
extern "C" char data_ov003_02232e60[0x1c] = "/fg/stone/obj_stns.nsbmd";
extern "C" char data_ov003_02234300[0x28] = "/fg/tree/cedar_mdl/cedar_deadw.nsbmd";
extern "C" char data_ov003_02232e7c[0x1c] = "/fg/hole/obj_crack.nsbmd";
extern "C" const TreeLeafFxParams *data_ov003_02232680[2] = {
    (const TreeLeafFxParams *)data_ov003_0222f400, (const TreeLeafFxParams *)data_ov003_0222f358,
};
extern "C" const u32 data_ov003_0222f49c[3] = {
    4, 0x1e0001, 0x666,
};
extern "C" char data_ov003_02233724[0x24] = "/fg/tree/tree_mdl/tree_03f.nsbmd";
extern "C" const u32 data_ov003_0222f2e0[3] = {
    0x1a, 0xf001b, 0xa4,
};
extern "C" char data_ov003_02233748[0x24] = "/fg/tree/tree_mdl/tree_04f.nsbmd";
extern "C" const TreeLeafFxParams *data_ov003_022327c8[2] = {
    (const TreeLeafFxParams *)data_ov003_0222f31c, (const TreeLeafFxParams *)data_ov003_0222f310,
};
extern "C" const TreeLeafFxParams ***data_ov003_02232c28[5] = {
    data_ov003_02232a98, data_ov003_02232bb8, data_ov003_02232b98, data_ov003_02232a38,
    data_ov003_02232a68,
};
extern "C" const u32 data_ov003_0222f340[3] = {
    0x13, 0xb0006, 0x1ec,
};
extern "C" char data_ov003_022337b4[0x24] = "/fg/tree/tree_mdl/tree_03s.nsbmd";
extern "C" char *data_ov003_02232a48[4] = {
    data_ov003_022345a8, data_ov003_022345d4, 0, 0,
};
extern "C" char data_ov003_022337d8[0x24] = "/fg/tree/tree_mdl/tree_04s.nsbmd";
extern "C" char *data_ov003_02232650[1] = {
    data_ov003_022334a0,
};
extern "C" const u32 data_ov003_0222f3ac[3] = {
    2, 0xa0001, 0x200,
};
extern "C" const u32 data_ov003_0222f1e4[3] = {
    3, 0x280001, 0x23d,
};
extern "C" const s32 data_ov003_0222f298[3] = {
    0x23, 0x26, 0x28,
};
extern "C" char data_ov003_02234350[0x28] = "/fg/tree/cedar_mdl/cedar_deads.nsbmd";
extern "C" char data_ov003_02233440[0x20] = "/fg/flower/tulip/tulip.nsbtx";
extern "C" const u32 data_ov003_0222f04c[3] = {
    1, 0x80001, 0x8f,
};
extern "C" char data_ov003_0223388c[0x24] = "/fg/tree/palm_mdl/palm_03w.nsbmd";
extern "C" const TreeLeafFxParams *data_ov003_02232808[2] = {
    (const TreeLeafFxParams *)data_ov003_0222f1cc, 0,
};
extern "C" char data_ov003_02232f40[0x1c] = "/fg/eff_tree/cutL_03.nsbca";
extern "C" const u32 data_ov003_0222f07c[3] = {
    4, 0x230001, 0x666,
};
extern "C" char data_ov003_02233940[0x24] = "/fg/tree/palm_mdl/palm_04s.nsbmd";
extern "C" const u32 data_ov003_0222f1a8[3] = {
    8, 0xa0005, 0xe1,
};
extern "C" char data_ov003_02233368[0x1c] = "/fg/eff_tree/shake_03.nsbca";
extern "C" char **data_ov003_02232934[3] = {
    data_ov003_02232e14, data_ov003_02232e2c, data_ov003_02232e2c,
};
extern "C" char data_ov003_022329d8[0x10] = "cedar03_stamp";
extern "C" char data_ov003_02233460[0x20] = "/fg/flower/pansy/pansy.nsbtx";
extern "C" const TreeLeafFxParams *data_ov003_022326d8[2] = {
    (const TreeLeafFxParams *)data_ov003_0222f0d0, (const TreeLeafFxParams *)data_ov003_0222f3b8,
};
extern "C" char data_ov003_02233a60[0x24] = "/fg/tree/tree_tex/tree_03f.nsbtx";
extern "C" const TreeLeafFxParams *data_ov003_02232740[2] = {
    (const TreeLeafFxParams *)data_ov003_0222f118, (const TreeLeafFxParams *)data_ov003_0222f280,
};
extern "C" const u32 data_ov003_0222f2d4[3] = {
    0x13, 0x12000a, 0x1ec,
};
extern "C" char data_ov003_02232a08[0x10] = "cedar04_stamp";
extern "C" const u32 data_ov003_0222f388[3] = {
    0x1a, 0x11000f, 0x52,
};
extern "C" char data_ov003_02233acc[0x24] = "/fg/tree/tree_tex/tree_02s.nsbtx";
extern "C" const u32 data_ov003_0222f208[3] = {
    2, 0x190001, 0x171,
};
extern "C" char *data_ov003_02232ab8[4] = {
    data_ov003_02233090, data_ov003_022330ac, data_ov003_022330c8, data_ov003_022330e4,
};
extern "C" const TreeLeafFxParams **data_ov003_02232a38[4] = {
    0, data_ov003_022326f0, data_ov003_02232678, data_ov003_022327d0,
};
extern "C" char *data_ov003_02232940[3] = {
    data_ov003_02233ca0, data_ov003_02233cc4, data_ov003_02233ce8,
};
extern "C" char *data_ov003_02232d0c[6] = {
    data_ov003_02233d78, data_ov003_02233bec, data_ov003_02233700, data_ov003_02233724,
    data_ov003_02233748, data_ov003_02233d9c,
};
extern "C" char data_ov003_02233ba4[0x24] = "/fg/tree/palm_tex/palm_03w.nsbtx";
extern "C" const TreeLeafFxParams *data_ov003_022327c0[2] = {
    (const TreeLeafFxParams *)data_ov003_0222f2a4, (const TreeLeafFxParams *)data_ov003_0222f3c4,
};
extern "C" char data_ov003_022334e0[0x20] = "/fg/eff_tree/shakeL_02_1.nsbca";
extern "C" char data_ov003_02233c34[0x24] = "/fg/tree/palm_tex/palm_02s.nsbtx";
extern "C" char *data_ov003_02232720[2] = {
    data_ov003_02232690, data_ov003_0223285c,
};
extern "C" const u32 data_ov003_0222f100[3] = {
    2, 0xb0001, 0x266,
};
extern "C" const u32 data_ov003_0222f484[3] = {
    3, 0x190001, 0x59a,
};
extern "C" const TreeLeafFxParams **data_ov003_02232a68[4] = {
    data_ov003_02232708, data_ov003_02232730, data_ov003_02232780, data_ov003_02232750,
};
extern "C" const TreeLeafFxParams *data_ov003_02232758[2] = {
    (const TreeLeafFxParams *)data_ov003_0222f1b4, (const TreeLeafFxParams *)data_ov003_0222f388,
};
extern "C" const u32 data_ov003_0222f040[3] = {
    3, 0xc0001, 0x59a,
};
extern "C" char data_ov003_02233ca0[0x24] = "/fg/tree/tree_pal/tree_setA.nsbtx";
extern "C" char data_ov003_022343c8[0x28] = "/fg/tree/cedar_tex/cedar_babys.nsbtx";
extern "C" const u32 data_ov003_0222f124[3] = {
    2, 0x160001, 0x571,
};
extern "C" const u32 data_ov003_0222f088[3] = {
    5, 0x230001, 0x6e1,
};
extern "C" char data_ov003_02233074[0x1c] = "/fg/eff_tree/anm_t04w.nsbmd";
extern "C" const u32 data_ov003_0222f19c[3] = {
    2, 0x140001, 0x4cd,
};
extern "C" const u32 data_ov003_0222f094[3] = {
    4, 0x230001, 0x63d,
};
extern "C" char *data_ov003_02232d54[6] = {
    data_ov003_02234328, data_ov003_02233e98, data_ov003_02233ebc, data_ov003_02233ee0,
    data_ov003_02233f04, data_ov003_02234350,
};
extern "C" char **data_ov003_022328ec[3] = {
    data_ov003_02232c14, data_ov003_02232bec, data_ov003_02232bec,
};
extern "C" char data_ov003_022330ac[0x1c] = "/fg/eff_tree/anm_t02f.nsbmd";
extern "C" char data_ov003_02233d54[0x24] = "/fg/tree/tree_mdl/tree_deadw.nsbmd";
extern "C" char data_ov003_022330e4[0x1c] = "/fg/eff_tree/anm_t04f.nsbmd";
extern "C" const u32 data_ov003_0222f0dc[3] = {
    0x1a, 0x250008, 0x15c,
};
extern "C" char data_ov003_02233d9c[0x24] = "/fg/tree/tree_mdl/tree_deadf.nsbmd";
extern "C" const u32 data_ov003_0222f1b4[3] = {
    0x12, 0x8000f, 0x52,
};
extern "C" const s32 sItemPopScaleXZ[17] = {
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0xc00, 0x1000, 0x1600, 0x1300,
    0x1000,
};
extern "C" char data_ov003_02234490[0x28] = "/fg/flower/pansy/pansy_palette.nsbtx";
extern "C" char data_ov003_02232820[0xc] = "obj_stnDs";
extern "C" char data_ov003_0223318c[0x1c] = "/fg/eff_tree/anm_c02w.nsbmd";
extern "C" const u32 data_ov003_0222f424[3] = {
    2, 0x140001, 0x51f,
};
extern "C" char *data_ov003_022326e8[2] = {
    data_ov003_02233604, data_ov003_022342b0,
};
extern "C" char data_ov003_022331a8[0x1c] = "/fg/eff_tree/anm_c03w.nsbmd";
extern "C" char data_ov003_022344b8[0x28] = "/fg/flower/cosmos/cosmos_palette.nsbtx";
extern "C" char data_ov003_02233ee0[0x24] = "/fg/tree/cedar_mdl/cedar_03s.nsbmd";
extern "C" char data_ov003_02233f4c[0x24] = "/fg/tree/palm_mdl/palm_deadw.nsbmd";
extern "C" const u32 data_ov003_0222f0c4[3] = {
    0x13, 0x250004, 0x2e1,
};
extern "C" const TreeLeafFxParams **data_ov003_02232b08[4] = {
    0, data_ov003_02232680, data_ov003_022326e0, data_ov003_02232710,
};
extern "C" const u32 data_ov003_0222f43c[3] = {
    5, 0xe0001, 0x6e1,
};
extern "C" char data_ov003_02233f94[0x24] = "/fg/tree/palm_mdl/palm_deads.nsbmd";
extern "C" const u32 data_ov003_0222f34c[3] = {
    8, 0xc0005, 0x15c,
};
extern "C" TreeLeafFxCbs data_ov003_02232b48 = {{
    {TreeLeafFx_OnEffectInit, TreeLeafFx_OnEffectStep},
    {TreeLeafFx_OnEffectInit, TreeLeafFx_OnEffectUpdate},
}};
extern "C" char data_ov003_02234000[0x24] = "/fg/tree/tree_tex/tree_babyf.nsbtx";
extern "C" const TreeLeafFxParams *data_ov003_022327d0[2] = {
    (const TreeLeafFxParams *)data_ov003_0222f1c0, (const TreeLeafFxParams *)data_ov003_0222f0c4,
};
extern "C" char *data_ov003_02232778[2] = {
    data_ov003_022335e0, data_ov003_02234288,
};
extern "C" char data_ov003_0223406c[0x24] = "/fg/tree/tree_tex/tree_deads.nsbtx";
extern "C" char *data_ov003_02232e14[6] = {
    data_ov003_022341b0, data_ov003_02233b5c, data_ov003_02233b80, data_ov003_02233ba4,
    data_ov003_02233bc8, data_ov003_022341d4,
};
extern "C" const TreeLeafFxParams *data_ov003_02232818[2] = {
    (const TreeLeafFxParams *)data_ov003_0222f340, (const TreeLeafFxParams *)data_ov003_0222f334,
};
extern "C" const u32 data_ov003_0222f758[46] = {
    0x13, 0x16, 0x14, 0x16, 0x15, 0x16, 0, 0x16, 1, 0x16, 2, 0x16, 3, 0x16, 4, 0x16, 5, 0x16, 6, 0x16, 7, 0x16, 8,
    0x16, 9, 0x16, 0xa, 0x16, 0xb, 0x16, 0xc, 0x16, 0xd, 0x16, 0xe, 0x16, 0xf, 0x16, 0x10, 0x16, 0x11, 0x16, 0x12,
    0x16, 0x13, 0x16,
};
extern "C" const u32 data_ov003_0222f2bc[3] = {
    3, 0x250001, 0x2cd,
};
extern "C" const TreeLeafFxParams *data_ov003_02232810[2] = {
    (const TreeLeafFxParams *)data_ov003_0222f1a8, 0,
};
extern "C" char data_ov003_022332a4[0x1c] = "/fg/eff_tree/anm_p01s.nsbmd";
extern "C" const TreeLeafFxParams **data_ov003_02232b68[4] = {
    data_ov003_022326b0, data_ov003_02232700, data_ov003_022327f0, data_ov003_022327f8,
};
extern "C" char *data_ov003_02232e2c[6] = {
    data_ov003_022341f8, data_ov003_02233c10, data_ov003_02233c34, data_ov003_02233c58,
    data_ov003_02233c7c, data_ov003_0223421c,
};
extern "C" alignas(4) const u8 data_ov003_0222f4c8[24] = {
    0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00,
};
extern "C" const TreeLeafFxParams **data_ov003_02232b98[4] = {
    data_ov003_022327a0, data_ov003_022327c8, data_ov003_02232728, data_ov003_022326b8,
};
Heap *sFieldObjectAnimHeap;
extern "C" char data_ov003_02234144[0x24] = "/fg/tree/cedar_tex/cedar_02s.nsbtx";
extern "C" char *data_ov003_022328b0[3] = {
    data_ov003_02232e44, data_ov003_02232e60, data_ov003_02232e60,
};
extern "C" const TreeLeafFxParams **data_ov003_02232bb8[4] = {
    0, data_ov003_022326c8, data_ov003_02232748, data_ov003_02232760,
};
extern "C" char data_ov003_022335a0[0x20] = "/fg/flower/cosmos/cosmos.nsbtx";
extern "C" const u32 data_ov003_0222f3d0[3] = {
    1, 0x60001, 0x8f,
};
extern "C" char *data_ov003_02232bc8[4] = {
    data_ov003_02233440, data_ov003_02233460, data_ov003_022335a0, data_ov003_02232fb0,
};
extern "C" const TreeLeafFxParams *data_ov003_022326b8[2] = {
    (const TreeLeafFxParams *)data_ov003_0222f370, (const TreeLeafFxParams *)data_ov003_0222f0dc,
};
extern "C" char data_ov003_022341b0[0x24] = "/fg/tree/palm_tex/palm_babyw.nsbtx";
extern "C" char data_ov003_022341d4[0x24] = "/fg/tree/palm_tex/palm_deadw.nsbtx";
extern "C" const TreeLeafFxParams ***data_ov003_02232c50[5] = {
    data_ov003_02232b88, data_ov003_02232ae8, data_ov003_02232a78, data_ov003_02232b08,
    data_ov003_02232b68,
};
extern "C" char data_ov003_02234580[0x28] = "/fg/tree/cedar_pal/obj_x_cedar_pl.nsbtx";
extern "C" const u32 data_ov003_0222f460[3] = {
    2, 0x210001, 0x200,
};
extern "C" const VecFx32 data_ov003_0222f510[3] = {
    {-0x1100, 0x2800, -0x100},
    {0x1100, 0x3000, -0x800},
    {-0x300, 0x3e00, -0xf00},
};
extern "C" char *data_ov003_02232bd8[5] = {
    data_ov003_02232c64, data_ov003_02232c7c, data_ov003_02232c94, data_ov003_02232cac,
    data_ov003_02232cc4,
};
extern "C" char data_ov003_02232958[0x10] = "palm01_stamp";
extern "C" char data_ov003_02232968[0x10] = "tree04_stamp";
extern "C" char data_ov003_022342b0[0x28] = "/fg/flower/rafflesia/rafflesia.nsbmd";
extern "C" char data_ov003_0223364c[0x24] = "/fg/tree/tree_mdl/tree_01w.nsbmd";
extern "C" char data_ov003_02233670[0x24] = "/fg/tree/tree_mdl/tree_02w.nsbmd";
extern "C" const u32 data_ov003_0222f190[3] = {
    5, 0x280001, 0x852,
};
extern "C" VecFx32 *data_ov003_0223291c[3] = {
    (VecFx32 *)data_ov003_0222f510, (VecFx32 *)data_ov003_0222f4e0, (VecFx32 *)data_ov003_0222f4f8,
};
extern "C" const u32 data_ov003_0222f6a0[23] = {
    9, 9, 9, 0, 1, 1, 1, 2, 3, 4, 4, 4, 5, 5, 5, 6, 6, 6, 7, 7, 8, 8, 9,
};
extern "C" const TreeLeafFxParams *data_ov003_022327a8[2] = {
    (const TreeLeafFxParams *)data_ov003_0222f094, (const TreeLeafFxParams *)data_ov003_0222f0a0,
};
extern "C" char data_ov003_02232e98[0x1c] = "/fg/grass/redTurnip.nsbmd";
extern "C" const u32 data_ov003_0222f058[3] = {
    3, 0xc0001, 0x614,
};
extern "C" const u32 data_ov003_0222f070[3] = {
    3, 0x1e0001, 0x614,
};
extern "C" char data_ov003_02232838[0xc] = "obj_stnCs";
extern "C" char data_ov003_02232eec[0x1c] = "/fg/hole/obj_hole_pl.nsbtx";
extern "C" char **data_ov003_022328a4[3] = {
    data_ov003_02232d6c, data_ov003_02232d84, data_ov003_02232d84,
};
extern "C" TreeLeafFxCbs data_ov003_02232b78 = {{
    {TreeLeafFx_OnEffectInit, TreeLeafFx_OnEffectStep},
    {TreeLeafFx_OnEffectInit, TreeLeafFx_OnEffectUpdate},
}};
extern "C" char *data_ov003_02232c14[5] = {
    data_ov003_02232880, data_ov003_02232844, data_ov003_0223288c, data_ov003_02232874,
    data_ov003_0223282c,
};
extern "C" const u32 data_ov003_0222f454[3] = {
    0x13, 0xe0004, 0x214,
};
extern "C" const TreeLeafFxParams *data_ov003_022326a0[2] = {
    (const TreeLeafFxParams *)data_ov003_0222f0f4, (const TreeLeafFxParams *)data_ov003_0222f160,
};
extern "C" char data_ov003_02233964[0x24] = "/fg/eff_tree/cutR_03_1unit.nsbca";
extern "C" const u32 data_ov003_0222f3f4[3] = {
    0x12, 0x7001b, 0x8f,
};
extern "C" char data_ov003_022339f4[0x24] = "/fg/tree/tree_tex/tree_04w.nsbtx";
extern "C" char **data_ov003_022328f8[3] = {
    data_ov003_02232b18, data_ov003_02232af8, data_ov003_02232af8,
};
extern "C" const u32 data_ov003_0222f274[3] = {
    0x12, 0xa000a, 0x8f,
};
extern "C" const u32 data_ov003_0222f0b8[3] = {
    5, 0x1e0001, 0x733,
};
extern "C" char *data_ov003_02232af8[4] = {
    data_ov003_022331e0, data_ov003_022331fc, data_ov003_02233218, data_ov003_02233234,
};
extern "C" char data_ov003_02233b38[0x24] = "/fg/eff_tree/cutR_04_1unit.nsbca";
extern "C" char *data_ov003_02232cf4[6] = {
    data_ov003_02233d30, data_ov003_0223364c, data_ov003_02233670, data_ov003_02233694,
    data_ov003_022336b8, data_ov003_02233d54,
};
extern "C" char data_ov003_022330c8[0x1c] = "/fg/eff_tree/anm_t03f.nsbmd";
extern "C" const u32 data_ov003_0222f184[3] = {
    0x12, 0xe0008, 0x15c,
};
extern "C" char data_ov003_02232fb0[0x1c] = "/fg/flower/rose/rose.nsbtx";
extern "C" char data_ov003_02232fcc[0x1c] = "/fg/eff_tree/cutR_03.nsbca";
extern "C" char data_ov003_02232fe8[0x1c] = "/fg/eff_tree/anm_t01s.nsbmd";
extern "C" char *data_ov003_02232d24[6] = {
    data_ov003_02233dc0, data_ov003_0223376c, data_ov003_02233790, data_ov003_022337b4,
    data_ov003_022337d8, data_ov003_02233de4,
};
extern "C" const u32 data_ov003_0222f064[3] = {
    2, 0xc0001, 0x4cd,
};
extern "C" char data_ov003_02233cc4[0x24] = "/fg/tree/tree_pal/tree_setB.nsbtx";
extern "C" const u32 data_ov003_0222f3dc[3] = {
    0x13, 0x280004, 0x2e1,
};
extern "C" const u32 data_ov003_0222f1f0[3] = {
    0x13, 0xa000a, 0x1ec,
};
extern "C" char *data_ov003_02232d6c[6] = {
    data_ov003_02233f28, data_ov003_02233844, data_ov003_02233868, data_ov003_0223388c,
    data_ov003_022338b0, data_ov003_02233f4c,
};
extern "C" const u32 data_ov003_0222f0f4[3] = {
    2, 0xa0001, 0x171,
};
extern "C" char data_ov003_02233dc0[0x24] = "/fg/tree/tree_mdl/tree_babys.nsbmd";
extern "C" char data_ov003_02233154[0x1c] = "/fg/eff_tree/anm_p02w.nsbmd";
extern "C" char data_ov003_02233e08[0x24] = "/fg/tree/cedar_mdl/cedar_01w.nsbmd";
extern "C" const u32 data_ov003_0222f1fc[3] = {
    0x13, 0x14000a, 0x28f,
};
extern "C" const TreeLeafFxParams *data_ov003_022327e8[2] = {
    (const TreeLeafFxParams *)data_ov003_0222f3d0, 0,
};
extern "C" const u32 sFieldObjShapeTable[197] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x6101000, 0, 0, 0, 0, 0, 0, 0, 0x600, 0x6201006,
    0x10062010, 0x20100620, 0x6101006, 0x10061010, 0x10100610, 0x6000006, 0x10062010, 0x20100620, 0x6201006,
    0x10062010, 0x20100620, 0x6000006, 0x10062010, 0x20100620, 0x6201006, 0x10062010, 0x20100620, 0x6000006,
    0x10062010, 0x20100620, 0x6201006, 0x10062010, 0x20100620, 0x6000006, 0x10062010, 0x20100620, 0x6201006,
    0x10062010, 0x20100620, 0x6000006, 0x10062010, 0x20100620, 0x6201006, 0x10062010, 0x20100620, 0x6000006,
    0x10062010, 0x20100620, 0x6201006, 0x60000, 0x20100600, 0x6201006, 0x10062010, 0x10100620, 0x6101006, 0x10061010,
    0x20100610, 0x6201006, 0x10062010, 0x20100620, 0x6201006, 0x10062010, 0x620, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0x10000000, 0x610, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x6000000, 0x10060000, 0x20100620, 0x6201006,
    0x10062010, 0x20100620, 0x6201006, 0x10061010, 0x10100610, 0x6101006, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x4101000,
    0x10041010, 0x10100410, 0x4101004, 0x10041010, 0x10100410, 0x4101004, 0x10041010, 0x10100410, 0x4101004,
    0x10041010, 0x10100410, 0x4101004, 0x10041010, 0x10100410, 0x4101004, 0x10041010, 0x10100410, 0x4101004,
    0x10020410, 0x204, 0x6101000, 0x10061010, 0x10100610, 6, 4, 4,
};
extern "C" char *data_ov003_02232de4[6] = {
    data_ov003_02234378, data_ov003_02234090, data_ov003_022340b4, data_ov003_022340d8,
    data_ov003_022340fc, data_ov003_022343a0,
};
extern "C" const TreeLeafFxParams **data_ov003_02232b38[4] = {
    0, data_ov003_022326a0, data_ov003_02232698, data_ov003_022327c0,
};
extern "C" char data_ov003_02233fdc[0x24] = "/fg/tree/tree_tex/tree_deadw.nsbtx";
extern "C" char data_ov003_022331fc[0x1c] = "/fg/eff_tree/anm_c02s.nsbmd";
extern "C" char data_ov003_02234090[0x24] = "/fg/tree/cedar_tex/cedar_01w.nsbtx";
extern "C" const TreeLeafFxParams *data_ov003_02232790[2] = {
    (const TreeLeafFxParams *)data_ov003_0222f3ac, (const TreeLeafFxParams *)data_ov003_0222f13c,
};
extern "C" const TreeLeafFxParams *data_ov003_02232780[2] = {
    (const TreeLeafFxParams *)data_ov003_0222f07c, (const TreeLeafFxParams *)data_ov003_0222f49c,
};
extern "C" const s32 sItemPopScaleY[17] = {
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x1600, 0x1100, 0xc00, 0xe00,
    0x1000,
};
extern "C" char data_ov003_02233580[0x20] = "/fg/eff_tree/shakeL_02_2.nsbca";
extern "C" const TreeLeafFxParams *data_ov003_022327a0[2] = {
    (const TreeLeafFxParams *)data_ov003_0222f2ec, (const TreeLeafFxParams *)data_ov003_0222f2e0,
};
extern "C" const u32 data_ov003_0222f370[3] = {
    0x12, 0xe0008, 0x15c,
};
extern "C" const u32 data_ov003_0222f0e8[3] = {
    8, 0xa0005, 0xe1,
};
extern "C" char data_ov003_02233314[0x1c] = "/fg/eff_tree/anm_t03w.nsbmd";
extern "C" const u32 data_ov003_0222f400[3] = {
    0x13, 0xa000a, 0x28f,
};
extern "C" char data_ov003_0223334c[0x1c] = "/fg/eff_tree/shake_02.nsbca";
extern "C" const u32 data_ov003_0222f448[3] = {
    0x1a, 0x1e000a, 0xe1,
};
extern "C" const FieldObjShapeCategory data_ov003_0222f4b8[2] = {
    {(const u8 *)sFieldObjShapeTable, 0, 0, 0x103},
    {NULL, 0, 0, 0},
};
extern "C" const u32 data_ov003_0222f16c[3] = {
    3, 0xc0001, 0x59a,
};
extern "C" char data_ov003_02232880[0xc] = "obj_stnAw";
extern "C" char data_ov003_02232844[0xc] = "obj_stnBw";
extern "C" char data_ov003_022336dc[0x24] = "/fg/eff_tree/cutL_03_1unit.nsbca";
extern "C" const TreeLeafFxParams *data_ov003_022326f0[2] = {
    (const TreeLeafFxParams *)data_ov003_0222f46c, (const TreeLeafFxParams *)data_ov003_0222f1fc,
};
extern "C" char data_ov003_0223376c[0x24] = "/fg/tree/tree_mdl/tree_01s.nsbmd";
extern "C" const u32 data_ov003_0222f310[3] = {
    0x1a, 0x14000f, 0xa4,
};
extern "C" char *data_ov003_022328c8[3] = {
    data_ov003_022344e0, data_ov003_02234508, data_ov003_02234530,
};
extern "C" char data_ov003_02233844[0x24] = "/fg/tree/palm_mdl/palm_01w.nsbmd";
extern "C" char data_ov003_022338f8[0x24] = "/fg/tree/palm_mdl/palm_02s.nsbmd";
extern "C" char data_ov003_022339d0[0x24] = "/fg/tree/tree_tex/tree_03w.nsbtx";
extern "C" const u32 data_ov003_0222f0ac[3] = {
    5, 0xe0001, 0x852,
};
extern "C" char **data_ov003_022328e0[3] = {
    data_ov003_02232cf4, data_ov003_02232d0c, data_ov003_02232d24,
};
extern "C" char data_ov003_02233b5c[0x24] = "/fg/tree/palm_tex/palm_01w.nsbtx";
extern "C" const TreeLeafFxParams *data_ov003_02232710[2] = {
    (const TreeLeafFxParams *)data_ov003_0222f214, (const TreeLeafFxParams *)data_ov003_0222f3dc,
};
extern "C" char data_ov003_02232690[0x8] = "tanpopo";
extern "C" char data_ov003_02233c58[0x24] = "/fg/tree/palm_tex/palm_03s.nsbtx";
extern "C" char data_ov003_02233058[0x1c] = "/fg/eff_tree/anm_t02w.nsbmd";
extern "C" const u32 data_ov003_0222f154[3] = {
    0x13, 0xb0006, 0x28f,
};
extern "C" char *data_ov003_02232644[1] = {
    data_ov003_02234558,
};
extern "C" char data_ov003_0223311c[0x1c] = "/fg/eff_tree/anm_t03s.nsbmd";
extern "C" char data_ov003_02233de4[0x24] = "/fg/tree/tree_mdl/tree_deads.nsbmd";
extern "C" const TreeLeafFxParams *data_ov003_02232678[2] = {
    (const TreeLeafFxParams *)data_ov003_0222f154, (const TreeLeafFxParams *)data_ov003_0222f490,
};
extern "C" const TreeLeafFxParams **data_ov003_02232ae8[4] = {
    0, data_ov003_02232790, data_ov003_02232660, data_ov003_022327b0,
};
extern "C" const u32 data_ov003_0222f478[3] = {
    4, 0x230001, 0x70a,
};
extern "C" const TreeLeafFxParams *data_ov003_02232698[2] = {
    (const TreeLeafFxParams *)data_ov003_0222f268, (const TreeLeafFxParams *)data_ov003_0222f208,
};
extern "C" const TreeLeafFxParams *data_ov003_02232700[2] = {
    (const TreeLeafFxParams *)data_ov003_0222f058, (const TreeLeafFxParams *)data_ov003_0222f070,
};
extern "C" const TreeLeafFxParams *data_ov003_02232730[2] = {
    (const TreeLeafFxParams *)data_ov003_0222f16c, (const TreeLeafFxParams *)data_ov003_0222f178,
};
extern "C" const TreeLeafFxParams *data_ov003_02232658[2] = {
    (const TreeLeafFxParams *)data_ov003_0222f3f4, (const TreeLeafFxParams *)data_ov003_0222f25c,
};
extern "C" const TreeLeafFxParams *data_ov003_022326c8[2] = {
    (const TreeLeafFxParams *)data_ov003_0222f220, (const TreeLeafFxParams *)data_ov003_0222f28c,
};
extern "C" char data_ov003_02233330[0x1c] = "/fg/eff_tree/shake_01.nsbca";
extern "C" char data_ov003_02234240[0x24] = "/fg/tree/cedar_pal/cedar_set.nsbtx";
extern "C" char data_ov003_022342d8[0x28] = "/fg/tree/cedar_mdl/cedar_babyw.nsbmd";
extern "C" char data_ov003_02233700[0x24] = "/fg/tree/tree_mdl/tree_02f.nsbmd";
extern "C" char data_ov003_022329a8[0x10] = "tree02_stamp";
extern "C" char data_ov003_02232c94[0x18] = "/fg/grass/grassC.nsbmd";
extern "C" char data_ov003_02233170[0x1c] = "/fg/eff_tree/anm_c01w.nsbmd";
extern "C" char data_ov003_022329e8[0x10] = "cedar01_stamp";
extern "C" const u32 data_ov003_0222f280[3] = {
    0x1a, 0xc001b, 0x29,
};
extern "C" char data_ov003_02233bc8[0x24] = "/fg/tree/palm_tex/palm_04w.nsbtx";
extern "C" const TreeLeafFxParams *data_ov003_02232748[2] = {
    (const TreeLeafFxParams *)data_ov003_0222f100, (const TreeLeafFxParams *)data_ov003_0222f3e8,
};
extern "C" char data_ov003_02233d78[0x24] = "/fg/tree/tree_mdl/tree_babyf.nsbmd";
extern "C" char data_ov003_0223282c[0xc] = "obj_stnEw";
extern "C" char data_ov003_022331c4[0x1c] = "/fg/eff_tree/anm_c04w.nsbmd";
extern "C" char *data_ov003_02232b58[4] = {
    data_ov003_022332a4, data_ov003_022332c0, data_ov003_022332dc, data_ov003_022332f8,
};
extern "C" char data_ov003_022340b4[0x24] = "/fg/tree/cedar_tex/cedar_02w.nsbtx";
extern "C" char data_ov003_02234168[0x24] = "/fg/tree/cedar_tex/cedar_03s.nsbtx";
extern "C" const TreeLeafFxParams *data_ov003_02232670[2] = {
    (const TreeLeafFxParams *)data_ov003_0222f364, 0,
};
extern "C" char data_ov003_022333a0[0x20] = "/fg/eff_tree/shakeS_02.nsbca";
extern "C" const TreeLeafFxParams *data_ov003_022327e0[2] = {
    (const TreeLeafFxParams *)data_ov003_0222f0e8, 0,
};
extern "C" char data_ov003_02234378[0x28] = "/fg/tree/cedar_tex/cedar_babyw.nsbtx";
extern "C" char data_ov003_022334c0[0x20] = "/fg/flower/cosmos/cosmos.nsbmd";
extern "C" const u32 data_ov003_0222f334[3] = {
    0x13, 0x190006, 0x1ec,
};
extern "C" const TreeLeafFxParams **data_ov003_02232ad8[4] = {
    0, data_ov003_022327e8, data_ov003_02232670, data_ov003_022327b8,
};
extern "C" const u32 data_ov003_0222f810[23][3] = {
    {0x0, 0x0, 0x0},
    {0x0, 0x0, 0x0},
    {0x0, 0x0, 0x0},
    {0x2, 0x2, 0x2},
    {0x1, 0x2, 0x2},
    {0x1, 0x1, 0x2},
    {0x2, 0x2, 0x2},
    {0x2, 0x2, 0x2},
    {0x2, 0x2, 0x2},
    {0x2, 0x2, 0x2},
    {0x2, 0x2, 0x2},
    {0x2, 0x2, 0x2},
    {0x2, 0x2, 0x2},
    {0x2, 0x2, 0x2},
    {0x2, 0x2, 0x2},
    {0x2, 0x2, 0x2},
    {0x2, 0x2, 0x2},
    {0x2, 0x2, 0x2},
    {0x2, 0x2, 0x2},
    {0x2, 0x2, 0x2},
    {0x2, 0x2, 0x2},
    {0x2, 0x2, 0x2},
    {0x0, 0x0, 0x0},
};
extern "C" const TreeLeafFxParams **data_ov003_02232ba8[4] = {
    0, data_ov003_02232770, data_ov003_02232818, data_ov003_022326d0,
};
extern "C" char *data_ov003_02234604[12] = {
    data_ov003_022329c8, data_ov003_022329a8, data_ov003_022329b8, data_ov003_02232968,
    data_ov003_022329e8, data_ov003_022329f8, data_ov003_022329d8, data_ov003_02232a08,
    data_ov003_02232958, data_ov003_02232978, data_ov003_02232988, data_ov003_02232998,
};
extern "C" char data_ov003_02233a3c[0x24] = "/fg/tree/tree_tex/tree_02f.nsbtx";
extern "C" char *data_ov003_02232640[1] = {
    data_ov003_02234418,
};
extern "C" const TreeLeafFxParams *data_ov003_02232750[2] = {
    (const TreeLeafFxParams *)data_ov003_0222f130, (const TreeLeafFxParams *)data_ov003_0222f0b8,
};
extern "C" char data_ov003_022333e0[0x20] = "/fg/flower/pansy/pansy.nsbmd";
// vtable at 0x2234634 (compiler generated)
// vtable at 0x2232c00 (compiler generated)
// the functions defined in this file, with their definitions' types (the bodies below are in descending address order)
extern "C" void * FieldObjectManager_Create();
extern "C" const u8 * FieldObj_GetShapeRecord(u16 * p);
extern "C" s32 FieldObj_LoadTextureFiles(void * self, void ** a, void ** b, char ** names, s32 n);
extern "C" s32 FieldObj_LoadPaletteFiles(void * self, void ** a, void ** b, char ** names, s32 n);
extern "C" s32 FieldObj_LoadIconModels(FieldObjectManager * self);
extern "C" s32 FieldObj_LoadStoneModels(FieldObjectManager * self);
extern "C" s32 FieldObj_LoadStones();
extern "C" s32 FieldObj_LoadStumpModels(FieldObjectManager * self);
extern "C" s32 FieldObj_LoadStumps();
extern "C" s32 FieldObj_GetSeasonSet(void * self, s32 i);
extern "C" void * FieldObj_GetTreeModelPath(void * self, s32 a, s32 i);
extern "C" void * FieldObj_GetTreeAnimModelPath(void * self, s32 a, s32 i);
extern "C" void * FieldObj_GetCedarModelPath(FieldObjectManager * self, s32 i);
extern "C" void * FieldObj_GetCedarAnimModelPath(FieldObjectManager * self, s32 i);
extern "C" void * FieldObj_GetPalmModelPath(FieldObjectManager * self, s32 i);
extern "C" void * FieldObj_GetPalmAnimModelPath(void * self, s32 i);
extern "C" char ** FieldObj_GetTreeTexturePaths(void * self, s32 i);
extern "C" s32 FieldObj_LoadTreeTextures(void * self, void ** a, void ** b);
extern "C" s32 FieldObj_LoadCedarTextures(void * a, void ** b, void ** c);
extern "C" s32 FieldObj_LoadLitCedarTexture(void * a, void ** b, void ** c);
extern "C" s32 FieldObj_LoadPalmTextures(void * a, void ** b, void ** c);
extern "C" s32 FieldObj_LoadTreePalettes(void * a, void ** b, void ** c, void ** d, void ** e);
extern "C" s32 FieldObj_LoadCedarPalettes(void * a, void ** b, void ** c, void ** d, void ** e);
extern "C" s32 FieldObj_LoadLitCedarPalette(void * a, void ** b, void ** c);
extern "C" s32 FieldObj_LoadTreeSaplingModels(FieldObjectManager * self, void ** a, void ** b, u32 * c);
extern "C" s32 FieldObj_LoadCedarSaplingModel(FieldObjectManager * self, void ** a, void ** b, u32 * c);
extern "C" s32 FieldObj_LoadPalmSaplingModel(FieldObjectManager * self, void ** a, void ** b, u32 * c);
extern "C" s32 FieldObj_LoadTreeStageModels(FieldObjectManager * o, void ** a, void ** b, u32 * c);
extern "C" s32 FieldObj_LoadCedarStageModels(FieldObjectManager * o, void ** a, void ** b, u32 * c);
extern "C" s32 FieldObj_LoadLitCedarModels(FieldObjectManager * o, void ** a, void ** b);
extern "C" s32 FieldObj_LoadPalmStageModels(FieldObjectManager * o, void ** a, void ** b, u32 * c);
extern "C" s32 FieldObj_LoadTreeDeadModels(FieldObjectManager * o, void ** a, void ** b, u32 * c);
extern "C" s32 FieldObj_LoadCedarDeadModel(FieldObjectManager * o, void ** a, void ** b, u32 * c);
extern "C" s32 FieldObj_LoadPalmDeadModel(FieldObjectManager * o, void ** a, void ** b, u32 * c);
extern "C" void FieldObj_ClearTreeFileBufs(FieldObjectManager * o, void ** a, void ** b, void ** c, void ** d, void ** e, void ** f);
extern "C" void FieldObj_ClearCedarPalmFileBufs(FieldObjectManager * o, void ** a, void ** b, void ** c, void ** d, void ** e, void ** f);
extern "C" void FieldObj_ClearLitCedarFileBufs(FieldObjectManager * o, void ** a, void ** b, void ** c, void ** d);
extern "C" void FieldObj_FreeTreeFileBufs(FieldObjectManager * o, void ** a, void ** b, void ** c);
extern "C" void FieldObj_FreeCedarFileBufs(FieldObjectManager * o, void ** a, void ** b, void ** c);
extern "C" void FieldObj_FreeLitCedarFileBufs(FieldObjectManager * o, void ** a, void ** b);
extern "C" void FieldObj_FreePalmFileBufs(FieldObjectManager * o, void ** a, void ** b, void ** c);
extern "C" s32 FieldObj_LoadTrees();
extern "C" s32 FieldObj_LoadFlowerTextures(FieldObjectManager * o, void ** a, void ** b);
extern "C" s32 FieldObj_LoadFlowerPalettes(FieldObjectManager * o, void ** a, void ** b);
extern "C" s32 FieldObj_LoadFlowerModels(FieldObjectManager * o, void ** a, void ** b);
extern "C" s32 FieldObj_LoadSpecialFlowerTextures(FieldObjectManager * o, void ** a, void ** b);
extern "C" s32 FieldObj_LoadSpecialFlowerPalettes(FieldObjectManager * o, void ** a, void ** b);
extern "C" s32 FieldObj_LoadSpecialFlowerModels(FieldObjectManager * o, void ** a, void ** b);
extern "C" s32 FieldObj_LoadDandelionModels(FieldObjectManager * o);
extern "C" void FieldObj_FreeFlowerFiles(FieldObjectManager * o, void ** b, void ** d, void ** f, void ** h);
extern "C" s32 FieldObj_LoadFlowers();
extern "C" s32 FieldObj_LoadGrassPalette(FieldObjectManager * o, void ** a, void ** b);
extern "C" s32 FieldObj_LoadGrassModels(FieldObjectManager * o, void * pltt);
extern "C" s32 FieldObj_LoadGrass();
extern "C" u32 FieldObj_GetGroundTexSet();
extern "C" s32 FieldObj_LoadHolePalette(FieldObjectManager * o, void ** a, void ** b);
extern "C" s32 FieldObj_LoadHoleModels(FieldObjectManager * o, void * pltt);
extern "C" s32 FieldObj_LoadHoles();
extern "C" s32 FieldObj_LoadCrackPalette(FieldObjectManager * o, void ** a, void ** b);
extern "C" s32 FieldObj_LoadCrackModels(FieldObjectManager * o, void * pltt);
extern "C" s32 FieldObj_LoadCracks();
extern "C" s32 FieldObj_LoadTurnipModels(FieldObjectManager * o);
extern "C" s32 FieldObj_LoadTurnips();
extern "C" s32 FieldObj_LoadDesignModels(FieldObjectManager * o);
extern "C" s32 FieldObj_LoadDesigns();
extern "C" s32 FieldObj_RunLoaders(FieldObjectManager * o);
extern "C" CachedModel * FieldObj_DrawIconModel(FieldObjectManager * o, u32 idx, VecFx32 * a, s32 b, VecFx32 * c, s32 d, s32 e, s32 f);
extern "C" CachedModel * FieldObj_DrawItemIcon(FieldObjectManager * o, u32 t, VecFx32 * a, s32 b, VecFx32 * c, s32 d, s32 e, s32 f);
extern "C" void FieldObj_DrawItemModel(FieldObjectManager * o, u32 t, VecFx32 * pos, VecFx32 * scale, s32 rx, s32 ry, s32 rz);
extern "C" void FieldObj_DrawTreeFruit(FieldObjectManager * o, u16 * t, VecFx32 * v);
extern "C" void FieldObj_DrawCedarLights(FieldObjectManager * o, VecFx32 * t, s32 a, s32 b);
extern "C" CachedModel * FieldObj_DrawTree(FieldObjectManager * o, u16 * t, s32 a, s32 b, VecFx32Copy v, Mtx43 m);
extern "C" void FieldObj_DrawTreeAt(FieldObjectManager * o, u16 * t, s32 a, s32 b, VecFx32Copy v, Mtx43 m);
extern "C" CachedModel * FieldObj_DrawSapling(FieldObjectManager * o, u16 * t, s32 a, s32 b, Mtx43 m);
extern "C" CachedModel * FieldObj_DrawStump(FieldObjectManager * o, u16 * t, Mtx43 m);
extern "C" CachedModel * FieldObj_DrawFlower(FieldObjectManager * o, u16 * t, Mtx43 m);
extern "C" CachedModel * FieldObj_DrawFlowerBySpecies(FieldObjectManager * o, u16 * t, Mtx43 m);
extern "C" CachedModel * FieldObj_DrawSpecialFlower(FieldObjectManager * o, u16 * t, Mtx43 m);
extern "C" CachedModel * FieldObj_DrawGrass(FieldObjectManager * o, u16 * t, Mtx43 m);
extern "C" CachedModel * FieldObj_DrawRock(FieldObjectManager * o, u16 * t, Mtx43 m);
extern "C" void FieldObj_DrawRockAt(FieldObjectManager * o, u16 * t, VecFx32 * pos, Mtx43 m);
extern "C" CachedModel * FieldObj_DrawHole(FieldObjectManager * o, u16 * t, Mtx43 m);
extern "C" void FieldObj_DrawCrack(FieldObjectManager * o, u16 * t, Mtx43 m);
extern "C" CachedModel * FieldObj_DrawTurnip(FieldObjectManager * o, u16 * t, Mtx43 m);
extern "C" CachedModel * FieldObj_DrawDesign(FieldObjectManager * o, u16 * t, Mtx43 m);
extern "C" void FieldObj_DrawModel(FieldObjectManager * o, CachedModel * p, Mtx43 m);
extern "C" void FieldObj_DrawUnits(FieldObjectManager * o, BlockMap * g);
extern "C" s32 Field_FindFlowerNear(VecFx32Copy * out, VecFx32Copy pos, s32 mask);
extern "C" s32 Field_IsRafflesiaNear(VecFx32Copy * out, VecFx32 * b);
extern "C" void Field_DrawIconModel(u32 item, VecFx32Copy a, VecFx32Copy b, s32 c, s16 d, s16 e);
extern "C" void Field_DrawItemIcon(u32 item, VecFx32Copy a, VecFx32Copy b, s32 c, s16 d, s16 e);
extern "C" void Field_DrawItemModel(u32 item, VecFx32Copy a, VecFx32Copy b, s32 c, s16 d, s16 e);
extern "C" s32 Tree_IsPendingStump(s32 o, FieldUnitPos pos);
extern "C" s32 Tree_BeginReaction(s32 o, FieldUnitPos pos);
extern "C" void Tree_RequestShake(s32 o, FieldUnitPos pos, s32 mode);
extern "C" void Tree_RequestChop(s32 o, FieldUnitPos pos, s32 mode);
extern "C" void FieldItemFx_StartStrikeResult(FieldUnitPos pos);
extern "C" void Field_SetUnitItem(s32 x, s32 y, u32 tile, s32 flag);
extern "C" void TreeLeafFx_Free(TreeLeafFxEntry * r);
extern "C" void TreeLeafFx_SetRecord(TreeLeafFxEntry * r, s32 a1, s32 a2, s32 a3, u32 c, VecFx32 * pos, s32 flag);
extern "C" const TreeLeafFxParams * TreeLeafFx_GetParams(TreeLeafFxEntry * r);
extern "C" void TreeLeafFx_ApplyParams(TreeLeafFxEntry * r, EffectSplEmitter * t);
extern "C" void TreeLeafFx_UpdatePos(TreeLeafFxEntry * r, EffectSplEmitter * t);
extern "C" void TreeLeafFx_OnEffectInit(EffectEmitterEntry * self);
extern "C" s32 TreeLeafFx_OnEffectStep(EffectEmitterEntry * self);
extern "C" s32 TreeLeafFx_OnEffectUpdate(EffectEmitterEntry * self);
extern "C" TreeLeafFxEntry * TreeLeafFx_FindFree(TreeLeafFx * self);
extern "C" void TreeLeafFx_GetTreeType(TreeLeafFx * self, TreeLeafFxEntry * r, s32 * o1, s32 * o2, VecFx32 * out, u16 * tile, VecXZ * pos);
extern "C" TreeLeafFxEntry * TreeLeafFx_SpawnSeasonal(TreeLeafFx * self, s32 p1, u32 p2, u16 * p3, VecXZ * p4, s32 p5);
extern "C" TreeLeafFxEntry * TreeLeafFx_SpawnLeaves(TreeLeafFx * self, s32 p1, u32 p2, u16 * p3, VecXZ * p4, s32 p5);
extern "C" void TreeLeafFx_Init(TreeLeafFx * self);
extern "C" TreeAnim * TreeAnim_Construct(TreeAnim * self);
extern "C" TreeAnimSet * TreeAnimSet_Construct(TreeAnimSet * self);
extern "C" TreeAnim * TreeAnim_Destruct(TreeAnim * self);
extern "C" void TreeAnimSet_Init(TreeAnimSet * self);
extern "C" void TreeAnimSet_Update(TreeAnimSet * self);
extern "C" void TreeAnimSet_Draw(TreeAnimSet * self);
extern "C" void TreeAnimSet_Release(TreeAnimSet * self);
extern "C" TreeAnim * TreeAnimSet_GetInstance(TreeAnimSet * self, u16 * p, s32 a, s32 b);
extern "C" void TreeAnimSet_ProcessRequest(TreeAnimSet * a, TreeAnimRequest * o);
extern "C" void TreeAnimSet_Request(TreeAnimSet * a, s32 id, s32 * pos, s32 c, s32 d);
extern "C" void Tree_KeepShaking(s32 id, s32 * pos);
extern "C" void TreeAnim_Init(TreeAnim * o);
extern "C" void TreeAnim_Update(TreeAnim * o);
extern "C" void TreeAnim_Draw(TreeAnim * o);
extern "C" void TreeAnim_Release(TreeAnim * o);
extern "C" void TreeAnim_Begin(TreeAnim * o, s32 flag);
extern "C" s32 Tree_GetDropSide(s32 aid, VecFx32 * p);
extern "C" void Tree_DropFruit(u16 * cell, s32 id, s32 * pos);
extern "C" void Tree_DropBeeHive(s32 aid, s32 * p);
extern "C" void Tree_DropSpecial(u16 * cell, s32 a, FieldUnitPos p);
extern "C" void Tree_DropItems(u16 * cell, s32 a, FieldUnitPos p);
extern "C" void TreeAnim_DropItems(TreeAnim * self, void * cell);
extern "C" void TreeAnim_Start(TreeAnim * self, s32 a, FieldUnitPos p, s32 kind, s32 idx, s32 last);
extern "C" void TreeAnim_Reset(TreeAnim * self);
extern "C" void TreeAnim_ModelCallback(NNSG3dRS * rs);
extern "C" void TreeAnimSet_LoadAnims(TreeAnimSet * set);
extern "C" void TreeAnimSet_FreeAnims(TreeAnimSet * set);
extern "C" char * TreeAnimSet_GetAnimPath(TreeAnimSet * self, u32 a, u32 b);
extern "C" void TreeAnimRequest_Resolve(TreeAnimRequest * self);
extern "C" void FieldItemFx_ReleasePending(FieldItemFx * self);
extern "C" void FieldItemFx_Init(FieldItemFx * self, s32 a, FieldUnitPos p, VecFx32Copy pos, s32 kind, u16 w, BlockMap * map, s16 y, s32 z);
extern "C" void FieldItemFx_Finish(FieldItemFx * self);
extern "C" void FieldItemFx_Clear(FieldItemFx * self);
extern "C" void FieldItemFx_InitToss(FieldItemFx * self, FieldUnitPos p, VecFx32Copy q);
extern "C" void FieldItemFx_UpdateToss(FieldItemFx * self);
extern "C" void FieldItemFx_InitDigHole(FieldItemFx * self, FieldUnitPos p, BlockMap * g, s32 flag);
extern "C" void FieldItemFx_UpdateHoleOpen(FieldItemFx * self);
extern "C" void FieldItemFx_InitPitfallHole(FieldItemFx * self, FieldUnitPos p);
extern "C" void FieldItemFx_InitFillHole(FieldItemFx * self, FieldUnitPos p, BlockMap * g);
extern "C" void FieldItemFx_UpdateHoleClose(FieldItemFx * self);
extern "C" void FieldItemFx_InitPop(FieldItemFx * self, FieldUnitPos * p);
extern "C" void FieldItemFx_UpdatePop(FieldItemFx * self);
extern "C" void FieldItemFx_InitTreeDrop(FieldItemFx * self, FieldUnitPos * p, VecFx32 * q, s32 skip);
extern "C" void FieldItemFx_UpdateTreeDrop(FieldItemFx * self);
extern "C" void FieldItemFx_InitTreeDropFloat(FieldItemFx * self, FieldUnitPos * p, VecFx32 * q);
extern "C" void FieldItemFx_UpdateTreeDropFloat(FieldItemFx * self);
extern "C" void FieldItemFx_InitBeeHiveDrop(FieldItemFx * self, FieldUnitPos * unit, VecFx32 * p);
extern "C" void FieldItemFx_UpdateBeeHiveDrop(FieldItemFx * self);
extern "C" void FieldItemFx_InitDigUpTree(FieldItemFx * self, FieldUnitPos * p, u32 ang);
extern "C" void FieldItemFx_UpdateDigUpTree(FieldItemFx * self);
extern "C" void FieldItemFx_InitPlant(FieldItemFx * self, FieldUnitPos * p, VecFx32 * q, u32 w);
extern "C" void FieldItemFx_UpdatePlant(FieldItemFx * self);
extern "C" void FieldItemFx_InitHoleShrink(FieldItemFx * self, FieldUnitPos * p);
extern "C" void FieldItemFx_UpdateHoleShrink(FieldItemFx * self);
extern "C" void FieldItemFx_InitStrikeShake(FieldItemFx * self, FieldUnitPos * p);
extern "C" void FieldItemFx_UpdateStrikeShake(FieldItemFx * self);
extern "C" void FieldItemFx_SplashPosCallback(EffectModel * p);
extern "C" void FieldItemFx_InitBalloonDrop(FieldItemFx * self, VecFx32 * p);
extern "C" void FieldItemFx_UpdateBalloonDrop(FieldItemFx * self);
extern "C" void FieldItemFx_InitPitfallClose(FieldItemFx * e, FieldUnitPos * p, BlockMap * m);
extern "C" void FieldItemFxTable_Init(FieldItemFxTable * t);
extern "C" void FieldItemFxTable_Update(FieldItemFxTable * t);
extern "C" void FieldItemFxTable_Draw(FieldItemFx * e);
extern "C" void FieldItemFxTable_Release(FieldItemFxTable * t);
extern "C" FieldItemFx * FieldItemFxTable_FindFree(FieldItemFxTable * t, s32 g);
extern "C" s32 FieldItemFxTable_Start(FieldItemFxTable * t, s32 a, FieldUnitPos p, VecFx32Copy v, s32 f, u32 g, s32 h, s32 i);
extern "C" s32 FieldItemFxTable_StartForPlayer(FieldItemFxTable * t, s32 a, FieldUnitPos p, VecFx32Copy v, s32 f, u32 g, s32 h, s32 i);
extern "C" s32 FieldItemFx_CancelAt(s32 idx, FieldUnitPos * q);
extern "C" s32 FieldItemFx_StartDrop(s32 a, u32 b, FieldUnitPos p, VecFx32Copy v);
extern "C" s32 FieldItemFx_StartDigHole(s32 type, VecFx32 * v, s32 w);
extern "C" s32 FieldItemFx_StartPitfallHole(s32 type, VecFx32 * v);
extern "C" s32 FieldItemFx_StartFillHole(s32 type, VecFx32 * v);
extern "C" s32 FieldItemFx_StartFillHoleWithItem(s32 a, VecFx32 * b);
extern "C" s32 FieldItemFx_StartPop(s32 a, s32 b, FieldUnitPos c);
extern "C" void FieldItemFx_StartTreeDrop(s32 a, s32 v, FieldUnitPos p, VecFx32Copy q, s16 t, s32 x);
extern "C" void FieldItemFx_StartTreeDropFloat(s32 a, s32 v, FieldUnitPos p, VecFx32Copy q, s32 w);
extern "C" void FieldItemFx_StartBeeHiveDrop(s32 a, s32 v, FieldUnitPos p, VecFx32Copy q);
extern "C" s32 FieldItemFx_StartDigUpTree(s32 a, VecFx32 * b, s32 c);
extern "C" void FieldItemFx_StartPlant(s32 a, s32 b, FieldUnitPos c, VecFx32Copy d);
extern "C" s32 FieldItemFx_StartHoleShrink(s32 a, VecFx32 * b);
extern "C" void FieldItemFx_StartStrikeShake(s32 a, s32 b, FieldUnitPos c);
extern "C" s32 FieldItemFx_StartBalloonDrop(s32 a, s32 n);
extern "C" s32 FieldItemFx_FindLandingUnit(s32 * p);
extern "C" void FieldItemFx_StartStrikeEject(s32 a, s32 v, FieldUnitPos p, VecFx32Copy q);
extern "C" s32 FieldItemFx_StartPitfallClose(s32 a, VecFx32 * b);

// ================================================================ functions (descending address)
FieldItemFx::FieldItemFx()
{
    unit.x = 0;
    unit.z = 0;
}

FieldItemFx::~FieldItemFx() {}

FieldItemFxTable::~FieldItemFxTable() {}

namespace ns_0221f798 {
extern "C" {
void *FieldObjectManager_Create()
{
    return new FieldObjectManager;
}
}
}

namespace ns_0221f798 {
extern "C" {
const u8 *FieldObj_GetShapeRecord(u16 *p)
{
    const u8 *r = NULL;
    if (p != NULL) {
        s32 v = *p;
        s32 hi = v & 0xf000;
        s32 tag = hi >> 12;
        s32 lo = v & 0xfff;
        const FieldObjShapeCategory *e = data_ov003_0222f4b8;
        for (; e->shapes != NULL; e++) {
            if (tag == e->category) {
                if (e->shapeCount > lo) {
                    r = e->shapes + lo * 3;
                }
                break;
            }
        }
        if (r != NULL && *r == 0) {
            r = NULL;
        }
    }
    return r;
}
}
}

BOOL FieldObjectShapeQuery::getUnitShape(s32 *a, s32 *b, s32 *c, s32 x, s32 z) {
    using ns_0221f798::gSceneBlockMap;
    using ns_0221f798::FieldObj_GetShapeRecord;

    BlockMap *g = gSceneBlockMap;
    BOOL ok = FALSE;
    if (g != NULL) {
        s32 px = *(volatile s32 *)&x;
        s32 pz = *(volatile s32 *)&z;
        s32 hx = px >> 4;
        s32 hz = pz >> 4;
        const u8 *cell = FieldObj_GetShapeRecord((u16 *)BlockMap_GetItemPtr(g, hx, hz, px - (hx << 4), pz - (hz << 4), 0));
        if (cell != NULL && cell[2] != 0) {
            *a = ((s32)cell[0] << 12) >> 4;
            *b = ((s32)cell[1] << 12) >> 4;
            *c = cell[2];
            ok = TRUE;
        }
    }
    return ok;
}

namespace ns_0221f798 {
extern "C" {
BOOL FieldObj_LoadTextureFiles(void *self, void **a, void **b, char **names, s32 n)
{
    Heap *heap = gCurrentHeap;
    s32 i;
    for (i = 0; i < n; a++, b++, names++, i++) {
        if (*names != NULL) {
            *b = File_LoadAlloc(*names, heap, -4, 0);
            if (*b == NULL) {
                return FALSE;
            }
            *a = NNS_G3dGetTex(*b);
            Gfx3d_LoadTex(*a, 0);
        } else {
            *a = NULL;
        }
    }
    return TRUE;
}
}
}

namespace ns_0221f798 {
extern "C" {
BOOL FieldObj_LoadPaletteFiles(void *self, void **a, void **b, char **names, s32 n)
{
    Heap *heap = gCurrentHeap;
    s32 i;
    for (i = 0; i < n; a++, b++, names++, i++) {
        *b = File_LoadAlloc(*names, heap, -4, 0);
        if (*b == NULL) {
            return FALSE;
        }
        *a = NNS_G3dGetTex(*b);
        Gfx3d_LoadPltt(*a, 0);
    }
    return TRUE;
}
}
}

namespace ns_0221f798 {
extern "C" {
BOOL FieldObj_LoadIconModels(FieldObjectManager *self)
{
    BOOL ok = FALSE;
    if (ModelSet_Load(&self->iconModelSet, (char *)"/fg/icon/icon.nsbmd", sFieldObjectModelHeap)) {
        s32 i;
        for (i = 0; i < 0x49; i++) {
            self->iconModels[i] = (CachedModel *)ModelSet_Find(&self->iconModelSet, (void *)Item_GetIconModelName(i, 1));
        }
        ok = TRUE;
    }
    return ok;
}
}
}

namespace ns_0221f798 {
extern "C" {
BOOL FieldObj_LoadStoneModels(FieldObjectManager *self)
{
    BOOL ok = FALSE;
    s32 t = FieldObj_GetSeasonSet(self, 2);
    if (ModelSet_Load(&self->stoneModelSet, data_ov003_022328b0[t], sFieldObjectAnimHeap)) {
        char **p = data_ov003_022328ec[t];
        void *h = &self->stoneModelSet;
        s32 i;
        for (i = 0; i < 5; i++) {
            self->stoneModels[i] = (CachedModel *)ModelSet_Find(h, *p);
            p++;
        }
        ok = TRUE;
    }
    return ok;
}
}
}

namespace ns_0221f798 {
extern "C" {
BOOL FieldObj_LoadStones()
{
    return FieldObj_LoadStoneModels((FieldObjectManager *)gFieldObjectManager);
}
}
}

namespace ns_0221f798 {
extern "C" {
BOOL FieldObj_LoadStumpModels(FieldObjectManager *self)
{
    BOOL ok = FALSE;
    if (ModelSet_Load(&self->stumpModelSet, (char *)"/fg/tree/stamp/stamp.nsbmd", sFieldObjectModelHeap)) {
        s32 i;
        for (i = 0; i < 12; i++) {
            self->stumpModels[i] = (CachedModel *)ModelSet_Find(&self->stumpModelSet, data_ov003_02234604[i]);
        }
        ok = TRUE;
    }
    return ok;
}
}
}

namespace ns_0221f798 {
extern "C" {
BOOL FieldObj_LoadStumps()
{
    return FieldObj_LoadStumpModels((FieldObjectManager *)gFieldObjectManager);
}
}
}

namespace ns_0221f798 {
extern "C" {
s32 FieldObj_GetSeasonSet(void *self, s32 i)
{
    return data_ov003_0222f810[TownState_GetSeasonPeriod()][i];
}
}
}

namespace ns_0221f798 {
extern "C" {
void *FieldObj_GetTreeModelPath(void *self, s32 a, s32 i)
{
    return data_ov003_022328e0[FieldObj_GetSeasonSet(self, a)][i];
}
}
}

namespace ns_0221f798 {
extern "C" {
void *FieldObj_GetTreeAnimModelPath(void *self, s32 a, s32 i)
{
    return data_ov003_0223294c[FieldObj_GetSeasonSet(self, a)][i];
}
}
}

namespace ns_0221f798 {
extern "C" {
void *FieldObj_GetCedarModelPath(FieldObjectManager *self, s32 i)
{
    return data_ov003_02232910[FieldObj_GetSeasonSet(self, 2)][i];
}
}
}

namespace ns_0221f798 {
extern "C" {
void *FieldObj_GetCedarAnimModelPath(FieldObjectManager *self, s32 i)
{
    return data_ov003_022328f8[FieldObj_GetSeasonSet(self, 2)][i];
}
}
}

namespace ns_0221f798 {
extern "C" {
void *FieldObj_GetPalmModelPath(FieldObjectManager *self, s32 i)
{
    return data_ov003_022328a4[FieldObj_GetSeasonSet(self, 2)][i];
}
}
}

namespace ns_0221f798 {
extern "C" {
void *FieldObj_GetPalmAnimModelPath(void *self, s32 i)
{
    return data_ov003_022328bc[FieldObj_GetSeasonSet(self, 2)][i];
}
}
}

namespace ns_0221f798 {
extern "C" {
char **FieldObj_GetTreeTexturePaths(void *self, s32 i)
{
    return data_ov003_02232904[i];
}
}
}

namespace ns_0221f798 {
extern "C" {
BOOL FieldObj_LoadTreeTextures(void *self, void **a, void **b)
{
    s32 j;
    u32 done[3];
    s32 i;
    for (j = 0; j < 3; j++) {
        done[j] = 0;
    }
    for (i = 0; i < 3; i++) {
        s32 t = FieldObj_GetSeasonSet(self, i);
        u32 *d = &done[t];
        if (*d == 0) {
            void **pa = a + t * 6;
            void **pb = b + t * 6;
            if (!FieldObj_LoadTextureFiles(self, pa, pb, FieldObj_GetTreeTexturePaths(self, t), 6)) {
                return FALSE;
            }
            *d = 1;
        }
    }
    return TRUE;
}
}
}

namespace ns_0221f798 {
extern "C" {
BOOL FieldObj_LoadCedarTextures(void *a, void **b, void **c)
{
    s32 t = FieldObj_GetSeasonSet(a, 2);
    if (FieldObj_LoadTextureFiles(a, b, c, data_ov003_022328d4[t], 6)) {
        return TRUE;
    }
    return FALSE;
}
}
}

namespace ns_0221f798 {
extern "C" {
BOOL FieldObj_LoadLitCedarTexture(void *a, void **b, void **c)
{
    BOOL ok = TRUE;
    if (!FieldObj_LoadTextureFiles(a, b, c, data_ov003_02232640, ok)) {
        ok = FALSE;
    }
    return ok;
}
}
}

namespace ns_0221f798 {
extern "C" {
BOOL FieldObj_LoadPalmTextures(void *a, void **b, void **c)
{
    s32 t = FieldObj_GetSeasonSet(a, 2);
    if (FieldObj_LoadTextureFiles(a, b, c, data_ov003_02232934[t], 6)) {
        return TRUE;
    }
    return FALSE;
}
}
}

namespace ns_0221f798 {
extern "C" {
BOOL FieldObj_LoadTreePalettes(void *a, void **b, void **c, void **d, void **e)
{
    if (!FieldObj_LoadPaletteFiles(a, b, c, data_ov003_02232940, 3)) {
        return FALSE;
    }
    if (FieldObj_LoadPaletteFiles(a, d, e, data_ov003_022328c8, 3)) {
        return TRUE;
    }
    return FALSE;
}
}
}

namespace ns_0221f798 {
extern "C" {
BOOL FieldObj_LoadCedarPalettes(void *a, void **b, void **c, void **d, void **e)
{
    if (!FieldObj_LoadPaletteFiles(a, b, c, data_ov003_02232638, 1)) {
        return FALSE;
    }
    if (FieldObj_LoadPaletteFiles(a, d, e, data_ov003_02232644, 1)) {
        return TRUE;
    }
    return FALSE;
}
}
}

namespace ns_0221f798 {
extern "C" {
BOOL FieldObj_LoadLitCedarPalette(void *a, void **b, void **c)
{
    BOOL ok = TRUE;
    if (!FieldObj_LoadPaletteFiles(a, b, c, data_ov003_0223263c, ok)) {
        ok = FALSE;
    }
    return ok;
}
}
}

namespace ns_0221f798 {
extern "C" {
BOOL FieldObj_LoadTreeSaplingModels(FieldObjectManager *self, void **a, void **b, u32 *c)
{
    s32 i;
    BOOL ok = TRUE;
    for (i = 0; i < 3; i++) {
        s32 t = FieldObj_GetSeasonSet(self, i);
        void *n = FieldObj_GetTreeModelPath(self, i, 0);
        if (!self->treeModels[i][0].loadWithTex(n, sFieldObjectModelHeap, a[t * 6], b[i], c, ok)) {
            ok = FALSE;
            break;
        }
    }
    return ok;
}
}
}

namespace ns_0221f798 {
extern "C" {
BOOL FieldObj_LoadCedarSaplingModel(FieldObjectManager *self, void **a, void **b, u32 *c)
{
    BOOL ok = TRUE;
    if (!self->cedarModels[0].loadWithTex(FieldObj_GetCedarModelPath(self, 0), sFieldObjectModelHeap, *a, *b, c, ok)) {
        ok = FALSE;
    }
    return ok;
}
}
}

namespace ns_0221f798 {
extern "C" {
BOOL FieldObj_LoadPalmSaplingModel(FieldObjectManager *self, void **a, void **b, u32 *c)
{
    BOOL ok = TRUE;
    if (!self->palmModels[0].loadWithTex(FieldObj_GetPalmModelPath(self, 0), sFieldObjectModelHeap, *a, *b, c, ok)) {
        ok = FALSE;
    }
    return ok;
}
}
}

namespace ns_0221ede8 {
extern "C" BOOL FieldObj_LoadTreeStageModels(Obj *o, void **a, void **b, u32 *c)
{
    s32 i, j;
    TreeAnim *base2; void **pb1; void **pb2; void **q2; void **q1; Elem *base1;
    for (i = 0; i < 3; i++) {
        s32 t = FieldObj_GetSeasonSet(o, i);
        j = 1;
        pb1 = &b[i];
        q1 = a + t * 6;
        base1 = o->treeModels[i];
        for (; j <= 4; j++) {
            void *n = FieldObj_GetTreeModelPath(o, i, j);
            if (!base1[j].loadWithTex(n, sFieldObjectModelHeap, q1[j], *pb1, c, 2)) {
                return FALSE;
            }
        }
    }
    for (i = 0; i < 3; i++) {
        s32 t = FieldObj_GetSeasonSet(o, i);
        j = 0;
        base2 = o->treeAnimSet.treeAnims[i];
        pb2 = &b[i];
        q2 = a + t * 6;
        for (; j < 4; j++) {
            void *n = FieldObj_GetTreeAnimModelPath(o, i, j);
            if (!((CachedModel *)base2[j].animModel)->loadWithTex(n, sFieldObjectModelHeap, (q2 + j)[1], *pb2, c, 2)) {
                return FALSE;
            }
        }
    }
    return TRUE;
}
}

namespace ns_0221ede8 {
extern "C" BOOL FieldObj_LoadCedarStageModels(Obj *o, void **a, void **b, u32 *c)
{
    s32 i;
    for (i = 1; i <= 4; i++) {
        void *n = FieldObj_GetCedarModelPath(o, i);
        if (!o->cedarModels[i].loadWithTex(n, sFieldObjectModelHeap, a[i], *b, c, 2)) {
            return FALSE;
        }
    }
    for (i = 0; i < 4; i++) {
        void *n = FieldObj_GetCedarAnimModelPath(o, i);
        TreeAnim *sg = &o->treeAnimSet.cedarAnims[i];
        if (!((CachedModel *)sg->animModel)->loadWithTex(n, sFieldObjectModelHeap, (a + i)[1], *b, c, 2)) {
            return FALSE;
        }
    }
    return TRUE;
}
}

namespace ns_0221ede8 {
extern "C" BOOL FieldObj_LoadLitCedarModels(Obj *o, void **a, void **b)
{
    u32 cnt;
    for (cnt = 0; cnt < 3; cnt++) {
        if (!o->litCedarModels[cnt].loadWithTex(data_ov003_0223264c[0], sFieldObjectModelHeap, *a, *b, &cnt, 1)) {
            return FALSE;
        }
    }
    return TRUE;
}
}

namespace ns_0221ede8 {
extern "C" BOOL FieldObj_LoadPalmStageModels(Obj *o, void **a, void **b, u32 *c)
{
    s32 i;
    for (i = 1; i <= 4; i++) {
        void *n = FieldObj_GetPalmModelPath(o, i);
        if (!o->palmModels[i].loadWithTex(n, sFieldObjectModelHeap, a[i], *b, c, 2)) {
            return FALSE;
        }
    }
    for (i = 0; i < 4; i++) {
        void *n = FieldObj_GetPalmAnimModelPath(o, i);
        TreeAnim *sg = &o->treeAnimSet.palmAnims[i];
        if (!((CachedModel *)sg->animModel)->loadWithTex(n, sFieldObjectModelHeap, (a + i)[1], *b, c, 2)) {
            return FALSE;
        }
    }
    return TRUE;
}
}

namespace ns_0221ede8 {
extern "C" BOOL FieldObj_LoadTreeDeadModels(Obj *o, void **a, void **b, u32 *c)
{
    s32 i;
    for (i = 0; i < 3; i++) {
        s32 t = FieldObj_GetSeasonSet(o, i);
        void *n = FieldObj_GetTreeModelPath(o, i, 5);
        if (!o->treeModels[i][5].loadWithTex(n, sFieldObjectModelHeap, (a + t * 6)[5], b[i], c, 1)) {
            return FALSE;
        }
    }
    return TRUE;
}
}

namespace ns_0221ede8 {
extern "C" BOOL FieldObj_LoadCedarDeadModel(Obj *o, void **a, void **b, u32 *c)
{
    void *n = FieldObj_GetCedarModelPath(o, 5);
    if (o->cedarModels[5].loadWithTex(n, sFieldObjectModelHeap, a[5], *b, c, 1)) {
        return TRUE;
    }
    return FALSE;
}
}

namespace ns_0221ede8 {
extern "C" BOOL FieldObj_LoadPalmDeadModel(Obj *o, void **a, void **b, u32 *c)
{
    void *n = FieldObj_GetPalmModelPath(o, 5);
    if (o->palmModels[5].loadWithTex(n, sFieldObjectModelHeap, a[5], *b, c, 1)) {
        return TRUE;
    }
    return FALSE;
}
}

namespace ns_0221ede8 {
extern "C" void FieldObj_ClearTreeFileBufs(Obj *o, void **a, void **b, void **c, void **d, void **e, void **f)
{
    s32 i, j, k;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 6; a++, b++, j++) {
            *a = 0;
            *b = 0;
        }
    }
    for (k = 0; k < 3; c++, d++, e++, f++, k++) {
        *c = 0;
        *d = 0;
        *e = 0;
        *f = 0;
    }
}
}

namespace ns_0221ede8 {
extern "C" void FieldObj_ClearCedarPalmFileBufs(Obj *o, void **a, void **b, void **c, void **d, void **e, void **f)
{
    s32 i;
    for (i = 0; i < 6; i++) {
        *a++ = 0;
        *b++ = 0;
    }
    *c = 0;
    *d = 0;
    *e = 0;
    *f = 0;
}
}

namespace ns_0221ede8 {
extern "C" void FieldObj_ClearLitCedarFileBufs(Obj *o, void **a, void **b, void **c, void **d)
{
    *a = 0;
    *b = 0;
    *c = 0;
    *d = 0;
}
}

namespace ns_0221ede8 {
extern "C" void FieldObj_FreeTreeFileBufs(Obj *o, void **a, void **b, void **c)
{
    Heap *heap = gCurrentHeap;
    s32 j, i;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 6; a++, j++) {
            if (*a) {
                Heap_Free(heap, *a);
                *a = 0;
            }
        }
    }
    for (i = 0; i < 3; b++, c++, i++) {
        if (*b) {
            Heap_Free(heap, *b);
            *b = 0;
        }
        if (*c) {
            Heap_Free(heap, *c);
            *c = 0;
        }
    }
}
}

namespace ns_0221ede8 {
extern "C" void FieldObj_FreeCedarFileBufs(Obj *o, void **a, void **b, void **c)
{
    Heap *heap = gCurrentHeap;
    s32 i;
    for (i = 0; i < 6; a++, i++) {
        if (*a) {
            Heap_Free(heap, *a);
            *a = 0;
        }
    }
    if (*b) {
        Heap_Free(heap, *b);
        *b = 0;
    }
    if (*c) {
        Heap_Free(heap, *c);
        *c = 0;
    }
}
}

namespace ns_0221ede8 {
extern "C" void FieldObj_FreeLitCedarFileBufs(Obj *o, void **a, void **b)
{
    Heap *heap = gCurrentHeap;
    if (*a) {
        Heap_Free(heap, *a);
        *a = 0;
    }
    if (*b) {
        Heap_Free(heap, *b);
        *b = 0;
    }
}
}

namespace ns_0221ede8 {
extern "C" void FieldObj_FreePalmFileBufs(Obj *o, void **a, void **b, void **c)
{
    Heap *heap = gCurrentHeap;
    s32 i;
    for (i = 0; i < 6; a++, i++) {
        if (*a) {
            Heap_Free(heap, *a);
            *a = 0;
        }
    }
    if (*b) {
        Heap_Free(heap, *b);
        *b = 0;
    }
    if (*c) {
        Heap_Free(heap, *c);
        *c = 0;
    }
}
}

namespace ns_0221ede8 {
extern "C" BOOL FieldObj_LoadTrees()
{
    u32 idx = TownState_GetSeasonPeriod();
    u32 *p;
    BOOL r7 = FALSE, r4 = FALSE, r6 = FALSE;
    p = (u32 *)&data_ov003_0222f758[idx * 2];   // (CachedModel::loadWithTex takes the pair as u32 *)
    void *l_cc[18];
    void *l_114[18];
    void *l_3c[3];
    void *l_48[3];
    void *l_54[3];
    void *l_60[3];
    void *l_6c[6];
    void *l_84[6];
    void *l_c[1];
    void *l_10[1];
    void *l_14[1];
    void *l_18[1];
    void *l_1c[1];
    void *l_20[1];
    void *l_24[1];
    void *l_28[1];
    void *l_9c[6];
    void *l_b4[6];
    void *l_2c[1];
    void *l_30[1];
    void *l_34[1];
    void *l_38[1];

    FieldObj_ClearTreeFileBufs(gFieldObjectManager, l_cc, l_114, l_3c, l_48, l_54, l_60);
    if (FieldObj_LoadTreeTextures(gFieldObjectManager, l_cc, l_114)) {
        if (FieldObj_LoadTreePalettes(gFieldObjectManager, l_3c, l_48, l_54, l_60)) {
            if (FieldObj_LoadTreeSaplingModels(gFieldObjectManager, l_cc, l_54, p)) {
                if (FieldObj_LoadTreeStageModels(gFieldObjectManager, l_cc, l_3c, p)) {
                    if (FieldObj_LoadTreeDeadModels(gFieldObjectManager, l_cc, l_54, p)) {
                        r7 = TRUE;
                    }
                }
            }
        }
    }
    FieldObj_FreeTreeFileBufs(gFieldObjectManager, l_114, l_48, l_60);
    FieldObj_ClearCedarPalmFileBufs(gFieldObjectManager, l_6c, l_84, l_c, l_10, l_14, l_18);
    if (FieldObj_LoadCedarTextures(gFieldObjectManager, l_6c, l_84)) {
        if (FieldObj_LoadCedarPalettes(gFieldObjectManager, l_c, l_10, l_14, l_18)) {
            if (FieldObj_LoadCedarSaplingModel(gFieldObjectManager, l_6c, l_14, p)) {
                if (FieldObj_LoadCedarStageModels(gFieldObjectManager, l_6c, l_c, p)) {
                    if (FieldObj_LoadCedarDeadModel(gFieldObjectManager, l_6c, l_14, p)) {
                        r4 = TRUE;
                    }
                }
            }
        }
    }
    FieldObj_FreeCedarFileBufs(gFieldObjectManager, l_84, l_10, l_18);
    FieldObj_ClearLitCedarFileBufs(gFieldObjectManager, l_1c, l_20, l_24, l_28);
    if (FieldObj_LoadLitCedarTexture(gFieldObjectManager, l_1c, l_20)) {
        if (FieldObj_LoadLitCedarPalette(gFieldObjectManager, l_24, l_28)) {
            if (FieldObj_LoadLitCedarModels(gFieldObjectManager, l_1c, l_24)) {
                r4 = TRUE;
            }
        }
    }
    FieldObj_FreeLitCedarFileBufs(gFieldObjectManager, l_20, l_28);
    FieldObj_ClearCedarPalmFileBufs(gFieldObjectManager, l_9c, l_b4, l_2c, l_30, l_34, l_38);
    if (FieldObj_LoadPalmTextures(gFieldObjectManager, l_9c, l_b4)) {
        if (FieldObj_LoadCedarPalettes(gFieldObjectManager, l_2c, l_30, l_34, l_38)) {
            if (FieldObj_LoadPalmSaplingModel(gFieldObjectManager, l_9c, l_34, p)) {
                if (FieldObj_LoadPalmStageModels(gFieldObjectManager, l_9c, l_2c, p)) {
                    if (FieldObj_LoadPalmDeadModel(gFieldObjectManager, l_9c, l_34, p)) {
                        r6 = TRUE;
                    }
                }
            }
        }
    }
    FieldObj_FreePalmFileBufs(gFieldObjectManager, l_b4, l_30, l_38);
    if (r7 && r4 && r6) {
        return TRUE;
    }
    return FALSE;
}
}

namespace ns_0221ede8 {
extern "C" s32 FieldObj_LoadFlowerTextures(Obj *o, void **a, void **b)
{
    return FieldObj_LoadTextureFiles(o, a, b, data_ov003_02232bc8, 4);
}
}

namespace ns_0221ede8 {
extern "C" s32 FieldObj_LoadFlowerPalettes(Obj *o, void **a, void **b)
{
    return FieldObj_LoadPaletteFiles(o, a, b, data_ov003_02232a18, 4);
}
}

namespace ns_0221ede8 {
extern "C" BOOL FieldObj_LoadFlowerModels(Obj *o, void **a, void **b)
{
    s32 i;
    for (i = 0; i < 4; a++, b++, i++) {
        u32 j = 0;
        char **pname = &data_ov003_02232a28[i];
        Elem *base = o->flowerModels[i];
        u32 n = data_ov003_0222f4a8[i];
        for (; j < n; j++) {
            if (!base[j].loadWithTex(*pname, sFieldObjectModelHeap, *a, *b, &j, 1)) {
                return FALSE;
            }
        }
    }
    return TRUE;
}
}

namespace ns_0221ede8 {
extern "C" s32 FieldObj_LoadSpecialFlowerTextures(Obj *o, void **a, void **b)
{
    return FieldObj_LoadTextureFiles(o, a, b, data_ov003_02232778, 2);
}
}

namespace ns_0221ede8 {
extern "C" s32 FieldObj_LoadSpecialFlowerPalettes(Obj *o, void **a, void **b)
{
    return FieldObj_LoadPaletteFiles(o, a, b, data_ov003_02232a48, 2);
}
}

namespace ns_0221ede8 {
extern "C" BOOL FieldObj_LoadSpecialFlowerModels(Obj *o, void **a, void **b)
{
    s32 i;
    for (i = 0; i < 2; a++, b++, i++) {
        u32 j;
        for (j = 0; j < 2; j++) {
            if (!o->specialFlowerModels[i][j].loadWithTex(data_ov003_022326e8[i], sFieldObjectModelHeap, *a, *b, &j, 1)) {
                return FALSE;
            }
        }
    }
    return TRUE;
}
}

namespace ns_0221ede8 {
extern "C" BOOL FieldObj_LoadDandelionModels(Obj *o)
{
    BOOL r = FALSE;
    if (ModelSet_Load(&o->dandelionModelSet, (u8 *)"/fg/flower/tanpopo.nsbmd", sFieldObjectModelHeap)) {
        s32 i;
        for (i = 0; i < 2; i++) {
            o->dandelionModels[i] = (CachedModel *)ModelSet_Find(&o->dandelionModelSet, data_ov003_02232720[i]);
        }
        r = TRUE;
    }
    return r;
}
}

namespace ns_0221e4d4 {
extern "C" {
void FieldObj_FreeFlowerFiles(Obj *o, void **b, void **d, void **f, void **h) {
    Heap *heap = gCurrentHeap;
    s32 i;
    for (i = 0; i < 4; i++) {
        if (*b != 0) {
            Heap_Free(heap, *b);
            *b = 0;
        }
        if (*d != 0) {
            Heap_Free(heap, *d);
            *d = 0;
        }
        b++;
        d++;
    }
    for (i = 0; i < 2; i++) {
        if (*f != 0) {
            Heap_Free(heap, *f);
            *f = 0;
        }
        if (*h != 0) {
            Heap_Free(heap, *h);
            *h = 0;
        }
        f++;
        h++;
    }
}
}
}

namespace ns_0221e4d4 {
extern "C" {
BOOL FieldObj_LoadFlowers() {
    BOOL r5 = FALSE, r4 = FALSE;
    void *A[4], *B[4], *C[4], *D[4];
    void *E[2], *F[2], *G[2], *H[2];
    s32 i, j;
    for (i = 0; i < 4; i++) {
        A[i] = NULL;
        B[i] = NULL;
        C[i] = NULL;
        D[i] = NULL;
    }
    for (j = 0; j < 2; j++) {
        E[j] = NULL;
        F[j] = NULL;
        G[j] = NULL;
        H[j] = NULL;
    }
    if (FieldObj_LoadFlowerTextures(gFieldObjectManager, A, B)) {
        if (FieldObj_LoadFlowerPalettes(gFieldObjectManager, C, D)) {
            if (FieldObj_LoadFlowerModels(gFieldObjectManager, A, C)) {
                r5 = TRUE;
            }
        }
    }
    if (FieldObj_LoadSpecialFlowerTextures(gFieldObjectManager, E, F)) {
        if (FieldObj_LoadSpecialFlowerPalettes(gFieldObjectManager, G, H)) {
            if (FieldObj_LoadSpecialFlowerModels(gFieldObjectManager, E, G)) {
                r4 = TRUE;
            }
        }
    }
    FieldObj_FreeFlowerFiles(gFieldObjectManager, B, D, F, H);
    s32 r = FieldObj_LoadDandelionModels(gFieldObjectManager);
    if (r5 && r4 && r) {
        return TRUE;
    }
    return FALSE;
}
}
}

namespace ns_0221e4d4 {
extern "C" {
s32 FieldObj_LoadGrassPalette(Obj *o, void **a, void **b) {
    return FieldObj_LoadPaletteFiles(o, a, b, data_ov003_02232650, 1);
}
}
}

namespace ns_0221e4d4 {
extern "C" {
BOOL FieldObj_LoadGrassModels(Obj *o, void *pltt) {
    s32 i;
    u32 n = TownState_GetSeasonPeriod();
    i = 0;
    u32 *p = (u32 *)&data_ov003_0222f6a0[n];   // (CachedModel::loadWithTex takes it as u32 *)
    for (; i < 5; i++) {
        if (!o->grassModels[i].loadWithTex(data_ov003_02232bd8[i], sFieldObjectModelHeap, NULL, pltt, p, 1)) {
            return FALSE;
        }
    }
    return TRUE;
}
}
}

namespace ns_0221e4d4 {
extern "C" {
BOOL FieldObj_LoadGrass() {
    BOOL r = FALSE;
    void *a = NULL;
    void *b = NULL;
    if (FieldObj_LoadGrassPalette(gFieldObjectManager, &a, &b)) {
        if (FieldObj_LoadGrassModels(gFieldObjectManager, a)) {
            r = TRUE;
        }
    }
    if (b != NULL) {
        Heap_Free(gCurrentHeap, b);
    }
    return r;
}
}
}

namespace ns_0221e4d4 {
extern "C" {
u32 FieldObj_GetGroundTexSet() {
    return data_ov003_0222f6fc[TownState_GetSeasonPeriod()];
}
}
}

namespace ns_0221e4d4 {
extern "C" {
s32 FieldObj_LoadHolePalette(Obj *o, void **a, void **b) {
    return FieldObj_LoadPaletteFiles(o, a, b, data_ov003_02232654, 1);
}
}
}

namespace ns_0221e4d4 {
extern "C" {
BOOL FieldObj_LoadHoleModels(Obj *o, void *pltt) {
    u32 m = FieldObj_GetGroundTexSet();
    if (o->holeModels[0].loadWithTex(data_ov003_02232634[0], sFieldObjectModelHeap, NULL, pltt, &m, 1) == 0) {
        return FALSE;
    }
    if (o->holeModels[1].loadWithTex(data_ov003_02232634[0], sFieldObjectModelHeap, NULL, pltt, (u32 *)&sFieldObjShapeTable[0x310 / 4], 1)) {
        return TRUE;
    }
    return FALSE;
}
}
}

namespace ns_0221e4d4 {
extern "C" {
BOOL FieldObj_LoadHoles() {
    BOOL r = FALSE;
    void *a = NULL;
    void *b = NULL;
    if (FieldObj_LoadHolePalette(gFieldObjectManager, &a, &b)) {
        if (FieldObj_LoadHoleModels(gFieldObjectManager, a)) {
            r = TRUE;
        }
    }
    if (b != NULL) {
        Heap_Free(gCurrentHeap, b);
    }
    return r;
}
}
}

namespace ns_0221e4d4 {
extern "C" {
s32 FieldObj_LoadCrackPalette(Obj *o, void **a, void **b) {
    return FieldObj_LoadPaletteFiles(o, a, b, data_ov003_02232630, 1);
}
}
}

namespace ns_0221e4d4 {
extern "C" {
BOOL FieldObj_LoadCrackModels(Obj *o, void *pltt) {
    u32 m = FieldObj_GetGroundTexSet();
    if (o->crackModels[0].loadWithTex(data_ov003_02232648[0], sFieldObjectModelHeap, NULL, pltt, &m, 1) == 0) {
        return FALSE;
    }
    if (o->crackModels[1].loadWithTex(data_ov003_02232648[0], sFieldObjectModelHeap, NULL, pltt, (u32 *)&sFieldObjShapeTable[0x30c / 4], 1)) {
        return TRUE;
    }
    return FALSE;
}
}
}

namespace ns_0221e4d4 {
extern "C" {
BOOL FieldObj_LoadCracks() {
    BOOL r = FALSE;
    void *a = NULL;
    void *b = NULL;
    if (FieldObj_LoadCrackPalette(gFieldObjectManager, &a, &b)) {
        if (FieldObj_LoadCrackModels(gFieldObjectManager, a)) {
            r = TRUE;
        }
    }
    if (b != NULL) {
        Heap_Free(gCurrentHeap, b);
    }
    return r;
}
}
}

namespace ns_0221e4d4 {
extern "C" {
BOOL FieldObj_LoadTurnipModels(Obj *o) {
    s32 i;
    for (i = 0; i < 2; i++) {
        if (!gFieldObjectManager->turnipModels[i].load(data_ov003_02232788[i], sFieldObjectAnimHeap)) {
            return FALSE;
        }
    }
    return TRUE;
}
}
}

namespace ns_0221e4d4 {
extern "C" {
BOOL FieldObj_LoadTurnips() {
    // (called through sFieldObjectLoaders without arguments; FieldObj_LoadTurnipModels uses gFieldObjectManager)
    return FieldObj_LoadTurnipModels(gFieldObjectManager);
}
}
}

namespace ns_0221e4d4 {
extern "C" {
BOOL FieldObj_LoadDesignModels(Obj *o) {
    Elem *p = &o->designModels[0];
    s32 i;
    u32 j;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 8; j++) {
            if (!((CachedModel *)(p))->loadWithSharedTex((u8 *)"/fg/obj_myD.nsbmd", i, j)) {
                return FALSE;
            }
            p++;
        }
    }
    return TRUE;
}
}
}

namespace ns_0221e4d4 {
extern "C" {
BOOL FieldObj_LoadDesigns() {
    return FieldObj_LoadDesignModels(gFieldObjectManager);
}
}
}

namespace ns_0221e4d4 {
extern "C" {
BOOL FieldObj_RunLoaders(Obj *o) {
    s32 i;
    for (i = 0; i < 9; i++) {
        if (!sFieldObjectLoaders[i]()) {
            return FALSE;
        }
    }
    return TRUE;
}
}
}

BOOL FieldObjectManager::onCreate() {
    using namespace ns_0221e4d4;
    FieldObjectManager *o = this;
    BOOL r = FALSE;
    if (sFieldObjectModelHeap == NULL) {
        sFieldObjectModelHeap = (Heap *)ExpHeap_CreateInPlace(sFieldObjectModelHeapBuf, FIELDOBJ_MODEL_HEAP_SIZE);
    }
    if (sFieldObjectAnimHeap == NULL) {
        sFieldObjectAnimHeap = (Heap *)ExpHeap_Create(0x8c00, gCurrentHeap);
    }
    if (FieldObj_LoadIconModels(o)) {
        gFieldObjectManager = o;
        if (FieldObj_RunLoaders(o)) {
            o->frameCounter = 0;
            TreeAnimSet_Init(&o->treeAnimSet);
            TreeLeafFx_Init(&sTreeLeafFx);
            FieldItemFxTable_Init(&sFieldItemFxTable);
            Field_OnEnter();
            r = TRUE;
        } else {
            gFieldObjectManager = NULL;
        }
    }
    return r;
}

BOOL FieldObjectManager::onExecute() {
    using namespace ns_0221e4d4;
    FieldItemFxTable_Update(&sFieldItemFxTable);
    TreeAnimSet_Update(&treeAnimSet);
    return TRUE;
}

namespace ns_0221e4d4 {
extern "C" {
CachedModel *FieldObj_DrawIconModel(Obj *o, u32 idx, V3 *a, s32 b, V3 *c, s32 d, s32 e, s32 f) {
    CachedModel *m = o->iconModels[idx];
    V3 t;
    s32 ang = WorldCurve_ToCurved(&t, a);
    Mtx43_SetTranslate(&data_021f47e0, t.x, t.y, t.z);
    Mtx43_RotateX(&data_021f47e0, ang);
    Mtx43_RotateXYZ(&data_021f47e0, *(s16 *)&d, *(s16 *)&e, *(s16 *)&f);
    Mtx43_Scale(&data_021f47e0, c->x, c->y, c->z);
    NNS_G3dMdlSetMdlAlphaAll(m->resMdl, b);
    FieldObj_DrawModel(o, m, data_021f47e0);
    return m;
}
}
}

namespace ns_0221e4d4 {
extern "C" {
CachedModel *FieldObj_DrawItemIcon(Obj *o, u32 t, V3 *a, s32 b, V3 *c, s32 d, s32 e, s32 f) {
    u16 type = 0xfff1;
    V3 va;
    V3 vc;
    type = t;
    va.x = a->x;
    va.y = a->y;
    va.z = a->z;
    vc.x = c->x;
    vc.y = c->y;
    vc.z = c->z;
    s32 idx = Item_GetInfoUnk07(&type);
    return FieldObj_DrawIconModel(o, idx, &va, b, &vc, *(s16 *)&d, *(s16 *)&e, *(s16 *)&f);
}
}
}

namespace ns_0221e4d4 {
extern "C" {
void FieldObj_DrawItemModel(Obj *o, u32 t, V3 *pos, V3 *scale, s32 rx, s32 ry, s32 rz) {
    s32 idx;
    BOOL f9, f8, f7, f6, f5, f4, f3, f2;
    u32 a;
    u16 type = 0xfff1;
    V3 v;
    type = t;
    s32 ang = WorldCurve_ToCurved(&v, pos);
    Mtx43_SetTranslate(&data_021f47e0, v.x, v.y, v.z);
    Mtx43_RotateX(&data_021f47e0, ang);
    Mtx43_RotateXYZ(&data_021f47e0, *(s16 *)&rx, *(s16 *)&ry, *(s16 *)&rz);
    Mtx43_Scale(&data_021f47e0, scale->x, scale->y, scale->z);
    if (Unk_ov003_0221e4d4_Chk1(&type)) {
        u16 u = t - 0xd4;
        u16 nv;
        if (u < 7) {
            nv = u + 0x153b;
        } else {
            nv = 0x153b;
        }
        type = nv;
    } else if (t == 0x20) {
        type = 0x156a;
    }
    a = type;
    s32 k = (type & 0xf000) >> 12;
    switch (k) {
    case 0:
        BOOL c4;
        c4 = FALSE;
        u16 x;
        x = a + 0xffe6;
        if (x <= 4) {
            if ((1 << x) & 0x1b) {
                c4 = TRUE;
            }
        }
        if (!c4) {
            if (a != 0x88 && a != 0x89) {
                goto rest;
            }
        }
        FieldObj_DrawSpecialFlower(o, &type, data_021f47e0);
        break;
    rest:
        f9 = TRUE, f8 = TRUE, f7 = TRUE, f6 = TRUE, f5 = TRUE, f4 = TRUE, f3 = TRUE, f2 = FALSE;
        if (a <= 5) {
            f2 = TRUE;
        }
        if (!f2) {
            if (a < 6 || a > 0xb) {
                f3 = FALSE;
            }
        }
        if (!f3) {
            if (a < 0xc || a > 0x11) {
                f4 = FALSE;
            }
        }
        if (!f4) {
            if ((a < 0x12 || a > 0x19) && a != 0x1c) {
                f5 = FALSE;
            }
        }
        if (!f5) {
            if ((a < 0x8a || a > 0x8f) && (a < 0x90 || a > 0x95) && (a < 0x96 || a > 0x9b) && (a < 0x9c || a > 0xa3) && a != 0xa5) {
                f6 = FALSE;
            }
        }
        if (!f6) {
            if (a != 0x1a) {
                f7 = FALSE;
            }
        }
        if (!f7) {
            if (a != 0xa4) {
                f8 = FALSE;
            }
        }
        if (!f8) {
            if (a != 0x1d) {
                f9 = FALSE;
            }
        }
        if (f9) {
            FieldObj_DrawFlower(o, &type, data_021f47e0);
        } else if (a >= 0xa7 && a <= 0xc6) {
            FieldObj_DrawDesign(o, &type, data_021f47e0);
        }
        break;
    case 1:
    case 3:
    case 4:
        idx = Item_GetInfoUnk07(&type);
        FieldObj_DrawModel(o, o->iconModels[idx], data_021f47e0);
        break;
    case 2:
        break;
    }
}
}
}

namespace ns_0221db54 {
extern "C" void FieldObj_DrawTreeFruit(O *o, u16 *t, VecFx32 *v)
{
    u32 id;
    u32 k = 0;
    u32 w = *t;
    if (w >= 0x57 && w <= 0x5b) {
        k = 1;
    }
    if (k) {
        id = 0x14b8;
    } else {
        id = sTreeFruitIcons[Item_GetFruitTreeFruit(t)];
    }
    s32 i = 0;
    s32 z = i;
    do {
        struct {
            VecFx32 a, b, c;
        } l;
        l.a.x = v->x + data_ov003_0222f510[i].x;
        l.a.y = v->y + data_ov003_0222f510[i].y;
        l.a.z = v->z + data_ov003_0222f510[i].z;
        l.b.x = l.a.x;
        l.b.y = l.a.y;
        l.b.z = l.a.z;
        l.c.x = 0x1000;
        l.c.y = 0x1000;
        l.c.z = 0x1000;
        FieldObj_DrawItemIcon(o, id, (V3 *)&l.b, 0x1f, (V3 *)&l.c, z, z, z);
        i++;
    } while (i < 3);
}
}

namespace ns_0221db54 {
extern "C" void FieldObj_DrawCedarLights(O *o, VecFx32 *t, s32 a, s32 b)
{
    M *q = NULL;
    s32 idx = LitCedarList_Find(a, b);
    if (idx != -1) {
        s32 d = (o->frameCounter + idx * 15) / 20;
        q = &o->litCedarModels[(idx + d) % 3];
    }
    VecFx32 v;
    s32 r = WorldCurve_ToCurved(&v, t);
    Mtx43_SetTranslate(&data_021f47e0, v.x, v.y, v.z);
    Mtx43_RotateX(&data_021f47e0, r);
    FieldObj_DrawModel(o, q, data_021f47e0);
}
}

namespace ns_0221db54 {
extern "C" M *FieldObj_DrawTree(O *o, u16 *t, s32 a, s32 b, V3 v, Blk m)
{
    M *q;
    struct {
        VecFx32 a1, a2, t1, t2, b1, sc, t3, b2;
    } l;
    s32 n = Item_GetTreeStage(t);
    BOOL f = FALSE;
    u32 id = *t;
    if (id >= 0x2f && id <= 0x56) {
        f = TRUE;
    }
    if (f) {
        if (Item_IsTreeGrown(t)) {
            l.t1.x = v.x;
            l.t1.y = v.y;
            l.t1.z = v.z;
            FieldObj_DrawTreeFruit(o, t, &l.t1);
        }
        q = &o->treeModels[2][0] + n;
    } else if (id >= 0x57 && id <= 0x5b) {
        a = (a ^ b) & 1;
        if (Item_IsTreeGrown(t)) {
            l.t2.x = v.x;
            l.t2.y = v.y;
            l.t2.z = v.z;
            FieldObj_DrawTreeFruit(o, t, &l.t2);
        }
        q = &o->treeModels[a][0] + n;
    } else if (id == 0x69) {
        s32 i, z;
        i = 0;
        z = 0;
        do {
            l.a1.x = v.x + data_ov003_0222f510[i].x;
            l.a1.y = v.y + data_ov003_0222f510[i].y;
            l.a1.z = v.z + data_ov003_0222f510[i].z;
            l.b1.x = l.a1.x;
            l.b1.y = l.a1.y;
            l.b1.z = l.a1.z;
            l.sc.x = 0x1000;
            l.sc.y = 0x1000;
            l.sc.z = 0x1000;
            FieldObj_DrawItemIcon(o, 0x1542, (V3 *)&l.b1, 0x1f, (V3 *)&l.sc, z, z, z);
            i++;
        } while (i < 3);
        q = &o->treeModels[2][0] + n;
    } else if (id >= 0x6a && id <= 0x6c) {
        q = &o->cedarModels[0] + n;
    } else if (id == 0x6d) {
        l.t3.x = v.x;
        l.t3.y = v.y;
        l.t3.z = v.z;
        FieldObj_DrawCedarLights(o, &l.t3, a, b);
        q = &o->cedarModels[0] + n;
    } else if (id >= 0x5d && id <= 0x61) {
        q = &o->cedarModels[0] + n;
    } else if (id >= 0xc8 && id <= 0xcf) {
        if (Item_IsTreeGrown(t)) {
            s32 i, z;
            i = 0;
            z = 0;
            do {
                l.a2.x = v.x + data_ov003_0222f4f8[i].x;
                l.a2.y = v.y + data_ov003_0222f4f8[i].y;
                l.a2.z = v.z + data_ov003_0222f4f8[i].z;
                l.b2.x = l.a2.x;
                l.b2.y = l.a2.y;
                l.b2.z = l.a2.z;
                l.sc.x = 0x1000;
                l.sc.y = 0x1000;
                l.sc.z = 0x1000;
                FieldObj_DrawItemIcon(o, 0x1548, (V3 *)&l.b2, 0x1f, (V3 *)&l.sc, z, z, z);
                i++;
            } while (i < 2);
        }
        q = &o->palmModels[0] + n;
    } else {
        q = &o->treeModels[(a ^ b) & 1][0] + n;
    }
    FieldObj_DrawModel(o, q, m);
    return q;
}
}

namespace ns_0221db54 {
extern "C" void FieldObj_DrawTreeAt(O *o, u16 *t, s32 a, s32 b, V3 v, Blk m)
{
    FieldObj_DrawTree(o, t, a, b, v, m);
    ObjShadow_DrawTree((VecFx32 *)&v, Item_GetTreeStage(t));
}
}

namespace ns_0221db54 {
extern "C" M *FieldObj_DrawSapling(O *o, u16 *t, s32 a, s32 b, Blk m)
{
    M *q;
    if (*t == 0x25) {
        q = &o->treeModels[(a ^ b) & 1][5];
    } else if (*t == 0xc7) {
        q = &o->palmModels[5];
    } else {
        q = &o->cedarModels[5];
    }
    FieldObj_DrawModel(o, q, m);
    return q;
}
}

namespace ns_0221db54 {
extern "C" M *FieldObj_DrawStump(O *o, u16 *t, Blk m)
{
    BOOL f = FALSE;
    u32 v = *t;
    if (v < 0x2b || v > 0x2e) {
    } else {
        f = TRUE;
    }
    if (f) {
        v -= 0x2b;
    } else if (v >= 0xff && v <= 0x102) {
        v -= 0xff;
    } else if (v >= 0x62 && v <= 0x65) {
        v -= 0x5e;
    } else {
        v -= 0xc8;
    }
    M *q = o->stumpModels[v];
    FieldObj_DrawModel(o, q, m);
    return q;
}
}

namespace ns_0221db54 {
extern "C" M *FieldObj_DrawFlower(O *o, u16 *t, Blk m)
{
    s32 a = Flower_GetSpecies(t);
    s32 b = Flower_GetColor(t);
    M *q = &o->flowerModels[a][b];
    FieldObj_DrawModel(o, q, m);
    return q;
}
}

namespace ns_0221db54 {
extern "C" M *FieldObj_DrawFlowerBySpecies(O *o, u16 *t, Blk m)
{
    M *q = &o->flowerModels[Flower_GetSpecies(t)][0];
    FieldObj_DrawModel(o, q, m);
    return q;
}
}

namespace ns_0221db54 {
extern "C" M *FieldObj_DrawSpecialFlower(O *o, u16 *t, Blk m)
{
    M *q;
    switch (*t) {
    case 0x1a:
        q = &o->specialFlowerModels[0][0];
        break;
    case 0x1b:
        q = &o->specialFlowerModels[1][0];
        break;
    case 0x1c:
        break;
    case 0x1d:
        q = o->dandelionModels[0];
        break;
    case 0x1e:
        q = o->dandelionModels[1];
        break;
    case 0x88:
    case 0xa4:
        q = &o->specialFlowerModels[0][1];
        break;
    case 0x89:
        q = &o->specialFlowerModels[1][1];
        break;
    }
    FieldObj_DrawModel(o, q, m);
    return q;
}
}

namespace ns_0221db54 {
extern "C" M *FieldObj_DrawGrass(O *o, u16 *t, Blk m)
{
    BOOL f = FALSE;
    u32 v = *t;
    if (v < 0x1f || v > 0x20) {
    } else {
        f = TRUE;
    }
    s32 k;
    if (f) {
        k = 4;
    } else {
        k = v - 0x21;
    }
    M *q = &o->grassModels[k];
    FieldObj_DrawModel(o, q, m);
    return q;
}
}

namespace ns_0221db54 {
extern "C" M *FieldObj_DrawRock(O *o, u16 *t, Blk m)
{
    M *q = o->stoneModels[(*t - 0xe3) % 5];
    FieldObj_DrawModel(o, q, m);
    return q;
}
}

namespace ns_0221db54 {
extern "C" void FieldObj_DrawRockAt(O *o, u16 *t, VecFx32 *pos, Blk m)
{
    FieldObj_DrawRock(o, t, m);
    ObjShadow_DrawRock(pos);
}
}

namespace ns_0221db54 {
extern "C" M *FieldObj_DrawHole(O *o, u16 *t, Blk m)
{
    s32 k = 0;
    if (*t == 0xfd) {
        k = 1;
    }
    M *q = &o->holeModels[k];
    FieldObj_DrawModel(o, q, m);
    return q;
}
}

namespace ns_0221db54 {
extern "C" void FieldObj_DrawCrack(O *o, u16 *t, Blk m)
{
    s32 k = 0;
    if (*t == 0xfe) {
        k = 1;
    }
    FieldObj_DrawModel(o, &o->crackModels[k], m);
}
}

namespace ns_0221db54 {
extern "C" M *FieldObj_DrawTurnip(O *o, u16 *t, Blk m)
{
    s32 k = 0;
    if (*t == 0xe2) {
        k = 1;
    }
    M *q = &o->turnipModels[k];
    FieldObj_DrawModel(o, q, m);
    return q;
}
}

namespace ns_0221db54 {
extern "C" M *FieldObj_DrawDesign(O *o, u16 *t, Blk m)
{
    M *q = &o->designModels[*t - 0xa7];
    FieldObj_DrawModel(o, q, m);
    return q;
}
}

namespace ns_0221db54 {
extern "C" void FieldObj_DrawModel(O *o, M *p, Blk m)
{
    if (p != NULL) {
        *(Blk *)p->unk_64 = m;
        ((Model *)(void *)(p))->drawShapesDirect((s32 *)(0));
        volatile u16 a = SceneLights_GetRoomColor();
        volatile u16 b = a;
        NNS_G3dMdlSetMdlEmiAll(p->resMdl, b);
    }
}
}

namespace ns_0221cb54 {
extern "C" {
void FieldObj_DrawUnits(O *o, BlockMap *g) {
    struct {
        s32 x, z;
        V3 D, E, A, B, C;
        union {
            Blk m;
            s32 mw[12];
        };
        V3 F, G;
    } l;
    s32 idx, dist, i, j, cnt;
    u16 *cell;
    if (gCamera != 0) {
        l.A = gCameraLookAt;
        FieldPos_SnapToUnitCenter(&l.C, &l.A);
        FieldPos_ToUnit(&l.x, &l.z, &l.C);
#ifdef HOST
        // (twice the DS's window: 18 units either side, 28 back - trees and the rest no longer pop in at its edge)
        enum { HALF_W = 18, BACK = 28, MAX_OBJS = 0x3f0 };
// (the per-object depth window too, or the doubled grid is cut back to the DS one)
enum { FAR_BACK = 0x1c200 * 2, FAR_FRONT = 0x8200 * 2, NEAR_BACK = 0x14a00 * 2, NEAR_FRONT = 0x6400 * 2 };
#else
        enum { HALF_W = 9, BACK = 14, MAX_OBJS = 0xfc };
enum { FAR_BACK = 0x1c200, FAR_FRONT = 0x8200, NEAR_BACK = 0x14a00, NEAR_FRONT = 0x6400 };
#endif
        l.B.x = l.C.x + HALF_W * 0x2000;
        cnt = 0;
        l.B.y = 0;
        l.B.z = l.C.z + 0x8000;
        j = 4;
        goto jtest;
    jloop:
        FieldPos_SnapToUnitCenter(&l.D, &l.B);
        {
            s32 r4 = WorldCurve_ToCurved(&l.E, &l.D);
            Mtx43_SetTranslate(&data_021f47e0, l.E.x, l.E.y, l.E.z);
            Mtx43_RotateX(&data_021f47e0, r4);
        }
        l.m = data_021f47e0;
        dist = l.D.z - l.C.z;
        i = HALF_W;
        goto itest;
    iloop:
        cell = BlockMap_GetItemPtrAtPos(g, &l.D, 0);
        if (cell == 0) goto step;
        if (Chk_0221d37c(cell)) {
            if (Item_IsTreeStage0(cell) == 0) {
                if (Camera_IsBlockingFocusView(&l.D, 0x2000, 0x2000) != 0) goto step;
            }
        }
        {
            switch (((s32)(*cell) & 0xf000) >> 12) {
            case 0:
                if (((((*cell) >= 0x26 && (*cell) <= 0x2a) || ((*cell) >= 0x5d && (*cell) <= 0x61) || ((*cell) >= 0x2f && (*cell) <= 0x56) ||
                      ((*cell) >= 0x57 && (*cell) <= 0x5b) || ((*cell) >= 0x66 && (*cell) <= 0x68) || (*cell) == 0x69 ||
                      ((*cell) >= 0x6a && (*cell) <= 0x6c) || (*cell) == 0x6d || ((*cell) >= 0xc8 && (*cell) <= 0xcf) ||
                      ((*cell) >= 0xe3 && (*cell) <= 0xe7) || ((*cell) >= 0xe8 && (*cell) <= 0xfb)) &&
                     dist > -FAR_BACK && dist < FAR_FRONT) ||
                    (dist > -NEAR_BACK && dist < NEAR_FRONT)) {
                    if (((*cell) >= 0x26 && (*cell) <= 0x2a) || ((*cell) >= 0x5d && (*cell) <= 0x61) || ((*cell) >= 0x2f && (*cell) <= 0x56) ||
                        ((*cell) >= 0x57 && (*cell) <= 0x5b) || ((*cell) >= 0x66 && (*cell) <= 0x68) || (*cell) == 0x69 ||
                        ((*cell) >= 0x6a && (*cell) <= 0x6c) || (*cell) == 0x6d || ((*cell) >= 0xc8 && (*cell) <= 0xcf)) {
                        l.F.x = l.D.x;
                        l.F.y = l.D.y;
                        l.F.z = l.D.z;
                        FieldObj_DrawTreeAt(o, cell, l.x + i, l.z + j, l.F, l.m);
                    } else if (((*cell) >= 0x21 && (*cell) <= 0x24) || ((*cell) >= 0x1f && (*cell) <= 0x20)) {
                        FieldObj_DrawGrass(o, cell, l.m);
                    } else {
                        BOOL k = FALSE;
                        if ((*cell) <= 0x19) k = TRUE;
                        if (k || (*cell) == 0x1c) {
                            FieldObj_DrawFlower(o, cell, l.m);
                        } else if (((*cell) >= 0x6e && (*cell) <= 0x73) || ((*cell) >= 0x74 && (*cell) <= 0x79) || ((*cell) >= 0x7a && (*cell) <= 0x7f) ||
                                   ((*cell) >= 0x80 && (*cell) <= 0x87) || ((*cell) >= 0x8a && (*cell) <= 0x8f) || ((*cell) >= 0x90 && (*cell) <= 0x95) ||
                                   ((*cell) >= 0x96 && (*cell) <= 0x9b) || ((*cell) >= 0x9c && (*cell) <= 0xa3) || (*cell) == 0xa5) {
                            FieldObj_DrawFlowerBySpecies(o, cell, l.m);
                        } else if (((*cell) >= 0xe3 && (*cell) <= 0xe7) || ((*cell) >= 0xe8 && (*cell) <= 0xfb)) {
                            l.G.x = l.D.x;
                            l.G.y = l.D.y;
                            l.G.z = l.D.z;
                            FieldObj_DrawRockAt(o, cell, &l.G, l.m);
                        } else if ((*cell) == 0xa6 || (*cell) == 0xfe) {
                            FieldObj_DrawCrack(o, cell, l.m);
                        } else if ((*cell) >= 0xfc && (*cell) <= 0xfd) {
                            FieldObj_DrawHole(o, cell, l.m);
                        } else if ((*cell) == 0x25 || (*cell) == 0x5c || (*cell) == 0xc7) {
                            FieldObj_DrawSapling(o, cell, l.x + i, l.z + j, l.m);
                        } else if (((*cell) >= 0x2b && (*cell) <= 0x2e) || ((*cell) >= 0xff && (*cell) <= 0x102) || ((*cell) >= 0x62 && (*cell) <= 0x65) ||
                                   ((*cell) >= 0xd0 && (*cell) <= 0xd3)) {
                            FieldObj_DrawStump(o, cell, l.m);
                        } else {
                            BOOL k2 = FALSE;
                            {
                                u32 bx = (u16)((*cell) + 0xffe6);
                                if (bx <= 4) {
                                    if (((1 << bx) & 0x1b) != 0) k2 = TRUE;
                                }
                            }
                            if (k2 || (*cell) == 0x88 || (*cell) == 0x89 || (*cell) == 0xa4) {
                                FieldObj_DrawSpecialFlower(o, cell, l.m);
                            } else if (TownBlockMap_IsBuried((CellGrid *)g, l.x + i, l.z + j)) {
                                FieldObj_DrawCrack(o, cell, l.m);
                            } else {
                                BOOL k3 = FALSE;
                                if ((*cell) >= 0xa7 && (*cell) <= 0xc6) k3 = TRUE;
                                if (k3) {
                                    FieldObj_DrawDesign(o, cell, l.m);
                                } else if (((*cell) >= 0xd4 && (*cell) <= 0xda) || ((*cell) >= 0xdb && (*cell) <= 0xe1) || (*cell) == 0xe2) {
                                    FieldObj_DrawTurnip(o, cell, l.m);
                                }
                            }
                        }
                    }
                    cnt++;
                }
                break;
            case 1:
            case 3:
            case 4:
                if (TownBlockMap_IsBuried((CellGrid *)g, l.x + i, l.z + j)) {
                    FieldObj_DrawCrack(o, cell, l.m);
                } else {
                    idx = Item_GetInfoUnk07(cell);
                    FieldObj_DrawModel(o, o->iconModels[idx], l.m);
                }
                cnt++;
                break;
            case 2:
                break;
            }
        }
        if (cnt >= MAX_OBJS) goto end;
    step:
        l.mw[9] -= 0x2000;
        l.D.x -= 0x2000;
        i--;
    itest:
        if (i >= -HALF_W) goto iloop;
        l.B.z -= 0x2000;
        j--;
    jtest:
        if (j >= -BACK) goto jloop;
    }
end:;
}
}
}

BOOL FieldObjectManager::onDraw() {
    using namespace ns_0221cb54;
    BlockMap *a = gSceneBlockMap;
    u8 *b = gCamera;
    if (a != 0 && b != 0) {
        frameCounter = (frameCounter + 1) % 60;
        FieldObj_DrawUnits(this, a);
        FieldItemFxTable_Draw(sFieldItemFxTable.entries);
        TreeAnimSet_Draw(&treeAnimSet);
    }
    return 1;
}

BOOL FieldObjectManager::onDelete() {
    using ns_0221cb54::PendingUnits_Flush;
    using ns_0221cb54::ModelSet_Release;

    ModelSet_Release(&iconModelSet);
    ModelSet_Release(&dandelionModelSet);
    ModelSet_Release(&stoneModelSet);
    ModelSet_Release(&stumpModelSet);
    TreeAnimSet_Release(&treeAnimSet);
    FieldItemFxTable_Release(&sFieldItemFxTable);
    PendingUnits_Flush();
    if (sFieldObjectModelHeap != 0) {
        ((Heap *)(sFieldObjectModelHeap))->destroy();
        sFieldObjectModelHeap = 0;
    }
    if (sFieldObjectAnimHeap != 0) {
        sFieldObjectAnimHeap->destroy();
        sFieldObjectAnimHeap = 0;
    }
    gFieldObjectManager = 0;
    return 1;
}

namespace ns_0221cb54 {
extern "C" {
s32 Field_FindFlowerNear(V3 *out, V3 pos, s32 mask) {
    s32 i;
    BlockMap *g = gSceneBlockMap;
    s32 r = 0;
    if (g != 0) {
        V3k keep(pos);
        s32 xy[2];
        xy[0] = 0;
        xy[1] = 0;
        FieldPos_ToUnit(&xy[0], &xy[1], &pos);
        s32 layer = 0;
        i = 0;
        goto test;
    loop:
        {
            u32 b = sUnitSearchOffsets81[i];
            s32 cx = xy[0] + (((s32)b >> 4) - 8);
            s32 cz = xy[1] + ((b & 0xf) - 8);
            s32 hx = cx >> 4;
            s32 hz = cz >> 4;
            u16 *cell = BlockMap_GetItemPtr(g, hx, hz, cx - (hx << 4), cz - (hz << 4), layer);
            if (cell != 0) {
                if (Chk_0221d118(cell)) {
                    s32 bit = Flower_GetSpecies(cell);
                    if (((mask >> bit) & 1) != 0) {
                        u32 b2 = ((volatile u8 *)sUnitSearchOffsets81)[i];
                        xy[0] = xy[0] + (((s32)b2 >> 4) - 8);
                        xy[1] = xy[1] + ((b2 & 0xf) - 8);
                        FieldPos_FromUnitCenter(out, xy[0], xy[1]);
                        r = 1;
                        goto end;
                    }
                }
            }
        }
        i++;
    test:
        if (i < 0x51) goto loop;
    }
end:
    return r;
}
}
}

namespace ns_0221cb54 {
extern "C" {
s32 Field_IsRafflesiaNear(V3 *out, VecFx32 *b) {
    out->x = 0;
    out->y = 0;
    out->z = 0;
    s32 r = Town_GetRafflesiaPos(out, 0);
    if (r != 0) {
        V3k t;
        V3k tmp;
        Vec_Sub((V3 *)&tmp, out, b);
        t = tmp;
        s32 sum = t.x * t.x + t.z * t.z;
        if (sum < (s32)0x90000000) {
            r = 1;
        }
    }
    return r;
}
}
}

namespace ns_0221cb54 {
extern "C" {
void Field_DrawIconModel(u32 item, V3 a, V3 b, s32 c, s16 d, s16 e) {
    if (gFieldObjectManager != 0) {
        V3 la(a);
        V3 lb(b);
        FieldObj_DrawIconModel(gFieldObjectManager, item, &la, 0x1f, &lb, c, d, e);
    }
}
}
}

namespace ns_0221cb54 {
extern "C" {
void Field_DrawItemIcon(u32 item, V3 a, V3 b, s32 c, s16 d, s16 e) {
    if (gFieldObjectManager != 0) {
        V3 la(a);
        V3 lb(b);
        FieldObj_DrawItemIcon(gFieldObjectManager, item, &la, 0x1f, &lb, c, d, e);
    }
}
}
}

namespace ns_0221cb54 {
extern "C" {
void Field_DrawItemModel(u32 item, V3 a, V3 b, s32 c, s16 d, s16 e) {
    if (gFieldObjectManager != 0) {
        V3 la(a);
        V3 lb(b);
        FieldObj_DrawItemModel(gFieldObjectManager, item, &la, &lb, c, d, e);
    }
}
}
}

namespace ns_0221cb54 {
extern "C" {
s32 Tree_IsPendingStump(s32 o, P2 pos) {
    BOOL r = FALSE;
    s32 idx = PendingUnit_FindForAid(o, pos, 0);
    if (idx >= 0) {
        volatile u16 v = 0xfff1;
        v = PendingUnit_Get(idx)->item;
        BOOL f1 = TRUE, f2 = TRUE, f3 = TRUE, f0 = FALSE;
        u32 t = v;
        if (v >= 0x2b && t <= 0x2e) f0 = TRUE;
        if (!f0) {
            if (t < 0xff || t > 0x102) f3 = FALSE;
        }
        if (!f3) {
            if (t < 0x62 || t > 0x65) f2 = FALSE;
        }
        if (!f2) {
            if (t < 0xd0 || t > 0xd3) f1 = FALSE;
        }
        if (f1) r = TRUE;
    }
    return r;
}
}
}

namespace ns_0221cb54 {
extern "C" {
s32 Tree_BeginReaction(s32 o, P2 pos) {
    s32 result = 0;
    if (gFieldObjectManager != 0) {
        BlockMap *g = TownBlockMap_Get();
        if (g != 0) {
            s32 x = pos.x;
            s32 z = pos.z;
            s32 hx = x >> 4;
            s32 hz = z >> 4;
            u16 *cell = BlockMap_GetItemPtr(g, hx, hz, x - (hx << 4), z - (hz << 4), 0);
            if (cell != 0) {
                if (Chk_0221cd80(cell)) {
                    if (Item_IsTreeStage0(cell) == 0) {
                        V3 v;
                        FieldPos_FromUnitCenter(&v, pos.x, pos.z);
                        if (Camera_IsBlockingFocusView(&v, 0x2000, 0x2000) == 0) {
                            result = 1;
                        }
                    }
                }
            }
        }
    }
    if (result == 0) {
        BlockMap *g2 = TownBlockMap_Get();
        if (g2 != 0) {
            s32 x = pos.x;
            s32 z = pos.z;
            s32 hx = x >> 4;
            s32 hz = z >> 4;
            u16 *cell = BlockMap_GetItemPtr(g2, hx, hz, x - (hx << 4), z - (hz << 4), 0);
            if (cell != 0) {
                s32 a = Field_AidOrZero(o);
                BOOL k = FALSE;
                u32 t = *cell;
                if (t >= 0x2f && t <= 0x56) k = TRUE;
                if (k || (t >= 0xc8 && t <= 0xcf) || (t >= 0x57 && t <= 0x5b)) {
                    if (Item_IsTreeGrown(cell) != 0) {
                        s32 i;
                        for (i = 0; i < 3; i++) {
                            s32 idx = PendingUnit_FindBySlot((u32)a, i);
                            if (idx >= 0) {
                                PendingUnit_Apply(PendingUnit_Get(idx));
                            }
                        }
                    }
                } else if ((t >= 0x66 && t <= 0x68) || (t >= 0x6a && t <= 0x6c)) {
                    s32 idx = PendingUnit_FindBySlot((u32)a, 0);
                    if (idx >= 0) {
                        PendingUnit_Apply(PendingUnit_Get(idx));
                    }
                }
            }
        }
        Vec2 u = {pos.x, pos.z};
        PendingUnit_ApplyAtIfAid(&u, Field_AidOrZero(o), 0);
    }
    return result;
}
}
}

namespace ns_0221cb54 {
extern "C" {
void Tree_RequestShake(s32 o, P2 pos, s32 mode) {
    if (Tree_BeginReaction(o, pos)) {
        TreeAnimSet_Request(&gFieldObjectManager->treeAnimSet, o, &pos.x, mode, 0);
    }
}
}
}

namespace ns_0221cb54 {
extern "C" {
void Tree_RequestChop(s32 o, P2 pos, s32 mode) {
    if (Tree_BeginReaction(o, pos)) {
        BlockMap *g = TownBlockMap_Get();
        s32 x = pos.x;
        s32 z = pos.z;
        s32 hx = x >> 4;
        s32 hz = z >> 4;
        BlockMap_GetItemPtr(g, hx, hz, x - (hx << 4), z - (hz << 4), 0);
        switch (mode) {
        case 4:
            if (Tree_IsPendingStump(o, pos) == 0) {
                mode = 0;
            } else {
                if (PendingUnit_FindForAid(o, pos, 0) >= 0) {
                    s32 x2 = pos.x;
                    s32 xx = x2 - 2;
                    s32 z2 = pos.z;
                    s32 hx2 = xx >> 4;
                    s32 hz2 = z2 >> 4;
                    u16 *cell = BlockMap_GetItemPtr(g, hx2, hz2, xx - (hx2 << 4), z2 - (hz2 << 4), 0);
                    if (cell != 0 && Town_IsSaplingBlocker(cell)) {
                        mode = 6;
                    } else if (x2 - 2 < 0x10) {
                        mode = 6;
                    }
                } else {
                    mode = 0;
                }
            }
            break;
        case 5:
            if (Tree_IsPendingStump(o, pos) == 0) {
                mode = 0;
            } else {
                if (PendingUnit_FindForAid(o, pos, 0) >= 0) {
                    s32 x2 = pos.x;
                    s32 xx = x2 + 2;
                    s32 z2 = pos.z;
                    s32 hx2 = xx >> 4;
                    s32 hz2 = z2 >> 4;
                    u16 *cell = BlockMap_GetItemPtr(g, hx2, hz2, xx - (hx2 << 4), z2 - (hz2 << 4), 0);
                    if (cell != 0 && Town_IsSaplingBlocker(cell)) {
                        mode = 7;
                    } else if (x2 + 2 >= 0x50) {
                        mode = 7;
                    }
                } else {
                    mode = 0;
                }
            }
            break;
        }
        TreeAnimSet_Request(&gFieldObjectManager->treeAnimSet, o, &pos.x, mode, 1);
    }
}
}
}

namespace ns_0221cb54 {
extern "C" {
void FieldItemFx_StartStrikeResult(P2 pos) {
    u32 t = gCommManager->myAid;
    s32 idx = PendingUnit_FindBySlot(t, 4);
    if (idx >= 0) {
        PendingUnit *r = PendingUnit_Get(idx);
        FieldItemFx_StartStrikeShake(t, r->item, pos);
    }
    idx = PendingUnit_FindBySlot(t, 3);
    if (idx >= 0) {
        PendingUnit *r = PendingUnit_Get(idx);
        V3 v;
        FieldPos_FromUnitCenter(&v, pos.x, pos.z);
        Col c;
        c.c = r->unit.v;
        u16 tt = c.c;
        c.b = tt;
        c.a = tt;
        FieldItemFx_StartStrikeEject(t, r->item, P2((s32)c.a >> 8, c.b & 0xff), v);
    }
}
}
}

namespace ns_0221c220 {
extern "C" {
void Field_SetUnitItem(s32 x, s32 y, u32 tile, s32 flag) {
    BlockMap *g;
    volatile u16 t[1];
    Scene_InTown();
    g = gSceneBlockMap;
    if (g != 0) {
        t[0] = 0xfff1;
        t[0] = tile;
        BlockMap_SetItemAtUnit(g, (u16 *)t, x, y, 0);
        if (tile != 0xfff1 && flag != 0) {
            BlockMap_SetBuriedAtUnit(g, x, y);
        } else {
            BlockMap_ClearBuriedAtUnit(g, x, y);
        }
    }
}
}
}

namespace ns_0221c220 {
extern "C" {
void TreeLeafFx_Free(Rec *r) {
    r->treeType = 3;
}
}
}

namespace ns_0221c220 {
extern "C" {
void TreeLeafFx_SetRecord(Rec *r, s32 a1, s32 a2, s32 a3, u32 c, VecFx32 *pos, s32 flag) {
    r->treeType = a1;
    r->isLeaves = a2;
    r->frame = 0;
    r->posX = pos->x;
    r->posY = pos->y;
    r->posZ = pos->z;
    r->animKind = a3;
    if (flag == 0) {
        r->motionType = data_ov003_022335c0[a3];
    } else {
        r->motionType = 4;
    }
    r->treeStage = c;
}
}
}

namespace ns_0221c220 {
extern "C" {
const PRec *TreeLeafFx_GetParams(Rec *r) {
    const PRec *res = 0;
    const PRec ****a = data_ov003_02232928[r->treeType];
    if (a != 0) {
        const PRec ***b = a[r->motionType];
        if (b != 0) {
            const PRec **c = b[r->treeStage];
            if (c != 0) res = c[r->isLeaves];
        }
    }
    return res;
}
}
}

namespace ns_0221c220 {
extern "C" {
void TreeLeafFx_ApplyParams(Rec *r, Tgt *t) {
    const PRec *p = TreeLeafFx_GetParams(r);
    t->genInterval = p->genInterval;
    t->particleLife = p->particleLife;
    t->initVelMagAxis = p->initVelMagAxis;
    r->lifeTimer = p->lifeTime;
}
}
}

namespace ns_0221c220 {
extern "C" {
void TreeLeafFx_UpdatePos(Rec *r, Tgt *t) {
    s32 sc = data_ov003_0222f564[r->treeType][r->treeStage];
    s32 x, y, z, d, e;
    s32 ang, lim;
    StackPad8 pad;
    switch (r->motionType) {
    case 3:
        switch (r->animKind - 4) {
        case 0:
            lim = 0x10;
            ang = -0x4000;
            break;
        case 1:
            lim = 0x10;
            ang = 0x4000;
            break;
        case 2:
            lim = 9;
            ang = -0x2000;
            break;
        case 3:
            lim = 9;
            ang = 0x2000;
            break;
        }
        e = r->frame;
        if (e < lim) ang = (s16)(DS_DIV(e * ang, lim));
        d = ((u16)ang >> 4) * 2;
        x = r->posX + ((sc * data_02135f44[d]) >> 12);
        y = r->posY + ((sc * data_02135f44[d + 1]) >> 12);
        z = r->posZ;
        break;
    case 4:
        switch (r->animKind - 4) {
        case 0:
            ang = -0x4000;
            break;
        case 1:
            ang = 0x4000;
            break;
        case 2:
            ang = -0x2000;
            break;
        case 3:
            ang = 0x2000;
            break;
        }
        d = ((u16)ang >> 4) * 2;
        x = r->posX + ((sc * data_02135f44[d]) >> 12);
        y = r->posY + ((sc * data_02135f44[d + 1]) >> 12);
        z = r->posZ;
        break;
    default:
        x = r->posX;
        y = r->posY + sc;
        z = r->posZ;
        break;
    }
    t->posX = x + t->resource->p_base->pos.x;
    t->posY = y + t->resource->p_base->pos.y;
    t->posZ = z + t->resource->p_base->pos.z;
}
}
}

namespace ns_0221c220 {
extern "C" {
void TreeLeafFx_OnEffectInit(EffectEmitterEntry *self) {
    s32 i;
    Tgt *t = (Tgt *)self->emitter;
    Rec *r;
    EffectCb_InitAtPos(self);
    self->userIndex = sTreeLeafFx.spawnIndex;
    i = sTreeLeafFx.spawnIndex;
    if (i < 0) i = 0;
    r = &sTreeLeafFx.records[i];
    TreeLeafFx_UpdatePos(r, t);
    self->userIndex = i;
    TreeLeafFx_ApplyParams(r, t);
    t->radius = data_ov003_0222f594[sTreeLeafFx.records[i].treeType][r->treeStage];
    t->tintVariant = r->tintVariant;
}
}
}

namespace ns_0221c220 {
extern "C" {
s32 TreeLeafFx_OnEffectStep(EffectEmitterEntry *self) {
    Rec *rec = &sTreeLeafFx.records[self->userIndex];
    s32 t = rec->lifeTimer;
    if (t - 1 < 0) rec->treeType = 3;
    rec->lifeTimer = t - 1;
    rec->frame = rec->frame + 1;
    return TreeLeafFx_OnEffectUpdate(self);
}
}
}

namespace ns_0221c220 {
extern "C" {
s32 TreeLeafFx_OnEffectUpdate(EffectEmitterEntry *self) {
    BOOL r = TRUE;
    EffectCb_UpdateTint(self);
    Rec *rec = &sTreeLeafFx.records[self->userIndex];
    if (rec->treeType == 3) {
        r = FALSE;
    } else {
        TreeLeafFx_UpdatePos(rec, (Tgt *)self->emitter);
    }
    return r;
}
}
}

namespace ns_0221c220 {
extern "C" {
Rec *TreeLeafFx_FindFree(Set *self) {
    s32 i = 0;
    goto test;
loop:
    if (self->records[i].treeType == 3) {
        self->spawnIndex = i;
        return &self->records[i];
    }
    i++;
test:
    if (i < 2) goto loop;
    return 0;
}
}
}

namespace ns_0221c220 {
extern "C" {
void TreeLeafFx_GetTreeType(Set *self, Rec *r, s32 *o1, s32 *o2, VecFx32 *out, u16 *tile, Pos *pos) {
    u32 c = TownState_GetSeasonPeriod();
    s32 k;
    BOOL f = FALSE;
    u32 t = *tile;
    if (t >= 0x26 && t <= 0x2a) f = TRUE;
    if (f) goto grass;
    if (t >= 0x2b && t <= 0x2e) goto grass;
    if (t == 0x67) {
    grass:
        k = (pos->x ^ pos->z) & 1;
        *o1 = 0;
        r->tintVariant = k;
        *o2 = 0x22;
        if ((data_ov003_0222f534 + k * 0x17)[c] != 0) *o2 = 0x24;
    } else if ((t >= 0x5d && t <= 0x61) || (t >= 0x62 && t <= 0x65) || t == 0x6d || t == 0x6b) {
        *o1 = 1;
        r->tintVariant = 3;
        *o2 = 0x25;
    } else if ((t >= 0xc8 && t <= 0xcf) || (t >= 0xd0 && t <= 0xd3)) {
        *o1 = 2;
        r->tintVariant = 3;
        *o2 = 0x27;
    } else {
        *o1 = 0;
        r->tintVariant = 2;
        *o2 = 0x22;
    }
    FieldPos_FromUnitCenter(out, pos->x, pos->z);
}
}
}

namespace ns_0221c220 {
extern "C" {
Rec *TreeLeafFx_SpawnSeasonal(Set *self, s32 p1, u32 p2, u16 *p3, Pos *p4, s32 p5) {
    TreeLeafFxCbs q = data_ov003_02232b48;
    Rec *r = 0;
    if (data_ov003_0222f4c8[TownState_GetSeasonPeriod()] != 0) {
        r = TreeLeafFx_FindFree(self);
        if (r != 0) {
            s32 v14, v18;
            Pos pos;
            VecFx32 out;
            VecFx32 out2;
            pos.x = p4->x;
            pos.z = p4->z;
            TreeLeafFx_GetTreeType(self, r, &v14, &v18, &out, p3, &pos);
            out2 = out;
            TreeLeafFx_SetRecord(r, v14, 0, p1, p2, &out2, p5);
            if (TreeLeafFx_GetParams(r) != 0) {
                EffectSpl_CreateTracked(data_ov003_0222f298[v14], &out, 0, q.emitters);
            } else {
                r->treeType = 3;
            }
            self->spawnIndex = -1;
        }
    }
    return r;
}
}
}

namespace ns_0221c220 {
extern "C" {
Rec *TreeLeafFx_SpawnLeaves(Set *self, s32 p1, u32 p2, u16 *p3, Pos *p4, s32 p5) {
    TreeLeafFxCbs q = data_ov003_02232b78;
    Rec *r = TreeLeafFx_FindFree(self);
    if (r != 0) {
        s32 v14, v18;
        Pos pos;
        VecFx32 out;
        VecFx32 out2;
        pos.x = p4->x;
        pos.z = p4->z;
        TreeLeafFx_GetTreeType(self, r, &v14, &v18, &out, p3, &pos);
        out2 = out;
        TreeLeafFx_SetRecord(r, v14, 1, p1, p2, &out2, p5);
        if (TreeLeafFx_GetParams(r) != 0) {
            EffectSpl_CreateTracked(v18, &out, 0, q.emitters);
        } else {
            r->treeType = 3;
        }
        self->spawnIndex = -1;
    }
    return r;
}
}
}

namespace ns_0221c220 {
extern "C" {
void TreeLeafFx_Init(Set *self) {
    Rec *r;
    s32 i;
    for (r = self->records, i = 0; i < 2; r++, i++) {
        TreeLeafFx_Free(r);
    }
    self->spawnIndex = -1;
}
}
}

namespace ns_0221c220 {
extern "C" {
Elem2 *TreeAnim_Construct(Elem2 *self) {
    s32 *p;
    new (self->animModel) AnimModel();
    self->unit.x = 0;
    self->unit.z = 0;
    new (self->seEmitter) SndSeEmitter();
    p = self->unk_120;
    do {
        p[0] = 0;
        p[1] = 0;
        p += 2;
    } while (p != &self->unk_120[6]);
    return self;
}
}
}

namespace ns_0221c220 {
extern "C" {
Big *TreeAnimSet_Construct(Big *self) {
    func_02135714(self, 12, sizeof(::TreeAnim), (void *)TreeAnim_Construct, (void *)TreeAnim_Destruct);
    func_02135714(self->cedarAnims, 4, sizeof(::TreeAnim), (void *)TreeAnim_Construct, (void *)TreeAnim_Destruct);
    func_02135714(self->litCedarAnims, 3, sizeof(::TreeAnim), (void *)TreeAnim_Construct, (void *)TreeAnim_Destruct);
    func_02135714(self->palmAnims, 4, sizeof(::TreeAnim), (void *)TreeAnim_Construct, (void *)TreeAnim_Destruct);
    {
        TreeAnimRequest *s = self->requests;
        do {
            s->unit.x = 0;
            s->unit.z = 0;
            s++;
        } while (s != &self->requests[5]);
    }
    return self;
}
}
}

TreeAnimSet::~TreeAnimSet() {
    using namespace ns_0221c220;
    TreeAnimSet *self = this;
    func_021355f0(self->palmAnims, 4, sizeof(::TreeAnim), (void *)TreeAnim_Destruct);
    func_021355f0(self->litCedarAnims, 3, sizeof(::TreeAnim), (void *)TreeAnim_Destruct);
    func_021355f0(self->cedarAnims, 4, sizeof(::TreeAnim), (void *)TreeAnim_Destruct);
    func_021355f0(self, 12, sizeof(::TreeAnim), (void *)TreeAnim_Destruct);
}

namespace ns_0221c220 {
extern "C" {
Elem2 *TreeAnim_Destruct(Elem2 *self) {
    ((SndSeEmitter *)(self->seEmitter))->SndSeEmitter::~SndSeEmitter();
    ((AnimModel *)(self->animModel))->AnimModel::~AnimModel();
    return self;
}
}
}

namespace ns_0221c220 {
extern "C" {
void TreeAnimSet_Init(Big *self) {
    s32 i, j;
    TreeAnimSet_LoadAnims(self);
    for (i = 0; i < 5; i++) self->requests[i].active = 0;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 4; j++) TreeAnim_Init(&self->treeAnims[i][j]);
    }
    for (i = 0; i < 4; i++) {
        TreeAnim_Init(&self->cedarAnims[i]);
        TreeAnim_Init(&self->palmAnims[i]);
    }
}
}
}

namespace ns_0221c220 {
extern "C" {
void TreeAnimSet_Update(Big *self) {
    s32 i, j;
    for (i = 0; i < 5; i++) {
        if (self->requests[i].active != 0) TreeAnimSet_ProcessRequest(self, &self->requests[i]);
    }
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 4; j++) TreeAnim_Update(&self->treeAnims[i][j]);
    }
    for (i = 0; i < 4; i++) {
        TreeAnim_Update(&self->cedarAnims[i]);
        TreeAnim_Update(&self->palmAnims[i]);
    }
}
}
}

namespace ns_0221c220 {
extern "C" {
void TreeAnimSet_Draw(Big *self) {
    s32 i, j;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 4; j++) TreeAnim_Draw(&self->treeAnims[i][j]);
    }
    for (i = 0; i < 4; i++) {
        TreeAnim_Draw(&self->cedarAnims[i]);
        TreeAnim_Draw(&self->palmAnims[i]);
    }
}
}
}

namespace ns_0221c220 {
extern "C" {
void TreeAnimSet_Release(Big *self) {
    s32 i, j;
    TreeAnimSet_FreeAnims(self);
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 4; j++) TreeAnim_Release(&self->treeAnims[i][j]);
    }
    for (i = 0; i < 4; i++) {
        TreeAnim_Release(&self->cedarAnims[i]);
        TreeAnim_Release(&self->palmAnims[i]);
    }
}
}
}

namespace ns_0221c220 {
extern "C" {
TreeAnim *TreeAnimSet_GetInstance(Big *self, u16 *p, s32 a, s32 b) {
    TreeAnimSet *s = self;
    BOOL f = FALSE;
    u32 t = *p;
    if (t >= 0x2f && t <= 0x56) f = TRUE;
    if (f) return &s->treeAnims[2][0] + a;
    if (t >= 0x57 && t <= 0x5b) return &s->treeAnims[b][0] + a;
    if (t == 0x69) return &s->treeAnims[2][0] + a;
    if (t >= 0x6a && t <= 0x6c) return s->cedarAnims + a;
    if (t == 0x6d) return s->cedarAnims + a;
    if (t >= 0x5d && t <= 0x61) return s->cedarAnims + a;
    if (t >= 0xc8 && t <= 0xcf) return s->palmAnims + a;
    return &s->treeAnims[b][0] + a;
}
}
}

namespace ns_0221b8bc {
extern "C" {
void TreeAnimSet_ProcessRequest(TreeAnimSet *a, TreeAnimRequest *o) {
    s32 w;
    u16 *cell;
    s32 x, z, n;
    FieldUnitPos p;
    s32 *q;
    s32 hx, hy;
    TreeAnim *e;
    BlockMap *g = TownBlockMap_Get();
    if (g == 0) {
        TreeAnimRequest_Resolve(o);
        return;
    }
    q = &o->unit.x;
    x = q[0];
    z = q[1];
    hx = x >> 4;
    hy = z >> 4;
    cell = BlockMap_GetItemPtr(g, hx, hy, x - (hx << 4), z - (hy << 4), 0);
    if (cell == 0) {
        TreeAnimRequest_Resolve(o);
        return;
    }
    q = &o->unit.x;
    x = q[0];
    z = q[1];
    w = o->animKind;
    n = Item_GetTreeStage(cell);
    if (n <= 0 || n > 4) {
        TreeAnimRequest_Resolve(o);
        return;
    }
    e = TreeAnimSet_GetInstance(a, cell, n - 1, (x ^ z) & 1);
    if (e == 0) {
        TreeAnimRequest_Resolve(o);
        return;
    }
    if (e->active == 1) {
        switch (e->animKind) {
        case 4:
        case 5:
        case 6:
        case 7:
            TreeAnimRequest_Resolve(o);
            return;
        default:
            break;
        }
    }
    s32 d = o->isChop;
    p.x = x;
    p.z = z;
    TreeAnim_Start(e, o->sessionSlot, p, w, n - 1, d);
    o->active = 0;
}
}
}

namespace ns_0221b8bc {
extern "C" {
void TreeAnimSet_Request(TreeAnimSet *a, s32 id, s32 *pos, s32 c, s32 d) {
    BlockMap *g = TownBlockMap_Get();
    if (g != 0) {
        s32 x = pos[0];
        s32 z = pos[1];
        s32 hx = x >> 4;
        s32 hy = z >> 4;
        u16 *cell = BlockMap_GetItemPtr(g, hx, hy, x - (hx << 4), z - (hy << 4), 0);
        if (cell != 0) {
            TreeAnimRequest *e;
            s32 f1, f2;
            u32 v;
            Vec2 t;
            if (gCommManager->isOnline() == 0) {
                id = 0;
            }
            e = &a->requests[id];
            if (e->active != 0) {
                f1 = 0;
                v = *cell;
                if (v >= 0x2f && v <= 0x56) {
                    f1 = 1;
                }
                if (f1 == 0 && v >= 0xc8 && v <= 0xcf) {
                    goto l_ae;
                }
                if (Item_IsTreeGrown(cell) != 0) {
                    goto set;
                }
            l_ae:
                f1 = 1;
                v = *cell;
                if (v != 0x67 && v != 0x6b) {
                    f1 = 0;
                }
                if (f1 != 0) {
                    if (Town_CanReleaseBees() != 0) {
                        goto set;
                    }
                }
                f2 = 1;
                f1 = 0;
                v = *cell;
                if (v >= 0x66 && v <= 0x68) {
                    f1 = f2;
                }
                if (f1 == 0) {
                    if (v < 0x6a || v > 0x6c) {
                        f2 = 0;
                    }
                }
                if (f2 != 0) {
                    goto set;
                }
                if (v >= 0x57 && v <= 0x5b) {
                    goto set;
                }
                if (((volatile s32 *)pos)[0] == e->unit.x && ((volatile s32 *)pos)[1] == e->unit.z) {
                    return;
                }
                t.x = pos[0];
                t.y = pos[1];
                PendingUnit_ApplyAt(&t, 0);
                return;
            }
        set:
            e->active = 1;
            e->sessionSlot = id;
            s32 tz = pos[1];
            e->unit.x = pos[0];
            e->unit.z = tz;
            e->animKind = c;
            e->isChop = d;
        }
    }
}
}
}

namespace ns_0221b8bc {
extern "C" {
void Tree_KeepShaking(s32 id, s32 *pos) {
    BlockMap *g = TownBlockMap_Get();
    if (g != 0) {
        s32 x = pos[0];
        s32 z = pos[1];
        s32 hx = x >> 4;
        s32 hy = z >> 4;
        u16 *cell = BlockMap_GetItemPtr(g, hx, hy, x - (hx << 4), z - (hy << 4), 0);
        if (cell != 0) {
            s32 n;
            TreeAnim *e;
            if (gCommManager->isOnline() == 0) {
                id = 0;
            }
            n = Item_GetTreeStage(cell);
            if (n > 0 && n <= 4) {
                e = TreeAnimSet_GetInstance(&gFieldObjectManager->treeAnimSet, cell, n - 1, (pos[0] ^ pos[1]) & 1);
                if (e != 0 && pos[0] == e->unit.x && pos[1] == e->unit.z && id == e->sessionSlot) {
                    e->playMode = 0;
                }
            }
        }
    }
}
}
}

namespace ns_0221b8bc {
extern "C" {
void TreeAnim_Init(TreeAnim *o) {
    ((CachedModel *)o->animModel)->allocJointRecord(sFieldObjectAnimHeap);
    ((AnimModel *)o->animModel)->allocAnmObj(sFieldObjectAnimHeap);
    o->animKind = 8;
    ((Model *)(o->animModel))->setCallback(TreeAnim_ModelCallback, 2, 2, o, 0);
    ((SndSeEmitter *)(o->seEmitter))->callInit();
    TreeAnim_Reset(o);
}
}
}

namespace ns_0221b8bc {
extern "C" {
void TreeAnim_Update(TreeAnim *o) {
    if (o->active == 1) {
        ((AnimModel *)(o->animModel))->stepAnim();
        if (((AnimFrameCtrl *)(o->frameCtrl))->isFinished()) {
            TreeAnim_Reset(o);
        } else {
            s32 r5 = 0;
            s32 r6 = gCommManager->localSlot;
            switch (o->animKind) {
            case 4:
            case 5:
                if (o->animFrame.b >= 0x11) {
                    o->alpha = o->alpha - 0x2955;
                    if (o->alpha < 0) {
                        o->alpha = 0;
                    }
                }
                switch (o->leafSpawnCount) {
                case 0:
                    if (o->animFrame.b >= 4) {
                        r5 = 1;
                    }
                    break;
                case 1:
                    if (o->animFrame.b >= 0x14) {
                        r5 = 1;
                        Snd_SeEmitterPlayOneShot((SndSeEmitter *)o->seEmitter, 0x7ea, 0x7f, 0);
                    }
                    break;
                }
                break;
            case 6:
            case 7:
                if (o->animFrame.b >= 0x11) {
                    o->alpha = o->alpha - 0x2627;
                    if (o->alpha < 0) {
                        o->alpha = 0;
                    }
                }
                switch (o->leafSpawnCount) {
                case 0:
                    if (o->animFrame.b >= 4) {
                        r5 = 1;
                    }
                    break;
                case 1:
                    if (o->animFrame.b >= 0xd) {
                        r5 = 1;
                        Snd_SeEmitterPlayOneShot((SndSeEmitter *)o->seEmitter, 0x7ea, 0x7f, 0);
                    }
                    break;
                }
                break;
            case 2:
                if (o->leafSpawnCount == 0 && o->animFrame.b >= 4) {
                    r5 = 1;
                } else {
                    if (o->leafRecord) {
                        o->leafRecord->lifeTimer = 0x1a;
                    }
                    if (o->seasonalRecord) {
                        o->seasonalRecord->lifeTimer = 0x1a;
                    }
                }
                break;
            case 0:
                if (o->leafSpawnCount == 0 && o->animFrame.b >= 4) {
                    r5 = 1;
                }
                break;
            case 3:
                break;
            case 1:
            default:
                if (o->leafSpawnCount == 0 && o->animFrame.b >= 4) {
                    r5 = 1;
                }
                break;
            }
            NNS_G3dMdlSetMdlAlphaAll(o->resMdl, o->alpha >> 12);
            NNS_G3dMdlSetMdlPolygonIDAll(o->resMdl, r6 + 0x33);
            if (r5 != 0) {
                BlockMap *g = TownBlockMap_Get();
                if (g != 0) {
                    s32 x = o->unit.x;
                    s32 z = o->unit.z;
                    s32 hx = x >> 4;
                    s32 hy = z >> 4;
                    u16 *cell = BlockMap_GetItemPtr(g, hx, hy, x - (hx << 4), z - (hy << 4), 0);
                    if (cell != 0) {
                        BOOL f3 = TRUE, f2 = TRUE, f1 = TRUE, f0 = FALSE;
                        u32 v = *cell;
                        s32 n;
                        if (v >= 0x2b && v <= 0x2e) {
                            f0 = TRUE;
                        }
                        if (!f0) {
                            if (v < 0xff || v > 0x102) {
                                f1 = FALSE;
                            }
                        }
                        if (!f1) {
                            if (v < 0x62 || v > 0x65) {
                                f2 = FALSE;
                            }
                        }
                        if (!f2) {
                            if (v < 0xd0 || v > 0xd3) {
                                f3 = FALSE;
                            }
                        }
                        if (f3) {
                            n = Item_GetStumpSize(cell);
                        } else {
                            n = Item_GetTreeStage(cell);
                        }
                        if (n > 0) {
                            VecXZ a, b;
                            a.x = o->unit.x;
                            a.z = o->unit.z;
                            o->leafRecord = (TreeLeafFxEntry *)TreeLeafFx_SpawnLeaves(&sTreeLeafFx, o->animKind, n - 1, cell, &a, o->leafSpawnCount);
                            b.x = o->unit.x;
                            b.z = o->unit.z;
                            o->seasonalRecord = (TreeLeafFxEntry *)TreeLeafFx_SpawnSeasonal(&sTreeLeafFx, o->animKind, n - 1, cell, &b, o->leafSpawnCount);
                            o->leafSpawnCount = o->leafSpawnCount + 1;
                        }
                    }
                }
            }
        }
        VecFx32 v3;
        v3.x = o->posX;
        v3.y = o->posY;
        v3.z = o->posZ;
        ((SndSeEmitter *)(o->seEmitter))->callUpdateRelative(&v3);
    }
    o->playMode = 1;
}
}
}

namespace ns_0221b8bc {
extern "C" {
void TreeAnim_Draw(TreeAnim *o) {
    if (o->active == 1) {
        volatile Unk_ov003_0221b8bc_Col2 l;
        VecFx32 v;
        s32 r = WorldCurve_ToCurved(&v, (VecFx32 *)&o->posX);
        Mtx43_SetTranslate(&data_021f47e0, v.x, v.y, v.z);
        Mtx43_RotateX(&data_021f47e0, r);
        o->baseMatrix = data_021f47e0;
        ((AnimModel *)(o->animModel))->drawAnimated((void *)(0));
        l.a = SceneLights_GetRoomColor();
        l.b = l.a;
        NNS_G3dMdlSetMdlEmi(o->resMdl, 0, l.b);
    }
}
}
}

namespace ns_0221b8bc {
extern "C" {
void TreeAnim_Release(TreeAnim *o) {
    TreeAnim_Reset(o);
    ((CachedModel *)(o->animModel))->release();
    ((SndSeEmitter *)(o->seEmitter))->callStop();
}
}
}

namespace ns_0221b8bc {
extern "C" {
void TreeAnim_Begin(TreeAnim *o, s32 flag) {
    Vec2 a, b, c;
    s32 hx, hy, x, z;
    u16 *cell;
    BlockMap *g = TownBlockMap_Get();
    x = o->unit.x;
    z = o->unit.z;
    hx = x >> 4;
    hy = z >> 4;
    cell = BlockMap_GetItemPtr(g, hx, hy, x - (hx << 4), z - (hy << 4), 0);
    switch (o->animKind) {
    case 1:
        break;
    case 2:
    case 3:
        a.x = o->unit.x;
        a.y = o->unit.z;
        if (PendingUnit_Find(&a, 0) >= 0) {
            TreeAnim_DropItems(o, cell);
            b.x = o->unit.x;
            b.y = o->unit.z;
            PendingUnit_ApplyAtIfAid(&b, o->sessionSlot, 0);
        }
        break;
    default:
        TreeAnim_DropItems(o, cell);
        c.x = o->unit.x;
        c.y = o->unit.z;
        PendingUnit_ApplyAtIfAid(&c, o->sessionSlot, 0);
        break;
    }
    if (flag == 0) {
        switch (o->animKind) {
        case 0:
        case 2:
        case 3:
            Snd_SeEmitterPlayOneShot((SndSeEmitter *)o->seEmitter, 0x7dc, 0x7f, 0);
            break;
        case 1:
            Snd_SeEmitterPlayOneShot((SndSeEmitter *)o->seEmitter, 0x7d9, 0x7f, 0);
            break;
        }
    } else {
        switch (o->animKind) {
        case 4:
        case 5:
        case 6:
        case 7:
            Snd_SeEmitterPlayOneShot((SndSeEmitter *)o->seEmitter, 0x855, 0x7f, 0);
            break;
        default:
            Snd_SeEmitterPlayOneShot((SndSeEmitter *)o->seEmitter, 0x7dd, 0x7f, 0);
            break;
        }
    }
    o->active = 1;
}
}
}

namespace ns_0221b8bc {
extern "C" {
s32 Tree_GetDropSide(s32 aid, VecFx32 *p) {
    s32 r = 0;
    void *q = PlayerActor_GetActor(Field_AidOrLocal(aid));
    if (q != 0) {
        if (*(s32 *)(ACTOR_POS_AT(q)) < p->x) {
            r = 1;
        }
    }
    return r;
}
}
}

namespace ns_0221b8bc {
extern "C" {
void Tree_DropFruit(u16 *cell, s32 id, s32 *pos) {
    volatile Unk_ov003_0221b8bc_Col col;
    VecXZ xy;
    FieldUnitPos xy2;
    VecFx32 base, cur, cp;
    VecFx32 *tbl;
    s32 n, arg, i;
    FieldPos_FromUnitCenter(&base, pos[0], pos[1]);
    switch (*cell) {
    case 0xcc:
        tbl = data_ov003_0223291c[2];
        n = 2;
        arg = 0x1548;
        break;
    case 0x5b:
        tbl = data_ov003_0223291c[0];
        n = 3;
        arg = 0x14b8;
        break;
    default:
        tbl = data_ov003_0223291c[0];
        n = 3;
        arg = sTreeFruitItems[Item_GetFruitTreeFruit(cell)];
        break;
    }
    xy.x = 0;
    xy.z = 0;
    for (i = 0; i < n; i++) {
        s32 r = PendingUnit_FindBySlot(id, i);
        cur.x = base.x + tbl->x;
        cur.y = base.y + tbl->y;
        cur.z = base.z + tbl->z;
        if (r >= 0) {
            u16 v = PendingUnit_Get(r)->unit.v;
            col.c = v;
            u16 t = col.c;
            col.b = t;
            col.a = t;
            xy.x = (s32)col.a >> 8;
            xy.z = col.b & 0xff;
            r = 0;
        } else {
            FieldPos_ToUnit(&xy.x, &xy.z, &cur);
            r = 1;
        }
        cp = cur;
        xy2.x = xy.x;
        xy2.z = xy.z;
        FieldItemFx_StartTreeDrop(id, arg, xy2, VecFx32Copy(cp.x, cp.y, cp.z), r, i);
        tbl++;
    }
}
}
}

namespace ns_0221b8bc {
extern "C" {
void Tree_DropBeeHive(s32 aid, s32 *p) {
    FieldUnitPos a;
    VecFx32 b;
    Unk_ov003_0221b8bc_V3D c;
    VecFx32 e;
    VecFx32 d;
    if (gCommManager->isOnline() == 0 && PlayerData_GetCurrent()->testFlag(1) == 0) {
        FieldPos_FromUnitCenter(&b, p[0], p[1]);
        VecFx32 *t = data_ov003_0223291c[0];
        t = t + Tree_GetDropSide(aid, &b);
        Vec_Add(&e, &b, t);
        c.x = e.x;
        c.y = e.y;
        c.z = e.z;
        d = c;
        a.x = p[0];
        a.z = p[1];
        FieldItemFx_StartBeeHiveDrop(aid, 0x1569, a, VecFx32Copy(d.x, d.y, d.z));
    }
}
}
}

namespace ns_0221aed4 {
extern "C" void Tree_DropSpecial(u16 *cell, s32 a, P2 p)
{
    s32 rec = PendingUnit_FindBySlot((u8)a, 0);
    if (rec >= 0) {
        s32 fl = 0;
        VecFx32 *tbl;
        BOOL f = FALSE;
        u32 t = *cell;
        if (t >= 0x66 && t <= 0x68) f = TRUE;
        if (f) {
            tbl = data_ov003_0223291c[0];
            if (t == 0x66) fl = 1;
        } else if (t >= 0x6a && t <= 0x6c) {
            tbl = data_ov003_0223291c[1];
            if (t == 0x6a) fl = 1;
        }
        VecFx32 d;
        FieldPos_FromUnitCenter(&d, p.x, p.z);
        s32 idx = Tree_GetDropSide(a, &d);
        tbl = tbl + idx;
        PendingUnit *e = PendingUnit_Get(rec);
        VecFx32 pos;
        s32 px = d.x + tbl->x;
        *(volatile s32 *)&pos.x = px;
        s32 py = d.y + tbl->y;
        *(volatile s32 *)&pos.y = py;
        s32 pz = d.z + tbl->z;
        *(volatile s32 *)&pos.z = pz;
        volatile u16 tt[3];
        tt[2] = e->unit.v;
        u16 vv = tt[2];
        tt[1] = vv;
        tt[0] = vv;
        s32 hi = tt[0] >> 8;
        s32 lo = tt[1] & 0xff;
        if (fl == 0) {
            FieldItemFx_StartTreeDrop(a, e->item, P2(hi, lo), V3(px, py, pz), 0, 0);
        } else {
            FieldItemFx_StartTreeDropFloat(a, e->item, P2(hi, lo), V3(px, py, pz), 0);
        }
    }
}
}

namespace ns_0221aed4 {
extern "C" void Tree_DropItems(u16 *cell, s32 a, P2 p)
{
    BOOL f = FALSE;
    u32 t = *cell;
    if (t >= 0x2f && t <= 0x56) f = TRUE;
    if (f || (t >= 0xc8 && t <= 0xcf) || (t >= 0x57 && t <= 0x5b)) {
        if (Item_IsTreeGrown(cell) != 0) {
            Tree_DropFruit(cell, a, &p.x);
        }
    } else if (t == 0x67 || t == 0x6b) {
        if (Town_CanReleaseBees() != 0) {
            Tree_DropBeeHive(a, &p.x);
        }
    } else if ((t >= 0x66 && t <= 0x68) || (t >= 0x6a && t <= 0x6c)) {
        Tree_DropSpecial(cell, a, p);
    }
}
}

namespace ns_0221aed4 {
extern "C" void TreeAnim_DropItems(TreeAnim *self, void *cell)
{
    Tree_DropItems((u16 *)cell, (s32)self->sessionSlot, P2(self->unit));
}
}

namespace ns_0221aed4 {
extern "C" void TreeAnim_Start(TreeAnim *self, s32 a, P2 p, s32 kind, s32 idx, s32 last)
{
    TownBlockMap_Get();
    BOOL f = TRUE;
    TreeAnimSet *set = &gFieldObjectManager->treeAnimSet;
    if (kind == 2) f = FALSE;
    ((BlendAnimModel *)(&self->animModel))->initAnim(set->anims[kind][idx], f, 0x1000, 0, 0);
    if (self->animKind == 8) {
        ((AnimModel *)(&self->animModel))->attachAnim();
    }
    self->animKind = kind;
    self->sessionSlot = a;
    self->unit.x = p.x;
    self->unit.z = p.z;
    VecFx32CtorDtor t;
    FieldPos_FromUnitCenter(&t, p.x, p.z);
    self->posX = t.x;
    self->posY = t.y;
    self->posZ = t.z;
    self->alpha = 0x1f000;
    self->leafSpawnCount = 0;
    TreeAnim_Begin(self, last);
}
}

namespace ns_0221aed4 {
extern "C" void TreeAnim_Reset(TreeAnim *self)
{
    self->active = 0;
    self->alpha = 0;
    self->unit.x = -1;
    self->unit.z = -1;
    for (s32 i = 0; i < 3; i++) {
        self->unk_138[i] = -1;
    }
    self->leafRecord = 0;
    self->seasonalRecord = 0;
}
}

namespace ns_0221aed4 {
extern "C" void TreeAnim_ModelCallback(NNSG3dRS *rs)
{
    TreeAnim *s = (TreeAnim *)rs->pRenderObj->ptrUser;
    if (rs->c[1] == 2) {
        switch (s->animKind) {
        case 0:
        case 1:
        case 2:
        case 3:
            *(s32 *)rs->pVisAnmResult = 0;
            break;
        }
    }
}
}

namespace ns_0221aed4 {
extern "C" void TreeAnimSet_LoadAnims(TreeAnimSet *set)
{
    s32 i = 0;
    for (; i < 8; i++) {
        s32 j = 0;
        for (; j < 4; j++) {
            char *v = TreeAnimSet_GetAnimPath(set, i, j);
            if (v != NULL) {
                u8 *file = static_cast<u8 *>(File_LoadAlloc(v, sFieldObjectAnimHeap, 4, 0));
                set->animFiles[i][j] = file;
                u8 *r = func_021065dc(file);
                set->anims[i][j] = func_021065f8(r, 0);
            } else {
                set->animFiles[i][j] = NULL;
                set->anims[i][j] = NULL;
            }
        }
    }
}
}

namespace ns_0221aed4 {
extern "C" void TreeAnimSet_FreeAnims(TreeAnimSet *set)
{
    Heap *heap = sFieldObjectAnimHeap;
    s32 i = 0;
    for (; i < 8; i++) {
        s32 j = 0;
        for (; j < 4; j++) {
            if (set->animFiles[i][j] != NULL) {
                Heap_Free(heap, set->animFiles[i][j]);
                set->animFiles[i][j] = NULL;
            }
        }
    }
}
}

namespace ns_0221aed4 {
extern "C" char *TreeAnimSet_GetAnimPath(TreeAnimSet *self, u32 a, u32 b)
{
    return sTreeAnimPaths[a * 4 + b];
}
}

namespace ns_0221aed4 {
extern "C" void TreeAnimRequest_Resolve(TreeAnimRequest *self)
{
    if (self->animKind != 1) {
        BlockMap *g = TownBlockMap_Get();
        if (g != 0) {
            s32 x = self->unit.x;
            s32 z = self->unit.z;
            s32 hx = x >> 4;
            s32 hz = z >> 4;
            u16 *c = BlockMap_GetItemPtr(g, hx, hz, x - (hx << 4), z - (hz << 4), 0);
            if (c != 0) {
                Tree_DropItems((u16 *)c, self->sessionSlot, P2(self->unit));
            }
        }
    }
    Vec2 u = {self->unit.x, self->unit.z};
    PendingUnit_ApplyAtIfAid(&u, self->sessionSlot, 0);
    self->active = 0;
}
}

namespace ns_0221aed4 {
extern "C" void FieldItemFx_ReleasePending(FieldItemFx *self)
{
    if (self->pendingApply != 0) {
        self->pendingApply = 0;
        Vec2 t;
        t.x = self->unit.x;
        t.y = self->unit.z;
        PendingUnit_ApplyAt(&t, 0);
    } else if (self->pendingCommit != 0) {
        self->pendingCommit = 0;
        VecXZ t;
        t.x = self->unit.x;
        t.z = self->unit.z;
        PendingUnit_CommitAt(&t, 0);
    }
}
}

namespace ns_0221aed4 {
extern "C" void FieldItemFx_Init(FieldItemFx *self, s32 a, P2 p, V3 pos, s32 kind, u16 w, BlockMap *map, s16 y, s32 z)
{
    self->sessionSlot = a;
    self->active = 1;
    self->kind = kind;
    self->unit.x = p.x;
    self->unit.z = p.z;
    self->startArg = y;
    self->unk_4a = 0;
    self->origin.x = pos.x;
    self->origin.y = pos.y;
    self->origin.z = pos.z;
    self->pendingApply = 1;
    self->pendingCommit = 0;
    self->offset.x = 0;
    self->offset.y = 0;
    self->offset.z = 0;
    {
        Vec2 t;
        t.x = self->unit.x;
        t.y = self->unit.z;
        s32 i = PendingUnit_Find(&t, 0);
        if (i >= 0) {
            self->pendingItem = PendingUnit_Get(i)->item;
        } else {
            self->pendingItem = 0xfff1;
        }
    }
    self->item = w;
    self->step = 0;
    self->frame = 0;
    self->timer = 0;
    self->alpha = 0x1f;
    switch (kind) {
    case 0:
    case 13:
        FieldItemFx_InitToss(self, p, pos);
        break;
    case 1:
        FieldItemFx_InitDigHole(self, p, map, z);
        break;
    case 2:
        FieldItemFx_InitPitfallHole(self, p);
        break;
    case 3:
        FieldItemFx_InitFillHole(self, p, map);
        break;
    case 4:
        FieldItemFx_InitPop(self, &p);
        break;
    case 5:
        FieldItemFx_InitTreeDrop(self, &p, &pos, z);
        break;
    case 6:
        FieldItemFx_InitTreeDropFloat(self, &p, &pos);
        break;
    case 7:
        FieldItemFx_InitBeeHiveDrop(self, &p, &pos);
        break;
    case 8:
        FieldItemFx_InitDigUpTree(self, &p, y);
        break;
    case 9:
        FieldItemFx_InitPlant(self, &p, &pos, w);
        break;
    case 10:
        FieldItemFx_InitHoleShrink(self, &p);
        break;
    case 11:
        FieldItemFx_InitStrikeShake(self, &p);
        break;
    case 12:
        FieldItemFx_InitBalloonDrop(self, &pos);
        break;
    case 14:
        FieldItemFx_InitPitfallClose(self, &p, map);
        break;
    }
}
}

namespace ns_0221aed4 {
extern "C" void FieldItemFx_Finish(FieldItemFx *self)
{
    FieldItemFx_ReleasePending(self);
    self->active = 0;
    self->kind = 0xf;
    self->item = 0xfff1;
    self->pendingItem = 0xfff1;
}
}

namespace ns_0221aed4 {
extern "C" void FieldItemFx_Clear(FieldItemFx *self)
{
    self->active = 0;
    self->kind = 0xf;
    self->item = 0xfff1;
    self->pendingItem = 0xfff1;
}
}

namespace ns_0221aed4 {
extern "C" void FieldItemFx_InitToss(FieldItemFx *self, P2 p, V3 q)
{
    VecFx32 t;
    FieldPos_FromUnitCenter(&t, p.x, p.z);
    s32 a = func_02133150(t.z - q.z, 9);
    self->velocityX = func_02133150(t.x - q.x, 9);
    self->velocityY = 0x1000;
    self->velocityZ = a;
    self->pos.x = q.x;
    self->pos.y = q.y;
    self->pos.z = q.z;
    BOOL f = FALSE;
    self->scale.x = 0;
    self->scale.y = 0x1000;
    self->scale.z = 0;
    volatile u16 w = 0xfff1;
    s32 k = -1;
    w = self->item;
    u32 a1 = w;
    u32 b1 = w;
    if (b1 >= 0x1492 && a1 <= 0x14fd) f = TRUE;
    if (f) {
        if (self->kind == 0xd) {
            u32 h = *(volatile u16 *)&self->item;
            if (h >= 0x149b) k = 0x815;
        } else {
            k = 0x74;
        }
    } else {
        k = 0x75;
    }
    if (k >= 0) {
        Snd_SeEmitterPlayOneShot(&self->seEmitter, k, 0x7f, 0);
    }
}
}

namespace ns_0221aed4 {
extern "C" void FieldItemFx_UpdateToss(FieldItemFx *self)
{
    self->scale.x = self->scale.x + 0x19a;
    if (self->scale.x >= 0x1000) {
        self->scale.x = 0x1000;
    }
    self->scale.z = self->scale.x;
    self->velocityY = self->velocityY - 0x400;
    VEC_Add(&self->pos, &self->velocityX, &self->pos);
    if (self->pos.y < 0) {
        self->pos.y = 0;
        if (self->step == 0) {
            self->step = 1;
            self->velocityX = (self->velocityX * 0x4cd) >> 12;
            self->velocityY = 0x600;
            self->velocityZ = (self->velocityZ * 0x4cd) >> 12;
            volatile u16 t = 0xfff1;
            t = self->item;
            BOOL f = FALSE;
            u32 a = t;
            u32 b = t;
            if (b >= 0x1492 && a <= 0x14fd) f = TRUE;
            if (f) {
                Snd_SeEmitterPlayOneShot(&self->seEmitter, 0x70, 0x7f, 0);
            }
        } else {
            self->step = 2;
            self->velocityX = 0;
            self->velocityY = 0;
            self->velocityZ = 0;
            FieldItemFx_Finish(self);
        }
    }
}
}

namespace ns_0221aed4 {
extern "C" void FieldItemFx_InitDigHole(FieldItemFx *self, P2 p, BlockMap *g, s32 flag)
{
    FieldPos_FromUnitCenter(&self->pos, p.x, p.z);
    self->scale.x = 0;
    self->scale.y = 0x1000;
    self->scale.z = 0;
    self->velocityX = 0;
    self->velocityY = 0;
    self->velocityZ = 0;
    s32 x = p.x;
    s32 z = p.z;
    s32 hx = x >> 4;
    s32 hz = z >> 4;
    u16 *c = (u16 *)BlockMap_GetItemPtr(g, hx, hz, x - (hx << 4), z - (hz << 4), 0);
    if (c != 0) {
        s32 r = BlockMap_IsBuriedAtUnit(g, p.x, p.z);
        if (Item_IsFlower(c) != 0) {
            VecXZ q;
            q.x = p.x;
            q.z = p.z;
            Flower_SpawnPetalFx(c, &q, 0, 0);
        } else {
            BOOL f = FALSE;
            u32 v = *c;
            if (v >= 0x21 && v <= 0x24) f = TRUE;
            if (f || (v >= 0x1f && v <= 0x20)) {
                VecXZ q;
            q.x = p.x;
            q.z = p.z;
                Weed_SpawnPullFx(c, &q);
            } else if (v == 0xe2) {
                VecXZ q;
            q.x = p.x;
            q.z = p.z;
                DeadTurnip_SpawnDigFx(&q);
            } else if (r != 0 || (v >= 0xd4 && v <= 0xda) || (v >= 0xdb && v <= 0xe1)) {
                if (flag != 0) {
                    self->pendingApply = 0;
                    self->pendingCommit = 1;
                }
            }
        }
        if (BlockMap_IsBuriedAtUnit(g, p.x, p.z) != 0) {
            Field_SetUnitItem(p.x, p.z, 0xfff1, 0);
        }
    }
}
}

namespace ns_0221aed4 {
extern "C" void FieldItemFx_UpdateHoleOpen(FieldItemFx *self)
{
    self->scale.x = self->scale.x + 0x571;
    if (self->scale.x >= 0x1000) {
        self->scale.x = 0x1000;
        FieldItemFx_Finish(self);
    }
    self->scale.z = self->scale.x;
}
}

namespace ns_0221aed4 {
extern "C" void FieldItemFx_InitPitfallHole(FieldItemFx *self, P2 p)
{
    FieldPos_FromUnitCenter(&self->pos, p.x, p.z);
    self->scale.x = 0;
    self->scale.y = 0x1000;
    self->scale.z = 0;
    self->velocityX = 0;
    self->velocityY = 0;
    self->velocityZ = 0;
    self->pendingApply = 0;
    self->pendingCommit = 1;
}
}

namespace ns_0221aed4 {
extern "C" void FieldItemFx_InitFillHole(FieldItemFx *self, P2 p, BlockMap *g)
{
    FieldPos_FromUnitCenter(&self->pos, p.x, p.z);
    if (g->getDigKind(p.x, p.z) == 0) {
        self->item = 0xfc;
    } else {
        self->item = 0xfd;
    }
    self->scale.x = 0x1000;
    self->scale.y = 0x1000;
    self->scale.z = 0x1000;
    self->velocityX = 0;
    self->velocityY = 0;
    self->velocityZ = 0;
    Field_SetUnitItem(p.x, p.z, 0xfff1, 0);
}
}

namespace ns_0221a4a0 {
extern "C" void FieldItemFx_UpdateHoleClose(FieldItemFx *self)
{
    self->scale.x = self->scale.x - 0x154;
    if (self->scale.x < 0) {
        volatile u16 tmp;
        FieldUnitPos pos;
        tmp = 0xfff1;
        self->scale.x = 0;
        tmp = self->pendingItem;
        u32 v;
        if (Unk_ov003_0221ad84_Chk(&tmp, v) || (v >= 0x26 && v <= 0x2a) || (v >= 0x5d && v <= 0x61) ||
            (v >= 0x2f && v <= 0x56) || (v >= 0x57 && v <= 0x5b) || (v >= 0x66 && v <= 0x68) || v == 0x69 ||
            (v >= 0x6a && v <= 0x6c) || v == 0x6d || (v >= 0xc8 && v <= 0xcf) || (v >= 0xd4 && v <= 0xda)) {
            pos.x = self->unit.x;
            pos.z = self->unit.z;
            if (FieldItemFx_StartPop(self->sessionSlot, self->pendingItem, pos)) {
                self->pendingApply = 0;
                FieldItemFx_Finish(self);
            }
        } else {
            FieldItemFx_Finish(self);
        }
    }
    self->scale.z = self->scale.x;
}
}

namespace ns_0221a4a0 {
extern "C" void FieldItemFx_InitPop(FieldItemFx *self, FieldUnitPos *p)
{
    struct Pad { s32 v[2]; Pad() {} ~Pad() {} } pad;
    volatile u16 tmp;
    FieldPos_FromUnitCenter(&self->pos, p->x, p->z);
    s32 z = 0;
    self->scale.x = z;
    self->scale.y = z;
    self->scale.z = z;
    s32 id = 0x813;
    tmp = 0xfff1;
    tmp = self->item;
    u32 a = tmp;
    u32 c = tmp;
    if (c >= 0xa7 && a <= 0xc6) {
        z = 1;
    }
    if (z) {
        id = 0x7f0;
    }
    Snd_SeEmitterPlayOneShot(&self->seEmitter, id, 0x7f, 0);
}
}

namespace ns_0221a4a0 {
extern "C" void FieldItemFx_UpdatePop(FieldItemFx *self)
{
    if ((u16)self->frame >= 0x10) {
        self->frame = 0x10;
        FieldItemFx_Finish(self);
    }
    self->scale.x = sItemPopScaleXZ[(u16)self->frame];
    self->scale.z = self->scale.x;
    self->scale.y = sItemPopScaleY[(u16)self->frame];
}
}

namespace ns_0221a4a0 {
extern "C" void FieldItemFx_InitTreeDrop(FieldItemFx *self, FieldUnitPos *p, VecFx32 *q, s32 skip)
{
    volatile u16 tmp;
    s32 z = 0;
    self->velocityX = z;
    self->velocityY = 0x800;
    self->velocityZ = 0x300;
    self->pos.x = q->x;
    self->pos.y = q->y;
    self->pos.z = q->z;
    self->scale.x = 0x1000;
    self->scale.y = 0x1000;
    self->scale.z = 0x1000;
    if (self->startArg != 0) {
        self->pendingApply = z;
    }
    if (skip == 0) {
        tmp = 0xfff1;
        tmp = self->item;
        BOOL r = FALSE;
        u32 a = tmp;
        u32 c = tmp;
        if (c >= 0x1492 && a <= 0x14fd) {
            r = TRUE;
        }
        Snd_SeEmitterPlayOneShot(&self->seEmitter, r ? 0x74 : 0x7d6, 0x7f, 0);
    }
}
}

namespace ns_0221a4a0 {
extern "C" void FieldItemFx_UpdateTreeDrop(FieldItemFx *self)
{
    VecFx32 v;
    volatile u16 tmp;
    self->velocityY = self->velocityY - 0x400;
    VEC_Add(&self->pos, &self->velocityX, &self->pos);
    if (self->pos.y < 0) {
        if (self->step == 0) {
            FieldPos_FromUnitCenter(&v, self->unit.x, self->unit.z);
            s32 r5 = func_02133150(v.z - self->pos.z, 8);
            self->velocityX = func_02133150(v.x - self->pos.x, 8);
            self->velocityY = 0x1000;
            self->velocityZ = r5;
            self->step = 1;
            tmp = 0xfff1;
            tmp = self->item;
            s32 z = 0;
            u32 a = tmp;
            u32 c = tmp;
            if (c >= 0x1492 && a <= 0x14fd) {
                z = 1;
            }
            if (z) {
                Snd_SeEmitterPlayOneShot(&self->seEmitter, 0x70, 0x7f, 0);
            }
        } else {
            self->velocityX = 0;
            self->velocityY = 0;
            self->velocityZ = 0;
            FieldItemFx_Finish(self);
        }
        self->pos.y = 0;
    }
}
}

namespace ns_0221a4a0 {
extern "C" void FieldItemFx_InitTreeDropFloat(FieldItemFx *self, FieldUnitPos *p, VecFx32 *q)
{
    VecFx32 v;
    FieldPos_FromUnitCenter(&v, p->x, p->z);
    s32 r7 = func_02133150(v.z - q->z, 0x1e);
    s32 r6 = -func_02133150(q->y, 0x1e);
    self->velocityX = func_02133150(v.x - q->x, 0x1e);
    self->velocityY = r6;
    self->velocityZ = r7;
    self->pos.x = q->x;
    self->pos.y = q->y;
    self->pos.z = q->z;
    self->scale.x = 0x1000;
    self->scale.y = 0x1000;
    self->scale.z = 0x1000;
    if (self->startArg != 0) {
        FieldItemFx_ReleasePending(self);
    }
    Snd_SeEmitterPlayOneShot(&self->seEmitter, 0x7eb, 0x7f, 0);
}
}

namespace ns_0221a4a0 {
extern "C" void FieldItemFx_UpdateTreeDropFloat(FieldItemFx *self)
{
    VEC_Add(&self->pos, &self->velocityX, &self->pos);
    self->frame = self->frame + 0x888;
    self->offset.x = data_02135f44[((u16)(s16)self->frame >> 4) * 2] >> 1;
    if (self->pos.y < 0) {
        self->pos.y = 0;
        self->velocityX = 0;
        self->velocityY = 0;
        self->velocityZ = 0;
        FieldItemFx_Finish(self);
    }
}
}

namespace ns_0221a4a0 {
extern "C" void FieldItemFx_InitBeeHiveDrop(FieldItemFx *self, FieldUnitPos *unit, VecFx32 *p)
{
    self->velocityX = 0;
    self->velocityY = 0x800;
    self->velocityZ = 0x300;
    self->pos.x = p->x;
    self->pos.y = p->y;
    self->pos.z = p->z;
    self->scale.x = 0x1000;
    self->scale.y = 0x1000;
    self->scale.z = 0x1000;
    Snd_SeEmitterPlayOneShot(&self->seEmitter, 0x7d6, 0x7f, 0);
    FieldItemFx_ReleasePending(self);
    Town_SetBeesReleased();
}
}

namespace ns_0221a4a0 {
extern "C" void FieldItemFx_UpdateBeeHiveDrop(FieldItemFx *self)
{
    switch (self->step) {
    case 0:
        self->velocityY = self->velocityY - 0x400;
        VEC_Add(&self->pos, &self->velocityX, &self->pos);
        if (self->pos.y < 0) {
            s32 z = 0;
            self->pos.y = z;
            self->velocityX = z;
            self->velocityY = 0x1000;
            self->velocityZ = z;
            self->step = 1;
        }
        break;
    case 1:
        self->velocityY = self->velocityY - 0x400;
        VEC_Add(&self->pos, &self->velocityX, &self->pos);
        if (self->pos.y < 0) {
            self->pos.y = 0;
            self->velocityX = 0;
            self->velocityY = 0;
            self->velocityZ = 0;
            self->step = 2;
            self->timer = 20;
        }
        break;
    case 2:
        self->timer = self->timer - 1;
        if (self->timer < 0) {
            Insect_SpawnBeeSwarm((VecFx32Ctor *)&self->pos);
            self->timer = 30;
            self->step = 3;
        }
        break;
    case 3:
        self->timer = self->timer - 1;
        if (self->timer < 0) {
            self->step = 4;
        }
        break;
    default:
        self->alpha = self->alpha - 1;
        if (self->alpha < 0) {
            FieldItemFx_Clear(self);
        }
        break;
    }
}
}

namespace ns_0221a4a0 {
extern "C" void FieldItemFx_InitDigUpTree(FieldItemFx *self, FieldUnitPos *p, u32 ang)
{
    FieldPos_FromUnitCenter(&self->pos, p->x, p->z);
    s32 i = ((u16)ang >> 4) * 2;
    self->velocityX = (data_02135f44[i] << 10) >> 12;
    self->velocityY = 0x1000;
    self->velocityZ = (data_02135f44[i + 1] << 10) >> 12;
    self->scale.x = 0x1000;
    self->scale.y = 0x1000;
    self->scale.z = 0x1000;
    self->pendingApply = 0;
    Field_SetUnitItem(p->x, p->z, 0xfff1, 0);
}
}

namespace ns_0221a4a0 {
extern "C" void FieldItemFx_UpdateDigUpTree(FieldItemFx *self)
{
    self->velocityY = self->velocityY - 0x400;
    VEC_Add(&self->pos, &self->velocityX, &self->pos);
    if (self->pos.y < 0) {
        self->pos.y = 0;
        self->velocityX = 0;
        self->velocityY = 0;
        self->velocityZ = 0;
        FieldItemFx_Finish(self);
    }
}
}

namespace ns_0221a4a0 {
extern "C" void FieldItemFx_InitPlant(FieldItemFx *self, FieldUnitPos *p, VecFx32 *q, u32 w)
{
    VecFx32 v;
    volatile u16 tmp;
    FieldPos_FromUnitCenter(&v, p->x, p->z);
    s32 r6 = func_02133150(v.z - q->z, 8);
    self->velocityX = func_02133150(v.x - q->x, 8);
    self->velocityY = 0x1000;
    self->velocityZ = r6;
    self->pos.x = q->x;
    self->pos.y = q->y;
    self->pos.z = q->z;
    s32 z = 0;
    self->scale.x = z;
    self->scale.y = 0x1000;
    self->scale.z = z;
    self->pendingApply = z;
    tmp = 0xfff1;
    tmp = w;
    u32 a = tmp;
    u32 c = tmp;
    if (c >= 0x1492 && a <= 0x14fd) {
        z = 1;
    }
    Snd_SeEmitterPlayOneShot(&self->seEmitter, z ? 0x74 : 0x75, 0x7f, 0);
}
}

namespace ns_0221a4a0 {
extern "C" void FieldItemFx_UpdatePlant(FieldItemFx *self)
{
    volatile u16 tmp;
    self->scale.x = self->scale.x + 0x19a;
    if (self->scale.x >= 0x1000) {
        self->scale.x = 0x1000;
    }
    self->scale.z = self->scale.x;
    self->velocityY = self->velocityY - 0x400;
    VEC_Add(&self->pos, &self->velocityX, &self->pos);
    if (self->pos.y < 0) {
        self->pos.y = 0;
        self->velocityX = 0;
        self->velocityY = 0;
        self->velocityZ = 0;
        if (self->step == 0) {
            BOOL r = FALSE;
            tmp = 0xfff1;
            tmp = self->pendingItem;
            u32 a = tmp;
            u32 c = tmp;
            if (c >= 0x1492 && a <= 0x14fd) {
                r = TRUE;
            }
            if (r) {
                Snd_SeEmitterPlayOneShot(&self->seEmitter, 0x70, 0x7f, 0);
            }
        }
        self->step = 1;
        FieldItemFx_Finish(self);
    }
}
}

namespace ns_0221a4a0 {
extern "C" void FieldItemFx_InitHoleShrink(FieldItemFx *self, FieldUnitPos *p)
{
    FieldPos_FromUnitCenter(&self->pos, p->x, p->z);
    self->scale.x = 0x1000;
    self->scale.y = 0x1000;
    self->scale.z = 0x1000;
    self->velocityX = 0;
    self->velocityY = 0;
    self->velocityZ = 0;
    FieldItemFx_ReleasePending(self);
}
}

namespace ns_0221a4a0 {
extern "C" void FieldItemFx_UpdateHoleShrink(FieldItemFx *self)
{
    u32 r = (u16)self->frame;
    if ((r >= 6 && r <= 10) || r >= 0x12) {
        self->scale.x = self->scale.x - 0x19a;
    }
    if (self->scale.x <= 0) {
        self->scale.x = 0;
        FieldItemFx_Finish(self);
    }
    self->scale.z = self->scale.x;
}
}

namespace ns_0221a4a0 {
extern "C" void FieldItemFx_InitStrikeShake(FieldItemFx *self, FieldUnitPos *p)
{
    self->scale.x = 0x1000;
    self->scale.y = 0x1000;
    self->scale.z = 0x1000;
    Field_SetUnitItem(p->x, p->z, 0xfff1, 0);
}
}

namespace ns_0221a4a0 {
extern "C" void FieldItemFx_UpdateStrikeShake(FieldItemFx *self)
{
    VecFx32 v;
    VecFx32 out;
    self->timer = self->timer + 0x2000;
    s32 h = data_02135f44[((u16)self->timer >> 4) * 2] >> 1;
    v.x = (h * data_02135f44[((u16)self->startArg >> 4) * 2]) >> 12;
    v.y = 0;
    v.z = (h * data_02135f44[((u16)self->startArg >> 4) * 2 + 1]) >> 12;
    Vec_Add(&out, &self->origin, &v);
    self->pos.x = out.x;
    self->pos.y = out.y;
    self->pos.z = out.z;
    if (self->timer < 0) {
        FieldItemFx_Finish(self);
    }
}
}

namespace ns_0221a4a0 {
extern "C" void FieldItemFx_SplashPosCallback(EffectModel *p)
{
    Unk_ov003_0221a664_Obj *o = (Unk_ov003_0221a664_Obj *)p;
    VecFx32 *d = &o->pos;
    o->pos.x = sBalloonSplashPos.x;
    d->y = sBalloonSplashPos.y;
    d->z = sBalloonSplashPos.z;
}
}

namespace ns_0221a4a0 {
extern "C" void FieldItemFx_InitBalloonDrop(FieldItemFx *self, VecFx32 *p)
{
    self->pos.x = p->x;
    self->pos.y = p->y;
    self->pos.z = p->z;
    self->velocityX = 0;
    self->velocityY = -0xc00;
    self->velocityZ = 0;
}
}

namespace ns_0221a4a0 {
extern "C" void FieldItemFx_UpdateBalloonDrop(FieldItemFx *self)
{
    GroundInfoBase b;
    VecFx32 v;
    ((GroundInfo *)(void *)(&b))->initAtPos((VecFx32 *)(&self->pos), 0, 0);
    if (b.waterKind != 0) {
        if (self->step == 0) {
            VEC_Add(&self->pos, &self->velocityX, &self->pos);
            s32 t = b.waterSurfaceY;
            if (self->pos.y < t) {
                self->velocityY = 0;
                sBalloonSplashPos.x = self->pos.x;
                sBalloonSplashPos.y = self->pos.y;
                sBalloonSplashPos.z = self->pos.z;
                sBalloonSplashPos.y = t;
                EffectSpl_CreateOneShot(0x45, &sBalloonSplashPos, 0, gEffectSplDefaultInitCbs);
                EffectSpl_CreateOneShot(0x4a, &sBalloonSplashPos, 0, gEffectSplDefaultInitCbs);
                EffectModel_Start(2, FieldItemFx_SplashPosCallback);
                self->step = 1;
                self->timer = 12;
                self->pos.y = self->pos.y - 0x2000;
                Sky_PlayBalloonDropSe(1);
            }
        } else {
            self->timer = self->timer - 1;
            if (self->timer < 0) {
                Sky_EndBalloonDrop();
                PlayerActor_LocalRequestAct77From(&self->pos);
                self->pendingApply = 0;
                FieldItemFx_Finish(self);
            }
        }
    } else {
        if (self->step == 0) {
            VEC_Add(&self->pos, &self->velocityX, &self->pos);
            if (self->pos.y < 0) {
                self->pos.y = 0;
                self->timer = FieldItemFx_FindLandingUnit(&self->unit.x);
                FieldPos_FromUnitCenter(&v, self->unit.x, self->unit.z);
                s32 r5 = func_02133150(v.z - self->pos.z, 8);
                self->velocityX = func_02133150(v.x - self->pos.x, 8);
                self->velocityY = 0x1000;
                self->velocityZ = r5;
                self->step = 1;
                Sky_PlayBalloonDropSe(0);
            }
        } else {
            self->velocityY = self->velocityY - 0x400;
            VEC_Add(&self->pos, &self->velocityX, &self->pos);
            if (self->pos.y < 0) {
                self->pos.y = 0;
                self->velocityX = 0;
                self->velocityY = 0;
                self->velocityZ = 0;
                Sky_EndBalloonDrop();
                if (self->timer == 0) {
                    if (self->item == 0x137b) {
                        PlayerData_GetCurrent()->setFlag(0x30);
                    }
                    Field_SetUnitItem(self->unit.x, self->unit.z, self->item, 0);
                }
                self->pendingApply = 0;
                FieldItemFx_Finish(self);
            }
        }
    }
    GroundInfo_Destruct(&b);
}
}

namespace ns_02219b84 {
extern "C" {
void FieldItemFx_InitPitfallClose(Ent *e, P2 *p, BlockMap *m) {
    FieldPos_FromUnitCenter(&e->pos, p->x, p->z);
    if (m->getDigKind(p->x, p->z) == 0) {
        e->item = 0xfc;
    } else {
        e->item = 0xfd;
    }
    e->scale.x = 0x1000;
    e->scale.y = 0x1000;
    e->scale.z = 0x1000;
    s32 z = 0;
    e->velocityX = z;
    e->velocityY = z;
    e->velocityZ = z;
    Field_SetUnitItem(p->x, p->z, 0xfff1, z);
    Vec2 u = {e->unit.x, e->unit.z};
    s32 i = PendingUnit_Find(&u, 0);
    if (i >= 0) {
        PendingUnit_Get(i)->item = 0xfff1;
    }
}
}
}

namespace ns_02219b84 {
extern "C" {
void FieldItemFxTable_Init(Tbl *t) {
    Ent *e = t->entries;
    s32 i;
    for (i = 0; i < 20; e++, i++) {
        e->active = 0;
        e->item = 0xfff1;
        e->seEmitter.callInit();
    }
}
}
}

namespace ns_02219b84 {
extern "C" {
void FieldItemFxTable_Update(Tbl *t) {
    volatile u16 v = 0xfff1;
    Ent *e = t->entries;
    V3 tmp;
    s32 i;
    gCommManager->isOnline();
    for (i = 0; i < 20; e++, i++) {
        if (e->active != 0) {
            e->frame++;
            v = e->item;
            switch (e->kind) {
            case 0:
            case 13:
                FieldItemFx_UpdateToss(e);
                break;
            case 1:
            case 2:
                FieldItemFx_UpdateHoleOpen(e);
                break;
            case 3:
            case 14:
                FieldItemFx_UpdateHoleClose(e);
                break;
            case 4:
                FieldItemFx_UpdatePop(e);
                break;
            case 5:
                FieldItemFx_UpdateTreeDrop(e);
                break;
            case 6:
                FieldItemFx_UpdateTreeDropFloat(e);
                break;
            case 7:
                FieldItemFx_UpdateBeeHiveDrop(e);
                break;
            case 8:
                FieldItemFx_UpdateDigUpTree(e);
                break;
            case 9:
                FieldItemFx_UpdatePlant(e);
                break;
            case 10:
                FieldItemFx_UpdateHoleShrink(e);
                break;
            case 11:
                FieldItemFx_UpdateStrikeShake(e);
                break;
            case 12:
                FieldItemFx_UpdateBalloonDrop(e);
                break;
            }
            tmp.x = e->pos.x;
            tmp.y = e->pos.y;
            tmp.z = e->pos.z;
            e->seEmitter.callUpdateRelative((VecFx32 *)(&tmp));
        }
    }
}
}
}

namespace ns_02219b84 {
extern "C" {
void FieldItemFxTable_Draw(Ent *e) {
    BOOL k4, k3, k2, f1;
    u16 type = 0xfff1;
    s32 i = 0;
    s32 y4;
    s32 y8;
    s32 ang;
    volatile u16 *tp = &type;
    s32 z0c = 0;
    struct {
        V3 v50, v5c, v68, v74, v80, v8c, v98;
    } l;
    for (;;) {
        if (e->active != 0 && e->item != 0xfff1) {
            Vec_Add((VecFx32 *)&l.v74, &e->pos, &e->offset);
            l.v5c.x = l.v74.x;
            l.v5c.y = l.v74.y;
            l.v5c.z = l.v74.z;
            type = e->item;
            s32 k = (type & 0xf000) >> 12;
            switch (k) {
            case 1:
            case 3:
            case 4: {
                l.v80.x = l.v5c.x;
                l.v80.y = l.v5c.y;
                l.v80.z = l.v5c.z;
                l.v8c.x = 0x1000;
                l.v8c.y = 0x1000;
                l.v8c.z = 0x1000;
                FieldObj_DrawItemIcon(gFieldObjectManager, e->item, &l.v80, e->alpha, &l.v8c, z0c, z0c, z0c);
                break;
            }
            default: {
                VecFx32 *pv = &e->scale;
                l.v68.x = pv->x;
                l.v68.y = pv->y;
                l.v68.z = pv->z;
                ang = WorldCurve_ToCurved(&l.v50, &l.v5c);
                Mtx43_SetTranslate(&data_021f47e0, l.v50.x, l.v50.y, l.v50.z);
                Mtx43_RotateX(&data_021f47e0, ang);
                Mtx43_Scale(&data_021f47e0, l.v68.x, l.v68.y, l.v68.z);
                u16 a;
                if (Unk_ov003_02219e7c_Chk1(&type, tp, a) || (a >= 0x1f && a <= 0x20)) {
                    FieldObj_DrawGrass(gFieldObjectManager, &type, data_021f47e0);
                } else {
                k4 = TRUE, k3 = TRUE, k2 = TRUE, f1 = FALSE;
                if (a <= 5) {
                    f1 = TRUE;
                }
                if (!f1) {
                    if (a < 6 || a > 0xb) {
                        k2 = FALSE;
                    }
                }
                if (!k2) {
                    if (a < 0xc || a > 0x11) {
                        k3 = FALSE;
                    }
                }
                if (!k3) {
                    if ((a < 0x12 || a > 0x19) && a != 0x1c) {
                        k4 = FALSE;
                    }
                }
                if (k4 || (a >= 0x8a && a <= 0x8f) || (a >= 0x90 && a <= 0x95) ||
                           (a >= 0x96 && a <= 0x9b) || (a >= 0x9c && a <= 0xa3) || a == 0xa5) {
                    FieldObj_DrawFlower(gFieldObjectManager, &type, data_021f47e0);
                } else if ((a >= 0x6e && a <= 0x73) || (a >= 0x74 && a <= 0x79) || (a >= 0x7a && a <= 0x7f) ||
                           (a >= 0x80 && a <= 0x87)) {
                    FieldObj_DrawFlowerBySpecies(gFieldObjectManager, &type, data_021f47e0);
                } else if (Unk_ov003_02219e7c_Chk4(a) || !(a != 0x88 && a != 0x89)) {
                    FieldObj_DrawSpecialFlower(gFieldObjectManager, &type, data_021f47e0);
                } else if (a >= 0xfc && a <= 0xfd) {
                    FieldObj_DrawHole(gFieldObjectManager, &type, data_021f47e0);
                } else if ((a >= 0x2b && a <= 0x2e) || (a >= 0xff && a <= 0x102) || (a >= 0x62 && a <= 0x65) ||
                           (a >= 0xd0 && a <= 0xd3)) {
                    FieldObj_DrawStump(gFieldObjectManager, &type, data_021f47e0);
                } else if ((a >= 0x26 && a <= 0x2a) || (a >= 0x5d && a <= 0x61) || (a >= 0x2f && a <= 0x56) ||
                           (a >= 0x57 && a <= 0x5b) || (a >= 0x66 && a <= 0x68) || a == 0x69 ||
                           (a >= 0x6a && a <= 0x6c) || a == 0x6d || (a >= 0xc8 && a <= 0xcf)) {
                    l.v98.x = l.v5c.x;
                    l.v98.y = l.v5c.y;
                    l.v98.z = l.v5c.z;
                    s32 px = *(volatile s32 *)&e->unit.x;
                    y4 = *(volatile s32 *)&e->unit.z;
                    FieldObj_DrawTree(gFieldObjectManager, &type, px, y4, l.v98, data_021f47e0);
                } else if (a == 0x25 || a == 0x5c || a == 0xc7) {
                    s32 px = *(volatile s32 *)&e->unit.x;
                    y8 = *(volatile s32 *)&e->unit.z;
                    FieldObj_DrawSapling(gFieldObjectManager, &type, px, y8, data_021f47e0);
                } else if ((a >= 0xe3 && a <= 0xe7) || (a >= 0xe8 && a <= 0xfb)) {
                    FieldObj_DrawRock(gFieldObjectManager, &type, data_021f47e0);
                } else if ((a >= 0xd4 && a <= 0xda) || (a >= 0xdb && a <= 0xe1)) {
                    FieldObj_DrawTurnip(gFieldObjectManager, &type, data_021f47e0);
                } else if (a >= 0xa7 && a <= 0xc6) {
                    FieldObj_DrawDesign(gFieldObjectManager, &type, data_021f47e0);
                }
                }
            }
            }
        }
        e++;
        i++;
        if (i >= 20) {
            break;
        }
    }
}
}
}

namespace ns_02219b84 {
extern "C" {
void FieldItemFxTable_Release(Tbl *t) {
    Ent *e = t->entries;
    s32 i;
    for (i = 0; i < 20; e++, i++) {
        if (e->active != 0) {
            FieldItemFx_Finish(e);
        }
        e->seEmitter.callStop();
    }
}
}
}

namespace ns_02219b84 {
extern "C" {
Ent *FieldItemFxTable_FindFree(Tbl *t, s32 g) {
    s32 i;
    Ent *r = 0;
    Ent *e = &t->entries[g * 4];
    for (i = 0; i < 4; e++, i++) {
        if (e->active == 0) {
            r = e;
            break;
        }
    }
    return r;
}
}
}

namespace ns_02219b84 {
extern "C" {
s32 FieldItemFxTable_Start(Tbl *t, s32 a, P2 p, V3 v, s32 f, u32 g, s32 h, s32 i) {
    BlockMap *map = TownBlockMap_Get();
    if (map == 0) {
        return 0;
    }
    s32 r = 0;
    Ent *e = FieldItemFxTable_FindFree(t, a);
    if (e != 0) {
        FieldItemFx_Init(e, a, p, v, f, *(u16 *)&g, map, *(s16 *)&h, i);
        r = 1;
    }
    return r;
}
}
}

namespace ns_02219b84 {
extern "C" {
s32 FieldItemFxTable_StartForPlayer(Tbl *t, s32 a, P2 p, V3 v, s32 f, u32 g, s32 h, s32 i) {
    if (gCommManager->isOnline() == 0) {
        a = 0;
    }
    return FieldItemFxTable_Start(t, a, p, v, f, *(u16 *)&g, *(s16 *)&h, i);
}
}
}

namespace ns_02219b84 {
extern "C" {
s32 FieldItemFx_CancelAt(s32 idx, P2 *q) {
    s32 r = 0;
    s32 i;
    Ent *e = &FxTable()->entries[idx * 4];
    for (i = 0; i < 4; e++, i++) {
        if (e->active != 0) {
            if (P2(e->unit.x, e->unit.z) == *q) {
                if (e->pendingApply != 0) {
                    r = 1;
                }
                FieldItemFx_Finish(e);
                break;
            }
        }
    }
    return r;
}
}
}

namespace ns_02219b84 {
extern "C" {
s32 FieldItemFx_StartDrop(s32 a, u32 b, P2 p, V3 v) {
    return FieldItemFxTable_Start(FxTable(), a, p, v, 0, b, 0, 0);
}
}
}

namespace ns_02219b84 {
extern "C" {
s32 FieldItemFx_StartDigHole(s32 type, VecFx32 *v, s32 w) {
    BlockMap *map = TownBlockMap_Get();
    if (map == 0) {
        return 0;
    }
    s32 ox = 0;
    s32 oy = 0;
    FieldPos_ToUnit(&ox, &oy, v);
    u32 k;
    if (((BlockMap *)map)->getDigKind(ox, oy) == 0) {
        k = 0xfc;
    } else {
        k = 0xfd;
    }
    return FieldItemFxTable_StartForPlayer(FxTable(), type, P2(ox, oy), V3(0, 0, 0), 1, k, 0, w);
}
}
}

namespace ns_02219b84 {
extern "C" {
s32 FieldItemFx_StartPitfallHole(s32 type, VecFx32 *v) {
    BlockMap *map = TownBlockMap_Get();
    if (map == 0) {
        return 0;
    }
    s32 ox = 0;
    s32 oy = 0;
    FieldPos_ToUnit(&ox, &oy, v);
    u32 k;
    if (((BlockMap *)map)->getDigKind(ox, oy) == 0) {
        k = 0xfc;
    } else {
        k = 0xfd;
    }
    return FieldItemFxTable_StartForPlayer(FxTable(), type, P2(ox, oy), V3(0, 0, 0), 2, k, 0, 0);
}
}
}

namespace ns_02219b84 {
extern "C" {
s32 FieldItemFx_StartFillHole(s32 type, VecFx32 *v) {
    BlockMap *map = TownBlockMap_Get();
    if (map == 0) {
        return 0;
    }
    s32 ox = 0;
    s32 oy = 0;
    FieldPos_ToUnit(&ox, &oy, v);
    u32 k;
    if (((BlockMap *)map)->getDigKind(ox, oy) == 0) {
        k = 0xfc;
    } else {
        k = 0xfd;
    }
    return FieldItemFxTable_StartForPlayer(FxTable(), type, P2(ox, oy), V3(0, 0, 0), 3, k, 0, 0);
}
}
}

namespace ns_0221927c {
extern "C" s32 FieldItemFx_StartFillHoleWithItem(s32 a, VecFx32 *b)
{
    s32 p0 = 0;
    s32 p1 = 0;
    u32 r = 0xfff1;
    FieldPos_ToUnit(&p0, &p1, b);
    Vec2 pos;
    pos.x = p0;
    pos.y = p1;
    s32 k;
    if ((k = PendingUnit_Find(&pos, 0)) >= 0) {
        PendingUnit *c = PendingUnit_Get(k);
        if (c != NULL) {
            r = c->item;
        }
    }
    return FieldItemFxTable_StartForPlayer(&sFieldItemFxTable, a, P2(p0, p1), V3(0, 0, 0), 3, r, 0, 0);
}
}

namespace ns_0221927c {
extern "C" s32 FieldItemFx_StartPop(s32 a, s32 b, P2 c)
{
    return FieldItemFxTable_Start(&sFieldItemFxTable, a, c, V3(0, 0, 0), 4, b, 0, 0);
}
}

namespace ns_0221927c {
extern "C" void FieldItemFx_StartTreeDrop(s32 a, s32 v, P2 p, V3 q, s16 t, s32 x)
{
    FieldItemFxTable_Start(&sFieldItemFxTable, a, p, q, 5, v, t, x);
}
}

namespace ns_0221927c {
extern "C" void FieldItemFx_StartTreeDropFloat(s32 a, s32 v, P2 p, V3 q, s32 w)
{
    FieldItemFxTable_Start(&sFieldItemFxTable, a, p, q, 6, v, 0, w);
}
}

namespace ns_0221927c {
extern "C" void FieldItemFx_StartBeeHiveDrop(s32 a, s32 v, P2 p, V3 q)
{
    FieldItemFxTable_Start(&sFieldItemFxTable, a, p, q, 7, v, 0, 0);
}
}

namespace ns_0221927c {
extern "C" s32 FieldItemFx_StartDigUpTree(s32 a, VecFx32 *b, s32 c)
{
    BlockMap *g = TownBlockMap_Get();
    if (g == NULL) {
        return 0;
    }
    s32 p0 = 0;
    s32 p1 = 0;
    FieldPos_ToUnit(&p0, &p1, b);
    s32 x = *(volatile s32 *)&p0;
    s32 z = *(volatile s32 *)&p1;
    s32 hx = x >> 4;
    s32 hz = z >> 4;
    u16 *cell = (u16 *)BlockMap_GetItemPtr(g, hx, hz, x - (hx << 4), z - (hz << 4), 0);
    u32 r = 0x2b;
    if (cell != NULL) {
        BOOL f4 = TRUE, f3 = TRUE, f2 = TRUE, f1 = FALSE;
        u32 v = *cell;
        if (v >= 0x2b && v <= 0x2e) f1 = TRUE;
        if (!f1) { if (v < 0xff || v > 0x102) f2 = FALSE; }
        if (!f2) { if (v < 0x62 || v > 0x65) f3 = FALSE; }
        if (!f3) { if (v < 0xd0 || v > 0xd3) f4 = FALSE; }
        if (f4 || (v >= 0x26 && v <= 0x2a) || (v >= 0x5d && v <= 0x61) || (v >= 0x2f && v <= 0x56)
            || (v >= 0x57 && v <= 0x5b) || (v >= 0x66 && v <= 0x68) || v == 0x69 || (v >= 0x6a && v <= 0x6c)
            || v == 0x6d || (v >= 0xc8 && v <= 0xcf) || v == 0x25 || v == 0x5c || v == 0xc7) {
            r = v;
        }
    }
    return FieldItemFxTable_StartForPlayer(&sFieldItemFxTable, a, P2(p0, p1), V3(0, 0, 0), 8, r, c, 0);
}
}

namespace ns_0221927c {
extern "C" void FieldItemFx_StartPlant(s32 a, s32 b, P2 c, V3 d)
{
    volatile u16 t;
    t = 0xfff1;
    t = b;
    BOOL f9 = TRUE, f8 = TRUE, f7 = TRUE, f6 = TRUE, f5 = TRUE, f4 = TRUE, f3 = TRUE, f2 = TRUE, f1 = FALSE;
    u32 v = t;
    if (t <= 5) f1 = TRUE;
    if (!f1) { if (v < 6 || v > 0xb) f2 = FALSE; }
    if (!f2) { if (v < 0xc || v > 0x11) f3 = FALSE; }
    if (!f3) { if (v < 0x12 || v > 0x19) { if (v != 0x1c) f4 = FALSE; } }
    if (!f4) {
        if (v < 0x8a || v > 0x8f) {
            if (v < 0x90 || v > 0x95) {
                if (v < 0x96 || v > 0x9b) {
                    if (v < 0x9c || v > 0xa3) {
                        if (v != 0xa5) f5 = FALSE;
                    }
                }
            }
        }
    }
    if (!f5) { if (v != 0x1a) f6 = FALSE; }
    if (!f6) { if (v != 0xa4) f7 = FALSE; }
    if (!f7) { if (v != 0x1d) f8 = FALSE; }
    if (f8 || (v >= 0x14fe && v <= 0x1517) || b == 0x1567) b = 0x1408;
    FieldItemFxTable_Start(&sFieldItemFxTable, a, c, d, 9, b, 0, 0);
}
}

namespace ns_0221927c {
extern "C" s32 FieldItemFx_StartHoleShrink(s32 a, VecFx32 *b)
{
    BlockMap *g = TownBlockMap_Get();
    if (g == NULL) {
        return 0;
    }
    s32 p0 = 0;
    s32 p1 = 0;
    FieldPos_ToUnit(&p0, &p1, b);
    s32 sel;
    if (((BlockMap *)g)->getDigKind(p0, p1) == 0) {
        sel = 0xfc;
    } else {
        sel = 0xfd;
    }
    return FieldItemFxTable_StartForPlayer(&sFieldItemFxTable, a, P2(p0, p1), V3(0, 0, 0), 0xa, sel, 0, 0);
}
}

namespace ns_0221927c {
extern "C" void FieldItemFx_StartStrikeShake(s32 a, s32 b, P2 c)
{
    s32 r = 0;
    VecFx32 tmp;
    FieldPos_FromUnitCenter(&tmp, c.x, c.z);
    Actor *obj = (Actor *)PlayerActor_GetActor(4);
    if (obj != NULL) {
        r = (s32)(Math_AngleToDir8(obj->rotY) << 29) >> 16;
    }
    FieldItemFxTable_Start(&sFieldItemFxTable, a, c, V3(tmp.x, tmp.y, tmp.z), 0xb, b, r, 0);
}
}

namespace ns_0221927c {
extern "C" s32 FieldItemFx_StartBalloonDrop(s32 a, s32 n)
{
    V3 A(0, 0, 0);
    s32 p0 = 0;
    s32 p1 = 0;
    Actor *obj = (Actor *)PlayerActor_GetActor(4);
    if (obj != NULL) {
        VecFx32 *pv = &obj->position;
        A.x = pv->x;
        A.y = pv->y;
        A.z = pv->z;
        A.x = A.x + ((n << 12) >> 4);
        FieldPos_ToUnit(&p0, &p1, &A);
        p1 = (func_02133150(p1, 16) << 4) + 1;
        VecFx32 tmp;
        FieldPos_FromUnitCenter(&tmp, p0, p1);
        A.z = tmp.z;
    }
    A.y = 0xa000;
    s32 t;
    if (a == 0) {
        t = 0x156b;
    } else {
        t = 0x137b;
    }
    CommManager *g = gCommManager;
    s32 r;
    if ((g)->isOnline()) {
        r = g->myAid;
    } else {
        r = 0;
    }
    return FieldItemFxTable_Start(&sFieldItemFxTable, r, P2(p0, p1), V3(A.x, A.y, A.z), 0xc, t, 0, 0);
}
}

namespace ns_0221927c {
extern "C" s32 FieldItemFx_FindLandingUnit(s32 *p)
{
    s32 result;
    BlockMap *g;
    g = TownBlockMap_Get();
    result = 2;
    if (g == NULL) {
        return result;
    }
    s32 k10, k14;
    s32 i = 0;
    k14 = i;
    k10 = i;
    for (; i < 9; i++) {
        u32 b = sLandingUnitOffsets[i];
        s32 x = p[0] + (((s32)b >> 4) - 8);
        s32 z = p[1] + ((b & 0xf) - 8);
        s32 hx = x >> 4;
        s32 hz = z >> 4;
        u16 *c = (u16 *)BlockMap_GetItemPtr(g, hx, hz, x - (hx << 4), z - (hz << 4), k10);
        if (c != NULL && *c == 0xfff1) {
            Vec2 pos;
            pos.x = x;
            pos.y = z;
            if (PendingUnit_Find(&pos, k14) < 0) {
                if (((BlockMap *)g)->canPlaceItem(x, z)) {
                    p[0] = x;
                    p[1] = z;
                    result = 0;
                }
                break;
            }
        }
    }
    return result;
}
}

namespace ns_0221927c {
extern "C" void FieldItemFx_StartStrikeEject(s32 a, s32 v, P2 p, V3 q)
{
    FieldItemFxTable_Start(&sFieldItemFxTable, a, p, q, 0xd, v, 0, 0);
}
}

namespace ns_0221927c {
extern "C" s32 FieldItemFx_StartPitfallClose(s32 a, VecFx32 *b)
{
    BlockMap *g = TownBlockMap_Get();
    if (g == NULL) {
        return 0;
    }
    s32 p0 = 0;
    s32 p1 = 0;
    FieldPos_ToUnit(&p0, &p1, b);
    s32 sel;
    if (((BlockMap *)g)->getDigKind(p0, p1) == 0) {
        sel = 0xfc;
    } else {
        sel = 0xfd;
    }
    return FieldItemFxTable_StartForPlayer(&sFieldItemFxTable, a, P2(p0, p1), V3(0, 0, 0), 0xe, sel, 0, 0);
}
}

// Run when the overlay is loaded (modern/host/overlay.c), as the DS's __sinit: the default constructors of
// the unit's objects.
extern "C" void StaticInit_unk_ov003_022192b4(void) {
    HOST_STATIC_CONSTRUCT(sFieldItemFxTable);
}

// ---- overlay variables (generated: tools/host_overlay_vars.py) ----
#include "host_overlay_vars.h"

HOST_OVERLAY_VARS(unk_ov003_022192b4) {
    HOST_OVERLAY_VAR(data_ov003_02232eb4),
    HOST_OVERLAY_VAR(data_ov003_022333c0),
    HOST_OVERLAY_VAR(data_ov003_02233790),
    HOST_OVERLAY_VAR(data_ov003_02234328),
    HOST_OVERLAY_VAR(data_ov003_02232c7c),
    HOST_OVERLAY_VAR(data_ov003_02233400),
    HOST_OVERLAY_VAR(data_ov003_022345d4),
    HOST_OVERLAY_VAR(data_ov003_02233288),
    HOST_OVERLAY_VAR(data_ov003_02232ed0),
    HOST_OVERLAY_VAR(data_ov003_02232874),
    HOST_OVERLAY_VAR(data_ov003_02232f08),
    HOST_OVERLAY_VAR(data_ov003_02232c64),
    HOST_OVERLAY_VAR(data_ov003_022337fc),
    HOST_OVERLAY_VAR(sTreeLeafFx),
    HOST_OVERLAY_VAR(data_ov003_02232cac),
    HOST_OVERLAY_VAR(data_ov003_022329b8),
    HOST_OVERLAY_VAR(data_ov003_02233868),
    HOST_OVERLAY_VAR(data_ov003_02233420),
    HOST_OVERLAY_VAR(data_ov003_022326a8),
    HOST_OVERLAY_VAR(data_ov003_022329c8),
    HOST_OVERLAY_VAR(data_ov003_02232dcc),
    HOST_OVERLAY_VAR(data_ov003_02232cc4),
    HOST_OVERLAY_VAR(data_ov003_022327b0),
    HOST_OVERLAY_VAR(data_ov003_022338b0),
    HOST_OVERLAY_VAR(data_ov003_02232f24),
    HOST_OVERLAY_VAR(data_ov003_022338d4),
    HOST_OVERLAY_VAR(data_ov003_0223391c),
    HOST_OVERLAY_VAR(data_ov003_02232988),
    HOST_OVERLAY_VAR(data_ov003_02233820),
    HOST_OVERLAY_VAR(data_ov003_02232f5c),
    HOST_OVERLAY_VAR(data_ov003_02233988),
    HOST_OVERLAY_VAR(data_ov003_022339ac),
    HOST_OVERLAY_VAR(data_ov003_022327f0),
    HOST_OVERLAY_VAR(data_ov003_022326c0),
    HOST_OVERLAY_VAR(data_ov003_022343a0),
    HOST_OVERLAY_VAR(data_ov003_02232800),
    HOST_OVERLAY_VAR(data_ov003_02233a18),
    HOST_OVERLAY_VAR(data_ov003_022327f8),
    HOST_OVERLAY_VAR(data_ov003_02233480),
    HOST_OVERLAY_VAR(data_ov003_02233a84),
    HOST_OVERLAY_VAR(data_ov003_02232cdc),
    HOST_OVERLAY_VAR(data_ov003_02233aa8),
    HOST_OVERLAY_VAR(data_ov003_0223263c),
    HOST_OVERLAY_VAR(data_ov003_02232f78),
    HOST_OVERLAY_VAR(data_ov003_02233af0),
    HOST_OVERLAY_VAR(data_ov003_02233b14),
    HOST_OVERLAY_VAR(data_ov003_02232a28),
    HOST_OVERLAY_VAR(data_ov003_02232b18),
    HOST_OVERLAY_VAR(data_ov003_0223288c),
    HOST_OVERLAY_VAR(data_ov003_02232660),
    HOST_OVERLAY_VAR(data_ov003_02232718),
    HOST_OVERLAY_VAR(data_ov003_022334a0),
    HOST_OVERLAY_VAR(data_ov003_02233b80),
    HOST_OVERLAY_VAR(data_ov003_02233bec),
    HOST_OVERLAY_VAR(data_ov003_02232f94),
    HOST_OVERLAY_VAR(data_ov003_02233c10),
    HOST_OVERLAY_VAR(data_ov003_022326b0),
    HOST_OVERLAY_VAR(data_ov003_02232768),
    HOST_OVERLAY_VAR(data_ov003_022327d8),
    HOST_OVERLAY_VAR(data_ov003_02232630),
    HOST_OVERLAY_VAR(data_ov003_02232770),
    HOST_OVERLAY_VAR(data_ov003_02232a58),
    HOST_OVERLAY_VAR(sTreeAnimPaths),
    HOST_OVERLAY_VAR(data_ov003_02233c7c),
    HOST_OVERLAY_VAR(data_ov003_02232a78),
    HOST_OVERLAY_VAR(data_ov003_022329f8),
    HOST_OVERLAY_VAR(data_ov003_02233004),
    HOST_OVERLAY_VAR(data_ov003_02232bec),
    HOST_OVERLAY_VAR(data_ov003_02232910),
    HOST_OVERLAY_VAR(data_ov003_02233020),
    HOST_OVERLAY_VAR(data_ov003_0223303c),
    HOST_OVERLAY_VAR(data_ov003_02232a88),
    HOST_OVERLAY_VAR(data_ov003_02232d3c),
    HOST_OVERLAY_VAR(data_ov003_02232760),
    HOST_OVERLAY_VAR(data_ov003_02232654),
    HOST_OVERLAY_VAR(data_ov003_022343f0),
    HOST_OVERLAY_VAR(data_ov003_02233ce8),
    HOST_OVERLAY_VAR(sFieldItemFxTable),
    HOST_OVERLAY_VAR(data_ov003_02233d0c),
    HOST_OVERLAY_VAR(data_ov003_02233d30),
    HOST_OVERLAY_VAR(sFieldObjectModelHeap),
    HOST_OVERLAY_VAR(data_ov003_02233090),
    HOST_OVERLAY_VAR(data_ov003_02233500),
    HOST_OVERLAY_VAR(data_ov003_02234418),
    HOST_OVERLAY_VAR(data_ov003_02232d84),
    HOST_OVERLAY_VAR(data_ov003_02233100),
    HOST_OVERLAY_VAR(data_ov003_02232d9c),
    HOST_OVERLAY_VAR(data_ov003_022327b8),
    HOST_OVERLAY_VAR(sFieldObjectManagerProfile),
    HOST_OVERLAY_VAR(data_ov003_02233138),
    HOST_OVERLAY_VAR(data_ov003_02234440),
    HOST_OVERLAY_VAR(data_ov003_02232998),
    HOST_OVERLAY_VAR(data_ov003_02234468),
    HOST_OVERLAY_VAR(data_ov003_02232a98),
    HOST_OVERLAY_VAR(data_ov003_02232db4),
    HOST_OVERLAY_VAR(data_ov003_02232aa8),
    HOST_OVERLAY_VAR(data_ov003_0223294c),
    HOST_OVERLAY_VAR(data_ov003_02232c3c),
    HOST_OVERLAY_VAR(data_ov003_02233520),
    HOST_OVERLAY_VAR(data_ov003_02233e2c),
    HOST_OVERLAY_VAR(data_ov003_02233e50),
    HOST_OVERLAY_VAR(data_ov003_0223264c),
    HOST_OVERLAY_VAR(data_ov003_02232ac8),
    HOST_OVERLAY_VAR(data_ov003_02233e74),
    HOST_OVERLAY_VAR(data_ov003_02233e98),
    HOST_OVERLAY_VAR(data_ov003_022326e0),
    HOST_OVERLAY_VAR(data_ov003_02233ebc),
    HOST_OVERLAY_VAR(data_ov003_02233f04),
    HOST_OVERLAY_VAR(data_ov003_02233f28),
    HOST_OVERLAY_VAR(data_ov003_02232898),
    HOST_OVERLAY_VAR(data_ov003_02233540),
    HOST_OVERLAY_VAR(data_ov003_0223285c),
    HOST_OVERLAY_VAR(data_ov003_02233f70),
    HOST_OVERLAY_VAR(data_ov003_02232a18),
    HOST_OVERLAY_VAR(data_ov003_02232b28),
    HOST_OVERLAY_VAR(data_ov003_022331e0),
    HOST_OVERLAY_VAR(data_ov003_02232dfc),
    HOST_OVERLAY_VAR(data_ov003_02233fb8),
    HOST_OVERLAY_VAR(gFieldObjectManager),
    HOST_OVERLAY_VAR(data_ov003_02234024),
    HOST_OVERLAY_VAR(data_ov003_02233218),
    HOST_OVERLAY_VAR(data_ov003_02232708),
    HOST_OVERLAY_VAR(data_ov003_02233234),
    HOST_OVERLAY_VAR(data_ov003_02234048),
    HOST_OVERLAY_VAR(data_ov003_02233250),
    HOST_OVERLAY_VAR(data_ov003_022344e0),
    HOST_OVERLAY_VAR(data_ov003_022340d8),
    HOST_OVERLAY_VAR(data_ov003_02232634),
    HOST_OVERLAY_VAR(data_ov003_0223326c),
    HOST_OVERLAY_VAR(data_ov003_02233560),
    HOST_OVERLAY_VAR(data_ov003_022345a8),
    HOST_OVERLAY_VAR(data_ov003_022340fc),
    HOST_OVERLAY_VAR(data_ov003_02234508),
    HOST_OVERLAY_VAR(data_ov003_02232648),
    HOST_OVERLAY_VAR(data_ov003_02232798),
    HOST_OVERLAY_VAR(data_ov003_02232b88),
    HOST_OVERLAY_VAR(sBalloonSplashPos),
    HOST_OVERLAY_VAR(data_ov003_022332c0),
    HOST_OVERLAY_VAR(data_ov003_022326d0),
    HOST_OVERLAY_VAR(data_ov003_022332dc),
    HOST_OVERLAY_VAR(data_ov003_02234120),
    HOST_OVERLAY_VAR(data_ov003_022332f8),
    HOST_OVERLAY_VAR(data_ov003_02234530),
    HOST_OVERLAY_VAR(data_ov003_02232738),
    HOST_OVERLAY_VAR(data_ov003_02232638),
    HOST_OVERLAY_VAR(sFieldObjectModelHeapBuf),
    HOST_OVERLAY_VAR(data_ov003_02234558),
    HOST_OVERLAY_VAR(data_ov003_022328bc),
    HOST_OVERLAY_VAR(data_ov003_02232928),
    HOST_OVERLAY_VAR(data_ov003_0223418c),
    HOST_OVERLAY_VAR(data_ov003_02232728),
    HOST_OVERLAY_VAR(data_ov003_02232688),
    HOST_OVERLAY_VAR(data_ov003_022335c0),
    HOST_OVERLAY_VAR(data_ov003_022341f8),
    HOST_OVERLAY_VAR(data_ov003_02233384),
    HOST_OVERLAY_VAR(data_ov003_0223421c),
    HOST_OVERLAY_VAR(data_ov003_02232788),
    HOST_OVERLAY_VAR(sFieldObjectLoaders),
    HOST_OVERLAY_VAR(data_ov003_022326f8),
    HOST_OVERLAY_VAR(data_ov003_02234288),
    HOST_OVERLAY_VAR(data_ov003_022328d4),
    HOST_OVERLAY_VAR(data_ov003_02232904),
    HOST_OVERLAY_VAR(data_ov003_022335e0),
    HOST_OVERLAY_VAR(data_ov003_02233604),
    HOST_OVERLAY_VAR(data_ov003_02232978),
    HOST_OVERLAY_VAR(data_ov003_02232868),
    HOST_OVERLAY_VAR(data_ov003_02233628),
    HOST_OVERLAY_VAR(data_ov003_02233694),
    HOST_OVERLAY_VAR(data_ov003_02232850),
    HOST_OVERLAY_VAR(data_ov003_02232e44),
    HOST_OVERLAY_VAR(data_ov003_022336b8),
    HOST_OVERLAY_VAR(data_ov003_02232e60),
    HOST_OVERLAY_VAR(data_ov003_02234300),
    HOST_OVERLAY_VAR(data_ov003_02232e7c),
    HOST_OVERLAY_VAR(data_ov003_02232680),
    HOST_OVERLAY_VAR(data_ov003_02233724),
    HOST_OVERLAY_VAR(data_ov003_02233748),
    HOST_OVERLAY_VAR(data_ov003_022327c8),
    HOST_OVERLAY_VAR(data_ov003_02232c28),
    HOST_OVERLAY_VAR(data_ov003_022337b4),
    HOST_OVERLAY_VAR(data_ov003_02232a48),
    HOST_OVERLAY_VAR(data_ov003_022337d8),
    HOST_OVERLAY_VAR(data_ov003_02232650),
    HOST_OVERLAY_VAR(data_ov003_02234350),
    HOST_OVERLAY_VAR(data_ov003_02233440),
    HOST_OVERLAY_VAR(data_ov003_0223388c),
    HOST_OVERLAY_VAR(data_ov003_02232808),
    HOST_OVERLAY_VAR(data_ov003_02232f40),
    HOST_OVERLAY_VAR(data_ov003_02233940),
    HOST_OVERLAY_VAR(data_ov003_02233368),
    HOST_OVERLAY_VAR(data_ov003_02232934),
    HOST_OVERLAY_VAR(data_ov003_022329d8),
    HOST_OVERLAY_VAR(data_ov003_02233460),
    HOST_OVERLAY_VAR(data_ov003_022326d8),
    HOST_OVERLAY_VAR(data_ov003_02233a60),
    HOST_OVERLAY_VAR(data_ov003_02232740),
    HOST_OVERLAY_VAR(data_ov003_02232a08),
    HOST_OVERLAY_VAR(data_ov003_02233acc),
    HOST_OVERLAY_VAR(data_ov003_02232ab8),
    HOST_OVERLAY_VAR(data_ov003_02232a38),
    HOST_OVERLAY_VAR(data_ov003_02232940),
    HOST_OVERLAY_VAR(data_ov003_02232d0c),
    HOST_OVERLAY_VAR(data_ov003_02233ba4),
    HOST_OVERLAY_VAR(data_ov003_022327c0),
    HOST_OVERLAY_VAR(data_ov003_022334e0),
    HOST_OVERLAY_VAR(data_ov003_02233c34),
    HOST_OVERLAY_VAR(data_ov003_02232720),
    HOST_OVERLAY_VAR(data_ov003_02232a68),
    HOST_OVERLAY_VAR(data_ov003_02232758),
    HOST_OVERLAY_VAR(data_ov003_02233ca0),
    HOST_OVERLAY_VAR(data_ov003_022343c8),
    HOST_OVERLAY_VAR(data_ov003_02233074),
    HOST_OVERLAY_VAR(data_ov003_02232d54),
    HOST_OVERLAY_VAR(data_ov003_022328ec),
    HOST_OVERLAY_VAR(data_ov003_022330ac),
    HOST_OVERLAY_VAR(data_ov003_02233d54),
    HOST_OVERLAY_VAR(data_ov003_022330e4),
    HOST_OVERLAY_VAR(data_ov003_02233d9c),
    HOST_OVERLAY_VAR(data_ov003_02234490),
    HOST_OVERLAY_VAR(data_ov003_02232820),
    HOST_OVERLAY_VAR(data_ov003_0223318c),
    HOST_OVERLAY_VAR(data_ov003_022326e8),
    HOST_OVERLAY_VAR(data_ov003_022331a8),
    HOST_OVERLAY_VAR(data_ov003_022344b8),
    HOST_OVERLAY_VAR(data_ov003_02233ee0),
    HOST_OVERLAY_VAR(data_ov003_02233f4c),
    HOST_OVERLAY_VAR(data_ov003_02232b08),
    HOST_OVERLAY_VAR(data_ov003_02233f94),
    HOST_OVERLAY_VAR(data_ov003_02232b48),
    HOST_OVERLAY_VAR(data_ov003_02234000),
    HOST_OVERLAY_VAR(data_ov003_022327d0),
    HOST_OVERLAY_VAR(data_ov003_02232778),
    HOST_OVERLAY_VAR(data_ov003_0223406c),
    HOST_OVERLAY_VAR(data_ov003_02232e14),
    HOST_OVERLAY_VAR(data_ov003_02232818),
    HOST_OVERLAY_VAR(data_ov003_02232810),
    HOST_OVERLAY_VAR(data_ov003_022332a4),
    HOST_OVERLAY_VAR(data_ov003_02232b68),
    HOST_OVERLAY_VAR(data_ov003_02232e2c),
    HOST_OVERLAY_VAR(data_ov003_02232b98),
    HOST_OVERLAY_VAR(sFieldObjectAnimHeap),
    HOST_OVERLAY_VAR(data_ov003_02234144),
    HOST_OVERLAY_VAR(data_ov003_022328b0),
    HOST_OVERLAY_VAR(data_ov003_02232bb8),
    HOST_OVERLAY_VAR(data_ov003_022335a0),
    HOST_OVERLAY_VAR(data_ov003_02232bc8),
    HOST_OVERLAY_VAR(data_ov003_022326b8),
    HOST_OVERLAY_VAR(data_ov003_022341b0),
    HOST_OVERLAY_VAR(data_ov003_022341d4),
    HOST_OVERLAY_VAR(data_ov003_02232c50),
    HOST_OVERLAY_VAR(data_ov003_02234580),
    HOST_OVERLAY_VAR(data_ov003_02232bd8),
    HOST_OVERLAY_VAR(data_ov003_02232958),
    HOST_OVERLAY_VAR(data_ov003_02232968),
    HOST_OVERLAY_VAR(data_ov003_022342b0),
    HOST_OVERLAY_VAR(data_ov003_0223364c),
    HOST_OVERLAY_VAR(data_ov003_02233670),
    HOST_OVERLAY_VAR(data_ov003_0223291c),
    HOST_OVERLAY_VAR(data_ov003_022327a8),
    HOST_OVERLAY_VAR(data_ov003_02232e98),
    HOST_OVERLAY_VAR(data_ov003_02232838),
    HOST_OVERLAY_VAR(data_ov003_02232eec),
    HOST_OVERLAY_VAR(data_ov003_022328a4),
    HOST_OVERLAY_VAR(data_ov003_02232b78),
    HOST_OVERLAY_VAR(data_ov003_02232c14),
    HOST_OVERLAY_VAR(data_ov003_022326a0),
    HOST_OVERLAY_VAR(data_ov003_02233964),
    HOST_OVERLAY_VAR(data_ov003_022339f4),
    HOST_OVERLAY_VAR(data_ov003_022328f8),
    HOST_OVERLAY_VAR(data_ov003_02232af8),
    HOST_OVERLAY_VAR(data_ov003_02233b38),
    HOST_OVERLAY_VAR(data_ov003_02232cf4),
    HOST_OVERLAY_VAR(data_ov003_022330c8),
    HOST_OVERLAY_VAR(data_ov003_02232fb0),
    HOST_OVERLAY_VAR(data_ov003_02232fcc),
    HOST_OVERLAY_VAR(data_ov003_02232fe8),
    HOST_OVERLAY_VAR(data_ov003_02232d24),
    HOST_OVERLAY_VAR(data_ov003_02233cc4),
    HOST_OVERLAY_VAR(data_ov003_02232d6c),
    HOST_OVERLAY_VAR(data_ov003_02233dc0),
    HOST_OVERLAY_VAR(data_ov003_02233154),
    HOST_OVERLAY_VAR(data_ov003_02233e08),
    HOST_OVERLAY_VAR(data_ov003_022327e8),
    HOST_OVERLAY_VAR(data_ov003_02232de4),
    HOST_OVERLAY_VAR(data_ov003_02232b38),
    HOST_OVERLAY_VAR(data_ov003_02233fdc),
    HOST_OVERLAY_VAR(data_ov003_022331fc),
    HOST_OVERLAY_VAR(data_ov003_02234090),
    HOST_OVERLAY_VAR(data_ov003_02232790),
    HOST_OVERLAY_VAR(data_ov003_02232780),
    HOST_OVERLAY_VAR(data_ov003_02233580),
    HOST_OVERLAY_VAR(data_ov003_022327a0),
    HOST_OVERLAY_VAR(data_ov003_02233314),
    HOST_OVERLAY_VAR(data_ov003_0223334c),
    HOST_OVERLAY_VAR(data_ov003_02232880),
    HOST_OVERLAY_VAR(data_ov003_02232844),
    HOST_OVERLAY_VAR(data_ov003_022336dc),
    HOST_OVERLAY_VAR(data_ov003_022326f0),
    HOST_OVERLAY_VAR(data_ov003_0223376c),
    HOST_OVERLAY_VAR(data_ov003_022328c8),
    HOST_OVERLAY_VAR(data_ov003_02233844),
    HOST_OVERLAY_VAR(data_ov003_022338f8),
    HOST_OVERLAY_VAR(data_ov003_022339d0),
    HOST_OVERLAY_VAR(data_ov003_022328e0),
    HOST_OVERLAY_VAR(data_ov003_02233b5c),
    HOST_OVERLAY_VAR(data_ov003_02232710),
    HOST_OVERLAY_VAR(data_ov003_02232690),
    HOST_OVERLAY_VAR(data_ov003_02233c58),
    HOST_OVERLAY_VAR(data_ov003_02233058),
    HOST_OVERLAY_VAR(data_ov003_02232644),
    HOST_OVERLAY_VAR(data_ov003_0223311c),
    HOST_OVERLAY_VAR(data_ov003_02233de4),
    HOST_OVERLAY_VAR(data_ov003_02232678),
    HOST_OVERLAY_VAR(data_ov003_02232ae8),
    HOST_OVERLAY_VAR(data_ov003_02232698),
    HOST_OVERLAY_VAR(data_ov003_02232700),
    HOST_OVERLAY_VAR(data_ov003_02232730),
    HOST_OVERLAY_VAR(data_ov003_02232658),
    HOST_OVERLAY_VAR(data_ov003_022326c8),
    HOST_OVERLAY_VAR(data_ov003_02233330),
    HOST_OVERLAY_VAR(data_ov003_02234240),
    HOST_OVERLAY_VAR(data_ov003_022342d8),
    HOST_OVERLAY_VAR(data_ov003_02233700),
    HOST_OVERLAY_VAR(data_ov003_022329a8),
    HOST_OVERLAY_VAR(data_ov003_02232c94),
    HOST_OVERLAY_VAR(data_ov003_02233170),
    HOST_OVERLAY_VAR(data_ov003_022329e8),
    HOST_OVERLAY_VAR(data_ov003_02233bc8),
    HOST_OVERLAY_VAR(data_ov003_02232748),
    HOST_OVERLAY_VAR(data_ov003_02233d78),
    HOST_OVERLAY_VAR(data_ov003_0223282c),
    HOST_OVERLAY_VAR(data_ov003_022331c4),
    HOST_OVERLAY_VAR(data_ov003_02232b58),
    HOST_OVERLAY_VAR(data_ov003_022340b4),
    HOST_OVERLAY_VAR(data_ov003_02234168),
    HOST_OVERLAY_VAR(data_ov003_02232670),
    HOST_OVERLAY_VAR(data_ov003_022333a0),
    HOST_OVERLAY_VAR(data_ov003_022327e0),
    HOST_OVERLAY_VAR(data_ov003_02234378),
    HOST_OVERLAY_VAR(data_ov003_022334c0),
    HOST_OVERLAY_VAR(data_ov003_02232ad8),
    HOST_OVERLAY_VAR(data_ov003_02232ba8),
    HOST_OVERLAY_VAR(data_ov003_02234604),
    HOST_OVERLAY_VAR(data_ov003_02233a3c),
    HOST_OVERLAY_VAR(data_ov003_02232640),
    HOST_OVERLAY_VAR(data_ov003_02232750),
    HOST_OVERLAY_VAR(data_ov003_022333e0),
    HOST_OVERLAY_VARS_END
};
