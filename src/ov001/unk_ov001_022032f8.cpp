// mwcc-flags: -O4,p
#include "types.h"
#include "net/WifiLinkEvent.h"
#include "net/WifiLinkConfig.h"
#include "net/WifiLinkWork.h"
#include "uptr.h"
#include "proto/ov001/unk_ov001_022032f8.h"

typedef void (*AossWcmNotifyFunc)(s32, ...);

// WCMConfig


extern "C" u8 *sAossWcmWork = 0;
extern "C" u8 *sAossWcmWepDesc = 0;
extern "C" u8 *sAossWcmBssDesc = 0;
extern "C" u8 sAossWcmBssid[6] = {0};
extern "C" s32 sAossWcmState = 0;
extern "C" u8 sAossWcmSsid[0x20] = {0};
extern "C" u8 *sAossWcmBssidPtr = 0;
extern "C" s32 sAossWcmConnectOption = 0;
extern "C" AossWcmNotifyFunc sAossWcmNotifyCb = 0;
extern "C" u8 *sAossWcmSsidPtr = 0;
extern "C" WifiLinkConfig *sAossWcmConfig = 0;
extern "C" s32 sAossWcmSearchOption = 0;

extern "C" {
u32 OS_DisableInterrupts(void);
void OS_RestoreInterrupts(u32 v);
void MI_CpuFill8(void *p, u32 v, u32 n);
void MIi_CpuCopy32(void *src, void *dst, u32 n);
void MI_CpuCopy8(void *a, void *b, u32 n);

s32 WifiLink_CleanupAsync(void);
s32 WifiLink_DisconnectAsync(void);
s32 WifiLink_SearchAsync(void *a, void *b, s32 c);
s32 WifiLink_StartupAsync(WifiLinkConfig *a, WifiLinkNotifyCallback b);
s32 WifiLink_ConnectAsync(u8 *a, u8 *b, u32 c);
s32 WifiLink_Init(void *, s32);
void WifiLink_Finish(void);
void WifiLink_LockApList(s32 a);
s32 WifiLink_GetApListCount(void);
void *WifiLink_GetApListEntry(u32 a);
extern u8 gWifiLinkAnyBssid[];
extern u8 gWifiLinkAnySsid[];

}

extern "C" void Aoss_WcmCallback(WifiLinkEvent *p) {
    if (p == NULL) {
        return;
    }
    switch ((u32)p->request) {
    case 1:
        if (p->result == 0) {
            if (sAossWcmState == 4) {
                sAossWcmState = 3;
                if (sAossWcmNotifyCb != NULL) {
                    sAossWcmNotifyCb(6, 0);
                }
            } else if (sAossWcmState == 6) {
                if (WifiLink_SearchAsync(sAossWcmBssidPtr, sAossWcmSsidPtr, sAossWcmSearchOption) == 3) {
                    return;
                }
                sAossWcmState = 3;
                if (sAossWcmNotifyCb != NULL) {
                    sAossWcmNotifyCb(2, 0);
                }
            } else if (sAossWcmState == 8) {
                if (WifiLink_ConnectAsync(sAossWcmBssDesc, sAossWcmWepDesc, sAossWcmConnectOption) == 3) {
                    return;
                }
                sAossWcmState = 3;
                if (sAossWcmNotifyCb != NULL) {
                    sAossWcmNotifyCb(2, 0);
                }
            }
        } else {
            sAossWcmState = 1;
            if (sAossWcmNotifyCb != NULL) {
                sAossWcmNotifyCb(2, 0);
            }
        }
        break;
    case 3:
        if (p->result == 0) {
            if (sAossWcmState == 6) {
                sAossWcmState = 5;
                if (sAossWcmNotifyCb != NULL) {
                    sAossWcmNotifyCb(8, 0);
                }
            }
        } else {
            sAossWcmState = 3;
            if (sAossWcmNotifyCb != NULL) {
                sAossWcmNotifyCb(9, 0);
            }
        }
        break;
    case 5:
        if (p->result == 0) {
            if (sAossWcmState == 8) {
                sAossWcmState = 7;
                if (sAossWcmNotifyCb != NULL) {
                    sAossWcmNotifyCb(0xc, 0);
                }
            }
        } else {
            sAossWcmState = 3;
            if (sAossWcmNotifyCb != NULL) {
                sAossWcmNotifyCb(0xd, 0);
            }
        }
        break;
    case 4:
        if (p->result == 0) {
            if (sAossWcmState == 4) {
                sAossWcmState = 3;
                if (sAossWcmNotifyCb != NULL) {
                    sAossWcmNotifyCb(0xa, 0);
                }
            } else if (sAossWcmState == 6) {
                if (WifiLink_SearchAsync(sAossWcmBssidPtr, sAossWcmSsidPtr, sAossWcmSearchOption) == 3) {
                    return;
                }
                sAossWcmState = 3;
                if (sAossWcmNotifyCb != NULL) {
                    sAossWcmNotifyCb(2, 0);
                }
            } else if (sAossWcmState == 2) {
                if (WifiLink_CleanupAsync() == 3) {
                    return;
                }
                sAossWcmState = 3;
                if (sAossWcmNotifyCb != NULL) {
                    sAossWcmNotifyCb(2, 0);
                }
            } else if (sAossWcmState == 8) {
                if (WifiLink_ConnectAsync(sAossWcmBssDesc, sAossWcmWepDesc, sAossWcmConnectOption) == 3) {
                    return;
                }
                sAossWcmState = 3;
                if (sAossWcmNotifyCb != NULL) {
                    sAossWcmNotifyCb(2, 0);
                }
            }
        } else {
            sAossWcmState = 3;
            if (sAossWcmNotifyCb != NULL) {
                sAossWcmNotifyCb(0xb, 0);
            }
        }
        break;
    case 6:
        if (p->result == 0) {
            if (sAossWcmState == 4) {
                sAossWcmState = 3;
                if (sAossWcmNotifyCb != NULL) {
                    sAossWcmNotifyCb(0xe, 0);
                }
            } else if (sAossWcmState == 6) {
                if (WifiLink_SearchAsync(sAossWcmBssidPtr, sAossWcmSsidPtr, sAossWcmSearchOption) == 3) {
                    return;
                }
                sAossWcmState = 3;
                if (sAossWcmNotifyCb != NULL) {
                    sAossWcmNotifyCb(2, 0);
                }
            } else if (sAossWcmState == 2) {
                if (WifiLink_CleanupAsync() == 3) {
                    return;
                }
                sAossWcmState = 3;
                if (sAossWcmNotifyCb != NULL) {
                    sAossWcmNotifyCb(2, 0);
                }
            } else if (sAossWcmState == 8) {
                if (WifiLink_ConnectAsync(sAossWcmBssDesc, sAossWcmWepDesc, sAossWcmConnectOption) == 3) {
                    return;
                }
                sAossWcmState = 3;
                if (sAossWcmNotifyCb != NULL) {
                    sAossWcmNotifyCb(2, 0);
                }
            } else {
                sAossWcmState = 3;
            }
        } else {
            sAossWcmState = 3;
            if (sAossWcmNotifyCb != NULL) {
                sAossWcmNotifyCb(0xf, 0);
            }
        }
        break;
    case 2:
        if (p->result == 0) {
            if (sAossWcmState == 2) {
                WifiLink_Finish();
                sAossWcmState = 0;
                if (sAossWcmNotifyCb != NULL) {
                    sAossWcmNotifyCb(0x14, 0);
                }
            }
        } else {
            sAossWcmState = 3;
            if (sAossWcmNotifyCb != NULL) {
                sAossWcmNotifyCb(2, 0);
            }
        }
        break;
    case 7:
        if (sAossWcmState == 5) {
            if (sAossWcmNotifyCb != NULL) {
                sAossWcmNotifyCb(5, 0);
            }
        }
        break;
    default:
        if (sAossWcmNotifyCb != NULL) {
            sAossWcmNotifyCb(1, 0);
        }
        break;
    }
}

extern "C" s32 Aoss_WcmToIdle(void) {
    switch (sAossWcmState) {
    case 5:
        if (WifiLink_SearchAsync(0, 0, 0) == 3) {
            goto ok;
        }
        return FALSE;
    case 7:
        if (WifiLink_DisconnectAsync() == 3) {
            goto ok;
        }
        return FALSE;
    case 1:
        if (WifiLink_StartupAsync(sAossWcmConfig, Aoss_WcmCallback) == 3) {
            goto ok;
        }
        return FALSE;
    case 0:
    case 2:
    case 3:
    case 4:
    case 6:
    case 8:
        break;
    }
    return FALSE;
ok:
    return TRUE;
}

extern "C" s32 Aoss_WcmReadApList(u8 *dst, s32 max) {
    s32 cnt;
    s32 i;
    WifiLink_LockApList(1);
    cnt = WifiLink_GetApListCount();
    if (cnt > 0) {
        for (i = 0; i < cnt; i++, dst += 0xc0) {
            if (i >= max) {
                break;
            }
            MIi_CpuCopy32(WifiLink_GetApListEntry((u16)i), dst, 0xc0);
        }
    }
    WifiLink_LockApList(0);
    return cnt;
}

extern "C" s32 Aoss_WcmStartSearch(u8 *a, u8 *b, s32 n, s32 d) {
    u32 irq = OS_DisableInterrupts();
    s32 i;
    sAossWcmSearchOption = d;
    if (a != NULL) {
        u8 *dst;
        for (i = 0, dst = sAossWcmBssid; i < 6; i++) {
            *dst++ = *a++;
        }
        sAossWcmBssidPtr = sAossWcmBssid;
    } else {
        MI_CpuFill8(sAossWcmBssid, 0xff, 6);
        sAossWcmBssidPtr = gWifiLinkAnyBssid;
    }
    if (b != NULL && n > 0 && n <= 0x20) {
        i = 0;
        if (i < n) {
            u8 *dst = sAossWcmSsid;
            do {
                *dst++ = *b++;
                i++;
            } while (i < n);
        }
        if (i < 0x20) {
            u8 *z = sAossWcmSsid + i;
            do {
                *z++ = 0;
                i++;
            } while (i < 0x20);
        }
        sAossWcmSsidPtr = sAossWcmSsid;
    } else {
        MI_CpuFill8(sAossWcmSsid, 0xff, 0x20);
        sAossWcmSsidPtr = gWifiLinkAnySsid;
    }
    if (sAossWcmState == 3) {
        if (WifiLink_SearchAsync(sAossWcmBssidPtr, sAossWcmSsidPtr, sAossWcmSearchOption) != 3) {
            goto fail;
        }
        sAossWcmState = 6;
        OS_RestoreInterrupts(irq);
        return TRUE;
    } else {
        if (Aoss_WcmToIdle() != 1) {
            goto fail;
        }
        sAossWcmState = 6;
        OS_RestoreInterrupts(irq);
        return TRUE;
    }
fail:
    OS_RestoreInterrupts(irq);
    return FALSE;
}

extern "C" s32 Aoss_WcmEndSearch(void) {
    u32 irq = OS_DisableInterrupts();
    if (sAossWcmState == 5) {
        if (WifiLink_SearchAsync(0, 0, 0) == 3) {
            sAossWcmState = 4;
            OS_RestoreInterrupts(irq);
            return TRUE;
        }
    }
    OS_RestoreInterrupts(irq);
    return FALSE;
}

extern "C" s32 Aoss_WcmDisconnect(void) {
    u32 irq = OS_DisableInterrupts();
    if (sAossWcmState == 7) {
        if (WifiLink_DisconnectAsync() == 3) {
            sAossWcmState = 4;
            OS_RestoreInterrupts(irq);
            return TRUE;
        }
    }
    OS_RestoreInterrupts(irq);
    return FALSE;
}

extern "C" s32 Aoss_WcmCleanup(void) {
    u32 irq = OS_DisableInterrupts();
    if (sAossWcmState == 3) {
        if (WifiLink_CleanupAsync() != 3) {
            OS_RestoreInterrupts(irq);
            return FALSE;
        }
        sAossWcmState = 2;
        OS_RestoreInterrupts(irq);
        return TRUE;
    }
    if (Aoss_WcmToIdle() == 1) {
        sAossWcmState = 2;
        OS_RestoreInterrupts(irq);
        return TRUE;
    }
    OS_RestoreInterrupts(irq);
    return FALSE;
}

extern "C" s32 Aoss_WcmConnect(void *a, void *b, u32 c) {
    u32 irq = OS_DisableInterrupts();
    sAossWcmConnectOption = c;
    if (b) {
        MI_CpuCopy8(b, sAossWcmWepDesc, 0x50);
    } else {
        MI_CpuFill8(sAossWcmWepDesc, 0, 0x50);
    }
    MIi_CpuCopy32(a, sAossWcmBssDesc, 0xc0);
    if (Aoss_WcmToIdle() == 1) {
        sAossWcmState = 8;
        OS_RestoreInterrupts(irq);
        return 1;
    }
    if (sAossWcmState == 3) {
        if (WifiLink_ConnectAsync(sAossWcmBssDesc, sAossWcmWepDesc, sAossWcmConnectOption) == 3) {
            sAossWcmState = 8;
            OS_RestoreInterrupts(irq);
            return 1;
        }
    }
    OS_RestoreInterrupts(irq);
    return 0;
}

extern "C" s32 Aoss_WcmInit(void *fn, void *buf, u32 size) {
    u32 irq = OS_DisableInterrupts();
    // buf: the WEP descriptor (0x50), the WCM config, the WifiLink work (0x2300), a BSS descriptor (0xc0), then the
    // BSS descriptor buffer, each aligned
    sAossWcmWepDesc = (u8 *)buf;
    WifiLinkConfig *p = (WifiLinkConfig *)(((uptr)buf + 0x53) & ~(uptr)3);
    sAossWcmConfig = p;
    uptr t = ((uptr)p + 0x2f) & ~(uptr)0x1f;
    sAossWcmWork = (u8 *)t;
    t = (t + WIFI_LINK_WORK_SIZE + 0x1f) & ~(uptr)0x1f;
    sAossWcmBssDesc = (u8 *)t;
    p->bssDescBuffer = (void *)((t + 0xdf) & ~(uptr)0x1f);
    sAossWcmConfig->bssDescBufferSize = (s32)((u8 *)buf + size - (u8 *)sAossWcmConfig->bssDescBuffer);
    sAossWcmConfig->bssDescMode = 0;
    sAossWcmConfig->dmaNo = 3;
    sAossWcmNotifyCb = (AossWcmNotifyFunc)fn;
    if (sAossWcmState == 0) {
        if (WifiLink_Init(sAossWcmWork, WIFI_LINK_WORK_SIZE)) {
            OS_RestoreInterrupts(irq);
            return 0;
        }
        sAossWcmState = 1;
    }
    if (sAossWcmState == 1) {
        if (WifiLink_StartupAsync(sAossWcmConfig, Aoss_WcmCallback) != 3) {
            OS_RestoreInterrupts(irq);
            return 0;
        }
        sAossWcmState = 4;
        OS_RestoreInterrupts(irq);
        return 1;
    }
    OS_RestoreInterrupts(irq);
    return 0;
}

// ---- overlay variables (generated: tools/host_overlay_vars.py) ----
#include "host_overlay_vars.h"

HOST_OVERLAY_VARS(unk_ov001_022032f8) {
    HOST_OVERLAY_VAR(sAossWcmWork),
    HOST_OVERLAY_VAR(sAossWcmWepDesc),
    HOST_OVERLAY_VAR(sAossWcmBssDesc),
    HOST_OVERLAY_VAR(sAossWcmBssid),
    HOST_OVERLAY_VAR(sAossWcmState),
    HOST_OVERLAY_VAR(sAossWcmSsid),
    HOST_OVERLAY_VAR(sAossWcmBssidPtr),
    HOST_OVERLAY_VAR(sAossWcmConnectOption),
    HOST_OVERLAY_VAR(sAossWcmNotifyCb),
    HOST_OVERLAY_VAR(sAossWcmSsidPtr),
    HOST_OVERLAY_VAR(sAossWcmConfig),
    HOST_OVERLAY_VAR(sAossWcmSearchOption),
    HOST_OVERLAY_VARS_END
};
