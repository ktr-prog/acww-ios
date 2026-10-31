#include "town/TownBlockCell.h"
#include "host_labels.h"
#include "host_shared_calls.h"
#include "sys/NNSFndArchive.h"
#include "types.h"
#include "town/BuildingInfo.h"
#include "nitro/mtx.h"
#include "gfx/NNSG3dRS.h"
#include "item/ItemId.h"
#include "game/StrBSizeData.h"
#include "game/LightLevel.h"
#include "game/UnitShapeQueryX.h"
#include "game/FxVec3.h"
#include "town/TownUnitShapeQuery.h"
#include "game/NibblePair.h"
#include "gfx/G3dResAccess.h"
#include "net/CommManager.h"
#include "save/Pattern.h"
#include "town/BuildingActor.h"
#include "town/BuildingLights.h"
#include "field/FieldObjectShapeQuery.h"
#include "gfx/NNSG3dResMdl.h"
#include "save/TownStyleRecordView.h"
#include "town/TownStyleRecord.h"
#include "field/SnowmanRecords.h"
#include "proto/main/unk_020b4828.h"
#include "proto/main/unk_02070560.h"
#include "field/FieldStructureMgr.h"
#include "proto/autoload_2/unk_020e8558.h"
#include "proto/autoload_2/unk_02101048.h"
#include "proto/main/unk_02063904.h"
#include "proto/main/unk_02063f3c.h"
#include "proto/main/unk_0206d988.h"
#include "proto/main/unk_02070560.h"
#include "proto/main/unk_02082d74.h"
#include "proto/main/unk_02085940.h"
#include "proto/itcm/unk_01ffb4e8.h"
#include "proto/main/unk_020b0e60.h"
#include "proto/ov003/unk_ov003_022187fc.h"
#include "proto/main/unk_0204cc1c.h"


extern "C" {
extern void *gCommManager;
extern u8 gFieldSceneKind;
extern s16 data_02135f44[];
extern BlockMap *gSceneBlockMap;
#define gSaveVillagers HOST_LABEL(u8 *, gSaveVillagers)
#define data_021e58a6 HOST_LABEL(u8 *, data_021e58a6)
#define data_021ed2e6 HOST_LABEL(u8 (*)[], data_021ed2e6)
extern s32 gFrameCounter;
#define data_027e0148 HOST_LABEL(u32 (*)[], data_027e0148)
}


struct Bits {
    u8 a : 2;
    u8 b : 6;
};

struct Flags1 {
    u8 mode : 2;
    u8 idx : 6;
};

struct Flags2 {
    u16 a : 6;
    u16 b : 6;
    u16 c : 4;
};


typedef ItemId Marker2;

// Info table of the next unit
extern BuildingInfo data_020d0a7c[];


inline BOOL isFlag1() {
    return gFieldSceneKind == 1;
}

static inline BOOL IsEnabled() {
    if (gFieldSceneKind == 0) {
        return TRUE;
    }
    return FALSE;
}

static inline BOOL func_020b2768_is_flag() {
    if (gFieldSceneKind == 0) {
        return TRUE;
    }
    return FALSE;
}

static inline MapBlock *GetCell(BlockMap *g, s32 x, s32 y) {
    if ((u32)x < (u32)g->width && (u32)y < (u32)g->height && g->blocks) {
        return g->blocks + (x + y * g->width);
    }
    return NULL;
}

inline BOOL InRange(const u16 &v)
{
    BOOL r = FALSE;
    if (v >= 0x5000 && v <= 0x5021) {
        r = TRUE;
    }
    return r;
}
inline BOOL InRangeV(u16 v)
{
    BOOL r = FALSE;
    if (v >= 0x5000 && v <= 0x5021) {
        r = TRUE;
    }
    return r;
}
inline BOOL InRange32(const u32 &v)
{
    BOOL r = FALSE;
    if (v >= 0x5000 && v <= 0x5021) {
        r = TRUE;
    }
    return r;
}

struct BuildingStateStackPad {
    s32 v[2];
    BuildingStateStackPad() {}
    ~BuildingStateStackPad() {}
};













// overlay class whose vtable is at 0x02232c00 (only its D1 is in this unit)


extern "C" {
void MTX_RotZ33_(void *m, s32 sn, s32 cs);
s32 FX_Div(s32 a, s32 b);
s32 func_01ffcb0c(s32 a, s32 b);
BOOL MapBlock_HasAnyAttr(MapBlock *o, u32 mask);
BOOL MapBlock_FindItemInRange(void *cell, u32 *a, u32 *b, ItemId *m, ItemId *c, u8 d);
u16 *MapBlock_GetItemPtr(void *cell, u32 x, u32 y, u32 z);
BOOL MapBlock_SetItem(void *cell, u16 *t, u32 x, u32 y, u8 z);
void TownUpdater_MarkEventApplied(u32 id);
u16 Item_MakePlayerHouse(u32 a);
BOOL Item_IsPlayerHouse(u16 *p);
s32 Item_GetSnowmanIndex(void *p);
BOOL Item_IsSnowman(void *p);
u16 Item_MakeNookShop(u32 a);
BOOL Item_IsNookShop(u16 *p);
u32 Item_MakeBuilding(u32 v);
s32 Item_GetFurnitureIndex(const u16 *p);
BOOL Item_IsFurniture(u16 *p);
void Pattern_CopyFields(void *dst, void *src);
u8 *SaveVillagers_Get(u8 *base, s32 idx);
u32 Villager_GetWhereabouts(void *a);
void Clock_GetMinuteHour(u8 *out);
u32 Scene_GetCurrent(void);
BOOL Scene_InTown(void);
void Math_StepS32(void *p, s32 a, s32 b);
void NNS_FndUnmountArchive(void *file);
BOOL NNS_FndMountArchive(void *file, const void *mode, void *arc);
void NNS_G3dMdlSetMdlAlpha(NNSG3dResMdl *mdl, s32 i, u8 v);
void NNS_G3dMdlSetMdlEmi(NNSG3dResMdl *mdl, s32 i, u16 c);
void NNS_G3dMdlSetMdlDiff(NNSG3dResMdl *mdl, s32 i, u16 v);
void NNSi_G3dModifyMatFlag(NNSG3dResMdl *mdl, s32 a, s32 b);
}


extern const u32 sLightFlickerTable[11];
extern const u16 sSceneBuildingTable[52];

// own prototypes
extern "C" {
void BuildingInfo_Copy(BuildingInfo *dst, const BuildingInfo *src);
void BuildingInfo_Destroy(BuildingInfo *i);
BOOL Building_RequestState(BuildingActor *o, s32 v);
}


extern "C" u32 Math_CountBits4(s32 v) {
    u32 n = 0;
    for (u32 i = 0; i < 4; i++) {
        if ((v >> i) & 1) {
            n++;
        }
    }
    return n;
}

extern "C" s32 Item_GetBuildingIndex(u32 arg) {
    u16 v = arg;
    if (Item_IsNookShop(&v)) {
        v = Item_MakeNookShop(0);
    }
    if (Item_IsPlayerHouse(&v)) {
        v = Item_MakePlayerHouse(0);
    }
    BOOL in = FALSE;
    volatile u16 &vv = v;
    u32 w = vv;
    if (vv >= 0x5000 && w <= 0x5021) { in = TRUE; }
    if (in) { return w & 0xfff; }
    return -1;
}

u32 StrBSizeData::getLightUnitCount() {
    s8 *p = (s8 *)this;
    u32 n = 0;
    while (p[0] == 0x4d) {
        p += 4;
        n++;
    }
    return n;
}

u32 StrBSizeData::getClearUnitCount() {
    s8 *p = (s8 *)this;
    u32 n = 0;
    s32 c;
    while ((c = p[0]) == 0x52 || c == 0x4d) {
        p += 4;
        n++;
    }
    return n;
}

u32 StrBSizeData::getFloorUnitCount() {
    s8 *p = (s8 *)this + getClearUnitCount() * 4;
    u32 n = 0;
    while (p[0] == 0x46) {
        p += 4;
        n++;
    }
    return n;
}

u32 StrBSizeData::getSolidUnitCount() {
    u32 a = getClearUnitCount();
    u32 b = getFloorUnitCount();
    s8 *p = (s8 *)this + a * 4 + b * 4;
    u32 n = 0;
    while (p[0] == 0x53) {
        p += 4;
        n++;
    }
    return n;
}

u32 StrBSizeData::getFootprintUnitCount() {
    u32 a = getFloorUnitCount();
    return a + getSolidUnitCount();
}

BOOL StrBSizeData::getLightUnit(s32 *a, s32 *b, u32 idx) {
    if (idx < getLightUnitCount()) {
        return getClearUnit(a, b, idx);
    }
    return FALSE;
}

BOOL StrBSizeData::getClearUnit(s32 *a, s32 *b, u32 idx) {
    if (idx < getClearUnitCount()) {
        s8 *e = (s8 *)this + idx * 4;
        *a = e[2];
        *b = e[3];
        return TRUE;
    }
    return FALSE;
}

BOOL StrBSizeData::getFootprintUnit(s32 *a, s32 *b, u32 idx) {
    s8 *base = (s8 *)this + getClearUnitCount() * 4;
    if (idx < getFootprintUnitCount()) {
        s8 *e = base + idx * 4;
        *a = e[2];
        *b = e[3];
        if (*a == 0 && *b == 0) {
            return FALSE;
        }
        return TRUE;
    }
    return FALSE;
}

BOOL StrBSizeData::getSolidUnit(s32 *a, s32 *b, s32 *c, u32 idx) {
    s8 *base = (s8 *)this + getClearUnitCount() * 4;
    if (idx < getSolidUnitCount()) {
        u32 t = getFloorUnitCount();
        s8 *e = base + (t + idx) * 4;
        *a = e[1];
        *b = e[2];
        *c = e[3];
        return TRUE;
    }
    return FALSE;
}

u32 StrBSizeData::getTriangleCount() {
    u32 a = getFloorUnitCount();
    u32 b = getClearUnitCount();
    u32 c = getSolidUnitCount();
    return *(u32 *)((u8 *)this + (a + (b + c) + 1) * 4);
}

BOOL StrBSizeData::getTriangle(s32 *a, s32 *b, s32 *c, u32 idx) {
    u32 count = getTriangleCount();
    a[0] = 0;
    a[1] = 0;
    a[2] = 0;
    b[0] = 0;
    b[1] = 0;
    b[2] = 0;
    c[0] = 0;
    c[1] = 0;
    c[2] = 0;
    if (idx < count) {
        u32 x = getFloorUnitCount();
        u32 y = getClearUnitCount();
        u32 z = getSolidUnitCount();
        u8 *base = (u8 *)this + ((x + (y + z) + 1) * 4 + 4);
        s32 *e = (s32 *)(base + idx * 0x24);
        a[0] = e[0];
        a[1] = e[1];
        a[2] = e[2];
        b[0] = e[3];
        b[1] = e[4];
        b[2] = e[5];
        c[0] = e[6];
        c[1] = e[7];
        c[2] = e[8];
        return TRUE;
    }
    return FALSE;
}

void StrBSizeData::getSolidBounds(s32 *outX, s32 *outY, s32 *outW, s32 *outH) {
    s32 maxX = 0, minX = 0, maxY = 0, minY = 0;
    u32 count = getSolidUnitCount();
    if (count != 0) {
        u32 i = 0;
        maxX = -1;
        minX = 1;
        maxY = -1;
        minY = 1;
        for (; i < count; i++) {
            s32 ea, ex, ey;
            if (getSolidUnit(&ea, &ex, &ey, i)) {
                if (ex < minX) {
                    minX = ex;
                }
                if (ey < minY) {
                    minY = ey;
                }
                if (ex > maxX) {
                    maxX = ex;
                }
                if (ey > maxY) {
                    maxY = ey;
                }
            }
        }
    }
    s32 x1 = (maxX << 13) + 0x1000;
    s32 x0 = (minX << 13) - 0x1000;
    s32 y1 = (maxY << 13) + 0x1000;
    s32 y0 = (minY << 13) - 0x1000;
    *outX = (x1 + x0) >> 1;
    *outY = (y1 + y0) >> 1;
    s32 w = x1 - x0;
    if (w < 0) {
        w = -w;
    }
    *outW = w;
    s32 h = y1 - y0;
    if (h < 0) {
        h = -h;
    }
    *outH = h;
}

extern "C" void StrBSize_Load(void) {
    char name[0x20];
    NNSFndArchive file;
    u32 i;
    void *p;
    p = File_LoadAlloc("/str/bsize.arc", gCurrentHeap, 4, 0);
    sStrBSizeArchive = p;
    if (p != NULL) {
        if (NNS_FndMountArchive(&file, "STR", p)) {
            for (i = 0; i < 0x22; i++) {
                Str_SPrintf(name, "STR:a/%d.bsize", i);
                sStrBSizeTable[i] = (StrBSizeData *)NNS_FndGetArchiveFileByName(name);
            }
            NNS_FndUnmountArchive(&file);
        }
    } else {
        for (i = 0; i < 0x22; i++) {
            sStrBSizeTable[i] = NULL;
        }
    }
}

extern "C" void StrBSize_Unload(void) {
    for (u32 i = 0; i < 0x22; i++) {
        sStrBSizeTable[i] = NULL;
    }
    if (sStrBSizeArchive != NULL) {
        Mem_Free(sStrBSizeArchive);
        sStrBSizeArchive = NULL;
    }
}

extern "C" StrBSizeData *StrBSize_Get(u16 *p) {
    if (sStrBSizeArchive != NULL) {
        s32 idx;
        BOOL in = FALSE;
        u32 v = *p;
        if (v >= 0x5000 && v <= 0x5021) {
            in = TRUE;
        }
        if (in) {
            idx = v & 0xfff;
        } else {
            idx = -1;
        }
        if (idx != -1) {
            return sStrBSizeTable[idx];
        }
    }
    return 0;
}

extern "C" BOOL FieldStructureMgr_Register(FieldStructureMgr *v) {
    if (sFieldStructureMgr == 0) {
        sFieldStructureMgr = v;
        return TRUE;
    }
    return FALSE;
}

extern "C" BOOL FieldStructureMgr_Unregister(void) {
    if (sFieldStructureMgr != 0) {
        sFieldStructureMgr = 0;
        return TRUE;
    }
    return FALSE;
}

extern "C" FieldStructureMgr *FieldStructureMgr_Get(void) {
    return sFieldStructureMgr;
}

TownUnitShapeQuery::TownUnitShapeQuery() {}

TownUnitShapeQuery::~TownUnitShapeQuery() {}

BOOL TownUnitShapeQuery::getUnitShape(s32 *a, s32 *b, s32 *c, volatile s32 x, volatile s32 y) {
    if (func_020b2768_is_flag()) {
        if (!sFieldObjectShapeQuery.FieldObjectShapeQuery::getUnitShape(a, b, c, x, y)) {
            BlockMap *m = gSceneBlockMap;
            if (m != NULL) {
                s32 lx = x;
                s32 ly = y;
                s32 xh = lx >> 4;
                s32 yh = ly >> 4;
                u16 *p = BlockMap_GetItemPtr(m, xh, yh, lx - (xh << 4), ly - (yh << 4), 0);
                if (p != NULL) {
                    if (*p == 0x500a) {
                        *a = 0xb33;
                        *b = 0x2000;
                        *c = 10;
                        return TRUE;
                    } else if (Item_IsSnowman(p)) {
                        s32 t = Item_GetSnowmanIndex(p);
                        s32 u, w;
                        ((SnowmanRecords *)data_021ed2e6)->getInfo(t, (u32 *)&u, (u32 *)&w, NULL, NULL, NULL, NULL);
                        *a = 0x1000;
                        *b = 0x2000;
                        *c = 0;
                        return TRUE;
                    }
                }
            }
            return FALSE;
        }
        return TRUE;
    } else if (Scene_GetCurrent() == 0x1d && x == 3 && y == 4) {
        *a = 0xe66;
        *b = 0x2000;
        *c = 10;
        return TRUE;
    }
    return FALSE;
}

extern "C" void TownStyleRecord_DestructStub(void *p) {}

extern "C" void TownStyleRecord_ConstructStub(void *p) {}

extern "C" void TownStyle_RollVillagerHouseStyles(u8 *out) {
    s32 a = Random_GlobalBelow(5);
    s32 sel = 0;
    s32 b = Random_GlobalBelow(4);
    s32 n = 0;
    u32 i;
    for (i = 0; i < 5; i++) {
        if ((s32)i == a) {
            continue;
        }
        if (n == b) {
            sel = i;
            break;
        }
        n++;
    }
    for (i = 0; i < 4; i++) {
        out[i] = 0xff;
    }
    u32 k = 0;
    s32 base = a % 5;
    base = base * 5;
    for (; k < 3; k++) {
        s32 skip = Random_GlobalBelow(5 - k);
        s32 cnt = 0;
        for (u32 j = 0; j < 5; j++) {
            BOOL found = FALSE;
            u32 m = 0;
            u32 v = j % 5 + base;
            for (; m < 4; m++) {
                u32 w = out[m];
                if (w == v) {
                    found = TRUE;
                    break;
                }
            }
            if (!found) {
                if (cnt == skip) {
                    out[k] = v;
                    break;
                }
                cnt++;
            }
        }
    }
    u32 r = Random_GlobalBelow(5);
    s32 s = sel % 5;
    u32 q = r % 5;
    s = s * 5;
    out[3] = s + q;
}

extern "C" u32 TownStyle_GetVillagerHouseStyle(u8 *p, u32 idx) {
    u32 v = p[idx & 3];
    if (v >= 25) {
        v = (s32)v % 25;
    }
    return v;
}

TownFlag::~TownFlag() {
    new (this) TownFlagPattern();
}

TownFlag::TownFlag() {
    ((TownFlagPattern *)(this))->~TownFlagPattern();
}

TownFlag *TownFlag::initDefault() {
    gateDesign = Random_GlobalBelow(3);
    TownFlagPattern_InitDefault(this);
    return this;  // (the DS returned what the call left in r0: no caller uses it)
}

s32 TownFlag::getGateDesign() {
    if (gateDesign >= 3) {
        gateDesign = (u32)gateDesign % 3;
        return getGateDesign();
    }
    return gateDesign;
}

extern "C" void TownFlag_SetPattern(void *p, void *src) { Pattern_CopyFields(p, src); }

extern "C" void *TownFlag_GetPattern(void *p) { return TownFlagPattern_GetPattern(p); }

TownStyleRecord::~TownStyleRecord() {
    TownStyleRecord_DestructStub(this);
}

TownStyleRecord::TownStyleRecord() {
    TownStyleRecord_ConstructStub(this);
}

extern "C" void TownStyleRecord_InitNew(u8 *self)
{
    TownStyle_RollVillagerHouseStyles(self);
    ((TownStyleRecord *)self)->townFlag.initDefault();
    s32 j;
    s32 i;
    MapBlock *cell;
    BlockMap *m;
    u16 *p;
    m = TownBlockMap_Get();
    for (u32 y = 1; y <= 4; y++) {
        for (u32 x = 1; x <= 4; x++) {
            if (x < (u32)m->width && y < (u32)m->height && m->blocks) {
                cell = m->blocks + (x + y * m->width);
            } else {
                cell = NULL;
            }
            if (cell) {
                for (j = 0; j < 16; j++) {
                    for (i = 0; i < 16; i++) {
                        p = MapBlock_GetItemPtr(cell, i, j, 0);
                        if (p && InRange(*p)) {
                            s32 ox, oy;
                            FieldUnit_FromBlockUnit(&ox, &oy, x, y, i, j);
                            BlockMap_PutStructure(m, p, ox, oy);
                        }
                    }
                }
            }
        }
    }
}

void *TownStyleRecordView::getHouseStyles() { return this; }

u8 *TownStyleRecordView::getTownFlag() { return (u8 *)&((TownStyleRecord *)this)->townFlag; }

LightLevel::LightLevel()
{
    level = 0;
    targetLevel = 0;
    isFlickering = 0;
}

LightLevel::~LightLevel() {}

BOOL LightLevel::switchLightAnimated(BOOL on) { return switchLight(on, 0, 0, 0x800); }

void LightLevel::update()
{
    if (isFlickering == 1) {
        if (flickerDelay != 0) {
            level = 0;
            flickerDelay--;
        }
        if (flickerDelay == 0) {
            if (flickerIndex < 0xb) {
                level = sLightFlickerTable[flickerIndex];
                flickerIndex++;
            } else {
                isFlickering = 0;
                flickerIndex = 0;
            }
        }
    } else if (targetLevel != level) {
        Math_StepS32(this, targetLevel, fadeStep);
    }
}

BOOL LightLevel::switchLight(BOOL on, s32 a, s32 b, u32 param)
{
    fadeStep = param;
    if (on) {
        if (targetLevel == 0) {
            targetLevel = 0x1000;
            if (a == 0) {
                level = 0x1000;
                return FALSE;
            }
            if (b) {
                isFlickering = 1;
                flickerIndex = 0;
                flickerDelay = 5;
            }
            return TRUE;
        }
    } else {
        isFlickering = 0;
        if (targetLevel != 0) {
            targetLevel = 0;
            if (a == 0) {
                level = 0;
                return FALSE;
            }
            return TRUE;
        }
    }
    return FALSE;
}

s32 Math_LerpFx(s32 t, s32 lo, s32 hi) { return lo + func_01ffcb0c(t, hi - lo); }

s32 LightLevel::getLevel() { return level; }

DoorLight::DoorLight() { matIdx = -1; }

DoorLight::~DoorLight() {}

void DoorLight::bindMaterial(NNSG3dResMdl *c) { matIdx = ((G3dResAccess *)c)->findMatIdx("m_door"); }

void DoorLight::apply(NNSG3dResMdl *c, s32 t)
{
    if (matIdx != -1) {
        NNS_G3dMdlSetMdlDiff(c, matIdx, (u16)((u8)(t * 0xd >> 12) << 10 | ((u8)(t * 0x1f >> 12) | (u8)(t * 0x1b >> 12) << 5)));
    }
}

WindowLight::WindowLight() { matIdx = -1; }

WindowLight::~WindowLight() { matIdx = -1; }

void WindowLight::bindMaterial(NNSG3dResMdl *c) { matIdx = ((G3dResAccess *)c)->findMatIdx("m_window"); }

void WindowLight::apply(NNSG3dResMdl *c, s32 t)
{
    if (matIdx != -1) {
        NNS_G3dMdlSetMdlAlpha(c, matIdx, (u8)((t * 0x1d >> 12) + 1));
    }
}

LampLights::LampLights()
{
    for (u32 i = 0; i < 3; i++) {
        matIndices[i] = -1;
    }
}

LampLights::~LampLights() {}

void LampLights::bindMaterials(NNSG3dResMdl *c)
{
    for (u32 i = 0; i < 3; i++) {
        if (getMaterialName(i)) {
            s32 r = ((G3dResAccess *)c)->findMatIdx(getMaterialName(i));
            if (r != -1) {
                matIndices[i] = r;
            } else {
                matIndices[i] = -1;
            }
        }
    }
}

BOOL LampLights::isLampMaterial(s32 v)
{
    for (s8 *p = matIndices; p < matIndices + 3; p++) {
        if (*p == v) {
            return TRUE;
        }
    }
    return FALSE;
}

void LampLights::apply(NNSG3dResMdl *p, s32 t)
{
    if (t != 0) {
        NNSi_G3dModifyMatFlag(p, 1, 0x400);
        u16 col = Light_GetBaseEmission();
        s32 r7 = Math_LerpFx(t, ((col >> 10) & 0x1f) << 12, 0x1f000);
        s32 g = Math_LerpFx(t, (col & 0x1f) << 12, 0x1f000);
        s32 b = Math_LerpFx(t, ((col >> 5) & 0x1f) << 12, 0x1f000);
        u16 c2 = (r7 >> 12) << 10 | ((g >> 12) | (b >> 12) << 5);
        for (s32 i = 0; i < p->info.numMat; i++) {
            NNS_G3dMdlSetMdlEmi(p, i, isLampMaterial(i) ? c2 : col);
        }
    } else {
        NNSi_G3dModifyMatFlag(p, 0, 0x400);
    }
}

extern "C" u16 Light_GetBaseEmission(void)
{
    return (u16)(data_027e0148[6] >> 16);
}

const char *LampLights::getMaterialName(s32 i)
{
    Str_SPrintf(sLampMatNameBuf, "lp_m%d", i);
    return sLampMatNameBuf;
}

BuildingLights::BuildingLights() {}

BuildingLights::~BuildingLights() {}

void BuildingLights::bind(NNSG3dResMdl *c, BOOL on)
{
    switchLightAnimated(on);
    if (c) {
        doorLight.bindMaterial(c);
        lampLights.bindMaterials(c);
        windowLight.bindMaterial(c);
    }
}

void BuildingLights::updateLights(NNSG3dResMdl *c)
{
    update();
    s32 v = getLevel();
    if (c) {
        lampLights.apply(c, v);
        windowLight.apply(c, v);
        doorLight.apply(c, v);
    }
}

BOOL BuildingLights::setLit(BOOL on, s32 a, s32 b) { return switchLight(on, a, b, 0x800); }

BOOL BuildingLights::isLit() { return getLevel() ? TRUE : FALSE; }

extern "C" void JointCb_RotateClockHourHand(NNSG3dRS *rs)
{
    MtxFx33 tmp;
    u8 t[2];
    MtxFx33 *m = &rs->pJntAnmResult->rot;
    Clock_GetMinuteHour(t);
    if (sHourHandFrame != gFrameCounter) {
        s32 rem = t[1] % 0xc;
        s32 a = (s16)-(FX_Div(rem << 12, 0xc000) * 0xffff >> 12);
        s32 b = (FX_Div(t[0] << 12, 0x3c000) * 0x1555 << 4) >> 16;
        s32 idx = (u16)(s16)(a - b) >> 4;
        sHourHandSin = data_02135f44[idx * 2];
        sHourHandCos = data_02135f44[idx * 2 + 1];
        sHourHandFrame = gFrameCounter;
    }
    MTX_RotZ33_(&tmp, sHourHandSin, sHourHandCos);
    if (rs->pJntAnmResult->flag & 2) {
        *m = tmp;
    } else {
        MTX_Concat33(m, &tmp, m);
    }
    rs->pJntAnmResult->flag &= ~2;
}

extern "C" void JointCb_RotateClockMinuteHand(NNSG3dRS *rs)
{
    MtxFx33 tmp;
    u8 t[2];
    MtxFx33 *m = &rs->pJntAnmResult->rot;
    Clock_GetMinuteHour(t);
    s32 rem = t[0] % 0x3c;
    s32 a = -(FX_Div(rem << 12, 0x3c000) * 0xffff >> 12);
    s32 idx = (u16)(s16)a >> 4;
    MTX_RotZ33_(&tmp, data_02135f44[idx * 2], data_02135f44[idx * 2 + 1]);
    if (rs->pJntAnmResult->flag & 2) {
        *m = tmp;
    } else {
        MTX_Concat33(m, &tmp, m);
    }
    rs->pJntAnmResult->flag &= ~2;
}

extern "C" void BuildingStates_Reset(void)
{
    u32 i;
    u8 v = 0;
    for (i = 0; i < 0x22; i++) {
        sBuildingStates[i] = v;
    }
    sHouseVisitorPresent = v;
}

extern "C" u8 BuildingState_Get(u32 id)
{
    BuildingStateStackPad pad;
    if (InRange32(id)) {
        u32 idx;
        if (InRange32(id)) {
            idx = id & 0xfff;
        } else {
            idx = -1;
        }
        if (idx < 0x22) {
            return sBuildingStates[idx];
        }
    }
    return 0;
}

extern "C" BOOL BuildingState_Set(u32 id, u8 val)
{
    BuildingStateStackPad pad;
    if (InRange32(id)) {
        u32 idx;
        if (InRange32(id)) {
            idx = id & 0xfff;
        } else {
            idx = -1;
        }
        if (idx < 0x22) {
            sBuildingStates[idx] = val;
            return TRUE;
        }
    }
    return FALSE;
}

extern "C" u16 *Town_GetEventPlotItem(s32 *outX, s32 *outY)
{
    s32 x, y;
    BlockMap *m = TownBlockMap_Get();
    s32 found = 0;
    *outX = 0;
    *outY = 0;
    if (m) {
        s32 w = m->width;
        s32 h = m->height;
        for (y = 0; y < h; y++) {
            for (x = 0; x < w; x++) {
                if (BlockMap_GetBlockAttr(m, x, y) & 0x200) {
                    found = 1;
                    break;
                }
            }
            if (found) {
                break;
            }
        }
        if (found) {
            MapBlock *cell;
            FieldUnit_FromBlockUnit(outX, outY, x, y, 10, 9);
            if ((u32)x < (u32)m->width && (u32)y < (u32)m->height && m->blocks) {
                cell = m->blocks + (x + y * m->width);
            } else {
                cell = NULL;
            }
            if (cell) {
                return MapBlock_GetItemPtr(cell, 10, 9, 0);
            }
        }
    }
    return NULL;
}

extern "C" void TownStructure_PlaceAtEventPlot(u16 id)
{
    u16 t[1];
    t[0] = id;
    if (InRangeV(t[0])) {
        s32 x, y;
        u16 *p = Town_GetEventPlotItem(&x, &y);
        if (p) {
            BlockMap *m = TownBlockMap_Get();
            if (m) {
                if (InRange(*p)) {
                    BlockMap_RemoveStructure(m, x, y, NULL);
                }
                BlockMap_PutStructure(m, t, x, y);
            }
        }
    }
}

extern "C" BOOL TownStructure_RemoveFromEventPlot(u16 id)
{
    u16 t[2];
    t[0] = id;
    if (InRangeV(t[0])) {
        s32 x, y;
        u16 *p = Town_GetEventPlotItem(&x, &y);
        if (p) {
            BOOL eq;
            if (Item_IsFurniture(p)) {
                t[1] = id;
                eq = Item_GetFurnitureIndex(p) == Item_GetFurnitureIndex(&t[1]);
            } else {
                eq = *p == id;
            }
            if (eq) {
                BlockMap *m = TownBlockMap_Get();
                if (m) {
                    return BlockMap_RemoveStructure(m, x, y, NULL);
                }
            }
        }
    }
    return FALSE;
}

extern "C" void Town_PlaceReddTent(void) { TownStructure_PlaceAtEventPlot(0x5012); }

extern "C" BOOL Town_RemoveReddTent(void) { return TownStructure_RemoveFromEventPlot(0x5012); }

extern "C" void Town_PlaceKatrinaTent(void) { TownStructure_PlaceAtEventPlot(0x5013); }

extern "C" BOOL Town_RemoveKatrinaTent(void) { return TownStructure_RemoveFromEventPlot(0x5013); }

extern "C" void Town_PlaceGracieCar(void) { TownStructure_PlaceAtEventPlot(0x5021); }

extern "C" BOOL Town_RemoveGracieCar(void) { return TownStructure_RemoveFromEventPlot(0x5021); }

extern "C" void Town_PlaceCountdownSign(void) { TownStructure_PlaceAtEventPlot(0x501e); }

extern "C" BOOL Town_RemoveCountdownSign(void) { return TownStructure_RemoveFromEventPlot(0x501e); }

extern "C" u32 MapBlock_CountSigns(MapBlock *cell) {
    u32 count = 0;
    u32 x, y;
    if (cell) {
        if (MapBlock_HasAnyAttr(cell, 0xe0) || MapBlock_HasAnyAttr(cell, 0x10)) {
            return 0;
        }
        for (y = 0; y < 16; y++) {
            for (x = 0; x < 16; x++) {
                u16 *p = MapBlock_GetItemPtr(cell, x, y, 0);
                if (p && *p == 0x500a) {
                    count++;
                }
            }
        }
    }
    return count;
}

extern "C" u32 MapBlock_ReplaceNthSign(MapBlock *cell, u32 target, u32 v) {
    u32 count = 0;
    u32 y, x;
    if (cell) {
        if (MapBlock_HasAnyAttr(cell, 0xe0) || MapBlock_HasAnyAttr(cell, 0x10)) {
            return 0;
        }
        for (y = 0; y < 16; y++) {
            for (x = 0; x < 16; x++) {
                u16 *p = MapBlock_GetItemPtr(cell, x, y, 0);
                if (p && *p == 0x500a) {
                    if (target == count) {
                        u16 t = v;
                        if (MapBlock_SetItem(cell, &t, x, y, 0)) {
                            return 1;
                        }
                    }
                    count++;
                }
            }
        }
    }
    return 0;
}

extern "C" BOOL MapBlock_ReplaceRandomSign(MapBlock *cell, u32 v) {
    u32 n = MapBlock_CountSigns(cell);
    if (n) {
        return MapBlock_ReplaceNthSign(cell, Random_GlobalBelow(n), v);
    }
    return 0;
}

extern "C" BOOL Town_PlaceGulliverShip(s32 *pos, BOOL flag) {
    BlockMap *g = TownBlockMap_Get();
    s32 x, y, i, j;
    if (g == NULL) {
        return FALSE;
    }
    x = pos[0] >> 17;
    y = pos[2] >> 17;
    if (flag) {
        for (i = x - 1; i >= 1; i--) {
            for (j = 1; j <= 4; j++) {
                if (MapBlock_ReplaceRandomSign(GetCell(g, i, j), 0x5020)) {
                    GulliverQuest_Init(&data_021e58a6);
                    TownUpdater_MarkEventApplied(0x44);
                    return TRUE;
                }
            }
        }
    } else {
        for (i = x + 1; i <= 4; i++) {
            for (j = 1; j <= 4; j++) {
                if (MapBlock_ReplaceRandomSign(GetCell(g, i, j), 0x5020)) {
                    GulliverQuest_Init(&data_021e58a6);
                    TownUpdater_MarkEventApplied(0x44);
                    return TRUE;
                }
            }
        }
    }
    for (i = 1; i <= 4; i++) {
        if (i != y) {
            if (MapBlock_ReplaceRandomSign(GetCell(g, x, i), 0x5020)) {
                GulliverQuest_Init(&data_021e58a6);
                TownUpdater_MarkEventApplied(0x44);
                return TRUE;
            }
        }
    }
    if (flag) {
        for (i = x + 1; i <= 4; i++) {
            for (j = 1; j <= 4; j++) {
                if (MapBlock_ReplaceRandomSign(GetCell(g, i, j), 0x5020)) {
                    GulliverQuest_Init(&data_021e58a6);
                    TownUpdater_MarkEventApplied(0x44);
                    return TRUE;
                }
            }
        }
    } else {
        for (i = x - 1; i >= 1; i--) {
            for (j = 1; j <= 4; j++) {
                if (MapBlock_ReplaceRandomSign(GetCell(g, i, j), 0x5020)) {
                    GulliverQuest_Init(&data_021e58a6);
                    TownUpdater_MarkEventApplied(0x44);
                    return TRUE;
                }
            }
        }
    }
    if (MapBlock_ReplaceRandomSign(GetCell(g, x, y), 0x5020)) {
        GulliverQuest_Init(&data_021e58a6);
        TownUpdater_MarkEventApplied(0x44);
        return TRUE;
    }
    return FALSE;
}


extern "C" BOOL Town_RemoveGulliverShip(void) {
    BlockMap *g = TownBlockMap_Get();
    s32 y, x;
    if (g == NULL) {
        return FALSE;
    }
    static ItemId m1(0x5020);
    for (y = 1; y <= 4; y++) {
        for (x = 1; x <= 4; x++) {
            MapBlock *cell = GetCell(g, x, y);
            if (cell) {
                u32 a, b;
                if (MapBlock_FindItemInRange(cell, &a, &b, &m1, &m1, 0)) {
                    static Marker2 m2(0x500a);
                    if (BlockMap_RemoveStructureAt(g, x, y, a, b, &m2.id)) {
                        Town_RemoveShipParts();
                        return TRUE;
                    }
                }
            }
        }
    }
    return FALSE;
}

extern "C" void BuildingInfo_Copy(BuildingInfo *dst, const BuildingInfo *src) {
    dst->profile = src->profile;
    dst->entranceType = src->entranceType;
    dst->kind = src->kind;
    dst->interiorScene = src->interiorScene;
    dst->flickeringLights = src->flickeringLights;
    dst->viewRangeX = src->viewRangeX;
    dst->viewRangeFront = src->viewRangeFront;
    dst->viewRangeBack = src->viewRangeBack;
    dst->capacity = src->capacity;
}

extern "C" void BuildingInfo_Destroy(BuildingInfo *i) {}

extern "C" u16 BuildingInfo_GetProfile(BuildingInfo *i) { return i->profile; }

extern "C" u8 BuildingInfo_GetEntranceType(BuildingInfo *i) { return i->entranceType; }

extern "C" u8 BuildingInfo_GetKind(BuildingInfo *i) { return i->kind; }

extern "C" s8 BuildingInfo_GetInteriorScene(BuildingInfo *i) { return i->interiorScene; }

extern "C" u8 BuildingInfo_GetFlickeringLights(BuildingInfo *i) { return i->flickeringLights; }

extern "C" u8 BuildingInfo_GetViewRangeX(BuildingInfo *i) { return i->viewRangeX; }

extern "C" u8 BuildingInfo_GetViewRangeFront(BuildingInfo *i) { return i->viewRangeFront; }

extern "C" u8 BuildingInfo_GetViewRangeBack(BuildingInfo *i) { return i->viewRangeBack; }

extern "C" u8 BuildingInfo_GetCapacity(BuildingInfo *i) { return i->capacity; }

extern "C" u32 BuildingInfo_MakeKey(BuildingInfo *i, u32 b, u32 c) { return BuildingKey_Make(BuildingInfo_GetProfile(i), b, c); }

extern "C" u32 BuildingKey_Make(u32 a, u32 b, u32 c) { return (a << 16) | (((c & 0xff) << 8) | (b & 0xff)); }

extern "C" u32 Building_GetDoorAnimParamAt(VecFx32 *a) {
    BuildingActor *o;
    if (!IsEnabled()) {
        return 1;
    }
    o = Building_FindNearPos(a);
    if (o) {
        if (o->getEntranceType() == 1) {
            u16 *i = static_cast<u16 *>(o->getDoorInAnim());
            if (i) {
                return i[2];
            }
            return 1;
        }
        return 1;
    }
    return 1;
}

extern "C" BOOL Field_HasExitedBuildingKey(void) {
    if (!Scene_InTown()) {
        return TRUE;
    }
    if (sExitedBuildingKey != 0) {
        return TRUE;
    }
    return FALSE;
}

extern "C" void Field_CacheExitedBuildingKey(void) {
    sExitedBuildingKey = Field_GetExitedBuildingKey();
}

extern "C" u32 Field_GetExitedBuildingKey(void) {
    s32 x, y;
    u16 *p;
    if (!Scene_InTown() || Taxi_IsArriving()) {
        return 0;
    }
    if (IsEnabled()) {
        Scene_GetWarpRequest();
        x = ScenePos_GetUnitX(&gTownReturnPos);
        Scene_GetWarpRequest();
        y = ScenePos_GetUnitZ(&gTownReturnPos);
        if (gSceneBlockMap) {
            s32 xh = x >> 4;
            s32 yh = y >> 4;
            p = BlockMap_GetItemPtr(gSceneBlockMap, xh, yh, x - (xh << 4), y - (yh << 4), 0);
            if (p) {
                BOOL in = FALSE;
                u16 v = *p;
                if (v >= 0x5000 && v <= 0x5021) {
                    in = TRUE;
                }
                if (in) {
                    u32 i;
                    BuildingInfo *src;
                    BuildingInfo info;
                    u32 result;
                    if (v >= 0x5000 && v <= 0x5021) {
                        i = v & 0xfff;
                    } else {
                        i = -1;
                    }
                    if (i < 0x22) {
                        src = &data_020d0a7c[i];
                    } else {
                        src = &data_020d0a7c[0];
                    }
                    BuildingInfo_Copy(&info, src);
                    result = BuildingInfo_MakeKey(&info, x, y);
                    BuildingInfo_Destroy(&info);
                    return result;
                }
            }
        }
    }
    return 0;
}

extern "C" BOOL Building_RequestState(BuildingActor *o, s32 v) {
    if (IsEnabled()) {
        if (o->setDoorState(v)) {
            if (((CommManager *)(gCommManager))->isOnline()) {
                u16 f;
                void *d;
                f = (f & ~0x3f) | (o->getItemId()[0] & 0x3f);
                f = (f & ~0xfc0) | ((v & 0x3f) << 6);
                f &= ~0xf000;
                d = gCommManager;
                ((CommManager *)(d))->beginRecord();
                ((CommManager *)(d))->writeRecord((u8 *)(&f), 2);
                ((CommManager *)(d))->endRecord(0x22, 4);
            }
            return TRUE;
        }
    }
    return FALSE;
}

extern "C" u32 SaveVillager_IsAsleepAt(s32 v) {
    u8 *villager = SaveVillagers_Get(&gSaveVillagers, v);
    if (villager) {
        if (Villager_GetWhereabouts(villager) == 2) {
            return TRUE;
        }
        return FALSE;
    }
    return FALSE;
}

extern "C" u32 Building_IsOwnerAsleep(u32 v) {
    volatile u16 x = v;
    BOOL in = FALSE;
    u16 y = x;
    if (y >= 0x5001 && y <= 0x5008) {
        in = TRUE;
    }
    if (in) {
        s32 i;
        if (y >= 0x5001 && y <= 0x5008) {
            i = y - 0x5001;
        } else {
            i = -1;
        }
        return SaveVillager_IsAsleepAt(i);
    }
    return 0;
}

extern "C" BOOL Building_ApplyStateRecord(u16 *p) {
    Flags2 *f = (Flags2 *)p;
    u32 a = Item_MakeBuilding(f->a);
    u8 b = f->b;
    if (IsEnabled()) {
        BuildingActor *o = (BuildingActor *)Shared_BuildingList_FindByItem(a);
        if (o) {
            return o->setDoorState(b);
        }
    }
    return BuildingState_Set(a, b);
}

extern "C" void BuildingOccupancy_OnEnterRecord(Flags1 *p, s32 bit) {
    u32 mode = p->mode;
    u32 idx = p->idx;
    switch (mode) {
    case 1:
    case 2:
    case 3:
        sBuildingOccupancy[idx].hi = mode;
        break;
    default: {
        NibblePair *e = &sBuildingOccupancy[idx];
        u8 lo = e->lo;
        u32 before = Math_CountBits4(lo);
        u32 after;
        void *d;
        Flags1 pk;
        BuildingInfo info;
        BuildingInfo *src;
        lo = lo | (1 << bit);
        after = Math_CountBits4(lo);
        if (idx < 0x22) {
            src = &data_020d0a7c[idx];
        } else {
            src = &data_020d0a7c[0];
        }
        BuildingInfo_Copy(&info, src);
        if (after > BuildingInfo_GetCapacity(&info)) {
            e->hi = 2;
        } else {
            idx = Item_MakeBuilding(idx);
            if (Building_IsOwnerAsleep(idx) && before == 0) {
                e->hi = 3;
            } else {
                e->lo = lo;
                e->hi = 1;
                Building_IsOwnerAsleep(idx);
            }
        }
        pk = *p;
        pk.mode = e->hi;
        d = gCommManager;
        ((CommManager *)(d))->beginRecord();
        ((CommManager *)(d))->writeRecord((u8 *)(&pk), 1);
        ((CommManager *)(d))->endRecord(0x23, bit);
        BuildingInfo_Destroy(&info);
    }
    }
}

extern "C" void BuildingOccupancy_OnLeaveRecord(u8 *p, s32 bit) {
    sBuildingOccupancy[*p].lo = sBuildingOccupancy[*p].lo & ~(1 << bit);
}

extern "C" void BuildingOccupancy_Reset(void) {
    u8 *p;
    for (p = (u8 *)sBuildingOccupancy; p < (u8 *)(sBuildingOccupancy + 0x22); p++) {
        *p &= ~0xf;
        *p &= ~0xf0;
    }
    sHouseVisitorPresent = 0;
}

extern "C" void BuildingOccupancy_RequestEnter(u32 arg) {
    Bits bits;
    u16 buf[5];
    u32 idx = Item_GetBuildingIndex(arg);
    void *p = gCommManager;
    if (!((CommManager *)(p))->isOnline() || ((CommManager *)(p))->isMyAid(0)) {
        NibblePair *e = sBuildingOccupancy + idx;
        u8 lo;
        u32 y, z;
        lo = e->lo;
        z = Math_CountBits4(lo);
        lo = (u8)(lo | 1);
        y = Math_CountBits4(lo);
        BuildingInfo_Copy((BuildingInfo *)buf, idx < 0x22 ? &data_020d0a7c[idx] : data_020d0a7c);
        if (y > BuildingInfo_GetCapacity((BuildingInfo *)buf)) {
            e->hi = 2;
        } else if (Building_IsOwnerAsleep(arg) && z == 0) {
            e->hi = 3;
        } else {
            e->lo = lo;
            e->hi = 1;
            Building_IsOwnerAsleep(arg);
        }
        BuildingInfo_Destroy((BuildingInfo *)buf);
    } else {
        sBuildingOccupancy[idx].hi = 0;
        bits.a = 0;
        bits.b = idx;
        p = gCommManager;
        ((CommManager *)(p))->beginRecord();
        ((CommManager *)(p))->writeRecord((u8 *)(&bits), 1);
        ((CommManager *)(p))->endRecord(0x23, 0);
    }
}

extern "C" u8 BuildingOccupancy_GetAnswer(u32 a) {
    return sBuildingOccupancy[Item_GetBuildingIndex(a)].hi;
}

extern "C" void BuildingOccupancy_Leave(u32 a, u32 b) {
    u32 i = Item_GetBuildingIndex(a);
    void *p = gCommManager;
    if (!((CommManager *)(p))->isOnline() || ((CommManager *)(p))->isMyAid(0) || ((CommManager *)(p))->isMyAid(4)) {
        sBuildingOccupancy[i].lo &= ~(1 << b);
    } else {
        u8 x = i;
        p = gCommManager;
        ((CommManager *)(p))->beginRecord();
        ((CommManager *)(p))->writeRecord((u8 *)(&x), 1);
        ((CommManager *)(p))->endRecord(0x24, 0);
    }
}

extern "C" u8 HouseVisitor_IsPresent(void) {
    return sHouseVisitorPresent;
}

extern "C" void HouseVisitor_SetPresent(void) {
    sHouseVisitorPresent = 1;
}

extern "C" void HouseVisitor_ClearPresent(void) {
    sHouseVisitorPresent = 0;
}

extern "C" u32 BuildingOccupancy_GetPlayersForScene(u32 a) {
    void *p = gCommManager;
    if (!((CommManager *)(p))->isOnline() || ((CommManager *)(p))->isMyAid(0)) {
        volatile u16 id = ((u16 *)sSceneBuildingTable)[a];
        BOOL ok = FALSE;
        u16 v = id;
        if (v < 0x5000 || v > 0x5021) {
        } else {
            ok = TRUE;
        }
        if (ok) {
            return sBuildingOccupancy[Item_GetBuildingIndex(v)].lo;
        }
    }
    return 0;
}

extern "C" u32 BuildingOccupancy_CountForScene(u32 a) {
    u32 r = Math_CountBits4(BuildingOccupancy_GetPlayersForScene(a));
    if (SceneId_IsHouseRoom(a) && HouseVisitor_IsPresent()) {
        return r + 2;
    }
    return r;
}

extern "C" u32 Room_CountOccupants(void) {
    if (isFlag1()) {
        return BuildingOccupancy_CountForScene(Scene_GetCurrent());
    }
    return 0;
}

extern "C" void Taxi_SetLeaving(void) {
    sTaxiLeaving = 1;
}

extern "C" void Taxi_ClearLeaving(void) {
    sTaxiLeaving = 0;
}

extern "C" u8 Taxi_IsLeaving(void) {
    return sTaxiLeaving;
}

extern "C" void Taxi_SetArriving(void) {
    sTaxiArriving = 1;
}

extern "C" void Taxi_ClearArriving(void) {
    sTaxiArriving = 0;
}

extern "C" u8 Taxi_IsArriving(void) {
    return sTaxiArriving;
}

extern "C" void Building_SetLastEntranceType(u32 a) {
    sLastEntranceType = a;
}

extern "C" u32 Building_GetLastEntranceType(void) {
    return sLastEntranceType;
}


s32 sHourHandFrame;
u32 sLastEntranceType;
void *sStrBSizeArchive;
u8 sTaxiLeaving;
u32 sExitedBuildingKey;
const u16 sSceneBuildingTable[52] = {
    0xfff1, 0x5014, 0x5014, 0x5014, 0x5014, 0x5014, 0x5014, 0x5014, 0x5014, 0x5000, 0x500c, 0x500b, 0x500b,
    0x500b, 0x500b, 0x5012, 0x5013, 0x5001, 0x5002, 0x5003, 0x5004, 0x5005, 0x5006, 0x5007, 0x5008, 0x5009,
    0x500d, 0x500e, 0x500f, 0x5010, 0x5010, 0x5010, 0x5011, 0x5011, 0x5011, 0x5011, 0x5011, 0x5011, 0x5011,
    0x5011, 0x5011, 0x5011, 0xfff1, 0xfff1, 0xfff1, 0xfff1, 0xfff1, 0x500b, 0xfff1, 0xfff1, 0xfff1, 0x0000};
const u32 sLightFlickerTable[11] = {0x0, 0x19a, 0x333, 0x4cd, 0x333, 0x19a, 0x4cd, 0x666, 0x800, 0x666, 0x4cd};
NibblePair sBuildingOccupancy[0x22];
char sLampMatNameBuf[12];
FieldObjectShapeQuery sFieldObjectShapeQuery;

extern "C" void Building_PlayDoorChime(void) {
    u32 a = Scene_GetCurrent();
    s32 r = -1;
    if (SceneId_IsHouseRoom(a) || SceneId_IsVillagerHouse(a)) {
        r = 0;
    } else if (a == 0x1a) {
        r = 1;
    } else if (a == 9) {
        r = 2;
    } else if (a == 10) {
        r = 3;
    }
    if (r != -1) {
        static FxVec3 obj(0x10000, 0, 0x1e000);
        Melody_PlayAt(&obj, r);
    }
}



u8 sBuildingStates[0x22];
FieldStructureMgr *sFieldStructureMgr;
u8 sHouseVisitorPresent;
s32 sHourHandCos = 0x1000;
StrBSizeData *sStrBSizeTable[0x22];
u8 sTaxiArriving;
s32 sHourHandSin;

#ifdef HOST64
// (modern/host/frontend.c's ACWW_LIGHTS_TEST: a building's lights switched on at once, whatever the hour)
extern "C" void *Shared_BuildingList_FindByItem(unsigned int a0);
extern "C" int Building_HostSetLit(u32 item) {
    BuildingActor *b = (BuildingActor *)Shared_BuildingList_FindByItem(item);
    if (b == NULL) return 0;
    b->lights.setLit(TRUE, 0, 0);
    return 1;
}
#endif
