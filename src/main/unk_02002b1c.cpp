#include "uptr.h"
#include "types.h"
#include "gfx/VecFx32.h"
#include "Unk_020d8c7c.h"
#include "gfx/ToonTable.h"
#include "gfx/Mtx43.h"
#include "actor/ActorCollider.h"
#include "actor/ActorListNode.h"
#include "gfx/ViewFrustum.h"
#include "gfx/AbAllObjGfx.h"
#include "npc/VillagerId.h"
#include "actor/Actor.h"
#include "actor/ActorProfile.h"
#include "proto/autoload_2/unk_020e8558.h"
#include "proto/autoload_2/unk_02105d50.h"
#include "proto/autoload_2/unk_0210fa84.h"
#include "proto/autoload_2/unk_02110c98.h"
#include "proto/autoload_2/unk_02119434.h"
#include "proto/main/unk_020027b4.h"
#include "proto/main/unk_020029e8.h"
#include "proto/main/unk_02002b1c.h"
#include "proto/main/unk_02003008.h"
#include "proto/main/unk_0202e840.h"
#include "proto/main/unk_02063904.h"
#include "proto/main/unk_02063f3c.h"
#include "proto/main/unk_020a427c.h"
#include "proto/main/unk_020d477c.h"

extern "C" {
extern s16 data_02135f44[];  // the sine / cosine table (FX_SinCosTable_)
extern ProcProfile **gProfileTable;
void List_PushBack(void *list, void *node);
s16 Math_Atan2(s32 x, s32 y);
void Mtx43_SetTranslate(void *m, s32 a, s32 b, s32 c);
void Mtx43_RotateX(void *m, s32 a);
void Mtx43_RotateY(void *m, s32 a);
s32 func_01ffcb0c(s32 a, s32 b);
void VEC_Add(void *a, void *b, void *c);
BOOL Villager_GetSpeciesName(MsgString *buf, u32 x);
void *ProcList_FindByProfile(void *list, u32 id, void *p);
void *ProcList_FindById(void *list, u32 id);
}
extern ViewFrustum gViewFrustum;

ActorList gActorList;
void *sActorSpawnPos;
void *sActorSpawnRot;

extern "C" u32 VillagerId_GetVoiceType(VillagerId *o) { return Villager_PersonalityToVoiceType(o->personality); }

extern "C" u32 VillagerId_GetSpecies(VillagerId *o) { return o->species; }

extern "C" u32 VillagerId_IsValidSpecies(u32 id) {
    if (id < 0x96) return TRUE;
    return FALSE;
}

BOOL VillagerId::getName(MsgString *buf) {
    BOOL r = FALSE;
    if (isValid() == 1) r = Villager_GetSpeciesName(buf, species);
    return r;
}

Actor::Actor() {
    listNode.prev = 0;
    listNode.next = 0;
    listNode.owner = this;
    List_PushBack(&gActorList, &listNode);
    VecFx32 *v = (VecFx32 *)sActorSpawnPos;
    if (v) {
        position.x = v->x;
        position.y = v->y;
        position.z = v->z;
    }
    VecFx16 *w = (VecFx16 *)sActorSpawnRot;
    if (w) {
        rotX = w->x;
        rotY = w->y;
        rotZ = w->z;
        VecFx16 *x = (VecFx16 *)sActorSpawnRot;
        moveAngleX = x->x;
        moveAngleY = x->y;
        moveAngleZ = x->z;
    }
    ActorProfile *e = (ActorProfile *)gProfileTable[profile];  // an actor's profile is an ActorProfile (ProcProfile first)
    actorFlags = e->actorFlags;
    setCullParams(e->cullHeight, e->cullRadius, e->cullDepth);
}

BOOL Actor::preCreate() {
    if (ProcBase::preCreate()) return TRUE;
    return FALSE;
}

void Actor::postCreate(s32 status) {
    GameProc::postCreate(status);
    actorFlags |= 4;
}

BOOL Actor::preDelete() {
    if (ProcBase::preDelete()) return TRUE;
    return FALSE;
}

BOOL Actor::postDelete(s32 status) { return ProcBase::postDelete(status); }

BOOL Actor::preExecute() {
    s32 r4;
    if (!ProcBase::preExecute()) return FALSE;
    prevPosition.x = position.x;
    prevPosition.y = position.y;
    prevPosition.z = position.z;
    if (cullRadius) {
        s32 x = drawPos.z + func_01ffcb0c(cullHeight, data_02135f44[(drawTilt >> 4) * 2]);
        s32 z = drawPos.y + func_01ffcb0c(cullHeight, data_02135f44[(drawTilt >> 4) * 2 + 1]);
        s32 v[3];
        v[0] = drawPos.x;
        v[1] = z;
        v[2] = x;
        r4 = gViewFrustum.testSphere(&gViewMtx, v, cullRadius, (s32 *)viewPos);
    }
    actorFlags &= ~4;
    if (actorFlags & 3) {
        if (r4 > cullDepth) {
            actorFlags |= 4;
            if (actorFlags & 1) return FALSE;
        }
    }
    return TRUE;
}

BOOL Actor::postExecute(u32 status) { return ProcBase::postExecute(status); }

BOOL Actor::preDraw() {
    if (!ProcBase::preDraw()) return FALSE;
    if ((actorFlags & 4) && (actorFlags & 2)) return FALSE;
    return TRUE;
}

BOOL Actor::postDraw(s32 status) { return ProcBase::postDraw(status); }

void *Actor::findById(u32 id) {
    void *r = ProcList_FindById(&gActorList, id);
    if (r) return ((ActorListNode *)r)->owner;
    return 0;
}

void *Actor::findByProfile(u32 id, Actor *o) {
    void *r;
    if (o != 0) {
        r = ProcList_FindByProfile(&gActorList, id, &o->listNode);
    } else {
        r = ProcList_FindByProfile(&gActorList, id, 0);
    }
    if (r) return ((ActorListNode *)r)->owner;
    return 0;
}

void Actor::setSpawnTransform(void *a, void *b) {
    sActorSpawnPos = a;
    sActorSpawnRot = b;
}

void *Actor::spawn(u32 profile, u32 param, void *pos, void *angle, void *parent) {
    void *t = parent;
    if (t == 0) t = gActorDefaultParent;
    setSpawnTransform(pos, angle);
    return GameProc_CreateChild(profile, t, param, 3);
}

void Actor::setCullParams(s32 a, s32 b, s32 c) {
    cullHeight = a;
    cullRadius = b;
#ifdef HOST
    // (drawn twice as far as on the DS: buildings and the other actors no longer pop in and out at the DS's distance;
    // FieldObj_DrawUnits does the same for trees and the rest of the ground's objects)
    cullDepth = c * 2;
#else
    cullDepth = c;
#endif
}

void Actor::applyVelocity(ActorCollider *v) {
    VEC_Add(&position, &velocity, &position);
    if (v) {
        position.x = position.x + v->pushX;
        position.z = position.z + v->pushZ;
    }
}

void Actor::calcVelocity() {
    if (speed == 0) {
        s32 v = maxFallSpeed;
        s32 w = velocity.y + gravity;
        if (w >= v) v = w;
        velocity.x = 0;
        velocity.y = v;
        velocity.z = 0;
    } else {
        s32 r = func_01ffcb0c(speed, data_02135f44[((u16)moveAngleY >> 4) * 2 + 1]);
        s32 v = maxFallSpeed;
        s32 w = velocity.y + gravity;
        if (w >= v) v = w;
        velocity.x = func_01ffcb0c(speed, data_02135f44[((u16)moveAngleY >> 4) * 2]);
        velocity.y = v;
        velocity.z = r;
    }
}

void Actor::updatePosition(ActorCollider *v) {
    calcVelocity();
    applyVelocity(v);
}

extern "C" s32 Math_AngleXZ(const VecFx32 *a, const VecFx32 *b) {
    return Math_Atan2(b->x - a->x, b->z - a->z);
}

void Actor::calcModelMatrix(void *out) {
    Mtx43 m;
    Mtx43_SetTranslate(&m, drawPos.x, drawPos.y, drawPos.z);
    Mtx43_RotateX(&m, (s16)drawTilt);
    Mtx43_RotateY(&m, rotY);
    if (rotX != 0) Mtx43_RotateX(&m, rotX);
    *(Mtx43 *)out = m;
}


// ACTOR_POS_AT (include/uptr.h)
extern "C" unsigned char *HostActor_Pos(void *actor) { return (unsigned char *)&((Actor *)actor)->position; }
