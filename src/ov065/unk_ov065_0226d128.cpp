// mwcc-flags: -O4,p -str reuse
#include "types.h"
#include "host_mem.h"
#include "net/DwcHttp.h"
#include "net/NasAuthWork.h"
#include "nitro/os_rtc.h"

#include "types64.h"
#include "proto/autoload_2/unk_02115468.h"
#include "proto/autoload_2/unk_02128030.h"
#include "proto/autoload_2/unk_0212a120.h"
#include "proto/autoload_2/unk_0212b770.h"
#include "proto/itcm/unk_01ffa4a0.h"
#include "proto/ov065/unk_ov065_0226ad84.h"
#include "proto/ov065/unk_ov065_0226d128.h"
#include "proto/ov065/unk_ov065_0226f9e0.h"











typedef NasAuthWork S;

// Word view of NasAuthParams for the struct copy in NasAuth_Start (a copy of the field-typed struct compiles differently).
struct NasAuthParamsWords {
    u32 v[11];
};

extern "C" s32 DwcHttp_Destroy(void *p);

extern "C" {
extern S *sNasAuth;
extern NasUserIdInfo sNasUserId;
extern u32 data_0220064c;
extern char sNasLangCode01[4];
extern char sNasLangCode03[4];
extern char sNasLangCode04[4];
extern char sNasLangCode02[4];
extern char sNasLangCode05[4];
extern char sNasLangCode00[4];
extern char sNasLangCode06[4];
extern char *sNasLangCodeTable[7];
extern char sNasDefaultUrl[0x20];
extern DwcHttpParams sNasHttpParams;

extern void MI_CpuCopy8(const void *src, void *dst, u32 n);
extern void MI_CpuFill8(void *dst, u32 v, u32 n);
extern void OS_GetMacAddress(void *p);
extern s32 RTC_GetDate(void *p);
extern s32 RTC_GetTime(void *p);
extern u32 OS_DisableInterrupts(void);
extern void OS_RestoreInterrupts(u32 v);
extern s32 OS_SPrintf(char *buf, const char *fmt, ...);
extern s32 OS_SNPrintf(char *buf, u32 n, const char *fmt, ...);
extern s32 strlen(const char *s);
extern s32 wcslen(const void *s);
extern void DWCi_BM_GetWiFiInfo(void *p);
extern void OS_LockMutex(void *m);
extern void OS_UnlockMutex(void *m);
extern s32 OS_InitMutex(void *m);
extern s32 DWCi_AUTH_UpDateWiFiID(void *p, u8 *buf);
extern void DWCi_AUTH_RemakeWiFiID(void *p);
extern void DWCi_AUTH_MakeWiFiID(u8 *buf);
extern void OS_JoinThread(void *p);
extern s32 OS_IsThreadTerminated(void *t);
extern s32 OS_WakeupThreadDirect(void *t);
extern s32 OS_CreateThread(void *t, s32 (*fn)(void *), void *arg, void *stack, u32 size, u32 prio);
extern void OS_Sleep(u32 ms);

extern void WifiAp_GetNdwcshapApInfo(u8 *a, void *dst);
extern u8 *WifiLink_GetConnectedBssid(void);
extern u8 *WifiLink_GetConnectedSsid(u16 *out);
extern s32 DwcHttp_AddField(void *l, const char *k, const char *v);
extern s32 DwcHttp_AddHeader(void *a, const char *k, const char *v);
extern s32 DwcHttp_AddFormParam(void *a, const char *k, const char *v, u32 n);
extern char *DwcHttp_FindField(void *buf, s32 n, const char *key);
extern s32 DwcHttp_GetFieldDecoded(void *buf, s32 n, const char *key, char *out, u32 max);
extern s32 DwcHttp_GetFieldString(void *buf, s32 n, const char *key, char *out, s32 max);
extern s32 DwcHttp_ParseResponse(DwcHttpField *tbl, s32 n, s32 flag, char *text);
extern s32 DwcHttp_Init(DwcHttp *c, DwcHttpParams *cfg);
extern s32 DwcHttp_FinishHeaders(DwcHttp *c);
extern void DwcHttp_StartThread(DwcHttp *c);
extern s32 DwcHttp_Abort(void *c);

void NasAuth_SetState(s32 v);
s32 NasAuth_BuildRequest(void *a0, const char *a1, const u16 *a2, DwcHttpField *a3, s32 a4, s32 a5);
s32 NasAuth_ParseResponse(void);
s32 NasAuth_HandleResponse(void);
void NasAuth_ThreadMain(void);
s32 NasAuth_SendRequest(s32 a);
void NasAuth_GetResult(s32 *p);
s32 NasAuth_GetState(void);
void NasAuth_JoinThread(void);
void NasAuth_Destroy(void);
void NasAuth_Abort(void);
void NasAuth_StartThread(void);
s32 NasAuth_Start(NasAuthParams *cfg, DwcHttp *http);
void NasAuth_SetServerUrl(char *s);
}

extern "C" {
char sNasLangCode04[4] = "04";
NasUserIdInfo sNasUserId;
char sNasLangCode01[4] = "01";
char sNasLangCode06[4] = "06";
char sNasDefaultUrl[0x20] = "https://nas.nintendowifi.net/ac";
char sNasLangCode00[4] = "00";
char sNasLangCode05[4] = "05";
char sNasLangCode02[4] = "02";
DwcHttpParams sNasHttpParams = {sNasDefaultUrl, 0, 0, 0x1000, 0, 0, 0, 0x4e20};
char sNasLangCode03[4] = "03";
S *sNasAuth;
char *sNasLangCodeTable[7] = {sNasLangCode00, sNasLangCode01, sNasLangCode02,
                                sNasLangCode03, sNasLangCode04, sNasLangCode05,
                                sNasLangCode06};
}

extern "C" {

void NasAuth_SetServerUrl(char *s) {
    sNasHttpParams.url = s;
}

s32 NasAuth_Start(NasAuthParams *cfg, DwcHttp *http) {
    if (sNasAuth != NULL) {
        return 2;
    }
    void *p = cfg->allocFunc("DWCAuth", sizeof(NasAuthWork));
    if (p == NULL) {
        return 2;
    }
    sNasAuth = (S *)p;
    MI_CpuFill8(p, 0, sizeof(NasAuthWork));
    sNasAuth->http = http;
    MI_CpuFill8(&sNasAuth->resultCode, 0, 0x1c4);
    sNasAuth->resultCode = -1;
    sNasAuth->config = *(NasAuthParams *)cfg;  // (allocFunc / freeFunc: pointers, wider on 64-bit hosts)
    sNasAuth->config.inGameName[0x14] = 0; // (the u16 name ends after 10 characters)
    sNasAuth->config.inGameName[0x15] = 0;
    sNasAuth->config.gsbrcd[0xb] = 0;
    sNasHttpParams.allocFunc = cfg->allocFunc;
    sNasHttpParams.freeFunc = cfg->freeFunc;
    sNasAuth->state = NasAuth_SendRequest(1);
    if (sNasAuth->state == 0) {
        NasAuth_StartThread();
        return 0;
    }
    return sNasAuth->state;
}

void NasAuth_StartThread(void) {
    OS_InitMutex(&sNasAuth->mutex);
    sNasAuth->isAborting = 0;
    if (sNasAuth->thread.id == 0 || OS_IsThreadTerminated(&sNasAuth->thread) != 0) {
        OS_CreateThread(&sNasAuth->thread, (s32 (*)(void *))NasAuth_ThreadMain, &sNasAuth,
                      sNasAuth->stack + sizeof(sNasAuth->stack), sizeof(sNasAuth->stack), 0x10);
        OS_WakeupThreadDirect(&sNasAuth->thread);
    }
}

void NasAuth_Abort(void) {
    if (sNasAuth != NULL) {
        OS_LockMutex(&sNasAuth->mutex);
        sNasAuth->isAborting = 1;
        OS_UnlockMutex(&sNasAuth->mutex);
        if (sNasAuth->http) {
            DwcHttp_Abort(sNasAuth->http);
        }
        if (sNasAuth->thread.id) {
            OS_JoinThread(&sNasAuth->thread);
        }
    }
}

void NasAuth_Destroy(void) {
    if (sNasAuth != NULL) {
        if (sNasAuth->http) {
            DwcHttp_Destroy(sNasAuth->http);
        }
        sNasAuth->config.freeFunc("DWCauth", sNasAuth, 0);
        sNasAuth = NULL;
    }
}

void NasAuth_JoinThread(void) {
    if (sNasAuth->thread.id) {
        OS_JoinThread(&sNasAuth->thread);
    }
}

s32 NasAuth_GetState(void) {
    s32 r;
    if (sNasAuth == NULL) {
        return 0x15;
    }
    OS_LockMutex(&sNasAuth->mutex);
    r = sNasAuth->state;
    OS_UnlockMutex(&sNasAuth->mutex);
    return r;
}

void NasAuth_GetResult(s32 *p) {
    if (sNasAuth == NULL) {
        MI_CpuFill8(p, 0, 0x1c4);
#ifdef HOST64
        return;  // (the DS went on to copy from address 8)
#endif
    }
    MI_CpuCopy8(&sNasAuth->resultCode, p, 0x1c4);
    s32 v = p[0];
    if (v >= 0) {
        if (v < 20000 || v >= 30000) {
            p[0] = -20998;
        }
    } else if (v > -20000 || v <= -30000) {
        p[0] = -20998;
    }
}

s32 NasAuth_SendRequest(s32 a) {
    if (strcmp(sNasHttpParams.url, sNasDefaultUrl)) {
        sNasHttpParams.useTestServer = 1;
    }
    if (DwcHttp_Init(sNasAuth->http, &sNasHttpParams)) {
        return 4;
    }
    if (a == 1) {
        DWCi_BM_GetWiFiInfo(&sNasUserId);
    }
    sNasAuth->state = NasAuth_BuildRequest(
        sNasAuth->http, sNasAuth->config.gsbrcd, (u16 *)sNasAuth->config.inGameName,
        sNasAuth->httpFields, 0x20, 0);
    if (sNasAuth->state != 0) {
        return 4;
    }
    if (DwcHttp_FinishHeaders(sNasAuth->http)) {
        return 4;
    }
    DwcHttp_StartThread(sNasAuth->http);
    return 0;
}

void NasAuth_ThreadMain(void) {
    s32 tries = 0;
    s32 flag;
    s32 z0 = 0;
    s32 z1 = 0;
    s32 z2 = 0;
    s32 r;
    s64 t0;
    s64 ms;
    DwcHttp *o;
    S *g;

    for (;;) {
        o = sNasAuth->http;
        if (o->thread.id != 0) {
            OS_JoinThread(&o->thread);
        }
        g = sNasAuth;
        if (g->http->result != 8) {
            g->resultCode = -0x4e84;
            r = sNasAuth->http->result;
            if (r == 7) {
                NasAuth_SetState(0x13);
                return;
            }
            if (tries > 2) {
                if (r == 2) {
                    NasAuth_SetState(9);
                    return;
                }
                NasAuth_SetState(0xc);
                return;
            }
            tries++;
            flag = 1;
        } else {
            r = NasAuth_HandleResponse();
            switch (r) {
            case 0x14:
                NasAuth_SetState(0x14);
                return;
            case 0xf:
                if (tries >= 2) {
                    NasAuth_SetState(0xf);
                    sNasAuth->resultCode = -sNasAuth->resultCode;
                    return;
                }
                tries++;
                flag = z1;
                break;
            case 0x10:
                sNasAuth->resultCode = -sNasAuth->resultCode;
                NasAuth_SetState(r);
                return;
            default:
                if (tries >= 2) {
                    sNasAuth->resultCode = -sNasAuth->resultCode;
                    NasAuth_SetState(r);
                    return;
                }
                tries++;
                flag = 1;
                break;
            }
        }
        t0 = OS_GetTick();
        while ((u64)((OS_GetTick() - t0) * 64) / 0x82ea < 0x1388) {
            OS_LockMutex(&sNasAuth->mutex);
            if (sNasAuth->isAborting == 1) {
                sNasAuth->resultCode = -0x4e84;
                OS_UnlockMutex(&sNasAuth->mutex);
                NasAuth_SetState(0x13);
                return;
            }
            OS_UnlockMutex(&sNasAuth->mutex);
            OS_Sleep(0x1388);
        }
        DwcHttp_Destroy(sNasAuth->http);
        OS_LockMutex(&sNasAuth->mutex);
        sNasAuth->state = NasAuth_SendRequest(flag);
        if (sNasAuth->state != 0) {
            sNasAuth->resultCode = -0x4e84;
            OS_UnlockMutex(&sNasAuth->mutex);
            return;
        }
        OS_UnlockMutex(&sNasAuth->mutex);
    }
}

s32 NasAuth_HandleResponse(void) {
    S *g;
    char *m;
    void *r;

    g = sNasAuth;
    if (DwcHttp_ParseResponse(g->httpFields, 0x20, 0, (char *)g->http->responseBuffer.base) != 1) {
        sNasAuth->resultCode = 0x4e84;
        return 0xd;
    }
    if (NasAuth_ParseResponse() != 0) {
        return 0xd;
    }
    g = sNasAuth;
    s32 st = g->resultCode;
    if (st < 0x4e84) {
        if (st == 0x4e22) {
            m = "bmwork";
            r = g->config.allocFunc(m, 0x71f);
            if (r == 0) {
                sNasAuth->resultCode = 0x4e84;
                return 2;
            }
            if (DWCi_AUTH_UpDateWiFiID(&sNasUserId, (u8 *)(((uptr)r + 0x1f) & ~(uptr)0x1f)) != 1) {
                sNasAuth->config.freeFunc(m, r, 0);
                sNasAuth->resultCode = 0x4e84;
                return 0xe;
            }
            sNasAuth->config.freeFunc(m, r, 0);
        }
        return 0x14;
    }
    switch (st) {
    case 0x4e88:
        DWCi_AUTH_RemakeWiFiID(&sNasUserId);
        sNasAuth->resultCode = 0x4e88;
        return 0xf;
    case 0x4e8c:
        m = "bmwork";
        r = g->config.allocFunc(m, 0x71f);
        if (r == 0) {
            sNasAuth->resultCode = 0x4e8c;
            return 0x10;
        }
        DWCi_AUTH_MakeWiFiID((u8 *)(((uptr)r + 0x1f) & ~(uptr)0x1f));
        sNasAuth->config.freeFunc(m, r, 0);
        sNasAuth->resultCode = 0x4e8c;
        return 0x10;
    default:
        return 0x11;
    }
}

s32 NasAuth_ParseResponse(void) {
    char *end = 0;
    S *g;

    s32 st = atol(DwcHttp_FindField(sNasAuth->httpFields, 0x20, "httpresult"));
    if (data_0220064c == 0x22) {
        sNasAuth->resultCode = 0x4e85;
        return 0xb;
    }
    if (st != 200) {
        sNasAuth->resultCode = st + 0x59d8;
        return 0x11;
    }
    g = sNasAuth;
    if (DwcHttp_GetFieldDecoded(g->httpFields, 0x20, "returncd", g->returnCd, 4) <= 0) {
        sNasAuth->resultCode = 0x4e85;
        return 0xd;
    }
    s32 code = strtol(sNasAuth->returnCd, &end, 10);
    g = sNasAuth;
    s32 l = strlen(g->returnCd);
    if (end != g->returnCd + l) {
        g->resultCode = 0x4e85;
        return 0xb;
    }
    g->resultCode = code + 0x4e20;
    if (code < 100) {
        sNasAuth->token[0] = 0;
        sNasAuth->locator[0] = 0;
        sNasAuth->challenge[0] = 0;
        sNasAuth->datetime[0] = 0;
        sNasAuth->cookie[0] = 0;
        g = sNasAuth;
        DwcHttp_GetFieldDecoded(g->httpFields, 0x20, "token", g->token, 0x12d);
        g = sNasAuth;
        DwcHttp_GetFieldDecoded(g->httpFields, 0x20, "locator", g->locator, 0x33);
        g = sNasAuth;
        DwcHttp_GetFieldDecoded(g->httpFields, 0x20, "challenge", g->challenge, 9);
        g = sNasAuth;
        DwcHttp_GetFieldDecoded(g->httpFields, 0x20, "datetime", g->datetime, 0xf);
        g = sNasAuth;
        DwcHttp_GetFieldString(g->httpFields, 0x20, "Set-Cookie", g->cookie, 0x41);
        sNasAuth->cookie[0x2b] = 0;
    }
    return 0;
}

s32 NasAuth_BuildRequest(void *a0, const char *a1, const u16 *a2, DwcHttpField *a3, s32 a4, s32 a5) {
    u16 len;
    u8 mac[6];
    u8 mac2[6];
    char code4[5];
    char code2[3];
    char birth[5];
    char pw[4];
    RTCDate date;
    RTCTime time;
    DwcHttpFieldList form;
    char macstr[13];
    char devtime[13];
    char nick[0x15];
    char bssid[13];
    u8 buf[0x21];
    char apinfo[14];
    char userid[14];
    OSOwnerInfo owner;
    s32 i;
    u32 irq;
    u8 *ptr;

    OS_GetMacAddress(mac);
    OS_GetOwnerInfo(&owner);
    if (RTC_GetDate(&date) != 0 || RTC_GetTime(&time) != 0) {
        return 5;
    }
    irq = OS_DisableInterrupts();
    ptr = WifiLink_GetConnectedBssid();
    if (ptr == 0) {
        OS_RestoreInterrupts(irq);
        return 3;
    }
    MI_CpuCopy8(ptr, mac2, 6);
    MI_CpuFill8(buf, 0, 0x21);
    ptr = WifiLink_GetConnectedSsid(&len);
    if (ptr == 0) {
        OS_RestoreInterrupts(irq);
        return 3;
    }
    MI_CpuCopy8(ptr, buf, len);
    OS_RestoreInterrupts(irq);
    MI_CpuCopy8(HW_SYSWORK_AT(void, romHeader, 0xc), code4, 4);
    code4[4] = 0;
    MI_CpuCopy8(HW_SYSWORK_AT(void, romHeader, 0x10), code2, 2);
    code2[2] = 0;
    for (i = 0; i < 6; i++) {
        OS_SPrintf(macstr + i * 2, "%02x", mac[i]);
    }
    macstr[12] = 0;
    if (owner.language > 6) {
        owner.language = 1;
    }
    OS_SNPrintf(birth, 5, "%02x%02x", owner.birthMonth, owner.birthDay);
    OS_SNPrintf(devtime, 13, "%02d%02d%02d%02d%02d%02d", date.year, date.month, date.day,
                  time.hour, time.minute, time.second);
    MI_CpuCopy8(owner.nickName, nick, 0x14);
    nick[0x14] = 0;
    MI_CpuFill8(&form, 0, sizeof form);  // (0xc on the DS)
    form.entries = a3;
    form.capacity = a4;
    if (a5 != 1) {
        if (sNasUserId.userId == 0) {
            DwcHttp_AddField(&form, "action", "acctcreate");
        } else {
            if (strlen(a1) == 0) {
                return 6;
            }
            DwcHttp_AddField(&form, "action", "login");
            DwcHttp_AddField(&form, "gsbrcd", a1);
        }
    } else {
        DWCi_BM_GetWiFiInfo(&sNasUserId);
    }
    DwcHttp_AddField(&form, "sdkver", "001000");
    if (sNasUserId.userId != 0) {
        OS_SNPrintf(userid, 14, "%013llu", sNasUserId.userId);
    } else {
        OS_SNPrintf(userid, 14, "%013llu", sNasUserId.tempUserId);
    }
    DwcHttp_AddField(&form, "userid", userid);
    OS_SNPrintf(pw, 4, "%03u", sNasUserId.password);
    DwcHttp_AddField(&form, "passwd", pw);
    OS_SNPrintf(bssid, 13, "%02x%02x%02x%02x%02x%02x", mac2[0], mac2[1], mac2[2], mac2[3], mac2[4], mac2[5]);
    OS_SNPrintf(apinfo, 14, "%02d:0000000-00", WifiAp_GetConnectedApType());
    WifiAp_GetNdwcshapApInfo(buf, apinfo + 3);
    DwcHttp_AddField(&form, "gamecd", code4);
    DwcHttp_AddField(&form, "makercd", code2);
    DwcHttp_AddField(&form, "unitcd", "0");
    DwcHttp_AddField(&form, "macadr", macstr);
    DwcHttp_AddField(&form, "lang", sNasLangCodeTable[owner.language]);
    DwcHttp_AddField(&form, "birth", birth);
    DwcHttp_AddField(&form, "devtime", devtime);
    DwcHttp_AddField(&form, "bssid", bssid);
    DwcHttp_AddField(&form, "apinfo", apinfo);
    if (DwcHttp_AddHeader(a0, "User-Agent", "Nitro WiFi SDK/1.0") != 0) {
        return 7;
    }
    if (DwcHttp_AddHeader(a0, "HTTP_X_GAMECD", code4) != 0) {
        return 7;
    }
    i = 0;
    for (; a3->key != 0; a3++, i++) {
        const char *v = a3->value;
        if (DwcHttp_AddFormParam(a0, a3->key, v, strlen(v)) != 0) {
            return 8;
        }
    }
    if (DwcHttp_AddFormParam(a0, "devname", nick, 0x14) != 0) {
        return 8;
    }
    if (wcslen(a2) != 0) {
        if (DwcHttp_AddFormParam(a0, "ingamesn", (const char *)a2, wcslen(a2) * 2) != 0) {
            return 8;
        }
    }
    return 0;
}

void NasAuth_SetState(s32 v) {
    OS_LockMutex(&sNasAuth->mutex);
    sNasAuth->state = v;
    OS_UnlockMutex(&sNasAuth->mutex);
}

}

// ---- overlay variables (generated: tools/host_overlay_vars.py) ----
#include "host_overlay_vars.h"

HOST_OVERLAY_VARS(unk_ov065_0226d128) {
    HOST_OVERLAY_VAR(sNasLangCode04),
    HOST_OVERLAY_VAR(sNasUserId),
    HOST_OVERLAY_VAR(sNasLangCode01),
    HOST_OVERLAY_VAR(sNasLangCode06),
    HOST_OVERLAY_VAR(sNasDefaultUrl),
    HOST_OVERLAY_VAR(sNasLangCode00),
    HOST_OVERLAY_VAR(sNasLangCode05),
    HOST_OVERLAY_VAR(sNasLangCode02),
    HOST_OVERLAY_VAR(sNasHttpParams),
    HOST_OVERLAY_VAR(sNasLangCode03),
    HOST_OVERLAY_VAR(sNasAuth),
    HOST_OVERLAY_VAR(sNasLangCodeTable),
    HOST_OVERLAY_VARS_END
};
