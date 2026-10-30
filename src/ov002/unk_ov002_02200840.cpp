// ov002: shared library overlay (menu / cursor / slider helpers used by the scene overlays).
#include "ds_div.h"
#include "host_overlay_vars.h"
#include "types.h"
#include "sys/InputReplay.h"
#include "Unk_020d8c7c.h"
#include "text/Unk_02050288.h"
#include "talk/MsgStringBase.h"
#include "talk/MsgStringAttr.h"
#include "ui/CursorMotion.h"
#include "sys/KeyRepeat.h"
#include "menu/MenuTween.h"
#include "menu/PopupChoiceIdList.h"
#include "ui/AddresseePageArg.h"
#include "nitro/gxoam.h"
#include "talk/TalkWindowState.h"
#include "gfx/SpriteAnim.h"
#include "ui/UiWidget.h"
#include "talk/LabelBalloonText.h"
#include "menu/MenuSlide.h"
#include "talk/MsgString.h"
#include "talk/MsgRequest.h"
#include "menu/MenuProc.h"
#include "ui/HandCursor.h"
#include "ui/ScrollKnob.h"
#include "ui/LabelButton.h"
#include "ui/LabelString.h"
#include "ui/LabelBalloon.h"
#include "ui/TouchPromptBalloon.h"
#include "menu/MenuTextButton.h"
#include "menu/MenuTitleBalloon.h"
#include "menu/MenuLabelButton.h"
#include "menu/MenuCursor.h"
#include "menu/MenuScrollKnob.h"
#include "menu/PopupChoiceRow.h"
#include "menu/MenuBottomButtons.h"
#include "menu/PopupChoiceMenu.h"
#include "talk/TalkMsgRequest.h"
#include "menu/MenuErrorMessage.h"
#include "proto/autoload_2/unk_020e7fd4.h"
#include "proto/autoload_2/unk_020e8558.h"
#include "proto/autoload_2/unk_020ec848.h"
#include "proto/autoload_2/unk_02128030.h"
#include "proto/main/unk_0200137c.h"
#include "proto/main/unk_020039ec.h"
#include "proto/main/unk_0205b7b0.h"
#include "proto/main/unk_02063904.h"
#include "proto/main/unk_02063f3c.h"
#include "proto/main/unk_02065330.h"
#include "proto/main/unk_02065e88.h"
#include "proto/main/unk_0206db88.h"
#include "proto/main/unk_0206e54c.h"
#include "proto/main/unk_0206f834.h"
#include "proto/main/unk_02087e70.h"
#include "proto/main/unk_02097444.h"
#include "proto/main/unk_020b2c50.h"
#include "proto/main/unk_020b7d84.h"
#include "proto/ov002/unk_ov002_02200840.h"
#include "proto/ov002/unk_ov002_022013ac.h"
#include "proto/ov002/unk_ov002_022025cc.h"
#include "proto/ov002/unk_ov002_02202fac.h"

// ---------------------------------------------------------------------------------------------------------------------

extern "C" {
s32 func_01ffcb0c(s32 a, s32 b);

extern volatile u16 gPad[];
}


// ---------------------------------------------------------------------------------------------------------------------
// Overlay 2 classes

class MenuProc;
typedef void (MenuProc::*Unk_ov002_02200a68_Fn)();

static inline BOOL Unk_ov002_022009d4_Both() {
    if (gTouchHeld != 0 && gTouchChanged != 0) {
        return TRUE;
    }
    return FALSE;
}

KeyRepeat::KeyRepeat() {}

KeyRepeat::~KeyRepeat() {}

void KeyRepeat::update()
{
    u32 prev = heldKeys;
    heldKeys = gPad[0] & 0xf0;
    u8 *p = &pendingKeys;
    pendingKeys |= (u8)(gPad[1] & 0xf0);
    u32 cur = heldKeys;
    if (cur == 0 || prev != cur) {
        pendingKeys = gPad[1] & 0xf0;
        interval = delay;
        countdown = delay;
    } else if (countdown > 0) {
        countdown--;
    } else {
        *p |= cur & 0xf0;
        interval = interval - intervalStep;
        if (interval < minInterval) {
            interval = minInterval;
        }
        countdown = interval;
    }
}

u8 KeyRepeat::take()
{
    takenKeys = pendingKeys;
    pendingKeys = 0;
    return takenKeys;
}

BOOL KeyRepeat::isUp()
{
    if (takenKeys & 0x40) {
        return TRUE;
    }
    return FALSE;
}

BOOL KeyRepeat::isDown()
{
    if (takenKeys & 0x80) {
        return TRUE;
    }
    return FALSE;
}

BOOL KeyRepeat::isLeft()
{
    if (takenKeys & 0x20) {
        return TRUE;
    }
    return FALSE;
}

BOOL KeyRepeat::isRight()
{
    if (takenKeys & 0x10) {
        return TRUE;
    }
    return FALSE;
}

extern "C" BOOL MenuKeys_HasUp(u32 v)
{
    if (v & 0x40) {
        return TRUE;
    }
    return FALSE;
}

extern "C" BOOL MenuKeys_HasDown(u32 v)
{
    if (v & 0x80) {
        return TRUE;
    }
    return FALSE;
}

extern "C" BOOL MenuKeys_HasLeft(u32 v)
{
    if (v & 0x20) {
        return TRUE;
    }
    return FALSE;
}

extern "C" BOOL MenuKeys_HasRight(u32 v)
{
    if (v & 0x10) {
        return TRUE;
    }
    return FALSE;
}

void KeyRepeat::init(s32 a, s32 b, s32 c)
{
    heldKeys = 0;
    pendingKeys = 0;
    takenKeys = 0;
    interval = 0;
    countdown = 0;
    delay = a;
    minInterval = b;
    intervalStep = c;
}

MenuTween::MenuTween() {}

MenuTween::~MenuTween() {}

void MenuTween::start(u32 n)
{
    progress = 0x1000;
    stepSize = DS_UDIV(0x1000, n);
}

BOOL MenuTween::step()
{
    s32 a = progress;
    if (a == 0) {
        return TRUE;
    }
    s32 b = stepSize;
    if (a > b) {
        progress = a - b;
    } else {
        progress = 0;
    }
    return FALSE;
}

s32 MenuTween::scaleQuadratic(s32 v)
{
    return (v * func_01ffcb0c(progress, progress)) >> 12;
}

s32 MenuTween::scaleLinear(s32 v)
{
    return (v * progress) >> 12;
}

MenuSlide::MenuSlide() {}

MenuSlide::~MenuSlide() {}

s32 MenuSlide::getOffsetY()
{
    switch (direction) {
    case 0:
        return offset;
    case 1:
        return -offset;
    }
    return 0;
}

s32 MenuSlide::getOffsetX()
{
    switch (direction) {
    case 2:
        return -offset;
    case 3:
        return offset;
    }
    return 0;
}

void MenuSlide::updateSlideInVertical(s32 mode)
{
    offset = scaleQuadratic(extent);
    switch (mode) {
    case 0:
        if (offset > edgeDistance) {
            ((MenuSlideView *)this)->applyWindow(2);
        } else {
            Gfx2d_DisableSubWindows(1);
            Gfx2d_SetSubWinOutPlanes(0x1f);
        }
        break;
    case 1:
        if (offset > edgeDistance) {
            ((MenuSlideView *)this)->applyWindow(0);
        } else {
            Gfx2d_DisableMainWindows(1);
            Gfx2d_SetMainWinOutPlanes(0x1f);
        }
        break;
    }
}

void MenuSlide::updateSlideInHorizontal(s32 mode)
{
    offset = scaleQuadratic(extent);
    switch (mode) {
    case 0:
        ((MenuSlideView *)this)->applyWindow(2);
        break;
    case 1:
        ((MenuSlideView *)this)->applyWindow(0);
        break;
    }
}

BOOL MenuSlide::stepSlideIn(s32 mode)
{
    if (step()) {
        offset = 0;
        switch (mode) {
        case 0:
            Gfx2d_DisableSubWindows(1);
            Gfx2d_SetSubWinOutPlanes(0x1f);
            break;
        case 1:
            Gfx2d_DisableMainWindows(1);
            Gfx2d_SetMainWinOutPlanes(0x1f);
            break;
        }
        return TRUE;
    }
    switch (direction) {
    case 0:
    case 1:
        updateSlideInVertical(mode);
        break;
    default:
        updateSlideInHorizontal(mode);
        break;
    }
    return FALSE;
}

void MenuSlide::updateSlideOutVertical(s32 mode)
{
    offset = extent - scaleLinear(extent);
    if (offset > edgeDistance) {
        switch (mode) {
        case 0:
            Gfx2d_EnableSubWindows(1);
            ((MenuSlideView *)this)->applyWindow(2);
            break;
        case 1:
            Gfx2d_EnableMainWindows(1);
            ((MenuSlideView *)this)->applyWindow(0);
            break;
        }
    }
}

// ---------------------------------------------------------------- MenuProc

void MenuSlide::updateSlideOutHorizontal(s32 mode)
{
    offset = extent - scaleLinear(extent);
    switch (mode) {
    case 0:
        ((MenuSlideView *)this)->applyWindow(2);
        break;
    case 1:
        ((MenuSlideView *)this)->applyWindow(0);
        break;
    }
}

BOOL MenuSlideView::stepSlideOut(s32 a) {
    if (step()) {
        switch (a) {
        case 0:
            Gfx2d_DisableSubWindows(1);
            break;
        case 1:
            Gfx2d_DisableMainWindows(1);
            break;
        }
        return TRUE;
    }
    switch (direction) {
    case 0:
    case 1:
        updateSlideOutVertical(a);
        break;
    default:
        updateSlideOutHorizontal(a);
        break;
    }
    return FALSE;
}

void MenuSlideView::beginSubSlideIn(s32 a, s32 b, s32 mode, s32 dist) {
    initSlideIn(b, mode, dist);
    Gfx2d_EnableSubWindows(1);
    Gfx2d_SetSubWin0Planes(0x1f, 1);
    Gfx2d_SetSubWinOutPlanes(~a & 0x1f);
    applyWindow(2);
}

void MenuSlideView::beginSubSlideOut(s32 a, s32 b, s32 mode, s32 dist) {
    initSlideOut(b, mode, dist);
    Gfx2d_EnableSubWindows(1);
    Gfx2d_SetSubWin0Planes(0x1f, 1);
    Gfx2d_SetSubWinOutPlanes(~a & 0x1f);
    applyWindow(2);
}

void MenuSlideView::beginMainSlideIn(s32 a, s32 b, s32 mode, s32 dist) {
    initSlideIn(b, mode, dist);
    Gfx2d_EnableMainWindows(1);
    Gfx2d_SetMainWin0Planes(0x1f);
    Gfx2d_SetMainWinOutPlanes(~a & 0x1f);
    applyWindow(0);
}

void MenuSlideView::beginMainSlideOut(s32 a, s32 b, s32 mode, s32 dist) {
    initSlideOut(b, mode, dist);
    Gfx2d_EnableMainWindows(1);
    Gfx2d_SetMainWin0Planes(0x1f);
    if (Gfx2d_GetMainWindows() & 2) {
        Gfx2d_RemoveMainWinOutPlanes(a);
    } else {
        Gfx2d_SetMainWinOutPlanes(~a & 0x1f);
    }
    applyWindow(0);
}

void MenuSlideView::initSlideIn(s32 a, s32 mode, s32 dist) {
    direction = mode;
    edgeDistance = dist;
    start(a);
    switch (direction) {
    case 0:
    case 1:
        offset = 0xc0;
        extent = 0xc0;
        break;
    case 2:
    case 3:
        offset = 0x100;
        extent = 0x100;
        break;
    }
}

void MenuSlideView::initSlideOut(s32 a, s32 mode, s32 dist) {
    direction = mode;
    edgeDistance = dist;
    start(a);
    offset = 0;
    switch (direction) {
    case 0:
    case 1:
        extent = 0xc0;
        break;
    case 2:
    case 3:
        extent = 0x100;
        break;
    }
}

void MenuSlideView::applyLayerOffset(s32 a, s32 b, s32 c) {
    switch (direction) {
    case 0:
        Gfx2d_SetLayerOffset(a, -b, -(offset + c));
        break;
    case 1:
        Gfx2d_SetLayerOffset(a, -b, offset - c);
        break;
    case 2:
        Gfx2d_SetLayerOffset(a, offset - b, -c);
        break;
    case 3:
        Gfx2d_SetLayerOffset(a, -(offset + b), -c);
        break;
    }
}

void MenuSlideView::applyWindow(s32 a) {
    s32 x0 = 0;
    s32 y0 = 0;
    s32 x1 = 0xff;
    s32 y1 = 0xc0;
    switch (direction) {
    case 0:
        y0 = offset - edgeDistance;
        if (y0 < 0) {
            y0 = x0;
        }
        break;
    case 1:
        y1 = edgeDistance + 0xc0 - offset;
        if (y1 > 0xbf) {
            y1 = 0xbf;
        }
        break;
    case 2:
        x1 = 0xff - offset;
        if (x1 < 1) {
            x1 = 1;
        }
        break;
    case 3:
        x0 = offset;
        if (x0 > 0xfe) {
            x0 = 0xfe;
        }
        break;
    }
    Gfx2d_SetWindowRect(a, x0, y0, x1, y1);
}

// ---------------------------------------------------------------- MenuSlideView

void MenuSlideView::setExtent(s32 v) { extent = v; }

MenuProc::MenuProc() {
    openMenuLink.prev = 0;
    openMenuLink.next = 0;
}

MenuProc::~MenuProc() {}

BOOL MenuProc::preCreate() {
    if (!ProcBase::preCreate()) {
        return FALSE;
    }
    menuId = (s32)param;
    openMenuOwner = this;
    keyRepeat.init(8, 1, 7);
    return TRUE;
}

void MenuProc::postCreate(s32 a) {
    MenuCtrl_AddOpenMenu(&openMenuLink);
    if (a == 2) {
        ProcBase *p = ProcBase_GetParent(this);
        if (p != 0) {
            ProcBase_SetExecutePriority(this, (u16)(p->executeNode.priority + 1));
            ProcBase_SetDrawPriority(this, (u16)(p->drawNode.priority + 1));
        }
    }
    GameProc::postCreate(a);
}

BOOL MenuProc::preDelete() {
    if (ProcBase::preDelete()) {
        return TRUE;
    }
    return FALSE;
}

BOOL MenuProc::postDelete(s32 a) {
    if (a == 2) {
        MenuCtrl_RemoveOpenMenu(&openMenuLink);
    }
    return ProcBase::postDelete(a);
}

BOOL MenuProc::preExecute() {
    if (ProcBase::preExecute()) {
        return TRUE;
    }
    return FALSE;
}

BOOL MenuProc::postExecute(u32 status) { return ProcBase::postExecute(status); }

BOOL MenuProc::execWaitScreen() {
    if (MenuScreen_IsOpen()) {
        setPhase(1);
    }
    if (MenuScreen_IsClosed()) {
        MenuScreen_ClearState();
        setPhase(5);
    }
    return TRUE;
}

BOOL MenuProc::execTransition() { return TRUE; }

BOOL MenuProc::execMain() { return TRUE; }

BOOL MenuProc::execPhase3() { return TRUE; }

BOOL MenuProc::execPhase4() { return TRUE; }

BOOL MenuProc::execClosed() { return TRUE; }

// MenuProc::onExecute's static tbl, initialised at its first use after the overlay was loaded
Unk_ov002_02200a68_Fn MenuProc_onExecute_tbl[6];
u8 MenuProc_onExecute_tbl_init;
BOOL MenuProc::onExecute() {
    if (!MenuProc_onExecute_tbl_init) {
        HOST_STATIC_INIT_ARRAY(MenuProc_onExecute_tbl, {
        (Unk_ov002_02200a68_Fn)&MenuProc::execWaitScreen,
        (Unk_ov002_02200a68_Fn)&MenuProc::execTransition,
        (Unk_ov002_02200a68_Fn)&MenuProc::execMain,
        (Unk_ov002_02200a68_Fn)&MenuProc::execPhase3,
        (Unk_ov002_02200a68_Fn)&MenuProc::execPhase4,
        (Unk_ov002_02200a68_Fn)&MenuProc::execClosed,
    });
        MenuProc_onExecute_tbl_init = 1;
    }
    keyRepeat.update();
    (this->*MenuProc_onExecute_tbl[phase])();
    return TRUE;
}

void MenuProc::setPhase(u8 v) { phase = v; }

void MenuProc::setMainState(u8 v) { mainState = v; }

void MenuProc::setTransitionState(u8 v) { transitionState = v; }

// (port: buttons and touch at the same time, sys/InputReplay.h: no switch sound, and the press acts next frame)
BOOL MenuProc::checkSwitchToButtons(s32 a) {
    if ((gPad[1] & 0xfff) != 0) {
        MenuCtrl_SetButtons();
        Input_ReplayKeys(gPad[1] & 0xfff);
        (void)a;
        return TRUE;
    }
    return FALSE;
}

BOOL MenuProc::checkSwitchToTouch() {
    if (Unk_ov002_022009d4_Both()) {
        MenuCtrl_SetTouch();
        Input_ReplayTouch();
        return TRUE;
    }
    return FALSE;
}

u8 MenuProc::takeRepeatedKeys() { return keyRepeat.take(); }

BOOL MenuProc::isRepeatUp() { return keyRepeat.isUp(); }

BOOL MenuProc::isRepeatDown() { return keyRepeat.isDown(); }

BOOL MenuProc::isRepeatLeft() { return keyRepeat.isLeft(); }

BOOL MenuProc::isRepeatRight() { return keyRepeat.isRight(); }

void MenuProc::restartKeyRepeat() { keyRepeat.init(8, 1, 7); }

void MenuProc::initKeyRepeat(s32 a, s32 b, s32 c) { keyRepeat.init(a, b, c); }

void *MenuProc::operator new(size_t size) {
    void *p = Heap_AllocTail(gMenuHeap, size);
    if (p == 0) {
        return 0;
    }
    memset(p, 0, size);
    return p;
}

void MenuProc::operator delete(void *p) { Heap_Free(gMenuHeap, p); }

s32 MenuProc::getSlideOffsetY() { return ((MenuSlideView *)&slide)->getOffsetY(); }

s32 MenuProc::getSlideOffsetX() { return ((MenuSlideView *)&slide)->getOffsetX(); }

BOOL MenuProc::stepSlideIn(s32 a) { return ((MenuSlideView *)&slide)->stepSlideIn(a); }

BOOL MenuProc::stepSlideOut(s32 a) { return ((MenuSlideView *)&slide)->stepSlideOut(a); }

void MenuProc::beginSubSlideIn(s32 a, s32 b, s32 mode, s32 dist) {
    if (b == 0) {
        b = 4;
    }
    ((MenuSlideView *)&slide)->beginSubSlideIn(a, b, mode, dist);
}

void MenuProc::beginSubSlideOut(s32 a, s32 b, s32 mode, s32 dist) {
    if (b == 0) {
        b = 3;
    }
    ((MenuSlideView *)&slide)->beginSubSlideOut(a, b, mode, dist);
}

void MenuProc::beginMainSlideIn(s32 a, s32 b, s32 mode, s32 dist) {
    if (b == 0) {
        b = 4;
    }
    ((MenuSlideView *)&slide)->beginMainSlideIn(a, b, mode, dist);
}

void MenuProc::beginMainSlideOut(s32 a, s32 b, s32 mode, s32 dist) {
    if (b == 0) {
        b = 3;
    }
    ((MenuSlideView *)&slide)->beginMainSlideOut(a, b, mode, dist);
}

void MenuProc::initSlideIn(s32 a, s32 mode) {
    if (a == 0) {
        a = 4;
    }
    ((MenuSlideView *)&slide)->initSlideIn(a, mode, 0x30);
}

void MenuProc::initSlideOut(s32 a, s32 mode) {
    if (a == 0) {
        a = 3;
    }
    ((MenuSlideView *)&slide)->initSlideOut(a, mode, 0x30);
}

void MenuProc::setSlideExtent(s32 v) { ((MenuSlideView *)&slide)->setExtent(v); }

void MenuProc::applySlideOffset(s32 a, s32 b, s32 c) { ((MenuSlideView *)&slide)->applyLayerOffset(a, b, c); }

// ---- overlay variables (generated: tools/host_overlay_vars.py) ----
#include "host_overlay_vars.h"

HOST_OVERLAY_VARS(unk_ov002_02200840) {
    HOST_OVERLAY_VAR(MenuProc_onExecute_tbl),
    HOST_OVERLAY_VAR(MenuProc_onExecute_tbl_init),
    HOST_OVERLAY_VARS_END
};
