#include "uptr.h"
// mwcc-flags: -O4,p -str reuse
#include "types.h"
#include "net/SockUdpDropCounters.h"
#include "net/SockHostEnt.h"
#include "net/IpStackConfig.h"
#include "net/SockCoreSocket.h"
#include "net/SockCoreCommand.h"
#include "net/SockAddrIn.h"
#include "net/SockStartupConfig.h"
#include "net/SockCoreConfig.h"
#include "proto/autoload_2/unk_02113088.h"
#include "proto/autoload_2/unk_02114410.h"
#include "proto/autoload_2/unk_0212703c.h"
#include "proto/ov065/unk_ov065_0225f1a0.h"
#include "proto/ov065/unk_ov065_0225fdf0.h"
extern "C" {
SockCoreCommand *SockCore_AllocMsg(void *fn, SockCoreSocket *o, s32 c); // src/ov065/unk_ov065_0225f378.cpp
u32 OS_DisableInterrupts(void);
u32 OS_RestoreInterrupts(u32 state);
void OS_Sleep(u32 msec);
void MI_CpuCopy8(const void *src, void *dst, u32 size);
void MI_CpuFill8(void *dst, u32 data, u32 size);
}

namespace Unk_ov065_02260de4_Ns {

// ov065_003: socket-library style free functions (0x02260de4..0x02261638)

// (the socket's handle, not its address: GameSpy's code keeps them in ints - GsPollFd, include/net/GsSocket.h)
struct SockPollFd {
    s32 sock;
    s16 events;
    u16 revents;
};

extern "C" {

// main module
s32 OS_SNPrintf(char *, u32, const char *, ...);
s64 func_02133100(s64, s64);

// same overlay, out of range
s32 SockCore_GetSendRingFree(SockCoreSocket *);
s32 SockCore_Cleanup();
s32 SockCore_SetDnsServers(u32, u32);
sptr SockCore_Accept(SockCoreSocket *, u16 *, u32 *);
s32 SockCore_Listen(SockCoreSocket *);
s32 SockCore_Connect(SockCoreSocket *, u16, u32);
s32 SockCore_Bind(SockCoreSocket *, u16);
sptr SockCore_Create(void *);
s32 SockCore_Close(SockCoreSocket *);
s32 SockCore_Shutdown(SockCoreSocket *);
s32 SockCore_SendTo(SockCoreSocket *, const void *, s32, u32, u32, u32);
s32 SockCore_RecvFrom(SockCoreSocket *, void *, s32, u16 *, u32 *, s32);
s32 Dns_QueryServer(s32, u32, u32);
s32 IpAddr_Parse(s32, s32 *);

// data
extern u32 sDnsServers[2];

// forward declarations (the original file defined them in ascending order)
u32 SockCore_GetPollEvents(SockCoreSocket *o);
s32 SockCore_GetRecvAvailable(SockCoreSocket *o);
s32 SockCore_IsInvalidHandle(SockCoreSocket *x);
SockCoreSocket **SockList_Find(SockCoreSocket **pp, SockCoreSocket *n);
void SockList_Remove(SockCoreSocket **pp, SockCoreSocket *n);
void SockList_Push(SockCoreSocket **pp, SockCoreSocket *n);
void *Sock_AllocHook(u32 sz);
void Sock_FreeHook(void *p);
BOOL SockCore_IsInClosedList(SockCoreSocket *n);
char *Sock_InetNtoP(s32 mode, const void *x, char *buf, u32 len);
void IpAddr_StoreBe32(u32 v, u8 *p);

extern IpRandStateSignedStep sIpRandState;

static inline u32 HTONL(u32 x) {
    return ((x << 24) & 0xff000000) | (((x << 8) & 0xff0000) | (((x >> 24) & 0xff) | ((x >> 8) & 0xff00)));
}

static inline u16 HTONS(u16 x) {
    return (x >> 8 & 0xff) | ((x << 8) & 0xff00);
}

// own data
u32 sSockUdpParams[6] = {0x05c00101, 0x00000001, 0x00000000, 0x08000000, 0x080c0800, 0x00000000};
u32 sSockTcpParams[6] = {0x00000100, 0x05ea0000, 0x00000000, 0x00000bd5, 0x080c0800, 0x080d0800};
u32 sSockSendOnlyParams[6] = {0x00000002, 0x05ea0000, 0x00000000, 0x000006eb, 0x00000000, 0x080d0800};
SockAllocFunc sSockUserAlloc;
SockFreeFunc sSockUserFree;
SockCoreSocket *sSockOpenList;
SockCoreSocket *sSockClosedList;
u32 sHostentAddr;
char *sHostentAddrList[2];
char sInetNtoaBuf[16];
SockHostEnt sHostent;
SockCoreConfig sSockStartupConfig;
char sHostentName[0x104];

// Sockets are handed out as small numbers (1-64), not their addresses: on the DS a socket was its 32-bit address,
// and the callers keep it in an int (GameSpy's SOCKET, the poll records, ov001's tests); a 64-bit address there lost
// its upper half. HOST_SOCK_HANDLES holds the sockets by number.
#define HOST_SOCK_HANDLES 64
SockCoreSocket *sSockHandles[HOST_SOCK_HANDLES];
static sptr Sock_NewHandle(sptr r) {
    if (r < 0) {
        return r;  // (an error code)
    }
    for (s32 i = 0; i < HOST_SOCK_HANDLES; i++) {
        if (sSockHandles[i] == NULL) {
            sSockHandles[i] = (SockCoreSocket *)r;
            return i + 1;
        }
    }
    SockCore_Close((SockCoreSocket *)r);
    return -0x1c;  // (no room: as the DS's "no memory")
}
static SockCoreSocket *Sock_Of(sptr h) {
    return h > 0 && h <= HOST_SOCK_HANDLES ? sSockHandles[h - 1] : NULL;
}

sptr Sock_Create(s32 a, s32 b) {
    if (b == 1) {
        return Sock_NewHandle(SockCore_Create(sSockTcpParams));
    }
    return Sock_NewHandle(SockCore_Create(sSockUdpParams));
}

s32 Sock_Bind(sptr a, SockAddrIn *sa) {
    return SockCore_Bind(Sock_Of(a), HTONS(sa->port));
}

s32 Sock_Connect(sptr a, SockAddrIn *sa) {
    return SockCore_Connect(Sock_Of(a), HTONS(sa->port), HTONL(sa->addr));
}

s32 Sock_Recv(sptr a, void *b, s32 c, u32 d) {
    return SockCore_RecvFrom(Sock_Of(a), b, c, 0, 0, d);
}

s32 Sock_RecvFrom(sptr a, void *b, s32 c, u32 d, SockAddrIn *sa) {
    u16 port;
    u32 ip;
    s32 r = SockCore_RecvFrom(Sock_Of(a), b, c, &port, &ip, d);
    if (r >= 0) {
        SockAddrIn *q = *(SockAddrIn *volatile *)&sa;
        if (q != NULL) {
            q->port = HTONS(port);
            q->addr = HTONL(ip);
        }
    }
    return r;
}

s32 Sock_Send(sptr a, const void *b, s32 c, u32 d) {
    return SockCore_SendTo(Sock_Of(a), b, c, 0, 0, d);
}

s32 Sock_SendTo(sptr a, const void *b, s32 c, u32 d, SockAddrIn *sa) {
    u32 port;
    u32 ip;
    if (sa != NULL) {
        port = HTONS(sa->port);
        ip = HTONL(sa->addr);
    } else {
        port = 0;
        ip = port;
    }
    return SockCore_SendTo(Sock_Of(a), b, c, port, ip, d);
}

s32 Sock_Shutdown(sptr a) {
    return SockCore_Shutdown(Sock_Of(a));
}

s32 Sock_Close(sptr a) {
    s32 r = SockCore_Close(Sock_Of(a));
    if (Sock_Of(a) != NULL) {
        sSockHandles[a - 1] = NULL;
    }
    return r;
}

SockHostEnt *Sock_GetHostByName(const char *x) {
    u32 ip = SockCore_ResolveHost(x);
    if (ip == 0) {
        return NULL;
    }
    STD_CopyLString(sHostentName, x, 0x101);
    SockHostEnt *h = &sHostent;
    h->hostName = sHostentName;
    h->aliases = NULL;
    h->addrType = 2;
    h->addrLength = 4;
    h->addrList = sHostentAddrList;
    sHostentAddrList[0] = (char *)&sHostentAddr;
    sHostentAddrList[1] = NULL;
    sHostentAddr = HTONL(ip);
    return h;
}

s32 Sock_GetSockName(sptr h, SockAddrIn *sa) {
    SockCoreSocket *o = Sock_Of(h);
    u32 ip;
    u32 port;
    if (o == NULL) {
        return -0x27;
    }
    ip = SockCore_GetHostIp();
    if (o != NULL) {
        port = o->boundPort;
    } else {
        port = 0;
    }
    if (ip == 0) {
        port = 0;
    }
    sa->len = 8;
    sa->family = 2;
    sa->port = HTONS(port);
    sa->addr = HTONL(ip);
    return 0;
}

u32 Sock_GetHostId() {
    return HTONL(SockCore_GetHostIp());
}

s32 Sock_SetDnsServers(u32 *a, u32 *b) {
    return SockCore_SetDnsServers(HTONL(*a), HTONL(*b));
}

s32 Sock_Listen(sptr a) {
    return SockCore_Listen(Sock_Of(a));
}

sptr Sock_Accept(sptr a, SockAddrIn *sa) {
    u16 port;
    u32 addr;
    sptr r = Sock_NewHandle(SockCore_Accept(Sock_Of(a), &port, &addr));
    if (r >= 0) {
        sa->port = HTONS(port);
        sa->addr = HTONL(addr);
    }
    return r;
}

s32 Sock_Fcntl(sptr h, s32 cmd, u32 flags) {
    SockCoreSocket *o = Sock_Of(h);
    if (o == NULL) {
        return -1;
    }
    switch (cmd) {
    case 3:
        if (o->blocking == 1) {
            return 0;
        }
        return 4;
    case 4:
        if (flags & 4) {
            o->blocking = 0;
        } else {
            o->blocking = 1;
        }
        break;
    }
    return 0;
}

void *Sock_AllocHook(u32 sz) {
    u32 n = sz + 4;
    u32 *q = (u32 *)sSockUserAlloc(0, n);
    if (q != NULL) {
        *q = n;
        q++;
    }
    return q;
}

void Sock_FreeHook(void *p) {
    u32 *q = (u32 *)p;
    if (q != NULL) {
        q--;
        sSockUserFree(0, q, *q);
    }
}

s32 Sock_Startup(SockStartupConfig *s) {
    u32 t;
    SockCoreConfig *c;
    if (s->dhcpMode == 1) {
        t = 1;
    } else {
        t = 0;
    }
    c = &sSockStartupConfig;
    c->useDhcp = t;
    c->ownIp = HTONL(s->ownIp);
    c->netmask = HTONL(s->netmask);
    c->gateway = HTONL(s->gateway);
    c->dns1 = HTONL(s->dns1);
    c->dns2 = HTONL(s->dns2);
    c->alloc = Sock_AllocHook;
    c->free = Sock_FreeHook;
    sSockUserAlloc = s->allocFunc;
    sSockUserFree = s->freeFunc;
    c->msgPoolSize = 0x40;
    c->mtu = s->mtu;
    c->recvWindow = s->recvWindow;
    MI_CpuFill8(sSockHandles, 0, sizeof sSockHandles);
    return SockCore_Startup(c);
}

s32 Sock_Cleanup() {
    return SockCore_Cleanup();
}

char *Sock_InetNtoA(u32 a) {
    Sock_InetNtoP(2, &a, sInetNtoaBuf, 0x10);
    return sInetNtoaBuf;
}

s32 Sock_InetAtoN(const char *a, u32 *out) {
    u32 r = SockCore_ParseDottedAddr(a);
    if (r == 0) {
        return 0;
    }
    *out = HTONL(r);
    return 1;
}

char *Sock_InetNtoP(s32 mode, const void *x, char *buf, u32 len) {
    u8 b[8];
    if (mode != 2) {
        return NULL;
    }
    if (len < 0x10) {
        return NULL;
    }
    MI_CpuCopy8(x, b, 4);
    IpAddr_StoreBe32(*(u32 *)b, b + 4);
    OS_SNPrintf(buf, 0x10, "%d.%d.%d.%d", b[7], b[6], b[5], b[4]);
    return buf;
}

void IpAddr_StoreBe32(u32 v, u8 *p) {
    p[0] = v >> 24;
    p[1] = v >> 16;
    p[2] = v >> 8;
    p[3] = v;
}

s32 Sock_Poll(SockPollFd *arr, u32 n, s64 timeout) {
    s32 cnt;
    BOOL finite = (timeout != -1) ? TRUE : FALSE;
    for (;;) {
        SockPollFd *p;
        u32 i;
        p = arr;
        i = cnt = 0;
        if (i < n) {
            do {
                s32 ev = p->events;
                ev |= 0xe0;
                ev &= SockCore_GetPollEvents(Sock_Of(p->sock));
                if (ev != 0) {
                    cnt++;
                }
                p->revents = ev;
                p++;
                i++;
            } while (i < n);
        }
        if (cnt > 0) {
            break;
        }
        if (finite && timeout <= 0) {
            break;
        }
        OS_Sleep(1);
        timeout = timeout - 0x20b;
    }
    return cnt;
}

void SockCore_AddToOpenList(SockCoreSocket *n) {
    SockList_Push(&sSockOpenList, n);
}

void SockList_Push(SockCoreSocket **pp, SockCoreSocket *n) {
    n->next = *pp;
    *pp = n;
}

void SockCore_AddToClosedList(SockCoreSocket *n) {
    SockList_Push(&sSockClosedList, n);
}

void SockCore_RemoveFromOpenList(SockCoreSocket *n) {
    SockList_Remove(&sSockOpenList, n);
}

void SockList_Remove(SockCoreSocket **pp, SockCoreSocket *n) {
    SockCoreSocket **l = SockList_Find(pp, n);
    if (l != NULL) {
        *l = n->next;
    }
}

SockCoreSocket **SockList_Find(SockCoreSocket **pp, SockCoreSocket *n) {
    SockCoreSocket *p = *pp;
    if (p != NULL) {
        do {
            if (p == n) {
                return pp;
            }
            pp = &p->next;
            p = p->next;
        } while (p != NULL);
    }
    return NULL;
}

void SockCore_RemoveFromClosedList(SockCoreSocket *n) {
    SockList_Remove(&sSockClosedList, n);
}

s32 SockCore_IsInvalidHandle(SockCoreSocket *x) {
    if ((sptr)x <= 0 || SockList_Find(&sSockOpenList, x) == NULL) {
        return 1;
    }
    return 0;
}

BOOL SockCore_IsInClosedList(SockCoreSocket *n) {
    if (SockList_Find(&sSockClosedList, n) == NULL) {
        return FALSE;
    }
    return TRUE;
}

u32 SockCore_Align4(u32 x) {
    return (x + 3) & ~3;
}

u32 SockCore_GetPollEvents(SockCoreSocket *o) {
    u32 r = 0;
    BOOL ok;
    if (SockCore_IsInvalidHandle(o)) {
        r |= 0x80;
    } else {
        if (o->flags & 0x40) {
            r |= 0x20;
        }
        if (o->sockType == 1 || (o->flags & 4)) {
            s32 h = OS_DisableInterrupts();
            if (SockCore_GetRecvAvailable(o) > 0) {
                r |= 1;
            }
            if (SockCore_GetSendRingFree(o) > 0) {
                r |= 8;
            }
            OS_RestoreInterrupts(h);
        }
        ok = TRUE;
        if (o->sockType != 0 && o->sockType != 4) {
            ok = FALSE;
        }
        if (ok) {
            if ((o->flags & 4) && o->state != 4 && (r & 1) == 0) {
                o->flags &= ~6;
            }
            if ((o->flags & 2) == 0 && (o->flags & 4) == 0) {
                r |= 0x40;
            }
        }
    }
    return r;
}

s32 SockCore_GetRecvAvailable(SockCoreSocket *o) {
    s32 r;
    SockRecvPipe *c;
    c = o->recvPipe;
    r = 0;
    if (c != NULL) {
        s32 m = o->sockType;
        if (m == 1) {
            SockUdpRecvNode *b = c->head;
            if (b != NULL) {
                r = b->len;
            }
        } else if (m == 0 || m == 4) {
            r = o->rxLen - c->pos;
        }
    }
    return r;
}

}

}

namespace Unk_ov065_0225faf4_Ns { struct Unk_ov065_022603bc_Job; } // (the send job, defined below)

namespace Unk_ov065_02260488_Ns {


struct Unk_ov065_02260488_Node {
    u8 pad_00[2 * sizeof(void *)];  // (handler, socket: SockCoreCommand)
    void *replyQueue;
    u8 pad_0c;
    u8 blocking;
    u8 pad_0e[2];
    u8 *data1;
    s32 len1;
    u8 *data2;
    s32 len2;
    u16 ringEnd;
    u8 pad_22[2];
    u16 localPort;
    u16 remotePort;
    u32 remoteAddr;
};

struct Unk_ov065_02260488_Msg {
    Unk_ov065_02260488_Msg *next;
    SockCoreSocket *sock;
    void *replyQueue;
};
static_assert(sizeof(Unk_ov065_02260488_Node) <= SOCK_MSG_SIZE, "Unk_ov065_02260488_Node: a pool message");

typedef SockCoreSocket File;
typedef SockSendPipe Q68;
typedef SockRecvPipe Q64;
typedef Unk_ov065_02260488_Node Node;
typedef SockCoreConfig Alloc;

extern "C" {
s32 OS_GetProcMode();
void OS_JoinThread(void *);
void OS_DisableScheduler();
void OS_EnableScheduler();
s32 OS_ReceiveMessage(void *, void *, s32);
void OS_SendMessage(void *, void *, s32);
s32 OS_TryLockMutex(void *);

s32 SockCore_CmdSend(Unk_ov065_0225faf4_Ns::Unk_ov065_022603bc_Job *j);
s32 SockCore_ExecOnSendSide(SockCoreSocket *, Node *);
void SockCore_PostCommand(void *, s32);
void SockCore_PostCommandAsync(void *, void *);
void SockCore_FreeMsg(void *);
s32 SockCore_DestroyMsgPool();
s32 Dns_Resolve(const char *);
s32 IpSoc_AllocEphemeralPort();
void IpSoc_Use(void *);
void IpSoc_Unuse();
void IpSoc_TcpShutdown();
void IpSoc_TcpWaitClosed();
void IpSoc_Release();
void IpStack_Shutdown();
void IpStack_SetIdleCallback(s32);
s32 IpStack_RequestStop();
void WifiLink_SetRecvCallback(s32);
BOOL SockCore_IsInClosedList(SockCoreSocket *);
s32 SockCore_IsInvalidHandle(SockCoreSocket *);

extern File *sSockClosedList;
extern File *sSockOpenList;
extern Alloc *sSockCoreConfig;
extern u32 gOwnIp;
extern IpStackConfig sIpStackParams;
extern u32 sDnsServers[2];

s32 SockCore_QueueSend(File *self, const u8 *buf, s32 len, s32 off, u32 a5, u32 a6, s32 wait);
s32 SockCore_GetSendRingFree(File *self);
s32 SockCore_WaitSendRingSpace(File *self, s32 max, s32 want, s32 *out, s32 wait);
s32 SockCore_SendLoop(File *self, const u8 *buf, s32 len, s32 a4, u32 a5, s32 wait);
s32 SockCore_CmdShutdown(Unk_ov065_02260488_Msg *m);
s32 SockCore_Shutdown(File *self);
void SockCore_FreeClosedSockets();
void SockCore_StopCommandThread(OSMessageQueue *q, OSThread *thread);
// (a pipe's command queue and thread; a socket without that pipe has none)
extern "C++" template <class Pipe> inline void SockCore_StopPipe(Pipe *p) {
    SockCore_StopCommandThread(p != NULL ? &p->msgQueue : NULL, p != NULL ? &p->thread : NULL);
}
void SockCore_Free(File *self);
s32 SockCore_CmdClose(Unk_ov065_02260488_Msg *m);
s32 SockCore_Close(File *self);
s32 SockCore_IsFreed(File *self);
s32 SockCore_TryShutdown();
s32 SockCore_CloseAllUserSockets();
s32 SockCore_Cleanup();
s32 SockCore_GetHostIp();
s32 SockCore_SetDnsServers(u32 a, u32 b);
s32 SockCore_ParseDottedAddr(const char *x);
s32 SockCore_ResolveHost(const char *x);
}

static inline BOOL Unk_ov065_02260488_Valid(File *p) {
    BOOL r = FALSE;
    if (p == NULL || (p->flags & 1) == 0) {
    } else {
        r = TRUE;
    }
    return r;
}

static inline BOOL Unk_ov065_02260488_Local(File *p) {
    BOOL r = TRUE;
    s32 t = p->sockType;
    if (t != 0 && t != 4) {
        r = FALSE;
    }
    return r;
}

extern "C" {

s32 SockCore_ResolveHost(const char *x) {
    IpSocket conn;
    if (x == NULL) {
        return 0;
    }
    u8 *p = (u8 *)sSockCoreConfig->alloc(0x1154);
    if (p == NULL) {
        return 0;
    }
    MI_CpuFill8(&conn, 0, sizeof(conn));
    conn.rxBuf = p;
    conn.rxBufSize = 0xb68;
    conn.txBuf = p + 0xb68;
    conn.txBufSize = 0x5ea;
    IpSoc_Use(&conn);
    s32 r = Dns_Resolve(x);
    IpSoc_Unuse();
    sSockCoreConfig->free(p);
    return r;
}

s32 SockCore_ParseDottedAddr(const char *x) {
    u32 irq = OS_DisableInterrupts();
    u32 a = sDnsServers[0];
    u32 b = sDnsServers[1];
    sDnsServers[0] = 0;
    sDnsServers[1] = 0;
    s32 r = Dns_Resolve(x);
    sDnsServers[0] = a;
    sDnsServers[1] = b;
    OS_RestoreInterrupts(irq);
    return r;
}

s32 SockCore_SetDnsServers(u32 a, u32 b) {
    if (SockCore_GetHostIp() == 0) {
        return -0x27;
    }
    sDnsServers[0] = a;
    sDnsServers[1] = b;
    return 0;
}

s32 SockCore_GetHostIp() {
    u32 v = gOwnIp;
    if (v == 0) {
        if ((sSockCoreState & 3) == 1) {
            if (OS_GetProcMode() != 0x12) {
                OS_Sleep(10);
            }
        }
    } else if (sSockLastHostIp == 0) {
        sSockLastHostIp = v;
    }
    return gOwnIp;
}

s32 SockCore_Cleanup() {
    if (sSockLastHostIp == 0) {
        sSockLastHostIp = gOwnIp;
    }
    s32 r = SockCore_TryShutdown();
    if (r == -0x1a) {
        do {
            OS_Sleep(100);
        } while (SockCore_TryShutdown() == -0x1a);
    }
    r = SockCore_DestroyMsgPool();
    if (r >= 0) {
        IpStack_Shutdown();
        IpStack_SetIdleCallback(0);
        Alloc *a = sSockCoreConfig;
        if (a->recvRingBuf == 0) {
            a->free(sIpStackParams.recvRingBuf);
        }
        sSockCoreConfig = NULL;
    }
    return r;
}

s32 SockCore_CloseAllUserSockets() {
    File *p;
    for (;;) {
        u32 irq = OS_DisableInterrupts();
        p = sSockOpenList;
        if (p != NULL) {
            File *g = sSockDefaultSocket;
            do {
                if (p != g && (p->flags & 0x10) == 0) {
                    break;
                }
                p = p->next;
            } while (p != NULL);
        }
        OS_RestoreInterrupts(irq);
        if (p == NULL) {
            break;
        }
        SockCore_Close(p);
    }
    File *q = sSockOpenList;
    if (q != NULL) {
        if (q != sSockDefaultSocket) {
            goto fail;
        }
        if (q->next != NULL) {
            goto fail;
        }
    }
    if (sSockClosedList == NULL) {
        return 0;
    }
fail:
    return -0x1a;
}

s32 SockCore_TryShutdown() {
    s32 r;
    if (sSockDefaultSocket != NULL) {
        r = SockCore_CloseAllUserSockets();
        if (r == 0) {
            SockCore_Close(sSockDefaultSocket);
            if (SockCore_IsFreed(sSockDefaultSocket)) {
                sSockDefaultSocket = NULL;
            }
            r = -0x1a;
        }
        SockCore_FreeClosedSockets();
    } else {
        if (IpStack_RequestStop()) {
            WifiLink_SetRecvCallback(0);
            r = 0;
        } else {
            r = -0x1a;
        }
    }
    return r;
}

s32 SockCore_IsFreed(File *self) {
    if ((sptr)self >= 0 && SockCore_IsInvalidHandle(self) != 0 && SockCore_IsInClosedList(self) == 0) {
        return TRUE;
    }
    return FALSE;
}

s32 SockCore_Close(File *self) {
    if ((sptr)self <= 0) {
        return -0x1c;
    }
    if (SockCore_IsInClosedList(self)) {
        return -0x1a;
    }
    if (SockCore_IsInvalidHandle(self)) {
        return 0;
    }
    if (!Unk_ov065_02260488_Valid(self)) {
        return -0x27;
    }
    if ((*(volatile s16 *)&self->flags & 0x10) != 0) {
        return -0x1a;
    }
    self->flags = *(volatile s16 *)&self->flags | 0x18;
    if (Unk_ov065_02260488_Local(self)) {
        SockCore_PostCommand(self->sendPipe, 0);
    }
    Node *n = (Node *)SockCore_AllocMsg((void *)SockCore_CmdClose, self, 1);
    n->replyQueue = NULL;
    SockCore_PostCommandAsync(self, n);
    return 0;
}

s32 SockCore_CmdClose(Unk_ov065_02260488_Msg *m) {
    File *f = m->sock;
    if (Unk_ov065_02260488_Local(f)) {
        OS_JoinThread(&f->sendPipe->thread);
        IpSoc_TcpShutdown();
        IpSoc_TcpWaitClosed();
        IpSoc_Release();
    }
    IpSoc_Unuse();
    f->flags = f->flags & ~6;
    void *x;
    if (f->sockType == 2) {
        x = f->sendPipe;
    } else {
        x = f->recvPipe;
    }
    SockCore_PostCommand(x, 0);
    u32 irq = OS_DisableInterrupts();
    SockCore_RemoveFromOpenList(f);
    SockCore_AddToClosedList(f);
    OS_RestoreInterrupts(irq);
    f->flags = f->flags | 0x20;
    return 0;
}

void SockCore_Free(File *self) {
    if (self != NULL) {
        self->flags = 0;
        BOOL local = Unk_ov065_02260488_Local(self);
        if (local) {
            SockCore_StopPipe(self->sendPipe);
            SockCore_StopPipe(self->recvPipe);
        } else if (self->sockType == 1) {
            Unk_ov065_02260488_Msg *p = (Unk_ov065_02260488_Msg *)self->recvPipe->head;
            if (p != NULL) {
                do {
                    Unk_ov065_02260488_Msg *next = p->next;
                    sSockCoreConfig->free(p);
                    p = next;
                } while (p != NULL);
            }
            self->recvPipe->used = 0;
            self->recvPipe->tail = 0;
            self->recvPipe->head = 0;
            OS_WakeupThread(&self->recvPipe->waitQueue);
            SockCore_StopPipe(self->recvPipe);
        } else if (self->sockType == 2) {
            SockCore_StopPipe(self->sendPipe);
        }
        u32 irq = OS_DisableInterrupts();
        SockCore_RemoveFromOpenList(self);
        SockCore_RemoveFromClosedList(self);
        sSockCoreConfig->free(self);
        OS_RestoreInterrupts(irq);
    }
}

void SockCore_StopCommandThread(OSMessageQueue *q, OSThread *thread) {
    if (q != NULL) {
        void *msg;
        OS_JoinThread(thread);
        u32 irq = OS_DisableInterrupts();
        OS_DisableScheduler();
        if (OS_ReceiveMessage(q, &msg, 0)) {
            do {
                if (msg != NULL) {
                    if (((Unk_ov065_02260488_Msg *)msg)->replyQueue != NULL) {
                        OS_SendMessage(((Unk_ov065_02260488_Msg *)msg)->replyQueue, (void *)(sptr)-0xb, 0);  // (the result as the message)
                    }
                    SockCore_FreeMsg(msg);
                }
            } while (OS_ReceiveMessage(q, &msg, 0));
        }
        OS_EnableScheduler();
        OS_RestoreInterrupts(irq);
        OSi_RescheduleThread();
    }
}

void SockCore_FreeClosedSockets() {
    u32 irq = OS_DisableInterrupts();
    File *p = sSockClosedList;
    if (p != NULL) {
        do {
            SockCore_Free(p);
            p = sSockClosedList;
        } while (p != NULL);
    }
    OS_RestoreInterrupts(irq);
}

s32 SockCore_Shutdown(File *self) {
    if (SockCore_IsInvalidHandle(self)) {
        return -0x1c;
    }
    BOOL v = Unk_ov065_02260488_Valid(self);
    if (!v) {
        return -0x27;
    }
    if ((*(volatile s16 *)&self->flags & 4) == 0 || (*(volatile s16 *)&self->flags & 8) != 0) {
        return -0x38;
    }
    self->flags = self->flags | 8;
    Q68 *q = self->sendPipe;
    if (q != NULL && q->owner != 0) {
        Node *n = (Node *)SockCore_AllocMsg((void *)SockCore_CmdShutdown, q->owner, self->blocking);
        if (n == NULL) {
            return -0x21;
        }
        return SockCore_ExecOnSendSide(q->owner, n);
    }
    return 0;
}

s32 SockCore_CmdShutdown(Unk_ov065_02260488_Msg *m) {
    if (Unk_ov065_02260488_Local(m->sock)) {
        IpSoc_TcpShutdown();
    }
    return 0;
}

s32 SockCore_SendTo(File *self, const void *buf, s32 len, u32 a4, u32 a5, u32 a6) {
    Q68 *q;
    s32 flag;
    if (SockCore_IsInvalidHandle(self)) {
        return -0x1c;
    }
    BOOL v = Unk_ov065_02260488_Valid(self);
    if (!v) {
        return -0x27;
    }
    if (Unk_ov065_02260488_Local(self)) {
        if ((*(volatile s16 *)&self->flags & 4) == 0 || (*(volatile s16 *)&self->flags & 8) != 0) {
            return -0x38;
        }
    }
    q = self->sendPipe;
    if ((a6 & 4) != 0 || self->blocking == 0) {
        if (!OS_TryLockMutex(&q->mutex)) {
            return -6;
        }
        flag = 0;
    } else {
        OS_LockMutex(&q->mutex);
        flag = 1;
    }
    s32 r = SockCore_SendLoop(self, (const u8 *)buf, len, a4, a5, flag);
    OS_UnlockMutex(&q->mutex);
    return r;
}

s32 SockCore_SendLoop(File *self, const u8 *buf, s32 len, s32 a4, u32 a5, s32 wait) {
    s32 lim, total, off;
    total = 0;
    lim = self->sendPipe->owner->txBufSize;
    if (self->sockType == 1) {
        lim -= 0x2a;
        if (len > lim) {
            return -0x23;
        }
        lim = len;
    } else {
        lim -= 0x36;
        if (len <= lim) {
            lim = len;
        }
    }
    if (len > 0) {
        for (;;) {
            s32 n = SockCore_WaitSendRingSpace(self, len, lim, &off, wait);
            if (n > 0) {
                if (SockCore_QueueSend(self, buf, n, off, a4, a5, wait) <= 0) {
                    return -6;
                }
                buf += n;
                len -= n;
                total += n;
            }
            if (wait == 0) {
                if (n > 0) {
                    break;
                }
                return -6;
            }
            if (len <= 0) {
                break;
            }
        }
    }
    return total;
}

s32 SockCore_WaitSendRingSpace(File *self, s32 max, s32 want, s32 *out, s32 wait) {
    Q68 *q = self->sendPipe;
    s32 avail;
    if (want > max) {
        want = max;
    }
    u32 irq = OS_DisableInterrupts();
    for (;;) {
        avail = SockCore_GetSendRingFree(self);
        if (avail >= want) {
            if (avail >= max) {
                avail = max;
            }
            *out = q->ringWrite;
            break;
        }
        if (wait == 0) {
            avail = 0;
            break;
        }
        OS_SleepThread(&q->spaceWaitQueue);
    }
    OS_RestoreInterrupts(irq);
    return avail;
}

s32 SockCore_GetSendRingFree(File *self) {
    Q68 *q = self->sendPipe;
    s32 size = q->ring.size;
    s32 a = *(volatile u16 *)&q->ringWrite;
    s32 b = *(volatile u16 *)&q->ringRead;
    s32 r = b - a - 1;
    if (r < 0) {
        r += size;
    }
    return r;
}

s32 SockCore_QueueSend(File *self, const u8 *buf, s32 len, s32 off, u32 a5, u32 a6, s32 wait) {
    Q68 *q = self->sendPipe;
    Node *n = (Node *)SockCore_AllocMsg((void *)SockCore_CmdSend, q->owner, wait);
    if (n == NULL) {
        return -0x21;
    }
    n->blocking = 0;
    u8 *base = q->ring.buf;
    s32 size = q->ring.size;
    s32 end = off + len;
    if (end < size) {
        n->data1 = base + off;
        n->len1 = len;
        n->data2 = 0;
        n->len2 = 0;
        off = end;
    } else {
        n->data1 = base + off;
        n->len1 = size - off;
        n->data2 = base;
        n->len2 = len - n->len1;
        off = n->len2;
        MI_CpuCopy8(buf + n->len1, n->data2, off);
    }
    MI_CpuCopy8(buf, n->data1, n->len1);
    u16 *p = &q->ringWrite;
    u32 saved = *p;
    n->ringEnd = off;
    *p = n->ringEnd;
    if (self->sockType == 1) {
        if (self->boundPort == 0) {
            self->boundPort = IpSoc_AllocEphemeralPort();
            self->localPort = self->boundPort;
        }
        n->localPort = self->boundPort;
        u32 w = self->peerAddr;
        if (w == 0 || a6 != 0) {
            n->remoteAddr = a6;
            n->remotePort = *(u16 *)&a5;
        } else {
            n->remoteAddr = w;
            n->remotePort = self->peerPort;
        }
    } else {
        n->remoteAddr = 0;
    }
    if (SockCore_ExecOnSendSide(q->owner, n)) {
        q->ringWrite = saved;
        len = 0;
    }
    return len;
}

}

}

namespace Unk_ov065_0225faf4_Ns {







typedef SockUdpRecvNode Node;
typedef SockCoreSocket Sess;
typedef SockRecvPipe Ctx;
typedef SockSendPipe Rx;


struct Unk_ov065_0225ff64_Job {
    void *handler;  // (the SockCoreCommand head)
    Sess *sock;
    void *replyQueue;
    s8 sockType;
    s8 blocking;
    u8 pad_0e[2];
    u8 *buf;
    u32 len;
    u16 *outPort;
    u32 *outAddr;
};

struct Unk_ov065_022603bc_Job {
    void *handler;  // (the SockCoreCommand head)
    Sess *sock;
    void *replyQueue;
    s8 sockType;
    s8 blocking;
    u8 pad_0e[2];
    u8 *data1;
    s32 len1;
    u8 *data2;
    s32 len2;
    u16 ringEnd;
    u8 pad_22[2];
    u16 localPort;
    u16 remotePort;
    u32 remoteAddr;  // IPv4 address
};
static_assert(sizeof(Unk_ov065_0225ff64_Job) <= SOCK_MSG_SIZE, "Unk_ov065_0225ff64_Job: a pool message");
static_assert(sizeof(Unk_ov065_022603bc_Job) <= SOCK_MSG_SIZE, "Unk_ov065_022603bc_Job: a pool message");

typedef Unk_ov065_0225faf4_Job Job;
typedef Unk_ov065_0225ff64_Job RJob;
typedef Unk_ov065_022603bc_Job WJob;


extern "C" {
extern s32 sSockConnectInProgressError;
extern SockCoreConfig *sSockCoreConfig;
extern SockUdpDropCounters sSockUdpDropCount;

s32 OS_GetProcMode();
s32 OS_TryLockMutex(void *m);

s32 SockCore_ExecOnRecvSide(Sess *s, void *m);
s32 SockCore_IsInvalidHandle(Sess *s);
void IpSoc_Bind(u16 a, u16 b, u32 c);
s32 IpSoc_TcpConnect();
void IpSoc_Consume(s32 n);
u8 *IpSoc_Read(u32 *out);
s32 IpSoc_Write(u8 *p, s32 n);

s32 SockCore_CmdConnect(Job *j);
s32 SockCore_PostConnect(Sess *s);
s32 SockCore_Connect(Sess *s, u16 a, u32 b);
s32 SockCore_Bind(Sess *s, u16 a);
s32 SockCore_OnUdpReceive(u8 *data, u32 len, IpSocket *ip);
s32 SockCore_RecvDatagram(Sess *s, u8 *dst, s32 max, u16 *o1, u32 *o2, s32 block);
s32 SockCore_ConsumeRecvData(Sess *s);
s32 SockCore_CmdConsume(Job *j);
s32 SockCore_RequestConsume(Sess *s);
s32 SockCore_CmdRecvWait(RJob *j);
s32 SockCore_PostRecvWait(Sess *s, u8 *a, s32 b, u16 *c, u32 *d);
u8 *SockCore_PeekStreamData(Sess *s, s32 *outlen, u16 *a, u16 *b, u32 *c);
s32 SockCore_RecvStreamData(Sess *s, u8 *buf, s32 n, u16 *pa, u32 *pb);
s32 SockCore_RecvStream(Sess *s, u8 *buf, s32 n, u16 *pa, u32 *pb, s32 e);
s32 SockCore_RecvLocked(Sess *s, u8 *buf, s32 n, u16 *pa, u32 *pb, s32 e, s32 f);
s32 SockCore_CopySendChunk(u8 *buf, s32 n, WJob *j);
s32 SockCore_CmdSend(WJob *j);
}

static inline Unk_ov065_0225faf4_Job *AllocJob(void *fn, Sess *s, s32 i)
{
    return (Unk_ov065_0225faf4_Job *)SockCore_AllocMsg(fn, s, i);
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

s32 SockCore_CmdSend(WJob *j)
{
    Sess *s = j->sock;
    Rx *rx = s->sendPipe;
    u8 *buf;
    s32 r = 0;
    s32 off;
    s32 room;
    s32 k;
    s32 m;

    if (!IsTcp(s) || (s->flags & 4)) {
        if (j->remoteAddr != 0) {
            IpSoc_Bind(j->localPort, j->remotePort, j->remoteAddr);
        }
        if (IsTcp(s)) {
            off = 0x36;
        } else {
            off = 0x2a;
        }
        buf = s->txBuf + off;
        room = s->txBufSize - off;
        for (;;) {
            k = SockCore_CopySendChunk(buf, room, j);
            if (k <= 0) {
                break;
            }
            m = IpSoc_Write(buf, k);
            if (m > 0) {
                goto addm;
            }
            if (IsTcp(s)) {
                s->flags &= ~0xe;
            }
            r = -0x4c;
            break;
        addm:
            r += m;
        }
    } else {
        r = -0x4c;
    }
    rx->ringRead = j->ringEnd;
    OS_WakeupThread(&rx->spaceWaitQueue);
    return r;
}

s32 SockCore_CopySendChunk(u8 *buf, s32 n, WJob *j)
{
    s32 a = j->len1;
    s32 b = j->len2;
    if (a > n) {
        a = n;
        b = 0;
    } else if (b > n - a) {
        b = n - a;
    }
    if (a > 0) {
        MI_CpuCopy8(j->data1, buf, a);
        j->data1 += a;
        j->len1 -= a;
    }
    if (b > 0) {
        MI_CpuCopy8(j->data2, buf + a, b);
        j->data2 += b;
        j->len2 -= b;
    }
    return a + b;
}

s32 SockCore_RecvFrom(Sess *s, void *buf, s32 n, u16 *pa, u32 *pb, s32 e)
{
    s32 blk;
    s32 r;
    Ctx *c;

    if (SockCore_IsInvalidHandle(s) != 0) {
        return -0x1c;
    }
    if ((e & 4) || s->blocking == 0) {
        if (s->sockType == 4) {
            return -0x1c;
        }
        blk = 0;
    } else {
        if (OS_GetProcMode() == 0x12) {
            return -0x1c;
        }
        blk = 1;
    }
    if (!IsValid(s)) {
        return -0x27;
    }
    if (IsTcp(s)) {
        if (!(s->flags & 4) || (s->flags & 8)) {
            return -0x38;
        }
    }
    c = s->recvPipe;
    if (blk == 0) {
        if (OS_TryLockMutex(&c->mutex) == 0) {
            return -6;
        }
    } else {
        OS_LockMutex(&c->mutex);
    }
    r = SockCore_RecvLocked(s, (u8 *)buf, n, pa, pb, blk, e);
    OS_UnlockMutex(&c->mutex);
    return r;
}

s32 SockCore_RecvLocked(Sess *s, u8 *buf, s32 n, u16 *pa, u32 *pb, s32 e, s32 f)
{
    Ctx *c = s->recvPipe;
    BOOL lock;
    s8 saved;
    s32 res;

    if ((f & 2) && c) {
        lock = TRUE;
    } else {
        lock = FALSE;
    }
    if (lock) {
        saved = c->lock;
        c->lock = 1;
    }
    if (s->sockType == 1) {
        res = SockCore_RecvDatagram(s, buf, n, pa, pb, e);
    } else {
        res = SockCore_RecvStream(s, buf, n, pa, pb, e);
        if (res >= 0) {
            SockCore_RequestConsume(s);
        }
    }
    if (lock) {
        c->lock = saved;
    }
    return res;
}

s32 SockCore_RecvStream(Sess *s, u8 *buf, s32 n, u16 *pa, u32 *pb, s32 e)
{
    s32 r;
    if (s->sockType == 4) {
        return SockCore_PostRecvWait(s, buf, n, pa, pb);
    }
    r = SockCore_RecvStreamData(s, buf, n, pa, pb);
    if (r == -6 && e == 1) {
        r = SockCore_PostRecvWait(s, buf, n, pa, pb);
    }
    return r;
}

s32 SockCore_RecvStreamData(Sess *s, u8 *buf, s32 n, u16 *pa, u32 *pb)
{
    u32 irq = OS_DisableInterrupts();
    u16 v[2];
    s32 outlen;
    u32 c;
    s32 r;
    u8 *src;

    src = SockCore_PeekStreamData(s, &outlen, &v[0], &v[1], &c);
    if (src != NULL) {
        r = outlen;
        if (r == 0) {
            r = -6;
        } else {
            if (n > r) {
                n = r;
            }
            if (IsTcp(s)) {
                r = n;
            }
            MI_CpuCopy8(src, buf, n);
            if (s->recvPipe->lock == 0) {
                s->recvPipe->pos = s->recvPipe->pos + r;
            }
        }
    } else {
        if (outlen == 0) {
            r = 0;
        } else {
            r = -0x1c;
        }
        s->flags &= ~6;
    }
    if (r >= 0) {
        if (pa != NULL && pb != NULL) {
            *pa = v[1];
            *pb = c;
        }
        if (s->boundPort == 0) {
            s->boundPort = v[0];
        }
    }
    OS_RestoreInterrupts(irq);
    return r;
}

u8 *SockCore_PeekStreamData(Sess *s, s32 *outlen, u16 *a, u16 *b, u32 *c)
{
    Ctx *ctx = s->recvPipe;
    Sess *e = (Sess *)ctx->thread.specific[0];
    s32 pos = ctx->pos;
    s32 d = e->rxLen - pos;
    if (d >= 0) {
        *a = e->localPort;
        *b = e->remotePort;
        *c = e->remoteAddr;
        *outlen = d;
        if (d == 0 && e->state != 4) {
            return NULL;
        }
    } else {
        *outlen = -1;
        return NULL;
    }
    return e->rxBuf + pos;
}

s32 SockCore_PostRecvWait(Sess *s, u8 *a, s32 b, u16 *c, u32 *d)
{
    RJob *j = (RJob *)AllocJob((void *)SockCore_CmdRecvWait, s, 1);
    j->buf = a;
    j->len = b;
    j->outPort = c;
    j->outAddr = d;
    return SockCore_ExecOnRecvSide(s, j);
}

s32 SockCore_CmdRecvWait(RJob *j)
{
    Sess *s = j->sock;
    Ctx *c = s->recvPipe;
    u8 *dst = j->buf;
    u32 n = j->len;
    u16 *pa = j->outPort;
    u32 *pb = j->outAddr;
    s32 pos = c->pos;
    u8 *src;
    u32 x;

    for (;;) {
        src = IpSoc_Read(&x);
        if (src == NULL) {
            break;
        }
        if ((s32)(x - pos) > 0) {
            break;
        }
        if (IsTcp(s)) {
            if (s->state != 4) {
                src = NULL;
                break;
            }
        }
        OS_Sleep(10);
    }
    if (s->sockType == 4) {
        if (src == NULL) {
            return 0;
        }
        if (n > x) {
            n = x;
        }
        MI_CpuCopy8(src, dst, n);
        IpSoc_Consume(n);
        return n;
    }
    if (src != NULL) {
        pos = SockCore_RecvStreamData(s, dst, n, pa, pb);
    } else {
        pos = 0;
    }
    if (pos <= 0) {
        return pos;
    }
    if (c->pos >= c->limit) {
        SockCore_ConsumeRecvData(s);
    }
    return pos;
}

s32 SockCore_RequestConsume(Sess *s)
{
    Ctx *c = s->recvPipe;
    if (c->pos < c->limit) {
        return 0;
    }
    Job *j = AllocJob((void *)SockCore_CmdConsume, s, 0);
    if (j == NULL) {
        return -0x21;
    }
    return SockCore_ExecOnRecvSide(s, j);
}

s32 SockCore_CmdConsume(Job *j)
{
    return SockCore_ConsumeRecvData(j->sock);
}

s32 SockCore_ConsumeRecvData(Sess *s)
{
    Ctx *c = s->recvPipe;
    u32 irq = OS_DisableInterrupts();
    s32 v = c->pos;
    if (v != 0) {
        c->pos = 0;
        IpSoc_Consume(v);
    }
    OS_RestoreInterrupts(irq);
    return v;
}

s32 SockCore_RecvDatagram(Sess *s, u8 *dst, s32 max, u16 *o1, u32 *o2, s32 block)
{
    u32 irq;
    s32 err;
    Ctx *c = s->recvPipe;
    Node *n;

    irq = OS_DisableInterrupts();
    n = c->head;
    while (n == NULL) {
        if (block == 0) {
            err = -6;
            break;
        }
        OS_SleepThread(&c->waitQueue);
        if (SockCore_IsInvalidHandle(s) != 0 || !IsValid(s)) {
            err = -0x37 - 1;
            break;
        }
        n = c->head;
    }
    if (n != NULL) {
        if (max > n->len) {
            max = n->len;
        }
        MI_CpuCopy8(n->data, dst, max);
        if (o1 != NULL) {
            *o1 = n->remotePort;
        }
        if (o2 != NULL) {
            *o2 = n->remoteAddr;
        }
        err = n->len;
        if (c->lock == 0) {
            c->head = n->next;
            if (n->next == NULL) {
                c->tail = NULL;
            }
            sSockCoreConfig->free(n);
            c->used = c->used - err;
        }
    }
    OS_RestoreInterrupts(irq);
    return err;
}

}

}

// ---- overlay variables (generated: tools/host_overlay_vars.py) ----
#include "host_overlay_vars.h"

HOST_OVERLAY_VARS(unk_ov065_0225fdf0) {
    HOST_OVERLAY_VAR(Unk_ov065_02260de4_Ns::sSockUdpParams),
    HOST_OVERLAY_VAR(Unk_ov065_02260de4_Ns::sSockTcpParams),
    HOST_OVERLAY_VAR(Unk_ov065_02260de4_Ns::sSockSendOnlyParams),
    HOST_OVERLAY_VAR(Unk_ov065_02260de4_Ns::sSockUserAlloc),
    HOST_OVERLAY_VAR(Unk_ov065_02260de4_Ns::sSockUserFree),
    HOST_OVERLAY_VAR(Unk_ov065_02260de4_Ns::sSockOpenList),
    HOST_OVERLAY_VAR(Unk_ov065_02260de4_Ns::sSockClosedList),
    HOST_OVERLAY_VAR(Unk_ov065_02260de4_Ns::sHostentAddr),
    HOST_OVERLAY_VAR(Unk_ov065_02260de4_Ns::sHostentAddrList),
    HOST_OVERLAY_VAR(Unk_ov065_02260de4_Ns::sInetNtoaBuf),
    HOST_OVERLAY_VAR(Unk_ov065_02260de4_Ns::sHostent),
    HOST_OVERLAY_VAR(Unk_ov065_02260de4_Ns::sSockStartupConfig),
    HOST_OVERLAY_VAR(Unk_ov065_02260de4_Ns::sHostentName),
    HOST_OVERLAY_VAR(Unk_ov065_02260de4_Ns::sSockHandles),
    HOST_OVERLAY_VARS_END
};
