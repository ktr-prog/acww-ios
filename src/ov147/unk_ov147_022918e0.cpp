#include "types.h"
#include "host_labels.h"
#include "host_overlay_vars.h"
#include "Unk_020d8c7c.h"
#include "menu/TitleBlinkText.h"
#include "save/SaveData.h"
#include "save/TownExchangeRecord.h"
#include "talk/TalkWindowState.h"
#include "player/PlayerId.h"
#include "talk/MsgString.h"
#include "talk/MsgRequest.h"
#include "item/LetterView.h"
#include "talk/MsgString9B.h"
#include "gfx/BgVramTask.h"
#include "player/PlayerData.h"
#include "talk/ChoiceList.h"
#include "talk/TalkMsgRequest.h"
#include "sys/ProcProfile.h"
#include "player/PlayerOptions.h"
#include "snd/BgmManager.h"
#include "snd/BgmVolumeMixer.h"
#include "gfx/ScreenTransition.h"
#include "proto/main/unk_020b4828.h"
#include "proto/autoload_2/unk_020e7fd4.h"
#include "proto/autoload_2/unk_020e8558.h"
#include "proto/main/unk_0203cbc0.h"
#include "proto/main/unk_020411dc.h"
#include "proto/main/unk_0200137c.h"
#include "proto/main/unk_020039ec.h"
#include "proto/main/unk_02034438.h"
#include "proto/main/unk_0203c92c.h"
#include "proto/main/unk_02063f3c.h"
#include "proto/main/unk_02065e88.h"
#include "proto/main/unk_0206d5b8.h"
#include "proto/main/unk_0208eeac.h"
#include "proto/main/unk_0208f0b0.h"
#include "proto/main/unk_02097444.h"
#include "proto/main/unk_0209eb94.h"
#include "proto/main/unk_020a427c.h"
#include "proto/main/unk_020a8c9c.h"
#include "proto/ov147/unk_ov147_022918e0.h"

struct TitleChoiceSet {
    u8 *msgIds;
    u8 *nextMsgIds;
    u8 numChoices;
};



class TalkMsgRequest;










extern "C" {
void MI_CpuCopy8(void *a, void *b, u32 n);
void MI_CpuFill8(void *a, s32 b, u32 n);
u32 Clock_GetTimeOfDay();

#define data_021e7f8c HOST_LABEL(u8 (*)[], data_021e7f8c)
#define gSavePlayers HOST_LABEL(u8 (*)[], gSavePlayers)
extern SaveData gSaveData;
extern u8 gTalkMsgIndexEnd;
}

class TitleScreen;




typedef void (TitleScreen::*Unk_ov147_022933e8_Fn)();
struct TitleStateEntry {
    Unk_ov147_022933e8_Fn enter;
    Unk_ov147_022933e8_Fn update;
};

// Sub-object at +0x54 of the scene (vtable 0x022934a4, size 0x48)
class TitleTalk : public TalkMsgRequest {
public:
    TitleTalk();
    virtual ~TitleTalk();
    virtual void onMessageEnd(u32 attr);
    virtual void onChoice(u32 attr);
    virtual void onWindowClose();

    void setOwner(void *owner);
    u8 getGreetingMsg();
    void chooseTagMode();
    void chooseResident();
    void chooseNeverMind();
    void chooseContinue();
    void chooseNewGame();
    void openResidentChoices();
    void openChoiceSet(TitleChoiceSet *d);

    /* 0x44 */ TitleScreen *titleScreen;
};


extern "C" {
extern const u8 sTitleGreetingMsgs[];
extern TitleStateEntry sTitleStates[];
extern u16 gPad[];
extern BgmManager *data_021c1b3c;

void TalkRequestFlags_SetTitleTimeout();
BOOL Math_CountDownU16(void *p);
void TalkRequestFlags_ClearSceneHold();
void TalkRequestFlags_SetSceneHold();
s32 TalkRequestFlags_IsTitleTimeout();
void TalkRequestFlags_ClearTitleTimeout();

}

// Data definition order below (and at the end of the file) and the declaration order of vfunc_14's statics
// reproduce the original data/bss order (mwcc heapsorts by size over the reversed creation order).
extern "C" u32 sTitleLogoRevealMasks[15][8] = { 0 };
extern "C" char sTitleTalkFile[] = "sp_etc_sequence1";
extern "C" const u8 sTitleGreetingMsgs[4] = { 0x2d, 0x2e, 0x2f, 0x2f };

static inline BOOL Unk_ov147_02291b28_IsTwo(u8 v) {
    return v == 2 ? TRUE : FALSE;
}

static inline BOOL Unk_ov147_022924c0_IsTwo() {
    if (gScreenTransition.state == 2) {
        return TRUE;
    }
    return FALSE;
}

static inline BOOL Unk_ov147_0229281c_Both() {
    if (gTouchHeld != 0 && gTouchChanged != 0) {
        return TRUE;
    }
    return FALSE;
}

// Vtable 0x022933e8, size 0xf4
class TitleScreen : public GameProc {
public:
    TitleScreen();
    virtual ~TitleScreen();
    virtual BOOL onCreate();
    virtual BOOL onDelete();
    virtual BOOL onExecute();

    void maskLogoCell(u32 *p, s32 x, s32 y);
    void maskLogoChar(u32 *p, s32 i);
    BOOL stepLogoHide();
    void startLogoHide();
    BOOL stepLogoReveal();
    void startLogoReveal();
    void hideLogo();
    void loadLogo();
    void updateWifiSettings();
    void enterWifiSettings();
    void updateTagMode();
    void enterTagMode();
    void updateImmigration();
    void enterImmigration();
    void updateStartGame();
    void enterStartGame();
    void updateContinue();
    void enterContinue();
    void updateEraseTown();
    void enterEraseTown();
    void updateEraseResident();
    void enterEraseResident();
    void updateBackToTitle();
    void enterBackToTitle();
    void updateTalking();
    void enterTalking();
    void updateOpenMenu();
    void enterOpenMenu();
    void updateIdleTimeout();
    void enterIdleTimeout();
    void updateWaitStart();
    void enterWaitStart();
    void changeState(s32 state);
    void holdTalkDuck();
    void stopBgm();
    void startBgm(BOOL flag);
    void dismissLogo();
    void skipLogoReveal();
    void updateLogo();

    /* 0x50 */ s32 titleState;
    /* 0x54 */ TitleTalk talk;
    /* 0x9c */ u8 pad_9c[2];
    /* 0x9e */ u8 backupError;
    /* 0x9f */ u8 bgmMode;
    /* 0xa0 */ s32 logoStep;
    /* 0xa4 */ u16 logoTimer;
    /* 0xa6 */ u8 logoState;
    /* 0xa7 */ u8 skipDelay;
    /* 0xa8 */ u16 idleTimer;
    /* 0xaa */ u8 pad_aa[2];
    /* 0xac */ TitleBlinkText blinkText;
    /* 0xcc */ BgVramTask vramTask;
    /* 0xf0 */ u8 wifiIdErased;
    /* 0xf1 */ u8 pad_f1[3];
};
extern "C" TitleStateEntry sTitleStates[12] = {
    { &TitleScreen::enterWaitStart, &TitleScreen::updateWaitStart },
    { &TitleScreen::enterOpenMenu, &TitleScreen::updateOpenMenu },
    { &TitleScreen::enterTalking, &TitleScreen::updateTalking },
    { &TitleScreen::enterBackToTitle, &TitleScreen::updateBackToTitle },
    { &TitleScreen::enterEraseResident, &TitleScreen::updateEraseResident },
    { &TitleScreen::enterEraseTown, &TitleScreen::updateEraseTown },
    { &TitleScreen::enterContinue, &TitleScreen::updateContinue },
    { &TitleScreen::enterStartGame, &TitleScreen::updateStartGame },
    { &TitleScreen::enterWifiSettings, &TitleScreen::updateWifiSettings },
    { &TitleScreen::enterImmigration, &TitleScreen::updateImmigration },
    { &TitleScreen::enterTagMode, &TitleScreen::updateTagMode },
    { &TitleScreen::enterIdleTimeout, &TitleScreen::updateIdleTimeout },
};

extern "C" TitleScreen *TitleScreen_Create() { return new TitleScreen(); }

TitleScreen::TitleScreen() {}

TitleScreen::~TitleScreen() {}

BOOL TitleScreen::onCreate() {
    talk.setOwner(this);
    PlayerOptions_Get()->reset();
    TalkRequestFlags_SetSceneHold();
    if (Main_TakeDwcInitResult() == 3) {
        wifiIdErased = 1;
    }
    changeState(0);
    logoState = 0;
    skipDelay = 0;
    blinkText.init();
    if (Save_CheckBackupError() == 1) {
        backupError = 1;
    }
    startBgm(TalkRequestFlags_IsTitleTimeout() != 0 ? TRUE : FALSE);
    if (TalkRequestFlags_IsTitleTimeout() != 0) {
        logoState = 6;
        TalkRequestFlags_ClearTitleTimeout();
    }
    return TRUE;
}

BOOL TitleScreen::onDelete() {
    stopBgm();
    blinkText.shutdown();
    vramTask.cancel();
    TalkRequestFlags_ClearSceneHold();
    if (titleState == 7) {
        GameStart_SetupSave();
    }
    return TRUE;
}

BOOL TitleScreen::onExecute() {
    if (sTitleStates[titleState].update) { // (the update member: the DS form's +8 is half an entry on 64-bit hosts)
        (this->*sTitleStates[titleState].update)();
    }
    updateLogo();
    blinkText.update();
    return TRUE;
}

void TitleScreen::startBgm(BOOL flag) {
    if (bgmMode == 0) {
        if (flag) {
            Bgm_RequestSilence(1, 0xf, 0);
            bgmMode = 2;
        } else {
            Bgm_Request(2, 0, 0x7f, 0);
            bgmMode = 1;
        }
    }
}

void TitleScreen::stopBgm() {
    u32 t = bgmMode;
    if (t != 0) {
        if (t == 1) {
            Bgm_Release(0);
        } else if (t == 2) {
            Bgm_ReleasePriority(1);
        }
        Bgm_RequestSilence(1, 0xf, 0xf);
        bgmMode = 0;
    }
}

void TitleScreen::holdTalkDuck() {
    if (bgmMode == 1) {
        data_021c1b3c->mixer.talkDuckHold = 0xb;
    }
}

void TitleScreen::changeState(s32 state) {
    if (sTitleStates[state].enter) {
        (this->*sTitleStates[state].enter)();
    }
    titleState = state;
}

void TitleScreen::enterWaitStart() { idleTimer = 0xe10; }

void TitleScreen::updateWaitStart() {
    if (gPad[1] != 0) {
        InputMode_SetButtons();
    } else if (Unk_ov147_0229281c_Both()) {
        InputMode_SetTouch();
    }
    u32 pad = gPad[1];
    if ((pad & 2) == 0 && (pad & 0x400) == 0 && (pad & 0x800) == 0) {
        if (wifiIdErased != 0 || backupError != 0 || (pad & 8) != 0 || (pad & 1) != 0 || Unk_ov147_0229281c_Both()) {
            if (Unk_ov147_022924c0_IsTwo()) {
                u32 t = logoState;
                if (t != 0) {
                    if (t == 1) {
                        skipLogoReveal();
                        skipDelay = 0xc;
                    } else if (skipDelay == 0) {
                        dismissLogo();
                        blinkText.requestHide();
                        changeState(1);
                    }
                }
            }
        }
    }
    if (skipDelay != 0) {
        skipDelay = *(volatile u8 *)&skipDelay - 1;
        if (skipDelay == 0) {
            blinkText.requestVariant(0);
        }
    }
    if ((u8)(logoState + 0xfc) <= 1) {
        if (Math_CountDownU16(&idleTimer) == 0) {
            blinkText.requestHide();
            changeState(0xb);
        }
    } else {
        idleTimer = 0xe10;
    }
}

void TitleScreen::enterIdleTimeout() {}

void TitleScreen::updateIdleTimeout() {
    if (blinkText.isHidden()) {
        TalkRequestFlags_SetTitleTimeout();
        SceneWarp_RequestFade(Scene_GetWarpRequest(), 0x2c, 2, 2);
        stopBgm();
    }
}

void TitleScreen::enterOpenMenu() {}

void TitleScreen::updateOpenMenu() {
    if (blinkText.isHidden()) {
        TalkWindowState *r = TalkWindow_Get(0);
        talk.resetMsg();
        if (wifiIdErased != 0) {
            talk.setFileName(sTitleTalkFilePtr);
            talk.msgIndex = 0x32;
            wifiIdErased = 0;
        } else if (backupError != 0) {
            talk.setFileName("sp_etc_sequence2");
            talk.msgIndex = 9;
        } else if (gSaveData.testFlag(0x12)) {
            talk.setFileName(sTitleTalkFilePtr);
            talk.msgIndex = 0x24;
        } else {
            talk.setFileName(sTitleTalkFilePtr);
            talk.msgIndex = talk.getGreetingMsg();
        }
        r->attachRequest(&talk);
        r->nextState = 1;
        changeState(2);
    }
}

void TitleScreen::enterTalking() {}

void TitleScreen::updateTalking() {}

void TitleScreen::enterBackToTitle() {}

void TitleScreen::updateBackToTitle() {
    TalkWindowState *r = TalkWindow_Get(0);
    if (r->state == 0) {
        r->detachRequest();
        changeState(0);
        blinkText.requestVariant(1);
    }
}

void TitleScreen::enterEraseResident() { holdTalkDuck(); }

void TitleScreen::updateEraseResident() {
    if (Unk_ov147_022924c0_IsTwo()) {
        TalkWindowState *r = TalkWindow_Get(0);
        if (r->state == 0) {
            r->detachRequest();
            SceneWarp_RequestFade(Scene_GetWarpRequest(), 0x2e, 2, 3);
            SaveManager_RequestAct06();
            stopBgm();
        }
    }
}

void TitleScreen::enterEraseTown() { holdTalkDuck(); }

void TitleScreen::updateEraseTown() {
    if (Unk_ov147_022924c0_IsTwo()) {
        TalkWindowState *r = TalkWindow_Get(0);
        if (r->state == 0) {
            r->detachRequest();
            SceneWarp_RequestFade(Scene_GetWarpRequest(), 0x2e, 2, 3);
            SaveManager_RequestAct05();
            stopBgm();
        }
    }
}

void TitleScreen::enterContinue() { holdTalkDuck(); }

void TitleScreen::updateContinue() {
    TalkWindowState *r = TalkWindow_Get(0);
    if (Unk_ov147_022924c0_IsTwo() && r->state == 0) {
        r->detachRequest();
        SceneWarp_RequestFade(Scene_GetWarpRequest(), 6, 2, 2);
        stopBgm();
    }
}

void TitleScreen::enterStartGame() { holdTalkDuck(); }

void TitleScreen::updateStartGame() {
    if (Unk_ov147_022924c0_IsTwo()) {
        TalkWindowState *r = TalkWindow_Get(0);
        if (r->state == 0) {
            r->detachRequest();
            SceneWarp_RequestFade(Scene_GetWarpRequest(), 0x2d, 2, 0);
            stopBgm();
        }
    }
}

void TitleScreen::enterImmigration() { holdTalkDuck(); }

void TitleScreen::updateImmigration() {
    if (Unk_ov147_022924c0_IsTwo()) {
        TalkWindowState *r = TalkWindow_Get(0);
        if (r->state == 0) {
            r->detachRequest();
            SceneWarp_RequestFade(Scene_GetWarpRequest(), 0x2e, 2, 3);
            SaveManager_RequestAct1C();
            stopBgm();
        }
    }
}

void TitleScreen::enterTagMode() { holdTalkDuck(); }

void TitleScreen::updateTagMode() {
    TalkWindowState *r = TalkWindow_Get(0);
    if (Unk_ov147_022924c0_IsTwo() && r->state == 0) {
        r->detachRequest();
        SceneWarp_RequestFade(Scene_GetWarpRequest(), 0x30, 2, 2);
        stopBgm();
    }
}

void TitleScreen::enterWifiSettings() { holdTalkDuck(); }

void TitleScreen::updateWifiSettings() {
    TalkWindowState *r = TalkWindow_Get(0);
    if (Unk_ov147_022924c0_IsTwo() && r->state == 0) {
        r->detachRequest();
        Scene_Request(2, 2, 0, 0);
        stopBgm();
    }
}

TitleTalk::TitleTalk() {}

TitleTalk::~TitleTalk() {}

// ---- TitleTalk ctor/dtor, TitleScreen (part 2) ----

void TitleTalk::setOwner(void *owner) { titleScreen = (TitleScreen *)owner; }

// TitleTalk::onMessageEnd's static s258
u8 TitleTalk_onMessageEnd_s258[4] = { 0x02, 0x0a, 0x0b, 0x05 };
// TitleTalk::onMessageEnd's static s260, initialised at its first use after the overlay was loaded
u8 TitleTalk_onMessageEnd_s260[4];
u8 TitleTalk_onMessageEnd_s260_init;
// TitleTalk::onMessageEnd's static s250
u8 TitleTalk_onMessageEnd_s250[4] = { 0x00, 0x0a, 0x0b, 0x05 };
// TitleTalk::onMessageEnd's static s268, initialised at its first use after the overlay was loaded
u8 TitleTalk_onMessageEnd_s268[4];
u8 TitleTalk_onMessageEnd_s268_init;
// TitleTalk::onMessageEnd's static s244
u8 TitleTalk_onMessageEnd_s244[2] = { 0x12, 0x13 };
// TitleTalk::onMessageEnd's static s254, initialised at its first use after the overlay was loaded
u8 TitleTalk_onMessageEnd_s254[4];
u8 TitleTalk_onMessageEnd_s254_init;
// TitleTalk::onMessageEnd's static s27c
u8 TitleTalk_onMessageEnd_s27c[5] = { 0x01, 0x02, 0x0a, 0x0b, 0x05 };
// TitleTalk::onMessageEnd's static s284, initialised at its first use after the overlay was loaded
u8 TitleTalk_onMessageEnd_s284[5];
u8 TitleTalk_onMessageEnd_s284_init;
// TitleTalk::onMessageEnd's static s274
u8 TitleTalk_onMessageEnd_s274[4] = { 0x03, 0x18, 0x0e, 0x05 };
// TitleTalk::onMessageEnd's static s264
u8 TitleTalk_onMessageEnd_s264[4] = { 0x09, 0x30, 0x00, 0x31 };
// TitleTalk::onMessageEnd's static s248
u8 TitleTalk_onMessageEnd_s248[3] = { 0x18, 0x0e, 0x05 };
// TitleTalk::onMessageEnd's static s26c
u8 TitleTalk_onMessageEnd_s26c[4] = { 0x01, 0x0a, 0x0b, 0x05 };
// TitleTalk::onMessageEnd's static s25c
u8 TitleTalk_onMessageEnd_s25c[4] = { 0x03, 0x04, 0x0e, 0x05 };
// TitleTalk::onMessageEnd's static s278
u8 TitleTalk_onMessageEnd_s278[4] = { 0x09, 0x03, 0x00, 0x31 };
// TitleTalk::onMessageEnd's static s28c
u8 TitleTalk_onMessageEnd_s28c[5] = { 0x03, 0x04, 0x18, 0x0e, 0x05 };
// TitleTalk::onMessageEnd's static s294
u8 TitleTalk_onMessageEnd_s294[5] = { 0x09, 0x03, 0x30, 0x00, 0x31 };
// TitleTalk::onMessageEnd's static s24c
u8 TitleTalk_onMessageEnd_s24c[3] = { 0x30, 0x00, 0x31 };
// TitleTalk::onMessageEnd's static s240, initialised at its first use after the overlay was loaded
u8 TitleTalk_onMessageEnd_s240[2];
u8 TitleTalk_onMessageEnd_s240_init;
// TitleTalk::onMessageEnd's static descs
TitleChoiceSet TitleTalk_onMessageEnd_descs[9] = {
        { TitleTalk_onMessageEnd_s258, TitleTalk_onMessageEnd_s260, 4 },
        { TitleTalk_onMessageEnd_s250, TitleTalk_onMessageEnd_s268, 4 },
        { TitleTalk_onMessageEnd_s26c, TitleTalk_onMessageEnd_s254, 4 },
        { TitleTalk_onMessageEnd_s27c, TitleTalk_onMessageEnd_s284, 5 },
        { TitleTalk_onMessageEnd_s274, TitleTalk_onMessageEnd_s264, 4 },
        { TitleTalk_onMessageEnd_s248, TitleTalk_onMessageEnd_s24c, 3 },
        { TitleTalk_onMessageEnd_s25c, TitleTalk_onMessageEnd_s278, 4 },
        { TitleTalk_onMessageEnd_s28c, TitleTalk_onMessageEnd_s294, 5 },
        { TitleTalk_onMessageEnd_s244, TitleTalk_onMessageEnd_s240, 2 },
    };
void TitleTalk::onMessageEnd(u32) {
    if (!TitleTalk_onMessageEnd_s260_init) {
        HOST_STATIC_INIT_ARRAY(TitleTalk_onMessageEnd_s260, { 2, 0x37, 1, gTalkMsgIndexEnd });
        TitleTalk_onMessageEnd_s260_init = 1;
    }
    if (!TitleTalk_onMessageEnd_s268_init) {
        HOST_STATIC_INIT_ARRAY(TitleTalk_onMessageEnd_s268, { gTalkMsgIndexEnd, 0x37, 1, gTalkMsgIndexEnd });
        TitleTalk_onMessageEnd_s268_init = 1;
    }
    if (!TitleTalk_onMessageEnd_s254_init) {
        HOST_STATIC_INIT_ARRAY(TitleTalk_onMessageEnd_s254, { gTalkMsgIndexEnd, 0x37, 1, gTalkMsgIndexEnd });
        TitleTalk_onMessageEnd_s254_init = 1;
    }
    if (!TitleTalk_onMessageEnd_s284_init) {
        HOST_STATIC_INIT_ARRAY(TitleTalk_onMessageEnd_s284, { gTalkMsgIndexEnd, 2, 0x37, 1, gTalkMsgIndexEnd });
        TitleTalk_onMessageEnd_s284_init = 1;
    }
    if (!TitleTalk_onMessageEnd_s240_init) {
        HOST_STATIC_INIT_ARRAY(TitleTalk_onMessageEnd_s240, { gTalkMsgIndexEnd, 0x31 });
        TitleTalk_onMessageEnd_s240_init = 1;
    }
    TalkWindowState *r5 = TalkWindow_Get(0);
    s32 r6 = PlayerDataArray_CountUsed(gSavePlayers);
    s32 r0 = gSaveData.isValid();
    TitleScreen *r2 = titleScreen;
    if (r2->backupError != 0) {
        r5->lockAdvance();
        return;
    }
    switch (msgIndex) {
    case 0x2d:
    case 0x2e:
    case 0x2f:
    case 0x31:
        if (r6 <= 0 && r0 != 0) {
            openChoiceSet(&TitleTalk_onMessageEnd_descs[0]);
        } else if (r0 == 0) {
            openChoiceSet(&TitleTalk_onMessageEnd_descs[1]);
        } else if (r6 == 4) {
            openChoiceSet(&TitleTalk_onMessageEnd_descs[2]);
        } else {
            openChoiceSet(&TitleTalk_onMessageEnd_descs[3]);
        }
        break;
    case 1:
        if (r6 <= 0 && r0 != 0) {
            openChoiceSet(&TitleTalk_onMessageEnd_descs[4]);
        } else if (r0 == 0) {
            openChoiceSet(&TitleTalk_onMessageEnd_descs[5]);
        } else if (r6 == 4) {
            openChoiceSet(&TitleTalk_onMessageEnd_descs[6]);
        } else {
            openChoiceSet(&TitleTalk_onMessageEnd_descs[7]);
        }
        break;
    case 0x27:
        GameStart_SetNewResident();
        r5->setSilent();
        titleScreen->changeState(7);
        break;
    case 3:
        openResidentChoices();
        break;
    case 6:
        r5->setNextMessage(&gTalkMsgIndexEnd, 0);
        titleScreen->changeState(4);
        break;
    case 0xb:
        r5->setNextMessage(&gTalkMsgIndexEnd, 0);
        titleScreen->changeState(5);
        break;
    case 5:
    case 0xa: {
        u8 v = 0x31;
        r5->setNextMessage(&v, sTitleTalkFilePtr);
        break;
    }
    case 0x37:
        r2->changeState(3);
        break;
    case 0:
        openChoiceSet(&TitleTalk_onMessageEnd_descs[8]);
        break;
    case 0x30:
        openChoiceSet(&TitleTalk_onMessageEnd_descs[8]);
        break;
    case 0x35:
        openChoiceSet(&TitleTalk_onMessageEnd_descs[8]);
        break;
    case 0x24:
        r5->setSilent();
        r5->setNextMessage(&gTalkMsgIndexEnd, 0);
        GameStart_SetMode3();
        titleScreen->changeState(7);
        break;
    case 0x32:
        r5->setNextMessage(&gTalkMsgIndexEnd, 0);
        titleScreen->changeState(3);
        break;
    }
}

void TitleTalk::openChoiceSet(TitleChoiceSet *d) {
    TalkWindowState *sp0 = window;
    ChoiceList *sp10 = sp0->getChoiceList();
    u8 *r5 = d->msgIds;
    u8 *r6 = d->nextMsgIds;
    u32 r7 = d->numChoices;
#ifdef HOST64
    // (no "Nintendo WFC settings" (choice 0xe): the game connects through the device's own network, set up
    // already - the settings program would need the system to start again)
    u8 keepIds[5], keepNext[5];
    u32 kept = 0;
    for (u32 i = 0; i < r7; i++)
        if (r5[i] != 0x0e) keepIds[kept] = r5[i], keepNext[kept] = r6[i], kept++;
    r5 = keepIds, r6 = keepNext, r7 = kept;
#endif
    sp10->reset(r7, r7 - 1);
    s32 r4;
    s32 z0 = 0;
    for (r4 = 0; r4 < (s32)r7; r4++) {
        s32 f = z0;
        u8 c = r5[r4];
        u8 t[2];
        if (c == 0x12 || c <= 1) {
            f = 1;
        }
        t[0] = c;
        t[1] = r6[r4];
        sp10->setEntry(r4, t, 1, &t[1], NULL, f);
    }
    sp10->loadTexts();
    sp0->openChoices(1);
}

void TitleTalk::openResidentChoices() {
    TalkWindowState *sp0 = window;
    ChoiceList *r6 = sp0->getChoiceList();
    r6->clear();
    s32 r5 = 0;
    s32 r4 = 0;
    u8 buf[3];
    for (r4 = 0; r4 < 4; r4++) {
        if (PlayerDataArray_IsUsed(gSavePlayers, r4)) {
            MsgString9B o;
            ((PlayerId *)((PlayerData *)PlayerData_GetResident(gSavePlayers, r4))->getPlayerId())->getNameString((MsgString *)&o);
            ChoiceEntry *r7 = r6->getEntry(r5);
            buf[0] = 4;
            r7->setValue(&buf[0]);
            ((MsgString *)r7->getText())->copy((MsgString *)&o);
            r5++;
        }
    }
    ChoiceEntry *p = r6->getEntry(r5);
    buf[1] = 0x10;
    p->setMsgIndex(&buf[1]);
    p->setBmgName(Choice_GetBmgName(1));
    buf[2] = 0;
    p->setValue(&buf[2]);
    p->loadText();
    r6->setCount(r5 + 1);
    r6->setCancelToLast();
    sp0->openChoices(1);
}

void TitleTalk::onWindowClose() {
    if (msgIndex == 0x24 || msgIndex == 0x27) {
        Snd_PlaySe(0x3a);
    }
}

typedef void (TitleTalk::*TitleTalk_onChoice_Fn)();
// TitleTalk::onChoice's static t0
TitleTalk_onChoice_Fn TitleTalk_onChoice_t0[4] = { 0, &TitleTalk::chooseTagMode, 0, &TitleTalk::chooseNeverMind };
// TitleTalk::onChoice's static t1
TitleTalk_onChoice_Fn TitleTalk_onChoice_t1[4] = { &TitleTalk::chooseNewGame, &TitleTalk::chooseTagMode, 0, &TitleTalk::chooseNeverMind };
// TitleTalk::onChoice's static t2
TitleTalk_onChoice_Fn TitleTalk_onChoice_t2[4] = { &TitleTalk::chooseContinue, &TitleTalk::chooseTagMode, 0, &TitleTalk::chooseNeverMind };
// TitleTalk::onChoice's static t3
TitleTalk_onChoice_Fn TitleTalk_onChoice_t3[5] = { &TitleTalk::chooseContinue, 0, &TitleTalk::chooseTagMode, 0, &TitleTalk::chooseNeverMind };
void TitleTalk::onChoice(u32) {
    typedef void (TitleTalk::*Fn)();
    TalkWindowState *sp0 = TalkWindow_Get(0);
    s32 r5 = sp0->getChoiceList()->getResult();
    s32 sp4 = PlayerDataArray_CountUsed(gSavePlayers);
    s32 sp8 = gSaveData.isValid();
    static Fn *tbl[4] = { TitleTalk_onChoice_t0, TitleTalk_onChoice_t1, TitleTalk_onChoice_t2, TitleTalk_onChoice_t3 };
    s32 mode = 3;
    switch (msgIndex) {
    case 0:
        if (r5 == 0) {
            sp0->setNextMessage(&gTalkMsgIndexEnd, 0);
            titleScreen->changeState(8);
        }
        break;
    case 0x2d:
    case 0x2e:
    case 0x2f:
    case 0x31:
        if (sp4 <= 0 && sp8 != 0) {
            mode = 0;
        } else if (sp8 == 0) {
            mode = 1;
        } else if (sp4 == 4) {
            mode = 2;
        }
        {
            Fn *row = tbl[mode];
            if (row[r5]) {
                (this->*row[r5])();
            }
        }
        break;
    case 0x30:
        if (r5 == 0) {
            sp0->setNextMessage(&gTalkMsgIndexEnd, 0);
            titleScreen->changeState(9);
        }
        break;
    case 0x32:
    case 0x33:
    case 0x34:
        break;
    case 0x35:
        if (r5 == 0) {
            window->setNextMessage(&gTalkMsgIndexEnd, 0);
            titleScreen->changeState(10);
        }
        break;
    case 3:
        chooseResident();
        break;
    }
}

void TitleTalk::chooseNewGame() {
    GameStart_SetNewTown();
    titleScreen->changeState(7);
}

void TitleTalk::chooseContinue() {
    titleScreen->changeState(6);
}

void TitleTalk::chooseNeverMind() {
    titleScreen->changeState(3);
}

void TitleTalk::chooseResident() {
    TalkWindowState *r7 = TalkWindow_Get(0);
    s32 a = r7->getChoiceList()->getResult();
    u8 r6 = 0x31;
    s32 r5 = 0;
    s32 r4;
    for (r4 = 0; r4 < 4; r4++) {
        if (PlayerDataArray_IsUsed(gSavePlayers, r4)) {
            if (a == r5) {
                SaveManager_SetEraseResidentSlot(r4);
                MsgString9B o;
                ((PlayerId *)((PlayerData *)PlayerData_GetResident(gSavePlayers, r4))->getPlayerId())->getNameString((MsgString *)&o);
                r7->setSlot(0, &o);
                r6 = 4;
            }
            r5++;
        }
    }
    u8 v = r6;
    r7->setNextMessage(&v, sTitleTalkFilePtr);
}

// ---- TitleTalk ----

void TitleTalk::chooseTagMode() {
    u8 *g = data_021e7f8c;
    if (((LetterView *)TownExchange_GetLetter(g))->getState()) {
        if (((TownExchangeRecord *)g)->getCounter() == 0) {
            u8 v = 0x35;
            window->setNextMessage(&v, 0);
        }
    }
}

u8 TitleTalk::getGreetingMsg() {
    return sTitleGreetingMsgs[Clock_GetTimeOfDay()];
}

void TitleScreen::loadLogo() {
    ScreenLayers_AcquireStub();
    Gfx2d_SetLayerControl(5, 0, 0, 0);
    Gfx2d_SetLayerPriority(5, 1);
    Gfx2d_LoadPaletteFile("menu/title/bg_us.bpl", gCurrentHeap, 5, 8, 8, 0xf);
    File_LoadToBuffer("menu/title/bg_us.bsc", sTitleLogoScreen, 0x800);
    Gfx2d_LoadScreen((u8 *)sTitleLogoScreen, 5, 0x800, 0);
    File_LoadToBuffer("menu/title/bg_us.bch", sTitleLogoChars, 0x3800);
    MI_CpuFill8(sTitleLogoCharsWork, 0, 0x3800);
    File_LoadToBuffer("menu/title/mask0.bch", sTitleLogoHideMasks, 0xe0);
    File_LoadToBuffer("menu/title/mask1.bch", sTitleLogoRevealMasks, 0x1e0);
    File_LoadToBuffer("menu/title/mask2.bch", sTitleLogoMask2, 0x1e0);
    Gfx2d_LoadCharRange((u8 *)sTitleLogoCharsWork, 5, 0x140, 0x140, 0x2ff);
}

void TitleScreen::updateLogo() {
    switch (logoState) {
    case 0:
        if (Unk_ov147_02291b28_IsTwo(gScreenTransition.state)) {
            loadLogo();
            startLogoReveal();
        }
        break;
    case 6:
        if (Unk_ov147_02291b28_IsTwo(gScreenTransition.state)) {
            blinkText.requestVariant(1);
            idleTimer = 0xe10;
            logoState = 5;
        }
        break;
    case 1:
        if (stepLogoReveal()) {
            logoState = 2;
            if (skipDelay == 0) {
                blinkText.requestVariant(0);
            }
        }
        break;
    case 2:
        if (logoTimer != 0) {
            logoTimer = *(volatile u16 *)&logoTimer - 1;
        } else {
            startLogoHide();
            blinkText.requestVariant(1);
        }
        break;
    case 3:
        if (stepLogoHide()) {
            logoState = 4;
        }
        break;
    case 4:
    case 5:
        break;
    }
}

void TitleScreen::skipLogoReveal() {
    if (logoState == 1) {
        logoStep = 0x64;
    }
}

void TitleScreen::dismissLogo() {
    if (logoState == 2) {
        startLogoHide();
    } else {
        hideLogo();
    }
}

void TitleScreen::hideLogo() {
    Gfx2d_HideLayer(5);
    ScreenLayers_ReleaseStub();
    logoState = 5;
}

void TitleScreen::startLogoReveal() {
    logoState = 1;
    logoStep = 0;
    logoTimer = 0x4b0;
    MI_CpuFill8(sTitleLogoCharsWork, 0, 0x3800);
    Gfx2d_ShowLayer(5);
}

BOOL TitleScreen::stepLogoReveal() {
    s32 y;
    s32 r4 = logoStep;
    if (r4 < 0x36) {
        r4 += 9;
        s32 r6 = 0;
        for (; r6 < 0xf && r4 >= 9; r6++, r4--) {
            u32 *p = sTitleLogoRevealMasks[r6];
            s32 x = r4;
            y = 0;
            if (r4 > 0x1f) {
                y = r4 - 0x1f;
                x = 0x1f;
            }
            for (; x >= 0 && y < 0x18; x--, y++) {
                if (y < 0x15) {
                    maskLogoCell(p, x, y);
                }
            }
        }
        vramTask.requestChars(sTitleLogoCharsWork, 5, 0x140, 0x140, 0x2ff);
        logoStep = logoStep + 1;
        goto ret0;
    }
    MI_CpuCopy8(sTitleLogoChars, sTitleLogoCharsWork, 0x3800);
    vramTask.requestChars(sTitleLogoCharsWork, 5, 0x140, 0x140, 0x2ff);
    return TRUE;
ret0:
    return FALSE;
}

void TitleScreen::startLogoHide() {
    logoStep = 7;
    logoState = 3;
}

BOOL TitleScreen::stepLogoHide() {
    if (logoStep == 0) {
        Gfx2d_HideLayer(5);
        ScreenLayers_ReleaseStub();
        return TRUE;
    }
    logoStep = logoStep - 1;
    u32 *p = sTitleLogoHideMasks[logoStep];
    s32 i;
    for (i = 0; i < 0x1c0; i++) {
        maskLogoChar(p, i);
    }
    vramTask.requestChars(sTitleLogoCharsWork, 5, 0x140, 0x140, 0x2ff);
    return FALSE;
}

void TitleScreen::maskLogoChar(u32 *p, s32 i) {
    u32 *src = sTitleLogoChars[i];
    u32 *dst = sTitleLogoCharsWork[i];
    s32 j;
    for (j = 0; j < 8; j++) {
        *dst++ = *src & *p;
        p++;
        src++;
    }
}

// ---- TitleScreen (part 1) ----

void TitleScreen::maskLogoCell(u32 *p, s32 x, s32 y) {
    s32 v = sTitleLogoScreen[x + (y << 5)] & 0x3ff;
    if (v != 0x10 && v >= 0x140) {
        maskLogoChar(p, v - 0x140);
    }
}

extern "C" u32 sTitleLogoHideMasks[7][8] = { 0 };
extern "C" u16 sTitleLogoScreen[0x400] = { 0 };
extern "C" u8 sTitleLogoMask2[0x1e0] = { 0 };
extern "C" ProcProfile sTitleScreenProfile = { (void *(*)())TitleScreen_Create, 0xd4, 0xcf };
extern "C" char *sTitleTalkFilePtr = sTitleTalkFile;
extern "C" u32 sTitleLogoCharsWork[0x1c0][8] = { 0 };
extern "C" u32 sTitleLogoChars[0x1c0][8] = { 0 };

// ---- overlay variables (generated: tools/host_overlay_vars.py) ----
#include "host_overlay_vars.h"

HOST_OVERLAY_VARS(unk_ov147_022918e0) {
    HOST_OVERLAY_VAR(sTitleLogoRevealMasks),
    HOST_OVERLAY_VAR(sTitleTalkFile),
    HOST_OVERLAY_VAR(sTitleStates),
    HOST_OVERLAY_VAR(TitleTalk_onMessageEnd_s258),
    HOST_OVERLAY_VAR(TitleTalk_onMessageEnd_s260),
    HOST_OVERLAY_VAR(TitleTalk_onMessageEnd_s260_init),
    HOST_OVERLAY_VAR(TitleTalk_onMessageEnd_s250),
    HOST_OVERLAY_VAR(TitleTalk_onMessageEnd_s268),
    HOST_OVERLAY_VAR(TitleTalk_onMessageEnd_s268_init),
    HOST_OVERLAY_VAR(TitleTalk_onMessageEnd_s244),
    HOST_OVERLAY_VAR(TitleTalk_onMessageEnd_s254),
    HOST_OVERLAY_VAR(TitleTalk_onMessageEnd_s254_init),
    HOST_OVERLAY_VAR(TitleTalk_onMessageEnd_s27c),
    HOST_OVERLAY_VAR(TitleTalk_onMessageEnd_s284),
    HOST_OVERLAY_VAR(TitleTalk_onMessageEnd_s284_init),
    HOST_OVERLAY_VAR(TitleTalk_onMessageEnd_s274),
    HOST_OVERLAY_VAR(TitleTalk_onMessageEnd_s264),
    HOST_OVERLAY_VAR(TitleTalk_onMessageEnd_s248),
    HOST_OVERLAY_VAR(TitleTalk_onMessageEnd_s26c),
    HOST_OVERLAY_VAR(TitleTalk_onMessageEnd_s25c),
    HOST_OVERLAY_VAR(TitleTalk_onMessageEnd_s278),
    HOST_OVERLAY_VAR(TitleTalk_onMessageEnd_s28c),
    HOST_OVERLAY_VAR(TitleTalk_onMessageEnd_s294),
    HOST_OVERLAY_VAR(TitleTalk_onMessageEnd_s24c),
    HOST_OVERLAY_VAR(TitleTalk_onMessageEnd_s240),
    HOST_OVERLAY_VAR(TitleTalk_onMessageEnd_s240_init),
    HOST_OVERLAY_VAR(TitleTalk_onMessageEnd_descs),
    HOST_OVERLAY_VAR(TitleTalk_onChoice_t0),
    HOST_OVERLAY_VAR(TitleTalk_onChoice_t1),
    HOST_OVERLAY_VAR(TitleTalk_onChoice_t2),
    HOST_OVERLAY_VAR(TitleTalk_onChoice_t3),
    HOST_OVERLAY_VAR(sTitleLogoHideMasks),
    HOST_OVERLAY_VAR(sTitleLogoScreen),
    HOST_OVERLAY_VAR(sTitleLogoMask2),
    HOST_OVERLAY_VAR(sTitleScreenProfile),
    HOST_OVERLAY_VAR(sTitleTalkFilePtr),
    HOST_OVERLAY_VAR(sTitleLogoCharsWork),
    HOST_OVERLAY_VAR(sTitleLogoChars),
    HOST_OVERLAY_VARS_END
};
