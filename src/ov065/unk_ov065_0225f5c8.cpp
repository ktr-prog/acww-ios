// mwcc-flags: -O4,p -str reuse
#include "types.h"
#include "uptr.h"
#include "sys/OSMessageQueue.h"
#include "net/SockUdpDropCounters.h"
#include "net/SockCoreCommand.h"
#include "net/SockCoreConfig.h"
#include "net/SockCoreSocket.h"
#include "proto/autoload_2/unk_02113088.h"
#include "proto/autoload_2/unk_02114410.h"
#include "proto/ov065/unk_ov065_0225f5c8.h"

// ---- types of the former unk_0225f1a0.cpp part (functions 0x0225f5c8..0x0225fa6c)




struct SockCoreSocket;





// ---- types of the former unk_0225faf4.cpp part (functions 0x0225faf4..0x0225fd18)






typedef SockUdpRecvNode Node;
typedef SockCoreSocket Sess;
typedef SockRecvPipe Ctx;


typedef Unk_ov065_0225faf4_Job Job;


typedef SockCoreSocket Obj;
typedef SockCoreCommand Msg;

// ---- externals

extern "C" {
// own data (other TUs of the overlay)
extern SockCoreConfig *sSockCoreConfig;
extern SockCreateParams sSockTcpParams;

// main module
u32 OS_DisableInterrupts(void);
void OS_RestoreInterrupts(u32 v);
void OS_CreateThread(void *, void *, void *, void *, u32, u32);
void OS_WakeupThreadDirect(void *);
s32 OS_InitMessageQueue(void *, void *, s32);
void MI_CpuFill8(void *, s32, u32);
void MI_CpuCopy8(const void *src, void *dst, u32 n);

// other TUs of the overlay
void SockCore_CommandThreadMain(void *);
s32 SockCore_ExecCommand(Obj *, Msg *);
s32 SockCore_ExecOnRecvSide(Obj *, Msg *);
Msg *SockCore_AllocMsg(void *, Obj *, s32);
s32 SockCore_IsInvalidHandle(Obj *o);
u32 SockCore_Align4(u32);
void SockCore_AddToOpenList(void *);
void IpSoc_Use(void *);
void IpSoc_ShareWithThread(void *);
void IpSoc_Init(void);
void IpSoc_SetUdp(void);
void IpSoc_SetUdpCallback(IpSocketUdpCallback v);
void IpSoc_Bind(u32, u32, u32);
void IpSoc_TcpListen(void);
u32 IpSoc_GetPeer(u16 *, u32 *);
s32 IpSoc_TcpConnect(void);

// this TU
s32 SockCore_OnUdpReceive(u8 *data, u32 len, IpSocket *ip);
s32 SockCore_Bind(Sess *s, u16 a);
s32 SockCore_Connect(Sess *s, u16 a, u32 b);
s32 SockCore_PostConnect(Sess *s);
s32 SockCore_CmdConnect(Job *j);
s32 SockCore_Listen(Obj *);
sptr SockCore_Accept(Obj *, u32 *, u32 *);
s32 SockCore_StartAccept(Obj *, u32 *, u32 *);
s32 SockCore_CmdAccept(Msg *);
sptr SockCore_Create(SockCreateParams *);
s32 SockCore_CmdOpen(Msg *);
Obj *SockCore_Alloc(SockCreateParams *);
u32 SockCore_CalcSize(SockCreateParams *);
u32 SockCore_CalcThreadAreaSize(SockThreadParams *);
u8 *SockCore_InitLayout(Obj *, SockCreateParams *);
u8 *SockCore_CarveBuffer(u8 *, u8 **, u32);
u8 *SockCore_StartCommandThread(u8 *, OSMessageQueue *, OSThread *, OSMutex *, void *, SockThreadParams *);
}

static inline BOOL Unk_ov065_0225f8dc_IsOpen(Obj *o)
{
    BOOL r = FALSE;
    if (o == NULL || (o->flags & 1) == 0) {
    } else {
        r = TRUE;
    }
    return r;
}

static inline BOOL Unk_ov065_0225f8dc_IsIdle(Obj *o)
{
    BOOL r = TRUE;
    s32 s = o->sockType;
    if (s != 0 && s != 4) {
        r = FALSE;
    }
    return r;
}

static inline BOOL IsTcp(Sess *s)
{
    BOOL r = TRUE;
    if (s->sockType != 0 && s->sockType != 4) {
        r = FALSE;
    }
    return r;
}

static inline BOOL IsValid(Sess *s)
{
    BOOL r = FALSE;
    if (s == NULL || !(s->flags & 1)) {
    } else {
        r = TRUE;
    }
    return r;
}

extern "C" {
s32 sSockConnectInProgressError = -0x1a;
SockUdpDropCounters sSockUdpDropCount;

s32 SockCore_OnUdpReceive(u8 *data, u32 len, IpSocket *ip)
{
    Sess *s = static_cast<Sess *>(ip);  // (the IpSocket of a SockCoreSocket)
    Ctx *c = s->recvPipe;
    u32 irq = OS_DisableInterrupts();

    if (c->cap >= c->used + len) {
#ifdef HOST64
        Node *n = (Node *)sSockCoreConfig->alloc(len + __builtin_offsetof(Node, data));  // (the header: 12 bytes on the DS, 16 here)
#else
        Node *n = (Node *)sSockCoreConfig->alloc(len + 12);
#endif
        if (n != NULL) {
            c->used += len;
            n->next = NULL;
            n->len = len;
            n->remotePort = s->remotePort;
            n->remoteAddr = s->remoteAddr;
            MI_CpuCopy8(data, n->data, len);
            if (s->boundPort == 0) {
                s->boundPort = s->localPort;
            }
            s->remotePort = s->boundRemotePort;
            s->remoteAddr = s->boundRemoteAddr;
            if (c->tail != NULL) {
                c->tail->next = n;
            }
            c->tail = n;
            if (c->head == NULL) {
                c->head = n;
            }
        } else {
            sSockUdpDropCount.noMemDrops++;
        }
    } else {
        sSockUdpDropCount.queueFullDrops++;
    }
    OS_WakeupThread(&c->waitQueue);
    OS_RestoreInterrupts(irq);
    return 1;
}

s32 SockCore_Bind(Sess *s, u16 a)
{
    if (SockCore_IsInvalidHandle(s) != 0) {
        return -0x1c;
    }
    if (!IsValid(s)) {
        return -0x27;
    }
    if (s->flags & 2) {
        return -7;
    }
    s->boundPort = a;
    if (s->sockType == 1) {
        return SockCore_PostConnect(s);
    }
    return 0;
}

s32 SockCore_Connect(Sess *s, u16 a, u32 b)
{
    s32 r;
    if (SockCore_IsInvalidHandle(s) != 0 || (s->flags & 8) != 0) {
        return -0x1c;
    }
    if (!IsValid(s)) {
        return -0x27;
    }
    if (IsTcp(s)) {
        if (s->flags & 4) {
            if (s->blocking == 1) {
                return -0x1e;
            }
            return 0;
        }
        if (s->flags & 2) {
            if (s->flags & 0x40) {
                return s->result;
            }
            return sSockConnectInProgressError;
        }
        s->peerPort = a;
        s->peerAddr = b;
        r = SockCore_PostConnect(s);
        if (s->blocking == 1) {
            return r;
        }
        return -0x1a;
    }
    s->peerPort = a;
    s->peerAddr = b;
    return 0;
}

s32 SockCore_PostConnect(Sess *s)
{
    Job *j = (Job *)SockCore_AllocMsg((void *)SockCore_CmdConnect, (Obj *)s, s->blocking);
    if (j == NULL) {
        return -0x21;
    }
    j->localPort = s->boundPort;
    j->remotePort = s->peerPort;
    j->remoteAddr = s->peerAddr;
    s->flags |= 2;
    return SockCore_ExecOnRecvSide((Obj *)s, (Msg *)j);
}

s32 SockCore_CmdConnect(Job *j)
{
    Sess *s = j->sock;
    Ctx *c;
    s32 err = 0;
    c = s->recvPipe;

    OS_LockMutex(&c->mutex);
    IpSoc_Bind(j->localPort, j->remotePort, j->remoteAddr);
    c->pos = err;
    if (j->sockType == 0 || j->sockType == 4) {
        err = IpSoc_TcpConnect();
    }
    OS_UnlockMutex(&c->mutex);
    if (err) {
        s->flags |= 0x40;
        return -0x4c;
    }
    s->flags |= 4;
    return 0;
}

s32 SockCore_Listen(Obj *o)
{
    if (SockCore_IsInvalidHandle(o) != 0) {
        return -0x1c;
    }
    if (!Unk_ov065_0225f8dc_IsOpen(o)) {
        return -0x27;
    }
    if ((o->flags & 2) != 0) {
        return -0x1c;
    }
    if (!Unk_ov065_0225f8dc_IsIdle(o)) {
        return -0x1c;
    }
    if (o->blocking == 1) {
        return 0;
    }
    return -6;
}

// the handle of the accepting socket, or a negative error
sptr SockCore_Accept(Obj *o, u32 *x, u32 *y)
{
    sptr h;
    if (SockCore_IsInvalidHandle(o) != 0) {
        return -0x1c;
    }
    if (!Unk_ov065_0225f8dc_IsOpen(o)) {
        return -0x27;
    }
    if ((o->flags & 2) != 0) {
        return -0x1c;
    }
    if (!Unk_ov065_0225f8dc_IsIdle(o)) {
        return -0x1c;
    }
    if (o->blocking != 1) {
        return -6;
    }
    h = SockCore_Create(&sSockTcpParams);
    if (h >= 0) {
        sptr r = SockCore_Bind((Sess *)h, o->boundPort);
        if (r >= 0) {
            r = SockCore_StartAccept((Obj *)h, x, y);
            if (r >= 0) {
                r = h;
            }
        }
        return r;
    }
    return h;
}

s32 SockCore_StartAccept(Obj *o, u32 *x, u32 *y)
{
    s32 t;
    Msg *m;
    if (SockCore_IsInvalidHandle(o) != 0) {
        return -0x1c;
    }
    if (!Unk_ov065_0225f8dc_IsOpen(o)) {
        return -0x27;
    }
    if ((o->flags & 2) != 0) {
        return -0x1c;
    }
    if (!Unk_ov065_0225f8dc_IsIdle(o)) {
        return -0x1c;
    }
    t = o->blocking;
    if (t != 1) {
        return -6;
    }
    if (o->boundPort == 0) {
        return -0x1c;
    }
    m = SockCore_AllocMsg((void *)SockCore_CmdAccept, o, t);
    m->localPort = o->boundPort;
    m->outPort = x;
    m->outAddr = y;
    o->flags = o->flags | 2;
    return SockCore_ExecOnRecvSide(o, (Msg *)m);
}

s32 SockCore_CmdAccept(Msg *m)
{
    Obj *o = m->sock;
    SockRecvPipe *s = o->recvPipe;
    u16 a;
    u32 b;
    s32 r;
    OS_LockMutex(&s->mutex);
    IpSoc_Bind(m->localPort, 0, 0);
    IpSoc_TcpListen();
    s->pos = 0;
    r = IpSoc_GetPeer(&a, &b);
    *(u16 *)m->outPort = a;
    *(s32 *)m->outAddr = r;
    o->flags = o->flags | 4;
    OS_UnlockMutex(&s->mutex);
    return 0;
}

// the handle: the socket, or a negative error
sptr SockCore_Create(SockCreateParams *p)
{
    Obj *o = SockCore_Alloc(p);
    if (o == NULL) {
        return -0x31;
    }
    SockCore_ExecCommand(o, SockCore_AllocMsg((void *)SockCore_CmdOpen, o, 1));
    return (sptr)o;
}

s32 SockCore_CmdOpen(Msg *m)
{
    Obj *o = (Obj *)m->sock;
    SockSendPipe *sub;
    IpSoc_Use(o);
    sub = o->sendPipe;
    switch (o->sockType) {
    case 0:
    case 4:
        IpSoc_ShareWithThread(&sub->thread);
        IpSoc_Init();
        break;
    case 1:
        IpSoc_Init();
        IpSoc_SetUdp();
        IpSoc_SetUdpCallback(SockCore_OnUdpReceive);
        break;
    case 2:
        IpSoc_SetUdp();
        break;
    case 3:
        break;
    }
    o->flags = 1;
    return 0;
}

Obj *SockCore_Alloc(SockCreateParams *p)
{
    u32 size = SockCore_CalcSize(p);
    u32 irq = OS_DisableInterrupts();
    Obj *o = (Obj *)sSockCoreConfig->alloc(size);
    if (o != NULL) {
        MI_CpuFill8(o, 0, size);
        SockCore_InitLayout(o, p);
        SockCore_AddToOpenList(o);
    }
    OS_RestoreInterrupts(irq);
    return o;
}

u32 SockCore_CalcSize(SockCreateParams *p)
{
    u32 sz = sizeof(SockCoreSocket);
    if (p->rxBufSize != 0) {
        sz += sizeof(SockRecvPipe);
        sz += SockCore_Align4(p->rxBufSize);
        sz += SockCore_Align4(p->rxAuxBufSize);
        sz += SockCore_CalcThreadAreaSize(&p->recvThread);
    }
    if (p->txBufSize != 0) {
        sz += sizeof(SockSendPipe);
        sz += SockCore_Align4(p->txBufSize);
        sz += SockCore_Align4(p->pendingTxBufSize);
        sz += SockCore_Align4(p->sendRingSize);
        sz += SockCore_CalcThreadAreaSize(&p->sendThread);
    }
    return sz;
}

u32 SockCore_CalcThreadAreaSize(SockThreadParams *t)
{
    u32 a = SockCore_Align4(t->msgQueueSize * sizeof(OSMessage));
    return a + SockCore_Align4(t->stackSize);
}

u8 *SockCore_InitLayout(Obj *o, SockCreateParams *p)
{
    SockRecvPipe *s1;
    SockSendPipe *s2;
    u8 *cur;
    o->sockType = p->sockType;
    o->blocking = p->blocking;
    cur = (u8 *)(o + 1);
    if (p->rxBufSize != 0) {
        s1 = (SockRecvPipe *)cur;
        o->recvPipe = s1;
        s1->limit = p->rxConsumeLimit;
        cur = SockCore_StartCommandThread((u8 *)(s1 + 1), &s1->msgQueue, &s1->thread, &s1->mutex, s1, &p->recvThread);
        cur = SockCore_CarveBuffer(cur, &o->rxBuf, p->rxBufSize);
        o->rxBufSize = p->rxBufSize;
        cur = SockCore_CarveBuffer(cur, &o->rxAuxBuf, p->rxAuxBufSize);
        o->rxAuxBufSize = p->rxAuxBufSize;
        s1->cap = p->udpQueueCap;
        s1->waitQueue.head = s1->waitQueue.tail = 0;
    }
    if (p->txBufSize != 0) {
        s2 = (SockSendPipe *)cur;
        o->sendPipe = s2;
        s2->owner = o;
        cur = SockCore_StartCommandThread((u8 *)(s2 + 1), &s2->msgQueue, &s2->thread, &s2->mutex, s2, &p->sendThread);
        cur = SockCore_CarveBuffer(cur, &o->txBuf, p->txBufSize);
        o->txBufSize = p->txBufSize;
        cur = SockCore_CarveBuffer(cur, &o->pendingTx, p->pendingTxBufSize);
        o->pendingTxBufSize = p->pendingTxBufSize;
        cur = SockCore_CarveBuffer(cur, &s2->ring.buf, p->sendRingSize);
        s2->ring.size = p->sendRingSize;
        s2->spaceWaitQueue.head = s2->spaceWaitQueue.tail = 0;
    } else {
        o->sendPipe = sSockDefaultSocket->sendPipe;
    }
    return cur;
}

// the next n bytes from base (NULL if n is 0) to *buf; returns the area after them
u8 *SockCore_CarveBuffer(u8 *base, u8 **buf, u32 n)
{
    u8 *v = base;
    if (n == 0) {
        v = NULL;
    }
    *buf = v;
    return base + SockCore_Align4(n);
}

// the command thread of a pipe: its message array and stack at area (SockCore_CalcThreadAreaSize bytes); returns the
// area after them
u8 *SockCore_StartCommandThread(u8 *area, OSMessageQueue *queue, OSThread *thread, OSMutex *mutex, void *pipe, SockThreadParams *c)
{
    u8 *r = area + SockCore_CalcThreadAreaSize(c);
    OS_InitMessageQueue(queue, area, c->msgQueueSize);
    OS_InitMutex(mutex);
    OS_CreateThread(thread, (void *)SockCore_CommandThreadMain, pipe, r, c->stackSize, c->priority);
    OS_WakeupThreadDirect(thread);
    return r;
}
}

// ---- overlay variables (generated: tools/host_overlay_vars.py) ----
#include "host_overlay_vars.h"

HOST_OVERLAY_VARS(unk_ov065_0225f5c8) {
    HOST_OVERLAY_VAR(sSockConnectInProgressError),
    HOST_OVERLAY_VAR(sSockUdpDropCount),
    HOST_OVERLAY_VARS_END
};
