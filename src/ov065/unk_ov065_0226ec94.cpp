// mwcc-flags: -O4,p -str reuse
#include "types.h"
#include "net/NetCheck.h"
#include "net/NasAuthParams.h"
#include "net/NasAuthWork.h"

#include "types64.h"
#include "proto/autoload_2/unk_0212a120.h"
#include "proto/autoload_2/unk_0212b770.h"
#include "proto/ov065/unk_ov065_0226ec94.h"

// Word view of NetCheckParams for the struct copy in NetCheck_Start (a copy of the field-typed struct compiles differently).
struct NetCheckParamsWords {
    s32 v[3];
};

struct NetCheckSsidBuf {
    char buf[0x21];
    s32 e;
};

struct NetCheckSsidLenBuf {
    u16 ssidLength;
    char returnCd[4];
};

struct NetCheckResultPad {
    u8 v[0x1c4];
    NetCheckResultPad() {}
    ~NetCheckResultPad() {}
};

extern "C" s32 DwcHttp_Destroy(DwcHttp *c);

extern "C" {
extern s32 data_0220064c;
extern char *sNasHttpParams;

void OS_LockMutex(void *p);
void OS_UnlockMutex(void *p);
void OS_JoinThread(void *p);
void OS_Sleep(s32 t);
s32 OS_DisableInterrupts(void);
void OS_RestoreInterrupts(s32 v);
void DWCi_BM_GetWiFiInfo(u64 *out);
s32 strlen(const char *s);
void strncpy(char *dst, const char *src, u32 n);
void MI_CpuFill8(void *p, u32 v, u32 n);
void MI_CpuCopy8(const void *src, void *dst, u32 n);
s32 OS_IsThreadTerminated(void *);
void OS_CreateThread(void *, void (*)(), void *, void *, u32, u32);
void OS_WakeupThreadDirect(void *);
void OS_InitMutex(void *);

void DwcHttp_Abort(DwcHttp *c);
void NasAuth_Abort();
s32 DwcHttp_Init(DwcHttp *c, DwcHttpParams *r);
s32 DwcHttp_FinishHeaders(DwcHttp *c);
void DwcHttp_StartThread(DwcHttp *c);
s32 DwcHttp_ParseResponse(DwcHttpField *tbl, s32 n, s32 flag, char *text);
char *DwcHttp_FindField(void *tbl, s32 n, const char *name);
s32 DwcHttp_GetFieldDecoded(void *tbl, s32 n, const char *name, char *out, s32 cap);
s32 NasAuth_BuildRequest(DwcHttp *c, const char *a, const char *b, void *tbl, s32 n, s32 k);
s32 DwcHttp_AddFormParam(DwcHttp *c, const char *a, const char *b, s32 n);
s32 DwcHttp_AppendBody(DwcHttp *c, char *p);
s32 NasAuth_Start(NasAuthParams *o, DwcHttp *c);
s32 NasAuth_JoinThread(void);
s32 NasAuth_GetState(void);
s32 NasAuth_Destroy(void);
s32 NasAuth_GetResult(s32 *out);
u8 *WifiLink_GetConnectedSsid(u16 *out);

}

extern "C" {
extern char sNetCheckUrl[0x24];
char *sNetCheckUrlPtr = sNetCheckUrl;
DwcHttp *sNetCheckHttp;
char sNetCheckUrl[0x24] = "http://conntest.nintendowifi.net/";
NetCheckWork *sNetCheck;
DwcHttpParams sNetCheckHttpParams;
NasAuthParams sNetCheckNasConfig;

s32 NetCheck_GetErrorCode(void);
void NetCheck_SetState(s32 v);
s32 NetCheck_GetState(void);
void NetCheck_ThreadMain(void);
void NetCheck_Abort();
void NetCheck_StartThread();
void NetCheck_Destroy();
s32 NetCheck_Start(NetCheckParams *cfg);

s32 NetCheck_Start(NetCheckParams *cfg) {
    if (sNetCheck != 0) {
        return 4;
    }
    sNetCheck = (NetCheckWork *)cfg->allocFunc("DWCnetcheck", sizeof(NetCheckWork));
    if (sNetCheck == 0) {
        return 4;
    }
    MI_CpuFill8(sNetCheck, 0, sizeof(NetCheckWork));
    sNetCheck->errorCode = -0x1869f;
#ifdef HOST64
    // (field by field: the word view is the DS's 32-bit layout, and the function pointers here are 8 bytes)
    sNetCheck->allocFunc = cfg->allocFunc;
    sNetCheck->freeFunc = cfg->freeFunc;
    sNetCheck->pad110 = cfg->unk_08;
#else
    *(NetCheckParamsWords *)&sNetCheck->allocFunc = *(NetCheckParamsWords *)cfg;
#endif
    if (sNetCheckHttp != 0) {
        return 4;
    }
    sNetCheckHttp = (DwcHttp *)sNetCheck->allocFunc("DWChttp", sizeof(DwcHttp));
    if (sNetCheckHttp == 0) {
        return 4;
    }
    OS_InitMutex(&sNetCheck->mutex);
    NetCheck_StartThread();
    return 0;
}

void NetCheck_Destroy() {
    NetCheckWork *g;
#ifdef HOST64
    // (its thread first ends: the work holds the thread, and a thread still listed in freed memory broke the OS's
    // thread list once the memory was used again - the thread runs at the caller's priority and was not always done)
    if (sNetCheck != 0 && sNetCheck->thread.id != 0 && OS_IsThreadTerminated(&sNetCheck->thread) == 0) {
        if (sNetCheckHttp != 0) {
            DwcHttp_Abort(sNetCheckHttp);
        }
        OS_JoinThread(&sNetCheck->thread);
    }
#endif
    if (sNetCheckHttp != 0) {
        DwcHttp_Destroy(sNetCheckHttp);
        sNetCheck->freeFunc("DWChttp", sNetCheckHttp, 0);
        sNetCheckHttp = 0;
    }
    NasAuth_Destroy();
    g = sNetCheck;
    if (g != 0) {
        if (g->body302 != 0) {
            g->freeFunc("DWCnetcheck->body_302", g->body302, 0);
            sNetCheck->body302 = 0;
        }
        g = sNetCheck;
        if (g->bodyWayport != 0) {
            g->freeFunc("DWCnetcheck->body_wayport", g->bodyWayport, 0);
            sNetCheck->bodyWayport = 0;
        }
        sNetCheck->freeFunc("DWCnetcheck", sNetCheck, 0);
        sNetCheck = 0;
    }
}

void NetCheck_StartThread() {
    NetCheckWork *g = sNetCheck;
    if (g->thread.id == 0 || OS_IsThreadTerminated(&g->thread) != 0) {
        g = sNetCheck;
        OS_CreateThread(&g->thread, NetCheck_ThreadMain, g, g->stack + sizeof(g->stack), sizeof(g->stack), 0x10);
        g = sNetCheck;
        OS_WakeupThreadDirect(&g->thread);
    }
}

void NetCheck_Abort() {
    if (sNetCheck != 0) {
        if (sNetCheckHttp != 0) {
            DwcHttp_Abort(sNetCheckHttp);
        }
        NasAuth_Abort();
        if (sNetCheck->thread.id != 0) {
            OS_JoinThread(&sNetCheck->thread);
        }
        sNetCheck->errorCode = -7;
    }
}

void NetCheck_ThreadMain(void) {
    char *a = 0;
    s32 n1;
    char *b = 0;
    s32 n2;
    char *c = 0;
    s32 t;
    char *p1;
    char *p2;
    char *loc1;
    char *loc2;
    DwcHttp *cx;
    s32 v;
    s32 n3;
    s32 n;
    char *p;
    NetCheckSsidLenBuf pn;
    NasUserIdInfo tk;
    NetCheckSsidBuf bb;
    NetCheckResultPad pad;

    for (;;) {
        sNetCheckHttpParams.url = sNetCheckUrlPtr;
        sNetCheckHttpParams.method = 1;
        sNetCheckHttpParams.userRecvBuffer = 0;
        sNetCheckHttpParams.rxBufSize = 0x1000;
        sNetCheckHttpParams.allocFunc = sNetCheck->allocFunc;
        sNetCheckHttpParams.freeFunc = sNetCheck->freeFunc;
        sNetCheckHttpParams.timeoutMs = 0x4e20;
        sNetCheck->errorCode = -2;
        if (DwcHttp_Init(sNetCheckHttp, &sNetCheckHttpParams)) {
            DwcHttp_Destroy(sNetCheckHttp);
            NetCheck_SetState(1);
            goto end;
        }
        if (DwcHttp_FinishHeaders(sNetCheckHttp)) {
            DwcHttp_Destroy(sNetCheckHttp);
            NetCheck_SetState(1);
            goto end;
        }
        DwcHttp_StartThread(sNetCheckHttp);
        if (sNetCheckHttp->thread.id) {
            OS_JoinThread(&sNetCheckHttp->thread);
        }
        cx = sNetCheckHttp;
        switch (cx->result) {
        case 2:
            sNetCheck->errorCode = -1;
        default:
            DwcHttp_Destroy(sNetCheckHttp);
            NetCheck_SetState(3);
            goto end;
        case 8:
            break;
        }
        if (DwcHttp_ParseResponse(sNetCheck->responseFields, 0x20, 0, (char *)cx->responseBuffer.base) != 1) {
            DwcHttp_Destroy(sNetCheckHttp);
            NetCheck_SetState(2);
            goto end;
        }
        v = atol(DwcHttp_FindField(sNetCheck->responseFields, 0x20, "httpresult"));
        if (data_0220064c == 0x22) {
            NetCheck_SetState(2);
            goto end;
        }
        if (v == 200) {
        } else if (v == 0x12e) {
        if (sNetCheck->bodyWayport != 0) {
            sNetCheck->errorCode = -6;
            DwcHttp_Destroy(sNetCheckHttp);
            sNetCheckHttpParams.url = sNasHttpParams;
            sNetCheckHttpParams.method = 0;
            sNetCheckHttpParams.userRecvBuffer = 0;
            sNetCheckHttpParams.rxBufSize = 0x200;
            sNetCheckHttpParams.allocFunc = sNetCheck->allocFunc;
            sNetCheckHttpParams.freeFunc = sNetCheck->freeFunc;
            sNetCheckHttpParams.timeoutMs = 0x4e20;
            if (strcmp(sNetCheckHttpParams.url, "https://nas.nintendowifi.net/ac")) {
                sNetCheckHttpParams.useTestServer = 1;
            }
            if (DwcHttp_Init(sNetCheckHttp, &sNetCheckHttpParams)) {
                DwcHttp_Destroy(sNetCheckHttp);
                NetCheck_SetState(1);
                goto end;
            }
            if (NasAuth_BuildRequest(sNetCheckHttp, "", "",
                                    sNetCheck->responseFields, 0x20, 1)) {
                DwcHttp_Destroy(sNetCheckHttp);
                NetCheck_SetState(8);
                goto end;
            }
            if (DwcHttp_AddFormParam(sNetCheckHttp, "action", "message", 7)) {
                DwcHttp_Destroy(sNetCheckHttp);
                NetCheck_SetState(8);
                goto end;
            }
            {
                s32 ie = OS_DisableInterrupts();
                MI_CpuFill8(bb.buf, 0, 0x21);
                WifiLink_GetConnectedSsid(&pn.ssidLength);
                MI_CpuCopy8(WifiLink_GetConnectedSsid(0), bb.buf, pn.ssidLength);
                OS_RestoreInterrupts(ie);
            }
            if (DwcHttp_AddFormParam(sNetCheckHttp, "ssid", bb.buf, strlen(bb.buf))) {
                DwcHttp_Destroy(sNetCheckHttp);
                NetCheck_SetState(8);
                goto end;
            }
            p = sNetCheck->bodyWayport;
            if (DwcHttp_AddFormParam(sNetCheckHttp, "HotSpotResponse", p, strlen(p))) {
                DwcHttp_Destroy(sNetCheckHttp);
                NetCheck_SetState(8);
                goto end;
            }
            sNetCheck->freeFunc("DWCnetcheck->body_wayport", sNetCheck->bodyWayport, 0);
            sNetCheck->bodyWayport = 0;
            if (DwcHttp_FinishHeaders(sNetCheckHttp)) {
                DwcHttp_Destroy(sNetCheckHttp);
                NetCheck_SetState(1);
                goto end;
            }
            DwcHttp_StartThread(sNetCheckHttp);
            if (sNetCheckHttp->thread.id) {
                OS_JoinThread(&sNetCheckHttp->thread);
            }
            switch (sNetCheckHttp->result) {
            case 2:
                sNetCheck->errorCode = -1;
            default:
                DwcHttp_Destroy(sNetCheckHttp);
                NetCheck_SetState(3);
                goto end;
            case 8:
                DwcHttp_Destroy(sNetCheckHttp);
                NetCheck_SetState(7);
                goto end;
            }
        } else {
            loc1 = DwcHttp_FindField(sNetCheck->responseFields, 0x20, "httpbody");
            if (loc1 == 0) {
                DwcHttp_Destroy(sNetCheckHttp);
                NetCheck_SetState(2);
                goto end;
            }
            sNetCheck->body302 =
                (char *)sNetCheck->allocFunc("DWCnetcheck->body_302", strlen(loc1) + 1);
            p1 = sNetCheck->body302;
            if (p1 == 0) {
                DwcHttp_Destroy(sNetCheckHttp);
                NetCheck_SetState(4);
                goto end;
            }
            strncpy(p1, loc1, strlen(loc1));
        }
        } else {
            DwcHttp_Destroy(sNetCheckHttp);
            NetCheck_SetState(10);
            goto end;
        }
        DwcHttp_Destroy(sNetCheckHttp);
        DWCi_BM_GetWiFiInfo(&tk.userId);
        if (tk.userId == 0) {
            sNetCheck->errorCode = -3;
            sNetCheckNasConfig.inGameName[0] = 0;
            sNetCheckNasConfig.inGameName[1] = 0;
            sNetCheckNasConfig.gsbrcd[0] = 0;
            sNetCheckNasConfig.allocFunc = sNetCheck->allocFunc;
            sNetCheckNasConfig.freeFunc = sNetCheck->freeFunc;
            if (NasAuth_Start(&sNetCheckNasConfig, sNetCheckHttp)) {
                NetCheck_SetState(5);
                goto end;
            }
            NasAuth_JoinThread();
            if (NasAuth_GetState() != 0x14) {
                if (NasAuth_GetState() == 9) {
                    sNetCheck->errorCode = -1;
                } else {
#ifdef HOST64
                    {   // (the whole result, 0x1c4 bytes: on the DS it ran on into the frame's next local)
                        s32 res[0x1c4 / 4];
                        NasAuth_GetResult(res);
                        sNetCheck->errorCode = res[0];
                    }
#else
                    NasAuth_GetResult(&bb.e);
                    sNetCheck->errorCode = bb.e;
#endif
                }
                NetCheck_SetState(6);
                goto end;
            }
            NasAuth_Destroy();
        }
        if (v == 200) {
            sNetCheck->errorCode = 0;
            NetCheck_SetState(0xb);
            goto end;
        }
        sNetCheck->errorCode = -4;
        sNetCheckHttpParams.url = sNasHttpParams;
        sNetCheckHttpParams.method = 0;
        sNetCheckHttpParams.userRecvBuffer = 0;
        sNetCheckHttpParams.rxBufSize = 0x1000;
        sNetCheckHttpParams.allocFunc = sNetCheck->allocFunc;
        sNetCheckHttpParams.freeFunc = sNetCheck->freeFunc;
        sNetCheckHttpParams.timeoutMs = 0x9c40;
        if (strcmp(sNetCheckHttpParams.url, "https://nas.nintendowifi.net/ac")) {
            sNetCheckHttpParams.useTestServer = 1;
        }
        if (DwcHttp_Init(sNetCheckHttp, &sNetCheckHttpParams)) {
            DwcHttp_Destroy(sNetCheckHttp);
            NetCheck_SetState(1);
            goto end;
        }
        if (NasAuth_BuildRequest(sNetCheckHttp, "", "",
                                sNetCheck->responseFields, 0x20, 1)) {
            DwcHttp_Destroy(sNetCheckHttp);
            NetCheck_SetState(8);
            goto end;
        }
        if (DwcHttp_AddFormParam(sNetCheckHttp, "action", "parse", 5)) {
            DwcHttp_Destroy(sNetCheckHttp);
            NetCheck_SetState(8);
            goto end;
        }
        {
            s32 ie = OS_DisableInterrupts();
            MI_CpuFill8(bb.buf, 0, 0x21);
            WifiLink_GetConnectedSsid(&pn.ssidLength);
            MI_CpuCopy8(WifiLink_GetConnectedSsid(0), bb.buf, pn.ssidLength);
            OS_RestoreInterrupts(ie);
        }
        if (DwcHttp_AddFormParam(sNetCheckHttp, "ssid", bb.buf, strlen(bb.buf))) {
            DwcHttp_Destroy(sNetCheckHttp);
            NetCheck_SetState(8);
            goto end;
        }
        p = sNetCheck->body302;
        if (DwcHttp_AddFormParam(sNetCheckHttp, "HTML", p, strlen(p))) {
            DwcHttp_Destroy(sNetCheckHttp);
            NetCheck_SetState(8);
            goto end;
        }
        sNetCheck->freeFunc("DWCnetcheck->body_302", sNetCheck->body302, 0);
        sNetCheck->body302 = 0;
        if (DwcHttp_FinishHeaders(sNetCheckHttp)) {
            DwcHttp_Destroy(sNetCheckHttp);
            NetCheck_SetState(1);
            goto end;
        }
        DwcHttp_StartThread(sNetCheckHttp);
        if (sNetCheckHttp->thread.id) {
            OS_JoinThread(&sNetCheckHttp->thread);
        }
        cx = sNetCheckHttp;
        switch (cx->result) {
        case 2:
            sNetCheck->errorCode = -1;
        default:
            DwcHttp_Destroy(sNetCheckHttp);
            NetCheck_SetState(3);
            goto end;
        case 8:
            break;
        }
        if (DwcHttp_ParseResponse(sNetCheck->responseFields, 0x20, 0, (char *)cx->responseBuffer.base) != 1) {
            DwcHttp_Destroy(sNetCheckHttp);
            NetCheck_SetState(2);
            goto end;
        }
        v = atol(DwcHttp_FindField(sNetCheck->responseFields, 0x20, "httpresult"));
        if (data_0220064c == 0x22) {
            DwcHttp_Destroy(sNetCheckHttp);
            NetCheck_SetState(2);
            goto end;
        }
        if (v != 200) {
            DwcHttp_Destroy(sNetCheckHttp);
            NetCheck_SetState(2);
            goto end;
        }
        if (DwcHttp_GetFieldDecoded(sNetCheck->responseFields, 0x20, "returncd", pn.returnCd, 4) <= 0) {
            DwcHttp_Destroy(sNetCheckHttp);
            NetCheck_SetState(9);
            goto end;
        }
        v = atol(pn.returnCd);
        if (data_0220064c == 0x22) {
            DwcHttp_Destroy(sNetCheckHttp);
            NetCheck_SetState(9);
            goto end;
        }
        if (v >= 100) {
            DwcHttp_Destroy(sNetCheckHttp);
            NetCheck_SetState(6);
            goto end;
        }
        n1 = DwcHttp_GetFieldDecoded(sNetCheck->responseFields, 0x20, "url", 0, 0);
        if (n1 <= 0) {
            DwcHttp_Destroy(sNetCheckHttp);
            NetCheck_SetState(9);
            goto end;
        }
        n2 = DwcHttp_GetFieldDecoded(sNetCheck->responseFields, 0x20, "data", 0, 0);
        if (n2 <= 0) {
            DwcHttp_Destroy(sNetCheckHttp);
            NetCheck_SetState(9);
            goto end;
        }
        n3 = DwcHttp_GetFieldDecoded(sNetCheck->responseFields, 0x20, "wait", 0, 0);
        a = (char *)sNetCheck->allocFunc("url", n1 + 1);
        if (a == 0) {
            DwcHttp_Destroy(sNetCheckHttp);
            NetCheck_SetState(4);
            goto end;
        }
        b = (char *)sNetCheck->allocFunc("data", n2 + 1);
        if (b == 0) {
            DwcHttp_Destroy(sNetCheckHttp);
            NetCheck_SetState(4);
            goto end;
        }
        if (n3 > 0) {
            c = (char *)sNetCheck->allocFunc("wait", n3 + 1);
            if (c == 0) {
                DwcHttp_Destroy(sNetCheckHttp);
                NetCheck_SetState(4);
                goto end;
            }
        }
        n = DwcHttp_GetFieldDecoded(sNetCheck->responseFields, 0x20, "url", a, n1 + 1);
        if (n < 0) {
            DwcHttp_Destroy(sNetCheckHttp);
            NetCheck_SetState(9);
            goto end;
        }
        a[n] = 0;
        n = DwcHttp_GetFieldDecoded(sNetCheck->responseFields, 0x20, "data", b, n2 + 1);
        if (n < 0) {
            DwcHttp_Destroy(sNetCheckHttp);
            NetCheck_SetState(9);
            goto end;
        }
        b[n] = 0;
        t = 0;
        if (n3 > 0) {
            n = DwcHttp_GetFieldDecoded(sNetCheck->responseFields, 0x20, "wait", c, n3 + 1);
            if (n < 0) {
                DwcHttp_Destroy(sNetCheckHttp);
                NetCheck_SetState(9);
                goto end;
            }
            c[n] = 0;
            t = atol(c);
            if (data_0220064c == 0x22) {
                DwcHttp_Destroy(sNetCheckHttp);
                NetCheck_SetState(9);
                goto end;
            }
            t = t * 1000;
            if (t > 0x2bf20) {
                t = 0x2bf20;
            }
        }
        DwcHttp_Destroy(sNetCheckHttp);
        sNetCheck->errorCode = -5;
        sNetCheckHttpParams.url = a;
        sNetCheckHttpParams.method = 0;
        sNetCheckHttpParams.userRecvBuffer = 0;
        sNetCheckHttpParams.rxBufSize = 0x1000;
        sNetCheckHttpParams.allocFunc = sNetCheck->allocFunc;
        sNetCheckHttpParams.freeFunc = sNetCheck->freeFunc;
        sNetCheckHttpParams.timeoutMs = 0x1d4c0;
        if (DwcHttp_Init(sNetCheckHttp, &sNetCheckHttpParams)) {
            DwcHttp_Destroy(sNetCheckHttp);
            NetCheck_SetState(1);
            goto end;
        }
        if (DwcHttp_AppendBody(sNetCheckHttp, b)) {
            DwcHttp_Destroy(sNetCheckHttp);
            NetCheck_SetState(8);
            goto end;
        }
        if (DwcHttp_FinishHeaders(sNetCheckHttp)) {
            DwcHttp_Destroy(sNetCheckHttp);
            NetCheck_SetState(1);
            goto end;
        }
        DwcHttp_StartThread(sNetCheckHttp);
        if (sNetCheckHttp->thread.id) {
            OS_JoinThread(&sNetCheckHttp->thread);
        }
        cx = sNetCheckHttp;
        switch (cx->result) {
        case 2:
            sNetCheck->errorCode = -1;
        default:
            DwcHttp_Destroy(sNetCheckHttp);
            NetCheck_SetState(3);
            goto end;
        case 8:
            break;
        }
        if (DwcHttp_ParseResponse(sNetCheck->responseFields, 0x20, 1, (char *)cx->responseBuffer.base) != 1) {
            DwcHttp_Destroy(sNetCheckHttp);
            NetCheck_SetState(2);
            goto end;
        }
        loc2 = DwcHttp_FindField(sNetCheck->responseFields, 0x20, "httpbody");
        if (loc2 == 0) {
            DwcHttp_Destroy(sNetCheckHttp);
            NetCheck_SetState(2);
            goto end;
        }
        sNetCheck->bodyWayport =
            (char *)sNetCheck->allocFunc("DWCnetcheck->body_wayport", strlen(loc2) + 1);
        p2 = sNetCheck->bodyWayport;
        if (p2 == 0) {
            DwcHttp_Destroy(sNetCheckHttp);
            NetCheck_SetState(4);
            goto end;
        }
        strncpy(p2, loc2, strlen(loc2));
        DwcHttp_Destroy(sNetCheckHttp);
        OS_Sleep(t);
    }
end:
    if (a) {
        sNetCheck->freeFunc("url", a, 0);
    }
    if (b) {
        sNetCheck->freeFunc("data", b, 0);
    }
    if (c) {
        sNetCheck->freeFunc("wait", c, 0);
    }
}

s32 NetCheck_GetState(void) {
    NetCheckWork *g;
    s32 r;
    OS_LockMutex(&sNetCheck->mutex);
    r = sNetCheck->state;
    OS_UnlockMutex(&sNetCheck->mutex);
    return r;
}

void NetCheck_SetState(s32 v) {
    OS_LockMutex(&sNetCheck->mutex);
    sNetCheck->state = v;
    OS_UnlockMutex(&sNetCheck->mutex);
}

s32 NetCheck_GetErrorCode(void) {
    return sNetCheck->errorCode;
}

// Not in the original binary (unreferenced, not in symbols.txt: dead-stripped by the link). Defined last so that it is compiled
// first: it creates the five buffer tags in the order the original literal pool has them (the original pool starts with them).
__declspec(weak) void Unk_ov065_0226ecfc_pool_order(void) {
    strlen("DWCnetcheck->body_302");
    strlen("url");
    strlen("data");
    strlen("wait");
    strlen("DWCnetcheck->body_wayport");
}

}

// ---- overlay variables (generated: tools/host_overlay_vars.py) ----
#include "host_overlay_vars.h"

HOST_OVERLAY_VARS(unk_ov065_0226ec94) {
    HOST_OVERLAY_VAR(sNetCheckUrlPtr),
    HOST_OVERLAY_VAR(sNetCheckHttp),
    HOST_OVERLAY_VAR(sNetCheckUrl),
    HOST_OVERLAY_VAR(sNetCheck),
    HOST_OVERLAY_VAR(sNetCheckHttpParams),
    HOST_OVERLAY_VAR(sNetCheckNasConfig),
    HOST_OVERLAY_VARS_END
};
