// mwcc-version: 1.2/sp2
#include "types.h"
#include "host_shared_calls.h"
#include "host_overlay_vars.h"
#include "gfx/Camera.h"
#include "gfx/VecFx32.h"
#include "game/Unk_0202f2ac_V3.h"
#include "actor/CharacterListNode.h"
#include "snd/BgmSceneFade.h"
#include "game/StrBSizeData.h"
#include "town/BuildingResources.h"
#include "gfx/Mtx43.h"
#include "town/Unk_ov009_0225b880.h"
#include "game/TouchPicker.h"
#include "sys/ProcBase.h"
#include "talk/MsgRequest.h"
#include "gfx/ObjShadowStrip.h"
#include "town/BuildingShadowTable.h"
#include "actor/Actor.h"
#include "actor/Character.h"
#include "snd/BgmManager.h"
#include "snd/SndSeEmitter.h"
#include "snd/Unk_0213b9c4.h"
#include "talk/TalkMsgRequest.h"
#include "talk/TalkWindowState.h"
#include "snd/BuildingSeEmitter.h"
#include "town/BuildingActor.h"
#include "game/TriangleTrigger.h"
#include "town/BuildingCollider.h"
#include "game/TouchPickTriangle.h"
#include "game/CollisionTriangle.h"
#include "gfx/DebugColor.h"
#include "actor/ActorProfile.h"
#include "gfx/AnimFrameCtrl.h"
#include "gfx/AnimModel.h"
#include "gfx/Model.h"
#include "gfx/TwoLayerAnimModel.h"
#include "proto/autoload_2/unk_020e8558.h"
#include "proto/autoload_2/unk_020e92f4.h"
#include "proto/autoload_2/unk_02101048.h"
#include "proto/main/unk_020039ec.h"
#include "proto/main/unk_0203e7d0.h"
#include "proto/main/unk_0203ecec.h"
#include "proto/main/unk_020943dc.h"
#include "proto/itcm/unk_01ffcfc0.h"
#include "proto/main/unk_0205b7b0.h"
#include "proto/main/unk_02063904.h"
#include "proto/main/unk_02063f3c.h"
#include "proto/main/unk_0206d988.h"
#include "proto/main/unk_0209c08c.h"
#include "proto/main/unk_02094810.h"
#include "proto/main/unk_020b0e60.h"
#include "proto/main/unk_020b5e5c.h"
#include "proto/ov003/unk_ov003_02204ce8.h"
#include "proto/ov003/unk_ov003_022187f8.h"
#include "proto/ov003/unk_ov003_022187fc.h"
#include "proto/ov009/unk_ov009_0225b880.h"
#include "proto/main/unk_020b4828.h"

typedef void (BuildingActor::*Unk_ov009_0225c290_Fn)();
typedef BOOL (BuildingActor::*Unk_ov009_0225c360_Fn)();

extern "C" {
extern char sBuildingDefaultMsgFile[];
extern char data_ov009_0225e3e8[];
extern char data_ov009_0225e3ec[];
extern char data_ov009_0225e3fc[];
extern char data_ov009_0225e40c[];
extern char data_ov009_0225e41c[];
extern char data_ov009_0225e42c[];
extern char data_ov009_0225e43c[];
extern char data_ov009_0225e44c[];
extern char data_ov009_0225e45c[];
extern char sBuildingLightTexPathFmt[];
extern char sBuildingTexPathFmt[];
extern char sBuildingArcPathFmt[];
extern char sBuildingArcPath[];
extern char sBuildingTexPath[];
extern char sBuildingLightTexPath[];
extern BuildingResources sBuildingResources[];
extern Camera *gCamera;
extern VecFx32 gCameraLookAt;
extern BuildingInfo data_020d0a7c[0x22];
extern BgmManager *data_021c1b3c;

// (definitions in files whose types are local: include/proto/.excluded)
void BuildingInfo_Copy(BuildingInfo *dst, const BuildingInfo *src);
void BuildingInfo_Destroy(BuildingInfo *i);
s32 func_01ffcb0c(s32, s32);
// (src/autoload_2/unk_020e7500.cpp; its header's List_Remove conflicts with the one actor/Actor.h declares)
s16 Math_Atan2(s32 x, s32 y);
s32 Math_AngleDiffAbs(s32 a, s32 b);
BOOL Item_IsFurniture(u16 *);
s32 Item_GetFurnitureIndex(u16 *);
void Camera_SetMode3(void);
BOOL TalkRequest_SetTargetDone(Character *c);  // (defined with u32: it is the Character)
s32 Scene_GetCurrent(void);
s32 Ground_GetDefaultY(void);
BOOL Item_IsNookShop(u16 *);
BOOL TalkRequestFlags_IsResetti(void);
BOOL TalkRequest_AddPlayerTalk6(Character *c, s32 b);  // (likewise)
s32 TriangleTrigger_Unregister(TriangleTrigger *node);
s32 TriangleTrigger_Register(TriangleTrigger *node);
BOOL Gfx3d_LoadTex(void *tex, void *slot);
BOOL Gfx3d_LoadTexAndPltt(void *tex, void *slot);
void *Gfx3d_CopyTex(void *tex, void *heap);
u16 Item_MakeBuilding(u32 x);
void Clock_GetMinuteHour(u8 *out);
void NookShop_SetVisitState(int v);
BOOL Camera_IsBlockingFocusView(void *pos, s32 w, s32 h);

// SDK (NitroSystem / NitroSDK)
void Mtx43_SetTranslate(Mtx43 *m, s32 a, s32 b, s32 c);
void Mtx43_RotateX(Mtx43 *m, s32 a);
s32 NNS_FndMountArchive(void *archive, const char *name, void *data);
void NNS_FndUnmountArchive(void *archive);
void *func_02106654(void *p);
void *func_02106670(void *p, s32 a);
void *func_02106690(void *p);
void *func_021066ac(void *p, s32 a);
void *NNS_G3dGetTex(void *p);
void NNS_G3dBindMdlPltt(void *, void *);
void NNS_G3dBindMdlTex(void *, void *);
void *func_021065dc(void *p);
void *func_021065f8(void *, u32);
void *NNS_G3dGetMdlSet(void *p);
void MTX_MultVec43(const VecFx32 *, const Mtx43 *, VecFx32 *);

BOOL Building_IsNight();
void *BuildingActor_Create();
BOOL BuildingResources_IsLoaded(BuildingResources *e);
void *Building_GetFirstAnm(void *res);
NNSG3dResMdl *Building_GetFirstMdl(void *res);
void Building_InitModelCallback(NNSG3dRS *o);
void Building_ModelCallback(struct NNSG3dRS *a);
}

static inline BOOL Unk_ov009_0225d0d8_Match(u16 *p, u32 v) {
    BOOL r;
    if (Item_IsFurniture(p)) {
        u16 t;
        t = v;
        s32 a = Item_GetFurnitureIndex(p);
        s32 b = Item_GetFurnitureIndex(&t);
        if (a == b) {
            r = TRUE;
        } else {
            r = FALSE;
        }
    } else {
        if (*p == v) {
            r = TRUE;
        } else {
            r = FALSE;
        }
    }
    return r;
}

static inline BOOL Unk_ov009_0225cc24_IsNine(u16 v) {
    if (v == 9) {
        return TRUE;
    }
    return FALSE;
}

static inline BOOL Unk_ov009_0225d858_Is(u16 *p, u32 v) {
    if (Item_IsFurniture(p)) {
        u16 t = v;
        s32 a = Item_GetFurnitureIndex(p);
        s32 b = Item_GetFurnitureIndex(&t);
        if (a == b) {
            return TRUE;
        }
        return FALSE;
    }
    if (*p == v) {
        return TRUE;
    }
    return FALSE;
}

extern "C" void *BuildingActor_Create() {
    return new BuildingActor();
}

extern "C" void Building_LocalToWorld(VecFx32 *p, const VecFx32 *a, const Mtx43 *b) {
    VecFx32 v;
    MTX_MultVec43(a, b, &v);
    WorldCurve_FromCurved(p, &v);
}

BuildingResources::BuildingResources() {
    u32 i;
    bmd0 = 0;
    bmd1 = 0;
    tex = 0;
    lightTex = 0;
    shadowTable = 0;
    bca0 = 0;
    bca1 = 0;
    bca2 = 0;
    solidCenterX = 0;
    solidCenterZ = 0;
    solidSizeX = 0;
    solidSizeZ = 0;
    for (i = 0; i < 4; i++) {
        btaAnims[i] = 0;
    }
}

BuildingResources::~BuildingResources() {}

extern "C" {
DebugColor data_ov009_0225e4fc(31, 20, 20, 31);
DebugColor data_ov009_0225e4e0(20, 20, 31, 31);
DebugColor data_ov009_0225e4f4(31, 31, 20, 31);
DebugColor data_ov009_0225e4f0(20, 31, 20, 31);
DebugColor data_ov009_0225e500(20, 31, 31, 31);
DebugColor data_ov009_0225e4f8(20, 24, 24, 31);
BuildingResources sBuildingResources[0x22];
}

extern "C" BOOL BuildingResources_IsLoaded(BuildingResources *e) {
    if (e->bmd0 != 0 || e->bmd1 != 0 || e->tex != 0 || e->lightTex != 0 || e->shadowTable != 0 || e->bca0 != 0 ||
        e->bca1 != 0 || e->bca2 != 0) {
        return TRUE;
    }
    return FALSE;
}

void BuildingActor::onJointCalcPost(u32 a, void *b) {
}

extern "C" void Building_ModelCallback(NNSG3dRS *a) {
    BuildingActor *o = (BuildingActor *)a->pRenderObj->ptrUser;
    if (o != NULL) {
        o->onJointCalcPost(a->c[1], a);
    }
}

extern "C" void Building_InitModelCallback(NNSG3dRS *o) {
    o->cbVecFunc[6] = (void *)Building_ModelCallback;
    o->cbVecTiming[6] = 2;
}

extern "C" NNSG3dResMdl *Building_GetFirstMdl(void *res) {
    u8 *p = (u8 *)NNS_G3dGetMdlSet(res);
    return (NNSG3dResMdl *)(p + *(s32 *)(p + *(u16 *)(p + 0xe) + 0xc));
}

extern "C" void *Building_GetFirstAnm(void *res) {
    void *p = func_021065dc(res);
    return func_021065f8(p, 0);
}

BuildingActor::BuildingActor() {
    itemId = 0xfff1;
}

BuildingActor::~BuildingActor() {
}

BOOL BuildingActor::initBuilding() { return TRUE; }

BOOL BuildingActor::onCreate() {
    Mtx43 b1;
    Mtx43 b2;
    Mtx43 m;
    VecFx32 v;
    BuildingList_Add(this);
    gridX = position.x >> 13;
    gridZ = position.z >> 13;
    setCharId((u16)(((gridZ & 0xff) << 8) | (gridX & 0xff)));
    itemId = param;  // (ProcBase::param: the spawn parameter)
    buildingIndex = itemId & 0xfff;
    char *a = getArcPath();
    char *bb = getTexPath();
    char *c = getLightTexPath();
    setupModel(a, bb, c);
    initEntryArea();
    setupAnims();
    updateBaseMatrix(&b1);
    model.setInitCallback(Building_InitModelCallback, this);
    b2 = b1;
    createShadows(&b2);
    s32 ang = WorldCurve_Apply(&v, &position);
    Mtx43_SetTranslate(&m, v.x, v.y, v.z);
    Mtx43_RotateX(&m, ang);
    createColliders(&m);
    BuildingResources *r = getResources();
    lights.bind(r ? r->bmd0 : NULL, 1);
    setInteractionRange(0);
    if (getDoorPos(&entryPos, (s16 *)0)) {
        entryPos.z -= 0x4000;
    }
    BOOL res = initBuilding();
    setEntryState(0);
    return res;
}

BOOL BuildingActor::preExecute() {
    if (!Character::preExecute()) {
        return FALSE;
    }
    seEmitter.activate();
    u16 *p = getItemId();
    if (Unk_ov009_0225d858_Is(p, 0x501d)) {
        s32 t = BuildingState_Get(itemId);
        if (doorState != t) {
            setDoorState(t);
        }
    }
    updateOffscreen();
    if (Scene_GetCurrent() != 0x2c) {
        updateEntryState();
    }
    updateDoorState();
    if (Scene_GetCurrent() != 0x2c) {
        submitColliders();
    }
    BOOL on = areLightsOn();
    s32 b = hasFlickeringLights();
    lights.setLit(on, 1, b);
    lights.updateLights(model.resMdl);
    updateDoorExit();
    return TRUE;
}

BOOL BuildingActor::postExecute(u32 a) {
    VecFx32Ctor v = getSoundPos();
    u16 *pp = getItemId();
    seEmitter.setPosition(&v);
    if (colliderFlags & 2) {
        colliderFlags |= 8;
    } else {
        colliderFlags &= ~8;
    }
    colliderFlags &= ~2;
    colliderFlags &= ~4;
    return Actor::postExecute(a);
}

BOOL BuildingActor::preDraw() {
    if (!Actor::preDraw()) {
        return FALSE;
    }
    if ((colliderFlags & 1) == 0) {
        u16 *p = getItemId();
        BOOL r = Unk_ov009_0225d858_Is(p, 0x500b);
        if (r || !Camera_IsBlockingFocusView(&solidCenterX, solidSizeX, solidSizeZ)) {
            if (needsMatrixUpdate()) {
                updateMatrix();
            }
            return TRUE;
        }
    }
    return FALSE;
}

BOOL BuildingActor::preDelete() {
    if (!Character::preDelete()) {
        return FALSE;
    }
    seEmitter.deactivate();
    destroyShadows();
    destroyColliders();
    model.clearResource();
    BuildingList_Remove(this);
    if (entryFlags.f0) {
        CharInteractSync_ReleaseLock();
        if (Item_IsNookShop(&itemId)) {
            NookShop_SetVisitState(1);
        }
    }
    return TRUE;
}

void BuildingActor::initEntryArea() {
    BuildingResources *r = getResources();
    if (r != NULL) {
        s32 z = position.z + r->solidCenterZ;
        s32 y = position.y;
        s32 x = position.x + r->solidCenterX;
        solidCenterX = x;
        solidCenterY = y;
        solidCenterZ = z;
        solidSizeX = r->solidSizeX;
        solidSizeZ = r->solidSizeZ;
    }
}

void BuildingActor::setupAnims() {
    if (getDoorInAnim() != 0 || getDoorOutAnim() != 0) {
        model.allocAnmObj(gFieldStructureHeap);
        static_cast<BlendAnimModel *>(&model)->initAnim(getDoorInAnim(), 0, 0x1000, 0, 0);
        model.attachAnim();
    }
    if (getEntranceType() != 0) {
        u16 *p = getItemId();
        if (Unk_ov009_0225d858_Is(p, 0x501d)) {
            setDoorState(BuildingState_Get(itemId));
        } else {
            setDoorState(0);
        }
    }
}

s32 BuildingActor::getViewRangeX() {
    u32 v;
    BOOL in = FALSE;
    v = itemId;
    if (v >= 0x5000 && v <= 0x5021) {
        in = TRUE;
    }
    if (in) {
        v = v & 0xfff;
    } else {
        v = -1;
    }
    BuildingInfo t;
    BuildingInfo_Copy(&t, v < 0x22 ? &data_020d0a7c[v] : &data_020d0a7c[0]);
    s32 r = BuildingInfo_GetViewRangeX(&t) << 13;
    BuildingInfo_Destroy(&t);
    return r;
}

s32 BuildingActor::getViewRangeBack() {
    u32 v;
    BOOL in = FALSE;
    v = itemId;
    if (v >= 0x5000 && v <= 0x5021) {
        in = TRUE;
    }
    if (in) {
        v = v & 0xfff;
    } else {
        v = -1;
    }
    BuildingInfo t;
    BuildingInfo_Copy(&t, v < 0x22 ? &data_020d0a7c[v] : &data_020d0a7c[0]);
    s32 r = BuildingInfo_GetViewRangeBack(&t) << 13;
    BuildingInfo_Destroy(&t);
    return r;
}

s32 BuildingActor::getViewRangeFront() {
    u32 v;
    BOOL in = FALSE;
    v = itemId;
    if (v >= 0x5000 && v <= 0x5021) {
        in = TRUE;
    }
    if (in) {
        v = v & 0xfff;
    } else {
        v = -1;
    }
    BuildingInfo t;
    BuildingInfo_Copy(&t, v < 0x22 ? &data_020d0a7c[v] : &data_020d0a7c[0]);
    s32 r = BuildingInfo_GetViewRangeFront(&t) << 13;
    BuildingInfo_Destroy(&t);
    return r;
}

void *BuildingActor::getDoorInAnim() {
    BuildingResources *r = getResources();
    if (r != NULL) {
        return r->bca0;
    }
    return 0;
}

void *BuildingActor::getDoorOutAnim() {
    BuildingResources *r = getResources();
    if (r != NULL) {
        return r->bca1;
    }
    return 0;
}

void *BuildingActor::getBca2Anim() {
    BuildingResources *r = getResources();
    if (r != NULL) {
        return r->bca2;
    }
    return 0;
}

void *BuildingActor::getBtaAnim(u32 idx) {
    if (idx < 4) {
        BuildingResources *r = getResources();
        if (r != NULL) {
            return r->btaAnims[idx];
        }
    }
    return 0;
}

s32 BuildingActor::getEntranceType() {
    u32 v;
    BOOL in = FALSE;
    v = itemId;
    if (v >= 0x5000 && v <= 0x5021) {
        in = TRUE;
    }
    if (in) {
        v = v & 0xfff;
    } else {
        v = -1;
    }
    BuildingInfo t;
    BuildingInfo_Copy(&t, v < 0x22 ? &data_020d0a7c[v] : &data_020d0a7c[0]);
    s32 r = BuildingInfo_GetEntranceType(&t);
    BuildingInfo_Destroy(&t);
    return r;
}

BOOL BuildingActor::areLightsOn() {
    if (Building_IsNight() && isOpen()) {
        return TRUE;
    }
    return FALSE;
}

BOOL BuildingActor::isOpen() { return TRUE; }

extern "C" BOOL Building_IsNight() {
    u8 t[4];
    Clock_GetMinuteHour(t);
    u32 b = t[1];
    if (b >= 6 && b < 0x12) {
        return FALSE;
    }
    return TRUE;
}

char *BuildingActor::getArcPath() {
    u32 i = buildingIndex;
    Str_SPrintf(sBuildingArcPath, sBuildingArcPathFmt, i, i, Shared_Field_GetStructureTexSuffix());
    return sBuildingArcPath;
}

char *BuildingActor::getTexPath() {
    u32 i = buildingIndex;
    Str_SPrintf(sBuildingTexPath, sBuildingTexPathFmt, i, i, Shared_Field_GetStructureTexSuffix());
    return sBuildingTexPath;
}

char *BuildingActor::getLightTexPath() {
    volatile u16 v = Item_MakeBuilding(buildingIndex);
    switch (v) {
    case 0x500a:
    case 0x5011:
    case 0x501c:
    case 0x501d:
        return 0;
    }
    u32 i = buildingIndex;
    Str_SPrintf(sBuildingLightTexPath, sBuildingLightTexPathFmt, i, i, Shared_Field_GetStructureTexSuffix());
    return sBuildingLightTexPath;
}

BOOL BuildingActor::setupModel(char *a, char *b, char *c) {
    if (shadows != 0 || model.resMdl != 0) {
        return TRUE;
    }
    loadResources(a, b, c);
    BuildingResources *r = getResources();
    if (r != NULL && r->bmd0 != 0) {
        if (model.setResource(r->bmd0, NULL)) {
            NNSG3dResTex *x = PlayerHouseTex_Get(FieldStructureMgr_GetPlayerHouseTex());
            NNS_G3dBindMdlPltt(r->bmd0, x);
            if (r->tex) {
                NNS_G3dBindMdlTex(r->bmd0, r->tex);
            }
            if (r->lightTex) {
                NNS_G3dBindMdlTex(r->bmd0, r->lightTex);
                NNS_G3dBindMdlPltt(r->bmd0, r->lightTex);
            }
            return TRUE;
        }
        return FALSE;
    }
    return TRUE;
}

BOOL BuildingActor::loadResources(char *a, char *b, char *c) {
    BOOL result = FALSE;
    BuildingResources *e = getResources();
    if (BuildingResources_IsLoaded(e)) {
        return TRUE;
    }
    StrBSizeData *h = StrBSize_Get(&itemId);
    if (h != NULL) {
        h->getSolidBounds(&e->solidCenterX, &e->solidCenterZ, &e->solidSizeX, &e->solidSizeZ);
    }
    if (a != NULL) {
        void *data = File_LoadAlloc(a, gFieldStructureHeap, 4, 0);
        if (data != NULL) {
            char b1[0x1e];
            char b2[0x1e];
            NNSFndArchive obj;
            s32 z1, z2;
            u32 i;
            if (NNS_FndMountArchive(&obj, data_ov009_0225e3e8, data)) {
                void *t;
                t = NNS_FndGetArchiveFileByName(data_ov009_0225e3ec);
                if (t) {
                    e->bca0 = Building_GetFirstAnm(t);
                }
                t = NNS_FndGetArchiveFileByName(data_ov009_0225e3fc);
                if (t) {
                    e->bca1 = Building_GetFirstAnm(t);
                }
                t = NNS_FndGetArchiveFileByName(data_ov009_0225e40c);
                if (t) {
                    e->bca2 = Building_GetFirstAnm(t);
                }
                i = 0;
                z1 = i;
                for (; i < 4; i++) {
                    Str_SPrintf(b1, data_ov009_0225e41c, i);
                    t = NNS_FndGetArchiveFileByName(b1);
                    if (t) {
                        e->btaAnims[i] = func_02106670(func_02106654(t), z1);
                    }
                }
                i = 0;
                z2 = i;
                for (; i < 4; i++) {
                    Str_SPrintf(b2, data_ov009_0225e42c, i);
                    t = NNS_FndGetArchiveFileByName(b2);
                    if (t) {
                        e->btpAnims[i] = func_021066ac(func_02106690(t), z2);
                    }
                }
                e->bmd0 = Building_GetFirstMdl(NNS_FndGetArchiveFileByName(data_ov009_0225e43c));
                t = NNS_FndGetArchiveFileByName(data_ov009_0225e44c);
                if (t) {
                    e->bmd1 = Building_GetFirstMdl(t);
                }
                e->shadowTable = (BuildingShadowTable *)NNS_FndGetArchiveFileByName(data_ov009_0225e45c);
                NNS_FndUnmountArchive(&obj);
            }
            result = TRUE;
        }
    }
    if (b != NULL) {
        if (File_Exists(b)) {
            void *r5 = File_LoadAlloc(b, gCurrentHeap, -4, 0);
            if (r5 != NULL) {
                e->tex = NNS_G3dGetTex(r5);
                if (Gfx3d_LoadTex(e->tex, 0)) {
                    e->tex = Gfx3d_CopyTex(e->tex, gFieldStructureHeap);
                }
                Mem_Free(r5);
            }
        }
    }
    if (c != NULL) {
        if (File_Exists(c)) {
            void *r5 = File_LoadAlloc(c, gCurrentHeap, -4, 0);
            if (r5 != NULL) {
                e->lightTex = NNS_G3dGetTex(r5);
                if (Gfx3d_LoadTexAndPltt(e->lightTex, 0)) {
                    e->lightTex = Gfx3d_CopyTex(e->lightTex, gFieldStructureHeap);
                }
                Mem_Free(r5);
            }
        }
    }
    return result;
}

void BuildingActor::makeCurvedMatrix(Mtx43 *out) {
    Mtx43 m;
    Mtx43_SetTranslate(&m, drawPos.x, drawPos.y, drawPos.z);
    Mtx43_RotateX(&m, (s16)drawTilt);
    *out = m;
}

// tiny callees defined last so they stay out of line
BuildingResources *BuildingActor::getResources() {
    if (buildingIndex < 0x22) {
        return &sBuildingResources[buildingIndex];
    }
    return NULL;
}

void BuildingActor::updateDoorExit() {
    if (exitDelay >= 1) {
        if (exitDelay == 3) {
            if (PlayerActor_IsInterruptibleByMenu()) {
                switch (getEntranceType()) {
                case 2:
                    Field_SetDoorExitMode(1);
                    data_021c1b3c->sceneFade.setFadeDelay(1);
                    if (PlayerActor_LocalRequestDoorExit()) {
                        exitDelay = 0;
                        return;
                    }
                    break;
                case 3:
                    Field_SetDoorExitMode(0);
                    data_021c1b3c->sceneFade.setFadeDelay(2);
                    if (PlayerActor_LocalRequestDoorExit()) {
                        exitDelay = 0;
                        return;
                    }
                    break;
                case 1:
                    Field_SetDoorExitMode(0);
                    if (Unk_ov009_0225d0d8_Match(&itemId, 0x5012) || Unk_ov009_0225d0d8_Match(&itemId, 0x5013)) {
                        data_021c1b3c->sceneFade.setFadeDelay(4);
                    } else {
                        data_021c1b3c->sceneFade.setFadeDelay(3);
                    }
                    if (PlayerActor_LocalRequestDoorExit()) {
                        exitDelay = 0;
                        return;
                    }
                    break;
                default:
                    exitDelay = 0;
                    return;
                }
            }
        }
        if (exitDelay < 3) {
            exitDelay++;
        }
    }
}

void BuildingActor::updateBaseMatrix(Mtx43 *out) {
    Mtx43 blk;
    if (!calcCustomBaseMatrix(&blk)) {
        drawTilt = WorldCurve_ToCurved(&drawPos, &position);
        makeCurvedMatrix(&blk);
    }
    model.mtx = blk;
    if (out != NULL) {
        *out = blk;
    }
}

void BuildingActor::createShadows(Mtx43 *m) {
    BuildingResources *e = getResources();
    if (e != NULL) {
        if (e->shadowTable != NULL) {
            Heap *heap = (Heap *)gFieldStructureHeap;
            u32 n = e->shadowTable->getCount();
            shadows = (ObjShadowStrip *)Heap_Alloc(heap, n * sizeof(ObjShadowStrip));
            ObjShadowStrip *p = shadows;
            u32 i;
            s32 zero;
            i = 0;
            zero = i;
            for (; i < n; p++, i++) {
                if (p != NULL) {
                    p->func_020ac1e0();  // (the constructor: it returns this on ARM only)
                }
                BuildingShadowEntry *it = e->shadowTable->getEntry(i);
                VecFx32Ctor v(it->offsetX, zero, it->offsetZ);
                VecFx32 out;
                Building_LocalToWorld(&out, &v, m);
                p->build(&out, it->size, it->shift, it->texIndex, it->texLeft, it->texRight, (Heap *)heap);
            }
        }
    }
}

void BuildingActor::updateShadows(Mtx43 *m) {
    BuildingResources *e = getResources();
    if (e != NULL) {
        ObjShadowStrip *p = shadows;
        if (p != NULL) {
            s32 i = 0;
            s32 zero = i;
            for (; (u32)i < e->shadowTable->getCount(); p++, i++) {
                BuildingShadowEntry *it = e->shadowTable->getEntry(i);
                VecFx32Ctor v(it->offsetX, zero, it->offsetZ);
                VecFx32 out;
                Building_LocalToWorld(&out, &v, m);
                p->draw(&out);
            }
        }
    }
}

void BuildingActor::destroyShadows() {
    BuildingResources *e = getResources();
    if (e != NULL) {
        if (shadows != NULL) {
            u32 i;
            for (i = 0; i < e->shadowTable->getCount(); i++) {
            }
            shadows = NULL;
        }
    }
}

void BuildingActor::createColliders(Mtx43 *m) {
    colliderCount = 0;
    StrBSizeData *h = StrBSize_Get(&itemId);
    if (h != NULL) {
        colliderCount = h->getTriangleCount();
        if (colliderCount != 0) {
            BuildingCollider *e4;
            TouchPickTriangle *e6;
            u8 k;
            u32 i;
            VecFx32 a, b, c;
            VecFx32 wa, wb, wc;
            VecFx32 la, lb, lc;
            collisionShapes = (TouchPickTriangle *)Heap_Alloc((Heap *)gFieldStructureHeap, colliderCount * sizeof(TouchPickTriangle));
            colliders = (BuildingCollider *)Heap_Alloc((Heap *)gFieldStructureHeap, colliderCount * sizeof(BuildingCollider));
            e4 = colliders;
            e6 = collisionShapes;
            k = BuildingList_IndexOf(this);
            for (i = 0; i < colliderCount; e4++, e6++, i++) {
                if (h->getTriangle(&a.x, &b.x, &c.x, i)) {
                    Building_LocalToWorld(&wa, &a, m);
                    Building_LocalToWorld(&wb, &b, m);
                    Building_LocalToWorld(&wc, &c, m);
                    Vec_Add(&la, &position, &a);
                    Vec_Add(&lb, &position, &b);
                    Vec_Add(&lc, &position, &c);
                    e6 = new (e6) TouchPickTriangle;
                    Scene_GetTouchPicker()->addTriangle(e6, &wa, &wb, &wc, 7, k);
                    e4 = new (e4) BuildingCollider;
                    e4->building = this;
                    e4->entranceType = getEntranceType();
                    e4->setupTrigger(&la, &lb, &lc, 0x3000);
                    TriangleTrigger_Register(e4);
                }
            }
        }
    }
}

void BuildingActor::submitColliders() {
    if ((colliderFlags & 1) == 0) {
        TouchPickTriangle *p = collisionShapes;
        if (p != NULL) {
            for (; p < collisionShapes + colliderCount; p++) {
                Scene_GetTouchPicker()->pushTriangle(p);
            }
        }
    }
}

// ---------------------------------------------------------------- actor
void BuildingActor::destroyColliders() {
    if (collisionShapes != NULL) {
        collisionShapes = NULL;
    }
    BuildingCollider *p = colliders;
    if (p != NULL) {
        for (; p < colliders + colliderCount; p += 2) {
            TriangleTrigger_Unregister(p);
            p->building = NULL;
        }
        colliders = NULL;
    }
    colliderCount = 0;
}

u32 BuildingShadowTable::getCount() {
    return count;
}

BuildingShadowEntry *BuildingShadowTable::getEntry(u32 i) {
    return &entries[i];
}

BuildingCollider::BuildingCollider() {}

void BuildingCollider::onActorNear(VecFx32 *a, Actor *o, s32 off) {
    if (isPlayerAtDoor(a, off, o)) {
        building->colliderFlags |= 2;
        if (o->speed >= 0x200) {
            building->colliderFlags |= 4;
        }
    }
}

// ---------------------------------------------------------------- element
BOOL BuildingCollider::isPlayerAtDoor(VecFx32 *v, s32 off, Actor *o) {
    s16 ang;
    VecFx32 p;
    VecFx32 a;
    VecFx32 b;
    VecFx32 c;
    if (o != NULL) {
        if (building != NULL) {
            if (Unk_ov009_0225cc24_IsNine(o->profile)) {
                if (PlayerActor_GetActor(4) == o) {
                    s32 d = distanceTo((Unk_0202f2ac_V3 *)v);
                    if (d >= 0) {
                        if (d <= off + 0x666) {
                            if (building->getDoorPos(&p, &ang)) {
                                if (Math_AngleDiffAbs(ang, o->rotY) <= 0x1100) {
                                    a.x = v->x;
                                    a.y = v->y;
                                    a.z = v->z;
                                    a.y = a.y + off;
                                    b.x = a.x;
                                    b.y = a.y;
                                    b.z = a.z;
                                    b.x = b.x - func_01ffcb0c(normal.x, 0x2000);
                                    b.z = b.z - func_01ffcb0c(normal.z, 0x2000);
                                    if (intersectLine((Unk_0202f2ac_V3 *)&c, (Unk_0202f2ac_V3 *)&a, (Unk_0202f2ac_V3 *)&b)) {
                                        return TRUE;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return FALSE;
}

// ---------------------------------------------------------------- vfunc_6c
s32 BuildingActor::setDoorState(s32 a) {
    static BOOL (BuildingActor::*tbl[7])() = {
        &BuildingActor::enterDoorIdle, &BuildingActor::enterDoorOpenIn,
        &BuildingActor::enterDoorOpenOut, &BuildingActor::enterDoorSlideOpen,
        &BuildingActor::enterDoorSlideClose, &BuildingActor::enterDoorNoAnimIn,
        &BuildingActor::enterDoorNoAnimOut,
    };
    if ((u32)a < 7) {
        if ((this->*tbl[a])()) {
            if (BuildingState_Set(itemId, a)) {
                doorState = a;
                return TRUE;
            }
        }
    }
    return FALSE;
}

void BuildingActor::updateDoorState() {
    static Unk_ov009_0225c290_Fn tbl[7] = {
        &BuildingActor::execDoorIdle, &BuildingActor::execDoorOpenIn,
        &BuildingActor::execDoorOpenOut, &BuildingActor::execDoorSlideOpen,
        &BuildingActor::execDoorSlideClose, &BuildingActor::execDoorNoAnimIn,
        &BuildingActor::execDoorNoAnimOut
    };
    if (doorState < 7) {
        (this->*tbl[doorState])();
    }
}

BOOL BuildingActor::enterDoorIdle() {
    switch (getEntranceType()) {
    case 1:
    case 2: {
        void *r = (void *)getDoorInAnim();
        if (r == 0) {
            return FALSE;
        }
        static_cast<BlendAnimModel *>(&model)->initAnim(r, 0, 0x1000, 0, 0);
        break;
    }
    }
    return TRUE;
}

void BuildingActor::execDoorIdle() {
    if (PlayerActor_TestSlotFlag(0x13, 4) == 0 && TalkRequestFlags_IsResetti() == 0) {
        s32 st = getEntranceType();
        s32 f = 0;
        if (st == 1 || st == 3) {
            if ((colliderFlags & 4) != 0) {
                if (isOpen() == 0) {
                    clearTalkStartMode();
                    closedTalk = 1;
                } else {
                    clearTalkStartMode();
                    closedTalk = 0;
                }
                TalkRequest_AddPlayerTalk6(this, 0);
                f = 1;
            }
        }
        if ((colliderFlags & 2) != 0 && f == 0) {
            void *a = TouchPick_GetTappedObject(Scene_GetTouchPicker(), 0, 0);
            void *b = PlayerActor_GetActor(4);
            if (b != 0 && b == a) {
                if (isOpen() == 0) {
                    clearTalkStartMode();
                    closedTalk = 1;
                } else {
                    clearTalkStartMode();
                    closedTalk = 0;
                }
                TalkRequest_AddPlayerTalk6(this, 0);
            }
        }
    }
}

BOOL BuildingActor::enterDoorOpenIn() {
    void *r = (void *)getDoorInAnim();
    if (r != 0) {
        static_cast<BlendAnimModel *>(&model)->initAnim(r, 1, 0x1000, 0, 0);
        seEmitter.playSe(0x7d1);
        seEmitter.playSe(0x7d2);
        if (playsDoorMelody()) {
            s32 m = 0;
            u16 v[3];
            BOOL ok;
            if (Item_IsFurniture(&itemId)) {
                v[0] = 0x500d;
                if (Item_GetFurnitureIndex(&itemId) == Item_GetFurnitureIndex(&v[0])) {
                    ok = TRUE;
                } else {
                    ok = FALSE;
                }
            } else {
                if (itemId == 0x500d) {
                    ok = TRUE;
                } else {
                    ok = FALSE;
                }
            }
            if (ok) {
                m = 1;
            } else {
                if (Item_IsFurniture(&itemId)) {
                    v[1] = 0x5000;
                    if (Item_GetFurnitureIndex(&itemId) == Item_GetFurnitureIndex(&v[1])) {
                        ok = TRUE;
                    } else {
                        ok = FALSE;
                    }
                } else {
                    if (itemId == 0x5000) {
                        ok = TRUE;
                    } else {
                        ok = FALSE;
                    }
                }
                if (ok) {
                    m = 2;
                } else {
                    if (Item_IsFurniture(&itemId)) {
                        v[2] = 0x500c;
                        if (Item_GetFurnitureIndex(&itemId) == Item_GetFurnitureIndex(&v[2])) {
                            ok = TRUE;
                        } else {
                            ok = FALSE;
                        }
                    } else {
                        if (itemId == 0x500c) {
                            ok = TRUE;
                        } else {
                            ok = FALSE;
                        }
                    }
                    if (ok) {
                        m = 3;
                    }
                }
            }
            VecFx32Ctor msg = getSoundPos();
            Melody_PlayAt(&msg, m);
        }
        return TRUE;
    }
    return FALSE;
}

void BuildingActor::execDoorOpenIn() {
    model.stepAnim();
    if (model.isFinished()) {
        setDoorState(0);
    } else if (model.hasPassedFrame(0x14)) {
        seEmitter.playSe(0x7d3);
    } else if (model.hasPassedFrame(0x1e)) {
        seEmitter.playSe(0x7d4);
    }
}

BOOL BuildingActor::enterDoorOpenOut() {
    void *r = (void *)getDoorOutAnim();
    if (r != 0) {
        static_cast<BlendAnimModel *>(&model)->initAnim(r, 1, 0x1000, 0, 0);
        seEmitter.playSe(0x7d1);
        seEmitter.playSe(0x7d2);
        if (playsDoorMelody()) {
            s32 m = 0;
            u16 v[3];
            BOOL ok;
            if (Item_IsFurniture(&itemId)) {
                v[0] = 0x500d;
                if (Item_GetFurnitureIndex(&itemId) == Item_GetFurnitureIndex(&v[0])) {
                    ok = TRUE;
                } else {
                    ok = FALSE;
                }
            } else {
                if (itemId == 0x500d) {
                    ok = TRUE;
                } else {
                    ok = FALSE;
                }
            }
            if (ok) {
                m = 1;
            } else {
                if (Item_IsFurniture(&itemId)) {
                    v[1] = 0x5000;
                    if (Item_GetFurnitureIndex(&itemId) == Item_GetFurnitureIndex(&v[1])) {
                        ok = TRUE;
                    } else {
                        ok = FALSE;
                    }
                } else {
                    if (itemId == 0x5000) {
                        ok = TRUE;
                    } else {
                        ok = FALSE;
                    }
                }
                if (ok) {
                    m = 2;
                } else {
                    if (Item_IsFurniture(&itemId)) {
                        v[2] = 0x500c;
                        if (Item_GetFurnitureIndex(&itemId) == Item_GetFurnitureIndex(&v[2])) {
                            ok = TRUE;
                        } else {
                            ok = FALSE;
                        }
                    } else {
                        if (itemId == 0x500c) {
                            ok = TRUE;
                        } else {
                            ok = FALSE;
                        }
                    }
                    if (ok) {
                        m = 3;
                    }
                }
            }
            VecFx32Ctor msg = getSoundPos();
            Melody_PlayAt(&msg, m);
        }
        return TRUE;
    }
    return FALSE;
}

void BuildingActor::execDoorOpenOut() {
    model.stepAnim();
    if (model.isFinished()) {
        setDoorState(0);
    } else if (model.hasPassedFrame(0x12)) {
        seEmitter.playSe(0x7d3);
    } else if (model.hasPassedFrame(0x18)) {
        seEmitter.playSe(0x7d4);
    }
}

BOOL BuildingActor::enterDoorSlideOpen() {
    void *r = (void *)getDoorInAnim();
    if (r) {
        static_cast<BlendAnimModel *>(&model)->initAnim(r, 1, 0x1000, 0, 0);
        if (Item_IsNookShop(&itemId)) {
            seEmitter.playSe(0x806);
        } else {
            seEmitter.playSe(0x808);
        }
        return TRUE;
    }
    return FALSE;
}

void BuildingActor::execDoorSlideOpen() {
    model.stepAnim();
    if (model.isFinished()) {
        setDoorState(4);
    }
}

BOOL BuildingActor::enterDoorSlideClose() {
    doorCloseSeDelay = 0x1a;
    void *r = (void *)getDoorOutAnim();
    if (r) {
        static_cast<BlendAnimModel *>(&model)->initAnim(r, 1, 0x1000, 0, 0);
        return TRUE;
    }
    return FALSE;
}

void BuildingActor::execDoorSlideClose() {
    if (doorCloseSeDelay != 0) {
        doorCloseSeDelay--;
        if (doorCloseSeDelay == 0) {
            if (Item_IsNookShop(&itemId)) {
                seEmitter.playSe(0x807);
            } else {
                seEmitter.playSe(0x809);
            }
        }
    }
    if (doorCloseSeDelay == 0) {
        model.stepAnim();
        if (model.isFinished()) {
            setDoorState(0);
        }
    }
}

BOOL BuildingActor::enterDoorNoAnimIn() { return TRUE; }

void BuildingActor::execDoorNoAnimIn() { setDoorState(0); }

BOOL BuildingActor::enterDoorNoAnimOut() { return TRUE; }

void BuildingActor::execDoorNoAnimOut() { setDoorState(0); }

BOOL BuildingActor::setEntryState(s32 a) {
    static Unk_ov009_0225c360_Fn tbl[9] = {
        &BuildingActor::enterEntryIdle, &BuildingActor::enterEntryCheck,
        &BuildingActor::enterEntryTalkOpen, &BuildingActor::enterEntryTalk,
        &BuildingActor::enterEntry04, &BuildingActor::enterEntry05,
        &BuildingActor::enterEntryWarp, &BuildingActor::enterEntry07,
        &BuildingActor::enterEntry08
    };
    if (a < 9) {
        if ((this->*tbl[a])()) {
            entryState = a;
            return TRUE;
        }
    }
    return FALSE;
}

void BuildingActor::updateEntryState() {
    static Unk_ov009_0225c290_Fn tbl[9] = {
        &BuildingActor::execEntryIdle, &BuildingActor::execEntryCheck,
        &BuildingActor::execEntryTalkOpen, &BuildingActor::execEntryTalk,
        &BuildingActor::execEntry04, &BuildingActor::execEntry05,
        &BuildingActor::execEntryWarp, &BuildingActor::execEntry07,
        &BuildingActor::execEntry08
    };
    if (entryState < 9) {
        (this->*tbl[entryState])();
    }
}

BOOL BuildingActor::enterEntryIdle() { return TRUE; }

void BuildingActor::execEntryIdle() {}

BOOL BuildingActor::enterEntryCheck() {
    if (closedTalk == 0) {
        BuildingOccupancy_RequestEnter(itemId);
    }
    entryFlags.f1 = 0;
    visitRefused = 0;
    return TRUE;
}

void BuildingActor::execEntryCheck() {
    if (closedTalk == 0) {
        s32 r = BuildingOccupancy_GetAnswer(itemId);
        if (r != 0) {
            s32 v = (r == 2) ? 1 : 0;
            u8 *p = (u8 *)&entryFlags;
            *p = (*p & ~2) | ((v & 1) << 1);
            visitRefused = (r == 3) ? 1 : 0;
            if (entryFlags.f1 != 0 || visitRefused != 0) {
                setEntryState(2);
            } else if (getEntranceType() == 1) {
                setEntryState(7);
            } else {
                setEntryState(4);
            }
        }
    } else {
        setEntryState(2);
    }
}

BOOL BuildingActor::enterEntryTalkOpen() {
    attachTalkRequest(this);
    window->nextState = 1;
    setupTalkMsg();
    return TRUE;
}

void BuildingActor::execEntryTalkOpen() {
    if (window != NULL && window->state != 0) {
        onTalkOpened();
        setEntryState(3);
    }
}

BOOL BuildingActor::enterEntryTalk() { return TRUE; }

void BuildingActor::execEntryTalk() {
    if (window != NULL && window->state == 0) {
        onTalkEnded();
        detachTalkRequest(this);
        TalkRequest_SetTargetDone(this);
    } else {
        updateTalk();
    }
}

BOOL BuildingActor::enterEntry04() {
    PlayerActor_RequestStowThenAct10(0);
    return TRUE;
}

void BuildingActor::execEntry04() {
    if (PlayerActor_IsStowFinished()) {
        setEntryState(5);
    }
}

BOOL BuildingActor::enterEntry05() {
    s16 ang;
    s32 p4;
    VecFx32 v;
    if (getDoorPos(&v, &ang)) {
        p4 = v.x;
        if (alignsPlayerToDoor() == 0) {
            VecFx32 *q = (VecFx32 *)PlayerActor_GetBodyPos(4);
            if (q != NULL) {
                p4 = q->x;
            }
        }
        if (usesDoorApproach()) {
            if (Shared_PlayerActor_LocalRequestDoorApproach(&p4, &v.z, &ang)) {
                return TRUE;
            }
        } else {
            BOOL m = getEntranceType() == 2 ? TRUE : FALSE;
            if (PlayerActor_LocalRequestDoorEnter(m, &p4, &v.z, ang)) {
                return TRUE;
            }
        }
    }
    return FALSE;
}

void BuildingActor::execEntry05() {
    if (PlayerActor_IsEnteringDoor()) {
        switch (getEntranceType()) {
        case 2:
            Camera_SetMode3();
            setEntryState(6);
            break;
        case 3:
            Camera_SetMode3();
            setEntryState(6);
            break;
        }
    }
}

BOOL BuildingActor::enterEntryWarp() {
    warpTimer = 0;
    return TRUE;
}

void BuildingActor::execEntryWarp() {
    warpTimer = warpTimer + 1;
    getEntranceType();
    u32 lim = 0x14;
    if (getEntranceType() == 1) {
        lim += 0xc;
    }
    if (warpTimer >= lim) {
        s32 r = getInteriorScene();
        s16 ang;
        VecFx32 v;
        if (getDoorPos(&v, &ang)) {
            if (SceneWarp_RequestExit(Scene_GetWarpRequest(), r)) {
                v.y = Ground_GetDefaultY();
                v.z = v.z + 0x1000;
                SceneWarp *o = Scene_GetWarpRequest();
                s32 k = Scene_GetCurrent();
                Scene_SetTownReturnPos(o, k, &v, 0xf000000, (s16)(ang + 0x8000), gridX, gridZ);
                Building_SetLastEntranceType(getEntranceType());
                entryFlags.f0 = 1;
            }
        }
    }
}

BOOL BuildingActor::enterEntry07() {
    PlayerActor_RequestStowThenAct10(0);
    return TRUE;
}

void BuildingActor::execEntry07() {
    if (PlayerActor_IsStowFinished()) {
        setEntryState(8);
    }
}

BOOL BuildingActor::enterEntry08() { return TRUE; }

void BuildingActor::execEntry08() {
    s16 ang;
    s32 p4;
    VecFx32 v;
    if (getDoorPos(&v, &ang)) {
        p4 = v.x;
        if (alignsPlayerToDoor() == 0) {
            VecFx32 *q = (VecFx32 *)PlayerActor_GetBodyPos(4);
            if (q != NULL) {
                p4 = q->x;
            }
        }
        if (PlayerActor_LocalRequestDoorEnter(2, &p4, &v.z, ang)) {
            setEntryState(6);
        }
    }
}

VecFx32 *BuildingActor::getInteractionPos() { return (VecFx32 *)&entryPos; }

BOOL BuildingActor::acceptsInteraction(void *other) {
    Character *a = (Character *)other;
    if (a == NULL) {
        return FALSE;
    }
    s32 d = Math_AngleDiffAbs((s16)(rotY + 0x8000), a->rotY);
    if (d <= 0x1000) {
        if (getEntranceType() != 0) {
            if ((colliderFlags & 8) != 0 && getEntranceType() == 2) {
                if (isOpen() == 0) {
                    clearTalkStartMode();
                    closedTalk = 1;
                    return TRUE;
                }
                clearTalkStartMode();
                closedTalk = 0;
                return TRUE;
            }
        } else if (isOpen() == 0) {
            s32 r = Vec_DistXZ(a->getInteractionPos(), getInteractionPos());
            clearTalkStartMode();
            closedTalk = 1;
            if (r >= 0x3000) {
                return FALSE;
            }
            return TRUE;
        }
    }
    return FALSE;
}

void BuildingActor::onMessageEnd(u32) {
    BOOL ok;
    if (Item_IsFurniture(&itemId)) {
        u16 v = 0x500a;
        if (Item_GetFurnitureIndex(&itemId) == Item_GetFurnitureIndex(&v)) {
            ok = TRUE;
        } else {
            ok = FALSE;
        }
    } else {
        if (itemId == 0x500a) {
            ok = TRUE;
        } else {
            ok = FALSE;
        }
    }
    if (!ok) {
        window->startAutoAdvance(0x64);
    }
}

void BuildingActor::setupTalkMsg() {
    StackPad8 pad;
    setFileName((const char *)sBuildingDefaultMsgFile);
    msgIndex = 0;
}

void BuildingActor::onTalkOpened() {}

void BuildingActor::updateTalk() {}

void BuildingActor::onTalkEnded() {}

void BuildingActor::updateMatrix() {
    if (model.resMdl != NULL) {
        updateBaseMatrix(0);
        model.drawAnimated(NULL);
        Mtx43 t = model.mtx;
        updateShadows(&t);
    }
}

BOOL BuildingActor::getDoorPos(VecFx32 *out, s16 *ang) {
    if (colliders != NULL && colliderCount != 0) {
        s32 a = Math_Atan2(colliders->normal.x, colliders->normal.z);
        s32 t0 = func_01ffcb0c(0x1000, colliders->normal.z);
        VecFx32 v;
        v.x = func_01ffcb0c(0x1000, colliders->normal.x);
        v.y = 0;
        v.z = t0;
        if (ang != NULL) {
            *ang = a + 0x8000;
        }
        if (out != NULL) {
            VecFx32 r;
            Vec_Add(&r, (VecFx32 *)colliders->getCenter(), &v);
            out->x = r.x;
            out->y = r.y;
            out->z = r.z;
            out->y = 0x200;
            out->z = out->z - 0x200;
        }
        return TRUE;
    }
    out->x = position.x;
    out->y = position.y;
    out->z = position.z;
    return FALSE;
}

s32 BuildingActor::getInteriorScene(){
    u32 v;
    BOOL in = FALSE;
    v = itemId;
    if (v >= 0x5000 && v <= 0x5021) {
        in = TRUE;
    }
    if (in) {
        v = v & 0xfff;
    } else {
        v = -1;
    }
    BuildingInfo t;
    BuildingInfo_Copy(&t, v < 0x22 ? &data_020d0a7c[v] : &data_020d0a7c[0]);
    s32 r = BuildingInfo_GetInteriorScene(&t);
    BuildingInfo_Destroy(&t);
    return r;
}

s32 BuildingActor::hasFlickeringLights(){
    u32 v;
    BOOL in = FALSE;
    v = itemId;
    if (v >= 0x5000 && v <= 0x5021) {
        in = TRUE;
    }
    if (in) {
        v = v & 0xfff;
    } else {
        v = -1;
    }
    BuildingInfo t;
    BuildingInfo_Copy(&t, v < 0x22 ? &data_020d0a7c[v] : &data_020d0a7c[0]);
    s32 r = BuildingInfo_GetFlickeringLights(&t);
    BuildingInfo_Destroy(&t);
    return r;
}

BOOL BuildingActor::usesDoorApproach() { return FALSE; }

BOOL BuildingActor::needsMatrixUpdate() { return TRUE; }

BOOL BuildingActor::alignsPlayerToDoor() { return TRUE; }

BOOL BuildingActor::playsDoorMelody() { return FALSE; }

BOOL BuildingActor::isOffscreen() {
    if (gCamera != 0) {
        VecFx32 *g = &gCameraLookAt;
        s32 dx = position.x - g->x;
        if (dx < 0) {
            dx = -dx;
        }
        s32 dz = position.z - g->z;
#ifdef HOST
        // (every building - the houses, the shops - has its own view window besides the actor cull; doubled with the
        // rest of the draw distance, or the buildings still pop in and out at the DS's edge)
        if (dx > getViewRangeX() * 2 || dz > getViewRangeFront() * 2 || dz < -getViewRangeBack() * 2) {
#else
        if (dx > getViewRangeX() || dz > getViewRangeFront() || dz < -getViewRangeBack()) {
#endif
            return TRUE;
        }
        return FALSE;
    }
    return TRUE;
}

void BuildingActor::updateOffscreen() {
    colliderFlags = colliderFlags & ~1;
    if (isOffscreen()) {
        colliderFlags = colliderFlags | 1;
    }
}

BOOL BuildingActor::isDoorIdle() {
    if (doorState == 0) {
        return TRUE;
    }
    return FALSE;
}

BOOL BuildingActor::openDoorForEntry() {
    switch (getEntranceType()) {
    case 2:
        return setDoorState(1);
    case 3:
        return setDoorState(5);
    case 1:
        return setDoorState(3);
    default:
        return FALSE;
    }
}

BOOL BuildingActor::tryOpenDoorForEntry() {
    if (entryState == 0) {
        return openDoorForEntry();
    }
    return FALSE;
}

BOOL BuildingActor::openDoorForExit() {
    switch (getEntranceType()) {
    case 2:
        return setDoorState(2);
    case 3:
        return setDoorState(6);
    case 1:
        return setDoorState(3);
    default:
        return FALSE;
    }
}

BOOL BuildingActor::tryOpenDoorForExit() {
    if (entryState == 0) {
        return openDoorForExit();
    }
    return FALSE;
}

u16 *BuildingActor::getItemId() { return &itemId; }

u32 BuildingActor::getGridX() { return gridX; }

u32 BuildingActor::getGridZ() { return gridZ; }

s32 BuildingActor::callIsLit() { return lights.isLit(); }

BuildingSeEmitter::BuildingSeEmitter() {
    active = 0;
}

// The emitter's destructor (D1 of SndSeEmitterKind2, implicit in autoload_2; ov009 has its own copy).
Unk_0213b9c4::~Unk_0213b9c4() {}

void BuildingSeEmitter::activate() {
    if (active == 0) {
        emitter.callInit();
        active = 1;
    }
}

void BuildingSeEmitter::setPosition(VecFx32 *v) {
    if (active != 0) {
        VecFx32 t;
        t.x = v->x;
        t.y = v->y;
        t.z = v->z;
        emitter.callUpdateRelative(&t);
    }
}

void BuildingSeEmitter::deactivate() {
    if (active != 0) {
        emitter.callStop();
        active = 0;
    }
}

void BuildingSeEmitter::playSe(u32 a) {
    if (active != 0) {
        Snd_SeEmitterPlayOneShot(&emitter, a, 0x7f, 0);
    }
}

void BuildingSeEmitter::playSeHeld(u32 a) {
    if (active != 0) {
        Snd_SeEmitterPlayHeld(&emitter, a, 0x7f, 0);
    }
}

VecFx32Ctor BuildingActor::getSoundPos() {
    VecFx32Ctor r;
    r.x = position.x;
    r.y = position.y;
    r.z = position.z;
    return r;
}

// Slots b4 / b8 of the vtable (were free functions BuildingActor::getSoundPos / BuildingActor::calcCustomBaseMatrix)
BOOL BuildingActor::calcCustomBaseMatrix(Mtx43 *out) { return 0; }

// ---------------------------------------------------------------- data

extern "C" ActorProfile sBuildingActorProfile = {(void *(*)())BuildingActor_Create, 0x1a, 0x20, 0, 0xc8000, 0x12c000, 0x258000};
extern "C" char sBuildingDefaultMsgFile[16] = "obj_etc_error";
extern "C" char data_ov009_0225e3e8[4] = "STR";
extern "C" char data_ov009_0225e3ec[16] = "STR:a/bca/bca0";
extern "C" char data_ov009_0225e3fc[16] = "STR:a/bca/bca1";
extern "C" char data_ov009_0225e40c[16] = "STR:a/bca/bca2";
extern "C" char data_ov009_0225e41c[16] = "STR:a/bta/bta%d";
extern "C" char data_ov009_0225e42c[16] = "STR:a/btp/btp%d";
extern "C" char data_ov009_0225e43c[16] = "STR:a/bmd/bmd0";
extern "C" char data_ov009_0225e44c[16] = "STR:a/bmd/bmd1";
extern "C" char data_ov009_0225e45c[24] = "STR:a/bshadow/bshadow0";
extern "C" char sBuildingLightTexPathFmt[32] = "/str/arc/%d/str%d%c_lt.nsbtx";
extern "C" char sBuildingTexPathFmt[28] = "/str/arc/%d/str%d%c.nsbtx";
extern "C" char sBuildingArcPathFmt[24] = "/str/arc/%d/str%d%c.arc";
extern "C" char sBuildingArcPath[32] = {0};
extern "C" char sBuildingTexPath[32] = {0};
extern "C" char sBuildingLightTexPath[32] = {0};

// Run when the overlay is loaded (modern/host/overlay.c), as the DS's __sinit: the default constructors of
// the unit's objects.
extern "C" void StaticInit_unk_ov009_0225b880(void) {
    HOST_STATIC_CONSTRUCT(sBuildingResources);
}

// ---- overlay variables (generated: tools/host_overlay_vars.py) ----
#include "host_overlay_vars.h"

HOST_OVERLAY_VARS(unk_ov009_0225b880) {
    HOST_OVERLAY_VAR(data_ov009_0225e4fc),
    HOST_OVERLAY_VAR(data_ov009_0225e4e0),
    HOST_OVERLAY_VAR(data_ov009_0225e4f4),
    HOST_OVERLAY_VAR(data_ov009_0225e4f0),
    HOST_OVERLAY_VAR(data_ov009_0225e500),
    HOST_OVERLAY_VAR(data_ov009_0225e4f8),
    HOST_OVERLAY_VAR(sBuildingResources),
    HOST_OVERLAY_VAR(sBuildingActorProfile),
    HOST_OVERLAY_VAR(sBuildingDefaultMsgFile),
    HOST_OVERLAY_VAR(data_ov009_0225e3e8),
    HOST_OVERLAY_VAR(data_ov009_0225e3ec),
    HOST_OVERLAY_VAR(data_ov009_0225e3fc),
    HOST_OVERLAY_VAR(data_ov009_0225e40c),
    HOST_OVERLAY_VAR(data_ov009_0225e41c),
    HOST_OVERLAY_VAR(data_ov009_0225e42c),
    HOST_OVERLAY_VAR(data_ov009_0225e43c),
    HOST_OVERLAY_VAR(data_ov009_0225e44c),
    HOST_OVERLAY_VAR(data_ov009_0225e45c),
    HOST_OVERLAY_VAR(sBuildingLightTexPathFmt),
    HOST_OVERLAY_VAR(sBuildingTexPathFmt),
    HOST_OVERLAY_VAR(sBuildingArcPathFmt),
    HOST_OVERLAY_VAR(sBuildingArcPath),
    HOST_OVERLAY_VAR(sBuildingTexPath),
    HOST_OVERLAY_VAR(sBuildingLightTexPath),
    HOST_OVERLAY_VARS_END
};
