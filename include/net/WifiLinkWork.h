#ifndef NET_WIFILINKWORK_H
#define NET_WIFILINKWORK_H

#include "types.h"
#include "nitro/wm.h"
#include "nitro/os_alarm.h"
#include "net/WifiLinkEvent.h"

// WifiLink work area (sWifiLinkWork, 0x2300 bytes given to WifiLink_Init; WifiLink_GetWork) and the found-AP list
// that lives in the buffer WifiLink_ApplyConfig is given (src/ov065/unk_ov065_02268c64.cpp; also read by
// unk_ov065_0226ad84.cpp and unk_ov065_0226b3c4.cpp).

struct WifiApListEntry {
    /* 0x00 */ u8 inUse;
    /* 0x01 */ u8 unk_01;
    /* 0x02 */ u16 linkLevel;
    /* 0x04 */ u32 id;
    /* 0x08 */ WifiApListEntry *prev;
    /* 0x0c */ WifiApListEntry *next;
    /* 0x10 */ u8 bssDesc[0xc0];
};

struct WifiApList {
    /* 0x00 */ u32 count;
    /* 0x04 */ WifiApListEntry *head;
    /* 0x08 */ WifiApListEntry *tail;
    /* 0x0c */ WifiApListEntry nodes[1];
};

struct WifiLinkWork {
    /* 0x0000 */ u8 unk_0000[0xf00];
    /* 0x0f00 */ u8 sendBuf[0x600]; // WM_SetDCFData frames (WifiLink_SendFrame copies the data here)
    /* 0x1500 */ u8 wmBuf[0xc40]; // WM_StartDCF receive buffer; also the scan result buffer (scanParam.scanBuf)
    /* 0x2140 */ WMBssDesc targetBss; // AP to connect to (WM_StartConnectEx)
    /* 0x2200 */ u8 wepKeys[0x50];
    /* 0x2250 */ u8 wepMode;
    /* 0x2251 */ u8 wepKeyId;
    /* 0x2252 */ u8 unk_2252[0xe];
    /* 0x2260 */ s32 phase;
    /* 0x2264 */ u32 options;
    /* 0x2268 */ u16 scanPeriod;
    /* 0x226a */ u8 isApListLocked;
    /* 0x226b */ u8 isResetting;
    /* 0x226c */ u32 dmaNo;
    /* 0x2270 */ WifiApList *apList;
    /* 0x2274 */ u32 apListSize;
    /* 0x2278 */ s32 apListReplaceOldest;
    /* 0x227c */ WifiLinkNotifyCallback notifyCallback;
    /* 0x2280 */ s16 pendingRequest;
    /* 0x2282 */ u16 aid;
    /* 0x2284 */ u32 scanCount;
    /* 0x2288 */ WMScanExParam scanParam;
    /* 0x22cc */ OSAlarm keepAliveAlarm;
    /* 0x22f8 */ u16 connectFailStatus;
    /* 0x22fa */ u8 unk_22fa[6];
};

// the work's size as given to WifiLink_Init: 0x2300 on the DS; on 64-bit hosts the struct is larger (pointers, the
// alarm), so the struct's own size, in whole 32-byte units
#ifdef HOST64
#define WIFI_LINK_WORK_SIZE ((sizeof(WifiLinkWork) + 0x1f) & ~(u32)0x1f)
#else
#define WIFI_LINK_WORK_SIZE 0x2300
#endif

#endif
