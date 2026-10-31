#include "types.h"
#include "host_labels.h"
#include "host_overlay_vars.h"
#include "gfx/VecFx32.h"
#include "save/Pattern.h"
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
#include "talk/MsgString25.h"
#include "actor/Actor.h"
#include "actor/Character.h"
#include "gfx/ThreeLayerAnimModel.h"
#include "talk/TalkMsgRequest.h"
#include "talk/ActorTalkRequest.h"
#include "talk/TalkWindowState.h"
#include "actor/NpcActor.h"
#include "actor/SpNpcActor.h"
#include "talk/ActorTalkRequest.h"
#include "actor/ActorProfile.h"
#include "net/CommManager.h"
#include "talk/TalkStartMsg.h"
#include "npc/NpcActionCtrl.h"
#include "npc/NpcFaceAnim.h"
#include "npc/NpcLookAt.h"
#include "npc/NpcMoveAnimSet.h"
#include "player/PlayerData.h"
#include "player/PlayerId.h"
#include "save/BlancaFaceRecord.h"
#include "save/LostChildRecord.h"
#include "talk/ChoiceList.h"
#include "talk/EncodedString8.h"
#include "talk/MsgRequest.h"
#include "talk/MsgString.h"
#include "talk/MsgString9B.h"
#include "talk/MsgString9C.h"
#include "talk/MsgString25.h"
#include "talk/EncodedString8B.h"
#include "town/TownTravelState.h"
#include "town/KatieVisitState.h"
#include "save/SaveManager.h"
#include "save/TownId.h"
#include "gfx/ScreenTransition.h"
#include "proto/main/unk_020943dc.h"
#include "proto/main/unk_020b2c50.h"
#include "proto/autoload_2/unk_020e9a08.h"
#include "proto/autoload_2/unk_02128030.h"
#include "proto/main/unk_020039ec.h"
#include "proto/main/unk_020411dc.h"
#include "proto/main/unk_02063904.h"
#include "proto/main/unk_02065e88.h"
#include "proto/main/unk_0206e54c.h"
#include "proto/main/unk_020720f8.h"
#include "proto/main/unk_020742f4.h"
#include "proto/main/unk_020850e0.h"
#include "proto/main/unk_02085940.h"
#include "proto/main/unk_02094810.h"
#include "proto/main/unk_02097444.h"
#include "proto/main/unk_0209eb94.h"
#include "proto/main/unk_020a427c.h"
#include "proto/main/unk_020b7190.h"
#include "proto/ov004/unk_ov004_022257e4.h"
#include "proto/ov048/unk_ov048_02258de0.h"
#include "proto/main/unk_020b4828.h"
#include "proto/main/unk_0204cc1c.h"


class ActorTalkRequest;
class NpcActor;
class SpNpcCopper;
class SpNpcCopperTalk;




// Two-step storage for the three model/sequence name pointers (their strings are named arrays below).
extern "C" {
extern const u8 sCopperWifiStartMsgs[4];
extern const VecFx32 sCopperGateCheckPos;
extern const VecFx32 sCopperTurnBackPos;
extern const VecFx32 sCopperSendOffWalkPos;
extern const VecFx32 sCopperSendOffExitPos;
extern const VecFx32 sCopperArrivalWalkPos;
extern const VecFx32 sCopperDepartWalkPos;
extern CommManager *gCommManager;
extern u16 data_020c6cc8;
#define gSaveTownId HOST_LABEL(TownId *, gSaveTownId)
BOOL EncodedString_SetRaw(void *, const void *, s32);
BOOL Math_CountDownU16(void *);
s32 Math_IsInRange(s32, s32, s32);
void SpNpcKatie_ChangeAct05();
s32 SpNpcKatie_ResetAct();
void MI_CpuCopy8(void *, void *, u32);
s32 func_02133150(s32, s32);
s32 Talk_IsInOwnTown();
u32 Clock_GetTimeOfDay();
void * FriendList_GetEntries(void *);
void Camera_SaveView();
s32 Math_CountDownU8(void *);
s32 Camera_SetMode15();
s32 Camera_SetMode14();
s32 Camera_IsBlending();
void TalkRequest_SetTargetDone(void *);
s32 Scene_GetCurrent();
void TalkRequestFlags_ClearSceneHold();
void TalkRequestFlags_SetSceneHold();
void TalkRequest_AddPlayerTalk6(void *, s32);
BOOL SpNpcKatie_IsIdle();
BOOL TalkRequest_IsActive();

}





// Dialog-state sub object embedded in the scene (vtable data_ov048_0225cbc8).
class SpNpcCopperTalk : public ActorTalkRequest {
public:
    typedef void (SpNpcCopperTalk::*Fn)();
    typedef void (SpNpcCopperTalk::*ArgFn)(s32);

    SpNpcCopperTalk();
    virtual ~SpNpcCopperTalk();
    virtual void onMessageEnd(u32 attr);
    virtual void onChoice(u32 attr);
    virtual void start(TalkStartMsg *out);
    virtual void update();
    virtual void onTaskDone(u32 id);

    void waitGoHomeAccepted();
    void requestGoHome();
    void waitJoinAccepted();
    void requestJoin();
    void waitConnected();
    void connectToTown();
    void showErrorAndAbort(u32 msg, s32 unused);
    BOOL checkScanTimeout();
    BOOL checkWifiLoginError();
    BOOL checkFindTownTimeout();
    BOOL checkConnectTimeout();
    BOOL checkNetError(s32 flag);
    void waitSaveDone();
    void requestSave();
    void shutdownWifi();
    void onTownListClosed();
    void onFriendListClosed();
    void scanForOpenTowns();
    void onWifiLoggedIn();
    BOOL isSkippableNetError();
    void pollGameStatsDownload();
    void startGameStatsDownload();
    void pollGameStatsUpload();
    void startGameStatsUpload();
    void pollBbsDownload();
    void startBbsDownload();
    void pollMailDownload();
    void startMailDownload();
    void waitWifiLogin();
    void setScript(s32 s);
    s32 getWifiErrorMsg(s32 id);
    void onChoiceUnk5F(s32 p);
    void onChoiceGateAlreadyOpen(s32 p);
    void onChoiceSaveAndQuit(s32 p);
    void onChoiceRetryWifi(s32 p);
    void onChoiceRetryLocal(s32 p);
    void onChoiceGoOutInstead(s32 p);
    void confirmStartComm(s32 p, s32 id);
    void onChoiceVisitWifi(s32 p);
    void onChoiceVisitLocal(s32 p);
    void onChoiceHostWifi(s32 p);
    void onChoiceHostLocal(s32 p);
    void onChoiceBackToMenu(s32 p);
    void onChoiceHostMenu(s32 p);
    void onChoiceMainMenu(s32 p);
    void onChoiceGoHome(s32 p);
    void checkWifiReady();
    void onChoiceHostMethod(s32 p);
    void onChoiceVisitMethod(s32 p);
    void onChoiceFriendCode(s32 p);
    void onChoiceConnectWifi(s32 p);
    void onChoiceVisitTown(s32 p);
    void onChoiceUpdateWifiId(s32 p);
    void startGoHome();
    void showWifiIdSavedResult();
    void showAnythingElseMenu();
    void startFarewellAct();
    void startSendOffAct();
    void startWifiVisit();
    void startLocalVisit();
    void openFriendList();
    void openTownList();
    void startTownSearch();
    void startWifiLogin();
    void startOpenGateAct();
    void startTurnBackAct();
    void unlockWindow();
    void lockWindow(s32 flag);
    void closeGate();
    void showMainMenu();
    void startSave();
    BOOL hasFriends();
    void setFriendCodeArgs(s64 v);
    void startComm(s32 a, s32 b);
    void endComm();
    s32 getTopic();
    void setTopic(s32 v);
    void attachOwner(SpNpcCopper *p);
    u32 getJoinErrorMsg();
    u32 getConnectErrorMsg();
    u32 getAnythingElseMsg();
    s32 getInviteOrCloseGateMsg();
    s32 getWifiLoginResultMsg();

    s32 topic;
    s32 script;
    SpNpcCopper *owner;
    /* 0x0b8 */ Pattern pattern;
    /* 0x2e0 */ u8 hostGameInfo[0x14];
    /* 0x2f4 */ u8 hostBeacon[0xe0];
    /* 0x3d4 */ s32 hostFriendId;
    /* 0x3d8 */ AxMail mail;
    /* 0x4e0 */ AxBbsNotice bbsNotice;
    /* 0x5b4 */ BlancaFaceRecord blancaFace;
    u8 wifiIdChanged;
    u8 saveDone;
    u8 syncPayload;
    u8 wifiPurpose;
    s16 homeAngle;
    u16 timer;
    u8 animTimer;
    u8 subStep;
    u8 pad_7ea[2];
};








class SpNpcCopper : public SpNpcActor {
public:
    typedef BOOL (SpNpcCopper::*Fn)();

    SpNpcCopper() {}
    virtual BOOL onCreate();
    virtual BOOL preCreate();
    virtual BOOL onDelete();
    virtual BOOL acceptsInteraction(void *other);
    virtual void onInteractionEvent(u32 cmd, u8 arg);
    virtual BOOL acceptsSelfRequestedInteraction(void *a);
    virtual BOOL updateAct();
    virtual u8 *getTexturePath();
    virtual u8 *getModelPath();
    virtual BOOL canPlayTalkMelody();

    BOOL canStartSave();
    BOOL checkPlayerAtGate();
    BOOL mainAct10();
    BOOL setupAct10();
    BOOL mainAct11();
    BOOL setupAct11();
    BOOL mainAct0F();
    BOOL setupAct0F();
    BOOL mainAct0E();
    BOOL setupAct0E();
    BOOL mainAct0D();
    BOOL setupAct0D();
    BOOL mainAct0C();
    BOOL setupAct0C();
    BOOL mainAct0B();
    BOOL setupAct0B();
    BOOL act0BStep3();
    BOOL act0BStep2();
    BOOL act0BStep1();
    BOOL act0BStep0();
    BOOL mainAct0A();
    BOOL setupAct0A();
    BOOL mainAct09();
    BOOL setupAct09();
    BOOL act09Step3();
    BOOL act09Step2();
    BOOL act09Step1();
    BOOL act09Step0();
    BOOL mainAct08();
    BOOL setupAct08();
    BOOL act08Step5();
    BOOL act08Step4();
    BOOL act08Step3();
    BOOL act08Step2();
    BOOL act08Step1();
    BOOL act08Step0();
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

    s32 act;
    SpNpcCopperTalk talk;
};

struct Unk_ov048_State_Ent {
    SpNpcCopper::Fn enter;
    SpNpcCopper::Fn exit;
};

struct SpNpcCopperTalkScript {
    SpNpcCopperTalk::Fn f;
    u8 flag;
};

struct SpNpcCopperChoiceHandler {
    u32 id;
    SpNpcCopperTalk::ArgFn f;
};

struct Unk_ov048_0225a8d4_Row {
    u32 id;
    SpNpcCopperTalk::Fn f;
};


extern "C" {
extern Unk_ov048_State_Ent sSpNpcCopperActTable[];
extern SpNpcCopperTalkScript sSpNpcCopperTalkScripts[];
}

extern "C" {
}

#define RNG(v, lo, hi) (Math_IsInRange((v), (lo), (hi)) != 0)

// ---- b444
static inline BOOL Unk_ov048_0225b4e4_Is2() {
    if (gScreenTransition.state == 2) {
        return TRUE;
    }
    return FALSE;
}

static inline BOOL Unk_ov048_0225b854_Is0() {
    if (gScreenTransition.state == 0) {
        return TRUE;
    }
    return FALSE;
}

extern "C" SpNpcCopper *SpNpcCopper_Create() {
    return new SpNpcCopper();
}

BOOL SpNpcCopper::preCreate() {
    if (SpNpcActor::preCreate() == 0) {
        return FALSE;
    }
    setTalkRequest((ActorTalkRequest *)&talk);
    talk.attachOwner(this);
    setCollisionRadius(0x100);
    ((NpcMoveAnimSet *)(&moveAnimSet))->setWalkAnim(0xd9);
    ((NpcMoveAnimSet *)(&moveAnimSet))->setStandAnim(0xd8);
    if (Scene_GetCurrent() != 0xb) {
        netSyncOff = 1;
    }
    if (Scene_GetCurrent() == 0xc) {
        collisionEnabled = 0;
    }
    return TRUE;
}

BOOL SpNpcCopper::onCreate() {
    if (SpNpcActor::onCreate() == 0) {
        return FALSE;
    }
    talk.homeAngle = rotY;
    collider.groups |= 2;
    if (((CommManager *)(gCommManager))->isOnline() && Scene_GetCurrent() == 0xb) {
        if (isNetOwner()) {
            SpNpcCopper_ChangeAct(this, 1);
        } else {
            SpNpcCopper_ChangeAct(this, 0xf);
        }
        return TRUE;
    }
    TownTravelState *p = (TownTravelState *)TownSessionState_GetTravelState(TownSessionState_Get());
    if (p->getMode() == 1 || p->getMode() == 2) {
        if (p->getMode() == 1) {
            talk.setTopic(6);
        } else {
            talk.setTopic(7);
        }
        SpNpcCopper_ChangeAct(this, 0);
        rotY = p->getAngle();
        moveAngleY = p->getAngle();
        p->clearMode();
    } else if (Scene_GetCurrent() == 0xd) {
        TalkRequestFlags_SetSceneHold();
        SpNpcCopper_ChangeAct(this, 9);
    } else if (Scene_GetCurrent() == 0xe) {
        TalkRequestFlags_SetSceneHold();
        SpNpcCopper_ChangeAct(this, 0xb);
    } else {
        SpNpcCopper_ChangeAct(this, 1);
    }
    return TRUE;
}

BOOL SpNpcCopper::onDelete() {
    if (SpNpcActor::onDelete() == 0) {
        return FALSE;
    }
    if (Scene_GetCurrent() == 0xd || Scene_GetCurrent() == 0xe) {
        TalkRequestFlags_ClearSceneHold();
    }
    return TRUE;
}

u8 *SpNpcCopper::getTexturePath() {
    return (u8 *)sSpNpcCopperTexturePathPtr;
}

u8 *SpNpcCopper::getModelPath() {
    return (u8 *)sSpNpcCopperModelPathPtr;
}

BOOL SpNpcCopper::updateAct() {
    BOOL r = FALSE;
    if (sSpNpcCopperActTable[act].exit) {
        r = (this->*sSpNpcCopperActTable[act].exit)();
    }
    return r;
}

extern "C" void SpNpcCopper_ChangeAct(SpNpcCopper *self, s32 s) {
    BOOL r = TRUE;
    if (sSpNpcCopperActTable[s].enter) {
        r = (self->*sSpNpcCopperActTable[s].enter)();
    }
    if (r) {
        self->act = s;
    }
}

BOOL SpNpcCopper::setupAct00() {
    return TRUE;
}

BOOL SpNpcCopper::mainAct00() {
    TalkRequest_AddPlayerTalk6(this, 0);
    return TRUE;
}

BOOL SpNpcCopper::setupAct01() {
    ((NpcActionCtrl *)(&actionCtrl))->requestStand(1, data_020c6cc8);
    return TRUE;
}

BOOL SpNpcCopper::mainAct01() {
    checkPlayerAtGate();
    return TRUE;
}

BOOL SpNpcCopper::setupAct02() {
    NpcActor *o = (NpcActor *)talk.getTalkPlayer();
    s32 r = 0;
    if (o) {
        r = getAngleTo(o);
    }
    ((NpcTalkCtrl *)(&talkCtrl))->requestTurnAndTalk(0, r, 0);
    return TRUE;
}

BOOL SpNpcCopper::mainAct02() {
    if (((NpcTalkCtrl *)(&talkCtrl))->isBusy() == 0) {
        TalkRequest_SetTargetDone(this);
    }
    return TRUE;
}

BOOL SpNpcCopper::setupAct03() {
    return TRUE;
}

BOOL SpNpcCopper::mainAct03() {
    return TRUE;
}

BOOL SpNpcCopper::setupAct04() {
    ((NpcActionCtrl *)(&actionCtrl))->requestAction(3, 1, 0, 0, 0, talk.homeAngle, 0, 0, data_020c6cc8, 0);
    return TRUE;
}

BOOL SpNpcCopper::mainAct04() {
    if (checkPlayerAtGate()) {
        return TRUE;
    }
    if (((NpcActionCtrl *)(&actionCtrl))->getAction() == 3) {
        if (((NpcActionCtrl *)(&actionCtrl))->isActionDone()) {
            SpNpcCopper_ChangeAct(this, 1);
        }
    }
    return TRUE;
}

BOOL SpNpcCopper::setupAct05() {
    talk.subStep = 0;
    return TRUE;
}

BOOL SpNpcCopper::mainAct05() {
    VecFx32Ctor a;
    VecFx32Ctor b;
    PlayerData_GetCurrent();
    VecFx32 *p = (VecFx32 *)PlayerActor_GetBodyPos(4);
    static_cast<VecFx32 &>(a) = *p;
    switch (talk.subStep) {
    case 0:
        if (((NpcTalkCtrl *)(&talkCtrl))->isBusy() == 0) {
            talk.subStep = 1;
        }
        break;
    case 1:
        static_cast<VecFx32 &>(b) = sCopperTurnBackPos;
        PlayerActor_RequestWalkTo(&b, 0x266, 4);
        talk.subStep = 3;
        break;
    case 3:
        if (PlayerActor_IsScriptedWalking(4) == 0) {
            TalkRequest_SetTargetDone(this);
        }
        break;
    }
    return TRUE;
}

// ---- bde0

BOOL SpNpcCopper::setupAct06() {
    talk.subStep = 0;
    return TRUE;
}

BOOL SpNpcCopper::mainAct06() {
    u8 buf[0x14];
    void *h;
    switch (talk.subStep) {
    case 0:
        if (((NpcTalkCtrl *)(&talkCtrl))->isBusy() == 0) {
            talk.subStep = 1;
        }
        break;
    case 1:
        if (Camera_IsBlending() == 0) {
            CheckInGate_Open();
            talk.timer = 0x3c;
            talk.subStep = 2;
            if (Net_GetMode() == 4) {
                NetOverlay_AssertWifi();
                Net_WifiStartHost();
                ((CommManager *)(gCommManager))->setErrorMode(0);
                Comm_BeginHostSession();
            }
        }
        break;
    case 2:
        if (Math_CountDownU16(&talk.timer) == 0) {
            u8 t = talk.msgIndex;
            if (t == 0x46 || t == 0x6c) {
                if (Net_GetMode() == 3) {
                    NetOverlay_AssertAny();
                    if (Net_PollConnected() != 0) {
                        TalkRequest_SetTargetDone(this);
                    }
                } else {
                    talk.startComm(1, 0);
                    h = PlayerData_GetCurrent();
                    MI_CpuCopy8(TownId_GetName(&gSaveTownId.townId), buf, 8);
                    MI_CpuCopy8(((PlayerId *)(((PlayerData *)(h))->getPlayerId()))->getName(), buf + 8, 8);
                    buf[0x10] = 0;
                    NetOverlay_AssertWireless();
                    Net_SetLocalGameInfo(buf, 0x11);
                    Comm_BeginHostSession();
                    TalkRequest_SetTargetDone(this);
                }
            }
        } else {
            if (Net_GetMode() == 3) {
                NetOverlay_AssertAny();
                Net_PollConnected();
            }
        }
        break;
    }
    return TRUE;
}

BOOL SpNpcCopper::setupAct07() {
    talk.subStep = 0;
    return TRUE;
}

BOOL SpNpcCopper::mainAct07() {
    switch (talk.subStep) {
    case 0:
        if (((NpcTalkCtrl *)(&talkCtrl))->isBusy() == 0) {
            talk.subStep = 1;
        }
        break;
    case 1:
        if (Camera_IsBlending() == 0) {
            CheckInGate_Close();
            talk.timer = 0x3c;
            talk.subStep = 2;
        }
        break;
    case 2:
        if (Math_CountDownU16(&talk.timer) == 0) {
            TalkRequest_SetTargetDone(this);
        }
        break;
    }
    return TRUE;
}

BOOL SpNpcCopper::act08Step0() {
    VecFx32Ctor v;
    if (((NpcTalkCtrl *)(&talkCtrl))->isBusy() == 0) {
        Camera_SetMode14();
        static_cast<VecFx32 &>(v) = sCopperSendOffWalkPos;
        PlayerActor_RequestWalkTo(&v, 0x400, 4);
        PlayerActor_SetNoFaceTalkTarget(1, 4);
        return TRUE;
    }
    return FALSE;
}

BOOL SpNpcCopper::act08Step1() {
    if (PlayerActor_IsScriptedWalking(4) == 0) {
        PlayerActor_RequestTurnTo((s32)0xffff8000, 4);
        ((NpcActionCtrl *)(&actionCtrl))->requestAction(3, 1, 0, 0, 0, (s32)0xffffc000, 0, 0, data_020c6cc8, 0);
        talk.timer = (u8)(Random_GlobalBelow(5) + 5);
        return TRUE;
    }
    return FALSE;
}

BOOL SpNpcCopper::act08Step2() {
    if (Math_CountDownU16(&talk.timer) == 0) {
        NpcActor *katie = (NpcActor *)Actor::findByProfile(0x73, NULL);
        ((NpcActionCtrl *)(&katie->actionCtrl))->requestAction(3, 1, 0, 0, 0, 0x4000, 0, 0, data_020c6cc8, 0);
        return TRUE;
    }
    return FALSE;
}

BOOL SpNpcCopper::act08Step3() {
    NpcActor *katie = (NpcActor *)Actor::findByProfile(0x73, NULL);
    NpcActionCtrl *r4 = (NpcActionCtrl *)&katie->actionCtrl;
    VecFx32Ctor v;
    if (((NpcActionCtrl *)(&actionCtrl))->getAction() == 3) {
        if (((NpcActionCtrl *)(&actionCtrl))->isActionDone()) {
            ((NpcActionCtrl *)(&actionCtrl))->requestStand(1, data_020c6cc8);
        }
    }
    if (((NpcActionCtrl *)(r4))->getAction() == 3) {
        if (((NpcActionCtrl *)(r4))->isActionDone()) {
            ((NpcActionCtrl *)(r4))->requestAction(0, 1, 0, 0, 0, (s32)0xffffc000, 0, 0, data_020c6cc8, 0);
        }
    }
    if (((NpcActionCtrl *)(&actionCtrl))->getAction() == 0) {
        if (((NpcActionCtrl *)(r4))->getAction() == 0) {
            static_cast<VecFx32 &>(v) = sCopperSendOffExitPos;
            PlayerActor_RequestWalkTo(&v, 0x666, 4);
            ((NpcActionCtrl *)(&actionCtrl))->requestPlayAnim(1, 0x81, 1, data_020c6cc8, 0);
            talk.animTimer = Random_GlobalBelow(5) + 5;
            talk.timer = 0x16;
            Camera_SetMode15();
            return TRUE;
        }
    }
    return FALSE;
}

BOOL SpNpcCopper::act08Step4() {
    NpcActor *katie = (NpcActor *)Actor::findByProfile(0x73, NULL);
    NpcAnimCtrl *r4 = (NpcAnimCtrl *)&katie->animCtrl;
    NpcActionCtrl *r6 = (NpcActionCtrl *)&katie->actionCtrl;
    void *r7 = &katie->moveAnimSet;
    NpcLookAt *sp8 = (NpcLookAt *)&katie->lookAt;
    if (Math_CountDownU8(&talk.animTimer) == 0) {
        if (((NpcAnimCtrl *)(r4))->isPlayingAnim(0x81, r7) == 0) {
            if (((NpcAnimCtrl *)(r4))->isPlayingAnim(0x82, r7) == 0) {
                ((NpcActionCtrl *)(r6))->requestPlayAnim(1, 0x81, 1, data_020c6cc8, 0);
            }
        }
    }
    if (((NpcAnimCtrl *)(&animCtrl))->isPlayingAnim(0x81, &moveAnimSet) != 0) {
        if (((NpcActionCtrl *)(&actionCtrl))->isActionDone() != 0) {
            ((NpcActionCtrl *)(&actionCtrl))->requestPlayAnim(1, 0x82, 0, data_020c6cc8, 0);
        }
    }
    if (((NpcAnimCtrl *)(r4))->isPlayingAnim(0x81, r7) != 0) {
        if (((NpcActionCtrl *)(r6))->isActionDone() != 0) {
            ((NpcActionCtrl *)(r6))->requestPlayAnim(1, 0x82, 0, data_020c6cc8, 0);
        }
    }
    if (talk.timer == 2) {
        ((NpcLookAt *)(&lookAt))->disable();
        ((NpcLookAt *)(sp8))->disable();
    }
    if (Math_CountDownU16(&talk.timer) == 0) {
        if (act == 8) {
            if (ScreenTransition_StartFadeOut(2, 0xf)) {
                Snd_FadeOutScene();
                return TRUE;
            }
        } else {
            return TRUE;
        }
    }
    return FALSE;
}

extern "C" BOOL SpNpcCopper_CheckKatieEscort() {
    void *h = PlayerData_GetCurrent();
    void *r4;
    if (LostChild_IsKatieDue()) {
        r4 = ((PlayerData *)(h))->getLostChildRecord();
        if (TownId_IsValid(((LostChildRecord *)(r4))->getTownId())) {
            TownId *p = (TownId *)((LostChildRecord *)(r4))->getTownId();
            if (p->townId == gSaveTownId.townId) {
                if (memcmp(p->townName, gSaveTownId.townName, 8) == 0) {
                    goto skip;
                }
            }
        }
        ((LostChildRecord *)(PlayerData_GetCurrent()->getLostChildRecord()))->clearEscorting();
    skip:
        if (((LostChildRecord *)(r4))->isEscorting()) {
            ((PlayerData *)(h))->setFlag(0x36);
            return TRUE;
        }
    }
    return FALSE;
}

BOOL SpNpcCopper::act08Step5() {
    if (act == 8) {
        if (Unk_ov048_0225b854_Is0()) {
            SaveManager_SetGatePassed();
        } else {
            return FALSE;
        }
        SpNpcCopper_ChangeAct(this, 3);
    } else {
        SceneWarp_RequestExit(Scene_GetWarpRequest(), 0);
        SpNpcCopper_ChangeAct(this, 3);
    }
    return TRUE;
}

BOOL SpNpcCopper::setupAct08() {
    talk.subStep = 0;
    ((NpcMoveCtrl *)(&moveCtrl))->setTargetAngle(talk.homeAngle);
    return TRUE;
}

// Declarations for data defined further down (definition order sets the data layout)
extern "C" char sSpNpcCopperSequence4Key[];
extern "C" char sSpNpcCopperModelPath[];
extern "C" const u8 sCopperWifiStartMsgs[4];
extern "C" void *sSpNpcCopperTexturePathPtr;
extern "C" SpNpcCopperTalkScript sSpNpcCopperTalkScripts[23];
extern "C" void *sSpNpcCopperMsgKey;
extern "C" const VecFx32 sCopperDepartWalkPos;
extern "C" Unk_ov048_State_Ent sSpNpcCopperActTable[18];
extern "C" const VecFx32 sCopperGateCheckPos;
extern "C" char sSpNpcCopperKey[];
extern "C" void *sSpNpcCopperModelPathPtr;
extern "C" const VecFx32 sCopperTurnBackPos;
extern "C" const VecFx32 sCopperSendOffExitPos;
extern "C" char sSpNpcCopperTexturePath[];
extern "C" ActorProfile sSpNpcCopperProfile;
extern "C" const VecFx32 sCopperSendOffWalkPos;
extern "C" const VecFx32 sCopperArrivalWalkPos;

extern "C" char sSpNpcCopperSequence4Key[] = {'s', 'p', '_', 'e', 't', 'c', '_', 's', 'e', 'q', 'u', 'e', 'n', 'c', 'e', '4', 0};

extern "C" char sSpNpcCopperModelPath[] = {'n', 'p', 'c', '_', 's', 'p', '/', 'm', 'o', 'd', 'e', 'l', '/', 'p', 'l', 'c', '.', 'n', 's', 'b', 'm', 'd', 0};

extern "C" const u8 sCopperWifiStartMsgs[4] = {0x24, 0x41, 0x7f, 0x00};

BOOL SpNpcCopper::mainAct08() {
    static SpNpcCopper::Fn tbl[6] = {
        &SpNpcCopper::act08Step0,
        &SpNpcCopper::act08Step1,
        &SpNpcCopper::act08Step2,
        &SpNpcCopper::act08Step3,
        &SpNpcCopper::act08Step4,
        &SpNpcCopper::act08Step5,
    };
    if (talk.subStep < 6) {
        if ((this->*tbl[talk.subStep])()) {
            talk.subStep++;
        }
    }
    return TRUE;
}

BOOL SpNpcCopper::act09Step0() {
    VecFx32Ctor v;
    s32 a = Net_GetJoiningAid();
    if (Unk_ov048_0225b4e4_Is2()) {
        if (PlayerActor_SetNetFollowPaused(1, a)) {
            PlayerActor_SetNoFaceTalkTarget(1, 4);
            static_cast<VecFx32 &>(v) = sCopperArrivalWalkPos;
            PlayerActor_RequestWalkTo(&v, 0x35c, a);
            return TRUE;
        }
    }
    return FALSE;
}

BOOL SpNpcCopper::act09Step1() {
    void *h;
    TalkWindowState *o;
    s32 a = Net_GetJoiningAid();
    h = PlayerData_GetBySessionSlot(a);
    if (PlayerActor_IsScriptedWalking(a) == 0) {
        o = TalkWindow_Get(0);
        talk.resetMsg();
        ((MsgRequest *)(&talk))->setFileName((const char *)sSpNpcCopperMsgKey);
        talk.msgIndex = 0x67;
        ((TalkWindowState *)(o))->attachRequest((TalkMsgRequest *)(&talk));
        MsgString9B name;
        ((PlayerId *)(((PlayerData *)(h))->getPlayerId()))->getNameString(&name);
        ((TalkWindowState *)(o))->setSlot(1, &name);
        o->nextState = 1;
        return TRUE;
    }
    return FALSE;
}

BOOL SpNpcCopper::act09Step2() {
    TalkWindowState *o = TalkWindow_Get(0);
    if (o->state == 0) {
        if (SaveManager_IsSessionJoined() == 0) {
            FieldInfoBalloon_ShowPleaseWait();
            return FALSE;
        }
        ((TalkWindowState *)(o))->detachRequest();
        return TRUE;
    }
    return FALSE;
}

BOOL SpNpcCopper::act09Step3() {
    SceneWarp_RequestExit(Scene_GetWarpRequest(), 1);
    return TRUE;
}

BOOL SpNpcCopper::setupAct09() {
    talk.subStep = 0;
    return TRUE;
}
extern "C" void *sSpNpcCopperTexturePathPtr = sSpNpcCopperTexturePath;

BOOL SpNpcCopper::mainAct09() {
    static SpNpcCopper::Fn tbl[4] = {
        &SpNpcCopper::act09Step0,
        &SpNpcCopper::act09Step1,
        &SpNpcCopper::act09Step2,
        &SpNpcCopper::act09Step3,
    };
    if (talk.subStep < 4) {
        if ((this->*tbl[talk.subStep])()) {
            talk.subStep++;
        }
    }
    return TRUE;
}

BOOL SpNpcCopper::setupAct0A() {
    return setupAct08();
}

BOOL SpNpcCopper::mainAct0A() {
    return mainAct08();
}

BOOL SpNpcCopper::act0BStep0() {
    void *h;
    TalkWindowState *o;
    h = PlayerData_GetBySessionSlot(NetSession_GetLastSyncSlot());
    if (Unk_ov048_0225b4e4_Is2()) {
        o = TalkWindow_Get(0);
        talk.resetMsg();
        ((MsgRequest *)(&talk))->setFileName((const char *)sSpNpcCopperMsgKey);
        talk.msgIndex = 0x7b;
        ((TalkWindowState *)(o))->attachRequest((TalkMsgRequest *)(&talk));
        MsgString9B name;
        ((PlayerId *)(((PlayerData *)(h))->getPlayerId()))->getNameString(&name);
        ((TalkWindowState *)(o))->setSlot(1, &name);
        o->nextState = 1;
        return TRUE;
    }
    return FALSE;
}

BOOL SpNpcCopper::act0BStep1() {
    s32 a = NetSession_GetLastSyncSlot();
    TalkWindowState *o = TalkWindow_Get(0);
    if (o->state == 0) {
        VecFx32Ctor v;
        ((TalkWindowState *)(o))->detachRequest();
        static_cast<VecFx32 &>(v) = sCopperDepartWalkPos;
        PlayerActor_RequestWalkTo(&v, 0x35c, a);
        return TRUE;
    }
    return FALSE;
}

BOOL SpNpcCopper::act0BStep2() {
    s32 a = NetSession_GetLastSyncSlot();
    void *b = PlayerData_GetBySessionSlot(a);
    if (PlayerActor_IsScriptedWalking(a) == 0) {
        ((PlayerData *)(b))->reset();
        return TRUE;
    }
    return FALSE;
}

BOOL SpNpcCopper::act0BStep3() {
    SceneWarp_RequestExit(Scene_GetWarpRequest(), 1);
    return TRUE;
}

BOOL SpNpcCopper::setupAct0B() {
    talk.subStep = 0;
    return TRUE;
}
extern "C" SpNpcCopperTalkScript sSpNpcCopperTalkScripts[23] = {
    {NULL, 0},
    {&SpNpcCopperTalk::scanForOpenTowns, 1},
    {&SpNpcCopperTalk::onFriendListClosed, 0},
    {&SpNpcCopperTalk::onTownListClosed, 0},
    {&SpNpcCopperTalk::connectToTown, 1},
    {&SpNpcCopperTalk::waitConnected, 1},
    {&SpNpcCopperTalk::requestJoin, 1},
    {&SpNpcCopperTalk::waitJoinAccepted, 1},
    {&SpNpcCopperTalk::requestGoHome, 1},
    {&SpNpcCopperTalk::waitGoHomeAccepted, 1},
    {&SpNpcCopperTalk::waitWifiLogin, 1},
    {&SpNpcCopperTalk::startGameStatsUpload, 1},
    {&SpNpcCopperTalk::pollGameStatsUpload, 1},
    {&SpNpcCopperTalk::startGameStatsDownload, 1},
    {&SpNpcCopperTalk::pollGameStatsDownload, 1},
    {&SpNpcCopperTalk::startMailDownload, 1},
    {&SpNpcCopperTalk::pollMailDownload, 1},
    {&SpNpcCopperTalk::startBbsDownload, 1},
    {&SpNpcCopperTalk::pollBbsDownload, 1},
    {&SpNpcCopperTalk::onWifiLoggedIn, 1},
    {&SpNpcCopperTalk::shutdownWifi, 1},
    {&SpNpcCopperTalk::requestSave, 1},
    {&SpNpcCopperTalk::waitSaveDone, 1},
};

extern "C" void *sSpNpcCopperMsgKey = sSpNpcCopperKey;

extern "C" const VecFx32 sCopperDepartWalkPos = {0x10000, 0x0, 0x5000};

extern "C" Unk_ov048_State_Ent sSpNpcCopperActTable[18] = {
    {&SpNpcCopper::setupAct00, &SpNpcCopper::mainAct00},
    {&SpNpcCopper::setupAct01, &SpNpcCopper::mainAct01},
    {&SpNpcCopper::setupAct02, &SpNpcCopper::mainAct02},
    {&SpNpcCopper::setupAct03, &SpNpcCopper::mainAct03},
    {&SpNpcCopper::setupAct04, &SpNpcCopper::mainAct04},
    {&SpNpcCopper::setupAct05, &SpNpcCopper::mainAct05},
    {&SpNpcCopper::setupAct06, &SpNpcCopper::mainAct06},
    {&SpNpcCopper::setupAct07, &SpNpcCopper::mainAct07},
    {&SpNpcCopper::setupAct08, &SpNpcCopper::mainAct08},
    {&SpNpcCopper::setupAct09, &SpNpcCopper::mainAct09},
    {&SpNpcCopper::setupAct0A, &SpNpcCopper::mainAct0A},
    {&SpNpcCopper::setupAct0B, &SpNpcCopper::mainAct0B},
    {&SpNpcCopper::setupAct0C, &SpNpcCopper::mainAct0C},
    {&SpNpcCopper::setupAct0D, &SpNpcCopper::mainAct0D},
    {&SpNpcCopper::setupAct0E, &SpNpcCopper::mainAct0E},
    {&SpNpcCopper::setupAct0F, &SpNpcCopper::mainAct0F},
    {&SpNpcCopper::setupAct10, &SpNpcCopper::mainAct10},
    {&SpNpcCopper::setupAct11, &SpNpcCopper::mainAct11},
};

BOOL SpNpcCopper::mainAct0B() {
    static SpNpcCopper::Fn tbl[4] = {
        &SpNpcCopper::act0BStep0,
        &SpNpcCopper::act0BStep1,
        &SpNpcCopper::act0BStep2,
        &SpNpcCopper::act0BStep3,
    };
    if (talk.subStep < 4) {
        if ((this->*tbl[talk.subStep])() != 0) {
            talk.subStep++;
        }
    }
    return TRUE;
}

BOOL SpNpcCopper::setupAct0C() {
    return TRUE;
}

BOOL SpNpcCopper::mainAct0C() {
    if (TalkWindow_Get(0)->state == 5) {
        TownTravelState *h;
        s32 a, b;
        s32 s;
        VecFx32 v;
        Actor *e;
        h = (TownTravelState *)TownSessionState_GetTravelState(TownSessionState_Get());
        e = (Actor *)PlayerActor_GetActor(4);
        VecFx32 *pv = &e->position;
        v.x = pv->x;
        v.y = pv->y;
        v.z = pv->z;
        s = e->rotY;
        a = 0;
        b = 0;
        FieldPos_ToUnit(&a, &b, &v);
        h->setMode(1);
        h->setAngle(rotY);
        Camera_SaveView();
        SaveManager_RequestAct14();
        SceneWarp_RequestAt(Scene_GetWarpRequest(), 0xc, &v, 0x800000, s, 2, 2);
        SpNpcCopper_ChangeAct(this, 3);
    }
    return TRUE;
}

BOOL SpNpcCopper::setupAct0D() {
    return TRUE;
}

BOOL SpNpcCopper::mainAct0D() {
    if (TalkWindow_Get(0)->state == 5) {
        TownTravelState *h;
        s32 a, b;
        s32 s;
        VecFx32 v;
        Actor *e;
        h = (TownTravelState *)TownSessionState_GetTravelState(TownSessionState_Get());
        e = (Actor *)PlayerActor_GetActor(4);
        VecFx32 *pv = &e->position;
        v.x = pv->x;
        v.y = pv->y;
        v.z = pv->z;
        s = e->rotY;
        a = 0;
        b = 0;
        FieldPos_ToUnit(&a, &b, &v);
        h->setMode(2);
        h->setAngle(rotY);
        Scene_SetSavedPos(Scene_GetWarpRequest(), 0xc, &v, 0x800000, s, a, b);
        Camera_SaveView();
        SaveManager_RequestAct17();
        SceneWarp_RequestFade(Scene_GetWarpRequest(), 0x2e, 2, 3);
        SpNpcCopper_ChangeAct(this, 3);
    }
    return TRUE;
}

BOOL SpNpcCopper::setupAct0E() {
    return TRUE;
}

BOOL SpNpcCopper::mainAct0E() {
    if (TalkWindow_Get(0)->state == 5) {
        SaveManager_RequestAct02();
        SceneWarp_RequestFade(Scene_GetWarpRequest(), 0x2e, 2, 3);
        SpNpcCopper_ChangeAct(this, 3);
    }
    return TRUE;
}

BOOL SpNpcCopper::setupAct0F() {
    return TRUE;
}

BOOL SpNpcCopper::mainAct0F() {
    s32 a, b;
    if (isNetOwner() != 0) {
        a = 4;
        b = 4;
        s32 u;
        s32 x;
        if (((NpcActor *)(this))->netGetSlots(&a, &b) != 0 && (x = a, u = gCommManager->myAid, x == u) && x == b) {
            netSetSlotsIfOwner(1, u, u);
            ((ActorTalkRequest *)&talk)->resetMsg();
            talk.setTalkPlayer(getPlayerActor(4));
            SpNpcCopper_ChangeAct(this, 2);
        } else if (NetArea_IsLocalOwner() != 0 && b == 4) {
            netSetSlotsIfOwner(1, gCommManager->myAid, 4);
            SpNpcCopper_ChangeAct(this, 1);
        }
    }
    return TRUE;
}

BOOL SpNpcCopper::setupAct11() {
    return TRUE;
}

BOOL SpNpcCopper::mainAct11() {
    s32 a, b;
    if (isNetOwner() != 0) {
        a = 4;
        b = 4;
        if (((NpcActor *)(this))->netGetSlots(&a, &b) != 0) {
            if (a == 4) {
                if (NetArea_IsLocalOwner() != 0) {
                    netSetSlotsIfOwner(1, gCommManager->myAid, 4);
                    SpNpcCopper_ChangeAct(this, 4);
                }
            }
        }
    }
    return TRUE;
}

BOOL SpNpcCopper::setupAct10() {
    return TRUE;
}

BOOL SpNpcCopper::mainAct10() {
    return TRUE;
}

SpNpcCopperTalk::SpNpcCopperTalk() {}

SpNpcCopperTalk::~SpNpcCopperTalk() {}

void SpNpcCopperTalk::attachOwner(SpNpcCopper *p) {
    resetMsg();
    owner = p;
    topic = 0xb;
}

void SpNpcCopperTalk::setTopic(s32 v) {
    topic = v;
}

s32 SpNpcCopperTalk::getTopic() {
    return topic;
}

void SpNpcCopperTalk::endComm() {
    if (Comm_End() != 0) {
        NetOverlay_Restore();
    }
}

void SpNpcCopperTalk::startComm(s32 a, s32 b) {
    switch (a) {
    case 3:
    case 4:
        NetOverlay_LoadWifi();
        break;
    case 1:
    case 2:
        NetOverlay_LoadWireless();
        break;
    case 0:
        break;
    }
    Comm_Start(a, 4, b);
}

void SpNpcCopperTalk::setFriendCodeArgs(s64 v) {
    s64 q1 = (u64)v / 100000000;
    s64 m1 = q1 * 100000000;
    s64 rem = v - m1;
    s64 q2 = (u64)rem / 10000;
    setNumberSlot((s32)q1, 5, 4, 6, 0);
    setNumberSlot((s32)q2, 8, 4, 6, 0);
    setNumberSlot((s32)v - (s32)(q2 * 10000) - (s32)m1, 9, 4, 6, 0);
}

BOOL SpNpcCopperTalk::hasFriends() {
    u8 *arr = (u8 *)FriendList_GetEntries(PlayerData_GetCurrent()->getFriendList());
    s32 i;
    for (i = 0; i < 0x20; i++) {
        if (DwcFriendData_IsValid(FriendEntry_GetFriendData(arr + i * 0x1c)) != 0) {
            return TRUE;
        }
    }
    return FALSE;
}

// SpNpcCopperTalk::start's static tbl, initialised at its first use after the overlay was loaded
TalkStartMsg SpNpcCopperTalk_start_tbl[11];
u8 SpNpcCopperTalk_start_tbl_init;
void SpNpcCopperTalk::start(TalkStartMsg *arg) {
    TalkStartMsg *out = (TalkStartMsg *)arg;
    if (!SpNpcCopperTalk_start_tbl_init) {
        HOST_STATIC_INIT_ARRAY(SpNpcCopperTalk_start_tbl, {
        {(const char *)sSpNpcCopperMsgKey, 0}, {(const char *)sSpNpcCopperMsgKey, 4}, {(const char *)sSpNpcCopperMsgKey, 5},
        {(const char *)sSpNpcCopperMsgKey, 6}, {(const char *)sSpNpcCopperMsgKey, 7}, {(const char *)sSpNpcCopperMsgKey, 0xa},
        {(const char *)sSpNpcCopperMsgKey, 0x68}, {(const char *)sSpNpcCopperMsgKey, 0xd}, {sSpNpcCopperSequence4Key, 9},
        {(const char *)sSpNpcCopperMsgKey, 0x47}, {(const char *)sSpNpcCopperMsgKey, 0x48},
    });
        SpNpcCopperTalk_start_tbl_init = 1;
    }
    void *h;
    saveDone = 0;
    wifiIdChanged = 0;
    h = PlayerData_GetCurrent();
    if (GameStart_IsActive() != 0) {
        setTopic(8);
    } else if (getTopic() == 6) {
        if (CheckInGate_IsOpen() == 0) {
            CheckInGate_Open();
        }
    } else if (getTopic() != 7 && getTopic() != 9 && getTopic() != 0xa) {
        if (Talk_IsInOwnTown() == 0) {
            setTopic(5);
        } else if (((PlayerData *)(h))->testFlag(5) == 0) {
            setTopic(0);
            ((PlayerData *)(h))->setFlag(5);
        } else {
            s32 t = Clock_GetTimeOfDay() + 1;
            setTopic(t);
        }
    }
    if (topic >= 0 && topic < 0xb) {
        out->msgIndex = SpNpcCopperTalk_start_tbl[topic].msgIndex;
        out->msgKey = SpNpcCopperTalk_start_tbl[topic].msgKey;
    }
}

void SpNpcCopperTalk::startSave() {
    lockWindow(0);
    setScript(0x15);
}

void SpNpcCopperTalk::showMainMenu() {
    u8 buf[2];
    s32 v;
    if (((CommManager *)(gCommManager))->isMyAid(0) != 0) {
        v = 0x10;
    } else {
        v = 0xf;
    }
    buf[0] = v;
    ((TalkWindowState *)(window))->setNextMessage(buf, sSpNpcCopperMsgKey);
}

void SpNpcCopperTalk::closeGate() {
    SpNpcCopper_ChangeAct(owner, 7);
    Comm_ResetNetSession();
    if (Net_GetMode() == 3 || Net_GetMode() == 4) {
        setScript(0x14);
    } else {
        endComm();
    }
}

void SpNpcCopperTalk::lockWindow(s32 flag) {
    void *p = window;
    if (flag != 0) {
        ((TalkWindowState *)(p))->showBusyIcon(1);
    } else {
        ((TalkWindowState *)(p))->showBusyIcon(0);
    }
    ((TalkWindowState *)(p))->lockAdvance();
}

void SpNpcCopperTalk::unlockWindow() {
    void *p = window;
    ((TalkWindowState *)(p))->hideBusyIcon();
    ((TalkWindowState *)(p))->unlockAdvance();
}

void SpNpcCopperTalk::startTurnBackAct() {
    SpNpcCopper_ChangeAct(owner, 5);
}

void SpNpcCopperTalk::startOpenGateAct() {
    SpNpcCopper_ChangeAct(owner, 6);
}

void SpNpcCopperTalk::startWifiLogin() {
    lockWindow(1);
    startComm(4, 2);
    owner->talk.timer = 0x960;
    setScript(0xa);
}

void SpNpcCopperTalk::startTownSearch() {
    lockWindow(1);
    startComm(2, 2);
    owner->talk.timer = 200;
    setScript(1);
}

void SpNpcCopperTalk::openTownList() {
    setSelectionList(hostBeacon, hostGameInfo, 0);
    openSubScene(4);
    setScript(3);
}

void SpNpcCopperTalk::openFriendList() {
    setSubSceneKind(0x3c, 0);
    openSubScene(2);
    setScript(2);
}

void SpNpcCopperTalk::startLocalVisit() {
    lockWindow(1);
    if (Net_GetMode() == 2) {
        ((CommManager *)(gCommManager))->setErrorMode(1);
    } else {
        startComm(2, 1);
    }
    owner->talk.timer = 200;
    setScript(4);
}

void SpNpcCopperTalk::startWifiVisit() {
    ((CommManager *)(gCommManager))->setErrorMode(1);
    if (Net_GetMode() != 4) {
        startWifiLogin();
    } else {
        s32 t;
        lockWindow(1);
        t = hostFriendId;
        NetOverlay_AssertWifi();
        Net_WifiConnectToHost(t);
        owner->talk.timer = 0x960;
        setScript(5);
    }
}

void SpNpcCopperTalk::startSendOffAct() {
    SpNpcCopper_ChangeAct(owner, 8);
}

void SpNpcCopperTalk::startFarewellAct() {
    SpNpcCopper_ChangeAct(owner, 0xa);
}

void SpNpcCopperTalk::showAnythingElseMenu() {
    u8 buf[2];
    buf[0] = getAnythingElseMsg();
    ((TalkWindowState *)(window))->setNextMessage(buf, sSpNpcCopperMsgKey);
}

void SpNpcCopperTalk::showWifiIdSavedResult() {
    u8 buf[2];
    if (wifiIdChanged != 0) {
        setFriendCodeArgs(Net_GetOwnFriendKey(PlayerWifiData_GetDwcUserData(PlayerData_GetCurrent()->getWifiUserData())));
        buf[0] = 0x76;
        ((TalkWindowState *)(window))->setNextMessage(buf, sSpNpcCopperMsgKey);
    } else {
        buf[1] = getWifiLoginResultMsg();
        ((TalkWindowState *)(window))->setNextMessage(&buf[1], sSpNpcCopperMsgKey);
    }
}

// ---- aad4

void SpNpcCopperTalk::startGoHome() {
    lockWindow(1);
    owner->talk.timer = 200;
    setScript(8);
}
extern "C" const VecFx32 sCopperGateCheckPos = {0x10000, 0x0, 0x10000};

extern "C" char sSpNpcCopperKey[] = {'s', 'p', '_', 'n', 'p', 'c', '_', 'g', 'a', 't', 'e', 'k', 'e', 'e', 'p', 'e', 'r', 0};

extern "C" void *sSpNpcCopperModelPathPtr = sSpNpcCopperModelPath;

extern "C" const VecFx32 sCopperTurnBackPos = {0x10000, 0x0, 0x11800};

extern "C" const VecFx32 sCopperSendOffExitPos = {0x10000, 0x0, 0x2000};

void SpNpcCopperTalk::onMessageEnd(u32) {
    static Unk_ov048_0225a8d4_Row tbl[28] = {
        {0x00, &SpNpcCopperTalk::showMainMenu},
        {0x04, &SpNpcCopperTalk::showMainMenu},
        {0x05, &SpNpcCopperTalk::showMainMenu},
        {0x06, &SpNpcCopperTalk::showMainMenu},
        {0x07, &SpNpcCopperTalk::showMainMenu},
        {0x38, &SpNpcCopperTalk::closeGate},
        {0x46, &SpNpcCopperTalk::startOpenGateAct},
        {0x54, &SpNpcCopperTalk::startTownSearch},
        {0x3e, &SpNpcCopperTalk::startWifiLogin},
        {0x5b, &SpNpcCopperTalk::startWifiLogin},
        {0x6c, &SpNpcCopperTalk::startOpenGateAct},
        {0x47, &SpNpcCopperTalk::startTurnBackAct},
        {0x48, &SpNpcCopperTalk::startTurnBackAct},
        {0x49, &SpNpcCopperTalk::startTurnBackAct},
        {0x25, &SpNpcCopperTalk::startWifiLogin},
        {0x55, &SpNpcCopperTalk::openTownList},
        {0x5d, &SpNpcCopperTalk::openFriendList},
        {0x6d, &SpNpcCopperTalk::openFriendList},
        {0x62, &SpNpcCopperTalk::startWifiVisit},
        {0x59, &SpNpcCopperTalk::startLocalVisit},
        {0x68, &SpNpcCopperTalk::startSendOffAct},
        {0x0d, &SpNpcCopperTalk::startFarewellAct},
        {0x11, &SpNpcCopperTalk::showAnythingElseMenu},
        {0x58, &SpNpcCopperTalk::showAnythingElseMenu},
        {0x61, &SpNpcCopperTalk::showAnythingElseMenu},
        {0x75, &SpNpcCopperTalk::showWifiIdSavedResult},
        {0x12, &SpNpcCopperTalk::startGoHome},
        {0x6a, &SpNpcCopperTalk::startSave},
    };
    s32 i = 0;
    u8 *p = &msgIndex;
    for (; (u32)i < 0x1c; i++) {
        u32 a = tbl[i].id;
        u32 b = *p;
        if (a == b) {
            (this->*tbl[i].f)();
        }
    }
}

void SpNpcCopperTalk::onChoiceUpdateWifiId(s32 p) {
    if (p == 0) {
        NetOverlay_LoadWifi();
        PlayerWifiData_Create((u8 *)PlayerData_GetCurrent()->getWifiUserData());
        NetOverlay_Restore();
    }
    onChoiceConnectWifi(p);
}

void SpNpcCopperTalk::onChoiceVisitTown(s32 p) {
    if (p == 2) {
        endComm();
    }
}

void SpNpcCopperTalk::onChoiceConnectWifi(s32 p) {
    if (p == 0) {
        u32 id = 0xff;
        switch (wifiPurpose) {
        case 2:
            afterSaveMsg = 0x5b;
            id = 0x6a;
            break;
        case 1:
            afterSaveMsg = 0x3e;
            id = 0x6a;
            break;
        case 0:
            id = 0x25;
            break;
        }
        if (id != 0xff) {
            u8 v = id;
            ((TalkWindowState *)(window))->setNextMessage(&v, sSpNpcCopperMsgKey);
        }
    }
}

void SpNpcCopperTalk::onChoiceFriendCode(s32 p) {
    if (p == 0) {
        wifiPurpose = 0;
        checkWifiReady();
    }
}

void SpNpcCopperTalk::onChoiceVisitMethod(s32 p) {
    if (p == 1) {
        wifiPurpose = 2;
        checkWifiReady();
    } else if (p == 2) {
        u8 v = getAnythingElseMsg();
        ((TalkWindowState *)(window))->setNextMessage(&v, sSpNpcCopperMsgKey);
    }
}

void SpNpcCopperTalk::onChoiceHostMethod(s32 p) {
    if (p == 1) {
        wifiPurpose = 1;
        checkWifiReady();
    } else if (p == 2) {
        u8 v = getAnythingElseMsg();
        ((TalkWindowState *)(window))->setNextMessage(&v, sSpNpcCopperMsgKey);
    }
}

void SpNpcCopperTalk::checkWifiReady() {
    u8 v0, v1, v2, v3, v4;
    if (wifiPurpose != 0 && hasFriends() == 0) {
        if (wifiPurpose == 2) {
            v0 = 0x6f;
            ((TalkWindowState *)(window))->setNextMessage(&v0, sSpNpcCopperMsgKey);
        } else {
            v1 = 0x70;
            ((TalkWindowState *)(window))->setNextMessage(&v1, sSpNpcCopperMsgKey);
        }
        return;
    }
    void *s = PlayerWifiData_GetDwcUserData(PlayerData_GetCurrent()->getWifiUserData());
    NetOverlay_LoadWifi();
    if (Net_HasWifiUserId(s)) {
        if (Net_IsWifiConfigValid(s)) {
            v2 = sCopperWifiStartMsgs[wifiPurpose];
            ((TalkWindowState *)(window))->setNextMessage(&v2, sSpNpcCopperMsgKey);
        } else {
            wifiIdChanged = 1;
            v3 = 0x72;
            ((TalkWindowState *)(window))->setNextMessage(&v3, sSpNpcCopperMsgKey);
        }
    } else {
        v4 = 0x71;
        ((TalkWindowState *)(window))->setNextMessage(&v4, sSpNpcCopperMsgKey);
    }
    NetOverlay_Restore();
}

void SpNpcCopperTalk::onChoiceGoHome(s32 p) {
    if (p == 0) {
        u8 v = 0x19;
        ((TalkWindowState *)(window))->setNextMessage(&v, sSpNpcCopperMsgKey);
    }
}

void SpNpcCopperTalk::onChoiceMainMenu(s32 p) {
    void *h = PlayerData_GetCurrent();
    u32 id = 0xff;
    switch (p) {
    case 0:
        if (((PlayerData *)(h))->testFlag(1)) {
            id = 8;
        } else {
            CommManager *g = gCommManager;
            if (((CommManager *)(g))->isOnline() && ((CommManager *)(g))->isMyAid(0)) {
                id = 0x50;
            } else if (((CommManager *)(gCommManager))->isSlotActive(gCommManager->myAid)) {
                id = 0x51;
            } else {
                id = 0x52;
            }
        }
        break;
    case 1:
        if (((PlayerData *)(h))->testFlag(1)) {
            id = 0xe;
        } else {
            id = getInviteOrCloseGateMsg();
        }
        break;
    case 2:
        if (((CommManager *)(gCommManager))->isMyAid(0) == 0) {
            if (((PlayerData *)(h))->testFlag(1)) {
                id = 8;
            } else {
                void *s;
                NetOverlay_LoadWifi();
                s = PlayerWifiData_GetDwcUserData(((PlayerData *)(h))->getWifiUserData());
                if (Net_IsWifiConfigValid(s)) {
                    if (Net_HasWifiUserId(s)) {
                        id = 0x29;
                        s64 t = Net_GetOwnFriendKey(s);
                        ((SpNpcCopperTalk *)(this))->setFriendCodeArgs(t);  // (all 64 bits, as the DS passes them: r0:r1 to r1:r2 - a friend code has 12 digits)
                    } else {
                        id = 0x23;
                    }
                } else {
                    id = 0x2f;
                }
                NetOverlay_Restore();
            }
        }
        break;
    }
    if (id != 0xff) {
        u8 v = id;
        ((TalkWindowState *)(window))->setNextMessage(&v, sSpNpcCopperMsgKey);
    }
}

void SpNpcCopperTalk::onChoiceHostMenu(s32 p) {
    void *h = PlayerData_GetCurrent();
    u32 id = 0xff;
    switch (p) {
    case 0:
        if (((PlayerData *)(h))->testFlag(1)) {
            id = 8;
        } else {
            CommManager *g = gCommManager;
            if (((CommManager *)(g))->isOnline() && ((CommManager *)(g))->isMyAid(0)) {
                id = 0x50;
            } else if (((CommManager *)(gCommManager))->isSlotActive(gCommManager->myAid)) {
                id = 0x51;
            } else {
                id = 0x52;
            }
        }
        break;
    case 1:
        if (((PlayerData *)(h))->testFlag(1)) {
            id = 0xe;
        } else {
            id = getInviteOrCloseGateMsg();
        }
        break;
    }
    if (id != 0xff) {
        u8 v = id;
        ((TalkWindowState *)(window))->setNextMessage(&v, sSpNpcCopperMsgKey);
    }
}

void SpNpcCopperTalk::onChoiceBackToMenu(s32 p) {
    if (p == 1) {
        u8 v = getAnythingElseMsg();
        ((TalkWindowState *)(window))->setNextMessage(&v, sSpNpcCopperMsgKey);
    }
}

void SpNpcCopperTalk::onChoiceHostLocal(s32 p) { confirmStartComm(p, 0x46); }

void SpNpcCopperTalk::onChoiceHostWifi(s32 p) { confirmStartComm(p, 0x3e); }

void SpNpcCopperTalk::onChoiceVisitLocal(s32 p) { confirmStartComm(p, 0x54); }

void SpNpcCopperTalk::onChoiceVisitWifi(s32 p) { confirmStartComm(p, 0x5b); }

void SpNpcCopperTalk::confirmStartComm(s32 p, s32 id) {
    u8 v0;
    u8 v1;
    u8 v2;
    if (p == 0) {
        afterSaveMsg = id;
        if (saveDone != 0) {
            v0 = afterSaveMsg;
            ((TalkWindowState *)(window))->setNextMessage(&v0, sSpNpcCopperMsgKey);
        } else {
            v1 = 0x6a;
            ((TalkWindowState *)(window))->setNextMessage(&v1, sSpNpcCopperMsgKey);
        }
    }
    if (p == 1) {
        v2 = getAnythingElseMsg();
        ((TalkWindowState *)(window))->setNextMessage(&v2, sSpNpcCopperMsgKey);
    }
}

void SpNpcCopperTalk::onChoiceGoOutInstead(s32 p) {
    if (p == 0) {
        u8 v = 0x52;
        ((TalkWindowState *)(window))->setNextMessage(&v, sSpNpcCopperMsgKey);
        CheckInGate_Close();
        Comm_ResetNetSession();
        if (Net_GetMode() == 3 || Net_GetMode() == 4) {
            setScript(0x14);
        } else {
            endComm();
        }
    }
}

void SpNpcCopperTalk::onChoiceRetryLocal(s32 p) {
    if (p == 0) {
        u8 v = 0x59;
        ((TalkWindowState *)(window))->setNextMessage(&v, sSpNpcCopperMsgKey);
    }
}

void SpNpcCopperTalk::onChoiceRetryWifi(s32 p) {
    if (p == 0) {
        u8 v = 0x62;
        ((TalkWindowState *)(window))->setNextMessage(&v, sSpNpcCopperMsgKey);
    }
}

void SpNpcCopperTalk::onChoiceSaveAndQuit(s32 p) {
    TalkWindowState *o = (TalkWindowState *)window;
    if (p == 0) {
        o->openMode = 0;
        SpNpcCopper_ChangeAct(owner, 0xe);
    } else if (p == 1) {
        u8 v = getAnythingElseMsg();
        ((TalkWindowState *)(o))->setNextMessage(&v, sSpNpcCopperMsgKey);
    }
}

void SpNpcCopperTalk::onChoiceGateAlreadyOpen(s32 p) {}

void SpNpcCopperTalk::onChoiceUnk5F(s32 p) {
    switch (p) {
    case 0:
        setScript(0x14);
        break;
    case 1: {
        u8 v = 0x6c;
        ((TalkWindowState *)(window))->setNextMessage(&v, sSpNpcCopperMsgKey);
        break;
    }
    case 2:
        setScript(0x14);
        break;
    }
}
extern "C" char sSpNpcCopperTexturePath[] = {'n', 'p', 'c', '_', 's', 'p', '/', 'm', 'o', 'd', 'e', 'l', '/', 'p', 'l', 'c', '_', 't', 'e', 'x', '.', 'n', 's', 'b', 't', 'x', 0};

extern "C" ActorProfile sSpNpcCopperProfile = {(void *(*)())SpNpcCopper_Create, 0x72, 0x77, 2, 0x5000, 0x5000, 0x3e800};

extern "C" const VecFx32 sCopperSendOffWalkPos = {0x10000, 0x0, 0x13000};

extern "C" const VecFx32 sCopperArrivalWalkPos = {0x10000, 0x0, 0x11800};

void SpNpcCopperTalk::onChoice(u32) {
    if (GameStart_IsActive() == 0) {
        static SpNpcCopperChoiceHandler tbl[29] = {
            {0x15, &SpNpcCopperTalk::onChoiceHostLocal},
            {0x41, &SpNpcCopperTalk::onChoiceHostWifi},
            {0x14, &SpNpcCopperTalk::onChoiceVisitLocal},
            {0x7f, &SpNpcCopperTalk::onChoiceVisitWifi},
            {0x24, &SpNpcCopperTalk::onChoiceBackToMenu},
            {0x56, &SpNpcCopperTalk::onChoiceBackToMenu},
            {0x0a, &SpNpcCopperTalk::onChoiceGoHome},
            {0x10, &SpNpcCopperTalk::onChoiceHostMenu},
            {0x21, &SpNpcCopperTalk::onChoiceHostMenu},
            {0x0f, &SpNpcCopperTalk::onChoiceMainMenu},
            {0x22, &SpNpcCopperTalk::onChoiceMainMenu},
            {0x51, &SpNpcCopperTalk::onChoiceGoOutInstead},
            {0x63, &SpNpcCopperTalk::onChoiceRetryLocal},
            {0x65, &SpNpcCopperTalk::onChoiceRetryLocal},
            {0x7d, &SpNpcCopperTalk::onChoiceRetryWifi},
            {0x7e, &SpNpcCopperTalk::onChoiceRetryWifi},
            {0x80, &SpNpcCopperTalk::onChoiceRetryWifi},
            {0x82, &SpNpcCopperTalk::onChoiceRetryWifi},
            {0x2c, &SpNpcCopperTalk::onChoiceSaveAndQuit},
            {0x6b, &SpNpcCopperTalk::onChoiceGateAlreadyOpen},
            {0x5f, &SpNpcCopperTalk::onChoiceUnk5F},
            {0x23, &SpNpcCopperTalk::onChoiceFriendCode},
            {0x28, &SpNpcCopperTalk::onChoiceFriendCode},
            {0x2f, &SpNpcCopperTalk::onChoiceFriendCode},
            {0x52, &SpNpcCopperTalk::onChoiceVisitMethod},
            {0x3d, &SpNpcCopperTalk::onChoiceHostMethod},
            {0x71, &SpNpcCopperTalk::onChoiceConnectWifi},
            {0x74, &SpNpcCopperTalk::onChoiceUpdateWifiId},
            {0x57, &SpNpcCopperTalk::onChoiceVisitTown},
        };
        s32 i = 0;
        u8 *p = &msgIndex;
        for (; (u32)i < 0x1d; i++) {
            u32 off = i * 12;
            u32 a = tbl[i].id;
            u32 b = *p;
            if (a == b) {
                s32 arg = ((ChoiceList *)(((TalkWindowState *)(window))->getChoiceList()))->getResult();
                SpNpcCopperChoiceHandler *r = &tbl[i];
                (this->*r->f)(arg);
            }
        }
    }
}

void SpNpcCopperTalk::update() {
    if (sSpNpcCopperTalkScripts[script].flag != 0) {
        if (sSpNpcCopperTalkScripts[script].f) {
            (this->*sSpNpcCopperTalkScripts[script].f)();
        }
    }
}

void SpNpcCopperTalk::onTaskDone(u32) {
    if (sSpNpcCopperTalkScripts[script].flag == 0) {
        if (sSpNpcCopperTalkScripts[script].f) {
            (this->*sSpNpcCopperTalkScripts[script].f)();
        }
    }
}

// Tiny setter last so it is not inlined into callers.
void SpNpcCopperTalk::setScript(s32 s) { script = s; }

void SpNpcCopperTalk::waitWifiLogin() {
    NetOverlay_AssertAny();
    Net_PollConnected();
    if (checkWifiLoginError() == 0) {
        NetOverlay_AssertWifi();
        if (Net_GetWifiFriendList() != 0) {
            if (wifiPurpose == 0) {
                setScript(0x13);
            } else {
                setScript(script + 1);
            }
        }
    }
}

// ---- a000

void SpNpcCopperTalk::startMailDownload() {
    if (AxMail_DownloadMail()) {
        setScript(script + 1);
    } else {
        setScript(script + 2);
    }
}

void SpNpcCopperTalk::pollMailDownload() {
    if (isSkippableNetError()) {
        Net_ClearWifiError();
        setScript(script + 1);
    } else if (AxMail_PollMail()) {
        setScript(script + 1);
    }
}

void SpNpcCopperTalk::startBbsDownload() {
    if (AxMail_DownloadBbs()) {
        setScript(script + 1);
    } else {
        setScript(script + 2);
    }
}

void SpNpcCopperTalk::pollBbsDownload() {
    if (isSkippableNetError()) {
        Net_ClearWifiError();
        setScript(script + 1);
    } else if (AxMail_PollBbs()) {
        setScript(script + 1);
    }
}

void SpNpcCopperTalk::startGameStatsUpload() {
    if (GameStats_Upload()) {
        setScript(script + 1);
    } else {
        setScript(script + 2);
    }
}

void SpNpcCopperTalk::pollGameStatsUpload() {
    if (isSkippableNetError()) {
        Net_ClearWifiError();
        setScript(script + 1);
    } else if (GameStats_PollUpload()) {
        setScript(script + 1);
    }
}

void SpNpcCopperTalk::startGameStatsDownload() {
    if (GameStats_Download()) {
        setScript(script + 1);
    } else {
        setScript(script + 2);
    }
}

void SpNpcCopperTalk::pollGameStatsDownload() {
    if (isSkippableNetError()) {
        Net_ClearWifiError();
        setScript(script + 1);
    } else if (GameStats_PollDownload()) {
        setScript(script + 1);
    }
}

BOOL SpNpcCopperTalk::isSkippableNetError() {
    s32 v = Net_GetLastErrorCode();
    if (v < 0) {
        v = -v;
    }
    if (Math_IsInRange(v, 0x17ed0, 0x182b7)) {
        return TRUE;
    }
    return FALSE;
}

void SpNpcCopperTalk::onWifiLoggedIn() {
    u8 *g = (u8 *)gCommManager;
    u8 *r4 = ((CommManager *)(g))->getWifiUserData();
    void *r6 = PlayerData_GetCurrent();
    u8 m;
    s32 mv;
    if (msgIndex == 0x62) {
        u8 *r6b = ((CommManager *)(g))->getWifiFriendList();
        s32 h = hostFriendId;
        s32 k;
        NetOverlay_AssertWifi();
        k = Net_WifiFindFriend(h);
        u8 *e = r6b + k * 0x13;
        if (e[0x190] != 6 || k == -1) {
            showErrorAndAbort(0x78, 1);
        } else {
            h = hostFriendId;
            NetOverlay_AssertWifi();
            Net_WifiConnectToHost(h);
            owner->talk.timer = 0x960;
            setScript(5);
        }
    } else {
        if (Net_CheckUserDataChanged(r4 + 0x10)) {
            Net_MakeOwnFriendData(r4 + 0x10, DwcFriendData_GetBytes(PlayerWifiData_GetOwnFriendData((u8 *)((PlayerData *)(r6))->getWifiUserData())));
            MI_CpuCopy8(r4 + 0x10, PlayerWifiData_GetDwcUserData(((PlayerData *)(r6))->getWifiUserData()), 0x40);
            if (Save_WritePlayerWifiData() == 0) {
                mv = 0x75;
            } else {
                mv = 2;
            }
        } else {
            mv = getWifiLoginResultMsg();
        }
        m = mv;
        ((TalkWindowState *)(window))->setNextMessage(&m, sSpNpcCopperMsgKey);
        if (wifiPurpose == 0 || wifiIdChanged != 0) {
            setScript(0x14);
        } else {
            unlockWindow();
            setScript(0);
        }
    }
}

void SpNpcCopperTalk::scanForOpenTowns() {
    s32 n;
    void **arr;
    void *p;
    void *r7;
    u8 i;
    u8 buf[0x14];
    if (checkScanTimeout() == 0) {
      Comm_SendEmpty();
      n = Net_CountHostCandidates();
      if (n > 0) {
        NetOverlay_AssertWireless();
        arr = Net_GetScanResults();
        MsgString9C A;
        EncodedString8B B;
        MsgString9B C;
        EncodedString8 D;
        r7 = window;
        for (i = 0; i < n; i++) {
            p = arr[i];
            s32 t;
            if (p) {
                NetOverlay_AssertWireless();
                t = Net_GetBeaconGameInfoSize(p);
                if (t == 0x11) {
                    NetOverlay_AssertWireless();
                    MI_CpuCopy8(Net_GetBeaconGameInfo(p), &buf[1], t);
                    if (buf[0x11] == 0) {
                        MI_CpuCopy8(p, hostBeacon, 0xe0);
                        EncodedString_SetRaw(&B, &buf[1], 8);
                        A.fromEncoded(&B, 0, 0);
                        EncodedString_SetRaw(&D, &buf[9], 8);
                        C.fromEncoded(&D, 0, 0);
                        ((TalkWindowState *)(r7))->setSlot(3, &A);
                        ((TalkWindowState *)(r7))->setSlot(4, &C);
                        buf[0] = 0x57;
                        ((TalkWindowState *)(r7))->setNextMessage(buf, sSpNpcCopperMsgKey);
                        unlockWindow();
                        setScript(0);
                        break;
                    }
                }
            }
        }
    }
    }
}

void SpNpcCopperTalk::onFriendListClosed() {
    if (MenuCtrl_IsResultOk()) {
        s32 r5 = MenuCtrl_GetIndex();
        u8 a;
        NetOverlay_AssertWifi();
        hostFriendId = Net_WifiGetFriendProfileId(r5);
        a = 0x62;
        ((TalkWindowState *)(window))->setNextMessage(&a, sSpNpcCopperMsgKey);
        setScript(0);
    } else {
        u8 b;
        setScript(0x14);
        b = 0x61;
        ((TalkWindowState *)(window))->setNextMessage(&b, sSpNpcCopperMsgKey);
    }
}

void SpNpcCopperTalk::onTownListClosed() {
    if (MenuCtrl_IsResultOk()) {
        u8 a = 0x59;
        ((TalkWindowState *)(window))->setNextMessage(&a, sSpNpcCopperMsgKey);
    } else {
        u8 b = 0x58;
        ((TalkWindowState *)(window))->setNextMessage(&b, sSpNpcCopperMsgKey);
        endComm();
    }
    setScript(0);
}

void SpNpcCopperTalk::shutdownWifi() {
    void *t = window;
    if (t) {
        ((TalkWindowState *)(t))->lockAdvance();
    }
    NetOverlay_AssertWifi();
    if (Net_WifiShutdownStepExt()) {
        if (t) {
            unlockWindow();
        }
        void *pd;
        if ((pd = PlayerData_GetCurrent()) != NULL) {
            s32 r = ((PlayerData *)(pd))->getIndex();
            if (r >= 0 && r < 4) {
                u8 b;
                GameStats_ApplyDownload();
                AxMail_ApplyMail();
                AxMail_ApplyBbs();
                endComm();
                Wifi_StoreFriendList();
                if (Save_WritePlayerFriendList()) {
                    b = 2;
                    ((TalkWindowState *)(window))->setNextMessage(&b, sSpNpcCopperMsgKey);
                }
            }
        }
        setScript(0);
    }
}

void SpNpcCopperTalk::requestSave() {
    if (owner->canStartSave()) {
        SaveManager_RequestAct13();
        setScript(0x16);
    } else {
        SpNpcKatie_ResetAct();
    }
}

void SpNpcCopperTalk::waitSaveDone() {
    void *t = window;
    if (SaveManager_HasAct13Failed()) {
        u8 a;
        unlockWindow();
        a = 0xa;
        ((TalkWindowState *)(t))->setNextMessage(&a, (void *)"sp_etc_sequence2");
        setScript(0);
    } else if (SaveManager_IsIdleAfterAct13()) {
        u8 b;
        saveDone = 1;
        unlockWindow();
        b = afterSaveMsg;
        ((TalkWindowState *)(t))->setNextMessage(&b, sSpNpcCopperMsgKey);
        SpNpcKatie_ChangeAct05();
        setScript(0);
    }
}

BOOL SpNpcCopperTalk::checkNetError(s32 flag) {
    s32 code = Net_GetError();
    if (code != 0 || flag != 0) {
        if (flag != 0) {
            s32 r = getConnectErrorMsg();
            showErrorAndAbort(r, 0);
        } else if (Net_GetMode() == 3 || Net_GetMode() == 4) {
            s32 v = Net_GetLastErrorCode();
            s32 q;
            if (v < 0) {
                v = -v;
            }
            q = v / 1000;
            MsgString25 a;
            MsgString25 b;
            String_FormatNumber(&a, q, 2, 6, 0, 0);
            String_FormatNumber(&b, v - q * 1000, 3, 6, 0, 0);
            ((TalkWindowState *)(window))->setSlot(6, &a);
            ((TalkWindowState *)(window))->setSlot(7, &b);
            if (code == 0x400b) {
                showErrorAndAbort(0x7d, 0);
            } else if (code == 0x400a) {
                showErrorAndAbort(0x78, 0);
            } else if (v == 0x13a1a) {
                showErrorAndAbort(0x7a, 0);
            } else {
                showErrorAndAbort(0x7e, 0);
            }
        } else {
            s32 r = getConnectErrorMsg();
            showErrorAndAbort(r, 0);
        }
        return TRUE;
    }
    return FALSE;
}

BOOL SpNpcCopperTalk::checkConnectTimeout() {
    if (Math_CountDownU16(&owner->talk.timer) == 0) {
        s32 r = getConnectErrorMsg();
        showErrorAndAbort(r, 0);
        return TRUE;
    }
    return FALSE;
}

BOOL SpNpcCopperTalk::checkFindTownTimeout() {
    if (Math_CountDownU16(&owner->talk.timer) == 0) {
        showErrorAndAbort(0x65, 0);
        return TRUE;
    }
    return FALSE;
}

BOOL SpNpcCopperTalk::checkWifiLoginError() {
    if (Math_CountDownU16(&owner->talk.timer) == 0) {
        showErrorAndAbort(0x7a, 1);
        return TRUE;
    }
    if (Net_GetError() != 0) {
        s32 v = Net_GetLastErrorCode();
        if (v < 0) {
            v = -v;
        }
        s32 r = getWifiErrorMsg(v);
        showErrorAndAbort(r, 1);
        return TRUE;
    }
    return FALSE;
}

BOOL SpNpcCopperTalk::checkScanTimeout() {
    if (Math_CountDownU16(&owner->talk.timer) == 0) {
        showErrorAndAbort(0x56, 0);
        return TRUE;
    }
    return FALSE;
}

void SpNpcCopperTalk::showErrorAndAbort(u32 msg, s32 unused) {
    u8 b = msg;
    ((TalkWindowState *)(window))->setNextMessage(&b, sSpNpcCopperMsgKey);
    if (Net_GetMode() == 3 || Net_GetMode() == 4) {
        setScript(0x14);
    } else {
        unlockWindow();
        endComm();
        setScript(0);
    }
}

// ---- 96e8

void SpNpcCopperTalk::connectToTown() {
    BOOL z = FALSE;
    s32 n;
    void **arr;
    u8 i;
    u8 buf[0x18];
    if (checkNetError(0)) {
        return;
    }
    if (checkFindTownTimeout()) {
        return;
    }
    Comm_SendEmpty();
    n = Net_CountHostCandidates();
    if (n > 0) {
        NetOverlay_AssertWireless();
        arr = Net_GetScanResults();
        for (i = z; i < n; i++) {
            u8 *p = (u8 *)arr[i];
            s32 t;
            BOOL hit;
            if (p) {
                NetOverlay_AssertWireless();
                t = Net_GetBeaconGameInfoSize(p);
                if (t == 0x11) {
                    NetOverlay_AssertWireless();
                    MI_CpuCopy8(Net_GetBeaconGameInfo(p), buf, t);
                    if (buf[0x10] == 0) {
                        if (p[2] == hostBeacon[2] && p[3] == hostBeacon[3] && p[4] == hostBeacon[4] && p[5] == hostBeacon[5] &&
                            p[6] == hostBeacon[6] && p[7] == hostBeacon[7]) {
                            hit = TRUE;
                        } else {
                            hit = z;
                        }
                        if (hit) {
                            NetOverlay_AssertWireless();
                            Net_ConnectToParent(p);
                            owner->talk.timer = 200;
                            setScript(5);
                        }
                    }
                }
            }
        }
    }
}

void SpNpcCopperTalk::waitConnected() {
    if (checkNetError(0) == 0 && checkConnectTimeout() == 0) {
        NetOverlay_AssertAny();
        if (Net_PollConnected()) {
            if (Net_GetMode() != 3) {
                Net_GetMode();
            }
            owner->talk.timer = 200;
            setScript(6);
            Comm_PrepareJoin(Net_GetMyAid());
        }
    }
}

void SpNpcCopperTalk::requestJoin() {
    NetOverlay_AssertAny();
    BOOL b;
    if (Net_PollConnected() == 0) {
        b = TRUE;
    } else {
        b = FALSE;
    }
    if (checkNetError(b)) {
        Comm_ClearSyncState();
    } else if (Comm_RequestSync(0)) {
        syncPayload = 0;
        setScript(7);
    }
}

void SpNpcCopperTalk::waitJoinAccepted() {
    NetOverlay_AssertAny();
    BOOL b;
    if (Net_PollConnected() == 0) {
        b = TRUE;
    } else {
        b = FALSE;
    }
    if (checkNetError(b)) {
        Comm_ClearSyncState();
        if (((KatieVisitState *)TownSessionState_GetKatieState(TownSessionState_Get()))->isFollowing()) {
            if (((LostChildRecord *)(PlayerData_GetCurrent()->getLostChildRecord()))->isEscorting()) {
                ((LostChildRecord *)(PlayerData_GetCurrent()->getLostChildRecord()))->clearEscorting();
            }
        }
    } else {
        s32 t = Comm_GetSyncState();
        if (t == 5) {
            if (((KatieVisitState *)TownSessionState_GetKatieState(TownSessionState_Get()))->isFollowing()) {
                if (((LostChildRecord *)(PlayerData_GetCurrent()->getLostChildRecord()))->isEscorting() == 0) {
                    ((LostChildRecord *)(PlayerData_GetCurrent()->getLostChildRecord()))->setEscorting();
                }
            }
            if (CommSend_PlayerData(&syncPayload, NetSession_GetSyncMemberMask())) {
                ((CommManager *)(gCommManager))->setErrorMode(0);
                NetSession_SetSyncKind(0);
                NetSession_SetActiveSyncKind(0);
                Net_SetJoiningAid(Net_GetMyAid());
                Comm_ClearSyncState();
                window->openMode = 0;
                unlockWindow();
                setScript(0);
                SpNpcCopper_ChangeAct(owner, 0xc);
            }
        } else if (t == 6 || Net_GetError() == 0x800c) {
            Comm_ClearSyncState();
            if (Net_GetError() == 0x800c) {
                showErrorAndAbort(0x79, 1);
            } else {
                showErrorAndAbort(getJoinErrorMsg(), 0);
            }
        }
    }
}

void SpNpcCopperTalk::requestGoHome() {
    Comm_RequestSync(1);
    setScript(9);
}

void SpNpcCopperTalk::waitGoHomeAccepted() {
    TalkWindowState *o = (TalkWindowState *)window;
    s32 t = Comm_GetSyncState();
    if ((u32)(t - 5) <= 1) {
        unlockWindow();
        setScript(0);
        if (t == 5) {
            o->openMode = 0;
            SpNpcCopper_ChangeAct(owner, 0xd);
            NetSession_SetSyncKind(1);
            NetSession_SetActiveSyncKind(1);
        } else {
            u8 b[4];
            b[0] = 0xc;
            ((TalkWindowState *)(o))->setNextMessage(b, sSpNpcCopperMsgKey);
        }
        Comm_ClearSyncState();
    }
}

s32 SpNpcCopperTalk::getWifiLoginResultMsg() {
    u8 *p = ((CommManager *)(gCommManager))->getWifiFriendList();
    s32 n = 0;
    s32 i;
    for (i = 0; i < 0x20; i++) {
        if (*(p + i * 0x13 + 0x190) == 6) {
            n++;
        }
    }
    s32 r = 0;
    switch (wifiPurpose) {
    case 0: {
        NetOverlay_LoadWifi();
        r = 0x26;
        s64 t = Net_GetOwnFriendKey(PlayerWifiData_GetDwcUserData(PlayerData_GetCurrent()->getWifiUserData()));
        ((SpNpcCopperTalk *)(this))->setFriendCodeArgs(t);  // (all 64 bits, as the DS passes them: r0:r1 to r1:r2 - a friend code has 12 digits)
        NetOverlay_Restore();
        break;
    }
    case 1:
        if (n != 0) {
            r = 0x6b;
        } else {
            r = 0x46;
        }
        break;
    case 2:
        r = 0x5d;
        break;
    }
    return r;
}

s32 SpNpcCopperTalk::getInviteOrCloseGateMsg() {
    void *g = gCommManager;
    if (((CommManager *)(g))->isMyAid(0)) {
        if (((CommManager *)(g))->isOnline()) {
            return 0x17;
        }
        if (Net_GetMode() == 3) {
            return 0x3a;
        }
        return 0x38;
    }
    return 0x3d;
}

u32 SpNpcCopperTalk::getAnythingElseMsg() {
    return (u8)(((CommManager *)(gCommManager))->isMyAid(0) ? 0x21 : 0x22);
}

u32 SpNpcCopperTalk::getConnectErrorMsg() {
    BOOL r = TRUE;
    if (Net_GetMode() != 3 && Net_GetMode() != 4) {
        r = FALSE;
    }
    return (u8)(r ? 0x82 : 0x65);
}

u32 SpNpcCopperTalk::getJoinErrorMsg() {
    BOOL r = TRUE;
    if (Net_GetMode() != 3 && Net_GetMode() != 4) {
        r = FALSE;
    }
    return (u8)(r ? 0x80 : 0x63);
}

s32 SpNpcCopperTalk::getWifiErrorMsg(s32 id) {
    s32 r = 0x84;
    if (id == 0x4e85 || id == 0x5bcf || RNG(id, 0x59d8, 0x5dbf)) {
        r = 0x83;
    } else if (RNG(id, 0x4e86, 0x4e8b) || id == 0x4e8d || RNG(id, 0x4e8f, 0x5207) || RNG(id, 0xcb24, 0xcb82) ||
               RNG(id, 0xcc4c, 0xccaf) || RNG(id, 0xcf08, 0xcf6b) || RNG(id, 0xcf6c, 0xcfcf) ||
               RNG(id, 0xcfd0, 0xd033)) {
        r = 0x84;
    } else if (id == 0x4e8c) {
        r = 0x85;
    } else if (id == 0x4e8e) {
        r = 0x86;
    } else if (id == 0xc3b3) {
        r = 0x87;
    } else if (RNG(id, 0xc79b, 0xc79e) || RNG(id, 0xc7a0, 0xc7fe) || RNG(id, 0xc864, 0xc8c6)) {
        r = 0x88;
    } else if (id == 0xc79f) {
        r = 0x89;
    } else if (RNG(id, 0xc800, 0xc863)) {
        r = 0x8a;
    } else if (RNG(id, 0xcb20, 0xcb23) || RNG(id, 0xcb84, 0xcb87) || RNG(id, 0xcbe8, 0xcbeb)) {
        r = 0x8b;
    }
    s32 q = id / 1000;
    MsgString25 a;
    MsgString25 b;
    String_FormatNumber(&a, q, 2, 6, 0, 0);
    String_FormatNumber(&b, id - q * 1000, 3, 6, 0, 0);
    window->setSlot(6, &a);
    window->setSlot(7, &b);
    return r;
}

BOOL SpNpcCopper::acceptsInteraction(void *) {
    if (((NpcTalkCtrl *)(&talkCtrl))->isBusy() || netIsTalkLocked()) {
        return FALSE;
    }
    return TRUE;
}

BOOL SpNpcCopper::acceptsSelfRequestedInteraction(void *) {
    if (netSyncOff != 0) {
        return TRUE;
    }
    if (((NpcTalkCtrl *)(&talkCtrl))->isBusy() || netIsTalkLocked()) {
        return FALSE;
    }
    return TRUE;
}

void SpNpcCopper::onInteractionEvent(u32 cmd, u8 arg) {
    s32 a, b;
    switch (cmd) {
    case 3:
        partnerPlayer = arg;
        if (arg != 4) {
            netSetSlotsIfOwner(1, gCommManager->myAid, arg);
            SpNpcCopper_ChangeAct(this, 0x10);
        } else if (isNetOwner()) {
            s32 g = gCommManager->myAid;
            netSetSlotsIfOwner(1, g, g);
            SpNpcCopper_ChangeAct(this, 0x10);
        }
        break;
    case 1: {
        ((ActorTalkRequest *)&talk)->resetMsg();
        talk.setTalkPlayer(getPlayerActor(4));
        SpNpcCopper_ChangeAct(this, 2);
        break;
    }
    case 0:
        partnerPlayer = arg;
        if (arg != 4 && arg != gCommManager->myAid) {
            netSetSlotsIfOwner(1, arg, arg);
            SpNpcCopper_ChangeAct(this, 0x11);
        } else if (isNetOwner()) {
            s32 g = gCommManager->myAid;
            netSetSlotsIfOwner(1, g, g);
            ((ActorTalkRequest *)&talk)->resetMsg();
            talk.setTalkPlayer(getPlayerActor(4));
            SpNpcCopper_ChangeAct(this, 2);
        }
        break;
    case 2:
    case 5:
    case 6:
    case 7:
        break;
    case 8:
        if (arg == 4) {
            if (NetArea_IsLocalOwner()) {
                netSetSlotsIfOwner(1, gCommManager->myAid, 4);
                SpNpcCopper_ChangeAct(this, 4);
            } else {
                netSetSlotsIfOwner(1, 4, gCommManager->myAid);
                SpNpcCopper_ChangeAct(this, 0xf);
            }
        }
        talk.setTopic(0xb);
        break;
    case 4:
        if (netIsTalkLocked()) {
            if (isNetOwner()) {
                a = 4;
                b = 4;
                if (((NpcActor *)(this))->netGetSlots(&a, &b)) {
                    if ((arg != 4 && (s32)arg == b) || arg == 4) {
                        netSetSlotsIfOwner(1, gCommManager->myAid, 4);
                        SpNpcCopper_ChangeAct(this, 1);
                    }
                }
            }
        }
        break;
    }
}

BOOL SpNpcCopper::checkPlayerAtGate() {
    CommManager *g = gCommManager;
    VecFx32Ctor v;
    if (((CommManager *)(g))->isOnline()) {
        return FALSE;
    }
    if (Scene_GetCurrent() == 0x2f) {
        return FALSE;
    }
    if (TalkRequest_IsActive()) {
        return FALSE;
    }
    if (((CommManager *)(gCommManager))->isSlotActive(gCommManager->myAid) == 0) {
        return FALSE;
    }
    VecFx32 *p = (VecFx32 *)PlayerActor_GetBodyPos(4);
    static_cast<VecFx32 &>(v) = *p;
    if (v.z <= sCopperGateCheckPos.z) {
        PlayerData_GetCurrent();
        if (((CommManager *)(g))->isOnline()) {
            talk.setTopic(9);
        } else {
            talk.setTopic(10);
        }
        TalkRequest_AddPlayerTalk6(this, 0);
        return TRUE;
    }
    return FALSE;
}

BOOL SpNpcCopper::canPlayTalkMelody() {
    if (Scene_GetCurrent() == 0xc) {
        return FALSE;
    }
    return SpNpcActor::canPlayTalkMelody();
}

// ---- 8de0

BOOL SpNpcCopper::canStartSave() {
    if (((NpcFaceAnim *)(&faceAnim))->getMouthAnim() == 0xba && SpNpcKatie_IsIdle() && talk.window->state == 2) {
        return TRUE;
    }
    return FALSE;
}

// ---- overlay variables (generated: tools/host_overlay_vars.py) ----
#include "host_overlay_vars.h"

HOST_OVERLAY_VARS(unk_ov048_02258de0) {
    HOST_OVERLAY_VAR(sSpNpcCopperSequence4Key),
    HOST_OVERLAY_VAR(sSpNpcCopperModelPath),
    HOST_OVERLAY_VAR(sSpNpcCopperTexturePathPtr),
    HOST_OVERLAY_VAR(sSpNpcCopperTalkScripts),
    HOST_OVERLAY_VAR(sSpNpcCopperMsgKey),
    HOST_OVERLAY_VAR(sSpNpcCopperActTable),
    HOST_OVERLAY_VAR(SpNpcCopperTalk_start_tbl),
    HOST_OVERLAY_VAR(SpNpcCopperTalk_start_tbl_init),
    HOST_OVERLAY_VAR(sSpNpcCopperKey),
    HOST_OVERLAY_VAR(sSpNpcCopperModelPathPtr),
    HOST_OVERLAY_VAR(sSpNpcCopperTexturePath),
    HOST_OVERLAY_VAR(sSpNpcCopperProfile),
    HOST_OVERLAY_VARS_END
};
