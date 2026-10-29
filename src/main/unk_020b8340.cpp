#include "types.h"
#include "sys/PrioListNode.h"
#include "sys/ListNode.h"
#include "gfx/BgTransfer.h"
#include "gfx/TexTransfer.h"
#include "gfx/VramTask.h"
#include "gfx/BgVramTask.h"
#include "nitro/io_access.h"
#include "host_note.h"

extern "C" {
void List_Remove(void *list, void *node);
BOOL PrioList_Insert(void *list, void *node);
void PrioList_Init(void *list);
}


extern List sVramQueue2d;
extern List sVramQueueTex;



extern "C" {
u8 BgTransfer_GetPaletteRangeCost(BgTransfer *p);
u8 BgTransfer_GetPaletteCost(BgTransfer *p);
u8 BgTransfer_GetScreenCost(BgTransfer *p);
}



extern "C" {
void VramQueue2d_Dequeue(VramTask *p);
BOOL VramQueue2d_Enqueue(VramTask *p);
}



BOOL BgVramTaskPair::requestCharPair(void *a, void *b, u8 c, u32 d, u32 e, u32 f, u32 g) {
    prepare();
    kind = 8;
    xfer.setChars(a, c, d, d, e);
    cost = xfer.getCharCost();
    xfer2.setChars(b, c, f, f, g);
    cost += xfer2.getCharCost();
    priority = 4;
    if (VramQueue2d_Enqueue(this)) {
        return TRUE;
    }
    clear();
    return FALSE;
}

BOOL BgVramTaskPair::requestCharsAndPalette(void *a, u8 b, u32 c, u32 d, u32 e, void *f, u8 g) {
    prepare();
    kind = 9;
    xfer.setChars(a, b, c, d, e);
    cost = xfer.getCharCost();
    xfer2.setPalette(f, b, g);
    cost += BgTransfer_GetPaletteCost(&xfer2);
    priority = 4;
    if (VramQueue2d_Enqueue(this)) {
        return TRUE;
    }
    clear();
    return FALSE;
}

extern "C" void VramQueueTex_Init(void) {
    PrioList_Init(&sVramQueueTex);
}

BOOL VramTask::enqueueTex(void) {
    return PrioList_Insert(&sVramQueueTex, (PrioListNode *)this);
}

void VramTask::dequeueTex(void) {
    List_Remove(&sVramQueueTex, (PrioListNode *)this);
}

// the queue links the tasks' PrioListNode base (after the vtable pointer): back to the task
static inline VramTask *Unk_020b8340_First(void **l) {
    return static_cast<VramTask *>((PrioListNode *)*l);
}

extern "C" void VramQueueTex_Run(void) {
    VramTask *r5;
    for (r5 = Unk_020b8340_First((void **)&sVramQueueTex); r5 != 0; r5 = Unk_020b8340_First((void **)&sVramQueueTex)) {
        if (IO_R((u16 *)0x4000006) + r5->cost > 0xd4) break;
        BOOL ready = (r5->state == 1) ? TRUE : FALSE;
        if (ready) {
            if (r5->execute() != 0) {
                r5->state = 2;
            }
        }
        if (r5 != 0) r5 = reinterpret_cast<VramTask *>(static_cast<PrioListNode *>(r5));  // (the node base: after the vtable pointer)
        List_Remove(&sVramQueueTex, r5);
    }
    volatile u16 *vc = (volatile u16 *)0x4000006;
    if (IO_R(vc) <= 0xd5) {
        u16 t = IO_R(vc);
    }
}

extern "C" void VramQueue2d_Init(void) {
    PrioList_Init(&sVramQueue2d);
}

extern "C" BOOL VramQueue2d_Enqueue(VramTask *p) {
    void *n = p != 0 ? (void *)static_cast<PrioListNode *>(p) : 0;  // (the node base: where the queue links it)
    return PrioList_Insert(&sVramQueue2d, n);
}

extern "C" void VramQueue2d_Dequeue(VramTask *p) {
    void *n = p != 0 ? (void *)static_cast<PrioListNode *>(p) : 0;  // (the node base: where the queue links it)
    List_Remove(&sVramQueue2d, n);
}

extern "C" void VramQueue2d_Run(void) {
    VramTask *r5;
    for (r5 = Unk_020b8340_First((void **)&sVramQueue2d); r5 != 0; r5 = Unk_020b8340_First((void **)&sVramQueue2d)) {
        if (IO_R((u16 *)0x4000006) + r5->cost > 0x104) {
            // (diagnostics: uploads left for the next frame - a menu cancels its own each frame)
            host_note("vram queue 2d: out of time at line, cost, kind", IO_R((u16 *)0x4000006), r5->cost, r5->kind);
            break;
        }
        BOOL ready = (r5->state == 1) ? TRUE : FALSE;
        if (ready) {
            if (r5->execute() != 0) {
                r5->state = 2;
            }
        }
        if (r5 != 0) r5 = reinterpret_cast<VramTask *>(static_cast<PrioListNode *>(r5));  // (the node base: after the vtable pointer)
        List_Remove(&sVramQueue2d, r5);
    }
}

List sVramQueueTex;
List sVramQueue2d;
