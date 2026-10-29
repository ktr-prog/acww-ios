#include "ds_div.h"
#include "gfx/SplReal.h"
#include "uptr.h"
// mwcc-flags: -nothumb -O4,p
// G013a: autoload_2 0x020f92d4-0x020fa0f4 (9 functions). mwcc 1.2/base, C++, ARM, -O4,p. PARTIAL: plain func_ names, nothing defined but the functions.
// SPL-style particle manager (continued from G011b): resource file loader (func_020f92d4: counts + texture table), manager creation (SPL_Init),
// file-read callbacks (f9620/f9658), emitter draw dispatch (f969c, f9714, f97d0) and the per-emitter simulation step (spl_calc).
#include "types.h"
#include "gfx/V3.h"
#include "gfx/SplManager.h"
#include "gfx/SplRes.h"
#include "gfx/SplEmitterViews.h"
#include "gfx/VecFx32.h"
#include "gfx/SplTex.h"
#include "gfx/SplViews.h"
#include "gfx/SplPt.h"
#include "gfx/SplNode.h"
#include "proto/autoload_2/unk_020f92d4.h"
#include "proto/autoload_2/unk_02101048.h"
#include "proto/autoload_2/unk_020fa398.h"
#include "proto/autoload_2/unk_020fa39c.h"
#include "proto/autoload_2/unk_021104ac.h"







struct Res {
#ifdef HOST64
    // the 64-bit layout (tools/host_view_layout.py: the members at their places in SplRealRes)
    HdrW * p0;  // DS +0x0
    u8 p4[0x20];  // DS +0x4 [0x10]
    Blk14 * p14;  // DS +0x14
#else
    HdrW *p0;
    u8 p4[0x10];
    Blk14 *p14;
#endif
};




extern "C" {
void *MI_CpuFill8(void *p, u32 v, u32 n);
void spl_push_front(void *list, void *e);
void spl_gen_ptcl(void *e, void *l);
void sDrawChild(Pm *m, SplPosFn a);
void sDrawParent(Pm *m, SplPosFn a);
void spl_draw_chld_bb(Mc *m, Node *n, SplPosFn a);
void spl_draw_chld_dbb(Mc *m, Node *n, SplPosFn a);
void spl_draw_bb(Mc *m, Node *n, SplPosFn a);
void spl_draw_dbb(Mc *m, Node *n, SplPosFn a);
}













typedef u16 (*PosCb)(VecFx32 *, VecFx32);  // (SplPosFn, an angle: was s32, the u16 result's upper bits undefined)
typedef void (*SetFn)(s32, s32, s32, s32);
typedef void (*MkFn)(s32, s32, Mt *);

extern "C" {
extern s16 data_02135f44[];
extern MkFn data_0213bb9c[];
extern SetFn data_0213bb94[];
void MTX_Scale43_(Mt *m, s32 x, s32 y, s32 z);
void MTX_RotX43_(Mt *m, s32 s, s32 c);
void MTX_Concat43(Mt *a, Mt *b, Mt *ab);
}


static inline s32 FxMul(s32 a, s32 b) {
    return (s32)(((s64)a * b + 0x800) >> 12);
}
extern "C" {
void spl_draw_chld_pol(Mg2 *m, Nd *p, PosCb cb);
void spl_draw_pol(Mg2 *m, Nd *p, PosCb cb);
}



typedef void (*TexFn)(void *);
typedef void (*DrawFn)(Mc *, Node *, SplPosFn);

















typedef void (*FldFn)(SplPt *, RU *, u32);


extern "C" {
void spl_scl_in_out(SplPt *, RU *, u32);
void spl_clr_in_out(SplPt *, RU *, u32);
void spl_alp_in_out(SplPt *, RU *, u32);
void spl_tex_ptn_anm(SplPt *, RU *, u32);
void spl_chld_scl_out(SplPt *, RU *, u32);
void spl_chld_alp_out(SplPt *, RU *, u32);
void spl_gen_child(SplPt *, EU *, void *);
void spl_gen_ptcl(void *, void *);
u32 func_02133150x(void);
SplPt *spl_del(void *, SplPt *);
void spl_push_front(void *, void *);
}

typedef void (*IFn)(void *, SplPt *, s32 *, EU *);

extern "C" void spl_calc(MU *m, EU *e) {
    SplPt *next;
    RU *res = e->res;
    HdrP *hp = res->p0;
    B14 *b14 = res->p14;
    HdrU h = *(HdrU *)hp;
    s32 drag = hp->c66 + 384;
    SplPt *p;
    s32 i;
    s32 n = 0;
    s32 cnt = res->h1c;
    FEnt arr[4];
    FEnt arr2[4];
    u8 fr[2];
    s32 acc[3];
    if (e->cb) e->cb(e, 0);
    if (hp->h56 == 0 || e->h56 < hp->h56) {
        if (DS_MOD(e->h56, e->c104) == 0 && !e->fl.b0 && !e->fl.b1 && e->fl.b4) spl_gen_ptcl(e, &m->freelist);
    }
    if (h.b8) {
        arr[n].fn = spl_scl_in_out;
        arr[n++].arg = res->p4->b0;

    }
    if (h.b9) {
        B8 *q = res->p8;
        if (q->b0 == 0) {
            arr[n].fn = spl_clr_in_out;
            arr[n++].arg = q->b1;

        }
    }
    if (h.b10) {
        arr[n].fn = spl_alp_in_out;
        arr[n++].arg = res->pc->b8;

    }
    if (h.b11) {
        B10 *q = res->p10;
        if (q->b16 == 0) {
            arr[n].fn = spl_tex_ptn_anm;
            arr[n++].arg = q->b17;

        }
    }
    p = e->l8;
    if (p != 0) {
        do {
            next = p->next;
            fr[0] = (p->h42 * p->h38) >> 8;
            fr[1] = p->c45 + ((p->h40 * p->h38) >> 8);
            for (i = 0; i < n; i++) arr[i].fn(p, res, fr[arr[i].arg]);
            acc[0] = acc[1] = acc[2] = 0;
            if (h.b15) {
                p->v56 = e->v32;
            }
            for (i = 0; i < cnt; i++) ((IFn)res->p18[i].fn)(res->p18[i].p4, p, acc, e);
            p->h32 = p->h32 + p->s34;
            p->w20 = (p->w20 * drag) >> 9;
            p->w24 = (p->w24 * drag) >> 9;
            p->w28 = (p->w28 * drag) >> 9;
            p->w20 += acc[0];
            p->w24 += acc[1];
            p->w28 += acc[2];
            p->w8 = p->w8 + (p->w20 + e->w44);
            p->w12 = p->w12 + (p->w24 + e->w48);
            p->w16 = p->w16 + (p->w28 + e->w52);
            if (h.b16) {
                s32 d = (p->h38 << 12) - (FxMul(p->h36 << 12, b14->c13 << 12) >> 8);
                if (d >= 0) {
                    if (DS_MOD((d >> 12), b14->c14) == 0) spl_gen_child(p, e, &m->freelist);
                }
            }
            if (((HdrU *)e->res->p0)->b30) {
                p->id46 = (u16)m->b18;
            } else {
                p->id46 = (u16)m->b12;
                m->b12 = m->b12 + 1;
                if (m->b12 > m->b6) m->b12 = m->b0;
            }
            p->h38 = p->h38 + 1;
            if (p->h38 > p->h36) spl_push_front(&m->freelist, spl_del(&e->l8, p));
            p = next;
        } while (p != 0);
    }
    if (h.b16) {
        n = 0;
        if (b14->b1) {
            arr2[n].fn = spl_chld_scl_out;
            arr2[n].arg = n;
            n++;
        }
        if (b14->b2) {
            arr2[n].fn = spl_chld_alp_out;
            arr2[n].arg = 0;
            n++;
        }
        p = e->l16;
        if (!b14->b0) cnt = 0;
        if (p != 0) {
            do {
                next = p->next;
                fr[0] = DS_DIV((p->h38 << 8), p->h36);
                for (i = 0; i < n; i++) arr2[i].fn(p, res, fr[0]);
                acc[0] = acc[1] = acc[2] = 0;
                if (b14->b5) {
                    p->v56 = e->v32;
                }
                for (i = 0; i < cnt; i++) ((IFn)res->p18[i].fn)(res->p18[i].p4, p, acc, e);
                p->h32 = p->h32 + p->s34;
                p->w20 = (p->w20 * drag) >> 9;
                p->w24 = (p->w24 * drag) >> 9;
                p->w28 = (p->w28 * drag) >> 9;
                p->w20 += acc[0];
                p->w24 += acc[1];
                p->w28 += acc[2];
                p->w8 = p->w8 + (p->w20 + e->w44);
                p->w12 = p->w12 + (p->w24 + e->w48);
                p->w16 = p->w16 + (p->w28 + e->w52);
                if (((HdrU *)e->res->p0)->b31) {
                    p->id46 = (u16)m->b18;
                } else {
                    p->id46 = (u16)m->b12;
                    m->b12 = m->b12 + 1;
                    if (m->b12 > m->b6) m->b12 = m->b0;
                }
                p->h38 = p->h38 + 1;
                if (p->h38 > p->h36) spl_push_front(&m->freelist, spl_del(&e->l16, p));
                p = next;
            } while (p != 0);
        }
    }
    e->h56 = e->h56 + 1;
    if (e->cb) e->cb(e, 1);
}

extern "C" void sDrawParent(Pm *mp, SplPosFn a) {
    Mc *m = (Mc *)mp;
    Em *cur = m->cur;
    HdrW *h = cur->res->p0;
    TexFn tf;
    Node *n;
    DrawFn fn = 0;
    spl_set_tex((u8 *)m->tex + h->c43 * SPL_TEX_SIZE);
    switch (h->k4) {
        case 0:
            fn = spl_draw_bb;
            break;
        case 1:
            fn = spl_draw_dbb;
            break;
        case 2:
            fn = (DrawFn)spl_draw_pol;
            break;
    }
    tf = h->b11 ? spl_set_tex : spl_set_tex_dummy;
    n = cur->l8;
    if (n == 0) return;
    do {
        tf((u8 *)m->tex + n->c2c * SPL_TEX_SIZE);
        fn(m, n, a);
        n = n->next;
    } while (n != 0);
}

extern "C" void sDrawChild(Pm *mp, SplPosFn a) {
    Mc *m = (Mc *)mp;
    Em *cur = m->cur;
    Res *res = cur->res;
    DrawFn fn = 0;
    Node *n;
    if (!res->p0->b16) return;
    spl_set_tex((u8 *)m->tex + res->p14->c15 * SPL_TEX_SIZE);
    switch (res->p14->k7) {
        case 0:
            fn = spl_draw_chld_bb;
            break;
        case 1:
            fn = spl_draw_chld_dbb;
            break;
        case 2:
            fn = (DrawFn)spl_draw_chld_pol;
            break;
    }
    n = cur->l16;
    if (n == 0) return;
    do {
        fn(m, n, a);
        n = n->next;
    } while (n != 0);
}

extern "C" void spl_draw(Pm *m, SplPosFn a) {
    Mc *mm = (Mc *)m;
    HdrW *ri = mm->cur->res->p0;
    if (ri->b21) {
        sDrawChild(m, a);
        if (ri->b22) return;
        sDrawParent(m, a);
    } else {
        if (!ri->b22) sDrawParent(m, a);
        sDrawChild(m, a);
    }
}

extern "C" void func_020f9690(void *e, void *l) {
    spl_gen_ptcl(e, l);
}

extern "C" u32 sAllocTex(u32 a, u32 b) {
    u32 r = data_0213bc10(a, b, 0);
    return (r & 0xffff) << 3;
}

extern "C" u32 sAllocTexPalette(u32 a, u32 b) {
    u32 r = data_0213bc18(a, b, 0);
    return (r & 0xffff) << 3;
}

extern "C" PmNew *SPL_Init(void *(*alloc)(u32), s32 n1, s32 n2, u32 a3, u16 a4, u16 a5) {
    PmNew *m = (PmNew *)alloc(SPL_MANAGER_SIZE);
    s32 i;
    u8 *p;
    MI_CpuFill8(m, 0, SPL_MANAGER_SIZE);
    m->h40 = n1;
    m->h42 = n2;
    m->b0 = a4;
    m->b6 = a5;
    m->b12 = m->b0;
    m->b18 = a3;
    m->pad24 = 0;  // (DS offset 0x2f: the top byte of the bitfield word; the 64-bit layout has it elsewhere)
    m->alloc = alloc;
    m->w8 = 0;
    m->w16 = 0;
    m->w24 = 0;
    m->w4 = 0;
    m->w12 = 0;
    m->w20 = 0;
    m->w48 = 0;
    p = (u8 *)alloc(n1 * SPL_EMITTER_SIZE);
    MI_CpuFill8(p, 0, n1 * SPL_EMITTER_SIZE);
    for (i = 0; i < n1; i++) {
        spl_push_front(&m->w12, p);
        p += SPL_EMITTER_SIZE;
    }
    p = (u8 *)alloc(n2 * SPL_PARTICLE_SIZE);
    MI_CpuFill8(p, 0, n2 * SPL_PARTICLE_SIZE);
    for (i = 0; i < n2; i++) {
        spl_push_front(&m->w20, p);
        p += SPL_PARTICLE_SIZE;
    }
    m->w28 = 0;
    m->w32 = 0;
    m->h38 = 0;
    m->h36 = m->h38;
    return m;
}

extern "C" void func_020f92d4(Pm *self, u8 *base) {
    s32 i;
    u32 off = 32;
    self->h24 = *(u16 *)(base + 8);
    self->h26 = *(u16 *)(base + 10);
    for (i = 0; i < self->h24; i++) {
        HdrBits h = *(HdrBits *)(base + off);
        off += 0x54;
        if (h.b8) off += 12;
        if (h.b9) off += 12;
        if (h.b10) off += 8;
        if (h.b11) off += 12;
        if (h.b16) off += 20;
        {
            u32 cnt = h.b24 + h.b25 + h.b26 + h.b27 + h.b28 + h.b29;
            if (cnt != 0) {
                if (h.b24) off += 8;
                if (h.b25) off += 8;
                if (h.b26) off += 16;
                if (h.b27) off += 4;
                if (h.b28) off += 8;
                if (h.b29) off += 16;
            }
        }
    }
    self->tex = (TexEnt *)self->alloc(self->h26 * SPL_TEX_SIZE);
    MI_CpuFill8(self->tex, 0, self->h26 * SPL_TEX_SIZE);
    {
        for (i = 0; i < self->h26; i++) {
            TexEnt *t = &self->tex[i];
            Hdr20 *e = (Hdr20 *)(base + off);
            t->e = e;
            t->h16 = 1 << (((e->w4 << 24) >> 28) + 3);
            t->h18 = 1 << (((e->w4 << 20) >> 28) + 3);
            t->w12 = e->w4;
            off += e->w1c;
        }
    }
}

#ifdef HOST64
static_assert(__builtin_offsetof(Res, p0) == 0x0, "Res.p0");
static_assert(__builtin_offsetof(Res, p4) == 0x8, "Res.p4");
static_assert(__builtin_offsetof(Res, p14) == 0x28, "Res.p14");
#endif

#ifdef HOST64
// RU (include/gfx/SplManager.h) views SplRealRes
static_assert(__builtin_offsetof(RU, p0) == 0x0, "RU.p0");
static_assert(__builtin_offsetof(RU, p4) == 0x8, "RU.p4");
static_assert(__builtin_offsetof(RU, p8) == 0x10, "RU.p8");
static_assert(__builtin_offsetof(RU, pc) == 0x18, "RU.pc");
static_assert(__builtin_offsetof(RU, p10) == 0x20, "RU.p10");
static_assert(__builtin_offsetof(RU, p14) == 0x28, "RU.p14");
static_assert(__builtin_offsetof(RU, p18) == 0x30, "RU.p18");
static_assert(__builtin_offsetof(RU, h1c) == 0x38, "RU.h1c");
#endif

#ifdef HOST64
// Mc (include/gfx/SplViews.h) views SplRealManager
static_assert(__builtin_offsetof(Mc, p0) == 0x0, "Mc.p0");
static_assert(__builtin_offsetof(Mc, tex) == 0x40, "Mc.tex");
static_assert(__builtin_offsetof(Mc, p24) == 0x48, "Mc.p24");
static_assert(__builtin_offsetof(Mc, cur) == 0x58, "Mc.cur");
#endif

#ifdef HOST64
// Mg2 (include/gfx/SplViews.h) views SplRealManager
static_assert(__builtin_offsetof(Mg2, p0) == 0x0, "Mg2.p0");
static_assert(__builtin_offsetof(Mg2, w48) == 0x54, "Mg2.w48");
static_assert(__builtin_offsetof(Mg2, cur) == 0x58, "Mg2.cur");
static_assert(__builtin_offsetof(Mg2, mt) == 0x60, "Mg2.mt");
#endif

#ifdef HOST64
// MU (include/gfx/SplViews.h) views SplRealManager
static_assert(__builtin_offsetof(MU, p0) == 0x0, "MU.p0");
static_assert(__builtin_offsetof(MU, freelist) == 0x28, "MU.freelist");
static_assert(__builtin_offsetof(MU, p18) == 0x30, "MU.p18");
#endif

#ifdef HOST64
// ItemU (include/gfx/SplViews.h) views SplRealField
static_assert(__builtin_offsetof(ItemU, fn) == 0x0, "ItemU.fn");
static_assert(__builtin_offsetof(ItemU, p4) == 0x8, "ItemU.p4");
#endif

#ifdef HOST64
// Node (include/gfx/SplNode.h) views SplRealParticle
static_assert(__builtin_offsetof(Node, next) == 0x0, "Node.next");
static_assert(__builtin_offsetof(Node, p4) == 0x8, "Node.p4");
static_assert(__builtin_offsetof(Node, c2c) == 0x34, "Node.c2c");
#endif

#ifdef HOST64
// Nd (include/gfx/SplNode.h) views SplRealParticle
static_assert(__builtin_offsetof(Nd, p0) == 0x0, "Nd.p0");
static_assert(__builtin_offsetof(Nd, w8) == 0x10, "Nd.w8");
static_assert(__builtin_offsetof(Nd, w12) == 0x14, "Nd.w12");
static_assert(__builtin_offsetof(Nd, w16) == 0x18, "Nd.w16");
static_assert(__builtin_offsetof(Nd, p14) == 0x1c, "Nd.p14");
static_assert(__builtin_offsetof(Nd, h32) == 0x28, "Nd.h32");
static_assert(__builtin_offsetof(Nd, p22) == 0x2a, "Nd.p22");
static_assert(__builtin_offsetof(Nd, c46) == 0x36, "Nd.c46");
static_assert(__builtin_offsetof(Nd, w48) == 0x38, "Nd.w48");
static_assert(__builtin_offsetof(Nd, s52) == 0x3c, "Nd.s52");
static_assert(__builtin_offsetof(Nd, h54) == 0x3e, "Nd.h54");
static_assert(__builtin_offsetof(Nd, w56) == 0x40, "Nd.w56");
static_assert(__builtin_offsetof(Nd, w60) == 0x44, "Nd.w60");
static_assert(__builtin_offsetof(Nd, w64) == 0x48, "Nd.w64");
#endif

#ifdef HOST64
// SplPt (include/gfx/SplPt.h) views SplRealParticle
static_assert(__builtin_offsetof(SplPt, next) == 0x0, "SplPt.next");
static_assert(__builtin_offsetof(SplPt, p4) == 0x8, "SplPt.p4");
static_assert(__builtin_offsetof(SplPt, w8) == 0x10, "SplPt.w8");
static_assert(__builtin_offsetof(SplPt, w12) == 0x14, "SplPt.w12");
static_assert(__builtin_offsetof(SplPt, w16) == 0x18, "SplPt.w16");
static_assert(__builtin_offsetof(SplPt, w20) == 0x1c, "SplPt.w20");
static_assert(__builtin_offsetof(SplPt, w24) == 0x20, "SplPt.w24");
static_assert(__builtin_offsetof(SplPt, w28) == 0x24, "SplPt.w28");
static_assert(__builtin_offsetof(SplPt, h32) == 0x28, "SplPt.h32");
static_assert(__builtin_offsetof(SplPt, s34) == 0x2a, "SplPt.s34");
static_assert(__builtin_offsetof(SplPt, h36) == 0x2c, "SplPt.h36");
static_assert(__builtin_offsetof(SplPt, h38) == 0x2e, "SplPt.h38");
static_assert(__builtin_offsetof(SplPt, h40) == 0x30, "SplPt.h40");
static_assert(__builtin_offsetof(SplPt, h42) == 0x32, "SplPt.h42");
static_assert(__builtin_offsetof(SplPt, p44) == 0x34, "SplPt.p44");
static_assert(__builtin_offsetof(SplPt, c45) == 0x35, "SplPt.c45");
static_assert(__builtin_offsetof(SplPt, p48) == 0x38, "SplPt.p48");
static_assert(__builtin_offsetof(SplPt, v56) == 0x40, "SplPt.v56");
#endif

#ifdef HOST64
// Em (include/gfx/SplEmitterViews.h) views SplRealEmitter
static_assert(__builtin_offsetof(Em, p0) == 0x0, "Em.p0");
static_assert(__builtin_offsetof(Em, l8) == 0x10, "Em.l8");
static_assert(__builtin_offsetof(Em, pc) == 0x18, "Em.pc");
static_assert(__builtin_offsetof(Em, l16) == 0x20, "Em.l16");
static_assert(__builtin_offsetof(Em, p14) == 0x28, "Em.p14");
static_assert(__builtin_offsetof(Em, res) == 0x30, "Em.res");
#endif

#ifdef HOST64
// EmI (include/gfx/SplEmitterViews.h) views SplRealEmitter
static_assert(__builtin_offsetof(EmI, w0) == 0x0, "EmI.w0");
static_assert(__builtin_offsetof(EmI, w4) == 0x8, "EmI.w4");
static_assert(__builtin_offsetof(EmI, w8) == 0x10, "EmI.w8");
static_assert(__builtin_offsetof(EmI, w12) == 0x18, "EmI.w12");
static_assert(__builtin_offsetof(EmI, w16) == 0x20, "EmI.w16");
static_assert(__builtin_offsetof(EmI, w20) == 0x28, "EmI.w20");
static_assert(__builtin_offsetof(EmI, res) == 0x30, "EmI.res");
static_assert(__builtin_offsetof(EmI, w28) == 0x38, "EmI.w28");
static_assert(__builtin_offsetof(EmI, w32) == 0x3c, "EmI.w32");
static_assert(__builtin_offsetof(EmI, w36) == 0x40, "EmI.w36");
static_assert(__builtin_offsetof(EmI, w40) == 0x44, "EmI.w40");
static_assert(__builtin_offsetof(EmI, w44) == 0x48, "EmI.w44");
static_assert(__builtin_offsetof(EmI, w48) == 0x4c, "EmI.w48");
static_assert(__builtin_offsetof(EmI, w52) == 0x50, "EmI.w52");
static_assert(__builtin_offsetof(EmI, h56) == 0x54, "EmI.h56");
static_assert(__builtin_offsetof(EmI, h58) == 0x56, "EmI.h58");
static_assert(__builtin_offsetof(EmI, h60) == 0x58, "EmI.h60");
static_assert(__builtin_offsetof(EmI, h66) == 0x5e, "EmI.h66");
static_assert(__builtin_offsetof(EmI, w68) == 0x60, "EmI.w68");
static_assert(__builtin_offsetof(EmI, w72) == 0x64, "EmI.w72");
static_assert(__builtin_offsetof(EmI, w76) == 0x68, "EmI.w76");
static_assert(__builtin_offsetof(EmI, w80) == 0x6c, "EmI.w80");
static_assert(__builtin_offsetof(EmI, w84) == 0x70, "EmI.w84");
static_assert(__builtin_offsetof(EmI, h88) == 0x74, "EmI.h88");
static_assert(__builtin_offsetof(EmI, h90) == 0x76, "EmI.h90");
static_assert(__builtin_offsetof(EmI, w92) == 0x78, "EmI.w92");
static_assert(__builtin_offsetof(EmI, s96) == 0x7c, "EmI.s96");
static_assert(__builtin_offsetof(EmI, s98) == 0x7e, "EmI.s98");
static_assert(__builtin_offsetof(EmI, s100) == 0x80, "EmI.s100");
static_assert(__builtin_offsetof(EmI, s102) == 0x82, "EmI.s102");
static_assert(__builtin_offsetof(EmI, p108) == 0x88, "EmI.p108");
static_assert(__builtin_offsetof(EmI, w120) == 0x98, "EmI.w120");
static_assert(__builtin_offsetof(EmI, w124) == 0xa0, "EmI.w124");
static_assert(__builtin_offsetof(EmI, w128) == 0xa8, "EmI.w128");
#endif

#ifdef HOST64
// TexEnt (include/gfx/SplTex.h) views SplRealTex
static_assert(__builtin_offsetof(TexEnt, e) == 0x0, "TexEnt.e");
static_assert(__builtin_offsetof(TexEnt, w4) == 0x8, "TexEnt.w4");
static_assert(__builtin_offsetof(TexEnt, w8) == 0xc, "TexEnt.w8");
static_assert(__builtin_offsetof(TexEnt, w12) == 0x10, "TexEnt.w12");
static_assert(__builtin_offsetof(TexEnt, h16) == 0x14, "TexEnt.h16");
static_assert(__builtin_offsetof(TexEnt, h18) == 0x16, "TexEnt.h18");
#endif

#ifdef HOST64
// Pm (include/gfx/SplManager.h) views SplRealManager
static_assert(__builtin_offsetof(Pm, alloc) == 0x0, "Pm.alloc");
static_assert(__builtin_offsetof(Pm, p4) == 0x8, "Pm.p4");
static_assert(__builtin_offsetof(Pm, tex) == 0x40, "Pm.tex");
static_assert(__builtin_offsetof(Pm, h24) == 0x48, "Pm.h24");
static_assert(__builtin_offsetof(Pm, h26) == 0x4a, "Pm.h26");
#endif

#ifdef HOST64
// PmNew (include/gfx/SplManager.h) views SplRealManager
static_assert(__builtin_offsetof(PmNew, alloc) == 0x0, "PmNew.alloc");
static_assert(__builtin_offsetof(PmNew, w4) == 0x8, "PmNew.w4");
static_assert(__builtin_offsetof(PmNew, w8) == 0x10, "PmNew.w8");
static_assert(__builtin_offsetof(PmNew, w12) == 0x18, "PmNew.w12");
static_assert(__builtin_offsetof(PmNew, w16) == 0x20, "PmNew.w16");
static_assert(__builtin_offsetof(PmNew, w20) == 0x28, "PmNew.w20");
static_assert(__builtin_offsetof(PmNew, w24) == 0x30, "PmNew.w24");
static_assert(__builtin_offsetof(PmNew, w28) == 0x38, "PmNew.w28");
static_assert(__builtin_offsetof(PmNew, w32) == 0x40, "PmNew.w32");
static_assert(__builtin_offsetof(PmNew, h36) == 0x48, "PmNew.h36");
static_assert(__builtin_offsetof(PmNew, h38) == 0x4a, "PmNew.h38");
static_assert(__builtin_offsetof(PmNew, h40) == 0x4c, "PmNew.h40");
static_assert(__builtin_offsetof(PmNew, h42) == 0x4e, "PmNew.h42");
static_assert(__builtin_offsetof(PmNew, w48) == 0x54, "PmNew.w48");
#endif

#ifdef HOST64
// EU (include/gfx/SplEmitterViews.h) views SplRealEmitter
static_assert(__builtin_offsetof(EU, p0) == 0x0, "EU.p0");
static_assert(__builtin_offsetof(EU, l8) == 0x10, "EU.l8");
static_assert(__builtin_offsetof(EU, pc) == 0x18, "EU.pc");
static_assert(__builtin_offsetof(EU, l16) == 0x20, "EU.l16");
static_assert(__builtin_offsetof(EU, p14) == 0x28, "EU.p14");
static_assert(__builtin_offsetof(EU, res) == 0x30, "EU.res");
static_assert(__builtin_offsetof(EU, fl) == 0x38, "EU.fl");
static_assert(__builtin_offsetof(EU, v32) == 0x3c, "EU.v32");
static_assert(__builtin_offsetof(EU, w44) == 0x48, "EU.w44");
static_assert(__builtin_offsetof(EU, w48) == 0x4c, "EU.w48");
static_assert(__builtin_offsetof(EU, w52) == 0x50, "EU.w52");
static_assert(__builtin_offsetof(EU, h56) == 0x54, "EU.h56");
static_assert(__builtin_offsetof(EU, p3a) == 0x56, "EU.p3a");
static_assert(__builtin_offsetof(EU, c104) == 0x84, "EU.c104");
static_assert(__builtin_offsetof(EU, p69) == 0x85, "EU.p69");
static_assert(__builtin_offsetof(EU, cb) == 0x98, "EU.cb");
#endif
