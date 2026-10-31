#include "game/CollisionState.h"
#include "host_mem.h"
#include "host_shared_calls.h"
#include "proto/main/unk_0202fa70.h"
// mwcc-version: 1.2/sp2
// Translation unit 0x02004558-0x0201106c (the player object, class PlayerActor). Built by two compilers:
// this file (thunks need mwcc 1.2/sp2) and unk_02004558_extra.cpp (14 functions need 1.2/base); the
// functions and data objects are placed by address (config/usa/arm9/object_order.txt).
// Generated from 20 earlier source files: one
// namespace per old file holds that file's own declarations and its view of the object.
#include "types.h"
#include "gfx/VecFx32.h"
#include "actor/ActorProfile.h"
#include "net/CommManager.h"
#include "talk/TalkWindowState.h"
#include "gfx/NNSG3dRS.h"
#include "player/PendingFieldActionFx.h"
#include "player/Unk_02005f50_Area.h"
#include "player/FieldActionFxEvent.h"
#include "player/Unk_02006d14_Pair.h"
#include "player/Unk_02006d14_V3.h"
#include "player/Unk_02006d14_Trip.h"
#include "player/PlayerPickUpWork.h"
#include "player/Unk_02006d14_Ptr.h"
#include "player/PlayerInitWork.h"
#include "player/PlayerActionRequestBase.h"
#include "game/Unk_0200f6d4_V2.h"
#include "player/Unk_0200f070_V3.h"
#include "player/Unk_020107c8_Blk.h"
#include "player/PlayerNetActionArgs.h"
#include "player/PlayerU16ArgsRequest.h"
#include "talk/TalkMsgRequest.h"
#include "snd/SndSeEmitterKind99.h"
#include "snd/SndSeEmitterKind1.h"
#include "talk/MsgRequest.h"
#include "player/PlayerActionWork.h"
#include "actor/Character.h"
#include "actor/Actor.h"
#include "gfx/MatTexVramTask.h"
#include "player/PM_020076f0.h"
#include "player/PM_020063a0.h"
#include "player/PlayerWalkToRequest.h"
#include "gfx/MatTexPatAnim.h"
#include "sys/ProcBase.h"
#include "player/Unk_02007694.h"
#include "player/PlayerWalkToWork.h"
#include "player/PlayerAct76Request.h"
#include "player/Unk_02005e7c.h"
#include "player/PlayerAct30Request.h"
#include "player/PlayerPickUpReachRequest.h"
#include "player/PlayerWalkToArgs.h"
#include "player/PlayerActionRequest.h"
#include "player/PMRaw.h"
#include "snd/SndSeEmitter.h"
#include "actor/BlinkTimer.h"
#include "player/PlayerGlassesModelRef.h"
#include "player/PlayerFaceTexRef.h"
#include "player/PlayerBodyWorkRef.h"
#include "player/PlayerBodyModelRef.h"
#include "actor/CharaFaceAnimWorkRef.h"
#include "actor/CharaFaceAnimRef.h"
#include "actor/CharaClothTexRef.h"
#include "player/PlayerPaletteRef.h"
#include "player/Unk_0200bc78_Vec.h"
#include "player/AnimSlotRef.h"
#include "game/Unk_0203d820_Ptr.h"
#include "gfx/Mtx43.h"
#include "player/PlayerWalkWork.h"
#include "player/PlayerHead.h"
#include "player/PlayerChangeClothesArgs.h"
#include "player/PlayerAct10Args.h"
#include "player/Unk_0200b908_Obj.h"
#include "player/PlayerNoArgsRequest.h"
#include "player/PlayerPickUpReachArgs.h"
#include "player/PlayerPickUpArgs.h"
#include "player/PlayerPickUpFanfareArgs.h"
#include "player/PlayerAct30Args.h"
#include "player/PlayerAct32Locals.h"
#include "player/PlayerChangeHeldItemWork.h"
#include "player/Unk_020092c8_Loc.h"
#include "player/PlayerTurnToWork.h"
#include "player/PlayerAct76Args.h"

class PlayerActor;
class Unk_02007694;
class PlayerActor;
struct PlayerActionRequest;
struct Unk_02006d14_Pair;
struct Unk_0200b750_Pair;
struct PlayerActionRequest;
struct Unk_0200f6d4_V2;
struct Unk_020107c8_Blk;


// ---- member-function-pointer constants as named objects (see notes.txt)
typedef void (*PMF)();

// ---- library base class chain of the object (declarations only; vtables and code are in other units)






// ---- sound emitters of PlayerActor (snd/SndSeEmitterKind1.h, snd/SndSeEmitterKind99.h). SndSeEmitterKind1 has no key
// function: this unit emits its link-once vtable and D1 0x02004b48 / D0 0x02010fa4, so the destructor that the header
// declares out of line is defined inline here.
inline SndSeEmitterKind1::~SndSeEmitterKind1() {}










#include "player/PlayerActor.h"
#include "actor/NpcActor.h"
#include "actor/ActorCollider.h"
#include "actor/ActorPlacedCollider.h"
#include "game/TouchPicker.h"
#include "gfx/AnimFrameCtrl.h"
#include "gfx/AnimModel.h"
#include "gfx/CachedModel.h"
#include "gfx/Model.h"
#include "gfx/TwoLayerAnimModel.h"
#include "npc/NpcEmotionFx.h"
#include "player/FishBobber.h"
#include "player/PlayerData.h"
#include "player/PlayerId.h"
#include "snd/BgmManager.h"
#include "sys/Heap.h"
#include "gfx/ScreenTransition.h"
#include "gfx/Camera.h"
#include "proto/main/unk_020b4828.h"
#include "proto/autoload_2/unk_020e7fd4.h"
#include "proto/autoload_2/unk_020e8558.h"
#include "proto/autoload_2/unk_020e9a08.h"
#include "proto/autoload_2/unk_02128030.h"
#include "proto/main/unk_020039ec.h"
#include "proto/main/unk_02004558.h"
#include "proto/main/unk_0201106c.h"
#include "proto/main/unk_020119cc.h"
#include "proto/main/unk_0202e840.h"
#include "proto/main/unk_02034438.h"
#include "proto/main/unk_0203cbc0.h"
#include "proto/main/unk_0203ecec.h"
#include "proto/main/unk_020571f4.h"
#include "proto/main/unk_0205b7b0.h"
#include "proto/main/unk_0205d480.h"
#include "proto/main/unk_0205d7d8.h"
#include "proto/main/unk_0205dfa4.h"
#include "proto/main/unk_0205ef08.h"
#include "proto/main/unk_02063904.h"
#include "proto/main/unk_0206d988.h"
#include "proto/main/unk_0206e54c.h"
#include "proto/main/unk_0206fc44.h"
#include "proto/main/unk_020742f4.h"
#include "proto/main/unk_020815f8.h"
#include "proto/main/unk_0208eeac.h"
#include "proto/main/unk_02090268.h"
#include "proto/main/unk_02093ff0.h"
#include "proto/main/unk_020943dc.h"
#include "proto/main/unk_02094810.h"
#include "proto/main/unk_02095cdc.h"
#include "proto/main/unk_02097444.h"
#include "proto/main/unk_02097d1c.h"
#include "proto/main/unk_02098e90.h"
#include "proto/main/unk_0209c37c.h"
#include "proto/main/unk_0209c4a8.h"
#include "proto/main/unk_020abbcc.h"
#include "proto/main/unk_020af258.h"
#include "proto/main/unk_020b0e60.h"
#include "proto/main/unk_020b5e5c.h"
#include "proto/main/unk_020b7190.h"
#include "proto/main/unk_020b7d10.h"
#include "proto/ov003/unk_ov003_02204ce8.h"
#include "proto/ov003/unk_ov003_02225800.h"
#include "proto/ov004/unk_ov004_02233074.h"
#include "proto/autoload_2/unk_020ef150.h"
#include "proto/autoload_2/unk_020f0dec.h"
#include "proto/itcm/unk_01ffb4e8.h"
#include "proto/main/unk_0203c638.h"
#include "proto/main/unk_020411dc.h"
#include "proto/main/unk_0205c66c.h"
#include "proto/main/unk_0205c91c.h"
#include "proto/main/unk_0205edfc.h"
#include "proto/main/unk_020b0a80.h"
#include "proto/ov003/unk_ov003_0221ffb8.h"
#include "proto/ov003/unk_ov003_022192b4.h"
#include "proto/ov004/unk_ov004_0221e7a8.h"
#include "proto/ov004/unk_ov004_0222bed0.h"
#include "proto/ov004/unk_ov004_02235fd0.h"
#include "proto/main/unk_0204cc1c.h"
#include "unk_02004558_shared.h"



























// ---- unk_020044dc.cpp
namespace nA {
extern "C" {

s32 Field_DrawItemModel(u16 a, void *b, void *c, s32 d, s32 e, s32 f);
extern u8 data_020d5e20[];
extern CommManager *gCommManager;
union PM_02004ce8 {
    PMRaw raw;
    void (PlayerActor::*fn)();
};
}
}

// ---- unk_02004e0c.cpp
namespace nB {
extern "C" {

void MTX_RotX33_(MtxFx33 *out, fx32 s, fx32 c);
void MTX_RotY33_(MtxFx33 *out, fx32 s, fx32 c);
void JointCb_CalcCpuMatrix(void *a, NNSG3dRS *b, u32 c);
void JointCb_UseRestTranslation(NNSG3dRS *a, u32 b);
void PlayerActor_FieldUpdateTan(void *);
void PlayerActor_FieldPollPitfall(void *);
void PlayerActor_FieldCheckStepUnit(void *);
void PlayerActor_FieldRunStep(void *);
void PlayerActor_FieldCheckUnitAhead(void *);
extern s16 data_02135f44[];
extern CommManager *gCommManager;
extern s16 data_02135f44[];
union PM_02005294 {
    PMRaw raw;
    void (PlayerActor::*fn)();
};
extern PM_02005294 data_020d622c;
extern PM_02005294 data_020d621c;
extern PM_02005294 data_020d6214;
extern PM_02005294 data_020d619c;
extern PM_02005294 data_020d6204;
extern PM_02005294 data_020d61fc;
extern PM_02005294 data_020d61f4;
extern PM_02005294 data_020d61c4;
extern PM_02005294 data_020d61dc;
extern PM_02005294 data_020d658c;
extern PM_02005294 data_020d6b94;
extern PM_02005294 data_020d660c;
extern PM_02005294 data_020d672c;
extern PM_02005294 data_020d617c;
extern PM_02005294 data_020d6174;
extern PM_02005294 data_020d616c;
extern PM_02005294 data_020d6164;
extern PM_02005294 data_020d615c;
extern PM_02005294 data_020d6154;
extern PM_02005294 data_020d614c;
extern PM_02005294 data_020d6144;
extern PM_02005294 data_020d613c;
extern PM_02005294 data_020d6134;
extern PM_02005294 data_020d612c;
extern PM_02005294 data_020d64bc;
extern PM_02005294 data_020d64ac;
extern PM_02005294 data_020d6114;
extern PM_02005294 data_020d5e5c;
extern PM_02005294 data_020d6104;
extern PM_02005294 data_020d60fc;
extern PM_02005294 data_020d60f4;
extern PM_02005294 data_020d60ec;
extern PM_02005294 data_020d60e4;
extern PM_02005294 data_020d60ac;
extern PM_02005294 data_020d60a4;
extern PM_02005294 data_020d609c;
extern PM_02005294 data_020d6094;
extern PM_02005294 data_020d5eac;
extern PM_02005294 data_020d6084;
extern PM_02005294 data_020d607c;
extern PM_02005294 data_020d6074;
extern PM_02005294 data_020d606c;
extern PM_02005294 data_020d605c;
extern PM_02005294 data_020d6054;
extern PM_02005294 data_020d604c;
extern PM_02005294 data_020d6044;
extern PM_02005294 data_020d603c;
extern PM_02005294 data_020d6034;
extern PM_02005294 data_020d602c;
extern PM_02005294 data_020d6024;
extern PM_02005294 data_020d601c;
extern PM_02005294 data_020d6014;
extern PM_02005294 data_020d600c;
extern PM_02005294 data_020d6284;
extern PM_02005294 data_020d5fc4;
extern PM_02005294 data_020d625c;
extern PM_02005294 data_020d6254;
extern PM_02005294 data_020d5fe4;
extern PM_02005294 data_020d5fec;
extern PM_02005294 data_020d6194;
extern PM_02005294 data_020d61a4;
extern PM_02005294 data_020d61bc;
extern PM_02005294 data_020d61cc;
extern PM_02005294 data_020d61ec;
extern PM_02005294 data_020d6594;
extern PM_02005294 data_020d65f4;
extern PM_02005294 data_020d668c;
extern PM_02005294 data_020d698c;
extern PM_02005294 data_020d6d9c;
extern PM_02005294 data_020d5f84;
extern PM_02005294 data_020d5f7c;
extern PM_02005294 data_020d5f74;
extern PM_02005294 data_020d5f6c;
extern PM_02005294 data_020d5f64;
extern PM_02005294 data_020d5f5c;
extern PM_02005294 data_020d6124;
extern PM_02005294 data_020d5e64;
extern PM_02005294 data_020d5f44;
extern PM_02005294 data_020d5f3c;
extern PM_02005294 data_020d5f34;
extern PM_02005294 data_020d5f2c;
extern PM_02005294 data_020d5f24;
extern PM_02005294 data_020d5f1c;
extern PM_02005294 data_020d5f14;
extern PM_02005294 data_020d5eb4;
extern PM_02005294 data_020d5f04;
extern PM_02005294 data_020d5efc;
extern PM_02005294 data_020d5ef4;
extern PM_02005294 data_020d5ed4;
extern PM_02005294 data_020d5f9c;
extern PM_02005294 data_020d5fa4;
extern PM_02005294 data_020d6004;
extern PM_02005294 data_020d6264;
extern PM_02005294 data_020d618c;
extern PM_02005294 data_020d659c;
extern PM_02005294 data_020d6dcc;
extern PM_02005294 data_020d5e84;
extern PM_02005294 data_020d5e7c;
extern PM_02005294 data_020d5e74;
extern PM_02005294 data_020d64b4;
extern PM_02005294 data_020d5e8c;
extern PM_02005294 data_020d5e94;
extern PM_02005294 data_020d5ea4;
extern PM_02005294 data_020d5ebc;
extern PM_02005294 data_020d65a4;
extern PM_02005294 data_020d6994;
extern PM_02005294 data_020d5e54;
extern PM_02005294 data_020d611c;
extern PM_02005294 data_020d5e9c;
extern PM_02005294 data_020d5ec4;
extern PM_02005294 data_020d5ee4;
}
}

// ---- unk_02005e7c.cpp
namespace nC {
extern "C" {

void MI_CpuCopy8(void *dst, void *src, s32 n);
extern PM_020063a0 data_020d66ec;
extern PM_020063a0 data_020d66dc;
extern PM_020063a0 data_020d66d4;
extern PM_020063a0 data_020d66cc;
extern PM_020063a0 data_020d66c4;
extern PM_020063a0 data_020d66bc;
extern PM_020063a0 data_020d66b4;
extern PM_020063a0 data_020d66ac;
extern PM_020063a0 data_020d66a4;
extern PM_020063a0 data_020d6694;
extern PM_020063a0 data_020d620c;
extern PM_020063a0 data_020d666c;
extern PM_020063a0 data_020d6664;
extern PM_020063a0 data_020d663c;
extern PM_020063a0 data_020d6634;
extern PM_020063a0 data_020d662c;
extern PM_020063a0 data_020d6624;
extern PM_020063a0 data_020d661c;
extern PM_020063a0 data_020d6614;
extern PM_020063a0 data_020d5f54;
extern PM_020063a0 data_020d5e6c;
extern PM_020063a0 data_020d6be4;
extern PM_020063a0 data_020d65b4;
extern PM_020063a0 data_020d65cc;
extern PM_020063a0 data_020d65d4;
extern PM_020063a0 data_020d65dc;
extern PM_020063a0 data_020d65ec;
extern PM_020063a0 data_020d65fc;
extern PM_020063a0 data_020d670c;
extern PM_020063a0 data_020d6714;
extern PM_020063a0 data_020d674c;
extern PM_020063a0 data_020d6754;
extern PM_020063a0 data_020d6dbc;
extern PM_020063a0 data_020d656c;
extern PM_020063a0 data_020d6564;
extern PM_020063a0 data_020d655c;
extern PM_020063a0 data_020d6554;
extern PM_020063a0 data_020d654c;
extern PM_020063a0 data_020d6544;
extern PM_020063a0 data_020d653c;
extern PM_020063a0 data_020d6534;
extern PM_020063a0 data_020d652c;
extern PM_020063a0 data_020d651c;
extern PM_020063a0 data_020d6514;
extern PM_020063a0 data_020d650c;
extern PM_020063a0 data_020d6504;
extern PM_020063a0 data_020d64fc;
extern PM_020063a0 data_020d64f4;
extern PM_020063a0 data_020d64ec;
extern PM_020063a0 data_020d64e4;
extern PM_020063a0 data_020d64dc;
extern PM_020063a0 data_020d64d4;
extern PM_020063a0 data_020d64cc;
extern PM_020063a0 data_020d64c4;
extern PM_020063a0 data_020d6bec;
extern PM_020063a0 data_020d6bdc;
extern PM_020063a0 data_020d6bd4;
extern PM_020063a0 data_020d64a4;
extern PM_020063a0 data_020d649c;
extern PM_020063a0 data_020d6494;
extern PM_020063a0 data_020d5f4c;
extern PM_020063a0 data_020d6484;
extern PM_020063a0 data_020d647c;
extern PM_020063a0 data_020d6474;
extern PM_020063a0 data_020d646c;
extern PM_020063a0 data_020d6464;
extern PM_020063a0 data_020d645c;
extern PM_020063a0 data_020d6454;
extern PM_020063a0 data_020d644c;
extern PM_020063a0 data_020d6444;
extern PM_020063a0 data_020d643c;
extern PM_020063a0 data_020d6434;
extern PM_020063a0 data_020d642c;
extern PM_020063a0 data_020d6424;
extern PM_020063a0 data_020d641c;
extern PM_020063a0 data_020d6414;
extern PM_020063a0 data_020d640c;
extern PM_020063a0 data_020d6404;
extern PM_020063a0 data_020d63fc;
extern PM_020063a0 data_020d63f4;
extern PM_020063a0 data_020d63ec;
extern PM_020063a0 data_020d63e4;
extern PM_020063a0 data_020d63dc;
extern PM_020063a0 data_020d63d4;
extern PM_020063a0 data_020d63cc;
extern PM_020063a0 data_020d63c4;
extern PM_020063a0 data_020d63bc;
extern PM_020063a0 data_020d63b4;
extern PM_020063a0 data_020d63ac;
extern PM_020063a0 data_020d6394;
extern PM_020063a0 data_020d5f0c;
extern PM_020063a0 data_020d6384;
extern PM_020063a0 data_020d637c;
extern PM_020063a0 data_020d6374;
extern PM_020063a0 data_020d635c;
extern PM_020063a0 data_020d634c;
extern PM_020063a0 data_020d6344;
extern PM_020063a0 data_020d633c;
extern PM_020063a0 data_020d6334;
extern PM_020063a0 data_020d632c;
extern PM_020063a0 data_020d6324;
extern PM_020063a0 data_020d631c;
extern PM_020063a0 data_020d6314;
extern PM_020063a0 data_020d630c;
extern PM_020063a0 data_020d62d4;
extern PM_020063a0 data_020d62cc;
extern PM_020063a0 data_020d62c4;
extern PM_020063a0 data_020d62bc;
extern PM_020063a0 data_020d62b4;
extern PM_020063a0 data_020d62ac;
extern PM_020063a0 data_020d62a4;
}
}

// ---- unk_02006d14.cpp
namespace nD {
extern "C" {

typedef void (PlayerActor::*Unk_02006d14_Fn)(PlayerActionRequest*, u32);
extern CommManager* gCommManager;
union PM_02006d14 {
    PMRaw raw;
    Unk_02006d14_Fn fn;
};
extern PM_02006d14 data_020d6b9c;
extern PM_02006d14 data_020d610c;
extern PM_02006d14 data_020d6b84;
extern PM_02006d14 data_020d6b7c;
extern PM_02006d14 data_020d6b74;
extern PM_02006d14 data_020d6b6c;
extern PM_02006d14 data_020d6b64;
extern PM_02006d14 data_020d6b5c;
extern PM_02006d14 data_020d6b54;
extern PM_02006d14 data_020d6b44;
extern PM_02006d14 data_020d6b3c;
extern PM_02006d14 data_020d6b1c;
extern PM_02006d14 data_020d6b14;
extern PM_02006d14 data_020d6aec;
extern PM_02006d14 data_020d6ae4;
extern PM_02006d14 data_020d6adc;
extern PM_02006d14 data_020d6ad4;
extern PM_02006d14 data_020d6acc;
extern PM_02006d14 data_020d6ac4;
extern PM_02006d14 data_020d6abc;
extern PM_02006d14 data_020d6ab4;
extern PM_02006d14 data_020d6aac;
extern PM_02006d14 data_020d6aa4;
extern PM_02006d14 data_020d6a9c;
extern PM_02006d14 data_020d6a94;
extern PM_02006d14 data_020d6a8c;
extern PM_02006d14 data_020d6a84;
extern PM_02006d14 data_020d6a7c;
extern PM_02006d14 data_020d6a74;
extern PM_02006d14 data_020d6a6c;
extern PM_02006d14 data_020d6a64;
extern PM_02006d14 data_020d6a5c;
extern PM_02006d14 data_020d6a54;
extern PM_02006d14 data_020d6a1c;
extern PM_02006d14 data_020d6a14;
extern PM_02006d14 data_020d6a0c;
extern PM_02006d14 data_020d6a04;
extern PM_02006d14 data_020d69fc;
extern PM_02006d14 data_020d69f4;
extern PM_02006d14 data_020d69ec;
extern PM_02006d14 data_020d69e4;
extern PM_02006d14 data_020d69dc;
extern PM_02006d14 data_020d69cc;
extern PM_02006d14 data_020d69c4;
extern PM_02006d14 data_020d69bc;
extern PM_02006d14 data_020d69b4;
extern PM_02006d14 data_020d69ac;
extern PM_02006d14 data_020d69a4;
extern PM_02006d14 data_020d699c;
extern PM_02006d14 data_020d638c;
extern PM_02006d14 data_020d608c;
extern PM_02006d14 data_020d6984;
extern PM_02006d14 data_020d697c;
extern PM_02006d14 data_020d6974;
extern PM_02006d14 data_020d696c;
extern PM_02006d14 data_020d6964;
extern PM_02006d14 data_020d695c;
extern PM_02006d14 data_020d6954;
extern PM_02006d14 data_020d694c;
extern PM_02006d14 data_020d6944;
extern PM_02006d14 data_020d693c;
extern PM_02006d14 data_020d6934;
extern PM_02006d14 data_020d692c;
extern PM_02006d14 data_020d6924;
extern PM_02006d14 data_020d691c;
extern PM_02006d14 data_020d6914;
extern PM_02006d14 data_020d690c;
extern PM_02006d14 data_020d6904;
extern PM_02006d14 data_020d68fc;
extern PM_02006d14 data_020d68f4;
extern PM_02006d14 data_020d68ec;
extern PM_02006d14 data_020d68e4;
extern PM_02006d14 data_020d68dc;
extern PM_02006d14 data_020d68d4;
extern PM_02006d14 data_020d68cc;
extern PM_02006d14 data_020d68c4;
extern PM_02006d14 data_020d68bc;
extern PM_02006d14 data_020d68b4;
extern PM_02006d14 data_020d68ac;
extern PM_02006d14 data_020d68a4;
extern PM_02006d14 data_020d689c;
extern PM_02006d14 data_020d6894;
extern PM_02006d14 data_020d688c;
extern PM_02006d14 data_020d6884;
extern PM_02006d14 data_020d687c;
extern PM_02006d14 data_020d6874;
extern PM_02006d14 data_020d686c;
extern PM_02006d14 data_020d6864;
extern PM_02006d14 data_020d685c;
extern PM_02006d14 data_020d6844;
extern PM_02006d14 data_020d683c;
extern PM_02006d14 data_020d6834;
extern PM_02006d14 data_020d682c;
extern PM_02006d14 data_020d6824;
extern PM_02006d14 data_020d680c;
extern PM_02006d14 data_020d67fc;
extern PM_02006d14 data_020d67f4;
extern PM_02006d14 data_020d67ec;
extern PM_02006d14 data_020d67e4;
extern PM_02006d14 data_020d67dc;
extern PM_02006d14 data_020d67d4;
extern PM_02006d14 data_020d67cc;
extern PM_02006d14 data_020d67c4;
extern PM_02006d14 data_020d67bc;
extern PM_02006d14 data_020d6784;
extern PM_02006d14 data_020d677c;
extern PM_02006d14 data_020d6774;
extern PM_02006d14 data_020d627c;
extern PM_02006d14 data_020d6764;
extern PM_02006d14 data_020d675c;
extern PM_02006d14 data_020d626c;
}
}

// ---- unk_02007694.cpp
namespace nE {
extern "C" {

void MI_CpuFill8(void *p, u32 v, u32 n);
s32 FX_Div(s32 a, s32 b);
void MTX_MultVec43(VecFx32 *v, Mtx43 *m, VecFx32 *out);
extern PM_020076f0 data_020d6d74;
extern PM_020076f0 data_020d6d64;
extern PM_020076f0 data_020d6d5c;
extern PM_020076f0 data_020d6d54;
extern PM_020076f0 data_020d6d4c;
extern PM_020076f0 data_020d6d44;
extern PM_020076f0 data_020d6d34;
extern PM_020076f0 data_020d6d1c;
extern PM_020076f0 data_020d6d14;
extern PM_020076f0 data_020d6d0c;
extern PM_020076f0 data_020d6d04;
extern PM_020076f0 data_020d6cfc;
extern PM_020076f0 data_020d6cf4;
extern PM_020076f0 data_020d6cec;
extern PM_020076f0 data_020d6ce4;
extern PM_020076f0 data_020d6cdc;
extern PM_020076f0 data_020d6cd4;
extern PM_020076f0 data_020d6ccc;
extern PM_020076f0 data_020d6cc4;
extern PM_020076f0 data_020d6cbc;
extern PM_020076f0 data_020d6cac;
extern PM_020076f0 data_020d6ca4;
extern PM_020076f0 data_020d6c9c;
extern PM_020076f0 data_020d6c94;
extern PM_020076f0 data_020d6c8c;
extern PM_020076f0 data_020d6c84;
extern PM_020076f0 data_020d6c7c;
extern PM_020076f0 data_020d6c74;
extern PM_020076f0 data_020d6c6c;
extern PM_020076f0 data_020d6c64;
extern PM_020076f0 data_020d6c5c;
extern PM_020076f0 data_020d6c54;
extern PM_020076f0 data_020d6c4c;
extern PM_020076f0 data_020d6c44;
extern PM_020076f0 data_020d6c3c;
extern PM_020076f0 data_020d6c34;
extern PM_020076f0 data_020d6c2c;
extern PM_020076f0 data_020d6c24;
extern PM_020076f0 data_020d6c1c;
extern PM_020076f0 data_020d6c14;
extern PM_020076f0 data_020d6c0c;
}
}

// ---- unk_02008040.cpp
namespace nF {
extern "C" {

extern CommManager *gCommManager;
void Camera_SetMode4();
void Camera_SetModeDefault();
static inline BOOL Unk_02008858_IsZero(u8 v) {
    return v == 0 ? TRUE : FALSE;
}
}
}

// ---- unk_02008cc0.cpp
namespace nG {
extern "C" {

inline s32 Unk_0200905c_abs(s32 x) { return x < 0 ? -x : x; }
extern CommManager *gCommManager;
extern s16 data_02135f44[];
s32 FX_Div(s32, s32);
s32 func_01ffcb0c(s32, s32);
s32 Math_Atan2(s32, s32);
void Clock_GetDateTime(void *);
void DateTime_SubDays(void *, s32);
}
}

// ---- unk_020095f8.cpp
namespace nH {
extern "C" {

extern CommManager* gCommManager;
extern s16 data_02135f44[];
s32 func_01ffcb0c(s32 a, s32 b);
s32 _s32_div_f(s32 a, s32 b);
s32 Math_Atan2(s32 a, s32 b);
s32 Math_AngleDiffAbs(s32 a, s32 b);
static inline BOOL Unk_02009624_Check()
{
    if (gFieldSceneKind == 0) {
        return TRUE;
    }
    return FALSE;
}
}
}

// ---- unk_02009f68.cpp
namespace nI {
extern "C" {

static inline BOOL Unk_0200a114_IsZero(u8* p)
{
    if (*p == 0) return TRUE;
    return FALSE;
}
extern CommManager* gCommManager;
extern BlockMap *gSceneBlockMap;
void Camera_SetMode4();
void Camera_SetModeDefault();
}
}

// ---- unk_0200abc8.cpp
namespace nJ {
extern "C" {

extern CommManager *gCommManager;
void VillagerTrend_NotifyUnk6(void* p);
extern BlockMap *gSceneBlockMap;
s32 _s32_div_f(s32 a, s32 b);
static inline BOOL Unk_0200add8_R1(u16* p, u32 lo, u32 hi) { BOOL r = FALSE; if (*p >= lo && *p <= hi) r = TRUE; return r; }
static inline BOOL Unk_0200add8_InRange(u32 c, u32 lo, u32 hi) { BOOL r = FALSE; if (c >= lo && c <= hi) r = TRUE; return r; }
static inline BOOL Unk_0200add8_IsFFF1(u16* p, u16* t) {
    BOOL r;
    if (Item_IsFurniture(p)) {
        *t = 0xfff1;
        r = Item_GetFurnitureIndex(p) == Item_GetFurnitureIndex(t) ? TRUE : FALSE;
    } else {
        r = *p == 0xfff1 ? TRUE : FALSE;
    }
    return r;
}
}
}

// ---- unk_0200b510.cpp
namespace nK {
extern "C" {

static inline void func_0200bc78_sub(VecFx32Ctor *o, VecFx32Ctor *a, VecFx32Ctor *b) {
    o->x = a->x - b->x;
    o->z = a->z - b->z;
}
extern BlockMap *gSceneBlockMap;
extern CommManager *gCommManager;
BOOL Scene_InHouseRoom();
s32 Math_Atan2(s32, s32);
}
}

// ---- unk_0200be2c.cpp
namespace nL {
extern "C" {

s32 func_01ffcb0c(s32 a, s32 b);
s32 Math_AngleDiffAbs(s32 a, s32 b);
void PlayerActor_RequestTrip(Unk_02007694 *o, u32 a, s32 b);
extern CommManager *gCommManager;
extern s16 data_02135f44[];
}
}

// ---- unk_0200c778.cpp
namespace nM {
extern "C" {

extern s16 data_02135f44[];
extern CommManager *gCommManager;
extern BgmManager *data_021c1b3c;  // (the BGM manager)
s32 FX_Div(s32 a, s32 b);
s32 func_01ffcb0c(s32 a, s32 b);
s32 FX_Sqrt(s32 a);
s32 Scene_InHouseRoom();
s32 Scene_InUnk6To8();
s32 Scene_InTown();
s32 Scene_NoPlayerInUnsharedScene();
void FishShadow_RunAi(void *t, s32 a, s32 b, s32 c);
void PlayerActor_RequestDoorExit(void *t, s32 a, s32 b);
void PlayerActor_RequestFaint(void *t, s32 a, s32 b, s32 c);
void PlayerActor_RequestAct3C(void *t, s32 a, s32 b);
void PlayerActor_RequestAct6E(void *t, s32 a, s32 b);
void PlayerActor_RequestAct43(void *t, s32 a, s32 b);
void PlayerActor_RequestAct44(void *t, s32 a, s32 b);
void PlayerActor_RequestSit(void *t, s32 a, s32 b, s32 c);
void PlayerActor_RequestDoorWalkIn(void *t, void *a, s32 b, s32 c);
void PlayerActor_RequestExitWalkIn(void *t, void *a, s32 b, s32 c);
}
}

// ---- unk_0200d2b4.cpp
namespace nN {
extern "C" {

s32 Scene_NoPlayerInUnsharedScene();
s32 Math_Atan2(s32 a, s32 b);
s32 func_01ffcb0c(s32 a, s32 b);
s32 FX_Div(s32 a, s32 b);
s32 FX_Sqrt(s32 a);
extern s32 gGfxMainOnTop;
extern BlockMap *gSceneBlockMap;
extern s16 data_02135f44[];
extern CommManager *gCommManager;
extern Camera *gCamera;
static inline BOOL Unk_0200d64c_InRange(u16 h) {
    if (h >= 0xfc && h <= 0xfd) {
        return TRUE;
    }
    return FALSE;
}
static inline BOOL Unk_0200d64c_IsTwo() {
    return gScreenTransition.state == 2;
}
static inline BOOL Unk_0200d64c_Both() {
    return gTouchPrevHeld && gTouchPrevChanged;
}
}
}

// ---- unk_0200dde0.cpp
namespace nO {
extern "C" {

void *NNS_G3dGetTex(void *p);
s32 func_01ffcb0c(s32 a, s32 b);
s32 Math_Atan2(s32 a, s32 b);
extern BgmManager *data_021c1b3c;
extern u8 gTalkMsgIndexEnd;
}
}

// ---- unk_0200e758.cpp
namespace nP {
extern "C" {

extern CommManager *gCommManager;
extern s16 data_02135f44[];
s32 Scene_GetPrevious();
s32 Math_Atan2(s32 a, s32 b);
s32 Math_AngleDiffAbs(s32 a, s32 b);
s32 func_01ffcb0c(s32 a, s32 b);
static inline BOOL Unk_0200ec54_Bit(u32 f, u32 m)
{
    return (f & m) != 0 ? TRUE : FALSE;
}
}
}

// ---- unk_0200f070.cpp
namespace nQ {
extern "C" {

extern s16 data_02135f44[];
extern CommManager *gCommManager;
extern BlockMap *gSceneBlockMap;
s32 func_01ffcb0c(s32 a, s32 b);
void MTX_MultVec43(VecFx32 *v, Mtx43 *m, VecFx32 *out);
Unk_0200f17c_Date *PlayerSession_GetLastPlayDate();
// Defined as s32 PlayerData::getLastPlayDate(); GCC returns the struct through a hidden pointer (-fpcc-struct-return)
void Clock_GetDateTime(void *p);
void MI_CpuCopy8(void *a, void *b, s32 n);
s32 DateTime_Compare(void *a, void *b, s32 n);
s32 ItemInfo_GetHoldableIndex(Unk_0200f660_S *s);
void PlayerActor_RequestPickUpItem(PlayerActor *o, Unk_0200f6d4_V2 v, s32 a, s32 b, s32 c);
void PlayerActor_RequestPluckReach(PlayerActor *o, Unk_0200f6d4_V2 v, s32 a, s32 b);
static inline void Unk_0200f070_Set(VecFx32 *r, s32 x, s32 y, s32 z)
{
    r->x = x;
    r->y = y;
    r->z = z;
}
}
}

// ---- unk_0200f9bc.cpp
namespace nR {
extern "C" {

inline BOOL Unk_0200f9d4_IsOne(u8 v) { return v == 1 ? TRUE : FALSE; }
inline BOOL Unk_020102a0_IsZero(u8 v) { return v == 0 ? TRUE : FALSE; }
extern u32 data_020cb35c;
extern u32 data_020cb360;
s32 PlayerActor_RoomInteractAt(void *, s32, s32);
s32 PlayerActor_RoomUseTool(void *);
s32 Math_Atan2(s32, s32);
s32 Math_AngleDiffAbs(s32, s32);
void *NNS_G3dGetTex(void *p);
void *Character_FindInteractionTarget(void *);
s32 func_01ffcb0c(s32, s32);
void *func_021065dc(void *a);
void *func_021065f8(void *, u32);
inline BOOL Unk_02010154_In(u16 *p) { BOOL r = FALSE; u16 v = *p; if (*p >= 0x1380 && v <= 0x139f) r = TRUE; return r; }
}
}

// ---- unk_020102ec.cpp
namespace nS {
extern "C" {

extern CommManager *gCommManager;
BOOL Scene_InUnk6To8(void);
void *func_021065dc(void *a);
void *func_021065f8(void *a, u32 b);
void BlendAnimModel_Play(void *p, void *anm, s32 blend, s32 mode, s32 speed, u16 frame, u16 numFrames);
s32 DateTime_DiffDays(void *a, void *b);
}
}

// ---- unk_02010c74.cpp
namespace nT {
extern "C" {

s32 func_01ffcb0c(s32 a, s32 b);
s32 Ground_GetExitAtPos(VecFx32 *p);
BOOL Scene_InUnk6To8(void);
extern CommManager *gCommManager;
extern u8 sHudObjCharPathTen2[];
}
}



namespace nPM {
extern "C" {
void PlayerActor_MainReleaseCreature();
void PlayerActor_MainLieInBed();
void PlayerActor_MainGetOutOfBedCheck();
void PlayerActor_MainGetOutOfBed();
void PlayerActor_MainBedRollCheck();
void PlayerActor_MainBedRollBlocked();
void PlayerActor_MainBedRoll();
void PlayerActor_MainBedApproach();
void PlayerActor_MainGetIntoBed();
void PlayerActor_MainAct11();
void PlayerActor_MainAct12();
void PlayerActor_MainPluckReach();
void PlayerActor_MainPluck();
void PlayerActor_MainPickUpItem();
void PlayerActor_MainFtrGrabApproach();
void PlayerActor_MainFtrHold();
void PlayerActor_MainFtrRotate();
void FishCatch_StateStart();
void PlayerActor_MainFtrPull();
void PlayerActor_MainFtrPushMove();
void FishCatch_Reset();
void PlayerActor_MainSeatApproach();
void PlayerActor_MainSitDownFront();
void PlayerActor_MainSitDownSide2();
void PlayerActor_MainSitDownSide1();
void PlayerActor_MainSit();
void PlayerActor_MainStandUpSide2();
void PlayerActor_MainStandUpCheck();
void PlayerActor_MainStandUpFront();
void PlayerActor_MainStorageOpen();
void PlayerActor_MainStorageHold();
void PlayerActor_MainStorageClose();
void PlayerActor_MainAct36();
void PlayerActor_MainAct37();
void PlayerActor_MainDoorApproach();
void PlayerActor_MainDoorEnter();
void PlayerActor_MainDoorExit();
void PlayerActor_MainStowItem();
void PlayerActor_MainStowUmbrella();
void PlayerActor_MainLeaveRoom();
void PlayerActor_MainAct41();
void PlayerActor_MainAct42();
void PlayerActor_MainAct43();
void PlayerActor_MainAct44();
void PlayerActor_MainUmbrellaSpin();
void PlayerActor_MainAxeSwing();
void PlayerActor_MainAxeFollowThrough();
void PlayerActor_MainAct48();
void PlayerActor_MainAxeStrike();
void PlayerActor_MainAxeChop();
void PlayerActor_MainAxeBreak();
void PlayerActor_MainAxeBrokenMessage();
void PlayerActor_MainFishCast();
void PlayerActor_MainFishCastFail();
void PlayerActor_MainFishWait();
void PlayerActor_MainFishHook();
void PlayerActor_MainFishReelIn();
void PlayerActor_MainFishEscape();
void PlayerActor_MainFishLand();
void PlayerActor_MainFishShowCatch();
void PlayerActor_MainFishStore();
void PlayerActor_MainBugNetSwing();
void PlayerActor_MainInsectShowCatch();
void PlayerActor_MainInsectStore();
void PlayerActor_MainShovelReady();
void PlayerActor_MainShovelWait();
void PlayerActor_MainAct5B();
void PlayerActor_MainAct5C();
void PlayerActor_MainShovelStrike();
void PlayerActor_MainDig();
void PlayerActor_MainDigUpItem();
void PlayerActor_MainDugItemStore();
void PlayerActor_MainBuryItem();
void PlayerActor_MainFillHole();
void PlayerActor_MainWateringCan();
void PlayerActor_MainSlingshot();
void PlayerActor_MainSlingshotWatch();
void PlayerActor_MainAct66();
void PlayerActor_MainAct67();
void PlayerActor_MainTreeShake();
void PlayerActor_MainTreeShakeRelease();
void PlayerActor_MainMailboxOpen();
void PlayerActor_MainMailboxWait();
void PlayerActor_MainMailboxClose();
void PlayerActor_MainFaint();
void PlayerActor_MainAct6E();
void PlayerActor_MainTrip();
void PlayerActor_MainAct72();
void PlayerActor_MainPitfallFall();
void PlayerActor_MainPitfallStruggle();
void PlayerActor_MainPitfallClimbOut();
void PlayerActor_MainBeeSting();
void PlayerActor_MainHaircutCut();
void PlayerActor_MainHaircutFinish();
void PlayerActor_MainPhonePickUp();
void PlayerActor_MainPhoneHold();
void PlayerActor_MainPhoneHangUp();
void PlayerActor_MainAct80();
void PlayerActor_MainFirework();
void PlayerActor_MainAct82();
void PlayerActor_MainThrowBottle();
void PlayerActor_MainDrinkCoffee();
void FieldObj_FreeCedarFileBufs();
void PlayerActor_MainDoorWalkOut();
void PlayerActor_MainExitWalkOut();
void PlayerActor_MainExitWalkIn();
void PlayerActor_MainFishRelease();
void PlayerActor_NetReleaseCreature();
void PlayerActor_NetLieInBed();
void PlayerActor_NetGetOutOfBed();
void PlayerActor_NetBedRollCheck();
void PlayerActor_NetBedRollBlocked();
void PlayerActor_NetBedRoll();
void PlayerActor_NetBedApproach();
void PlayerActor_NetGetIntoBed();
void PlayerActor_NetAct11();
void PlayerActor_NetPluckReach();
void PlayerActor_NetFtrGrabApproach();
void PlayerActor_NetFtrHold();
void PlayerActor_NetFtrPush();
void PlayerActor_NetFtrPull();
void PlayerActor_NetSeatApproach();
void PlayerActor_NetSitDownFront();
void PlayerActor_NetSitDownSide2();
void PlayerActor_NetSitDownSide1();
void PlayerActor_NetSit();
void PlayerActor_NetStandUpSide2();
void PlayerActor_NetStandUpSide1();
void PlayerActor_NetStandUpFront();
void PlayerActor_NetStorageOpen();
void PlayerActor_NetStorageHold();
void PlayerActor_NetStorageClose();
void PlayerActor_NetAct36();
void PlayerActor_NetAct37();
void PlayerActor_NetDoorApproach();
void PlayerActor_NetDoorEnter();
void PlayerActor_NetDoorEntered();
void PlayerActor_NetDoorExit();
void PlayerActor_NetAct3C();
void PlayerActor_NetStowItem();
void PlayerActor_NetStowUmbrella();
void PlayerActor_NetLeaveRoom();
void PlayerActor_NetAct41();
void PlayerActor_NetAct42();
void PlayerActor_NetAct43();
void PlayerActor_NetUmbrellaSpin();
void PlayerActor_NetAxeSwing();
void PlayerActor_NetAxeFollowThrough();
void PlayerActor_NetAct48();
void PlayerActor_NetAxeStrike();
void PlayerActor_NetAxeChop();
void PlayerActor_NetAxeBreak();
void PlayerActor_NetAxeBrokenMessage();
void PlayerActor_NetFishCast();
void PlayerActor_NetFishCastFail();
void PlayerActor_NetFishWait();
void PlayerActor_NetFishHook();
void PlayerActor_NetFishReelIn();
void PlayerActor_NetFishEscape();
void PlayerActor_NetFishLand();
void PlayerActor_NetFishShowCatch();
void PlayerActor_NetFishStore();
void PlayerActor_NetBugNetSwing();
void PlayerActor_NetInsectShowCatch();
void PlayerActor_NetInsectStore();
void PlayerActor_NetShovelReady();
void PlayerActor_NetShovelWait();
void PlayerActor_NetAct5B();
void PlayerActor_NetAct5C();
void PlayerActor_NetShovelStrike();
void PlayerActor_NetDig();
void PlayerActor_NetDigUpItem();
void PlayerActor_NetDugItemStore();
void PlayerActor_NetWateringCan();
void PlayerActor_NetSlingshot();
void PlayerActor_NetAct66();
void PlayerActor_NetAct67();
void PlayerActor_NetTreeShake();
void PlayerActor_NetMailboxOpen();
void PlayerActor_NetMailboxWait();
void PlayerActor_NetMailboxClose();
void PlayerActor_NetFaint();
void PlayerActor_NetAct6E();
void PlayerActor_NetTrip();
void PlayerActor_NetAct72();
void PlayerActor_NetPitfallFall();
void PlayerActor_NetPitfallStruggle();
void PlayerActor_NetPitfallClimbOut();
void PlayerActor_NetHaircutStart();
void PlayerActor_NetHaircutCut();
void PlayerActor_NetHaircutFinish();
void PlayerActor_NetPhonePickUp();
void PlayerActor_NetPhoneHold();
void PlayerActor_NetPhoneHangUp();
void PlayerActor_NetAct80();
void PlayerActor_NetFirework();
void PlayerActor_NetAct82();
void PlayerActor_NetThrowBottle();
void PlayerActor_NetDoorWalkIn();
void PlayerActor_NetDoorWalkOut();
void PlayerActor_NetExitWalkOut();
void PlayerActor_NetExitWalkIn();
void PlayerActor_NetFishRelease();
void PlayerActor_SetupReleaseCreature();
void PlayerActor_SetupLieInBed();
void PlayerActor_SetupGetOutOfBedCheck();
void PlayerActor_SetupGetOutOfBed();
void PlayerActor_SetupBedRollCheck();
void PlayerActor_SetupBedRollBlocked();
void PlayerActor_SetupBedRoll();
void PlayerActor_SetupBedApproach();
void PlayerActor_SetupGetIntoBed();
void PlayerActor_SetupAct11();
void PlayerActor_SetupAct12();
void PlayerActor_SetupPluckReach();
void PlayerActor_SetupPluck();
void PlayerActor_SetupPickUpItem();
void PlayerActor_SetupFtrGrabApproach();
void PlayerActor_SetupFtrHold();
void PlayerActor_SetupFtrRotate();
void PlayerActor_SetupFtrPush();
void PlayerActor_SetupFtrPull();
void PlayerActor_SetupFtrPushMove();
void PlayerActor_SetupFtrPullMove();
void PlayerActor_SetupSeatApproach();
void PlayerActor_SetupSitDownFront();
void PlayerActor_SetupSitDownSide2();
void PlayerActor_SetupSitDownSide1();
void PlayerActor_SetupSit();
void PlayerActor_SetupStandUpSide2();
void PlayerActor_SetupStandUpSide1();
void PlayerActor_SetupStandUpCheck();
void PlayerActor_SetupStandUpFront();
void PlayerActor_SetupStorageOpen();
void PlayerActor_SetupStorageHold();
void PlayerActor_SetupStorageClose();
void PlayerActor_SetupAct36();
void PlayerActor_SetupAct37();
void PlayerActor_SetupDoorApproach();
void PlayerActor_SetupDoorEnter();
void PlayerActor_SetupDoorEntered();
void PlayerActor_SetupDoorExit();
void PlayerActor_SetupAct3C();
void PlayerActor_SetupStowItem();
void PlayerActor_SetupStowUmbrella();
void PlayerActor_SetupLeaveRoom();
void PlayerActor_SetupAct41();
void PlayerActor_SetupAct42();
void PlayerActor_SetupAct43();
void PlayerActor_SetupAct44();
void PlayerActor_SetupUmbrellaSpin();
void PlayerActor_SetupAxeSwing();
void PlayerActor_SetupAxeFollowThrough();
void PlayerActor_SetupAct48();
void PlayerActor_SetupAxeStrike();
void PlayerActor_SetupAxeChop();
void PlayerActor_SetupAxeBreak();
void PlayerActor_SetupAxeBrokenMessage();
void PlayerActor_SetupFishCast();
void PlayerActor_SetupFishCastFail();
void PlayerActor_SetupFishWait();
void PlayerActor_SetupFishHook();
void PlayerActor_SetupFishReelIn();
void PlayerActor_SetupFishEscape();
void PlayerActor_SetupFishLand();
void PlayerActor_SetupFishShowCatch();
void PlayerActor_SetupFishStore();
void PlayerActor_SetupBugNetSwing();
void PlayerActor_SetupInsectShowCatch();
void PlayerActor_SetupInsectStore();
void PlayerActor_SetupShovelReady();
void PlayerActor_SetupShovelWait();
void PlayerActor_SetupAct5B();
void PlayerActor_SetupAct5C();
void PlayerActor_SetupShovelStrike();
void PlayerActor_SetupDig();
void PlayerActor_SetupDigUpItem();
void PlayerActor_SetupDugItemStore();
void PlayerActor_SetupBuryItem();
void PlayerActor_SetupFillHole();
void PlayerActor_SetupWateringCan();
void PlayerActor_SetupSlingshot();
void PlayerActor_SetupSlingshotWatch();
void PlayerActor_SetupAct66();
void PlayerActor_SetupAct67();
void PlayerActor_SetupTreeShake();
void PlayerActor_SetupTreeShakeRelease();
void PlayerActor_SetupMailboxOpen();
void PlayerActor_SetupMailboxWait();
void PlayerActor_SetupMailboxClose();
void PlayerActor_SetupFaint();
void PlayerActor_SetupAct6E();
void PlayerActor_SetupTrip();
void PlayerActor_SetupAct72();
void PlayerActor_SetupPitfallFall();
void PlayerActor_SetupPitfallStruggle();
void PlayerActor_SetupPitfallClimbOut();
void PlayerActor_SetupBeeSting();
void PlayerActor_SetupHaircutStart();
void PlayerActor_SetupHaircutFinish();
void PlayerActor_SetupPhonePickUp();
void PlayerActor_SetupPhoneHold();
void PlayerActor_SetupPhoneHangUp();
void PlayerActor_SetupAct80();
void PlayerActor_SetupFirework();
void PlayerActor_SetupAct82();
void PlayerActor_SetupThrowBottle();
void PlayerActor_SetupDrinkCoffee();
void PlayerActor_SetupDoorWalkIn();
void PlayerActor_SetupDoorWalkOut();
void PlayerActor_SetupExitWalkOut();
void PlayerActor_SetupExitWalkIn();
void PlayerActor_SetupFishRelease();
void PlayerActor_EndReleaseCreature();
void PlayerActor_EndLieInBed();
void PlayerActor_EndGetOutOfBed();
void PlayerActor_EndBedRoll();
void PlayerActor_EndBedApproach();
void PlayerActor_EndGetIntoBed();
void PlayerActor_EndPluck();
void PlayerActor_EndFtrGrabApproach();
void PlayerActor_EndFtrPushMove();
void PlayerActor_EndFtrPullMove();
void PlayerActor_EndSeatApproach();
void PlayerActor_EndSitDownFront();
void PlayerActor_EndSitDownSide2();
void PlayerActor_EndSitDownSide1();
void PlayerActor_EndSit();
void PlayerActor_EndStandUpSide2();
void PlayerActor_EndStandUpSide1();
void PlayerActor_EndStandUpFront();
void PlayerActor_EndAct3C();
void PlayerActor_EndStowItem();
void PlayerActor_EndUmbrellaSpin();
void PlayerActor_EndAxeStrike();
void PlayerActor_EndAxeChop();
void PlayerActor_EndAxeBreak();
void PlayerActor_EndAxeBrokenMessage();
void PlayerActor_EndFishWait();
void PlayerActor_EndInsectShowCatch();
void PlayerActor_EndInsectStore();
void PlayerActor_EndShovelStrike();
void PlayerActor_EndDig();
void PlayerActor_EndDigUpItem();
void PlayerActor_EndFillHole();
void PlayerActor_EndWateringCan();
void PlayerActor_EndAct66();
void PlayerActor_EndTreeShake();
void PlayerActor_EndPitfallFall();
void PlayerActor_EndPitfallStruggle();
void PlayerActor_EndHaircutFinish();
void PlayerActor_EndAct80();
void PlayerActor_EndFirework();
void PlayerActor_EndAct82();
}
}

namespace nA {
extern "C" {
}
}

namespace nB {
extern "C" {
PM_02005294 data_020d622c = {{(PMF)Shared_PlayerActor_MainReleaseCreature, 0}};
PM_02005294 data_020d621c = {{(PMF)nPM::PlayerActor_MainLieInBed, 0}};
PM_02005294 data_020d6214 = {{(PMF)nPM::PlayerActor_MainGetOutOfBedCheck, 0}};
PM_02005294 data_020d619c = {{(PMF)nPM::PlayerActor_MainGetOutOfBed, 0}};
PM_02005294 data_020d6204 = {{(PMF)nPM::PlayerActor_MainBedRollCheck, 0}};
PM_02005294 data_020d61fc = {{(PMF)nPM::PlayerActor_MainBedRollBlocked, 0}};
PM_02005294 data_020d61f4 = {{(PMF)nPM::PlayerActor_MainBedRoll, 0}};
PM_02005294 data_020d61c4 = {{(PMF)nPM::PlayerActor_MainBedApproach, 0}};
PM_02005294 data_020d61dc = {{(PMF)nPM::PlayerActor_MainGetIntoBed, 0}};
PM_02005294 data_020d658c = {{(PMF)nPM::PlayerActor_MainAct11, 0}};
PM_02005294 data_020d6b94 = {{(PMF)nPM::PlayerActor_MainAct12, 0}};
PM_02005294 data_020d660c = {{(PMF)nPM::PlayerActor_MainPluckReach, 0}};
PM_02005294 data_020d672c = {{(PMF)nPM::PlayerActor_MainPluck, 0}};
PM_02005294 data_020d617c = {{(PMF)nPM::PlayerActor_MainPickUpItem, 0}};
PM_02005294 data_020d6174 = {{(PMF)nPM::PlayerActor_MainFtrGrabApproach, 0}};
PM_02005294 data_020d616c = {{(PMF)nPM::PlayerActor_MainFtrHold, 0}};
PM_02005294 data_020d6164 = {{(PMF)nPM::PlayerActor_MainFtrRotate, 0}};
PM_02005294 data_020d615c = {{(PMF)Shared_FishCatch_StateStart, 0}};
PM_02005294 data_020d6154 = {{(PMF)nPM::PlayerActor_MainFtrPull, 0}};
PM_02005294 data_020d614c = {{(PMF)nPM::PlayerActor_MainFtrPushMove, 0}};
PM_02005294 data_020d6144 = {{(PMF)Shared_FishCatch_Reset, 0}};
PM_02005294 data_020d613c = {{(PMF)nPM::PlayerActor_MainSeatApproach, 0}};
PM_02005294 data_020d6134 = {{(PMF)nPM::PlayerActor_MainSitDownFront, 0}};
PM_02005294 data_020d612c = {{(PMF)nPM::PlayerActor_MainSitDownSide2, 0}};
PM_02005294 data_020d64bc = {{(PMF)nPM::PlayerActor_MainSitDownSide1, 0}};
PM_02005294 data_020d64ac = {{(PMF)nPM::PlayerActor_MainSit, 0}};
PM_02005294 data_020d6114 = {{(PMF)nPM::PlayerActor_MainStandUpSide2, 0}};
PM_02005294 data_020d5e5c = {{(PMF)Shared_FishShadow_EnterSpawn, 0}};
PM_02005294 data_020d6104 = {{(PMF)nPM::PlayerActor_MainStandUpCheck, 0}};
PM_02005294 data_020d60fc = {{(PMF)nPM::PlayerActor_MainStandUpFront, 0}};
PM_02005294 data_020d60f4 = {{(PMF)nPM::PlayerActor_MainStorageOpen, 0}};
PM_02005294 data_020d60ec = {{(PMF)nPM::PlayerActor_MainStorageHold, 0}};
PM_02005294 data_020d60e4 = {{(PMF)nPM::PlayerActor_MainStorageClose, 0}};
PM_02005294 data_020d60ac = {{(PMF)nPM::PlayerActor_MainAct36, 0}};
PM_02005294 data_020d60a4 = {{(PMF)nPM::PlayerActor_MainAct37, 0}};
PM_02005294 data_020d609c = {{(PMF)nPM::PlayerActor_MainDoorApproach, 0}};
PM_02005294 data_020d6094 = {{(PMF)nPM::PlayerActor_MainDoorEnter, 0}};
PM_02005294 data_020d5eac = {{(PMF)PlayerActor_MainDoorEntered, 0}};
PM_02005294 data_020d6084 = {{(PMF)nPM::PlayerActor_MainDoorExit, 0}};
PM_02005294 data_020d607c = {{(PMF)PlayerActor_MainAct3C, 0}};
PM_02005294 data_020d6074 = {{(PMF)nPM::PlayerActor_MainStowItem, 0}};
PM_02005294 data_020d606c = {{(PMF)nPM::PlayerActor_MainStowUmbrella, 0}};
PM_02005294 data_020d605c = {{(PMF)nPM::PlayerActor_MainLeaveRoom, 0}};
PM_02005294 data_020d6054 = {{(PMF)nPM::PlayerActor_MainAct41, 0}};
PM_02005294 data_020d604c = {{(PMF)nPM::PlayerActor_MainAct42, 0}};
PM_02005294 data_020d6044 = {{(PMF)nPM::PlayerActor_MainAct43, 0}};
PM_02005294 data_020d603c = {{(PMF)nPM::PlayerActor_MainAct44, 0}};
PM_02005294 data_020d6034 = {{(PMF)nPM::PlayerActor_MainUmbrellaSpin, 0}};
PM_02005294 data_020d602c = {{(PMF)nPM::PlayerActor_MainAxeSwing, 0}};
PM_02005294 data_020d6024 = {{(PMF)nPM::PlayerActor_MainAxeFollowThrough, 0}};
PM_02005294 data_020d601c = {{(PMF)nPM::PlayerActor_MainAct48, 0}};
PM_02005294 data_020d6014 = {{(PMF)nPM::PlayerActor_MainAxeStrike, 0}};
PM_02005294 data_020d600c = {{(PMF)nPM::PlayerActor_MainAxeChop, 0}};
PM_02005294 data_020d6284 = {{(PMF)nPM::PlayerActor_MainAxeBreak, 0}};
PM_02005294 data_020d5fc4 = {{(PMF)nPM::PlayerActor_MainAxeBrokenMessage, 0}};
PM_02005294 data_020d625c = {{(PMF)nPM::PlayerActor_MainFishCast, 0}};
PM_02005294 data_020d6254 = {{(PMF)nPM::PlayerActor_MainFishCastFail, 0}};
PM_02005294 data_020d5fe4 = {{(PMF)nPM::PlayerActor_MainFishWait, 0}};
PM_02005294 data_020d5fec = {{(PMF)nPM::PlayerActor_MainFishHook, 0}};
PM_02005294 data_020d6194 = {{(PMF)nPM::PlayerActor_MainFishReelIn, 0}};
PM_02005294 data_020d61a4 = {{(PMF)nPM::PlayerActor_MainFishEscape, 0}};
PM_02005294 data_020d61bc = {{(PMF)nPM::PlayerActor_MainFishLand, 0}};
PM_02005294 data_020d61cc = {{(PMF)nPM::PlayerActor_MainFishShowCatch, 0}};
PM_02005294 data_020d61ec = {{(PMF)nPM::PlayerActor_MainFishStore, 0}};
PM_02005294 data_020d6594 = {{(PMF)nPM::PlayerActor_MainBugNetSwing, 0}};
PM_02005294 data_020d65f4 = {{(PMF)nPM::PlayerActor_MainInsectShowCatch, 0}};
PM_02005294 data_020d668c = {{(PMF)nPM::PlayerActor_MainInsectStore, 0}};
PM_02005294 data_020d698c = {{(PMF)nPM::PlayerActor_MainShovelReady, 0}};
PM_02005294 data_020d6d9c = {{(PMF)nPM::PlayerActor_MainShovelWait, 0}};
PM_02005294 data_020d5f84 = {{(PMF)nPM::PlayerActor_MainAct5B, 0}};
PM_02005294 data_020d5f7c = {{(PMF)Shared_PlayerActor_MainAct5C, 0}};
PM_02005294 data_020d5f74 = {{(PMF)Shared_PlayerActor_MainShovelStrike, 0}};
PM_02005294 data_020d5f6c = {{(PMF)nPM::PlayerActor_MainDig, 0}};
PM_02005294 data_020d5f64 = {{(PMF)nPM::PlayerActor_MainDigUpItem, 0}};
PM_02005294 data_020d5f5c = {{(PMF)nPM::PlayerActor_MainDugItemStore, 0}};
PM_02005294 data_020d6124 = {{(PMF)nPM::PlayerActor_MainBuryItem, 0}};
PM_02005294 data_020d5e64 = {{(PMF)nPM::PlayerActor_MainFillHole, 0}};
PM_02005294 data_020d5f44 = {{(PMF)nPM::PlayerActor_MainWateringCan, 0}};
PM_02005294 data_020d5f3c = {{(PMF)nPM::PlayerActor_MainSlingshot, 0}};
PM_02005294 data_020d5f34 = {{(PMF)nPM::PlayerActor_MainSlingshotWatch, 0}};
PM_02005294 data_020d5f2c = {{(PMF)nPM::PlayerActor_MainAct66, 0}};
PM_02005294 data_020d5f24 = {{(PMF)nPM::PlayerActor_MainAct67, 0}};
PM_02005294 data_020d5f1c = {{(PMF)nPM::PlayerActor_MainTreeShake, 0}};
PM_02005294 data_020d5f14 = {{(PMF)nPM::PlayerActor_MainTreeShakeRelease, 0}};
PM_02005294 data_020d5eb4 = {{(PMF)nPM::PlayerActor_MainMailboxOpen, 0}};
PM_02005294 data_020d5f04 = {{(PMF)Shared_PlayerActor_MainMailboxWait, 0}};
PM_02005294 data_020d5efc = {{(PMF)nPM::PlayerActor_MainMailboxClose, 0}};
PM_02005294 data_020d5ef4 = {{(PMF)nPM::PlayerActor_MainFaint, 0}};
PM_02005294 data_020d5ed4 = {{(PMF)nPM::PlayerActor_MainAct6E, 0}};
PM_02005294 data_020d5f9c = {{(PMF)nPM::PlayerActor_MainTrip, 0}};
PM_02005294 data_020d5fa4 = {{(PMF)nPM::PlayerActor_MainAct72, 0}};
PM_02005294 data_020d6004 = {{(PMF)nPM::PlayerActor_MainPitfallFall, 0}};
PM_02005294 data_020d6264 = {{(PMF)nPM::PlayerActor_MainPitfallStruggle, 0}};
PM_02005294 data_020d618c = {{(PMF)nPM::PlayerActor_MainPitfallClimbOut, 0}};
PM_02005294 data_020d659c = {{(PMF)nPM::PlayerActor_MainBeeSting, 0}};
PM_02005294 data_020d6dcc = {{(PMF)Shared_FieldObj_LoadIconModels, 0}};
PM_02005294 data_020d5e84 = {{(PMF)nPM::PlayerActor_MainHaircutCut, 0}};
PM_02005294 data_020d5e7c = {{(PMF)nPM::PlayerActor_MainHaircutFinish, 0}};
PM_02005294 data_020d5e74 = {{(PMF)nPM::PlayerActor_MainPhonePickUp, 0}};
PM_02005294 data_020d64b4 = {{(PMF)nPM::PlayerActor_MainPhoneHold, 0}};
PM_02005294 data_020d5e8c = {{(PMF)nPM::PlayerActor_MainPhoneHangUp, 0}};
PM_02005294 data_020d5e94 = {{(PMF)nPM::PlayerActor_MainAct80, 0}};
PM_02005294 data_020d5ea4 = {{(PMF)nPM::PlayerActor_MainFirework, 0}};
PM_02005294 data_020d5ebc = {{(PMF)nPM::PlayerActor_MainAct82, 0}};
PM_02005294 data_020d65a4 = {{(PMF)Shared_PlayerActor_MainThrowBottle, 0}};
PM_02005294 data_020d6994 = {{(PMF)nPM::PlayerActor_MainDrinkCoffee, 0}};
PM_02005294 data_020d5e54 = {{(PMF)Shared_FieldObj_FreeCedarFileBufs, 0}};
PM_02005294 data_020d611c = {{(PMF)nPM::PlayerActor_MainDoorWalkOut, 0}};
PM_02005294 data_020d5e9c = {{(PMF)nPM::PlayerActor_MainExitWalkOut, 0}};
PM_02005294 data_020d5ec4 = {{(PMF)nPM::PlayerActor_MainExitWalkIn, 0}};
PM_02005294 data_020d5ee4 = {{(PMF)nPM::PlayerActor_MainFishRelease, 0}};
}
}

namespace nC {
extern "C" {
PM_020063a0 data_020d66ec = {{(PMF)nPM::PlayerActor_NetReleaseCreature, 0}};
PM_020063a0 data_020d66dc = {{(PMF)nPM::PlayerActor_NetLieInBed, 0}};
PM_020063a0 data_020d66d4 = {{(PMF)PlayerActor_NetGetOutOfBedCheck, 0}};
PM_020063a0 data_020d66cc = {{(PMF)nPM::PlayerActor_NetGetOutOfBed, 0}};
PM_020063a0 data_020d66c4 = {{(PMF)nPM::PlayerActor_NetBedRollCheck, 0}};
PM_020063a0 data_020d66bc = {{(PMF)nPM::PlayerActor_NetBedRollBlocked, 0}};
PM_020063a0 data_020d66b4 = {{(PMF)nPM::PlayerActor_NetBedRoll, 0}};
PM_020063a0 data_020d66ac = {{(PMF)nPM::PlayerActor_NetBedApproach, 0}};
PM_020063a0 data_020d66a4 = {{(PMF)nPM::PlayerActor_NetGetIntoBed, 0}};
PM_020063a0 data_020d6694 = {{(PMF)nPM::PlayerActor_NetAct11, 0}};
PM_020063a0 data_020d620c = {{(PMF)Shared_PlayerActor_NetAct12, 0}};
PM_020063a0 data_020d666c = {{(PMF)nPM::PlayerActor_NetPluckReach, 0}};
PM_020063a0 data_020d6664 = {{(PMF)PlayerActor_NetPluck, 0}};
PM_020063a0 data_020d663c = {{(PMF)PlayerActor_NetPickUpItem, 0}};
PM_020063a0 data_020d6634 = {{(PMF)nPM::PlayerActor_NetFtrGrabApproach, 0}};
PM_020063a0 data_020d662c = {{(PMF)nPM::PlayerActor_NetFtrHold, 0}};
PM_020063a0 data_020d6624 = {{(PMF)PlayerActor_NetFtrRotate, 0}};
PM_020063a0 data_020d661c = {{(PMF)nPM::PlayerActor_NetFtrPush, 0}};
PM_020063a0 data_020d6614 = {{(PMF)nPM::PlayerActor_NetFtrPull, 0}};
PM_020063a0 data_020d5f54 = {{(PMF)Shared_PlayerActor_NetFtrPushMove, 0}};
PM_020063a0 data_020d5e6c = {{(PMF)PlayerActor_NetFtrPullMove, 0}};
PM_020063a0 data_020d6be4 = {{(PMF)nPM::PlayerActor_NetSeatApproach, 0}};
PM_020063a0 data_020d65b4 = {{(PMF)nPM::PlayerActor_NetSitDownFront, 0}};
PM_020063a0 data_020d65cc = {{(PMF)nPM::PlayerActor_NetSitDownSide2, 0}};
PM_020063a0 data_020d65d4 = {{(PMF)nPM::PlayerActor_NetSitDownSide1, 0}};
PM_020063a0 data_020d65dc = {{(PMF)nPM::PlayerActor_NetSit, 0}};
PM_020063a0 data_020d65ec = {{(PMF)nPM::PlayerActor_NetStandUpSide2, 0}};
PM_020063a0 data_020d65fc = {{(PMF)nPM::PlayerActor_NetStandUpSide1, 0}};
PM_020063a0 data_020d670c = {{(PMF)PlayerActor_NetStandUpCheck, 0}};
PM_020063a0 data_020d6714 = {{(PMF)nPM::PlayerActor_NetStandUpFront, 0}};
PM_020063a0 data_020d674c = {{(PMF)nPM::PlayerActor_NetStorageOpen, 0}};
PM_020063a0 data_020d6754 = {{(PMF)nPM::PlayerActor_NetStorageHold, 0}};
PM_020063a0 data_020d6dbc = {{(PMF)nPM::PlayerActor_NetStorageClose, 0}};
PM_020063a0 data_020d656c = {{(PMF)nPM::PlayerActor_NetAct36, 0}};
PM_020063a0 data_020d6564 = {{(PMF)nPM::PlayerActor_NetAct37, 0}};
PM_020063a0 data_020d655c = {{(PMF)nPM::PlayerActor_NetDoorApproach, 0}};
PM_020063a0 data_020d6554 = {{(PMF)nPM::PlayerActor_NetDoorEnter, 0}};
PM_020063a0 data_020d654c = {{(PMF)nPM::PlayerActor_NetDoorEntered, 0}};
PM_020063a0 data_020d6544 = {{(PMF)nPM::PlayerActor_NetDoorExit, 0}};
PM_020063a0 data_020d653c = {{(PMF)nPM::PlayerActor_NetAct3C, 0}};
PM_020063a0 data_020d6534 = {{(PMF)nPM::PlayerActor_NetStowItem, 0}};
PM_020063a0 data_020d652c = {{(PMF)nPM::PlayerActor_NetStowUmbrella, 0}};
PM_020063a0 data_020d651c = {{(PMF)nPM::PlayerActor_NetLeaveRoom, 0}};
PM_020063a0 data_020d6514 = {{(PMF)nPM::PlayerActor_NetAct41, 0}};
PM_020063a0 data_020d650c = {{(PMF)nPM::PlayerActor_NetAct42, 0}};
PM_020063a0 data_020d6504 = {{(PMF)nPM::PlayerActor_NetAct43, 0}};
PM_020063a0 data_020d64fc = {{(PMF)Shared_FieldObjectManager_Create, 0}};
PM_020063a0 data_020d64f4 = {{(PMF)nPM::PlayerActor_NetUmbrellaSpin, 0}};
PM_020063a0 data_020d64ec = {{(PMF)nPM::PlayerActor_NetAxeSwing, 0}};
PM_020063a0 data_020d64e4 = {{(PMF)Shared_PlayerActor_NetAxeFollowThrough, 0}};
PM_020063a0 data_020d64dc = {{(PMF)nPM::PlayerActor_NetAct48, 0}};
PM_020063a0 data_020d64d4 = {{(PMF)nPM::PlayerActor_NetAxeStrike, 0}};
PM_020063a0 data_020d64cc = {{(PMF)nPM::PlayerActor_NetAxeChop, 0}};
PM_020063a0 data_020d64c4 = {{(PMF)Shared_PlayerActor_NetAxeBreak, 0}};
PM_020063a0 data_020d6bec = {{(PMF)nPM::PlayerActor_NetAxeBrokenMessage, 0}};
PM_020063a0 data_020d6bdc = {{(PMF)nPM::PlayerActor_NetFishCast, 0}};
PM_020063a0 data_020d6bd4 = {{(PMF)nPM::PlayerActor_NetFishCastFail, 0}};
PM_020063a0 data_020d64a4 = {{(PMF)Shared_PlayerActor_NetFishWait, 0}};
PM_020063a0 data_020d649c = {{(PMF)nPM::PlayerActor_NetFishHook, 0}};
PM_020063a0 data_020d6494 = {{(PMF)nPM::PlayerActor_NetFishReelIn, 0}};
PM_020063a0 data_020d5f4c = {{(PMF)nPM::PlayerActor_NetFishEscape, 0}};
PM_020063a0 data_020d6484 = {{(PMF)nPM::PlayerActor_NetFishLand, 0}};
PM_020063a0 data_020d647c = {{(PMF)nPM::PlayerActor_NetFishShowCatch, 0}};
PM_020063a0 data_020d6474 = {{(PMF)nPM::PlayerActor_NetFishStore, 0}};
PM_020063a0 data_020d646c = {{(PMF)nPM::PlayerActor_NetBugNetSwing, 0}};
PM_020063a0 data_020d6464 = {{(PMF)nPM::PlayerActor_NetInsectShowCatch, 0}};
PM_020063a0 data_020d645c = {{(PMF)nPM::PlayerActor_NetInsectStore, 0}};
PM_020063a0 data_020d6454 = {{(PMF)nPM::PlayerActor_NetShovelReady, 0}};
PM_020063a0 data_020d644c = {{(PMF)nPM::PlayerActor_NetShovelWait, 0}};
PM_020063a0 data_020d6444 = {{(PMF)nPM::PlayerActor_NetAct5B, 0}};
PM_020063a0 data_020d643c = {{(PMF)nPM::PlayerActor_NetAct5C, 0}};
PM_020063a0 data_020d6434 = {{(PMF)nPM::PlayerActor_NetShovelStrike, 0}};
PM_020063a0 data_020d642c = {{(PMF)nPM::PlayerActor_NetDig, 0}};
PM_020063a0 data_020d6424 = {{(PMF)nPM::PlayerActor_NetDigUpItem, 0}};
PM_020063a0 data_020d641c = {{(PMF)nPM::PlayerActor_NetDugItemStore, 0}};
PM_020063a0 data_020d6414 = {{(PMF)PlayerActor_NetBuryItem, 0}};
PM_020063a0 data_020d640c = {{(PMF)PlayerActor_NetFillHole, 0}};
PM_020063a0 data_020d6404 = {{(PMF)nPM::PlayerActor_NetWateringCan, 0}};
PM_020063a0 data_020d63fc = {{(PMF)nPM::PlayerActor_NetSlingshot, 0}};
PM_020063a0 data_020d63f4 = {{(PMF)PlayerActor_NetSlingshotWatch, 0}};
PM_020063a0 data_020d63ec = {{(PMF)nPM::PlayerActor_NetAct66, 0}};
PM_020063a0 data_020d63e4 = {{(PMF)nPM::PlayerActor_NetAct67, 0}};
PM_020063a0 data_020d63dc = {{(PMF)nPM::PlayerActor_NetTreeShake, 0}};
PM_020063a0 data_020d63d4 = {{(PMF)PlayerActor_NetTreeShakeRelease, 0}};
PM_020063a0 data_020d63cc = {{(PMF)nPM::PlayerActor_NetMailboxOpen, 0}};
PM_020063a0 data_020d63c4 = {{(PMF)nPM::PlayerActor_NetMailboxWait, 0}};
PM_020063a0 data_020d63bc = {{(PMF)nPM::PlayerActor_NetMailboxClose, 0}};
PM_020063a0 data_020d63b4 = {{(PMF)nPM::PlayerActor_NetFaint, 0}};
PM_020063a0 data_020d63ac = {{(PMF)nPM::PlayerActor_NetAct6E, 0}};
PM_020063a0 data_020d6394 = {{(PMF)nPM::PlayerActor_NetTrip, 0}};
PM_020063a0 data_020d5f0c = {{(PMF)nPM::PlayerActor_NetAct72, 0}};
PM_020063a0 data_020d6384 = {{(PMF)nPM::PlayerActor_NetPitfallFall, 0}};
PM_020063a0 data_020d637c = {{(PMF)nPM::PlayerActor_NetPitfallStruggle, 0}};
PM_020063a0 data_020d6374 = {{(PMF)nPM::PlayerActor_NetPitfallClimbOut, 0}};
PM_020063a0 data_020d635c = {{(PMF)PlayerActor_NetBeeSting, 0}};
PM_020063a0 data_020d634c = {{(PMF)nPM::PlayerActor_NetHaircutStart, 0}};
PM_020063a0 data_020d6344 = {{(PMF)nPM::PlayerActor_NetHaircutCut, 0}};
PM_020063a0 data_020d633c = {{(PMF)nPM::PlayerActor_NetHaircutFinish, 0}};
PM_020063a0 data_020d6334 = {{(PMF)nPM::PlayerActor_NetPhonePickUp, 0}};
PM_020063a0 data_020d632c = {{(PMF)nPM::PlayerActor_NetPhoneHold, 0}};
PM_020063a0 data_020d6324 = {{(PMF)nPM::PlayerActor_NetPhoneHangUp, 0}};
PM_020063a0 data_020d631c = {{(PMF)nPM::PlayerActor_NetAct80, 0}};
PM_020063a0 data_020d6314 = {{(PMF)nPM::PlayerActor_NetFirework, 0}};
PM_020063a0 data_020d630c = {{(PMF)Shared_PlayerActor_NetAct82, 0}};
PM_020063a0 data_020d62d4 = {{(PMF)nPM::PlayerActor_NetThrowBottle, 0}};
PM_020063a0 data_020d62cc = {{(PMF)PlayerActor_NetDrinkCoffee, 0}};
PM_020063a0 data_020d62c4 = {{(PMF)nPM::PlayerActor_NetDoorWalkIn, 0}};
PM_020063a0 data_020d62bc = {{(PMF)nPM::PlayerActor_NetDoorWalkOut, 0}};
PM_020063a0 data_020d62b4 = {{(PMF)nPM::PlayerActor_NetExitWalkOut, 0}};
PM_020063a0 data_020d62ac = {{(PMF)nPM::PlayerActor_NetExitWalkIn, 0}};
PM_020063a0 data_020d62a4 = {{(PMF)nPM::PlayerActor_NetFishRelease, 0}};
}
}

namespace nD {
extern "C" {
PM_02006d14 data_020d6b9c = {{(PMF)nPM::PlayerActor_SetupReleaseCreature, 0}};
PM_02006d14 data_020d610c = {{(PMF)nPM::PlayerActor_SetupLieInBed, 0}};
PM_02006d14 data_020d6b84 = {{(PMF)nPM::PlayerActor_SetupGetOutOfBedCheck, 0}};
PM_02006d14 data_020d6b7c = {{(PMF)nPM::PlayerActor_SetupGetOutOfBed, 0}};
PM_02006d14 data_020d6b74 = {{(PMF)nPM::PlayerActor_SetupBedRollCheck, 0}};
PM_02006d14 data_020d6b6c = {{(PMF)nPM::PlayerActor_SetupBedRollBlocked, 0}};
PM_02006d14 data_020d6b64 = {{(PMF)nPM::PlayerActor_SetupBedRoll, 0}};
PM_02006d14 data_020d6b5c = {{(PMF)nPM::PlayerActor_SetupBedApproach, 0}};
PM_02006d14 data_020d6b54 = {{(PMF)nPM::PlayerActor_SetupGetIntoBed, 0}};
PM_02006d14 data_020d6b44 = {{(PMF)nPM::PlayerActor_SetupAct11, 0}};
PM_02006d14 data_020d6b3c = {{(PMF)nPM::PlayerActor_SetupAct12, 0}};
PM_02006d14 data_020d6b1c = {{(PMF)nPM::PlayerActor_SetupPluckReach, 0}};
PM_02006d14 data_020d6b14 = {{(PMF)nPM::PlayerActor_SetupPluck, 0}};
PM_02006d14 data_020d6aec = {{(PMF)nPM::PlayerActor_SetupPickUpItem, 0}};
PM_02006d14 data_020d6ae4 = {{(PMF)nPM::PlayerActor_SetupFtrGrabApproach, 0}};
PM_02006d14 data_020d6adc = {{(PMF)nPM::PlayerActor_SetupFtrHold, 0}};
PM_02006d14 data_020d6ad4 = {{(PMF)nPM::PlayerActor_SetupFtrRotate, 0}};
PM_02006d14 data_020d6acc = {{(PMF)nPM::PlayerActor_SetupFtrPush, 0}};
PM_02006d14 data_020d6ac4 = {{(PMF)nPM::PlayerActor_SetupFtrPull, 0}};
PM_02006d14 data_020d6abc = {{(PMF)nPM::PlayerActor_SetupFtrPushMove, 0}};
PM_02006d14 data_020d6ab4 = {{(PMF)nPM::PlayerActor_SetupFtrPullMove, 0}};
PM_02006d14 data_020d6aac = {{(PMF)nPM::PlayerActor_SetupSeatApproach, 0}};
PM_02006d14 data_020d6aa4 = {{(PMF)nPM::PlayerActor_SetupSitDownFront, 0}};
PM_02006d14 data_020d6a9c = {{(PMF)nPM::PlayerActor_SetupSitDownSide2, 0}};
PM_02006d14 data_020d6a94 = {{(PMF)nPM::PlayerActor_SetupSitDownSide1, 0}};
PM_02006d14 data_020d6a8c = {{(PMF)nPM::PlayerActor_SetupSit, 0}};
PM_02006d14 data_020d6a84 = {{(PMF)nPM::PlayerActor_SetupStandUpSide2, 0}};
PM_02006d14 data_020d6a7c = {{(PMF)nPM::PlayerActor_SetupStandUpSide1, 0}};
PM_02006d14 data_020d6a74 = {{(PMF)nPM::PlayerActor_SetupStandUpCheck, 0}};
PM_02006d14 data_020d6a6c = {{(PMF)nPM::PlayerActor_SetupStandUpFront, 0}};
PM_02006d14 data_020d6a64 = {{(PMF)nPM::PlayerActor_SetupStorageOpen, 0}};
PM_02006d14 data_020d6a5c = {{(PMF)nPM::PlayerActor_SetupStorageHold, 0}};
PM_02006d14 data_020d6a54 = {{(PMF)Shared_PlayerActor_SetupStorageClose, 0}};
PM_02006d14 data_020d6a1c = {{(PMF)nPM::PlayerActor_SetupAct36, 0}};
PM_02006d14 data_020d6a14 = {{(PMF)nPM::PlayerActor_SetupAct37, 0}};
PM_02006d14 data_020d6a0c = {{(PMF)nPM::PlayerActor_SetupDoorApproach, 0}};
PM_02006d14 data_020d6a04 = {{(PMF)nPM::PlayerActor_SetupDoorEnter, 0}};
PM_02006d14 data_020d69fc = {{(PMF)nPM::PlayerActor_SetupDoorEntered, 0}};
PM_02006d14 data_020d69f4 = {{(PMF)nPM::PlayerActor_SetupDoorExit, 0}};
PM_02006d14 data_020d69ec = {{(PMF)Shared_PlayerActor_SetupAct3C, 0}};
PM_02006d14 data_020d69e4 = {{(PMF)nPM::PlayerActor_SetupStowItem, 0}};
PM_02006d14 data_020d69dc = {{(PMF)nPM::PlayerActor_SetupStowUmbrella, 0}};
PM_02006d14 data_020d69cc = {{(PMF)nPM::PlayerActor_SetupLeaveRoom, 0}};
PM_02006d14 data_020d69c4 = {{(PMF)nPM::PlayerActor_SetupAct41, 0}};
PM_02006d14 data_020d69bc = {{(PMF)nPM::PlayerActor_SetupAct42, 0}};
PM_02006d14 data_020d69b4 = {{(PMF)nPM::PlayerActor_SetupAct43, 0}};
PM_02006d14 data_020d69ac = {{(PMF)nPM::PlayerActor_SetupAct44, 0}};
PM_02006d14 data_020d69a4 = {{(PMF)nPM::PlayerActor_SetupUmbrellaSpin, 0}};
PM_02006d14 data_020d699c = {{(PMF)nPM::PlayerActor_SetupAxeSwing, 0}};
PM_02006d14 data_020d638c = {{(PMF)Shared_PlayerActor_SetupAxeFollowThrough, 0}};
PM_02006d14 data_020d608c = {{(PMF)nPM::PlayerActor_SetupAct48, 0}};
PM_02006d14 data_020d6984 = {{(PMF)nPM::PlayerActor_SetupAxeStrike, 0}};
PM_02006d14 data_020d697c = {{(PMF)nPM::PlayerActor_SetupAxeChop, 0}};
PM_02006d14 data_020d6974 = {{(PMF)nPM::PlayerActor_SetupAxeBreak, 0}};
PM_02006d14 data_020d696c = {{(PMF)nPM::PlayerActor_SetupAxeBrokenMessage, 0}};
PM_02006d14 data_020d6964 = {{(PMF)nPM::PlayerActor_SetupFishCast, 0}};
PM_02006d14 data_020d695c = {{(PMF)nPM::PlayerActor_SetupFishCastFail, 0}};
PM_02006d14 data_020d6954 = {{(PMF)nPM::PlayerActor_SetupFishWait, 0}};
PM_02006d14 data_020d694c = {{(PMF)nPM::PlayerActor_SetupFishHook, 0}};
PM_02006d14 data_020d6944 = {{(PMF)nPM::PlayerActor_SetupFishReelIn, 0}};
PM_02006d14 data_020d693c = {{(PMF)Shared_PlayerActor_SetupFishEscape, 0}};
PM_02006d14 data_020d6934 = {{(PMF)nPM::PlayerActor_SetupFishLand, 0}};
PM_02006d14 data_020d692c = {{(PMF)nPM::PlayerActor_SetupFishShowCatch, 0}};
PM_02006d14 data_020d6924 = {{(PMF)nPM::PlayerActor_SetupFishStore, 0}};
PM_02006d14 data_020d691c = {{(PMF)nPM::PlayerActor_SetupBugNetSwing, 0}};
PM_02006d14 data_020d6914 = {{(PMF)nPM::PlayerActor_SetupInsectShowCatch, 0}};
PM_02006d14 data_020d690c = {{(PMF)nPM::PlayerActor_SetupInsectStore, 0}};
PM_02006d14 data_020d6904 = {{(PMF)nPM::PlayerActor_SetupShovelReady, 0}};
PM_02006d14 data_020d68fc = {{(PMF)nPM::PlayerActor_SetupShovelWait, 0}};
PM_02006d14 data_020d68f4 = {{(PMF)nPM::PlayerActor_SetupAct5B, 0}};
PM_02006d14 data_020d68ec = {{(PMF)nPM::PlayerActor_SetupAct5C, 0}};
PM_02006d14 data_020d68e4 = {{(PMF)nPM::PlayerActor_SetupShovelStrike, 0}};
PM_02006d14 data_020d68dc = {{(PMF)nPM::PlayerActor_SetupDig, 0}};
PM_02006d14 data_020d68d4 = {{(PMF)nPM::PlayerActor_SetupDigUpItem, 0}};
PM_02006d14 data_020d68cc = {{(PMF)nPM::PlayerActor_SetupDugItemStore, 0}};
PM_02006d14 data_020d68c4 = {{(PMF)nPM::PlayerActor_SetupBuryItem, 0}};
PM_02006d14 data_020d68bc = {{(PMF)nPM::PlayerActor_SetupFillHole, 0}};
PM_02006d14 data_020d68b4 = {{(PMF)nPM::PlayerActor_SetupWateringCan, 0}};
PM_02006d14 data_020d68ac = {{(PMF)nPM::PlayerActor_SetupSlingshot, 0}};
PM_02006d14 data_020d68a4 = {{(PMF)nPM::PlayerActor_SetupSlingshotWatch, 0}};
PM_02006d14 data_020d689c = {{(PMF)nPM::PlayerActor_SetupAct66, 0}};
PM_02006d14 data_020d6894 = {{(PMF)nPM::PlayerActor_SetupAct67, 0}};
PM_02006d14 data_020d688c = {{(PMF)nPM::PlayerActor_SetupTreeShake, 0}};
PM_02006d14 data_020d6884 = {{(PMF)Shared_PlayerActor_SetupTreeShakeRelease, 0}};
PM_02006d14 data_020d687c = {{(PMF)nPM::PlayerActor_SetupMailboxOpen, 0}};
PM_02006d14 data_020d6874 = {{(PMF)nPM::PlayerActor_SetupMailboxWait, 0}};
PM_02006d14 data_020d686c = {{(PMF)nPM::PlayerActor_SetupMailboxClose, 0}};
PM_02006d14 data_020d6864 = {{(PMF)nPM::PlayerActor_SetupFaint, 0}};
PM_02006d14 data_020d685c = {{(PMF)nPM::PlayerActor_SetupAct6E, 0}};
PM_02006d14 data_020d6844 = {{(PMF)nPM::PlayerActor_SetupTrip, 0}};
PM_02006d14 data_020d683c = {{(PMF)nPM::PlayerActor_SetupAct72, 0}};
PM_02006d14 data_020d6834 = {{(PMF)nPM::PlayerActor_SetupPitfallFall, 0}};
PM_02006d14 data_020d682c = {{(PMF)nPM::PlayerActor_SetupPitfallStruggle, 0}};
PM_02006d14 data_020d6824 = {{(PMF)nPM::PlayerActor_SetupPitfallClimbOut, 0}};
PM_02006d14 data_020d680c = {{(PMF)nPM::PlayerActor_SetupBeeSting, 0}};
PM_02006d14 data_020d67fc = {{(PMF)nPM::PlayerActor_SetupHaircutStart, 0}};
PM_02006d14 data_020d67f4 = {{(PMF)Shared_FieldObj_LoadStones, 0}};
PM_02006d14 data_020d67ec = {{(PMF)nPM::PlayerActor_SetupHaircutFinish, 0}};
PM_02006d14 data_020d67e4 = {{(PMF)nPM::PlayerActor_SetupPhonePickUp, 0}};
PM_02006d14 data_020d67dc = {{(PMF)nPM::PlayerActor_SetupPhoneHold, 0}};
PM_02006d14 data_020d67d4 = {{(PMF)nPM::PlayerActor_SetupPhoneHangUp, 0}};
PM_02006d14 data_020d67cc = {{(PMF)nPM::PlayerActor_SetupAct80, 0}};
PM_02006d14 data_020d67c4 = {{(PMF)Shared_PlayerActor_SetupFirework, 0}};
PM_02006d14 data_020d67bc = {{(PMF)nPM::PlayerActor_SetupAct82, 0}};
PM_02006d14 data_020d6784 = {{(PMF)nPM::PlayerActor_SetupThrowBottle, 0}};
PM_02006d14 data_020d677c = {{(PMF)nPM::PlayerActor_SetupDrinkCoffee, 0}};
PM_02006d14 data_020d6774 = {{(PMF)nPM::PlayerActor_SetupDoorWalkIn, 0}};
PM_02006d14 data_020d627c = {{(PMF)nPM::PlayerActor_SetupDoorWalkOut, 0}};
PM_02006d14 data_020d6764 = {{(PMF)nPM::PlayerActor_SetupExitWalkOut, 0}};
PM_02006d14 data_020d675c = {{(PMF)nPM::PlayerActor_SetupExitWalkIn, 0}};
PM_02006d14 data_020d626c = {{(PMF)nPM::PlayerActor_SetupFishRelease, 0}};
}
}

namespace nE {
extern "C" {
PM_020076f0 data_020d6d74 = {{(PMF)nPM::PlayerActor_EndReleaseCreature, 0}};
PM_020076f0 data_020d6d64 = {{(PMF)nPM::PlayerActor_EndLieInBed, 0}};
PM_020076f0 data_020d6d5c = {{(PMF)nPM::PlayerActor_EndGetOutOfBed, 0}};
PM_020076f0 data_020d6d54 = {{(PMF)nPM::PlayerActor_EndBedRoll, 0}};
PM_020076f0 data_020d6d4c = {{(PMF)nPM::PlayerActor_EndBedApproach, 0}};
PM_020076f0 data_020d6d44 = {{(PMF)nPM::PlayerActor_EndGetIntoBed, 0}};
PM_020076f0 data_020d6d34 = {{(PMF)nPM::PlayerActor_EndPluck, 0}};
PM_020076f0 data_020d6d1c = {{(PMF)nPM::PlayerActor_EndFtrGrabApproach, 0}};
PM_020076f0 data_020d6d14 = {{(PMF)nPM::PlayerActor_EndFtrPushMove, 0}};
PM_020076f0 data_020d6d0c = {{(PMF)nPM::PlayerActor_EndFtrPullMove, 0}};
PM_020076f0 data_020d6d04 = {{(PMF)nPM::PlayerActor_EndSeatApproach, 0}};
PM_020076f0 data_020d6cfc = {{(PMF)nPM::PlayerActor_EndSitDownFront, 0}};
PM_020076f0 data_020d6cf4 = {{(PMF)nPM::PlayerActor_EndSitDownSide2, 0}};
PM_020076f0 data_020d6cec = {{(PMF)nPM::PlayerActor_EndSitDownSide1, 0}};
PM_020076f0 data_020d6ce4 = {{(PMF)nPM::PlayerActor_EndSit, 0}};
PM_020076f0 data_020d6cdc = {{(PMF)nPM::PlayerActor_EndStandUpSide2, 0}};
PM_020076f0 data_020d6cd4 = {{(PMF)nPM::PlayerActor_EndStandUpSide1, 0}};
PM_020076f0 data_020d6ccc = {{(PMF)nPM::PlayerActor_EndStandUpFront, 0}};
PM_020076f0 data_020d6cc4 = {{(PMF)Shared_PlayerActor_EndAct3C, 0}};
PM_020076f0 data_020d6cbc = {{(PMF)nPM::PlayerActor_EndStowItem, 0}};
PM_020076f0 data_020d6cac = {{(PMF)Shared_PlayerActor_EndUmbrellaSpin, 0}};
PM_020076f0 data_020d6ca4 = {{(PMF)nPM::PlayerActor_EndAxeStrike, 0}};
PM_020076f0 data_020d6c9c = {{(PMF)nPM::PlayerActor_EndAxeChop, 0}};
PM_020076f0 data_020d6c94 = {{(PMF)nPM::PlayerActor_EndAxeBreak, 0}};
PM_020076f0 data_020d6c8c = {{(PMF)nPM::PlayerActor_EndAxeBrokenMessage, 0}};
PM_020076f0 data_020d6c84 = {{(PMF)Shared_PlayerActor_EndFishWait, 0}};
PM_020076f0 data_020d6c7c = {{(PMF)nPM::PlayerActor_EndInsectShowCatch, 0}};
PM_020076f0 data_020d6c74 = {{(PMF)nPM::PlayerActor_EndInsectStore, 0}};
PM_020076f0 data_020d6c6c = {{(PMF)Shared_PlayerActor_EndShovelStrike, 0}};
PM_020076f0 data_020d6c64 = {{(PMF)nPM::PlayerActor_EndDig, 0}};
PM_020076f0 data_020d6c5c = {{(PMF)nPM::PlayerActor_EndDigUpItem, 0}};
PM_020076f0 data_020d6c54 = {{(PMF)Shared_PlayerActor_EndFillHole, 0}};
PM_020076f0 data_020d6c4c = {{(PMF)nPM::PlayerActor_EndWateringCan, 0}};
PM_020076f0 data_020d6c44 = {{(PMF)nPM::PlayerActor_EndAct66, 0}};
PM_020076f0 data_020d6c3c = {{(PMF)Shared_PlayerActor_EndTreeShake, 0}};
PM_020076f0 data_020d6c34 = {{(PMF)nPM::PlayerActor_EndPitfallFall, 0}};
PM_020076f0 data_020d6c2c = {{(PMF)nPM::PlayerActor_EndPitfallStruggle, 0}};
PM_020076f0 data_020d6c24 = {{(PMF)nPM::PlayerActor_EndHaircutFinish, 0}};
PM_020076f0 data_020d6c1c = {{(PMF)nPM::PlayerActor_EndAct80, 0}};
PM_020076f0 data_020d6c14 = {{(PMF)nPM::PlayerActor_EndFirework, 0}};
PM_020076f0 data_020d6c0c = {{(PMF)nPM::PlayerActor_EndAct82, 0}};
}
}

namespace nT {
extern "C" void *PlayerActor_Create() {
    return new PlayerActor();
}
}

namespace nT {
extern "C" void PlayerActor_CheckSceneExit(s32 a) {
    s16 sp0;
    s32 sp4;
    u32 sp8;
    VecFx32 *pos;
    if (((CommManager *)(gCommManager))->isLocalSlot(a) && !PlayerActor_TestSlotFlag(0xb, a) && !PlayerActor_TestSlotFlag(0x13, a) &&
        !PlayerActor_TestSlotFlag(5, a) && (pos = (VecFx32 *)PlayerActor_GetBodyPos(4)) != NULL) {
        sp4 = Ground_GetExitAtPos(pos);
        if (sp4 != -1) {
            PlayerActor_SetLocalExitId(&sp4);
            if (SceneExit_GetDoor(Scene_GetWarpRequest(), sp4, &sp8, &sp0)) {
                PlayerActor_SetLocalExitKind(&sp8);
                if (sp8 == 1 || sp8 == 2 || sp8 == 4 || RoomEntry_IsExclusiveExit(sp4)) {
                    if (Scene_InUnk6To8()) {
                        TalkRequest_FinishSceneEntry();
                    }
                    TalkRequest_AddLeaveRoom();
                } else {
                    TalkRequest_AddSceneExit((Character *)PlayerActor_GetCharacter(4), sp4);
                }
            }
        }
    }
}
}

namespace nT {
extern "C" void PlayerActor_ApproachValue(s32 *p, s32 target, s32 rate, s32 maxstep, s32 minstep) {
    s32 cur = *p;
    if (cur == target) return;
    s32 d = target - cur;
    s32 ad = d < 0 ? -d : d;
    if (ad < minstep) {
        *p = target;
        return;
    }
    s32 v = func_01ffcb0c(d, rate);
    s32 av = v < 0 ? -v : v;
    if (av > maxstep) {
        if (v >= 0) *p += maxstep;
        else *p -= maxstep;
    } else if (av < minstep) {
        if (v >= 0) *p += minstep;
        else *p -= minstep;
    } else {
        *p += v;
    }
}
}

namespace nT {
extern "C" void PlayerActor_ApproachCoord(s32 *p, s32 target) {
    PlayerActor_ApproachValue(p, target, 0xe66, 0x1ec, 0x31);
}
}

namespace nT {
extern "C" s32 PlayerActor_ApproachAngle(s16 *p, s32 target, s32 rate, s32 maxstep, s32 minstep) {
    s32 cur = *p;
    s32 d, t, c;
    if (cur == target) {
        return 0;
    }
    c = cur << 12;
    t = target << 12;
    d = t - c;
    if (d < 0) d = -d;
    s32 step;
    if (d > 0x8000000) {
        step = func_01ffcb0c(rate, 0x10000000 - d);
    } else {
        step = func_01ffcb0c(rate, d);
    }
    if (step <= minstep) {
        *p = target;
        return 0;
    }
    if (step > maxstep) step = maxstep;
    s32 v;
    if (t >= c) {
        if (d > 0x8000000) v = c - step;
        else v = c + step;
    } else {
        if (d > 0x8000000) v = c + step;
        else v = c - step;
    }
    *p = v >> 12;
    return (s16)(target - *p);
}
}

namespace nT {
extern "C" s32 PlayerActor_TurnAngle(s16 *p, s32 target) {
    return PlayerActor_ApproachAngle(p, target, 0x800, 0x1770000, 0xc0000);
}
}

namespace nT {
extern "C" s32 PlayerActor_TurnAngleSlow(s16 *p, s32 target) {
    return PlayerActor_ApproachAngle(p, target, 0x666, 0xbb8000, 0xc0000);
}
}

namespace nT {
extern "C" s32 PlayerActor_Accelerate(s32 a, s32 b) {
    s32 v = a + 0x93;
    if (v > b) v = b;
    return v;
}
}

namespace nT {
extern "C" s32 PlayerActor_DecreaseClamped(s32 a, s32 b, s32 c) {
    s32 v = a - c;
    if (v < b) v = b;
    return v;
}
}

namespace nT {
extern "C" s32 PlayerActor_Decelerate(s32 a, s32 b) {
    return PlayerActor_DecreaseClamped(a, b, 0x9c);
}
}

namespace nT {
extern "C" s32 PlayerActor_DecelerateSkid(s32 a, s32 b) {
    return PlayerActor_DecreaseClamped(a, b, 0x8f);
}
}

namespace nT {
extern "C" PlayerData *PlayerActor_GetPlayerData(PlayerActor *p) {
    s32 i = PlayerSession_GetDataIndex(p->sessionSlot);
    if (i < 7) {
        return PlayerData_Get(i);
    }
    return NULL;
}
}

namespace nT {
extern "C" BOOL PlayerActor_GetGender(PlayerActor *p) {
    void *r = PlayerActor_GetPlayerData(p);
    BOOL result = FALSE;
    if (r != NULL) {
        void *id = ((PlayerData *)(r))->getPlayerId();
        if (((PlayerId *)(id))->getGender()) {
            result = TRUE;
        } else {
            result = FALSE;
        }
    }
    return result;
}
}

namespace nT {
extern "C" void PlayerActor_GetShirt(u16 *out, PlayerActor *p) {
    void *r = PlayerActor_GetPlayerData(p);
    *out = 0xfff1;
    if (r != NULL) {
        *out = *((PlayerData *)(r))->getShirt();
    }
}
}

namespace nT {
extern "C" void PlayerActor_GetHat(u16 *out, PlayerActor *p) {
    void *r = PlayerActor_GetPlayerData(p);
    *out = 0xfff1;
    if (r != NULL) {
        *out = *((PlayerData *)(r))->getHat();
    }
}
}

namespace nT {
extern "C" s32 PlayerActor_GetHairStyle(PlayerActor *p) {
    return ((PlayerData *)PlayerActor_GetPlayerData(p))->getHairStyle();
}
}

namespace nT {
extern "C" s32 PlayerActor_GetHairColor(PlayerActor *p) {
    return ((PlayerData *)PlayerActor_GetPlayerData(p))->getHairColor();
}
}

namespace nT {
extern "C" s32 PlayerActor_GetTan(PlayerActor *p) {
    return ((PlayerData *)PlayerActor_GetPlayerData(p))->getTan();
}
}

u32 PlayerActor::calcTan(u32 r7) {
    using namespace nS;
    u32 r5 = PlayerActor_GetTan(this);
    BOOL flag = FALSE;
    u32 k = Scene_GetCurrent();
    if (k == 0x2e || k == 0xd || k == 0xe || k == 0xc || k == 0x2f) {
        return r5;
    }
    if (!(Scene_InUnk6To8() && PlayerActor_ParamGetAction(param) == 8)) {
        if (!((CommManager *)(void *)(gCommManager))->isLocalSlot(sessionSlot)) {
            return r5;
        }
        if (r7 == 0) {
            return r5;
        }
    } else if (r7 == 1) {
        r7 = 0;
    }
    if (isGuestInSession()) {
        flag = TRUE;
    }
    PlayerData *obj = (PlayerData *)PlayerActor_GetPlayerData(this);
    Unk_02010b08_Time a, b;
    Unk_0200f17c_Date bits;
    a.unk_00 = 0;
    a.unk_04 = 0;
    b.unk_00 = 0;
    b.unk_04 = 0;
    s32 r3 = PlayerActor_CompareLastPlayDate(this, &a, (u8 *)&b);
    bits.a = *((u8 *)&a + 5);
    bits.b = *((u8 *)&a + 4);
    bits.c = *((u8 *)&a + 3);
    switch (r3) {
    case -1:
        if (r7 != 0) {
            PlayerActor_SetLastPlayDate(this, obj, &bits);
        }
        break;
    case 0:
        break;
    default: {
        s32 n = DateTime_DiffDays(&b, &a);
        u8 *p = PlayerSession_GetSessionFlags();
        if (n >= 2) {
            s32 h = n >> 1;
            if ((s32)r5 > h) {
                r5 = (u8)(r5 - h);
            } else {
                r5 = 0;
            }
            if (r7 != 0) {
                obj->setTan(r5);
                PlayerActor_SetLastPlayDate(this, obj, &bits);
                *p &= 0xfd;
                if (!flag) {
                    *p &= ~0x10;
                }
                netSendTan();
            }
        }
        *p &= 0xfe;
        break;
    }
    }
    u16 *q = PlayerSession_GetTanTimer();
    if (*q == 0) {
        *q = 0x4650;
    }
    return r5;
}

namespace nS {
extern "C" void PlayerActor_GetFaceItem(u16 *out, PlayerActor *x) {
    PlayerData *pd = (PlayerData *)PlayerActor_GetPlayerData(x);
    *out = *pd->getFaceItem();
}
}

namespace nS {
extern "C" u32 PlayerActor_GetFaceAltFlag(PlayerActor *x) {
    void *a = PlayerActor_GetPlayerData(x);
    if (a) {
        return PlayerData_HasStungFace(a);
    }
    return 0;
}
}

namespace nS {
extern "C" u32 PlayerActor_GetFaceTexIndex(PlayerActor *x) {
    PlayerData *a = (PlayerData *)PlayerActor_GetPlayerData(x);
    s32 r = 0x20;
    if (a) {
        r = a->getFaceType();
        if (PlayerData_HasStungFace(a)) {
            r += 0x10;
        }
    }
    if (r >= 0x20) {
        r = 0;
    }
    return r;
}
}

namespace nS {
extern "C" void PlayerActor_GetHeldItem(u16 *out, PlayerActor *x) {
    *out = 0xfff1;
    PlayerData *pd;
    if ((pd = (PlayerData *)PlayerActor_GetPlayerData(x)) != 0) {
        *out = *pd->getHeldItem();
    }
}
}

u32 PlayerActor::getAnimResIndex(u32 *a) {
    using namespace nS;
    return sPlayerAnimResIndex[*a];
}

void PlayerActor::setAngleY(s16 *a) {
    using namespace nS;
    Unk_02010a58_Blk *p = (Unk_02010a58_Blk *)(u8 *)&rotX;
    s16 v = *a;
    p->rotY = v;
    moveAngleY = p->rotY;
}

void PlayerActor::setRotX(u16 a) {
    using namespace nS;
    rotX = a;
}

void PlayerActor::approachRotX(s32 target) {
    using namespace nS;
    PlayerActor_TurnAngle(&rotX, target);
}

void PlayerActor::setSpeed(u32 *a) {
    using namespace nS;
    speed = *a;
    ((Actor *)(void *)(this))->calcVelocity();
}

void PlayerActor::moveWithCollision() {
    using namespace nS;
    ((Actor *)(void *)(this))->applyVelocity((ActorCollider *)((u8 *)&bodyCollider));
    CollisionState tmp;
    s16 h = rotY;
    CollisionState *q = ((Unk_02007694 *)(this))->keepsBgCheckWork(action) == 0 ? &tmp : &bgCheckWork;
    Collision_Move(q, &position, &prevPosition, h, 0xfd7, this, 0xf);
    position.y = Ground_GetDefaultY();
}

void PlayerActor::moveNoCollision() {
    using namespace nS;
    ((Actor *)(void *)(this))->applyVelocity((ActorCollider *)(0));
    position.y = Ground_GetDefaultY();
}

BOOL PlayerActor::netApproachTransform() {
    using namespace nS;
    PlayerNetApproachLocals m;
    u32 x, y;
    BOOL r;
    if (!getRemoteTransform(&m.scene, (s32 *)&x, (s32 *)&y, &m.netAngle)) {
        return FALSE;
    }
    if (m.scene != Scene_GetCurrent()) {
        return FALSE;
    }
    m.curAngle = rotY;
    r = PlayerActor_TurnAngleSlow(&m.curAngle, m.netAngle) == 0 ? TRUE : FALSE;
    setAngleY(&m.curAngle);
    PlayerActor_ApproachCoord(&position.x, x);
    PlayerActor_ApproachCoord(&position.z, y);
    if (r) {
        if (x == position.x && y == position.z) {
            r = TRUE;
        } else {
            r = FALSE;
        }
    }
    return r;
}

void PlayerActor::advanceAnim() {
    using namespace nS;
    ((TwoLayerAnimModel *)((u8 *)&bodyModel))->updateLayers();
}

void PlayerActor::updateFaceAnims() {
    using namespace nS;
    updateEyeAnim();
    updateMouthAnim();
}

void PlayerActor::updateEyeAnim() {
    using namespace nS;
    s32 t = eyeAnimId;
    if (t != 0x16f) {
        if (t == 0) {
            if (((AnimFrameCtrl *)((u8 *)&eyeTexAnim))->hasPassedFrame(0)) {
                if (!BlinkTimer_Update((u8 *)&blinkTimer)) {
                    return;
                }
            }
            ((MatTexPatAnim *)((u8 *)&eyeTexAnim))->update();
        } else {
            ((MatTexPatAnim *)((u8 *)&eyeTexAnim))->update();
        }
    }
}

void PlayerActor::updateMouthAnim() {
    using namespace nS;
    if (mouthAnimId != 0x16f) {
        ((MatTexPatAnim *)((u8 *)&mouthTexAnim))->update();
    }
}

u32 PlayerActor::getBodyColliderFlags(u32 *a) {
    using namespace nS;
    if (sPlayerActionColliderFlag2[*a] != 0) {
        return 2;
    }
    return 0;
}

void PlayerActor::setBodyColliderAt(Unk_020107c8_Blk *a, u32 *b) {
    using namespace nS;
    u32 f = getBodyColliderFlags(b);
    bodyCollider.setupForActorAt(this, (VecFx32 *)a, 0xfd7, 0x2800, f | 4, 0x2fc, 0x15, (u8)sessionSlot, 0x1000);  // a: s32 x, y, z
}

void PlayerActor::setBodyCollider(u32 *a) {
    using namespace nS;
    setBodyColliderAt((Unk_020107c8_Blk *)(u8 *)&position, a);
}

void PlayerActor::setBodyColliderAtDrawPos(u32 *a) {
    using namespace nS;
    Unk_020107c8_Blk b;
    b.x = bodyPos.x;
    b.y = Ground_GetDefaultY();
    b.z = bodyPos.z;
    setBodyColliderAt(&b, a);
}

void PlayerActor::setSubColliderBody(u32 *a) {
    using namespace nS;
    subCollider.setupForActorAt(this, (VecFx32 *)a, 0xfd7, 0x2800, 6, 0x2fc, 0, 0xff, 0x1000);  // (a is a position)
}

void PlayerActor::setSubCollider(VecFx32 *pos, u32 b, u32 c) {
    using namespace nS;
    ((ActorPlacedCollider *)((u8 *)&subCollider))->setupForActorAt(this, pos, b, c, 0x11, 0x2c, 0, 0xff, 0x1000);
}

void PlayerActor::updateBodyCollider() {
    using namespace nS;
    setBodyCollider((u32 *)(u8 *)&action);
    ((ActorCollider *)((u8 *)&bodyCollider))->submit();
}

void PlayerActor::updateCollidersAtDrawPos(u32 *a) {
    using namespace nS;
    setBodyColliderAtDrawPos((u32 *)(u8 *)&action);
    ((ActorCollider *)((u8 *)&bodyCollider))->submit();
    setSubColliderBody(a);
    ((ActorCollider *)((u8 *)&subCollider))->submit();
}

void PlayerActor::submitSceneCollider() {
    using namespace nS;
    if (((CommManager *)(gCommManager))->isLocalSlot(sessionSlot) || (Scene_InUnk6To8() && action == 8)) {
        VecFx32 *p = (VecFx32 *)(u8 *)&position;
        VecFx32 v;
        v.x = p->x;
        v.y = p->y;
        v.z = p->z;
        v.y += 0xb33;
        TouchPicker *r = Scene_GetTouchPicker();
        r->addCylinder((TouchPickCylinder *)((u8 *)&touchCylinder), (VecFx32 *)(&v), 0xa00, 0x15cf, 1, (u8)sessionSlot);
    }
}

void PlayerActor::initFaceAnims() {
    using namespace nS;
    void *t = headModel0.resMdl;
    void *a = faceTex.getBuffer();
    void *b = faceAnimWork.getHeap();
    eyeTexAnim.init(t, a, 1, b);
    t = headModel0.resMdl;
    a = faceTex.getBuffer();
    b = faceAnimWork.getHeap();
    mouthTexAnim.init(t, a, 1, b);
}

void PlayerActor::setEyeAnimForBody(s32 *a, u8 *b) {
    using namespace nS;
    s32 t = eyeAnimId;
    s32 v = CharaAnim_GetEyeAnim(*a);
    if (v == 0x16f && t != 0) {
        v = 0;
    }
    if (v != 0x16f) {
        setEyeAnim(&v, b);
    }
}

void PlayerActor::setMouthAnimForBody(s32 *a, u8 *b) {
    using namespace nS;
    s32 t = mouthAnimId;
    s32 v = CharaAnim_GetMouthAnim(*a);
    if (v == 0x16f && t != 0xba) {
        v = 0xba;
    }
    if (v != 0x16f) {
        setMouthAnim(&v, b);
    }
}

void PlayerActor::setEyeAnim(s32 *a, u8 *b) {
    using namespace nS;
    ((CharaFaceAnimRef *)((u8 *)&faceAnimRef))->loadAnim(*a, 1, 0);
    void *r = ((CharaFaceAnimRef *)(void *)((u8 *)&faceAnimRef))->getEyeAnimBuffer();
    eyeTexAnim.setAnim(r, 0, 0, *b, 0x1000);
    ((MatTexPatAnim *)((u8 *)&eyeTexAnim))->update();
    eyeAnimId = *a;
}

void PlayerActor::setMouthAnim(s32 *a, u8 *b) {
    using namespace nS;
    ((CharaFaceAnimRef *)((u8 *)&faceAnimRef))->loadAnim(*a, 1, 0);
    void *r = ((CharaFaceAnimRef *)(void *)((u8 *)&faceAnimRef))->getMouthAnimBuffer();
    mouthTexAnim.setAnim(r, 0, 0, *b, 0x1000);
    ((MatTexPatAnim *)((u8 *)&mouthTexAnim))->update();
    mouthAnimId = *a;
}

void PlayerActor::playAnim(s32 a, u32 b, u8 c, s32 d, u32 e, u16 f, s32 g) {
    using namespace nS;
    s32 t = animId;
    s32 v;
    if (t == 0xa1) {
        b = 0;
        f = 0;
    }
    v = g >= 0x137 ? getAnimResIndex((u32 *)&a) : g;
    if (!(a == 0 && t == 0 && *(u16 *)&e == 0 && b != 0 && g >= 0x137)) {
        AnimSlotRef_Load(&bodyAnimSlot, v, 1, 0);
        void *r = func_021065f8(func_021065dc(AnimSlotRef_GetData(&bodyAnimSlot)), 0);
        BlendAnimModel_Play((u8 *)&bodyModel, r, b, c, d, *(u16 *)&e, 0);
        animId = a;
        animMode = c;
    }
    applyHeldItemPose(v, f);
    setEyeAnimForBody(&v, &c);
    (this)->setMouthAnimForBody(&v, &c);
}

void PlayerActor::startAnim(s32 a, u32 b, u16 c) {
    using namespace nS;
    playAnim(a, b, 0, 0x1000, 0, c, 0x137);
}

void PlayerActor::switchAnim(s32 a, u32 b, u16 c) {
    using namespace nS;
    playAnim(a, b, 0, bodyModel.frameStep, ((u32)bodyModel.curFrame << 4 >> 16), c, 0x137);
}

void PlayerActor::startAnimOnce(s32 a, u32 b, u16 c) {
    using namespace nS;
    playAnim(a, b, 1, 0x1000, 0, c, 0x137);
}

void PlayerActor::replayAnim() {
    using namespace nS;
    u32 b, a;
    a = (u32)(eyeTexAnim.curFrame >> 12) << 16;
    b = (u32)(mouthTexAnim.curFrame >> 12) << 16;
    playAnim(animId, 3, bodyModel.playMode, bodyModel.frameStep, ((u32)bodyModel.curFrame << 4 >> 16), 0, 0x137);
    eyeTexAnim.curFrame = a >> 4;
    mouthTexAnim.curFrame = b >> 4;
}

void PlayerActor::applyHeldItemPose(s32 b, u32 c) {
    using namespace nR;
    u16 v[3];
    v[0] = 0xfff1;
    if (!(Unk_020102a0_IsZero(gFieldSceneKind) && ((PlayerActor *)(this))->testActionFlag(0))) {
        v[0] = 0xfff1;
    } else {
        PlayerActor_GetHeldItem(&v[1], ((PlayerActor *)this));
        v[0] = v[1];
    }
    v[2] = v[0];
    applyHoldPose(&v[2], b, c);
}

void PlayerActor::applyHoldPose(u16 *p, s32 b, u32 c) {
    using namespace nR;
    s32 r6 = HeldItem_GetHandPose(p);
    s32 r7 = CharaAnim_GetHoldPoseMode(b);
    if (r7 == 3 || r6 == 0x144) goto B;
    if (r7 == 1) {
        if (!Unk_02010154_In(p)) {
            if (*p < 0x13a0 || *p > 0x13a7) goto B;
        }
    }
    {
        s32 n, cnt, item;
        s32 i;
        AnimSlotRef_Load(&((PlayerActor *)this)->holdAnimSlot, r6, 1, 0);
        ((PlayerActor *)this)->bodyModel.playLayer2(func_021065f8(func_021065dc(AnimSlotRef_GetData(&((PlayerActor *)this)->holdAnimSlot)), 0), c, 0, 0x1000, 0, 0, 0);
        n = CharaAnim_GetJointGroup(r6);
        if (n < 4) {
            cnt = JointGroup_GetRangeCount(n);
            for (i = 0; (u32)i < (u32)cnt; i++) {
                item = JointGroup_GetRangeFirst(n, i);
                ((TwoLayerAnimModel *)(&((PlayerActor *)this)->bodyModel))->assignJointsToLayer2(item, JointGroup_GetRangeLast(n, i));
            }
            if (r7 == 2) {
                i = JointGroup_GetRangeFirst(1, 0);
                ((TwoLayerAnimModel *)(&((PlayerActor *)this)->bodyModel))->releaseJointsFromLayer2(i, JointGroup_GetRangeLast(1, 0));
            }
        } else {
            bodyModel.playLayer2FromBase(c, 0);
        }
    }
    goto end;
B:
    bodyModel.playLayer2FromBase(c, 0);
end:
    ((PlayerActor *)this)->handPose = r6;
}

s32 PlayerActor::getTargetWalkSpeed() {
    using namespace nR;
    s32 r4 = func_01ffcb0c(((PlayerActor *)(this))->getInputMagnitude(), 0x6e2);
    if (r4 > 0x6e2) {
        r4 = 0x6e2;
    } else if (r4 < 0x108) {
        r4 = 0;
    }
    if (((ActorCollider *)(&((PlayerActor *)this)->bodyCollider))->isHitByGroup(8) && ((ActorCollider *)(&((PlayerActor *)this)->bodyCollider))->isPushedFromAngle(((PlayerActor *)this)->rotY)) {
        if (r4 > 0x245) r4 = 0x245;
    } else if (((PlayerActor *)this)->bgCheckWork.flags & 8) {
        if (r4 > 0x307) r4 = 0x307;
    }
    return r4;
}

void PlayerActor::applySkinHairPalette(s32 a, s32 b) {
    using namespace nR;
    s32 r6 = PlayerActor_GetFaceTexIndex(this);
    s32 r1, r4;
    if (PlayerActor_GetFaceAltFlag(this)) {
        r1 = b + ((r6 - 0x10) << 3);
    } else {
        r1 = b + (r6 << 3);
    }
    r4 = b + ((a << 3) + 0x80);
    if (r1 < 0x80) {
        PlayerPaletteRef_LoadSkin(&((PlayerActor *)this)->skinHairPalette.slot, r1);
    }
    if (r4 >= 0x80 && r4 < 0xc0) {
        PlayerPaletteRef_LoadHair(&((PlayerActor *)this)->skinHairPalette.slot, r4);
    }
}

void PlayerActor::setShirtTexture(void *p) {
    using namespace nR;
    shirtTex.loadItem((u16 *)p, PlayerActor_GetPlayerData(this), 0, 0);
}

BOOL PlayerActor::requestShirtTexUpload() {
    using namespace nR;
    void *r4 = ((PlayerActor *)this)->bodyModel.resMdl;
    return ((PlayerActor *)this)->shirtTexUpload.request(r4, (const char *)sPlayerShirtTexName,
                                         ClothTex_GetTexThunk(CharaClothTexRef_GetBuffer(&((PlayerActor *)this)->shirtTex.v)), 0, 0);
}

s32 PlayerActor::tryInteract() {
    using namespace nR;
    if (TalkRequestFlags_IsResetti() || ((PlayerActor *)(this))->testActionFlag(0x13)) return FALSE;
    if (((PlayerActor *)(this))->isInteractPressed()) {
        TalkRequest_AddTalk(((PlayerActor *)this), ((PlayerActor *)this)->interactTarget);
        if (((PlayerActor *)this)->interactTarget == 0) {
            if (Character_FindInteractionTarget(((PlayerActor *)this))) return TRUE;
        } else if (!((Character *)(((PlayerActor *)this)->interactTarget))->checkInteraction((Character *)(((PlayerActor *)this)))) {
            if (((PlayerActor *)this)->interactTarget->acceptsInteractionOutOfRange(((PlayerActor *)this))) return TRUE;
        } else {
            return TRUE;
        }
    }
    if (useHeldTool()) return TRUE;
    if (((PlayerActor *)this)->actionPressed != 0 && ((PlayerActor *)this)->inputMode == 2) {
        volatile VecFx32 saved;
        VecFx32 res;
        saved.x = ((PlayerActor *)this)->targetPos.x;
        saved.y = ((PlayerActor *)this)->targetPos.y;
        saved.z = ((PlayerActor *)this)->targetPos.z;
        PlayerActor_OffsetByAngle(&res, ((PlayerActor *)this), &((PlayerActor *)this)->position, &((PlayerActor *)this)->rotY, &data_020d5e38);
        ((PlayerActor *)this)->targetPos.x = res.x;
        ((PlayerActor *)this)->targetPos.y = res.y;
        ((PlayerActor *)this)->targetPos.z = res.z;
        s32 r = interactAt(0);
        ((PlayerActor *)this)->targetPos.x = saved.x;
        ((PlayerActor *)this)->targetPos.y = saved.y;
        ((PlayerActor *)this)->targetPos.z = saved.z;
        return r;
    }
    return FALSE;
}

void PlayerActor::applyFaceItemChange() {
    using namespace nR;
    if (((PlayerActor *)this)->faceItemState != 0) return;
    s32 r4 = 0x4b;
    volatile u16 buf = ((PlayerActor *)this)->pendingFaceItem;
    BOOL in = FALSE;
    u16 v = buf;
    if (buf >= 0x1431 && v <= 0x1470) in = TRUE;
    if (in) {
        s32 idx;
        if (v >= 0x1431 && v <= 0x1470) idx = v - 0x1431; else idx = -1;
        if (idx >= 0 && (u32)idx < data_020cb35c) {
            r4 = PlayerGlassesModel_GetAccessoryModelId(idx);
        }
    } else if (v >= 0x1471 && v <= 0x1491) {
        if (Item_IsFlowerAltItem((u16 *)&buf)) {
            s32 idx = Item_GetFlowerAltOrdinal((u16 *)&buf);
            if (idx >= 0 && (u32)idx < data_020cb360) {
                r4 = PlayerGlassesModel_GetFlowerModelId(idx);
            }
        }
    }
    PlayerGlassesModelRef_Load(&((PlayerActor *)this)->faceItemRef.slot, r4);
    if (PlayerGlassesModelRef_GetModelId(&((PlayerActor *)this)->faceItemRef.slot) != 0x4b) {
        PlayerGlassesModelRef_RelocateTexture(&((PlayerActor *)this)->faceItemRef.slot);
        PlayerGlassesModelRef_CancelTexUpload(&((PlayerActor *)this)->faceItemRef.slot);
        ((CachedModel *)(&((PlayerActor *)this)->faceItemModel))->release();
        ((CachedModel *)(&((PlayerActor *)this)->faceItemModel))->setFromFile(PlayerGlassesModelRef_GetBuffer(&((PlayerActor *)this)->faceItemRef.slot));
        PlayerGlassesModelRef_PollTexUpload(&((PlayerActor *)this)->faceItemRef.slot);
        ((PlayerActor *)this)->faceItemState = 1;
    } else {
        ((PlayerActor *)this)->faceItemState = 1;
        pollFaceItemLoad();
    }
}

void PlayerActor::pollFaceItemLoad() {
    using namespace nR;
    if (((PlayerActor *)this)->faceItemState == 1) {
        s32 r = 1;
        if (PlayerGlassesModelRef_GetModelId(&((PlayerActor *)this)->faceItemRef.slot) != 0x4b) {
            r = PlayerGlassesModelRef_PollTexUpload(&((PlayerActor *)this)->faceItemRef.slot);
        }
        if (r != 0) {
            ((PlayerActor *)this)->faceItemState = 2;
        }
    }
}

BOOL PlayerActor::requestFaceItemChange(u16 *p) {
    using namespace nR;
    if (((PlayerActor *)this)->faceItemState == 2) {
        ((PlayerActor *)this)->faceItemState = 0;
        ((PlayerActor *)this)->pendingFaceItem = *p;
        return TRUE;
    }
    return FALSE;
}

void PlayerActor::applyHatChange() {
    using namespace nR;
    u16 a[2];
    s32 x, y;
    void *r4, *r6, *r7;
    void *t;
    if (((PlayerActor *)this)->hatState != 0) return;
    a[0] = 0xfff1;
    if (((PlayerActor *)(this))->testActionFlag(12)) {
        a[0] = 0xfff1;
    } else {
        a[0] = ((PlayerActor *)this)->pendingHat;
    }
    PlayerHead_GetModelIds(PlayerActor_GetGender(this), ((PlayerActor *)this)->pendingHairStyle, a, (u32 *)&y, (u32 *)&x);
    a[1] = ((PlayerActor *)this)->pendingHat;
    PlayerHead_Load(&headRef.slot, y, x, &a[1], PlayerActor_GetPlayerData(this));
    applySkinHairPalette(((PlayerActor *)this)->pendingHairColor, calcTan(0));
    r4 = NNS_G3dGetTex(PlayerHead_GetModelFile(&((PlayerActor *)this)->headRef.slot, 0));
    r6 = 0;
    if (x < 0x9e) {
        r6 = NNS_G3dGetTex(PlayerHead_GetModelFile(&((PlayerActor *)this)->headRef.slot, 1));
    }
    if (((PlayerActor *)this)->pendingHatBindFace != 0) {
        r7 = NNS_G3dGetTex(((PlayerFaceTexRef *)(&((PlayerActor *)this)->faceTex))->getBuffer());
        bindTextureByName(r7, r4, sPlayerEyeTexName, sPlayerEyeTexName);
        bindTextureByName(r7, r4, sPlayerMouthTexName, sPlayerMouthTexName);
    }
    r7 = NNS_G3dGetTex(PlayerPaletteRef_GetSkin(&((PlayerActor *)this)->skinHairPalette.slot));
    t = NNS_G3dGetTex(PlayerPaletteRef_GetHair(&((PlayerActor *)this)->skinHairPalette.slot));
    bindPaletteByName(r7, r4, sPlayerSkinPalName, sPlayerSkinPalName);
    bindPaletteByName(t, r4, sPlayerHairPalName, sPlayerHairPalName);
    if (r6 != 0) {
        bindPaletteByName(r7, r6, sPlayerSkinPalName, sPlayerSkinPalName);
    }
    PlayerHead_RelocateTextures(&((PlayerActor *)this)->headRef.slot);
    PlayerHead_CancelTexUpload(&((PlayerActor *)this)->headRef.slot);
    PlayerHead_PollTexUpload(&((PlayerActor *)this)->headRef.slot);
    ((CachedModel *)(&((PlayerActor *)this)->headModel0))->release();
    ((CachedModel *)(&((PlayerActor *)this)->headModel1))->release();
    ((CachedModel *)(&((PlayerActor *)this)->headModel0))->setFromFile(PlayerHead_GetModelFile(&((PlayerActor *)this)->headRef.slot, 0));
    if (PlayerHead_GetModelId(&((PlayerActor *)this)->headRef.slot, 1) < 0x9e) {
        ((CachedModel *)(&((PlayerActor *)this)->headModel1))->setFromFile(PlayerHead_GetModelFile(&((PlayerActor *)this)->headRef.slot, 1));
    }
    if (((PlayerActor *)this)->pendingHatBindFace == 0) {
        u32 p = ((u32)((PlayerActor *)this)->eyeTexAnim.curFrame << 4) >> 16;
        u32 q = ((u32)((PlayerActor *)this)->mouthTexAnim.curFrame << 4) >> 16;
        ((MatTexPatAnim *)(&((PlayerActor *)this)->eyeTexAnim))->release();
        ((MatTexPatAnim *)(&((PlayerActor *)this)->mouthTexAnim))->release();
        ((Heap *)(((CharaFaceAnimWorkRef *)(&((PlayerActor *)this)->faceAnimWork))->getHeap()))->freeAll();
        ((PlayerActor *)(this))->initFaceAnims();
        ((PlayerActor *)this)->eyeTexAnim.setAnim(((CharaFaceAnimRef *)(&((PlayerActor *)this)->faceAnimRef))->getEyeAnimBuffer(), p, 0, ((PlayerActor *)this)->animMode, 0x1000);
        ((PlayerActor *)this)->mouthTexAnim.setAnim(((CharaFaceAnimRef *)(&((PlayerActor *)this)->faceAnimRef))->getMouthAnimBuffer(), q, 0, ((PlayerActor *)this)->animMode, 0x1000);
    }
    ((PlayerActor *)this)->hatState = 1;
}

void PlayerActor::pollHatLoad() {
    using namespace nR;
    if (((PlayerActor *)this)->hatState == 1) {
        if (PlayerHead_PollTexUpload(&((PlayerActor *)this)->headRef.slot)) {
            ((PlayerActor *)this)->hatState = 2;
        }
    }
}

BOOL PlayerActor::requestHatChange(u16 *p, u8 b, u8 c, u8 d) {
    using namespace nR;
    if (((PlayerActor *)this)->hatState == 2) {
        ((PlayerActor *)this)->hatState = 0;
        ((PlayerActor *)this)->pendingHat = *p;
        ((PlayerActor *)this)->pendingHairStyle = b;
        ((PlayerActor *)this)->pendingHairColor = c;
        ((PlayerActor *)this)->pendingHatBindFace = d;
        return TRUE;
    }
    return FALSE;
}

void PlayerActor::bindTextureByName(void *a, void *b, void *c, void *d) {
    using namespace nR;
    G3dRes_CopyTexByName(a, b, (const char *)c, (const char *)d);
}

void PlayerActor::bindPaletteByName(void *a, void *b, void *c, void *d) {
    using namespace nR;
    G3dRes_CopyPlttByName(a, b, (const char *)c, (const char *)d);
}

BOOL PlayerActor::isPosInReach(s32 *pos, u32 idx) {
    using namespace nR;
    VecFx32 p = {pos[0], pos[1], pos[2]};  // (the callers pass the position as three s32: PlayerActor.h)
    if (Vec_DistXZ(&((PlayerActor *)this)->bodyPos, &p) < (s32)(sPlayerReachDist[idx] << 13) >> 12) {
        s32 a = Math_Atan2(pos[0] - ((PlayerActor *)this)->bodyPos.x, pos[2] - ((PlayerActor *)this)->bodyPos.z);
        if (Math_AngleDiffAbs(a, ((PlayerActor *)this)->rotY) < 0x2aaa) {
            return TRUE;
        }
    }
    return FALSE;
}

s32 PlayerActor::useHeldTool() {
    using namespace nR;
    if (Unk_0200f9d4_IsOne(gFieldSceneKind)) {
        return PlayerActor_RoomUseTool(((PlayerActor *)this));
    } else {
        return PlayerActor_FieldUseTool(((PlayerActor *)this));
    }
}

s32 PlayerActor::interactAt(s32 a) {
    using namespace nR;
    if (Unk_0200f9d4_IsOne(gFieldSceneKind)) {
        return Shared_PlayerActor_RoomInteractAt(((PlayerActor *)this), a, 0);
    } else {
        return PlayerActor_FieldInteractAt(((PlayerActor *)this), a);
    }
}

u32 PlayerActor::getFieldAnswerKind() {
    using namespace nR;
    return *(u32 *)((u8 *)FieldAction_Get(((PlayerActor *)this)->fieldQuery) + 0xc);
}

BOOL PlayerActor::startFieldQuery(VecFx32 *a, s32 mode, s32 idx) {
    using namespace nQ;
    if (((PlayerActor *)this)->fieldQuery == -1 && gSceneBlockMap != NULL) {
        Vec2 v;
        v.x = 0;
        v.y = 0;
        FieldPos_ToUnit(&v.x, &v.y, a);
        switch (mode) {
        case 0:
            ((PlayerActor *)this)->fieldQuery = FieldAction_RequestPickUpForAid(((PlayerActor *)this)->sessionSlot, &v, data_020c6210[idx]);
            break;
        case 1:
            ((PlayerActor *)this)->fieldQuery = FieldAction_RequestToolForAid(((PlayerActor *)this)->sessionSlot, &v, data_020c6210[idx], 0, 0xfff1);
            break;
        case 2:
            ((PlayerActor *)this)->fieldQuery = FieldAction_RequestFillHoleForAid(((PlayerActor *)this)->sessionSlot, &v, data_020c6210[idx]);
            break;
        }
    }
    return ((PlayerActor *)this)->fieldQuery != -1 ? TRUE : FALSE;
}

void PlayerActor::pollStoreQuery() {
    using namespace nQ;
    if (((PlayerActor *)this)->dropQuery != -1) {
        switch (FieldAction_PollDrop(((PlayerActor *)this)->dropQuery)) {
        case 1:
        case 2:
            FieldAction_Release(((PlayerActor *)this)->dropQuery);
            ((PlayerActor *)this)->dropQuery = -1;
        }
    }
}

s32 PlayerActor::pollFieldQuery() {
    using namespace nQ;
    s32 r = 0;
    ((PlayerActor *)this)->fieldAnswerKind = 0;
    if (((PlayerActor *)this)->fieldQuery != -1) {
        r = FieldAction_PollResult(((PlayerActor *)this)->fieldQuery);
        switch (r) {
        case 1:
        case 2:
            ((PlayerActor *)this)->fieldAnswerKind = (((PlayerActor *)this))->getFieldAnswerKind();
            FieldAction_Release(((PlayerActor *)this)->fieldQuery);
            ((PlayerActor *)this)->fieldQuery = -1;
        }
    }
    return r;
}

s32 PlayerActor::startUnitItemQuery(Unk_0200f6d4_V2 *p, s32 a, s32 b) {
    using namespace nQ;
    struct {
        s32 pad;
        Vec2 v;
    } l;
    s32 result;
    if (((PlayerActor *)this)->fieldQuery == -1) {
        s32 x = p->x, y = p->y;
        s32 hx = x >> 4, hy = y >> 4;
        u16 *r = BlockMap_GetItemPtr(gSceneBlockMap, hx, hy, x - (hx << 4), y - (hy << 4), 0);
        if (r == NULL) {
            ((PlayerActor *)this)->actionItem = ((PlayerActor *)this)->shownItem = 0xfff1;
            result = 2;
        } else {
            ((PlayerActor *)this)->actionItem = ((PlayerActor *)this)->shownItem = *(u16 *)r;
            l.v.x = p->x;
            l.v.y = p->y;
            ((PlayerActor *)this)->fieldQuery = FieldAction_RequestToolForAid(((PlayerActor *)this)->sessionSlot, &l.v, a, b, 0xfff1);
            if (((PlayerActor *)this)->fieldQuery == -1) {
                result = 0;
            } else {
                ((PlayerActor *)this)->fieldAnswer = pollFieldQuery();
                switch (((PlayerActor *)this)->fieldAnswer) {
                case 0: result = 1; break;
                case 2: result = 0; break;
                default: result = 3; break;
                }
            }
        }
    } else {
        result = 1;
    }
    return result;
}

BOOL PlayerActor::requestByFieldAnswer(VecFx32 *a, s32 b) {
    using namespace nQ;
    s32 t = (((PlayerActor *)this))->getFieldAnswerKind();
    Unk_0200f6d4_V2 v;
    v.x = 0;
    v.y = 0;
    FieldPos_ToUnit(&v.x, &v.y, a);
    switch (t) {
    case 3:
    case 4:
    case 0x15:
    case 0x16:
        if (b) {
            PlayerActor_RequestPickUpItem(((PlayerActor *)this), v, -2, 6, -1);
        } else {
            requestPickUpReach(Unk_0200b750_Pair(v.x, v.y), -1, 6, -1);
        }
        return TRUE;
    case 5:
        PlayerActor_RequestPluckReach(((PlayerActor *)this), v, 6, -1);
        return TRUE;
    case 0x14:
        PlayerActor_RequestAct66(((PlayerActor *)this), &v, 0, 6, -1);
        return TRUE;
    }
    return FALSE;
}

s32 PlayerActor::getHeldHoldableIndex() {
    using namespace nQ;
    Unk_0200f660_S s;
    PlayerActor_GetHeldItem(&s.a, ((PlayerActor *)this));
    if ((((PlayerActor *)this))->testActionFlag(0)) {
        BOOL ok;
        if (Item_IsFurniture(&s.a)) {
            s.b = 0xfff1;
            ok = Item_GetFurnitureIndex(&s.a) == Item_GetFurnitureIndex(&s.b);
        } else {
            ok = s.a == 0xfff1;
        }
        if (!ok) return ItemInfo_GetHoldableIndex(&s) + 1;
    }
    return 0;
}

s32 PlayerActor::getHeldToolKind() {
    using namespace nQ;
    s32 f = getHeldHoldableIndex();
    if (f <= 0) return 0;
    u16 v = f + 0x1368;
    if (v <= 0x136a) return 1;
    if (v <= 0x1373) return 2;
    if (v <= 0x1375) return 3;
    if (v <= 0x1377) return 4;
    if (v <= 0x1379) return 5;
    if (v <= 0x137b) return 6;
    if (v == 0x137c) return 7;
    if (v == 0x137d) return 8;
    if (v <= 0x137f) return 9;
    if (v <= 0x13a7) return 10;
    return 0;
}

namespace nQ {
extern "C" void PlayerActor_StepTowardPose(PlayerActor *o, s32 a, s32 b, s32 c) {
    PlayerActor_StepTowardXZ(o, a, b);
    ((PlayerActor *)(o))->turnToward(c);
}
}

namespace nQ {
extern "C" void PlayerActor_StepTowardPoseFast(PlayerActor *o, s32 a, s32 b, s32 c) {
    s32 *p = &o->position.x;
    PlayerActor_ApproachValue(p, a, 0xe66, 0x4cd, 0x31);
    p += 2;
    PlayerActor_ApproachValue(p, b, 0xe66, 0x4cd, 0x31);
    ((PlayerActor *)(o))->turnToward(c);
}
}

namespace nQ {
extern "C" void PlayerActor_StepTowardXZ(PlayerActor *o, s32 a, s32 b) {
    s32 *p = &o->position.x;
    PlayerActor_ApproachCoord(p, a);
    p += 2;
    PlayerActor_ApproachCoord(p, b);
}
}

void PlayerActor::turnToward(s32 a) {
    using namespace nQ;
    s16 t = ((PlayerActor *)this)->rotY;
    PlayerActor_TurnAngle(&t, a);
    setAngleY(&t);
}

s32 PlayerActor::turnToCamera(s32 a) {
    using namespace nQ;
    s16 t = ((PlayerActor *)this)->rotY;
    BOOL r = PlayerActor_ApproachAngle(&t, 0, a, 0x1770000, 0xc0000) == 0;
    ((PlayerActor *)(((PlayerActor *)this)))->setAngleY(&t);
    return r;
}

s32 PlayerActor::turnAwayFromCamera(s32 a) {
    using namespace nQ;
    s16 t = ((PlayerActor *)this)->rotY;
    BOOL r = PlayerActor_ApproachAngle(&t, -0x8000, a, 0x1770000, 0xc0000) == 0;
    ((PlayerActor *)(((PlayerActor *)this)))->setAngleY(&t);
    return r;
}

namespace nQ {
extern "C" void PlayerActor_GetFrontUnitCenter(VecFx32 *a, PlayerActor *b) {
    VecFx32 v;
    PlayerActor_GetFrontPoint(&v, b);
    FieldPos_SnapToUnitCenter(a, &v);
}
}

namespace nQ {
extern "C" void PlayerActor_GetFrontPoint(VecFx32 *out, PlayerActor *o) {
    PlayerActor_OffsetByAngle(out, o, &o->position, &o->rotY, &sPlayerFrontPointDist);
}
}

namespace nQ {
extern "C" void PlayerActor_OffsetByAngle(VecFx32 *out, void *o, const VecFx32 *in, const s16 *ang, const s32 *p) {
    s32 i;
    s32 s, c;
    *out = *in;
    i = ((u16)*ang >> 4) * 2;
    s = func_01ffcb0c(data_02135f44[i + 1], *p);
    c = func_01ffcb0c(data_02135f44[i], *p);
    out->x += c;
    out->z += s;
}
}

void PlayerActor::updateFootstepFx() {
    using namespace nQ;
    if ((&(AnimFrameCtrl &)((PlayerActor *)this)->bodyModel)->hasPassedFrame(1) || (&(AnimFrameCtrl &)((PlayerActor *)this)->bodyModel)->hasPassedFrame(9)) {
        s16 rot[2];
        VecFx32 v;
        rot[0] = ((PlayerActor *)this)->rotY;
        rot[1] = rot[0] + 0x8000;
        if (((AnimFrameCtrl *)(&(AnimFrameCtrl &)((PlayerActor *)this)->bodyModel))->hasPassedFrame(9)) {
            v.x = ((PlayerActor *)this)->footPosA.x;
            v.y = ((PlayerActor *)this)->footPosA.y;
            v.z = ((PlayerActor *)this)->footPosA.z;
        } else {
            v.x = ((PlayerActor *)this)->footPosB.x;
            v.y = ((PlayerActor *)this)->footPosB.y;
            v.z = ((PlayerActor *)this)->footPosB.z;
        }
        v.y = ((PlayerActor *)this)->position.y;
        if (((PlayerActor *)this)->animId == 2) {
            Effect_Create(0, &v, &rot[1], 0);
        }
        Effect_Create(0x27, &v, &rot[0], 0);
        playFootstepSe();
    }
}

void PlayerActor::playFootstepSe() {
    using namespace nQ;
    u32 f = ((PlayerActor *)this)->actorFlags;
    BOOL c = (f & 4) ? TRUE : FALSE;
    if (!(c && ((f & 2) ? TRUE : FALSE))) {
        if ((((PlayerActor *)this))->testActionFlag(0xa)) {
            BOOL flag = ((PlayerActor *)this)->animId == 2;
            VecFx32 v;
            v.x = (((PlayerActor *)this)->footPosA.x + ((PlayerActor *)this)->footPosB.x) >> 1;
            v.y = (((PlayerActor *)this)->footPosA.y + ((PlayerActor *)this)->footPosB.y) >> 1;
            v.z = (((PlayerActor *)this)->footPosA.z + ((PlayerActor *)this)->footPosB.z) >> 1;
            if ((((PlayerActor *)this))->testActionFlag(0x19)) {
                Snd_SeEmitterPlayAlternate(&((PlayerActor *)this)->seEmitterLocal, Footstep_GetSeAtPos(&v));
            } else {
                Snd_SeEmitterPlayAlternate(&((PlayerActor *)this)->seEmitterRemote, Footstep_GetSeAtPos(&v));
            }
        }
    }
}

namespace nQ {
extern "C" s32 PlayerActor_CompareLastPlayDateNow(PlayerActor *o) {
    u32 a[4];
    a[0] = 0;
    a[1] = 0;
    a[2] = 0;
    a[3] = 0;
    return PlayerActor_CompareLastPlayDate(o, a, (u8 *)&a[2]);
}
}

namespace nQ {
extern "C" s32 PlayerActor_CompareLastPlayDate(PlayerActor *o, void *a, u8 *b) {
    Unk_0200f17c_Date d0, d1, d2;
    {
        u16 raw = ((PlayerData *)(void *)(PlayerActor_GetPlayerData(o)))->getLastPlayDate();
        __builtin_memcpy(&d0, &raw, sizeof(d0));
    }
    d1 = d0;
    d2 = d1;
    Clock_GetDateTime(a);
    MI_CpuCopy8(a, b, 8);
    b[5] = d2.a;
    b[4] = d2.b;
    b[3] = d2.c;
    return DateTime_Compare(a, b, 0x3f);
}
}

namespace nQ {
extern "C" void PlayerActor_SetLastPlayDate(PlayerActor *o, PlayerData *a, Unk_0200f17c_Date *d) {
    Unk_0209865c_Bits v;
    v.a = d->a;
    v.b = d->b;
    v.c = d->c;
    a->setLastPlayDate(v);
    u16 *r = (u16 *)DebugVar_GetPtr(0, 0x50);
    *r = d->c * 10 + (d->b * 1000 + PlayerActor_GetTan(o));
    if (!(o)->isGuestInSession()) {
        *PlayerSession_GetLastPlayDate() = *d;
    }
}
}

BOOL PlayerActor::checkLidAndError() {
    using namespace nQ;
    if (((CommManager *)(gCommManager))->isLocalSlot(((PlayerActor *)this)->sessionSlot)) {
        if ((((PlayerActor *)this))->testActionFlag(0xb) || (((PlayerActor *)this))->testActionFlag(0x1b)) return FALSE;
        if (LowBattery_Poll()) {
            (((PlayerActor *)this))->requestErrorMessage(0, 6, -1);
            return TRUE;
        }
        if ((s32)(*HW_SYSWORK(volatile u16, buttonXY) & 0x8000) >> 15) {
            (((PlayerActor *)this))->requestLidClosed(1, -1);
            return TRUE;
        }
    }
    return FALSE;
}

namespace nQ {
extern "C" void PlayerActor_CalcHandItemPos(VecFx32 *dst, PlayerActor *o, u32 *p) {
    Mtx43 m = o->itemHandMtx;
    s32 tx = m.m[9], ty = m.m[10], tz = m.m[11];
    m.m[9] = m.m[10] = m.m[11] = 0;
    Mtx43 m2 = m;
    VecFx32 v, out;
    v.x = func_01ffcb0c(0x800, *p);
    v.y = func_01ffcb0c(-0x400, *p);
    v.z = 0;
    MTX_MultVec43(&v, &m2, &out);
    VecFx32 r;
    tx += out.x;
    ty += out.y;
    tz += out.z;
    r.x = tx;
    r.y = ty;
    r.z = tz;
    WorldCurve_FromCurved(dst, &r);
}
}

void PlayerActor::updateShownItemPos(u32 a, ...) {
    using namespace nP;
    VecFx32 v;
    PlayerActor_CalcHandItemPos(&v, ((PlayerActor *)this), &a);
    Math_ApproachS32(&((PlayerActor *)this)->shownItemPos.x, v.x, 0x800, 0x2000, 0x333);
    Math_ApproachS32(&((PlayerActor *)this)->shownItemPos.y, v.y, 0x800, 0x2000, 0x333);
    Math_ApproachS32(&((PlayerActor *)this)->shownItemPos.z, v.z, 0x800, 0x2000, 0x333);
}

BOOL PlayerActor::getNetTransformInArea(VecFx32 *out, s16 *ang) {
    using namespace nP;
    if (gCommManager->isLocalSlot(((PlayerActor *)this)->sessionSlot)) {
        return FALSE;
    }
    if (testActionFlag(0x10)) {
        return FALSE;
    }
    out->y = ((PlayerActor *)this)->position.y + 0;
    u8 buf;
    if (!(((PlayerActor *)this))->getRemoteTransform(&buf, &out->x, &out->z, ang)) {
        return FALSE;
    }
    if (buf == Scene_GetCurrent()) {
        return TRUE;
    }
    return FALSE;
}

BOOL PlayerActor::netFollowTransform() {
    using namespace nP;
    VecFx32 v;
    s16 ang;
    if (getNetTransformInArea(&v, &ang)) {
        VecFx32 *pos = (VecFx32 *)&((PlayerActor *)this)->position;
        if (pos->x == v.x && pos->z == v.z && ang == ((PlayerActor *)this)->rotY) {
            return TRUE;
        }
        if (Vec_DistXZ(pos, &v) >= 0xe000) {
            VecFx32 *q = (VecFx32 *)&((PlayerActor *)this)->prevPosition;
            *q = v;
            *pos = *q;
            ((PlayerActor *)this)->rotY = ang;
            return FALSE;
        } else {
            PlayerActor_StepTowardPoseFast(((PlayerActor *)this), v.x, v.z, ang);
            return FALSE;
        }
    }
    return TRUE;
}

BOOL PlayerActor::netSyncNearUnit(s32 *p) {
    using namespace nP;
    VecFx32 t;
    FieldPos_FromUnitCenter(&t, p[0], p[1]);
    return netSyncNearPoint(&t);
}

BOOL PlayerActor::netSyncNearPoint(VecFx32 *p) {
    using namespace nP;
    VecFx32 v;
    s16 ang;
    VecFx32 *pos = (VecFx32 *)&((PlayerActor *)this)->position;
    if (getNetTransformInArea(&v, &ang)) {
        if (Vec_DistXZ(&v, p) >= 0x4000) {
            ang = Math_Atan2(p->x - pos->x, p->z - pos->z);
            v.x = p->x - func_01ffcb0c(0x2000, data_02135f44[(u16)ang >> 4 << 1]);
            v.z = p->z - func_01ffcb0c(0x2000, data_02135f44[((u16)ang >> 4 << 1) + 1]);
        } else {
            ang = ((PlayerActor *)this)->rotY;
        }
    } else {
        return TRUE;
    }
    if (Vec_DistXZ(pos, &v) <= 0x200 && Math_AngleDiffAbs(((PlayerActor *)this)->rotY, ang) < 0x100) {
        return TRUE;
    }
    if (Vec_DistXZ(pos, &v) >= 0xe000) {
        VecFx32 *q = (VecFx32 *)&((PlayerActor *)this)->prevPosition;
        *q = v;
        *pos = *q;
        ((PlayerActor *)this)->rotY = ang;
        return TRUE;
    }
    return FALSE;
}

void PlayerActor::playSe(u32 a) {
    using namespace nP;
    u32 f = ((PlayerActor *)this)->actorFlags;
    if (!(Unk_0200ec54_Bit(f, 4) && Unk_0200ec54_Bit(f, 2))) {
        if (testActionFlag(0x19)) {
            Snd_SeEmitterPlayOneShot(&((PlayerActor *)this)->seEmitterLocal, a, 0x7f, 0);
        } else {
            Snd_SeEmitterPlayOneShot(&((PlayerActor *)this)->seEmitterRemote, a, 0x7f, 0);
        }
    }
}

void PlayerActor::playSeAt(u32 a, VecFx32 *v) {
    using namespace nP;
    u32 f = ((PlayerActor *)this)->actorFlags;
    if (!(Unk_0200ec54_Bit(f, 4) && Unk_0200ec54_Bit(f, 2))) {
        if (testActionFlag(0x19)) {
            Snd_SeEmitterPlayHeld(&((PlayerActor *)this)->seEmitterLocal, a, 0x7f, 0);
        } else {
            Snd_SeEmitterPlayHeld(&((PlayerActor *)this)->seEmitterRemote, a, 0x7f, 0);
        }
        ((PlayerActor *)this)->sePosX = v->x;
        ((PlayerActor *)this)->sePosY = v->y;
        ((PlayerActor *)this)->sePosZ = v->z;
    }
}

u32 PlayerActor::testActionFlag(u32 id) {
    using namespace nP;
    return ((PlayerActor *)this)->actionFlags & (1 << id);
}

void PlayerActor::setActionFlag(u32 id) {
    using namespace nP;
    ((PlayerActor *)this)->actionFlags |= (1 << id);
}

void PlayerActor::clearActionFlag(u32 id) {
    using namespace nP;
    ((PlayerActor *)this)->actionFlags &= ~(1 << id);
}

void PlayerActor::loadInputMode() {
    using namespace nP;
    if (InputMode_IsButtons()) {
        ((PlayerActor *)this)->inputMode = 1;
    } else if (InputMode_IsTouch()) {
        ((PlayerActor *)this)->inputMode = 2;
    } else {
        ((PlayerActor *)this)->inputMode = 0;
    }
}

BOOL PlayerActor::getHeldItemPos(VecFx32 *out) {
    using namespace nP;
    u32 a = ((PlayerActor *)this)->itemHandMtx.m[9];
    if (a == 0 && ((PlayerActor *)this)->itemHandMtx.m[10] == 0 && ((PlayerActor *)this)->itemHandMtx.m[11] == 0) {
        return FALSE;
    }
    out->x = a;
    out->y = ((PlayerActor *)this)->itemHandMtx.m[10];
    out->z = ((PlayerActor *)this)->itemHandMtx.m[11];
    WorldCurve_FromCurved(out, out);
    return TRUE;
}

void PlayerActor::netSendClothesChange(u32 a, u32 b) {
    using namespace nP;
    if (gCommManager->isOnline()) {
        u8 buf[4];
        PlayerActor_PackClothesChange(buf, b, a);
        CommManager *g = gCommManager;
        (g)->beginRecord();
        (g)->writeRecord(buf, 3);
        (g)->endRecord(0x2b, 4);
    }
}

namespace nP {
extern "C" void PlayerActor_NetSendFaceChange() {
    if (gCommManager->isOnline()) {
        u8 buf = 0;
        CommManager *g = gCommManager;
        (g)->beginRecord();
        (g)->writeRecord((u8 *)(&buf), 1);
        (g)->endRecord(0x2d, 4);
    }
}
}

void PlayerActor::netSendTan() {
    using namespace nP;
    CommManager *g = gCommManager;
    if ((g)->isOnline()) {
        u8 buf = PlayerActor_GetTan(((PlayerActor *)this));
        CommManager *g2 = gCommManager;
        (g2)->beginRecord();
        (g2)->writeRecord((u8 *)(&buf), 1);
        (g2)->endRecord(0x2e, 4);
    }
}

void PlayerActor::updateHeadLook() {
    using namespace nP;
    if (testActionFlag(0x15)) {
        PlayerActor_ApproachAngle(&((PlayerActor *)this)->headPitch, ((PlayerActor *)this)->headPitchTarget, 0x400, 0x1770000, 0xc0000);
        PlayerActor_ApproachAngle(&((PlayerActor *)this)->headYaw, ((PlayerActor *)this)->headYawTarget, 0x400, 0x1770000, 0xc0000);
        if (((PlayerActor *)this)->headPitchTarget == 0 && ((PlayerActor *)this)->headYawTarget == 0 && ((PlayerActor *)this)->headPitch == 0 && ((PlayerActor *)this)->headYaw == 0) {
            clearActionFlag(0x15);
        }
    }
}

BOOL PlayerActor::canAcceptTalk(u32 id) {
    using namespace nP;
    if (id == 0x1e) {
        if (((PlayerActor *)this)->actionWorkRaw[1] == 0 && ((PlayerActor *)this)->requestCount == 0) {
            return TRUE;
        }
    } else if (id == 0x4f) {
        if (((PlayerActor *)this)->requestCount == 0) {
            return TRUE;
        }
    }
    return sPlayerActionTalkable[id];
}

void PlayerActor::netUpdateBodyCollider() {
    using namespace nP;
    struct { u8 scene; s16 angle; } s;
    volatile s32 sx, sy, sz;
    VecFx32 *q = (VecFx32 *)&((PlayerActor *)this)->position;
    sx = q->x;
    sy = q->y;
    sz = q->z;
    if (((PlayerActor *)this)->bodyCollider.isHit != 0 && ((PlayerActor *)this)->bodyCollider.hitTargetKind == 0x15) {
        u32 n = ((PlayerActor *)this)->bodyCollider.hitTargetIndex;
        if (gCommManager->isLocalSlot(n) && PlayerActor_IsInAction(2, 4)) {
            VecFx32 v;
            if (PlayerActor_GetSlotPosXZ(&s.scene, &v.x, &v.z, -1, ((PlayerActor *)this)->sessionSlot)) {
                VecFx32 v2;
                VecFx32 *p = (VecFx32 *)PlayerActor_GetBodyPos(n);  // (the definition returns the position as u8 *)
                VecFx32 *q2 = (VecFx32 *)&((PlayerActor *)this)->position;
                v2 = *q2;
                s32 a = Math_Atan2(p->x - v2.x, p->z - v2.z);
                s.angle = Math_Atan2(v.x - v2.x, v.z - v2.z);
                if (Math_AngleDiffAbs(a, s.angle) < 0x400) {
                    s32 d1 = Vec_DistXZ(&v2, p);
                    s32 d2 = Vec_DistXZ(&v2, &v);
                    if (d1 <= d2) {
                        VecFx32 out;
                        if (((PlayerActor *)this)->sessionSlot >= (s32)n) {
                            s.angle = s.angle + 0x4000;
                        } else {
                            s.angle = s.angle - 0x4000;
                        }
                        PlayerActor_OffsetByAngle(&out, ((PlayerActor *)this), &v2, &s.angle, &data_020d5e40);
                        VecFx32 *q3 = (VecFx32 *)&((PlayerActor *)this)->position;
                        *q3 = out;
                    }
                }
            }
        }
    }
    (((PlayerActor *)this))->updateBodyCollider();
    q = (VecFx32 *)&((PlayerActor *)this)->position;
    q->x = sx;
    q->y = sy;
    q->z = sz;
}

void PlayerActor::resetHeldToolAnim() {
    using namespace nP;
    s32 r = ((PlayerActor *)(this))->getHeldToolKind();
    if (r == 4) {
        HeldItemModel_PlayAnim(&((PlayerActor *)this)->heldItemModel, 0, 9, 0);
    } else if (r == 3) {
        HeldItemModel_PlayAnim(&((PlayerActor *)this)->heldItemModel, 0x13, 3, 0);
    } else if (r == 10) {
        HeldItemModel_PlayAnim(&((PlayerActor *)this)->heldItemModel, 0x23, 3, 0);
    } else if (r == 6) {
        HeldItemModel_PlayAnim(&((PlayerActor *)this)->heldItemModel, 0x21, 3, 0);
    }
}

void PlayerActor::calcHandMtx() {
    using namespace nP;
    Mtx43 buf;
    ((PlayerActor *)this)->drawTilt = WorldCurve_ToCurved(&((PlayerActor *)this)->drawPos, &((PlayerActor *)this)->position);
    ((Actor *)(void *)(((PlayerActor *)this)))->calcModelMatrix(&buf);
    ((PlayerActor *)this)->bodyModel.mtx = buf;
    data_021cb69c = ((PlayerActor *)this)->bodyModel.mtx;
    ((Model *)(&((PlayerActor *)this)->bodyModel))->setAlpha(0);
    ((TwoLayerAnimModel *)(&((PlayerActor *)this)->bodyModel))->drawLayered(0);
    Model_GetJointWorldMtx(&((PlayerActor *)this)->bodyModel, &((PlayerActor *)this)->itemHandMtx, 11);
}

BOOL PlayerActor::isGuestInSession() {
    using namespace nP;
    if (!gCommManager->isSlotActive(gCommManager->myAid)) {
        return FALSE;
    }
    if (((PlayerActor *)this)->sessionSlot != 0) {
        return TRUE;
    }
    return FALSE;
}

namespace nP {
extern "C" BOOL PlayerActor_IsWaitingForSlots() {
    s32 t = Scene_GetPrevious();
    if (t == 12 || t == 13 || t == 14 || (u8)(t + 0xd2) <= 1) {
        s16 buf;
        s32 i = 0;
        CommManager *g = gCommManager;
        for (; i < 4; i++) {
            if ((g)->isSlotActive(i) && !PlayerActor_GetSlotAngle(&buf, -1, i)) {
                return TRUE;
            }
        }
    }
    return FALSE;
}
}

// The same opening as onMessageEnd / onChoice with nothing after it: the load of msgStep and the compare stay.
void PlayerActor::onMessageStart(u32 attr) {
    using namespace nP;
    if (((PlayerActor *)this)->msgStep >= 0xf) return;
}

u8 PlayerActor::isLocomotionAction(u32 i) {
    using namespace nO;
    return sPlayerActionIsLocomotion[i];
}

BOOL PlayerActor::getRemoteTransform(u8 *a, s32 *b, s32 *c, s16 *d) {
    using namespace nO;
    struct {
        u8 a;
        s16 b;
    } t;
    s32 t8, tc;
    if (!PlayerActor_GetSlotPosXZ(&t.a, &t8, &tc, -1, ((PlayerActor *)this)->sessionSlot)) return FALSE;
    if (!PlayerActor_GetSlotAngle(&t.b, -1, ((PlayerActor *)this)->sessionSlot)) return FALSE;
    *a = t.a;
    *b = t8;
    *c = tc;
    *d = t.b;
    return TRUE;
}

namespace nO {
extern "C" BOOL PlayerActor_CalcNetFollowAngle(PlayerActor *unused, s32 a, s32 b, s32 c, s16 d, s16 *out) {
    s32 r5;
    BOOL r;
    if (c < func_01ffcb0c(0x108, 0x108)) return FALSE;
    r5 = Math_Atan2(a, b);
    if (c >= func_01ffcb0c(0x2666, 0x2666)) {
        r = TRUE;
    } else {
        s32 diff = (s16)(r5 - d);
        if (diff < 0) diff = -diff;
        if (diff < 0x4000) r = TRUE; else r = FALSE;
    }
    if (r) *out = r5;
    return r;
}
}

PlayerActionRequest::PlayerActionRequest() {
    using namespace nO;}

PlayerActionRequest::~PlayerActionRequest() {
    using namespace nO;}

PlayerActionRequestBase::PlayerActionRequestBase() {
    using namespace nO;}

PlayerActionRequestBase::~PlayerActionRequestBase() {
    using namespace nO;}

void PlayerActionRequest::assign(s32 a, s32 b, s16 c) {
    using namespace nO;
    action = a;
    priority = b;
    netSeq = c;
}

BOOL PlayerActor::pushRequest(PlayerActionRequest *r) {
    using namespace nO;
    s32 n = ((PlayerActor *)this)->requestCount;
    if (n < 0x1e) {
        if (((PlayerActor *)this)->bestRequest >= 0) {
            s32 t = r->priority;
            if (getRequest(((PlayerActor *)this)->bestRequest)->priority < t) {
                ((PlayerActor *)this)->bestRequest = ((PlayerActor *)this)->requestCount;
            }
        } else {
            ((PlayerActor *)this)->bestRequest = n;
        }
        PlayerActionRequest *d = &((PlayerActor *)this)->requests[((PlayerActor *)this)->requestCount];
        *d = *r;  // (the whole payload: larger than unk_0c when it holds a pointer)
        ((PlayerActor *)this)->requestCount++;
        return TRUE;
    }
    return FALSE;
}

PlayerActionRequest *PlayerActor::getRequest(s32 i) {
    using namespace nO;
    if (i >= 0 && i < 0x1e && i < ((PlayerActor *)this)->requestCount) {
        return &((PlayerActor *)this)->requests[i];
    }
    return NULL;
}

void PlayerActor::clearRequests() {
    using namespace nO;
    ((PlayerActor *)this)->requestCount = 0;
    ((PlayerActor *)this)->bestRequest = -1;
}

s32 PlayerActor::getEffectivePriority() {
    using namespace nO;
    s32 r2 = 0;
    if (((PlayerActor *)this)->bestRequest >= 0) {
        r2 = getRequest(((PlayerActor *)this)->bestRequest)->priority;
    }
    s32 r0 = ((PlayerActor *)this)->actionPriority;
    if (r0 <= r2) r0 = r2;
    return r0;
}

s32 PlayerActor::getRequiredPriority() {
    using namespace nO;
    s32 r2 = 0;
    if (((PlayerActor *)this)->bestRequest >= 0) {
        r2 = getRequest(((PlayerActor *)this)->bestRequest)->priority;
    }
    s32 r0 = ((PlayerActor *)this)->actionPriority + 1;
    if (r0 <= r2) r0 = r2;
    return r0;
}

void PlayerActor::initShirtModel() {
    using namespace nO;
    u16 v;
    void *r4, *r6;
    PlayerBodyModelRef_SetSlot(&((PlayerActor *)this)->bodyModelRef.slot, data_020c61a8[PlayerSession_GetGfxSlot(((PlayerActor *)this)->sessionSlot)]);
    if (PlayerActor_GetGender(this) == 0) {
        PlayerBodyModelRef_Load(&((PlayerActor *)this)->bodyModelRef.slot, 0);
    } else {
        PlayerBodyModelRef_Load(&((PlayerActor *)this)->bodyModelRef.slot, 1);
    }
    ((CharaClothTexRef *)(&((PlayerActor *)this)->shirtTex))->assign(data_020c61a0[PlayerSession_GetGfxSlot(((PlayerActor *)this)->sessionSlot)]);
    PlayerActor_GetShirt(&v, ((PlayerActor *)this));
    ((PlayerActor *)(this))->setShirtTexture(&v);
    r4 = NNS_G3dGetTex(PlayerBodyModelRef_GetBuffer(&((PlayerActor *)this)->bodyModelRef.slot));
    r6 = NNS_G3dGetTex(CharaClothTexRef_GetBuffer(&((PlayerActor *)this)->shirtTex.v));
    ((PlayerActor *)(this))->bindTextureByName(r6, r4, (void *)(void *)(sPlayerClothTexName), (void *)(void *)(sPlayerClothTexName));
    ((PlayerActor *)(this))->bindPaletteByName(r6, r4, (void *)(void *)(sPlayerClothTexName), (void *)(void *)(sPlayerClothTexName));
    r4 = NNS_G3dGetTex(PlayerPaletteRef_GetSkin(&((PlayerActor *)this)->skinHairPalette.slot));
    bindPaletteByName(r4, NNS_G3dGetTex(PlayerBodyModelRef_GetBuffer(&((PlayerActor *)this)->bodyModelRef.slot)), (void *)sPlayerSkinPalName, (void *)sPlayerSkinPalName);
    PlayerBodyModelRef_RelocateTexture(&((PlayerActor *)this)->bodyModelRef.slot);
}

void PlayerActor::initHatModel() {
    using namespace nO;
    u16 v[2];
    s32 r4, r3;
    PlayerHead_SetSlot(&((PlayerActor *)this)->headRef.slot, data_020c61b4[PlayerSession_GetGfxSlot(((PlayerActor *)this)->sessionSlot)]);
    PlayerActor_GetHat(&v[1], ((PlayerActor *)this));
    v[0] = v[1];
    r4 = PlayerActor_GetHairStyle(this);
    r3 = PlayerActor_GetHairColor(this);
    ((PlayerActor *)(this))->requestHatChange(v, r4, r3, 1);
}

void PlayerActor::initFaceItemModel() {
    using namespace nO;
    u16 v[2];
    PlayerGlassesModelRef_SetSlot(&((PlayerActor *)this)->faceItemRef.slot, data_020c6198[PlayerSession_GetGfxSlot(((PlayerActor *)this)->sessionSlot)]);
    PlayerActor_GetFaceItem(&v[0], ((PlayerActor *)this));
    v[1] = v[0];
    ((PlayerActor *)(this))->requestFaceItemChange(&v[1]);
    ((PlayerActor *)(this))->applyFaceItemChange();
}

BOOL PlayerActor::doCreate() {
    using namespace nO;
    s32 r6 = ((PlayerActor *)this)->param;
    s32 r5;
    ((PlayerActor *)this)->sessionSlot = PlayerActor_ParamGetSlot(r6);
    ((Character *)(this))->setCharId((u16)((PlayerActor *)this)->sessionSlot);
    PlayerSession_SetActor(((PlayerActor *)this)->sessionSlot, ((PlayerActor *)this));
    PlayerPaletteRef_Assign(&((PlayerActor *)this)->skinHairPalette.slot, data_020c6194[PlayerSession_GetGfxSlot(((PlayerActor *)this)->sessionSlot)]);
    r5 = PlayerActor_GetHairColor(this);
    ((PlayerActor *)(this))->applySkinHairPalette(r5, ((PlayerActor *)(this))->calcTan(1));
    initShirtModel();
    ((CharaFaceAnimWorkRef *)(&((PlayerActor *)this)->faceAnimWork))->assign(data_020c61a4[PlayerSession_GetGfxSlot(((PlayerActor *)this)->sessionSlot)]);
    ((CharaFaceAnimRef *)(&((PlayerActor *)this)->faceAnimRef))->assign(data_020c61ac[PlayerSession_GetGfxSlot(((PlayerActor *)this)->sessionSlot)]);
    ((PlayerFaceTexRef *)(&((PlayerActor *)this)->faceTex))->assign(data_020c619c[PlayerSession_GetGfxSlot(((PlayerActor *)this)->sessionSlot)]);
    ((PlayerFaceTexRef *)(&((PlayerActor *)this)->faceTex))->load(PlayerActor_GetFaceTexIndex(this));
    initHatModel();
    initFaceItemModel();
    r5 = PlayerSession_GetGfxSlot(((PlayerActor *)this)->sessionSlot);
    u16 t;
    PlayerActor_GetHeldItem(&t, ((PlayerActor *)this));
    HeldItemModel_Setup(&((PlayerActor *)this)->heldItemModel, data_020c61b0[r5], ((PlayerActor *)this), &t, PlayerActor_GetPlayerData(this), 0);
    ((FishBobber *)(&((PlayerActor *)this)->heldItemModel.bobber))->setOwnerAid(((PlayerActor *)this)->sessionSlot);
    ((PlayerActor *)this)->gravity = 0xffffb000;
    ((PlayerActor *)this)->maxFallSpeed = 0xffff6000;
    ((PlayerActor *)(this))->requestInit(PlayerActor_ParamGetAction(r6), 9, -1);
    ((PlayerActor *)this)->pendingEvent.set = 0;
    ((Unk_02005e7c *)(this))->processRequests();
    ((PlayerActor *)this)->actionFlags = 0;
    ((PlayerActor *)(this))->setActionFlag(1);
    BOOL b = (gFieldSceneKind == 0);
    if (b) ((PlayerActor *)(this))->setActionFlag(0);
    VecFx32 *p = (VecFx32 *)&((PlayerActor *)this)->position;
    ((PlayerActor *)this)->bodyPos.x = p->x;
    ((PlayerActor *)this)->bodyPos.y = p->y;
    ((PlayerActor *)this)->bodyPos.z = p->z;
    ((PlayerActor *)this)->fieldQuery = -1;
    ((PlayerActor *)this)->dropQuery = -1;
    ((PlayerActor *)this)->exitIndex = -1;
    ((PlayerActor *)this)->exitMode = 0;
    ((PlayerActor *)this)->fieldAnswerKind = 0;
    ((PlayerActor *)this)->fieldAnswer = 0;
    ((PlayerActor *)this)->msgStep = 0xf;
    ((PlayerActor *)this)->alpha = 0x1f;
    ((PlayerActor *)this)->tripCooldown = 0;
    ((PlayerActor *)this)->toolHitActor = 0;
    ((PlayerActor *)this)->toolHitKind = 0;
    return TRUE;
}

s32 PlayerActor::getInputMagnitude() {
    using namespace nN;
    return ((PlayerActor *)this)->inputMagnitude;
}

s16 PlayerActor::getInputAngleRaw() {
    using namespace nN;
    return ((PlayerActor *)this)->inputAngle;
}

s16 PlayerActor::getInputAngle() {
    using namespace nN;
    s16 r = getInputAngleRaw();
    if (((PlayerActor *)this)->inputMode != 2 && gCamera) {
        r = r + gCamera->getYaw();
    }
    return r;
}

s32 PlayerActor::getInputSideRelative() {
    using namespace nN;
    return Math_AngleToSide((s16)(getInputAngle() - ((PlayerActor *)this)->rotY));
}

s32 PlayerActor::getInputDirRelative() {
    using namespace nN;
    return Math_AngleToDir4((s16)(getInputAngle() - ((PlayerActor *)this)->rotY));
}

u8 PlayerActor::isInteractPressed() {
    using namespace nN;
    return ((PlayerActor *)this)->interactPressed;
}

void PlayerInitArgs::setInitArgs(u32 v) {
    using namespace nN;
    firstAction = v;
}

BOOL PlayerActor::requestInit(u32 a, u32 b, u32 c) {
    using namespace nN;
    PlayerActionRequest m;
    m.assign(0, b, c);
    m.unk_0c_d5b4.setInitArgs(a);
    ((PlayerActor *)this)->prevAction = 0;
    u32 r = ((PlayerActor *)(this))->pushRequest(&m);
    return r;
}

void PlayerInitWork::initInitWork(u32 v) {
    using namespace nN;
    firstAction = v;
    modelSetupDone = 0;
}

void PlayerActor::setupInit(PlayerActionRequest *item) {
    using namespace nN;
    ((PlayerInitWork *)((PlayerActor *)this)->actionWorkRaw)->initInitWork(item->unk_0c_d5b4.firstAction);
    ((PlayerActor *)this)->drawStep = 0;
}

void PlayerActor::netInit() {
    using namespace nN;
}

void PlayerActor::endInit(u32 a) {
    using namespace nN;
    void *r6 = gCommManager;
    struct { u8 scene; s16 angle; u16 item[2]; } arr;
    s32 vx, vz;
    if (((CommManager *)(r6))->isLocalSlot(((PlayerActor *)this)->sessionSlot) || (a != 2 && a != 0x8b)) {
        ((PlayerActor *)this)->drawStep = 1;
    }
    if (!((CommManager *)(r6))->isLocalSlot(((PlayerActor *)this)->sessionSlot)) {
        s32 t = Scene_GetCurrent();
        if (!((PlayerActor *)(this))->testActionFlag(0x10) && t != 0x2e && t != 0xd && t != 0xe && t != 0xc && t != 0x2f) {
            if (PlayerActor_GetSlotPosXZ(&arr.scene, &vx, &vz, -1, ((PlayerActor *)this)->sessionSlot)) {
                s32 *e = (s32 *)&((PlayerActor *)this)->position;
                s32 *d = (s32 *)&((PlayerActor *)this)->prevPosition;
                d[0] = vx;
                e[0] = d[0];
                d[2] = vz;
                e[2] = d[2];
            }
            if (PlayerActor_GetSlotAngle(&arr.angle, -1, ((PlayerActor *)this)->sessionSlot)) {
                ((PlayerActor *)(this))->setAngleY(&arr.angle);
            }
        }
    }
    if (((PlayerActor *)(this))->testActionFlag(0)) {
        PlayerData *t = PlayerData_GetBySessionSlot(((PlayerActor *)this)->sessionSlot);
        arr.item[0] = *t->getHeldItem();
        HeldItemModel_SetItem(&((PlayerActor *)this)->heldItemModel, &arr.item[0], t);
        BOOL ok;
        if (Item_IsFurniture(&arr.item[0])) {
            arr.item[1] = 0xfff1;
            ok = Item_GetFurnitureIndex(&arr.item[0]) == Item_GetFurnitureIndex(&arr.item[1]);
        } else {
            ok = arr.item[0] == 0xfff1;
        }
        if (!ok) {
            ((PlayerActor *)(this))->startAnim(0, 0, 0);
            HeldItemModel_Update(&((PlayerActor *)this)->heldItemModel);
        }
    }
}

BOOL PlayerActor::finishModelSetup() {
    using namespace nN;
    BOOL result = FALSE;
    PlayerInitWork *p = (PlayerInitWork *)((PlayerActor *)this)->actionWorkRaw;
    if (p->modelSetupDone == 0) {
        BOOL a = PlayerBodyModelRef_PollTexUpload(&((PlayerActor *)this)->bodyModelRef.slot);
        BOOL b = ((PlayerActor *)this)->hatState == 2;
        BOOL c = ((PlayerActor *)this)->faceItemState == 2;
        if (a && b && c) {
            ((CachedModel *)(&((PlayerActor *)this)->bodyModel))->setFromFile(PlayerBodyModelRef_GetBuffer(&((PlayerActor *)this)->bodyModelRef.slot));
            PlayerBodyWorkRef_Assign(&((PlayerActor *)this)->bodyWork.slot, data_020c6190[PlayerSession_GetGfxSlot(((PlayerActor *)this)->sessionSlot)]);
            ((TwoLayerAnimModel *)(&((PlayerActor *)this)->bodyModel))->allocLayerAnims(PlayerBodyWorkRef_GetHeap(&((PlayerActor *)this)->bodyWork.slot));
            ((CachedModel *)(&((PlayerActor *)this)->bodyModel))->allocJointRecord(PlayerBodyWorkRef_GetHeap(&((PlayerActor *)this)->bodyWork.slot));
            ((PlayerActor *)(this))->initFaceAnims();
            s32 i = PlayerSession_GetGfxSlot(((PlayerActor *)this)->sessionSlot) * 2;
            AnimSlotRef_Assign(&((PlayerActor *)this)->bodyAnimSlot, data_020c61b8[i]);
            AnimSlotRef_Assign(&((PlayerActor *)this)->holdAnimSlot, (data_020c61b8 + 1)[i]);
            ((PlayerActor *)(this))->startAnim(0, 0, 0);
            ((AnimModel *)(&((PlayerActor *)this)->bodyModel))->attachAnim();
            ((Model *)&((PlayerActor *)this)->bodyModel)->setCallback((void (*)(NNSG3dRS *))PlayerActor_JointCbPre, 6, 1, (PlayerActor *)this, 0);
            ((MatTexPatAnim *)(&((PlayerActor *)this)->eyeTexAnim))->applyFrame();
            ((MatTexPatAnim *)(&((PlayerActor *)this)->mouthTexAnim))->applyFrame();
            result = TRUE;
            p->modelSetupDone++;
        }
    }
    return result;
}

void PlayerActor::startFirstAction(s32 *p) {
    using namespace nM;
    if (*p == 0) {
        return;
    }
    ((PlayerActor *)this)->actionPriority = ((Unk_02007694 *)(void *)(this))->getActionDonePriority(((PlayerActor *)this)->action);
    s32 r4 = ((PlayerInitWork *)((PlayerActor *)this)->actionWorkRaw)->firstAction;
    s32 f;
    CommManager *r7;
    BOOL c = gFieldSceneKind == 0 ? TRUE : FALSE;
    if (c && r4 != 1 && gCommManager->isLocalSlot(sessionSlot)) {
        if (Ground_GetHeightAt(&((PlayerActor *)this)->position, NULL, 0x19) >= 0x800) {
            r4 = 0x6d;
        }
    }
    c = gFieldSceneKind == 0 ? TRUE : FALSE;
    if (!c || Scene_InTown()) {
        ((NpcEmotionFx *)(&((PlayerActor *)this)->emotionFx))->reset();
        if (gCommManager->isLocalSlot(((PlayerActor *)this)->sessionSlot) || (Scene_InUnk6To8() && r4 == 8)) {
            ((SndSeEmitter *)(&((PlayerActor *)this)->seEmitterLocal))->callInit();
            ((PlayerActor *)this)->emotionFx.seEmitter = &((PlayerActor *)this)->seEmitterLocal;
            ((PlayerActor *)this)->emotionFx.useGlobalSe = 1;
            ((PlayerActor *)(this))->setActionFlag(0x19);
        } else {
            ((SndSeEmitter *)(&((PlayerActor *)this)->seEmitterRemote))->callInit();
            ((PlayerActor *)this)->emotionFx.seEmitter = &((PlayerActor *)this)->seEmitterRemote;
        }
        ((PlayerActor *)(this))->setActionFlag(0xa);
    }
    r7 = gCommManager;
    if (((CommManager *)(r7))->isLocalSlot(((PlayerActor *)this)->sessionSlot)) {
        s32 r0 = Scene_GetCurrent();
        if (r0 != 0xc && r0 != 0x2f) {
            if (!Scene_NoPlayerInUnsharedScene() || PlayerActor_IsWaitingForSlots() == 1) {
                FieldInfoBalloon_ShowPleaseWait();
                ((PlayerActor *)(this))->setActionFlag(0x1b);
            } else if (r7->myAid == 0) {
                if ((u8)(Net_GetMode() + 0xfd) <= 1) {
                    Net_WifiHostKeepAlive();
                }
            }
        }
    }
    f = 0;
    switch (r4) {
    case 1:
        requestAct01(9, -1);
        break;
    case 8:
        if (Scene_InUnk6To8()) {
            Shared_FishShadow_RunAi(((PlayerActor *)this), 0, 6, -1);
        } else {
            if (!((PlayerActor *)(this))->testActionFlag(0x1b)) {
                f = 1;
            }
            Shared_FishShadow_RunAi(((PlayerActor *)this), 1, 6, -1);
        }
        break;
    case 0x3b:
        PlayerActor_RequestDoorExit(((PlayerActor *)this), 6, -1);
        break;
    case 0x3c:
        if (((CommManager *)(r7))->isLocalSlot(((PlayerActor *)this)->sessionSlot) && !Field_HasExitedBuildingKey()) {
            data_021c1b3c->faintBgm.startSilence();
            PlayerActor_RequestFaint(((PlayerActor *)this), 0, 7, -1);
            r4 = 0x6d;
        } else {
            Shared_PlayerActor_RequestAct3C(((PlayerActor *)this), 1, -1);
        }
        break;
    case 0x6d:
        data_021c1b3c->faintBgm.startSilence();
        PlayerActor_RequestFaint(((PlayerActor *)this), 1, 7, -1);
        break;
    case 0x6e:
        PlayerActor_RequestAct6E(((PlayerActor *)this), 7, -1);
        break;
    case 0x43:
        PlayerActor_RequestAct43(((PlayerActor *)this), 6, -1);
        break;
    case 0x44:
        PlayerActor_RequestAct44(((PlayerActor *)this), 6, -1);
        break;
    case 0x28:
        if (!((PlayerActor *)(this))->testActionFlag(0x1b)) {
            f = 1;
        }
        PlayerActor_RequestSit(((PlayerActor *)this), 0, 6, -1);
        break;
    case 0x10:
        ((PlayerActor *)this)->drawStep = 1;
        ((PlayerActor *)(this))->requestAct10(3, 5, -1);
        break;
    case 0x8b:
        PlayerActor_RequestDoorWalkIn(((PlayerActor *)this), &((PlayerActor *)this)->position, 6, -1);
        ((PlayerActor *)(this))->setActionFlag(5);
        break;
    case 0x8e:
        PlayerActor_RequestExitWalkIn(((PlayerActor *)this), &((PlayerActor *)this)->position, 6, -1);
        ((PlayerActor *)(this))->setActionFlag(5);
        break;
    default:
        r4 = 2;
        requestWait(0, 1, -1);
        if (!((PlayerActor *)(this))->testActionFlag(0x1b)) {
            f = 1;
        }
        ((PlayerActor *)(this))->offsetSpawnBySlot();
        break;
    }
    ((PlayerActor *)this)->param = ((r4 << 22) & 0x3fc00000) | (((PlayerActor *)this)->param & 0xc03fffff);
    if (f) {
        if (((CommManager *)(r7))->isLocalSlot(((PlayerActor *)this)->sessionSlot)) {
            TalkRequest_FinishSceneEntry();
        }
    }
}

void PlayerActor::mainInit() {
    using namespace nM;
    s32 v = 0;
    v = ((PlayerActor *)(this))->finishModelSetup();
    startFirstAction(&v);
}

BOOL PlayerActor::requestAct01(u32 a, u32 b) {
    using namespace nM;
    PlayerActionRequest m;
    m.assign(1, a, b);
    BOOL r = this->pushRequest(&m);
    return r;
}

void PlayerActor::setupAct01(PlayerActionRequest *item, u32 old) {
    using namespace nM;
    ((PlayerActor *)this)->drawStep = 0;
}

void PlayerActor::netAct01(u32 a) {
    using namespace nM;
    requestAct01(9, a);
}

void PlayerActor::endAct01() {
    using namespace nM;
    ((PlayerActor *)this)->drawStep = 1;
}

void PlayerActor::mainAct01() {
    using namespace nM;
}

namespace nM {
extern "C" void PlayerActor_SetArgsWait(u16 *p, u32 v) {
    *p = v;
}
}

BOOL PlayerActor::requestWait(u32 a, u32 b, u32 c) {
    using namespace nM;
    PlayerActionRequest m;
    m.assign(2, b, c);
    PlayerActor_SetArgsWait((u16 *)&m.unk_0c_b[0], a);
    BOOL r = this->pushRequest(&m);
    return r;
}

void PlayerActor::setupWait(PlayerActionRequest *item, u32 old) {
    using namespace nM;
    if (!((PlayerActor *)(this))->checkLidAndError()) {
        u16 h = *(u16 *)&((PlayerActionRequest *)item)->unk_0c_b[0];
        *(u8 *)(PlayerWalkWork *)((PlayerActor *)this)->actionWorkRaw = old == 4 ? 1 : 0;
        ((PlayerActor *)(this))->startAnim(0, h, h);
        ((PlayerActor *)(this))->resetHeldToolAnim();
        ((PlayerActor *)(this))->clearActionFlag(9);
        ((PlayerActor *)this)->toolHitActor = 0;
        ((PlayerActor *)this)->toolHitKind = 0;
        PlayerActor_SetHeadTilt(0, 0, 4);
    }
}

void PlayerActor::netWait(u32 a) {
    using namespace nM;
    requestWait(3, 5, a);
}

void PlayerActor::endWait() {
    using namespace nM;
    if (!gCommManager->isLocalSlot(((PlayerActor *)this)->sessionSlot)) {
        ((PlayerActor *)this)->drawStep = 1;
    }
}

BOOL PlayerActor::waitFollowNet() {
    using namespace nM;
    u8 a;
    s16 b, c, e;
    s32 x, z;
    if (!this->getRemoteTransform(&a, &x, &z, &b)) {
        return FALSE;
    }
    if (a != Scene_GetCurrent()) {
        return FALSE;
    }
    s32 dx = x - ((PlayerActor *)this)->position.x;
    s32 dz = z - ((PlayerActor *)this)->position.z;
    s32 d2 = func_01ffcb0c(dx, dx) + func_01ffcb0c(dz, dz);
    BOOL r4 = PlayerActor_CalcNetFollowAngle(this, dx, dz, d2, b, &c);
    if (!r4) {
        e = ((PlayerActor *)this)->rotY;
        PlayerActor_TurnAngleSlow(&e, b);
        ((PlayerActor *)(this))->setAngleY((s16 *)(&e));
        PlayerActor_ApproachCoord(&((PlayerActor *)this)->position.x, x);
        PlayerActor_ApproachCoord(&((PlayerActor *)this)->position.z, z);
    }
    return r4;
}

void PlayerActor::waitCheckInput() {
    using namespace nM;
    s16 buf;
    s32 r6 = ((PlayerActor *)(this))->getTargetWalkSpeed();
    s32 r4 = 0;
    if (Scene_InHouseRoom()) {
        BOOL b = gScreenTransition.state == 2 ? TRUE : FALSE;
        if (b) {
            if (!(*(u8 *)PlayerSession_GetSessionFlags() & 8)) {
                if (sHouseRoachActiveCount) {
                    *(u8 *)PlayerSession_GetSessionFlags() |= 8;
                    ((PlayerActor *)(this))->requestAct77(((PlayerActor *)this)->rotY, 6, -1);
                    return;
                }
            }
        }
    }
    if (r6 > 0) {
        u32 v = *(u8 *)(PlayerWalkWork *)((PlayerActor *)this)->actionWorkRaw;
        if (r6 < 0x53f) {
            v = 0;
        }
        r4 = requestWalk(v, 1, -1);
    }
    if (!this->tryInteract()) {
        r4 |= ((PlayerActor *)(this))->checkLidAndError();
    } else {
        r4 = 1;
    }
    if (r4) {
        s32 h = ((PlayerActor *)this)->rotY;
        buf = h + 0xaf0;
        Effect_Create(0x27, &((PlayerActor *)this)->footPosB, &buf, 0);
        buf = h - 0xaf0;
        Effect_Create(0x27, &((PlayerActor *)this)->footPosA, &buf, 0);
    }
}

void PlayerActor::waitNetCheckEnd(u8 *p) {
    using namespace nM;
    if (*p != 0) {
        requestWalk(0, 1, -1);
    }
    if (((PlayerActor *)this)->drawStep == 0) {
        if (PlayerActor_TestSlotFlag(0x1b, 4)) {
            ((PlayerActor *)this)->drawStep = 1;
        } else {
            u32 buf;
            if (PlayerActor_GetSlotAction(&buf, -1, ((PlayerActor *)this)->sessionSlot)) {
                if (buf == 2 || buf == 0x40) {
                    ((PlayerActor *)this)->drawStep = 1;
                }
            }
        }
    }
}

void PlayerActor::mainWait() {
    using namespace nM;
    if (gCommManager->isLocalSlot(((PlayerActor *)this)->sessionSlot)) {
        ((PlayerActor *)(this))->moveWithCollision();
        ((PlayerActor *)(this))->advanceAnim();
        ((PlayerActor *)(this))->updateBodyCollider();
        ((PlayerActor *)(this))->submitSceneCollider();
        waitCheckInput();
    } else {
        u8 b = waitFollowNet();
        ((PlayerActor *)(this))->advanceAnim();
        ((PlayerActor *)(this))->netUpdateBodyCollider();
        ((PlayerActor *)(this))->submitSceneCollider();
        waitNetCheckEnd(&b);
    }
    *(u8 *)(PlayerWalkWork *)((PlayerActor *)this)->actionWorkRaw = 0;
}

namespace nM {
extern "C" void PlayerActor_SetArgsWalk(u8 *p, u32 v) {
    *p = v;
}
}

BOOL PlayerActor::requestWalk(u32 a, u32 b, u32 c) {
    using namespace nM;
    PlayerActionRequest m;
    m.assign(3, b, c);
    PlayerActor_SetArgsWalk(&m.unk_0c_b[0], a);
    BOOL r = this->pushRequest(&m);
    return r;
}

void PlayerWalkWork::initWalk() {
    using namespace nM;
    walkSpeed = 0;
}

void PlayerActor::setupWalk(PlayerActionRequest *item, u32 old) {
    using namespace nM;
    u8 *p = &((PlayerActionRequest *)item)->unk_0c_b[0];
    PlayerWalkWork *q = (PlayerWalkWork *)((PlayerActor *)this)->actionWorkRaw;
    ((PlayerActor *)(this))->startAnim(1, 3, 3);
    if (((PlayerActor *)(this))->getHeldToolKind() == 4) {
        HeldItemModel_PlayAnim(&((PlayerActor *)this)->heldItemModel, 1, 3, 0);
    }
    q->initWalk();
    if (*p != 0) {
        q->walkSpeed = 0x456;
    }
}

void PlayerActor::netWalk() {
    using namespace nM;
}

void PlayerActor::walkUpdateSpeed() {
    using namespace nM;
    s32 *r6 = &((PlayerWalkWork *)((PlayerActor *)this)->actionWorkRaw)->walkSpeed;
    s32 r4 = ((PlayerActor *)(this))->getTargetWalkSpeed();
    s32 cur = *r6;
    if (cur < r4) {
        *r6 = PlayerActor_Accelerate(cur, r4);
    } else if (cur > r4) {
        *r6 = PlayerActor_Decelerate(cur, r4);
    }
    s32 r7 = this->getInputAngle();
    s16 buf = ((PlayerActor *)this)->rotY;
    if (r4 > 0) {
        PlayerActor_TurnAngle(&buf, r7);
        ((PlayerActor *)(this))->setAngleY((s16 *)(&buf));
    }
    s32 r = func_01ffcb0c(*r6, data_02135f44[((u16)(s16)(buf - r7) >> 4) * 2 + 1]);
    if (r < 0) {
        r = -r;
    }
    s32 t = r;
    ((PlayerActor *)(this))->setSpeed((u32 *)(&t));
}

BOOL PlayerActor::walkFollowNet() {
    using namespace nM;
    u8 a;
    s16 b, c, e;
    s32 x, z, v;
    if (!this->getRemoteTransform(&a, &x, &z, &b)) {
        return TRUE;
    }
    if (a != Scene_GetCurrent()) {
        return TRUE;
    }
    s32 dx = x - ((PlayerActor *)this)->position.x;
    s32 dz = z - ((PlayerActor *)this)->position.z;
    s32 d2 = func_01ffcb0c(dx, dx) + func_01ffcb0c(dz, dz);
    if (!PlayerActor_CalcNetFollowAngle(this, dx, dz, d2, b, &c)) {
        return TRUE;
    }
    e = ((PlayerActor *)this)->rotY;
    PlayerActor_TurnAngle(&e, c);
    ((PlayerActor *)(this))->setAngleY((s16 *)(&e));
    v = ((PlayerActor *)this)->speed;
    s32 r6 = FX_Sqrt(d2);
    s32 m = r6;
    if (r6 > 0x6e2) {
        m = 0x6e2;
    }
    s32 s = data_02135f44[((u16)(s16)(e - c) >> 4) * 2 + 1];
    if (s < 0) {
        s = 0;
    }
    s32 r4 = func_01ffcb0c(m, s);
    if (r6 >= 0x2666) {
        if (r6 >= 0xe000) {
            ((PlayerActor *)this)->position.x = x;
            ((PlayerActor *)this)->position.z = z;
            ((PlayerActor *)this)->rotY = b;
            return TRUE;
        }
    } else {
        s32 t = func_01ffcb0c(r6 - 0x108, FX_Div(0x5da, 0x255e)) + 0x108;
        if (t < 0x108) {
            t = 0x108;
        }
        if (r4 > t) {
            r4 = t;
        }
    }
    if (v < r4) {
        v = PlayerActor_Accelerate(v, r4);
    } else if (v > r4) {
        v = PlayerActor_Decelerate(v, r4);
    }
    ((PlayerActor *)(this))->setSpeed((u32 *)(&v));
    return FALSE;
}

void PlayerActor::walkMove() {
    using namespace nM;
    s32 v, c, r4;
    VecFx32 out;
    s16 sx;
    v = ((PlayerActor *)this)->speed;
    VecFx32 old;
    VecFx32 *pp = (VecFx32 *)&((PlayerActor *)this)->position;
    old.x = pp->x;
    old.y = pp->y;
    old.z = pp->z;
    BOOL r6 = Field_TryPushSnowball(&out, &sx, &c, ((PlayerActor *)this)->inputMagnitude, ((PlayerActor *)this)->inputAngle);
    if (r6) {
        pp = (VecFx32 *)&((PlayerActor *)this)->position;
        pp->x = out.x;
        pp->y = out.y;
        pp->z = out.z;
        ((PlayerActor *)(this))->setAngleY((s16 *)(&sx));
        v = Vec_DistXZ(&((PlayerActor *)this)->position, &old);
        ((PlayerActor *)(this))->setSpeed((u32 *)(&v));
        v = c;
        v = v << 2;
    }
    r4 = func_01ffcb0c(v, 0x3ae1);
    if (r4 <= (s32)((PlayerActor *)this)->bodyModel.numFrames) {
        ((PlayerActor *)this)->bodyModel.frameStep = r4;
        AnimFrameCtrl &p = *(BlendAnimModel *)HeldItemModel_GetModel(&((PlayerActor *)this)->heldItemModel.slot);
        if (r4 <= (s32)p.numFrames) {
            AnimFrameCtrl &q = *(BlendAnimModel *)HeldItemModel_GetModel(&((PlayerActor *)this)->heldItemModel.slot);
            q.frameStep = r4;
        }
    }
    if (r6) {
        if (((PlayerActor *)this)->animId != 3) {
            ((PlayerActor *)(this))->switchAnim(3, 3, 3);
        }
    } else if (v > 0x53f) {
        if (((PlayerActor *)this)->animId != 2) {
            ((PlayerActor *)(this))->switchAnim(2, 3, 3);
        }
    } else if (((PlayerActor *)this)->animId != 1) {
        ((PlayerActor *)(this))->switchAnim(1, 3, 3);
    }
    ((TwoLayerAnimModel *)(&((PlayerActor *)this)->bodyModel))->updateLayers();
    ((PlayerActor *)(this))->updateFootstepFx();
}

void PlayerActor::walkUpdateLean() {
    using namespace nM;
    if (((PlayerActor *)this)->animId != 2) {
        PlayerActor_LevelTiltForAction(this, 0x93, 1);
    } else {
        s32 t = ((PlayerActor *)this)->speed - 0x53f;
        if (t < 0) {
            PlayerActor_LevelTiltForAction(this, 0x93, 1);
        }
        s32 a = FX_Div(t, 0x1a3);
        s32 b = func_01ffcb0c(0xc17000, a);
        ((PlayerActor *)this)->approachRotX((b << 4) >> 16);
    }
}

void Unk_02007694::walkCheckEnd(s16 *p) {
    using namespace nL;
    BOOL c = gFieldSceneKind == 0;
    if (c && ((Unk_02007694 *)this)->tripCooldown == 0 && ((Unk_02007694 *)this)->speed == 0x6e2) {
        u16 t;
        if (PlayerData_GetCurrent()->getFortune() == 2 || (PlayerActor_GetHat(&t, ((Unk_02007694 *)this)), t == 0x13c3)) {
            if (Random_GlobalBelow(0x17a) == 0) {
                PlayerActor_RequestTrip(((Unk_02007694 *)this), 6, -1);
                return;
            }
        }
    }
    if (((Unk_02007694 *)this)->actionWork <= 0) {
        ((PlayerActor *)(this))->requestWait(3, 1, -1);
    }
    if (((Unk_02007694 *)this)->animId == 2 && ((PlayerActor *)(this))->getInputMagnitude() > 0) {
        u16 x = ((PlayerActor *)(this))->getInputAngle();
        if (Math_AngleDiffAbs(x, *p) > 0x3a4c) {
            requestSkidTurn(x, 3, -1);
        }
    }
    ((PlayerActor *)(this))->tryInteract();
}

void Unk_02007694::walkNetCheckEnd(u8 *p) {
    using namespace nL;
    if (*p != 0) {
        ((PlayerActor *)(this))->requestWait(3, 1, -1);
    }
}

void Unk_02007694::mainWalk() {
    using namespace nL;
    if (((CommManager *)(void *)(gCommManager))->isLocalSlot(((Unk_02007694 *)this)->sessionSlot)) {
        s16 x = ((Unk_02007694 *)this)->moveAngleY;
        ((PlayerActor *)(this))->walkUpdateSpeed();
        ((PlayerActor *)(this))->walkMove();
        ((PlayerActor *)(this))->walkUpdateLean();
        ((PlayerActor *)(this))->moveWithCollision();
        ((PlayerActor *)(this))->updateBodyCollider();
        ((PlayerActor *)(this))->submitSceneCollider();
        walkCheckEnd(&x);
    } else {
        u8 f = ((PlayerActor *)(this))->walkFollowNet();
        ((PlayerActor *)(this))->walkMove();
        ((PlayerActor *)(this))->walkUpdateLean();
        ((PlayerActor *)(this))->moveNoCollision();
        ((PlayerActor *)(this))->netUpdateBodyCollider();
        walkNetCheckEnd(&f);
    }
}

void PlayerChangeClothesArgs::setSkidTurnArgs(u16 a) {
    using namespace nL;
    unk_00 = a;
}

u32 Unk_02007694::requestSkidTurn(u16 a, u32 b, u32 c) {
    using namespace nL;
    PlayerActionRequest obj;
    ((PlayerActionRequest *)(&obj))->assign(4, b, c);
    obj.unk_0c_c2fc.setSkidTurnArgs(a);
    u32 r = ((PlayerActor *)(this))->pushRequest(&obj);
    return r;
}

void PlayerChangeClothesWork::initSkidTurn(s16 a) {
    using namespace nL;
    unk_00 = a;
}

void Unk_02007694::setupSkidTurn(PlayerActionRequest *item) {
    using namespace nL;
    s16 v = *(s16 *)&item->unk_0c_c2fc;
    VecFx32 vec;
    PlayerChangeClothesWork *p = &((Unk_02007694 *)this)->clothesWork;
    ((PlayerActor *)(this))->startAnim(4, 3, 0);
    p->initSkidTurn(v);
    s32 b, a;
    a = ((Unk_02007694 *)this)->rotY << 12;
    b = v << 12;
    s32 d = b - a;
    if (d < 0) d = -d;
    if (b >= a) {
        if (d > 0x8000000) {
            v += 0x6fd0;
        } else {
            v += 0x9030;
        }
    } else {
        if (d > 0x8000000) {
            v += 0x9030;
        } else {
            v += 0x6fd0;
        }
    }
    vec = ((Unk_02007694 *)this)->footPosA;
    s32 i = (u16)v >> 4;
    s32 t1, t0;
    s32 sn = data_02135f44[i * 2];
    s32 cs = data_02135f44[i * 2 + 1];
    t0 = func_01ffcb0c(cs, 0x666) - func_01ffcb0c(sn, 0x666);
    t1 = func_01ffcb0c(sn, 0x666) + func_01ffcb0c(cs, 0x666);
    vec.x += t1;
    vec.z += t0;
    p->skidEffectAngle = v + 0x8000;
    p->skidEffectAngle -= 0x38e;
    Effect_Create(1, &vec, &v, 0);
    ((PlayerActor *)(this))->playSe(0x53);
}

void Unk_02007694::netSkidTurn() {
    using namespace nL;
}

void Unk_02007694::endSkidTurn(u32 a) {
    using namespace nL;
    ((Unk_02007694 *)this)->moveAngleY = ((Unk_02007694 *)this)->rotY;
}

void Unk_02007694::skidDecelerate() {
    using namespace nL;
    PlayerChangeClothesWork *p = &((Unk_02007694 *)this)->clothesWork;
    s32 t;
    PlayerActor_TurnAngle(&((Unk_02007694 *)this)->rotY, (s16)p->unk_00);
    s32 v = ((Unk_02007694 *)this)->speed;
    t = PlayerActor_DecelerateSkid(v, 0);
    ((PlayerActor *)(this))->setSpeed((u32 *)(void *)(&t));
    if (t == 0 && v != 0) {
        Effect_Create(0x29, &((PlayerActor *)this)->position, &p->skidEffectAngle, 0);
    }
}

void Unk_02007694::skidCheckEnd() {
    using namespace nL;
    s32 t = ((Unk_02007694 *)this)->speed;
    if (((Unk_02007694 *)this)->rotY == (s16)((Unk_02007694 *)this)->clothesWork.unk_00 && t <= 0) {
        ((Unk_02007694 *)this)->actionPriority = this->getActionDonePriority(((Unk_02007694 *)this)->action);
        ((PlayerActor *)(this))->requestWait(3, 1, -1);
    }
}

void Unk_02007694::mainSkidTurn() {
    using namespace nL;
    skidDecelerate();
    ((PlayerActor *)(this))->advanceAnim();
    ((PlayerActor *)(this))->moveWithCollision();
    ((PlayerActor *)(this))->updateBodyCollider();
    ((PlayerActor *)(this))->submitSceneCollider();
    skidCheckEnd();
}

void PlayerChangeClothesArgs::setAct05Args(u16 a) {
    using namespace nL;
    unk_00 = a;
}

u32 Unk_02007694::requestAct05(u16 a, u32 b, u32 c) {
    using namespace nL;
    PlayerActionRequest obj;
    ((PlayerActionRequest *)(&obj))->assign(5, b, c);
    obj.unk_0c_c2fc.setAct05Args(a);
    u32 r = ((PlayerActor *)(this))->pushRequest(&obj);
    return r;
}

void Unk_02007694::setupAct05(PlayerActionRequest *item) {
    using namespace nL;
    u16 v = item->unk_0c_c2fc.unk_00;
    ((PlayerActor *)(this))->startAnim(0x12, v, v);
    ((PlayerActor *)(this))->resetHeldToolAnim();
}

void Unk_02007694::netAct05(u32 a) {
    using namespace nL;
    requestAct05(3, 5, a);
}

void Unk_02007694::mainAct05() {
    using namespace nL;
    ((PlayerActor *)(this))->advanceAnim();
    if (((PlayerActor *)(this))->netFollowTransform()) {
        ((PlayerActor *)(this))->moveWithCollision();
    }
    ((PlayerActor *)(this))->updateBodyCollider();
}

void PlayerChangeClothesArgs::setChangeClothesArgs(u16 a, s32 b, s32 c) {
    using namespace nL;
    unk_00 = a;
    changeKind = b;
    wearStyle = c;
}

u32 Unk_02007694::requestChangeClothes(u16 a, u32 b, u32 c, u32 d, s16 e) {
    using namespace nL;
    PlayerActionRequest obj;
    ((PlayerActionRequest *)(&obj))->assign(7, d, e);
    obj.unk_0c_c2fc.setChangeClothesArgs(a, b, c);
    u32 r = ((PlayerActor *)(this))->pushRequest(&obj);
    return r;
}

void PlayerChangeClothesWork::initChangeClothes(u16 a, s32 b, s32 c, s16 d) {
    using namespace nL;
    unk_00 = a;
    changeKind = b;
    wearStyle = c;
    isApplied = 0;
    spinStep = -d / 3;
}

void PlayerNetChangeClothesArgs::writeChangeClothesNet(u16 a, u8 b, u8 c) {
    using namespace nL;
    NetBuf_WriteU16(this, a);
    changeKind = b;
    wearStyle = c;
}

void PlayerNetChangeClothesArgs::readChangeClothesNet(u16 *a, u8 *b, u8 *c) {
    using namespace nL;
    *a = NetBuf_ReadU16(this);
    *b = changeKind;
    *c = wearStyle;
}

void Unk_02007694::setupChangeClothes(PlayerActionRequest *item) {
    using namespace nL;
    PlayerChangeClothesArgs *p = &item->unk_0c_c2fc;
    u16 a = p->unk_00;
    s32 b = p->changeKind;
    s32 c = p->wearStyle;
    if (c == 0x10) {
        ((PlayerActor *)(this))->startAnimOnce(6, 3, 0);
    } else {
        ((PlayerActor *)(this))->startAnimOnce(5, 3, 0);
    }
    ((Unk_02007694 *)this)->clothesWork.initChangeClothes(a, b, c, ((Unk_02007694 *)this)->rotY);
    ((PlayerNetChangeClothesArgs *)((Unk_02007694 *)this)->netData.bytes)->writeChangeClothesNet(a, b, c);
    if (b == 2 && a == 0x13c3) {
        ((PlayerActor *)(this))->playSe(0x77);
    } else {
        ((PlayerActor *)(this))->playSe(0x76);
    }
    Effect_PlayById2(0x61, &((PlayerActor *)this)->position, 0, 0);
}

void Unk_02007694::netChangeClothes(s16 a) {
    using namespace nL;
    u16 x;
    u8 y, z;
    ((PlayerNetChangeClothesArgs *)((Unk_02007694 *)this)->netData.bytes)->readChangeClothesNet(&x, &y, &z);
    requestChangeClothes(x, y, z, 6, a);
}

void Unk_02007694::changeClothesEffects() {
    using namespace nL;
    ((PlayerActor *)(this))->advanceAnim();
    if ((&static_cast<AnimFrameCtrl &>(((Unk_02007694 *)this)->bodyModel))->hasPassedFrame(0xd)) {
        Effect_Create(0x22, &((PlayerActor *)this)->position, 0, 0);
    } else if ((&static_cast<AnimFrameCtrl &>(((Unk_02007694 *)this)->bodyModel))->hasPassedFrame(3)) {
        s16 x = ((Unk_02007694 *)this)->rotY;
        VecFx32 v;
        s16 h;
        v = ((Unk_02007694 *)this)->footPosB;
        v.z += 0x266;
        h = x + 0xaf0;
        Effect_Create(0x27, &v, &h, 0);
        v = ((Unk_02007694 *)this)->footPosA;
        v.z += 0x266;
        h = x - 0xaf0;
        Effect_Create(0x27, &v, &h, 0);
    }
}

void Unk_02007694::changeClothesShirt() {
    using namespace nL;
    PlayerChangeClothesWork *p = &((Unk_02007694 *)this)->clothesWork;
    if (p->isApplied == 0) {
        u16 t = p->unk_00;
        ((PlayerActor *)(this))->setShirtTexture((void *)(void *)(&t));
        if (((PlayerActor *)(this))->requestShirtTexUpload()) {
            p->isApplied = 1;
        }
    }
}

void Unk_02007694::changeClothesFaceItem() {
    using namespace nL;
    PlayerChangeClothesWork *p = &((Unk_02007694 *)this)->clothesWork;
    if (p->isApplied == 0) {
        u16 t = p->unk_00;
        if (((PlayerActor *)(this))->requestFaceItemChange(&t)) {
            p->isApplied = 1;
        }
    }
}

void Unk_02007694::changeClothesHat() {
    using namespace nL;
    PlayerChangeClothesWork *p = &((Unk_02007694 *)this)->clothesWork;
    if (p->isApplied == 0) {
        u16 t = p->unk_00;
        u32 a = PlayerActor_GetHairStyle(this);
        u32 b = PlayerActor_GetHairColor(this);
        if (((PlayerActor *)(this))->requestHatChange(&t, a, b, 0)) {
            p->isApplied = 1;
        }
    }
}

void Unk_02007694::changeClothesSpin() {
    using namespace nL;
    s16 v = ((Unk_02007694 *)this)->rotY; s32 a; s16 t = ((Unk_02007694 *)this)->clothesWork.spinStep;
    if (v != 0) {
        a = v;
        if (a < 0) a = -a;
        if (a < (t < 0 ? -t : t)) {
            v = 0;
        } else {
            v += t;
        }
    }
    ((PlayerActor *)(this))->setAngleY(&v);
}

void PlayerActor::changeClothesCheckEnd() {
    using namespace nK;
    if (((AnimFrameCtrl *)(&(AnimFrameCtrl &)((PlayerActor *)this)->bodyModel))->isFinished()) {
        ((PlayerActor *)this)->actionPriority = ((Unk_02007694 *)(void *)(this))->getActionDonePriority(((PlayerActor *)this)->action);
        s32 t = ((PlayerActionWork *)((PlayerActor *)this)->actionWorkRaw)->w8;
        if (t == 5) {
            ((Unk_02007694 *)(this))->requestAct05(0, 5, -1);
        } else if (t == 0x10) {
            requestAct10(0, 5, -1);
        } else {
            PlayerActor_RequestReturnToWait();
        }
    }
}

void PlayerActor::mainChangeClothes() {
    using namespace nK;
    ((Unk_02007694 *)(this))->changeClothesEffects();
    ((Unk_02007694 *)(this))->changeClothesApply();
    ((Unk_02007694 *)(this))->changeClothesSpin();
    ((PlayerActor *)(this))->updateBodyCollider();
    changeClothesCheckEnd();
}

void PlayerAct10Args::setAct10Args(s16 v) {
    using namespace nK;
    blendFrames = v;
}

s32 PlayerActor::requestAct10(s16 a, u32 b, s32 c) {
    using namespace nK;
    PlayerActionRequest m;
    s32 r;
    m.assign(0x10, b, c);
    m.act10.setAct10Args(a);
    r = pushRequest(&m);
    return r;
}

void PlayerActor::setupAct10(PlayerActionRequest *item, u32 old) {
    using namespace nK;
    u16 v = *(u16 *)&((PlayerActionRequest *)item)->unk_0c_b[0];
    if (!this->testActionFlag(0x17)) {
        ((PlayerActor *)(this))->startAnim(0, v, 0);
    }
    ((PlayerActor *)(this))->resetHeldToolAnim();
    ((PlayerActor *)(this))->clearActionFlag(0x1d);
}

void PlayerActor::netAct10(u32 v) {
    using namespace nK;
    requestAct10(3, 5, v);
}

void PlayerActor::act10UpdateAnim() {
    using namespace nK;
    ((PlayerActor *)(this))->advanceAnim();
    if (((AnimFrameCtrl *)(&(AnimFrameCtrl &)((PlayerActor *)this)->bodyModel))->isFinished()) {
        ((PlayerActor *)(this))->startAnim(0, 3, 0);
    }
}

void PlayerActor::act10FaceTalkTarget() {
    using namespace nK;
    Character *o;
    s16 res;
    s16 h;
    Unk_0200bc78_Vec d;
    VecFx32Ctor *pv;
    s16 *pr;
    o = (Character *)TalkRequest_GetTalkTarget();
    pr = 0;
    if (((PlayerActor *)(this))->testActionFlag(0xe)) o = 0;
    if (o != 0) {
        VecFx32Ctor *ov;
        pv = (VecFx32Ctor *)&((PlayerActor *)this)->position;
        ov = (VecFx32Ctor *)o->getInteractionPos();
        func_0200bc78_sub(&d, ov, pv);
        res = Math_Atan2(d.x, d.z);
        pr = &res;
    }
    if (pr != 0) {
        h = ((PlayerActor *)this)->rotY;
        PlayerActor_TurnAngle(&h, *pr);
        ((PlayerActor *)(this))->setAngleY((s16 *)(&h));
    }
}

void PlayerActor::act10CheckTalk() {
    using namespace nK;
    ProcBase *p = 0;
    u16 tmp;
    void *o;
    if (((PlayerActor *)this)->inputMode == 2) {
        p = interactTarget;
    } else if (((PlayerActor *)this)->inputMode == 1) {
        if (gPad.trig & 0x400) {
            p = (ProcBase *)TalkRequest_GetTalkTarget();
        }
    }
    if (p != 0) {
        tmp = p->param;
        o = NpcRegistry_FindByHandle(&tmp);
        if (o != 0) {
            if (((NpcActor *)o)->getTeachableEmotion() != -1) {
                requestAct15(5, -1);
            }
        }
    }
}

void PlayerActor::mainAct10() {
    using namespace nK;
    act10UpdateAnim();
    if (((CommManager *)(gCommManager))->isLocalSlot(((PlayerActor *)this)->sessionSlot)) {
        act10FaceTalkTarget();
        ((PlayerActor *)(this))->moveWithCollision();
        ((PlayerActor *)(this))->updateBodyCollider();
        act10CheckTalk();
    } else {
        if (((PlayerActor *)(this))->netFollowTransform() != 0) {
            ((PlayerActor *)(this))->moveWithCollision();
        }
        ((PlayerActor *)(this))->updateBodyCollider();
    }
}

BOOL PlayerActor::requestAct13(u32 a, u32 b) {
    using namespace nK;
    PlayerActionRequest m;
    BOOL r;
    if (((PlayerActor *)this)->animId != 0x12) return TRUE;
    m.assign(0x13, a, b);
    r = pushRequest(&m);
    return r;
}

void PlayerActor::setupAct13(PlayerActionRequest *item, u32 old) {
    using namespace nK;
    ((PlayerActor *)(this))->startAnimOnce(0x84, 3, 0);
}

s32 PlayerActor::netAct13(u32 v) {
    using namespace nK;
    return requestAct13(5, v);
}

void PlayerActor::act13CheckEnd() {
    using namespace nK;
    if (((AnimFrameCtrl *)(&(AnimFrameCtrl &)((PlayerActor *)this)->bodyModel))->isFinished()) {
        ((PlayerActor *)this)->actionPriority = ((Unk_02007694 *)(void *)(this))->getActionDonePriority(((PlayerActor *)this)->action);
        ((Unk_02007694 *)(this))->requestAct05(7, 5, -1);
    }
}

void PlayerActor::mainAct13() {
    using namespace nK;
    ((PlayerActor *)(this))->advanceAnim();
    if (!((CommManager *)(gCommManager))->isLocalSlot(((PlayerActor *)this)->sessionSlot)) {
        ((PlayerActor *)(this))->netFollowTransform();
    }
    ((PlayerActor *)(this))->updateBodyCollider();
    act13CheckEnd();
}

s32 PlayerActor::requestEmotion(u8 a, u8 b, u32 c, s16 d) {
    using namespace nK;
    PlayerActionRequest m;
    s32 r;
    m.assign(0x14, c, d);
    PlayerNetPickUpReachArgs *pl = &m.emotion;
    pl->unk_01 = a;
    pl->unk_00 = b;
    r = pushRequest(&m);
    return r;
}

void PlayerNetPickUpReachArgs::writeEmotionNet(u8 a, u8 b) {
    using namespace nK;
    unk_01 = a;
    unk_00 = b;
}

void PlayerNetPickUpReachArgs::readEmotionNet(u8 *a, u8 *b) {
    using namespace nK;
    *a = unk_01;
    *b = unk_00;
}

void PlayerActor::setupEmotion(PlayerActionRequest *item, u32 old) {
    using namespace nK;
    u8 *q = ((PlayerActionRequest *)item)->unk_0c_b;
    u8 a = q[1];
    u8 b = q[0];
    Unk_0200b908_Obj *p;
    PlayerActionWork *pp;
    ((PlayerNetPickUpReachArgs *)&((PlayerActor *)this)->netData)->writeEmotionNet(a, b);
    pp = (PlayerActionWork *)((PlayerActor *)this)->actionWorkRaw;
    pp->c0 = a;
    pp->c1 = b;
    p = (Unk_0200b908_Obj *)Emotion_GetEntry(a);
    ((PlayerActor *)this)->emotionEntry = p;
    ((PlayerActor *)this)->emotionFx.emotionId = a;
    ((NpcEmotionFx *)(&((PlayerActor *)this)->emotionFx))->startEntry((NpcEmotionPhase *)(p), 0);
    ((PlayerActor *)(this))->playAnim(0x12, 5, p->animPlayMode, 0x1000, 0, 5, p->animId);
}

s32 PlayerActor::netEmotion(s16 v) {
    using namespace nK;
    u8 a, b;
    ((PlayerNetPickUpReachArgs *)&((PlayerActor *)this)->netData)->readEmotionNet(&a, &b);
    return requestEmotion(a, b, 5, v);
}

void PlayerActor::endEmotion() {
    using namespace nK;
    ((NpcEmotionFx *)(&((PlayerActor *)this)->emotionFx))->stop();
}

void PlayerActor::emotionUpdateAnim() {
    using namespace nK;
    Unk_0200b908_Obj *p;
    u32 t = AnimSlotRef_GetAnimId(&((PlayerActor *)this)->bodyAnimSlot);
    ((NpcEmotionFx *)(&((PlayerActor *)this)->emotionFx))->update(&((PlayerActor *)this)->headTopPos, ((PlayerActor *)this)->rotY, t, (u32)(((PlayerActor *)this)->bodyModel.curFrame << 4) >> 16);
    ((PlayerActor *)(this))->advanceAnim();
    if (((AnimFrameCtrl *)(&(AnimFrameCtrl &)((PlayerActor *)this)->bodyModel))->isFinished()) {
        p = ((PlayerActor *)this)->emotionEntry;
        if (p->nextAnimId != 0x137) {
            ((PlayerActor *)(this))->playAnim(0x12, 0, 0, 0x1000, 0, 0, p->nextAnimId);
            ((NpcEmotionFx *)(&((PlayerActor *)this)->emotionFx))->startEntry((NpcEmotionPhase *)(p), 1);
        } else {
            requestAct10(data_020c61d0[((PlayerActionWork *)((PlayerActor *)this)->actionWorkRaw)->c0 - 1], 5, -1);
        }
    }
}

void PlayerActor::emotionCheckEnd() {
    using namespace nK;
    PlayerActionWork *p = (PlayerActionWork *)((PlayerActor *)this)->actionWorkRaw;
    u8 *q = &p->c1;
    if (*q != 0) {
        *q = *q - 1;
    }
    if (((PlayerActor *)this)->bodyModel.playMode == 0 && *q == 0) {
        ((Unk_02007694 *)(this))->requestAct05(data_020c61d0[p->c0 - 1], 5, -1);
    }
}

void PlayerActor::mainEmotion() {
    using namespace nK;
    emotionUpdateAnim();
    ((PlayerActor *)(this))->netFollowTransform();
    ((PlayerActor *)(this))->updateBodyCollider();
    emotionCheckEnd();
}

s32 PlayerActor::requestAct15(u32 a, u32 b) {
    using namespace nK;
    PlayerActionRequest m;
    s32 r;
    m.assign(0x15, a, b);
    r = pushRequest(&m);
    return r;
}

void PlayerActor::setupAct15(PlayerActionRequest *item, u32 old) {
    using namespace nK;
    ((PlayerActor *)(this))->startAnimOnce(0x9d, 3, 0);
}

s32 PlayerActor::netAct15(u32 v) {
    using namespace nK;
    return requestAct15(5, v);
}

void PlayerActor::act15UpdateAnim() {
    using namespace nK;
    ((PlayerActor *)(this))->advanceAnim();
    if (((AnimFrameCtrl *)(&(AnimFrameCtrl &)((PlayerActor *)this)->bodyModel))->hasPassedFrame(6)) {
        Effect_Create(0x50, &((PlayerActor *)this)->headTopPos, 0, 0);
        ((PlayerActor *)(this))->playSe(0x68);
    }
}

void PlayerActor::act15CheckEnd() {
    using namespace nK;
    if (((AnimFrameCtrl *)(&(AnimFrameCtrl &)((PlayerActor *)this)->bodyModel))->isFinished()) {
        requestAct10(5, 5, -1);
    }
}

void PlayerActor::mainAct15() {
    using namespace nK;
    act15UpdateAnim();
    ((PlayerActor *)(this))->updateBodyCollider();
    act15CheckEnd();
}

void PlayerPickUpReachArgs::setPickUpReachArgs(Unk_0200b750_Pair pr, u32 v) {
    using namespace nK;
    unitX = pr.unitX;
    unitZ = pr.unitZ;
    ftrActorIndex = v;
}

s32 PlayerActor::requestPickUpReach(Unk_0200b750_Pair pr, s32 a, u32 b, s16 c) {
    using namespace nK;
    PlayerActionRequest m;
    s32 r;
    m.assign(0x18, b, c);
    m.pickUpReach.setPickUpReachArgs(pr, a);
    r = pushRequest(&m);
    return r;
}

void PlayerNetPickUpReachArgs::writePickUpReachNet(Unk_0200b750_Pair pr, s32 v) {
    using namespace nK;
    unk_00 = pr.unitX;
    unk_01 = pr.unitZ;
    ftrActorIndex = v;
}

void PlayerNetPickUpReachArgs::readPickUpReachNet(Unk_0200b750_Pair *pr, s32 *out) {
    using namespace nK;
    pr->unitX = unk_00;
    pr->unitZ = unk_01;
    *out = ftrActorIndex;
}

void PlayerActor::setupPickUpReach(PlayerActionRequest *item, u32 old) {
    using namespace nK;
    u8 *q = ((PlayerActionRequest *)item)->unk_0c_b;
    u8 a = q[4];
    u8 b = q[5];
    s32 c = *(s32 *)((PlayerActionRequest *)item)->unk_0c_b;
    PlayerActionWork *p;
    ((PlayerActor *)(this))->clearActionFlag(0xd);
    p = (PlayerActionWork *)((PlayerActor *)this)->actionWorkRaw;
    p->w0 = c;
    p->b5 = a;
    p->b6 = b;
    p->b4 = 0;
    ((PlayerNetPickUpReachArgs *)&((PlayerActor *)this)->netData)->writePickUpReachNet(Unk_0200b750_Pair(a, b), c);
    if (c < 0) {
        FieldPos_FromUnitCenter(&((PlayerActor *)this)->shownItemPos, a, b);
        ((PlayerActor *)this)->shownItemScaleX = 0x1000;
        ((PlayerActor *)this)->shownItemScaleY = 0x1000;
        ((PlayerActor *)this)->shownItemScaleZ = 0x1000;
        ((PlayerActor *)this)->shownItem = *BlockMap_GetItemPtrAtPos(gSceneBlockMap, &((PlayerActor *)this)->shownItemPos, 0);
        ((PlayerActor *)this)->actionItem = ((PlayerActor *)this)->shownItem;
    }
    if (Pocket_FindEmpty() == -1 && Scene_InHouseRoom()) {
        BOOL f = FALSE;
        if (((PlayerActor *)this)->actionItem >= 0xa7 && ((PlayerActor *)this)->actionItem <= 0xc6) f = TRUE;
        if (!f) {
            ((PlayerActor *)(this))->startAnim(0, 3, 0);
            p->b4 = 3;
            goto end;
        }
    }
    if (c >= 0) {
        if (Room_CountOccupants() <= 1) {
            FtrMgr_RemoveActor(p->w0, &((PlayerActor *)this)->actionItem, &((PlayerActor *)this)->shownItem);
            ((PlayerActor *)(this))->startAnimOnce(0x15, 3, 0);
        } else {
            p->b4 = 7;
            ((PlayerActor *)(this))->startAnim(0, 3, 0);
        }
    } else if (c == -3) {
        p->b4 = 7;
        ((PlayerActor *)(this))->startAnim(0, 3, 0);
    } else {
        ((PlayerActor *)(this))->startAnim(0x15, 3, 0);
    }
end:
    if (((PlayerActor *)this)->animId == 0x15 && ((PlayerActor *)(this))->getHeldToolKind() == 4) {
        HeldItemModel_PlayAnim(&((PlayerActor *)this)->heldItemModel, 0xb, 3, 0);
    }
}

void PlayerActor::netPickUpReach(s16 v) {
    using namespace nK;
    BOOL f;
    if (gFieldSceneKind == 1) f = TRUE; else f = FALSE;
    if (!f) {
        if (((PlayerActor *)this)->action == 0x18) {
            netSeq = v;
        } else {
            Unk_0200b750_Pair pr(0, 0);
            s32 x;
            ((PlayerNetPickUpReachArgs *)&((PlayerActor *)this)->netData)->readPickUpReachNet(&pr, &x);
            if (x < 0) {
                requestPickUpReach(pr, -1, 6, v);
            }
        }
    }
}

void PlayerActor::pickUpReachWaitAnswer() {
    using namespace nK;
    PlayerActionWork *p = (PlayerActionWork *)((PlayerActor *)this)->actionWorkRaw;
    s32 a = p->w0;
    u32 b = p->b5;
    u32 c = p->b6;
    u32 d = p->b4;
    if (d >= 3) {
        ((PlayerActor *)(this))->advanceAnim();
    } else {
        if (a >= 0) {
            Vec2 pr = {(s32)b, (s32)c};
            requestPickUpAt(&pr, a, 0, 6, -1);
        } else if (d == 0) {
            if (((PlayerActor *)this)->fieldAnswer == 2) {
                p->b4 = 2;
            } else if (((PlayerActor *)this)->fieldAnswer == 1) {
                p->b4 = 1;
            }
        }
        ((PlayerActor *)(this))->advanceAnim();
    }
}

void PlayerActor::mainPickUpReach() {
    using namespace nJ;
    Vec2 p;
    if (((CommManager *)(gCommManager))->isLocalSlot(((PlayerActor *)this)->sessionSlot)) {
        ((PlayerActor *)(this))->pickUpReachWaitAnswer();
        ((PlayerActor *)(this))->updateBodyCollider();
        pickUpReachUpdate();
    } else {
        ((PlayerActor *)(this))->advanceAnim();
        PlayerPickUpWork* q = (PlayerPickUpWork *)((PlayerActor *)this)->actionWorkRaw;
        Vec2 t = { q->unk_04.b.unk_05, q->unk_04.b.unk_06 };
        p = t;
        ((PlayerActor *)(this))->netSyncNearUnit((s32 *)(void *)(&p));
        ((PlayerActor *)(this))->updateBodyCollider();
        ((PlayerActor *)this)->actionPriority = ((Unk_02007694 *)(void *)(this))->getActionDonePriority(((PlayerActor *)this)->action);
        pickUpReachUpdate();
    }
}

namespace nJ {
extern "C" void PlayerActor_SetArgsPickUp(PlayerPickUpArgs* out, Vec2* pos, s32 a, s32 b, u8 c) {
    out->unitX = pos->x;
    out->unitZ = pos->y;
    out->ftrActorIndex = a;
    out->item = b;
    out->commitUnit = c;
}
}

s32 PlayerActor::requestPickUpAt(Vec2* pos, s32 a, u8 b, s32 c, s16 d) {
    using namespace nJ;
    Vec2 p;
    s32 r;
    PlayerActionRequest obj;
    obj.assign(0x19, c, d);
    p.x = pos->x;
    p.y = pos->y;
    PlayerActor_SetArgsPickUp((PlayerPickUpArgs*)&obj.unk_0c, &p, a, 0xfff1, b);
    r = pushRequest(&obj);
    return r;
}

s32 PlayerActor::requestPickUpWithItem(Vec2* pos, u16 a, u8 b, s32 c, s16 d) {
    using namespace nJ;
    Vec2 p;
    s32 r;
    PlayerActionRequest obj;
    obj.assign(0x19, c, d);
    p.x = pos->x;
    p.y = pos->y;
    PlayerActor_SetArgsPickUp((PlayerPickUpArgs*)&obj.unk_0c, &p, -1, a, b);
    r = pushRequest(&obj);
    return r;
}

namespace nJ {
extern "C" void PlayerActor_NetWritePickUp(PlayerNetPickUpArgs* dst, Vec2* pos, s8 a, u16 b, u8 c) {
    dst->unitX = pos->x;
    dst->unitZ = pos->y;
    dst->ftrActorIndex = a;
    NetBuf_WriteU16(&dst->item, b);
    dst->commitUnit = c;
}
}

namespace nJ {
extern "C" void PlayerActor_NetReadPickUp(PlayerNetPickUpArgs* src, Vec2* pos, s8* a, u16* b, u8* c) {
    pos->x = src->unitX;
    pos->y = src->unitZ;
    *a = src->ftrActorIndex;
    *b = NetBuf_ReadU16(&src->item);
    *c = src->commitUnit;
}
}

void PlayerActor::setupPickUp(PlayerActionRequest* item, u32 old) {
    using namespace nJ;
    PlayerPickUpArgs* o = (PlayerPickUpArgs *)((PlayerActionRequest *)item)->unk_0c_b;
    u8 x = o->unitX;
    u8 y = o->unitZ;
    s32 z = o->ftrActorIndex;
    u16 t[4];
    t[0] = ((PlayerPickUpArgs *)((PlayerActionRequest *)item)->unk_0c_b)->item;
    u8 flag = o->commitUnit;
    if (z < 0) {
        BOOL r;
        FieldPos_FromUnitCenter(&shownItemPos, x, y);
        ((PlayerActor *)this)->shownItemScaleX = 0x1000;
        ((PlayerActor *)this)->shownItemScaleY = 0x1000;
        ((PlayerActor *)this)->shownItemScaleZ = 0x1000;
        r = Unk_0200add8_IsFFF1(&t[0], &t[2]);
        if (r) {
            ((PlayerActor *)this)->shownItem = *BlockMap_GetItemPtrAtPos(gSceneBlockMap, &shownItemPos, 0);
            ((PlayerActor *)this)->actionItem = ((PlayerActor *)this)->shownItem;
        } else {
            ((PlayerActor *)this)->shownItem = t[0];
            ((PlayerActor *)this)->actionItem = ((PlayerActor *)this)->shownItem;
        }
        if (((CommManager *)(gCommManager))->isLocalSlot(((PlayerActor *)this)->sessionSlot)) {
            void* q = PlayerData_GetCurrent();
            if (q != NULL) {
                if (Unk_0200add8_R1(&((PlayerActor *)this)->actionItem, 0x137b, 0x137b)) {
                    if (((PlayerData *)(q))->testFlag(0x28) == 0) {
                        Vec2 p;
                        ((PlayerData *)(q))->setFlag(0x28);
                        p.x = x;
                        p.y = y;
                        ((PlayerActor *)(this))->requestPickUpFanfareAt((Unk_02006d14_Pair *)(void *)(&p), flag, 6, -1);
                        return;
                    }
                } else if (((PlayerActor *)this)->actionItem >= 0x136a && ((PlayerActor *)this)->actionItem <= 0x136a) {
                    if (((PlayerData *)(q))->testFlag(0x27) == 0) {
                        Vec2 p;
                        ((PlayerData *)(q))->setFlag(0x27);
                        p.x = x;
                        p.y = y;
                        ((PlayerActor *)(this))->requestPickUpFanfareAt((Unk_02006d14_Pair *)(void *)(&p), flag, 6, -1);
                        return;
                    }
                }
            }
        }
        {
            BOOL s8 = 1;
            BOOL s7 = 1;
            BOOL s6 = 1;
            BOOL s5 = 1;
            BOOL s4 = 1;
            BOOL s3 = 1;
            BOOL s2 = 1;
            BOOL s1 = 0;
            u32 v = ((PlayerActor *)this)->actionItem;
            if (v <= 5) {
                s1 = 1;
            }
            if (!s1) {
                if (!Unk_0200add8_InRange(v, 6, 0xb)) {
                    s2 = 0;
                }
            }
            if (!s2) {
                if (!Unk_0200add8_InRange(v, 0xc, 0x11)) {
                    s3 = 0;
                }
            }
            if (!s3) {
                if (!Unk_0200add8_InRange(v, 0x12, 0x19) && v != 0x1c) {
                    s4 = 0;
                }
            }
            if (!s4) {
                if (!Unk_0200add8_InRange(v, 0x8a, 0x8f) && !Unk_0200add8_InRange(v, 0x90, 0x95) &&
                    !Unk_0200add8_InRange(v, 0x96, 0x9b) && !Unk_0200add8_InRange(v, 0x9c, 0xa3) && v != 0xa5) {
                    s5 = 0;
                }
            }
            if (!s5) {
                if (v != 0x1a) {
                    s6 = 0;
                }
            }
            if (!s6) {
                if (v != 0xa4) {
                    s7 = 0;
                }
            }
            if (!s7 && v != 0x1d) {
                s8 = 0;
            }
            if (s8) {
                ((PlayerActor *)(this))->playSe(0x7d7);
                ((PlayerActor *)(this))->setActionFlag(9);
            } else {
                ((PlayerActor *)(this))->playSe(0x5d);
                if (Unk_0200add8_R1(&((PlayerActor *)this)->actionItem, 0x1554, 0x155c)) {
                    VillagerTrend_NotifyUnk6(&position);
                }
            }
            ((PlayerActor *)(this))->setActionFlag(0xd);
        }
    }
    u8 b20;
    if (flag != 0 && !Unk_0200add8_IsFFF1(&((PlayerActor *)this)->actionItem, &t[3]) && !Unk_0200add8_R1(&((PlayerActor *)this)->actionItem, 0xa7, 0xc6)) {
        b20 = 1;
    } else {
        b20 = 0;
    }
    if (b20) {
        Vec2 p;
        p.x = x;
        p.y = y;
        PendingUnit_CommitAt(&p, 0);
    } else {
        Vec2 p;
        p.x = x;
        p.y = y;
        PendingUnit_ApplyAt(&p, 0);
    }
    {
        PlayerPickUpWork* s = (PlayerPickUpWork *)((PlayerActor *)this)->actionWorkRaw;
        Vec2 p;
        s->storeStep = 0;
        s->unk_04.s = z;
        s->pickUnitX = x;
        s->pickUnitZ = y;
        s->commitUnit = b20;
        u16 w = ((PlayerActor *)this)->actionItem;
        p.x = x;
        p.y = y;
        PlayerActor_NetWritePickUp((PlayerNetPickUpArgs *)netData.bytes, &p, (s8)z, w, b20);
    }
    if (z >= 0) {
        ((PlayerActor *)(this))->startAnimOnce(0x16, 6, 0);
    } else {
        ((PlayerActor *)(this))->startAnimOnce(0x16, 3, 0);
    }
    if (((PlayerActor *)(this))->getHeldToolKind() == 4) {
        HeldItemModel_PlayAnim(&((PlayerActor *)this)->heldItemModel, 0xc, 3, 1);
    }
}

void PlayerActor::netPickUp(s16 old) {
    using namespace nJ;
    Vec2 p;
    s8 a;
    u8 b;
    u16 c;
    Vec2 q;
    if (((PlayerActor *)this)->netPickUpDelay != 0) {
        ((PlayerActor *)this)->netPickUpDelay = ((PlayerActor *)this)->netPickUpDelay + 1;
    } else if (((PlayerActor *)this)->action == 0) {
        p.x = 0;
        p.y = 0;
        PlayerActor_NetReadPickUp((PlayerNetPickUpArgs *)netData.bytes, &p, &a, &c, &b);
        if (a < 0) {
            q.x = p.x;
            q.y = p.y;
            requestPickUpWithItem(&q, c, b, 6, old);
        }
    }
}

void PlayerActor::endPickUp() {
    using namespace nJ;
    ((PlayerActor *)(this))->clearActionFlag(0xd);
}

void PlayerActor::pickUpUpdateAnim(PlayerActionRequest* item, u32 old) {
    using namespace nJ;
    ((PlayerActor *)(this))->advanceAnim();
    if (((PlayerActor *)this)->animId == 0x16) {
        if (static_cast<AnimFrameCtrl &>(bodyModel).hasPassedFrame(9)) {
            ((PlayerActor *)(this))->playSe(0x4f);
        }
    }
}

void PlayerActor::pickUpUpdateItem() {
    using namespace nJ;
    PlayerPickUpWork* s;
    VecFx32 *r;
    s32 t;
    u32 v;
    s16 sh;
    if (((PlayerActor *)this)->animId != 0x16) {
        ((PlayerActor *)this)->shownItemScaleX = 0;
        ((PlayerActor *)this)->shownItemScaleY = 0;
        ((PlayerActor *)this)->shownItemScaleZ = 0;
        ((PlayerActor *)(this))->clearActionFlag(0xd);
        return;
    }
    s = (PlayerPickUpWork *)((PlayerActor *)this)->actionWorkRaw;
    if (s->unk_04.s >= 0) {
        r = FtrMgr_PollRemovedPos(s->unk_04.s);
        if (r == NULL) {
            return;
        }
        ((PlayerActor *)this)->shownItemPos.x = r->x;
        ((PlayerActor *)this)->shownItemPos.y = r->y;
        ((PlayerActor *)this)->shownItemPos.z = r->z;
        ((PlayerActor *)(this))->setActionFlag(0xd);
        ((PlayerActor *)this)->shownItemScaleX = 0x1000;
        ((PlayerActor *)this)->shownItemScaleY = 0x1000;
        ((PlayerActor *)this)->shownItemScaleZ = 0x1000;
        s->unk_04.s = -1;
    }
    v = ((u32)((PlayerActor *)this)->bodyModel.curFrame << 4) >> 16;
    if (v >= 6) {
        volatile Unk_02006d14_V3 sv;
        Mtx43 b1, b2;
        t = 0x1000 - _s32_div_f((v - 6) << 12, 12);
        if (t < 0) {
            t = 0;
            ((PlayerActor *)(this))->clearActionFlag(0xd);
        }
        ((PlayerActor *)this)->shownItemScaleX = t;
        ((PlayerActor *)this)->shownItemScaleY = t;
        ((PlayerActor *)this)->shownItemScaleZ = t;
        Unk_02006d14_V3 *pv = (Unk_02006d14_V3 *)&((PlayerActor *)this)->drawPos;
        sv.x = pv->x;
        sv.y = pv->y;
        sv.z = pv->z;
        sh = ((PlayerActor *)this)->drawTilt;
        b1 = *(Mtx43 *)&((PlayerActor *)this)->bodyModel.mtx;
        b2 = *(Mtx43 *)&((PlayerActor *)this)->itemHandMtx;
        ((PlayerActor *)(this))->calcHandMtx();
        ((PlayerActor *)(this))->updateShownItemPos(t);
        ((PlayerActor *)this)->drawPos.x = sv.x;
        ((PlayerActor *)this)->drawPos.y = sv.y;
        ((PlayerActor *)this)->drawPos.z = sv.z;
        ((PlayerActor *)this)->drawTilt = sh;
        *(Mtx43 *)&((PlayerActor *)this)->bodyModel.mtx = b1;
        *(Mtx43 *)&((PlayerActor *)this)->itemHandMtx = b2;
    }
}

void PlayerActor::pickUpRemoteCheckEnd() {
    using namespace nI;
    if (((AnimFrameCtrl *)(&(AnimFrameCtrl &)((PlayerActor *)this)->bodyModel))->isFinished()) {
        if (((PlayerActor *)this)->animId == 0x16 || ((PlayerActor *)this)->animId == 0x18) {
            ((PlayerActor *)(this))->startAnim(0x6c, 3, 0);
        }
        ((PlayerActor *)this)->actionPriority = ((Unk_02007694 *)(void *)(this))->getActionDonePriority(((PlayerActor *)this)->action);
        ((PlayerActor *)(this))->clearActionFlag(0x12);
        ((PlayerActor *)(this))->clearActionFlag(0xd);
        if (((PlayerActor *)(this))->getHeldToolKind() == 4) {
            HeldItemModel_PlayAnim(&((PlayerActor *)this)->heldItemModel, 0, 9, 0);
        }
    }
}

void PlayerActor::mainPickUp() {
    using namespace nI;
    ((PlayerActor *)(this))->pickUpUpdateAnim((PlayerActionRequest *)(0), 0); // unused by pickUpUpdateAnim
    if (gCommManager->isLocalSlot(((PlayerActor *)this)->sessionSlot)) {
        ((PlayerActor *)(this))->updateBodyCollider();
        ((PlayerActor *)(this))->submitSceneCollider();
        ((PlayerActor *)(this))->pickUpUpdateItem();
        PlayerPickUpWork* s = (PlayerPickUpWork *)((PlayerActor *)this)->actionWorkRaw;
        pickUpUpdateStore(&s->storeStep, s->commitUnit);
    } else {
        PlayerPickUpWork* s = (PlayerPickUpWork *)((PlayerActor *)this)->actionWorkRaw;
        u32 c = s->pickUnitZ;
        u32 a = s->pickUnitX;
        Unk_02006d14_Pair p;
        p.unitX = a;
        p.unitZ = c;
        ((PlayerActor *)(this))->netSyncNearUnit((s32 *)(void *)(&p));
        ((PlayerActor *)(this))->updateBodyCollider();
        ((PlayerActor *)(this))->pickUpUpdateItem();
        pickUpRemoteCheckEnd();
    }
}

namespace nI {
extern "C" void PlayerActor_SetArgsPickUpFanfare(PlayerPickUpFanfareArgs* s, Unk_02006d14_Pair* p, u16 h, u8 b) {
    s->unitX = p->unitX;
    s->unitZ = p->unitZ;
    s->item = h;
    s->commitUnit = b;
}
}

s32 PlayerActor::requestPickUpFanfareAt(Unk_02006d14_Pair* p, u8 b, u32 x, s16 y) {
    using namespace nI;
    PlayerActionRequest o;
    o.assign(0x1a, x, y);
    Unk_02006d14_Pair q;
    q = *p;
    PlayerActor_SetArgsPickUpFanfare(&o.pickUpFanfare, &q, 0xfff1, b);
    s32 r = pushRequest(&o);
    return r;
}

s32 PlayerActor::requestPickUpFanfareWithItem(Unk_02006d14_Pair* p, u16 h, u8 b, u32 x, s16 y) {
    using namespace nI;
    PlayerActionRequest o;
    o.assign(0x1a, x, y);
    Unk_02006d14_Pair q;
    q = *p;
    PlayerActor_SetArgsPickUpFanfare(&o.pickUpFanfare, &q, h, b);
    s32 r = pushRequest(&o);
    return r;
}

namespace nI {
extern "C" void PlayerActor_NetWritePickUpFanfare(PlayerNetPickUpFanfareArgs* s, Unk_02006d14_Pair* p, u16 h, u8 b) {
    s->unitX = p->unitX;
    s->unitZ = p->unitZ;
    NetBuf_WriteU16(s->item, h);
    s->commitUnit = b;
}
}

namespace nI {
extern "C" void PlayerActor_NetReadPickUpFanfare(PlayerNetPickUpFanfareArgs* s, Unk_02006d14_Pair* out, u16* h, u8* b) {
    out->unitX = s->unitX;
    out->unitZ = s->unitZ;
    *h = NetBuf_ReadU16(s->item);
    *b = s->commitUnit;
}
}

void PlayerActor::setupPickUpFanfare(PlayerActionRequest* item, u32 v) {
    using namespace nI;
    u8* it = ((PlayerActionRequest *)item)->unk_0c_b;
    u32 a = it[2];
    u32 c = it[3];
    u16 g[2];
    g[0] = *(u16*)it;
    u8 d = it[4];
    PlayerPickUpWork* s = (PlayerPickUpWork *)((PlayerActor *)this)->actionWorkRaw;
    s->fanfareStep = 0;
    s->unitX = a;
    s->unitZ = c;
    s->commitUnit = d;
    FieldPos_FromUnitCenter(&((PlayerActor *)this)->shownItemPos, a, c);
    ((PlayerActor *)this)->shownItemScaleX = 0x1000;
    ((PlayerActor *)this)->shownItemScaleY = 0x1000;
    ((PlayerActor *)this)->shownItemScaleZ = 0x1000;
    BOOL r;
    if (Item_IsFurniture(&g[0])) {
        g[1] = 0xfff1;
        s32 x = Item_GetFurnitureIndex(&g[0]);
        r = x == Item_GetFurnitureIndex(&g[1]);
    } else {
        r = g[0] == 0xfff1;
    }
    if (r) {
        u16* pp = (u16*)BlockMap_GetItemPtrAtPos(gSceneBlockMap, &((PlayerActor *)this)->shownItemPos, 0);
        ((PlayerActor *)this)->shownItem = *pp;
        ((PlayerActor *)this)->actionItem = ((PlayerActor *)this)->shownItem;
    } else {
        ((PlayerActor *)this)->shownItem = g[0];
        ((PlayerActor *)this)->actionItem = ((PlayerActor *)this)->shownItem;
    }
    u16 hv = ((PlayerActor *)this)->actionItem;
    Unk_02006d14_Pair p;
    p.unitX = a;
    p.unitZ = c;
    PlayerActor_NetWritePickUpFanfare((PlayerNetPickUpFanfareArgs *)&((PlayerActor *)this)->netData, &p, hv, d);
    ((PlayerActor *)(this))->startAnimOnce(0x17, 3, 0);
    if (((PlayerActor *)(this))->getHeldToolKind() == 4) {
        HeldItemModel_PlayAnim(&((PlayerActor *)this)->heldItemModel, 0xd, 3, 1);
    }
    if (gCommManager->isLocalSlot(((PlayerActor *)this)->sessionSlot)) {
        Bgm_RequestSilence(0x12, 0xf, 0);
    }
    ((PlayerActor *)(this))->setActionFlag(0x1c);
}

void PlayerActor::netPickUpFanfare(s16 v) {
    using namespace nI;
    if (((PlayerActor *)this)->netPickUpDelay != 0) {
        ((PlayerActor *)this)->netPickUpDelay++;
    } else if (((PlayerActor *)this)->action == 0x1a) {
        ((PlayerActor *)this)->netSeq = v;
    } else {
        Unk_02006d14_Pair p;
        p.unitX = 0;
        p.unitZ = 0;
        u16 hh;
        u8 bb;
        PlayerActor_NetReadPickUpFanfare((PlayerNetPickUpFanfareArgs *)&((PlayerActor *)this)->netData, &p, &hh, &bb);
        Unk_02006d14_Pair q;
        q = p;
        requestPickUpFanfareWithItem(&q, hh, bb, 6, v);
    }
}

void PlayerActor::endPickUpFanfare() {
    using namespace nI;
    if (!gCommManager->isLocalSlot(((PlayerActor *)this)->sessionSlot)) {
        if (((PlayerActor *)(this))->testActionFlag(0x1c)) {
            pickUpFanfareTakeItem();
        }
    }
}

void PlayerActor::pickUpFanfareUpdateItemPos() {
    using namespace nI;
    volatile u32 t[3];
    Unk_02006d14_Trip* pt = (Unk_02006d14_Trip *)&((PlayerActor *)this)->drawPos;
    t[0] = pt->x;
    t[1] = pt->y;
    t[2] = pt->z;
    s16 h = ((PlayerActor *)this)->drawTilt;
    Mtx43 a = bodyModel.mtx;
    Mtx43 b = itemHandMtx;
    ((PlayerActor *)(this))->calcHandMtx();
    ((PlayerActor *)(this))->updateShownItemPos(0x1000);
    ((PlayerActor *)this)->drawPos.x = t[0];
    ((PlayerActor *)this)->drawPos.y = t[1];
    ((PlayerActor *)this)->drawPos.z = t[2];
    ((PlayerActor *)this)->drawTilt = h;
    bodyModel.mtx = a;
    itemHandMtx = b;
    if ((((u32)((PlayerActor *)this)->bodyModel.curFrame << 4) >> 16) >= 6) {
        ((PlayerActor *)(this))->turnToCamera(0x400);
    }
}

void PlayerActor::pickUpFanfareUpdate() {
    using namespace nI;
    PlayerPickUpWork* s = (PlayerPickUpWork *)((PlayerActor *)this)->actionWorkRaw;
    u8* state = &s->fanfareStep;
    struct { u32 pad; Vec2 p[2]; Unk_02006d14_Pair stow; u32 pad2; } l;
    switch (*state) {
    case 0:
        if (((AnimFrameCtrl *)(&(AnimFrameCtrl &)((PlayerActor *)this)->bodyModel))->hasPassedFrame(1)) {
            Camera_SetMode4();
        }
        if (((AnimFrameCtrl *)(&(AnimFrameCtrl &)((PlayerActor *)this)->bodyModel))->hasPassedFrame(6)) {
            u32 b = s->unitX;
            u32 c = s->unitZ;
            u8 flag = s->commitUnit;
            if (flag) {
                l.p[0].x = b;
                l.p[0].y = c;
                PendingUnit_CommitAt(&l.p[0], 0);
            } else {
                l.p[1].x = b;
                l.p[1].y = c;
                PendingUnit_ApplyAt(&l.p[1], 0);
            }
            ((PlayerActor *)(this))->clearActionFlag(0x1c);
            if (Unk_0200a114_IsZero(&gFieldSceneKind)) {
                ((PlayerActor *)(this))->playSe(0x5d);
            }
            ((PlayerActor *)(this))->setActionFlag(0xd);
        }
        if ((((u32)((PlayerActor *)this)->bodyModel.curFrame << 4) >> 16) >= 6 && TalkRequest_AddPlayerMessage()) {
            *state = 1;
            ((Character *)(this))->attachTalkRequest((PlayerActor *)this);
            ((PlayerActor *)(this))->setActionFlag(0x11);
            static_cast<TalkMsgRequest *>(this)->setFileName(sPlayerActorMsgFile);
            BOOL r = FALSE;
            u16 h = ((PlayerActor *)this)->actionItem;
            if (h >= 0x137b && h <= 0x137b) r = TRUE;
            if (r) {
                ((PlayerActor *)this)->msgIndex = 0x23;
            } else if (h >= 0x136a && h <= 0x136a) {
                ((PlayerActor *)this)->msgIndex = 0x21;
            } else {
                ((PlayerActor *)this)->msgIndex = 0x25;
            }
            ((PlayerActor *)this)->window->nextState = 1;
            Bgm_ReleasePriority(0x12);
            Bgm_RequestSilence(0xc, 0, 0x10);
            Bgm_Request(0xd, 0x39, 0x7f, 1);
            ((PlayerActor *)this)->msgStep = 10;
        }
        break;
    case 1:
        if (((PlayerActor *)this)->window && ((PlayerActor *)this)->window->state) {
            *state = 2;
        }
        break;
    case 2:
        if (((PlayerActor *)this)->window && !((PlayerActor *)this)->window->state) {
            ((Character *)(this))->detachTalkRequest((PlayerActor *)this);
            ((PlayerActor *)(this))->clearActionFlag(0x11);
            u8 flag = s->commitUnit;
            if (flag == 0) {
                TalkRequest_FinishPlayerMessage();
            }
            Camera_SetModeDefault();
            u32 b = s->unitX;
            u32 c = s->unitZ;
            ((PlayerActor *)this)->actionPriority = ((Unk_02007694 *)(void *)(this))->getActionDonePriority(((PlayerActor *)this)->action);
            l.stow.unitX = b;
            l.stow.unitZ = c;
            requestPickUpFanfareStow(&l.stow, flag, 6, -1);
        }
        break;
    }
}

void PlayerActor::pickUpFanfareNetTake() {
    using namespace nI;
    if (((AnimFrameCtrl *)(&(AnimFrameCtrl &)((PlayerActor *)this)->bodyModel))->hasPassedFrame(6)) {
        pickUpFanfareTakeItem();
    }
}

void PlayerActor::pickUpFanfareTakeItem() {
    using namespace nI;
    ((PlayerActor *)(this))->setActionFlag(0xd);
    PlayerPickUpWork* s = (PlayerPickUpWork *)((PlayerActor *)this)->actionWorkRaw;
    u32 b = s->unitX;
    u32 c = s->unitZ;
    u8 flag = s->commitUnit;
    if (flag) {
        Vec2 p;
        p.x = b;
        p.y = c;
        PendingUnit_CommitAt(&p, 0);
    } else {
        Vec2 p;
        p.x = b;
        p.y = c;
        PendingUnit_ApplyAt(&p, 0);
    }
    ((PlayerActor *)(this))->clearActionFlag(0x1c);
    if (Unk_0200a114_IsZero(&gFieldSceneKind)) {
        ((PlayerActor *)(this))->playSe(0x5d);
    }
    ((PlayerActor *)this)->actionPriority = ((Unk_02007694 *)(void *)(this))->getActionDonePriority(((PlayerActor *)this)->action);
    ((PlayerActor *)(this))->clearActionFlag(0x12);
}

void PlayerActor::mainPickUpFanfare() {
    using namespace nI;
    ((PlayerActor *)(this))->advanceAnim();
    if (gCommManager->isLocalSlot(((PlayerActor *)this)->sessionSlot)) {
        ((PlayerActor *)(this))->updateBodyCollider();
        pickUpFanfareUpdateItemPos();
        pickUpFanfareUpdate();
    } else {
        PlayerPickUpWork* s = (PlayerPickUpWork *)((PlayerActor *)this)->actionWorkRaw;
        u32 c = s->unitZ;
        u32 a = s->unitX;
        Unk_02006d14_Pair p;
        p.unitX = a;
        p.unitZ = c;
        ((PlayerActor *)(this))->netSyncNearUnit((s32 *)(void *)(&p));
        ((PlayerActor *)(this))->updateBodyCollider();
        pickUpFanfareUpdateItemPos();
        pickUpFanfareNetTake();
    }
}

namespace nI {
extern "C" void PlayerActor_SetArgsPickUpFanfareStow(PlayerPickUpFanfareStowArgs* dst, Unk_02006d14_Pair* p, u8 b) {
    dst->unitX = p->unitX;
    dst->unitZ = p->unitZ;
    dst->commitUnit = b;
}
}

s32 PlayerActor::requestPickUpFanfareStow(Unk_02006d14_Pair* p, u8 b, u32 x, s16 y) {
    using namespace nI;
    PlayerActionRequest o;
    o.assign(0x1b, x, y);
    Unk_02006d14_Pair q;
    q = *p;
    PlayerActor_SetArgsPickUpFanfareStow(&o.pickUpFanfareStow, &q, b);
    s32 r = pushRequest(&o);
    return r;
}

namespace nI {
extern "C" void PlayerActor_NetWritePickUpFanfareStow(PlayerNetPickUpFanfareStowArgs* dst, Unk_02006d14_Pair* p, u8 b) {
    dst->unitX = p->unitX;
    dst->unitZ = p->unitZ;
    dst->commitUnit = b;
}
}

namespace nI {
extern "C" void PlayerActor_NetReadPickUpFanfareStow(PlayerNetPickUpFanfareStowArgs* src, Unk_02006d14_Pair* out, u8* b) {
    out->unitX = src->unitX;
    out->unitZ = src->unitZ;
    *b = src->commitUnit;
}
}

void PlayerActor::setupPickUpFanfareStow(PlayerActionRequest* item, u32 v) {
    using namespace nI;
    PlayerNetPickUpFanfareStowArgs* it = (PlayerNetPickUpFanfareStowArgs *)((PlayerActionRequest *)item)->unk_0c_b;
    u32 a = it->unitX;
    u32 c = it->unitZ;
    u8 b = it->commitUnit;
    PlayerPickUpWork* s = (PlayerPickUpWork *)((PlayerActor *)this)->actionWorkRaw;
    s->fanfareStep = 0;
    s->unitX = a;
    s->unitZ = c;
    s->commitUnit = b;
    Unk_02006d14_Pair p;
    p.unitX = a;
    p.unitZ = c;
    PlayerActor_NetWritePickUpFanfareStow((PlayerNetPickUpFanfareStowArgs*)(PlayerNetPickUpFanfareArgs *)&((PlayerActor *)this)->netData, &p, b);
    ((PlayerActor *)this)->shownItemScaleX = 0x1000;
    ((PlayerActor *)this)->shownItemScaleY = 0x1000;
    ((PlayerActor *)this)->shownItemScaleZ = 0x1000;
    ((PlayerActor *)(this))->playSe(0x4f);
    ((PlayerActor *)(this))->startAnimOnce(0x18, 3, 0);
    if (((PlayerActor *)(this))->getHeldToolKind() == 4) {
        HeldItemModel_PlayAnim(&((PlayerActor *)this)->heldItemModel, 6, 3, 1);
    }
}

void PlayerActor::netPickUpFanfareStow(s16 v) {
    using namespace nI;
    Unk_02006d14_Pair p;
    u8 b;
    p.unitX = 0;
    p.unitZ = 0;
    PlayerActor_NetReadPickUpFanfareStow((PlayerNetPickUpFanfareStowArgs*)(PlayerNetPickUpFanfareArgs *)&((PlayerActor *)this)->netData, &p, &b);
    Unk_02006d14_Pair q;
    q = p;
    requestPickUpFanfareStow(&q, b, 6, v);
}

void PlayerActor::pickUpFanfareStowShrink() {
    using namespace nH;
    u32 x;
    s32 t;
    if (((PlayerActor *)this)->animId != 0x18) {
        ((PlayerActor *)this)->shownItemScaleX = 0;
        ((PlayerActor *)this)->shownItemScaleY = 0;
        ((PlayerActor *)this)->shownItemScaleZ = 0;
        ((PlayerActor *)(this))->clearActionFlag(0xd);
    } else {
        x = ((u32)((PlayerActor *)this)->bodyModel.curFrame << 4) >> 16;
        if (x < 6) {
            t = 0x1000 - _s32_div_f(x << 12, 6);
            if (t < 0) {
                t = 0;
                ((PlayerActor *)(this))->clearActionFlag(0xd);
            }
            ((PlayerActor *)this)->shownItemScaleX = t;
            ((PlayerActor *)this)->shownItemScaleY = t;
            ((PlayerActor *)this)->shownItemScaleZ = t;
            ((PlayerActor *)(this))->updateShownItemPos(t);
        } else {
            ((PlayerActor *)this)->shownItemScaleX = 0;
            ((PlayerActor *)this)->shownItemScaleY = 0;
            ((PlayerActor *)this)->shownItemScaleZ = 0;
        }
    }
}

void PlayerActor::mainPickUpFanfareStow() {
    using namespace nH;
    ((PlayerActor *)(this))->advanceAnim();
    if (gCommManager->isLocalSlot(((PlayerActor *)this)->sessionSlot)) {
        ((PlayerActor *)(this))->updateBodyCollider();
        pickUpFanfareStowShrink();
        u8* p = (u8*)(PlayerChangeHeldItemWork *)((PlayerActor *)this)->actionWorkRaw;
        ((PlayerActor *)(this))->pickUpUpdateStore(p + 1, p[0]);
    } else {
        u8* p = (u8*)(PlayerChangeHeldItemWork *)((PlayerActor *)this)->actionWorkRaw;
        u32 v[2];
        u32 hi = p[3];
        u32 lo = p[2];
        v[0] = lo;
        v[1] = hi;
        ((PlayerActor *)(this))->netSyncNearUnit((s32 *)(void *)(v));
        ((PlayerActor *)(this))->updateBodyCollider();
        pickUpFanfareStowShrink();
        ((PlayerActor *)(this))->pickUpRemoteCheckEnd();
    }
}

namespace nH {
extern "C" void PlayerActor_SetArgsAct30(PlayerAct30Args* p, u32 a, u32 b, u32 c, Character *d) {
    p->kind = a;
    p->nextMode = b;
    p->variant = c;
    p->partner = d;
}
}

s32 PlayerActor::requestAct30(u16* p, u32 b, u32 c, u32 d, Character *e, u32 f, s16 g) {
    using namespace nH;
    PlayerActionRequest obj;
    s32 r;
    obj.assign(0x30, f, g);
    ((PlayerActor *)this)->shownItem = *p;
    ((PlayerActor *)this)->actionItem = ((PlayerActor *)this)->shownItem;
    PlayerActor_SetArgsAct30(&obj.act30, b, c, d, e);
    r = pushRequest(&obj);
    return r;
}

void PlayerActor::setupAct30(PlayerActionRequest* item, u32 old) {
    using namespace nH;
    PlayerAct30Args* s = (PlayerAct30Args *)((PlayerActionRequest *)item)->unk_0c_b;
    HandOverItem_Begin(&((PlayerActor *)this)->actionItem, ((PlayerAct30Args *)((PlayerActionRequest *)item)->unk_0c_b)->kind, s->nextMode, s->variant, ((PlayerActor *)this), s->partner);
    ((PlayerActor *)(this))->startAnimOnce(0x28, 3, 0);
    HandOverItem_RequestMode(1, ((PlayerActor *)this));
    ((PlayerActor *)(this))->playSe(0x4f);
    if (((PlayerActor *)this)->actionItem == 0x1373 || ((PlayerActor *)this)->actionItem == 0x1375 || ((PlayerActor *)this)->actionItem == 0x1377 || ((PlayerActor *)this)->actionItem == 0x1379 || ((PlayerActor *)this)->actionItem == 0x136a || ((PlayerActor *)this)->actionItem == 0x137b) {
        ((PlayerActor *)this)->pendingAct76Kind = -1;
    } else {
        ((PlayerActor *)this)->pendingAct76Kind = 0;
    }
}

void PlayerActor::netAct30() {
    using namespace nH;
}

void PlayerActor::act30UpdateAnim() {
    using namespace nH;
    if (((AnimFrameCtrl *)(&(AnimFrameCtrl &)((PlayerActor *)this)->bodyModel))->hasPassedFrame(8)) {
        ((PlayerActor *)(this))->playSe(0x63);
    }
    ((PlayerActor *)(this))->advanceAnim();
}

void PlayerActor::act30CheckEnd() {
    using namespace nH;
    if (((AnimFrameCtrl *)(&(AnimFrameCtrl &)((PlayerActor *)this)->bodyModel))->isFinished()) {
        requestAct31(6, -1);
    }
}

void PlayerActor::mainAct30() {
    using namespace nH;
    act30UpdateAnim();
    ((PlayerActor *)(this))->updateBodyCollider();
    act30CheckEnd();
}

s32 PlayerActor::requestAct31(u32 a, u32 b) {
    using namespace nH;      PlayerActionRequest obj;      s32 r;      obj.assign(0x31, a, b);      r = pushRequest(&obj);      return r;  }

void PlayerActor::setupAct31(PlayerActionRequest* item, u32 old) {
    using namespace nH;
    ((PlayerActor *)(this))->startAnim(0x29, 3, 0);
    HandOverItem_RequestMode(2, ((PlayerActor *)this));
}

void PlayerActor::netAct31() {
    using namespace nH;
}

void PlayerActor::act31CheckEnd() {
    using namespace nH;
    if (HandOverItem_IsMaster(((PlayerActor *)this)) == 0) {
        ((PlayerActor *)this)->actionPriority = ((Unk_02007694 *)(void *)(this))->getActionDonePriority(((PlayerActor *)this)->action);
        ((PlayerActor *)(this))->requestAct10(7, 5, -1);
    }
}

void PlayerActor::mainAct31() {
    using namespace nH;
    ((PlayerActor *)(this))->advanceAnim();
    ((PlayerActor *)(this))->updateBodyCollider();
    act31CheckEnd();
}

s32 PlayerActor::requestAct32(u32 a, u32 b) {
    using namespace nH;      PlayerActionRequest obj;      s32 r;      obj.assign(0x32, a, b);      r = pushRequest(&obj);      return r;  }

namespace nH {
extern "C" void PlayerActor_InitAct32Work(u32* p) {
    *p = 0;
}
}

void PlayerActor::setupAct32(PlayerActionRequest* item, u32 old) {
    using namespace nH;
    ((PlayerActor *)(this))->startAnim(1, 3, 0);
    PlayerActor_InitAct32Work((u32*)(PlayerChangeHeldItemWork *)((PlayerActor *)this)->actionWorkRaw);
}

s32 PlayerActor::netAct32(u32 a) {
    using namespace nH;
    return requestAct32(6, a);
}

void PlayerActor::act32UpdateSpeed() {
    using namespace nH;
    u32* r4 = (u32 *)actionWorkRaw;
    s16 ang[3];
    s32 dist;
    PlayerAct32Locals L;
    VecFx32* pv = (VecFx32 *)&((PlayerActor *)this)->position;
    L.cur = *pv;
    ang[2] = ((PlayerActor *)this)->rotY;
    if (gCommManager->isLocalSlot(((PlayerActor *)this)->sessionSlot)) {
        if (!HandOverItem_GetPos(&L.pos)) {
            return;
        }
        L.diff.x = L.pos.x - ((PlayerActor *)this)->position.x;
        L.diff.z = L.pos.z - ((PlayerActor *)this)->position.z;
        if (HandOverItem_CanTake(((PlayerActor *)this))) {
            *r4 = 0;
        } else {
            *r4 = PlayerActor_Accelerate(*r4, 0x333);
        }
        ang[1] = Math_Atan2(L.diff.x, L.diff.z);
    } else {
        if (!((PlayerActor *)(this))->getRemoteTransform((u8 *)(void *)(ang), (s32 *)(void *)(&L.pos), &L.pos.z, &ang[1])) {
            ((PlayerActor *)(((PlayerActor *)this)))->setSpeed(&data_020d5e4c);
            return;
        }
        if (L.pos.x != ((PlayerActor *)this)->position.x || L.pos.z != ((PlayerActor *)this)->position.z) {
            s32 t;
            L.diff.x = L.pos.x - ((PlayerActor *)this)->position.x;
            L.diff.z = L.pos.z - ((PlayerActor *)this)->position.z;
            *r4 = PlayerActor_Accelerate(*r4, 0x333);
            t = Math_Atan2(L.diff.x, L.diff.z);
            if (Math_AngleDiffAbs(t, ang[2]) >= 0x4000) {
                *r4 = 0;
            } else {
                ang[1] = t;
            }
        } else {
            *r4 = 0;
        }
    }
    if (*r4 != 0) {
        PlayerActor_TurnAngle(&ang[2], ang[1]);
        ((PlayerActor *)(((PlayerActor *)this)))->setAngleY(&ang[2]);
    }
    s32 t2 = func_01ffcb0c(*r4, data_02135f44[(((u16)(s16)(ang[2] - ang[1])) >> 4) * 2 + 1]);
    if (t2 < 0) {
        t2 = -t2;
    }
    dist = t2;
    ((PlayerActor *)(((PlayerActor *)this)))->setSpeed((u32 *)(&dist));
}

void PlayerActor::act32UpdateAnim() {
    using namespace nH;
    s32 r = func_01ffcb0c(((PlayerActor *)this)->speed, 0x3ae1);
    if (r <= (s32)((PlayerActor *)this)->bodyModel.numFrames) {
        ((PlayerActor *)this)->bodyModel.frameStep = r;
    }
    ((TwoLayerAnimModel *)(&((PlayerActor *)this)->bodyModel))->updateLayers();
    ((PlayerActor *)(this))->updateFootstepFx();
}

void PlayerActor::act32CheckEnd() {
    using namespace nH;
    if (gCommManager->isLocalSlot(((PlayerActor *)this)->sessionSlot)) {
        if (((PlayerActor *)this)->speed == 0) {
            requestAct33(6, -1);
        }
    } else {
        ((PlayerActor *)this)->actionPriority = 0;
        if (((PlayerActor *)this)->speed == 0) {
            ((PlayerActor *)(this))->requestAct10(3, 5, -1);
        }
    }
}

void PlayerActor::mainAct32() {
    using namespace nH;
    act32UpdateSpeed();
    act32UpdateAnim();
    if (gCommManager->isLocalSlot(((PlayerActor *)this)->sessionSlot)) {
        ((PlayerActor *)(this))->moveWithCollision();
    } else {
        ((PlayerActor *)(this))->moveNoCollision();
    }
    ((PlayerActor *)(this))->updateBodyCollider();
    act32CheckEnd();
}

s32 PlayerActor::requestAct33(u32 a, u32 b) {
    using namespace nH;      PlayerActionRequest obj;      s32 r;      obj.assign(0x33, a, b);      r = pushRequest(&obj);      return r;  }

void PlayerActor::setupAct33(PlayerActionRequest* item, u32 old) {
    using namespace nH;
    ((PlayerActor *)(this))->startAnimOnce(0x2a, 3, 0);
}

void PlayerActor::netAct33() {
    using namespace nH;
}

void PlayerActor::act33CheckEnd() {
    using namespace nH;
    if (((AnimFrameCtrl *)(&(AnimFrameCtrl &)((PlayerActor *)this)->bodyModel))->isFinished()) {
        if (HandOverItem_IsMaster(((PlayerActor *)this)) == 0) {
            if (HandOverItem_SwitchMaster(((PlayerActor *)this)) != 1) {
                return;
            }
            HandOverItem_RequestMode(3, ((PlayerActor *)this));
        }
        if (HandOverItem_IsModeActive(3) == 0) {
            requestAct34(6, -1);
        }
    }
}

void PlayerActor::mainAct33() {
    using namespace nH;
    ((PlayerActor *)(this))->advanceAnim();
    ((PlayerActor *)(this))->updateBodyCollider();
    act33CheckEnd();
}

s32 PlayerActor::requestAct34(u32 a, u32 b) {
    using namespace nH;      PlayerActionRequest obj;      s32 r;      obj.assign(0x34, a, b);      r = pushRequest(&obj);      return r;  }

void PlayerActor::setupAct34(PlayerActionRequest* item, u32 old) {
    using namespace nH;
    ((PlayerActor *)(this))->startAnimOnce(0x2b, 3, 0);
    HandOverItem_RequestMode(4, ((PlayerActor *)this));
}

void PlayerActor::netAct34() {
    using namespace nH;
}

void PlayerActor::act34CheckEnd() {
    using namespace nH;
    if (((AnimFrameCtrl *)(&(AnimFrameCtrl &)((PlayerActor *)this)->bodyModel))->isFinished()) {
        if (HandOverItem_GetNextMode() == 5) {
            requestAct35(6, -1);
        }
    }
}

void PlayerActor::mainAct34() {
    using namespace nH;
    ((PlayerActor *)(this))->advanceAnim();
    ((PlayerActor *)(this))->updateBodyCollider();
    act34CheckEnd();
}

s32 PlayerActor::requestAct35(u32 a, u32 b) {
    using namespace nH;      PlayerActionRequest obj;      s32 r;      obj.assign(0x35, a, b);      r = pushRequest(&obj);      return r;  }

void PlayerActor::setupAct35(PlayerActionRequest* item, u32 old) {
    using namespace nH;
    ((PlayerActor *)(this))->startAnimOnce(0x2c, 3, 0);
    HandOverItem_RequestMode(HandOverItem_GetNextMode(), ((PlayerActor *)this));
    ((PlayerActor *)(this))->playSe(0x4f);
}

void PlayerActor::netAct35() {
    using namespace nH;
}

void PlayerActor::act35CheckEnd() {
    using namespace nH;
    u16 v[2];
    if (((AnimFrameCtrl *)(&(AnimFrameCtrl &)((PlayerActor *)this)->bodyModel))->isFinished()) {
        ((PlayerActor *)this)->actionPriority = ((Unk_02007694 *)(void *)(this))->getActionDonePriority(((PlayerActor *)this)->action);
        ((PlayerActor *)(this))->requestAct10(3, 5, -1);
        if (((PlayerActor *)this)->pendingAct76Kind >= 0) {
            HandOverItem_GetItem(v);
            switch (v[0]) {
            case 0x1373:
                ((PlayerActor *)this)->pendingAct76Kind = 3;
                break;
            case 0x1375:
                ((PlayerActor *)this)->pendingAct76Kind = 2;
                break;
            case 0x1377:
                ((PlayerActor *)this)->pendingAct76Kind = 1;
                break;
            }
        }
        HandOverItem_End(((PlayerActor *)this));
    }
}

void PlayerActor::mainAct35() {
    using namespace nH;
    ((PlayerActor *)(this))->advanceAnim();
    ((PlayerActor *)(this))->updateBodyCollider();
    act35CheckEnd();
}

s32 PlayerActor::requestChangeHeldItem(u16 a, u32 b, u32 c) {
    using namespace nH;
    PlayerActionRequest obj;
    s32 r;
    obj.assign(0x3f, b, c);
    obj.unk_0c_h = a;
    r = pushRequest(&obj);
    return r;
}

namespace nH {
extern "C" void PlayerActor_NetWriteChangeHeldItem(void* p, u32 v) {
    NetBuf_WriteU16(p, v);
}
}

namespace nH {
extern "C" void PlayerActor_NetReadChangeHeldItem(void* p, u16* out) {
    *out = NetBuf_ReadU16(p);
}
}

void PlayerActor::setupChangeHeldItem(PlayerActionRequest* item, u32 old) {
    using namespace nH;
    u16* q = (u16*)((PlayerActionRequest *)item)->unk_0c_b;
    if (Unk_02009624_Check()) {
        u16 buf[2];
        PlayerActor_GetHeldItem(buf, ((PlayerActor *)this));
        void* r = PlayerActor_GetPlayerData(this);
        buf[1] = 0xfff1;
        ((PlayerData *)(r))->setHeldItem((u16 *)(&buf[1]));
        ((PlayerActor *)(this))->startAnimOnce(0x13, 5, 5);
        ((PlayerData *)(r))->setHeldItem((u16 *)(buf));
    }
    PlayerChangeHeldItemWork* d = (PlayerChangeHeldItemWork *)((PlayerActor *)this)->actionWorkRaw;
    d->unk_00 = 0x1000;
    d->item = *q;
    if (old == 0x10) {
        d->fromAct10 = 1;
    } else {
        d->fromAct10 = 0;
    }
    if (d->item != 0xfff1) {
        ((PlayerActor *)(this))->playSe(0x31);
    } else {
        ((PlayerActor *)(this))->playSe(0x78);
    }
    PlayerActor_NetWriteChangeHeldItem(&((PlayerActor *)this)->netData, *q);
}

s32 PlayerActor::netChangeHeldItem(u32 a) {
    using namespace nH;
    u16 v;
    PlayerActor_NetReadChangeHeldItem(&((PlayerActor *)this)->netData, &v);
    return requestChangeHeldItem(v, 6, a);
}

void PlayerActor::endChangeHeldItem() {
    using namespace nG;
    if (((CommManager *)(gCommManager))->isLocalSlot(((PlayerActor *)this)->sessionSlot)) {
        u16 *p = PlayerData_GetBySessionSlot(((PlayerActor *)this)->sessionSlot)->getHeldItem();
        if (p) (((PlayerActor *)this))->netSendClothesChange(3, *p);
    }
}

void PlayerActor::changeHeldItemUpdate() {
    using namespace nG;
    PlayerActionWork *r6 = (PlayerActionWork *)((PlayerActor *)this)->actionWorkRaw;
    u16 r4 = r6->h4;
    BOOL b = (gFieldSceneKind == 1);
    if (b) {
        if (r4 >= 0x1369 && r4 <= 0x13a7) {
            PlayerActor_SetHoldableItem(r4 - 0x1368, ((PlayerActor *)this)->sessionSlot);
        } else {
            PlayerActor_SetHoldableItem(0, ((PlayerActor *)this)->sessionSlot);
        }
        ((PlayerActor *)this)->actionPriority = ((Unk_02007694 *)(void *)(this))->getActionDonePriority(((PlayerActor *)this)->action);
        if (r6->b6) (((PlayerActor *)this))->requestAct10(3, 5, -1);
        else ((Unk_02007694 *)(void *)(((PlayerActor *)this)))->requestAct05(3, 5, -1);
    } else {
        (((PlayerActor *)this))->turnToCamera(0x400);
        switch (((u32)((PlayerActor *)this)->bodyModel.curFrame << 4) >> 16) {
        case 8:
            PlayerActor_SetHoldableItem(0, ((PlayerActor *)this)->sessionSlot);
            break;
        case 0xb:
            if (r4 >= 0x1369 && r4 <= 0x13a7) PlayerActor_SetHoldableItem(r4 - 0x1368, ((PlayerActor *)this)->sessionSlot);
            break;
        case 0xe:
            applyHeldItemPose(0, 6);
            break;
        }
        if (((AnimFrameCtrl *)(&(AnimFrameCtrl &)((PlayerActor *)this)->bodyModel))->isFinished()) {
            ((PlayerActor *)this)->actionPriority = ((Unk_02007694 *)(void *)(this))->getActionDonePriority(((PlayerActor *)this)->action);
            if (r6->b6) (((PlayerActor *)this))->requestAct10(3, 5, -1);
            else ((Unk_02007694 *)(void *)(((PlayerActor *)this)))->requestAct05(3, 5, -1);
        }
    }
}

void PlayerActor::mainChangeHeldItem() {
    using namespace nG;
    (((PlayerActor *)this))->advanceAnim();
    (((PlayerActor *)this))->netFollowTransform();
    (((PlayerActor *)this))->updateBodyCollider();
    changeHeldItemUpdate();
}

void PlayerWalkToArgs::setWalkToArgs(VecFx32Ctor v, s32 a) {
    using namespace nG;
    targetPos = v;
    maxSpeed = a;
}

BOOL PlayerActor::requestWalkTo(VecFx32Ctor *v, u32 a, u32 b, s16 c) {
    using namespace nG;
    PlayerActionRequest m;
    BOOL r;
    if (((PlayerActor *)this)->action == 0x6f) return 0;
    m.assign(0x6f, b, c);
    m.walkTo.setWalkToArgs(*v, a);
    r = pushRequest(&m);
    return r;
}

void PlayerWalkToWork::initWalkTo(VecFx32Ctor v, s32 a, s32 b) {
    using namespace nG;
    targetPos = v;
    walkSpeed = 0;
    maxSpeed = a;
    prevAction = b;
}

void PlayerActor::setupWalkTo(PlayerActionRequest *item, u32 old) {
    using namespace nG;
    PlayerWalkToArgs *p = &item->walkTo;
    Unk_020092c8_Loc loc;
    s32 r6;
    ((PlayerActor *)(this))->startAnim(1, 3, 0);
    r6 = p->maxSpeed;
    ((PlayerWalkToWork *)(PlayerActionWork *)((PlayerActor *)this)->actionWorkRaw)->initWalkTo(p->targetPos, r6, old);
    ((PlayerActor *)(this))->setActionFlag(5);
    ((PlayerActor *)this)->exitMode = 0;
    if ((((PlayerActor *)this))->isGuestInSession()) {
        if (((CommManager *)(gCommManager))->isLocalSlot(((PlayerActor *)this)->sessionSlot)) {
            if (Scene_GetCurrent() == 0xc) {
                if (r6 == 0x666) {
                    loc.date.w0 = 0;
                    loc.date.w1 = 0;
                    Clock_GetDateTime(&loc.date);
                    { u8 *q0 = PlayerSession_GetSessionFlags(); *q0 = *q0 & 0xf9; }
                    { u8 *q1 = PlayerSession_GetSessionFlags(); if ((*q1 & 1) == 0) DateTime_SubDays(&loc.date, 1); }
                    loc.bits.y = loc.date.b[5];
                    loc.bits.m = loc.date.b[4];
                    loc.bits.d = loc.date.b[3];
                    Unk_0209865c_Bits v;
                    v.a = loc.bits.y;
                    v.b = loc.bits.m;
                    v.c = loc.bits.d;
                    ((PlayerData *)PlayerActor_GetPlayerData(this))->setLastPlayDate(v);
                }
            }
        }
    }
}

void PlayerActor::netWalkTo() {
    using namespace nG;}

s32 PlayerActor::walkToUpdateSpeed() {
    using namespace nG;
    PlayerActionWork *p = (PlayerActionWork *)((PlayerActor *)this)->actionWorkRaw;
    s32 *r6 = &p->wc;
    s32 sp0 = p->w10;
    s32 r7 = 0;
    VecFx32Ctor d;
    VecFx32Ctor *pv = (VecFx32Ctor *)&((PlayerActor *)this)->position;
    VecFx32Ctor saved = *pv;
    s32 t;
    s32 ang;
    s16 h;
    s32 v;
    if (((PlayerActor *)(this))->testActionFlag(0x18)) p->w0 = ((PlayerActor *)this)->position.x;
    if (Scene_GetCurrent() == 0x20 && p->w14 == 0x40 && saved.z >= p->w8) {
        p->w8 = saved.z;
    }
    d.x = p->w0 - ((PlayerActor *)this)->position.x;
    d.z = p->w8 - ((PlayerActor *)this)->position.z;
    t = func_01ffcb0c(0x4000, *r6);
    if (t < 0x1000) t = 0x1000;
    if (Vec_MagXZ(&d) < t) {
        if (*r6 <= 0x333) {
            PlayerActor_ApproachCoord(&((PlayerActor *)this)->position.x, p->w0);
            PlayerActor_ApproachCoord(&((PlayerActor *)this)->position.z, p->w8);
            if (p->w0 == ((PlayerActor *)this)->position.x && p->w8 == ((PlayerActor *)this)->position.z) {
                *r6 = 0;
                r7 = 1;
            } else {
                *r6 = Vec_DistXZ(&((PlayerActor *)this)->position, &saved);
                VecFx32Ctor *pw = (VecFx32Ctor *)&((PlayerActor *)this)->position;
                *pw = saved;
            }
        } else {
            *r6 = PlayerActor_Decelerate(*r6, 0x333);
        }
    } else {
        *r6 = PlayerActor_Accelerate(*r6, sp0);
    }
    ang = Math_Atan2(d.x, d.z);
    h = ((PlayerActor *)this)->rotY;
    if (*r6) {
        PlayerActor_TurnAngle(&h, ang);
        (((PlayerActor *)this))->setAngleY((s16 *)(&h));
    }
    s32 vt = func_01ffcb0c(*r6, data_02135f44[(((u16)(s16)(h - ang)) >> 4) * 2 + 1]);
    if (vt < 0) vt = -vt;
    v = vt;
    (((PlayerActor *)this))->setSpeed((u32 *)(&v));
    return r7;
}

void PlayerActor::walkToUpdateAnim() {
    using namespace nG;
    if (((PlayerActor *)this)->animId == 0) {
        (((PlayerActor *)this))->advanceAnim();
        return;
    }
    s32 r4 = ((PlayerActor *)this)->speed;
    if (((PlayerActor *)(this))->testActionFlag(0x18)) r4 <<= 1;
    s32 q = func_01ffcb0c(r4, 0x3ae1);
    if (q <= (s32)((PlayerActor *)this)->bodyModel.numFrames) ((PlayerActor *)this)->bodyModel.frameStep = q;
    if (r4 > 0x53f) {
        if (((PlayerActor *)this)->animId != 2) (((PlayerActor *)this))->switchAnim(2, 3, 0);
    } else {
        if (((PlayerActor *)this)->animId != 1) (((PlayerActor *)this))->switchAnim(1, 3, 0);
    }
    ((TwoLayerAnimModel *)(&((PlayerActor *)this)->bodyModel))->updateLayers();
    (((PlayerActor *)this))->updateFootstepFx();
}

void PlayerActor::walkToMove() {
    using namespace nG;
    if (((PlayerActor *)(this))->testActionFlag(0x18)) {
        (((PlayerActor *)this))->moveWithCollision();
        RoomEntryRequest *r7 = (RoomEntryRequest *)RoomEntry_GetRequest();
        if (RoomEntryRequest_GetDoorKind(r7) == 1) {
            s32 *r4 = &((PlayerActor *)this)->position.y;
            s32 r6 = Ground_GetDefaultY();
            if (((PlayerActor *)this)->position.y >= r6) {
                s32 d = Unk_0200905c_abs(((s32 *)RoomEntryRequest_GetPos(r7))[2] - ((PlayerActor *)this)->position.z);
                if (d < 0x1000) {
                    s32 m = FX_Div(0x1000 - d, 0x1000) * 6;
                    *r4 = *r4 + (m >> 5);
                } else {
                    *r4 = r6;
                }
            } else {
                *r4 = r6;
            }
        }
    } else {
        (((PlayerActor *)this))->moveWithCollision();
    }
}

void PlayerActor::walkToCheckEnd(s32 f) {
    using namespace nG;
    if (f) {
        if (((PlayerActor *)(this))->testActionFlag(0x18)) {
            if (((PlayerActor *)this)->animId) {
                ((PlayerActor *)(this))->startAnim(0, 3, 0);
            }
            ((PlayerActor *)this)->actionPriority = ((Unk_02007694 *)(void *)(this))->getActionDonePriority(((PlayerActor *)this)->action);
            (((PlayerActor *)this))->requestWait(3, 1, -1);
            TalkRequest_FinishLeaveRoom();
            ((PlayerActor *)(this))->clearActionFlag(5);
            ((PlayerActor *)(this))->clearActionFlag(0x18);
            ((PlayerActor *)(this))->clearActionFlag(3);
        } else {
            (((PlayerActor *)this))->requestAct10(3, 5, -1);
            ((PlayerActor *)(this))->clearActionFlag(5);
        }
    }
}

void PlayerActor::mainWalkTo() {
    using namespace nG;
    s32 r = walkToUpdateSpeed();
    walkToUpdateAnim();
    walkToMove();
    (((PlayerActor *)this))->updateBodyCollider();
    walkToCheckEnd(r);
}

void PlayerTurnToArgs::setTurnToArgs(s16 v) {
    using namespace nG; targetAngle = v; }

BOOL PlayerActor::requestTurnTo(s16 v, u32 a, u32 b) {
    using namespace nG;
    PlayerActionRequest m;
    BOOL r;
    m.assign(0x70, a, b);
    m.turnTo.setTurnToArgs(v);
    r = pushRequest(&m);
    return r;
}

void PlayerTurnToWork::initTurnTo(s16 v) {
    using namespace nG; targetAngle = v; }

void PlayerActor::setupTurnTo(PlayerActionRequest *item, u32 old) {
    using namespace nG;
    s16 *p = (s16 *)&((PlayerActionRequest *)item)->unk_0c_b[0];
    PlayerTurnToWork *s = (PlayerTurnToWork *)(PlayerActionWork *)((PlayerActor *)this)->actionWorkRaw;
    if (!this->testActionFlag(0x17)) {
        ((PlayerActor *)(this))->startAnim(1, 3, 0);
    }
    s->initTurnTo(*p);
    ((PlayerActor *)(this))->setActionFlag(6);
}

void PlayerActor::netTurnTo() {
    using namespace nG;}

void PlayerActor::turnToUpdate() {
    using namespace nG;
    PlayerActionWork *p = (PlayerActionWork *)((PlayerActor *)this)->actionWorkRaw;
    s16 t = ((PlayerActor *)this)->rotY;
    p->h2 = t;
    PlayerActor_TurnAngle(&t, p->unk_00);
    (((PlayerActor *)this))->setAngleY((s16 *)(&t));
}

void PlayerActor::turnToCheckEnd() {
    using namespace nG;
    if (((PlayerActionWork *)((PlayerActor *)this)->actionWorkRaw)->unk_00 == ((PlayerActor *)this)->rotY) {
        (((PlayerActor *)this))->requestAct10(3, 5, -1);
        ((PlayerActor *)(this))->clearActionFlag(6);
    }
}

void PlayerActor::mainTurnTo() {
    using namespace nG;
    turnToUpdate();
    (((PlayerActor *)this))->advanceAnim();
    (((PlayerActor *)this))->updateBodyCollider();
    turnToCheckEnd();
}

BOOL PlayerActor::requestAct76(u8 a, u8 b, u8 c, u32 d, s16 e) {
    using namespace nG;
    PlayerActionRequest m;
    BOOL r;
    m.assign(0x76, d, e);
    PlayerAct76Args *pp = &m.act76;
    pp->set(a, b, c);
    r = pushRequest(&m);
    return r;
}

void PlayerNetActionArgs::writeAct76Net(u8 a, u8 b, u8 c) {
    using namespace nG;
    bytes[0] = a;
    bytes[1] = b;
    bytes[2] = c;
}

void PlayerNetActionArgs::readAct76Net(u8 *a, u8 *b, u8 *c) {
    using namespace nG;
    *a = bytes[0];
    *b = bytes[1];
    *c = bytes[2];
}

BOOL PlayerActor::netAct76(s16 v) {
    using namespace nG;
    u8 a, b, c;
    ((PlayerActor *)this)->netData.readAct76Net(&a, &b, &c);
    return requestAct76(a, b, c, 6, v);
}

void PlayerActor::mainAct76() {
    using namespace nF;
    (this)->advanceAnim();
    VecFx32 *pv = (VecFx32 *)&drawPos;
    volatile VecFx32 v;
    v.x = pv->x;
    v.y = pv->y;
    v.z = pv->z;
    s16 d = (s16)drawTilt;
    Mtx43 blk1 = bodyModel.mtx;
    Mtx43 blk2 = itemHandMtx;
    (this)->calcHandMtx();
    (this)->updateBodyCollider();
    act76Update();
    drawPos.x = v.x;
    drawPos.y = v.y;
    drawPos.z = v.z;
    drawTilt = d;
    bodyModel.mtx = blk1;
    itemHandMtx = blk2;
}

BOOL PlayerActor::requestAct77(s16 v, u32 a, u32 b) {
    using namespace nF;
    PlayerActionRequest m;
    BOOL r;
    m.assign(0x77, a, b);
    m.unk_0c_h = v;
    r = pushRequest(&m);
    return r;
}

void PlayerNetActionArgs::writeS16(s16 v) {
    using namespace nF;
    NetBuf_WriteS16B(this, v);
}

void PlayerNetActionArgs::readS16(s16 *out) {
    using namespace nF;
    *out = NetBuf_ReadS16B(this);
}

void PlayerActor::setupAct77(u8 *msg) {
    using namespace nF;
    s16 v = *(s16 *)(((PlayerActionRequest *)(void *)msg)->unk_0c_b);  // (the payload: 0xc on the DS, 0x10 on 64-bit)
    (this)->startAnimOnce(0x70, 3, 0);
    Effect_Create(0x43, &headTopPos, 0, 0);
    (this)->playSe(0x86);
    *(s16 *)&actionWorkRaw[0] = v;
    netData.writeS16(v);
}

void PlayerActor::netAct77(u32 b) {
    using namespace nF;
    s16 v;
    netData.readS16(&v);
    requestAct77(v, 6, b);
}

void PlayerActor::act77Turn() {
    using namespace nF;
    s16 v = rotY;
    PlayerActor_ApproachAngle(&v, *(s16 *)&actionWorkRaw[0], 0x800, 0x1770000, 0xc0000);
    (this)->setAngleY((s16 *)(&v));
}

void PlayerActor::act77CheckEnd() {
    using namespace nF;
    if (((AnimFrameCtrl *)(&(AnimFrameCtrl &)bodyModel))->isFinished()) {
        actionPriority = ((Unk_02007694 *)(void *)(this))->getActionDonePriority(action);
        (this)->requestWait(3, 1, -1);
    }
}

void PlayerActor::mainAct77() {
    using namespace nF;
    (this)->advanceAnim();
    act77Turn();
    (this)->updateBodyCollider();
    act77CheckEnd();
}

BOOL PlayerActor::requestAct79(u32 a, u32 b) {
    using namespace nF;
    PlayerActionRequest m;
    m.assign(0x79, a, b);
    if (pushRequest(&m)) {
        (this)->setActionFlag(8);
        return TRUE;
    }
    return FALSE;
}

void PlayerActor::setupAct79() {
    using namespace nF;
    (this)->startAnimOnce(0x76, 3, 0);
    actionWorkRaw[0] = 0;
    if (((CommManager *)(gCommManager))->isLocalSlot(sessionSlot)) {
        Bgm_RequestSilence(0xc, 0xf, 0);
    }
    (this)->playSe(0x83a);
}

BOOL PlayerActor::netAct79(u32 b) {
    using namespace nF;
    return requestAct79(5, b);
}

void PlayerActor::act79Update() {
    using namespace nF;
    u8 *st = &actionWorkRaw[0];
    if (animId == 0x76) {
        if (((AnimFrameCtrl *)(&(AnimFrameCtrl &)bodyModel))->isFinished()) {
            (this)->startAnim(0x77, 0, 0);
            if (!((CommManager *)(gCommManager))->isLocalSlot(sessionSlot)) {
                *st = 3;
                actionPriority = ((Unk_02007694 *)(void *)(this))->getActionDonePriority(action);
            }
        }
    } else {
        (this)->turnToCamera(0x400);
        switch (*st) {
        case 0:
            if (TalkRequest_AddPlayerMessage()) {
                *st = 1;
                ((Character *)(void *)(this))->attachTalkRequest(this);
                (this)->setActionFlag(0x11);
                setFileName(sPlayerActorMsgFile);
                msgIndex = 0x15;
                window->nextState = 1;
                Camera_SetMode4();
            }
        case 1:
            if (window != NULL) {
                if (window->state != 0) {
                    *st = 2;
                }
            }
            break;
        case 2:
            if (window != NULL) {
                if (window->state == 0) {
                    ((Character *)(void *)(this))->detachTalkRequest(this);
                    (this)->clearActionFlag(0x11);
                    TalkRequest_FinishPlayerMessage();
                    actionPriority = ((Unk_02007694 *)(void *)(this))->getActionDonePriority(action);
                    (this)->requestWait(3, 1, -1);
                    (this)->clearActionFlag(8);
                    Camera_SetModeDefault();
                    Bgm_ReleasePriority(0xc);
                }
            }
            break;
        }
    }
}

void PlayerActor::mainAct79() {
    using namespace nF;
    (this)->advanceAnim();
    (this)->updateBodyCollider();
    act79Update();
}

BOOL PlayerActor::requestLidClosed(u32 a, u32 b) {
    using namespace nF;
    PlayerActionRequest m;
    BOOL r;
    m.assign(0x83, a, b);
    r = pushRequest(&m);
    return r;
}

void PlayerActor::setupLidClosed(u32 a, u32 flag) {
    using namespace nF;
    u32 t = 0;
    if (flag) {
        t = 5;
    }
    (this)->startAnim(0x81, t, t);
}

BOOL PlayerActor::netLidClosed(u32 b) {
    using namespace nF;
    return requestLidClosed(1, b);
}

void PlayerActor::lidClosedUpdateAnim() {
    using namespace nF;
    (this)->advanceAnim();
    if (((AnimFrameCtrl *)(&(AnimFrameCtrl &)bodyModel))->hasPassedFrame(1)) {
        if (((CommManager *)(gCommManager))->isLocalSlot(sessionSlot)) {
            SndMgr_PlaySe(&gSndMgr, 0x79);
        } else {
            (this)->playSe(0x79);
        }
    }
}

void PlayerActor::lidClosedCheckOpen() {
    using namespace nF;
    if (((*HW_SYSWORK(vu16, buttonXY) & 0x8000) >> 15) == 0) {
        (this)->requestWait(3, 1, -1);
    }
}

void PlayerActor::mainLidClosed() {
    using namespace nF;
    lidClosedUpdateAnim();
    if (((CommManager *)(gCommManager))->isLocalSlot(sessionSlot)) {
        (this)->updateBodyCollider();
        lidClosedCheckOpen();
    } else {
        (this)->netFollowTransform();
        (this)->updateBodyCollider();
    }
}

BOOL PlayerActor::requestErrorMessage(u8 v, u32 a, u32 b) {
    using namespace nF;
    PlayerActionRequest m;
    BOOL r;
    m.assign(0x84, a, b);
    m.unk_0c_b[0] = v;
    r = pushRequest(&m);
    return r;
}

void PlayerActor::setupErrorMessage(u8 *msg) {
    using namespace nF;
    u8 *p = &actionWorkRaw[0];
    p[0] = ((PlayerActionRequest *)(void *)msg)->unk_0c_b[0];
    p[1] = 0;
    (this)->startAnim(0, 3, 3);
}

BOOL PlayerActor::netErrorMessage(u32 b) {
    using namespace nF;
    return requestErrorMessage(0, 6, b);
}

void PlayerActor::func_020082a8() {
    using namespace nF;}

void PlayerActor::errorMessageUpdate() {
    using namespace nF;
    u8 *st;
    if (((CommManager *)(gCommManager))->isLocalSlot(sessionSlot)) {
        st = &actionWorkRaw[1];
        switch (*st) {
        case 0:
            if (TalkRequest_AddPlayerMessage()) {
                *st = 1;
                ((Character *)(void *)(this))->attachTalkRequest(this);
                (this)->setActionFlag(0x11);
                setFileName(sPlayerActorErrorMsgFile);
                msgIndex = 0x1e;
                window->nextState = 1;
                LowBattery_SetWarned();
            } else {
                actionPriority = ((Unk_02007694 *)(void *)(this))->getActionDonePriority(action);
                (this)->requestWait(3, 1, -1);
            }
            break;
        case 1:
            if (window != NULL) {
                if (window->state != 0) {
                    *st = 2;
                }
            }
            break;
        case 2:
            if (window != NULL) {
                if (window->state == 0) {
                    ((Character *)(void *)(this))->detachTalkRequest(this);
                    (this)->clearActionFlag(0x11);
                    TalkRequest_FinishPlayerMessage();
                    actionPriority = ((Unk_02007694 *)(void *)(this))->getActionDonePriority(action);
                    (this)->requestWait(3, 1, -1);
                }
            }
            break;
        }
    } else {
        actionPriority = ((Unk_02007694 *)(void *)(this))->getActionDonePriority(action);
    }
}

void PlayerActor::mainErrorMessage() {
    using namespace nF;
    (this)->advanceAnim();
    if (((CommManager *)(gCommManager))->isLocalSlot(sessionSlot)) {
        (this)->updateBodyCollider();
    } else {
        (this)->netFollowTransform();
        (this)->updateBodyCollider();
    }
    errorMessageUpdate();
}

BOOL PlayerActor::requestHoldUpItem(u16 *v, u32 a, u32 b) {
    using namespace nF;
    PlayerActionRequest m;
    BOOL r;
    m.assign(0x85, a, b);
    shownItem = *v;
    actionItem = shownItem;
    r = pushRequest(&m);
    return r;
}

void PlayerNetActionArgs::writeU16(u16 v) {
    using namespace nF;
    NetBuf_WriteU16(this, v);
}

void PlayerNetActionArgs::readU16(u16 *out) {
    using namespace nF;
    *out = NetBuf_ReadU16(this);
}

void PlayerActor::setupHoldUpItem() {
    using namespace nF;
    s32 t;
    netData.writeU16(actionItem);
    VecFx32 v;
    PlayerActor_CalcHeldUpItemPos(&v, this, 0);
    shownItemPos.x = v.x;
    shownItemPos.y = v.y;
    shownItemPos.z = v.z;
    t = 0x1000;
    shownItemScaleX = t;
    shownItemScaleY = t;
    shownItemScaleZ = t;
    (this)->startAnimOnce(0x7e, 3, 0);
}

void PlayerActor::netHoldUpItem(s32 arg) {
    using namespace nF;
    u16 a, b;
    netData.readU16(&a);
    b = a;
    requestHoldUpItem(&b, 6, arg);
}

void Unk_02007694::holdUpItemUpdate() {
    using namespace nE;
    VecFx32 v;
    ((PlayerActor *)(this))->advanceAnim();
    s32 n = ((u32)static_cast<AnimFrameCtrl &>(((Unk_02007694 *)this)->bodyModel).curFrame << 4) >> 16;
    s32 t = 0x1000;
    if (((Unk_02007694 *)this)->animId == 0x7e) {
        if (n == 10) {
            ((PlayerActor *)(this))->setActionFlag(0xd);
        }
    } else {
        s32 c = ((Unk_02007694 *)this)->shownItemScaleX;
        if (n >= 9) {
            t = c - 0x155;
            if (t < 0) {
                t = 0;
            }
        }
    }
    ((Unk_02007694 *)this)->shownItemScaleX = t;
    ((Unk_02007694 *)this)->shownItemScaleY = t;
    ((Unk_02007694 *)this)->shownItemScaleZ = t;
    PlayerActor_CalcHeldUpItemPos(&v, ((Unk_02007694 *)this), n);
    Math_ApproachS32(&((Unk_02007694 *)this)->shownItemPos.x, v.x, 0x800, 0x2000, 0x333);
    Math_ApproachS32(&((Unk_02007694 *)this)->shownItemPos.y, v.y, 0x800, 0x2000, 0x333);
    Math_ApproachS32(&((Unk_02007694 *)this)->shownItemPos.z, v.z, 0x800, 0x2000, 0x333);
}

namespace nE {
extern "C" void PlayerActor_CalcHeldUpItemPos(VecFx32 *out, PlayerActor *obj, s32 n) {
    Mtx43 m = obj->itemHandMtx;
    s32 tx = m.m[9];
    s32 ty = m.m[10];
    s32 tz = m.m[11];
    m.m[11] = 0;
    m.m[10] = 0;
    m.m[9] = 0;
    Mtx43 m2 = m;
    VecFx32 in;
    VecFx32 res;
    in.x = 0x320;
    in.z = 0;
    if (obj->animId == 0x7e) {
        if (n <= 10) {
            in.y = 0;
        } else {
            in.y = -0xa00;
        }
    } else {
        in.y = -((0xa000 - FX_Div(n * 0x5000, 0x15000)) >> 4);
    }
    MTX_MultVec43(&in, &m2, &res);
    tx += res.x;
    ty += res.y;
    tz += res.z;
    VecFx32 fin = {tx, ty, tz};
    WorldCurve_FromCurved(out, &fin);
}
}

void Unk_02007694::holdUpItemCheckEnd() {
    using namespace nE;
    if (((PlayerActor *)(this))->turnToCamera(0x800)) {
        if ((&static_cast<AnimFrameCtrl &>(((Unk_02007694 *)this)->bodyModel))->isFinished()) {
            if (((Unk_02007694 *)this)->animId == 0x7e) {
                ((PlayerActor *)(this))->startAnimOnce(0x7f, 0, 0);
                ((PlayerActor *)(this))->playSe(0x6e);
            } else {
                requestLowerHeldUpItem(6, -1);
                ((PlayerActor *)(this))->clearActionFlag(0xd);
            }
        }
    }
}

void Unk_02007694::mainHoldUpItem() {
    using namespace nE;
    holdUpItemUpdate();
    ((PlayerActor *)(this))->updateBodyCollider();
    holdUpItemCheckEnd();
}

u32 Unk_02007694::requestLowerHeldUpItem(u32 a, u32 b) {
    using namespace nE;
    PlayerActionRequest m;
    ((PlayerActionRequest *)(&m))->assign(0x86, a, b);
    u32 r = ((PlayerActor *)(this))->pushRequest(&m);
    return r;
}

void Unk_02007694::setupLowerHeldUpItem() {
    using namespace nE;
    ((PlayerActor *)(this))->startAnimOnce(0x80, 3, 0);
}

void Unk_02007694::netLowerHeldUpItem(u32 a) {
    using namespace nE;
    if (((Unk_02007694 *)this)->action == 0x85) {
    } else if (((Unk_02007694 *)this)->action == 0x86) {
        ((Unk_02007694 *)this)->netSeq = a;
    } else {
        requestLowerHeldUpItem(6, a);
    }
}

void Unk_02007694::lowerHeldUpItemCheckEnd() {
    using namespace nE;
    if ((&static_cast<AnimFrameCtrl &>(((Unk_02007694 *)this)->bodyModel))->isFinished()) {
        ((Unk_02007694 *)this)->actionPriority = getActionDonePriority(((Unk_02007694 *)this)->action);
        ((Unk_02007694 *)(this))->requestAct05(3, 5, -1);
    }
}

void Unk_02007694::mainLowerHeldUpItem() {
    using namespace nE;
    ((PlayerActor *)(this))->advanceAnim();
    ((PlayerActor *)(this))->updateBodyCollider();
    lowerHeldUpItemCheckEnd();
}

u32 Unk_02007694::requestWaitMenu(u32 a, u32 b, u32 c) {
    using namespace nE;
    PlayerActionRequest m;
    ((PlayerActionRequest *)(&m))->assign(0x90, b, c);
    m.unk_0c_h = a;
    u32 r = ((PlayerActor *)(this))->pushRequest(&m);
    return r;
}

void Unk_02007694::setupWaitMenu(PlayerActionRequest *p) {
    using namespace nE;
    u16 v = p->unk_0c_h;
    ((PlayerActor *)(this))->startAnim(0, v, v);
    ((PlayerActor *)(this))->resetHeldToolAnim();
}

void Unk_02007694::netWaitMenu(u32 a) {
    using namespace nE;
    ((PlayerActor *)(this))->requestWait(3, 5, a);
}

void Unk_02007694::waitMenuCheckEnd() {
    using namespace nE;
    if (MenuCtrl_IsFinished()) {
        TalkRequest_FinishPlayerMessage();
        ((PlayerActor *)(this))->requestWait(3, 1, -1);
    }
}

void Unk_02007694::mainWaitMenu() {
    using namespace nE;
    ((PlayerActor *)(this))->moveWithCollision();
    ((PlayerActor *)(this))->advanceAnim();
    ((PlayerActor *)(this))->updateBodyCollider();
    ((PlayerActor *)(this))->submitSceneCollider();
    waitMenuCheckEnd();
}

void Unk_02007694::setupAct91() {
    using namespace nE;}

void Unk_02007694::netAct91() {
    using namespace nE;}

void Unk_02007694::endAct91() {
    using namespace nE;}

void Unk_02007694::mainAct91() {
    using namespace nE;}

void Unk_02007694::setupAct92() {
    using namespace nE;}

void Unk_02007694::netAct92() {
    using namespace nE;}

void Unk_02007694::mainAct92() {
    using namespace nE;}

void Unk_02007694::calcModelMatrixCurved() {
    using namespace nE;
    Mtx43 m;
    ((Actor *)this)->drawTilt = WorldCurve_ToCurved(&((Actor *)this)->drawPos, &((Actor *)this)->position);
    ((Actor *)(this))->calcModelMatrix((void *)(void *)(&m));
    ((Unk_02007694 *)this)->bodyModel.mtx = m;
}

u8 Unk_02007694::keepsBgCheckWork(u32 a) {
    using namespace nE;
    return sPlayerActionKeepsBgCheckWork[a];
}

void Unk_02007694::updateBgCheckWork(u32 a, u32 b) {
    using namespace nE;
    u8 r = keepsBgCheckWork(a);
    if (r) {
        keepsBgCheckWork(b);
        if (!r) {
            (&((Unk_02007694 *)this)->bgCheckWork)->beginStep();
        }
    }
}

u8 Unk_02007694::getActionPriority(u32 a) {
    using namespace nE;
    return sPlayerActionPriority[a];
}

u8 Unk_02007694::getActionDonePriority(u32 a) {
    using namespace nE;
    return sPlayerActionDonePriority[a];
}

void Unk_02007694::endAction(u32 a) {
    using namespace nE;
    static void (Unk_02007694::*tbl[147])(u32) = {
        reinterpret_cast<void (Unk_02007694::*)(u32)>(&PlayerActor::endInit), reinterpret_cast<void (Unk_02007694::*)(u32)>(&PlayerActor::endAct01), reinterpret_cast<void (Unk_02007694::*)(u32)>(&PlayerActor::endWait), 0, &Unk_02007694::endSkidTurn, 0,
        data_020d6d74.fn, &Unk_02007694::endChangeClothes, data_020d6d64.fn, 0, data_020d6d5c.fn, 0,
        0, data_020d6d54.fn, data_020d6d4c.fn, data_020d6d44.fn, 0, 0,
        0, 0, reinterpret_cast<void (Unk_02007694::*)(u32)>(&PlayerActor::endEmotion), 0, 0, data_020d6d34.fn,
        0, reinterpret_cast<void (Unk_02007694::*)(u32)>(&PlayerActor::endPickUp), reinterpret_cast<void (Unk_02007694::*)(u32)>(&PlayerActor::endPickUpFanfare), 0, 0, data_020d6d1c.fn,
        0, 0, 0, 0, data_020d6d14.fn, data_020d6d0c.fn,
        data_020d6d04.fn, data_020d6cfc.fn, data_020d6cf4.fn, data_020d6cec.fn, data_020d6ce4.fn, data_020d6cdc.fn,
        data_020d6cd4.fn, 0, data_020d6ccc.fn, 0, 0, 0,
        0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0,
        data_020d6cc4.fn, data_020d6cbc.fn, 0, reinterpret_cast<void (Unk_02007694::*)(u32)>(&PlayerActor::endChangeHeldItem), 0, 0,
        0, 0, 0, data_020d6cac.fn, 0, 0,
        0, data_020d6ca4.fn, data_020d6c9c.fn, data_020d6c94.fn, data_020d6c8c.fn, 0,
        0, data_020d6c84.fn, 0, 0, 0, 0,
        0, 0, 0, data_020d6c7c.fn, data_020d6c74.fn, 0,
        0, 0, 0, data_020d6c6c.fn, data_020d6c64.fn, data_020d6c5c.fn,
        0, 0, data_020d6c54.fn, data_020d6c4c.fn, 0, 0,
        data_020d6c44.fn, 0, data_020d6c3c.fn, 0, 0, 0,
        0, 0, 0, 0, 0, 0,
        0, data_020d6c34.fn, data_020d6c2c.fn, 0, 0, 0,
        0, 0, 0, 0, data_020d6c24.fn, 0,
        0, 0, data_020d6c1c.fn, data_020d6c14.fn, data_020d6c0c.fn, 0,
        0, 0, 0, reinterpret_cast<void (Unk_02007694::*)(u32)>(&PlayerActor::endTaxiGetOut), reinterpret_cast<void (Unk_02007694::*)(u32)>(&PlayerActor::endTaxiGetIn), 0,
        0, 0, 0, 0, 0, 0,
        0, reinterpret_cast<void (Unk_02007694::*)(u32)>(&Unk_02007694::endAct91), 0,
    };
    void (Unk_02007694::*fn)(u32) = tbl[((Unk_02007694 *)this)->action];
    if (fn) {
        (((Unk_02007694 *)this)->*fn)(a);
    }
    ((PlayerActor *)(this))->clearActionFlag(0x1c);
}

void Unk_02007694::clearActionWork() {
    using namespace nE;
    MI_CpuFill8(((Unk_02007694 *)this)->actionWorkRaw, 0, 0x1c);
}

void Unk_02007694::stopMovementForAction(u32 a) {
    using namespace nE;
    if (sPlayerActionStopsMovement[a]) {
        ((Unk_02007694 *)this)->speed = 0;
        u32 t = ((Unk_02007694 *)this)->velocity.y;
        ((Unk_02007694 *)this)->velocity.x = 0;
        ((Unk_02007694 *)this)->velocity.y = t;
        ((Unk_02007694 *)this)->velocity.z = 0;
    }
}

void Unk_02007694::resetRotXForAction(u32 a) {
    using namespace nE;
    if (sPlayerActionResetsRotX[a]) {
        ((PlayerActor *)(void *)(((Unk_02007694 *)this)))->setRotX(0);
    }
}

void PlayerActor::changeAction(PlayerActionRequest* item) {
    using namespace nD;
    static Unk_02006d14_Fn table[] = {
        reinterpret_cast<nD::Unk_02006d14_Fn>(&PlayerActor::setupInit), &PlayerActor::setupAct01, &PlayerActor::setupWait, &PlayerActor::setupWalk, reinterpret_cast<nD::Unk_02006d14_Fn>(&Unk_02007694::setupSkidTurn), reinterpret_cast<nD::Unk_02006d14_Fn>(&Unk_02007694::setupAct05),
        data_020d6b9c.fn, reinterpret_cast<nD::Unk_02006d14_Fn>(&Unk_02007694::setupChangeClothes), data_020d610c.fn, data_020d6b84.fn, data_020d6b7c.fn, data_020d6b74.fn,
        data_020d6b6c.fn, data_020d6b64.fn, data_020d6b5c.fn, data_020d6b54.fn, &PlayerActor::setupAct10, data_020d6b44.fn,
        data_020d6b3c.fn, &PlayerActor::setupAct13, &PlayerActor::setupEmotion, &PlayerActor::setupAct15, data_020d6b1c.fn, data_020d6b14.fn,
        &PlayerActor::setupPickUpReach, &PlayerActor::setupPickUp, &PlayerActor::setupPickUpFanfare, &PlayerActor::setupPickUpFanfareStow, data_020d6aec.fn, data_020d6ae4.fn,
        data_020d6adc.fn, data_020d6ad4.fn, data_020d6acc.fn, data_020d6ac4.fn, data_020d6abc.fn, data_020d6ab4.fn,
        data_020d6aac.fn, data_020d6aa4.fn, data_020d6a9c.fn, data_020d6a94.fn, data_020d6a8c.fn, data_020d6a84.fn,
        data_020d6a7c.fn, data_020d6a74.fn, data_020d6a6c.fn, data_020d6a64.fn, data_020d6a5c.fn, data_020d6a54.fn,
        &PlayerActor::setupAct30, &PlayerActor::setupAct31, &PlayerActor::setupAct32, &PlayerActor::setupAct33, &PlayerActor::setupAct34, &PlayerActor::setupAct35,
        data_020d6a1c.fn, data_020d6a14.fn, data_020d6a0c.fn, data_020d6a04.fn, data_020d69fc.fn, data_020d69f4.fn,
        data_020d69ec.fn, data_020d69e4.fn, data_020d69dc.fn, &PlayerActor::setupChangeHeldItem, data_020d69cc.fn, data_020d69c4.fn,
        data_020d69bc.fn, data_020d69b4.fn, data_020d69ac.fn, data_020d69a4.fn, data_020d699c.fn, data_020d638c.fn,
        data_020d608c.fn, data_020d6984.fn, data_020d697c.fn, data_020d6974.fn, data_020d696c.fn, data_020d6964.fn,
        data_020d695c.fn, data_020d6954.fn, data_020d694c.fn, data_020d6944.fn, data_020d693c.fn, data_020d6934.fn,
        data_020d692c.fn, data_020d6924.fn, data_020d691c.fn, data_020d6914.fn, data_020d690c.fn, data_020d6904.fn,
        data_020d68fc.fn, data_020d68f4.fn, data_020d68ec.fn, data_020d68e4.fn, data_020d68dc.fn, data_020d68d4.fn,
        data_020d68cc.fn, data_020d68c4.fn, data_020d68bc.fn, data_020d68b4.fn, data_020d68ac.fn, data_020d68a4.fn,
        data_020d689c.fn, data_020d6894.fn, data_020d688c.fn, data_020d6884.fn, data_020d687c.fn, data_020d6874.fn,
        data_020d686c.fn, data_020d6864.fn, data_020d685c.fn, &PlayerActor::setupWalkTo, &PlayerActor::setupTurnTo, data_020d6844.fn,
        data_020d683c.fn, data_020d6834.fn, data_020d682c.fn, data_020d6824.fn, &PlayerActor::setupAct76, reinterpret_cast<nD::Unk_02006d14_Fn>(&PlayerActor::setupAct77),
        data_020d680c.fn, reinterpret_cast<nD::Unk_02006d14_Fn>(&PlayerActor::setupAct79), data_020d67fc.fn, data_020d67f4.fn, data_020d67ec.fn, data_020d67e4.fn,
        data_020d67dc.fn, data_020d67d4.fn, data_020d67cc.fn, data_020d67c4.fn, data_020d67bc.fn, reinterpret_cast<nD::Unk_02006d14_Fn>(&PlayerActor::setupLidClosed),
        reinterpret_cast<nD::Unk_02006d14_Fn>(&PlayerActor::setupErrorMessage), reinterpret_cast<nD::Unk_02006d14_Fn>(&PlayerActor::setupHoldUpItem), reinterpret_cast<nD::Unk_02006d14_Fn>(&Unk_02007694::setupLowerHeldUpItem), reinterpret_cast<nD::Unk_02006d14_Fn>(&PlayerActor::setupTaxiGetOut), reinterpret_cast<nD::Unk_02006d14_Fn>(&PlayerActor::setupTaxiGetIn), data_020d6784.fn,
        data_020d677c.fn, data_020d6774.fn, data_020d627c.fn, data_020d6764.fn, data_020d675c.fn, data_020d626c.fn,
        reinterpret_cast<nD::Unk_02006d14_Fn>(&Unk_02007694::setupWaitMenu), reinterpret_cast<nD::Unk_02006d14_Fn>(&Unk_02007694::setupAct91), reinterpret_cast<nD::Unk_02006d14_Fn>(&Unk_02007694::setupAct92),
    };
    u32 id = ((PlayerActionRequest *)item)->action;
    s16 v = ((PlayerActionRequest *)item)->netSeq;
    Unk_02006d14_Fn fn = table[id];
    u32 old = ((PlayerActor *)this)->prevAction;
    ((Unk_02007694 *)(this))->endAction(id);
    ((Unk_02007694 *)(this))->clearActionWork();
    ((PlayerActor *)this)->action = id;
    ((Unk_02007694 *)(this))->stopMovementForAction(id);
    ((Unk_02007694 *)(this))->updateBgCheckWork(id, old);
    ((Unk_02007694 *)(this))->resetRotXForAction(id);
    ((PlayerActor *)(this))->setBodyCollider(&id);
    ((Unk_02005e7c *)(this))->setShadowForAction(id);
    CommManager* p = gCommManager;
    if ((p)->isSlotActive(p->myAid)) {
        if (!p->isLocalSlot(((PlayerActor *)this)->sessionSlot)) {
            if (v >= 0) { ((PlayerActor *)this)->netSeq = v; ((PlayerActor *)this)->netSeqAction = id; }
        } else {
            ((PlayerActor *)this)->netSeq = (p)->getSendSeq();
        }
    }
    (((PlayerActor *)this)->*fn)(((PlayerActionRequest *)item), old);
    ((PlayerActor *)this)->actionPriority = ((Unk_02007694 *)(this))->getActionPriority(id);
    if (!p->isLocalSlot(((PlayerActor *)this)->sessionSlot) && data_020c6a18[id] != 0) ((PlayerActor *)(this))->setActionFlag(0x12);
    else ((PlayerActor *)(this))->clearActionFlag(0x12);
}

void Unk_02005e7c::followNetAction() {
    using namespace nC;
    s32 st;
    if (!PlayerActor_GetSlotAction((u32 *)&st, -1, sessionSlot)) {
        lastNetAction = 0x93;
        return;
    }
    if (st >= 0x93) {
        lastNetAction = 0x93;
        return;
    }
    if (((PlayerActor *)(this))->testActionFlag(0x12) != 0) {
        if (st != 0x1a) {
            return;
        }
        if (action != 0x19) {
            return;
        }
        ((PlayerActor *)(this))->clearActionFlag(0x12);
    }
    lastNetAction = st;
    u8 *r = PlayerActor_GetNetStateVar(sessionSlot);
    if (r != NULL) {
        MI_CpuCopy8(r + 4, netData.bytes, 8);
    }
    s32 a = ((PlayerActor *)(this))->isLocomotionAction(action);
    s32 b = ((PlayerActor *)(this))->isLocomotionAction(st);
    if (a == 1 && b == 1) {
        return;
    }
    if (b == 1) {
        st = 2;
    }
    s32 idx = -1;
    r = PlayerActor_GetNetStateVar(sessionSlot);
    if (r != NULL) {
        idx = NetBuf_ReadS16(r + 2);
    }
    s32 c = netSeq;
    if (c >= 0 && netSeqAction == st && idx == c) {
        return;
    }
    static void (Unk_02005e7c::*tbl[147])(s32) = {
        reinterpret_cast<void (Unk_02005e7c::*)(s32)>(&PlayerActor::netInit), reinterpret_cast<void (Unk_02005e7c::*)(s32)>(&PlayerActor::netAct01), reinterpret_cast<void (Unk_02005e7c::*)(s32)>(&PlayerActor::netWait), reinterpret_cast<void (Unk_02005e7c::*)(s32)>(&PlayerActor::netWalk), reinterpret_cast<void (Unk_02005e7c::*)(s32)>(&Unk_02007694::netSkidTurn), reinterpret_cast<void (Unk_02005e7c::*)(s32)>(&Unk_02007694::netAct05),
        data_020d66ec.fn, reinterpret_cast<void (Unk_02005e7c::*)(s32)>(&Unk_02007694::netChangeClothes), data_020d66dc.fn, data_020d66d4.fn, data_020d66cc.fn, data_020d66c4.fn,
        data_020d66bc.fn, data_020d66b4.fn, data_020d66ac.fn, data_020d66a4.fn, reinterpret_cast<void (Unk_02005e7c::*)(s32)>(&PlayerActor::netAct10), data_020d6694.fn,
        data_020d620c.fn, reinterpret_cast<void (Unk_02005e7c::*)(s32)>(&PlayerActor::netAct13), reinterpret_cast<void (Unk_02005e7c::*)(s32)>(&PlayerActor::netEmotion), reinterpret_cast<void (Unk_02005e7c::*)(s32)>(&PlayerActor::netAct15), data_020d666c.fn, data_020d6664.fn,
        reinterpret_cast<void (Unk_02005e7c::*)(s32)>(&PlayerActor::netPickUpReach), reinterpret_cast<void (Unk_02005e7c::*)(s32)>(&PlayerActor::netPickUp), reinterpret_cast<void (Unk_02005e7c::*)(s32)>(&PlayerActor::netPickUpFanfare), reinterpret_cast<void (Unk_02005e7c::*)(s32)>(&PlayerActor::netPickUpFanfareStow), data_020d663c.fn, data_020d6634.fn,
        data_020d662c.fn, data_020d6624.fn, data_020d661c.fn, data_020d6614.fn, data_020d5f54.fn, data_020d5e6c.fn,
        data_020d6be4.fn, data_020d65b4.fn, data_020d65cc.fn, data_020d65d4.fn, data_020d65dc.fn, data_020d65ec.fn,
        data_020d65fc.fn, data_020d670c.fn, data_020d6714.fn, data_020d674c.fn, data_020d6754.fn, data_020d6dbc.fn,
        reinterpret_cast<void (Unk_02005e7c::*)(s32)>(&PlayerActor::netAct30), reinterpret_cast<void (Unk_02005e7c::*)(s32)>(&PlayerActor::netAct31), reinterpret_cast<void (Unk_02005e7c::*)(s32)>(&PlayerActor::netAct32), reinterpret_cast<void (Unk_02005e7c::*)(s32)>(&PlayerActor::netAct33), reinterpret_cast<void (Unk_02005e7c::*)(s32)>(&PlayerActor::netAct34), reinterpret_cast<void (Unk_02005e7c::*)(s32)>(&PlayerActor::netAct35),
        data_020d656c.fn, data_020d6564.fn, data_020d655c.fn, data_020d6554.fn, data_020d654c.fn, data_020d6544.fn,
        data_020d653c.fn, data_020d6534.fn, data_020d652c.fn, reinterpret_cast<void (Unk_02005e7c::*)(s32)>(&PlayerActor::netChangeHeldItem), data_020d651c.fn, data_020d6514.fn,
        data_020d650c.fn, data_020d6504.fn, data_020d64fc.fn, data_020d64f4.fn, data_020d64ec.fn, data_020d64e4.fn,
        data_020d64dc.fn, data_020d64d4.fn, data_020d64cc.fn, data_020d64c4.fn, data_020d6bec.fn, data_020d6bdc.fn,
        data_020d6bd4.fn, data_020d64a4.fn, data_020d649c.fn, data_020d6494.fn, data_020d5f4c.fn, data_020d6484.fn,
        data_020d647c.fn, data_020d6474.fn, data_020d646c.fn, data_020d6464.fn, data_020d645c.fn, data_020d6454.fn,
        data_020d644c.fn, data_020d6444.fn, data_020d643c.fn, data_020d6434.fn, data_020d642c.fn, data_020d6424.fn,
        data_020d641c.fn, data_020d6414.fn, data_020d640c.fn, data_020d6404.fn, data_020d63fc.fn, data_020d63f4.fn,
        data_020d63ec.fn, data_020d63e4.fn, data_020d63dc.fn, data_020d63d4.fn, data_020d63cc.fn, data_020d63c4.fn,
        data_020d63bc.fn, data_020d63b4.fn, data_020d63ac.fn, reinterpret_cast<void (Unk_02005e7c::*)(s32)>(&PlayerActor::netWalkTo), reinterpret_cast<void (Unk_02005e7c::*)(s32)>(&PlayerActor::netTurnTo), data_020d6394.fn,
        data_020d5f0c.fn, data_020d6384.fn, data_020d637c.fn, data_020d6374.fn, reinterpret_cast<void (Unk_02005e7c::*)(s32)>(&PlayerActor::netAct76), reinterpret_cast<void (Unk_02005e7c::*)(s32)>(&PlayerActor::netAct77),
        data_020d635c.fn, reinterpret_cast<void (Unk_02005e7c::*)(s32)>(&PlayerActor::netAct79), data_020d634c.fn, data_020d6344.fn, data_020d633c.fn, data_020d6334.fn,
        data_020d632c.fn, data_020d6324.fn, data_020d631c.fn, data_020d6314.fn, data_020d630c.fn, reinterpret_cast<void (Unk_02005e7c::*)(s32)>(&PlayerActor::netLidClosed),
        reinterpret_cast<void (Unk_02005e7c::*)(s32)>(&PlayerActor::netErrorMessage), reinterpret_cast<void (Unk_02005e7c::*)(s32)>(&PlayerActor::netHoldUpItem), reinterpret_cast<void (Unk_02005e7c::*)(s32)>(&Unk_02007694::netLowerHeldUpItem), reinterpret_cast<void (Unk_02005e7c::*)(s32)>(&PlayerActor::netTaxiGetOut), reinterpret_cast<void (Unk_02005e7c::*)(s32)>(&PlayerActor::netTaxiGetIn), data_020d62d4.fn,
        data_020d62cc.fn, data_020d62c4.fn, data_020d62bc.fn, data_020d62b4.fn, data_020d62ac.fn, data_020d62a4.fn,
        reinterpret_cast<void (Unk_02005e7c::*)(s32)>(&Unk_02007694::netWaitMenu), reinterpret_cast<void (Unk_02005e7c::*)(s32)>(&Unk_02007694::netAct91), reinterpret_cast<void (Unk_02005e7c::*)(s32)>(&Unk_02007694::netAct92),
    };
    (this->*tbl[st])(idx);
}

void Unk_02005e7c::processRequests() {
    using namespace nC;
    handleNetEvent();
    s32 i;
    s32 *p;
    for (i = 0; i < 30; i++) {
        p = (s32 *)((PlayerActor *)(this))->getRequest(i);
        if (p == NULL) {
            break;
        }
        if (p[1] > actionPriority) {
            ((PlayerActor *)(this))->changeAction((PlayerActionRequest *)(p));
        }
    }
    ((PlayerActor *)(this))->clearRequests();
    ((PlayerActor *)(this))->clearActionFlag(0x17);
}

namespace nC {
extern "C" void PlayerActor_LevelTiltForAction(void *p, u32 i, s32 force) {
    u32 v;
    if (force != 0) {
        v = 1;
    } else {
        v = sPlayerActionLevelsTilt[i];
    }
    if (v != 0) {
        ((PlayerActor *)p)->approachRotX(0);
    }
}
}

void Unk_02005e7c::syncInputMode(u32 i) {
    using namespace nC;
    if (data_020c6434[i] != 0) {
        switch (inputMode) {
        case 1:
            InputMode_SetButtons();
            break;
        case 2:
            InputMode_SetTouch();
            break;
        default:
            InputMode_Clear();
            break;
        }
    } else {
        ((PlayerActor *)(this))->loadInputMode();
    }
}

void Unk_02005e7c::setShadowForAction(u32 i) {
    using namespace nC;
    if (data_020c64c8[i] == 0) {
        shadowSize = 0;
    } else {
        shadowSize = 0xb33;
    }
}

void PlayerActor::doExecute() {
    using namespace nB;
    VecFx32 v;
    u32 flags = ((PlayerActor *)this)->actorFlags;
    BOOL f4 = (flags & 4) != 0;
    if (f4) {
        BOOL f2 = (flags & 2) != 0;
        if (f2) {
            VecFx32 *p = (VecFx32 *)&((PlayerActor *)this)->position;
            v = *p;
            *(VecFx32 *)&((PlayerActor *)this)->footPosA = *(VecFx32 *)&((PlayerActor *)this)->footPosB = *(VecFx32 *)&((PlayerActor *)this)->headTopPos = *(VecFx32 *)&((PlayerActor *)this)->bodyPos = *p;
            WorldCurve_ToCurved(&v, &v);
            ((PlayerActor *)this)->toolHandMtx.m[9] = ((PlayerActor *)this)->itemHandMtx.m[9] = ((PlayerActor *)this)->heldItemJointMtx.m[9] = ((PlayerActor *)this)->heldItemJointMtx2.m[9] = v.x;
            ((PlayerActor *)this)->toolHandMtx.m[10] = ((PlayerActor *)this)->itemHandMtx.m[10] = ((PlayerActor *)this)->heldItemJointMtx.m[10] = ((PlayerActor *)this)->heldItemJointMtx2.m[10] = v.y;
            ((PlayerActor *)this)->toolHandMtx.m[11] = ((PlayerActor *)this)->itemHandMtx.m[11] = ((PlayerActor *)this)->heldItemJointMtx.m[11] = ((PlayerActor *)this)->heldItemJointMtx2.m[11] = v.z;
        }
    }
    ((PlayerActor *)(this))->pollFaceItemLoad();
    ((PlayerActor *)(this))->pollHatLoad();
    void *game = gCommManager;
    if (((CommManager *)(game))->isLocalSlot(((PlayerActor *)this)->sessionSlot)) {
        ((PlayerActor *)(this))->readInput();
        if (!((PlayerActor *)(this))->getInputMagnitude()) {
            ((PlayerActor *)this)->lastInputSide = 0;
        }
    } else if (!this->testActionFlag(0x10)) {
        ((Unk_02005e7c *)(this))->followNetAction();
    }
    ((Unk_02005e7c *)(this))->processRequests();
    HeldItemModel_Update(&((PlayerActor *)this)->heldItemModel);
    ((PlayerActor *)this)->sePosX = 0;
    ((PlayerActor *)this)->sePosY = 0;
    ((PlayerActor *)this)->sePosZ = 0;
    static void (PlayerActor::*tbl[147])() = {
        &PlayerActor::mainInit, &PlayerActor::mainAct01, &PlayerActor::mainWait, reinterpret_cast<void (PlayerActor::*)()>(&Unk_02007694::mainWalk), reinterpret_cast<void (PlayerActor::*)()>(&Unk_02007694::mainSkidTurn), reinterpret_cast<void (PlayerActor::*)()>(&Unk_02007694::mainAct05),
        data_020d622c.fn, &PlayerActor::mainChangeClothes, data_020d621c.fn, data_020d6214.fn, data_020d619c.fn, data_020d6204.fn,
        data_020d61fc.fn, data_020d61f4.fn, data_020d61c4.fn, data_020d61dc.fn, &PlayerActor::mainAct10, data_020d658c.fn,
        data_020d6b94.fn, &PlayerActor::mainAct13, &PlayerActor::mainEmotion, &PlayerActor::mainAct15, data_020d660c.fn, data_020d672c.fn,
        &PlayerActor::mainPickUpReach, &PlayerActor::mainPickUp, &PlayerActor::mainPickUpFanfare, &PlayerActor::mainPickUpFanfareStow, data_020d617c.fn, data_020d6174.fn,
        data_020d616c.fn, data_020d6164.fn, data_020d615c.fn, data_020d6154.fn, data_020d614c.fn, data_020d6144.fn,
        data_020d613c.fn, data_020d6134.fn, data_020d612c.fn, data_020d64bc.fn, data_020d64ac.fn, data_020d6114.fn,
        data_020d5e5c.fn, data_020d6104.fn, data_020d60fc.fn, data_020d60f4.fn, data_020d60ec.fn, data_020d60e4.fn,
        &PlayerActor::mainAct30, &PlayerActor::mainAct31, &PlayerActor::mainAct32, &PlayerActor::mainAct33, &PlayerActor::mainAct34, &PlayerActor::mainAct35,
        data_020d60ac.fn, data_020d60a4.fn, data_020d609c.fn, data_020d6094.fn, data_020d5eac.fn, data_020d6084.fn,
        data_020d607c.fn, data_020d6074.fn, data_020d606c.fn, &PlayerActor::mainChangeHeldItem, data_020d605c.fn, data_020d6054.fn,
        data_020d604c.fn, data_020d6044.fn, data_020d603c.fn, data_020d6034.fn, data_020d602c.fn, data_020d6024.fn,
        data_020d601c.fn, data_020d6014.fn, data_020d600c.fn, data_020d6284.fn, data_020d5fc4.fn, data_020d625c.fn,
        data_020d6254.fn, data_020d5fe4.fn, data_020d5fec.fn, data_020d6194.fn, data_020d61a4.fn, data_020d61bc.fn,
        data_020d61cc.fn, data_020d61ec.fn, data_020d6594.fn, data_020d65f4.fn, data_020d668c.fn, data_020d698c.fn,
        data_020d6d9c.fn, data_020d5f84.fn, data_020d5f7c.fn, data_020d5f74.fn, data_020d5f6c.fn, data_020d5f64.fn,
        data_020d5f5c.fn, data_020d6124.fn, data_020d5e64.fn, data_020d5f44.fn, data_020d5f3c.fn, data_020d5f34.fn,
        data_020d5f2c.fn, data_020d5f24.fn, data_020d5f1c.fn, data_020d5f14.fn, data_020d5eb4.fn, data_020d5f04.fn,
        data_020d5efc.fn, data_020d5ef4.fn, data_020d5ed4.fn, &PlayerActor::mainWalkTo, &PlayerActor::mainTurnTo, data_020d5f9c.fn,
        data_020d5fa4.fn, data_020d6004.fn, data_020d6264.fn, data_020d618c.fn, &PlayerActor::mainAct76, &PlayerActor::mainAct77,
        data_020d659c.fn, &PlayerActor::mainAct79, data_020d6dcc.fn, data_020d5e84.fn, data_020d5e7c.fn, data_020d5e74.fn,
        data_020d64b4.fn, data_020d5e8c.fn, data_020d5e94.fn, data_020d5ea4.fn, data_020d5ebc.fn, &PlayerActor::mainLidClosed,
        &PlayerActor::mainErrorMessage, reinterpret_cast<void (PlayerActor::*)()>(&Unk_02007694::mainHoldUpItem), reinterpret_cast<void (PlayerActor::*)()>(&Unk_02007694::mainLowerHeldUpItem), &PlayerActor::mainTaxiGetOut, &PlayerActor::mainTaxiGetIn, data_020d65a4.fn,
        data_020d6994.fn, data_020d5e54.fn, data_020d611c.fn, data_020d5e9c.fn, data_020d5ec4.fn, data_020d5ee4.fn,
        reinterpret_cast<void (PlayerActor::*)()>(&Unk_02007694::mainWaitMenu), reinterpret_cast<void (PlayerActor::*)()>(&Unk_02007694::mainAct91), reinterpret_cast<void (PlayerActor::*)()>(&Unk_02007694::mainAct92),
    };
    void (PlayerActor::*fn)() = tbl[((PlayerActor *)this)->action];
    ((PlayerActor *)(this))->updateFaceAnims();
    (((PlayerActor *)this)->*fn)();
    PlayerActor_CheckSceneExit(((PlayerActor *)this)->sessionSlot);
    s32 state = ((PlayerActor *)this)->action;
    PlayerActor_LevelTiltForAction(this, state, 0);
    if (((CommManager *)(game))->isLocalSlot(((PlayerActor *)this)->sessionSlot)) {
        ((Unk_02005e7c *)(this))->syncInputMode(state);
    }
    ((Unk_02007694 *)(this))->calcModelMatrixCurved();
    ((PlayerActor *)(this))->applyFaceItemChange();
    ((PlayerActor *)(this))->applyHatChange();
    if (((CommManager *)(game))->isLocalSlot(((PlayerActor *)this)->sessionSlot)) {
        BOOL z = (gFieldSceneKind == 0);
        if (z) {
            PlayerActor_FieldCheckUnitAhead(((PlayerActor *)this));
            PlayerActor_FieldRunStep(((PlayerActor *)this));
            PlayerActor_FieldCheckStepUnit(((PlayerActor *)this));
            PlayerActor_FieldUpdateTan(((PlayerActor *)this));
        }
        if (((PlayerActor *)this)->tripCooldown != 0) {
            ((PlayerActor *)this)->tripCooldown--;
        }
    }
    if (((PlayerActor *)this)->netPickUpDelay != 0) {
        ((PlayerActor *)this)->netPickUpDelay--;
    }
    ((PlayerActor *)(this))->pollStoreQuery();
    ((PlayerActor *)this)->fieldAnswer = ((PlayerActor *)(this))->pollFieldQuery();
    if (((CommManager *)(game))->isLocalSlot(((PlayerActor *)this)->sessionSlot)) {
        BOOL z = (gFieldSceneKind == 0);
        if (z) {
            PlayerActor_FieldPollPitfall(((PlayerActor *)this));
        }
    }
    ((PlayerActor *)(this))->updateHeadLook();
    if (((PlayerActor *)(this))->testActionFlag(0xa)) {
        if (((PlayerActor *)this)->sePosX == 0 && ((PlayerActor *)this)->sePosY == 0 && ((PlayerActor *)this)->sePosZ == 0) {
            ((PlayerActor *)this)->sePosX = ((PlayerActor *)this)->position.x;
            ((PlayerActor *)this)->sePosY = ((PlayerActor *)this)->position.y;
            ((PlayerActor *)this)->sePosZ = ((PlayerActor *)this)->position.z;
        }
        if (((PlayerActor *)(this))->testActionFlag(0x19)) {
            ((SndSeEmitter *)(&((PlayerActor *)this)->seEmitterLocal))->callUpdateRelative((VecFx32 *)(&((PlayerActor *)this)->sePosX));
        } else {
            ((SndSeEmitter *)(&((PlayerActor *)this)->seEmitterRemote))->callUpdateRelativeAlt((VecFx32 *)(&((PlayerActor *)this)->sePosX));
        }
    }
    if (((PlayerActor *)this)->prevAction != ((PlayerActor *)this)->action) {
        ((PlayerActor *)this)->prevAction = ((PlayerActor *)this)->action;
    }
}

namespace nB {
extern "C" void PlayerActor_JointCbPre(NNSG3dRS *p) {
    PlayerActor *obj = (PlayerActor *)p->pRenderObj->ptrUser;
    if (obj) {
        ((TwoLayerAnimModel *)(&obj->bodyModel))->onJointCalcPreLayer2(p);
    }
    p->cbVecFunc[6] = (void *)PlayerActor_JointCbPost;
    p->cbVecTiming[6] = 2;
}
}

namespace nB {
extern "C" void PlayerActor_JointCbPost(NNSG3dRS *p) {
    MtxFx33 *pm;
    MtxFx33 m2;
    MtxFx33 m1;
    VecFx32 v1;
    MtxFx33 m3;
    Mtx43 c;
    VecFx32 t3;
    VecFx32 v2;
    PlayerActor *o;
    PlayerActor *r6;
    JointCb_UseRestTranslation(p, 0);
    o = (PlayerActor *)p->pRenderObj->ptrUser;
    if (p->c[1] == 0xf && o != 0) {
        s16 a = o->headPitch;
        if (a != 0 || o->headYaw != 0) {
            pm = &p->pJntAnmResult->rot;
            s32 i = (u16)a >> 4;
            MTX_RotY33_(&m1, data_02135f44[i * 2], data_02135f44[i * 2 + 1]);
            MTX_Concat33(pm, &m1, pm);
            i = (u16)o->headYaw >> 4;
            MTX_RotX33_(&m2, data_02135f44[i * 2], data_02135f44[i * 2 + 1]);
            MTX_Concat33(pm, &m2, pm);
        }
    }
    r6 = (PlayerActor *)p->pRenderObj->ptrUser;
    if (r6) {
        ((TwoLayerAnimModel *)(&r6->bodyModel))->onJointCalcPostLayer2(p);
        if (p->c[1] == 0) {
            if ((u32)(r6->animId - 0x82) <= 1) {
                NNSG3dJntAnmResult *r = p->pJntAnmResult;
                pm = (MtxFx33 *)&r->rot;
                VecFx32 *pv = (VecFx32 *)&r->trans;
                v1 = *pv;
                v2 = *pv;
                s32 ang = WorldCurve_Apply(&v1, &v2);
                pv->z = v1.z;
                pv->y = v1.y - WorldCurve_GetRadius();
                if (ang != 0) {
                    s32 i = (u16)ang >> 4;
                    MTX_RotX33_(&m3, data_02135f44[i * 2], data_02135f44[i * 2 + 1]);
                    MTX_Concat33(pm, &m3, pm);
                }
            }
            JointCb_CalcCpuMatrix(&r6->bodyModel, p, 0);
            c = data_021cb69c;
            t3.x = c.m[9];
            t3.y = ((PlayerActor *)(r6))->getJointGroundY(c.m[10]);
            t3.z = c.m[11];
            WorldCurve_FromCurved(&r6->bodyPos, &t3);
        }
    }
    p->cbVecFunc[6] = (void *)PlayerActor_JointCbStart;
    p->cbVecTiming[6] = 3;
}
}

namespace nB {
extern "C" void PlayerActor_JointCbStart(NNSG3dRS *p) {
    p->cbVecFunc[6] = (void *)PlayerActor_JointCbPre;
    p->cbVecTiming[6] = 1;
}
}

void PlayerActor::drawNotReady() {
    using namespace nB;
    VecFx32 *p = (VecFx32 *)&((PlayerActor *)this)->position;
    *(VecFx32 *)&((PlayerActor *)this)->bodyPos = *p;
}

s32 PlayerActor::getJointGroundY(s32 x) {
    using namespace nB;
    BOOL b = (gFieldSceneKind == 0);
    if (b || this->testActionFlag(3) == 0) {
        return x;
    }
    return Ground_GetDefaultY();
}

void PlayerActor::doDraw() {
    using namespace nA;
    typedef void (PlayerActor::*Fn)();
    static Fn table[2] = {
        &PlayerActor::drawNotReady, &PlayerActor::drawReady,
    };
    Fn fn = table[drawStep];
    (this->*fn)();
    if (((PlayerActor *)(this))->testActionFlag(0xd)) {
        BOOL c = gFieldSceneKind == 0 ? TRUE : FALSE;
        if (c == TRUE) {
            VecFx32Copy a[2];
            a[0].x = shownItemPos.x;
            a[0].y = shownItemPos.y;
            a[0].z = shownItemPos.z;
            a[1].x = shownItemScaleX;
            a[1].y = shownItemScaleY;
            a[1].z = shownItemScaleZ;
            Field_DrawItemModel(shownItem, &a[0], &a[1], 0, 0, 0);
        } else {
            VecFx32Copy b[2];
            b[0].x = shownItemPos.x;
            b[0].y = shownItemPos.y;
            b[0].z = shownItemPos.z;
            b[1].x = shownItemScaleX;
            b[1].y = shownItemScaleY;
            b[1].z = shownItemScaleZ;
            RoomItemIcons_DrawItem(shownItem, &b[0], &b[1], 0, 0, 0);
        }
    }
}

BOOL PlayerActor::doDelete() {
    using namespace nA;
    if (action == 0x6d && actionWork != -1) {
        Effect_End(actionWork);
    }
    PlayerSession_ClearActor(sessionSlot);
    void *r5 = gCommManager;
    if (((CommManager *)(r5))->isOnline() && ((CommManager *)(r5))->isLocalSlot(sessionSlot)) {
        CommSyncVar_SetVar(sessionSlot + 8, 0, 0, 0);
    }
    ((MatTexVramTask *)(&shirtTexUpload))->cancel();
    ((MatTexPatAnim *)(&eyeTexAnim))->release();
    ((MatTexPatAnim *)(&mouthTexAnim))->release();
    ((CachedModel *)(&faceItemModel))->release();
    ((CachedModel *)(&headModel0))->release();
    ((CachedModel *)(&headModel1))->release();
    ((AnimModel *)(&bodyModel))->detachJointAnim();
    ((CachedModel *)(&bodyModel))->release();
    HeldItemModel_Release(&heldItemModel);
    PlayerGlassesModelRef_Release(&faceItemRef.slot);
    faceItemState = 2;
    PlayerHead_Release(&headRef.slot);
    ((CharaClothTexRef *)(&shirtTex))->release();
    PlayerBodyModelRef_CancelTexUpload(&bodyModelRef.slot);
    PlayerSession_ClearGfxSlot(sessionSlot);
    if (((PlayerActor *)(this))->testActionFlag(0x1a)) {
        Bgm_Release(0x3f);
        ((PlayerActor *)(this))->clearActionFlag(0x1a);
    }
    if (((PlayerActor *)(this))->testActionFlag(0xa)) {
        if (((PlayerActor *)(this))->testActionFlag(0x19)) {
            ((SndSeEmitter *)(&seEmitterLocal))->callStop();
        } else {
            ((SndSeEmitter *)(&seEmitterRemote))->callStop();
        }
        ((PlayerActor *)(this))->clearActionFlag(0xa);
    }
    if (((PlayerActor *)(this))->testActionFlag(0x1b)) {
        ((PlayerActor *)(this))->clearActionFlag(0x1b);
        FieldInfoBalloon_ClearNetMsg();
    }
    return TRUE;
}

ItemId::~ItemId() {}

PlayerActor::PlayerActor()
    : actionItem(0xfff1), shownItem(0xfff1), aheadUnitX(0), aheadUnitZ(0), runUnitX(0), runUnitZ(0), pitfallUnitX(0), pitfallUnitZ(0),
      pendingEventUnitX(0), pendingEventUnitZ(0) {
    using namespace nA;
    sessionSlot = 4;
    animId = 0xa1;
    handPose = 0x144;
    eyeAnimId = 0x16f;
    mouthAnimId = 0x16f;
    inputMode = 2;
    enum Unk_020d6df4_M1 { Unk_020d6df4_M1_V = -1 };
    Unk_020d6df4_M1 m1 = Unk_020d6df4_M1_V;
    bestRequest = m1;
    faceItemState = 2;
    hatState = 2;
    lastNetAction = 0x93;
    netSeq = m1;
    netSeqAction = 0x93;
}

PlayerActor::~PlayerActor() {
    using namespace nA;}

BOOL PlayerActor::onCreate() {
    using namespace nA;
    return ((PlayerActor *)(this))->doCreate();
}

BOOL PlayerActor::onExecute() {
    using namespace nA;
    ((PlayerActor *)(this))->doExecute();
    return TRUE;
}

BOOL PlayerActor::onDraw() {
    using namespace nA;
    doDraw();
    return TRUE;
}

BOOL PlayerActor::onDelete() {
    using namespace nA;
    return doDelete();
}

void *PlayerActor::operator new(size_t size) {
    using namespace nA;
    Heap *heap = (Heap *)gPlayerActorHeap;
    void *p = Heap_AllocAligned(heap, size, PlayerActor_GetObjectAlign());
    if (p == 0) {
        return 0;
    }
    memset(p, 0, size);
    return p;
}

void PlayerActor::operator delete(void *p) {
    using namespace nA;
    Heap_Free((Heap *)gPlayerActorHeap, p);
}

namespace nZ {
extern "C" {
extern const u8 data_020c6190[4] = {
    0x00, 0x01, 0x02, 0x03,
};
extern const u8 data_020c6194[4] = {
    0x00, 0x01, 0x02, 0x03,
};
extern const u8 data_020c6198[4] = {
    0x00, 0x01, 0x02, 0x03,
};
extern const u8 data_020c619c[4] = {
    0x00, 0x01, 0x02, 0x03,
};
extern const u8 data_020c61a0[4] = {
    0x00, 0x01, 0x02, 0x03,
};
extern const u8 data_020c61a4[4] = {
    0x00, 0x01, 0x02, 0x03,
};
extern const u8 data_020c61a8[4] = {
    0x00, 0x01, 0x02, 0x03,
};
extern const u8 data_020c61ac[4] = {
    0x00, 0x01, 0x02, 0x03,
};
extern const u8 data_020c61b0[4] = {
    0x00, 0x01, 0x02, 0x03,
};
extern const u8 data_020c61b4[4] = {
    0x00, 0x01, 0x02, 0x03,
};
extern const u8 data_020c61b8[8] = {
    0x00, 0x09, 0x01, 0x0a, 0x02, 0x0b, 0x03, 0x0c,
};
extern const u16 data_020c61c0[8] = {
    0x003b, 0x003a, 0x003c, 0x003d, 0x003b, 0x003b, 0x003b, 0x003b,
};
extern const u8 data_020c61d0[32] = {
    0x0d, 0x08, 0x0a, 0x09, 0x0a, 0x07, 0x09, 0x0b, 0x0a, 0x0c, 0x0a, 0x09, 0x08, 0x08, 0x0d, 0x0c,
    0x0a, 0x0e, 0x0d, 0x0c, 0x0b, 0x0e, 0x0c, 0x0c, 0x0e, 0x07, 0x0b, 0x0c, 0x0b, 0x00, 0x00, 0x00,
};
extern const u32 sPlayerAct76Anims[8] = {
    0x00000087, 0x00000089, 0x00000086, 0x00000086, 0x00000086, 0x00000086, 0x00000086, 0x00000086,
};
extern const u32 data_020c6210[11] = {
    0x00000000, 0x00000002, 0x00000001, 0x00000000, 0x00000003, 0x00000004, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000,
};
extern const u32 sPlayerReachDist[15] = {
    0x0000119a, 0x00001333, 0x0000119a, 0x00003000, 0x00002800, 0x0000119a, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x0000119a, 0x0000119a, 0x00001000, 0x0000119a, 0x000010cd,
};
extern const u8 sPlayerActionStopsMovement[148] = {
    0x01, 0x01, 0x01, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x01, 0x01, 0x00, 0x00, 0x01, 0x00, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00,
    0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x01, 0x01,
    0x01, 0x01, 0x00, 0x00,
};
extern const u8 sPlayerActionResetsRotX[148] = {
    0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x01,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00,
    0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x01, 0x01,
    0x00, 0x01, 0x00, 0x00,
};
extern const u8 sPlayerActionLevelsTilt[148] = {
    0x00, 0x00, 0x01, 0x00, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x00, 0x00, 0x00,
};
extern const u8 data_020c6434[148] = {
    0x00, 0x00, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x01, 0x00, 0x00,
};
extern const u8 data_020c64c8[148] = {
    0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x00, 0x00,
};
extern const u8 sPlayerActionColliderFlag2[148] = {
    0x01, 0x01, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x00, 0x00, 0x00, 0x00,
};
extern const u8 sPlayerActionTalkable[148] = {
    0x00, 0x00, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x01,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x00, 0x00, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00,
    0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x00, 0x00,
    0x00, 0x01, 0x01, 0x01, 0x00, 0x00, 0x01, 0x01, 0x00, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
    0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x00, 0x00, 0x00,
};
extern const u8 sPlayerActionIsLocomotion[148] = {
    0x00, 0x00, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
};
extern const u8 sPlayerActionKeepsBgCheckWork[148] = {
    0x00, 0x00, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
    0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x00, 0x00, 0x00,
};
extern const u8 sPlayerActionPriority[148] = {
    0x09, 0x09, 0x00, 0x00, 0x02, 0x04, 0x04, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05,
    0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x04, 0x04, 0x05,
    0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05,
    0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x00, 0x05, 0x05, 0x05,
    0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05,
    0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05,
    0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x06, 0x06, 0x04,
    0x04, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05,
    0x05, 0x05, 0x05, 0x00, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05,
    0x00, 0x09, 0x00, 0x00,
};
extern const u8 sPlayerActionDonePriority[148] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x04, 0x04, 0x05, 0x00, 0x00, 0x05, 0x05, 0x05, 0x00, 0x05,
    0x04, 0x04, 0x04, 0x00, 0x04, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x03, 0x03, 0x00, 0x00, 0x00, 0x05, 0x05, 0x05, 0x00, 0x05, 0x05, 0x00, 0x00, 0x05, 0x05, 0x00,
    0x05, 0x04, 0x05, 0x05, 0x05, 0x04, 0x04, 0x04, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x05, 0x00, 0x00,
    0x05, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x05, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x05, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x05, 0x00, 0x05, 0x05, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
};
extern const u16 sPlayerAnimResIndex[162] = {
    0x0000, 0x0001, 0x0002, 0x0003, 0x0004, 0x0005, 0x0006, 0x0007,
    0x0008, 0x0009, 0x000a, 0x000b, 0x000c, 0x000d, 0x000e, 0x000f,
    0x0010, 0x0011, 0x0012, 0x0013, 0x0035, 0x0036, 0x0037, 0x0038,
    0x0039, 0x003a, 0x0014, 0x0015, 0x0016, 0x0017, 0x0018, 0x0019,
    0x001a, 0x001b, 0x001c, 0x001d, 0x001e, 0x001f, 0x0020, 0x0021,
    0x0022, 0x0023, 0x0025, 0x0026, 0x0027, 0x0028, 0x002b, 0x002c,
    0x002d, 0x002e, 0x003b, 0x003c, 0x003d, 0x003e, 0x003f, 0x0040,
    0x002f, 0x0030, 0x0031, 0x0032, 0x0033, 0x0034, 0x0041, 0x0042,
    0x0043, 0x0044, 0x00ad, 0x00ae, 0x00af, 0x00b0, 0x00b1, 0x00b2,
    0x00b3, 0x00b4, 0x00b5, 0x00b6, 0x00b7, 0x00b8, 0x00b9, 0x00ba,
    0x00bb, 0x00bc, 0x00be, 0x00bf, 0x00c0, 0x00c1, 0x00c2, 0x00c3,
    0x00c4, 0x00c5, 0x00c6, 0x00c7, 0x00c8, 0x00c9, 0x00bd, 0x00ca,
    0x00cb, 0x00cc, 0x00cd, 0x00ce, 0x00cf, 0x00d0, 0x00d1, 0x00d2,
    0x00d3, 0x00d4, 0x00d5, 0x00d6, 0x00d7, 0x0106, 0x0107, 0x0108,
    0x0047, 0x0109, 0x010a, 0x010b, 0x010c, 0x010d, 0x010e, 0x010f,
    0x0110, 0x0111, 0x0112, 0x0113, 0x0114, 0x0115, 0x0116, 0x0117,
    0x0118, 0x0119, 0x011a, 0x011b, 0x011c, 0x005b, 0x011d, 0x011e,
    0x011f, 0x0120, 0x0121, 0x0122, 0x0123, 0x0124, 0x0125, 0x0126,
    0x0127, 0x0128, 0x0129, 0x012a, 0x012b, 0x012c, 0x012d, 0x012e,
    0x012f, 0x0130, 0x00da, 0x0131, 0x0132, 0x0133, 0x0134, 0x0135,
    0x0136, 0x0000,
};
extern const u32 data_020c6a18[147] = {
    0x00000001, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000001,
    0x00000000, 0x00000001, 0x00000001, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000001, 0x00000001, 0x00000001, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000001, 0x00000001, 0x00000001,
    0x00000000, 0x00000000, 0x00000001, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000001,
    0x00000000, 0x00000001, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000001, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000,
};
extern const s32 sPlayerFrontPointDist = 0x2000;
s32 data_020d5e34 = 0xccd;
s32 data_020d5e38 = 0xccd;
s32 data_020d5e3c = 0x5;
s32 data_020d5e40 = 0x19a;
s32 data_020d5e44 = 0x5;
s32 data_020d5e48 = 0xccd;
#pragma explicit_zero_data on
u32 data_020d5e4c = 0;
#pragma explicit_zero_data reset
s32 data_020d5e50 = 0x14000;
ActorProfile sPlayerActorProfile = {(void *(*)())PlayerActor_Create, 9, 0xd, 2, 0x800, 0x800, 0x2b000};
char sPlayerActorErrorMsgFile[] = "obj_etc_error";
char sPlayerActorMsgFile[] = "obj_etc_player";
char sPlayerActorGetInsectMsgFile[] = "obj_etc_getinsect";
char sPlayerActorGetFishMsgFile[] = "obj_etc_getfish";
char sPlayerClothTexName[] = "cloth";
char sPlayerSkinPalName[] = "skin_pl";
char sPlayerEyeTexName[] = "e.0";
char sPlayerMouthTexName[] = "m.0";
char sPlayerHairPalName[] = "hair_pl";
char sPlayerShirtTexName[] = "w";
}
}

#ifdef HOST64
// (modern/host/frontend.c's ACWW_EMOTION_TEST: the local player's action, body animation and emotion effects)
extern "C" int PlayerActor_HostEmotionState(void *obj, s32 *action, u32 *anim, u32 *mode, s32 *effects) {
    PlayerActor *o = (PlayerActor *)obj;
    if (o == NULL) return 0;
    *action = o->action;
    *anim = AnimSlotRef_GetAnimId(&o->bodyAnimSlot);
    *mode = o->bodyModel.playMode;
    for (int i = 0; i < 4; i++) effects[i] = o->emotionFx.effects[i];
    return 1;
}
#endif
