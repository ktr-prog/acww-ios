#include "ds_div.h"
#include "types.h"
#include "gfx/VecFx32.h"
#include "game/SegmentHitResult.h"
#include "game/Unk_0202f2ac_V3.h"
#include "game/GroundInfoBase.h"
#include "game/UnitShapeQueryX.h"
#include "game/CollisionCircle.h"
#include "game/CollisionState.h"
#include "game/FxVec3.h"
#include "game/GroundInfo.h"
#include "game/CollisionCylinder.h"
#include "game/CollisionTriangleX.h"
#include "game/TriangleTrigger.h"
#include "game/CollisionVec2.h"
#include "game/CollisionEdge.h"
#include "game/CollisionTriangle.h"
#include "game/BoxCollider.h"
#include "game/CollisionMap.h"
#include "proto/itcm/unk_01ffcb2c.h"
#include "proto/itcm/unk_01ffcfc0.h"
#include "proto/main/unk_0202fa70.h"

struct CollisionVisitor;
struct UnitShapeQueryX;
class CollisionWorld;
class WallEdgeList;
class FloorTriangleList;
struct ShapeCylinderList;
struct WallEdgeListWriter;
class BoxColliderListOwner;

class GroundCell;
class GroundCellGrid;

// one entry of the terrain attribute table sGroundAttrTable (0x7c entries of 30 bytes)
struct GroundAttrEntry {
    /* 0x00 */ u16 footstepSe;
    /* 0x02 */ u16 dragSe;
    /* 0x04 */ u8 itemPlaceable;
    /* 0x05 */ u8 walkable;
    /* 0x06 */ s16 plantFlag;
    /* 0x08 */ u8 grassSurface, digKind, unk_0a, flowDirBits;
    /* 0x0c */ u8 quadAttr0Q0, quadAttr0Q1, quadAttr0Q2, quadAttr0Q3;
    /* 0x10 */ u8 quadAttr1Q0, quadAttr1Q1, quadAttr1Q2, quadAttr1Q3;
    /* 0x14 */ u8 waterKind, flatFloor, unk_16, isShore;
    /* 0x18 */ u8 mapColor0, mapColor1, mapColor2, mapColor3;
    /* 0x1c */ s16 height;
};
typedef u32 (*GroundAttrGetter)(s32);

extern const u8 sDefaultGroundColors[4];
extern const s32 data_020c7c1c;
extern const u32 sDigHoleRadii[3];
extern const GroundAttrGetter sGroundQuadAttr0Getters[4];
extern const GroundAttrGetter sGroundQuadAttr1Getters[4];
extern const GroundAttrEntry sGroundAttrTable[0x7c];

extern "C" {
s32 func_01ffcb0c(s32 a, s32 b);
void VEC_Subtract(void *a, void *b, void *c);
void Vec_Sub(void *out, void *a, void *b);
s32 Vec_SafeNormalize(void *v);
void Vec_RotateY(void *v, s32 angle);  // (a fixed declaration: Apple arm64 passes a variadic call's angle on the stack)

// functions of this unit that the files declared with different stand-in prototypes
void Collision_Query(VecFx32 *a, VecFx32 *b, CollisionVisitor *visitor, u32 flags, u8 p5, u8 p6);
s32 Ground_GetHeightAt(VecFx32 *p, u32 *out, u32 flags0);
u32 CollisionMap_IsBound(s32 a);
u32 Ground_GetWalkLinks(s32 x, s32 y);
BOOL Ground_IsRaisedOrOccupied(VecFx32 *v);
UnitShapeQueryX *Collision_GetShapeQuery();
u32 Ground_GetQuadAttr1(s32 x, s32 y, u32 c);
u32 Ground_GetQuadAttr0(s32 x, s32 y, u32 c);
s32 Ground_GetUnitQuadrant(VecFx32 *p);
void Vec3_MinInPlace(s32 *a, s32 *b);
void Vec3_MaxInPlace(s32 *a, s32 *b);
u32 CollisionMap_IsFullyBound(s32 i);
void BoxCollider_ClearList(void *p);
void TriangleTrigger_CheckAll(VecFx32 *a, s32 b, Actor *c);
void TriangleTrigger_ClearList(void *p);
void *CollisionTag_Destruct(void *p);
void *CollisionTag_Construct(void *p);
void GroundInfo_Destruct(void *obj);

// methods called through a pointer to another view of the object (the names are the symbols)
}

// ---- 2D vector; its functions and the empty destructor at 0x0202ea3c belong to the unit at 0x0202e9d4


// ---- 3D vector with the destructor at 0x02000c8c
extern FxVec3 sCollisionUpVector;
extern FxVec3 sCollisionQueryMargin;
extern CollisionWorld sCollisionWorld;

// ---- triangle (vtable 0x020d8cc4, unit at 0x0202e9d4)

// ---- {attribute, callback} pair (functions 0x020339cc-0x020339f8; symbols.txt: CollisionTag, see aliases.txt)
class CollisionTagX {
public:
    CollisionTagX();
    ~CollisionTagX();
    void copyTag(const CollisionTagX &o);
    void setTag(u32 a, BoxCollider *owner);

    u8 attr;
    BoxCollider *f_04;
};

// ---- the two list owners inside sCollisionWorld (empty classes)
class BoxColliderListOwner {
public:
    BoxColliderListOwner();
    ~BoxColliderListOwner();
};

class TriangleTriggerListOwner {
public:
    TriangleTriggerListOwner();
    ~TriangleTriggerListOwner();
};

// ---------------------------------------------------------------- unk_0202f600.cpp
extern "C" s32 FX_Div(s32 a, s32 b);
extern "C" s32 FX_Sqrt(s32 a);
extern "C" s32 Vec_DistXZ(VecFx32 *a, VecFx32 *b);
struct FloorBoundsRange { s32 lo, hi; };
struct FloorBoundsStackPad { s32 v[6]; FloorBoundsStackPad() {} ~FloorBoundsStackPad() {} };

extern "C" void Ground_SetWalkLinks(s32 x, s32 z, s32 v);
extern "C" void Ground_ClearWalkLinks(s32 x, s32 z, s32 v);
#define TB(i, f, d) ((i) < 0x7c ? sGroundAttrTable[i].f : (d))
#define TS(i, d) ((i) < 0x7c ? ((v = sGroundAttrTable[i].plantFlag) > 0 ? 1 : v) : (d))
extern "C" void CollisionMap_Reset(s32 idx);
extern "C" void CollisionMap_ClearBlocks(s32 idx);
extern "C" void CollisionMap_Select(s32 v);
extern "C" BOOL Collision_GetUnitShape(s32 x, s32 z, s32 *p, s32 *q, s32 *r);
extern "C" s32 Collision_HasUnitShape(s32 x, s32 z);
enum Unk_0203081c_Flags { Unk_0203081c_Flags_0 = 0, Unk_0203081c_Flags_2 = 2, Unk_0203081c_Flags_4 = 4, Unk_0203081c_Flags_All = 0x7fffffff };

static inline void Unk_02030e48_Set(VecFx32 *v, s32 x, s32 y, s32 z) { v->x = x; v->y = y; v->z = z; }
struct GroundCellView {
    s32 quadHeights[4];
    u8 quadAttrs[4];
    u8 unitAttr;
};
extern "C" s32 Ground_CanPlaceItem(s32 a, s32 b);
extern "C" BOOL Ground_IsShore(s32 a, s32 b);
extern "C" BOOL Ground_IsWaterAround(VecFx32 *pos, s32 r, s32 *out, s32 flags);
extern "C" BOOL Ground_FindWaterAlongDir(VecFx32 *out, VecFx32 *pos, u32 dist, s32 dir, u32 count, s32 r, s32 flags);
extern "C" BOOL Ground_IsGrassSurface(s32 a, s32 b);
static inline BOOL Unk_02030f10_Flat(GroundCellView *T)
{
    BOOL f = FALSE, e = FALSE;
    if (T->quadHeights[0] == T->quadHeights[1] && T->quadHeights[0] == T->quadHeights[2]) e = TRUE;
    if (e && T->quadHeights[0] == T->quadHeights[3]) f = TRUE;
    return f;
}
struct NeighbourReachLocals { VecFx32Ctor R; GroundCellView T; VecFx32Ctor S; GroundCellView T2; };
static inline BOOL Unk_02030be4_A(GroundCellView *T)
{
    BOOL g = FALSE, f = FALSE, e = FALSE;
    if (T->quadAttrs[0] == 0x14 && T->quadAttrs[1] == 0x14) e = TRUE;
    if (e && T->quadAttrs[2] == 0x14) f = TRUE;
    if (f && T->quadAttrs[3] == 0x14) g = TRUE;
    return g;
}
static inline BOOL Unk_02030be4_B(GroundCellView *T)
{
    if (T->quadAttrs[0] == 0x14 && T->quadAttrs[1] == 0x14 && T->quadAttrs[2] == 0x14 && T->quadAttrs[3] == 0x14) return TRUE;
    return FALSE;
}
extern "C" void VEC_Add(VecFx32 *out, VecFx32 *a, VecFx32 *b);

extern "C" BOOL Collision_GetUnitShape(s32 x, s32 z, s32 *a, s32 *b, s32 *c);
struct Unk_02031960_P8 { s32 a, b; };
extern u8 data_021f47e0[];
extern "C" s32 Vec_NotEqual(void *a, void *b);
extern "C" void Mtx43_SetTranslate(void *m, s32 x, s32 y, s32 z);
extern "C" void Mtx43_RotateY(void *m, s32 ang);
extern "C" void Mtx43_Scale(void *m, s32 x, s32 y, s32 z);
extern "C" void MTX_MultVec43(VecFx32 *p, void *m, VecFx32 *out);
static inline s32 Unk_02031618_Abs(s32 v) { if (v < 0) v = -v; return v; }
extern "C" BOOL Collision_IsSegmentOutsideBox(VecFx32 *a, VecFx32 *b, VecFx32 *c, VecFx32 *d);

extern "C" void Collision_CalcTriangleNormal(void* out, VecFx32* a, VecFx32* b, VecFx32* c);
static inline BOOL Unk_02031f90_Ge0(s32 v) {
    if (v >= 0) {
        return TRUE;
    }
    return FALSE;
}
// ---- collision visitor base (vtable 0x020d8cf8)
struct CollisionVisitor {
    CollisionVisitor();
    ~CollisionVisitor();
    virtual void visitWalls(WallEdgeList *p);
    virtual void visitFloors(FloorTriangleList *p);
    virtual void visitCylinders(ShapeCylinderList *p);
};

struct WallCornerPadded {
    VecFx32Ctor v4;
    s32 pad[3];
    WallCornerPadded(s32 a, s32 b, s32 c) : v4(a, b, c) {}
};
// ---------------------------------------------------------------- CollisionState (accumulated collision flags) and friends
// ---------------------------------------------------------------- MoveCollisionVisitor (collision accumulator driver)
struct MoveCollisionVisitor : CollisionVisitor {
    CollisionState* state;
    VecFx32* pos;
    VecFx32 prevPos;
    u16 facingAngle;
    s32 radius;
    Actor *actor;
    u32 flags;

    MoveCollisionVisitor() {}
    virtual void visitWalls(WallEdgeList *p);
    virtual void visitFloors(FloorTriangleList *p);
    virtual void visitCylinders(ShapeCylinderList *p);
};

struct SegmentCollisionVisitor : CollisionVisitor {
    VecFx32* volatile segEnd;
    VecFx32 segStart;
    u32 flags;
    u8 hasHit;
    u8 unk_19[3];
    CollisionTagX hitTag;
    VecFx32 hitNormal;
    s32 hitAttr;
    s32 numWalls;
    s32 numFloors;
    s32 numCylinders;

    SegmentCollisionVisitor() {}
    virtual void visitWalls(WallEdgeList *p);
    virtual void visitFloors(FloorTriangleList *p);
    virtual void visitCylinders(ShapeCylinderList *p);
};

extern "C" s32 Math_Atan2(s32 x, s32 z);
struct CellWallNormal : VecXZ {
    inline CellWallNormal(s32 x, s32 z) {
        ((CollisionVec2 *)(this))->set(x, z);
    }
    ~CellWallNormal() {}  // CollisionVec2's (empty) destructor on the DS
};
struct ShapeCylinder : CollisionCylinderX, CollisionTagX {
    ShapeCylinder();
    ~ShapeCylinder();
    void setupShape(VecFx32 *pos, s32 b, s32 c, s32 a4, s32 a5);
    /* 0x1c */ s32 kind;
    /* 0x20 */ s16 unitX;
    /* 0x22 */ s16 unitZ;
};

BOOL ShapeCylinder_AddLinkWall(ShapeCylinder *a, WallEdgeListWriter *out, ShapeCylinder *b);
struct ShapeCylinderList {
    ShapeCylinderList();
    ~ShapeCylinderList();
    void addLinkWalls(WallEdgeListWriter *out);
    void collectFromUnits(UnitShapeQueryX *query, s32 x0, s32 x1, s32 z0, s32 z1, s32 flag);
    BOOL addCylinder(VecFx32 *pos, s32 b, s32 c, s32 a4, s32 a5);
    BOOL landOnTop(VecFx32 *pos, VecFx32 *prev, u32 *out);
    BOOL pushOut(VecFx32 *pos, s32 x, CollisionState *state);
    volatile u32 numCylinders;
    ShapeCylinder cylinders[16];
};

// ---------------------------------------------------------------- unk_02032dc4.cpp
extern "C" s32 Math_Atan2(s32 a, s32 b);
extern "C" BOOL GroundAttr_IsWater(s32 id, s32 a);
extern s32 gCollisionVec2Zero[];
static inline void Unk_02032dc4_Set(Unk_0202f2ac_V3 *p, s32 y, s32 x, s32 z) {
    p->x = x;
    p->y = y;
    p->z = z;
}
class WallEdge;
// declared before WallEdge: the three weak vtables (0x020d8d48, d54, d6c) come out in reverse declaration order
class FloorTriangle : public CollisionTriangleX, public CollisionTagX {
public:
    FloorTriangle();
    ~FloorTriangle();
    BOOL setupFloor(Unk_0202f2ac_V3 *a, Unk_0202f2ac_V3 *b, Unk_0202f2ac_V3 *c, Unk_0202f2ac_V3 *d, u32 e, BoxCollider *f);
};

class WallEdge : public CollisionEdge, public CollisionTagX {
public:
    WallEdge();
    WallEdge(CollisionVec2 *a, CollisionVec2 *b, CollisionVec2 *c, s32 p4, s32 p5, u32 p6, BoxCollider *p7);
    ~WallEdge();
    virtual BOOL hasRoundEnds()
    {
        if (kind == 3) {
            return FALSE;
        }
        return TRUE;
    }
    s32 kind, topY;
    BOOL setupEdge(CollisionVec2 *a, CollisionVec2 *b, CollisionVec2 *c, s32 p4, s32 p5, u32 p6, BoxCollider *p7);
    BOOL copyEdge(WallEdge *o);
};
class WallEdgeList {
public:
    WallEdge edges[24];
    u32 numEdges;
    WallEdgeList();
    ~WallEdgeList();
    BOOL collide(VecFx32 *pos, VecFx32 *q, s32 r, CollisionState *out, Actor *actor);
};
// the list's filling side (the collision query's wall gathering): the list itself (it was a DS-layout view - 0x30-byte
// edges, the count at 0x480 - which on 64-bit hosts wrote the edges and the count elsewhere: no walls were found)
struct WallEdgeListWriter : WallEdgeList {
    BOOL addEdge(void *a, void *b, void *c, s32 d0, s32 d1, s32 d2, BoxCollider *d3);
    BOOL addEdgeCopy(WallEdge *e);
    void buildFromCells(void *grid, s32 x0, s32 x1, s32 z0, s32 z1, s32 flag);
};
class FloorTriangleList {
public:
    FloorTriangle triangles[40];
    volatile u32 numTriangles;
    FloorTriangleList();
    ~FloorTriangleList();
    void buildFromCells(GroundCellGrid *grid, s32 x0, s32 x1, s32 y0, s32 y1);
    BOOL addTriangle(Unk_0202f2ac_V3 *a, Unk_0202f2ac_V3 *b, Unk_0202f2ac_V3 *c, Unk_0202f2ac_V3 *d, u32 e, BoxCollider *f);
};
static inline void Unk_020331a8_SetU(u32 *v, s32 a, s32 b, s32 c) { v[0] = a; v[1] = b; v[2] = c; }
static inline void Unk_02033438_Set(Unk_0202f2ac_V3 *p, s32 x, s32 y, s32 z) {
    p->x = x;
    p->y = y;
    p->z = z;
}
class GroundInfoCalc {
public:
    u8 onShore;
    Unk_0202f2ac_V3 shorelinePoint;
    Unk_0202f2ac_V3 queryPos;
    s32 unitX, unitZ;
    Unk_0202f2ac_V3 flowDir;
    s32 waterKind, attr, height, waterSurfaceY;
    void compute(Unk_0202f2ac_V3 *pos, s32 flag, s32 arg);
};

// ---------------------------------------------------------------- unk_0203389c.cpp
extern "C" s32 FX_Div(s32 a, s32 b);
extern "C" long long Vec_DistSqXZ(void *a, void *b);
extern "C" BOOL Collision_GetUnitShape(s32 a, s32 b, s32 *c, s32 *d, s32 *e);
class CollisionMapIndex {
public:
    BOOL setIndex(s32 v);
    s32 curMapIndex;
};
class GroundCell {
public:
    GroundCell();
    ~GroundCell();
    void load(s32 x, s32 y, s32 flag);
    GroundCell *loadAtPos(VecFx32 *p, s32 flag);

    s32 quadHeight0;
    s32 quadHeight1;
    s32 quadHeight2;
    s32 quadHeight3;
    u8 quadAttr0;
    u8 quadAttr1;
    u8 quadAttr2;
    u8 quadAttr3;
    u8 unitAttr;
};
class GroundCellGrid {
public:
    GroundCellGrid();
    GroundCell *getCell(s32 x, s32 z);
    void loadArea(s32 x0, s32 x1, s32 z0, s32 z1, s32 flag);

    GroundCell cells[0x31];
    s32 minX;
    s32 maxX;
    s32 minZ;
    s32 maxZ;
};
class DigHoleColliders {
public:
    DigHoleColliders() { clear(); }
    u32 getRadiusAt(s32 a, s32 b);
    void update();
    s32 findFreeSlot();
    BOOL addHole(s32 a, s32 b);
    void clear();

    u8 activeMask;
    u8 ages[4];
    s8 unitXs[4];
    s8 unitZs[4];
};
// the collision work area sCollisionWorld
class CollisionWorld {
public:
    ~CollisionWorld() {}

    /* 0x0000 */ s32 curMapIndex;
    /* 0x0004 */ CollisionMap maps[8];
    /* 0x09c4 */ WallEdgeList wallEdges;
    /* 0x0e48 */ FloorTriangleList floorTriangles;
    /* 0x184c */ ShapeCylinderList shapeCylinders;
    /* 0x1a90 */ BoxColliderListOwner boxColliders;
    /* 0x1a91 */ TriangleTriggerListOwner triangleTriggers;
    /* 0x1a94 */ GroundCellGrid cellGrid;
    /* 0x1f3c */ DigHoleColliders digHoles;
};

// ---------------------------------------------------------------- inline helpers
static inline CollisionBlockRef *Unk_0203030c_Get(CollisionMap *m, s32 x, s32 y) {
    if (x >= 0 && y >= 0 && (u32)x < m->numBlocksX && (u32)y < m->numBlocksZ) return &m->blocks[y][x];
    return NULL;
}
static inline BOOL Unk_020303d0_IsSet(CollisionBlockRef *c) {
    if (c->attrs != 0 && c->walkLinks != 0) return TRUE;
    return FALSE;
}
static inline u8 Unk_020303d0_All(s32 idx) {
    s32 i, j;
    for (i = sCollisionWorld.maps[idx].numBlocksZ - 1; i >= 0; i--) {
        for (j = sCollisionWorld.maps[idx].numBlocksX - 1; j >= 0; j--) {
            CollisionBlockRef *row = sCollisionWorld.maps[idx].blocks[i];
            if (!Unk_020303d0_IsSet(&row[j])) return 0;
        }
    }
    return 1;
}

// ---------------------------------------------------------------- functions of this unit
extern "C" BOOL Ground_GetFloorBounds(s32 *a, s32 *b, s32 *c, s32 *d);
extern "C" BOOL Ground_UnlockExit(void);
extern "C" BOOL Ground_IsOnLockedExit(VecFx32 *p);
extern "C" BOOL Ground_LockExit(s32 v);
extern "C" s32 Ground_GetExitAtPos(VecFx32 *p);
extern "C" s32 Ground_GetExitAt(s32 x, s32 z);
extern "C" void Ground_UnlinkUnit(s32 x, s32 z);
extern "C" BOOL Ground_SetQuadrantsBlocked(s32 x, s32 z, s32 mask);
extern "C" BOOL Ground_ClearPlantFlag(s32 x, s32 z);
extern "C" void Ground_SetWaveLevel(s32 v);
extern "C" void CollisionMap_Release(s32 idx);
extern "C" void CollisionMap_ClearBlocks(s32 idx);
extern "C" void CollisionMap_Reset(s32 idx);
extern "C" BOOL CollisionMap_SetBlock(s32 x, s32 y, void *bcl, s32 idx);
extern "C" void Ground_ClearWalkLinks(s32 x, s32 z, s32 mask);
extern "C" void Ground_SetWalkLinks(s32 x, s32 z, s32 v);
extern "C" void Collision_AddDigHole(s32 a, s32 b);
extern "C" void Collision_UpdateDigHoles(void);
extern "C" BOOL CollisionMap_Bind(u32 a, u32 b, UnitShapeQueryX *o, s32 idx);
extern "C" void CollisionMap_Select(s32 v);
extern "C" void Collision_Query(VecFx32 *a, VecFx32 *b, CollisionVisitor *visitor, u32 flags, u8 p5, u8 p6);
extern "C" s32 Collision_HasUnitShapeAt(VecFx32 *p);
extern "C" s32 Collision_HasUnitShape(s32 x, s32 z);
extern "C" BOOL Collision_GetUnitShape(s32 x, s32 z, s32 *p, s32 *q, s32 *r);
extern "C" s32 Ground_GetDefaultY(void);
extern "C" s32 Ground_GetHeightAt(VecFx32 *p, u32 *out, u32 flags0);
extern "C" BOOL Collision_ClampToRect(s32 *p, s32 a, s32 *c, s32 w, s32 h);
extern "C" u8 Collision_TestSegment(SegmentHitResult *out, VecFx32 *pos, VecFx32 *tgt, u32 flags);
extern "C" void Collision_Move(CollisionState *self, VecFx32 *pos, VecFx32 *tgt, u16 hh, s32 arg5, Actor *actor, u32 flags);
extern "C" s32 Ground_GetSpecialPieceKind(s32 x, s32 y);
extern "C" s32 Ground_FindTerrainMarker(s32 *a, s32 *b, s32 c, s32 d);
extern "C" u32 CollisionMap_IsBound(s32 a);
extern "C" BOOL Ground_IsWaterAt(VecFx32 *pos);
extern "C" BOOL Ground_FindWaterAhead(VecFx32 *out, VecFx32 *pos, s32 dir, u32 dist, u32 s5, s32 s6);
extern "C" BOOL Ground_FindWaterAlongDir(VecFx32 *out, VecFx32 *pos, u32 dist, s32 dir, u32 count, s32 r, s32 flags);
extern "C" BOOL Ground_IsWaterAround(VecFx32 *pos, s32 r, s32 *out, s32 flags);
extern "C" BOOL Ground_IsNeighbourReachable(s32 a, s32 b, s32 c, s32 d, u8 flag);
extern "C" u16 GroundAttr_GetDragSe(s32 i);
extern "C" u16 GroundAttr_GetFootstepSe(s32 i);
extern "C" BOOL Ground_GetMapColors(u8 *out, s32 a, s32 b);
extern "C" BOOL Ground_IsFreeGrass(s32 a, s32 b);
extern "C" BOOL Ground_IsFreeGrassOffPath(s32 a, s32 b);
extern "C" u32 Ground_GetWalkLinks(s32 x, s32 y);
extern "C" BOOL Ground_IsSandAboveSea(s32 a, s32 b);
extern "C" BOOL Ground_IsShore(s32 x, s32 y);
extern "C" s32 Ground_GetPlantFlag(s32 x, s32 y);
extern "C" s32 Ground_GetDigKind(s32 x, s32 y);
extern "C" s32 Ground_IsWalkable(s32 x, s32 y);
extern "C" s32 Ground_IsGrassSurface(s32 x, s32 y);
extern "C" s32 Ground_CanPlaceItem(s32 x, s32 y);
extern "C" s32 Ground_GetWaterKind(s32 x, s32 y);
extern "C" BOOL Ground_IsGrassUnit(s32 x, s32 y);
extern "C" BOOL Ground_IsPond(s32 x, s32 y);
extern "C" BOOL Ground_IsRaisedOrOccupied(VecFx32 *v);
extern "C" BOOL GroundAttr_IsWater(s32 t, s32 k);
extern "C" UnitShapeQueryX *Collision_GetShapeQuery();
extern "C" u32 Ground_GetQuadAttr1(s32 x, s32 y, u32 c);
extern "C" u32 GroundAttr_GetQuadAttr1Q0(s32 t);
extern "C" u32 GroundAttr_GetQuadAttr1Q1(s32 t);
extern "C" u32 GroundAttr_GetQuadAttr1Q2(s32 t);
extern "C" u32 GroundAttr_GetQuadAttr1Q3(s32 t);
extern "C" u32 Ground_GetQuadAttr0(s32 x, s32 y, u32 c);
extern "C" u32 GroundAttr_GetQuadAttr0Q0(s32 t);
extern "C" u32 GroundAttr_GetQuadAttr0Q1(s32 t);
extern "C" u32 GroundAttr_GetQuadAttr0Q2(s32 t);
extern "C" u32 GroundAttr_GetQuadAttr0Q3(s32 t);
extern "C" s32 Ground_GetUnitQuadrant(VecFx32 *p);
extern "C" void Vec3_MinInPlace(s32 *a, s32 *b);
extern "C" void Vec3_MaxInPlace(s32 *a, s32 *b);
extern "C" u32 CollisionMap_IsFullyBound(s32 i);
extern "C" void BoxCollider_GatherAll(BoxColliderListOwner *unused, VecFx32 *pos, VecFx32 *size, WallEdgeListWriter *walls, FloorTriangleList *floors, u32 flags, s32 mode);
extern "C" BOOL Collision_IsSegmentOutsideBox(VecFx32 *a, VecFx32 *b, VecFx32 *c, VecFx32 *d);
extern "C" BOOL BoxCollider_Unregister(BoxCollider *n);
extern "C" BOOL BoxCollider_Register(BoxCollider *n, s32 a, s32 b, s32 c, VecFx32 *pos, s16 e, VecFx32 *v);
extern "C" void BoxCollider_ClearList(void *p);
extern "C" void TriangleTrigger_CheckAll(VecFx32* a, s32 b, Actor *c);
extern "C" s32 TriangleTrigger_Unregister(TriangleTrigger* node);
extern "C" s32 TriangleTrigger_Register(TriangleTrigger* node);
extern "C" void TriangleTrigger_ClearList(void *p);
extern "C" void* CollisionTag_Destruct(void* p);
extern "C" void* CollisionTag_Construct(void* p);
BOOL ShapeCylinder_AddLinkWall(ShapeCylinder *a, WallEdgeListWriter *out, ShapeCylinder *b);
extern "C" void WallEdge_GetMidpoint(CollisionVec2 *out, WallEdge *e);
extern "C" void GroundInfo_Destruct(void *obj);

// ---------------------------------------------------------------- objects
// Data order: this unit is placed object by object (see object_order.txt).
TriangleTrigger *sTriangleTriggerList;
const GroundAttrEntry sGroundAttrTable[0x7c] = {
    {0xffff, 0xffff, 0, 0, -1, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0xffff, 0xffff, 0, 0, -1, 0, 2, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0xffff, 0xffff, 0, 0, -1, 0, 2, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0xc0, 0xcd, 1, 1, 1, 1, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 1, 1, 1, 1, 0},
    {0xc8, 0xce, 1, 1, -1, 0, 2, 0, 0, 4, 4, 4, 4, 4, 4, 4, 4, 0, 0, 0, 0, 7, 7, 7, 7, 0},
    {0x4a6, 0x4c1, 0, 1, -1, 0, 2, 0, 0, 5, 5, 5, 5, 5, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0x4ba, 0x4c0, 0, 1, -1, 0, 2, 0, 0, 6, 6, 6, 6, 6, 6, 6, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0xffff, 0x4c1, 0, 0, -1, 0, 3, 0, 0, 7, 7, 7, 7, 7, 7, 7, 7, 2, 0, 0, 0, 3, 3, 3, 3, -64},
    {0xffff, 0x4c1, 0, 0, -1, 0, 3, 0, 1, 8, 8, 8, 8, 8, 8, 8, 8, 1, 0, 0, 0, 3, 3, 3, 3, -64},
    {0xc4, 0xcc, 1, 1, 1, 0, 0, 0, 0, 9, 9, 9, 9, 9, 9, 9, 9, 0, 0, 0, 0, 5, 5, 5, 5, 0},
    {0xffff, 0x4c1, 0, 0, -1, 0, 2, 0, 0, 10, 10, 10, 10, 10, 10, 10, 10, 0, 0, 0, 0, 0, 0, 0, 0, 80},
    {0xffff, 0xffff, 0, 0, -1, 0, 3, 0, 16, 11, 11, 11, 11, 11, 11, 11, 11, 2, 0, 0, 0, 3, 3, 3, 3, -64},
    {0xffff, 0xffff, 0, 0, -1, 0, 3, 0, 32, 12, 12, 12, 12, 12, 12, 12, 12, 2, 0, 0, 0, 3, 3, 3, 3, -64},
    {0xffff, 0xffff, 0, 0, -1, 0, 3, 0, 64, 13, 13, 13, 13, 13, 13, 13, 13, 2, 0, 0, 0, 3, 3, 3, 3, -64},
    {0xffff, 0xffff, 0, 0, -1, 0, 3, 0, 128, 14, 14, 14, 14, 14, 14, 14, 14, 2, 0, 0, 0, 3, 3, 3, 3, -64},
    {0xffff, 0xffff, 0, 0, -1, 0, 3, 0, 1, 15, 15, 15, 15, 15, 15, 15, 15, 2, 0, 0, 0, 3, 3, 3, 3, -64},
    {0xffff, 0xffff, 0, 0, -1, 0, 3, 0, 2, 16, 16, 16, 16, 16, 16, 16, 16, 2, 0, 0, 0, 3, 3, 3, 3, -64},
    {0xffff, 0xffff, 0, 0, -1, 0, 3, 0, 4, 17, 17, 17, 17, 17, 17, 17, 17, 2, 0, 0, 0, 3, 3, 3, 3, -64},
    {0xffff, 0xffff, 0, 0, -1, 0, 3, 0, 8, 18, 18, 18, 18, 18, 18, 18, 18, 2, 0, 0, 0, 3, 3, 3, 3, -64},
    {0x87b, 0x4c2, 1, 1, 0, 0, 1, 0, 0, 19, 19, 19, 19, 19, 19, 19, 19, 0, 0, 0, 0, 5, 5, 5, 5, 0},
    {0xc8, 0x4c1, 1, 1, -1, 0, 2, 0, 0, 20, 20, 20, 20, 20, 20, 20, 20, 0, 0, 0, 0, 3, 3, 3, 3, 0},
    {0xffff, 0x4c1, 0, 0, -1, 0, 2, 0, 0, 21, 21, 21, 21, 21, 21, 21, 21, 0, 0, 0, 0, 0, 0, 0, 0, 192},
    {0x873, 0x4c1, 0, 0, -1, 0, 2, 0, 0, 22, 22, 22, 22, 22, 22, 22, 22, 1, 0, 0, 0, 3, 3, 3, 3, 0},
    {0x873, 0x4c1, 0, 0, -1, 0, 3, 0, 1, 23, 23, 23, 23, 23, 23, 23, 23, 1, 0, 0, 0, 3, 3, 3, 3, -16},
    {0x4ae, 0x4be, 1, 1, -1, 0, 2, 0, 0, 24, 24, 24, 24, 24, 24, 24, 24, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0xc8, 0xce, 1, 1, -1, 0, 2, 0, 0, 25, 25, 25, 25, 25, 25, 25, 25, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0x4a6, 0x4c1, 1, 1, -1, 0, 2, 0, 0, 26, 26, 26, 26, 26, 26, 26, 26, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0xffff, 0x4c1, 1, 0, -1, 0, 2, 0, 0, 27, 27, 27, 27, 27, 27, 27, 27, 0, 0, 0, 0, 0, 0, 0, 0, 16},
    {0xffff, 0x4c1, 1, 0, -1, 0, 2, 0, 0, 28, 28, 28, 28, 28, 28, 28, 28, 0, 0, 0, 0, 0, 0, 0, 0, 8},
    {0xffff, 0xffff, 1, 1, 0, 1, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 1, 1, 1, 1, 0},
    {0xffff, 0xffff, 1, 1, 0, 0, 0, 0, 0, 9, 9, 9, 9, 9, 9, 9, 9, 0, 0, 0, 0, 5, 5, 5, 5, 0},
    {0xffff, 0xffff, 1, 1, 0, 1, 0, 0, 0, 9, 9, 3, 3, 9, 9, 3, 3, 0, 0, 0, 0, 5, 2, 2, 1, 0},
    {0xffff, 0xffff, 1, 1, 0, 1, 0, 0, 0, 3, 9, 9, 3, 3, 9, 9, 3, 0, 0, 0, 0, 2, 1, 5, 2, 0},
    {0xffff, 0xffff, 1, 1, 0, 1, 0, 0, 0, 3, 3, 9, 9, 3, 3, 9, 9, 0, 0, 0, 0, 1, 2, 2, 5, 0},
    {0xffff, 0xffff, 1, 1, 0, 1, 0, 0, 0, 9, 3, 3, 9, 9, 3, 3, 9, 0, 0, 0, 0, 2, 5, 1, 2, 0},
    {0xffff, 0xffff, 1, 1, 1, 1, 0, 0, 0, 9, 9, 3, 3, 9, 9, 3, 3, 0, 0, 0, 0, 5, 2, 2, 1, 0},
    {0xffff, 0xffff, 1, 1, 1, 1, 0, 0, 0, 3, 9, 9, 3, 3, 9, 9, 3, 0, 0, 0, 0, 2, 1, 5, 2, 0},
    {0xffff, 0xffff, 1, 1, 1, 1, 0, 0, 0, 3, 3, 9, 9, 3, 3, 9, 9, 0, 0, 0, 0, 1, 2, 2, 5, 0},
    {0xffff, 0xffff, 1, 1, 1, 1, 0, 0, 0, 9, 3, 3, 9, 9, 3, 3, 9, 0, 0, 0, 0, 2, 5, 1, 2, 0},
    {0xffff, 0xffff, 0, 0, -1, 0, 3, 0, 0, 12, 12, 9, 9, 12, 12, 9, 9, 0, 0, 0, 0, 3, 4, 4, 5, 0},
    {0xffff, 0xffff, 0, 0, -1, 0, 3, 0, 0, 9, 14, 14, 9, 9, 14, 14, 9, 0, 0, 0, 0, 4, 5, 3, 4, 0},
    {0xffff, 0xffff, 0, 0, -1, 0, 3, 0, 0, 9, 9, 16, 16, 9, 9, 16, 16, 0, 0, 0, 0, 5, 4, 4, 3, 0},
    {0xffff, 0xffff, 0, 0, -1, 0, 3, 0, 0, 18, 9, 9, 18, 18, 9, 9, 18, 0, 0, 0, 0, 4, 3, 5, 4, 0},
    {0xffff, 0xffff, 1, 1, -1, 0, 2, 0, 0, 20, 20, 20, 20, 11, 11, 11, 11, 0, 0, 0, 0, 3, 3, 3, 3, 0},
    {0xffff, 0xffff, 1, 1, -1, 0, 2, 0, 0, 20, 20, 20, 20, 13, 13, 13, 13, 0, 0, 0, 0, 3, 3, 3, 3, 0},
    {0xffff, 0xffff, 1, 1, -1, 0, 2, 0, 0, 20, 20, 20, 20, 15, 15, 15, 15, 0, 0, 0, 0, 3, 3, 3, 3, 0},
    {0xffff, 0xffff, 1, 1, -1, 0, 2, 0, 0, 20, 20, 20, 20, 17, 17, 17, 17, 0, 0, 0, 0, 3, 3, 3, 3, 0},
    {0xffff, 0xffff, 1, 1, -1, 0, 2, 0, 0, 20, 20, 20, 20, 12, 12, 12, 12, 0, 0, 0, 0, 3, 3, 3, 3, 0},
    {0xffff, 0xffff, 1, 1, -1, 0, 2, 0, 0, 20, 20, 20, 20, 14, 14, 14, 14, 0, 0, 0, 0, 3, 3, 3, 3, 0},
    {0xffff, 0xffff, 1, 1, -1, 0, 2, 0, 0, 20, 20, 20, 20, 16, 16, 16, 16, 0, 0, 0, 0, 3, 3, 3, 3, 0},
    {0xffff, 0xffff, 1, 1, -1, 0, 2, 0, 0, 20, 20, 20, 20, 18, 18, 18, 18, 0, 0, 0, 0, 3, 3, 3, 3, 0},
    {0xffff, 0xffff, 1, 0, -1, 0, 2, 0, 0, 20, 20, 9, 9, 12, 12, 9, 9, 0, 0, 0, 0, 3, 4, 4, 5, 0},
    {0xffff, 0xffff, 1, 0, -1, 0, 2, 0, 0, 9, 20, 20, 9, 9, 14, 14, 9, 0, 0, 0, 0, 4, 5, 3, 4, 0},
    {0xffff, 0xffff, 1, 0, -1, 0, 2, 0, 0, 9, 9, 20, 20, 9, 9, 16, 16, 0, 0, 0, 0, 5, 4, 4, 3, 0},
    {0xffff, 0xffff, 1, 0, -1, 0, 2, 0, 0, 20, 9, 9, 20, 18, 9, 9, 18, 0, 0, 0, 0, 4, 3, 5, 4, 0},
    {0xffff, 0xffff, 0, 0, -1, 0, 3, 0, 0, 20, 20, 12, 12, 12, 12, 12, 12, 0, 0, 0, 0, 3, 3, 3, 3, 0},
    {0xffff, 0xffff, 0, 0, -1, 0, 3, 0, 0, 14, 20, 20, 14, 14, 14, 14, 14, 0, 0, 0, 0, 3, 3, 3, 3, 0},
    {0xffff, 0xffff, 0, 0, -1, 0, 3, 0, 0, 16, 16, 20, 20, 16, 16, 16, 16, 0, 0, 0, 0, 3, 3, 3, 3, 0},
    {0xffff, 0xffff, 0, 0, -1, 0, 3, 0, 0, 20, 18, 18, 20, 18, 18, 18, 18, 0, 0, 0, 0, 3, 3, 3, 3, 0},
    {0xffff, 0xffff, 0, 0, -1, 0, 3, 0, 0, 20, 20, 16, 16, 16, 16, 16, 16, 0, 0, 0, 0, 3, 3, 3, 3, 0},
    {0xffff, 0xffff, 0, 0, -1, 0, 3, 0, 0, 18, 20, 20, 18, 18, 18, 18, 18, 0, 0, 0, 0, 3, 3, 3, 3, 0},
    {0xffff, 0xffff, 0, 0, -1, 0, 3, 0, 0, 12, 12, 20, 20, 12, 12, 12, 12, 0, 0, 0, 0, 3, 3, 3, 3, 0},
    {0xffff, 0xffff, 0, 0, -1, 0, 3, 0, 0, 20, 14, 14, 20, 14, 14, 14, 14, 0, 0, 0, 0, 3, 3, 3, 3, 0},
    {0xffff, 0xffff, 0, 0, -1, 0, 3, 0, 0, 20, 20, 11, 11, 11, 11, 11, 11, 0, 0, 0, 0, 3, 3, 3, 3, 0},
    {0xffff, 0xffff, 0, 0, -1, 0, 3, 0, 0, 15, 20, 20, 15, 15, 15, 15, 15, 0, 0, 0, 0, 3, 3, 3, 3, 0},
    {0xffff, 0xffff, 0, 0, -1, 0, 3, 0, 0, 15, 15, 20, 20, 15, 15, 15, 15, 0, 0, 0, 0, 3, 3, 3, 3, 0},
    {0xffff, 0xffff, 0, 0, -1, 0, 3, 0, 0, 20, 11, 11, 20, 11, 11, 11, 11, 0, 0, 0, 0, 3, 3, 3, 3, 0},
    {0xffff, 0xffff, 0, 0, -1, 0, 3, 0, 0, 20, 20, 15, 15, 15, 15, 15, 15, 0, 0, 0, 0, 3, 3, 3, 3, 0},
    {0xffff, 0xffff, 0, 0, -1, 0, 3, 0, 0, 11, 20, 20, 11, 11, 11, 11, 11, 0, 0, 0, 0, 3, 3, 3, 3, 0},
    {0xffff, 0xffff, 0, 0, -1, 0, 3, 0, 0, 11, 11, 20, 20, 11, 11, 11, 11, 0, 0, 0, 0, 3, 3, 3, 3, 0},
    {0xffff, 0xffff, 0, 0, -1, 0, 3, 0, 0, 20, 15, 15, 20, 15, 15, 15, 15, 0, 0, 0, 0, 3, 3, 3, 3, 0},
    {0xffff, 0xffff, 0, 0, -1, 0, 3, 0, 0, 20, 20, 13, 13, 13, 13, 13, 13, 0, 0, 0, 0, 3, 3, 3, 3, 0},
    {0xffff, 0xffff, 0, 0, -1, 0, 3, 0, 0, 13, 20, 20, 13, 13, 13, 13, 13, 0, 0, 0, 0, 3, 3, 3, 3, 0},
    {0xffff, 0xffff, 0, 0, -1, 0, 3, 0, 0, 17, 17, 20, 20, 17, 17, 17, 17, 0, 0, 0, 0, 3, 3, 3, 3, 0},
    {0xffff, 0xffff, 0, 0, -1, 0, 3, 0, 0, 20, 17, 17, 20, 17, 17, 17, 17, 0, 0, 0, 0, 3, 3, 3, 3, 0},
    {0xffff, 0xffff, 0, 0, -1, 0, 3, 0, 0, 20, 20, 17, 17, 17, 17, 17, 17, 0, 0, 0, 0, 3, 3, 3, 3, 0},
    {0xffff, 0xffff, 0, 0, -1, 0, 3, 0, 0, 17, 20, 20, 17, 17, 17, 17, 17, 0, 0, 0, 0, 3, 3, 3, 3, 0},
    {0xffff, 0xffff, 0, 0, -1, 0, 3, 0, 0, 13, 13, 20, 20, 13, 13, 13, 13, 0, 0, 0, 0, 3, 3, 3, 3, 0},
    {0xffff, 0xffff, 0, 0, -1, 0, 3, 0, 0, 20, 13, 13, 20, 13, 13, 13, 13, 0, 0, 0, 0, 3, 3, 3, 3, 0},
    {0xffff, 0xffff, 0, 0, -1, 0, 3, 0, 0, 9, 9, 8, 8, 9, 9, 8, 8, 0, 0, 0, 0, 5, 4, 4, 3, 0},
    {0xffff, 0xffff, 0, 0, -1, 0, 3, 0, 0, 9, 8, 8, 9, 9, 8, 8, 9, 0, 0, 0, 0, 4, 5, 3, 4, 0},
    {0xffff, 0xffff, 1, 0, -1, 0, 3, 0, 0, 19, 19, 22, 22, 19, 19, 22, 22, 0, 0, 0, 1, 3, 3, 3, 3, 0},
    {0xffff, 0xffff, 1, 0, -1, 0, 3, 0, 0, 19, 22, 22, 19, 19, 22, 22, 19, 0, 0, 0, 1, 3, 3, 3, 3, 0},
    {0xffff, 0xffff, 1, 0, -1, 0, 3, 16, 0, 22, 22, 22, 22, 22, 22, 22, 22, 0, 0, 0, 1, 3, 3, 3, 3, 0},
    {0xffff, 0xffff, 0, 0, -1, 0, 3, 32, 0, 22, 22, 23, 23, 22, 22, 23, 23, 1, 0, 0, 0, 3, 3, 3, 3, 0},
    {0xffff, 0xffff, 0, 0, -1, 0, 3, 8, 0, 22, 23, 23, 22, 22, 23, 23, 22, 1, 0, 0, 0, 3, 3, 3, 3, 0},
    {0xffff, 0xffff, 0, 0, -1, 0, 3, 0, 0, 23, 23, 8, 8, 23, 23, 8, 8, 1, 0, 0, 0, 3, 3, 3, 3, 0},
    {0xffff, 0xffff, 0, 0, -1, 0, 3, 0, 0, 23, 8, 8, 23, 23, 8, 8, 23, 1, 0, 0, 0, 3, 3, 3, 3, 0},
    {0xffff, 0xffff, 0, 0, -1, 0, 2, 0, 0, 10, 10, 3, 3, 10, 10, 3, 3, 0, 0, 0, 0, 1, 1, 1, 1, 0},
    {0xffff, 0xffff, 0, 0, -1, 0, 2, 0, 0, 3, 10, 10, 3, 3, 10, 10, 3, 0, 0, 0, 0, 1, 1, 1, 1, 0},
    {0xffff, 0xffff, 0, 0, -1, 0, 2, 0, 0, 3, 3, 10, 10, 3, 3, 10, 10, 0, 0, 0, 0, 1, 1, 1, 1, 0},
    {0xffff, 0xffff, 0, 0, -1, 0, 2, 0, 0, 10, 3, 3, 10, 10, 3, 3, 10, 0, 0, 0, 0, 1, 1, 1, 1, 0},
    {0xffff, 0xffff, 0, 0, -1, 0, 2, 0, 0, 10, 10, 9, 9, 10, 10, 3, 3, 0, 0, 0, 0, 5, 5, 5, 5, 0},
    {0xffff, 0xffff, 0, 0, -1, 0, 2, 0, 0, 9, 10, 10, 9, 9, 10, 10, 3, 0, 0, 0, 0, 5, 5, 5, 5, 0},
    {0xffff, 0xffff, 0, 0, -1, 0, 2, 0, 0, 9, 9, 10, 10, 9, 3, 10, 10, 0, 0, 0, 0, 5, 5, 5, 5, 0},
    {0xffff, 0xffff, 0, 0, -1, 0, 2, 0, 0, 10, 9, 9, 10, 10, 3, 3, 10, 0, 0, 0, 0, 5, 5, 5, 5, 0},
    {0xffff, 0xffff, 0, 0, -1, 0, 2, 0, 0, 10, 10, 9, 3, 10, 10, 9, 3, 0, 0, 0, 0, 2, 1, 5, 2, 0},
    {0xffff, 0xffff, 0, 0, -1, 0, 2, 0, 0, 10, 10, 3, 9, 10, 10, 3, 9, 0, 0, 0, 0, 2, 5, 1, 2, 0},
    {0xffff, 0xffff, 0, 0, -1, 0, 2, 0, 0, 9, 10, 10, 3, 9, 10, 10, 3, 0, 0, 0, 0, 5, 2, 2, 1, 0},
    {0xffff, 0xffff, 0, 0, -1, 0, 2, 0, 0, 3, 10, 10, 9, 3, 10, 10, 9, 0, 0, 0, 0, 1, 2, 2, 5, 0},
    {0xffff, 0xffff, 0, 0, -1, 0, 2, 0, 0, 9, 3, 10, 10, 9, 3, 10, 10, 0, 0, 0, 0, 2, 5, 1, 2, 0},
    {0xffff, 0xffff, 0, 0, -1, 0, 2, 0, 0, 3, 9, 10, 10, 3, 9, 10, 10, 0, 0, 0, 0, 2, 1, 5, 2, 0},
    {0xffff, 0xffff, 0, 0, -1, 0, 2, 0, 0, 10, 3, 9, 10, 10, 3, 9, 10, 0, 0, 0, 0, 1, 2, 2, 5, 0},
    {0xffff, 0xffff, 0, 0, -1, 0, 2, 0, 0, 10, 9, 3, 10, 10, 9, 3, 10, 0, 0, 0, 0, 5, 2, 2, 1, 0},
    {0xffff, 0xffff, 0, 0, -1, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0xffff, 0xffff, 0, 0, -1, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0xffff, 0xffff, 0, 0, -1, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0xffff, 0xffff, 0, 0, -1, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0xffff, 0xffff, 0, 0, -1, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0xffff, 0xffff, 0, 0, -1, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0xffff, 0xffff, 0, 0, -1, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0xffff, 0xffff, 0, 0, -1, 0, 3, 0, 1, 15, 15, 15, 15, 15, 15, 15, 15, 2, 0, 0, 0, 3, 3, 3, 3, 0},
    {0xffff, 0xffff, 0, 0, -1, 0, 3, 0, 1, 15, 15, 15, 15, 15, 15, 15, 15, 2, 0, 0, 0, 3, 3, 3, 3, 0},
    {0xffff, 0xffff, 1, 1, -1, 0, 2, 0, 0, 4, 4, 9, 9, 4, 4, 9, 9, 0, 0, 0, 0, 7, 6, 6, 5, 0},
    {0xffff, 0xffff, 1, 1, -1, 0, 2, 0, 0, 9, 4, 4, 9, 9, 4, 4, 9, 0, 0, 0, 0, 6, 7, 5, 6, 0},
    {0xffff, 0xffff, 1, 1, -1, 0, 2, 0, 0, 9, 9, 4, 4, 9, 9, 4, 4, 0, 0, 0, 0, 5, 6, 6, 7, 0},
    {0xffff, 0xffff, 1, 1, -1, 0, 2, 0, 0, 4, 9, 9, 4, 4, 9, 9, 4, 0, 0, 0, 0, 6, 5, 7, 6, 0},
    {0xffff, 0xffff, 0, 0, -1, 0, 2, 0, 0, 10, 10, 4, 4, 10, 10, 4, 4, 0, 0, 0, 0, 7, 7, 7, 7, 0},
    {0xffff, 0xffff, 0, 0, -1, 0, 2, 0, 0, 4, 10, 10, 4, 4, 10, 10, 4, 0, 0, 0, 0, 7, 7, 7, 7, 0},
    {0xffff, 0xffff, 0, 0, -1, 0, 2, 0, 0, 4, 4, 10, 10, 4, 4, 10, 10, 0, 0, 0, 0, 7, 7, 7, 7, 0},
    {0xffff, 0xffff, 0, 0, -1, 0, 2, 0, 0, 10, 4, 4, 10, 10, 4, 4, 10, 0, 0, 0, 0, 7, 7, 7, 7, 0},
    {0x4aa, 0x4c3, 1, 1, -1, 0, 2, 0, 0, 121, 121, 121, 121, 121, 121, 121, 121, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0x4b6, 0x4bf, 1, 1, -1, 0, 2, 0, 0, 122, 122, 122, 122, 122, 122, 122, 122, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0x4b2, 0x4c2, 1, 1, -1, 0, 2, 0, 0, 123, 123, 123, 123, 123, 123, 123, 123, 0, 0, 0, 0, 0, 0, 0, 0, 0},
};
s32 data_021bf9a0 = func_01ffcb0c(0x20000, 0x800);
FxVec3 sCollisionUpVector(0, 0x1000, 0);
BoxCollider *sBoxColliderList;
FxVec3 sCollisionQueryMargin(0x1000, 0x1000, 0x1000);
CollisionMap *gCurCollisionMap = &sCollisionWorld.maps[0];
const GroundAttrGetter sGroundQuadAttr1Getters[4] = {GroundAttr_GetQuadAttr1Q0, GroundAttr_GetQuadAttr1Q1, GroundAttr_GetQuadAttr1Q2, GroundAttr_GetQuadAttr1Q3};
const u32 sDigHoleRadii[3] = {0x800, 0xc00, 0x1000};
const s32 data_020c7c1c = -0x1000;
const u8 sDefaultGroundColors[4] = {0, 0, 0, 0};

void CollisionMap::reset()
{
    s32 i, j;
    numBlocksX = 0;
    numBlocksZ = 0;
    shapeQuery = 0;
    prevWaveLevel = 0;
    waveLevel = 0;
    lockedExit = -1;
    allBlocksSet = 0;
    for (i = 0; i < 6; i++) {
        for (j = 0; j < 6; j++) {
            blocks[i][j].attrs = 0;
            blocks[i][j].walkLinks = 0;
        }
    }
}

void DigHoleColliders::clear()
{
    u8 *p;
    activeMask = 0;
    for (p = ages; p < (u8 *)unitXs; p++) {
        *p = 0xff;
    }
}

BOOL DigHoleColliders::addHole(s32 a, s32 b)
{
    s32 i = findFreeSlot();
    if (i != -1) {
        activeMask |= 1 << i;
        unitXs[i] = a;
        unitZs[i] = b;
        ages[i] = 0;
        return TRUE;
    }
    return FALSE;
}

s32 DigHoleColliders::findFreeSlot()
{
    u32 i;
    for (i = 0; i < 4; i++) {
        if (((activeMask >> i) & 1) == 0) {
            return i;
        }
    }
    return -1;
}

void DigHoleColliders::update()
{
    s32 i;
    u8 *p;
    for (i = 0, p = ages; p < (u8 *)unitXs; i++, p++) {
        if (((activeMask >> i) & 1) != 0 && *p < 3) {
            (*p)++;
        } else {
            *p = 0xff;
            activeMask &= ~(1 << i);
        }
    }
}

u32 DigHoleColliders::getRadiusAt(s32 a, s32 b)
{
    u32 i = 0;
    s8 *p5 = unitXs;
    s8 *p9 = unitZs;
    u8 *p1 = ages;
    for (; p1 < (u8 *)unitXs; p5++, p9++, i++, p1++) {
        if (((activeMask >> i) & 1) != 0 && *p1 < 3 && a == *p5 && b == *p9) {
            return sDigHoleRadii[*p1];
        }
    }
    return 0;
}

GroundCell *GroundCell::loadAtPos(VecFx32 *p, s32 flag)
{
    load(p->x >> 13, p->z >> 13, flag);
    return this;
}

void GroundCell::load(s32 x, s32 y, s32 flag)
{
    if (flag == 0) {
        unitAttr = Ground_GetUnitAttr(x, y);
        s32 a = unitAttr;
        quadAttr0 = a < 0x7c ? sGroundAttrTable[a].quadAttr0Q0 : 0;
        s32 b0 = quadAttr0;
        quadHeight0 = b0 < 0x7c ? sGroundAttrTable[b0].height << 8 : 0;
        quadAttr1 = a < 0x7c ? sGroundAttrTable[a].quadAttr0Q1 : 0;
        s32 b1 = quadAttr1;
        quadHeight1 = b1 < 0x7c ? sGroundAttrTable[b1].height << 8 : 0;
        quadAttr2 = a < 0x7c ? sGroundAttrTable[a].quadAttr0Q2 : 0;
        s32 b2 = quadAttr2;
        quadHeight2 = b2 < 0x7c ? sGroundAttrTable[b2].height << 8 : 0;
        quadAttr3 = a < 0x7c ? sGroundAttrTable[a].quadAttr0Q3 : 0;
        s32 b3 = quadAttr3;
        quadHeight3 = b3 < 0x7c ? sGroundAttrTable[b3].height << 8 : 0;
    } else {
        unitAttr = Ground_GetUnitAttr(x, y);
        s32 a = unitAttr;
        quadAttr0 = a < 0x7c ? sGroundAttrTable[a].quadAttr1Q0 : 0;
        s32 b0 = quadAttr0;
        quadHeight0 = b0 < 0x7c ? sGroundAttrTable[b0].height << 8 : 0;
        quadAttr1 = a < 0x7c ? sGroundAttrTable[a].quadAttr1Q1 : 0;
        s32 b1 = quadAttr1;
        quadHeight1 = b1 < 0x7c ? sGroundAttrTable[b1].height << 8 : 0;
        quadAttr2 = a < 0x7c ? sGroundAttrTable[a].quadAttr1Q2 : 0;
        s32 b2 = quadAttr2;
        quadHeight2 = b2 < 0x7c ? sGroundAttrTable[b2].height << 8 : 0;
        quadAttr3 = a < 0x7c ? sGroundAttrTable[a].quadAttr1Q3 : 0;
        s32 b3 = quadAttr3;
        quadHeight3 = b3 < 0x7c ? sGroundAttrTable[b3].height << 8 : 0;
    }
}

GroundCellGrid::GroundCellGrid()
{
    minX = maxX = minZ = maxZ = 0;
}

GroundCell::GroundCell()
{
}

void GroundCellGrid::loadArea(s32 x0, s32 x1, s32 z0, s32 z1, s32 flag)
{
    s32 z, x;
    s32 dz = z1 - z0 + 1;
    s32 dx = x1 - x0 + 1;
    if (dx <= 7 && dz <= 7) {
        minX = x0;
        maxX = x1;
        minZ = z0;
        maxZ = z1;
        for (z = minZ; z <= maxZ; z++) {
            for (x = minX; x <= maxX; x++) {
                ((GroundCell *)((u8 *)this + (z - minZ) * 0xa8 + (x - minX) * 0x18))->load(x, z, flag);
            }
        }
    } else {
        minX = x0;
        maxX = x0 + 6;
        minZ = z0;
        maxZ = z0 + 6;
        for (z = minZ; z <= maxZ; z++) {
            for (x = minX; x <= maxX; x++) {
                ((GroundCell *)((u8 *)this + (z - minZ) * 0xa8 + (x - minX) * 0x18))->load(x, z, flag);
            }
        }
    }
}

GroundCell *GroundCellGrid::getCell(s32 x, s32 z)
{
    if (x >= minX && x <= maxX && z >= minZ && z <= maxZ) {
        return (GroundCell *)((u8 *)this + (z - minZ) * 0xa8 + (x - minX) * 0x18);
    }
    return NULL;
}

BOOL CollisionMapIndex::setIndex(s32 v)
{
    if (v >= 0 && v < 8) {
        curMapIndex = v;
        return TRUE;
    }
    return FALSE;
}

CollisionTagX::CollisionTagX()
{
    attr = 0;
    f_04 = 0;
}

CollisionTagX::~CollisionTagX()
{
}

void CollisionTagX::setTag(u32 a, BoxCollider *owner)
{
    attr = a;
    f_04 = owner;
}

void CollisionTagX::copyTag(const CollisionTagX &o)
{
    attr = o.attr;
    f_04 = o.f_04;
}

GroundInfo::GroundInfo(VecFx32 *v, s32 a, s32 b)
{
    ((GroundInfoCalc *)(this))->compute((Unk_0202f2ac_V3 *)(v), a, b);
}

GroundInfo *GroundInfo::initAtPos(VecFx32 *v, s32 a, s32 b)
{
    ((GroundInfoCalc *)(this))->compute((Unk_0202f2ac_V3 *)(v), a, b);
    return this;
}

GroundInfo *GroundInfo::initAtUnit(s32 x, s32 z, s32 a, s32 b)
{
    VecFx32 v;
    v.x = (x << 13) + 0x1000;
    v.y = 0;
    v.z = (z << 13) + 0x1000;
    ((GroundInfoCalc *)(this))->compute((Unk_0202f2ac_V3 *)(&v), a, b);
    return this;
}

GroundInfo::~GroundInfo()
{
}

extern "C" void GroundInfo_Destruct(void *obj)
{
}

s32 GroundInfoBase::getHeight(s32 flag)
{
    if (flag == 0) {
        return height;
    }
    s32 a, b, c;
    VecFx32 v;
    if (Collision_GetUnitShape(unitX, unitZ, &a, &b, &c) && c != 2) {
        s32 z = unitZ;
        v.x = (unitX << 13) + 0x1000;
        v.y = 0;
        v.z = (z << 13) + 0x1000;
        s32 r7 = a;
        long long d = Vec_DistSqXZ(&v, queryPos);
        s32 m = func_01ffcb0c(r7, r7);
        if ((long long)m >= d) {
            return height + b;
        }
    }
    return height;
}

s32 GroundInfoBase::getWaterSurfaceY()
{
    if (attr == 0x16) {
        s32 t = FX_Div(0x1e000, 0x64000);
        return func_01ffcb0c(waterSurfaceY, t);
    }
    return waterSurfaceY;
}

BOOL GroundInfoBase::isBelowWaterSurface(s32 x)
{
    if (waterKind != 0) {
        if (x <= waterSurfaceY) {
            return TRUE;
        }
        return FALSE;
    }
    return FALSE;
}

void GroundInfoBase::setWaveDir(s32 a, s32 b, s32 c)
{
    b -= a;
    s32 v = 0;
    flowDir = v;
    flowDirY = v;
    flowDirZ = 0x1000;
    if (b <= 0) {
        v = 0x8000;
    }
    Vec_RotateY(&flowDir, (s16)(c + (s16)v));
}

void GroundInfoCalc::compute(Unk_0202f2ac_V3 *pos, s32 flag, s32 arg) {
    CollisionMap *g = gCurCollisionMap;
    s32 gc = g->prevWaveLevel;
    s32 sl = g->waveLevel;
    volatile Unk_0202f2ac_V3 ctr;
    s32 cx, cz;
    s32 r;
    onShore = 0;
    queryPos.x = pos->x;
    queryPos.y = pos->y;
    queryPos.z = pos->z;
    unitX = pos->x >> 13;
    unitZ = pos->z >> 13;
    r = Ground_GetUnitAttr(unitX, unitZ);
    s32 k = Ground_GetUnitQuadrant((VecFx32 *)(pos));
    if (flag != 0) {
        attr = Ground_GetQuadAttr1(unitX, unitZ, k);
    } else {
        attr = Ground_GetQuadAttr0(unitX, unitZ, k);
    }
    height = attr < 0x7c ? (sGroundAttrTable[attr].height << 8) : 0;
    waterSurfaceY = 0xfffee000;
    waterKind = attr < 0x7c ? sGroundAttrTable[attr].waterKind : 0;
    flowDir.x = 0;
    flowDir.y = 0;
    flowDir.z = 0x1000;
    if (GroundAttr_IsWater(attr, arg)) {
        if (r == 0x52) {
            cx = (pos->x & 0xffffe000) + 0x1000;
            ctr.x = cx;
            ctr.y = 0;
            cz = (pos->z & 0xffffe000) + 0x1000;
            ctr.z = cz;
            Unk_0202f2ac_V3 a(cx - 0x1000, 0, cz + 0x1000);
            Unk_0202f2ac_V3 b(a.x, a.y, a.z - func_01ffcb0c(sl, 0x1666));
            Unk_0202f2ac_V3 c(a.x + func_01ffcb0c(sl, 0x1666), a.y, a.z);
            CollisionTriangleX tri((VecFx32 *)&b, (VecFx32 *)&a, (VecFx32 *)&c, (VecFx32 *)&sCollisionUpVector);
            if (tri.containsXZ(pos)) {
                attr = 0x16;
                ((GroundInfoBase *)(this))->setWaveDir(gc, sl, 0x6000);
            } else {
                waterKind = 0;
                attr = 0x13;
            }
            onShore = 1;
            Unk_02033438_Set(&shorelinePoint, (b.x + c.x) >> 1, (b.y + c.y) >> 1, (b.z + c.z) >> 1);
        } else if (r == 0x55) {
            cx = (pos->x & 0xffffe000) + 0x1000;
            ctr.x = cx;
            ctr.y = 0;
            cz = (pos->z & 0xffffe000) + 0x1000;
            ctr.z = cz;
            Unk_0202f2ac_V3 a(cx + 0xb33, 0, cz - 0xb33);
            Unk_0202f2ac_V3 b(a.x, a.y, a.z + func_01ffcb0c(0x1000 - sl, 0x1666));
            Unk_0202f2ac_V3 c(a.x - func_01ffcb0c(0x1000 - sl, 0x1666), a.y, a.z);
            CollisionTriangleX tri((VecFx32 *)&a, (VecFx32 *)&c, (VecFx32 *)&b, (VecFx32 *)&sCollisionUpVector);
            if (!tri.containsXZ(pos)) {
                attr = 0x16;
                ((GroundInfoBase *)(this))->setWaveDir(gc, sl, 0x6000);
            } else {
                waterKind = 0;
                attr = 0x13;
            }
            onShore = 1;
            Unk_02033438_Set(&shorelinePoint, (b.x + c.x) >> 1, (b.y + c.y) >> 1, (b.z + c.z) >> 1);
        } else if (r == 0x51) {
            cx = (pos->x & 0xffffe000) + 0x1000;
            ctr.x = cx;
            ctr.y = 0;
            cz = (pos->z & 0xffffe000) + 0x1000;
            ctr.z = cz;
            Unk_0202f2ac_V3 a(cx + 0x1000, 0, cz + 0x1000);
            Unk_0202f2ac_V3 b(a.x, a.y, a.z - func_01ffcb0c(sl, 0x1666));
            Unk_0202f2ac_V3 c(a.x - func_01ffcb0c(sl, 0x1666), a.y, a.z);
            CollisionTriangleX tri((VecFx32 *)&b, (VecFx32 *)&c, (VecFx32 *)&a, (VecFx32 *)&sCollisionUpVector);
            if (tri.containsXZ(pos)) {
                attr = 0x16;
                ((GroundInfoBase *)(this))->setWaveDir(gc, sl, 0xffffa000);
            } else {
                waterKind = 0;
                attr = 0x13;
            }
            onShore = 1;
            Unk_02033438_Set(&shorelinePoint, (b.x + c.x) >> 1, (b.y + c.y) >> 1, (b.z + c.z) >> 1);
        } else if (r == 0x54) {
            cx = (pos->x & 0xffffe000) + 0x1000;
            ctr.x = cx;
            ctr.y = 0;
            cz = (pos->z & 0xffffe000) + 0x1000;
            ctr.z = cz;
            Unk_0202f2ac_V3 a(cx - 0xb33, 0, cz - 0xb33);
            Unk_0202f2ac_V3 b(a.x, a.y, a.z + func_01ffcb0c(0x1000 - sl, 0x1666));
            Unk_0202f2ac_V3 c(a.x + func_01ffcb0c(0x1000 - sl, 0x1666), a.y, a.z);
            CollisionTriangleX tri((VecFx32 *)&a, (VecFx32 *)&b, (VecFx32 *)&c, (VecFx32 *)&sCollisionUpVector);
            if (!tri.containsXZ(pos)) {
                attr = 0x16;
                ((GroundInfoBase *)(this))->setWaveDir(gc, sl, 0xffffa000);
            } else {
                waterKind = 0;
                attr = 0x13;
            }
            onShore = 1;
            Unk_02033438_Set(&shorelinePoint, (b.x + c.x) >> 1, (b.y + c.y) >> 1, (b.z + c.z) >> 1);
        } else if (r == 0x53) {
            cx = (pos->x & 0xffffe000) + 0x1000;
            ctr.x = cx;
            ctr.y = 0;
            cz = (pos->z & 0xffffe000) + 0x1000;
            ctr.z = cz;
            s32 e = cz + 0x1000;
            e = e - func_01ffcb0c(sl, 0x1666);
            if (e > pos->z) {
                waterKind = 0;
                attr = 0x13;
            } else {
                attr = 0x16;
                ((GroundInfoBase *)(this))->setWaveDir(gc, sl, 0xffff8000);
            }
            onShore = 1;
            shorelinePoint.x = ctr.x;
            shorelinePoint.y = 0;
            shorelinePoint.z = e;
        } else {
            u32 t = attr < 0x7c ? sGroundAttrTable[attr].flowDirBits : 0;
            if (t != 0) {
                s32 ang = 0;
                s32 i;
                for (i = 0; i < 8; i++) {
                    if (t & (1 << i)) {
                        ang = (i << 29) >> 16;
                        break;
                    }
                }
                Vec_RotateY(&flowDir, ang);
            }
        }
        if (waterKind != 0) {
            waterSurfaceY = 0xfffff000;
        }
    } else {
        waterKind = 0;
    }
}

FloorTriangle::FloorTriangle() {}

FloorTriangle::~FloorTriangle() {}

BOOL FloorTriangle::setupFloor(Unk_0202f2ac_V3 *a, Unk_0202f2ac_V3 *b, Unk_0202f2ac_V3 *c, Unk_0202f2ac_V3 *d, u32 e, BoxCollider *f) {
    setTag(e, f);
    set(a, b, c, d);
    return TRUE;
}

FloorTriangleList::FloorTriangleList() : numTriangles(0) {}

FloorTriangleList::~FloorTriangleList() {}

void FloorTriangleList::buildFromCells(GroundCellGrid *grid, s32 x0, s32 x1, s32 y0, s32 y1) {
    s32 y, x;
    for (y = y0; y <= y1; y++) {
        for (x = x0; x <= x1; x++) {
            GroundCell *cell = ((GroundCellGrid *)(grid))->getCell(x, y);
            if (cell == NULL) {
                continue;
            }
            s32 xc = (x << 13) + 0x1000;
            s32 yc = (y << 13) + 0x1000;
            Unk_0202f2ac_V3 ctr;
            Unk_020331a8_SetU((u32 *)&ctr, xc, 0, yc);
            s32 xr = xc + 0x1000;
            s32 xl = xc - 0x1000;
            s32 zr = yc + 0x1000;
            s32 zl = yc - 0x1000;
            u32 t = (s32)cell->unitAttr < 0x7c ? sGroundAttrTable[cell->unitAttr].flatFloor : 0;
            if (t != 0) {
                Unk_0202f2ac_V3 a(xl, cell->quadHeight0, zl);
                Unk_0202f2ac_V3 b(xl, cell->quadHeight0, zr);
                Unk_0202f2ac_V3 c(xr, cell->quadHeight0, zr);
                Unk_0202f2ac_V3 d(xr, cell->quadHeight0, zl);
                addTriangle(&a, &b, &c, (Unk_0202f2ac_V3 *)&sCollisionUpVector, cell->quadAttr0, 0);
                addTriangle(&a, &c, &d, (Unk_0202f2ac_V3 *)&sCollisionUpVector, cell->quadAttr0, 0);
            } else {
                Unk_0202f2ac_V3 a0(xc, cell->quadHeight0, yc);
                Unk_0202f2ac_V3 a1(xr, cell->quadHeight0, zl);
                Unk_0202f2ac_V3 a2(xl, cell->quadHeight0, zl);
                addTriangle(&a0, &a1, &a2, (Unk_0202f2ac_V3 *)&sCollisionUpVector, cell->quadAttr0, 0);
                Unk_0202f2ac_V3 b0(ctr.x, cell->quadHeight1, ctr.z);
                Unk_0202f2ac_V3 b1(xl, cell->quadHeight1, zl);
                Unk_0202f2ac_V3 b2(xl, cell->quadHeight1, zr);
                addTriangle(&b0, &b1, &b2, (Unk_0202f2ac_V3 *)&sCollisionUpVector, cell->quadAttr1, 0);
                Unk_0202f2ac_V3 c0(ctr.x, cell->quadHeight2, ctr.z);
                Unk_0202f2ac_V3 c1(xl, cell->quadHeight2, zr);
                Unk_0202f2ac_V3 c2(xr, cell->quadHeight2, zr);
                addTriangle(&c0, &c1, &c2, (Unk_0202f2ac_V3 *)&sCollisionUpVector, cell->quadAttr2, 0);
                Unk_0202f2ac_V3 d0(ctr.x, cell->quadHeight3, ctr.z);
                Unk_0202f2ac_V3 d1(xr, cell->quadHeight3, zr);
                Unk_0202f2ac_V3 d2(xr, cell->quadHeight3, zl);
                addTriangle(&d0, &d1, &d2, (Unk_0202f2ac_V3 *)&sCollisionUpVector, cell->quadAttr3, 0);
            }
        }
    }
}

BOOL FloorTriangleList::addTriangle(Unk_0202f2ac_V3 *a, Unk_0202f2ac_V3 *b, Unk_0202f2ac_V3 *c, Unk_0202f2ac_V3 *d, u32 e, BoxCollider *f) {
    if (numTriangles < 40) {
        u32 i = numTriangles;
        numTriangles = i + 1;
        return triangles[i].setupFloor(a, b, c, d, e, f);
    }
    return FALSE;
}

WallEdge::WallEdge() {}

WallEdge::~WallEdge() {}

WallEdge::WallEdge(CollisionVec2 *a, CollisionVec2 *b, CollisionVec2 *c, s32 p4, s32 p5, u32 p6, BoxCollider *p7) {
    setupEdge(a, b, c, p4, p5, p6, p7);
}

extern "C" void WallEdge_GetMidpoint(CollisionVec2 *out, WallEdge *e) {
    out->set((e->start.x + e->end.x) >> 1, (e->start.y + e->end.y) >> 1);
}

BOOL WallEdge::copyEdge(WallEdge *o) {
    copyTag(*o);
    set(&o->start, &o->end, &o->normal);
    kind = o->kind;
    topY = o->topY;
    return TRUE;
}

BOOL WallEdge::setupEdge(CollisionVec2 *a, CollisionVec2 *b, CollisionVec2 *c, s32 p4, s32 p5, u32 p6, BoxCollider *p7) {
    setTag(p6, p7);
    set(a, b, c);
    kind = p5;
    topY = p4;
    return TRUE;
}

WallEdgeList::WallEdgeList() : numEdges(0) {}

WallEdgeList::~WallEdgeList() {}

BOOL WallEdgeList::collide(VecFx32 *pos, VecFx32 *q, s32 r, CollisionState *out, Actor *actor) {
    WallEdge *e;
    s32 py;
    BOOL result = FALSE;
    CollisionVec2 a, b, d;
    a.set(pos->x, pos->z);
    b.set(q->x, q->z);
    d.set(pos->x - q->x, pos->z - q->z);
    s64 dist = d.distSq((CollisionVec2 *)gCollisionVec2Zero);
    if (dist >= (s64)func_01ffcb0c(r, r)) {
    for (e = edges; e < edges + numEdges; e++) {
        volatile Unk_0202f2ac_V3 t1a;
        t1a.x = pos->x;
        t1a.y = pos->y;
        t1a.z = pos->z;
        if (e->topY - 0x700 > pos->y) {
            if (e->pushBackCrossing(&a, &b, r)) {
                volatile Unk_0202f2ac_V3 t1b;
                py = *(volatile s32 *)&pos->y;
                t1b.x = a.x;
                t1b.y = py;
                t1b.z = a.y;
                s32 k = e->kind;
                if (k != 3) {
                    out->contacts.addContact(Math_Atan2(e->normal.x, e->normal.y), k, e->attr);
                    if (e->f_04) {
                        e->f_04->onEdgeContact(e, actor, r);
                    }
                }
                result = TRUE;
            }
        }
    }
    }
    for (e = edges; e < edges + numEdges; e++) {
        volatile Unk_0202f2ac_V3 t2a;
        t2a.x = pos->x;
        t2a.y = pos->y;
        t2a.z = pos->z;
        if (e->topY - 0x700 > pos->y) {
            if (e->pushOutFace(&a, &b, r)) {
                volatile Unk_0202f2ac_V3 t2b;
                py = *(volatile s32 *)&pos->y;
                t2b.x = a.x;
                t2b.y = py;
                t2b.z = a.y;
                s32 k = e->kind;
                if (k != 3) {
                    out->contacts.addContact(Math_Atan2(e->normal.x, e->normal.y), k, e->attr);
                    if (e->f_04) {
                        e->f_04->onEdgeContact(e, actor, r);
                    }
                }
                result = TRUE;
            }
        }
    }
    for (e = edges; e < edges + numEdges; e++) {
        volatile Unk_0202f2ac_V3 t3a;
        t3a.x = pos->x;
        t3a.y = pos->y;
        t3a.z = pos->z;
        if (e->topY - 0x700 > pos->y) {
            if (e->pushOutEnds(&a, &b, r)) {
                volatile Unk_0202f2ac_V3 t3b;
                py = *(volatile s32 *)&pos->y;
                t3b.x = a.x;
                t3b.y = py;
                t3b.z = a.y;
                s32 k = e->kind;
                if (k != 3) {
                    out->contacts.addContact(Math_Atan2(e->normal.x, e->normal.y), k, e->attr);
                    if (e->f_04) {
                        e->f_04->onEdgeContact(e, actor, r);
                    }
                }
                result = TRUE;
            }
        }
    }
    pos->x = a.x;
    pos->z = a.y;
    return result;
}

BOOL WallEdgeListWriter::addEdgeCopy(WallEdge *e) {
    if (numEdges < 0x18) {
        u32 n = numEdges;
        numEdges = n + 1;
        edges[n].copyEdge(e);
        return TRUE;
    }
    return FALSE;
}

BOOL WallEdgeListWriter::addEdge(void *a, void *b, void *c, s32 d0, s32 d1, s32 d2, BoxCollider *d3) {
    WallEdge t((CollisionVec2 *)a, (CollisionVec2 *)b, (CollisionVec2 *)c, d0, d1, d2, d3);
    BOOL r = addEdgeCopy(&t);
    return r;
}

void WallEdgeListWriter::buildFromCells(void *grid, s32 x0, s32 x1, s32 z0, s32 z1, s32 flag) {
    s32 z, x;
    GroundCell *qx, *qz;
    s32 zw;
    VecXZ t30, t38, t40, t48, t50, t58, t60, t68;
    volatile VecFx32 p;
    for (z = z1; z >= z0; z--) {
        x = x1;
        zw = (z << 13) + 0x1000;
        for (; x >= x0; x--) {
            GroundCell *q = ((GroundCellGrid *)(grid))->getCell(x, z);
            if (q == 0) continue;
            qx = ((GroundCellGrid *)(grid))->getCell(x + 1, z);
            qz = ((GroundCellGrid *)(grid))->getCell(x, z + 1);
            p.x = (x << 13) + 0x1000;
            p.y = 0;
            p.z = zw;
            if (qx != 0 && q->quadHeight3 != qx->quadHeight1) {
                static CellWallNormal sA(0x1000, 0);
                static CellWallNormal sB(-0x1000, 0);
                ((CollisionVec2 *)(&t30))->set(p.x + 0x1000, p.z - 0x1000);
                ((CollisionVec2 *)(&t38))->set(t30.x, p.z + 0x1000);
                if (q->quadHeight3 > qx->quadHeight1) {
                    addEdge(&t30, &t38, &sA, q->quadHeight3, 1, q->quadAttr3, 0);
                    if (flag) addEdge(&t30, &t38, &sB, 0x8000, 2, 2, 0);
                } else {
                    addEdge(&t30, &t38, &sB, qx->quadHeight0, 1, qx->quadAttr0, 0);
                    if (flag) addEdge(&t30, &t38, &sA, 0x8000, 2, 2, 0);
                }
            }
            if (qz != 0 && q->quadHeight2 != qz->quadHeight0) {
                static CellWallNormal sC(0, 0x1000);
                static CellWallNormal sD(0, -0x1000);
                ((CollisionVec2 *)(&t40))->set(p.x - 0x1000, p.z + 0x1000);
                ((CollisionVec2 *)(&t48))->set(p.x + 0x1000, t40.z);
                if (q->quadHeight2 > qz->quadHeight0) {
                    addEdge(&t40, &t48, &sC, q->quadHeight2, 1, q->quadAttr2, 0);
                    if (flag) addEdge(&t40, &t48, &sD, 0x8000, 2, 2, 0);
                } else {
                    addEdge(&t40, &t48, &sD, qz->quadHeight0, 1, qz->quadAttr0, 0);
                    if (flag) addEdge(&t40, &t48, &sC, 0x8000, 2, 2, 0);
                }
            }
            if (q->quadHeight0 != q->quadHeight1) {
                static CellWallNormal sE(-0xb50, 0xb50);
                static CellWallNormal sF(-sE.x, -sE.z);
                ((CollisionVec2 *)(&t50))->set(p.x - 0x1000, p.z - 0x1000);
                ((CollisionVec2 *)(&t58))->set(p.x + 0x1000, p.z + 0x1000);
                if (q->quadHeight0 > q->quadHeight1) {
                    addEdge(&t50, &t58, &sE, q->quadHeight0, 1, q->quadAttr0, 0);
                    if (flag) addEdge(&t50, &t58, &sF, 0x8000, 2, 2, 0);
                } else {
                    addEdge(&t50, &t58, &sF, q->quadHeight1, 1, q->quadAttr1, 0);
                    if (flag) addEdge(&t50, &t58, &sE, 0x8000, 2, 2, 0);
                }
            } else if (q->quadHeight0 != q->quadHeight3) {
                static CellWallNormal sG(0xb50, 0xb50);
                static CellWallNormal sH(-sG.x, -sG.z);
                ((CollisionVec2 *)(&t60))->set(p.x - 0x1000, p.z + 0x1000);
                ((CollisionVec2 *)(&t68))->set(p.x + 0x1000, p.z - 0x1000);
                if (q->quadHeight0 > q->quadHeight3) {
                    addEdge(&t60, &t68, &sG, q->quadHeight0, 1, q->quadAttr0, 0);
                    if (flag) addEdge(&t60, &t68, &sH, 0x8000, 2, 2, 0);
                } else {
                    addEdge(&t60, &t68, &sH, q->quadHeight3, 1, q->quadAttr3, 0);
                    if (flag) addEdge(&t60, &t68, &sG, 0x8000, 2, 2, 0);
                }
            }
        }
    }
}

const GroundAttrGetter sGroundQuadAttr0Getters[4] = {GroundAttr_GetQuadAttr0Q0, GroundAttr_GetQuadAttr0Q1, GroundAttr_GetQuadAttr0Q2, GroundAttr_GetQuadAttr0Q3};
CollisionWorld sCollisionWorld;

ShapeCylinder::ShapeCylinder() {
}

ShapeCylinder::~ShapeCylinder() {
}

void ShapeCylinder::setupShape(VecFx32 *pos, s32 b, s32 c, s32 a4, s32 a5) {
    s32 z = pos->z >> 13;
    s32 x = pos->x >> 13;
    unitX = x;
    unitZ = z;
    setCylinder((VecFx32 *)pos, b, c);
    kind = a4;
    setTag(a5, 0);
}

BOOL ShapeCylinder_AddLinkWall(ShapeCylinder *a, WallEdgeListWriter *out, ShapeCylinder *b) {
    s32 dx = b->unitX - a->unitX;
    s32 dz = b->unitZ - a->unitZ;
    if ((dx == 0 && dz == 1) || (dx == 1 && (u32)(dz + 1) <= 2)) {
        VecXZ p10, p18, p20, p28, p30, p38, p40;
        s32 r;
        ((CollisionVec2 *)(&p10))->set(a->center.x, a->center.z);
        ((CollisionVec2 *)(&p18))->set(b->center.x, b->center.z);
        ((CollisionVec2 *)(void *)(&p20))->setDiff((CollisionVec2 *)(void *)(&p18), (CollisionVec2 *)(void *)(&p10));
        ((CollisionVec2 *)(void *)(&p20))->normalize();
        r = p10.z - func_01ffcb0c(p20.z, a->circleRadius);
        s32 x = p10.x - func_01ffcb0c(p20.x, a->circleRadius);
        ((CollisionVec2 *)(&p28))->set(x, r);
        r = p18.z + func_01ffcb0c(p20.z, b->circleRadius);
        x = p18.x + func_01ffcb0c(p20.x, b->circleRadius);
        ((CollisionVec2 *)(&p30))->set(x, r);
        ((CollisionVec2 *)(&p38))->set(0, 0);
        ((CollisionVec2 *)(void *)(&p38))->setEdgeNormal((CollisionVec2 *)(void *)(&p10), (CollisionVec2 *)(void *)(&p18));
        ((CollisionVec2 *)(&p40))->set(-p38.x, -p38.z);
        s32 m = b->cylinderHeight;
        if (m > a->cylinderHeight) m = a->cylinderHeight;
        out->addEdge(&p28, &p30, &p38, m, 3, 0, 0);
        out->addEdge(&p28, &p30, &p40, m, 3, 0, 0);
        return TRUE;
    }
    return FALSE;
}

ShapeCylinderList::ShapeCylinderList() {
    numCylinders = 0;
}

ShapeCylinderList::~ShapeCylinderList() {
}

BOOL ShapeCylinderList::pushOut(VecFx32 *pos, s32 x, CollisionState *state) {
    BOOL r = FALSE;
    ShapeCylinder *base = cylinders;
    ShapeCylinder *e;
    for (e = base; e < base + numCylinders; e++) {
        volatile VecFx32 d;
        d.x = pos->x;
        d.y = pos->y;
        d.z = pos->z;
        if (e->pushOut((VecFx32 *)pos, x)) {
            VecFx32 t;
            Vec_Sub(&t, pos, (VecFx32 *)e);
            s32 ang = Math_Atan2(t.x, t.z);
            state->contacts.addContact(ang, e->kind, e->CollisionTagX::attr);
            r = TRUE;
        }
    }
    return r;
}

BOOL ShapeCylinderList::landOnTop(VecFx32 *pos, VecFx32 *prev, u32 *out) {
    ShapeCylinder *base = cylinders;
    ShapeCylinder *e;
    for (e = base; e < base + numCylinders; e++) {
        volatile VecFx32 d;
        d.x = pos->x;
        d.y = pos->y;
        d.z = pos->z;
        if (e->kind == 1 && e->landOnTop(pos, prev)) {
            *out = e->CollisionTagX::attr;
            return TRUE;
        }
    }
    return FALSE;
}

BOOL ShapeCylinderList::addCylinder(VecFx32 *pos, s32 b, s32 c, s32 a4, s32 a5) {
    if (numCylinders < 16) {
        cylinders[numCylinders].setupShape(pos, b, c, a4, a5);
        numCylinders++;
        return TRUE;
    }
    return FALSE;
}

void ShapeCylinderList::collectFromUnits(UnitShapeQueryX *query, s32 x0, s32 x1, s32 z0, s32 z1, s32 flag) {
    s32 z, x;
    for (z = z0; z <= z1; z++) {
        x = x0;
        s32 zw = (z << 13) + 0x1000;
        for (; x <= x1; x++) {
            s32 a, b;
            volatile s32 c;
            if (query->getUnitShape(&a, &b, (s32 *)&c, x, z)) {
                if (c == 2) {
                    if (flag) {
                        VecFx32 p;
                        p.x = (x << 13) + 0x1000;
                        p.y = 0;
                        p.z = zw;
                        p.y = 0;
                        addCylinder(&p, a, b, 2, c);
                    }
                } else {
                    VecFx32 p;
                    p.x = (x << 13) + 0x1000;
                    p.y = 0;
                    p.z = zw;
                    p.y = 0;
                    addCylinder(&p, a, b, 1, c);
                }
            } else if (flag) {
                if (sCollisionWorld.digHoles.activeMask != 0) {
                    u32 r = sCollisionWorld.digHoles.getRadiusAt(x, z);
                    if (r != 0) {
                        VecFx32 p;
                        p.x = (x << 13) + 0x1000;
                        p.y = 0;
                        p.z = zw;
                        p.y = 0;
                        addCylinder(&p, r, 0x1000, 2, 2);
                    }
                }
            }
        }
    }
}

void ShapeCylinderList::addLinkWalls(WallEdgeListWriter *out) {
    u32 n = numCylinders;
    ShapeCylinder *p = cylinders;
    u32 i;
    for (i = 0; i < n; p++, i++) {
        ShapeCylinder *q = cylinders;
        u32 j;
        for (j = 0; j < n; q++, j++) {
            ShapeCylinder_AddLinkWall(p, out, q);
        }
    }
}

CollisionContacts::CollisionContacts() {
    clear();
}

CollisionContacts::~CollisionContacts() {}

void CollisionContacts::clear() {
    s32* a = attrs;
    s32* b = kinds;
    s16* p = angles;
    s32 i;
    numContacts = 0;
    for (i = 0; i < 2; i++) {
        *p = 0;
        *a++ = 0;
        *b++ = 0;
        p++;
    }
}

BOOL CollisionContacts::addContact(s32 a, s32 b, s32 c) {
    s16* p = angles;
    s32* q = kinds;
    s32 i = 0;
    u8 n = numContacts;
    volatile s32 z = 0;
    for (; i < n; p++, q++, i++) {
        s32 t = *(s16*)((u8*)p + z);
        if (t == a && *q == b) {
            return FALSE;
        }
    }
    if (n < 2) {
        angles[n] = a;
        kinds[numContacts] = b;
        attrs[numContacts] = c;
        numContacts++;
        return TRUE;
    }
    return FALSE;
}

void CollisionState::beginStep() {
    contacts.clear();
    groundAttr = 0;
    prevFlags = flags;
    flags = 0;
    moveDelta = 0;
    moveDeltaY = 0;
    moveDeltaZ = 0;
}

void CollisionState::reset() {
    flags = 0;
    prevFlags = 0;
    groundAttr = 0;
    moveDelta = 0;
    moveDeltaY = 0;
    moveDeltaZ = 0;
}

CollisionState::CollisionState() {
    reset();
}

CollisionState::~CollisionState() {}

void CollisionState::updateWallFlags(s32 v) {
    s32 i = 0;
    s32 base = v + 0x8000;
    for (; i < contacts.numContacts; i++) {
        u32 d = (u16)(contacts.angles[i] - base);
        if (d < 0x2000 || d >= 0xe000) {
            if (contacts.kinds[i] == 2) {
                flags = flags | 0x80;
                flags = flags | 0x100;
            } else {
                flags = flags | 4;
                flags = flags | 8;
            }
        } else if (d < 0x6000) {
            if (contacts.kinds[i] == 2) {
                flags = flags | 0x80;
                flags = flags | 0x400;
            } else {
                flags = flags | 4;
                flags = flags | 0x20;
            }
        } else if (d < 0xa000) {
            if (contacts.kinds[i] == 2) {
                flags = flags | 0x80;
                flags = flags | 0x800;
            } else {
                flags = flags | 4;
                flags = flags | 0x40;
            }
        } else {
            if (contacts.kinds[i] == 2) {
                flags = flags | 0x80;
                flags = flags | 0x200;
            } else {
                flags = flags | 4;
                flags = flags | 0x10;
            }
        }
    }
    if (contacts.numContacts == 2) {
        s32 mid = ((contacts.angles[0] + contacts.angles[1]) << 15) >> 16;
        s32 diff = v - (s16)(mid + 0x7fff);
        if (diff < 0) {
            diff = -diff;
        }
        if (diff < 0x2000) {
            flags = flags | 0x1000;
        }
        if ((s16)(contacts.angles[0] + contacts.angles[1]) == 0) {
            flags = flags | 0x2000;
        }
    }
}

extern "C" void* CollisionTag_Construct(void* p) {
    new (p) CollisionTagX();
    return p;
}

extern "C" void* CollisionTag_Destruct(void* p) {
    static_cast<CollisionTagX *>(p)->~CollisionTagX();
    return p;
}

void MoveCollisionVisitor::visitWalls(WallEdgeList* p) {
    p->collide(pos, &prevPos, radius, state, actor);
}

void MoveCollisionVisitor::visitFloors(FloorTriangleList* fl) {
    u8* e;
    VecFx32CtorDtor dead;
    for (u32 i = 0; i < fl->numTriangles; i++) {
        FloorTriangle *t = &fl->triangles[i];
        e = (u8 *)t;
        if (pos->y < t->vertex0.y) {
            if (((CollisionTriangle *)(e))->containsXZ((Unk_0202f2ac_V3 *)(pos))) {
                pos->y = t->vertex0.y;
                state->flags |= 1;
                state->groundAttr = t->attr;
            }
        }
    }
}

void MoveCollisionVisitor::visitCylinders(ShapeCylinderList* p) {
    u32 out;
    if (flags & 1) {
        if (p->landOnTop(pos, &prevPos, &out)) {
            state->flags |= 1;
            state->groundAttr = out;
        }
    }
    if (flags & 6) {
        p->pushOut(pos, radius, state);
    }
}

void SegmentCollisionVisitor::visitWalls(WallEdgeList* p) {
    VecFx32 out;
    WallEdge* e;
    numWalls = p->numEdges;
    for (e = p->edges; e < p->edges + numWalls; e++) {
        VecFx32Ctor v0(e->normal.x, 0, e->normal.y);
        VecFx32Ctor v1(e->start.x, e->topY, e->start.y);
        VecFx32Ctor v2(e->start.x, -0x8000, e->start.y);
        VecFx32Ctor v3(e->end.x, -0x8000, e->end.y);
        WallCornerPadded l4(e->end.x, e->topY, e->end.y);
        CollisionTriangleX a((VecFx32 *)&v1, (VecFx32 *)&v2, (VecFx32 *)&v3, (VecFx32 *)&v0);
        CollisionTriangleX b((VecFx32 *)&v1, (VecFx32 *)&v3, (VecFx32 *)&l4.v4, (VecFx32 *)&v0);
        CollisionTriangleX* r;
        for (r = &a; r < &b + 1; r++) {
            if (((CollisionTriangle *)(r))->distanceTo((Unk_0202f2ac_V3 *)(&segStart)) >= 0) {
                if (Unk_02031f90_Ge0(((CollisionTriangle *)(r))->distanceTo((Unk_0202f2ac_V3 *)(segEnd)))) {
                    continue;
                }
                if (((CollisionTriangle *)(r))->intersectSegment((Unk_0202f2ac_V3 *)(&out), (Unk_0202f2ac_V3 *)(&segStart), (Unk_0202f2ac_V3 *)(segEnd))) {
                    s32 z = out.z;
                    VecFx32* d = segEnd;
                    s32 y = d->y;
                    s32 x = out.x;
                    d->x = x;
                    d->y = y;
                    d->z = z;
                    hitTag.copyTag(*e);
                    hitNormal = r->normal;
                    hitAttr = e->attr;
                    hasHit = 1;
                }
            }
        }
    }
}

void SegmentCollisionVisitor::visitFloors(FloorTriangleList* p) {
    VecFx32 out;
    FloorTriangle* e;
    BOOL one = TRUE;
    numFloors = p->numTriangles;
    for (e = p->triangles; e < p->triangles + numFloors; e++) {
        if (((CollisionTriangle *)(e))->distanceTo((Unk_0202f2ac_V3 *)(&segStart)) >= 0) {
            if (Unk_02031f90_Ge0(((CollisionTriangle *)(e))->distanceTo((Unk_0202f2ac_V3 *)(segEnd)))) {
                continue;
            }
            if (((CollisionTriangle *)(e))->intersectSegment((Unk_0202f2ac_V3 *)(&out), (Unk_0202f2ac_V3 *)(&segStart), (Unk_0202f2ac_V3 *)(segEnd))) {
                VecFx32* d = segEnd;
                d->x = out.x;
                d->y = out.y;
                d->z = out.z;
                hitTag.copyTag(*e);
                hitNormal = e->normal;
                hitAttr = e->attr;
                hasHit = one;
            }
        }
    }
}

void SegmentCollisionVisitor::visitCylinders(ShapeCylinderList* p) {
    ShapeCylinder* e;
    volatile VecFx32 t;
    VecFx32* q0;
    VecFx32* g;
    numCylinders = p->numCylinders;
    g = (VecFx32 *)&sCollisionUpVector;
    for (e = p->cylinders; e < p->cylinders + numCylinders; e++) {
        q0 = segEnd;
        t.x = q0->x;
        t.y = q0->y;
        t.z = q0->z;
        if (e->clipSegmentTop(segEnd, &segStart)) {
            hitTag.copyTag(*e);
            hitNormal.x = g->x;
            hitNormal.y = g->y;
            hitNormal.z = g->z;
            hitAttr = e->attr;
            hasHit = 1;
        }
        q0 = segEnd;
        t.x = q0->x;
        t.y = q0->y;
        t.z = q0->z;
        if (e->clipSegmentSide(segEnd, &segStart)) {
            hitTag.copyTag(*e);
            VecFx32* q = segEnd;
            s32 z = q->z - e->center.z;
            s32 x = q->x - e->center.x;
            hitNormal.x = x;
            hitNormal.y = 0;
            hitNormal.z = z;
            Vec_SafeNormalize(&hitNormal);
            hitAttr = e->attr;
            hasHit = 1;
        }
    }
}

TriangleTrigger::TriangleTrigger() {
    resetTrigger();
}

void TriangleTrigger::resetTrigger() {
    next = 0;
    radiusSq = 0;
    center.x = 0;
    center.y = 0;
    center.z = 0;
}

VecFx32* TriangleTrigger::getCenter() {
    return &center;
}

void TriangleTrigger::setupTrigger(VecFx32* a, VecFx32* b, VecFx32* c, s32 d) {
    VecFx32 v0, v1;
    u32 out[3];
    radiusSq = func_01ffcb0c(d, d);
    v0.x = a->x;
    v0.y = a->y;
    v0.z = a->z;
    v1.x = a->x;
    v1.y = a->y;
    v1.z = a->z;
    Vec3_MaxInPlace((s32 *)(&v0), (s32 *)(b));
    Vec3_MinInPlace((s32 *)(&v1), (s32 *)(b));
    Vec3_MaxInPlace((s32 *)(&v0), (s32 *)(c));
    Vec3_MinInPlace((s32 *)(&v1), (s32 *)(c));
    s32 z = (v1.z + v0.z) >> 1;
    s32 y = (v1.y + v0.y) >> 1;
    s32 x = (v1.x + v0.x) >> 1;
    center.x = x;
    center.y = y;
    center.z = z;
    Collision_CalcTriangleNormal(out, a, b, c);
    ((CollisionTriangle *)(this))->set((Unk_0202f2ac_V3 *)(void *)(a), (Unk_0202f2ac_V3 *)(void *)(b), (Unk_0202f2ac_V3 *)(void *)(c), (Unk_0202f2ac_V3 *)(out));
}

TriangleTriggerListOwner::TriangleTriggerListOwner() {}

TriangleTriggerListOwner::~TriangleTriggerListOwner() {}

extern "C" void TriangleTrigger_ClearList(void *p) {
    sTriangleTriggerList = NULL;
}

extern "C" s32 TriangleTrigger_Register(TriangleTrigger* node) {
    if (node->next == NULL) {
        node->next = sTriangleTriggerList;
        sTriangleTriggerList = node;
        return TRUE;
    }
    return FALSE;
}

extern "C" s32 TriangleTrigger_Unregister(TriangleTrigger* node) {
    TriangleTrigger* p;
    TriangleTrigger* prev;
    for (p = sTriangleTriggerList, prev = NULL; p != NULL; prev = p, p = p->next) {
        if (p == node) {
            if (prev != NULL) {
                prev->next = p->next;
            } else {
                sTriangleTriggerList = p->next;
            }
            node->resetTrigger();
            return TRUE;
        }
    }
    return FALSE;
}

extern "C" void TriangleTrigger_CheckAll(VecFx32* a, s32 b, Actor *c) {
    TriangleTrigger* p;
    p = sTriangleTriggerList;
    if (p != NULL) {
        for (; p != NULL; p = p->next) {
            if ((s64)p->radiusSq >= Vec_DistSq(&p->center, a)) {
                p->onActorNear(a, c, b);
            }
        }
    }
}

void BoxCollider::resetBox() {
    VecFx32* a;
    ZeroedCollisionVec2* b;
    s32 i;
    numCorners = 0;
    boxPos.x = 0;
    boxPos.y = 0;
    boxPos.z = 0;
    size.x = 0;
    size.y = 0;
    size.z = 0;
    scale.x = 0x1000;
    scale.y = 0x1000;
    scale.z = 0x1000;
    angle = 0;
    next = 0;
    isActive = 0;
    a = worldCorners;
    b = edgeNormals;
    for (i = 0; i < 4; i++) {
        a->x = 0;
        a->y = 0;
        a->z = 0;
        ((CollisionVec2 *)(b))->set(0, 0);
        a++;
        b++;
    }
}

BoxCollider::BoxCollider() {
    resetBox();
}

BoxCollider::~BoxCollider() {}

BOOL BoxCollider::setupBox(s32 a, s32 b, s32 c, VecFx32* p, s16 s, VecFx32* q) {
    size.x = a;
    size.y = b;
    size.z = c;
    static_cast<BoxColliderShape *>(this)->updateTransform(p, s, q);
    boxPos.x = p->x;
    boxPos.y = p->y;
    boxPos.z = p->z;
    scale.x = q->x;
    scale.y = q->y;
    scale.z = q->z;
    angle = s;
    isActive = 1;
    return TRUE;
}

void BoxCollider::onEdgeContact(CollisionEdge *edge, Actor *actor, s32 r) {}

BoxColliderListOwner::BoxColliderListOwner() {}

BoxColliderListOwner::~BoxColliderListOwner() {}

extern "C" void BoxCollider_ClearList(void *p) { sBoxColliderList = 0; }

BOOL BoxColliderShape::updateTransform(VecFx32 *a, s32 ang, VecFx32 *b)
{
    VecFx32 corners[4];
    VecFx32 hi, lo;
    VecFx32 *p, *q;
    VecFx32 *pv;
    ZeroedCollisionVec2 *as;
    s32 i, k;
    s32 hx, hy, hz;
    s32 cx, cy, cz, dx, dy, dz;
    BOOL result;
    if (ang == angle) {
        if (Vec_NotEqual(&boxPos, a) == 0) {
            if (Vec_NotEqual(&scale, b) == 0) goto end0;
        }
    }
    boxPos.x = a->x;
    boxPos.y = a->y;
    boxPos.z = a->z;
    scale.x = b->x;
    scale.y = b->y;
    scale.z = b->z;
    angle = ang;
    hx = size.x >> 1;
    hz = size.y >> 1;
    hy = size.z;
    corners[0].x = -hx;
    corners[0].y = hy;
    corners[0].z = hz;
    q = &corners[1];
    q->x = hx;
    q->y = hy;
    q->z = hz;
    q = &corners[2];
    q->x = hx;
    q->y = hy;
    q->z = -hz;
    q = &corners[3];
    q->x = -hx;
    q->y = hy;
    q->z = -hz;
    Mtx43_SetTranslate(data_021f47e0, a->x, a->y, a->z);
    Mtx43_RotateY(data_021f47e0, ang);
    Mtx43_Scale(data_021f47e0, b->x, b->y, b->z);
    p = corners;
    pv = worldCorners;
    as = edgeNormals;
    if (size.x == 0 || size.y == 0) {
        k = 1;
        if (size.y == 0) k = 0;
        numCorners = 2;
        for (i = 0; i < 4; p++, i++) {
            if ((i & 1) == k) {
                MTX_MultVec43(p, data_021f47e0, pv);
                if (i == k) {
                    hi.x = pv->x; hi.y = pv->y; hi.z = pv->z;
                    lo.x = pv->x; lo.y = pv->y; lo.z = pv->z;
                } else {
                    Vec3_MaxInPlace(&hi.x, &pv->x);
                    Vec3_MinInPlace(&lo.x, &pv->x);
                }
                as->set(0, 0x1000);
                as->rotate((s16)(ang + (i << 14)));
                pv++;
                as++;
            }
        }
    } else {
        numCorners = 4;
        for (i = 0; i < numCorners; i++) {
            MTX_MultVec43(p, data_021f47e0, pv);
            if (i == 0) {
                hi.x = pv->x; hi.y = pv->y; hi.z = pv->z;
                lo.x = pv->x; lo.y = pv->y; lo.z = pv->z;
            } else {
                Vec3_MaxInPlace(&hi.x, &pv->x);
                Vec3_MinInPlace(&lo.x, &pv->x);
            }
            as->set(0, 0x1000);
            as->rotate((s16)(ang + (i << 14)));
            pv++;
            p++;
            as++;
        }
    }
    cz = (hi.z + lo.z) >> 1;
    cy = (hi.y + lo.y) >> 1;
    cx = (hi.x + lo.x) >> 1;
    boundsCenter.x = cx;
    boundsCenter.y = cy;
    boundsCenter.z = cz;
    dz = hi.z - boundsCenter.z;
    dy = hi.y - boundsCenter.y;
    dx = hi.x - boundsCenter.x;
    boundsHalfSize.x = dx;
    boundsHalfSize.y = dy;
    boundsHalfSize.z = dz;
    result = TRUE;
    goto end;
end0:
    result = FALSE;
end:
    return result;
}

extern "C" BOOL BoxCollider_Register(BoxCollider *n, s32 a, s32 b, s32 c, VecFx32 *pos, s16 e, VecFx32 *v)
{
    VecFx32 s;
    s.x = 0x1000;
    s.y = 0x1000;
    s.z = 0x1000;
    if (v != 0) {
        s.x = v->x;
        s.y = v->y;
        s.z = v->z;
    }
    if (n->setupBox(a, b, c, pos, e, &s)) {
        n->next = sBoxColliderList;
        sBoxColliderList = n;
        return TRUE;
    }
    return FALSE;
}

extern "C" BOOL BoxCollider_Unregister(BoxCollider *n)
{
    BoxCollider *cur = sBoxColliderList;
    BoxCollider *prev = 0;
    while (cur != 0) {
        if (cur == n) {
            if (prev != 0) {
                prev->next = cur->next;
            } else {
                sBoxColliderList = cur->next;
            }
            n->resetBox();
            return TRUE;
        }
        prev = cur;
        cur = cur->next;
    }
    return FALSE;
}

extern "C" BOOL Collision_IsSegmentOutsideBox(VecFx32 *a, VecFx32 *b, VecFx32 *c, VecFx32 *d)
{
    VecFx32 mn, mx;
    VecFx32 *pts[2];
    s32 mask, i;
    pts[0] = c;
    pts[1] = d;
    Vec_Sub(&mn, a, b);
    Vec_Add(&mx, a, b);
    mask = 0xff;
    for (i = 0; i < 2; i++) {
        VecFx32 *p = pts[i];
        s32 f = 0;
        if (p->x < mn.x) f |= 1;
        else if (p->x > mx.x) f |= 2;
        if (p->z < mn.z) f |= 4;
        else if (p->z > mx.z) f |= 8;
        if (f == 0) return FALSE;
        mask &= f;
    }
    if (mask == 1) return TRUE;
    if (mask == 2) return TRUE;
    if (mask == 4) return TRUE;
    if (mask == 8) return TRUE;
    return FALSE;
}

extern "C" void BoxCollider_GatherAll(BoxColliderListOwner *unused, VecFx32 *pos, VecFx32 *size, WallEdgeListWriter *walls, FloorTriangleList *floors, u32 flags, s32 mode)
{
    u8 *q;
    BoxCollider *n = sBoxColliderList;
    u32 m2 = flags & 2;
    u32 m1 = flags & 1;
    for (; n != 0; n = n->next) {
        BOOL skip = FALSE;
        VecFx32 c, d;
        s32 i;
        if (mode == 0) {
            if (n->boxPos.y <= 0) skip = TRUE;
        } else {
            if (n->boxPos.y > 0) skip = TRUE;
        }
        if (skip) continue;
        c.x = n->boundsCenter.x;
        c.y = n->boundsCenter.y;
        c.z = n->boundsCenter.z;
        Vec_Sub(&d, &c, pos);
        if (Unk_02031618_Abs(d.x) >= n->boundsHalfSize.x + size->x) continue;
        if (Unk_02031618_Abs(d.z) >= n->boundsHalfSize.z + size->z) continue;
        if (m2 != 0) {
            u8 res[4];
            q = res;
            s32 base;
            for (i = 0; i < ((volatile BoxCollider *)n)->numCorners; i++) {
                *q = Collision_IsSegmentOutsideBox(pos, size, &n->worldCorners[i & (n->numCorners - 1)], &n->worldCorners[(i + 1) & (n->numCorners - 1)]);
                q++;
            }
            base = n->boxPos.y + func_01ffcb0c(n->size.z, n->scale.y);
            for (i = 0; i < ((volatile BoxCollider *)n)->numCorners; i++) {
                if (res[i & 3] == 0) {
                    Unk_02031960_P8 e0, e1;
                    ((CollisionVec2 *)(&e0))->set(n->worldCorners[i & (n->numCorners - 1)].x, n->worldCorners[i & (n->numCorners - 1)].z);
                    ((CollisionVec2 *)(&e1))->set(n->worldCorners[(i + 1) & (n->numCorners - 1)].x, n->worldCorners[(i + 1) & (n->numCorners - 1)].z);
                    // (a WallEdge of its own: it was built in a 0x34-byte buffer, the DS's size; 64 bytes on 64-bit hosts)
                    WallEdge obj((CollisionVec2 *)(void *)(&e0), (CollisionVec2 *)(void *)(&e1), &n->edgeNormals[i & (n->numCorners - 1)], base, 1, 1, n);
                    walls->addEdgeCopy(&obj);
                }
            }
        }
        if (m1 != 0) {
            s32 cc = n->numCorners - 1;
            floors->addTriangle((Unk_0202f2ac_V3 *)(void *)(&n->worldCorners[0]), (Unk_0202f2ac_V3 *)(void *)(&n->worldCorners[cc & 1]), (Unk_0202f2ac_V3 *)(void *)(&n->worldCorners[cc & 3]), (Unk_0202f2ac_V3 *)(&sCollisionUpVector), 1, n);
            cc = n->numCorners - 1;
            floors->addTriangle((Unk_0202f2ac_V3 *)(void *)(&n->worldCorners[cc & 1]), (Unk_0202f2ac_V3 *)(void *)(&n->worldCorners[cc & 2]), (Unk_0202f2ac_V3 *)(void *)(&n->worldCorners[cc & 3]), (Unk_0202f2ac_V3 *)(&sCollisionUpVector), 1, n);
        }
    }
}

UnitShapeQueryX::UnitShapeQueryX() {}

UnitShapeQueryX::~UnitShapeQueryX() {}

BOOL UnitShapeQueryX::getUnitShape(s32 *a, s32 *b, s32 *c, s32 x, s32 z) { return FALSE; }

CollisionVisitor::CollisionVisitor() {}

CollisionVisitor::~CollisionVisitor() {}

void CollisionVisitor::visitWalls(WallEdgeList *p) {}

void CollisionVisitor::visitFloors(FloorTriangleList *p) {}

void CollisionVisitor::visitCylinders(ShapeCylinderList *p) {}

extern "C" u32 CollisionMap_IsFullyBound(s32 i)
{
    return sCollisionWorld.maps[i].allBlocksSet;
}

extern "C" void Vec3_MaxInPlace(s32 *a, s32 *b)
{
    if (b[0] > a[0]) a[0] = b[0];
    if (b[1] > a[1]) a[1] = b[1];
    if (b[2] > a[2]) a[2] = b[2];
}

extern "C" void Vec3_MinInPlace(s32 *a, s32 *b)
{
    if (b[0] < a[0]) a[0] = b[0];
    if (b[1] < a[1]) a[1] = b[1];
    if (b[2] < a[2]) a[2] = b[2];
}

extern "C" s32 Ground_GetUnitQuadrant(VecFx32 *p)
{
    VecFx32 v, w;
    s32 s, d;
    v.x = p->x;
    v.y = p->y;
    v.z = p->z;
    w.x = (v.x & 0xffffe000) + 0x1000;
    w.y = 0;
    w.z = (v.z & 0xffffe000) + 0x1000;
    VEC_Subtract(&v, &w, &v);
    d = v.z - v.x;
    s = v.x + v.z;
    if (s > 0) {
        if (d > 0) return 2;
        return 3;
    }
    if (d > 0) return 1;
    return 0;
}

extern "C" u32 GroundAttr_GetQuadAttr0Q3(s32 t) { return t < 0x7c ? sGroundAttrTable[t].quadAttr0Q3 : 0; }

extern "C" u32 GroundAttr_GetQuadAttr0Q2(s32 t) { return t < 0x7c ? sGroundAttrTable[t].quadAttr0Q2 : 0; }

extern "C" u32 GroundAttr_GetQuadAttr0Q1(s32 t) { return t < 0x7c ? sGroundAttrTable[t].quadAttr0Q1 : 0; }

extern "C" u32 GroundAttr_GetQuadAttr0Q0(s32 t) { return t < 0x7c ? sGroundAttrTable[t].quadAttr0Q0 : 0; }

extern "C" u32 Ground_GetQuadAttr0(s32 x, s32 y, u32 c)
{
    return sGroundQuadAttr0Getters[c & 3](Ground_GetUnitAttr(x, y));
}

extern "C" u32 GroundAttr_GetQuadAttr1Q3(s32 t) { return t < 0x7c ? sGroundAttrTable[t].quadAttr1Q3 : 0; }

extern "C" u32 GroundAttr_GetQuadAttr1Q2(s32 t) { return t < 0x7c ? sGroundAttrTable[t].quadAttr1Q2 : 0; }

extern "C" u32 GroundAttr_GetQuadAttr1Q1(s32 t) { return t < 0x7c ? sGroundAttrTable[t].quadAttr1Q1 : 0; }

extern "C" u32 GroundAttr_GetQuadAttr1Q0(s32 t) { return t < 0x7c ? sGroundAttrTable[t].quadAttr1Q0 : 0; }

extern "C" u32 Ground_GetQuadAttr1(s32 x, s32 y, u32 c)
{
    return sGroundQuadAttr1Getters[c & 3](Ground_GetUnitAttr(x, y));
}

extern "C" UnitShapeQueryX *Collision_GetShapeQuery()
{
    static UnitShapeQueryX inst;
    UnitShapeQueryX *p = gCurCollisionMap->shapeQuery;
    if (p == 0) p = &inst;
    return p;
}

extern "C" BOOL GroundAttr_IsWater(s32 t, s32 k)
{
    u32 v = t < 0x7c ? sGroundAttrTable[t].waterKind : 0;
    if (v != 0) {
        switch (k) {
        case 1:
            if (t == 0x16) return FALSE;
            break;
        case 2:
            if (t == 0x16) return FALSE;
            break;
        }
        return TRUE;
    }
    return FALSE;
}

extern "C" BOOL Ground_IsRaisedOrOccupied(VecFx32 *v)
{
    u32 obj[16];
    s32 a, b, c;
    ((GroundInfo *)(obj))->initAtPos((VecFx32 *)(v), 0, 0);
    if (((GroundInfoBase *)(obj))->getHeight(0)) {
        GroundInfo_Destruct(obj);
        return TRUE;
    }
    if (Collision_GetUnitShape(v->x >> 13, v->z >> 13, &a, &b, &c)) {
        GroundInfo_Destruct(obj);
        return TRUE;
    }
    GroundInfo_Destruct(obj);
    return FALSE;
}

extern "C" BOOL Ground_IsPond(s32 x, s32 y)
{
    s32 t = Ground_GetUnitAttr(x, y);
    if (t == 7) return TRUE;
    return FALSE;
}

extern "C" BOOL Ground_IsGrassUnit(s32 x, s32 y)
{
    s32 t = Ground_GetUnitAttr(x, y);
    if (t == 3 || t == 0x1d) return TRUE;
    return FALSE;
}

extern "C" s32 Ground_GetWaterKind(s32 x, s32 y)
{
    s32 t = Ground_GetUnitAttr(x, y);
    if (t == 8) return 1;
    if (t == 7 || (t >= 0xb && t <= 0x12)) return 2;
    return 0;
}

extern "C" s32 Ground_CanPlaceItem(s32 x, s32 y)
{
    s32 t = Ground_GetUnitAttr(x, y);
    if (t < 0x7c) return sGroundAttrTable[t].itemPlaceable;
    return 0;
}

extern "C" s32 Ground_IsGrassSurface(s32 x, s32 y)
{
    s32 t = Ground_GetUnitAttr(x, y);
    if (t < 0x7c) return sGroundAttrTable[t].grassSurface;
    return 0;
}

extern "C" s32 Ground_IsWalkable(s32 x, s32 y)
{
    s32 t = Ground_GetUnitAttr(x, y);
    if (t < 0x7c) return sGroundAttrTable[t].walkable;
    return 0;
}

extern "C" s32 Ground_GetDigKind(s32 x, s32 y)
{
    s32 t = Ground_GetUnitAttr(x, y);
    if (t < 0x7c) return sGroundAttrTable[t].digKind;
    return 2;
}

extern "C" s32 Ground_GetPlantFlag(s32 x, s32 y)
{
    s32 t = Ground_GetUnitAttr(x, y);
    if (t < 0x7c) {
        s32 v = sGroundAttrTable[t].plantFlag;
        if (v > 0) v = 1;
        return v;
    }
    return -1;
}

extern "C" BOOL Ground_IsShore(s32 x, s32 y)
{
    s32 t = Ground_GetUnitAttr(x, y);
    u32 v = t < 0x7c ? sGroundAttrTable[t].isShore : 0;
    return v != 0 ? TRUE : FALSE;
}

extern "C" BOOL Ground_IsSandAboveSea(s32 a, s32 b)
{
    if (Ground_GetUnitAttr(a, b) == 0x1e && Ground_GetUnitAttr(a, b + 1) == 8) return TRUE;
    return FALSE;
}

extern "C" u32 Ground_GetWalkLinks(s32 x, s32 y)
{
    CollisionBlockRef *p = CollisionMap_GetBlockRef(x >> 4, y >> 4);
    if (p) {
        s32 i, b;
        x &= 15;
        y &= 15;
        i = x + y * 16;
        b = p->walkLinks[i >> 1];
        if (i & 1) return (b >> 4) & 15;
        return b & 15;
    }
    return 0;
}

extern "C" BOOL Ground_IsFreeGrassOffPath(s32 a, s32 b)
{
    if (Ground_GetWalkLinks(a, b)) return FALSE;
    return Ground_IsFreeGrass(a, b);
}

extern "C" BOOL Ground_IsFreeGrass(s32 a, s32 b)
{
    UnitShapeQueryX *query = Collision_GetShapeQuery();
    if (query) {
        s32 x, y, z;
        if (query->getUnitShape(&x, &y, &z, a, b)) return FALSE;
    }
    return Ground_IsGrassSurface(a, b);
}

extern "C" BOOL Ground_GetMapColors(u8 *out, s32 a, s32 b)
{
    s32 i = Ground_GetUnitAttr(a, b);
    if (i < 0x7c) {
        out[0] = sGroundAttrTable[i].mapColor0;
        out[1] = sGroundAttrTable[i].mapColor1;
        out[2] = sGroundAttrTable[i].mapColor2;
        out[3] = sGroundAttrTable[i].mapColor3;
        return TRUE;
    }
    out[0] = sDefaultGroundColors[0];
    out[1] = sDefaultGroundColors[1];
    out[2] = sDefaultGroundColors[2];
    out[3] = sDefaultGroundColors[3];
    return FALSE;
}

extern "C" u16 GroundAttr_GetFootstepSe(s32 i)
{
    if (i < 0x7c) return sGroundAttrTable[i].footstepSe;
    return 0xffff;
}

extern "C" u16 GroundAttr_GetDragSe(s32 i)
{
    if (i < 0x7c) return sGroundAttrTable[i].dragSe;
    return 0xffff;
}

extern "C" BOOL Ground_IsNeighbourReachable(s32 a, s32 b, s32 c, s32 d, u8 flag)
{
    s32 dy, dx;
    s32 r5;
    volatile VecFx32Ctor A;
    VecFx32Ctor Q;
    dx = a - c;
    if (dx < 0) dx = -dx;
    if (dx > 1) return FALSE;
    dy = b - d;
    if (dy < 0) dy = -dy;
    if (dy > 1) return FALSE;
    if (flag) {
        if (!Ground_CanPlaceItem(c, d) || Ground_IsShore(c, d)) return FALSE;
    } else {
        if (!Ground_CanPlaceItem(c, d)) return FALSE;
    }
    A.x = (a << 13) + 0x1000;
    A.y = 0;
    A.z = (b << 13) + 0x1000;
    Q.x = (c << 13) + 0x1000;
    Q.y = 0;
    Q.z = (d << 13) + 0x1000;
    dx += dy;
    r5 = Ground_GetHeightAt((VecFx32 *)(&Q), 0, 25);
    if (dx == 1) {
        if (r5 != 0) return FALSE;
    } else if (dx == 2) {
        NeighbourReachLocals l;
        s32 tR, tS;
        l.R.x = Q.x;
        l.R.y = 0;
        l.R.z = A.z;
        tR = Ground_GetHeightAt((VecFx32 *)(&l.R), 0, 25);
        if (tR == 0 && r5 == tR) {
            ((GroundCell *)(void *)(&l.T))->loadAtPos((VecFx32 *)&l.R, 0);
            if (Unk_02030f10_Flat(&l.T)) return TRUE;
        }
        l.S.x = A.x;
        l.S.y = 0;
        l.S.z = Q.z;
        tS = Ground_GetHeightAt((VecFx32 *)(&l.S), 0, 25);
        if (tS == 0 && r5 == tS) {
            ((GroundCell *)(void *)(&l.T2))->loadAtPos((VecFx32 *)&l.S, 0);
            if (Unk_02030f10_Flat(&l.T2)) return TRUE;
        }
        return FALSE;
    }
    return TRUE;
}

extern "C" BOOL Ground_IsWaterAround(VecFx32 *pos, s32 r, s32 *out, s32 flags)
{
    GroundInfo a((VecFx32 *)pos, 0, flags);
    if (a.waterKind) {
        VecFx32 tmp;
        VecFx32 d[8];
        s32 nr = -r;
        s32 z = 0;
        s32 best;
        s32 i;
        d[0].x = nr; d[0].y = 0; d[0].z = nr;
        { s32 *q = (s32 *)&d[1]; q[0] = nr; q[1] = z; q[2] = r; }
        { s32 *q = (s32 *)&d[2]; q[0] = r; q[1] = z; q[2] = r; }
        { s32 *q = (s32 *)&d[3]; q[0] = r; q[1] = z; q[2] = nr; }
        { s32 *q = (s32 *)&d[4]; q[0] = nr; q[1] = z; q[2] = z; }
        { s32 *q = (s32 *)&d[5]; q[0] = r; q[1] = z; q[2] = z; }
        { s32 *q = (s32 *)&d[6]; q[0] = z; q[1] = z; q[2] = r; }
        { s32 *q = (s32 *)&d[7]; q[0] = z; q[1] = z; q[2] = nr; }
        best = z;
        for (i = 0; i < 8; i++) {
            Vec_Add(&tmp, pos, &d[i]);
            GroundInfo b((VecFx32 *)&tmp, z, flags);
            if (!b.waterKind) return FALSE;
            best = a.waterSurfaceY;
        }
        if (out) *out = best;
        return TRUE;
    }
    return FALSE;
}

extern "C" BOOL Ground_FindWaterAlongDir(VecFx32 *out, VecFx32 *pos, u32 dist, s32 dir, u32 count, s32 r, s32 flags)
{
    if (count >= 1) {
        u32 step = DS_UDIV(dist, count);
        VecFx32 v;
        u32 i;
        v.x = 0;
        v.y = 0;
        v.z = 0x1000;
        Vec_RotateY(&v, dir);
        for (i = 0; i <= count; i++) {
            s32 t = i;
            s32 u, s, res;
            VecFx32 p;
            t = t * step;
            s = pos->z + func_01ffcb0c(v.z, t);
            u = pos->y + func_01ffcb0c(v.y, t);
            p.x = pos->x + func_01ffcb0c(v.x, t);
            p.y = u;
            p.z = s;
            if (Ground_IsWaterAround(&p, r, &res, flags)) {
                out->x = p.x;
                out->y = p.y;
                out->z = p.z;
                out->y = res;
                return TRUE;
            }
        }
    }
    return FALSE;
}

extern "C" BOOL Ground_FindWaterAhead(VecFx32 *out, VecFx32 *pos, s32 dir, u32 dist, u32 s5, s32 s6)
{
    return Ground_FindWaterAlongDir(out, pos, dist, dir, s6, s5, 2);
}

extern "C" BOOL Ground_IsWaterAt(VecFx32 *pos)
{
    return Ground_IsWaterAround(pos, 0xa00, 0, 2);
}

extern "C" u32 CollisionMap_IsBound(s32 a) { return CollisionMap_IsFullyBound(a); }

GroundCell::~GroundCell()
{
}

extern "C" s32 Ground_FindTerrainMarker(s32 *a, s32 *b, s32 c, s32 d)
{
    s32 bx, bz, j, i;
    GroundCellView X, Y, Z;
    if (CollisionMap_GetBlockRef(c, d) == 0) return 4;
    bx = c << 4;
    bz = d << 4;
    for (j = 0; j < 16; j++) {
        for (i = 0; i < 16; i++) {
            *a = bx + i;
            *b = bz + j;
            ((GroundCell *)(void *)(&X))->load(*a, *b, 0);
            ((GroundCell *)(void *)(&Y))->load(*a + 1, *b, 0);
            if (Unk_02030be4_A(&X)) {
                if (Unk_02030be4_B(&Y)) {
                    ((GroundCell *)(void *)(&Z))->load(*a + 2, *b, 0);
                    if (Unk_02030be4_B(&Z)) return 1;
                    return 0;
                }
            }
            if (X.quadAttrs[0] != 0x14 && X.quadAttrs[1] != 0x14 && X.quadAttrs[2] == 0x14 && X.quadAttrs[3] == 0x14
                && Y.quadAttrs[0] != 0x14 && Y.quadAttrs[1] == 0x14 && Y.quadAttrs[2] == 0x14 && Y.quadAttrs[3] != 0x14) {
                s32 idx = Y.quadAttrs[3];
                s32 v;
                if (idx < 0x7c) v = sGroundAttrTable[idx].waterKind;
                else v = 0;
                if (v == 0) return 2;
                return 3;
            }
        }
    }
    return 4;
}

extern "C" s32 Ground_GetSpecialPieceKind(s32 x, s32 y)
{
    s32 r = Ground_GetUnitAttr(x, y);
    if (r >= 0x6f && r <= 0x70) return r - 0x6f;
    return -1;
}

extern "C" void Collision_Move(CollisionState *self, VecFx32 *pos, VecFx32 *tgt, u16 hh, s32 arg5, Actor *actor, u32 flags)
{
    struct { VecFx32 A, B, V1, C, D; } l;
    s32 lim, dx, dz;
    BOOL fl;
    l.A = *pos;
    l.B = *tgt;
    lim = (sCollisionQueryMargin.x + arg5) * 2;
    dx = l.A.x - tgt->x;
    if (dx < 0) dx = -dx;
    if (lim + dx > 0xc000) goto reset;
    dz = l.A.z - tgt->z;
    if (dz < 0) dz = -dz;
    if (lim + dz > 0xc000) {
    reset:
        l.B = l.A;
    }
    l.V1.x = arg5;
    l.V1.y = arg5;
    l.V1.z = arg5;
    l.C = l.B;
    l.D = l.B;
    Vec3_MaxInPlace((s32 *)(&l.C), (s32 *)(&l.A));
    Vec3_MinInPlace((s32 *)(&l.D), (s32 *)(&l.A));
    VEC_Add(&l.C, &l.V1, &l.C);
    VEC_Subtract(&l.D, &l.V1, &l.D);
    MoveCollisionVisitor o;
    o.state = self;
    o.pos = &l.A;
    o.prevPos.x = l.B.x;
    o.prevPos.y = l.B.y;
    o.prevPos.z = l.B.z;
    o.facingAngle = hh;
    o.radius = arg5;
    o.actor = actor;
    o.flags = flags;
    (self)->beginStep();
    fl = (self->prevFlags & 2) ? TRUE : FALSE;
    ((void (*)(void *, void *, void *, u32, s32, s32))Collision_Query)(&l.D, &l.C, &o, flags, 0, fl);
    if ((flags & 4) && Ground_IsRaisedOrOccupied((VecFx32 *)(&l.A))) {
        l.A.x = tgt->x;
        l.A.y = tgt->y;
        l.A.z = tgt->z;
    }
    if (flags & 1) {
        s32 r2 = 1;
        s32 r3;
        if (!(self->prevFlags & 2)) r2 = 0;
        r3 = (flags & 0x80) ? 1 : 0;
        GroundInfo E((VecFx32 *)&l.A, r2, r3);
        if (l.A.y < E.getHeight(0) + 0x200) {
            self->flags |= 1;
            self->groundAttr = E.attr;
            l.A.y = E.getHeight(0) + 0x200;
        }
        if (E.isBelowWaterSurface(l.A.y)) self->flags |= 2;
    }
    (self)->updateWallFlags(hh);
    VecFx32 F;
    Vec_Sub(&F, &l.A, pos);
    self->moveDelta = F.x;
    self->moveDeltaY = F.y;
    self->moveDeltaZ = F.z;
    if (flags & 8) {
        pos->x = l.A.x;
        pos->y = l.A.y;
        pos->z = l.A.z;
    }
    if (actor) TriangleTrigger_CheckAll(&l.A, arg5, actor);
}

extern "C" u8 Collision_TestSegment(SegmentHitResult *out, VecFx32 *pos, VecFx32 *tgt, u32 flags)
{
    VecFx32 A, B, C;
    A = *pos;
    B = *tgt;
    C = *tgt;
    Vec3_MinInPlace((s32 *)(&B), (s32 *)(&A));
    Vec3_MaxInPlace((s32 *)(&C), (s32 *)(&A));
    SegmentCollisionVisitor o;
    o.hitAttr = 0;
    o.segEnd = (VecFx32 *)&A;
    o.segStart.x = tgt->x;
    o.segStart.y = tgt->y;
    o.segStart.z = tgt->z;
    o.flags = flags;
    o.hasHit = 0;
    o.numWalls = 0;
    o.numFloors = 0;
    o.numCylinders = 0;
    ((void (*)(void *, void *, void *, u32, s32, s32))Collision_Query)(&B, &C, &o, flags, 1, 0);
    if (flags & 8) *pos = A;
    out->hitNormal.x = o.hitNormal.x;
    out->hitNormal.y = o.hitNormal.y;
    out->hitNormal.z = o.hitNormal.z;
    ((CollisionTagX *)out)->copyTag(o.hitTag);
    return o.hasHit;
}

extern "C" BOOL Collision_ClampToRect(s32 *p, s32 a, s32 *c, s32 w, s32 h)
{
    s32 hw = w >> 1;
    s32 hh = h >> 1;
    s32 cx = c[0];
    s32 lox = a + (cx - hw);
    s32 hix = cx + hw - a;
    s32 cz = c[2];
    s32 loz = a + (cz - hh);
    s32 hiz = cz + hh - a;
    BOOL r = FALSE;
    if (p[0] < lox) { p[0] = lox; r = TRUE; }
    else if (p[0] > hix) { p[0] = hix; r = TRUE; }
    if (p[2] < loz) { p[2] = loz; r = TRUE; }
    else if (p[2] > hiz) { p[2] = hiz; r = TRUE; }
    return r;
}

extern "C" s32 Ground_GetHeightAt(VecFx32 *p, u32 *out, u32 flags0) {
    SegmentHitResult a;
    VecFx32 v14, v20;
    s32 r;
    CollisionTag_Construct(&a);
    Unk_0203081c_Flags flags = (Unk_0203081c_Flags)(flags0 & ~6);
    v14.x = p->x;
    v14.y = p->y;
    v14.z = p->z;
    v20.x = p->x;
    v20.y = p->y;
    v20.z = p->z;
    v14.y = 0x64000;
    v20.y = 0xfff9c000;
    if (Collision_TestSegment(&a, (VecFx32 *)(&v20), (VecFx32 *)(&v14), flags)) {
        if (out) *out = a.attr;
        r = v20.y;
        CollisionTag_Destruct(&a);
        return r;
    }
    {
        GroundInfo b((VecFx32 *)p, 0, 0);
        if (out) *out = b.attr;
        r = b.getHeight(0);
    }
    CollisionTag_Destruct(&a);
    return r;
}

extern "C" s32 Ground_GetDefaultY(void) {
    return 0x200;
}

extern "C" BOOL Collision_GetUnitShape(s32 x, s32 z, s32 *p, s32 *q, s32 *r) {
    UnitShapeQueryX *query = gCurCollisionMap->shapeQuery;
    if (query) {
        s32 a, b, c;
        if (query->getUnitShape(&a, &b, &c, x, z)) {
            *p = a;
            *q = b;
            *r = c;
            return TRUE;
        }
    }
    return FALSE;
}

extern "C" s32 Collision_HasUnitShape(s32 x, s32 z) {
    s32 a, b, c;
    return Collision_GetUnitShape(x, z, &a, &b, &c);
}

extern "C" s32 Collision_HasUnitShapeAt(VecFx32 *p) {
    return Collision_HasUnitShape(p->x >> 13, p->z >> 13);
}

extern "C" void Collision_Query(VecFx32 *a, VecFx32 *b, CollisionVisitor *visitor, u32 flags, u8 p5, u8 p6) {
    VecFx32 v18, v24, v30, v3c;
    s32 x1, x2, z1, z2;
    BOOL f4 = (flags & 4) ? TRUE : FALSE;
    Vec_Sub(&v18, a, &sCollisionQueryMargin);
    Vec_Add(&v24, b, &sCollisionQueryMargin);
    x1 = v18.x >> 13;
    z1 = v18.z >> 13;
    x2 = v24.x >> 13;
    z2 = v24.z >> 13;
    Vec_Add(&v30, &v18, &v24);
    v30.x >>= 1;
    v30.y >>= 1;
    v30.z >>= 1;
    Vec_Sub(&v3c, &v24, &v18);
    v3c.x >>= 1;
    v3c.y >>= 1;
    v3c.z >>= 1;
    sCollisionWorld.shapeCylinders.numCylinders = 0;
    sCollisionWorld.wallEdges.numEdges = 0;
    sCollisionWorld.floorTriangles.numTriangles = 0;
    ((GroundCellGrid *)(&sCollisionWorld.cellGrid))->loadArea(x1, x2, z1, z2, p6);
    if ((flags & 7) && !(flags & 0x20)) {
        sCollisionWorld.shapeCylinders.collectFromUnits(Collision_GetShapeQuery(), x1, x2, z1, z2, f4);
    }
    if (!(flags & 0x40)) {
        BoxCollider_GatherAll(&sCollisionWorld.boxColliders, &v30, &v3c, (WallEdgeListWriter *)&sCollisionWorld.wallEdges, &sCollisionWorld.floorTriangles, flags, 0);
        BoxCollider_GatherAll(&sCollisionWorld.boxColliders, &v30, &v3c, (WallEdgeListWriter *)&sCollisionWorld.wallEdges, &sCollisionWorld.floorTriangles, flags, 1);
    }
    if (flags & 6) {
        if (!(flags & 0x10)) ((ShapeCylinderList *)(&sCollisionWorld.shapeCylinders))->addLinkWalls((WallEdgeListWriter *)(&sCollisionWorld.wallEdges));
        ((WallEdgeListWriter *)(&sCollisionWorld.wallEdges))->buildFromCells(&sCollisionWorld.cellGrid, x1, x2, z1, z2, f4);
    }
    if (p5 && flags) {
        ((FloorTriangleList *)(&sCollisionWorld.floorTriangles))->buildFromCells((GroundCellGrid *)(&sCollisionWorld.cellGrid), x1, x2, z1, z2);
    }
    visitor->visitCylinders(&sCollisionWorld.shapeCylinders);
    visitor->visitWalls(&sCollisionWorld.wallEdges);
    visitor->visitFloors(&sCollisionWorld.floorTriangles);
}

extern "C" void CollisionMap_Select(s32 v) {
    if (!((CollisionMapIndex *)(&sCollisionWorld))->setIndex(v)) {
        if (v < 0) {
            ((CollisionMapIndex *)(&sCollisionWorld))->setIndex(0);
            gCurCollisionMap = &sCollisionWorld.maps[sCollisionWorld.curMapIndex];
        } else {
            ((CollisionMapIndex *)(&sCollisionWorld))->setIndex(7);
            gCurCollisionMap = &sCollisionWorld.maps[sCollisionWorld.curMapIndex];
        }
    } else {
        gCurCollisionMap = &sCollisionWorld.maps[sCollisionWorld.curMapIndex];
    }
}

extern "C" BOOL CollisionMap_Bind(u32 a, u32 b, UnitShapeQueryX *o, s32 idx) {
    CollisionMap *m = &sCollisionWorld.maps[idx];
    BOOL ok = TRUE;
    CollisionMap_Reset(idx);
    if (a <= 6) m->numBlocksX = (u8)a; else ok = FALSE;
    if (b <= 6) m->numBlocksZ = (u8)b; else ok = FALSE;
    m->shapeQuery = o;
    m->allBlocksSet = 0;
    CollisionMap_Select(0);
    return ok;
}

extern "C" void Collision_UpdateDigHoles(void) {
    ((DigHoleColliders *)(&sCollisionWorld.digHoles))->update();
}

extern "C" void Collision_AddDigHole(s32 a, s32 b) {
    ((DigHoleColliders *)(&sCollisionWorld.digHoles))->addHole(a, b);
}

extern "C" void Ground_SetWalkLinks(s32 x, s32 z, s32 v) {
    CollisionBlockRef *ch = CollisionMap_GetBlockRef(x >> 4, z >> 4);
    if (ch) {
        s32 n;
        u8 *p;
        x &= 0xf;
        z &= 0xf;
        n = x + (z << 4);
        p = ch->walkLinks + (n >> 1);
        if (n & 1) {
            *p = *p & ~0xf0;
            *p = *p | (v << 4);
        } else {
            *p = *p & ~0xf;
            *p = *p | v;
        }
    }
}

extern "C" void Ground_ClearWalkLinks(s32 x, s32 z, s32 mask) {
    s32 t = Ground_GetWalkLinks(x, z) & ~mask;
    Ground_SetWalkLinks(x, z, t);
}

extern "C" BOOL CollisionMap_SetBlock(s32 x, s32 y, void *bcl, s32 idx) {
    CollisionBlockRef *c = Unk_0203030c_Get(&sCollisionWorld.maps[idx], x, y);
    if (c) {
        c->attrs = (u8 *)bcl;  // (the block's collision data: 0x100 attribute bytes, then the walk links)
        c->walkLinks = (u8 *)bcl + 0x100;
        sCollisionWorld.maps[idx].allBlocksSet = Unk_020303d0_All(idx);
        return TRUE;
    }
    return FALSE;
}

extern "C" void CollisionMap_Reset(s32 idx) {
    ((CollisionMap *)(&sCollisionWorld.maps[idx]))->reset();
    sCollisionWorld.wallEdges.numEdges = 0;
    sCollisionWorld.floorTriangles.numTriangles = 0;
    sCollisionWorld.shapeCylinders.numCylinders = 0;
    BoxCollider_ClearList(&sCollisionWorld.boxColliders);
    TriangleTrigger_ClearList(&sCollisionWorld.triangleTriggers);
    ((DigHoleColliders *)(&sCollisionWorld.digHoles))->clear();
}

extern "C" void CollisionMap_ClearBlocks(s32 idx) {
    CollisionMap *m = &sCollisionWorld.maps[idx];
    s32 y, x;
    for (y = 0; (u32)y < m->numBlocksZ; y++) {
        for (x = 0; (u32)x < m->numBlocksX; x++) {
            CollisionBlockRef *c = Unk_0203030c_Get(m, x, y);
            if (c) {
                c->attrs = 0;
                c->walkLinks = 0;
            }
        }
    }
}

extern "C" void CollisionMap_Release(s32 idx) {
    CollisionMap_ClearBlocks(idx);
    CollisionMap_Reset(idx);
}

extern "C" void Ground_SetWaveLevel(s32 v) {
    CollisionMap *c = gCurCollisionMap;
    s16 t = c->waveLevel;
    if (t != v) {
        c->prevWaveLevel = t;
        gCurCollisionMap->waveLevel = v;
    }
}

extern "C" BOOL Ground_ClearPlantFlag(s32 x, s32 z) {
    CollisionBlockRef *ch = CollisionMap_GetBlockRef(x >> 4, z >> 4);
    if (ch) {
        u8 *base = ch->attrs;
        s32 i;
        u8 *cell;
        s32 old, t, v;
        u32 e0, e1, e2, e3;
        s32 d0, d1, d2, d3, d4;
        x &= 0xf;
        z &= 0xf;
        cell = base + (x + (z << 4));
        old = *cell;
        t = old < 0x7c ? ((v = sGroundAttrTable[old].plantFlag) > 0 ? 1 : v) : -1;
        if (t == 1) {
            if (old == 3) {
                *cell = 0x1d;
                return TRUE;
            }
            if (old == 9) {
                *cell = 0x1e;
                return TRUE;
            }
            e0 = old < 0x7c ? sGroundAttrTable[old].quadAttr0Q0 : 0;
            e1 = old < 0x7c ? sGroundAttrTable[old].quadAttr0Q1 : 0;
            e2 = old < 0x7c ? sGroundAttrTable[old].quadAttr0Q2 : 0;
            e3 = old < 0x7c ? sGroundAttrTable[old].quadAttr0Q3 : 0;
            d0 = -1;
            d1 = d2 = d3 = d4 = 0;
            for (i = 0; (u32)i < 0x7c; i++) {
                if (TS(i, d0) == 0 && e0 == TB(i, quadAttr0Q0, d1) && e1 == TB(i, quadAttr0Q1, d2) && e2 == TB(i, quadAttr0Q2, d3) && e3 == TB(i, quadAttr0Q3, d4)) {
                    *cell = i;
                    return TRUE;
                }
            }
            return FALSE;
        }
        return TRUE;
    }
    return FALSE;
}

extern "C" BOOL Ground_SetQuadrantsBlocked(s32 x, s32 z, s32 mask) {
    CollisionBlockRef *ch = CollisionMap_GetBlockRef(x >> 4, z >> 4);
    if (ch) {
        s32 c;
        s32 old;
 u8 *base;
        s32 a;
        s32 b;
        s32 i;
        s32 d0, d1, d2, d3;
        s32 was;
        u8 *cell;
        base = ch->attrs;
        x &= 0xf;
        z &= 0xf;
        cell = base + (x + (z << 4));
        old = *cell;
        was = old;
        a = (mask & 1) ? 10 : old;
        b = ((mask >> 1) & 1) ? 10 : old;
        c = ((mask >> 2) & 1) ? 10 : old;
        if ((mask >> 3) & 1) old = 10;
        d0 = d1 = d2 = d3 = 0;
        for (i = 0; (u32)i < 0x7c; i++) {
            if (a == TB(i, quadAttr0Q0, d0) && b == TB(i, quadAttr0Q1, d1) && c == TB(i, quadAttr0Q2, d2) && old == TB(i, quadAttr0Q3, d3)) goto found;
        }
        i = 0;
    found:
        if (was != 0) {
            *cell = i;
            return TRUE;
        }
        *cell = 10;
        return FALSE;
    }
    return FALSE;
}

extern "C" void Ground_UnlinkUnit(s32 x, s32 z) {
    Ground_SetWalkLinks(x, z, 0);
    Ground_ClearWalkLinks(x, z - 1, 4);
    Ground_ClearWalkLinks(x, z + 1, 1);
    Ground_ClearWalkLinks(x - 1, z, 8);
    Ground_ClearWalkLinks(x + 1, z, 2);
}

extern "C" s32 Ground_GetExitAt(s32 x, s32 z) {
    s32 i = Ground_GetUnitAttr(x, z);
    if (i >= 0x68 && i <= 0x6e) i -= 0x68; else i = -1;
    if (i < 0 || i == gCurCollisionMap->lockedExit) return -1;
    return i;
}

extern "C" s32 Ground_GetExitAtPos(VecFx32 *p) {
    return Ground_GetExitAt(p->x >> 13, p->z >> 13);
}

extern "C" BOOL Ground_LockExit(s32 v) {
    CollisionMap *c = gCurCollisionMap;
    s32 m = 0;
    if (c->lockedExit == -1) {
        c->lockedExit = v;
        return TRUE;
    }
    return m;
}

extern "C" BOOL Ground_IsOnLockedExit(VecFx32 *p) {
    s32 i = Ground_GetUnitAttr(p->x >> 13, p->z >> 13);
    if (i >= 0x68 && i <= 0x6e) i -= 0x68; else i = -1;
    if (i >= 0) {
        if (i == gCurCollisionMap->lockedExit) return TRUE;
        return FALSE;
    }
    return FALSE;
}

extern "C" BOOL Ground_UnlockExit(void) {
    CollisionMap *c = gCurCollisionMap;
    s32 m = 0;
    if (c->lockedExit != -1) {
        c->lockedExit = -1;
        return TRUE;
    }
    return m;
}

extern "C" BOOL Ground_GetFloorBounds(s32 *a, s32 *b, s32 *c, s32 *d) {
    if (CollisionMap_IsBound(0)) {
        struct { FloorBoundsRange xr, yr; } l;
        FloorBoundsStackPad pad;
        s32 y, x;
        l.xr.lo = 0x10;
        l.xr.hi = 0;
        l.yr = l.xr;
        for (y = 0; y < 0x20; y++) {
            for (x = 0; x < 0x20; x++) {
                if (Ground_GetUnitAttr(x, y) != 0x15) {
                    if (x < l.xr.lo) {
                        l.xr.lo = x;
                    } else if (x > l.xr.hi) {
                        l.xr.hi = x;
                    }
                    if (y < l.yr.lo) {
                        l.yr.lo = y;
                    } else if (y > l.yr.hi) {
                        l.yr.hi = y;
                    }
                }
            }
        }
        s32 e = (l.yr.hi << 13) + 0x1000;
        *a = l.xr.lo << 13;
        *b = (l.xr.hi << 13) + 0x2000;
        *c = l.yr.lo << 13;
        if (d) {
            *d = e + 0x1000;
        }
        return TRUE;
    }
    *a = *b = *c = 0;
    if (d) {
        *d = 0;
    }
    return FALSE;
}

CollisionCircle::CollisionCircle() {
    center.x = 0;
    center.y = 0;
    center.z = 0;
    circleRadius = 0;
}

CollisionCircle::CollisionCircle(VecFx32 *pos, s32 radius) {
    setCircle(pos, radius);
}

CollisionCircle::~CollisionCircle() {}

void CollisionCircle::setCircle(VecFx32 *pos, s32 radius) {
    center = *pos;
    circleRadius = radius;
}

BOOL CollisionCircle::containsXZ(VecFx32 *pt) {
    s32 dx = pt->x - center.x;
    s32 dz = pt->z - center.z;
    if ((dx < 0 ? -dx : dx) > circleRadius) {
        return FALSE;
    }
    if ((dz < 0 ? -dz : dz) > circleRadius) {
        return FALSE;
    }
    s32 a = func_01ffcb0c(dz, dz);
    s32 b = func_01ffcb0c(dx, dx);
    if (b + a > func_01ffcb0c(circleRadius, circleRadius)) {
        return FALSE;
    }
    return TRUE;
}

CollisionCylinderX::CollisionCylinderX() {
    cylinderHeight = 0;
}

CollisionCylinderX::CollisionCylinderX(VecFx32 *pos, s32 radius, s32 height) : CollisionCircle(pos, radius) {
    cylinderHeight = height;
}

CollisionCylinderX::~CollisionCylinderX() {}

void CollisionCylinderX::setCylinder(VecFx32 *pos, s32 radius, s32 height) {
    setCircle(pos, radius);
    cylinderHeight = height;
}

BOOL CollisionCylinderX::pushOut(VecFx32 *pos, s32 r) {
    s32 d = Vec_DistXZ(&center, pos);
    s32 lim = r + circleRadius;
    if (d < lim) {
        if (pos->y < center.y + cylinderHeight) {
            VecFx32Ctor v(pos->x - center.x, 0, pos->z - center.z);
            if (Vec_SafeNormalize(&v)) {
                s32 s = r + circleRadius - d;
                pos->x = pos->x + func_01ffcb0c(v.x, s);
                pos->z = pos->z + func_01ffcb0c(v.z, s);
                return TRUE;
            }
        }
    } else if (d <= lim + 0x200 && pos->y < center.y + cylinderHeight) {
        return TRUE;
    }
    return FALSE;
}

BOOL CollisionCylinderX::landOnTop(VecFx32 *out, VecFx32 *a) {
    s32 top = center.y + cylinderHeight;
    if (a->y >= top && out->y < top && containsXZ(out)) {
        out->y = top;
        return TRUE;
    }
    return FALSE;
}

BOOL CollisionCylinderX::clipSegmentTop(VecFx32 *out, VecFx32 *a) {
    s32 top = center.y + cylinderHeight;
    s32 t;
    s32 y, z;
    struct { VecFx32Ctor A, B, D, P; } l;
    l.A = VecFx32Ctor(a->x, a->y, a->z);
    if (l.A.y > top) {
        l.B = VecFx32Ctor(out->x, out->y, out->z);
        if (l.B.y < top) {
            Vec_Sub(&l.D, &l.B, &l.A);
            if (Vec_SafeNormalize(&l.D)) {
                s32 dy = l.D.y;
                if ((dy < 0 ? -dy : dy) >= 4) {
                    t = FX_Div(top - l.A.y, l.D.y);
                    z = l.A.z + func_01ffcb0c(l.D.z, t);
                    y = l.A.y + func_01ffcb0c(l.D.y, t);
                    l.P.x = l.A.x + func_01ffcb0c(l.D.x, t);
                    l.P.y = y;
                    l.P.z = z;
                    if (containsXZ((VecFx32 *)&l.P)) {
                        *out = *(VecFx32 *)&l.P;
                        return TRUE;
                    }
                }
            }
        }
    }
    return FALSE;
}

BOOL CollisionCylinderX::clipSegmentSide(VecFx32 *out, VecFx32 *a) {
    s32 ymax, y1, z1, z2;
    if (!containsXZ(a)) {
        struct { VecFx32Ctor A, B, C, D; u32 pad[6]; } l;
        l.A = VecFx32Ctor(a->x, a->y, a->z);
        l.B = VecFx32Ctor(out->x, out->y, out->z);
        l.C = VecFx32Ctor(center.x, center.y, center.z);
        s32 r = circleRadius;
        s32 h = cylinderHeight;
        Vec_Sub(&l.D, &l.B, &l.A);
        s32 t = func_01ffcb0c(l.D.z, l.D.z);
        s32 q = func_01ffcb0c(l.D.x, l.D.x);
        q += t;
        s32 aq = q < 0 ? -q : q;
        if (aq < 4) {
            return FALSE;
        }
        s32 b = FX_Div(func_01ffcb0c(l.D.x, l.A.x - l.C.x) + func_01ffcb0c(l.D.z, l.A.z - l.C.z), q) << 1;
        s32 zz = func_01ffcb0c(l.A.z - l.C.z, l.A.z - l.C.z);
        s32 xx = func_01ffcb0c(l.A.x - l.C.x, l.A.x - l.C.x);
        s32 c = FX_Div(xx + zz - func_01ffcb0c(r, r), q);
        s32 disc = func_01ffcb0c(b, b) - (c << 2);
        if (disc < 0) {
            return FALSE;
        }
        s32 s = FX_Sqrt(disc);
        if (s < 0) {
            s = -s;
        }
        s32 t1 = -(b + s) >> 1;
        s32 t2 = (s - b) >> 1;
        ymax = l.C.y + h;
        if ((t1 < 0 ? -t1 : t1) < 4 || (t1 >= 0 && t1 <= 0x1000)) {
            z1 = l.A.z + func_01ffcb0c(t1, l.D.z);
            y1 = l.A.y + func_01ffcb0c(t1, l.D.y);
            s32 x1 = l.A.x + func_01ffcb0c(t1, l.D.x);
            if (y1 <= ymax) {
                out->x = x1;
                out->y = y1;
                out->z = z1;
                return TRUE;
            }
        }
        if ((t2 < 0 ? -t2 : t2) < 4 || (t2 >= 0 && t2 <= 0x1000)) {
            z2 = a->z + func_01ffcb0c(t2, l.D.z);
            s32 y2 = a->y + func_01ffcb0c(t2, l.D.y);
            s32 x2 = a->x + func_01ffcb0c(t2, l.D.x);
            if (y2 <= ymax) {
                out->x = x2;
                out->y = y2;
                out->z = z2;
                return TRUE;
            }
        }
    }
    return FALSE;
}




