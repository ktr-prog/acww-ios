#include "ds_div.h"
#include "host_labels.h"
#include "snd/SndSeEmitter.h"
#include "save/VillagerData.h"
#include "uptr.h"
#include "types.h"
#include "gfx/VecFx32.h"
#include "Unk_020d8c7c.h"
#include "game/FxVec3.h"
#include "actor/Character.h"
#include "sys/ProcProfile.h"
#include "proto/main/unk_020039ec.h"
#include "proto/main/unk_020571f4.h"
#include "proto/main/unk_02090268.h"
#include "proto/ov004/unk_ov004_0222bed0.h"
#include "proto/main/unk_0204cc1c.h"


struct FishHoldOffsets {
    u8 pad_00[0xc];
    u8 altHoldOffset[0xc];
};



class HandOverItem;
typedef void (HandOverItem::*Unk_020dc034_Fn)();

struct HandOverItemAct {
    Unk_020dc034_Fn a;
    Unk_020dc034_Fn b;
};


class HandOverItem : public GameProc {
public:
    HandOverItem() { item = 0xfff1; }

    /* 0x50 */ u16 item;
    /* 0x54 */ s32 kind;
    /* 0x58 */ u8 nextMode;
    /* 0x5c */ s32 variant;
    /* 0x60 */ VecFx32 itemPos;
    /* 0x6c */ VecFx32 target;
    /* 0x78 */ VecFx32 worldOffset;
    /* 0x84 */ VecFx32 offset;
    /* 0x90 */ VecFx32 moveStep;
    /* 0x9c */ VecFx32 basePos;
    /* 0xa8 */ VecFx32 scale;
    /* 0xb4 */ s16 itemRot[3];
    u8 pad_ba[2];
    /* 0xbc */ s32 reach;
    /* 0xc0 */ s32 scaleStep;
    /* 0xc4 */ s32 fishDisplay;
    /* 0xc8 */ u8 mode;
    u8 pad_c9[3];
    /* 0xcc */ Character *chars[2];
    /* 0xd4 */ u8 phase;
    /* 0xd5 */ u8 takeTimer;
    /* 0xd6 */ volatile u16 frame;
    /* 0xd8 */ u8 visible;
    /* 0xd9 */ u8 busy;
    /* 0xda */ u8 modeRequest;
    u8 pad_db;
    /* 0xdc */ SndSeEmitter seEmitter;

    virtual BOOL onCreate();
    virtual BOOL onDelete();
    virtual BOOL onExecute();
    virtual BOOL onDraw();

    void updateAct09V1(void);
    void act09V1Phase2(void);
    void act09V1Phase1(void);
    void act09V1Phase0(void);
    void startAct09V1(void);
    void updateAct07V1(void);
    void startAct07V1(void);
    void updateAct05V1(void);

    void startAct05V1();
    void updateAct04V1();
    void startAct04V1();
    void updateAct03V1();
    void startAct03V1();
    void updateAct02V1();
    void startAct02V1();
    void updateAct01V1();
    void act01V1Phase2();
    void act01V1Phase1();
    void act01V1Phase0();
    void startAct01V1();
    void updateAct0B();
    void act0BPhase1();
    void act0BPhase0();

    void startAct0B();
    void updateAct08();
    void startAct08();
    void updateAct07();
    void startAct07();
    void updateAct06();
    void startAct06();
    void updateAct05();
    void startAct05();
    void updateAct0A();
    void startAct0A();
    void updateAct04();
    void startAct04();
    void updateAct03();
    void startAct03();
    void startAct02();
    void updateAct01();
    void act01Phase1();
    void act01Phase0();
    void startAct01();

    VecFx32 getHoldOffset(u32 idx);
    void localToWorld(VecFx32 *out, VecFx32 *in, u32 idx);
    BOOL setNextModeBy(u8 v, void *p);
    void setBusy(u32 v);
    void getMasterHoldPos(VecFx32 *out);
    void drawItem();
    BOOL isAwaitingTake();
    BOOL requestModeBy(u8 v, void *p);
    void setModeRequest(u8 v);
    void clearModeRequest();
    BOOL switchMaster(void *p);
    BOOL isCharAt(void *p, u32 idx);
    BOOL begin(u16 *id, s32 a, u8 b, s32 c, Character *o0, Character *o1);
    void resetState();
};

static inline s32 Unk_020574cc_Abs(s32 v) {
    return v < 0 ? -v : v;
}

static inline BOOL Unk_020586bc_Range(u16 *p, u32 lo, u32 hi)
{
    BOOL r = FALSE;
    if (*p >= lo && *p <= hi) r = TRUE;
    return r;
}

static inline BOOL Unk_02058ddc_IsZero(u8 v) { return v == 0 ? TRUE : FALSE; }

// ---- externals ----
extern "C" {
extern u8 gFieldSceneKind[];
#define gSaveVillagers HOST_LABEL(u8 (*)[], gSaveVillagers)

void FishDisplay_Release(s32 a);
s32 Vec_DistXZ(void *a, void *b);
void Math_StepS32(s32 *p, s32 target, s32 step);
void Vec_RotateY(VecFx32 *v, s32 angle);
void VEC_Add(VecFx32 *a, VecFx32 *b, VecFx32 *out);
s32 Scene_GetCurrent();
void Vec_Sub(VecFx32 *out, VecFx32 *a, VecFx32 *b);
void Math_StepS32Alt(s32 *p, s32 a, s32 b);
s32 func_01ffcb0c(s32 a, s32 b);
s32 func_02133150(s32 a, s32 b);
s32 Fish_GetSizeClass(s32 a);
void Field_DrawIconModel(u32 id, VecFx32Copy *a, VecFx32Copy *b, s16 x, s16 y, s16 z);
void Field_DrawItemIcon(u32 id, VecFx32Copy *a, VecFx32Copy *b, s16 x, s16 y, s16 z);
s32 FishDisplay_SetEntry(s32 c4, s32 idx, VecFx32 *p, VecFx32 *q, s32 a0, s32 a1, s32 a2, s32 z0, s32 z1, s32 k);
BOOL Item_IsFurniture(u16 *p);
s32 Item_GetFurnitureIndex(u16 *p);
s32 FishDisplay_Acquire();
void Math_CountDownU8(void *p);
VillagerData *SaveVillagers_Get(void *base, s32 idx);
u32 Villager_GetAnimalKind(VillagerData *a);

// ---- own data (defined below / after the functions) ----
extern const u8 sHandOverTakeTimeout[4];
extern const u8 data_020ca67c[4];
extern const u8 data_020ca680[4];
extern const u8 data_020ca684[4];
extern const s32 sHandOverReachFar;
extern const s32 sHandOverReach;
extern const u8 data_020ca690[4];
extern const VecFx32 sHoldOffsetDefault;
extern const VecFx32 data_020ca6a0;
extern const VecFx32 data_020ca6ac;
extern const VecFx32 data_020ca6b8;
extern const s32 data_020ca6c4[4];
extern const u32 sHandOverFishScale[8];
extern const s32 data_020ca6f4[0x30];
extern const s32 data_020ca7b4[0x30];
extern const s32 data_020ca874[0x30];
extern const s32 data_020ca934[0x30];
extern const s32 data_020ca9f4[0x30];
extern const s32 data_020caab4[0x30];


HandOverItem *sHandOverItem;
HandOverItemAct sHandOverItemActs[12][2] = {
    { { NULL, NULL }, { NULL, NULL } },
    { { &HandOverItem::startAct01, &HandOverItem::updateAct01 }, { &HandOverItem::startAct01V1, &HandOverItem::updateAct01V1 } },
    { { &HandOverItem::startAct02, NULL }, { &HandOverItem::startAct02V1, &HandOverItem::updateAct02V1 } },
    { { &HandOverItem::startAct03, &HandOverItem::updateAct03 }, { &HandOverItem::startAct03V1, &HandOverItem::updateAct03V1 } },
    { { &HandOverItem::startAct04, &HandOverItem::updateAct04 }, { &HandOverItem::startAct04V1, &HandOverItem::updateAct04V1 } },
    { { &HandOverItem::startAct05, &HandOverItem::updateAct05 }, { &HandOverItem::startAct05V1, &HandOverItem::updateAct05V1 } },
    { { &HandOverItem::startAct06, &HandOverItem::updateAct06 }, { NULL, NULL } },
    { { &HandOverItem::startAct07, &HandOverItem::updateAct07 }, { &HandOverItem::startAct07V1, &HandOverItem::updateAct07V1 } },
    { { &HandOverItem::startAct08, &HandOverItem::updateAct08 }, { NULL, NULL } },
    { { NULL, NULL }, { &HandOverItem::startAct09V1, &HandOverItem::updateAct09V1 } },
    { { &HandOverItem::startAct0A, &HandOverItem::updateAct0A }, { NULL, NULL } },
    { { &HandOverItem::startAct0B, &HandOverItem::updateAct0B }, { NULL, NULL } },
};

u8 data_021c5a30[4] = { (u8)(data_020ca690[1] - data_020ca690[0]), (u8)(data_020ca690[2] - data_020ca690[1]), (u8)(data_020ca690[3] - data_020ca690[2]) };
FxVec3 sPutDownOffset(-0x400, -0x300, 0x1100);
u8 data_021c5a2c[4] = { (u8)(data_020ca680[1] - data_020ca680[0]), (u8)(data_020ca680[2] - data_020ca680[1]) };
u8 data_021c5a28[4] = { (u8)(data_020ca684[1] - data_020ca684[0]), (u8)(data_020ca684[2] - data_020ca684[1]) };
u8 data_021c5a24[4] = { (u8)(data_020ca67c[1] - data_020ca67c[0]) };
FxVec3 data_021c5acc(0, 0, 0xd00);
s32 data_021c5ae8[4] = { 0, Unk_020574cc_Abs(DS_DIV(data_020ca6c4[1], data_021c5a30[0])), Unk_020574cc_Abs(DS_DIV(data_020ca6c4[2], data_021c5a30[1])), Unk_020574cc_Abs(DS_DIV(data_020ca6c4[3], data_021c5a30[2])) };
FxVec3 data_021c5a84(0, 0, 0x1300);

// ---- own functions (declared for forward references) ----
}

extern "C" u32 HandOverItem_GetFishScale(u32 i)
{
    return sHandOverFishScale[i];
}

extern "C" u32 HandOverItem_GetFishHoldOffsetSet(ProcBase *o)
{
    u32 r = 3;
    volatile u16 h = 0xfff1;
    h = o->param;
    s32 a = h;
    u32 b = h;
    s32 t = (s32)(b & 0xf000) >> 12;
    if (t != 0xd) {
      if (t == 0xe) {
        switch (Villager_GetAnimalKind(SaveVillagers_Get(gSaveVillagers, b & 0xfff))) {
        case 0: case 5: case 6: case 10: case 12: case 15: case 16: case 21: case 25: case 31: case 32:
            r = 4; break;
        case 1: case 2: case 4: case 7: case 9: case 13: case 14: case 17: case 19: case 20: case 22: case 23: case 26: case 27: case 29:
            r = 5; break;
        }
      }
    } else {
        switch (a) {
        case 0xd00c: r = 0; break;
        case 0xd013: r = 1; break;
        case 0xd012:
        case 0xd025: r = 2; break;
        }
    }
    return r;
}

extern "C" VecFx32 *HandOverItem_GetFishHoldOffset(ProcBase *o, u32 idx)
{
    if (o->profile == 9) {
        return (VecFx32 *)&sFishHoldOffsetSets[3][idx];
    }
    return (VecFx32 *)&sFishHoldOffsetSets[HandOverItem_GetFishHoldOffsetSet(o)][idx];
}

extern "C" VecFx32 *HandOverItem_GetFishHoldOffset2(ProcBase *o, u32 idx)
{
    if (o->profile == 9) {
        return (VecFx32 *)sFishHoldOffsetSets[3][idx].altHoldOffset;
    }
    return (VecFx32 *)sFishHoldOffsetSets[HandOverItem_GetFishHoldOffsetSet(o)][idx].altHoldOffset;
}

extern "C" HandOverItem *HandOverItem_Create()
{
    return new HandOverItem;
}

BOOL HandOverItem::onCreate()
{
    resetState();
    seEmitter.callInit();
    sHandOverItem = this;
    return TRUE;
}

BOOL HandOverItem::onDelete()
{
    sHandOverItem->seEmitter.callStop();
    sHandOverItem = NULL;
    return TRUE;
}

BOOL HandOverItem::onExecute()
{
    if (variant >= 0 && variant < 2) {
        if (modeRequest < 0xc) {
            mode = modeRequest;
            frame = 0;
            HandOverItemAct *e = &sHandOverItemActs[mode][variant];
            if (e->a != 0) {
                (this->*(e->a))();
            }
            clearModeRequest();
        }
        if (mode < 0xc) {
            if (sHandOverItemActs[mode][variant].b != 0) {
                (this->*(sHandOverItemActs[mode][variant].b))();
                VecFx32 v;
                v.x = itemPos.x;
                v.y = itemPos.y;
                v.z = itemPos.z;
                seEmitter.callUpdateRelative(&v);
            }
        }
        if (isAwaitingTake()) {
            Math_CountDownU8(&takeTimer);
        }
    }
    return TRUE;
}

BOOL HandOverItem::onDraw()
{
    drawItem();
    return TRUE;
}

void HandOverItem::resetState()
{
    mode = 0;
    for (s32 i = 0; i < 2; i++) chars[i] = NULL;
    item = 0xfff1;
    nextMode = 0xc;
    frame = 0;
    phase = 0;
    clearModeRequest();
    offset.x = 0;
    offset.y = 0;
    offset.z = 0;
    worldOffset.x = 0;
    worldOffset.y = 0;
    worldOffset.z = 0;
    scale.x = 0;
    scale.y = 0;
    scale.z = 0;
    visible = 0;
    setBusy(0);
    reach = sHandOverReach;
    fishDisplay = -1;
}

BOOL HandOverItem::begin(u16 *id, s32 a, u8 b, s32 c, Character *o0, Character *o1)
{
    if (mode == 0 && o0 != NULL) {
        item = *id;
        kind = a;
        nextMode = b;
        variant = c;
        chars[0] = o0;
        chars[1] = o1;
        BOOL x;
        if (Item_IsFurniture(&item)) {
            u16 tmp = 0x1565;
            x = Item_GetFurnitureIndex(&item) == Item_GetFurnitureIndex(&tmp);
        } else {
            x = item == 0x1565;
        }
        BOOL rr;
        if (x && kind == 0) {
            goto near;
        }
        rr = FALSE;
        if (item >= 0x1492 && item <= 0x14fd) rr = TRUE;
        if (rr == 1 && kind == 2) {
        near:
            if (Scene_GetCurrent() == 9 || Scene_GetCurrent() == 0x10) {
                reach = sHandOverReachFar;
            } else {
                reach = sHandOverReach;
            }
        } else {
            reach = sHandOverReach;
        }
        BOOL r2 = FALSE;
        if (item >= 0x12e8 && item <= 0x131f) r2 = TRUE;
        if (r2) {
            fishDisplay = FishDisplay_Acquire();
        }
    }
    return FALSE;
}

BOOL HandOverItem::isCharAt(void *p, u32 idx)
{
    if (idx < 2 && chars[idx] != NULL) {
        if (((Character *)(p))->getCharId() == chars[idx]->getCharId()) {
            return TRUE;
        }
    }
    return FALSE;
}

BOOL HandOverItem::switchMaster(void *p)
{
    BOOL r = FALSE;
    if (isCharAt(p, 1) == 1) {
        chars[1] = chars[0];
        chars[0] = (Character *)p;
        r = TRUE;
    } else if (isCharAt(p, 0) == 1) {
        r = TRUE;
    }
    return r;
}

void HandOverItem::clearModeRequest()
{
    setModeRequest(0xc);
}

void HandOverItem::setModeRequest(u8 v)
{
    modeRequest = v;
    if (v == 1 || v == 7) {
        takeTimer = sHandOverTakeTimeout[0];
    }
}

BOOL HandOverItem::requestModeBy(u8 v, void *p)
{
    BOOL r = FALSE;
    if (isCharAt(p, 0) == 1) {
        setModeRequest(v);
        r = TRUE;
    }
    return r;
}

BOOL HandOverItem::isAwaitingTake()
{
    BOOL r5 = TRUE;
    BOOL r4 = TRUE;
    if (mode != 2) {
        if (mode != 1 || HandOverItem_IsModeActive(1) != 0) r4 = FALSE;
    }
    if (!r4) {
        if (mode != 7 || HandOverItem_IsModeActive(7) != 0) r5 = FALSE;
    }
    return r5;
}

void HandOverItem_DrawItem(HandOverItem *self, u16 *id, VecFx32 *p, VecFx32 *q, s16 *ang)
{
    BOOL r = FALSE;
    if (*id >= 0x12e8 && *id <= 0x131f) r = TRUE;
    if (r) {
        s32 c4 = self->fishDisplay;
        if (c4 != -1) {
            s32 idx;
            if (*id >= 0x12e8 && *id <= 0x131f) idx = *id - 0x12e8; else idx = -1;
            FishDisplay_SetEntry(c4, idx, p, q, ang[0], ang[1], ang[2], 0, 0, 0x1f);
            return;
        }
    }
    if (Unk_02058ddc_IsZero(*gFieldSceneKind) == 1) {
        VecFx32Copy a(*(VecFx32Copy *)p);
        VecFx32Copy b(*(VecFx32Copy *)q);
        Field_DrawItemIcon(*id, &a, &b, ang[0], ang[1], ang[2]);
    } else {
        VecFx32Copy a(*(VecFx32Copy *)p);
        VecFx32Copy b(*(VecFx32Copy *)q);
        RoomItemIcons_DrawItem(*id, &a, &b, ang[0], ang[1], ang[2]);
    }
}

void HandOverItem_DrawIcon(void *unused, u32 id, VecFx32 *p, VecFx32 *q, s16 *ang)
{
    if (Unk_02058ddc_IsZero(*gFieldSceneKind) == 1) {
        VecFx32Copy a(*(VecFx32Copy *)p);
        VecFx32Copy b(*(VecFx32Copy *)q);
        Field_DrawIconModel(id, &a, &b, ang[0], ang[1], ang[2]);
    } else {
        VecFx32Copy a(*(VecFx32Copy *)p);
        VecFx32Copy b(*(VecFx32Copy *)q);
        RoomItemIcons_DrawIcon(id, &a, &b, ang[0], ang[1], ang[2]);
    }
}

void HandOverItem::drawItem()
{
    if (mode < 0xc && item != 0xfff1 && visible == 1) {
        if (kind == 2 && !(item >= 0x1561 && item <= 0x1564) && !(item >= 0x155f && item <= 0x1560)) {
            HandOverItem_DrawIcon(this, 0x27, &itemPos, (VecFx32 *)&scale, itemRot);
        } else {
            HandOverItem_DrawItem(this, &item, &itemPos, (VecFx32 *)&scale, itemRot);
        }
    }
}

void HandOverItem::getMasterHoldPos(VecFx32 *out)
{
    if (chars[0] != NULL) {
        chars[0]->getHeldItemPos((VecFx32 *)out);
    }
}

void HandOverItem::setBusy(u32 v)
{
    busy = v;
}

BOOL HandOverItem::setNextModeBy(u8 v, void *p)
{
    BOOL r = FALSE;
    if (mode != 0 && busy != 1 && isCharAt(p, 0) == 1) {
        nextMode = v;
        r = TRUE;
    }
    return r;
}

void HandOverItem::localToWorld(VecFx32 *out, VecFx32 *in, u32 idx)
{
    out->x = in->x;
    out->y = in->y;
    out->z = in->z;
    if (idx < 2) {
        if (chars[idx] != NULL) {
            Vec_RotateY(out, chars[idx]->rotY);
            VEC_Add(out, (VecFx32 *)&chars[idx]->position, out);
        }
    }
}

VecFx32 HandOverItem::getHoldOffset(u32 idx)
{
    u32 t = 0xd8;
    Character *o = chars[idx];
    if (o != NULL) {
        t = o->profile;  // (was the DS offset 0xc)
    }
    if (t == 0x6b) {
        return data_020ca6a0;
    } else if (t == 0x6a) {
        return data_020ca6ac;
    } else if (t == 0x76) {
        return data_020ca6b8;
    } else {
        return sHoldOffsetDefault;
    }
}

void HandOverItem::startAct01()
{
    VecFx32 pos, off;
    offset.x = 0;
    offset.y = 0;
    offset.z = 0;
    worldOffset.x = 0;
    worldOffset.y = 0;
    worldOffset.z = 0;
    basePos.x = 0;
    basePos.y = 0;
    basePos.z = 0;
    pos = *(VecFx32 *)&data_021c5acc;
    if (Unk_020586bc_Range(&item, 0x12e8, 0x131f)) {
        VEC_Add(&pos, HandOverItem_GetFishHoldOffset((ProcBase *)chars[0], Fish_GetSizeClass(item - 0x12e8)), &pos);
    }
    off = getHoldOffset(0);  // (the DS passes the result buffer, this and 0)
    VEC_Add(&pos, &off, &pos);
    localToWorld(&basePos, &pos, 0);
    scale.x = 0;
    scale.y = 0;
    scale.z = 0;
    visible = 1;
    phase = 0;
    setBusy(1);
}

void HandOverItem::act01Phase0()
{
    getMasterHoldPos(&itemPos);
    frame = frame + 1;
    if ((s32)frame >= data_020ca690[(*(volatile u8 *)&phase)]) {
        s32 n, t;
        (*(volatile u8 *)&phase) = (*(volatile u8 *)&phase) + 1;
        n = data_021c5a30[0];
        t = DS_DIV((basePos.x - itemPos.x), n);
        if (t < 0) t = -t;
        moveStep.x = t;
        t = DS_DIV(basePos.y, n);
        if (t < 0) t = -t;
        moveStep.y = t;
        t = DS_DIV((basePos.z - itemPos.z), n);
        if (t < 0) t = -t;
        moveStep.z = t;
    }
}

void HandOverItem::act01Phase1()
{
    VecFx32 cur;
    s32 v = scale.x;
    s32 a = data_020ca6c4[phase];
    s32 b = data_021c5ae8[phase];
    if (Unk_020586bc_Range(&item, 0x12e8, 0x131f)) {
        s32 k = HandOverItem_GetFishScale(Fish_GetSizeClass(item - 0x12e8));
        a = func_01ffcb0c(a, k);
        b = func_01ffcb0c(b, k);
    }
    Math_StepS32Alt(&v, a, b);
    s32 u = v;
    scale.x = u;
    scale.y = u;
    scale.z = u;
    getMasterHoldPos(&cur);
    Math_StepS32(&itemPos.x, basePos.x, moveStep.x);
    Math_StepS32(&itemPos.y, cur.y + basePos.y, moveStep.y);
    Math_StepS32(&itemPos.z, basePos.z, moveStep.z);
    frame = frame + 1;
    if ((s32)frame >= data_020ca690[(*(volatile u8 *)&phase)]) {
        (*(volatile u8 *)&phase) = (*(volatile u8 *)&phase) + 1;
        if ((*(volatile u8 *)&phase) >= 4) {
            setBusy(0);
        }
    }
}

void HandOverItem::updateAct01()
{
    static void (HandOverItem::*const tbl[4])() = {
        &HandOverItem::act01Phase0,
        &HandOverItem::act01Phase1,
        &HandOverItem::act01Phase1,
        &HandOverItem::act01Phase1,
    };
    if (phase < 4) {
        (this->*tbl[phase])();
    }
}

void HandOverItem::startAct02()
{
    setBusy(1);
}

void HandOverItem::startAct03()
{
    VecFx32 cur, pos, off;
    s32 a, b, c;
    getMasterHoldPos(&cur);
    basePos.x = 0;
    basePos.y = 0;
    basePos.z = 0;
    pos = *(VecFx32 *)&data_021c5a84;
    if (Unk_020586bc_Range(&item, 0x12e8, 0x131f)) {
        VEC_Add(&pos, HandOverItem_GetFishHoldOffset2((ProcBase *)chars[0], Fish_GetSizeClass(item - 0x12e8)), &pos);
    }
    off = getHoldOffset(0);  // (the DS passes the result buffer, this and 0)
    VEC_Add(&pos, &off, &pos);
    localToWorld(&basePos, &pos, 0);
    target.x = basePos.x;
    target.y = cur.y + basePos.y;
    target.z = basePos.z;
    c = (target.z - itemPos.z) / 2;
    if (c < 0) c = -c;
    b = (target.y - itemPos.y) / 2;
    if (b < 0) b = -b;
    a = (target.x - itemPos.x) / 2;
    if (a < 0) a = -a;
    moveStep.x = a;
    moveStep.y = b;
    moveStep.z = c;
    frame = 0;
    setBusy(1);
}

void HandOverItem::updateAct03()
{
    if ((s32)frame < 2) {
        Math_StepS32(&itemPos.x, target.x, moveStep.x);
        Math_StepS32(&itemPos.y, target.y, moveStep.y);
        Math_StepS32(&itemPos.z, target.z, moveStep.z);
        frame = frame + 1;
        if ((s32)frame >= 2) {
            setBusy(0);
        }
    }
}

void HandOverItem::startAct04()
{
    VecFx32 cur, pos, off;
    s32 a, b;
    getMasterHoldPos(&cur);
    basePos.x = 0;
    basePos.y = 0;
    basePos.z = 0;
    pos = *(VecFx32 *)&data_021c5acc;
    if (Unk_020586bc_Range(&item, 0x12e8, 0x131f)) {
        VEC_Add(&pos, HandOverItem_GetFishHoldOffset((ProcBase *)chars[0], Fish_GetSizeClass(item - 0x12e8)), &pos);
    }
    off = getHoldOffset(0);  // (the DS passes the result buffer, this and 0)
    VEC_Add(&pos, &off, &pos);
    localToWorld(&basePos, &pos, 0);
    target.x = basePos.x;
    target.y = basePos.y;
    target.z = basePos.z;
    a = (target.z - itemPos.z) / 2;
    if (a < 0) a = -a;
    b = (target.x - itemPos.x) / 2;
    if (b < 0) b = -b;
    moveStep.x = b;
    moveStep.y = 0;
    moveStep.z = a;
    frame = 0;
    setBusy(1);
}

void HandOverItem::updateAct04()
{
    VecFx32 cur, t;
    s32 n = frame;
    if (n < 9) {
        getMasterHoldPos(&cur);
        Math_StepS32(&itemPos.x, target.x, moveStep.x);
        itemPos.y = cur.y + target.y;
        Math_StepS32(&itemPos.z, target.z, moveStep.z);
        frame = frame + 1;
        if ((s32)frame >= 9) {
            Vec_Sub(&t, &itemPos, &cur);
            offset.x = t.x;
            offset.y = t.y;
            offset.z = t.z;
            setBusy(0);
        }
    } else if (n == 9) {
        getMasterHoldPos(&itemPos);
        VEC_Add(&itemPos, &offset, &itemPos);
    }
}

void HandOverItem::startAct0A()
{
    VecFx32 cur, t;
    setBusy(0);
    getMasterHoldPos(&cur);
    Vec_Sub(&t, &itemPos, &cur);
    offset.x = t.x;
    offset.y = t.y;
    offset.z = t.z;
    frame = 0;
}

void HandOverItem::updateAct0A()
{
    getMasterHoldPos(&itemPos);
    VEC_Add(&itemPos, &offset, &itemPos);
}

void HandOverItem::startAct05()
{
    VecFx32 cur, t;
    target.x = 0;
    target.y = 0;
    target.z = 0;
    getMasterHoldPos(&cur);
    Vec_Sub(&t, &itemPos, &cur);
    offset.x = t.x;
    offset.y = t.y;
    offset.z = t.z;
    if (chars[0] != NULL) {
        Vec_RotateY(&offset, (s16)-chars[0]->rotY);
    }
    worldOffset.x = 0;
    worldOffset.y = 0;
    worldOffset.z = 0;
    s32 a, b, c;
    c = offset.z / 8;
    if (c < 0) c = -c;
    b = offset.y / 8;
    if (b < 0) b = -b;
    a = offset.x / 8;
    if (a < 0) a = -a;
    moveStep.x = a;
    moveStep.y = b;
    moveStep.z = c;
    a = scale.x / 8;
    if (a < 0) a = -a;
    scaleStep = a;
    setBusy(1);
    frame = 0;
    phase = 0;
}

void HandOverItem::updateAct05()
{
    s32 v = scale.x;
    if ((s32)frame < 8) {
        getMasterHoldPos(&itemPos);
        Math_StepS32(&v, 0, scaleStep);
        s32 u = v;
        scale.x = u;
        scale.y = u;
        scale.z = u;
        Math_StepS32(&offset.x, 0, moveStep.x);
        Math_StepS32(&offset.y, 0, moveStep.y);
        Math_StepS32(&offset.z, 0, moveStep.z);
        if (chars[0] != NULL) {
            worldOffset.x = offset.x;
            worldOffset.y = offset.y;
            worldOffset.z = offset.z;
            Vec_RotateY(&worldOffset, chars[0]->rotY);
        }
        VEC_Add(&itemPos, &worldOffset, &itemPos);
        frame = frame + 1;
        if ((s32)frame >= 8) {
            setBusy(0);
        }
    }
}

void HandOverItem::startAct06() {}

void HandOverItem::updateAct06() {}

void HandOverItem::startAct07()
{
    VecFx32 cur, t;
    getMasterHoldPos(&cur);
    Vec_Sub(&t, &itemPos, &cur);
    offset.x = t.x;
    offset.y = t.y;
    offset.z = t.z;
    setBusy(1);
    frame = 0;
}

void HandOverItem::updateAct07()
{
    if ((s32)frame < 9) {
        getMasterHoldPos(&itemPos);
        VEC_Add(&itemPos, &offset, &itemPos);
        frame = frame + 1;
        if ((s32)frame >= 9) {
            setBusy(0);
        }
    }
}

void HandOverItem::startAct08()
{
    setBusy(1);
    frame = 0;
    if (kind == 2) {
        kind = 0;
    }
    Effect_PlayById(0x93, &itemPos, 0, 0);
}

void HandOverItem::updateAct08()
{
    if ((s32)frame < 10) {
        frame = frame + 1;
        if ((s32)frame >= 10) {
            setBusy(0);
        }
    }
}

void HandOverItem::startAct0B()
{
    VecFx32 cur, t;
    setBusy(1);
    getMasterHoldPos(&cur);
    Vec_Sub(&t, &itemPos, &cur);
    offset.x = t.x;
    offset.y = t.y;
    offset.z = t.z;
    frame = 0;
    phase = 0;
}

void HandOverItem::act0BPhase0()
{
    if ((s32)frame < data_020ca67c[phase]) {
        getMasterHoldPos(&itemPos);
        VEC_Add(&itemPos, &offset, &itemPos);
        frame = frame + 1;
        if ((s32)frame >= data_020ca67c[phase]) {
            s32 a, b, c;
    c = DS_DIV(scale.x, data_021c5a24[phase]);
            if (c < 0) c = -c;
            scaleStep = c;
            phase = phase + 1;
        }
    }
}

void HandOverItem::act0BPhase1()
{
    if ((s32)frame < data_020ca67c[(*(volatile u8 *)&phase)]) {
        s32 t = scale.x;
        Math_StepS32(&t, 0, scaleStep);
        s32 u = t;
        scale.x = u;
        scale.y = u;
        scale.z = u;
        getMasterHoldPos(&itemPos);
        VEC_Add(&itemPos, &offset, &itemPos);
        frame = frame + 1;
        if ((s32)frame >= data_020ca67c[(*(volatile u8 *)&phase)]) {
            (*(volatile u8 *)&phase) = (*(volatile u8 *)&phase) + 1;
        }
    }
}

void HandOverItem::updateAct0B()
{
    static Unk_020dc034_Fn tbl[2] = { &HandOverItem::act0BPhase0, &HandOverItem::act0BPhase1 };
    u32 i = phase;
    if (i < 2) {
        (this->*tbl[i])();
    }
}

void HandOverItem::startAct01V1()
{
    offset.x = 0;
    offset.y = 0;
    offset.z = 0;
    worldOffset.x = 0;
    worldOffset.y = 0;
    worldOffset.z = 0;
    scale.x = 0;
    scale.y = 0;
    scale.z = 0;
    visible = 1;
    phase = 0;
    frame = 0;
    setBusy(1);
}

void HandOverItem::act01V1Phase0()
{
    s32 n = data_020ca680[0];
    if ((s32)frame < n) {
        frame = frame + 1;
        if ((s32)frame >= n) {
            offset.x = 0;
            offset.y = 0;
            offset.z = 0;
            worldOffset.x = 0;
            worldOffset.y = 0;
            worldOffset.z = 0;
            target.x = sPutDownOffset.x;
            target.y = sPutDownOffset.y;
            target.z = sPutDownOffset.z;
            s32 a, b, c;
    c = target.z / 12;
            if (c < 0) c = -c;
            b = target.y / 12;
            if (b < 0) b = -b;
            a = target.x / 12;
            if (a < 0) a = -a;
            moveStep.x = a;
            moveStep.y = b;
            moveStep.z = c;
            phase = 1;
        }
    }
}

void HandOverItem::act01V1Phase1()
{
    s32 n = data_020ca680[1];
    if ((s32)frame < n) {
        s32 t = scale.x;
        getMasterHoldPos(&itemPos);
        Math_StepS32(&t, 0x1000, 0x200);
        s32 u = t;
        scale.x = u;
        scale.y = u;
        scale.z = u;
        Math_StepS32(&offset.x, target.x, moveStep.x);
        Math_StepS32(&offset.y, target.y, moveStep.y);
        Math_StepS32(&offset.z, target.z, moveStep.z);
        if (chars[0] != NULL) {
            worldOffset.x = offset.x;
            worldOffset.y = offset.y;
            worldOffset.z = offset.z;
            Vec_RotateY(&worldOffset, chars[0]->rotY);
        }
        VEC_Add(&itemPos, &worldOffset, &itemPos);
        frame = frame + 1;
        if ((s32)frame >= n) {
            static FxVec3 s(0x10000, 0, 0x17000);
            VecFx32 t2;
            t2.x = 0;
            t2.y = 0;
            t2.z = 0x2000;
            if (Scene_GetCurrent() == 0x10) {
                target.x = s.x;
                target.y = s.y;
                target.z = s.z;
            } else {
                Vec_RotateY(&t2, chars[0]->rotY);
                VEC_Add(&t2, (VecFx32 *)&chars[0]->position, &t2);
                FieldPos_SnapToUnitCenter(&target, &t2);
            }
            target.y = target.y + 0x1000;
            s32 m = data_021c5a2c[1];
            s32 a, b, c;
    c = DS_DIV((target.z - itemPos.z), m);
            if (c < 0) c = -c;
            b = DS_DIV((target.y - itemPos.y), m);
            if (b < 0) b = -b;
            a = DS_DIV((target.x - itemPos.x), m);
            if (a < 0) a = -a;
            moveStep.x = a;
            moveStep.y = b;
            moveStep.z = c;
            phase = 2;
        }
    }
}

void HandOverItem::act01V1Phase2()
{
    s32 n = data_020ca680[2];
    if ((s32)frame < n) {
        s32 t = scale.x;
        Math_StepS32(&t, 0x1000, 0x200);
        s32 u = t;
        scale.x = u;
        scale.y = u;
        scale.z = u;
        Math_StepS32(&itemPos.x, target.x, moveStep.x);
        Math_StepS32(&itemPos.y, target.y, moveStep.y);
        Math_StepS32(&itemPos.z, target.z, moveStep.z);
        frame = frame + 1;
        if ((s32)frame >= n) {
            BOOL r = FALSE;
            if (item >= 0x1492 && item <= 0x14fd) r = TRUE;
            if (r) {
                Snd_SeEmitterPlayOneShot(&seEmitter, 0x70, 0x7f, 0);
            }
            phase = 3;
            setBusy(0);
        }
    }
}

void HandOverItem::updateAct01V1()
{
    static Unk_020dc034_Fn tbl[3] = { &HandOverItem::act01V1Phase0, &HandOverItem::act01V1Phase1, &HandOverItem::act01V1Phase2 };
    u32 i = phase;
    if (i < 3) {
        (this->*tbl[i])();
    }
}

void HandOverItem::startAct02V1()
{
    static FxVec3 s(0x10000, 0, 0x17000);
    VecFx32 t;
    t.x = 0;
    t.y = 0;
    t.z = 0x2000;
    if (Scene_GetCurrent() == 0x10) {
        target.x = s.x;
        target.y = s.y;
        target.z = s.z;
    } else {
        Vec_RotateY(&t, chars[0]->rotY);
        VEC_Add(&t, (VecFx32 *)&chars[0]->position, &t);
        FieldPos_SnapToUnitCenter(&target, &t);
    }
    target.y = target.y + 0x1000;
    s32 a, b, c;
    c = (target.z - itemPos.z) / 4;
    if (c < 0) c = -c;
    b = (target.y - itemPos.y) / 4;
    if (b < 0) b = -b;
    a = (target.x - itemPos.x) / 4;
    if (a < 0) a = -a;
    moveStep.x = a;
    moveStep.y = b;
    moveStep.z = c;
    frame = 0;
    setBusy(1);
}

void HandOverItem::updateAct02V1()
{
    if ((s32)frame < 4) {
        Math_StepS32(&itemPos.x, target.x, moveStep.x);
        Math_StepS32(&itemPos.y, target.y, moveStep.y);
        Math_StepS32(&itemPos.z, target.z, moveStep.z);
        frame = frame + 1;
        if ((s32)frame >= 4) {
            setBusy(0);
        }
    }
}

void HandOverItem::startAct03V1()
{
    getMasterHoldPos(&target);
    if (chars[0] != NULL) {
        worldOffset.x = sPutDownOffset.x;
        worldOffset.y = sPutDownOffset.y;
        worldOffset.z = sPutDownOffset.z;
        Vec_RotateY(&worldOffset, chars[0]->rotY);
    }
    VEC_Add(&target, &worldOffset, &target);
    s32 a, b, c;
    c = (target.z - itemPos.z) / 2;
    if (c < 0) c = -c;
    b = (target.y - itemPos.y) / 2;
    if (b < 0) b = -b;
    a = (target.x - itemPos.x) / 2;
    if (a < 0) a = -a;
    moveStep.x = a;
    moveStep.y = b;
    moveStep.z = c;
    frame = 0;
    setBusy(1);
}

void HandOverItem::updateAct03V1()
{
    if ((s32)frame < 2) {
        Math_StepS32(&itemPos.x, target.x, moveStep.x);
        Math_StepS32(&itemPos.y, target.y, moveStep.y);
        Math_StepS32(&itemPos.z, target.z, moveStep.z);
        frame = frame + 1;
        if ((s32)frame >= 2) {
            setBusy(0);
        }
    }
}

void HandOverItem::startAct04V1()
{
    setBusy(1);
    frame = 0;
}

void HandOverItem::updateAct04V1()
{
    if ((s32)frame < 10) {
        getMasterHoldPos(&itemPos);
        if (chars[0] != NULL) {
            worldOffset.x = sPutDownOffset.x;
            worldOffset.y = sPutDownOffset.y;
            worldOffset.z = sPutDownOffset.z;
            Vec_RotateY(&worldOffset, chars[0]->rotY);
        }
        VEC_Add(&itemPos, &worldOffset, &itemPos);
        frame = frame + 1;
        if ((s32)frame >= 10) {
            setBusy(0);
        }
    }
}

void HandOverItem::startAct05V1()
{
    target.x = 0;
    target.y = 0;
    target.z = 0;
    offset.x = sPutDownOffset.x;
    offset.y = sPutDownOffset.y;
    offset.z = sPutDownOffset.z;
    worldOffset.x = 0;
    worldOffset.y = 0;
    worldOffset.z = 0;
    s32 a, b, c;
    c = offset.z / 8;
    if (c < 0) c = -c;
    b = offset.y / 8;
    if (b < 0) b = -b;
    a = offset.x / 8;
    if (a < 0) a = -a;
    moveStep.x = a;
    moveStep.y = b;
    moveStep.z = c;
    setBusy(1);
    frame = 0;
    phase = 0;
}

void HandOverItem::updateAct05V1(void) {
    static s32 step = Unk_020574cc_Abs(0x1000 / 8);
    s32 tmp = scale.x;
    if ((s32)frame >= 8) {
        return;
    }
    getMasterHoldPos(&itemPos);
    Math_StepS32(&tmp, 0, step);
    s32 t2 = tmp;
    scale.x = t2;
    scale.y = t2;
    scale.z = t2;
    Math_StepS32(&offset.x, 0, moveStep.x);
    Math_StepS32(&offset.y, 0, moveStep.y);
    Math_StepS32(&offset.z, 0, moveStep.z);
    if (chars[0]) {
        worldOffset.x = offset.x;
        worldOffset.y = offset.y;
        worldOffset.z = offset.z;
        Vec_RotateY(&worldOffset, chars[0]->rotY);
    }
    VEC_Add(&itemPos, &worldOffset, &itemPos);
    frame++;
    if ((s32)frame >= 8) {
        setBusy(0);
    }
}

void HandOverItem::startAct07V1(void) {
    setBusy(1);
    frame = 0;
}

void HandOverItem::updateAct07V1(void) {
    updateAct04();
}

void HandOverItem::startAct09V1(void) {
    setBusy(1);
    frame = 0;
    phase = 0;
}

void HandOverItem::act09V1Phase0(void) {
    s32 c, b, a;
    s32 n = data_020ca684[0];
    if (frame >= n) {
        return;
    }
    frame++;
    if (frame >= n) {
        getMasterHoldPos(&target);
        if (chars[0]) {
            worldOffset.x = sPutDownOffset.x;
            worldOffset.y = sPutDownOffset.y;
            worldOffset.z = sPutDownOffset.z;
            Vec_RotateY(&worldOffset, chars[0]->rotY);
        }
        VEC_Add(&target, &worldOffset, &target);
        u32 d = data_021c5a28[0];
        a = Unk_020574cc_Abs(DS_DIV((target.z - itemPos.z), (s32)d));
        b = Unk_020574cc_Abs(DS_DIV((target.y - itemPos.y), (s32)d));
        c = Unk_020574cc_Abs(DS_DIV((target.x - itemPos.x), (s32)d));
        moveStep.x = c;
        moveStep.y = b;
        moveStep.z = a;
        phase = 1;
    }
}

void HandOverItem::act09V1Phase1(void) {
    s32 c, b, a;
    s32 n = data_020ca684[1];
    if (frame >= n) {
        return;
    }
    getMasterHoldPos(&target);
    if (chars[0]) {
        worldOffset.x = sPutDownOffset.x;
        worldOffset.y = sPutDownOffset.y;
        worldOffset.z = sPutDownOffset.z;
        Vec_RotateY(&worldOffset, chars[0]->rotY);
    }
    VEC_Add(&target, &worldOffset, &target);
    Math_StepS32(&itemPos.x, target.x, moveStep.x);
    Math_StepS32(&itemPos.y, target.y, moveStep.y);
    Math_StepS32(&itemPos.z, target.z, moveStep.z);
    frame++;
    if (frame >= n) {
        target.x = 0;
        target.y = 0;
        target.z = 0;
        offset.x = sPutDownOffset.x;
        offset.y = sPutDownOffset.y;
        offset.z = sPutDownOffset.z;
        worldOffset.x = 0;
        worldOffset.y = 0;
        worldOffset.z = 0;
        a = Unk_020574cc_Abs(offset.z / 8);
        b = Unk_020574cc_Abs(offset.y / 8);
        c = Unk_020574cc_Abs(offset.x / 8);
        moveStep.x = c;
        moveStep.y = b;
        moveStep.z = a;
        phase = 2;
    }
}

void HandOverItem::act09V1Phase2(void) {
    static s32 step = Unk_020574cc_Abs(DS_DIV(0x1000, data_021c5a28[1]));
    s32 tmp = scale.x;
    s32 n = data_020ca684[2];
    if (frame >= n) {
        return;
    }
    getMasterHoldPos(&itemPos);
    Math_StepS32(&tmp, 0, step);
    s32 t2 = tmp;
    scale.x = t2;
    scale.y = t2;
    scale.z = t2;
    Math_StepS32(&offset.x, 0, moveStep.x);
    Math_StepS32(&offset.y, 0, moveStep.y);
    Math_StepS32(&offset.z, 0, moveStep.z);
    if (chars[0]) {
        worldOffset.x = offset.x;
        worldOffset.y = offset.y;
        worldOffset.z = offset.z;
        Vec_RotateY(&worldOffset, chars[0]->rotY);
    }
    VEC_Add(&itemPos, &worldOffset, &itemPos);
    frame++;
    if (frame >= n) {
        phase = 3;
        setBusy(0);
    }
}

void HandOverItem::updateAct09V1(void) {
    static void (HandOverItem::*tbl[3])(void) = {
        &HandOverItem::act09V1Phase0,
        &HandOverItem::act09V1Phase1,
        &HandOverItem::act09V1Phase2,
    };
    u32 state = phase;
    if (state < 3) {
        (this->*tbl[state])();
    }
}

extern "C" s32 HandOverItem_Begin(u16 *item, s32 b, u8 c, s32 d, Character *master, Character *partner) {
    s32 r = 0;
    if (sHandOverItem) {
        r = sHandOverItem->begin(item, b, c, d, master, partner);
    }
    return r;
}

extern "C" s32 HandOverItem_IsMaster(Character *a) {
    s32 r = 0;
    if (sHandOverItem) {
        r = sHandOverItem->isCharAt(a, 0);
    }
    return r;
}

extern "C" s32 HandOverItem_RequestMode(u8 a, Character *b) {
    s32 r = 0;
    if (sHandOverItem) {
        r = sHandOverItem->requestModeBy(a, b);
    }
    return r;
}

extern "C" u8 HandOverItem_GetNextMode(void) {
    if (sHandOverItem) {
        return sHandOverItem->nextMode;
    }
    return 0xc;
}

extern "C" void HandOverItem_End(Character *a) {
    if (sHandOverItem) {
        if (HandOverItem_IsMaster(a) == 1) {
            if (sHandOverItem->fishDisplay != -1) {
                FishDisplay_Release(sHandOverItem->fishDisplay);
            }
            sHandOverItem->resetState();
        }
    }
}

extern "C" BOOL HandOverItem_CanTake(Actor *p) {
    BOOL r = FALSE;
    if (p) {
        HandOverItem *g = sHandOverItem;
        if (g) {
            if (Vec_DistXZ(&p->position, &g->itemPos) <= g->reach) {
                r = TRUE;
            } else if (g->isAwaitingTake()) {
                if (sHandOverItem->takeTimer == 0) {
                    r = TRUE;
                }
            }
        }
    }
    return r;
}

extern "C" BOOL HandOverItem_GetPos(VecFx32 *out) {
    BOOL r = FALSE;
    HandOverItem *g = sHandOverItem;
    if (g) {
        VecFx32 *pv = &g->itemPos;
        out->x = pv->x;
        out->y = pv->y;
        out->z = pv->z;
        r = TRUE;
    }
    return r;
}

extern "C" s32 HandOverItem_SwitchMaster(Character *a) {
    s32 r = 0;
    if (sHandOverItem) {
        r = sHandOverItem->switchMaster(a);
    }
    return r;
}

extern "C" BOOL HandOverItem_IsModeActive(u32 a) {
    BOOL r = FALSE;
    HandOverItem *g = sHandOverItem;
    if (g) {
        if ((g->mode == a && g->busy == 1) || g->modeRequest == a) {
            r = TRUE;
        }
    }
    return r;
}

extern "C" BOOL HandOverItem_IsActive(void) {
    BOOL r = FALSE;
    if (sHandOverItem) {
        if (sHandOverItem->mode != 0) {
            r = TRUE;
        }
    }
    return r;
}

extern "C" void HandOverItem_GetItem(u16 *out) {
    *out = 0xfff1;
    if (sHandOverItem) {
        *out = sHandOverItem->item;
    }
}

// ======== FUNCTIONS ========

extern "C" s32 HandOverItem_SetNextMode(u8 a, Character *b) {
    s32 r = 0;
    if (sHandOverItem) {
        r = sHandOverItem->setNextModeBy(a, b);
    }
    return r;
}

ProcProfile sHandOverItemProfile = { (void *(*)())HandOverItem_Create, 0xd1, 0xcd };
FishHoldOffsets *sFishHoldOffsetSets[6] = { (FishHoldOffsets *)data_020caab4, (FishHoldOffsets *)data_020ca6f4, (FishHoldOffsets *)data_020ca7b4, (FishHoldOffsets *)data_020ca874, (FishHoldOffsets *)data_020ca934, (FishHoldOffsets *)data_020ca9f4 };

const u8 sHandOverTakeTimeout[4] = { 0x28, 0x0, 0x0, 0x0 };
const u8 data_020ca67c[4] = { 0xa, 0x16, 0x0, 0x0 };
const u8 data_020ca680[4] = { 0xb, 0x11, 0x15, 0x0 };
const u8 data_020ca684[4] = { 0x6, 0x9, 0x15, 0x0 };
const u8 data_020ca690[4] = { 0xb, 0x14, 0x16, 0x18 };
const s32 sHandOverReachFar = 0x4000;
const s32 sHandOverReach = 0x1500;
const VecFx32 sHoldOffsetDefault = { 0x0, -0x600, 0x0 };
const VecFx32 data_020ca6a0 = { 0x0, -0x1000, 0x0 };
const VecFx32 data_020ca6ac = { 0x0, -0x1000, 0x200 };
const VecFx32 data_020ca6b8 = { 0x0, -0x900, 0x300 };
const s32 data_020ca6c4[4] = { 0x0, 0x1100, 0xf80, 0x1000 };
const u32 sHandOverFishScale[8] = { 0x1666, 0x14cd, 0x1333, 0x10cd, 0x1000, 0x1000, 0x1000, 0xe66 };
const s32 data_020ca6f4[0x30] = {
    0x0, 0xb00, 0x200, 0x0, 0xb00, 0x0,
    0x0, 0xa00, 0x200, 0x0, 0xa00, 0x0,
    0x0, 0x900, 0x200, 0x0, 0x900, 0x0,
    0x0, 0x800, 0x200, 0x0, 0x800, 0x0,
    0x0, 0x700, 0x200, 0x0, 0x700, 0x0,
    0x0, 0x700, 0x200, 0x0, 0x700, 0x0,
    0x0, 0x600, 0x400, 0x0, 0x600, 0x0,
    0x0, 0x700, 0x200, 0x0, 0x700, 0x0
};
const s32 data_020ca7b4[0x30] = {
    0x0, -0x200, 0x300, 0x0, -0x200, 0x0,
    0x0, -0x200, 0x400, 0x0, -0x200, 0x0,
    0x0, -0x300, 0x400, 0x0, -0x300, 0x0,
    0xa00, 0x400, 0x300, 0x0, 0x400, 0x0,
    0xa00, 0x400, 0x300, 0x0, 0x400, 0x0,
    0xa00, 0x400, 0x300, 0x0, 0x400, 0x0,
    0xa00, 0x200, 0x400, 0x0, 0x200, 0x0,
    0x800, 0x200, 0x300, 0x0, 0x200, 0x0
};
const s32 data_020ca874[0x30] = {
    0x0, 0x500, 0x0, 0x0, 0x500, -0x500,
    0x0, 0x400, 0x0, 0x0, 0x400, -0x500,
    0x0, 0x300, 0x100, 0x0, 0x300, -0x500,
    0x0, 0x200, 0x100, 0x0, 0x200, -0x500,
    0x0, 0x200, 0x100, 0x0, 0x200, -0x500,
    0x0, 0x200, 0x100, 0x0, 0x200, -0x500,
    0x0, 0x200, 0x200, 0x0, 0x200, -0x500,
    0x0, 0x200, 0x100, 0x0, 0x200, -0x500
};
const s32 data_020ca934[0x30] = {
    0x0, 0x800, 0x0, 0x0, 0x800, -0x500,
    0x0, 0x700, 0x100, 0x0, 0x700, -0x500,
    0x0, 0x600, 0x100, 0x0, 0x600, -0x500,
    0x0, 0x600, 0x100, 0x0, 0x600, -0x500,
    0x0, 0x600, 0x100, 0x0, 0x600, -0x500,
    0x0, 0x600, 0x100, 0x0, 0x600, -0x500,
    0x0, 0x600, 0x200, 0x0, 0x600, -0x500,
    0x0, 0x600, 0x100, 0x0, 0x600, -0x500
};
const s32 data_020ca9f4[0x30] = {
    0x0, 0x100, 0x0, 0x0, 0x100, -0x500,
    0x0, 0x100, 0x0, 0x0, 0x100, -0x500,
    0x0, 0x100, 0x100, 0x0, 0x100, -0x500,
    0x700, 0x100, 0x100, 0x0, 0x100, -0x500,
    0x700, 0x100, 0x100, 0x0, 0x100, -0x500,
    0x700, 0x100, 0x100, 0x0, 0x100, -0x500,
    0x700, 0x100, 0x200, 0x0, 0x100, -0x500,
    0x800, 0x100, 0x100, 0x0, 0x100, -0x500
};
const s32 data_020caab4[0x30] = {
    0x0, 0x500, 0x0, 0x0, 0x500, -0x500,
    0x0, 0x400, 0x0, 0x0, 0x400, -0x500,
    0x0, 0x300, 0x100, 0x0, 0x300, -0x500,
    -0x600, 0x300, 0x100, 0x0, 0x300, -0x500,
    -0x600, 0x300, 0x100, 0x0, 0x300, -0x500,
    -0x600, 0x300, 0x100, 0x0, 0x300, -0x500,
    -0x600, 0x300, 0x200, 0x0, 0x300, -0x500,
    -0x800, 0x300, 0x100, 0x0, 0x300, -0x500
};
