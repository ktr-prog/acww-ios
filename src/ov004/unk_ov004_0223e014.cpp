// mwcc-version: 1.2/base
#include "types.h"
#include "sys/InputReplay.h"
#include "host_labels.h"
#include "proto/main/unk_020b4828.h"
#include "gfx/VecFx32.h"
#include "Unk_020d8c7c.h"
#include "ui/UiWidget.h"
#include "ui/HandCursor.h"
#include "ui/NameLabelBalloon.h"
#include "talk/MsgString9B.h"
#include "sys/ProcProfile.h"
#include "net/CommManager.h"
#include "room/RoomTelephone.h"
#include "player/PlayerData.h"
#include "player/PlayerId.h"
#include "save/SaveData.h"
#include "gfx/ScreenTransition.h"
#include "proto/autoload_2/unk_020e7fd4.h"
#include "proto/main/unk_020039ec.h"
#include "proto/main/unk_0206e54c.h"
#include "proto/main/unk_0208eeac.h"
#include "proto/main/unk_02094810.h"
#include "proto/main/unk_02097444.h"
#include "proto/main/unk_0209eb94.h"
#include "proto/main/unk_020411dc.h"
#include "proto/main/unk_020943dc.h"
#include "proto/main/unk_020b5e5c.h"
#include "proto/main/unk_020b2c50.h"
#include "proto/main/unk_020b7d10.h"
#include "proto/ov004/unk_ov004_0221e7a8.h"
#include "proto/ov004/unk_ov004_02229660.h"
#include "proto/ov004/unk_ov004_0223e014.h"

// ---- sub-object declarations (defined in src/main/unk_0208d154.cpp etc.) ----




// gPad (the same PadState as autoload_2 unk_020e7500.cpp)
struct PadState {
    u16 cur;
    u16 trig;
    s16 dir;
};

extern "C" {
#define gSavePlayers HOST_LABEL(u8 (*)[], gSavePlayers)
extern PadState gPad;
extern CommManager *gCommManager;
extern const Vec2 sResidentLabelOffsets[];
extern const Vec2 sResidentExtraLabelPos;
extern const s32 sResidentCursorExtraOffset[2];
extern const s32 sResidentCursorOffsets[4][2];

BOOL Camera_ProjectCurvedToScreen(s32 *x, s32 *y, VecFx32 *p);
void func_02094018(void *p);
void func_02094030(void *);
BOOL Math_CountDownU16(void *p);
void Clock_GetDateTime(void *p);
s32 DateTime_Compare(void *a, void *b, s32 n);
void TalkRequestFlags_ClearSceneHold(void);
void TalkRequestFlags_SetSceneHold(void);
void RoomCamera_StartBlendToPlayer(void);
}
extern SaveData gSaveData;

class ResidentSelect;
typedef void (ResidentSelect::*Unk_ov004_0224f20c_Fn)();
struct ResidentSelectStateEntry {
    Unk_ov004_0224f20c_Fn enter;
    Unk_ov004_0224f20c_Fn update;
};


static inline BOOL Unk_ov004_0223e2f4_IsMode2() {
    if (gScreenTransition.state == 2) {
        return TRUE;
    }
    return FALSE;
}

class ResidentSelect : public GameProc {
public:
    ResidentSelect();
    virtual BOOL onCreate();
    virtual BOOL onDelete();
    virtual BOOL onExecute();
    virtual BOOL onDraw();
    virtual ~ResidentSelect();

    void updateNameLabels();
    void updateLeave();
    void enterLeave();
    void updateSave();
    void enterSave();
    void updateCameraMove();
    void enterCameraMove();
    void updateDecided();
    void enterDecided();
    void updatePadSelect();
    void enterPadSelect();
    void updateTouchSelect();
    void enterTouchSelect();
    void updateWait();
    void enterWait();
    void changeState(s32 state);

    /* 0x050 */ HandCursor cursor;
    /* 0x09c */ NameLabelBalloon nameLabels[5];
    /* 0x31c */ u8 selectedResident;
    /* 0x31d */ u8 isPhoneSelected;
    /* 0x31e */ u8 pad_31e[2];
    /* 0x320 */ s32 selectState;
    /* 0x324 */ u16 timer;
    /* 0x326 */ u8 pad_326[2];
    /* 0x328 */ s32 startDateTime;
    /* 0x32c */ s32 unk_32c;
    /* 0x330 */ s32 cursorX;
    /* 0x334 */ s32 cursorY;
    /* 0x338 */ MsgString9B phoneLabel;
};

ProcProfile sResidentSelectProfile = {(void *(*)())ResidentSelect_Create, 0xd3, 0xce};

// state table (enter, update) filled by __sinit from the 14 member-function-pointer constants
ResidentSelectStateEntry sResidentSelectStates[7] = {
    {&ResidentSelect::enterWait, &ResidentSelect::updateWait}, {&ResidentSelect::enterTouchSelect, &ResidentSelect::updateTouchSelect}, {&ResidentSelect::enterPadSelect, &ResidentSelect::updatePadSelect}, {&ResidentSelect::enterDecided, &ResidentSelect::updateDecided},
    {&ResidentSelect::enterCameraMove, &ResidentSelect::updateCameraMove}, {&ResidentSelect::enterSave, &ResidentSelect::updateSave}, {&ResidentSelect::enterLeave, &ResidentSelect::updateLeave},
};

static inline BOOL Unk_ov004_0223e2f4_Both47() {
    if (gTouchHeld != 0 && gTouchChanged != 0) {
        return TRUE;
    }
    return FALSE;
}

static inline BOOL Unk_ov004_0223e580_BothEf() {
    if (gTouchPrevHeld != 0 && gTouchPrevChanged != 0) {
        return TRUE;
    }
    return FALSE;
}

extern "C" ResidentSelect *ResidentSelect_Create() {
    return new ResidentSelect;
}

ResidentSelect::ResidentSelect() : cursor(1), startDateTime(0), unk_32c(0) {
}

ResidentSelect::~ResidentSelect() {
}

BOOL ResidentSelect::onCreate() {
    SaveData *g;
    u8 c;
    u8 i;
    TalkRequestFlags_SetSceneHold();
    g = &gSaveData;
    for (i = 0; i < 4; i++) {
        void *o = PlayerData_GetResident(g->players, i);
        if (o != 0 && ((PlayerData *)(o))->isUsed() != 0) {
            selectedResident = i;
            break;
        }
    }
    for (i = 0; i < 5; i++) {
        nameLabels[i].setKind(i);
    }
    c = 0x81;
    String_Load2d(&phoneLabel, &c, 0);
    if (g->testFlag(0) == 0) {
        changeState(0);
    } else if (InputMode_IsButtons()) {
        changeState(2);
    } else {
        changeState(1);
    }
    isPhoneSelected = 0;
    Clock_GetDateTime(&startDateTime);
    return TRUE;
}

BOOL ResidentSelect::onDelete() {
    TalkRequestFlags_ClearSceneHold();
    u8 i;
    for (i = 0; i < 5; i++) {
        nameLabels[i].release();
    }
    if (MenuCtrl_IsClockEdited()) {
        s32 l[2];
        MenuCtrl_ClearClockChangeFlags();
        l[0] = 0;
        l[1] = 0;
        Clock_GetDateTime(l);
        if (DateTime_Compare(&startDateTime, l, 0x3f) == -1) {
            MenuCtrl_SetClockMovedForward();
        } else {
            MenuCtrl_SetClockMovedBack();
        }
    }
    return TRUE;
}

BOOL ResidentSelect::onExecute() {
    ResidentSelect_UpdateCursor(this);
    updateNameLabels();
    if (sResidentSelectStates[selectState].update) {
        (this->*sResidentSelectStates[selectState].update)();
    }
    return TRUE;
}

BOOL ResidentSelect::onDraw() {
    cursor.draw();
    u8 i;
    for (i = 0; i < 5; i++) {
        NameLabelBalloon *e = &nameLabels[i];
        e->draw();
    }
    return TRUE;
}

void ResidentSelect::changeState(s32 state) {
    if (sResidentSelectStates[state].enter) {
        (this->*sResidentSelectStates[state].enter)();
    }
    selectState = state;
}

void ResidentSelect::enterWait() {
    cursor.setAnim(0);
    u8 i;
    for (i = 0; i < 5; i++) {
        nameLabels[i].requestHide();
    }
}

void ResidentSelect::updateWait() {
    if (!RoomTelephone_IsTalking()) {
        if (InputMode_IsButtons()) {
            changeState(2);
        } else {
            changeState(1);
        }
    }
}

void ResidentSelect::enterTouchSelect() {
    cursor.setAnim(0);
    u8 i;
    for (i = 0; i < 5; i++) {
        nameLabels[i].requestShow();
    }
}

void ResidentSelect::updateTouchSelect() {
    u8 c;
    s32 a;
    VecFx32 out;
    if (!Unk_ov004_0223e2f4_IsMode2()) {
        return;
    }
    if (InputMode_IsTouch() && Unk_ov004_0223e580_BothEf()) {
        if (TouchPickResult_GetTarget(Scene_GetTouchPicker(), &out, &a, &c) && a == 1) {
            gCommManager->localSlot = c;
            selectedResident = c;
            changeState(3);
            return;
        }
    }
    if (RoomTelephone_IsTalking()) {
        changeState(0);
    } else if (gPad.trig & 0xff3) {
        InputMode_SetButtons();
        Input_ReplayKeys(gPad.trig & 0xff3);  // (port: sys/InputReplay.h)
        changeState(2);
    }
}

void ResidentSelect::enterPadSelect() {
    cursor.setAnim(7);
    u8 i;
    for (i = 0; i < 5; i++) {
        nameLabels[i].requestShow();
    }
}

void ResidentSelect::updatePadSelect() {
    CommManager *g;
    u8 st;
    if (!Unk_ov004_0223e2f4_IsMode2()) {
        return;
    }
    g = gCommManager;
    if (g->localSlot != 4) {
        changeState(3);
        return;
    }
    if (RoomTelephone_IsTalking()) {
        changeState(0);
        return;
    }
    if (Unk_ov004_0223e2f4_Both47()) {
        InputMode_SetTouch();
        Input_ReplayTouch();  // (port: sys/InputReplay.h)
        changeState(1);
        return;
    }
    if (isPhoneSelected != 0) {
        u16 k = gPad.trig;
        if (k & 0x80) {
            isPhoneSelected = 0;
            goto L500;
        }
        {
            u32 up = k & 0x10;
            if (up == 0 && (k & 0x20) == 0) {
                goto L500;
            }
            st = selectedResident;
            if (up != 0) {
                st = 1;
            } else if (k & 0x20) {
                st = 0;
            }
        }
        if (PlayerActor_GetCharacter(st) == 0) {
            goto L500;
        }
        isPhoneSelected = 0;
        selectedResident = st;
        goto L500;
    }
    st = selectedResident;
    switch (st) {
    case 0: {
        u16 k = gPad.trig;
        if (k & 0x10) {
            st = st + 1;
        } else if (k & 0x80) {
            st = st + 2;
            if (!PlayerActor_GetCharacter(st)) {
                st = st + 1;
            }
        } else if (k & 0x40) {
            isPhoneSelected = 1;
        }
        break;
    }
    case 1: {
        u16 k = gPad.trig;
        if (k & 0x20) {
            st = st - 1;
        } else if (k & 0x80) {
            st = st + 2;
            if (!PlayerActor_GetCharacter(st)) {
                st = st - 1;
            }
        } else if (k & 0x40) {
            isPhoneSelected = 1;
        }
        break;
    }
    case 2: {
        u16 k = gPad.trig;
        if (k & 0x10) {
            st = st + 1;
        } else if (k & 0x40) {
            st = st - 2;
            if (!PlayerActor_GetCharacter(st)) {
                st = st + 1;
                if (!PlayerActor_GetCharacter(st)) {
                    isPhoneSelected = 1;
                }
            }
        }
        break;
    }
    case 3: {
        u16 k = gPad.trig;
        if (k & 0x20) {
            st = st - 1;
        } else if (k & 0x40) {
            st = st - 2;
            if (!PlayerActor_GetCharacter(st)) {
                st = st - 1;
                if (!PlayerActor_GetCharacter(st)) {
                    isPhoneSelected = 1;
                }
            }
        }
        break;
    }
    }
    if (PlayerActor_GetCharacter(st) != 0) {
        selectedResident = st;
    }
L500:
    {
        u16 k = gPad.trig;
        if ((k & 8) || (k & 1)) {
            if (isPhoneSelected != 0) {
                RoomTelephone_StartAct0A();
            } else {
                g->localSlot = selectedResident;
                changeState(3);
            }
        }
    }
}

void ResidentSelect::enterDecided() {
    BOOL r = FALSE;
    u8 v = selectedResident;
    if (v == 0 || v == 2) {
        r = TRUE;
    }
    PlayerActor_LocalRequestGetOutOfBed(r, 1);
    cursor.setAnim(0);
    u8 i;
    for (i = 0; i < 5; i++) {
        nameLabels[i].requestHide();
    }
    timer = 0x19;
}

void ResidentSelect::updateDecided() {
    if (!Math_CountDownU16(&timer)) {
        changeState(4);
    }
}

void ResidentSelect::enterCameraMove() {
    RoomCamera_StartBlendToPlayer();
    PlayerActor_RequestTurnTo(0, 4);
    timer = 5;
}

void ResidentSelect::updateCameraMove() {
    if (!Math_CountDownU16(&timer)) {
        changeState(5);
    }
}

void ResidentSelect::enterSave() {
    SaveManager_RequestAct03();
}

void ResidentSelect::updateSave() {
    if (SaveManager_IsIdleForRoom()) {
        changeState(6);
    }
}

void ResidentSelect::enterLeave() {
    VecFx32Ctor v;
    VecFx32 *p = (VecFx32 *)PlayerActor_GetBodyPos(4);
    v.x = p->x;
    v.y = p->y;
    v.z = p->z;
    v.z = v.z + 0x6000;
    PlayerActor_RequestWalkTo(&v, 0x2b8, 4);
}

void ResidentSelect::updateLeave() {
}

void ResidentSelect::updateNameLabels() {
    u8 i;
    for (i = 0; i < 4; i++) {
        if (PlayerDataArray_IsUsed(gSavePlayers, i) != 0) {
            PlayerActor *a = (PlayerActor *)PlayerActor_GetCharacter(i);
            if (a != 0) {
                s32 x, y;
                VecFx32 v;
                VecFx32 *pv = &a->position;
                v.x = a->position.x;
                v.y = pv->y;
                v.z = pv->z;
                MsgString9B o;
                ((PlayerId *)(((PlayerData *)(PlayerData_GetResident(gSavePlayers, i)))->getPlayerId()))->getNameString((MsgString *)(&o));
                Camera_ProjectCurvedToScreen(&x, &y, &v);
                x += sResidentLabelOffsets[i].x;
                y += sResidentLabelOffsets[i].y;
                NameLabelBalloon *e = &nameLabels[i];
                e->setOffset(x, y);
                e->setText(&o);
                e->update();
            }
        }
    }
    nameLabels[4].setOffset(sResidentExtraLabelPos.x, sResidentExtraLabelPos.y);
    nameLabels[4].setText(&phoneLabel);
    NameLabelBalloon *e4 = &nameLabels[4];
    e4->update();
}

extern "C" void ResidentSelect_UpdateCursor(ResidentSelect *o) {
    VecFx32CtorDtor z;
    s32 xy[2];
    VecFx32 *pp;
    z.x = 0;
    z.y = 0;
    z.z = 0;
    if (o->isPhoneSelected != 0) {
        pp = RoomTelephone_GetInstance()->getInteractionPos();
    } else {
        pp = &((PlayerActor *)PlayerActor_GetCharacter(o->selectedResident))->position;
    }
    Camera_ProjectCurvedToScreen(&xy[0], &xy[1], pp);
    if (o->isPhoneSelected == 0) {
        xy[0] = xy[0] + sResidentCursorOffsets[o->selectedResident][0];
        xy[1] = xy[1] + sResidentCursorOffsets[o->selectedResident][1];
    } else {
        xy[0] = xy[0] + sResidentCursorExtraOffset[0];
        xy[1] = xy[1] + sResidentCursorExtraOffset[1];
    }
    BOOL t;
    if (gScreenTransition.state == 2) {
        t = TRUE;
    } else {
        t = FALSE;
    }
    if (t) {
        if (o->selectState == 2) {
            if (o->cursorX != xy[0] || o->cursorY != xy[1]) {
                Snd_PlaySe(0xb);
            }
        }
    }
    o->cursor.setPos(xy[0], xy[1]);
    o->cursor.update();
    o->cursorX = xy[0];
    o->cursorY = xy[1];
}

// ---- rodata (defined after the functions so that the compiler cannot fold the loads) ----
const s32 sResidentCursorExtraOffset[2] = {-6, -14};
const s32 sResidentCursorOffsets[4][2] = {{-15, -12}, {-8, -12}, {-19, -14}, {-11, -14}};
const Vec2 sResidentExtraLabelPos = {15, -92};
const Vec2 sResidentLabelOffsets[4] = {{-5, -5}, {5, -5}, {-5, 0}, {5, 0}};

// ---- overlay variables (generated: tools/host_overlay_vars.py) ----
#include "host_overlay_vars.h"

HOST_OVERLAY_VARS(unk_ov004_0223e014) {
    HOST_OVERLAY_VAR(sResidentSelectProfile),
    HOST_OVERLAY_VAR(sResidentSelectStates),
    HOST_OVERLAY_VARS_END
};
