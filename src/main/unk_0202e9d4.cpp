#include "types.h"
#include "gfx/VecFx32.h"
#include "game/Unk_0202f2ac_V3.h"
#include "game/CollisionVec2.h"
#include "game/CollisionCircle.h"
#include "game/CollisionTriangle.h"
#include "game/CollisionCylinder.h"
#include "game/CollisionEdge.h"
#include "game/CollisionSegment.h"
#include "proto/itcm/unk_01ffcb2c.h"
#include "proto/itcm/unk_01ffcfc0.h"
#include "proto/main/unk_0202e9d4.h"


extern "C" {
s32 FX_Div(s32 a, s32 b);
s32 func_01ffcb0c(s32 a, s32 b);
s32 FX_Sqrt(s32 a);  // (fixed, as defined: a variadic call passes the argument elsewhere on Apple arm64)
s32 VEC_DotProduct(void *a, void *b);
void VEC_Add(void *o, void *a, void *b);
void Vec_RotateY(void *v, s16 a);
void Vec_Sub(void *o, void *a, void *b);
void Vec_CrossCopy(void *o, void *a, void *b, void *c);
s32 Vec_SafeNormalize(void *v);
void Vec_Scale(void *v, s32 s);
s32 Vec_Distance(void *a, void *b);
s32 Vec_DistXZ(void *a, void *b);
}

static inline s32 Unk_0202f2ac_Abs(s32 v) { return v < 0 ? -v : v; }

// ---- 2D line segment with normal (vtable 0x020d8ce4) ----

CollisionVec2 gCollisionVec2Zero(0, 0);

// game/CollisionEdge.h declares hasRoundEnds out of line; in this unit it is inline (no key function: weak vtable)
inline BOOL CollisionEdge::hasRoundEnds() { return TRUE; }


// ---- triangle (vtable 0x020d8ccc) ----




BOOL CollisionCylinder::clipSegmentCaps(VecFx32 *out, VecFx32 *a) {
    s32 t1, t2;
    s32 y, z, y2, z2;
    struct { VecFx32Ctor A, B, D, P1, P2; } l;
    l.A = VecFx32Ctor(a->x, a->y, a->z);
    l.B = VecFx32Ctor(out->x, out->y, out->z);
    Vec_Sub(&l.D, &l.B, &l.A);
    if (Vec_SafeNormalize(&l.D)) {
        s32 dy = l.D.y;
        if ((dy < 0 ? -dy : dy) >= 4) {
            s32 top = center.y + cylinderHeight;
            s32 ay = l.A.y;
            if (ay > top && l.B.y < top) {
                t1 = FX_Div(top - ay, l.D.y);
                z = l.A.z + func_01ffcb0c(l.D.z, t1);
                y = l.A.y + func_01ffcb0c(l.D.y, t1);
                l.P1.x = l.A.x + func_01ffcb0c(l.D.x, t1);
                l.P1.y = y;
                l.P1.z = z;
                if (containsXZ((VecFx32 *)&l.P1)) {
                    *out = *(VecFx32 *)&l.P1;
                    return TRUE;
                }
            } else if (ay < 0 && l.B.y > 0) {
                t2 = FX_Div(-ay, l.D.y);
                z2 = l.A.z + func_01ffcb0c(l.D.z, t2);
                y2 = l.A.y + func_01ffcb0c(l.D.y, t2);
                l.P2.x = l.A.x + func_01ffcb0c(l.D.x, t2);
                l.P2.y = y2;
                l.P2.z = z2;
                if (containsXZ((VecFx32 *)&l.P2)) {
                    *out = *(VecFx32 *)&l.P2;
                    return TRUE;
                }
            }
        }
    }
    return FALSE;
}

BOOL CollisionCylinder::clipSegmentSideBounded(VecFx32 *out, VecFx32 *a) {
    s32 ymin, ymax, z;
    if (!containsXZ(a)) {
        struct { VecFx32Ctor A, B, C, D; u32 pad[6]; } l;
        l.A = VecFx32Ctor(a->x, a->y, a->z);
        l.B = VecFx32Ctor(out->x, out->y, out->z);
        l.C = VecFx32Ctor(center.x, center.y, center.z);
        s32 r = circleRadius;
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
        ymin = center.y;
        ymax = ymin + cylinderHeight;
        s32 y, x;
        if ((t1 < 0 ? -t1 : t1) < 4 || (t1 >= 0 && t1 <= 0x1000)) {
            z = l.A.z + func_01ffcb0c(t1, l.D.z);
            y = l.A.y + func_01ffcb0c(t1, l.D.y);
            x = l.A.x + func_01ffcb0c(t1, l.D.x);
            if (y >= ymin && y <= ymax) {
                out->x = x;
                out->y = y;
                out->z = z;
                return TRUE;
            }
        }
        if ((t2 < 0 ? -t2 : t2) < 4 || (t2 >= 0 && t2 <= 0x1000)) {
            z = l.A.z + func_01ffcb0c(t2, l.D.z);
            y = l.A.y + func_01ffcb0c(t2, l.D.y);
            x = l.A.x + func_01ffcb0c(t2, l.D.x);
            if (y >= ymin && y <= ymax) {
                out->x = x;
                out->y = y;
                out->z = z;
                return TRUE;
            }
        }
    }
    return FALSE;
}

CollisionSegment::CollisionSegment(VecFx32 *a, VecFx32 *b) { set(a, b); }

CollisionSegment::~CollisionSegment() {}

s32 CollisionSegment::calcDir(VecFx32 *out) {
    VecFx32 tmp;
    Vec_Sub(&tmp, &end, &start);
    *out = tmp;
    return Vec_SafeNormalize(out);
}

s32 CollisionSegment::distanceTo(VecFx32 *pt) {
    VecFx32 tmp;
    projectPoint(&tmp, pt);
    return Vec_Distance(&tmp, pt);
}

void CollisionSegment::set(VecFx32 *a, VecFx32 *b) {
    start = *a;
    end = *b;
    calcDir(&dir);
}

s32 CollisionSegment::closestPoint(VecFx32 *out, VecFx32 *pt) {
    VecFx32 tmp;
    projectPoint(&tmp, pt);
    *out = tmp;
    return Vec_Distance(&tmp, pt);
}

void CollisionSegment::projectPoint(VecFx32 *out, VecFx32 *pt) {
    s32 a = VEC_DotProduct(&dir, pt);
    s32 t = -(VEC_DotProduct(&dir, &start) - a);
    s32 y, z;
    z = func_01ffcb0c(dir.z, t);
    z += start.z;
    y = func_01ffcb0c(dir.y, t);
    y += start.y;
    s32 x = func_01ffcb0c(dir.x, t);
    out->x = x + start.x;
    out->y = y;
    out->z = z;
}

BOOL CollisionSegment::isBetweenEnds(VecFx32 *pt) {
    s32 a = VEC_DotProduct(&dir, &start);
    s32 b = VEC_DotProduct(&dir, pt);
    s32 c = VEC_DotProduct(&dir, &end);
    s32 d = VEC_DotProduct(&dir, pt);
    if (func_01ffcb0c(b - a, d - c) > 0) {
        return FALSE;
    }
    return TRUE;
}




CollisionTriangle::CollisionTriangle() {
    normal.x = 0;
    normal.y = 0;
    normal.z = 0;
    offset = 0;
}

CollisionTriangle::~CollisionTriangle() {}

CollisionTriangle::CollisionTriangle(VecFx32 *a, VecFx32 *b, VecFx32 *c, VecFx32 *d) { set((Unk_0202f2ac_V3 *)a, (Unk_0202f2ac_V3 *)b, (Unk_0202f2ac_V3 *)c, (Unk_0202f2ac_V3 *)d); }

BOOL CollisionTriangle::collide(Unk_0202f2ac_V3 *a, Unk_0202f2ac_V3 *b, s32 c) {
    BOOL r = FALSE;
    if (pushBackCrossing(a, b, c)) r = TRUE;
    if (pushOutFace(a, b, c)) r = TRUE;
    if (pushOutEdges(a, b, c)) r = TRUE;
    return r;
}

BOOL CollisionTriangle::pushBackCrossing(Unk_0202f2ac_V3 *a, Unk_0202f2ac_V3 *b, s32 c) {
    Unk_0202f2ac_V3 o;
    BOOL r = FALSE;
    if (distanceTo(a) <= 0 && distanceTo(b) > 0 && intersectSegment(&o, a, b)) {
        a->x = o.x + func_01ffcb0c(c, normal.x);
        a->y = o.y + func_01ffcb0c(c, normal.y);
        a->z = o.z + func_01ffcb0c(c, normal.z);
        r = TRUE;
    }
    return r;
}

BOOL CollisionTriangle::pushOutFace(Unk_0202f2ac_V3 *a, Unk_0202f2ac_V3 *b, s32 c) {
    BOOL result = FALSE;
    s32 d = distanceTo(a);
    if (d >= 0 && d <= c + 0x200) {
        if (d < c) {
            Unk_0202f2ac_V3 t, o;
            Vec_Add(&t, a, &normal);
            if (!intersectLine(&o, a, &t)) {
                goto end;
            }
            if (d < c) {
                s32 e = c - d;
                a->x = a->x + func_01ffcb0c(e, normal.x);
                a->y = a->y + func_01ffcb0c(e, normal.y);
                a->z = a->z + func_01ffcb0c(e, normal.z);
            }
            result = TRUE;
        } else {
            result = TRUE;
        }
    }
end:
    return result;
}

BOOL CollisionTriangle::pushOutEdges(Unk_0202f2ac_V3 *a, Unk_0202f2ac_V3 *b, s32 c) {
    Unk_0202f2ac_V3 w, v;
    BOOL result;
    s32 d;
    if (distanceTo(b) < 0) {
        return FALSE;
    }
    s32 e = distanceTo(a);
    if (e < 0) {
        return FALSE;
    }
    result = FALSE;
    if (e < c) {
        // The loop walks the three edges as an array: mwcc has t0..t2 next to each other on the stack.
        CollisionSegment ts[3] = {
            CollisionSegment((VecFx32 *)&vertex0, (VecFx32 *)&vertex1),
            CollisionSegment((VecFx32 *)&vertex1, (VecFx32 *)&vertex2),
            CollisionSegment((VecFx32 *)&vertex2, (VecFx32 *)&vertex0),
        };
        CollisionSegment &t0 = ts[0];
        CollisionSegment *p = &t0;
        for (; p < &t0 + 3; p++) {
            d = p->closestPoint((VecFx32 *)&w, (VecFx32 *)a);
            if (d < c && p->isBetweenEnds((VecFx32 *)a)) {
                Vec_Sub(&v, a, &w);
                if (Vec_SafeNormalize(&v) == 0) {
                    Unk_0202f2ac_V3 *q = &normal;
                    v = *q;
                    Vec_Scale(&v, c);
                } else {
                    Vec_Scale(&v, c - d);
                }
                VEC_Add(a, &v, a);
                result = TRUE;
                break;
            }
        }
    }
    return result;
}

extern "C" s32 Collision_CalcTriangleNormal(Unk_0202f2ac_V3 *n, Unk_0202f2ac_V3 *p, Unk_0202f2ac_V3 *a, Unk_0202f2ac_V3 *b) {
    Unk_0202f2ac_V3 u, v, t;
    Vec_Sub(&u, a, p);
    Vec_Sub(&v, b, p);
    Vec_CrossCopy(&t, n, &u, &v);
    return Vec_SafeNormalize(n);
}

BOOL CollisionTriangle::set(Unk_0202f2ac_V3 *a, Unk_0202f2ac_V3 *b, Unk_0202f2ac_V3 *c, Unk_0202f2ac_V3 *d) {
    vertex0 = *a;
    vertex1 = *b;
    vertex2 = *c;
    normal = *d;
    offset = calcOffset();
    return TRUE;
}

BOOL CollisionTriangle::containsXZ(Unk_0202f2ac_V3 *p) {
    Unk_0202f2ac_V3 a, b, c;
    Vec_Sub(&a, &vertex0, p);
    Vec_Sub(&b, &vertex1, p);
    Vec_Sub(&c, &vertex2, p);
    s32 r5 = func_01ffcb0c(a.z, b.x) - func_01ffcb0c(a.x, b.z);
    s32 r4 = func_01ffcb0c(b.z, c.x) - func_01ffcb0c(b.x, c.z);
    s32 r0 = func_01ffcb0c(c.z, a.x) - func_01ffcb0c(c.x, a.z);
    if ((r5 >= 0 && r4 >= 0 && r0 >= 0) || (r5 <= 0 && r4 <= 0 && r0 <= 0)) {
        return TRUE;
    }
    return FALSE;
}

s32 CollisionTriangle::calcOffset() {
    s32 z = func_01ffcb0c(normal.z, vertex0.z);
    s32 x = func_01ffcb0c(normal.x, vertex0.x);
    s32 y = func_01ffcb0c(normal.y, vertex0.y);
    return -(z + (x + y));
}

s32 CollisionTriangle::distanceTo(Unk_0202f2ac_V3 *p) {
    s32 d = offset;
    s32 z = func_01ffcb0c(normal.z, p->z);
    s32 x = func_01ffcb0c(normal.x, p->x);
    s32 y = func_01ffcb0c(normal.y, p->y);
    return d + (z + (x + y));
}

BOOL CollisionTriangle::containsXY(Unk_0202f2ac_V3 *p) {
    Unk_0202f2ac_V3 a, b, c;
    Vec_Sub(&a, &vertex0, p);
    Vec_Sub(&b, &vertex1, p);
    Vec_Sub(&c, &vertex2, p);
    s32 r5 = func_01ffcb0c(a.x, b.y) - func_01ffcb0c(a.y, b.x);
    s32 r4 = func_01ffcb0c(b.x, c.y) - func_01ffcb0c(b.y, c.x);
    s32 r0 = func_01ffcb0c(c.x, a.y) - func_01ffcb0c(c.y, a.x);
    if ((r5 >= 0 && r4 >= 0 && r0 >= 0) || (r5 <= 0 && r4 <= 0 && r0 <= 0)) {
        return TRUE;
    }
    return FALSE;
}

BOOL CollisionTriangle::containsYZ(Unk_0202f2ac_V3 *p) {
    Unk_0202f2ac_V3 a, b, c;
    Vec_Sub(&a, &vertex0, p);
    Vec_Sub(&b, &vertex1, p);
    Vec_Sub(&c, &vertex2, p);
    s32 r5 = func_01ffcb0c(a.y, b.z) - func_01ffcb0c(a.z, b.y);
    s32 r4 = func_01ffcb0c(b.y, c.z) - func_01ffcb0c(b.z, c.y);
    s32 r0 = func_01ffcb0c(c.y, a.z) - func_01ffcb0c(c.z, a.y);
    if ((r5 >= 0 && r4 >= 0 && r0 >= 0) || (r5 <= 0 && r4 <= 0 && r0 <= 0)) {
        return TRUE;
    }
    return FALSE;
}

BOOL CollisionTriangle::intersectSegment(Unk_0202f2ac_V3 *out, Unk_0202f2ac_V3 *p, Unk_0202f2ac_V3 *q) {
    s32 a = distanceTo(p);
    if (func_01ffcb0c(a, distanceTo(q)) < 0) {
        return intersectLine(out, p, q);
    }
    return FALSE;
}

BOOL CollisionTriangle::intersectLine(Unk_0202f2ac_V3 *out, Unk_0202f2ac_V3 *p, Unk_0202f2ac_V3 *q) {
    s32 s = distanceTo(p);
    s32 d = s - distanceTo(q);
    if (Unk_0202f2ac_Abs(d) >= 4) {
        Unk_0202f2ac_V3 v;
        s32 t, y, z;
        Vec_Sub(&v, q, p);
        t = FX_Div(s, d);
        z = p->z + func_01ffcb0c(t, v.z);
        y = p->y + func_01ffcb0c(t, v.y);
        out->x = p->x + func_01ffcb0c(t, v.x);
        out->y = y;
        out->z = z;
        if (Unk_0202f2ac_Abs(normal.y) >= 4) {
            if (containsXZ(out)) {
                return TRUE;
            }
        }
        if (Unk_0202f2ac_Abs(normal.x) >= 4) {
            if (containsYZ(out)) {
                return TRUE;
            }
        }
        if (Unk_0202f2ac_Abs(normal.z) >= 4) {
            if (containsXY(out)) {
                return TRUE;
            }
        }
    }
    return FALSE;
}

void CollisionVec2::set(s32 a, s32 b) {
    x = a;
    y = b;
}

CollisionVec2 *CollisionVec2::setFrom(CollisionVec2 *p) {
    set(p->x, p->y);
    return this;
}

void CollisionVec2::setSum(CollisionVec2 *a, CollisionVec2 *b) {
    set(a->x + b->x, a->y + b->y);
}

void CollisionVec2::add(CollisionVec2 *p) {
    x = x + p->x;
    y = y + p->y;
}

void CollisionVec2::setDiff(CollisionVec2 *a, CollisionVec2 *b) {
    set(a->x - b->x, a->y - b->y);
}

CollisionVec2 *CollisionVec2::scale(s32 k) {
    x = func_01ffcb0c(x, k);
    y = func_01ffcb0c(y, k);
    return this;
}

s64 CollisionVec2::distSq(CollisionVec2 *p) {
    CollisionVec2 d;
    d.set(x - p->x, y - p->y);
    s32 b = d.y;
    s32 a = func_01ffcb0c(d.x, d.x);
    s32 c = func_01ffcb0c(b, b);
    return a + c;
}

BOOL CollisionVec2::normalize() {
    s32 r = FX_Sqrt(distSq(&gCollisionVec2Zero));
    if (Unk_0202f2ac_Abs(r) < 4) {
        return FALSE;
    }
    x = FX_Div(x, r);
    y = FX_Div(y, r);
    return TRUE;
}

void CollisionVec2::rotate(s16 a) {
    Unk_0202f2ac_V3 v(y, x);
    Vec_RotateY(&v, a);
    x = v.x;
    y = v.z;
}

void CollisionVec2::setEdgeNormal(CollisionVec2 *a, CollisionVec2 *b) {
    CollisionVec2 d;
    d.setDiff(b, a);
    x = -d.y;
    y = d.x;
    normalize();
}

CollisionEdge::CollisionEdge(CollisionVec2 *a, CollisionVec2 *b) {
    CollisionVec2 n;
    start.set(0, 0);
    end.set(0, 0);
    normal.set(0, 0);
    n.set(0, 0);
    n.setEdgeNormal(a, b);
    set(a, b, &n);
}

CollisionEdge::CollisionEdge(CollisionVec2 *a, CollisionVec2 *b, CollisionVec2 *c) {
    start.set(0, 0);
    end.set(0, 0);
    normal.set(0, 0);
    set(a, b, c);
}


CollisionEdge::~CollisionEdge() {}

void CollisionEdge::set(CollisionVec2 *a, CollisionVec2 *b, CollisionVec2 *c) {
    start.setFrom(a);
    end.setFrom(b);
    normal.setFrom(c);
    offset = calcOffset();
}

s32 CollisionEdge::calcOffset() {
    s32 a = func_01ffcb0c(normal.x, start.x);
    s32 b = func_01ffcb0c(normal.y, start.y);
    return -(a + b);
}

s32 CollisionEdge::distanceTo(CollisionVec2 *p) {
    s32 d = offset;
    s32 a = func_01ffcb0c(normal.x, p->x);
    s32 b = func_01ffcb0c(normal.y, p->y);
    return d + (a + b);
}

BOOL CollisionEdge::intersectLine(CollisionVec2 *out, CollisionVec2 *a, CollisionVec2 *b) {
    CollisionEdge t(a, b);
    s32 det = func_01ffcb0c(normal.x, t.normal.y) - func_01ffcb0c(t.normal.x, normal.y);
    if (Unk_0202f2ac_Abs(det) >= 4) {
        s32 c = t.offset;
        s32 g = offset;
        out->y = FX_Div(func_01ffcb0c(t.normal.x, g) - func_01ffcb0c(normal.x, c), det);
        s32 d = normal.x;
        if (Unk_0202f2ac_Abs(d) >= 4) {
            s32 e = offset;
            s32 m = func_01ffcb0c(normal.y, out->y);
            out->x = FX_Div(-(m + e), d);
            return TRUE;
        } else {
            s32 f = t.normal.x;
            if (Unk_0202f2ac_Abs(f) >= 4) {
                s32 e = t.offset;
                s32 m = func_01ffcb0c(t.normal.y, out->y);
                out->x = FX_Div(-(m + e), f);
                return TRUE;
            }
        }
    }
    return FALSE;
}

BOOL CollisionEdge::intersectSegment(CollisionVec2 *out, CollisionVec2 *a, CollisionVec2 *b) {
    s32 x = distanceTo(a);
    if (func_01ffcb0c(x, distanceTo(b)) < 0) {
        CollisionEdge seg(a, b);
        s32 y = seg.distanceTo(&start);
        if (func_01ffcb0c(y, seg.distanceTo(&end)) < 0) {
            return intersectLine(out, a, b);
        }
    }
    return FALSE;
}

BOOL CollisionEdge::isBetweenEnds(CollisionVec2 *p) {
    CollisionVec2 d;
    d.setDiff(&start, &end);
    if (d.normalize()) {
        CollisionVec2 n(-d.x, -d.y);
        CollisionVec2 e;
        e.setSum(&start, &normal);
        CollisionEdge sa(&start, &e, &d);
        CollisionVec2 f;
        f.setSum(&end, &normal);
        CollisionEdge sb(&end, &f, &n);
        s32 x = sa.distanceTo(p);
        s32 y = sb.distanceTo(p);
        if (x >= 0 && y >= 0) {
            return TRUE;
        }
        if (x <= 0 && y <= 0) {
            return TRUE;
        }
        return FALSE;
    }
    return FALSE;
}

BOOL CollisionEdge::pushOutFace(CollisionVec2 *a, CollisionVec2 *b, s32 c) {
    s32 d = distanceTo(a);
    if (distanceTo(b) >= 0) {
        s32 ad = d < 0 ? -d : d;
        if (ad <= c) {
            if (isBetweenEnds(b) || isBetweenEnds(a)) {
                CollisionVec2 *q = &normal;
                CollisionVec2 t = *q;
                t.scale(c - d);
                a->add(&t);
                return TRUE;
            }
        } else if (d < c + 0x200) {
            return TRUE;
        }
    }
    return FALSE;
}


BOOL CollisionEdge::pushOutEnds(CollisionVec2 *a, CollisionVec2 *b, s32 c) {
    CollisionVec2 *q;
    if (!hasRoundEnds()) {
        return FALSE;
    }
    s32 d = distanceTo(a);
    if (d >= 0 && d < c + 0x200 && distanceTo(b) > 0) {
        if (d < c) {
            if (distanceTo(b) <= 0) {
                return FALSE;
            }
            CollisionVec2 arr[2];
            q = &start;
            arr[0] = *q;
            q = &end;
            CollisionVec2 *dd = &arr[1];
            *dd = *q;
            for (CollisionVec2 *p = arr; p < arr + 2; p++) {
                s32 dist = FX_Sqrt(p->distSq(a));
                if (dist < c) {
                    CollisionVec2 t;
                    t.setDiff(a, p);
                    if (!t.normalize()) {
                        q = &normal;
                        t.set(q->x, q->y);
                    } else {
                        c -= dist;
                    }
                    t.x = func_01ffcb0c(t.x, c);
                    t.y = func_01ffcb0c(t.y, c);
                    a->x += t.x;
                    a->y += t.y;
                    return TRUE;
                }
            }
        } else {
            return TRUE;
        }
    }
    return FALSE;
}

CollisionVec2::CollisionVec2(s32 a, s32 b) {
    x = a;
    y = b;
}



BOOL CollisionEdge::pushBackCrossing(CollisionVec2 *a, CollisionVec2 *b, s32 c) {
    if (distanceTo(b) > 0) {
        CollisionVec2 v(0, 0);
        if (intersectSegment(&v, a, b)) {
            s32 d = c - distanceTo(a);
            if (d < 0) {
                d = -d;
            }
            CollisionVec2 *q = &normal;
            CollisionVec2 t = *q;
            t.scale(d);
            a->add(&t);
            return TRUE;
        }
    }
    return FALSE;
}


