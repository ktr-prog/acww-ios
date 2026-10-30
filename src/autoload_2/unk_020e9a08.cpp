// mwcc-flags: -nothumb -O4,p -str reuse
// The network file (WFC / GameSpy stats glue, local wireless; calls into overlays 65/66/67), autoload_2
// 0x020e9a08-0x020ec848, mwcc 1.2/base, C++, ARM, -O4,p, built with -str reuse (the original has one copy of each string
// literal that several functions use: the stats secret, the game name). The four former units unk_020e9a08.cpp (the
// tail of RC_020e92f4), unk_020ea0b4.cpp, unk_020ea34c.cpp (G002c) and unk_020ea960.cpp (G003a) are merged into this
// one file with one set of typed declarations. Its .data (0x0213b058-0x0213b120) and bss (autoload_3
// 0x021f488c-0x021f5974) are defined at the end of the file.
#include "ds_div.h"
#include "host_shared_calls.h"
#include "types.h"
#include "net/HexTable.h"
#include "net/Net.h"
#include "net/WifiPingState.h"
#include "net/DwcControl.h"
#include "net/DwcMatchControl.h"
#include "sys/OSMessageQueue.h"
#include "proto/autoload_2/unk_020e9a08.h"
#include "proto/autoload_2/unk_0212703c.h"
#include "proto/itcm/unk_01ffa4a0.h"
#include "proto/main/unk_0206d5b8.h"
#include "proto/ov065/unk_ov065_0226f9e0.h"
#include "proto/ov065/unk_ov065_0226fc18.h"
#include "proto/ov065/unk_ov065_02270e34.h"
#include "proto/ov065/unk_ov065_02277140.h"
#include "proto/ov065/unk_ov065_02277974.h"
#include "proto/ov065/unk_ov065_02277e70.h"
#include "proto/ov066/unk_ov066_0225f1a0.h"
#include "proto/ov067/unk_ov067_0225f1a0.h"

extern "C" {
// assembly (the SDK's memory and message functions) and units whose generated headers leave these out
void MI_CpuFill8(void *dst, u32 v, u32 n);
void MI_CpuCopy8(const void *src, void *dst, u32 n);
s32 OS_SNPrintf(char *dst, u32 len, const char *fmt, ...);                  // unk_02113088.c
void OS_InitMessageQueue(OSMessageQueue *mq, OSMessage *buf, s32 count);  // unk_02113fd8.c
BOOL OS_SendMessage(OSMessageQueue *mq, OSMessage msg, s32 flags);
BOOL OS_ReceiveMessage(OSMessageQueue *mq, OSMessage *msg, s32 flags);
BOOL OS_ReadMessage(OSMessageQueue *mq, OSMessage *msg, s32 flags);
// DWC user / friend data (unk_020fedcc.c: their types UserData / FD are local to that file)
BOOL DWC_IsBuddyFriendData(void *fd);
u32 DWC_GetFriendDataType(void *fd);
BOOL DWC_CheckFriendKey(void *userData, u64 key);
void DWCi_Acc_LoginIdToUserName(void *fd, u32 gamecode, char *out);
BOOL DWCi_Acc_IsAuthentic(void *userData);
BOOL DWC_IsValidFriendData(void *fd);
void DWC_CreateUserData(void *userData, u32 key);
BOOL DWC_CheckValidConsole(void *userData);
BOOL DWC_CheckDirtyFlag(void *userData);
void DWC_ClearDirtyFlag(void *userData);
u64 DWC_GetFriendKey(void *fd);
u32 DWC_GetGsProfileId(void *userData, void *fd);
u64 DWC_CreateFriendKey(void *userData);
void DWC_CreateFriendKeyToken(void *fd, u32 lo, u32 hi);
void DWC_CreateExchangeToken(void *userData, void *out);
BOOL DWC_IsEqualFriendData(void *a, void *b);
// ov065 functions the generated headers leave out (include/proto/.excluded), as their definitions write them
void DwcInet_Init(DwcInetControl *p);
void DwcNet_SetAllocator(void *(*alloc)(s32, s32, s32), void *(*free)(s32, void *, s32));
s32 DwcMatch_SetOption(s32 m, u8 *p);
BOOL DwcMatch_IsServerLocked(void);
void DwcMatch_ClearServerLock(void);
// ov066
void *LocalWl_GetBeaconGameInfo(LocalWlBeacon *r);
}

// the parameters LocalWlMp_Start reads (ov066: its config message)
struct NetInit {
    u32 ggid;
    u8 maxPeers;
    u8 b5;
    u8 recordSize;
    u8 b7;
};

// a queued send
struct NetSlot {
    u8 *data;
    u32 size;
    u16 aid;  // the target aid or aid mask
    NetSendDoneFn done;
};

// the stats server's secret, which the hashes start with
#define NET_GAMESTATS_SECRET "gOAkBaBav5XHlGyUKOOD"

// (the generated header declares the rest of the file's data)
extern "C" {
extern NetSlot sSendSlots[16];
extern Ent sWifiPingState[20];
extern HexTable data_0213b070;
extern HexTable data_0213b084;
extern const char *data_021f48e4;
extern const char *data_021f48a8;
}

static inline BOOL timedout48ec(void) {
    s64 ms = (OS_GetTick() << 6) / 33514;
    u64 diff = ms - (s64)data_021f48ec;
    BOOL over = diff > (u64)data_0213b060;
    return over;
}

// ---- functions of the former unit unk_020ea960.cpp
extern "C" void Net_WaitFrame(void) {
    Net_Update();
    Main_WaitVBlank();
}

extern "C" void *Net_Alloc(u32 a, u32 b) {
    return sAllocHook(a, b);
}

extern "C" void Net_Free(void *p) {
    sFreeHook(p);
}

extern "C" void *Net_DwcAllocHook(s32 a, s32 b, s32 c) {
    return Net_Alloc(b, c);
}

extern "C" void Net_DwcFreeHook(s32 a, void *b, s32 c) {
    Net_Free(b);
}

extern "C" BOOL Net_SendNextQueued(void) {
    OSMessage msg;
    NetSlot *s;
    u32 st;
    if (OS_ReadMessage(&sSendQueue, &msg, 0) != 0) {
        s = &sSendSlots[(uptr)msg];
        st = sNetMode;
        if ((u8)(st + 255) <= 1) return LocalWl_Send(s->data, s->size, s->aid, Net_OnSendDone);
        if ((u8)(st + 253) <= 1) return DwcNet_SendReliable((u8)s->aid, s->data, s->size);
    }
    return FALSE;
}

extern "C" void Net_OnSendDone(u32 a) {
    OSMessage msg;
    NetSlot *s;
    if (OS_ReceiveMessage(&sSendQueue, &msg, 0) == 0) return;
    s = &sSendSlots[(uptr)msg];
    if (s->done != NULL) s->done(a);
    s->data = NULL;
    while (OS_ReadMessage(&sSendQueue, &msg, 0) != 0 && Net_SendNextQueued() == 0) {
        Net_WaitFrame();
    }
}

extern "C" void Net_ClearSendQueue(void) {
    OSMessage msg;
    u32 i;
    while (OS_ReceiveMessage(&sSendQueue, &msg, 0) != 0) {
    }
    for (i = 0; i < 16; i++) {
        sSendSlots[i].data = NULL;
    }
}

extern "C" BOOL Net_QueueSend(u8 *data, u32 size, u32 aid, NetSendDoneFn done) {
    u32 i; BOOL r;
    i = r = 0;
    for (; i < 16; i++) {
        if (sSendSlots[i].data == NULL) {
            sSendSlots[i].data = data;
            sSendSlots[i].size = size;
            sSendSlots[i].aid = aid;
            sSendSlots[i].done = done;
            OS_SendMessage(&sSendQueue, (OSMessage)(uptr)i, 0);
            r = TRUE;
            break;
        }
    }
    return r;
}

extern "C" BOOL Net_QueueSendChecked(u8 *data, u32 size, u32 aid, NetSendDoneFn done) {
    if (data != NULL && size != 0 && aid != 0) return Net_QueueSend(data, size, aid, done);
    return FALSE;
}

extern "C" BOOL Net_QueueSendPerAid(u8 *data, u32 size, u32 mask, NetSendDoneFn done) {
    u32 i;
    if (data != NULL && size != 0 && mask != 0) {
        for (i = 0; i < 16; i++) {
            if ((mask & (1 << i)) != 0) {
                if (Net_QueueSend(data, size, (u16)i, done) == 0) return FALSE;
            }
        }
        return TRUE;
    }
    return FALSE;
}

extern "C" BOOL Net_QueueSendToMask(u8 *data, u32 size, u16 mask, NetSendDoneFn done) {
    u32 st = sNetMode;
    if ((u8)(st + 255) <= 1) return Net_QueueSendChecked(data, size, mask, done);
    if ((u8)(st + 253) > 1) return FALSE;
    return Net_QueueSendPerAid(data, size, mask, done);
}

extern "C" void Net_OnLocalPeerEvent(void *ev) {
    u8 *p = (u8 *)ev;
    switch (p[0]) {
    case 0:
        sLocalPeerState[p[1]] = 1;
        break;
    case 1:
        sLocalPeerState[p[1]] = 0;
        break;
    case 2:
        sLocalPeerState[0] = 0xff;
        break;
    }
}

extern "C" void Net_OnLocalRecv(u32 aid, u8 *data, u32 size) {
    sRecvCallback(aid, data, size);
}

extern "C" void Net_OnWifiClientMatched(s32 result, s32 b, s32 c, s32 d, s32 e, void *param) {
    if (result == 0) data_0213b06c = 0;
}

extern "C" void Net_OnWifiClosedNop(s32 error, s32 isLocal, s32 isServer, s32 aid, s32 index, s32 param) {
}

extern "C" void Net_OnWifiSendDone(u32 size, u32 aid) {
    sWifiPingState[aid].b = 0;
    Net_OnSendDone(size);
}

extern "C" void Net_OnWifiRecv(u32 aid, u8 *data, u32 size) {
    sWifiPingState[aid].b = 0;
    sRecvCallback(aid, data, size);
}

extern "C" void Net_OnWifiHostMatched(s32 a, s32 b, s32 c, s32 d, s32 e, void *param) {
    if (a != 0) return;
    if (b != 0) return;
    data_021f48cc = (u32)((OS_GetTick() << 6) / 33514);
}

extern "C" void Net_WifiCallbackNop(s32 index, void *param) {
}

extern "C" void Net_OnWifiServersUpdated(s32 result, s32 b, s32 arg) {
    s32 local;
    u32 i;
    if (result != 0) return;
    data_0213b06c = 0;
    for (i = 0; i < 32; i++) {
        if (DWC_IsValidFriendData(&sWifiFriendList->friends[i]) != 0) {
            NetWifiFriendStatus *st = &sWifiFriendList->status[i];
            st->status = DwcFriend_GetStatusData(&sWifiFriendList->friends[i], &st->b, &st->c, st->statusString, &local);
        }
    }
}

extern "C" void Net_OnWifiFriendStatus(s32 index, s32 status, char *statusString, s32 param) {
    u32 a = index;
    s32 local;
    NetWifiFriendStatus *st = &sWifiFriendList->status[a];
    st->status = DwcFriend_GetStatusData(&sWifiFriendList->friends[a], &st->b, &st->c, st->statusString, &local);
}

extern "C" void Net_OnWifiFriendDeleted(s32 deletedIndex, s32 srcIndex, s32 param) {
    u32 a = deletedIndex;
    MI_CpuFill8(&sWifiFriendList->friends[a], 0, sizeof(NetWifiFriendData));
    MI_CpuFill8(&sWifiFriendList->status[a], 0, sizeof(NetWifiFriendStatus));
    sWifiFriendList->status[a].status = 0;
    if (data_021f48c8 != NULL) data_021f48c8(a);
}

extern "C" void Net_OnWifiLogin(s32 result, s32 b, s32 arg) {
    if (result != 0) return;
    DwcFriend_SetStatusData(sWifiUserData, 16);  // (the name and town)
    DwcFriend_UpdateServersAsync(NULL, Net_OnWifiServersUpdated, arg, Net_OnWifiFriendStatus, arg,
                                 Net_OnWifiFriendDeleted, arg);
}

extern "C" void Net_OnGameStatsUploadDone(char *data, s32 len, s32 result, void *ud) {
    data_0213b064 = 1;
}

// (the HTTP callbacks get the received data and the request's user data: the destination buffer)
extern "C" void Net_OnGameStatsDownloadDone(char *data, s32 len, s32 result, void *ud) {
    if (data != NULL && len != 0 && result == 0) {
        NasBase64_Decode((s8 *)data, len, (u8 *)ud, data_021f48ac);
    }
    data_0213b068 = 1;
}

extern "C" void Net_OnHttpDownloadDone(char *data, s32 len, s32 result, void *ud) {
    if (data != NULL && len != 0 && result == 0) {
        MI_CpuCopy8(data, ud, data_021f48ac);
    }
    data_0213b068 = 1;
}

// the server's challenge: the hash of the secret and the challenge, hex-encoded, goes into the download request
extern "C" void Net_OnGameStatsChallenge(char *challenge, s32 len, s32 result, void *ud) {
    u32 i;
    if (challenge != NULL && len != 0 && result == 0) {
        char *d = (char *)ud;
        STD_CopyString(d, NET_GAMESTATS_SECRET);
        STD_ConcatenateString(d, challenge);
        MATH_CalcSHA1(data_021f48a4 + 20, d, len + STD_GetStringLength(NET_GAMESTATS_SECRET));
        HexTable hex = data_0213b070;
        u8 *src = (u8 *)data_021f48a4 + 20;
        for (i = 0; i < 20; i++) {
            ((HexPair *)data_021f48a4)[i].hi = hex.c[src[i] >> 4];
            ((HexPair *)data_021f48a4)[i].lo = hex.c[src[i] & 15];
        }
        data_021f48a4[40] = 0;
        OS_SNPrintf(data_021f48dc, 0x100, "%s?pid=%llu&hash=%s&region=%s", data_021f48e4, Net_GetOwnFriendKey(&sWifiUserData->userData), data_021f48a4, data_021f48a8);
        DwcGsHttp_Get(data_021f48dc, Net_OnGameStatsDownloadDone, ud);
    } else {
        data_0213b068 = 1;
    }
}

extern "C" void Net_OnWlxStopped(s32 event, void *arg) {
    if (event == 0) data_021f48e0 = 1;
}

extern "C" void Net_OnWlxExchangeDone(void) {
    data_021f48e8 = 1;
}

extern "C" void Net_OnWifiPingReply(u32 a, u32 i) {
    sWifiPingState[i].a = a;
    sWifiPingState[i].b = 0;
}

extern "C" void Net_WifiPingNextPeer(void) {
    u64 ms;
    u8 v;
    if (sWifiConnectStep < 5) return;
    ms = (OS_GetTick() << 6) / 33514;
    ms = ms / 250;
    v = (u8)(DS_UMOD64(ms, data_021f488c));
    if (data_0213b058 == v) return;
    data_0213b058 = v;
    if (v != DwcConn_GetMyAid()) {
        if (DwcConn_IsAidValid(v) != 0) {
            sWifiPingState[v].b++;
            DwcNet_PingAid(v);
        } else {
            sWifiPingState[v].a = 0xffff;
            sWifiPingState[v].b = 0;
        }
    } else {
        sWifiPingState[v].a = 0;
        sWifiPingState[v].b = 0;
    }
}

extern "C" void Net_WifiLockServer(void) {
    u32 v;
    data_021f48cc = 0;
    if (sNetMode != 3) return;
    v = 1;
    DwcMatch_SetOption(1, (u8 *)&v);
}

extern "C" void Net_WifiCheckHostIdle(void) {
    if (sNetMode != 3) return;
    if (DwcMatch_IsServerLocked() == 0) return;
    if (data_021f48cc == 0) return;
    {
        s64 ms = (OS_GetTick() << 6) / 33514;
        u64 diff = ms - (s64)data_021f48cc;
        BOOL over = diff > (u64)120000;
        if (!over) return;
    }
    data_021f48cc = 0;
    DwcMatch_ClearServerLock();
}

extern "C" void Net_OnHeapCreatedNop(void) {
}

extern "C" void Net_OnHeapDestroyNop(void) {
}

// gameCode: the DWC game code; ggid: the local wireless group id; dmaNo: the wireless driver's DMA channel; timeout in
// ticks; maxPeers; the allocator
extern "C" void Net_Init(u32 gameCode, u32 ggid, u32 dmaNo, u64 timeout, u8 maxPeers, NetAllocFn alloc, NetFreeFn free) {
    sNetMode = 0;
    data_021f489c = gameCode;
    data_021f48b8 = ggid;
    data_0213b05c = dmaNo;
    data_0213b060 = (u32)((timeout << 6) / 33514);
    data_021f488c = maxPeers;
    sAllocHook = alloc;
    sFreeHook = free;
    OS_InitMessageQueue(&sSendQueue, data_021f4950, 16);
    MI_CpuFill8(sSendSlots, 0, sizeof(sSendSlots));
}

extern "C" void Net_Update(void) {
    if ((u8)(sNetMode + 253) > 1) return;
    if (sWifiConnectStep == 1) {
        DwcInet_Process();
        return;
    }
    DwcCore_Process();
    DwcGsHttp_Process();
    Net_WifiPingNextPeer();
    Net_WifiCheckHostIdle();
}

extern "C" void Net_StartLocal(u32 mode, NetRecvFn recv) {
    NetInit init;
    sRecvCallback = recv;
    sNetMode = mode;
    MI_CpuFill8(sLocalPeerState, 0, 16);
    init.ggid = data_021f48b8;
    init.maxPeers = data_021f488c;
    init.recordSize = 60;
    init.b7 = 2;
    init.b5 = 8;
    LocalWl_Init(data_0213b05c, Net_Alloc, Net_Free, NULL);
    Shared_LocalWl_SetEventCallback((void *)Net_OnLocalPeerEvent);
    LocalWlMp_Start((u8 *)&init);
    LocalWl_SetRecvCallback(Net_OnLocalRecv);
    if (mode == 1) {
        LocalWl_RequestMode(3, 0, 0);
    } else if (mode == 2) {
        LocalWl_RequestMode(4, 0, 0);
    }
}

extern "C" void Net_StartWifi(u32 mode, NetRecvFn recv, NetFriendFn friendDeleted, u8 *userData, u8 *friendList) {
    u32 i;
    sNetMode = mode;
    sRecvCallback = recv;
    sWifiUserData = (NetWifiUserData *)userData;
    data_021f48c8 = friendDeleted;
    sWifiFriendList = (NetWifiFriends *)friendList;
    sWifiConnectStep = 0;
    sWifiShutdownStep = 0;
    data_0213b064 = 0;
    data_0213b068 = 1;
    data_021f48b4 = NULL;
    data_021f48c4 = NULL;
    data_021f48dc = NULL;
    data_021f48e4 = NULL;
    data_021f48ac = 0;
    data_021f48a8 = NULL;
    data_021f48a4 = NULL;
    data_021f48cc = 0;
    MI_CpuFill8(sWifiPingState, 0, 16 * sizeof(Ent));
    for (i = 0; i < 32; i++) {
        sWifiFriendList->status[i].status = 0;
    }
    // (the free hook's result: whatever Net_Free leaves, as on the DS)
    DwcNet_SetAllocator(Net_DwcAllocHook, (void *(*)(s32, void *, s32))Net_DwcFreeHook);
}

extern "C" BOOL Net_IsLocalConnected(void) {
    return (u32)(LocalWl_GetState() - 10) <= 1;
}

extern "C" BOOL Net_WifiConnectStep(void) {
    switch (sWifiConnectStep) {
    case 0:
        DwcInet_Init((DwcInetControl *)data_021f48f4);
        DwcInet_SelectAuthServer(2);
        DwcInet_StartConnect();
        sWifiConnectStep = 1;
        break;
    case 1:
        if (DwcInet_IsConnectDone() != 0) {
            if (DwcInet_UpdateStatus() == 4) sWifiConnectStep = 2;
        }
        break;
    case 2:
        DwcGsHttp_Startup();  // (the original passes the game name, which it doesn't use)
        data_0213b064 = 1;
        data_0213b06c = 1;
        // (the game's friend list entries as DWC's friend data: both the SDK's 12-byte DWCFriendData)
        DwcCore_Init((DwcControl *)data_021f4bc0, &sWifiUserData->userData, 0x299e, "acrossingds", "h2P9x6", 0, 0,
                     (DwcFriendData *)sWifiFriendList->friends, 32);
        DwcLogin_Start(0, 0, Net_OnWifiLogin, 0);
        sWifiConnectStep = 3;
        break;
    case 3:
        if (data_0213b06c == 0) {
            data_0213b06c = 1;
            sWifiConnectStep = 4;
        }
        break;
    case 4:
        if (sNetMode == 3) data_0213b06c = 0;
        if (data_0213b06c == 0) {
            DwcNet_SetMaxChunkSize(0x100);
            DwcNet_SetPingCallback((void *)Net_OnWifiPingReply);
            Net_WifiLockServer();
            sWifiConnectStep = 5;
        }
        break;
    case 5:
        if (sNetMode == 4) {
            if ((Net_GetConnectedMask() & 1) == 0) return FALSE;
        }
        return TRUE;
    }
    return FALSE;
}

extern "C" BOOL Net_PollConnected(void) {
    u32 st = sNetMode;
    if ((u8)(st + 255) <= 1) return Net_IsLocalConnected();
    if ((u8)(st + 253) > 1) return FALSE;
    return Net_WifiConnectStep();
}

extern "C" BOOL Net_LocalShutdown(void) {
    u32 start;
    LocalWl_RequestMode(0, 0, 0);
    start = (u32)((OS_GetTick() << 6) / 33514);
    while (LocalWl_GetState() != 2) {
        s64 ms = (OS_GetTick() << 6) / 33514;
        u64 diff = ms - (s64)start;
        BOOL over = diff > (u64)data_0213b060;
        if (over != 0) return FALSE;
    }
    LocalWlMp_Shutdown();
    if (LocalWl_Finish() == 0) return FALSE;
    sNetMode = 6;
    return TRUE;
}

extern "C" BOOL Net_WifiShutdownStep(void) {
    switch (sWifiShutdownStep) {
    case 0:
        data_021f48ec = (u32)((OS_GetTick() << 6) / 33514);
        sWifiShutdownStep = 1;
        break;
    case 1:
        if (data_0213b06c == 0 || sWifiConnectStep == 4 || timedout48ec()) {
            Net_GetError();
            if (DwcConn_CloseAllConnections() < 0) {
                data_021f48ec = 0;
            } else {
                data_021f48ec = (u32)((OS_GetTick() << 6) / 33514);
            }
            sWifiShutdownStep = 2;
        }
        break;
    case 2:
        if (DwcConn_GetConnectionCount() < 2) {
            sWifiShutdownStep = 3;
        } else if (timedout48ec()) {
            sWifiShutdownStep = 3;
        }
        break;
    case 3:
        DwcGsHttp_Cleanup();
        sWifiShutdownStep = 4;
        break;
    case 4:
        DwcCore_Shutdown();
        sWifiShutdownStep = 5;
        break;
    case 5:
        if (DwcInet_Disconnect() != 0) sWifiShutdownStep = 6;
        break;
    case 6:
        if (data_021f48b4 != NULL) {
            Net_Free(data_021f48b4);
            Net_Free(data_021f48c4);
            data_021f48b4 = NULL;
            data_021f48c4 = NULL;
        }
        if (data_021f48dc != NULL) {
            Net_Free(data_021f48dc);
            Net_Free(data_021f48a4);
            data_021f48dc = NULL;
            data_021f48a4 = NULL;
        }
        sWifiShutdownStep = 7;
        break;
    case 7:
        return TRUE;
    }
    return FALSE;
}

extern "C" BOOL Net_ShutdownOv067(void) {
    Shared_Wlx_Stop();
    while (data_021f48e0 == 0) {
        Net_WaitFrame();
    }
    Net_Free(data_021f48c0);
    data_021f48c0 = NULL;
    sNetMode = 6;
    return TRUE;
}

extern "C" BOOL Net_Shutdown(void) {
    u32 st = sNetMode;
    if ((u8)(st + 255) <= 1) return Net_LocalShutdown();
    if (st == 3 || st == 4) {
        while (Net_WifiShutdownStep() == 0) {
            Net_WaitFrame();
        }
        sNetMode = 6;
        return TRUE;
    }
    if (st != 5) return FALSE;
    return Net_ShutdownOv067();
}

extern "C" BOOL Net_WifiShutdownStepExt(void) {
    return Net_WifiShutdownStep();
}

extern "C" u32 Net_LocalGetLinkLevel(void) {
    switch (LocalWl_GetLinkLevel()) {
    case 0:
        break;
    case 1:
        return 1;
    case 2:
        return 2;
    case 3:
        return 3;
    }
    return 0;
}

extern "C" u32 Net_WifiGetLinkLevel(void) {
    if (sWifiConnectStep <= 1) return 0;
    return DwcInet_GetLinkLevel();
}

extern "C" BOOL Net_GetLinkLevel(void) {
    u32 st = sNetMode;
    if ((u8)(st + 255) <= 1) return Net_LocalGetLinkLevel();
    if ((u8)(st + 253) > 1) return FALSE;
    return Net_WifiGetLinkLevel();
}

extern "C" BOOL Net_SetRecvBuffer(u32 aid, u8 *buf, u32 size) {
    u32 st = sNetMode;
    if ((u8)(st + 255) <= 1) return LocalWlMp_SetRecvBuffer(aid, buf, size);
    if ((u8)(st + 253) > 1) return FALSE;
    return DwcNet_SetRecvBuffer((u8)aid, buf, size);
}

extern "C" u32 Net_GetMemberCount(void) {
    u32 st = sNetMode;
    if ((u8)(st + 255) <= 1) return LocalWl_GetMemberCount();
    if ((u8)(st + 253) > 1) return TRUE;
    return (u8)DwcConn_GetConnectionCount();
}

extern "C" u32 Net_GetMyAid(void) {
    u32 st = sNetMode;
    if ((u8)(st + 255) <= 1) {
        if (st == 1) return FALSE;
        return LocalWl_GetMyAid();
    }
    if ((u8)(st + 253) > 1) return FALSE;
    return DwcConn_GetMyAid();
}

extern "C" u32 Net_GetConnectedMask(void) {
    u32 st = sNetMode;
    if ((u8)(st + 255) <= 1) return LocalWl_GetConnectedMask();
    if ((u8)(st + 253) > 1) return TRUE;
    return (u16)DwcConn_GetAidBitmap();
}

extern "C" u32 Net_GetMode(void) {
    return sNetMode;
}

extern "C" s32 Net_LocalCountBeacons(void) {
    if (LocalWl_GetState() != 7) return -1;
    return LocalWl_GetBeaconCount(0);
}

extern "C" s32 Net_WifiCountFriends(void) {
    return DwcFriend_CountValid(sWifiFriendList->friends[0].data, 32);
}

extern "C" s32 Net_CountHostCandidates(void) {
    u32 st = sNetMode;
    if (st == 2) return Net_LocalCountBeacons();
    if (st != 4) return -1;
    return Net_WifiCountFriends();
}

extern "C" BOOL Net_IsSendIdle(u32 a) {
    OSMessage tmp;
    if (a != 0) {
        if (OS_ReadMessage(&sSendQueue, &tmp, 0) != 0) {
            if (data_021f48b0 != 0) {
                s64 ms = (OS_GetTick() << 6) / 33514;
                u64 diff = ms - (s64)data_021f48b0;
                BOOL over = diff > (u64)data_0213b060;
                if (over) {
                    data_021f48b0 = 0;
                    Net_SendNextQueued();
                }
            } else {
                data_021f48b0 = (u32)((OS_GetTick() << 6) / 33514);
            }
        } else {
            data_021f48b0 = 0;
            return TRUE;
        }
    } else {
        data_021f48b0 = 0;
    }
    return FALSE;
}

extern "C" BOOL Net_WifiAllPeersSendable(void) {
    u8 *buf;
    u32 i;
    u32 n;
    n = DwcConn_GetAidList(&buf);
    i = 0;
    for (; i < n; i++) {
        u32 c = buf[i];
        if (c != DwcConn_GetMyAid()) {
            if (DwcNet_CanSendReliable(c) == 0) return FALSE;
        }
    }
    return TRUE;
}

extern "C" BOOL Net_IsReadyToSend(void) {
    u32 st = sNetMode;
    if ((u8)(st + 255) <= 1) {
        return Net_IsSendIdle(LocalWl_IsReadyToSend());
    }
    if ((u8)(st + 253) > 1) return FALSE;
    return Net_IsSendIdle(Net_WifiAllPeersSendable());
}

// three packets (data, size, target aid or aid mask, done callback each)
extern "C" BOOL Net_SendPackets3(u8 *a0, u32 b0, u16 c0, NetSendDoneFn d0, u8 *a1, u32 b1, u16 c1, NetSendDoneFn d1,
                                 u8 *a2, u32 b2, u16 c2, NetSendDoneFn d2) {
    if (Net_PollConnected() != 0) {
        if (Net_IsReadyToSend() != 0) {
            BOOL r5 = Net_QueueSendToMask(a0, b0, c0, d0);
            BOOL r4 = Net_QueueSendToMask(a1, b1, c1, d1);
            BOOL r = Net_QueueSendToMask(a2, b2, c2, d2);
            if (r5 != 0 || r4 != 0 || r != 0) {
                if (Net_SendNextQueued() != 0) return TRUE;
                Net_ClearSendQueue();
            }
        }
    }
    return FALSE;
}

extern "C" u32 Net_GetLocalError(void) {
    s32 st = LocalWl_GetState();
    if ((st & 0x80) != 0) {
        switch (st & ~0x80) {
        case 0:
            return 0;
        case 12:
            return 0x800c;
        case 1:
            return 0x8001;
        case 2:
            return 0x8002;
        case 3:
            return 0x8003;
        case 4:
            return 0x8004;
        case 5:
            return 0x8005;
        case 6:
            return 0x8006;
        case 7:
            return 0x8007;
        case 8:
            return 0x8008;
        case 9:
            return 0x8009;
        case 10:
            return 0x800a;
        case 11:
            return 0x800b;
        case 13:
            return 0x800d;
        case 14:
            return 0x800e;
        case 15:
            return 0x800f;
        case 16:
            return 0x8010;
        case 65:
            return 0x8041;
        case 66:
            return 0x8042;
        case 67:
            return 0x8043;
        case 68:
            return 0x8044;
        default:
            return 0xffff;
        }
    }
    if (sLocalPeerState[0] == 0xff) return 0x80ff;
    return 0;
}

// ---- functions of the former unit unk_020ea34c.cpp
extern "C" u32 Net_GetWifiError(void) {
    u32 err = DwcCore_GetLastError(&sLastErrorCode);
    u32 i;
    if (err != 0) {
        switch (err) {
        case 1:
            return 0x4001;
        case 2:
            return 0x4002;
        case 3:
            return 0x4003;
        case 4:
            return 0x4004;
        case 5:
            return 0x4005;
        case 6:
            return 0x4006;
        case 9:
            return 0x4009;
        case 10:
            return 0x400a;
        case 7:
            return 0x4007;
        case 11:
            return 0x400b;
        case 8:
            return 0x4008;
        default:
            return 0xffff;
        }
    }
    for (i = 0; i < 16; i++) {
        if (sWifiPingState[i].b > 30) {
            sLastErrorCode = 1000000;
            return 0x4007;
        }
    }
    return 0;
}

extern "C" u32 Net_GetError(void) {
    u32 st = sNetMode;
    sLastErrorCode = 0;
    if ((u8)(st + 255) <= 1) return Net_GetLocalError();
    if ((u8)(st + 253) <= 1) return Net_GetWifiError();
    if (st != 5) return 0xffff;
    return 0;
}

extern "C" u32 Net_GetLastErrorCode(void) {
    return sLastErrorCode;
}

// (DwcCore_ClearError has no result; the callers don't use one)
extern "C" void Net_ClearWifiError(void) {
    DwcCore_ClearError();
}

extern "C" s32 Net_SetLocalGameInfo(void *a, u32 n) {
    return LocalWl_SetGameInfo(a, n);
}

extern "C" void *Net_GetBeaconGameInfo(void *beacon) {
    if (beacon == NULL) return NULL;
    return LocalWl_GetBeaconGameInfo((LocalWlBeacon *)beacon);
}

extern "C" s32 Net_GetBeaconGameInfoSize(void *beacon) {
    if (beacon == NULL) return 0;
    return LocalWl_GetBeaconGameInfoSize((LocalWlBeacon *)beacon);
}

// the valid beacons found (LocalWlBeacon *), up to 8, NULL after the last
extern "C" void **Net_GetScanResults(void) {
    u32 i;
    u32 n;
    MI_CpuFill8(data_021f4910, 0, sizeof(data_021f4910));
    if (LocalWl_GetState() == 7) {
        n = i = 0;
        for (; i < 8; i++) {
            void *r = LocalWl_GetBeacon(0, i & 0xff);
            if (LocalWl_IsBeaconValid((WMMsg *)r) != 0) data_021f4910[n++] = r;
        }
    }
    return data_021f4910;
}

// (a copy of the beacon: LocalWlBeacon's size, 0xe0 bytes on the DS, unk_ov066_0225f1a0.cpp)
extern "C" s32 Net_ConnectToParent(void *p) {
    if (LocalWl_GetState() == 7 && p != NULL) {
        MI_CpuCopy8(p, data_021f49e0, sizeof data_021f49e0);
        return LocalWl_ConnectToParent((LocalWlBeacon *)data_021f49e0, 0, 0);
    }
    return 0;
}

extern "C" s32 Net_WifiFindFriend(u32 a) {
    if (sWifiConnectStep < 4) return -1;
    return DwcFriend_FindIndexByProfileId(a);
}

extern "C" s32 Net_WifiGetFriendProfileId(u32 a) {
    if (sWifiConnectStep < 4) return -1;
    return Shared_DwcFriend_GetProfileId(a);
}

extern "C" u8 *Net_GetWifiFriendList(void) {
    if (sWifiConnectStep < 4) return NULL;
    return sWifiFriendList->friends[0].data;
}

extern "C" BOOL Net_WifiStartHost(void) {
    if (sWifiConnectStep < 4) return FALSE;
    sNetMode = 3;
    DwcMatch_SetupGameServer(data_021f488c, Net_OnWifiHostMatched, NULL, Net_WifiCallbackNop, NULL);
    DwcNet_SetSendDoneCallback((void *)Net_OnWifiSendDone);
    DwcNet_SetRecvCallback((void *)Net_OnWifiRecv);
    DwcConn_SetClosedCallback(Net_OnWifiClosedNop, 0);
    return TRUE;
}

extern "C" BOOL Net_WifiConnectToHost(u32 a) {
    u32 r;
    if (sWifiConnectStep != 4) return FALSE;
    sNetMode = 4;
    r = Net_WifiFindFriend(a);
    if (r == (u32)-1) return FALSE;
    DwcMatch_ConnectToFriendServer(r, Net_OnWifiClientMatched, NULL, Net_WifiCallbackNop, NULL);
    DwcNet_SetSendDoneCallback((void *)Net_OnWifiSendDone);
    DwcNet_SetRecvCallback((void *)Net_OnWifiRecv);
    DwcConn_SetClosedCallback(Net_OnWifiClosedNop, 0);
    return TRUE;
}

extern "C" void Net_CreateUserData(void *p, u32 v) {
    DWC_CreateUserData(p, v);
    DWC_ClearDirtyFlag(p);
}

extern "C" BOOL Net_CheckUserDataChanged(void *p) {
    if (DWC_CheckDirtyFlag(p) == 0) return FALSE;
    DWC_ClearDirtyFlag(p);
    return TRUE;
}

extern "C" BOOL Net_HasWifiUserId(void *p) {
    return DWCi_Acc_IsAuthentic(p);
}

// (DWC_CreateExchangeToken has no result; the callers don't use one)
extern "C" void Net_MakeOwnFriendData(void *p, void *q) {
    DWC_CreateExchangeToken(p, q);
}

extern "C" s64 Net_GetOwnFriendKey(void *p) {
    return DWC_CreateFriendKey(p);
}

extern "C" BOOL Net_FriendKeyToFriendData(void *userData, void *out, u64 key) {
    if (DWC_CheckFriendKey(userData, key) != 0) {
        DWC_CreateFriendKeyToken(out, (u32)key, (u32)(key >> 32));
        if ((s32)DWC_GetGsProfileId(userData, out) > 0) return TRUE;
    }
    return FALSE;
}

extern "C" u64 Net_GetFriendKey(void *friendData) {
    return DWC_GetFriendKey(friendData);
}

// ---- functions of the former unit unk_020ea0b4.cpp
// uploads size bytes at data to url: base64 (after the secret, which the hash covers), the hash, the friend key and
// the region
extern "C" BOOL Net_GameStatsUpload(const char *url, void *data, u32 size, const char *region) {
    GHIPost *builder;
    HexTable hex;
    char buf[16];
    u32 sz;
    u32 enc;
    u32 len;
    u8 *dg;
    u32 i;
    if (sWifiConnectStep < 4) return FALSE;
    if (data_0213b064 != 0) {
        enc = ((size + 2) / 3) * 4 + 1;
        sz = STD_GetStringLength(NET_GAMESTATS_SECRET);
        data_021f48b4 = (char *)Net_Alloc(sz + enc, 4);
        if (data_021f48b4 == NULL) return FALSE;
        data_021f48c4 = (char *)Net_Alloc(0x29, 4);
        if (data_021f48c4 == NULL) {
            Net_Free(data_021f48b4);
            data_021f48b4 = NULL;
            return FALSE;
        }
        STD_CopyString(data_021f48b4, NET_GAMESTATS_SECRET);
        len = NasBase64_Encode((u8 *)data, size, (u8 *)data_021f48b4 + sz, enc);
        MATH_CalcSHA1(data_021f48c4 + 0x14, data_021f48b4, sz + len);
        hex = data_0213b084;
        dg = (u8 *)data_021f48c4 + 0x14;
        for (i = 0; i < 20; i++) {
            ((HexPair *)data_021f48c4)[i].hi = hex.c[dg[i] >> 4];
            ((HexPair *)data_021f48c4)[i].lo = hex.c[dg[i] & 15];
        }
        data_021f48c4[0x28] = 0;
        data_0213b064 = 0;
        MI_CpuFill8(buf, 0, 16);
        OS_SNPrintf(buf, 16, "%llu", Net_GetOwnFriendKey(&sWifiUserData->userData));
        DwcGsHttp_PostCreate(&builder);
        DwcGsHttp_PostAddString(&builder, "pid", buf);
        DwcGsHttp_PostAddString(&builder, "hash", data_021f48c4);
        DwcGsHttp_PostAddString(&builder, "data", data_021f48b4 + 0x14);
        DwcGsHttp_PostAddString(&builder, "region", region);
        DwcGsHttp_Post(url, &builder, Net_OnGameStatsUploadDone, NULL);
        return TRUE;
    }
    return FALSE;
}

// ---- functions of the former unit unk_020e9a08.cpp
extern "C" BOOL Net_IsUploadDone(s32 a) {
    if (sWifiConnectStep < 4) return FALSE;
    if (data_0213b064 != 0) {
        if (data_021f48b4 != NULL) {
            Net_Free(data_021f48b4);
            Net_Free(data_021f48c4);
            data_021f48b4 = NULL;
            data_021f48c4 = NULL;
        }
        return TRUE;
    }
    return FALSE;
}

// downloads size bytes from url into buf: a challenge first (Net_OnGameStatsChallenge), then the data
extern "C" BOOL Net_GameStatsDownload(const char *url, void *buf, u32 size, const char *region) {
    if (sWifiConnectStep < 4) return FALSE;
    if (data_0213b068 != 0) {
        data_021f48dc = (char *)Net_Alloc(0x100, 4);
        if (data_021f48dc == NULL) return FALSE;
        data_021f48a4 = (char *)Net_Alloc(0x29, 4);
        if (data_021f48a4 == NULL) {
            Net_Free(data_021f48dc);
            data_021f48dc = NULL;
            return FALSE;
        }
        data_0213b068 = 0;
        data_021f48e4 = url;
        data_021f48ac = size;
        data_021f48a8 = region;
        OS_SNPrintf(data_021f48dc, 0x100, "%s?pid=%llu&region=%s", data_021f48e4, Net_GetOwnFriendKey(&sWifiUserData->userData), region);
        DwcGsHttp_Get(data_021f48dc, Net_OnGameStatsChallenge, buf);
        return TRUE;
    }
    return FALSE;
}

// (region: unused)
extern "C" BOOL Net_HttpDownload(const char *url, void *buf, u32 size, const char *region) {
    if (sWifiConnectStep < 4) return FALSE;
    if (data_0213b068 == 0) return FALSE;
    data_021f48ac = size;
    data_0213b068 = 0;
    DwcGsHttp_Get(url, Net_OnHttpDownloadDone, buf);
    return TRUE;
}

extern "C" BOOL Net_IsDownloadDone(s32 a) {
    if (sWifiConnectStep < 4) return FALSE;
    if (data_0213b068 != 0) {
        if (data_021f48dc != NULL) {
            Net_Free(data_021f48dc);
            Net_Free(data_021f48a4);
            data_021f48dc = NULL;
            data_021f48a4 = NULL;
        }
        return TRUE;
    }
    return FALSE;
}

extern "C" BOOL Net_IsWifiConfigValid(void *p) {
    return DWC_CheckValidConsole(p);
}

extern "C" BOOL Net_IsSameFriendData(void *p, void *q) {
    return DWC_IsEqualFriendData(p, q);
}

extern "C" u32 Net_GetFriendDataType(void *p) {
    return DWC_GetFriendDataType(p);
}

extern "C" BOOL Net_IsBuddyFriendData(void *p) {
    return DWC_IsBuddyFriendData(p);
}

extern "C" s32 Net_WifiAddFriend(u32 a, void *b) {
    s32 r;
    if (Shared_DwcFriend_IsIdle() == 0 || sWifiConnectStep < 5 || data_0213b06c != 0) return 0;
    MI_CpuCopy8(b, &sWifiFriendList->friends[a], sizeof(NetWifiFriendData));
    MI_CpuFill8(&sWifiFriendList->status[a], 0, sizeof(NetWifiFriendStatus));
    sWifiFriendList->status[a].status = 0;
    data_0213b06c = 1;
    r = DwcFriend_UpdateServersAsync(NULL, Net_OnWifiServersUpdated, 0, Net_OnWifiFriendStatus, 0,
                                     Net_OnWifiFriendDeleted, 0);
    if (r == 0) data_0213b06c = 0;
    return r;
}

extern "C" BOOL Net_WifiDeleteFriend(u32 a) {
    if (Shared_DwcFriend_IsIdle() == 0 || sWifiConnectStep < 5 || data_0213b06c != 0) return FALSE;
    DwcFriend_DeleteFriend(&sWifiFriendList->friends[a]);
    MI_CpuFill8(&sWifiFriendList->friends[a], 0, sizeof(NetWifiFriendData));
    MI_CpuFill8(&sWifiFriendList->status[a], 0, sizeof(NetWifiFriendStatus));
    sWifiFriendList->status[a].status = 0;
    return TRUE;
}

extern "C" BOOL Net_WifiHostKeepAlive(void) {
    if (sNetMode == 3) {
        data_021f48cc = 0;
        DwcMatch_ClearServerLock();
    }
    return TRUE;
}

// the 12 characters of the player's login name after its first 9 (out: 12 bytes)
extern "C" BOOL Net_GetBrid(u8 *out) {
    char buf[24];
    if (sWifiConnectStep < 4) return FALSE;
    DWCi_Acc_LoginIdToUserName(&sWifiUserData->userData.loginId, data_021f489c, buf);
    MI_CpuCopy8(buf + 9, out, 12);
    return TRUE;
}

extern "C" void Net_StartOv067(void *send, void *recv, u32 n) {
    sNetMode = 5;
    data_021f48e0 = 0;
    data_021f48e8 = 0;
    data_021f48c0 = Net_Alloc(0xa000, 32);
    Wlx_Init(data_021f48c0, Net_OnWlxStopped, 2);
    Wlx_SetPacketSizes(60, 60, 1);
    Wlx_RegisterData(data_021f48b8, (void (*)(u32, void *))Net_OnWlxExchangeDone, (u8 *)send, n, (u8 *)recv, n);
}

// (Wlx_StartExchange / Wlx_StopExchange have no result; the callers don't use one)
extern "C" void Net_WlxStartExchange(void *p) {
    Shared_Wlx_StartExchange();
}

extern "C" void Net_WlxStopExchange(void *p) {
    Wlx_StopExchange();
}

extern "C" BOOL Net_WlxIsReady(void *p) {
    return Wlx_GetState() == 2;
}

extern "C" u32 Net_WlxIsExchangeDone(void *p) {
    return data_021f48e8;
}

// ---- the file's data (.data 0x0213b058-0x0213b120: the named objects, then the string literals of the code, which
// this file pools: it is built with -str reuse, see the first line) and bss (autoload_3 0x021f488c-0x021f5974). This
// definition order gives the original order after mwcc's size sort.
extern "C" {
u8 data_021f4bc0[0xdb4] __attribute__((aligned(8)));  // the DWC core's control block (DwcCore_Init: DwcControl)
NetSlot sSendSlots[16];      // send slots (the DS's interior labels data_021f4ac4 / 4ac8 / 4acc: slot 0's members)
#ifdef HOST64
u8 data_021f49e0[0xe8] __attribute__((aligned(8)));  // (LocalWlBeacon on 64-bit hosts: its owner list pointer is 8 bytes)
#else
u8 data_021f49e0[0xe0] __attribute__((aligned(8)));  // the beacon being connected to (Net_ConnectToParent)
#endif
Ent sWifiPingState[20];      // ping state per aid (data_021f4992: interior label)
OSMessage data_021f4950[16];  // sSendQueue's messages (slot numbers)
void *data_021f4910[8];      // Net_GetScanResults
OSMessageQueue sSendQueue;
HexTable data_0213b070 = {"0123456789abcdef"};
HexTable data_0213b084 = {"0123456789abcdef"};
u8 sLocalPeerState[0x10];
u8 data_021f48f4[0xc] __attribute__((aligned(4)));  // the DWC internet connection state (DwcInet_Init: DwcInetControl)
NetRecvFn sRecvCallback;
u32 data_021f48e8;           // the Wlx exchange is done
const char *data_021f48e4;   // the stats download URL
u32 data_021f48e0;           // Wlx stopped
char *data_021f48dc;         // the stats download request URL (0x100 bytes)
NetWifiUserData *sWifiUserData;
NetWifiFriends *sWifiFriendList;
u32 data_021f489c;           // the DWC game code
s32 data_0213b064 = 1;       // the stats upload is done
NetFriendFn data_021f48c8;   // a friend was deleted
char *data_021f48c4;         // the stats upload hash
u32 data_021f48b0;           // when the send queue started waiting (ms)
u32 data_021f48ac;           // the download's size
s32 sLastErrorCode;
u32 data_021f48b8;           // the local wireless group id
u32 data_021f48ec;           // the shutdown step's start (ms)
u32 data_021f48cc;           // when the server was matched (ms)
const char *data_021f48a8;   // the stats download region
char *data_021f48a4;         // the stats download hash
NetAllocFn sAllocHook;
s32 data_0213b068 = 1;       // the download is done
s32 data_0213b06c = 1;       // a friend server update is running
void *data_021f48c0;         // the Wlx work
char *data_021f48b4;         // the stats upload body
NetFreeFn sFreeHook;
u32 data_0213b060 = 10000;   // the timeout (ms)
u16 sWifiShutdownStep;
u16 sWifiConnectStep;
u8 sNetMode;
u8 data_0213b05c = 0xff;     // the wireless driver's DMA channel
u8 data_021f488c;            // the number of peers
u8 data_0213b058 = 0xff;     // the aid pinged last
}
