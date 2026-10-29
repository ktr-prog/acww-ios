// mwcc-flags: -str reuse
#include "types.h"
#include "gfx/VecFx32.h"
#include "game/TouchPicker.h"
#include "npc/NpcAnimCtrl.h"
#include "npc/NpcSpeechState.h"
#include "npc/NpcResHandle.h"
#include "npc/NpcObstacleProbe.h"
#include "npc/Unk_0201ad18.h"
#include "npc/NpcFootstepFx.h"
#include "sys/ProcBase.h"
#include "snd/SndSeEmitterKind1.h"
#include "npc/NpcTalkCtrl.h"
#include "talk/MsgString9B.h"
#include "actor/Actor.h"
#include "actor/Character.h"
#include "gfx/ThreeLayerAnimModel.h"
#include "talk/TalkMsgRequest.h"
#include "talk/ActorTalkRequest.h"
#include "actor/NpcActor.h"
#include "actor/SpNpcActor.h"
#include "talk/ActorTalkRequest.h"
#include "talk/SpNpcTalkRequest.h"
#include "actor/ActorProfile.h"
#include "net/CommManager.h"
#include "talk/TalkStartMsg.h"
#include "item/PocketMatches.h"
#include "npc/NpcActionCtrl.h"
#include "npc/NpcMoveCtrl.h"
#include "npc/VillagerId.h"
#include "player/PlayerData.h"
#include "player/PlayerSpNpcRecord.h"
#include "room/HouseData.h"
#include "save/SaveData.h"
#include "snd/BgmManager.h"
#include "talk/ChoiceList.h"
#include "talk/TalkWindowState.h"
#include "town/ShopClosingTime.h"
#include "game/TurnipMarket.h"
#include "player/PlayerErrands.h"
#include "npc/SickVillagerRecord.h"
#include "town/BlockMap.h"
#include "room/FtrActorGrid.h"
#include "proto/main/unk_020119cc.h"
#include "proto/main/unk_02034048.h"
#include "proto/main/unk_0206269c.h"
#include "proto/main/unk_02063904.h"
#include "proto/main/unk_0206e54c.h"
#include "proto/main/unk_020850e0.h"
#include "proto/main/unk_020943dc.h"
#include "proto/main/unk_02094810.h"
#include "proto/main/unk_02097444.h"
#include "proto/main/unk_02098e90.h"
#include "proto/main/unk_0209c37c.h"
#include "proto/main/unk_020a427c.h"
#include "proto/main/unk_020b5e5c.h"
#include "proto/main/unk_020b7d10.h"
#include "proto/ov004/unk_ov004_02233074.h"
#include "proto/ov004/unk_ov004_0223e9bc.h"
#include "proto/ov050/unk_ov050_02258de0.h"
#include "proto/main/unk_020b4828.h"
#include "proto/main/unk_0204cc1c.h"


class ActorTalkRequest;
class NpcActor;
class SpNpcNookShop;
class SpNpcNookShopTalk;


struct SpNpcNookShopVecLocal : VecFx32 {
    SpNpcNookShopVecLocal() {}
};

struct SpNpcNookShopDramaChoiceLocal {
    u8 a;
    u8 b;
    u8 pad[6];
};



struct SpNpcNookShopStartLocal {
    u8 lo : 2;
    u8 b : 3;
    u8 c : 3;
    u8 roofColor;
};

struct ClockDateBytes {
    u8 b[4];
};



struct Unk_ov050_0225c0a0_Msg {
    u8 msgIndex;
    u8 pad_01;
    u16 item;
    u16 cmpItem;
};




class ActorTalkRequest;
class NpcActor;
class SpNpcMabel;
class SpNpcMabelTalk;










class SpNpcNookShopTalk : public SpNpcTalkRequest {
public:
    typedef void (SpNpcNookShopTalk::*Fn)();
    typedef void (SpNpcNookShopTalk::*ArgFn)(s32);
    typedef void (SpNpcNookShopTalk::*PtrFn)(PlayerErrands *);

    SpNpcNookShopTalk();
    virtual ~SpNpcNookShopTalk();
    virtual void resetMsg();
    virtual void onMessageEnd(u32 attr);
    virtual void onChoice(u32 attr);
    virtual void start(TalkStartMsg *out);
    virtual void onTaskDone(u32 id);
    virtual void pickArbeitStartMsg0B(TalkStartMsg *out);
    virtual void pickArbeitStartMsg0C(TalkStartMsg *out);
    virtual void pickArbeitStartMsg0D(TalkStartMsg *out);
    virtual void pickArbeitStartMsg0E(TalkStartMsg *out);
    virtual void pickArbeitStartMsg0F(TalkStartMsg *out);
    virtual void pickArbeitStartMsg10(TalkStartMsg *out);
    virtual void pickArbeitStartMsg11(TalkStartMsg *out);
    virtual void pickArbeitStartMsg12(TalkStartMsg *out);
    virtual void onArbeitMessageEnd();

    void buySelectedItem();
    void onArbeitStationeryChoice(s32 p);
    void onArbeitChoice();
    void onDrama2Choice();
    void onRoofColorChoice3(s32 p);
    void onRoofColorChoice2(s32 p);
    void onRoofColorChoice1(s32 p);
    void onRoofColorChoice0(s32 p);
    void onSellConfirmChoice(s32 p);
    void onSafeOfferChoice(s32 p);
    void onBuyPaintChoice(s32 p);
    void onBuyOrShowChoice(s32 p);
    void onBuyAfterPreviewChoice(s32 p);
    void onBuyChoice(s32 p);
    void onCatalogOrderChoice(s32 p);
    void onCannotBuyChoice(s32 p);
    void onDeliveryChoice(s32 p);
    void onMainMenuChoice(s32 p);
    void onShopChoice();
    void setRoofColor(s32 row, s32 col);
    void ackHouseUpgrade();
    void showFirstPurchaseHint();
    void onPurchaseDone();
    void openCatalogMenu();
    void giveMoneyBag();
    void openSellMenu();
    void onSafeOfferDeclined();
    void giveGlamourShot();
    void arbeitPushPlayerBack(PlayerErrands *h);
    void arbeitFinish(PlayerErrands *h);
    void arbeitReduceLoan(PlayerErrands *h);
    void onArbeitMsgEnd31(PlayerErrands *h);
    void arbeitPresentWateringCan(PlayerErrands *h);
    void arbeitStartWateringCanDelivery(PlayerErrands *h);
    void arbeitGiveCarpet(PlayerErrands *h);
    void arbeitStartCarpetDelivery(PlayerErrands *h);
    void arbeitRetryLetter(PlayerErrands *h);
    void arbeitGiveNewStationery(PlayerErrands *h);
    void arbeitOfferStationery(PlayerErrands *h);
    void arbeitGiveStationery(PlayerErrands *h);
    void arbeitStartLetterTask(PlayerErrands *h);
    void arbeitGiveDeliveryParcel(PlayerErrands *h);
    void arbeitStartFurnitureDelivery(PlayerErrands *h);
    void onArbeitMsgEnd12(PlayerErrands *h);
    void onArbeitMsgEnd0E(PlayerErrands *h);
    void arbeitGivePlants(PlayerErrands *h);
    void arbeitCheckPocketSpace(PlayerErrands *h);
    void arbeitPresentUniform(PlayerErrands *h);
    void arbeitGiveUniform(PlayerErrands *h);
    BOOL checkNotInUniform(TalkStartMsg *out);
    void givePlantingItems();
    s32 getTopic();
    void setTopic(s32 v);
    void attachOwner(SpNpcNookShop *owner);
    void handleDeliveryMenu();
    void handleCatalogMenu();
    void handleSellMenu();
    void setPendingMenuHandler(s32 idx);

    /* 0xac */ s32 topic;
    /* 0xb0 */ SpNpcNookShop *ownerNpc;
    /* 0xb4 */ Fn pendingMenuHandler;
    /* 0xbc */ s32 soldCount;
    /* 0xc0 */ s32 price;
    /* 0xc4 */ s32 unk_c4;
    /* 0xc8 */ s32 safeSlot;
    /* 0xcc */ s32 nextMsg;
};

class SpNpcNookShop : public SpNpcActor {
public:
    typedef BOOL (SpNpcNookShop::*Fn)();

    SpNpcNookShop() : talk(), selectedItem(0xfff1), selectedItemX(0), selectedItemZ(0) {}
    virtual BOOL onCreate();
    virtual BOOL preCreate();
    virtual BOOL acceptsInteraction(void *other);
    virtual void onInteractionEvent(u32 cmd, u8 arg);
    virtual BOOL acceptsSelfRequestedInteraction(void *a);
    virtual BOOL updateAct();
    virtual u8 *getTexturePath();
    virtual u8 *getModelPath();
    virtual BOOL canPlayTalkMelody();
    virtual void onTalkMelodyPlayed();

    s32 getRunDistance();
    s32 getFollowDistance();
    BOOL isTwin();
    BOOL isTommy();
    BOOL isTimmy();
    BOOL isNook();
    BOOL tryClosingTimeTalk();
    BOOL tryFarewellTalk();
    BOOL tryStairsBlockTalk();
    BOOL tryItemTalk();
    BOOL pickItemTopic();
    s32 getOtherTwinProfile();
    BOOL isPlayerCloserThanOtherTwin();
    BOOL mainAct10();
    BOOL setupAct10();
    BOOL mainAct0F();
    BOOL setupAct0F();
    BOOL mainAct0E();
    BOOL setupAct0E();
    BOOL mainAct0D();
    BOOL setupAct0D();
    BOOL mainAct09();
    BOOL setupAct09();
    BOOL mainAct08();
    BOOL setupAct08();
    BOOL mainAct12();
    BOOL setupAct12();
    BOOL mainAct11();
    BOOL setupAct11();
    BOOL mainAct07();
    BOOL setupAct07();
    BOOL mainAct0B();
    BOOL setupAct0B();
    BOOL mainAct0C();
    BOOL setupAct0C();
    BOOL mainAct0A();
    BOOL setupAct0A();
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
    /* 0x658 */ SpNpcNookShopTalk talk;
    /* 0x728 */ u8 dramaPending;
    /* 0x72a */ u16 dramaTimer;
    /* 0x72c */ u16 standBlend;
    /* 0x72e */ u16 selectedItem;
    /* 0x730 */ s32 selectedItemX;
    /* 0x734 */ s32 selectedItemZ;
    /* 0x738 */ u8 purchasePending;
    /* 0x739 */ u8 approachTimer;
    /* 0x73a */ u8 arbeitPrompted;
    /* 0x73b */ u8 nookTalkMelodyPlayed;
};

struct Unk_ov050_0225d1d4_Ent {
    SpNpcNookShop::Fn enter;
    SpNpcNookShop::Fn exit;
};

typedef SpNpcNookShopTalk::ArgFn Unk_ov050_02259838_Fn;
typedef SpNpcNookShopTalk::ArgFn Unk_ov050_0225e4b4_ArgFn;
typedef SpNpcNookShopTalk::Fn Unk_ov050_0225e4b4_Fn;

struct SpNpcNookShopChoiceHandler {
    u32 id;
    SpNpcNookShopTalk::ArgFn fn;
};

struct Unk_ov050_0225a6e0_Row {
    u32 id;
    SpNpcNookShopTalk::Fn f;
};

struct SpNpcNookShopArbeitMessageEndHandler {
    u32 id;
    SpNpcNookShopTalk::PtrFn f;
};

extern "C" {
extern CommManager *gCommManager;
extern u8 gFieldSceneKind;
extern s16 data_02135f44[];
extern BgmManager *data_021c1b3c;
extern u8 gTalkMsgIndexEnd[];
extern u16 gPad[];
extern u16 data_020c6cc8;
extern s16 data_020c6cc0;
extern BlockMap *gSceneBlockMap;
extern const VecFx32 sSpNpcNookShopArbeitStairsBound;
extern const VecFx32 sSpNpcNookShopArbeitPushBackPos;
extern const s32 data_ov050_0225da40[3];
extern const u8 sSpNpcNookShopRoofColorChoices[16];
extern const u8 sSpNpcNookShopDramaMsgTable[32];
extern const s32 sSpNpcNookShopItemTopicTable[6][2];
extern const TalkStartMsg sSpNpcNookShopTopicTable[31];
extern Unk_ov050_0225d1d4_Ent sSpNpcNookShopActTable[19];

fx32 VEC_Mag(const VecFx32 *v);
fx32 func_01ffcb0c(fx32 a, fx32 b);
void NpcActor_FindFreeUnitNear(VecFx32 *out, NpcActor *self, VecFx32 *v);
BOOL Talk_IsInOwnTown();
void Talk_AdvanceDrama(void *a, void *b);
BOOL Talk_IsDramaPending(void *a, void *b, u32 c);
BOOL Talk_CheckAndSetPlayerFlag(u32 a, BOOL b);
BOOL Ground_IsOnLockedExit(VecFx32 *p);
BOOL Ground_LockExit(s32 a);
s32 Camera_RestorePrevMode();
BOOL Camera_SetModeDefault();
BOOL Camera_SetMode12();
BOOL Camera_SetMode11();
BOOL TalkRequest_SetTargetDone(NpcActor *self);
BOOL TalkRequest_AddPlayerTalk6(NpcActor *p, s32 v);
BOOL TalkRequest_IsActive();
s32 Item_GetFurnitureIndex(u16 *p);
BOOL Item_IsFurniture(u16 *p);
s32 Item_GetShopPrice(u16 *p);
s32 Item_GetMemberPrice(u16 *p);
s32 Item_GetPrice(u16 *p);
void VillagerTrend_OnFurnitureBought();
BOOL SaveVillagers_AllKnowPlayer(u8 *self, void *p);
s32 Date_DaysBetween(u8 *a, u8 *b);
s32 Clock_GetWeekday();
void Clock_GetDate(u8 *out);
BOOL DateTime_IsInvalid(u8 *p);
BOOL NookShop_SendCatalogOrder(void *p);
BOOL NookShop_CanTakeCatalogOrder();
u32 NookShop_IsPurchaseSynced();
void NookShop_BuyAt(u32 a, u32 b, u32 c, u32 d);
void NookShop_RecordBuyback(s32 a);
u32 NookShop_GetLevel(void *g);
BOOL NookShop_IsClosedTomorrow(void *g);
ClockDateBytes NookShop_GetClosedDate(void *g);  // a 4-byte record by value (the definition's Rgba)
void *NookShop_GetRenovation(void *g);
BOOL NookShop_HasClass4Furniture(void *g);
BOOL NookShop_IsPointSpecialToday(void *g);
BOOL NookShop_IsSaleTime(void *g);
u8 Scene_GetPrevious();
u32 Scene_GetCurrent();
BOOL TouchPick_GetGroundPos(TouchPickResult *a, VecFx32 *b);
u16 Math_CountDownU16(u16 *p);
u8 Math_CountDownU8(u8 *p);
s32 Math_AngleDiffAbs(s32 a, s32 b);
s32 Vec_NotEqual(VecFx32 *a, VecFx32 *b);
s32 Vec_Equal(VecFx32 *a, VecFx32 *b);
void Vec_Sub(VecFx32 *out, VecFx32 *a, VecFx32 *b);
s32 strncmp(const char *a, const char *b, u32 n);
u32 strlen(const char *s);
}

extern SaveData gSaveData;


// The player's birthday (month, day) is set (the game tests both bytes at once, as a halfword)
static inline BOOL Birthday_IsSet(u8 *bd) {
    return bd[0] != 0 || bd[1] != 0;
}

// The shop's closing time in the town session state
static inline ShopClosingTime *ShopClosingTime_Get() {
    return (ShopClosingTime *)TownSessionState_GetClosingTime(TownSessionState_Get());
}

static inline BOOL Unk_ov050_02258ea0_Eq(u16 v, u16 k) {
    if (v == k) {
        return TRUE;
    }
    return FALSE;
}

static inline BOOL Unk_ov050_022590f8_Flags() {
    if (gTouchPrevHeld && gTouchPrevChanged) {
        return TRUE;
    }
    return FALSE;
}

static inline BOOL Unk_ov050_022590f8_Eq(u16 *p, u16 *k) {
    if (Item_IsFurniture(p)) {
        *k = 0xfff1;
        s32 a = Item_GetFurnitureIndex(p);
        if (a == Item_GetFurnitureIndex(k)) return TRUE;
        return FALSE;
    }
    if (*p == 0xfff1) return TRUE;
    return FALSE;
}

// ---------------------------------------------------------------------------------------------------------------------

static inline BOOL Unk_ov050_02259bb8_Match(u16 *p) {
    BOOL r;
    if (Item_IsFurniture(p) != 0) {
        u16 v = 0xfff1;
        s32 a = Item_GetFurnitureIndex(p);
        if (a == Item_GetFurnitureIndex(&v)) {
            r = TRUE;
        } else {
            r = FALSE;
        }
    } else {
        if (*p == 0xfff1) {
            r = TRUE;
        } else {
            r = FALSE;
        }
    }
    return r;
}
static inline BOOL Unk_ov050_0225a2d0_R(u16 *p, u32 lo, u32 hi) {
    BOOL r = FALSE;
    if (*p >= lo && *p <= hi) {
        r = TRUE;
    }
    return r;
}

static inline BOOL Unk_ov050_0225a2d0_Same(u16 *p) {
    u16 t;
    if (Item_IsFurniture(p)) {
        t = 0xfff1;
        s32 a = Item_GetFurnitureIndex(p);
        s32 b = Item_GetFurnitureIndex(&t);
        if (a == b) {
            return TRUE;
        }
        return FALSE;
    }
    if (*p == 0xfff1) {
        return TRUE;
    }
    return FALSE;
}

static inline BOOL Unk_ov050_0225a888_Same(u16 *p, u16 *t) {
    if (Item_IsFurniture(p)) {
        *t = 0xfff1;
        s32 a = Item_GetFurnitureIndex(p);
        s32 b = Item_GetFurnitureIndex(t);
        if (a == b) {
            return TRUE;
        }
        return FALSE;
    }
    if (*p == 0xfff1) {
        return TRUE;
    }
    return FALSE;
}

static inline BOOL Unk_ov050_0225a888_Eq(u32 v, u32 k) {
    return v == k ? TRUE : FALSE;
}

static inline BOOL Unk_ov050_0225b7b4_IsOne(u8 v) {
    if (v == 1) {
        return TRUE;
    }
    return FALSE;
}
static inline BOOL Unk_ov050_0225bd54_Same(u16 *p, u16 *t, u32 k) {
    if (Item_IsFurniture(p)) {
        *t = k;
        s32 a = Item_GetFurnitureIndex(p);
        s32 b = Item_GetFurnitureIndex(t);
        if (a == b) {
            return TRUE;
        }
        return FALSE;
    }
    if (*p == k) {
        return TRUE;
    }
    return FALSE;
}

static inline BOOL Unk_ov050_0225bacc_Chk(u16 *p) {
    BOOL r = FALSE;
    if (Pocket_FindItem(p) >= 0) {
        r = TRUE;
    }
    return r;
}

static inline BOOL Unk_ov050_0225c294_Rng(volatile u16 *p, u32 lo, u32 hi, BOOL r) {
    u32 v = *p;
    if (*p >= lo && v <= hi) r = TRUE;
    return r;
}

extern "C" SpNpcNookShop *SpNpcNookShop_Create() { return new SpNpcNookShop; }

extern "C" SpNpcNookShop *SpNpcNookShop_CreateTimmy() { return new SpNpcNookShop; }

extern "C" SpNpcNookShop *SpNpcNookShop_CreateTommy() { return new SpNpcNookShop; }

BOOL SpNpcNookShop::preCreate() {
    if (!SpNpcActor::preCreate()) {
        return FALSE;
    }
    setTalkRequest((ActorTalkRequest *)&talk);
    talk.attachOwner(this);
    if (isTwin()) {
        setColliderSize(0xccd, 0x2000);
        setCollisionRadius(0xb00);
    }
    standBlend = data_020c6cc8;
    moveCtrl.setSpeedPreset(2, 0x400, 0x133, 0x199);
    if (gCommManager->isOnline() || *DebugVar_GetPtr(0, 0x4a) != 0) {
        switch (param) {
        case 0xd00f:
            position.x = 0xf000;
            position.y = 0;
            position.z = 0x11000;
            break;
        case 0xd010:
            position.x = 0xf000;
            position.y = 0;
            position.z = 0x13000;
            break;
        case 0xd019:
            position.x = 0xf000;
            position.y = 0;
            position.z = 0x19000;
            break;
        case 0xd01a:
            position.x = 0x13000;
            position.y = 0;
            position.z = 0x17000;
            break;
        case 0xd01b:
            position.x = 0xf000;
            position.y = 0;
            position.z = 0x15000;
            break;
        case 0xd01c:
            position.x = 0x11000;
            position.y = 0;
            position.z = 0x13000;
            break;
        }
    }
    return TRUE;
}

BOOL SpNpcNookShop::onCreate() {
    s32 v;
    if (!SpNpcActor::onCreate()) {
        return FALSE;
    }
    if (gCommManager->isOnline() != 0 || *DebugVar_GetPtr(0, 0x4a) != 0) {
        collider.groups |= 2;
        if (NetArea_IsLocalOwner()) {
            if (DateTime_IsInvalid((u8 *)ShopClosingTime_Get()->getTime())) {
                ShopClosingTime_Get()->setClosingTimeToday();
            }
            rotY = 0;
            moveAngleY = 0;
            changeAct(0xd);
        } else {
            changeAct(8);
        }
        return TRUE;
    }
    if (DateTime_IsInvalid((u8 *)ShopClosingTime_Get()->getTime())) {
        ShopClosingTime_Get()->setClosingTimeToday();
    }
    ShopClosingTime *h = ShopClosingTime_Get();
    PlayerData *p = PlayerData_GetCurrent();
    void *q = p->getErrands();
    if (p->testFlag(1)) {
        void *m = PlayerErrandSlot_GetRecord(PlayerErrands_GetSlot((PlayerErrands *)q, 0));
        Ground_LockExit(1);
        talk.setTopic(5);
        h->setClosingTimeToday();
        if (h->isPastClosingTime()) {
            data_021c1b3c->roomBgm.forceClosingMusic();
        }
        if (((ErrandRecord *)m)->isActive() && (((ErrandRecord *)m)->getStep() >= 1 || ((ErrandRecord *)m)->getKind() >= 0xc)) {
            changeAct(1);
        } else {
            changeAct(0);
        }
        return TRUE;
    }
    if (isTommy() && Scene_GetPrevious() == 0x1d) {
        h->setClosingTimeToday();
        changeAct(0);
    } else if (isTimmy() && Scene_GetPrevious() == 0x1d) {
        changeAct(0xa);
    } else if (isNook() && Scene_GetPrevious() == 0) {
        h->setClosingTimeToday();
        changeAct(0);
    } else {
        if (Scene_GetPrevious() != 0x1e) {
            h->setClosingTimeToday();
        }
        changeAct(1);
    }
    Ground_LockExit(0);
    if (isNook()) {
        if (h->isPastClosingTime()) {
            data_021c1b3c->roomBgm.forceClosingMusic();
        }
        if (Talk_IsDramaPending(this, &v, 3)) {
            dramaPending = 1;
        }
    }
    return TRUE;
}

u8 *SpNpcNookShop::getTexturePath() {
    s32 r = 0;
    if (isTwin()) {
        r = 4;
    } else {
        switch (param) {
        case 0xd019:
            r = 0;
            break;
        case 0xd01a:
            r = 1;
            break;
        case 0xd01b:
            r = 2;
            break;
        case 0xd01c:
            r = 3;
            break;
        }
    }
    return (u8 *)sSpNpcNookShopTexturePaths[r];
}

u8 *SpNpcNookShop::getModelPath() {
    s32 r = 0;
    if (isTwin()) {
        r = 4;
    } else {
        switch (param) {
        case 0xd019:
            r = 0;
            break;
        case 0xd01a:
            r = 1;
            break;
        case 0xd01b:
            r = 2;
            break;
        case 0xd01c:
            r = 3;
            break;
        }
    }
    return (u8 *)sSpNpcNookShopModelPaths[r];
}

BOOL SpNpcNookShop::updateAct() {
    BOOL r = FALSE;
    if (((Unk_ov050_0225d1d4_Ent *)&sSpNpcNookShopActTable[0].exit)[act].enter) {
        r = (this->*sSpNpcNookShopActTable[act].exit)();
    }
    return r;
}

void SpNpcNookShop::changeAct(s32 state) {
    BOOL ok = TRUE;
    if (sSpNpcNookShopActTable[state].enter) {
        ok = (this->*sSpNpcNookShopActTable[state].enter)();
    }
    if (ok) {
        act = state;
    }
}

BOOL SpNpcNookShop::setupAct00() {
    if (isNook()) {
        talk.setTopic(0);
    } else if (isTwin()) {
        talk.setTopic(0x11);
    }
    return TRUE;
}

BOOL SpNpcNookShop::mainAct00() {
    TalkRequest_AddPlayerTalk6(this, 0);
    return TRUE;
}

BOOL SpNpcNookShop::setupAct01() {
    s32 v;
    actionCtrl.requestStand(1, standBlend);
    dramaTimer = Random_GlobalBelow(5) * 20 + 100;
    standBlend = data_020c6cc8;
    if (isNook() && Talk_IsDramaPending(this, &v, 3)) {
        dramaPending = 1;
    } else {
        dramaPending = 0;
    }
    return TRUE;
}

BOOL SpNpcNookShop::mainAct01() {
    VecFx32 *pv = (VecFx32 *)PlayerActor_GetBodyPos(4);
    VecFx32 v;
    v.x = pv->x;
    v.y = pv->y;
    v.z = pv->z;
    s32 t = getDistanceToPlayer(4);
    s32 a = getAngleToPlayer(4);
    s32 k = Math_AngleDiffAbs(rotY, a);
    s32 s = getFollowDistance();
    VecFx32 out;
    NpcActor_FindFreeUnitNear(&out, this, &v);
    if (PlayerActor_IsInAction(0x8b, 4) || PlayerActor_IsInAction(0x8c, 4)) {
        return TRUE;
    }
    if (t > s && Vec_NotEqual(&out, &position)) {
        changeAct(3);
    } else if (k > 0x2000) {
        changeAct(2);
    }
    if (tryStairsBlockTalk()) {
        return TRUE;
    }
    if (tryFarewellTalk()) {
        return TRUE;
    }
    if (tryClosingTimeTalk()) {
        return TRUE;
    }
    if (tryItemTalk()) {
        return TRUE;
    }
    if (dramaPending && Math_CountDownU16(&dramaTimer) == 0) {
        changeAct(7);
    }
    return TRUE;
}

BOOL SpNpcNookShop::setupAct02() {
    actionCtrl.requestAction(3, 1, 0, 0, 0, 0, 0, 0, data_020c6cc8, 0);
    return TRUE;
}

BOOL SpNpcNookShop::mainAct02() {
    if (tryStairsBlockTalk()) {
        return TRUE;
    }
    if (tryFarewellTalk()) {
        return TRUE;
    }
    if (tryClosingTimeTalk()) {
        return TRUE;
    }
    if (tryItemTalk()) {
        return TRUE;
    }
    VecFx32 *pv = (VecFx32 *)PlayerActor_GetBodyPos(4);
    VecFx32 v;
    v.x = pv->x;
    v.y = pv->y;
    v.z = pv->z;
    s32 t = getDistanceToPlayer(4);
    s32 b = getAngleToPlayer(4);
    Math_AngleDiffAbs(rotY, b);
    s32 s = getFollowDistance();
    VecFx32 out;
    NpcActor_FindFreeUnitNear(&out, this, &v);
    if (PlayerActor_IsInAction(0x8b, 4) || PlayerActor_IsInAction(0x8c, 4)) {
        return TRUE;
    }
    if (t > s) {
        if (Vec_NotEqual(&out, &position)) {
            changeAct(3);
            return TRUE;
        }
    }
    moveCtrl.setTargetAngle(b);
    if (actionCtrl.getAction() == 3) {
        if (actionCtrl.isActionDone()) {
            changeAct(1);
        }
    }
    return TRUE;
}

BOOL SpNpcNookShop::setupAct03() {
    actionCtrl.requestAction(1, 1, 0, 0, 0, 0, 0, 0, data_020c6cc8, 0);
    return TRUE;
}

BOOL SpNpcNookShop::mainAct03() {
    if (PlayerActor_IsInAction(0x8b, 4) || PlayerActor_IsInAction(0x8c, 4)) {
        changeAct(1);
        return TRUE;
    }
    if (tryStairsBlockTalk()) {
        return TRUE;
    }
    if (tryFarewellTalk()) {
        return TRUE;
    }
    if (tryClosingTimeTalk()) {
        return TRUE;
    }
    if (tryItemTalk()) {
        return TRUE;
    }
    VecFx32 *pv = (VecFx32 *)PlayerActor_GetBodyPos(4);
    VecFx32 v;
    v.x = pv->x;
    v.y = pv->y;
    v.z = pv->z;
    s32 t = getDistanceToPlayer(4);
    VecFx32 out;
    NpcActor_FindFreeUnitNear(&out, this, &v);
    s32 s = getFollowDistance();
    if (t > getRunDistance()) {
        if (actionCtrl.getAction() == 1) {
            actionCtrl.requestAction(2, 1, 0, 0, 0x800, 0, 0, 0, data_020c6cc8, 0);
        }
    } else {
        if (actionCtrl.getAction() == 2) {
            actionCtrl.requestAction(1, 1, 0, 0, 0, 0, 0, 0, data_020c6cc8, 0);
        }
    }
    moveCtrl.setWaypoint(&out);
    if (t <= s || Vec_Equal(&out, &position) != 0) {
        changeAct(1);
    }
    return TRUE;
}

BOOL SpNpcNookShop::setupAct04() {
    s32 v;
    NpcActor *p = (NpcActor *)talk.getTalkPlayer();
    s32 r = 0;
    if (p) {
        r = getAngleTo(p);
    }
    talkCtrl.requestTurnAndTalk(0, r, 0);
    PlayerData *h = PlayerData_GetCurrent();
    h->getErrands();
    if (h->testFlag(1)) {
        return TRUE;
    }
    if (isNook() && Talk_IsDramaPending(this, &v, 3)) {
        dramaPending = 1;
    }
    return TRUE;
}

BOOL SpNpcNookShop::mainAct04() {
    if (talkCtrl.isBusy()) {
        return TRUE;
    }
    if (purchasePending && NookShop_IsPurchaseSynced() == 0) {
        return TRUE;
    }
    if (TownSessionState_TestFlag(TownSessionState_Get(), 5)) {
        SceneWarp_RequestExit(Scene_GetWarpRequest(), 0);
    } else {
        TalkRequest_SetTargetDone(this);
    }
    changeAct(6);
    return TRUE;
}

BOOL SpNpcNookShop::setupAct05() {
    NpcActor *p = (NpcActor *)talk.getTalkPlayer();
    s32 r = 0;
    if (p) {
        r = getAngleTo(p);
    }
    talkCtrl.requestTurnAndTalk(0, r, 1);
    if (isNook()) {
        ShopClosingTime_Get()->clearClosingTime();
    }
    return TRUE;
}

BOOL SpNpcNookShop::mainAct05() {
    if (talkCtrl.isBusy() == 0) {
        ShopClosingTime *h = ShopClosingTime_Get();
        if (isTwin() && h->isPastClosingTime()) {
            SceneWarp_RequestExit(Scene_GetWarpRequest(), 1);
        } else {
            SceneWarp_RequestExit(Scene_GetWarpRequest(), 0);
        }
        changeAct(6);
    }
    return TRUE;
}

BOOL SpNpcNookShop::setupAct06() { return TRUE; }

BOOL SpNpcNookShop::mainAct06() { return TRUE; }

BOOL SpNpcNookShop::setupAct0A() { return TRUE; }

BOOL SpNpcNookShop::mainAct0A() {
    SpNpcNookShop *o = (SpNpcNookShop *)Actor::findByProfile(getOtherTwinProfile(), NULL);
    if (o->act == 1) {
        changeAct(1);
    }
    return TRUE;
}

BOOL SpNpcNookShop::setupAct0C() {
    standBlend = data_020c6cc8;
    actionCtrl.requestStand(1, standBlend);
    return TRUE;
}

// ---------------------------------------------------------------------------------------------------------------------

BOOL SpNpcNookShop::mainAct0C() { return TRUE; }

BOOL SpNpcNookShop::setupAct0B() {
    actionCtrl.requestAction(3, 1, 0, 0, 0, 0, 0, 0, data_020c6cc8, 0);
    return TRUE;
}

BOOL SpNpcNookShop::mainAct0B() {
    moveCtrl.setTargetAngle(getAngleToPlayer(4));
    return TRUE;
}

BOOL SpNpcNookShop::setupAct07() {
    actionCtrl.requestAction(0xa, 1, 0, 0, 0, 0, 0, 0, data_020c6cc8, 0);
    return TRUE;
}

BOOL SpNpcNookShop::mainAct07() {
    if (tryStairsBlockTalk()) {
        return TRUE;
    }
    if (tryFarewellTalk()) {
        return TRUE;
    }
    if (tryClosingTimeTalk()) {
        return TRUE;
    }
    if (tryItemTalk()) {
        return TRUE;
    }
    if (actionCtrl.getAction() == 0xa) {
        if (actionCtrl.isActionDone()) {
            standBlend = 0x18;
            changeAct(1);
        }
    }
    return TRUE;
}

BOOL SpNpcNookShop::setupAct11() {
    actCounter = 0;
    return TRUE;
}

BOOL SpNpcNookShop::mainAct11() {
    PlayerData_GetCurrent();
    VecFx32 *pv = (VecFx32 *)PlayerActor_GetBodyPos(4);
    VecFx32 v0;
    v0.x = pv->x;
    v0.y = pv->y;
    v0.z = pv->z;
    s32 a0 = 0, a1 = 0, a2 = 0, a3 = 0;
    VecFx32 v1;
    v1 = sSpNpcNookShopArbeitPushBackPos;
    s32 gx = v1.x;
    s32 gy = v1.y;
    s32 gz = v1.z;
    FieldPos_ToUnit(&a2, &a3, &v1);
    FieldPos_ToUnit(&a0, &a1, &v0);
    switch (actCounter) {
    case 0:
        if (talkCtrl.isBusy() == 0) {
            Camera_SetModeDefault();
            actCounter = 1;
        }
        break;
    case 1: {
        VecFx32Ctor v2;
        v2.x = gx;
        v2.y = gy;
        v2.z = gz;
        PlayerActor_RequestWalkTo(&v2, 0x266, 4);
        actCounter = 2;
        break;
    }
    case 2:
        if (PlayerActor_IsScriptedWalking(4) == 0) {
            TalkRequest_SetTargetDone(this);
            changeAct(2);
        }
        break;
    }
    return TRUE;
}

BOOL SpNpcNookShop::setupAct12() {
    actionCtrl.requestAction(3, 1, 0, 0, 0, 0, 0, 0, data_020c6cc8, 0);
    return TRUE;
}

BOOL SpNpcNookShop::mainAct12() {
    if (actionCtrl.getAction() == 3) {
        if (actionCtrl.isActionDone()) {
            actionCtrl.requestStand(1, standBlend);
        }
    }
    moveCtrl.setTargetAngle(getAngleToPlayer(4));
    TalkRequest_AddPlayerTalk6(this, 0);
    return TRUE;
}

BOOL SpNpcNookShop::setupAct08() { return TRUE; }

BOOL SpNpcNookShop::mainAct08() {
    if (isNetOwner()) {
        s32 a = 4;
        s32 b = 4;
        u32 x, t;
        if (netGetSlots(&a, &b) && ((x = a), x == (t = gCommManager->myAid)) && x == b) {
            netSetSlotsIfOwner(1, t, t);
            talk.resetMsg();
            talk.setTalkPlayer(getPlayerActor(4));
            BOOL r;
            if (Item_IsFurniture(&selectedItem)) {
                u16 tmp = 0xfff1;
                s32 p = Item_GetFurnitureIndex(&selectedItem);
                if (p == Item_GetFurnitureIndex(&tmp)) {
                    r = TRUE;
                } else {
                    r = FALSE;
                }
            } else {
                if (selectedItem == 0xfff1) {
                    r = TRUE;
                } else {
                    r = FALSE;
                }
            }
            if (r) {
                if (isNook()) {
                    talk.setTopic(5);
                } else {
                    talk.setTopic(0x13);
                }
            }
            changeAct(4);
        } else if (NetArea_IsLocalOwner() && b == 4) {
            netSetSlotsIfOwner(1, gCommManager->myAid, 4);
            changeAct(0xd);
        }
    } else {
        tryItemTalk();
    }
    return TRUE;
}

BOOL SpNpcNookShop::setupAct09() { return TRUE; }

BOOL SpNpcNookShop::mainAct09() {
    if (isNetOwner()) {
        s32 a = 4;
        s32 b = 4;
        if (netGetSlots(&a, &b) && a == 4 && NetArea_IsLocalOwner()) {
            netSetSlotsIfOwner(1, gCommManager->myAid, 4);
            changeAct(0xd);
        }
    }
    return TRUE;
}

BOOL SpNpcNookShop::setupAct0D() {
    actionCtrl.requestStand(1, standBlend);
    return TRUE;
}

BOOL SpNpcNookShop::mainAct0D() {
    s32 t = getAngleToPlayer(4);
    if (Math_AngleDiffAbs(rotY, t) >= data_020c6cc0) {
        changeAct(0xe);
    } else {
        tryItemTalk();
    }
    return TRUE;
}

BOOL SpNpcNookShop::setupAct0E() {
    s32 f = getAngleToPlayer(4);
    actionCtrl.requestAction(3, 1, 0, 0, 0, f, 0, 0, data_020c6cc8, 0);
    return TRUE;
}

BOOL SpNpcNookShop::mainAct0E() {
    if (actionCtrl.getAction() == 3) {
        if (actionCtrl.isActionDone()) {
            changeAct(0xd);
        } else {
            tryItemTalk();
        }
    }
    return TRUE;
}

BOOL SpNpcNookShop::setupAct0F() { return TRUE; }

BOOL SpNpcNookShop::mainAct0F() { return TRUE; }

BOOL SpNpcNookShop::setupAct10() {
    approachTimer = 0x32;
    actionCtrl.requestAction(1, 1, 0, 0, 0, 0, 0, 0, data_020c6cc8, 0);
    return TRUE;
}

// ---------------------------------------------------------------------------------------------------------------------

BOOL SpNpcNookShop::mainAct10() {
    VecFx32 *pv = (VecFx32 *)PlayerActor_GetBodyPos(4);
    VecFx32 v;
    v.x = pv->x;
    v.y = pv->y;
    v.z = pv->z;
    VecFx32 out;
    NpcActor_FindFreeUnitNear(&out, this, &v);
    s32 t = getDistanceToPlayer(4);
    ((BlockMap *)gSceneBlockMap)->getWalkLinksAtPos(&position);
    if (t > 0x4000) {
        if (actionCtrl.getAction() == 1) {
            actionCtrl.requestAction(2, 1, 0, 0, 0x800, 0, 0, 0, data_020c6cc8, 0);
        }
    } else {
        if (actionCtrl.getAction() == 2) {
            actionCtrl.requestAction(1, 1, 0, 0, 0, 0, 0, 0, data_020c6cc8, 0);
        }
    }
    moveCtrl.setWaypoint(&out);
    if (t <= 0x3000 || Vec_Equal(&out, &position) != 0 || Math_CountDownU8(&approachTimer) == 0) {
        talk.resetMsg();
        talk.setTalkPlayer(getPlayerActor(4));
        changeAct(4);
    }
    return TRUE;
}

void SpNpcNookShopTalk::onTaskDone(u32) {
    if (pendingMenuHandler) {
        (this->*pendingMenuHandler)();
        pendingMenuHandler = NULL;
    }
}// Declarations for data defined further down (definition order sets the data layout)
extern "C" const VecFx32 sSpNpcNookShopArbeitStairsBound;
extern "C" const VecFx32 sSpNpcNookShopArbeitPushBackPos;
extern "C" const s32 data_ov050_0225da40[3];
extern "C" const s32 sSpNpcNookShopItemTopicTable[6][2];
extern "C" char data_ov050_0225e210[];
extern "C" char data_ov050_0225e228[];
extern "C" char data_ov050_0225e240[];
extern "C" char data_ov050_0225e258[];
extern "C" char data_ov050_0225e270[];
extern "C" char sSpNpcNookShopSequence5_1Key[];
extern "C" char data_ov050_0225e2d0[];
extern "C" char data_ov050_0225e2ec[];
extern "C" char data_ov050_0225e308[];
extern "C" char data_ov050_0225e324[];
extern "C" char data_ov050_0225e340[];
extern "C" char sSpNpcNookShopTwinsKey[];
extern "C" char sSpNpcNookShopDramaKey[];
extern "C" const TalkStartMsg sSpNpcNookShopTopicTable[31];
extern "C" const u8 sSpNpcNookShopRoofColorChoices[16];
extern "C" const u8 sSpNpcNookShopDramaMsgTable[32];
extern "C" char sSpNpcNookShopKey[];
extern "C" char *sSpNpcNookShopTexturePaths[5];
extern "C" char *sSpNpcNookShopModelPaths[5];
extern "C" ActorProfile sSpNpcNookShopProfile;
extern "C" ActorProfile sSpNpcTimmyProfile;
extern "C" ActorProfile sSpNpcTommyProfile;
extern "C" Unk_ov050_0225d1d4_Ent sSpNpcNookShopActTable[19];// Declarations for data defined further down (definition order sets the data layout)
extern "C" char sSpNpcNookShopKey[];
extern "C" char *sSpNpcNookShopTexturePaths[5];
extern "C" char *sSpNpcNookShopModelPaths[5];
extern "C" ActorProfile sSpNpcNookShopProfile;
extern "C" ActorProfile sSpNpcTimmyProfile;
extern "C" ActorProfile sSpNpcTommyProfile;
extern "C" Unk_ov050_0225d1d4_Ent sSpNpcNookShopActTable[19];
extern "C" const VecFx32 sSpNpcNookShopArbeitStairsBound;
extern "C" const VecFx32 sSpNpcNookShopArbeitPushBackPos;
extern "C" const s32 data_ov050_0225da40[3];
extern "C" char data_ov050_0225e2d0[];
extern "C" char data_ov050_0225e2ec[];
extern "C" char data_ov050_0225e308[];
extern "C" char data_ov050_0225e324[];
extern "C" char data_ov050_0225e340[];
extern "C" const u8 sSpNpcNookShopDramaMsgTable[32];
extern "C" const TalkStartMsg sSpNpcNookShopTopicTable[31];
extern "C" char sSpNpcNookShopSequence5_1Key[];
extern "C" const u8 sSpNpcNookShopRoofColorChoices[16];
extern "C" char sSpNpcNookShopTwinsKey[];
extern "C" char sSpNpcNookShopDramaKey[];
extern "C" char data_ov050_0225e210[];
extern "C" char data_ov050_0225e228[];
extern "C" char data_ov050_0225e240[];
extern "C" char data_ov050_0225e258[];
extern "C" char data_ov050_0225e270[];
extern "C" const s32 sSpNpcNookShopItemTopicTable[6][2];
// Declarations for data defined further down (definition order sets the data layout)
extern "C" char sSpNpcNookShopDramaKey[];
extern "C" const u8 sSpNpcNookShopRoofColorChoices[16];
extern "C" char *sSpNpcNookShopTexturePaths[5];
extern "C" char *sSpNpcNookShopModelPaths[5];
extern "C" char sSpNpcNookShopKey[];
extern "C" const TalkStartMsg sSpNpcNookShopTopicTable[31];
extern "C" const VecFx32 sSpNpcNookShopArbeitStairsBound;
extern "C" const VecFx32 sSpNpcNookShopArbeitPushBackPos;
extern "C" const s32 data_ov050_0225da40[3];
extern "C" const u8 sSpNpcNookShopDramaMsgTable[32];
extern "C" char data_ov050_0225e2d0[];
extern "C" char data_ov050_0225e2ec[];
extern "C" char data_ov050_0225e308[];
extern "C" char data_ov050_0225e324[];
extern "C" char data_ov050_0225e340[];
extern "C" const s32 sSpNpcNookShopItemTopicTable[6][2];
extern "C" Unk_ov050_0225d1d4_Ent sSpNpcNookShopActTable[19];
extern "C" char sSpNpcNookShopTwinsKey[];
extern "C" ActorProfile sSpNpcNookShopProfile;
extern "C" ActorProfile sSpNpcTimmyProfile;
extern "C" ActorProfile sSpNpcTommyProfile;
extern "C" char sSpNpcNookShopSequence5_1Key[];
extern "C" char data_ov050_0225e210[];
extern "C" char data_ov050_0225e240[];
extern "C" char data_ov050_0225e258[];
extern "C" char data_ov050_0225e270[];
extern "C" char data_ov050_0225e228[];

extern "C" char sSpNpcNookShopDramaKey[] = "sp_npc_drama2";

extern "C" const u8 sSpNpcNookShopRoofColorChoices[16] = {0x02, 0x0b, 0x05, 0x07, 0x03, 0x04, 0x0a, 0x0d, 0x06, 0x0e, 0x09, 0x0f, 0x00, 0x01, 0x08, 0x0c};

extern "C" char *sSpNpcNookShopTexturePaths[5] = {data_ov050_0225e2d0, data_ov050_0225e2ec, data_ov050_0225e308, data_ov050_0225e324, data_ov050_0225e340};

extern "C" char *sSpNpcNookShopModelPaths[5] = {data_ov050_0225e210, data_ov050_0225e228, data_ov050_0225e240, data_ov050_0225e258, data_ov050_0225e270};

// ---------------------------------------------------------------------------------------------------------------------
// Menu class

void SpNpcNookShopTalk::setPendingMenuHandler(s32 idx) {
    static Fn tbl[3] = {
        &SpNpcNookShopTalk::handleSellMenu,
        &SpNpcNookShopTalk::handleCatalogMenu,
        &SpNpcNookShopTalk::handleDeliveryMenu,
    };
    pendingMenuHandler = tbl[idx];
}

void SpNpcNookShopTalk::handleSellMenu() {
    void *owner = window;
    struct {
        u8 cmd;
        u16 item;
        u16 pick;
    } l;
    l.cmd = 8;
    price = 0;
    soldCount = 0;
    if (MenuCtrl_IsResultOk()) {
        u16 *list = MenuCtrl_GetChosenItems();
        BOOL b1 = FALSE;
        BOOL b2 = FALSE;
        BOOL b3 = FALSE;
        l.item = 0xfff1;
        s32 i = b1;
        s32 neg = ~i;
        BOOL za = i;
        BOOL zb = i;
        goto test0;
    loop0:
        {
            u32 cur = list[i];
            if (cur == 0xfff1) goto done0;
            l.item = cur;
            BOOL r = za;
            u32 v = *(volatile u16 *)&l.item;
            if (l.item >= 0x136a && v <= 0x136a) r = TRUE;
            if (r || (v >= 0x1373 && v <= 0x1373) || (v >= 0x1375 && v <= 0x1375) || (v >= 0x1377 && v <= 0x1377) ||
                (v >= 0x1379 && v <= 0x1379) || (v >= 0x137b && v <= 0x137b)) {
                b1 = TRUE;
            }
            if (v >= 0x38e4 && v <= 0x3933) {
                b3 = TRUE;
                s32 idx;
                if (v >= 0x38e4 && v <= 0x3933) {
                    idx = (s32)(v - 0x38e4) >> 2;
                } else {
                    idx = neg;
                }
                l.pick = ((u32)idx < 0x14) ? 0x3934 + idx * 4 : 0x3934;
                price += Item_GetPrice(&l.pick) / 4;
            } else if (PlayerData_GetCurrent()) {
                BOOL r2 = zb;
                u32 w = *(volatile u16 *)&l.item;
                if (l.item >= 0x1531 && w <= 0x153a) r2 = TRUE;
                if (r2) {
                    if (!Clock_GetWeekday()) {
                        b2 = TRUE;
                    } else {
                        s32 m = ((TurnipMarket *)gSaveData.turnipMarket)->getPrice();
                        price += m * Item_GetPrice(&l.item);
                    }
                } else {
                    price += Item_GetPrice(&l.item) / 4;
                }
            }
        }
        i++;
    test0:
        if (i < 15) goto loop0;
    done0:;
        if (b2 == TRUE) {
            l.cmd = 0xd;
        } else if (b1 == TRUE) {
            setNumberSlot(price, 0, 0xa, 1, 0);
            l.cmd = 0x11;
        } else if (b3 == TRUE) {
            l.cmd = 0x40;
            setNumberSlot(price, 0, 0xa, 1, 0);
        } else if (price == 0) {
            l.cmd = 0xa;
        } else {
            l.cmd = 0xe;
            setNumberSlot(price, 0, 0xa, 1, 0);
        }
    } else {
        l.cmd = 8;
    }
    if (ownerNpc->isNook()) {
        ((TalkWindowState *)(owner))->setNextMessage((u8 *)(&l.cmd), sSpNpcNookShopKey);
    } else {
        ((TalkWindowState *)(owner))->setNextMessage((u8 *)(&l.cmd), sSpNpcNookShopTwinsKey);
    }
}

void SpNpcNookShopTalk::handleCatalogMenu() {
    void *o = window;
    Unk_ov050_0225c0a0_Msg m;
    m.msgIndex = 0x1b;
    if (MenuCtrl_IsResultOk() == 1) {
        ownerNpc->selectedItem = MenuCtrl_GetCatalogItem();
        u16 *p = &ownerNpc->selectedItem;
        if (!Unk_ov050_0225bd54_Same(p, &m.item, 0xfff1)) {
            price = Item_GetMemberPrice(&ownerNpc->selectedItem);
            if (price) {
                setNumberSlot(price, 0, 10, 1, 0);
                setItemNameSlot(&ownerNpc->selectedItem, 1, 7);
                m.msgIndex = 0x1a;
            }
        }
    }
    if (ownerNpc->isNook()) {
        ((TalkWindowState *)(o))->setNextMessage((u8 *)(&m), sSpNpcNookShopKey);
    } else {
        ((TalkWindowState *)(o))->setNextMessage((u8 *)(&m), sSpNpcNookShopTwinsKey);
    }
}

extern "C" BOOL SpNpcNookShop_IsDeliveryParcel(u16 *p, s32 m) {
    if (m == 2) {
        BOOL r = FALSE;
        if (*p >= 0x155f && *p <= 0x1560) {
            r = TRUE;
        }
        return r;
    }
    return FALSE;
}

void SpNpcNookShopTalk::handleDeliveryMenu() {
    void *o = window;
    Unk_ov050_0225c0a0_Msg m;
    m.msgIndex = 4;
    if (MenuCtrl_IsResultOk()) {
        PlayerData *r7 = PlayerData_GetCurrent();
        if (Talk_IsInOwnTown()) {
            s32 r6 = MenuCtrl_GetIndex();
            m.item = Pocket_GetItem(r6);
            if (r6 >= 0) {
                Pocket_RemoveItem(r6);
            }
            if (!Unk_ov050_0225bd54_Same(&m.item, &m.cmpItem, 0xfff1)) {
                requestTakeItem(&m.item, 2, 5, 0);
            }
            m.msgIndex = 3;
            ((ErrandRecord *)(ParcelErrand_GetRecord(&(r7->getErrands())->parcelErrand)))->setStep(1);
        }
    }
    if (ownerNpc->isNook()) {
        ((TalkWindowState *)(o))->setNextMessage((u8 *)(&m), sSpNpcNookShopKey);
    } else {
        ((TalkWindowState *)(o))->setNextMessage((u8 *)(&m), sSpNpcNookShopTwinsKey);
    }
}

SpNpcNookShopTalk::SpNpcNookShopTalk() {}

SpNpcNookShopTalk::~SpNpcNookShopTalk() {}

void SpNpcNookShopTalk::resetMsg() {
    ActorTalkRequest::resetMsg();
    pendingMenuHandler = NULL;
}

void SpNpcNookShopTalk::attachOwner(SpNpcNookShop *owner) {
    resetMsg();
    ownerNpc = owner;
    safeSlot = -1;
}

void SpNpcNookShopTalk::setTopic(s32 v) {
    topic = v;
}

s32 SpNpcNookShopTalk::getTopic() {
    return topic;
}

extern "C" s32 SpNpcNookShop_GetHouseUpgradeMsg(void *self) {
    HouseData *house = (HouseData *)gSaveData.house;
    s32 r5 = house->getDebt();
    s32 r4 = house->getLevel();
    s32 r6 = house->isUpgradePaidOff();
    if (gSaveData.testFlag(0xe)) {
        return 0;
    }
    if (r5 == 0 && r4 == 0 && !house->isUpgradePending()) {
        return 0x41;
    }
    if (r4 == 1 && r6 != 0) {
        gSaveData.clearFlag(0xd);
        return 0x49;
    }
    if (r5 == 0 && r4 == 1 && !house->isUpgradePending()) {
        return 0x4a;
    }
    if (r4 == 2 && r6 != 0) {
        gSaveData.clearFlag(0xd);
        return 0x4b;
    }
    if (r5 == 0 && r4 == 2 && !house->isUpgradePending()) {
        return 0x4c;
    }
    if (r4 == 3 && r6 != 0) {
        gSaveData.clearFlag(0xd);
        return 0x4d;
    }
    if (r5 == 0 && r4 == 3 && !house->isUpgradePending()) {
        return 0x4e;
    }
    if (r4 == 4 && r6 != 0) {
        gSaveData.clearFlag(0xd);
        return 0x4f;
    }
    if (r5 == 0 && r4 == 4 && !house->isUpgradePending()) {
        return 0x50;
    }
    if (r4 == 5 && r6 != 0) {
        gSaveData.clearFlag(0xd);
        return 0x51;
    }
    if (r5 == 0 && r4 == 5 && !house->isUpgradePending()) {
        return 0x52;
    }
    if (r4 == 6 && r6 != 0) {
        gSaveData.clearFlag(0xd);
        return 0x53;
    }
    if (r5 == 0 && r4 == 6 && !house->isUpgradePending()) {
        gSaveData.setFlag(0xe);
        return 0x54;
    }
    return 0;
}

extern "C" BOOL SpNpcNookShop_IsEmptyItem(u16 *p) {
    if (*p == 0xfff1) {
        return TRUE;
    }
    return FALSE;
}

void SpNpcNookShopTalk::givePlantingItems() {
    u16 l[6];
    l[0] = 0x14fe + Random_GlobalBelow(3);
    Pocket_AddItem(&l[0], 0);
    l[1] = 0x1504 + Random_GlobalBelow(3);
    Pocket_AddItem(&l[1], 0);
    l[2] = 0x150a + Random_GlobalBelow(3);
    Pocket_AddItem(&l[2], 0);
    l[3] = 0x1510 + Random_GlobalBelow(3);
    Pocket_AddItem(&l[3], 0);
    s32 i;
    for (i = 0; i < 3; i++) {
        l[4] = 0x151d;
        Pocket_AddItem(&l[4], 0);
    }
    l[5] = 0x151d;
    requestGiveItem(&l[5], 0, 5, 0);
}

BOOL SpNpcNookShopTalk::checkNotInUniform(TalkStartMsg *out) {
    PlayerData *r4 = PlayerData_GetCurrent();
    if (Arbeit_IsOnDuty(r4->getErrands()) && r4) {
        u16 *p = r4->getShirt();
        u16 t;
        if (!Unk_ov050_0225bd54_Same(p, &t, 0x11a8)) {
            out->msgIndex = 0xe;
            return TRUE;
        }
    }
    return FALSE;
}

void SpNpcNookShopTalk::pickArbeitStartMsg0B(TalkStartMsg *out) {
    PlayerData *g = PlayerData_GetCurrent();
    PlayerErrands *s0 = g->getErrands();
    PlayerErrandSlot *a = PlayerErrands_GetSlot(s0, 0);
    void *r4 = PlayerErrandSlot_GetRecord(a);
    Pocket_FindEmpty();
    u16 l[4];
    PocketMatches r;
    if (((ErrandRecord *)r4)->getKind() == 0xb && (u32)((ErrandRecord *)r4)->getStep() >= 1 && PlayerErrandSlot_IsStepDone(a)) {
        if (!checkNotInUniform(out)) {
            Pocket_CountMatching(&r, SpNpcNookShop_IsEmptyItem);
            if (r.count >= 7) {
                out->msgIndex = 8;
            } else {
                out->msgIndex = 9;
            }
        }
    } else if (((ErrandRecord *)r4)->getKind() == 0xb && (u32)((ErrandRecord *)r4)->getStep() >= 1) {
        u16 *p = g->getShirt();
        if (Unk_ov050_0225bd54_Same(p, &l[3], 0x11a8)) {
            out->msgIndex = 7;
        } else {
            l[0] = 0x11a8;
            BOOL n = Pocket_FindItem(&l[0]) < 0 ? TRUE : FALSE;
            if (n) {
                l[1] = 0x11a8;
                if (Pocket_AddItem(&l[1], 0)) {
                    out->msgIndex = 4;
                } else {
                    out->msgIndex = 2;
                }
            } else {
                out->msgIndex = 5;
            }
        }
    } else {
        l[2] = 0x11a8;
        if (Pocket_AddItem(&l[2], 0)) {
            Arbeit_Start(s0);
            out->msgIndex = 3;
        } else {
            out->msgIndex = 1;
        }
    }
}

void SpNpcNookShopTalk::pickArbeitStartMsg0C(TalkStartMsg *out) {
    PlayerData *g = PlayerData_GetCurrent();
    void *p = PlayerErrandSlot_GetRecord(PlayerErrands_GetSlot(g->getErrands(), 0));
    Pocket_FindEmpty();
    if (!checkNotInUniform(out)) {
        BOOL k0 = FALSE, k1 = FALSE, k2 = FALSE, k3 = FALSE, k4 = FALSE;
        u16 l[5];
        s32 i;
        for (i = 0; i < 3; i++) {
            l[0] = 0x151d;
            if (Pocket_FindItem(&l[0]) >= 0 ? TRUE : k0) goto found;
            l[1] = 0x14fe + i;
            if (Pocket_FindItem(&l[1]) >= 0 ? TRUE : k1) goto found;
            l[2] = 0x1504 + i;
            if (Pocket_FindItem(&l[2]) >= 0 ? TRUE : k2) goto found;
            l[3] = 0x150a + i;
            if (Pocket_FindItem(&l[3]) >= 0 ? TRUE : k3) goto found;
            l[4] = 0x1510 + i;
            if (Pocket_FindItem(&l[4]) >= 0 ? TRUE : k4) {
            found:
                out->msgIndex = ownerNpc->arbeitPrompted + 0xf;
                ownerNpc->arbeitPrompted = 1;
                return;
            }
        }
        ownerNpc->arbeitPrompted = 1;
        if (g->testFlag(10) && SaveVillagers_AllKnowPlayer(gSaveData.villagers, g->getPlayerId()) &&
            Birthday_IsSet(PlayerData_GetCurrent()->getBirthday())) {
            ((ErrandRecord *)p)->setStep(1);
            out->msgIndex = 0x1a;
        } else {
            out->msgIndex = 0x11;
        }
    }
}

void SpNpcNookShopTalk::pickArbeitStartMsg0D(TalkStartMsg *out) {
    PlayerData *g = PlayerData_GetCurrent();
    PlayerErrandSlot_GetRecord(PlayerErrands_GetSlot(g->getErrands(), 0));
    Pocket_FindEmpty();
    if (!checkNotInUniform(out)) {
        if (!(g->testFlag(10) && SaveVillagers_AllKnowPlayer(gSaveData.villagers, g->getPlayerId()))) {
            if (ownerNpc->arbeitPrompted != 0) {
                out->msgIndex = 0x13;
            } else {
                out->msgIndex = 0x14;
            }
        } else {
            if (!Birthday_IsSet(PlayerData_GetCurrent()->getBirthday())) {
                out->msgIndex = 0x15;
            } else {
                out->msgIndex = 0x16;
            }
        }
    }
}

void SpNpcNookShopTalk::pickArbeitStartMsg0E(TalkStartMsg *out) {
    if (!checkNotInUniform(out)) {
        if (((ErrandRecord *)(PlayerErrandSlot_GetRecord(PlayerErrands_GetSlot(PlayerData_GetCurrent()->getErrands(), 0))))->getStep() == 1) {
            out->msgIndex = 0x1e;
        } else if (ownerNpc->arbeitPrompted == 0) {
            out->msgIndex = 0x1c;
            ownerNpc->arbeitPrompted = 1;
        } else {
            out->msgIndex = 0x10;
        }
    }
}

// ---------------------------------------------------------------------------------------------------------------------
void SpNpcNookShopTalk::pickArbeitStartMsg0F(TalkStartMsg *out) {
    if (!checkNotInUniform(out)) {
        PlayerData *g = PlayerData_GetCurrent();
        PlayerErrandSlot *a = PlayerErrands_GetSlot(g->getErrands(), 0);
        void *b = PlayerErrandSlot_GetRecord(a);
        VillagerId *c = PlayerErrandSlot_GetVillager(a, 1);
        MsgString9B o;
        if (((ErrandRecord *)b)->getStep() == 2) {
            out->msgIndex = 0x28;
            return;
        }
        if (((ErrandRecord *)b)->getStep() == 0) {
            u8 s = ownerNpc->arbeitPrompted;
            if (s == 0 || s == 2) {
                if (c) {
                    c->getName(&o);
                    window->setSlot(0, &o);
                }
                out->msgIndex = 0x21;
                if (ownerNpc->arbeitPrompted == 2) {
                    ownerNpc->arbeitPrompted = 1;
                }
            } else {
                out->msgIndex = 0x10;
            }
            return;
        }
        if (((ErrandRecord *)b)->getStep() == 1) {
            if (c) {
                ((VillagerId *)(c))->getName(&o);
                window->setSlot(0, &o);
            }
            out->msgIndex = 0x27;
            return;
        }
    }
}

void SpNpcNookShopTalk::pickArbeitStartMsg10(TalkStartMsg *out) {
    if (checkNotInUniform(out) == 0) {
        PlayerErrands *h = PlayerData_GetCurrent()->getErrands();
        if (((ErrandRecord *)(PlayerErrandSlot_GetRecord(PlayerErrands_GetSlot(h, 0))))->getStep() == 1) {
            out->msgIndex = 0x2c;
        } else if (ownerNpc->arbeitPrompted == 0) {
            out->msgIndex = 0x2b;
            ownerNpc->arbeitPrompted = 1;
        } else {
            out->msgIndex = 0x10;
        }
    }
}

void SpNpcNookShopTalk::pickArbeitStartMsg11(TalkStartMsg *out) {
    if (checkNotInUniform(out) == 0) {
        PlayerErrands *h = PlayerData_GetCurrent()->getErrands();
        if (((ErrandRecord *)(PlayerErrandSlot_GetRecord(PlayerErrands_GetSlot(h, 0))))->getStep() == 1) {
            out->msgIndex = 0x31;
        } else if (ownerNpc->arbeitPrompted == 0) {
            out->msgIndex = 0x2b;
            ownerNpc->arbeitPrompted = 1;
        } else {
            out->msgIndex = 0x10;
        }
    }
}

void SpNpcNookShopTalk::pickArbeitStartMsg12(TalkStartMsg *out) {
    if (checkNotInUniform(out) == 0) {
        PlayerErrands *h = PlayerData_GetCurrent()->getErrands();
        if (((ErrandRecord *)(PlayerErrandSlot_GetRecord(PlayerErrands_GetSlot(h, 0))))->getStep() == 1) {
            out->msgIndex = 0x33;
        } else if (ownerNpc->arbeitPrompted == 0) {
            out->msgIndex = 0x32;
            ownerNpc->arbeitPrompted = 1;
        } else {
            out->msgIndex = 0x10;
        }
    }
}

BOOL SpNpcNookShop::canPlayTalkMelody() {
    if (isTimmy()) {
        return FALSE;
    }
    if (Unk_ov050_0225b7b4_IsOne(gFieldSceneKind) == 0 || nookTalkMelodyPlayed == 0) {
        return TRUE;
    }
    return FALSE;
}

void SpNpcNookShop::onTalkMelodyPlayed() {
    nookTalkMelodyPlayed = 1;
}
extern "C" char sSpNpcNookShopKey[] = "sp_npc_raccoon";

extern "C" const TalkStartMsg sSpNpcNookShopTopicTable[31] = {
    {sSpNpcNookShopKey, 0x00}, {sSpNpcNookShopKey, 0x01}, {sSpNpcNookShopKey, 0x32}, {sSpNpcNookShopKey, 0x33},
    {sSpNpcNookShopKey, 0x34}, {sSpNpcNookShopKey, 0x06}, {sSpNpcNookShopKey, 0x57}, {sSpNpcNookShopKey, 0x02},
    {sSpNpcNookShopKey, 0x31}, {sSpNpcNookShopKey, 0x1e}, {sSpNpcNookShopKey, 0x2a}, {sSpNpcNookShopKey, 0x2b},
    {sSpNpcNookShopKey, 0x2d}, {sSpNpcNookShopKey, 0x2e}, {sSpNpcNookShopKey, 0x35}, {sSpNpcNookShopDramaKey, 0x00},
    {sSpNpcNookShopKey, 0x05}, {sSpNpcNookShopTwinsKey, 0x00}, {sSpNpcNookShopTwinsKey, 0x01}, {sSpNpcNookShopTwinsKey, 0x06},
    {sSpNpcNookShopTwinsKey, 0x31}, {sSpNpcNookShopTwinsKey, 0x1e}, {sSpNpcNookShopTwinsKey, 0x2a}, {sSpNpcNookShopTwinsKey, 0x2b},
    {sSpNpcNookShopTwinsKey, 0x2d}, {sSpNpcNookShopTwinsKey, 0x2e}, {sSpNpcNookShopTwinsKey, 0x32}, {sSpNpcNookShopTwinsKey, 0x33},
    {sSpNpcNookShopTwinsKey, 0x34}, {sSpNpcNookShopTwinsKey, 0x57}, {sSpNpcNookShopSequence5_1Key, 0x0c},
};

// SpNpcNookShopTalk::onArbeitMessageEnd's static tbl
SpNpcNookShopArbeitMessageEndHandler SpNpcNookShopTalk_onArbeitMessageEnd_tbl[24] = {
        {0x00, &SpNpcNookShopTalk::arbeitGiveUniform},
        {0x03, &SpNpcNookShopTalk::arbeitPresentUniform},
        {0x04, &SpNpcNookShopTalk::arbeitPresentUniform},
        {0x07, &SpNpcNookShopTalk::arbeitCheckPocketSpace},
        {0x08, &SpNpcNookShopTalk::arbeitGivePlants},
        {0x0e, &SpNpcNookShopTalk::onArbeitMsgEnd0E},
        {0x12, &SpNpcNookShopTalk::onArbeitMsgEnd12},
        {0x16, &SpNpcNookShopTalk::arbeitStartFurnitureDelivery},
        {0x1a, &SpNpcNookShopTalk::arbeitStartFurnitureDelivery},
        {0x18, &SpNpcNookShopTalk::arbeitGiveDeliveryParcel},
        {0x1e, &SpNpcNookShopTalk::arbeitStartLetterTask},
        {0x1f, &SpNpcNookShopTalk::arbeitGiveStationery},
        {0x21, &SpNpcNookShopTalk::arbeitOfferStationery},
        {0x25, &SpNpcNookShopTalk::arbeitGiveNewStationery},
        {0x27, &SpNpcNookShopTalk::arbeitRetryLetter},
        {0x28, &SpNpcNookShopTalk::arbeitStartCarpetDelivery},
        {0x29, &SpNpcNookShopTalk::arbeitGiveCarpet},
        {0x2c, &SpNpcNookShopTalk::arbeitStartWateringCanDelivery},
        {0x2e, &SpNpcNookShopTalk::arbeitPresentWateringCan},
        {0x31, &SpNpcNookShopTalk::onArbeitMsgEnd31},
        {0x33, &SpNpcNookShopTalk::arbeitReduceLoan},
        {0x35, &SpNpcNookShopTalk::arbeitFinish},
        {0x36, &SpNpcNookShopTalk::arbeitFinish},
        {0x0c, &SpNpcNookShopTalk::arbeitPushPlayerBack},
    };
void SpNpcNookShopTalk::onArbeitMessageEnd() {
    PlayerErrands *h = PlayerData_GetCurrent()->getErrands();
    PlayerErrandSlot_GetRecord(PlayerErrands_GetSlot(h, 0));
    nextMsg = 0xff;
    s32 i = 0;
    u8 *p = &msgIndex;
    SpNpcNookShopArbeitMessageEndHandler *t = SpNpcNookShopTalk_onArbeitMessageEnd_tbl;
    for (; (u32)i < 0x18; i++) {
        u32 a = SpNpcNookShopTalk_onArbeitMessageEnd_tbl[i].id;
        u32 b = *p;
        if (a == b) {
            (this->*t[i].f)(h);
        }
    }
    if (nextMsg != 0xff) {
        u8 v = nextMsg;
        window->setNextMessage((u8 *)(&v), sSpNpcNookShopSequence5_1Key);
    }
}

void SpNpcNookShopTalk::arbeitGiveUniform(PlayerErrands *h) {
    u16 v = 0x11a8;
    if (Pocket_AddItem(&v, 0)) {
        Arbeit_Start(h);
        nextMsg = 3;
    } else {
        nextMsg = 1;
    }
}

void SpNpcNookShopTalk::arbeitPresentUniform(PlayerErrands *h) {
    u16 v = 0x11a8;
    requestGiveItem(&v, 0, 5, 0);
    nextMsg = 6;
    void *r = PlayerErrandSlot_GetRecord(PlayerErrands_GetSlot(h, 0));
    if (((ErrandRecord *)r)->isActive() && ((ErrandRecord *)r)->getKind() == 0xb) {
        ((ErrandRecord *)r)->setStep(1);
    }
}

void SpNpcNookShopTalk::arbeitCheckPocketSpace(PlayerErrands *h) {
    PocketMatches b;
    Pocket_CountMatching(&b, SpNpcNookShop_IsEmptyItem);
    if (b.count >= 7) {
        nextMsg = 8;
    } else {
        nextMsg = 9;
    }
    void *r = PlayerErrandSlot_GetRecord(PlayerErrands_GetSlot(h, 0));
    Arbeit_SetOnDuty(h);
    ((ErrandRecord *)r)->setStep(2);
}

void SpNpcNookShopTalk::arbeitGivePlants(PlayerErrands *h) {
    givePlantingItems();
    Arbeit_StartPlanting(h);
    ownerNpc->arbeitPrompted = 0;
    nextMsg = 10;
}

void SpNpcNookShopTalk::onArbeitMsgEnd0E(PlayerErrands *h) {
    Arbeit_ClearOnDuty(h);
}

void SpNpcNookShopTalk::onArbeitMsgEnd12(PlayerErrands *h) {
    PlayerData_GetCurrent();
    Arbeit_StartGreetings(h);
}

void SpNpcNookShopTalk::arbeitStartFurnitureDelivery(PlayerErrands *h) {
    u16 v;
    void *r = PlayerErrandSlot_GetRecord(PlayerErrands_GetSlot(h, 0));
    if (((ErrandRecord *)r)->isActive() && ((ErrandRecord *)r)->getKind() == 0xd) {
        ((ErrandRecord *)r)->setStep(1);
    }
    if (Pocket_FindEmpty() >= 0) {
        MsgString9B name;
        Arbeit_StartFurnitureDelivery(h);
        v = 0x1563;
        PlayerErrands_GetDeliveryRecipientName(h, &name, &v);
        window->setSlot(1, &name);
        ownerNpc->arbeitPrompted = 0;
        nextMsg = 0x18;
    } else {
        nextMsg = 0x17;
    }
}

void SpNpcNookShopTalk::arbeitGiveDeliveryParcel(PlayerErrands *h) {
    u16 v[2];
    v[0] = 0x1563;
    if (Pocket_AddItem(&v[0], 2)) {
        v[1] = 0x1563;
        requestGiveItem(&v[1], 2, 5, 0);
    }
}

void SpNpcNookShopTalk::arbeitStartLetterTask(PlayerErrands *h) {
    PlayerErrandSlot *r = PlayerErrands_GetSlot(h, 0);
    ((ErrandRecord *)(PlayerErrandSlot_GetRecord(r)))->setStep(1);
    if (Pocket_FindEmpty() >= 0) {
        Arbeit_StartLetterTask(h);
        VillagerId *villager = (VillagerId *)PlayerErrandSlot_GetVillager(r, 1);
        if (villager) {
            MsgString9B name;
            villager->getName(&name);
            window->setSlot(0, &name);
        }
        ownerNpc->arbeitPrompted = 0;
        nextMsg = 0x1f;
    } else {
        nextMsg = 0x17;
    }
}

void SpNpcNookShopTalk::arbeitGiveStationery(PlayerErrands *h) {
    u16 v[2];
    v[0] = 0x1020;
    if (Pocket_AddItem(&v[0], 0)) {
        v[1] = 0x1020;
        requestGiveItem(&v[1], 0, 5, 0);
    }
    if (msgIndex == 0x1f) {
        nextMsg = 0x20;
        ownerNpc->arbeitPrompted = 2;
    } else {
        nextMsg = 0x26;
    }
}

void SpNpcNookShopTalk::arbeitOfferStationery(PlayerErrands *h) {
    if (ownerNpc->arbeitPrompted == 0 && Pocket_FindEmpty() != -1) {
        nextMsg = 0x22;
    } else {
        ownerNpc->arbeitPrompted = 1;
        window->setNextMessage((u8 *)(gTalkMsgIndexEnd), 0);
    }
}

void SpNpcNookShopTalk::arbeitGiveNewStationery(PlayerErrands *h) {
    ((ErrandRecord *)(PlayerErrandSlot_GetRecord(PlayerErrands_GetSlot(h, 0))))->setStep(0);
    ownerNpc->arbeitPrompted = 1;
    arbeitGiveStationery(h);
}

void SpNpcNookShopTalk::arbeitRetryLetter(PlayerErrands *h) {
    ((ErrandRecord *)(PlayerErrandSlot_GetRecord(PlayerErrands_GetSlot(h, 0))))->setStep(0);
    if (Pocket_FindEmpty() >= 0) {
        nextMsg = 0x25;
    } else {
        window->setNextMessage((u8 *)(gTalkMsgIndexEnd), 0);
    }
}

void SpNpcNookShopTalk::arbeitStartCarpetDelivery(PlayerErrands *h) {
    u16 v;
    void *r = PlayerErrandSlot_GetRecord(PlayerErrands_GetSlot(h, 0));
    MsgString9B name;
    ((ErrandRecord *)r)->setStep(2);
    ownerNpc->arbeitPrompted = 0;
    if (Pocket_FindEmpty() >= 0) {
        Arbeit_StartCarpetDelivery(h);
        v = 0x1561;
        PlayerErrands_GetDeliveryRecipientName(h, &name, &v);
        window->setSlot(1, &name);
        nextMsg = 0x29;
    } else {
        nextMsg = 0x17;
    }
}

void SpNpcNookShopTalk::arbeitGiveCarpet(PlayerErrands *h) {
    u16 v[2];
    v[0] = 0x1561;
    if (Pocket_AddItem(&v[0], 2)) {
        v[1] = 0x1561;
        requestGiveItem(&v[1], 0, 5, 0);
    }
}

void SpNpcNookShopTalk::arbeitStartWateringCanDelivery(PlayerErrands *h) {
    u16 v[2];
    v[0] = 0x1564;
    if (Pocket_AddItem(&v[0], 2)) {
        MsgString9B name;
        Arbeit_StartWateringCanDelivery(h);
        v[1] = 0x1564;
        PlayerErrands_GetDeliveryRecipientName(h, &name, &v[1]);
        window->setSlot(1, &name);
        nextMsg = 0x2e;
    } else {
        nextMsg = 0x17;
    }
}

void SpNpcNookShopTalk::arbeitPresentWateringCan(PlayerErrands *h) {
    u16 v = 0x1564;
    requestGiveItem(&v, 2, 5, 0);
    ownerNpc->arbeitPrompted = 0;
}

void SpNpcNookShopTalk::onArbeitMsgEnd31(PlayerErrands *h) {
    ((ErrandRecord *)(PlayerErrandSlot_GetRecord(PlayerErrands_GetSlot(h, 0))))->setStep(2);
    Arbeit_StartBbsTask(h);
    ownerNpc->arbeitPrompted = 0;
}

void SpNpcNookShopTalk::arbeitReduceLoan(PlayerErrands *h) {
    HouseData *house = (HouseData *)gSaveData.house;
    if (house->getDebt() < 0x579) {
        nextMsg = 0x36;
    } else {
        s32 t = house->getDebt();
        house->setDebt(t - 0x578);
        s32 s = house->getDebt();
        setNumberSlot(s, 2, 10, 1, 0);
        nextMsg = 0x34;
    }
}

void SpNpcNookShopTalk::arbeitFinish(PlayerErrands *h) {
    Arbeit_Finish(h);
    PlayerData *o = PlayerData_GetCurrent();
    o->clearFlag(1);
    PlayerSpNpcRecord *record = (PlayerSpNpcRecord *)o->getSpNpcRecord();
    TownSessionState_SetFlag(TownSessionState_Get(), 5);
    record->stampArbeitDate();
}

// ---------------------------------------------------------------------------------------------------------------------

void SpNpcNookShopTalk::arbeitPushPlayerBack(PlayerErrands *h) {
    ownerNpc->changeAct(0x11);
}

void SpNpcNookShopTalk::start(TalkStartMsg *out_) {
    TalkStartMsg *out = (TalkStartMsg *)out_;
    s32 flag;
    void *x;
    SpNpcNookShopStartLocal l;
    u16 w0, w4, w6, w8, wa, t0, t1, t2, t3;
    ClockDateBytes s, v, o1, o2, c;
    PlayerData *h = PlayerData_GetCurrent();
    ParcelErrand *r7 = &(h->getErrands())->parcelErrand;
    x = h->getErrands();
    if (h->testFlag(1)) {
        void *ev = PlayerErrandSlot_GetRecord(PlayerErrands_GetSlot((PlayerErrands *)x, 0));
        u32 t = ownerNpc->profile;
        if (Unk_ov050_0225a888_Eq(t, 0x75) || Unk_ov050_0225a888_Eq(t, 0x74)) {
            out->msgKey = sSpNpcNookShopTwinsKey;
            out->msgIndex = 3;
            return;
        }
        out->msgKey = sSpNpcNookShopSequence5_1Key;
        u16 *p = &ownerNpc->selectedItem;
        if (!Unk_ov050_0225a888_Same(p, &t0)) {
            ownerNpc->selectedItem = 0xfff1;
            out->msgIndex = 0xb;
            return;
        }
        if (topic == 0x1e) {
            out->msgIndex = 0xc;
            topic = 5;
            return;
        }
        out->msgIndex = 0;
        if (!((ErrandRecord *)ev)->isActive()) {
            return;
        }
        if (((ErrandRecord *)ev)->getKind() == 0x12) {
            pickArbeitStartMsg12((TalkStartMsg *)out);
        }
        if (((ErrandRecord *)ev)->getKind() == 0x11) {
            pickArbeitStartMsg11((TalkStartMsg *)out);
        }
        if (((ErrandRecord *)ev)->getKind() == 0x10) {
            pickArbeitStartMsg10((TalkStartMsg *)out);
        }
        if (((ErrandRecord *)ev)->getKind() == 0xf) {
            pickArbeitStartMsg0F(out);
        }
        if (((ErrandRecord *)ev)->getKind() == 0xe) {
            pickArbeitStartMsg0E(out);
        }
        if (((ErrandRecord *)ev)->getKind() == 0xd) {
            pickArbeitStartMsg0D(out);
        }
        if (((ErrandRecord *)ev)->getKind() == 0xc) {
            pickArbeitStartMsg0C(out);
        }
        if (((ErrandRecord *)ev)->getKind() == 0xb) {
            pickArbeitStartMsg0B(out);
        }
        return;
    }
    if (ownerNpc->isNook() && topic == 5 && !gCommManager->isOnline() && *DebugVar_GetPtr(0, 0x4a) == 0) {
        if (safeSlot == -1) {
            w0 = 0x36fc;
            safeSlot = Pocket_FindItem(&w0);
        }
        if (safeSlot >= 0) {
            topic = 0xe;
        } else {
            w4 = 0xd019;
            if (ParcelErrand_IsFor(r7, &w4) || (w6 = 0xd01a, ParcelErrand_IsFor(r7, &w6)) || (w8 = 0xd01b, ParcelErrand_IsFor(r7, &w8)) ||
                (wa = 0xd01c, ParcelErrand_IsFor(r7, &wa))) {
                topic = 0x10;
            } else if (Talk_IsDramaPending(ownerNpc, &l, 3)) {
                out->msgKey = sSpNpcNookShopDramaKey;
                out->msgIndex = (sSpNpcNookShopDramaMsgTable + l.b * 6)[l.c];
                setTopic(0xf);
                return;
            }
        }
    }
    if (topic == 0 || topic == 0x11) {
        flag = 0;
        if (topic == 0) {
            out->msgKey = sSpNpcNookShopKey;
            if (h->testFlag(0x26) == 0) {
                v = *(ClockDateBytes *)NookShop_GetRenovation(gSaveData.nookShop);
                Clock_GetDate(o1.b);
                out->msgIndex = 0;
                if (v.b[3] == 0) {
                    s32 r = Date_DaysBetween(o1.b, v.b);
                    if (ownerNpc->param == 0xd01a && h->testFlag(0x24) == 0 && r == 0) {
                        out->msgIndex = NookShop_GetLevel(gSaveData.nookShop) + 0x59;
                    }
                    if (ownerNpc->param == 0xd01b && NookShop_GetLevel(gSaveData.nookShop) == 2 && h->testFlag(0x25) == 0 && r == 0) {
                        out->msgIndex = NookShop_GetLevel(gSaveData.nookShop) + 0x59;
                    }
                    if (ownerNpc->param == 0xd01c && NookShop_GetLevel(gSaveData.nookShop) == 3 && h->testFlag(0x26) == 0 && r == 0) {
                        out->msgIndex = NookShop_GetLevel(gSaveData.nookShop) + 0x59;
                    }
                    switch (NookShop_GetLevel(gSaveData.nookShop)) {
                    case 3:
                        h->setFlag(0x26);
                    case 2:
                        h->setFlag(0x25);
                    case 1:
                        h->setFlag(0x24);
                    }
                    if (out->msgIndex != 0) {
                        return;
                    }
                }
            }
            if (h->testFlag(3) == 0) {
                u8 *q = ((PlayerSpNpcRecord *)(h->getSpNpcRecord()))->getArbeitDate();
                if (q[2] != 0 || q[1] != 0) {
                    Clock_GetDate(o2.b);
                    if (Date_DaysBetween(o2.b, q)) {
                        out->msgIndex = 0x3b;
                        h->setFlag(3);
                        return;
                    }
                }
            }
            if (gSaveData.testFlag(0x10) == 0) {
                out->msgIndex = SpNpcNookShop_GetHouseUpgradeMsg(this);
                if (out->msgIndex != 0) {
                    gSaveData.setFlag(0x10);
                    return;
                }
            }
            if (Talk_CheckAndSetPlayerFlag(0, 0) == 0) {
                if (NookShop_IsClosedTomorrow(gSaveData.nookShop)) {
                    s = NookShop_GetClosedDate(gSaveData.nookShop);
                    c = s;
                    setMonthSlot(c.b[1], 7);
                    setDaySlot(c.b[0], 8);
                    setTopic(7);
                    flag = 1;
                    Talk_CheckAndSetPlayerFlag(0, flag);
                }
            }
        }
        if (flag == 0) {
            if (NookShop_IsPointSpecialToday(gSaveData.nookShop)) {
                if (topic == 0) {
                    setTopic(2);
                } else {
                    setTopic(0x1a);
                }
            } else if (NookShop_IsSaleTime(gSaveData.nookShop)) {
                if (topic == 0) {
                    setTopic(3);
                } else {
                    setTopic(0x1b);
                }
            } else if (NookShop_HasClass4Furniture(gSaveData.nookShop)) {
                if (topic == 0) {
                    setTopic(4);
                } else {
                    setTopic(0x1c);
                }
            }
        }
    } else {
        u16 *p = &ownerNpc->selectedItem;
        if (!Unk_ov050_0225a888_Same(p, &t1)) {
            switch (topic) {
            case 0xc:
            case 0xd:
            case 0x18:
            case 0x19: {
                if (!Unk_ov050_0225a888_Same(&ownerNpc->selectedItem, &t2)) {
                    s32 idx;
                    BOOL r = FALSE;
                    if (ownerNpc->selectedItem >= 0x1521 && ownerNpc->selectedItem <= 0x1530) {
                        r = TRUE;
                    }
                    if (r) {
                        idx = ownerNpc->selectedItem - 0x1521;
                    } else {
                        idx = -1;
                    }
                    l.roofColor = idx;
                    window->setSlotFromString(2, &l.roofColor, "st_roof_paint");
                }
            }
            case 8:
            case 9:
            case 0xa:
            case 0xb:
            case 0x14:
            case 0x15:
            case 0x16:
            case 0x17:
                if (!Unk_ov050_0225a888_Same(&ownerNpc->selectedItem, &t3)) {
                    price = Item_GetShopPrice(&ownerNpc->selectedItem);
                    setNumberSlot(price, 4, 10, 1, 0);
                    setItemNameSlot(&ownerNpc->selectedItem, 1, 7);
                }
                break;
            case 0xe:
            case 0xf:
            case 0x10:
            case 0x11:
            case 0x12:
            case 0x13:
                break;
            }
        }
    }
    if (topic >= 0 && topic < 0x1f) {
        out->msgIndex = sSpNpcNookShopTopicTable[topic].msgIndex;
        out->msgKey = (char *)sSpNpcNookShopTopicTable[topic].msgKey;
        if (topic == 0x12) {
            void *r = Actor::findByProfile(ownerNpc->getOtherTwinProfile(), (Actor *)(0));
            if (r) {
                ((SpNpcNookShop *)r)->changeAct(0xc);
            }
        }
    }
}
// ---- rodata / data
extern "C" const VecFx32 sSpNpcNookShopArbeitStairsBound = {0xd000, 0, 0x8000};

extern "C" const VecFx32 sSpNpcNookShopArbeitPushBackPos = {0x10000, 0, 0x9000};

extern "C" const s32 data_ov050_0225da40[3] = {0x10000, 0, 0x8000};

extern "C" const u8 sSpNpcNookShopDramaMsgTable[32] = {0x00, 0x01, 0x02, 0x03, 0x00, 0x00, 0x04, 0x05, 0x06, 0x00, 0x00, 0x00, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x00, 0x0c, 0x0d, 0x0e, 0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x00, 0x00, 0x00, 0x00, 0x00};

void SpNpcNookShopTalk::onMessageEnd(u32) {
    PlayerData *h = PlayerData_GetCurrent();
    h->getErrands();
    if (h->testFlag(1)) {
        onArbeitMessageEnd();
        return;
    }
    if (ownerNpc->isNook()) {
        // (the request's message file, MsgRequest::fileName: DS offset 4, after the vtable pointer - 8 on 64-bit hosts)
        if (strncmp(fileName, sSpNpcNookShopKey, strlen(sSpNpcNookShopKey)) != 0) {
            return;
        }
    }
    const char *name = ownerNpc->isNook() ? sSpNpcNookShopKey : sSpNpcNookShopTwinsKey;
    nextMsg = 0xff;
    static Unk_ov050_0225a6e0_Row tbl[13] = {
        {0x37, &SpNpcNookShopTalk::giveGlamourShot},
        {0x36, &SpNpcNookShopTalk::onSafeOfferDeclined},
        {0x07, &SpNpcNookShopTalk::openSellMenu},
        {0x13, &SpNpcNookShopTalk::giveMoneyBag},
        {0x18, &SpNpcNookShopTalk::openCatalogMenu},
        {0x22, &SpNpcNookShopTalk::onPurchaseDone},
        {0x23, &SpNpcNookShopTalk::showFirstPurchaseHint},
        {0x49, &SpNpcNookShopTalk::ackHouseUpgrade},
        {0x4b, &SpNpcNookShopTalk::ackHouseUpgrade},
        {0x4d, &SpNpcNookShopTalk::ackHouseUpgrade},
        {0x4f, &SpNpcNookShopTalk::ackHouseUpgrade},
        {0x51, &SpNpcNookShopTalk::ackHouseUpgrade},
        {0x53, &SpNpcNookShopTalk::ackHouseUpgrade},
    };
    u32 i = 0;
    u8 *q = &msgIndex;
    goto test;
loop:
    {
        u32 off = i * 12;
        u32 a = tbl[i].id;
        u32 b = *q;
        if (a == b) {
            Unk_ov050_0225a6e0_Row *r = &tbl[i];
            (this->*r->f)();
        }
    }
    i++;
test:
    if (i < 13) goto loop;
    if (nextMsg != 0xff) {
        u8 c = nextMsg;
        window->setNextMessage((u8 *)(&c), (void *)name);
    }
}

void SpNpcNookShopTalk::giveGlamourShot() {
    u16 a = 0x4a30;
    requestGiveItem(&a, 0, 5, 0);
    u16 b = 0x4a30;
    Pocket_AddItem(&b, 0);
    safeSlot = -1;
    nextMsg = 0x38;
}

void SpNpcNookShopTalk::onSafeOfferDeclined() {
    safeSlot = -2;
}

void SpNpcNookShopTalk::openSellMenu() {
    setSubSceneKind(0x1d, 0);
    openSubScene(2);
    setPendingMenuHandler(0);
}

void SpNpcNookShopTalk::giveMoneyBag() {
    NpcActor_PayPlayer(ownerNpc, price);
}

void SpNpcNookShopTalk::openCatalogMenu() {
    setSubSceneKind(0x3e, 0);
    openSubScene(2);
    setPendingMenuHandler(1);
}

void SpNpcNookShopTalk::onPurchaseDone() {
    u16 *p = &ownerNpc->selectedItem;
    if (Unk_ov050_0225a2d0_Same(p)) {
        return;
    }
    if (Unk_ov050_0225a2d0_R(&ownerNpc->selectedItem, 0x1521, 0x1530)) {
        nextMsg = 0x2f;
        BOOL r = FALSE;
        u32 v = ownerNpc->selectedItem;
        if (v >= 0x1521 && v <= 0x1530) {
            r = TRUE;
        }
        s32 idx;
        if (r) {
            idx = v - 0x1521;
        } else {
            idx = -1;
        }
        ((HouseData *)gSaveData.house)->orderRoofPaint((u8)idx);
    } else {
        nextMsg = 0x23;
    }
}

void SpNpcNookShopTalk::showFirstPurchaseHint() {
    u16 *p = &ownerNpc->selectedItem;
    if (Unk_ov050_0225a2d0_Same(p)) {
        return;
    }
    PlayerData *h = PlayerData_GetCurrent();
    if (Unk_ov050_0225a2d0_R(&ownerNpc->selectedItem, 0x1369, 0x1369) && h->testFlag(0x29) == 0) {
        nextMsg = 0x24;
        h->setFlag(0x29);
    } else if (Unk_ov050_0225a2d0_R(&ownerNpc->selectedItem, 0x1376, 0x1376) && h->testFlag(0x2a) == 0) {
        nextMsg = 0x25;
        h->setFlag(0x2a);
    } else if (Unk_ov050_0225a2d0_R(&ownerNpc->selectedItem, 0x1374, 0x1374) && h->testFlag(0x2b) == 0) {
        nextMsg = 0x26;
        h->setFlag(0x2b);
    } else if (Unk_ov050_0225a2d0_R(&ownerNpc->selectedItem, 0x136b, 0x1372) && h->testFlag(0x2c) == 0) {
        nextMsg = 0x27;
        h->setFlag(0x2c);
    } else if (Unk_ov050_0225a2d0_R(&ownerNpc->selectedItem, 0x1378, 0x1378) && h->testFlag(0x2d) == 0) {
        nextMsg = 0x28;
        h->setFlag(0x2d);
    } else if (Unk_ov050_0225a2d0_R(&ownerNpc->selectedItem, 0x137a, 0x137a) && h->testFlag(0x2e) == 0) {
        nextMsg = 0x29;
        h->setFlag(0x2e);
    } else if (Unk_ov050_0225a2d0_R(&ownerNpc->selectedItem, 0x151f, 0x151f) && h->testFlag(0x2f) == 0) {
        nextMsg = 0x39;
        h->setFlag(0x2f);
    } else if (Unk_ov050_0225a2d0_R(&ownerNpc->selectedItem, 0x156c, 0x156c) && h->testFlag(0x37) == 0) {
        nextMsg = 0x10;
        h->setFlag(0x37);
    }
    if (nextMsg == 0xff) {
        if (Talk_IsInOwnTown()) {
            if (ownerNpc->isNook()) {
                if (h->testFlag(0x38) == 0) {
                    nextMsg = 0x30;
                    h->setFlag(0x38);
                    return;
                }
            }
        }
    }
    window->setNextMessage((u8 *)(gTalkMsgIndexEnd), 0);
}

void SpNpcNookShopTalk::ackHouseUpgrade() {
    ((HouseData *)gSaveData.house)->startLoan();
}

void SpNpcNookShopTalk::setRoofColor(s32 row, s32 col) {
    const u8 *q = sSpNpcNookShopRoofColorChoices + row * 4;
    u8 v = q[col];
    u8 b = v;
    window->setSlotFromString(2, &b, "st_roof_paint");
    ((HouseData *)gSaveData.house)->orderUpgrade(v);
}
extern "C" char data_ov050_0225e2d0[] = "npc_sp/model/rcn_tex.nsbtx";

extern "C" char data_ov050_0225e2ec[] = "npc_sp/model/rcc_tex.nsbtx";

extern "C" char data_ov050_0225e308[] = "npc_sp/model/rcs_tex.nsbtx";

extern "C" char data_ov050_0225e324[] = "npc_sp/model/rcd_tex.nsbtx";

extern "C" char data_ov050_0225e340[] = "npc_sp/model/lrc_tex.nsbtx";

extern "C" const s32 sSpNpcNookShopItemTopicTable[6][2] = {
    {0x0b, 0x17}, {0x0a, 0x16}, {0x0d, 0x19}, {0x0c, 0x18}, {0x08, 0x14}, {0x09, 0x15},
};

extern "C" Unk_ov050_0225d1d4_Ent sSpNpcNookShopActTable[19] = {
    {&SpNpcNookShop::setupAct00, &SpNpcNookShop::mainAct00},
    {&SpNpcNookShop::setupAct01, &SpNpcNookShop::mainAct01},
    {&SpNpcNookShop::setupAct02, &SpNpcNookShop::mainAct02},
    {&SpNpcNookShop::setupAct03, &SpNpcNookShop::mainAct03},
    {&SpNpcNookShop::setupAct04, &SpNpcNookShop::mainAct04},
    {&SpNpcNookShop::setupAct05, &SpNpcNookShop::mainAct05},
    {&SpNpcNookShop::setupAct06, &SpNpcNookShop::mainAct06},
    {&SpNpcNookShop::setupAct07, &SpNpcNookShop::mainAct07},
    {&SpNpcNookShop::setupAct08, &SpNpcNookShop::mainAct08},
    {&SpNpcNookShop::setupAct09, &SpNpcNookShop::mainAct09},
    {&SpNpcNookShop::setupAct0A, &SpNpcNookShop::mainAct0A},
    {&SpNpcNookShop::setupAct0B, &SpNpcNookShop::mainAct0B},
    {&SpNpcNookShop::setupAct0C, &SpNpcNookShop::mainAct0C},
    {&SpNpcNookShop::setupAct0D, &SpNpcNookShop::mainAct0D},
    {&SpNpcNookShop::setupAct0E, &SpNpcNookShop::mainAct0E},
    {&SpNpcNookShop::setupAct0F, &SpNpcNookShop::mainAct0F},
    {&SpNpcNookShop::setupAct10, &SpNpcNookShop::mainAct10},
    {&SpNpcNookShop::setupAct11, &SpNpcNookShop::mainAct11},
    {&SpNpcNookShop::setupAct12, &SpNpcNookShop::mainAct12},
};

// ---------------------------------------------------------------------------------------------------------------------

void SpNpcNookShopTalk::onChoice(u32) {
    static Unk_ov050_0225e4b4_Fn tbl[3] = {
        &SpNpcNookShopTalk::onShopChoice,
        &SpNpcNookShopTalk::onDrama2Choice,
        &SpNpcNookShopTalk::onArbeitChoice,
    };
    s32 i = 0;
    PlayerData *h = PlayerData_GetCurrent();
    h->getErrands();
    if (h->testFlag(1)) {
        i = 2;
    } else if (topic == 0xf) {
        i = 1;
    }
    (this->*tbl[i])();
}
extern "C" char sSpNpcNookShopTwinsKey[] = "sp_npc_twins";

extern "C" ActorProfile sSpNpcNookShopProfile = {(void *(*)())SpNpcNookShop_Create, 0x76, 0x7b, 2, 0x5000, 0x5000, 0x3e800};

extern "C" ActorProfile sSpNpcTimmyProfile = {(void *(*)())SpNpcNookShop_CreateTimmy, 0x75, 0x7a, 2, 0x5000, 0x5000, 0x3e800};

extern "C" ActorProfile sSpNpcTommyProfile = {(void *(*)())SpNpcNookShop_CreateTommy, 0x74, 0x79, 2, 0x5000, 0x5000, 0x3e800};

void SpNpcNookShopTalk::onShopChoice() {
    s32 t = getChoiceList()->getResult();
    nextMsg = 0xff;
    static SpNpcNookShopChoiceHandler tbl[37] = {
        {0x00, &SpNpcNookShopTalk::onMainMenuChoice},
        {0x02, &SpNpcNookShopTalk::onMainMenuChoice},
        {0x05, &SpNpcNookShopTalk::onDeliveryChoice},
        {0x06, &SpNpcNookShopTalk::onMainMenuChoice},
        {0x08, &SpNpcNookShopTalk::onMainMenuChoice},
        {0x0a, &SpNpcNookShopTalk::onCannotBuyChoice},
        {0x0b, &SpNpcNookShopTalk::onMainMenuChoice},
        {0x0c, &SpNpcNookShopTalk::onMainMenuChoice},
        {0x0e, &SpNpcNookShopTalk::onSellConfirmChoice},
        {0x0f, &SpNpcNookShopTalk::onMainMenuChoice},
        {0x11, &SpNpcNookShopTalk::onSellConfirmChoice},
        {0x12, &SpNpcNookShopTalk::onMainMenuChoice},
        {0x17, &SpNpcNookShopTalk::onMainMenuChoice},
        {0x19, &SpNpcNookShopTalk::onMainMenuChoice},
        {0x1a, &SpNpcNookShopTalk::onCatalogOrderChoice},
        {0x1b, &SpNpcNookShopTalk::onMainMenuChoice},
        {0x1c, &SpNpcNookShopTalk::onMainMenuChoice},
        {0x1d, &SpNpcNookShopTalk::onMainMenuChoice},
        {0x1e, &SpNpcNookShopTalk::onBuyChoice},
        {0x2a, &SpNpcNookShopTalk::onBuyChoice},
        {0x2b, &SpNpcNookShopTalk::onBuyOrShowChoice},
        {0x2c, &SpNpcNookShopTalk::onBuyAfterPreviewChoice},
        {0x2e, &SpNpcNookShopTalk::onBuyPaintChoice},
        {0x31, &SpNpcNookShopTalk::onBuyChoice},
        {0x35, &SpNpcNookShopTalk::onSafeOfferChoice},
        {0x40, &SpNpcNookShopTalk::onSellConfirmChoice},
        {0x42, &SpNpcNookShopTalk::onRoofColorChoice0},
        {0x43, &SpNpcNookShopTalk::onRoofColorChoice0},
        {0x44, &SpNpcNookShopTalk::onRoofColorChoice1},
        {0x45, &SpNpcNookShopTalk::onRoofColorChoice2},
        {0x46, &SpNpcNookShopTalk::onRoofColorChoice3},
        {0x47, &SpNpcNookShopTalk::onRoofColorChoice0},
        {0x55, &SpNpcNookShopTalk::onRoofColorChoice0},
        {0x56, &SpNpcNookShopTalk::onRoofColorChoice0},
        {0x58, &SpNpcNookShopTalk::onMainMenuChoice},
        {0x59, &SpNpcNookShopTalk::onMainMenuChoice},
        {0x5d, &SpNpcNookShopTalk::onMainMenuChoice},
    };
    u32 i = 0;
    u8 *pe = &msgIndex;
    goto test;
loop:
    {
        u32 id = tbl[i].id;
        u32 cur = *pe;
        if (id == cur) {
            (this->*tbl[i].fn)(t);
        }
    }
    i++;
test:
    if (i < 37) {
        goto loop;
    }
    if (nextMsg != 0xff) {
        char *str = ownerNpc->isNook() != 0 ? sSpNpcNookShopKey : sSpNpcNookShopTwinsKey;
        u8 b = nextMsg;
        window->setNextMessage((u8 *)(&b), str);
    }
}

void SpNpcNookShopTalk::onMainMenuChoice(s32 p) {
    PlayerData_GetCurrent();
    switch (p) {
    case 0:
        nextMsg = 7;
        break;
    case 1:
        if (Talk_IsInOwnTown() == 0) {
            nextMsg = 0x59;
        } else if (NookShop_CanTakeCatalogOrder() != 0) {
            nextMsg = 0x18;
        } else {
            nextMsg = 0x17;
        }
        break;
    case 2:
        if (Clock_GetWeekday() != 0) {
            s32 r = ((TurnipMarket *)gSaveData.turnipMarket)->getPrice();
            setNumberSlot(r, 1, 3, 1, 0);
            nextMsg = 0x16;
        } else {
            nextMsg = 0xd;
        }
        break;
    case 3:
        nextMsg = 0x15;
        break;
    }
}

void SpNpcNookShopTalk::onDeliveryChoice(s32 p) {
    if (p == 0) {
        setPocketFilter(SpNpcNookShop_IsDeliveryParcel, 0xd, 0);
        openSubScene(0);
        setPendingMenuHandler(2);
    } else {
        nextMsg = 0xf;
    }
}

void SpNpcNookShopTalk::onCannotBuyChoice(s32 p) {
    if (p != 0) {
        MenuCtrl_ReturnChosenItems(0);
    }
}

void SpNpcNookShopTalk::onCatalogOrderChoice(s32 p) {
    if (p == 0) {
        if (NpcActor_CanPlayerPay(ownerNpc, (s32)price) != 0) {
            NpcActor_ChargePlayer(ownerNpc, (s32)price);
            nextMsg = 0x1c;
            u16 *q = &ownerNpc->selectedItem;
            if (!Unk_ov050_02259bb8_Match(q)) {
                NookShop_SendCatalogOrder(&ownerNpc->selectedItem);
            }
        } else {
            nextMsg = 0x1d;
        }
    } else {
        nextMsg = 0x1b;
    }
}

void SpNpcNookShopTalk::onBuyChoice(s32 p) {
    if (p == 0) {
        if (Pocket_FindEmpty() < 0) {
            nextMsg = 0x20;
        } else if (NpcActor_CanPlayerPay(ownerNpc, (s32)price) == 0) {
            nextMsg = 0x21;
        } else {
            nextMsg = 0x22;
            buySelectedItem();
        }
    }
}

void SpNpcNookShopTalk::onBuyAfterPreviewChoice(s32 p) {
    u16 *q = &ownerNpc->selectedItem;
    if (!Unk_ov050_02259bb8_Match(q)) {
        BOOL r = FALSE;
        u32 v = ownerNpc->selectedItem;
        if (v < 0x1100 || v > 0x1143) {
        } else {
            r = TRUE;
        }
        if (r) {
            RoomWallFloor_RestoreWallpaper(1);
        } else {
            RoomWallFloor_RestoreCarpet(1);
        }
        Camera_RestorePrevMode();
    }
    onBuyChoice(p);
}

void SpNpcNookShopTalk::onBuyOrShowChoice(s32 p) {
    if (p == 0) {
        if (Pocket_FindEmpty() < 0) {
            nextMsg = 0x20;
        } else if (NpcActor_CanPlayerPay(ownerNpc, (s32)price) == 0) {
            nextMsg = 0x21;
        } else {
            nextMsg = 0x22;
            buySelectedItem();
        }
    } else if (p == 1) {
        u16 *q = &ownerNpc->selectedItem;
        if (!Unk_ov050_02259bb8_Match(q)) {
            BOOL r = FALSE;
            u32 v = ownerNpc->selectedItem;
            if (v < 0x1100 || v > 0x1143) {
            } else {
                r = TRUE;
            }
            if (r) {
                Camera_SetMode11();
                RoomWallFloor_SetWallpaper(&ownerNpc->selectedItem, 0, 1, 1);
            } else {
                Camera_SetMode12();
                RoomWallFloor_SetCarpet(&ownerNpc->selectedItem, 0, 1, 1);
            }
        }
    }
}

void SpNpcNookShopTalk::onBuyPaintChoice(s32 p) {
    if (p == 0) {
        if (NpcActor_CanPlayerPay(ownerNpc, (s32)price) == 0) {
            nextMsg = 0x21;
        } else {
            SpNpcNookShop *o;
            nextMsg = 0x22;
            ownerNpc->purchasePending = 1;
            NpcActor_ChargePlayer(ownerNpc, (s32)price);
            PlayerDataArray_FindById(gSaveData.players, (u16 *)PlayerData_GetCurrent()->getPlayerId());
            o = ownerNpc;
            NookShop_BuyAt(o->selectedItemX, o->selectedItemZ, price, Scene_GetCurrent());
        }
    }
}

void SpNpcNookShopTalk::onSafeOfferChoice(s32 p) {
    if (p == 0) {
        if (safeSlot >= 0) {
            u16 v;
            Pocket_RemoveItem(safeSlot);
            v = 0x36fc;
            requestTakeItem(&v, 0, 5, 0);
        }
        nextMsg = 0x37;
    }
}

void SpNpcNookShopTalk::onSellConfirmChoice(s32 p) {
    if (p == 0) {
        switch (NpcActor_CheckPayoutFits(ownerNpc, (s32)price, soldCount)) {
        case 0:
            NpcActor_PayPlayer(ownerNpc, (s32)price);
            NookShop_RecordBuyback(price);
            nextMsg = 0x12;
            break;
        case 1:
            NookShop_RecordBuyback(price);
            nextMsg = 0x13;
            break;
        case 2:
            nextMsg = 0x5d;
            MenuCtrl_ReturnChosenItems(0);
            break;
        }
    } else {
        if (msgIndex != 0x40) {
            MenuCtrl_ReturnChosenItems(0);
        } else {
            MenuCtrl_ReturnChosenItems(1);
        }
    }
}

void SpNpcNookShopTalk::onRoofColorChoice0(s32 p) {
    if (p != 4) {
        setRoofColor(0, p);
        nextMsg = 0x48;
    }
}

void SpNpcNookShopTalk::onRoofColorChoice1(s32 p) {
    if (p != 4) {
        setRoofColor(1, p);
        nextMsg = 0x48;
    }
}

void SpNpcNookShopTalk::onRoofColorChoice2(s32 p) {
    if (p != 4) {
        setRoofColor(2, p);
        nextMsg = 0x48;
    }
}

void SpNpcNookShopTalk::onRoofColorChoice3(s32 p) {
    if (p != 4) {
        setRoofColor(3, p);
        nextMsg = 0x48;
    }
}

void SpNpcNookShopTalk::onDrama2Choice() {
    s32 t = getChoiceList()->getResult();
    PlayerData_GetCurrent();
    u8 *g = gSaveData.turnipMarket;
    s32 lv = msgIndex;
    if (lv <= 0x14) {
        SpNpcNookShopDramaChoiceLocal buf;
        nextMsg = 0xff;
        if (t != 3) {
            setTopic(5);
        }
        switch (t) {
        case 0:
            nextMsg = 7;
            break;
        case 1:
            if (Talk_IsInOwnTown() == 0) {
                nextMsg = 0x59;
            } else if (NookShop_CanTakeCatalogOrder() != 0) {
                nextMsg = 0x18;
            } else {
                nextMsg = 0x17;
            }
            break;
        case 2:
            if (Clock_GetWeekday() != 0) {
                s32 r = ((TurnipMarket *)g)->getPrice();
                setNumberSlot(r, 1, 3, 1, 0);
                nextMsg = 0x16;
            } else {
                nextMsg = 0xd;
            }
            break;
        case 3:
            if (ownerNpc->isNook() != 0) {
                if (Talk_IsDramaPending(ownerNpc, &buf, 3) != 0) {
                    Talk_AdvanceDrama(ownerNpc, &buf);
                }
            }
            break;
        case 4:
            nextMsg = 0x15;
            break;
        }
        s32 c = nextMsg;
        if (c != 0xff) {
            buf.b = c;
            window->setNextMessage((u8 *)(&buf.b), sSpNpcNookShopKey);
        }
    }
}
extern "C" char sSpNpcNookShopSequence5_1Key[] = "sp_etc_sequence5_1";

void SpNpcNookShopTalk::onArbeitChoice() {
    s32 t = getChoiceList()->getResult();
    nextMsg = 0xff;
    static SpNpcNookShopChoiceHandler tbl[1] = {
        {0x22, &SpNpcNookShopTalk::onArbeitStationeryChoice},
    };
    u32 i = 0;
    u8 *pe = &msgIndex;
    goto test;
loop:
    {
        u32 id = tbl[i].id;
        u32 cur = *pe;
        if (id == cur) {
            (this->*tbl[i].fn)(t);
        }
    }
    i++;
test:
    if (i < 1) {
        goto loop;
    }
    if (nextMsg != 0xff) {
        u8 b = nextMsg;
        window->setNextMessage((u8 *)(&b), sSpNpcNookShopSequence5_1Key);
    }
}

void SpNpcNookShopTalk::onArbeitStationeryChoice(s32 p) {
    PlayerData_GetCurrent();
    if (p == 0) {
        if (Pocket_FindEmpty() >= 0) {
            nextMsg = 0x25;
        } else {
            nextMsg = 0x17;
        }
    }
}

void SpNpcNookShopTalk::buySelectedItem() {
    SpNpcNookShop *o;
    ownerNpc->purchasePending = 1;
    NpcActor_ChargePlayer(ownerNpc, (s32)price);
    Pocket_AddItem(&ownerNpc->selectedItem, 0);
    PlayerDataArray_FindById(gSaveData.players, (u16 *)PlayerData_GetCurrent()->getPlayerId());
    o = ownerNpc;
    NookShop_BuyAt(o->selectedItemX, o->selectedItemZ, price, Scene_GetCurrent());
    VillagerTrend_OnFurnitureBought();
}

BOOL SpNpcNookShop::acceptsInteraction(void *) {
    if (position.z < data_ov050_0225da40[2]) {
        return FALSE;
    }
    if (talkCtrl.isBusy() != 0 || netIsTalkLocked() != 0 || tryItemTalk() != 0) {
        return FALSE;
    }
    return TRUE;
}

// ---------------------------------------------------------------------------------------------------------------------
// SpNpcNookShop

BOOL SpNpcNookShop::acceptsSelfRequestedInteraction(void *) {
    if (talkCtrl.isBusy() != 0 || netIsTalkLocked() != 0) {
        return FALSE;
    }
    return TRUE;
}

void SpNpcNookShop::onInteractionEvent(u32 cmd, u8 arg) {
    CommManager *g;
    s32 a, b;
    switch (cmd) {
    case 3:
        partnerPlayer = arg;
        if (arg != 4) {
            netSetSlotsIfOwner(1, gCommManager->myAid, arg);
            changeAct(0xf);
        } else if (isNetOwner()) {
            u32 t = gCommManager->myAid;
            netSetSlotsIfOwner(1, t, t);
            changeAct(0xf);
        }
        break;
    case 1:
        partnerPlayer = arg;
        if (arg != 4 && arg != gCommManager->myAid) {
            netSetSlotsIfOwner(1, arg, arg);
            changeAct(9);
        } else if (isNetOwner()) {
            g = gCommManager;
            netSetSlotsIfOwner(1, g->myAid, g->myAid);
            talk.resetMsg();
            talk.setTalkPlayer(getPlayerActor(4));
            if (talk.getTopic() == 0 || talk.getTopic() == 0x11) {
                changeAct(4);
            } else if (talk.getTopic() == 1 || talk.getTopic() == 0x12 ||
                       talk.getTopic() == 6 || talk.getTopic() == 0x1d) {
                changeAct(5);
            } else if (((CommManager *)(g))->isOnline() != 0 || *DebugVar_GetPtr(0, 0x4a) != 0) {
                talk.resetMsg();
                talk.setTalkPlayer(getPlayerActor(4));
                changeAct(4);
            } else {
                changeAct(0x10);
            }
        }
        break;
    case 0:
        partnerPlayer = arg;
        if (arg != 4 && arg != gCommManager->myAid) {
            netSetSlotsIfOwner(1, arg, arg);
            changeAct(9);
        } else if (isNetOwner()) {
            netSetSlotsIfOwner(1, gCommManager->myAid, gCommManager->myAid);
            talk.resetMsg();
            talk.setTalkPlayer(getPlayerActor(4));
            if (isNook()) {
                talk.setTopic(5);
            } else {
                talk.setTopic(0x13);
            }
            changeAct(4);
        }
        break;
    case 8:
        if (arg == 4) {
            if (NetArea_IsLocalOwner()) {
                g = gCommManager;
                netSetSlotsIfOwner(1, g->myAid, 4);
                if (talk.getTopic() == 1 || talk.getTopic() == 0x12 ||
                    talk.getTopic() == 6 || talk.getTopic() == 0x1d) {
                } else if (((CommManager *)(g))->isOnline() != 0 || *DebugVar_GetPtr(0, 0x4a) != 0) {
                    changeAct(0xd);
                } else {
                    changeAct(1);
                }
            } else {
                netSetSlotsIfOwner(1, 4, gCommManager->myAid);
                changeAct(8);
            }
        }
        break;
    case 4:
        if (netIsTalkLocked()) {
            if (isNetOwner()) {
                a = 4;
                b = 4;
                if (netGetSlots(&a, &b)) {
                    if ((arg != 4 && (s32)arg == b) || arg == 4) {
                        netSetSlotsIfOwner(1, gCommManager->myAid, 4);
                        changeAct(0xd);
                    }
                }
            }
        }
        break;
    }
}

BOOL SpNpcNookShop::isPlayerCloserThanOtherTwin() {
    if (isTwin()) {
        VecFx32 a;
        VecFx32 *src = (VecFx32 *)PlayerActor_GetBodyPos(4);
        *(VecFx32 *)&a = *src;
        SpNpcNookShop *o = (SpNpcNookShop *)Actor::findByProfile(getOtherTwinProfile(), NULL);
        if (o != 0) {
            VecFx32 b;
            b.x = o->position.x;
            b.y = o->position.y;
            b.z = o->position.z;
            VecFx32 d1, d2;
            Vec_Sub(&d1, &a, &b);
            s32 l1 = VEC_Mag(&d1);
            Vec_Sub(&d2, &a, &position);
            if (VEC_Mag(&d2) < l1) {
                return TRUE;
            }
        }
    }
    return FALSE;
}

s32 SpNpcNookShop::getOtherTwinProfile() {
    if (isTommy()) {
        return 0x75;
    }
    return 0x74;
}

BOOL SpNpcNookShop::pickItemTopic() {
    u16 t[2];
    s32 bx, by;
    VecFx32 v;
    Character *p = (Character *)PlayerActor_GetActor(4);
    BOOL f = Unk_ov050_022590f8_Flags() ? TRUE : FALSE;
    if (p == 0 || TalkRequest_IsActive() != 0 || talkCtrl.isBusy() != 0 || ((gPad[1] & 1) == 0 && f == 0)) {
        return FALSE;
    }
    if (gCommManager->isOnline() != 0 || *DebugVar_GetPtr(0, 0x4a) != 0) {
        if (isTommy()) {
            return FALSE;
        }
    } else if (isTwin()) {
        if (isPlayerCloserThanOtherTwin() == 0) {
            return FALSE;
        }
    }
    selectedItem = 0xfff1;
    VecFx32 *pv = (VecFx32 *)&p->position;
    v.x = p->position.x;
    v.y = pv->y;
    v.z = pv->z;
    u32 ang = p->rotY;
    bx = 0;
    by = 0;
    s32 idx = ((u16)ang >> 4) * 2;
    v.x += func_01ffcb0c(0x2000, data_02135f44[idx]);
    v.z += func_01ffcb0c(0x2000, data_02135f44[idx + 1]);
    FieldPos_ToUnit(&bx, &by, &v);
    if (f) {
        void *o = FtrActorGrid_GetInstance()->getActor(bx, by, 0);
        if (o != 0) {
            if (o != TouchPick_GetTargetObject(Scene_GetTouchPicker(), 0, 0)) {
                return FALSE;
            }
        } else {
            s32 bx2 = 0, by2 = 0;
            VecFx32 v2;
            TouchPick_GetGroundPos(Scene_GetTouchPicker(), &v2);
            FieldPos_ToUnit(&bx2, &by2, &v2);
            if (bx2 != bx || by2 != by) {
                return FALSE;
            }
        }
    }
    t[0] = *ShopStock_GetItemAt(bx, by);
    if (Unk_ov050_022590f8_Eq(&t[0], &t[1])) {
        return FALSE;
    }
    s32 f2 = 0;
    s32 kind = 5;
    if (isTwin()) {
        f2 = 1;
    }
    {
        BOOL r = FALSE;
        volatile u16 *pt = &t[0];
        u32 x = *pt;
        u32 y = *pt;
        if (y < 0x1100 || x > 0x1143) {
        } else {
            r = TRUE;
        }
        if (r != 0 || (x >= 0x1144 && x <= 0x1187)) {
            kind = 0;
        } else if (x >= 0x1000 && x <= 0x10ff) {
            kind = 1;
        } else if (x >= 0x1521 && x <= 0x1530) {
            if (Talk_IsInOwnTown()) {
                kind = 2;
            } else {
                kind = 3;
            }
        } else if (Item_IsClass4Furniture(&t[0])) {
            kind = 4;
        }
    }
    talk.setTopic(sSpNpcNookShopItemTopicTable[kind][f2]);
    selectedItem = t[0];
    selectedItemX = bx;
    selectedItemZ = by;
    purchasePending = 0;
    return TRUE;
}

BOOL SpNpcNookShop::tryItemTalk() {
    if (pickItemTopic()) {
        TalkRequest_AddPlayerTalk6(this, 0);
        return TRUE;
    }
    return FALSE;
}

BOOL SpNpcNookShop::tryStairsBlockTalk() {
    PlayerData *p = PlayerData_GetCurrent();
    p->getErrands();
    if (p->testFlag(1)) {
        SpNpcNookShopVecLocal v;
        VecFx32 *src = (VecFx32 *)PlayerActor_GetBodyPos(4);
        *(VecFx32 *)&v = *src;
        if (v.z < sSpNpcNookShopArbeitStairsBound.z && v.x > sSpNpcNookShopArbeitStairsBound.x) {
            talk.setTopic(0x1e);
            TalkRequest_AddPlayerTalk6(this, 0);
            return TRUE;
        }
    }
    return FALSE;
}

BOOL SpNpcNookShop::tryFarewellTalk() {
    SpNpcNookShopVecLocal v;
    VecFx32 *src = (VecFx32 *)PlayerActor_GetBodyPos(4);
    *(VecFx32 *)&v = *src;
    if (talk.getTopic() != 1 && talk.getTopic() != 0x12) {
        if (isTwin() && isPlayerCloserThanOtherTwin() == 0) {
            if (Ground_IsOnLockedExit(&v)) {
                moveCtrl.setTargetAngle(getAngleToPlayer(4));
            }
            return FALSE;
        }
        if (gCommManager->isOnline() != 0 || *DebugVar_GetPtr(0, 0x4a) != 0) {
            return FALSE;
        }
        PlayerData *p = PlayerData_GetCurrent();
        p->getErrands();
        if (p->testFlag(1)) {
            return FALSE;
        }
    }
    if (Ground_IsOnLockedExit(&v)) {
        if (isTwin()) {
            talk.setTopic(0x12);
        } else {
            talk.setTopic(1);
        }
        TalkRequest_AddPlayerTalk6(this, 0);
        changeAct(0x12);
        return TRUE;
    }
    return FALSE;
}

BOOL SpNpcNookShop::tryClosingTimeTalk() {
    if (TalkRequest_IsActive()) {
        return FALSE;
    }
    if (gCommManager->isOnline() != 0 || *DebugVar_GetPtr(0, 0x4a) != 0) {
        return FALSE;
    }
    if (PlayerData_GetCurrent()->testFlag(1)) {
        return FALSE;
    }
    if (ShopClosingTime_Get()->isPastClosingTime()) {
        if (isNook()) {
            talk.setTopic(6);
        } else if (isTommy()) {
            talk.setTopic(0x1d);
        }
        TalkRequest_AddPlayerTalk6(this, 0);
        return TRUE;
    }
    return FALSE;
}

BOOL SpNpcNookShop::isNook() {
    if (Unk_ov050_02258ea0_Eq(profile, 0x76)) {
        return TRUE;
    }
    return FALSE;
}

BOOL SpNpcNookShop::isTimmy() {
    if (Unk_ov050_02258ea0_Eq(profile, 0x75)) {
        return TRUE;
    }
    return FALSE;
}

BOOL SpNpcNookShop::isTommy() {
    if (Unk_ov050_02258ea0_Eq(profile, 0x74)) {
        return TRUE;
    }
    return FALSE;
}

BOOL SpNpcNookShop::isTwin() {
    if (isTommy() || isTimmy()) {
        return TRUE;
    }
    return FALSE;
}

s32 SpNpcNookShop::getFollowDistance() {
    if (isTwin()) {
        isPlayerCloserThanOtherTwin();
        return 0x3000;
    }
    return 0x3000;
}

// ---------------------------------------------------------------------------------------------------------------------

s32 SpNpcNookShop::getRunDistance() {
    if (isTwin()) {
        isPlayerCloserThanOtherTwin();
        return 0x4000;
    }
    return 0x4000;
}

extern "C" char data_ov050_0225e210[] = "npc_sp/model/rcn.nsbmd";

extern "C" char data_ov050_0225e240[] = "npc_sp/model/rcs.nsbmd";

extern "C" char data_ov050_0225e258[] = "npc_sp/model/rcd.nsbmd";

extern "C" char data_ov050_0225e270[] = "npc_sp/model/lrc.nsbmd";

extern "C" char data_ov050_0225e228[] = "npc_sp/model/rcc.nsbmd";

// ---- overlay variables (generated: tools/host_overlay_vars.py) ----
#include "host_overlay_vars.h"

HOST_OVERLAY_VARS(unk_ov050_02258de0) {
    HOST_OVERLAY_VAR(sSpNpcNookShopDramaKey),
    HOST_OVERLAY_VAR(sSpNpcNookShopTexturePaths),
    HOST_OVERLAY_VAR(sSpNpcNookShopModelPaths),
    HOST_OVERLAY_VAR(sSpNpcNookShopKey),
    HOST_OVERLAY_VAR(SpNpcNookShopTalk_onArbeitMessageEnd_tbl),
    HOST_OVERLAY_VAR(data_ov050_0225e2d0),
    HOST_OVERLAY_VAR(data_ov050_0225e2ec),
    HOST_OVERLAY_VAR(data_ov050_0225e308),
    HOST_OVERLAY_VAR(data_ov050_0225e324),
    HOST_OVERLAY_VAR(data_ov050_0225e340),
    HOST_OVERLAY_VAR(sSpNpcNookShopActTable),
    HOST_OVERLAY_VAR(sSpNpcNookShopTwinsKey),
    HOST_OVERLAY_VAR(sSpNpcNookShopProfile),
    HOST_OVERLAY_VAR(sSpNpcTimmyProfile),
    HOST_OVERLAY_VAR(sSpNpcTommyProfile),
    HOST_OVERLAY_VAR(sSpNpcNookShopSequence5_1Key),
    HOST_OVERLAY_VAR(data_ov050_0225e210),
    HOST_OVERLAY_VAR(data_ov050_0225e240),
    HOST_OVERLAY_VAR(data_ov050_0225e258),
    HOST_OVERLAY_VAR(data_ov050_0225e270),
    HOST_OVERLAY_VAR(data_ov050_0225e228),
    HOST_OVERLAY_VARS_END
};
