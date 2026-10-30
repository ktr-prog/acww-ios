#include "uptr.h"
// mwcc-flags: -O4,p -str reuse
#include "types.h"
#include "net/DwcNetChannelTable.h"
#include "net/NetCheck.h"
#include "net/gpiCallback.h"
#include "net/NasAuthParams.h"
#include "net/NasAuthResult.h"
#include "net/DwcControl.h"
#include "net/gp.h"
#include "net/DwcConnInfo.h"
#include "net/gpiInfo.h"

#include "types64.h"
#include "proto/autoload_2/unk_02114410.h"
#include "proto/autoload_2/unk_0212703c.h"
#include "proto/autoload_2/unk_02128030.h"
#include "proto/autoload_2/unk_0212a120.h"
#include "proto/itcm/unk_01ffa4a0.h"
#include "proto/ov065/unk_ov065_0226fc18.h"
#include "proto/ov065/unk_ov065_02270e34.h"
#include "proto/ov065/unk_ov065_02277140.h"
#include "proto/ov065/unk_ov065_02277974.h"
#include "proto/ov065/unk_ov065_02277e70.h"
#include "proto/ov065/unk_ov065_02283720.h"
#include "proto/ov065/unk_ov065_02285778.h"
#include "proto/ov065/unk_ov065_02286934.h"
extern "C" {
void MI_CpuFill8(void *dst, u32 data, u32 size);
void MI_CpuCopy8(const void *src, void *dst, u32 size);
}

extern "C" {
DwcControl *sDwcControl;
u32 data_ov065_02290674;
void *sDwcConnTable[32]; // the GT2 connection of each slot
DwcConnInfo sDwcConnInfo[32];
}
static_assert(sizeof(sDwcConnInfo) == 0x100, "sDwcConnInfo: 0x100 bytes on the DS");

namespace F0226f7c8 {




extern "C" {
extern NetCheckWork *sNetCheck;
extern void *sNetCheckHttp;
extern u8 data_ov065_0228bbac[];
extern u8 data_ov065_0228bbb4[];
extern u8 data_ov065_0228bae4[];
extern u8 data_ov065_0228bb10[];
extern u32 data_ov065_02290674;
extern DwcControl *sDwcControl;
extern void *sDwcConnTable[];
void NetCheck_StartThread();
void DwcHttp_Abort();
void NasAuth_Abort();
void DwcHttp_Destroy();
void NasAuth_Destroy();
void NetCheck_ThreadMain();
s32 gt2GetRemoteIP(void *connection);
u32 GOAGetLastError();
void DwcCore_SetError(s32, s32);
s32 DwcMatch_IsInactive();
DwcConnInfo *gt2GetConnectionData(void *connection);
u32 DwcConn_RemoveAid(u32);
void DwcMatch_UpdateServerStatus();
void DwcMatch_UpdateValidAidCount();
void DwcMatch_OnClientDisconnected(u32);
s32 DwcMatch_OnConnectionClosed(s32, s32, u32);
u32 DwcMatch_RemoveMemberAt(s32 idx, s32 n); // src/ov065/unk_ov065_022723b8.cpp
void qr2_send_statechanged(void *qrec);
void DwcMatch_ResetPlayerCounts();
void DwcMatch_OnGpMatchCommand(void *, u32, void *);
s32 DwcCore_HandleGpResult(s32);
u32 DwcConn_AidListToBitmap(u8 *, s32);
void OS_JoinThread(void *);
s32 OS_IsThreadTerminated(void *);
void OS_CreateThread(void *, void (*)(), void *, void *, u32, u32);
void OS_WakeupThreadDirect(void *);
s32 func_02133150(s32, s32);
u32 func_0213335c(u32, u32);
void strncpy(void *, void *, u32);
s32 strtoul(void *, u32, u32);
void DwcConn_OnGt2SocketError(void *socket);
}
}

namespace F022700e4 {



extern "C" {
extern DwcControl *sDwcControl;
extern void *sDwcConnTable[32];
extern DwcConnInfo sDwcConnInfo[32];
void DwcConn_OnGt2SocketError(void *socket);
void DwcMatch_OnGt2ConnectAttempt();
void DwcMatch_OnUnrecognizedPacket();
void DwcFriend_OnUpdateDone(s32 a, s32 b);
DwcConnInfo *gt2GetConnectionData(void *connection);
void DwcLogin_Fail(s32 a, s32 b);
void DwcFriend_ResetTimer();
void gt2CloseAllConnectionsHard(void *socket);
void DwcFriend_Fail(s32 a, s32 b);
void DwcMatch_Fail(s32 a, s32 b);
s32 gpIsBuddy(void *connection, s32 profile);
s32 gpGetBuddyIndex(void *connection, s32 profile, void *index);
s32 gpGetBuddyStatus(void *connection, s32 index, void *status);
s32 DwcMatch_StartClient(u32 a, DwcMatchedScCallback b, void *c, DwcNewClientCallback d, void *e);
void DwcMatch_StartGameServer(u32 a, DwcMatchedScCallback b, void *c, DwcNewClientCallback d, void *e);
void DwcFriend_StartUpdate(char *a, char *b, void (*c)(s32, u32, s32), s32 d, DwcFriendStatusCallback e, s32 f, DwcFriendDeleteCallback g, s32 h);
u32 gpInitialize(void *a, u32 b, s32 c);
s32 gpSetCallback(void *a, s32 b, GPCallback c, void *d);
void DwcLogin_Begin();
void DwcLogin_Process();
void DwcFriend_Process();
void DwcMatch_Process(s32 a);
void qr2_shutdown(void *q);
void gt2CloseConnectionHard(void *connection);
s32 gt2CreateSocket(void *socket, char *localAddress, s32 outgoingBufferSize, s32 incomingBufferSize, void *errorCallback);
void gt2Listen(void *socket, void *callback);
void gt2SetUnrecognizedMessageCallback(void *socket, void *callback);
s32 DwcMatch_GetAidList(u8 **out);
s32 DwcMatch_GetValidAidList(u8 **out);
u32 DwcMatch_RemoveMemberAt(s32 idx, s32 n);
s32 DwcMatch_GetValidAidCount();
s32 DwcMatch_GetClientCount();
s32 DwcCore_HasError();
void DwcCore_SetError(s32 a, s32 b);
s32 DwcCore_CheckFatalError();
void DwcCore_Nop();
void MIi_CpuClear32(u32 v, void *dst, u32 size);
s32 DwcConn_HandleGt2Result(s32 x);
s32 DwcCore_HandleGpResult(s32 x);
u32 DwcConn_AidListToBitmap(u8 *p, s32 n);
u32 DwcConn_IsAidConnected(u32 v);
u32 DwcConn_GetSlotIndex(void *v);
void *DwcConn_FindConnectionByAid(u32 v);
void **DwcConn_GetSlot(u32 idx);
s32 DwcConn_GetAidList(u8 **out);
void DwcConn_ClearTables();
u32 DwcConn_RemoveAid(u32 c);
DwcConnInfo *DwcConn_GetConnInfo(u32 idx);
void **DwcConn_FindSlotByProfileId(u32 key, s32 n);
s32 DwcConn_FindFreeSlot();
u32 DwcConn_GetAid(void *v);
s32 DwcConn_CreateGt2Socket();
s32 DwcCore_GetState();
u32 DwcConn_IsAidValid(u32 bit);
u32 DwcConn_GetAidBitmap();
u32 DwcConn_GetMyAid();
s32 DwcConn_GetConnectionCount();
s32 DwcConn_CloseConnection(u32 a);
s32 DwcConn_CloseAllConnections();
void DwcConn_SetClosedCallback(DwcConnectionClosedCallback a, s32 b);
void DwcMatch_ConnectToFriendServer(s32 a, DwcMatchedScCallback cb, void *arg, DwcNewClientCallback d, void *e);
void DwcMatch_SetupGameServer(s32 a, DwcMatchedScCallback b, void *c, DwcNewClientCallback d, void *e);
s32 DwcFriend_UpdateServersAsync(char *s, DwcDoneCallback f1, s32 f2, DwcFriendStatusCallback f3, s32 p5, DwcFriendDeleteCallback p6, s32 p7);
void DwcLogin_Start(s32 a, s32 b, DwcDoneCallback c, s32 d);
void DwcCore_Process();
}
}

namespace F02270b74 {










namespace Unk_ov065_0227138c_Ns {
}
extern "C" {
extern DwcControl *sDwcControl;
extern s32 sDwcErrorClass;
extern s32 sDwcErrorCode;
extern u32 sDwcLoginDoneArg;
extern DwcLoginControl *sDwcLoginControl;
extern DwcNasLoginCallback sDwcLoginDoneCallback;
extern u8 data_ov065_0228c818[];
void qr2_shutdown(void *);
s32 ServerBrowserFree(void *sb);
s32 gpSetCallback(void *, s32, GPCallback, void *);
void gpProcess(void *);
void gpDestroy(void *);
void DwcFriend_ClearControl(void);
void DwcMatch_Shutdown(void);
void gt2CloseSocket(void *);
void DwcMatch_OnGt2Connected(void);
void DwcConn_ClearTables(void);
void DwcLogin_InitControl(void *mem, DwcUserData *a, GPConnection *b, u32 c, u32 d, DwcLoginResultCallback e, u32 f);
void DwcFriend_InitControl(DwcFriendControl *a, GPConnection *b, char *c, DwcFriendData *d, s32 e);
void DwcMatch_InitControl(void *, void *, void *, void *, void *, void *, void *, u32);
void DWCi_Acc_LoginIdToUserName(void *, u32, void *);
s32 gpSetInfosA(void *, s32, void *);
s32 gpGetInfo(void *, u32, s32, s32, GPCallback, void *);
s32 gpDisconnect(void *);
s32 gpConnectPreAuthenticatedA(void *, void *, void *, s32, s32, void *, void *);
void DWCi_Acc_SetLoginIdToUserData(void *, void *, u32);
s32 NasAuth_GetState(void);
void NasAuth_GetResult(s32 *);
void NasAuth_Destroy(void);
void NasAuth_Abort(void);
s32 NasAuth_Start(NasAuthParams *, DwcHttp *);
u64 func_02132ef8(u64, u32);
u64 func_02133100(u64, u32, u32);
s32 DWCi_Acc_IsAuthentic(void *);
s32 DWCi_Acc_IsValidLoginId(void *);
s32 DWCi_Acc_CheckConsoleUserId(void *);
void DWCi_Acc_CreateTempLoginId(void *);
void DWCi_Acc_SetPlayerId(void *, u32);
s32 DwcConn_CreateGt2Socket(void);
s32 DwcMatch_StartQr2(u32);
void DwcCore_Nop(void);
s32 DwcCore_CheckFatalError(void);
void DwcCore_Shutdown(void);
void DwcCore_Init(DwcControl *g, DwcUserData *a1, u32 productId, const char *a3, const char *a4, s32 sendBufSize, s32 recvBufSize, DwcFriendData *friendList, s32 friendListLen);
void DwcCore_SetError(s32 a, s32 b);
BOOL DwcCore_HasError(void);
void DwcCore_ClearError(void);
s32 DwcCore_GetLastError(s32 *out);
BOOL DwcLogin_IsLoggedIn(void);
void DwcLogin_OnGpProfileInfo(GPConnection *a0, GPGetInfoResponseArg *x);
void DwcLogin_PollNasAuth(void);
void DwcLogin_StartNasAuth(DwcNasLoginCallback cb, u32 arg);
void DwcLogin_GpConnect(const char *a, const char *b, void *c, s32 d);
void DwcLogin_OnNasAuthDone(const char *a, const char *b, u32 arg);
void DwcLogin_OnGpConnected(GPConnection *a0, GPConnectResponseArg *x);
void DwcLogin_ResetState(void);
void DwcLogin_Shutdown(void);
void DwcLogin_Fail(s32 a, s32 b);
void *DwcLogin_GetUserData(void);
}
}

#define G sDwcControl

#define FAIL710()                                                              \
    {                                                                          \
        DwcCore_SetError(10, 0);                                            \
        DwcControl *s = G;                                             \
        s->serverMatchCallback(10, 0, 1, 0, 0, s->serverMatchCallbackArg);                                  \
        if (G != 0 && G->state == 5) {                                        \
            DwcCore_SetState(3);                                            \
            DwcFriend_SetOwnStatus(1, "", NULL);                             \
            return;                                                            \
        }                                                                      \
    }

namespace F02270b74 {
extern "C" {
void DwcCore_Init(DwcControl *g, DwcUserData *a1, u32 productId, const char *a3, const char *a4, s32 sendBufSize, s32 recvBufSize, DwcFriendData *friendList, s32 friendListLen) {
    sDwcControl = g;
    DwcCore_ClearError();
    sDwcControl->transportSocket = NULL;
    sDwcControl->gt2ConnectedCallback = (void *)DwcMatch_OnGt2Connected;
    sDwcControl->gt2ReceivedCallback = (void *)DwcConn_OnGt2Receive;
    sDwcControl->gt2ClosedCallback = (void *)DwcConn_OnGt2Closed;
    sDwcControl->gt2PingCallback = (void *)DwcConn_OnGt2Ping;
    sDwcControl->gt2SendBufferSize = sendBufSize != 0 ? sendBufSize : 0x2000;
    sDwcControl->gt2RecvBufferSize = recvBufSize != 0 ? recvBufSize : 0x2000;
    sDwcControl->gpConnection = NULL;
    sDwcControl->userData = a1;
    sDwcControl->state = 0;
    sDwcControl->prevState = 0;
    sDwcControl->myAid = 0;
    sDwcControl->isClosingAll = 0;
    sDwcControl->unk_2e = 0;
    sDwcControl->ownProfileId = 0;
    sDwcControl->gameName = data_ov065_02291104;
    sDwcControl->secretKey = data_ov065_02291204;
    sDwcControl->loginCallback = 0;
    sDwcControl->loginCallbackArg = 0;
    sDwcControl->updateCallback = 0;
    sDwcControl->updateCallbackArg = 0;
    sDwcControl->matchCallback = 0;
    sDwcControl->matchCallbackArg = 0;
    sDwcControl->serverMatchCallback = 0;
    sDwcControl->serverMatchCallbackArg = 0;
    sDwcControl->closedCallback = 0;
    sDwcControl->closedCallbackArg = 0;
    DwcConn_ClearTables();
    DwcLogin_InitControl(&sDwcControl->loginControl, a1, &sDwcControl->gpConnection, productId, a1->gameCode,
                        (DwcLoginResultCallback)DwcLogin_OnLoginDone, 0);
    DwcFriend_InitControl(&sDwcControl->friendControl, &sDwcControl->gpConnection, sDwcControl->buddyRequestText, friendList,
                        friendListLen);
    DwcMatch_InitControl(&sDwcControl->matchControl, &sDwcControl->gpConnection, sDwcControl,
                        &sDwcControl->gt2ConnectedCallback, data_ov065_02291104, data_ov065_02291204, friendList, friendListLen);
#ifdef HOST64
    // (the table follows the control block, in the caller's buffer: data_021f4bc0, 0x1000 bytes on 64-bit hosts)
    static_assert(__builtin_offsetof(DwcControl, netChannelTable) + sizeof(DwcNetChannelTable) <= 0x1000, "DwcControl");
#endif
    DwcNet_InitChannelTable(&sDwcControl->netChannelTable);
    u32 n;
    if (STD_GetStringLength(a3) < 0x100) {
        n = STD_GetStringLength(a3);
    } else {
        n = 0xff;
    }
    MI_CpuCopy8(a3, data_ov065_02291104, n);
    data_ov065_02291104[n] = 0;
    u32 m;
    if (STD_GetStringLength(a4) < 0x100) {
        m = STD_GetStringLength(a4);
    } else {
        m = 0xff;
    }
    MI_CpuCopy8(a4, data_ov065_02291204, m);
    data_ov065_02291204[m] = 0;
}
}
}

namespace F02270b74 {
extern "C" {
void DwcCore_Shutdown(void) {
    if (sDwcControl == NULL) {
        return;
    }
    if (sDwcControl->matchControl.qr2Object != NULL) {
        qr2_shutdown((void *)sDwcControl->matchControl.qr2Object);
        sDwcControl->matchControl.qr2Object = NULL;
    }
    sDwcControl->matchControl.qr2ShutdownPending = 0;
    if (sDwcControl->matchControl.serverBrowser != NULL) {
        ServerBrowserFree((void *)sDwcControl->matchControl.serverBrowser);
        sDwcControl->matchControl.serverBrowser = NULL;
    }
    NNFreeNegotiateList();
    CloseStatsConnection();
    if (sDwcControl->gpConnection != NULL) {
        gpSetCallback(&sDwcControl->gpConnection, 0, 0, 0);
        gpSetCallback(&sDwcControl->gpConnection, 3, 0, 0);
        gpSetCallback(&sDwcControl->gpConnection, 1, 0, 0);
        gpSetCallback(&sDwcControl->gpConnection, 2, 0, 0);
        gpProcess(&sDwcControl->gpConnection);
        gpDestroy(&sDwcControl->gpConnection);
        sDwcControl->gpConnection = NULL;
    }
    DwcLogin_Shutdown();
    DwcFriend_ClearControl();
    DwcMatch_Shutdown();
    DwcNet_ClearChannelTable();
    if (sDwcControl->transportSocket != NULL) {
        gt2CloseSocket(sDwcControl->transportSocket);
        sDwcControl->transportSocket = NULL;
    }
    sDwcControl = NULL;
}
}
}

namespace F02270b74 {
extern "C" {
s32 DwcCore_CheckFatalError(void) {
    DwcInet_Process();
    if (DwcInet_IsLinkLost()) {
        DwcCore_SetError(7, 0);
        DwcInet_WaitDisconnect();
        return 1;
    }
    return 0;
}
}
}

namespace F02270b74 {
extern "C" {
void DwcCore_Nop(void) {}
}
}

namespace F022700e4 {
extern "C" {
void DwcCore_Process() {
    if (DwcCore_CheckFatalError() != 0) {
        DwcCore_Nop();
    }
    if (G == 0 || G->state == 0 || DwcCore_HasError() != 0) {
        return;
    }
    switch (G->state) {
    case 0:
        break;
    case 1:
        switch (GSIAvailableCheckThink()) {
        case 1:
            if (DwcCore_HandleGpResult(gpInitialize(&G->gpConnection, G->loginControl.productId, 0)) != 0) {
                return;
            }
            if (DwcCore_HandleGpResult(gpSetCallback(&G->gpConnection, 0, (GPCallback)DwcCore_OnGpError, NULL)) != 0) {
                return;
            }
            if (DwcCore_HandleGpResult(gpSetCallback(&G->gpConnection, 3, (GPCallback)DwcMatch_OnGpBuddyMessage, NULL)) != 0) {
                return;
            }
            if (DwcCore_HandleGpResult(gpSetCallback(&G->gpConnection, 1, (GPCallback)DwcFriend_OnBuddyRequest, NULL)) != 0) {
                return;
            }
            if (DwcCore_HandleGpResult(gpSetCallback(&G->gpConnection, 2, (GPCallback)DwcFriend_OnBuddyStatus, NULL)) != 0) {
                return;
            }
            DwcCore_SetState(2);
            DwcLogin_Begin();
            break;
        case 2:
            DwcLogin_Fail(3, -0x4e8e);
            return;
        case 3:
            DwcLogin_Fail(4, -0x4e85);
            return;
        }
        break;
    case 2:
        DwcLogin_Process();
        break;
    case 3:
    case 4:
        DwcFriend_Process();
        DwcMatch_Process(0);
        break;
    case 5:
        DwcMatch_Process(1);
        DwcFriend_Process();
        break;
    case 6: {
        DwcControl *s;
        DwcNet_ProcessSend();
        DwcFriend_Process();
        s = G;
        if (s->matchControl.matchType == 2 || s->matchControl.matchType == 3) {
            DwcMatch_Process(1);
        } else if (s->transportSocket != 0) {
            DwcMatch_Process(0);
        }
        break;
    }
    }
    if (G->matchControl.qr2ShutdownPending == 1) {
        if (G->matchControl.qr2Object != NULL) {
            qr2_shutdown(G->matchControl.qr2Object);
            G->matchControl.qr2Object = NULL;
        }
        G->matchControl.qr2ShutdownPending = 0;
    }
}
}
}

namespace F022700e4 {
extern "C" {
void DwcLogin_Start(s32 a, s32 b, DwcDoneCallback c, s32 d) {
    if (DwcCore_HasError() == 0 && G->state == 0) {
        G->loginCallback = c;
        G->loginCallbackArg = d;
        G->loginControl.unk_10 = a;
        G->loginControl.unk_14 = b;
        if (DwcInet_UpdateStatus() != 4) {
            DwcLogin_Fail(2, -0xea6a);
            return;
        }
        DwcCore_SetState(1);
        GSIStartAvailableCheckA(G->gameName);
    }
}
}
}

namespace F022700e4 {
extern "C" {
s32 DwcFriend_UpdateServersAsync(char *s, DwcDoneCallback f1, s32 f2, DwcFriendStatusCallback f3, s32 p5, DwcFriendDeleteCallback p6, s32 p7) {
    s32 n;
    if (G == 0 || DwcCore_HasError() != 0 || G->state < 3 || G->state == 4) {
        return 0;
    }
    if (s == 0 || *s == 0) {
        n = 0;
    } else {
        if (STD_GetStringLength(s) < 0x20) {
            n = STD_GetStringLength(s);
        } else {
            n = 0x1f;
        }
        MI_CpuCopy8(s, G->buddyRequestText, n);
    }
    G->buddyRequestText[n] = 0;
    G->updateCallback = f1;
    G->updateCallbackArg = f2;
    DwcCore_SetState(4);
    DwcFriend_StartUpdate(G->loginControl.authToken, G->loginControl.authChallenge, (void (*)(s32, u32, s32))DwcFriend_OnUpdateDone, 0, f3, p5, p6, p7);
    return 1;
}
}
}

namespace F022700e4 {
extern "C" {
void DwcMatch_SetupGameServer(s32 a, DwcMatchedScCallback b, void *c, DwcNewClientCallback d, void *e) {
    if (DwcCore_HasError() == 0 && G->state == 3) {
        DwcConn_ClearTables();
        G->serverMatchCallback = b;
        G->serverMatchCallbackArg = c;
        G->myAid = 0;
        DwcCore_SetState(5);
        DwcMatch_StartGameServer((u8)(a - 1), (DwcMatchedScCallback)DwcMatch_OnMatchDone, 0, d, e);
    }
}
}
}

namespace F022700e4 {
extern "C" {
void DwcMatch_ConnectToFriendServer(s32 a, DwcMatchedScCallback cb, void *arg, DwcNewClientCallback d, void *e) {
    s32 v = -1;
    GPBuddyStatus buf;
    if (DwcCore_HasError() == 0 && G->state == 3) {
        u32 t;
        DwcConn_ClearTables();
        G->serverMatchCallback = cb;
        G->serverMatchCallbackArg = arg;
        DwcCore_SetState(5);
        t = DwcFriend_GetProfileId(a);
        if (t == 0 || gpIsBuddy(&G->gpConnection, t) == 0) {
            FAIL710()
        } else {
            gpGetBuddyIndex(&G->gpConnection, t, &v);
            gpGetBuddyStatus(&G->gpConnection, v, &buf);
            if (buf.status != 6) {
                FAIL710()
            } else {
                DwcMatch_StartClient(t, (DwcMatchedScCallback)DwcMatch_OnMatchDone, 0, d, e);
            }
        }
    }
}
}
}

namespace F022700e4 {
extern "C" {
void DwcConn_SetClosedCallback(DwcConnectionClosedCallback a, s32 b) {
    if (G != 0) {
        G->closedCallback = a;
        G->closedCallbackArg = b;
    }
}
}
}

namespace F022700e4 {
extern "C" {
s32 DwcConn_CloseAllConnections() {
    DwcControl *s;
    if (G == 0 || DwcCore_HasError() != 0 || (s = G, s->state != 5 && s->state != 6)) {
        return -1;
    }
    if (s->matchControl.numClients == 0) {
        DwcFriend_SetOwnStatus(1, "", NULL);
        NNFreeNegotiateList();
        DwcCore_SetState(3);
        return 1;
    }
    s->isClosingAll = 1;
    gt2CloseAllConnectionsHard(G->transportSocket);
    G->isClosingAll = 0;
    return 0;
}
}
}

namespace F022700e4 {
extern "C" {
s32 DwcConn_CloseConnection(u32 a) {
    void *r;
    if (G == 0 || DwcCore_HasError() != 0 || (G->state != 5 && G->state != 6)) {
        return -1;
    }
    r = DwcConn_FindConnectionByAid(a);
    if (r == 0) {
        return -2;
    }
    gt2CloseConnectionHard(r);
    return 0;
}
}
}

namespace F022700e4 {
extern "C" {
s32 DwcConn_GetConnectionCount() {
    if (G == 0) {
        return 0;
    }
    if (G->matchControl.matchType == 2 || G->matchControl.matchType == 3) {
        return DwcMatch_GetValidAidCount() + 1;
    }
    return DwcMatch_GetClientCount() + 1;
}
}
}

namespace F022700e4 {
extern "C" {
u32 DwcConn_GetMyAid() {
    if (G != 0) {
        return G->myAid;
    }
    return 0;
}
}
}

namespace F022700e4 {
extern "C" {
s32 DwcConn_GetAidList(u8 **out) {
    if (G == 0) {
        return 0;
    }
    *out = G->matchControl.aids;
    if (G->matchControl.matchType == 2 || G->matchControl.matchType == 3) {
        return DwcMatch_GetValidAidList(out);
    }
    return DwcMatch_GetAidList(out);
}
}
}

namespace F022700e4 {
extern "C" {
u32 DwcConn_GetAidBitmap() {
    u8 *p;
    if (G == 0) {
        return 0;
    }
    s32 n = DwcConn_GetAidList(&p);
    return DwcConn_AidListToBitmap(p, n);
}
}
}

namespace F022700e4 {
extern "C" {
u32 DwcConn_IsAidValid(u32 bit) {
    if (G == 0) {
        return 0;
    }
    if (G->matchControl.validAidMask & (1 << bit)) {
        return DwcConn_IsAidConnected(bit);
    }
    return 0;
}
}
}

namespace F022700e4 {
extern "C" {
s32 DwcCore_GetState() {
    if (G != 0) {
        return G->state;
    }
    return 0;
}
}
}

namespace F022700e4 {
extern "C" {
s32 DwcConn_CreateGt2Socket() {
    DwcControl *s;
    u32 h;
    s32 r;
    if (G->transportSocket != 0) {
        return 0;
    }
    h = (u16)(DwcNet_Rand32(0x4000) + 0xc000);
    s = G;
    r = gt2CreateSocket(&G->transportSocket, gt2AddressToString(0, h, 0), s->gt2SendBufferSize, s->gt2RecvBufferSize, (void *)DwcConn_OnGt2SocketError);
    if (DwcConn_HandleGt2Result(r) != 0) {
        return r;
    }
    gt2Listen(G->transportSocket, (void *)DwcMatch_OnGt2ConnectAttempt);
    gt2SetUnrecognizedMessageCallback(G->transportSocket, (void *)DwcMatch_OnUnrecognizedPacket);
    return r;
}
}
}

namespace F022700e4 {
extern "C" {
void *DwcConn_FindConnectionByAid(u32 v) {
    s32 i;
    void **p;
    if (G == 0) {
        return 0;
    }
    for (i = 0, p = sDwcConnTable; i < 0x20; p++, i++) {
        if (*p != 0) {
            DwcConnInfo *r = gt2GetConnectionData(*p);
            if (v == r->aid) {
                return sDwcConnTable[i];
            }
        }
    }
    return 0;
}
}
}

namespace F022700e4 {
extern "C" {
u32 DwcConn_GetAid(void *v) {
    return gt2GetConnectionData(v)->aid;
}
}
}

namespace F022700e4 {
extern "C" {
u32 DwcConn_GetSlotIndex(void *v) {
    return gt2GetConnectionData(v)->slotIndex;
}
}
}

namespace F022700e4 {
extern "C" {
s32 DwcConn_FindFreeSlot() {
    s32 i;
    void **p;
    for (i = 0, p = sDwcConnTable; i < 0x20; p++, i++) {
        if (*p == 0) {
            return i;
        }
    }
    return -1;
}
}
}

namespace F022700e4 {
extern "C" {
void DwcConn_ClearTables() {
    volatile u32 a = 0;
    MIi_CpuClear32(a, sDwcConnTable, sizeof(sDwcConnTable));
    volatile u32 b = 0;
    MIi_CpuClear32(b, sDwcConnInfo, sizeof(sDwcConnInfo));
}
}
}

namespace F022700e4 {
extern "C" {
void **DwcConn_GetSlot(u32 idx) {
    return &sDwcConnTable[idx];
}
}
}

namespace F022700e4 {
extern "C" {
void **DwcConn_FindSlotByProfileId(u32 key, s32 n) {
    u8 i;
    for (i = 0; i < n; i++) {
        if (key == G->matchControl.memberProfileIds[i]) {
            break;
        }
    }
    if (i >= n) {
        return 0;
    }
    return DwcConn_GetSlot(DwcConn_GetSlotIndex(DwcConn_FindConnectionByAid(G->matchControl.aids[i])));
}
}
}

namespace F022700e4 {
extern "C" {
DwcConnInfo *DwcConn_GetConnInfo(u32 idx) {
    return &sDwcConnInfo[idx];
}
}
}

namespace F022700e4 {
extern "C" {
u32 DwcConn_IsAidConnected(u32 v) {
    s32 i;
    void **p;
    for (i = 0, p = sDwcConnTable; i < 0x20; p++, i++) {
        if (*p != 0) {
            DwcConnInfo *r = gt2GetConnectionData(*p);
            if (v == r->aid) {
                return 1;
            }
        }
    }
    return 0;
}
}
}

namespace F022700e4 {
extern "C" {
void DwcCore_SetState(s32 s) {
    G->prevState = G->state;
    G->state = s;
}
}
}

namespace F022700e4 {
extern "C" {
u32 DwcConn_RemoveAid(u32 c) {
    u8 *p;
    s32 i;
    s32 n;
    u8 *q;
    n = DwcMatch_GetAidList(&p);
    i = 0;
    if (n > 0) {
        q = p;
        do {
            if (c == *q) {
                break;
            }
            q++;
            i++;
        } while (i < n);
    }
    if (i == n) {
        return 0;
    }
    return DwcMatch_RemoveMemberAt(i, n);
}
}
}

namespace F022700e4 {
extern "C" {
u32 DwcConn_AidListToBitmap(u8 *p, s32 n) {
    u32 r = 0;
    s32 i = 0;
    for (; i < n; i++) {
        r |= 1 << p[i];
    }
    return r;
}
}
}

namespace F022700e4 {
extern "C" {
s32 DwcCore_HandleGpResult(s32 x) {
    s32 a, b;
    if (x == 0) {
        return 0;
    }
    switch (x) {
    case 0:
        break;
    case 1:
        a = 8;
        b = -1;
        break;
    case 2:
        a = 8;
        b = -2;
        break;
    case 3:
        a = 6;
        b = -10;
        break;
    case 4:
        a = 6;
        b = -20;
        break;
    }
    switch (G->state) {
    case 1:
        b = b - 0xee48;
        DwcLogin_Fail(a, b);
        break;
    case 2:
        b = b - 0xee48;
        DwcLogin_Fail(a, b);
        break;
    case 5:
        b = b - 0x13c68;
        DwcMatch_Fail(a, b);
        break;
    case 4:
        b = b - 0x11558;
        break;
    default:
        b = b - 0x16378;
        break;
    }
    DwcFriend_Fail(a, b);
    return x;
}
}
}

namespace F022700e4 {
extern "C" {
s32 DwcConn_HandleGt2Result(s32 x) {
    s32 a, b;
    if (x == 0) {
        return 0;
    }
    switch (x) {
    case 0:
        break;
    case 1:
        a = 8;
        b = -1;
        break;
    case 2:
    case 5:
        a = 0;
        b = 0;
        x = 0;
        break;
    case 3:
        a = 6;
        b = -10;
        break;
    case 4:
        a = 6;
        b = -30;
        break;
    case 6:
        a = 6;
        b = -70;
        break;
    case 7:
        a = 6;
        b = -80;
        break;
    }
    if (a != 0) {
        DwcLogin_Fail(a, b - 0x105b8);
    }
    return x;
}
}
}

namespace F022700e4 {
extern "C" {
void DwcLogin_OnLoginDone(s32 a, s32 b) {
    if (a == 0) {
        G->ownProfileId = b;
        DwcCore_SetState(3);
        DwcFriend_ResetTimer();
    } else {
        DwcCore_SetState(0);
    }
    if (G->loginCallback != 0) {
        G->loginCallback(a, b, G->loginCallbackArg);
    }
}
}
}

namespace F022700e4 {
extern "C" {
void DwcFriend_OnUpdateDone(s32 a, s32 b) {
    s32 t = G->prevState;
    if (t != 4) {
        DwcCore_SetState(t);
    }
    G->updateCallback(a, b, G->updateCallbackArg);
}
}
}

namespace F0226f7c8 {
extern "C" {
void DwcMatch_OnMatchDone(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4) {
    s32 i;
    DwcControl *g;
    DwcControl *h;

    if (a0 == 0 && a1 != 0) {
        if (sDwcControl->matchControl.matchState == 0) {
            DwcMatch_ResetPlayerCounts();
            DwcCore_SetState(3);
        }
    } else if (a0 == 0) {
        DwcCore_SetState(6);
        i = 0;
        g = sDwcControl;
        if (i <= *(volatile u8 *)&g->matchControl.numClients) {
            h = g;
            do {
                if (h->ownProfileId == h->matchControl.memberProfileIds[i]) {
                sDwcControl->myAid = sDwcControl->matchControl.aids[i];
                break;
                }
                i++;
            } while (i <= *(volatile u8 *)&h->matchControl.numClients);
        }
    }
    sDwcControl->matchControl.validAidMask = DwcConn_AidListToBitmap(sDwcControl->matchControl.aids, sDwcControl->matchControl.numClients + 1);
    DwcMatch_UpdateValidAidCount();
    g = sDwcControl;
    if (g->matchControl.matchType == 2 || g->matchControl.matchType == 3) {
        sDwcControl->serverMatchCallback(a0, a1, a2, a3, a4, sDwcControl->serverMatchCallbackArg);
    } else {
        g->matchCallback(a0, a1, g->matchCallbackArg);
    }
    if (a0 != 0 && sDwcControl != 0 && sDwcControl->state == 5) {
        DwcCore_SetState(3);
    }
}
}
}

namespace F0226f7c8 {
extern "C" {
void DwcCore_OnGpError(s32 a, GPErrorArg *b) {
    u32 t = b->errorCode;
    if (t != 0x603 && t != 0x901 && t != 0xb01) {
        DwcCore_HandleGpResult(3);
    }
}
}
}

namespace F0226f7c8 {
extern "C" {
void DwcMatch_OnGpBuddyMessage(GPConnection *a, GPRecvBuddyMessageArg *b, void *c) {
    u8 buf[12] = {0};
    char *s;
    char *e;
    u32 n;
    DwcControl *g;

    s = b->message;
    if (DwcFriend_HandleAuthorizedMessage(a, b) == 0) {
        if (memcmp(s, "GPCM", STD_GetStringLength("GPCM")) == 0) {
            s += STD_GetStringLength("GPCM");
            e = strchr(s, 0x76);
            n = e - s;
            strncpy(buf, s, n);
            if (n <= 10) {
                if (strtoul(buf, 0, 10) == 3) {
                    s += n + 1;
                    if (memcmp(s, "MAT", STD_GetStringLength("MAT")) == 0) {
                        g = sDwcControl;
                        if (g->state != 5) {
                            if (g->state != 6) {
                                goto fin;
                            }
                            if (g->matchControl.matchType != 2 && g->matchControl.matchType != 3) {
                                goto fin;
                            }
                        }
                        char *t = s + STD_GetStringLength("MAT");
                        DwcMatch_OnGpMatchCommand(a, b->profile, t);
                    }
                }
            }
        }
    }
fin:;
}
}
}

namespace F0226f7c8 {
extern "C" {
void DwcConn_OnGt2Receive(void *a, void *b, s32 c, s32 d) {
    DwcNet_OnReceive(a, b, c, d);
}
}
}

namespace F0226f7c8 {
extern "C" {
void DwcConn_OnGt2Closed(void *a0, s32 a1) {
    u32 r5;
    s32 r4;
    s32 r7;
    DwcControl *g;
    u32 v0c = 0;
    u32 v10 = 0;
    volatile BOOL k;
    DwcConnInfo *volatile ent;
    DwcConnInfo *et;

    if (DwcMatch_IsInactive() != 0) {
        return;
    }
    switch (a1) {
    case 0:
    case 1:
        r4 = 0;
        break;
    case 2:
    case 3:
        r4 = 6;
        r7 = -0x1db0;
        break;
    case 4:
        r4 = 8;
        r7 = -0x1db1;
        break;
    }
    if (r4 == 0) {
        et = gt2GetConnectionData(a0);
        ent = et;
        if (et == 0) {
            return;
        }
        r5 = et->aid;
        u32 m = *(volatile u32 *)&sDwcControl->matchControl.validAidMask;
        k = TRUE;
        if ((m & (1 << r5)) == 0) {
            k = FALSE;
        }
        DwcNet_ResetChannel(r5);
        g = sDwcControl;
        if ((g->matchControl.matchType == 2 && a1 == 0) || (g->matchControl.matchType == 3 && r5 == 0)) {
            v10 = 1;
        }
        v0c = DwcConn_RemoveAid(r5);
        sDwcConnTable[ent->slotIndex] = 0;
        sDwcControl->matchControl.numClients--;
        sDwcControl->matchControl.numPlayers--;
    }
    g = sDwcControl;
    if (g->isClosingAll == 0 && g->state == 6 && k == 0) {
        if (g->matchControl.matchType == 2 && r4 == 0) {
            DwcMatch_UpdateServerStatus();
            DwcMatch_OnClientDisconnected(v0c);
            return;
        }
        return;
    }
    if (DwcMatch_OnConnectionClosed(r4, r7, v0c) != 0) {
        return;
    }
    if (r4 != 0) {
        DwcCore_SetError(r4, r7);
        return;
    }
    g = sDwcControl;
    if (g->isClosingAll == 0) {
        if (g->matchControl.matchType != 2 && g->matchControl.matchType != 3) {
            goto skip1;
        }
        DwcControl *h = sDwcControl;
        u32 n = h->matchControl.numClients;
        u32 i = n + 2;
        if (h->matchControl.memberProfileIds[i] != 0) {
            h->matchControl.aids[n + 1] = h->matchControl.aids[i];
            DwcMatch_RemoveMemberAt(sDwcControl->matchControl.numClients + 1, sDwcControl->matchControl.numClients + 3);
        }
    }
skip1:
    g = sDwcControl;
    if (g->matchControl.matchType == 2) {
        if (g->isClosingAll == 0) {
            DwcMatch_UpdateServerStatus();
        } else if (g->matchControl.numClients == 0) {
            DwcFriend_SetOwnStatus(1, "", NULL);
        }
    } else if (g->matchControl.numClients == 0) {
        DwcFriend_SetOwnStatus(1, "", NULL);
    }
    g = sDwcControl;
    if (g->matchControl.matchType != 0 && g->matchControl.matchType != 1) {
    } else {
        sDwcControl->matchControl.maxPlayers = sDwcControl->matchControl.numPlayers;
        qr2_send_statechanged(sDwcControl->matchControl.qr2Object);
    }
    g = sDwcControl;
    if (g->closedCallback != 0 && k != 0) {
        if (a1 == 0) {
            a1 = 1;
        } else {
            a1 = 0;
        }
        sDwcControl->closedCallback(r4, a1, v10, r5, DwcFriend_FindIndexByProfileId(v0c), g->closedCallbackArg);
    }
    g = sDwcControl;
    if (g->isClosingAll == 0 && g->matchControl.matchType == 2) {
        return;
    }
    if (g->matchControl.numClients == 0) {
        NNFreeNegotiateList();
        DwcMatch_ResetPlayerCounts();
        DwcCore_SetState(3);
    }
}
}
}

namespace F0226f7c8 {
extern "C" {
void DwcConn_OnGt2Ping(void *a, s32 b) {
    DwcNet_OnPing(a, b);
}
}
}

namespace F0226f7c8 {
extern "C" {
void DwcConn_OnGt2SocketError(void *socket) {
    gt2GetRemoteIP(socket);
    data_ov065_02290674 = GOAGetLastError();
    DwcCore_SetError(8, -0x17aeb);
    *(u32 *)sDwcControl = 0;
}
}
}

// ---- overlay variables (generated: tools/host_overlay_vars.py) ----
#include "host_overlay_vars.h"

HOST_OVERLAY_VARS(unk_ov065_0226fc18) {
    HOST_OVERLAY_VAR(sDwcControl),
    HOST_OVERLAY_VAR(data_ov065_02290674),
    HOST_OVERLAY_VAR(sDwcConnTable),
    HOST_OVERLAY_VAR(sDwcConnInfo),
    HOST_OVERLAY_VARS_END
};
