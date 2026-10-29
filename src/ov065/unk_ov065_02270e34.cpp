#include "uptr.h"
// mwcc-flags: -O4,p -str reuse
#include "types.h"
#include "net/NasAuthParams.h"
#include "net/DwcHttp.h"
#include "net/NasAuthResult.h"
#include "net/gpi.h"
#include "net/DwcMatchCommandHeader.h"
#include "net/DwcFriendControl.h"
#include "net/DwcMatchControl.h"
#include "net/DwcControl.h"
#include "net/gpiInfo.h"

#include "types64.h"
#include "proto/autoload_2/unk_0212703c.h"
#include "proto/autoload_2/unk_0212a120.h"
#include "proto/itcm/unk_01ffa4a0.h"
#include "proto/ov065/unk_ov065_0226f9e0.h"
#include "proto/ov065/unk_ov065_02270e34.h"
#include "proto/ov065/unk_ov065_02277140.h"
#include "proto/ov065/unk_ov065_02277974.h"
#include "proto/ov065/unk_ov065_02283720.h"
#include "proto/ov065/unk_ov065_02285778.h"
#include "proto/ov065/unk_ov065_02286934.h"
#include "proto/ov065/unk_ov065_0227bd20.h"
extern "C" {
void MI_CpuFill8(void *dst, u32 data, u32 size);
void MI_CpuCopy8(const void *src, void *dst, u32 size);
}

extern "C" {
DwcNasLoginCallback sDwcLoginDoneCallback;
DwcLoginControl *sDwcLoginControl;
u32 sDwcLoginDoneArg;
s32 sDwcErrorCode;
s32 sDwcErrorClass;
DwcFriendControl *sDwcFriendControl;
}

// (src/ov065/unk_ov065_0227bd20.cpp: not in its generated header yet)
extern "C" s32 gpSetStatusA(GPConnection *h, s32 v, const char *s1, const char *s2);

namespace F02270b74 {










namespace Unk_ov065_0227138c_Ns {
extern "C" s32 DwcLogin_HandleGpResult(s32 r);
}
extern "C" {
extern s32 sDwcErrorClass;
extern s32 sDwcErrorCode;
extern u32 sDwcLoginDoneArg;
extern DwcLoginControl *sDwcLoginControl;
extern DwcNasLoginCallback sDwcLoginDoneCallback;
void qr2_shutdown(void *);
s32 ServerBrowserFree(void *sb);
void DwcFriend_ClearControl(void);
void DwcMatch_Shutdown(void);
void gt2CloseSocket(void *);
void DwcMatch_OnGt2Connected(void);
void DwcConn_OnGt2Receive(void);
void DwcConn_OnGt2Closed(void);
void DwcConn_OnGt2Ping(void);
void DwcConn_ClearTables(void);
void DwcLogin_InitControl(void *mem, DwcUserData *a, GPConnection *b, u32 c, u32 d, DwcLoginResultCallback e, u32 f);
void DwcLogin_OnLoginDone(void);
void DwcFriend_InitControl(DwcFriendControl *a, GPConnection *b, char *c, DwcFriendData *d, s32 e);
void DwcMatch_InitControl(void *, void *, void *, void *, void *, void *, void *, void *);
void DWCi_Acc_LoginIdToUserName(void *, u32, void *);
s32 gpGetInfo(GPConnection *h, s32 a1, s32 a2, s32 a3, GPCallback cb, void *arg); // src/ov065/unk_ov065_0227bd20.cpp
s32 gpConnectPreAuthenticatedA(void *, void *, void *, s32, s32, void *, void *);
void DWCi_Acc_SetLoginIdToUserData(void *, void *, u32);
s32 NasAuth_GetState(void);
void NasAuth_GetResult(s32 *);
void NasAuth_Destroy(void);
void NasAuth_Abort(void);
s32 NasAuth_Start(NasAuthParams *, DwcHttp *);
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
BOOL DwcCore_HasError(void);
void DwcCore_ClearError(void);
s32 DwcCore_GetLastError(s32 *out);
BOOL DwcLogin_IsLoggedIn(void);
void DwcLogin_OnGpProfileInfo(GPConnection *a0, GPGetInfoResponseArg *x);
void DwcLogin_GpConnect(const char *a, const char *b, void *c, s32 d);
void DwcLogin_OnGpConnected(GPConnection *a0, GPConnectResponseArg *x);
void DwcLogin_ResetState(void);
void DwcLogin_Shutdown(void);
void DwcLogin_Fail(s32 a, s32 b);
void *DwcLogin_GetUserData(void);
s32 DwcLogin_HandleGpResult(s32 r);
}
}

namespace F02271488 {




extern "C" {
extern DwcLoginControl *sDwcLoginControl;
extern DwcFriendControl *sDwcFriendControl;
s32 DWC_GetFriendDataType(void *);
s32 DWC_IsBuddyFriendData(void *);
void DWCi_SetBuddyFriendData(void *);
void DWC_LoginIdToUserName(void *, void *, void *);
void DWC_SetGsProfileId(void *, s32);
u32 DWC_GetGsProfileId(void *userData, void *fd);
void DwcLogin_Fail(s32, s32);
void *DwcLogin_GetUserData(void);
s32 DwcCore_HasError(void);
s32 DwcLogin_IsLoggedIn(void);
s32 gpSendBuddyRequestA(GPConnection *h, s32 v, const char *s);
s32 gpProfileSearchA(void *, char *, char *, char *, char *, char *, s32, s32, GPCallback, void *);
s32 DwcFriend_GetProfileId(s32);
s32 DwcFriend_HandleGpResult(s32);
s32 DwcFriend_MergeDuplicate(DwcFriendData *, s32, s32);
s32 DwcFriend_SendBuddyRequest(s32);
void DwcFriend_DeleteEntry(DwcFriendData *, s32, s32);
s32 DwcFriend_RemoveDuplicates(DwcFriendData *, s32, s32);
void DwcLogin_Process(void);
void DwcLogin_Begin(void);
void DwcLogin_InitControl(void *mem, DwcUserData *a, GPConnection *b, u32 c, u32 d, DwcLoginResultCallback e, u32 f);
void DwcFriend_OnProfileSearch(GPConnection *x, GPProfileSearchResponseArg *p, s32 idx);
void DwcFriend_FinishUpdate(void);
void DwcFriend_Abort(void);
s32 DwcFriend_Tick(void);
}
}

namespace F02271da0 {









extern "C" {
extern DwcFriendControl *sDwcFriendControl;
extern DwcMatchControl *sDwcMatch;
u32 DWC_GetGsProfileId(void *userData, void *fd);
s32 DWCi_Acc_IsValidFriendData(void *);
s32 strtoul(const char *, char **, s32);
s32 strncmp(const void *, const void *, u32);
void OS_SNPrintf(char *, s32, const char *, u32);
s32 DwcCore_HasError();
s32 DwcLogin_IsLoggedIn();
s32 DwcCore_GetState();
void *DwcLogin_GetUserData();
void DwcFriend_FinishUpdate();
void DwcFriend_Abort();
s32 DwcFriend_Tick();
s32 DwcFriend_HandleGpResult(s32 r);
s32 DwcMatch_HandleGt2Result();
s32 DwcMatch_HandleNnResult(s32, s32);
s32 DwcMatch_HandleNnStartResult();
s32 DwcMatch_SendNnRequest(void *);
s32 DwcMatch_Fail(s32, s32);
s32 gt2Connect(void *socket, void **connection, char *remoteAddress, char *message, s32 len, s32 timeout, void *callbacks, s32 blocking);
s32 DwcMatch_CancelNewClient(u32);
s32 DwcMatch_RestartAfterNnFailure(u32);
s32 DwcMatch_HandleCommand(u32, u32, u32, u32, void *, s32);
s32 gpGetInfo(GPConnection *h, s32 a1, s32 a2, s32 a3, GPCallback cb, void *arg);
s32 DwcFriend_GetStatus(void *, u8 *, u8 *, char *);
s32 DwcFriend_GetStatusString(void *, char *);
s32 DwcFriend_GetProfileId(s32);
s32 DwcFriend_FindIndexByProfileId(s32);
s32 DwcMatch_CountNnRetry(s32);
void DwcFriend_ClearControl();
void DwcFriend_ResetTimer();
void DwcFriend_OnBuddyStatus(GPConnection *a, u32 *b);
s32 DwcFriend_HandleAuthorizedMessage(GPConnection *a, GPRecvBuddyMessageArg *b);
void DwcFriend_OnBuddyRequest(GPConnection *a, u32 *b);
void DwcFriend_StartUpdate(char *a, char *b, void (*c)(s32, u32, s32), s32 d, DwcFriendStatusCallback e, s32 f, DwcFriendDeleteCallback g, s32 h);
void DwcFriend_Process();
void DwcFriend_InitControl(DwcFriendControl *a, GPConnection *b, char *c, DwcFriendData *d, s32 e);
void DwcFriend_DeleteFriend(void *p);
BOOL DwcFriend_IsIdle();
BOOL DwcFriend_SetStatusData(void *a, s32 b);
s32 DwcFriend_CountValid(u8 *p, s32 n);
s32 DwcFriend_GetStatusData(void *a, u8 *b, u8 *c, char *d, s32 *out);
}
}

namespace F02271da0 {
extern "C" {
s32 DwcFriend_GetStatus(void *a, u8 *p1, u8 *p2, char *dst) {
    char tmp[4];
    GPBuddyStatus o;
    if (DwcFriend_GetBuddyStatus(a, &o) != 0) {
        if (o.status == 6) {
            if (p1 != NULL) {
                if (GsUtil_GetKeyValue((char *)"SCM", tmp, o.statusString, 0x2f) > 0) {
                    *p1 = strtoul(tmp, NULL, 10);
                } else {
                    *p1 = 0;
                }
            }
            if (p2 != NULL) {
                if (GsUtil_GetKeyValue((char *)"SCN", tmp, o.statusString, 0x2f) > 0) {
                    *p2 = strtoul(tmp, NULL, 10);
                } else {
                    *p2 = 0;
                }
            }
        } else {
            if (p1 != NULL) {
                *p1 = 0;
            }
            if (p2 != NULL) {
                *p2 = 0;
            }
        }
        if (dst != NULL) {
            STD_CopyString(dst, o.locationString);
        }
        return (u8)o.status;
    }
    if (p1 != NULL) {
        *p1 = 0;
    }
    if (p2 != NULL) {
        *p2 = 0;
    }
    return 0;
}
}
}

namespace F02271da0 {
extern "C" {
s32 DwcFriend_GetStatusData(void *a, u8 *b, u8 *c, char *d, s32 *out) {
    char buf[0x100];
    s32 r = DwcFriend_GetStatus(a, b, c, buf);
    s32 t;
    if (r == 0) {
        *out = -1;
        return r;
    }
    *out = NasBase64_Decode((s8 *)buf, STD_GetStringLength(buf), NULL, 0);
    if (d == NULL || (t = *out) == -1) {
        return r;
    }
    NasBase64_Decode((s8 *)buf, STD_GetStringLength(buf), (u8 *)d, t);
    return r;
}
}
}

namespace F02271da0 {
extern "C" {
s32 DwcFriend_CountValid(u8 *p, s32 n) {
    s32 cnt = 0;
    s32 i;
    if (p == NULL) {
        return 0;
    }
    for (i = 0; i < n; i++) {
        if (DWCi_Acc_IsValidFriendData(p) != 0) {
            cnt++;
        }
        p += 12;
    }
    return cnt;
}
}
}

namespace F02271da0 {
extern "C" {
BOOL DwcFriend_SetStatusData(void *a, s32 b) {
    char buf[0x100];
    s32 n;
    if (sDwcFriendControl == NULL || DwcLogin_IsLoggedIn() == 0) {
        return FALSE;
    }
    n = NasBase64_Encode((u8 *)a, b, (u8 *)buf, 0xff);
    if (n == -1) {
        return FALSE;
    }
    buf[n] = 0;
    if (DwcFriend_SetOwnStatus(-1, NULL, buf) == 0) {
        return TRUE;
    }
    return FALSE;
}
}
}

namespace F02271da0 {
extern "C" {
BOOL DwcFriend_IsIdle() {
    if (sDwcFriendControl != NULL) {
        if ((u8)(sDwcFriendControl->syncPhase + 0xff) <= 1) {
            return FALSE;
        }
    }
    return TRUE;
}
}
}

namespace F02271da0 {
extern "C" {
void DwcFriend_DeleteFriend(void *p) {
    if (sDwcFriendControl != NULL && DwcLogin_IsLoggedIn() != 0 && DwcLogin_GetUserData() != 0) {
        s32 t = DWC_GetGsProfileId(DwcLogin_GetUserData(), p);
        if (t != 0 && t != -1 && gpIsBuddy(sDwcFriendControl->gpConnection, t) != 0) {
            gpDeleteBuddy(sDwcFriendControl->gpConnection, t);
        }
    }
    MI_CpuFill8(p, 0, 12);
}
}
}

namespace F02271da0 {
extern "C" {
void DwcFriend_InitControl(DwcFriendControl *a, GPConnection *b, char *c, DwcFriendData *d, s32 e) {
    sDwcFriendControl = a;
    a->updateState = 0;
    sDwcFriendControl->gpConnection = b;
    sDwcFriendControl->tickCount = 0;
    {
        DwcFriendControl *g = sDwcFriendControl;
        g->lastTick = 0;
        g->lastProcessTickHi = 0;
        g->numFriends = e;
    }
    sDwcFriendControl->friendList = d;
    sDwcFriendControl->syncIndex = 0;
    sDwcFriendControl->isListChanged = 0;
    sDwcFriendControl->syncPhase = 0;
    sDwcFriendControl->updateStep = 0;
    sDwcFriendControl->usePersist = 0;
    sDwcFriendControl->unk_24 = 0;
    sDwcFriendControl->buddyRequestText = c;
    sDwcFriendControl->updateCallback = NULL;
    sDwcFriendControl->updateCallbackArg = 0;
    sDwcFriendControl->statusCallback = NULL;
    sDwcFriendControl->statusCallbackArg = 0;
    sDwcFriendControl->deleteCallback = NULL;
    sDwcFriendControl->deleteCallbackArg = NULL;
    sDwcFriendControl->addedCallback = NULL;
    sDwcFriendControl->addedCallbackArg = 0;
    sDwcFriendControl->unk_4c = 0;
    sDwcFriendControl->unk_50 = 0;
}
}
}

namespace F02271da0 {
extern "C" {
void DwcFriend_Process() {
    if (sDwcFriendControl == NULL) {
        return;
    }
    if (sDwcFriendControl->friendList == NULL) {
        return;
    }
    if (DwcCore_HasError() != 0) {
        return;
    }
    if (DwcFriend_GetControlField20() != 0 && PersistThink() == 0) {
        DwcFriend_Fail(6, -0x1194a);
        return;
    }
    if (sDwcFriendControl->gpConnection != NULL && *sDwcFriendControl->gpConnection != NULL) {
        if (DwcFriend_HandleGpResult(DwcFriend_Tick()) != 0) {
            return;
        }
        if (sDwcFriendControl->friendList != NULL && sDwcFriendControl->syncPhase != 3 && sDwcFriendControl->tickCount > 7) {
            if (sDwcFriendControl->syncPhase <= 1) {
                DwcFriend_SyncList(sDwcFriendControl->friendList, sDwcFriendControl->numFriends);
            }
            if (sDwcFriendControl->syncIndex >= sDwcFriendControl->numFriends) {
                sDwcFriendControl->syncPhase = 3;
                sDwcFriendControl->updateStep++;
            }
        }
    }
    if (sDwcFriendControl->updateStep >= 2) {
        sDwcFriendControl->updateStep = 0;
        DwcFriend_FinishUpdate();
    }
}
}
}

namespace F02271da0 {
extern "C" {
void DwcFriend_StartUpdate(char *a, char *b, void (*c)(s32, u32, s32), s32 d, DwcFriendStatusCallback e, s32 f, DwcFriendDeleteCallback g, s32 h) {
    sDwcFriendControl->updateCallback = c;
    sDwcFriendControl->updateCallbackArg = d;
    sDwcFriendControl->statusCallback = e;
    sDwcFriendControl->statusCallbackArg = f;
    sDwcFriendControl->deleteCallback = g;
    sDwcFriendControl->deleteCallbackArg = h;
    sDwcFriendControl->isListChanged = 0;
    sDwcFriendControl->syncPhase = 0;
    sDwcFriendControl->updateStep = 0;
    sDwcFriendControl->syncIndex = 0;
    sDwcFriendControl->updateState = 1;
    sDwcFriendControl->updateStep++;
}
}
}

namespace F02271da0 {
extern "C" {
void DwcFriend_Fail(s32 a, s32 b) {
    if (sDwcFriendControl != NULL && a != 0) {
        DwcCore_SetError(a, b);
        if (sDwcFriendControl->updateState != 0 && sDwcFriendControl->updateState != 2) {
            sDwcFriendControl->updateCallback(a, sDwcFriendControl->isListChanged, sDwcFriendControl->updateCallbackArg);
        }
        DwcFriend_Abort();
    }
}
}
}

namespace F02271da0 {
extern "C" {
void DwcFriend_OnBuddyRequest(GPConnection *a, u32 *b) {
    if (sDwcFriendControl->friendList != NULL) {
        gpGetInfo(a, b[0], 0, 0, (GPCallback)DwcFriend_OnBuddyRequestInfo, NULL);
    }
}
}
}

namespace F02271da0 {
extern "C" {
s32 DwcFriend_HandleAuthorizedMessage(GPConnection *a, GPRecvBuddyMessageArg *b) {
    if (strcmp(b->message, "I have authorized your request to add me to your list") == 0) {
        gpGetInfo(a, b->profile, 0, 0, (GPCallback)DwcFriend_OnAuthorizedInfo, NULL);
        return 1;
    }
    return 0;
}
}
}

namespace F02271da0 {
extern "C" {
void DwcFriend_OnBuddyStatus(GPConnection *a, u32 *b) {
    GPBuddyStatus o;
    if (sDwcFriendControl->statusCallback != NULL) {
        s32 i = DwcFriend_FindIndexByProfileId(b[0]);
        if (i != -1) {
            gpGetBuddyStatus(a, b[2], &o);
            sDwcFriendControl->statusCallback(i, (u8)o.status, o.locationString, sDwcFriendControl->statusCallbackArg);
        }
    }
}
}
}

namespace F02271da0 {
extern "C" {
s32 DwcFriend_GetProfileId(s32 i) {
    s32 r = DWC_GetGsProfileId(DwcLogin_GetUserData(), &sDwcFriendControl->friendList[i]);
    s32 m = -1;
    if (r == 0 || r == m) {
        r = 0;
    }
    return r;
}
}
}

namespace F02271da0 {
extern "C" {
s32 DwcFriend_FindIndexByProfileId(s32 v) {
    s32 i;
    if (sDwcFriendControl == NULL || v == 0) {
        return -1;
    }
    for (i = 0; i < sDwcFriendControl->numFriends; i++) {
        if (v == DwcFriend_GetProfileId(i)) {
            return i;
        }
    }
    return -1;
}
}
}

namespace F02271da0 {
extern "C" {
void DwcFriend_ResetTimer() {
    if (sDwcFriendControl != NULL) {
        sDwcFriendControl->tickCount = 0;
        u64 t = OS_GetTick();
        DwcFriendControl *g = sDwcFriendControl;
        g->lastTick = (u32)t;
        g->lastProcessTickHi = (u32)(t >> 32);
    }
}
}
}

namespace F02271da0 {
extern "C" {
s32 DwcFriend_SetOwnStatus(s32 a, const char *b, const char *c) {
    GPConnection *s;
    if (sDwcFriendControl == NULL || sDwcFriendControl->gpConnection == NULL) {
        return 0;
    }
    s = sDwcFriendControl->gpConnection;
    if (a == -1) {
        a = (*s)->lastStatus;
    }
    if (b == NULL) {
        b = (*s)->lastStatusString;
    }
    if (c == NULL) {
        c = (*s)->lastLocationString;
    }
    return gpSetStatusA(s, a, b, c);
}
}
}

namespace F02271da0 {
extern "C" {
void DwcFriend_NotifyAdded(s32 idx) {
    GPBuddyStatus o;
    if (sDwcFriendControl->addedCallback != NULL && sDwcFriendControl->updateState != 1) {
        sDwcFriendControl->addedCallback(idx, sDwcFriendControl->addedCallbackArg);
    }
    if (sDwcFriendControl->statusCallback != NULL) {
        s32 r = DwcFriend_GetStatusString(&sDwcFriendControl->friendList[idx], o.locationString);
        sDwcFriendControl->statusCallback(idx, r, o.locationString, sDwcFriendControl->statusCallbackArg);
    }
}
}
}

namespace F02271da0 {
extern "C" {
void DwcFriend_ClearControl() {
    sDwcFriendControl = NULL;
}
}
}

namespace F02271488 {
extern "C" {
s32 DwcFriend_Tick(void)
{
    DwcFriendControl *b = sDwcFriendControl;
    u64 d = (OS_GetTick() - *(u64 *)&b->lastTick) << 6;
    d = d / 0x82ea;
    if (d >= 0x12c) {
        b->tickCount++;
        gpProcess(sDwcFriendControl->gpConnection);
        *(u64 *)&sDwcFriendControl->lastTick = OS_GetTick();
    }
    return 0;
}
}
}

namespace F02271488 {
extern "C" {
void DwcFriend_Abort(void)
{
    if (sDwcFriendControl != NULL) {
        CloseStatsConnection();
        sDwcFriendControl->updateState = 0;
    }
}
}
}

namespace F02271488 {
extern "C" {
void DwcFriend_SyncList(DwcFriendData *arr, s32 n)
{
    s32 cnt;
    s32 idx;
    u8 buf[0x18];
    GPBuddyStatus out;
    s32 j;
    s32 id;
    if (sDwcFriendControl->syncPhase == 0) {
        DwcFriend_HandleGpResult(gpGetNumBuddies(sDwcFriendControl->gpConnection, &cnt));
        idx = 0;
        if (cnt > 0) {
            do {
                DwcFriend_HandleGpResult(gpGetBuddyStatus(sDwcFriendControl->gpConnection, idx, &out));
                for (j = 0; j < n; j++) {
                    if (out.profile == DwcFriend_GetProfileId(j)) {
                        if (DWC_IsBuddyFriendData(&arr[j]) == 0) {
                            DwcFriendData *e = &arr[j];
                            DWC_SetGsProfileId(e, out.profile);
                            DWCi_SetBuddyFriendData(e);
                            sDwcFriendControl->isListChanged = 1;
                        }
                        break;
                    }
                }
                if (j == n) {
                    DwcFriend_HandleGpResult(gpDeleteBuddy(sDwcFriendControl->gpConnection, out.profile));
                    cnt--;
                    idx--;
                }
                idx++;
            } while (idx < cnt);
        }
        sDwcFriendControl->syncPhase = 1;
    }
    while (sDwcFriendControl->syncIndex < n) {
        id = DwcFriend_GetProfileId(sDwcFriendControl->syncIndex);
        if (id != 0) {
            if (DwcFriend_MergeDuplicate(arr, sDwcFriendControl->syncIndex, id) == 0) {
                DwcFriend_HandleGpResult(gpGetBuddyIndex(sDwcFriendControl->gpConnection, id, &idx));
                if (idx == -1) {
                    DwcFriend_SendBuddyRequest(id);
                }
            }
        } else {
            if (DWC_GetGsProfileId((void *)DwcLogin_GetUserData(), &arr[sDwcFriendControl->syncIndex]) == -1) {
                DWC_LoginIdToUserName((void *)DwcLogin_GetUserData(), &arr[sDwcFriendControl->syncIndex], buf);
                gpProfileSearchA(sDwcFriendControl->gpConnection, 0, 0, 0, 0, (char *)buf, 0, 0, (GPCallback)DwcFriend_OnProfileSearch, (void *)(sptr)sDwcFriendControl->syncIndex);
                sDwcFriendControl->syncPhase = 2;
                return;
            }
        }
        sDwcFriendControl->syncIndex++;
    }
}
}
}

namespace F02271488 {
extern "C" {
void DwcFriend_FinishUpdate(void)
{
    sDwcFriendControl->updateCallback(0, sDwcFriendControl->isListChanged, sDwcFriendControl->updateCallbackArg);
    sDwcFriendControl->updateState = 2;
}
}
}

namespace F02271488 {
extern "C" {
void DwcFriend_DeleteEntry(DwcFriendData *arr, s32 i, s32 j)
{
    if (sDwcFriendControl != NULL) {
        MI_CpuFill8(&arr[i], 0, 12);
        if (sDwcFriendControl->deleteCallback != NULL) {
            sDwcFriendControl->deleteCallback(i, j, sDwcFriendControl->deleteCallbackArg);
        }
    }
}
}
}

namespace F02271488 {
extern "C" {
s32 DwcFriend_MergeDuplicate(DwcFriendData *arr, s32 n, s32 id)
{
    s32 i;
    for (i = 0; i < n; i++) {
        s32 t = DwcFriend_GetProfileId(i);
        if (t != 0 && t == id) {
            if (DWC_IsBuddyFriendData(&arr[n]) != 0 && DWC_IsBuddyFriendData(&arr[i]) == 0) {
                DwcFriend_DeleteEntry(arr, i, n);
            } else {
                DwcFriend_DeleteEntry(arr, n, i);
            }
            sDwcFriendControl->isListChanged = 1;
            return TRUE;
        }
    }
    return FALSE;
}
}
}

namespace F02271488 {
extern "C" {
s32 DwcFriend_RemoveDuplicates(DwcFriendData *arr, s32 n, s32 id)
{
    s32 res, i, j, t;
    DwcFriendData *q, *p;
    res = -1;
    i = 0;
    if (n - 1 > 0) {
        q = arr;
        p = arr;
        do {
            t = DwcFriend_GetProfileId(i);
            if (t != 0) {
                if (t == id) {
                    res = i;
                }
                j = i + 1;
                for (; j < n; j++) {
                    if (t == DwcFriend_GetProfileId(j)) {
                        if (DWC_GetFriendDataType(q) == 2 && DWC_GetFriendDataType(&arr[j]) == 3) {
                            DWC_SetGsProfileId(p, t);
                        }
                        if (DWC_IsBuddyFriendData(&arr[j]) != 0) {
                            DWCi_SetBuddyFriendData(p);
                        }
                        DwcFriend_DeleteEntry(arr, j, i);
                        sDwcFriendControl->isListChanged = 1;
                    }
                }
            }
            q++;
            p++;
            i++;
        } while (i < n - 1);
    }
    return res;
}
}
}

namespace F02271488 {
extern "C" {
s32 DwcFriend_SendBuddyRequest(s32 a)
{
    s32 r = gpSendBuddyRequestA(sDwcFriendControl->gpConnection, a, sDwcFriendControl->buddyRequestText);
    DwcFriend_HandleGpResult(r);
    return r;
}
}
}

namespace F02271488 {
extern "C" {
s32 DwcFriend_GetBuddyStatus(void *a, GPBuddyStatus *b)
{
    s32 out;
    s32 t;
    out = 0;
    if (sDwcFriendControl == NULL || DwcLogin_IsLoggedIn() == 0) {
        return FALSE;
    }
    t = DWC_GetGsProfileId((void *)DwcLogin_GetUserData(), a);
    if (t > 0) {
        if (gpGetBuddyIndex(sDwcFriendControl->gpConnection, t, &out) != 0) {
            return FALSE;
        }
    }
    if (t <= 0 || out == -1) {
        return FALSE;
    }
    if (gpGetBuddyStatus(sDwcFriendControl->gpConnection, out, b) == 0) {
        goto ok;
    }
    return FALSE;
ok:
    return TRUE;
}
}
}

namespace F02271488 {
extern "C" {
s32 DwcFriend_HandleGpResult(s32 r)
{
    s32 a;
    s32 b;
    if (r == 0) {
        return 0;
    }
    switch (r) {
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
    DwcFriend_Fail(a, b - 0x11558);
    return r;
}
}
}

namespace F02271488 {
extern "C" {
void DwcFriend_OnProfileSearch(GPConnection *x, GPProfileSearchResponseArg *p, s32 idx)
{
    s32 off;
    s32 i;
    s32 out;
    if (p->result == 0 && p->numMatches != 0) {
        off = idx * 12;
        if (DWC_GetFriendDataType((u8 *)sDwcFriendControl->friendList + off) != 0) {
            if (sDwcFriendControl->updateState == 1) {
                sDwcFriendControl->isListChanged = 1;
                for (i = 0; i < p->numMatches; i++) {
                    if (DwcFriend_MergeDuplicate(sDwcFriendControl->friendList, idx, p->matches[i].profile) != 0) {
                        sDwcFriendControl->syncIndex++;
                        sDwcFriendControl->syncPhase = 1;
                        p->more = 0x601;
                        return;
                    }
                }
                for (i = 0; i < p->numMatches; i++) {
                    DwcFriend_HandleGpResult(gpGetBuddyIndex(x, p->matches[i].profile, &out));
                    if (out == -1) {
                        DwcFriend_SendBuddyRequest(p->matches[i].profile);
                    } else {
                        DWC_SetGsProfileId((u8 *)sDwcFriendControl->friendList + off, p->matches[0].profile);
                        DWCi_SetBuddyFriendData((u8 *)sDwcFriendControl->friendList + off);
                        DwcFriend_NotifyAdded(idx);
                        sDwcFriendControl->syncIndex++;
                        sDwcFriendControl->syncPhase = 1;
                        p->more = 0x601;
                        return;
                    }
                }
                if (p->more != 0x600) {
                    sDwcFriendControl->syncIndex++;
                    sDwcFriendControl->syncPhase = 1;
                    return;
                }
            }
            return;
        }
    }
    if (p->result != 0) {
        s32 e = DwcFriend_HandleGpResult(p->result);
        if (e > 0) {
            e = 1;
        } else if (e != 0) {
            e = e;
        }
    } else {
        if (sDwcFriendControl->updateState == 1 || DWC_GetFriendDataType((u8 *)sDwcFriendControl->friendList + idx * 12) == 0) {
            sDwcFriendControl->syncIndex++;
            sDwcFriendControl->syncPhase = 1;
        }
    }
}
}
}

namespace F02271488 {
extern "C" {
void DwcFriend_OnBuddyRequestInfo(GPConnection *x, GPGetInfoResponseArg *p)
{
    s32 i;
    s32 found;
    found = 0;
    if (p->result == 0) {
        i = found;
        for (; i < sDwcFriendControl->numFriends; i++) {
            if (DWC_GetFriendDataType(&sDwcFriendControl->friendList[i]) == 1) {
                u8 buf[24];
                DWC_LoginIdToUserName((void *)DwcLogin_GetUserData(), &sDwcFriendControl->friendList[i], buf);
                if (strcmp((char *)buf, p->lastname) == 0) {
                    gpAuthBuddyRequest(x, p->profile);
                    DWC_SetGsProfileId(&sDwcFriendControl->friendList[i], p->profile);
                    found = 1;
                }
            } else if (DWC_GetFriendDataType(&sDwcFriendControl->friendList[i]) == 3
                       || DWC_GetFriendDataType(&sDwcFriendControl->friendList[i]) == 2) {
                s32 v = p->profile;
                if (v == DWC_GetGsProfileId((void *)DwcLogin_GetUserData(), &sDwcFriendControl->friendList[i])) {
                    gpAuthBuddyRequest(x, v);
                    found = 1;
                }
            }
        }
        if (found != 0) {
            DwcFriend_SendBuddyRequest(p->profile);
        } else {
            gpDenyBuddyRequest(x, p->profile);
        }
    }
}
}
}

namespace F02271488 {
extern "C" {
void DwcFriend_OnAuthorizedInfo(GPConnection *x, GPGetInfoResponseArg *p)
{
    s32 i;
    s32 found;
    u8 buf[28];
    found = 0;
    if (p->result == 0) {
        i = found;
        for (; i < sDwcFriendControl->numFriends; i++) {
            if (DWC_GetFriendDataType(&sDwcFriendControl->friendList[i]) == 1) {
                DWC_LoginIdToUserName((void *)DwcLogin_GetUserData(), &sDwcFriendControl->friendList[i], buf);
                if (strcmp((char *)buf, p->lastname) == 0) {
                    DWC_SetGsProfileId(&sDwcFriendControl->friendList[i], p->profile);
                    DWCi_SetBuddyFriendData(&sDwcFriendControl->friendList[i]);
                    found = 1;
                }
            } else if (DWC_GetFriendDataType(&sDwcFriendControl->friendList[i]) == 3
                       || DWC_GetFriendDataType(&sDwcFriendControl->friendList[i]) == 2) {
                s32 v = p->profile;
                if (v == DWC_GetGsProfileId((void *)DwcLogin_GetUserData(), &sDwcFriendControl->friendList[i])) {
                    DWC_SetGsProfileId(&sDwcFriendControl->friendList[i], v);
                    DWCi_SetBuddyFriendData(&sDwcFriendControl->friendList[i]);
                    found = 1;
                }
            }
        }
        if (found != 0) {
            DwcFriend_NotifyAdded(DwcFriend_RemoveDuplicates(sDwcFriendControl->friendList, sDwcFriendControl->numFriends, p->profile));
            sDwcFriendControl->isListChanged = 1;
        }
    }
}
}
}

namespace F02271488 {
extern "C" {
u32 DwcFriend_GetControlField20(void)
{
    return sDwcFriendControl->usePersist;
}
}
}

namespace F02271488 {
extern "C" {
void DwcLogin_InitControl(void *mem, DwcUserData *a, GPConnection *b, u32 c, u32 d, DwcLoginResultCallback e, u32 f)
{
    sDwcLoginControl = (DwcLoginControl *)mem;
    MI_CpuFill8(sDwcLoginControl, 0, sizeof(DwcLoginControl));
    sDwcLoginControl->gpConnection = b;
    sDwcLoginControl->state = 0;
    sDwcLoginControl->productId = c;
    sDwcLoginControl->gameCode = d;
    sDwcLoginControl->resultCallback = e;
    sDwcLoginControl->resultCallbackArg = f;
    sDwcLoginControl->userData = a;
}
}
}

namespace F02271488 {
extern "C" {
void DwcLogin_Begin(void)
{
    DwcLogin_StartNasAuth(DwcLogin_OnNasAuthDone, 0);
    sDwcLoginControl->state = 1;
    sDwcLoginControl->gpConnectPending = 0;
}
}
}

namespace F02271488 {
extern "C" {
void DwcLogin_Process(void)
{
    if (sDwcLoginControl != NULL) {
        if (DwcCore_HasError() == 0) {
            switch (sDwcLoginControl->state) {
            case 0:
                break;
            case 1:
                DwcLogin_PollNasAuth();
                break;
            case 2:
            case 3:
            case 4: {
                GPConnection *in = sDwcLoginControl->gpConnection;
                if (in != NULL) {
                    if (*in != NULL) {
                        gpProcess(in);
                    }
                }
                if (sDwcLoginControl->gpConnectPending != 0) {
                    u64 d = (OS_GetTick() - *(u64 *)&sDwcLoginControl->gpConnectStartTick) << 6;
                    d = d / 0x82ea;
                    if (d > 0xea60) {
                        DwcLogin_Fail(6, -0xee8e);
                        sDwcLoginControl->gpConnectPending = 0;
                    }
                }
                break;
            }
            case 5:
                break;
            }
        }
    }
}
}
}

namespace F02270b74 {
extern "C" {
void *DwcLogin_GetUserData(void) {
    if (sDwcLoginControl != NULL) {
        return sDwcLoginControl->userData;
    }
    return NULL;
}
}
}

namespace F02270b74 {
extern "C" {
void DwcLogin_Fail(s32 a, s32 b) {
    if (sDwcLoginControl != NULL && a != 0) {
        DwcCore_SetError(a, b);
        if (sDwcLoginControl->resultCallback != NULL) {
            sDwcLoginControl->resultCallback(a, 0, sDwcLoginControl->resultCallbackArg);
        }
        DwcLogin_ResetState();
    }
}
}
}

namespace F02270b74 {
extern "C" {
void DwcLogin_Shutdown(void) {
    if (sDwcLoginControl->nasAuthWork != NULL) {
        NasAuth_Abort();
        NasAuth_Destroy();
        DwcNet_Free(0, sDwcLoginControl->nasAuthWork, 0);
        sDwcLoginControl->nasAuthWork = NULL;
    }
    sDwcLoginControl = NULL;
}
}
}

namespace F02270b74 {
extern "C" {
void DwcLogin_ResetState(void) {
    if (sDwcLoginControl != NULL) {
        sDwcLoginControl->state = 0;
        sDwcLoginControl->gpConnectPending = 0;
    }
}
}
}

namespace F02270b74 {
extern "C" {
s32 DwcLogin_HandleGpResult(s32 r) {
    s32 a = r;
    s32 b = 0;  // (set for every result gp* returns: GP_MEMORY_ERROR..GP_SERVER_ERROR, 1-4; the original left the
                // unreachable default in r1)
    if (r == 0) {
        return 0;
    }
    switch (r) {
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
    DwcLogin_Fail(a, b - 0xee48);
    return r;
}
}
}

namespace F02270b74 {
extern "C" {
void DwcLogin_OnGpConnected(GPConnection *a0, GPConnectResponseArg *x) {
    sDwcLoginControl->gpConnectPending = 0;
    if (x->result == 0) {
        if (sDwcLoginControl->state == 2) {
            if (Unk_ov065_0227138c_Ns::DwcLogin_HandleGpResult(DwcFriend_SetOwnStatus(1, "", NULL)) == 0) {
                if (sDwcLoginControl->userData->profileId == x->profile) {
                    if (DwcConn_CreateGt2Socket() == 0) {
                        if (DwcMatch_StartQr2(x->profile) == 0) {
                            sDwcLoginControl->state = 5;
                            sDwcLoginControl->resultCallback(0, x->profile, sDwcLoginControl->resultCallbackArg);
                        }
                    }
                } else {
                    DwcLogin_Fail(6, -60000);
                }
            }
        } else if (sDwcLoginControl->state == 3) {
            s32 r = Unk_ov065_0227138c_Ns::DwcLogin_HandleGpResult(gpGetInfo(a0, x->profile, 0, 0, (GPCallback)DwcLogin_OnGpProfileInfo, NULL));
            if (r == 0) {
            } else if (r != 0) {
                r = r;
            }
        }
    } else {
        Unk_ov065_0227138c_Ns::DwcLogin_HandleGpResult(x->result);
    }
}
}
}

namespace F02270b74 {
extern "C" {
void DwcLogin_OnNasAuthDone(const char *a, const char *b, u32 arg) { // a DwcNasLoginCallback (arg: unused)
    DwcLogin_GpConnect(a, b, (void *)DwcLogin_OnGpConnected, 2);
}
}
}

namespace F02270b74 {
extern "C" {
void DwcLogin_GpConnect(const char *a, const char *b, void *c, s32 d) {
    STD_CopyString(sDwcLoginControl->authToken, a);
    STD_CopyString(sDwcLoginControl->authChallenge, b);
    DwcLoginControl *g = sDwcLoginControl;
    u64 t = OS_GetTick();
    g->gpConnectStartTick = t;
    g->gpConnectPending = 1;
    DwcLoginControl *h = sDwcLoginControl;
    if (Unk_ov065_0227138c_Ns::DwcLogin_HandleGpResult(gpConnectPreAuthenticatedA(h->gpConnection, h->authToken, h->authChallenge, 1, 0, c, 0)) == 0) {
        sDwcLoginControl->state = d;
    }
}
}
}

namespace F02270b74 {
extern "C" {
void DwcLogin_StartNasAuth(DwcNasLoginCallback cb, u32 arg) {
    NasAuthParams cfg;
    MI_CpuFill8(&cfg, 0, sizeof(cfg));
    sDwcLoginDoneCallback = cb;
    sDwcLoginDoneArg = arg;
    if (DWCi_Acc_IsAuthentic(sDwcLoginControl->userData)) {
        DWCi_Acc_LoginIdToUserName(&sDwcLoginControl->userData->loginId, sDwcLoginControl->userData->gameCode,
                      sDwcLoginControl->loginIdText);
    } else {
        if (DWCi_Acc_IsValidLoginId(&sDwcLoginControl->loginId) == 0) {
            if (DWCi_Acc_CheckConsoleUserId(&sDwcLoginControl->userData->tempLoginId)) {
                sDwcLoginControl->loginId = sDwcLoginControl->userData->tempLoginId;
            } else {
                DWCi_Acc_CreateTempLoginId(&sDwcLoginControl->loginId);
            }
        } else {
            DWCi_Acc_SetPlayerId(&sDwcLoginControl->loginId, (u32)(((u64)((s64)OS_GetTick() * 0x5d588b656c078965LL) + 0x269ec3) >> 32));
        }
        DWCi_Acc_LoginIdToUserName(&sDwcLoginControl->loginId, sDwcLoginControl->gameCode, sDwcLoginControl->loginIdText);
    }
    STD_CopyString(cfg.gsbrcd, sDwcLoginControl->gsbrcd);
    cfg.allocFunc = (DwcAllocFunc)DwcNet_Alloc;
    cfg.freeFunc = (DwcFreeFunc)DwcNet_Free;
    DwcHttp *p = (DwcHttp *)DwcNet_AllocAligned(0, sizeof(DwcHttp), 4);
    sDwcLoginControl->nasAuthWork = p;
    u64 t2 = OS_GetTick();
    sDwcLoginControl->nasAuthStartTick = t2;
    NasAuth_Start(&cfg, p);
}
}
}

namespace F02270b74 {
extern "C" {
void DwcLogin_PollNasAuth(void) {
    NasAuthParams cfg;
    NasAuthResult s1;
    NasAuthResult s2;
    if (NasAuth_GetState() == 0x14) {
        NasAuth_GetResult(&s1.result);
        STD_CopyString(sDwcLoginControl->authToken, s1.token);
        STD_CopyString(sDwcLoginControl->authChallenge, s1.challenge);
        NasAuth_Destroy();
        DwcNet_Free(0, sDwcLoginControl->nasAuthWork, 0);
        sDwcLoginControl->nasAuthWork = NULL;
        if (DWCi_Acc_IsAuthentic(sDwcLoginControl->userData)) {
            sDwcLoginDoneCallback(sDwcLoginControl->authToken, sDwcLoginControl->authChallenge, sDwcLoginDoneArg);
        } else {
            DwcLogin_GpConnect(sDwcLoginControl->authToken, sDwcLoginControl->authChallenge,
                                (void *)DwcLogin_OnGpConnected, 3);
        }
    } else if (NasAuth_GetState() != 0) {
        u64 now = OS_GetTick();
        u64 d = now - sDwcLoginControl->nasAuthStartTick;
        if ((d * 64) / 0x82ea > 0x2710) {
            NasAuth_GetResult(&s2.result);
            NasAuth_Destroy();
            DwcNet_Free(0, sDwcLoginControl->nasAuthWork, 0);
            sDwcLoginControl->nasAuthWork = NULL;
            DwcLogin_Fail(2, s2.result);
        } else {
            NasAuth_Destroy();
            MI_CpuFill8(&cfg, 0, sizeof cfg);  // (0x2c on the DS)
            STD_CopyString(cfg.gsbrcd, sDwcLoginControl->gsbrcd);
            cfg.allocFunc = (DwcAllocFunc)DwcNet_Alloc;
            cfg.freeFunc = (DwcFreeFunc)DwcNet_Free;
            NasAuth_Start(&cfg, sDwcLoginControl->nasAuthWork);
        }
    }
}
}
}

namespace F02270b74 {
extern "C" {
void DwcLogin_OnGpProfileInfo(GPConnection *a0, GPGetInfoResponseArg *x) {
    char a[0x14];
    u8 b[0x14];
    u8 c[0x1c];
    if (x->result == 0) {
        if (sDwcLoginControl->state == 3) {
            if (x->lastname[0] == 0) {
                DWCi_Acc_LoginIdToUserName(&sDwcLoginControl->userData->tempLoginId, sDwcLoginControl->gameCode, a);
                if (Unk_ov065_0227138c_Ns::DwcLogin_HandleGpResult(gpSetInfosA(a0, 0x705, a)) == 0) {
                    sDwcLoginControl->state = 4;
                    s32 r = Unk_ov065_0227138c_Ns::DwcLogin_HandleGpResult(gpGetInfo(a0, x->profile, 0, 0, (GPCallback)DwcLogin_OnGpProfileInfo, NULL));
                    if (r == 0) {
                    } else if (r != 0) {
                        r = r;
                    }
                }
            } else {
                gpDisconnect(a0);
                DwcLogin_StartNasAuth(DwcLogin_OnNasAuthDone, 0);
                sDwcLoginControl->state = 1;
            }
        } else if (sDwcLoginControl->state == 4) {
            DWCi_Acc_LoginIdToUserName(&sDwcLoginControl->userData->tempLoginId, sDwcLoginControl->gameCode, &b[1]);
            if (strcmp((const char *)&x->lastname[0], (const char *)&b[1]) == 0) {
                DWCi_Acc_LoginIdToUserName(&sDwcLoginControl->loginId, sDwcLoginControl->gameCode, &c[2]);
                DWCi_Acc_SetLoginIdToUserData(sDwcLoginControl->userData, &sDwcLoginControl->loginId, x->profile);
                gpDisconnect(a0);
                sDwcLoginDoneCallback(sDwcLoginControl->authToken, sDwcLoginControl->authChallenge, sDwcLoginDoneArg);
            } else {
                s32 r = Unk_ov065_0227138c_Ns::DwcLogin_HandleGpResult(gpGetInfo(a0, x->profile, 0, 0, (GPCallback)DwcLogin_OnGpProfileInfo, NULL));
                if (r == 0) { return; }
            }
        }
    }
}
}
}

namespace F02270b74 {
extern "C" {
BOOL DwcLogin_IsLoggedIn(void) {
    if (sDwcLoginControl != NULL && sDwcLoginControl->state == 5) {
        return TRUE;
    }
    return FALSE;
}
}
}

namespace F02270b74 {
extern "C" {
s32 DwcCore_GetLastError(s32 *out) {
    if (out != NULL) {
        *out = sDwcErrorCode;
    }
    return sDwcErrorClass;
}
}
}

namespace F02270b74 {
extern "C" {
void DwcCore_ClearError(void) {
    if (sDwcErrorClass != 8) {
        sDwcErrorClass = 0;
        sDwcErrorCode = 0;
    }
}
}
}

namespace F02270b74 {
extern "C" {
BOOL DwcCore_HasError(void) {
    if (sDwcErrorClass != 0) {
        return TRUE;
    }
    return FALSE;
}
}
}

namespace F02270b74 {
extern "C" {
void DwcCore_SetError(s32 a, s32 b) {
    if (sDwcErrorClass != 8) {
        sDwcErrorClass = a;
        sDwcErrorCode = b;
    }
}
}
}

// ---- overlay variables (generated: tools/host_overlay_vars.py) ----
#include "host_overlay_vars.h"

HOST_OVERLAY_VARS(unk_ov065_02270e34) {
    HOST_OVERLAY_VAR(sDwcLoginDoneCallback),
    HOST_OVERLAY_VAR(sDwcLoginControl),
    HOST_OVERLAY_VAR(sDwcLoginDoneArg),
    HOST_OVERLAY_VAR(sDwcErrorCode),
    HOST_OVERLAY_VAR(sDwcErrorClass),
    HOST_OVERLAY_VAR(sDwcFriendControl),
    HOST_OVERLAY_VARS_END
};
