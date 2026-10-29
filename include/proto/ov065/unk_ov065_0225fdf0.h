#ifndef PROTO_PROTO_OV065_UNK_OV065_0225FDF0_H
#define PROTO_PROTO_OV065_UNK_OV065_0225FDF0_H

// Prototypes of the functions defined in src/ov065/unk_ov065_0225fdf0.cpp (generated: tools/host_protos.py)

#include "types.h"
#include "net/SockAddrIn.h"
#include "net/SockCoreConfig.h"
#include "net/SockHostEnt.h"
#include "net/SockStartupConfig.h"
#include "sys/OSMessageQueue.h"
#include "sys/OSThread.h"
#include "uptr.h"
#ifdef __cplusplus
#include "net/SockCoreSocket.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif
#ifndef PROTO_LOCAL_sSockUdpParams
extern u32 sSockUdpParams[6];
#endif
#ifndef PROTO_LOCAL_sSockTcpParams
extern u32 sSockTcpParams[6];
#endif
#ifndef PROTO_LOCAL_sSockSendOnlyParams
extern u32 sSockSendOnlyParams[6];
#endif
#ifndef PROTO_LOCAL_sSockUserAlloc
extern SockAllocFunc sSockUserAlloc;
#endif
#ifndef PROTO_LOCAL_sSockUserFree
extern SockFreeFunc sSockUserFree;
#endif
#ifndef PROTO_LOCAL_sHostentAddr
extern u32 sHostentAddr;
#endif
#ifndef PROTO_LOCAL_sHostentAddrList
extern char *sHostentAddrList[2];
#endif
#ifndef PROTO_LOCAL_sInetNtoaBuf
extern char sInetNtoaBuf[16];
#endif
#ifndef PROTO_LOCAL_sHostentName
extern char sHostentName[0x104];
#endif
#ifndef PROTO_LOCAL_Sock_Create
sptr Sock_Create(s32 a, s32 b);
#endif
#ifndef PROTO_LOCAL_Sock_Bind
s32 Sock_Bind(sptr a, SockAddrIn *sa);
#endif
#ifndef PROTO_LOCAL_Sock_Connect
s32 Sock_Connect(sptr a, SockAddrIn *sa);
#endif
#ifndef PROTO_LOCAL_Sock_Recv
s32 Sock_Recv(sptr a, void *b, s32 c, u32 d);
#endif
#ifndef PROTO_LOCAL_Sock_RecvFrom
s32 Sock_RecvFrom(sptr a, void *b, s32 c, u32 d, SockAddrIn *sa);
#endif
#ifndef PROTO_LOCAL_Sock_Send
s32 Sock_Send(sptr a, const void *b, s32 c, u32 d);
#endif
#ifndef PROTO_LOCAL_Sock_SendTo
s32 Sock_SendTo(sptr a, const void *b, s32 c, u32 d, SockAddrIn *sa);
#endif
#ifndef PROTO_LOCAL_Sock_Shutdown
s32 Sock_Shutdown(sptr a);
#endif
#ifndef PROTO_LOCAL_Sock_Close
s32 Sock_Close(sptr a);
#endif
#ifndef PROTO_LOCAL_Sock_GetHostId
u32 Sock_GetHostId();
#endif
#ifndef PROTO_LOCAL_Sock_SetDnsServers
s32 Sock_SetDnsServers(u32 *a, u32 *b);
#endif
#ifndef PROTO_LOCAL_Sock_Listen
s32 Sock_Listen(sptr a);
#endif
#ifndef PROTO_LOCAL_Sock_Accept
sptr Sock_Accept(sptr a, SockAddrIn *sa);
#endif
#ifndef PROTO_LOCAL_Sock_AllocHook
void *Sock_AllocHook(u32 sz);
#endif
#ifndef PROTO_LOCAL_Sock_FreeHook
void Sock_FreeHook(void *p);
#endif
#ifndef PROTO_LOCAL_Sock_Startup
s32 Sock_Startup(SockStartupConfig *s);
#endif
#ifndef PROTO_LOCAL_Sock_Cleanup
s32 Sock_Cleanup();
#endif
#ifndef PROTO_LOCAL_Sock_InetNtoA
char *Sock_InetNtoA(u32 a);
#endif
#ifndef PROTO_LOCAL_Sock_InetAtoN
s32 Sock_InetAtoN(const char *a, u32 *out);
#endif
#ifndef PROTO_LOCAL_Sock_InetNtoP
char *Sock_InetNtoP(s32 mode, const void *x, char *buf, u32 len);
#endif
#ifndef PROTO_LOCAL_IpAddr_StoreBe32
void IpAddr_StoreBe32(u32 v, u8 *p);
#endif
#ifndef PROTO_LOCAL_SockCore_Align4
u32 SockCore_Align4(u32 x);
#endif
#ifndef PROTO_LOCAL_SockCore_ResolveHost
s32 SockCore_ResolveHost(const char *x);
#endif
#ifndef PROTO_LOCAL_SockCore_ParseDottedAddr
s32 SockCore_ParseDottedAddr(const char *x);
#endif
#ifndef PROTO_LOCAL_SockCore_SetDnsServers
s32 SockCore_SetDnsServers(u32 a, u32 b);
#endif
#ifndef PROTO_LOCAL_SockCore_GetHostIp
s32 SockCore_GetHostIp();
#endif
#ifndef PROTO_LOCAL_SockCore_Cleanup
s32 SockCore_Cleanup();
#endif
#ifndef PROTO_LOCAL_SockCore_CloseAllUserSockets
s32 SockCore_CloseAllUserSockets();
#endif
#ifndef PROTO_LOCAL_SockCore_TryShutdown
s32 SockCore_TryShutdown();
#endif
#ifndef PROTO_LOCAL_SockCore_StopCommandThread
void SockCore_StopCommandThread(OSMessageQueue *q, OSThread *thread);
#endif
#ifndef PROTO_LOCAL_SockCore_FreeClosedSockets
void SockCore_FreeClosedSockets();
#endif
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
extern "C" {
#ifndef PROTO_LOCAL_sSockOpenList
extern SockCoreSocket *sSockOpenList;
#endif
#ifndef PROTO_LOCAL_sSockClosedList
extern SockCoreSocket *sSockClosedList;
#endif
#ifndef PROTO_LOCAL_sHostent
extern SockHostEnt sHostent;
#endif
#ifndef PROTO_LOCAL_sSockStartupConfig
extern SockCoreConfig sSockStartupConfig;
#endif
#ifndef PROTO_LOCAL_Sock_GetHostByName
SockHostEnt *Sock_GetHostByName(const char *x);
#endif
#ifndef PROTO_LOCAL_Sock_GetSockName
s32 Sock_GetSockName(sptr h, SockAddrIn *sa);
#endif
#ifndef PROTO_LOCAL_Sock_Fcntl
s32 Sock_Fcntl(sptr h, s32 cmd, u32 flags);
#endif
#ifndef PROTO_LOCAL_SockCore_AddToOpenList
void SockCore_AddToOpenList(SockCoreSocket *n);
#endif
#ifndef PROTO_LOCAL_SockList_Push
void SockList_Push(SockCoreSocket **pp, SockCoreSocket *n);
#endif
#ifndef PROTO_LOCAL_SockCore_AddToClosedList
void SockCore_AddToClosedList(SockCoreSocket *n);
#endif
#ifndef PROTO_LOCAL_SockCore_RemoveFromOpenList
void SockCore_RemoveFromOpenList(SockCoreSocket *n);
#endif
#ifndef PROTO_LOCAL_SockList_Remove
void SockList_Remove(SockCoreSocket **pp, SockCoreSocket *n);
#endif
#ifndef PROTO_LOCAL_SockList_Find
SockCoreSocket **SockList_Find(SockCoreSocket **pp, SockCoreSocket *n);
#endif
#ifndef PROTO_LOCAL_SockCore_RemoveFromClosedList
void SockCore_RemoveFromClosedList(SockCoreSocket *n);
#endif
#ifndef PROTO_LOCAL_SockCore_IsInvalidHandle
s32 SockCore_IsInvalidHandle(SockCoreSocket *x);
#endif
#ifndef PROTO_LOCAL_SockCore_IsInClosedList
BOOL SockCore_IsInClosedList(SockCoreSocket *n);
#endif
#ifndef PROTO_LOCAL_SockCore_GetPollEvents
u32 SockCore_GetPollEvents(SockCoreSocket *o);
#endif
#ifndef PROTO_LOCAL_SockCore_GetRecvAvailable
s32 SockCore_GetRecvAvailable(SockCoreSocket *o);
#endif
}
#endif

#endif
