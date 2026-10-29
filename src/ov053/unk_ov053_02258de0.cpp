#include "types.h"
#include "host_shared_calls.h"
#include "host_overlay_vars.h"
#include "gfx/VecFx32.h"

// mwcc-flags: -str reuse
#include "types.h"
#include "item/ItemId.h"
#include "talk/TalkStartMsg.h"
#include "talk/TalkWindowState.h"
#include "npc/NpcAnimCtrl.h"
#include "npc/NpcSpeechState.h"
#include "npc/NpcResHandle.h"
#include "npc/NpcObstacleProbe.h"
#include "npc/NpcMoveCtrl.h"
#include "npc/Unk_0201ad18.h"
#include "npc/NpcFootstepFx.h"
#include "sys/ProcBase.h"
#include "snd/SndSeEmitterKind1.h"
#include "npc/NpcTalkCtrl.h"
#include "actor/Actor.h"
#include "actor/Character.h"
#include "talk/ChoiceList.h"
#include "gfx/ThreeLayerAnimModel.h"
#include "talk/TalkMsgRequest.h"
#include "talk/ActorTalkRequest.h"
#include "actor/NpcActor.h"
#include "actor/SpNpcActor.h"
#include "talk/ActorTalkRequest.h"
#include "talk/SpNpcTalkRequest.h"
#include "actor/ActorProfile.h"
#include "net/CommManager.h"
#include "npc/NpcActionCtrl.h"
#include "npc/NpcLookAt.h"
#include "player/PlayerData.h"
#include "player/PlayerId.h"
#include "player/PlayerSpNpcRecord.h"
#include "proto/main/unk_02002b1c.h"
#include "proto/main/unk_020039ec.h"
#include "proto/main/unk_020119cc.h"
#include "proto/main/unk_02034438.h"
#include "proto/main/unk_02063904.h"
#include "proto/main/unk_020850e0.h"
#include "proto/main/unk_02089fbc.h"
#include "proto/main/unk_020943dc.h"
#include "proto/main/unk_02094810.h"
#include "proto/main/unk_02097444.h"
#include "proto/ov004/unk_ov004_0221e7a8.h"
#include "proto/ov004/unk_ov004_02225290.h"
#include "proto/ov053/unk_ov053_02258de0.h"
#include "proto/main/unk_020b4828.h"
#include "proto/main/unk_0204cc1c.h"



class ActorTalkRequest;
class SpNpcHarriet;
class SpNpcHarrietTalk;

struct SpNpcHarrietVecLocal : VecFx32 {
    SpNpcHarrietVecLocal() {}
};















class SpNpcHarrietTalk : public SpNpcTalkRequest {
public:
    SpNpcHarrietTalk();
    virtual ~SpNpcHarrietTalk();
    virtual void onMessageEnd(u32 attr);
    virtual void onChoice(u32 attr);
    virtual void onWindowClose();
    virtual void onTalkEnd();
    virtual void start(TalkStartMsg *out);

    s32 getQuestionsStartMsg();
    s32 getTopic();
    void setTopic(s32 v);
    void attachOwner(SpNpcHarriet *o);
    u8 getNewHairColor();
    u8 getNewHairStyle();

    /* 0xac */ s32 topic;
    /* 0xb0 */ SpNpcHarriet *owner;
    /* 0xb4 */ u8 crossGenderStyle;
    /* 0xb5 */ u8 haircutTalkStep;
    /* 0xb6 */ u8 newHairStyle;
    /* 0xb7 */ u8 newHairColor;
    /* 0xb8 */ u8 oldHairStyle;
    /* 0xb9 */ u8 oldHairColor;
    /* 0xba */ u8 pad_ba[2];
};

class SpNpcHarriet : public SpNpcActor {
public:
    SpNpcHarriet() {}
    virtual BOOL onCreate();
    virtual BOOL preCreate();
    virtual BOOL acceptsInteraction(void *other);
    virtual void onInteractionEvent(u32 cmd, u8 arg);
    virtual BOOL acceptsSelfRequestedInteraction(void *a);
    virtual BOOL updateAct();
    virtual u8 *getTexturePath();
    virtual u8 *getModelPath();

    s32 isSessionPaid();
    BOOL isWearingHeadItem();
    BOOL tryFarewellTalk();
    BOOL tryLeavePaidTalk();
    BOOL tryChairTalk();
    BOOL mainAct0A();
    BOOL setupAct0A();
    BOOL mainAct09();
    BOOL setupAct09();
    BOOL mainAct08();
    BOOL setupAct08();
    BOOL mainAct07();
    BOOL setupAct07();
    BOOL mainAct06();
    BOOL setupAct06();
    BOOL mainAct05();
    BOOL setupAct05();
    BOOL mainAct04();
    BOOL setupAct04();
    BOOL mainAct03();
    BOOL setupAct03();
    BOOL mainAct02();
    BOOL setupAct02();
    BOOL mainAct01();
    BOOL setupAct01();
    BOOL mainAct00();
    BOOL setupAct00();
    void changeAct(s32 state);

    /* 0x654 */ s32 act;
    /* 0x658 */ SpNpcHarrietTalk talk;
    /* 0x714 */ u8 pushBackStep;
    /* 0x715 */ u8 routeIndex;
    /* 0x716 */ u8 haircutTimer;
    /* 0x717 */ u8 bgmReleaseTimer;
    /* 0x718 */ s16 homeAngle;
};

struct SpNpcHarrietActEntry {
    BOOL (SpNpcHarriet::*enter)();
    BOOL (SpNpcHarriet::*exit)();
};


extern "C" {
extern const u8 sSpNpcHarrietCoolColors[];
extern const u8 sSpNpcHarrietWarmColors[];
extern const VecFx32 sSpNpcHarrietExitLine;
extern const VecFx32 sSpNpcHarrietReturnPos;
extern const VecFx32 data_ov053_0225a1a0;
extern const u8 sSpNpcHarrietStyleTable[];
extern const VecFx32 sSpNpcHarrietRouteToStation[2];
extern const VecFx32 sSpNpcHarrietRouteToCustomer[2];
extern const void *sSpNpcHarrietMsgKey;
extern ActorProfile sSpNpcHarrietProfile;
extern SpNpcHarrietActEntry sSpNpcHarrietActTable[11];

extern volatile u16 data_020c6cc8;
extern s32 data_020c6d1c;
extern u8 gVec3Zero[];
extern void *gCommManager;
extern u8 gTalkMsgIndexEnd[];

NpcActor *func_02015aac(void *self);
s32 Math_CountDownU8(void *);
BOOL Camera_SetMode16At(VecFx32 *v);
void TalkRequest_SetTargetDone(void *self);
void TalkRequest_AddPlayerTalk6(void *self, s32 a);
void Camera_SetModeDefault();
void Ground_LockExit(s32 a);
s32 Scene_GetPrevious();
s32 Ground_IsOnLockedExit(void *p);
BOOL TalkRequest_IsTalking(void);
s32 TalkRequest_AddPlayerTalk7(void *p, s32 a);
s32 func_020aa514(void *p);
s32 Talk_CheckAndSetPlayerFlag(s32 a, s32 b);
}

// Constants and tables (definition order sets the data layout)
typedef BOOL (SpNpcHarriet::*SpNpcHarrietActFn)();

extern "C" ActorProfile sSpNpcHarrietProfile = {(void *(*)())SpNpcHarriet_Create, 0x61, 0x68, 2, 0x5000, 0x5000, 0x3e800};
extern "C" const void *sSpNpcHarrietMsgKey = sSpNpcHarrietKey;
extern "C" const VecFx32 data_ov053_0225a1a0 = {0x300, 0x199, 0x199};
extern "C" const VecFx32 sSpNpcHarrietRouteToCustomer[2] = {{0xe800, 0, 0x15800}, {0xc000, 0, 0x15000}};
extern "C" const u8 sSpNpcHarrietWarmColors[] = {1, 4, 2, 6};
extern "C" const VecFx32 sSpNpcHarrietExitLine = {0xb000, 0, 0x1d000};
extern "C" u8 sSpNpcHarrietTexturePath[] = "npc_sp/model/poo_tex.nsbtx";
extern "C" char sSpNpcHarrietKey[] = "sp_npc_barber";
extern "C" const u8 sSpNpcHarrietCoolColors[] = {0, 3, 5, 7};
extern "C" u8 sSpNpcHarrietModelPath[] = "npc_sp/model/poo.nsbmd";
extern "C" const VecFx32 sSpNpcHarrietRouteToStation[2] = {{0xe000, 0, 0x15000}, {0xf000, 0, 0x17000}};
extern "C" SpNpcHarrietActEntry sSpNpcHarrietActTable[11] = {
    {&SpNpcHarriet::setupAct00, &SpNpcHarriet::mainAct00},
    {&SpNpcHarriet::setupAct01, &SpNpcHarriet::mainAct01},
    {&SpNpcHarriet::setupAct02, &SpNpcHarriet::mainAct02},
    {&SpNpcHarriet::setupAct03, &SpNpcHarriet::mainAct03},
    {&SpNpcHarriet::setupAct04, &SpNpcHarriet::mainAct04},
    {&SpNpcHarriet::setupAct05, &SpNpcHarriet::mainAct05},
    {&SpNpcHarriet::setupAct06, &SpNpcHarriet::mainAct06},
    {&SpNpcHarriet::setupAct07, &SpNpcHarriet::mainAct07},
    {&SpNpcHarriet::setupAct08, &SpNpcHarriet::mainAct08},
    {&SpNpcHarriet::setupAct09, &SpNpcHarriet::mainAct09},
    {&SpNpcHarriet::setupAct0A, &SpNpcHarriet::mainAct0A},
};
extern "C" const u8 sSpNpcHarrietStyleTable[] = {3, 0xb, 2, 0xa, 6, 0xe, 4, 0xc, 7, 0xf, 0, 8, 1, 9, 5, 0xd};
extern "C" const VecFx32 sSpNpcHarrietReturnPos = {0x10000, 0, 0x1b000};

SpNpcHarriet *SpNpcHarriet_Create() { return new SpNpcHarriet; }

BOOL SpNpcHarriet::preCreate() {
    if (!SpNpcActor::preCreate()) {
        return FALSE;
    }
    setTalkRequest((ActorTalkRequest *)&talk);
    talk.attachOwner(this);
    setCollisionRadius(0x100);
    const VecFx32 *d = &data_ov053_0225a1a0;
    ((NpcMoveCtrl *)(&moveCtrl))->setSpeedPreset(2, d->x, d->y, d->z);
    return TRUE;
}

BOOL SpNpcHarriet::onCreate() {
    if (!SpNpcActor::onCreate()) {
        return FALSE;
    }
    void *g = gCommManager;
    if (((CommManager *)(g))->isOnline() == 0) {
        Ground_LockExit(0);
    }
    homeAngle = 0;
    collider.groups |= 2;
    routeIndex = 0;
    if (((CommManager *)(g))->isOnline()) {
        changeAct(1);
        return TRUE;
    }
    if (Scene_GetPrevious() == 0x1d) {
        changeAct(0);
    } else {
        changeAct(1);
    }
    return TRUE;
}

u8 *SpNpcHarriet::getTexturePath() { return sSpNpcHarrietTexturePath; }

u8 *SpNpcHarriet::getModelPath() { return sSpNpcHarrietModelPath; }

BOOL SpNpcHarriet::updateAct() {
    BOOL r = FALSE;
    if (sSpNpcHarrietActTable[act].exit) { // (the DS label is the table + one member: the exit member)
        r = (this->*sSpNpcHarrietActTable[act].exit)();
    }
    return r;
}

void SpNpcHarriet::changeAct(s32 state) {
    BOOL ok = TRUE;
    if (sSpNpcHarrietActTable[state].enter) {
        ok = (this->*sSpNpcHarrietActTable[state].enter)();
    }
    if (ok) {
        act = state;
    }
}

u8 SpNpcHarrietTalk::getNewHairStyle() {
    return newHairStyle;  // (DS offset 0xb6: elsewhere in the 64-bit object)
}

u8 SpNpcHarrietTalk::getNewHairColor() {
    return newHairColor;  // (DS offset 0xb7)
}

BOOL SpNpcHarriet::setupAct00() {
    talk.setTopic(0);
    return TRUE;
}

BOOL SpNpcHarriet::mainAct00() {
    TalkRequest_AddPlayerTalk6(this, 0);
    return TRUE;
}

BOOL SpNpcHarriet::setupAct01() {
    ((NpcActionCtrl *)(&actionCtrl))->requestAction(0, 1, 0, 0, 0, 0, 0, 0, data_020c6cc8, 0);
    return TRUE;
}

BOOL SpNpcHarriet::mainAct01() {
    if (tryChairTalk()) {
        return TRUE;
    }
    if (tryLeavePaidTalk()) {
        return TRUE;
    }
    tryFarewellTalk();
    return TRUE;
}

BOOL SpNpcHarriet::setupAct02() {
    ((NpcLookAt *)(&lookAt))->setTarget(1, 0, 0, (VecFx32 *)(gVec3Zero), 4, data_020c6d1c, 1);
    NpcActor *p = (NpcActor *)talk.getTalkPlayer();
    s32 r4 = 0;
    if (p) {
        r4 = getAngleTo(p);
    }
    if (talk.getTopic() == 5) {
        VecFx32 v;
        VecFx32 *pv = (VecFx32 *)&position;
        v.x = pv->x;
        v.y = pv->y;
        v.z = pv->z;
        v.y += 0x2000;
        Camera_SetMode16At(&v);
        bgmReleaseTimer = 0x1f;
    }
    if (talk.getTopic() == 5 || talk.getTopic() == 8 || talk.getTopic() == 10) {
        ((NpcTalkCtrl *)(&talkCtrl))->requestTurnAndTalk(0, r4, 1);
        return TRUE;
    } else {
        ((NpcTalkCtrl *)(&talkCtrl))->requestTurnAndTalk(0, r4, 0);
        return TRUE;
    }
}

BOOL SpNpcHarriet::mainAct02() {
    if (Math_CountDownU8(&bgmReleaseTimer) == 1) {
        Bgm_ReleasePriority(0xe);
    }
    if (((NpcTalkCtrl *)(&talkCtrl))->isBusy() == 0) {
        s32 prev = act;
        Hud_Show();
        if (talk.getTopic() == 4 || talk.getTopic() == 3) {
            changeAct(7);
        } else if (talk.getTopic() == 5 && isWearingHeadItem()) {
            talk.setTopic(9);
            changeAct(6);
            return TRUE;
        } else {
            if (PlayerActor_IsInAction(0x28, 4)) {
                if (PlayerActor_LocalRequestStandUp(0)) {
                    TalkRequest_SetTargetDone(this);
                    changeAct(3);
                }
            } else if (talk.getTopic() == 10) {
                if (((NpcTalkCtrl *)(&talkCtrl))->isBusy() == 0) {
                    SceneWarp_RequestExit(Scene_GetWarpRequest(), 0);
                    changeAct(3);
                }
            } else {
                TalkRequest_SetTargetDone(this);
                changeAct(3);
            }
        }
        if (prev != act) {
            if (talk.getTopic() == 5) {
                Camera_SetModeDefault();
            }
        }
    }
    return TRUE;
}

BOOL SpNpcHarriet::setupAct03() { return TRUE; }

BOOL SpNpcHarriet::mainAct03() { return TRUE; }

BOOL SpNpcHarriet::setupAct04() {
    ((NpcActionCtrl *)(&actionCtrl))->requestAction(3, 1, 0, 0, 0, homeAngle, 0, 0, data_020c6cc8, 0);
    return TRUE;
}

BOOL SpNpcHarriet::mainAct04() {
    if (((NpcActionCtrl *)(&actionCtrl))->getAction() == 3) {
        if (((NpcActionCtrl *)(&actionCtrl))->isActionDone()) {
            changeAct(1);
        }
    }
    return TRUE;
}

BOOL SpNpcHarriet::setupAct05() {
    routeIndex = 0;
    ((NpcActionCtrl *)(&actionCtrl))->requestAction(3, 1, 0, 0, 0, 0x4000, 0, 0, data_020c6cc8, 0);
    return TRUE;
}

BOOL SpNpcHarriet::mainAct05() {
    if (((NpcActionCtrl *)(&actionCtrl))->getAction() == 6) {
        const VecFx32 *r = &sSpNpcHarrietRouteToStation[routeIndex];
        VecFx32 v;
        v.x = r->x;
        v.y = r->y;
        v.z = r->z;
        ((NpcMoveCtrl *)(&moveCtrl))->setWaypoint(&v);
        if (((NpcActionCtrl *)(&actionCtrl))->isActionDone()) {
            routeIndex++;
            u32 off = routeIndex;
            ((NpcActionCtrl *)(&actionCtrl))->requestAction(6, 1, sSpNpcHarrietRouteToStation[off].x, sSpNpcHarrietRouteToStation[off].z, 0x800, 0, 0, 0, data_020c6cc8, 0);
            if (routeIndex >= 2) {
                changeAct(4);
            }
        }
    }
    if (((NpcActionCtrl *)(&actionCtrl))->getAction() == 3) {
        if (((NpcActionCtrl *)(&actionCtrl))->isActionDone()) {
            u32 off = routeIndex;
            ((NpcActionCtrl *)(&actionCtrl))->requestAction(6, 1, sSpNpcHarrietRouteToStation[off].x, sSpNpcHarrietRouteToStation[off].z, 0x800, 0, 0, 0, data_020c6cc8, 0);
        }
    }
    return TRUE;
}

BOOL SpNpcHarriet::setupAct06() {
    routeIndex = 0;
    VecFx32 v;
    const VecFx32 *p = sSpNpcHarrietRouteToCustomer;
    v.x = p->x;
    v.y = p->y;
    v.z = p->z;
    s32 r = Math_AngleXZ(&position, &v);
    ((NpcActionCtrl *)(&actionCtrl))->requestAction(3, 1, 0, 0, 0, r, 0, 0, data_020c6cc8, 0);
    return TRUE;
}

BOOL SpNpcHarriet::mainAct06() {
    if (((NpcActionCtrl *)(&actionCtrl))->getAction() == 6) {
        const VecFx32 *r = &sSpNpcHarrietRouteToCustomer[routeIndex];
        VecFx32 v;
        v.x = r->x;
        v.y = r->y;
        v.z = r->z;
        ((NpcMoveCtrl *)(&moveCtrl))->setWaypoint(&v);
        if (((NpcActionCtrl *)(&actionCtrl))->isActionDone()) {
            routeIndex++;
            u32 off = routeIndex;
            ((NpcActionCtrl *)(&actionCtrl))->requestAction(6, 1, sSpNpcHarrietRouteToCustomer[off].x, sSpNpcHarrietRouteToCustomer[off].z, 0x800, 0, 0, 0, data_020c6cc8, 0);
            if (routeIndex >= 2) {
                changeAct(2);
            }
        }
    }
    if (((NpcActionCtrl *)(&actionCtrl))->getAction() == 3) {
        if (((NpcActionCtrl *)(&actionCtrl))->isActionDone()) {
            u32 off = routeIndex;
            ((NpcActionCtrl *)(&actionCtrl))->requestAction(6, 1, sSpNpcHarrietRouteToCustomer[off].x, sSpNpcHarrietRouteToCustomer[off].z, 0x800, 0, 0, 0, data_020c6cc8, 0);
        }
    }
    return TRUE;
}

BOOL SpNpcHarriet::setupAct07() {
    ((NpcLookAt *)(&lookAt))->setTarget(0, 0, 0, (VecFx32 *)(gVec3Zero), 4, data_020c6d1c, 1);
    routeIndex = 0;
    ((NpcActionCtrl *)(&actionCtrl))->requestAction(3, 1, 0, 0, 0, 0x4000, 0, 0, data_020c6cc8, 0);
    return TRUE;
}

BOOL SpNpcHarriet::mainAct07() {
    if (((NpcActionCtrl *)(&actionCtrl))->getAction() == 6) {
        const VecFx32 *r = &sSpNpcHarrietRouteToStation[routeIndex];
        VecFx32 v;
        v.x = r->x;
        v.y = r->y;
        v.z = r->z;
        ((NpcMoveCtrl *)(&moveCtrl))->setWaypoint(&v);
        if (((NpcActionCtrl *)(&actionCtrl))->isActionDone()) {
            routeIndex++;
            u32 t = data_020c6cc8;
            u32 off = routeIndex;
            ((NpcActionCtrl *)(&actionCtrl))->requestAction(6, 1, sSpNpcHarrietRouteToStation[off].x, sSpNpcHarrietRouteToStation[off].z, 0x800, 0, 0, 0, data_020c6cc8, 0);
            if (routeIndex >= 2) {
                ((NpcActionCtrl *)(&actionCtrl))->requestAction(3, 1, 0, 0, 0, -0x4000, 0, 0, t, 0);
            }
        }
    }
    if (((NpcActionCtrl *)(&actionCtrl))->getAction() == 3) {
        if (((NpcActionCtrl *)(&actionCtrl))->isActionDone()) {
            u32 off = routeIndex;
            if (off >= 2) {
                changeAct(8);
            } else {
                    ((NpcActionCtrl *)(&actionCtrl))->requestAction(6, 1, sSpNpcHarrietRouteToStation[off].x, sSpNpcHarrietRouteToStation[off].z, 0x800, 0, 0, 0, data_020c6cc8, 0);
            }
        }
    }
    return TRUE;
}

BOOL SpNpcHarriet::setupAct08() {
    haircutTimer = 0x64;
    ((NpcActionCtrl *)(&actionCtrl))->requestPlayAnim(1, 0xf1, 1, data_020c6cc8, 0);
    return TRUE;
}

BOOL SpNpcHarriet::mainAct08() {
    if (Math_CountDownU8(&haircutTimer) == 0) {
        talk.setTopic(5);
        changeAct(2);
    }
    if (haircutTimer == 0x55) {
        u8 out[2];
        out[1] = talk.getNewHairColor();
        out[0] = talk.getNewHairStyle();
        PlayerActor_LocalRequestHaircutStart(&out[0], &out[1]);
        Shared_BarberMachine_Start();
        Snd_SeEmitterPlayOneShotAlt(&seEmitter, 0x41, 0x7f, 0);
    }
    if (((NpcAnimCtrl *)(&animCtrl))->isPlayingAnim(0xf1, &moveAnimSet)) {
        if (((NpcActionCtrl *)(&actionCtrl))->isActionDone()) {
            ((NpcActionCtrl *)(&actionCtrl))->requestAction(0, 1, 0, 0, 0, 0, 0, 0, data_020c6cc8, 0);
        }
    }
    return TRUE;
}

BOOL SpNpcHarriet::setupAct09() {
    pushBackStep = 0;
    return TRUE;
}

BOOL SpNpcHarriet::mainAct09() {
    PlayerData_GetCurrent();
    SpNpcHarrietVecLocal v;
    VecFx32 *src = (VecFx32 *)PlayerActor_GetBodyPos(4);
    *(VecFx32 *)&v = *src;
    s32 bx1 = 0, by1 = 0, bx2 = 0, by2 = 0;
    SpNpcHarrietVecLocal w;
    *(VecFx32 *)&w = sSpNpcHarrietReturnPos;
    s32 dx = w.x, dy = w.y, dz = w.z;
    FieldPos_ToUnit(&bx2, &by2, &w);
    FieldPos_ToUnit(&bx1, &by1, &v);
    switch (pushBackStep) {
    case 0:
        if (((NpcTalkCtrl *)(&talkCtrl))->isBusy() == 0) {
            Camera_SetModeDefault();
            pushBackStep = 1;
        }
        break;
    case 1: {
        VecFx32Ctor u;
        u.x = dx;
        u.y = dy;
        u.z = dz;
        PlayerActor_RequestWalkTo(&u, 0x266, 4);
        pushBackStep = 2;
        break;
    }
    case 2:
        if (PlayerActor_IsScriptedWalking(4) == 0) {
            TalkRequest_SetTargetDone(this);
            changeAct(4);
        }
        break;
    }
    return TRUE;
}

BOOL SpNpcHarriet::setupAct0A() {
    return TRUE;
}

BOOL SpNpcHarriet::mainAct0A() {
    return TRUE;
}

SpNpcHarrietTalk::SpNpcHarrietTalk() {}

SpNpcHarrietTalk::~SpNpcHarrietTalk() {}

void SpNpcHarrietTalk::attachOwner(SpNpcHarriet *o) {
    resetMsg();
    owner = o;
    topic = 0xe;
}

void SpNpcHarrietTalk::setTopic(s32 v) {
    topic = v;
}

// ---- member small functions (defined late so callers keep bl) ----

s32 SpNpcHarrietTalk::getTopic() {
    return topic;
}

// SpNpcHarrietTalk::start's static tbl, initialised at its first use after the overlay was loaded
TalkStartMsg SpNpcHarrietTalk_start_tbl[14];
u8 SpNpcHarrietTalk_start_tbl_init;
void SpNpcHarrietTalk::start(TalkStartMsg *out) {
    void *p = PlayerData_GetCurrent();
    if (!SpNpcHarrietTalk_start_tbl_init) {
        HOST_STATIC_INIT_ARRAY(SpNpcHarrietTalk_start_tbl, {
        {(const char *)sSpNpcHarrietMsgKey, 0x43}, {(const char *)sSpNpcHarrietMsgKey, 0},    {(const char *)sSpNpcHarrietMsgKey, 2},
        {(const char *)sSpNpcHarrietMsgKey, 0x40}, {(const char *)sSpNpcHarrietMsgKey, 0x3e}, {(const char *)sSpNpcHarrietMsgKey, 0xd},
        {(const char *)sSpNpcHarrietMsgKey, 0x3d}, {(const char *)sSpNpcHarrietMsgKey, 0x45}, {(const char *)sSpNpcHarrietMsgKey, 0x42},
        {(const char *)sSpNpcHarrietMsgKey, 0x41}, {(const char *)sSpNpcHarrietMsgKey, 0x46}, {(const char *)sSpNpcHarrietMsgKey, 5},
        {(const char *)sSpNpcHarrietMsgKey, 6},    {(const char *)sSpNpcHarrietMsgKey, 7},
    });
        SpNpcHarrietTalk_start_tbl_init = 1;
    }
    if (getTopic() == 4) {
        out->msgKey = (const char *)SpNpcHarrietTalk_start_tbl[topic].msgKey;
        out->msgIndex = getQuestionsStartMsg();
    } else {
        if (getTopic() != 0 && getTopic() != 3 && getTopic() != 5 &&
            getTopic() != 7 && getTopic() != 8 && getTopic() != 9 &&
            getTopic() != 10) {
            if (owner->isSessionPaid() == 0) {
                if (Talk_CheckAndSetPlayerFlag(0x13, 0) == 0) {
                    if (((PlayerData *)(p))->testFlag(0xb) == 0) {
                        setTopic(1);
                        ((PlayerData *)(p))->setFlag(0xb);
                    } else {
                        setTopic(2);
                    }
                } else {
                    setTopic(Random_GlobalBelow(3) + 0xb);
                }
            } else {
                setTopic(6);
            }
        }
        if (topic >= 0 && topic < 0xe) {
            out->msgIndex = SpNpcHarrietTalk_start_tbl[topic].msgIndex;
            out->msgKey = (const char *)SpNpcHarrietTalk_start_tbl[topic].msgKey;
        }
    }
}

void SpNpcHarrietTalk::onTalkEnd() {
    u32 t = msgIndex;
    if (t == 0xe || t == 0x37) {
        if ((t == 0xe && haircutTalkStep == 1) || (t == 0x37 && haircutTalkStep == 2)) {
            PlayerActor_LocalRequestSit();
            ((PlayerSpNpcRecord *)(PlayerData_GetCurrent()->getSpNpcRecord()))->addHaircutCount(1);
        }
        haircutTalkStep = haircutTalkStep + 1;
    }
}

void SpNpcHarrietTalk::onWindowClose() {
    if (msgIndex == 0xc) {
        Bgm_RequestSilence(0xe, 0x46, 0);
    }
}

void SpNpcHarrietTalk::onMessageEnd(u32) {
    void *p = PlayerData_GetCurrent();
    const void *tbl = sSpNpcHarrietMsgKey;
    u32 msg = 0xff;
    switch (msgIndex) {
    case 0xd:
        if (newHairStyle == oldHairStyle && newHairColor == oldHairColor) {
            msg = 0x37;
        } else {
            msg = 0xe;
        }
        haircutTalkStep = 0;
        break;
    case 0x17:
        crossGenderStyle = 1;
        break;
    case 0x18:
        crossGenderStyle = 0;
        break;
    case 0x2f:
    case 0x30:
    case 0x31:
    case 0x32:
    case 0x33:
    case 0x34:
    case 0x35:
    case 0x36: {
        s32 r = ((PlayerId *)(((PlayerData *)(p))->getPlayerId()))->getGender();
        if (crossGenderStyle != 0) {
            if (r == 0) {
                r = 1;
            } else {
                r = 0;
            }
        }
        const u8 *q = &sSpNpcHarrietStyleTable[(msgIndex - 0x2f) * 2];
        newHairStyle = q[r];
        break;
    }
    case 0x40:
        PlayerActor_LocalSetHeadwearHidden(0);
        Snd_PlaySe(0x43);
        msg = getQuestionsStartMsg();
        break;
    case 0x42:
        owner->changeAct(9);
        break;
    case 0xe:
    case 0x37:
        Talk_CheckAndSetPlayerFlag(0x13, 1);
        TownSessionState_ClearFlag(TownSessionState_Get(), 7);
        window->setNextMessage(gTalkMsgIndexEnd, 0);
        break;
    case 0x41:
        PlayerActor_LocalSetHeadwearHidden(1);
        Snd_PlaySe(0x44);
        break;
    case 0x45:
        break;
    }
    if (msg != 0xff) {
        u8 m = msg;
        window->setNextMessage(&m, (void *)tbl);
    }
}

void SpNpcHarrietTalk::onChoice(u32) {
    void *p = PlayerData_GetCurrent();
    s32 arg = getChoiceList()->getResult();
    const void *tbl = sSpNpcHarrietMsgKey;
    u32 msg = 0xff;
    switch (msgIndex) {
    case 1:
    case 2:
    case 4:
        if (arg == 0) {
            Hud_Hide();
            msg = 8;
        }
        break;
    case 8:
        if (arg == 0) {
            if (NpcActor_CanPlayerPay(owner, 0xbb8)) {
                NpcActor_ChargePlayer(owner, 0xbb8);
                msg = 0x3c;
                TownSessionState_SetFlag(TownSessionState_Get(), 7);
                newHairStyle = 0;
                newHairColor = 0;
                oldHairStyle = ((PlayerData *)(p))->getHairStyle();
                oldHairColor = ((PlayerData *)(p))->getHairColor();
            } else {
                msg = 0x3f;
            }
        }
        break;
    case 9:
    case 10:
        if (arg != 4) {
            newHairColor = sSpNpcHarrietWarmColors[arg];
            msg = 0xc;
        }
        break;
    case 11:
    case 0x38:
        if (arg != 4) {
            newHairColor = sSpNpcHarrietCoolColors[arg];
            msg = 0xc;
        }
        break;
    }
    if (msg != 0xff) {
        u8 m = msg;
        window->setNextMessage(&m, (void *)tbl);
    }
}

s32 SpNpcHarrietTalk::getQuestionsStartMsg() {
    void *p = PlayerData_GetCurrent();
    if (((PlayerSpNpcRecord *)(((PlayerData *)(p))->getSpNpcRecord()))->getHaircutCount() >= 0x10) {
        if (((PlayerId *)(((PlayerData *)(p))->getPlayerId()))->getGender() == 0) {
            return 0x15;
        }
        return 0x16;
    }
    return 0x3e;
}

BOOL SpNpcHarriet::acceptsInteraction(void *) {
    if (((NpcTalkCtrl *)(&talkCtrl))->isBusy() == 0) {
        return TRUE;
    }
    return FALSE;
}

BOOL SpNpcHarriet::acceptsSelfRequestedInteraction(void *) {
    if (((NpcTalkCtrl *)(&talkCtrl))->isBusy() == 0) {
        return TRUE;
    }
    return FALSE;
}

void SpNpcHarriet::onInteractionEvent(u32 cmd, u8 arg) {
    PlayerData_GetCurrent();
    partnerPlayer = arg;
    switch (cmd) {
    case 3:
        changeAct(10);
        break;
    case 1:
        partnerPlayer = arg;
        talk.resetMsg();
        talk.setTalkPlayer(getPlayerActor(4));
        if (talk.getTopic() == 4 || talk.getTopic() == 3) {
            changeAct(6);
        } else {
            changeAct(2);
        }
        break;
    case 0:
        talk.resetMsg();
        talk.setTalkPlayer(getPlayerActor(4));
        changeAct(2);
        break;
    case 8:
        if (talk.getTopic() == 9) {
            talk.setTopic(14);
            changeAct(5);
        } else if (talk.getTopic() != 10) {
            talk.setTopic(14);
            changeAct(4);
        }
        break;
    }
}

BOOL SpNpcHarriet::tryChairTalk() {
    if (TalkRequest_IsTalking()) {
        return FALSE;
    }
    if (isSessionPaid() == 0) {
        return FALSE;
    }
    PlayerData_GetCurrent();
    SpNpcHarrietVecLocal v;
    VecFx32 *src = (VecFx32 *)PlayerActor_GetBodyPos(4);
    *(VecFx32 *)&v = *src;
    s32 bx = 0, by = 0;
    FieldPos_ToUnit(&bx, &by, &v);
    if ((PlayerActor_IsInAction(0x25, 4) || PlayerActor_IsInAction(0x28, 4)) && TalkRequest_AddPlayerTalk7(this, 0)) {
        s32 f = 0;
        s32 x = bx;
        if (*(volatile s32 *)&bx == 4 && by == 0xb) {
            f = 1;
        }
        if (f != 0 || (x == 5 && by == 0xb)) {
            if (isWearingHeadItem()) {
                talk.setTopic(3);
            } else {
                talk.setTopic(4);
            }
        } else {
            talk.setTopic(7);
        }
        return TRUE;
    }
    return FALSE;
}

BOOL SpNpcHarriet::tryLeavePaidTalk() {
    if (isSessionPaid() == 0) {
        return FALSE;
    }
    SpNpcHarrietVecLocal v;
    VecFx32 *src = (VecFx32 *)PlayerActor_GetBodyPos(4);
    *(VecFx32 *)&v = *src;
    if (v.z > sSpNpcHarrietExitLine.z) {
        talk.setTopic(8);
        TalkRequest_AddPlayerTalk6(this, 0);
        return TRUE;
    }
    return FALSE;
}

BOOL SpNpcHarriet::tryFarewellTalk() {
    SpNpcHarrietVecLocal v;
    VecFx32 *src = (VecFx32 *)PlayerActor_GetBodyPos(4);
    *(VecFx32 *)&v = *src;
    if (Ground_IsOnLockedExit(&v)) {
        talk.setTopic(10);
        TalkRequest_AddPlayerTalk6(this, 0);
        return TRUE;
    }
    return FALSE;
}

BOOL SpNpcHarriet::isWearingHeadItem() {
    void *p = PlayerData_GetCurrent();
    if (*((PlayerData *)(p))->getFaceItem() != 0xfff1 || *((PlayerData *)(p))->getHat() != 0xfff1) {
        return TRUE;
    }
    return FALSE;
}

s32 SpNpcHarriet::isSessionPaid() {
    return TownSessionState_TestFlag(TownSessionState_Get(), 7);
}

// ---- overlay variables (generated: tools/host_overlay_vars.py) ----
#include "host_overlay_vars.h"

HOST_OVERLAY_VARS(unk_ov053_02258de0) {
    HOST_OVERLAY_VAR(sSpNpcHarrietProfile),
    HOST_OVERLAY_VAR(sSpNpcHarrietMsgKey),
    HOST_OVERLAY_VAR(sSpNpcHarrietTexturePath),
    HOST_OVERLAY_VAR(sSpNpcHarrietKey),
    HOST_OVERLAY_VAR(sSpNpcHarrietModelPath),
    HOST_OVERLAY_VAR(sSpNpcHarrietActTable),
    HOST_OVERLAY_VAR(SpNpcHarrietTalk_start_tbl),
    HOST_OVERLAY_VAR(SpNpcHarrietTalk_start_tbl_init),
    HOST_OVERLAY_VARS_END
};
