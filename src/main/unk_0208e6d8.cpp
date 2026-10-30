#include "uptr.h"
extern void *const gSpriteSeqTable5b0c[71];  // (src/main/unk_020d5d34.cpp)
#include "types.h"
#include "Unk_020d8c7c.h"
#include "net/CommManager.h"
#include "gfx/SpriteAnim.h"
#include "ui/UiWidget.h"
#include "ui/TalkBusyIcon.h"
#include "ui/InputModeIcon.h"
#include "sys/ProcProfile.h"
#include "proto/itcm/unk_01ffa4a0.h"
#include "proto/main/unk_0200137c.h"
#include "proto/main/unk_020039ec.h"
#include "proto/main/unk_02087e70.h"
#include "proto/main/unk_0208e6d8.h"
#include "proto/main/unk_020d5d0c.h"
#include "proto/main/unk_020d5d34.h"

extern "C" {
s32 TalkRequestFlags_IsSceneHold();
extern u32 data_020d5b0c[][2];
}

extern const u8 sCommIconShowDelays[4];
extern const u8 sCommIconHideDelays[4];
extern const u16 sBusyIconSe[2];


extern "C" ProcBase *gActorDefaultParent;

extern "C" CommManager *gCommManager;

struct SpriteAnimSeq;




class TransitionCommIcon : public UiWidget {
public:
    TransitionCommIcon();
    virtual ~TransitionCommIcon();
    virtual void draw();
    virtual void update();
    void updateShown();
    void setupAnim();
    void enterShown();
    void updateHidden();
    void enterHidden();
    void callDraw();
    void callUpdate();
    void exit();
    void init();

    /* 0x0c */ SpriteAnim anim;
    /* 0x20 */ s32 state;
    /* 0x24 */ s32 showDelay;
    /* 0x28 */ s32 hideDelay;
    /* 0x2c */ s32 blinkPhase;
    /* 0x30 */ u64 blinkDeadline;
    /* 0x38 */ u8 blinkOn;
    /* 0x39 */ u8 suspended;
};

class TransitionCommIconProc : public GameProc {
public:
    TransitionCommIconProc();
    virtual ~TransitionCommIconProc();
    virtual BOOL onCreate();
    virtual BOOL onDelete();
    virtual BOOL onExecute();
    virtual BOOL onDraw();
};




extern "C" {
}

typedef void (TransitionCommIcon::*Unk_020e10f8_Fn)();

typedef void (TalkBusyIcon::*Unk_020e10dc_Fn)();


BOOL InputModeIcon::isDrawBlocked() {
    BOOL r = FALSE;
    if (TalkRequestFlags_IsSceneHold()) {
        r = TRUE;
    }
    return r;
}

void InputModeIcon::init() {}

void InputModeIcon::exit() {}

void InputModeIcon::startModeAnim() {
    // (port: buttons and touch are taken at the same time, sys/InputReplay.h - no mode to announce: the icon's
    // pop-up on each change stays hidden)
    if (1) {
        isVisible = 0;
        return;
    }
    if (inputMode == 0) {
        isVisible = 0;
    } else {
        s32 i;
        if (inputMode == 1) {
            i = 0x43;
        } else {
            i = 0x44;
        }
        anim.setSeq((SpriteAnimSeq *)gSpriteSeqTable5b0c[i]);  // (the block of tables: src/main/unk_020d5d34.cpp)
        anim.setPlayOnce(1);
        anim.restart();
        isVisible = 1;
    }
}

void TalkBusyIcon::requestShow(u32 v) {
    showRequested = 1;
    seIndex = v;
}

void TalkBusyIcon::requestHide() {
    showRequested = 0;
}

TalkBusyIcon::TalkBusyIcon() {
    state = 0;
    rotation = 0;
    objWindow = 0;
    showRequested = 0;
    posX = 0;
    posY = 0;
    seIndex = 0;
    sePlaying = 0;
}

TalkBusyIcon::~TalkBusyIcon() {
    stopSe();
}

void TalkBusyIcon::draw() {
    if (state != 0) {
        s32 x = posX + getOriginX();
        s32 y = posY + getOriginY();
        // the first cells of the spinner and of its frame (the DS read them as words 0 and 4 from data_020d5d0c)
        GXOamAttr *h0 = data_020d5d0c[0].frames[0].cell;
        GXOamAttr *h1 = data_020d5d14[1].frames[0].cell;
        Oam_DrawCell(0, h0, x, y, -1, -1, 0x1000, 0x1000, rotation, -1, 0, 0);
        Oam_DrawCell(0, h1, x, y, -1, -1, 0x1000, 0x1000, 0, -1, 0, 0);
        if (objWindow != 0) {
            Oam_DrawCell(0, h0, x, y, -1, -1, 0x1000, 0x1000, rotation, 2, 0, 0);
            Oam_DrawCell(0, h1, x, y, -1, -1, 0x1000, 0x1000, 0, 2, 0, 0);
        }
    }
}

void TalkBusyIcon::update() {
    static Unk_020e10dc_Fn tbl[2] = {&TalkBusyIcon::updateHidden, &TalkBusyIcon::updateShown};
    rotation += 0x1111;
    (this->*tbl[state])();
}

void TalkBusyIcon::init() { enterHidden(); }

void TalkBusyIcon::exit() {}

void TalkBusyIcon::callUpdate() { update(); }

// ---- TalkBusyIcon ----
void TalkBusyIcon::callDraw() { draw(); }

void TalkBusyIcon::setPos(s32 a, s32 b) {
    posX = a;
    posY = b;
}

void TalkBusyIcon::enterHidden() { state = 0; }

void TalkBusyIcon::updateHidden() {
    if (showRequested != 0) {
        enterShown();
    }
}

void TalkBusyIcon::enterShown() {
    state = 1;
    startSe();
    if (objWindow != 0) {
        Gfx2d_SetMainObjWinPlanes(0x10);
        Gfx2d_EnableMainWindows(4);
    }
}

void TalkBusyIcon::updateShown() {
    if (showRequested == 0) {
        stopSe();
        if (objWindow != 0) {
            Gfx2d_DisableMainWindows(4);
        }
        enterHidden();
    }
}

void TalkBusyIcon::startSe() {
    u16 v = sBusyIconSe[seIndex];
    sePlaying = 1;
    Snd_PlaySeOnHandle(v);
}

void TalkBusyIcon::stopSe() {
    u16 v = sBusyIconSe[seIndex];
    if (sePlaying != 0) {
        sePlaying = 0;
        Snd_StopSe(v, 1);
    }
}

TransitionCommIcon::TransitionCommIcon() : state(0), showDelay(0), hideDelay(0), blinkPhase(0), blinkDeadline(0), blinkOn(0), suspended(0) {}

TransitionCommIcon::~TransitionCommIcon() {}

void TransitionCommIcon::draw() {
    if (blinkOn != 0) {
        if (suspended == 0) {
            s32 x = getOriginX();
            s32 y = getOriginY();
            GXOamAttr *h = anim.getCell();
            Oam_DrawCell(0, h, x, y, -1, -1, 0x1000, 0x1000, 0, -1, 0, 0);
            Oam_DrawCell(0, h, x, y, -1, -1, 0x1000, 0x1000, 0, 2, 0, 0);
        }
    }
}

const u16 sBusyIconSe[2] = {4, 5};
const u8 sCommIconShowDelays[4] = {8, 8, 8, 1};
const u8 sCommIconHideDelays[4] = {5, 5, 5, 1};

void TransitionCommIcon::update() {
    static Unk_020e10f8_Fn tbl[2] = {&TransitionCommIcon::updateHidden, &TransitionCommIcon::updateShown};
    (this->*tbl[state])();
}

extern "C" void TransitionCommIcon_RequestShow(u32 i) {
    u32 t = gActorDefaultParent->profile;
    BOOL e = gCommManager->isOnline();
    if (t != 5 && e) {
        sTransitionCommIcon.showDelay = sCommIconShowDelays[i];
    }
}

extern "C" void TransitionCommIcon_RequestHide(u32 i) {
    if (gActorDefaultParent->profile != 5) {
        sTransitionCommIcon.hideDelay = sCommIconHideDelays[i];
    }
}

extern "C" void TransitionCommIcon_Resume() {
    if (sTransitionCommIcon.state != 0) {
        Gfx2d_SetMainObjWinPlanes(0x10);
        Gfx2d_EnableMainWindows(4);
    }
    sTransitionCommIcon.suspended = 0;
}

extern "C" void TransitionCommIcon_ResumeWinOut() {
    if (sTransitionCommIcon.state != 0) {
        Gfx2d_SetMainObjWinPlanes(0x10);
        Gfx2d_SetMainWinOutPlanes(4);
        Gfx2d_EnableMainWindows(4);
    }
    sTransitionCommIcon.suspended = 0;
}

extern "C" void TransitionCommIcon_Suspend() { sTransitionCommIcon.suspended = 1; }

extern "C" void TransitionCommIcon_Init() { sTransitionCommIcon.init(); }

extern "C" void TransitionCommIcon_Exit() { sTransitionCommIcon.exit(); }

extern "C" void TransitionCommIcon_Update() { sTransitionCommIcon.callUpdate(); }

extern "C" void TransitionCommIcon_Draw() { sTransitionCommIcon.callDraw(); }

void TransitionCommIcon::init() {
    setupAnim();
    showDelay = 0;
    hideDelay = 0;
    blinkPhase = 0;
    blinkDeadline = 0;
    blinkOn = 0;
    suspended = 0;
}

void TransitionCommIcon::exit() { enterHidden(); }

void TransitionCommIcon::callUpdate() { update(); }

// ---- TransitionCommIcon ----
void TransitionCommIcon::callDraw() { draw(); }

void TransitionCommIcon::enterHidden() {
    state = 0;
    showDelay = 0;
    blinkOn = 0;
}

void TransitionCommIcon::updateHidden() {
    if (showDelay > 0) {
        showDelay--;
        if (showDelay <= 0) {
            enterShown();
        }
    }
}

void TransitionCommIcon::enterShown() {
    state = 1;
    hideDelay = 0;
    Gfx2d_SetMainObjWinPlanes(0x10);
    Gfx2d_EnableMainWindows(4);
    u64 t = OS_GetTick();
    blinkPhase = 1;
    blinkDeadline = t + 0x1991b;
    blinkOn = 1;
}

void TransitionCommIcon::updateShown() {
    u64 now = OS_GetTick();
    if (now >= blinkDeadline) {
        if (blinkPhase == 0) {
            blinkDeadline = now + 0x1991b;
            blinkOn = 1;
            blinkPhase = 1;
        } else if (blinkPhase == 1) {
            blinkDeadline = now + 0x1991b;
            blinkOn = 0;
            blinkPhase = 2;
        } else if (blinkPhase == 2) {
            blinkDeadline = now + 0x1991b;
            blinkOn = 1;
            blinkPhase = 3;
        } else if (blinkPhase == 3) {
            blinkDeadline = now + 0x4cb51;
            blinkOn = 0;
            blinkPhase = 0;
        }
    }
    if (hideDelay > 0) {
        hideDelay--;
        if (hideDelay <= 0) {
            Gfx2d_DisableMainWindows(4);
            enterHidden();
        }
    }
}

void TransitionCommIcon::setupAnim() {
    anim.setSeq((SpriteAnimSeq *)data_020d5d34);
    anim.setPlayOnce(1);
    anim.restart();
}

extern "C" TransitionCommIconProc *TransitionCommIconProc_Create() { return new TransitionCommIconProc(); }

TransitionCommIconProc::TransitionCommIconProc() {}

TransitionCommIconProc::~TransitionCommIconProc() {}

BOOL TransitionCommIconProc::onCreate() {
    TransitionCommIcon_Init();
    return TRUE;
}

BOOL TransitionCommIconProc::onDelete() {
    TransitionCommIcon_Exit();
    return TRUE;
}

BOOL TransitionCommIconProc::onExecute() {
    TransitionCommIcon_Update();
    return TRUE;
}

BOOL TransitionCommIconProc::onDraw() {
    TransitionCommIcon_Draw();
    return TRUE;
}


TransitionCommIcon sTransitionCommIcon;
ProcProfile sTransitionCommIconProfile = {(void *(*)())TransitionCommIconProc_Create, 0xcc, 0xc8};

#ifdef HOST64
static_assert(sizeof(TalkBusyIcon) == 40, "TalkFrame::busyIcon in src/main/unk_02065e88.cpp");
#endif
