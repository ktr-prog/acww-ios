// mwcc-flags: -O4,p
#pragma opt_dead_assignments off
#include "types.h"
#include "host_shared_calls.h"
#include "net/WifiLinkEvent.h"
#include "net/WifiLinkConfig.h"
#include "net/WifiLinkWork.h"
#include "nitro/os_alarm.h"
#include "uptr.h"
#include "net/SimpleStart.h"
#include "net/SockAddrIn.h"
#include "net/SockStartupConfig.h"
#include "nitro/wm.h"
#include "sys/OSThread.h"
#include "proto/autoload_2/unk_02114410.h"
#include "proto/autoload_2/unk_02114ef4.h"
#include "proto/autoload_2/unk_02128030.h"
#include "proto/autoload_2/unk_0212a120.h"
#include "proto/itcm/unk_01ffa4a0.h"

struct SimpleStartMd5Ctx {
    u32 state[4];
    u32 count[2];
    u8 buffer[64];
};
struct SimpleStartApEntry {
    u32 len;
    u8 name[0x20];
    u8 unk_24[4];
    u8 bssid[6];
    u16 privacy;
};
struct SimpleStartApList {
    u32 count;
    SimpleStartApEntry e[1];
};
struct SimpleStartSessionId {
    u8 higherMac[6];
    u8 lowerMac[6];
    u8 tag[4];
};
struct SimpleStartWep40Key { u8 v[5]; };
struct SimpleStartWep104Key { u8 v[13]; };
struct SimpleStartWep128Key { u8 v[16]; };
struct SimpleStartPsk { s64 v[8]; };
struct SimpleStartResult { u8 pad[0x20]; s32 securityType; s32 unk_24; };
struct SimpleStartConfigView { u8 pad[0x2c]; s32 securityType; s32 unk_30; u8 pad2[0x28]; s32 unk_5c; };
struct SimpleStartExchangeKey { u8 v[8]; };

#include "proto/ov001/unk_ov001_02203b1c.h"

extern "C" {
// (NitroSDK, src/autoload_2 / itcm C units without generated prototypes)
u32 OS_DisableInterrupts(void);
void OS_RestoreInterrupts(u32 v);
void MI_CpuFill8(void *dst, u8 v, u32 size);
void MIi_CpuCopy32(const void *src, void *dst, u32 size);
void MI_CpuCopy8(const void *src, void *dst, u32 size);
void OS_Sleep(u32 msec);
BOOL OS_IsThreadTerminated(OSThread *t);
void OS_WakeupThreadDirect(OSThread *t);
void OS_JoinThread(OSThread *t);
u32 OS_IsThreadAvailable(void);
void OS_CreateThread(OSThread *thread, void (*func)(void *), void *arg, void *stack, u32 stackSize, u32 prio);
void OS_GetMacAddress(u8 *macAddr);
u32 strlen(const char *s);

// (src/ov065/unk_ov065_02268c64.cpp, unk_ov065_0225fdf0.cpp: their generated headers leave these out)
s32 WifiLink_CleanupAsync(void);
s32 WifiLink_DisconnectAsync(void);
s32 WifiLink_SearchAsync(void *a, void *b, s32 c);
s32 WifiLink_StartupAsync(WifiLinkConfig *a, WifiLinkNotifyCallback b);
s32 WifiLink_ConnectAsync(u8 *a, u8 *b, u32 c);
s32 WifiLink_Finish(void);
BOOL WifiLink_LockApList(BOOL a);
u32 WifiLink_GetApListCount(void);
void *WifiLink_GetApListEntry(u32 id);
s32 WifiLink_Init(void *a, u32 b);
s32 Sock_Cleanup();
s32 Sock_Startup(SockStartupConfig *s);
s32 SockCore_GetHostIp();
void IpAddr_StoreBe32(u32 v, u8 *p);
s32 Sock_SendTo(sptr a, const void *b, s32 c, u32 d, SockAddrIn *sa);
sptr Sock_Create(s32 a, s32 b);
s32 Sock_Bind(sptr a, SockAddrIn *sa);
s32 Sock_Close(sptr a);
s32 Sock_RecvFrom(sptr a, void *b, s32 c, u32 d, SockAddrIn *sa);

extern const u32 sSimpleStartAesRcon[10];
extern u8 gWifiLinkAnyBssid[];
extern u8 gWifiLinkAnySsid[];
extern const u32 data_ov001_02227334[256];
extern const u32 data_ov001_02227734[256];
extern const u32 data_ov001_02227b34[256];
extern const u32 data_ov001_02227f34[256];
extern const u32 data_ov001_02228334[256];
extern const u32 data_ov001_02228734[256];
extern const u32 data_ov001_02228b34[256];
extern const u32 data_ov001_02228f34[256];
extern const u32 data_ov001_02229334[256];
extern const u32 data_ov001_02229734[256];
extern WifiLinkConfig *data_ov001_0222c858;
}
#define data_ov001_0222c92c (data_ov001_0222c924 + 8)
#define data_ov001_0222ca70 ((u8 *)&sSimpleStartResult[10])
#define data_ov001_0222caf0 (*(SimpleStartPsk *)&sSimpleStartResult[42])
#define data_ov001_0222cc30 (*(SimpleStartConfigView *)&data_ov001_0222cb30[64])
#define data_ov001_0222cc94 ((u8 *)&data_ov001_0222cb30[89])
#define data_ov001_0222cd2c (*(SimpleStartPsk *)&data_ov001_0222cb30[127])
#define CA48_A (*(SimpleStartResult *)sSimpleStartResult)
#define C924_B8 (*(SimpleStartExchangeKey *)data_ov001_0222c924)

#define F(x, y, z) (((x) & (y)) | ((~(x)) & (z)))
#define G(x, y, z) (((x) & (z)) | ((y) & (~(z))))
#define H(x, y, z) ((x) ^ (y) ^ (z))
#define I(x, y, z) ((y) ^ ((x) | (~(z))))
#define ROL(x, n) (((x) << (n)) | ((x) >> (32 - (n))))
#define GETU32(p) (((u32)(p)[0] << 24) ^ ((u32)(p)[1] << 16) ^ ((u32)(p)[2] << 8) ^ ((u32)(p)[3]))
#define Te4 data_ov001_02228f34
#define Td0 data_ov001_02229334
#define Td1 data_ov001_02229734
#define Td2 data_ov001_02227334
#define Td3 data_ov001_02227734
#define rcon sSimpleStartAesRcon
#define Bswap(v) ((u16)((((u16)(v) >> 8) & 0xff) | (((u16)(v) << 8) & 0xff00)))
#define CP4(d, s) { u8 *d_ = (u8 *)(d); const u8 *s_ = (const u8 *)(s); d_[0] = s_[0]; d_[1] = s_[1]; d_[2] = s_[2]; d_[3] = s_[3]; }
#define CP6(d, s) { u8 *d_ = (u8 *)(d); const u8 *s_ = (const u8 *)(s); d_[0] = s_[0]; d_[1] = s_[1]; d_[2] = s_[2]; d_[3] = s_[3]; d_[4] = s_[4]; d_[5] = s_[5]; }
#define PUTU32(p, v) { (p)[0] = (u8)((v) >> 24); (p)[1] = (u8)((v) >> 16); (p)[2] = (u8)((v) >> 8); (p)[3] = (u8)(v); }
#define STEP(f, a, b, c, d, x, s, ac) { (a) += f(b, c, d) + (x) + (ac); (a) = ROL(a, s); (a) += (b); }

extern "C" const u32 sSimpleStartAesRcon[10] = {0x1000000, 0x2000000, 0x4000000, 0x8000000, 0x10000000, 0x20000000, 0x40000000, 0x80000000, 0x1b000000, 0x36000000};
extern "C" const u32 data_ov001_02228b34[256] = {
    0x6363a5c6, 0x7c7c84f8, 0x777799ee, 0x7b7b8df6, 0xf2f20dff, 0x6b6bbdd6,
    0x6f6fb1de, 0xc5c55491, 0x30305060, 0x01010302, 0x6767a9ce, 0x2b2b7d56,
    0xfefe19e7, 0xd7d762b5, 0xababe64d, 0x76769aec, 0xcaca458f, 0x82829d1f,
    0xc9c94089, 0x7d7d87fa, 0xfafa15ef, 0x5959ebb2, 0x4747c98e, 0xf0f00bfb,
    0xadadec41, 0xd4d467b3, 0xa2a2fd5f, 0xafafea45, 0x9c9cbf23, 0xa4a4f753,
    0x727296e4, 0xc0c05b9b, 0xb7b7c275, 0xfdfd1ce1, 0x9393ae3d, 0x26266a4c,
    0x36365a6c, 0x3f3f417e, 0xf7f702f5, 0xcccc4f83, 0x34345c68, 0xa5a5f451,
    0xe5e534d1, 0xf1f108f9, 0x717193e2, 0xd8d873ab, 0x31315362, 0x15153f2a,
    0x04040c08, 0xc7c75295, 0x23236546, 0xc3c35e9d, 0x18182830, 0x9696a137,
    0x05050f0a, 0x9a9ab52f, 0x0707090e, 0x12123624, 0x80809b1b, 0xe2e23ddf,
    0xebeb26cd, 0x2727694e, 0xb2b2cd7f, 0x75759fea, 0x09091b12, 0x83839e1d,
    0x2c2c7458, 0x1a1a2e34, 0x1b1b2d36, 0x6e6eb2dc, 0x5a5aeeb4, 0xa0a0fb5b,
    0x5252f6a4, 0x3b3b4d76, 0xd6d661b7, 0xb3b3ce7d, 0x29297b52, 0xe3e33edd,
    0x2f2f715e, 0x84849713, 0x5353f5a6, 0xd1d168b9, 0x00000000, 0xeded2cc1,
    0x20206040, 0xfcfc1fe3, 0xb1b1c879, 0x5b5bedb6, 0x6a6abed4, 0xcbcb468d,
    0xbebed967, 0x39394b72, 0x4a4ade94, 0x4c4cd498, 0x5858e8b0, 0xcfcf4a85,
    0xd0d06bbb, 0xefef2ac5, 0xaaaae54f, 0xfbfb16ed, 0x4343c586, 0x4d4dd79a,
    0x33335566, 0x85859411, 0x4545cf8a, 0xf9f910e9, 0x02020604, 0x7f7f81fe,
    0x5050f0a0, 0x3c3c4478, 0x9f9fba25, 0xa8a8e34b, 0x5151f3a2, 0xa3a3fe5d,
    0x4040c080, 0x8f8f8a05, 0x9292ad3f, 0x9d9dbc21, 0x38384870, 0xf5f504f1,
    0xbcbcdf63, 0xb6b6c177, 0xdada75af, 0x21216342, 0x10103020, 0xffff1ae5,
    0xf3f30efd, 0xd2d26dbf, 0xcdcd4c81, 0x0c0c1418, 0x13133526, 0xecec2fc3,
    0x5f5fe1be, 0x9797a235, 0x4444cc88, 0x1717392e, 0xc4c45793, 0xa7a7f255,
    0x7e7e82fc, 0x3d3d477a, 0x6464acc8, 0x5d5de7ba, 0x19192b32, 0x737395e6,
    0x6060a0c0, 0x81819819, 0x4f4fd19e, 0xdcdc7fa3, 0x22226644, 0x2a2a7e54,
    0x9090ab3b, 0x8888830b, 0x4646ca8c, 0xeeee29c7, 0xb8b8d36b, 0x14143c28,
    0xdede79a7, 0x5e5ee2bc, 0x0b0b1d16, 0xdbdb76ad, 0xe0e03bdb, 0x32325664,
    0x3a3a4e74, 0x0a0a1e14, 0x4949db92, 0x06060a0c, 0x24246c48, 0x5c5ce4b8,
    0xc2c25d9f, 0xd3d36ebd, 0xacacef43, 0x6262a6c4, 0x9191a839, 0x9595a431,
    0xe4e437d3, 0x79798bf2, 0xe7e732d5, 0xc8c8438b, 0x3737596e, 0x6d6db7da,
    0x8d8d8c01, 0xd5d564b1, 0x4e4ed29c, 0xa9a9e049, 0x6c6cb4d8, 0x5656faac,
    0xf4f407f3, 0xeaea25cf, 0x6565afca, 0x7a7a8ef4, 0xaeaee947, 0x08081810,
    0xbabad56f, 0x787888f0, 0x25256f4a, 0x2e2e725c, 0x1c1c2438, 0xa6a6f157,
    0xb4b4c773, 0xc6c65197, 0xe8e823cb, 0xdddd7ca1, 0x74749ce8, 0x1f1f213e,
    0x4b4bdd96, 0xbdbddc61, 0x8b8b860d, 0x8a8a850f, 0x707090e0, 0x3e3e427c,
    0xb5b5c471, 0x6666aacc, 0x4848d890, 0x03030506, 0xf6f601f7, 0x0e0e121c,
    0x6161a3c2, 0x35355f6a, 0x5757f9ae, 0xb9b9d069, 0x86869117, 0xc1c15899,
    0x1d1d273a, 0x9e9eb927, 0xe1e138d9, 0xf8f813eb, 0x9898b32b, 0x11113322,
    0x6969bbd2, 0xd9d970a9, 0x8e8e8907, 0x9494a733, 0x9b9bb62d, 0x1e1e223c,
    0x87879215, 0xe9e920c9, 0xcece4987, 0x5555ffaa, 0x28287850, 0xdfdf7aa5,
    0x8c8c8f03, 0xa1a1f859, 0x89898009, 0x0d0d171a, 0xbfbfda65, 0xe6e631d7,
    0x4242c684, 0x6868b8d0, 0x4141c382, 0x9999b029, 0x2d2d775a, 0x0f0f111e,
    0xb0b0cb7b, 0x5454fca8, 0xbbbbd66d, 0x16163a2c,
};
extern "C" const u32 data_ov001_02228f34[256] = {
    0x63636363, 0x7c7c7c7c, 0x77777777, 0x7b7b7b7b, 0xf2f2f2f2, 0x6b6b6b6b,
    0x6f6f6f6f, 0xc5c5c5c5, 0x30303030, 0x01010101, 0x67676767, 0x2b2b2b2b,
    0xfefefefe, 0xd7d7d7d7, 0xabababab, 0x76767676, 0xcacacaca, 0x82828282,
    0xc9c9c9c9, 0x7d7d7d7d, 0xfafafafa, 0x59595959, 0x47474747, 0xf0f0f0f0,
    0xadadadad, 0xd4d4d4d4, 0xa2a2a2a2, 0xafafafaf, 0x9c9c9c9c, 0xa4a4a4a4,
    0x72727272, 0xc0c0c0c0, 0xb7b7b7b7, 0xfdfdfdfd, 0x93939393, 0x26262626,
    0x36363636, 0x3f3f3f3f, 0xf7f7f7f7, 0xcccccccc, 0x34343434, 0xa5a5a5a5,
    0xe5e5e5e5, 0xf1f1f1f1, 0x71717171, 0xd8d8d8d8, 0x31313131, 0x15151515,
    0x04040404, 0xc7c7c7c7, 0x23232323, 0xc3c3c3c3, 0x18181818, 0x96969696,
    0x05050505, 0x9a9a9a9a, 0x07070707, 0x12121212, 0x80808080, 0xe2e2e2e2,
    0xebebebeb, 0x27272727, 0xb2b2b2b2, 0x75757575, 0x09090909, 0x83838383,
    0x2c2c2c2c, 0x1a1a1a1a, 0x1b1b1b1b, 0x6e6e6e6e, 0x5a5a5a5a, 0xa0a0a0a0,
    0x52525252, 0x3b3b3b3b, 0xd6d6d6d6, 0xb3b3b3b3, 0x29292929, 0xe3e3e3e3,
    0x2f2f2f2f, 0x84848484, 0x53535353, 0xd1d1d1d1, 0x00000000, 0xedededed,
    0x20202020, 0xfcfcfcfc, 0xb1b1b1b1, 0x5b5b5b5b, 0x6a6a6a6a, 0xcbcbcbcb,
    0xbebebebe, 0x39393939, 0x4a4a4a4a, 0x4c4c4c4c, 0x58585858, 0xcfcfcfcf,
    0xd0d0d0d0, 0xefefefef, 0xaaaaaaaa, 0xfbfbfbfb, 0x43434343, 0x4d4d4d4d,
    0x33333333, 0x85858585, 0x45454545, 0xf9f9f9f9, 0x02020202, 0x7f7f7f7f,
    0x50505050, 0x3c3c3c3c, 0x9f9f9f9f, 0xa8a8a8a8, 0x51515151, 0xa3a3a3a3,
    0x40404040, 0x8f8f8f8f, 0x92929292, 0x9d9d9d9d, 0x38383838, 0xf5f5f5f5,
    0xbcbcbcbc, 0xb6b6b6b6, 0xdadadada, 0x21212121, 0x10101010, 0xffffffff,
    0xf3f3f3f3, 0xd2d2d2d2, 0xcdcdcdcd, 0x0c0c0c0c, 0x13131313, 0xecececec,
    0x5f5f5f5f, 0x97979797, 0x44444444, 0x17171717, 0xc4c4c4c4, 0xa7a7a7a7,
    0x7e7e7e7e, 0x3d3d3d3d, 0x64646464, 0x5d5d5d5d, 0x19191919, 0x73737373,
    0x60606060, 0x81818181, 0x4f4f4f4f, 0xdcdcdcdc, 0x22222222, 0x2a2a2a2a,
    0x90909090, 0x88888888, 0x46464646, 0xeeeeeeee, 0xb8b8b8b8, 0x14141414,
    0xdededede, 0x5e5e5e5e, 0x0b0b0b0b, 0xdbdbdbdb, 0xe0e0e0e0, 0x32323232,
    0x3a3a3a3a, 0x0a0a0a0a, 0x49494949, 0x06060606, 0x24242424, 0x5c5c5c5c,
    0xc2c2c2c2, 0xd3d3d3d3, 0xacacacac, 0x62626262, 0x91919191, 0x95959595,
    0xe4e4e4e4, 0x79797979, 0xe7e7e7e7, 0xc8c8c8c8, 0x37373737, 0x6d6d6d6d,
    0x8d8d8d8d, 0xd5d5d5d5, 0x4e4e4e4e, 0xa9a9a9a9, 0x6c6c6c6c, 0x56565656,
    0xf4f4f4f4, 0xeaeaeaea, 0x65656565, 0x7a7a7a7a, 0xaeaeaeae, 0x08080808,
    0xbabababa, 0x78787878, 0x25252525, 0x2e2e2e2e, 0x1c1c1c1c, 0xa6a6a6a6,
    0xb4b4b4b4, 0xc6c6c6c6, 0xe8e8e8e8, 0xdddddddd, 0x74747474, 0x1f1f1f1f,
    0x4b4b4b4b, 0xbdbdbdbd, 0x8b8b8b8b, 0x8a8a8a8a, 0x70707070, 0x3e3e3e3e,
    0xb5b5b5b5, 0x66666666, 0x48484848, 0x03030303, 0xf6f6f6f6, 0x0e0e0e0e,
    0x61616161, 0x35353535, 0x57575757, 0xb9b9b9b9, 0x86868686, 0xc1c1c1c1,
    0x1d1d1d1d, 0x9e9e9e9e, 0xe1e1e1e1, 0xf8f8f8f8, 0x98989898, 0x11111111,
    0x69696969, 0xd9d9d9d9, 0x8e8e8e8e, 0x94949494, 0x9b9b9b9b, 0x1e1e1e1e,
    0x87878787, 0xe9e9e9e9, 0xcececece, 0x55555555, 0x28282828, 0xdfdfdfdf,
    0x8c8c8c8c, 0xa1a1a1a1, 0x89898989, 0x0d0d0d0d, 0xbfbfbfbf, 0xe6e6e6e6,
    0x42424242, 0x68686868, 0x41414141, 0x99999999, 0x2d2d2d2d, 0x0f0f0f0f,
    0xb0b0b0b0, 0x54545454, 0xbbbbbbbb, 0x16161616,
};
extern "C" const u32 data_ov001_02229334[256] = {
    0x51f4a750, 0x7e416553, 0x1a17a4c3, 0x3a275e96, 0x3bab6bcb, 0x1f9d45f1,
    0xacfa58ab, 0x4be30393, 0x2030fa55, 0xad766df6, 0x88cc7691, 0xf5024c25,
    0x4fe5d7fc, 0xc52acbd7, 0x26354480, 0xb562a38f, 0xdeb15a49, 0x25ba1b67,
    0x45ea0e98, 0x5dfec0e1, 0xc32f7502, 0x814cf012, 0x8d4697a3, 0x6bd3f9c6,
    0x038f5fe7, 0x15929c95, 0xbf6d7aeb, 0x955259da, 0xd4be832d, 0x587421d3,
    0x49e06929, 0x8ec9c844, 0x75c2896a, 0xf48e7978, 0x99583e6b, 0x27b971dd,
    0xbee14fb6, 0xf088ad17, 0xc920ac66, 0x7dce3ab4, 0x63df4a18, 0xe51a3182,
    0x97513360, 0x62537f45, 0xb16477e0, 0xbb6bae84, 0xfe81a01c, 0xf9082b94,
    0x70486858, 0x8f45fd19, 0x94de6c87, 0x527bf8b7, 0xab73d323, 0x724b02e2,
    0xe31f8f57, 0x6655ab2a, 0xb2eb2807, 0x2fb5c203, 0x86c57b9a, 0xd33708a5,
    0x302887f2, 0x23bfa5b2, 0x02036aba, 0xed16825c, 0x8acf1c2b, 0xa779b492,
    0xf307f2f0, 0x4e69e2a1, 0x65daf4cd, 0x0605bed5, 0xd134621f, 0xc4a6fe8a,
    0x342e539d, 0xa2f355a0, 0x058ae132, 0xa4f6eb75, 0x0b83ec39, 0x4060efaa,
    0x5e719f06, 0xbd6e1051, 0x3e218af9, 0x96dd063d, 0xdd3e05ae, 0x4de6bd46,
    0x91548db5, 0x71c45d05, 0x0406d46f, 0x605015ff, 0x1998fb24, 0xd6bde997,
    0x894043cc, 0x67d99e77, 0xb0e842bd, 0x07898b88, 0xe7195b38, 0x79c8eedb,
    0xa17c0a47, 0x7c420fe9, 0xf8841ec9, 0x00000000, 0x09808683, 0x322bed48,
    0x1e1170ac, 0x6c5a724e, 0xfd0efffb, 0x0f853856, 0x3daed51e, 0x362d3927,
    0x0a0fd964, 0x685ca621, 0x9b5b54d1, 0x24362e3a, 0x0c0a67b1, 0x9357e70f,
    0xb4ee96d2, 0x1b9b919e, 0x80c0c54f, 0x61dc20a2, 0x5a774b69, 0x1c121a16,
    0xe293ba0a, 0xc0a02ae5, 0x3c22e043, 0x121b171d, 0x0e090d0b, 0xf28bc7ad,
    0x2db6a8b9, 0x141ea9c8, 0x57f11985, 0xaf75074c, 0xee99ddbb, 0xa37f60fd,
    0xf701269f, 0x5c72f5bc, 0x44663bc5, 0x5bfb7e34, 0x8b432976, 0xcb23c6dc,
    0xb6edfc68, 0xb8e4f163, 0xd731dcca, 0x42638510, 0x13972240, 0x84c61120,
    0x854a247d, 0xd2bb3df8, 0xaef93211, 0xc729a16d, 0x1d9e2f4b, 0xdcb230f3,
    0x0d8652ec, 0x77c1e3d0, 0x2bb3166c, 0xa970b999, 0x119448fa, 0x47e96422,
    0xa8fc8cc4, 0xa0f03f1a, 0x567d2cd8, 0x223390ef, 0x87494ec7, 0xd938d1c1,
    0x8ccaa2fe, 0x98d40b36, 0xa6f581cf, 0xa57ade28, 0xdab78e26, 0x3fadbfa4,
    0x2c3a9de4, 0x5078920d, 0x6a5fcc9b, 0x547e4662, 0xf68d13c2, 0x90d8b8e8,
    0x2e39f75e, 0x82c3aff5, 0x9f5d80be, 0x69d0937c, 0x6fd52da9, 0xcf2512b3,
    0xc8ac993b, 0x10187da7, 0xe89c636e, 0xdb3bbb7b, 0xcd267809, 0x6e5918f4,
    0xec9ab701, 0x834f9aa8, 0xe6956e65, 0xaaffe67e, 0x21bccf08, 0xef15e8e6,
    0xbae79bd9, 0x4a6f36ce, 0xea9f09d4, 0x29b07cd6, 0x31a4b2af, 0x2a3f2331,
    0xc6a59430, 0x35a266c0, 0x744ebc37, 0xfc82caa6, 0xe090d0b0, 0x33a7d815,
    0xf104984a, 0x41ecdaf7, 0x7fcd500e, 0x1791f62f, 0x764dd68d, 0x43efb04d,
    0xccaa4d54, 0xe49604df, 0x9ed1b5e3, 0x4c6a881b, 0xc12c1fb8, 0x4665517f,
    0x9d5eea04, 0x018c355d, 0xfa877473, 0xfb0b412e, 0xb3671d5a, 0x92dbd252,
    0xe9105633, 0x6dd64713, 0x9ad7618c, 0x37a10c7a, 0x59f8148e, 0xeb133c89,
    0xcea927ee, 0xb761c935, 0xe11ce5ed, 0x7a47b13c, 0x9cd2df59, 0x55f2733f,
    0x1814ce79, 0x73c737bf, 0x53f7cdea, 0x5ffdaa5b, 0xdf3d6f14, 0x7844db86,
    0xcaaff381, 0xb968c43e, 0x3824342c, 0xc2a3405f, 0x161dc372, 0xbce2250c,
    0x283c498b, 0xff0d9541, 0x39a80171, 0x080cb3de, 0xd8b4e49c, 0x6456c190,
    0x7bcb8461, 0xd532b670, 0x486c5c74, 0xd0b85742,
};
extern "C" const u32 data_ov001_02229734[256] = {
    0x5051f4a7, 0x537e4165, 0xc31a17a4, 0x963a275e, 0xcb3bab6b, 0xf11f9d45,
    0xabacfa58, 0x934be303, 0x552030fa, 0xf6ad766d, 0x9188cc76, 0x25f5024c,
    0xfc4fe5d7, 0xd7c52acb, 0x80263544, 0x8fb562a3, 0x49deb15a, 0x6725ba1b,
    0x9845ea0e, 0xe15dfec0, 0x02c32f75, 0x12814cf0, 0xa38d4697, 0xc66bd3f9,
    0xe7038f5f, 0x9515929c, 0xebbf6d7a, 0xda955259, 0x2dd4be83, 0xd3587421,
    0x2949e069, 0x448ec9c8, 0x6a75c289, 0x78f48e79, 0x6b99583e, 0xdd27b971,
    0xb6bee14f, 0x17f088ad, 0x66c920ac, 0xb47dce3a, 0x1863df4a, 0x82e51a31,
    0x60975133, 0x4562537f, 0xe0b16477, 0x84bb6bae, 0x1cfe81a0, 0x94f9082b,
    0x58704868, 0x198f45fd, 0x8794de6c, 0xb7527bf8, 0x23ab73d3, 0xe2724b02,
    0x57e31f8f, 0x2a6655ab, 0x07b2eb28, 0x032fb5c2, 0x9a86c57b, 0xa5d33708,
    0xf2302887, 0xb223bfa5, 0xba02036a, 0x5ced1682, 0x2b8acf1c, 0x92a779b4,
    0xf0f307f2, 0xa14e69e2, 0xcd65daf4, 0xd50605be, 0x1fd13462, 0x8ac4a6fe,
    0x9d342e53, 0xa0a2f355, 0x32058ae1, 0x75a4f6eb, 0x390b83ec, 0xaa4060ef,
    0x065e719f, 0x51bd6e10, 0xf93e218a, 0x3d96dd06, 0xaedd3e05, 0x464de6bd,
    0xb591548d, 0x0571c45d, 0x6f0406d4, 0xff605015, 0x241998fb, 0x97d6bde9,
    0xcc894043, 0x7767d99e, 0xbdb0e842, 0x8807898b, 0x38e7195b, 0xdb79c8ee,
    0x47a17c0a, 0xe97c420f, 0xc9f8841e, 0x00000000, 0x83098086, 0x48322bed,
    0xac1e1170, 0x4e6c5a72, 0xfbfd0eff, 0x560f8538, 0x1e3daed5, 0x27362d39,
    0x640a0fd9, 0x21685ca6, 0xd19b5b54, 0x3a24362e, 0xb10c0a67, 0x0f9357e7,
    0xd2b4ee96, 0x9e1b9b91, 0x4f80c0c5, 0xa261dc20, 0x695a774b, 0x161c121a,
    0x0ae293ba, 0xe5c0a02a, 0x433c22e0, 0x1d121b17, 0x0b0e090d, 0xadf28bc7,
    0xb92db6a8, 0xc8141ea9, 0x8557f119, 0x4caf7507, 0xbbee99dd, 0xfda37f60,
    0x9ff70126, 0xbc5c72f5, 0xc544663b, 0x345bfb7e, 0x768b4329, 0xdccb23c6,
    0x68b6edfc, 0x63b8e4f1, 0xcad731dc, 0x10426385, 0x40139722, 0x2084c611,
    0x7d854a24, 0xf8d2bb3d, 0x11aef932, 0x6dc729a1, 0x4b1d9e2f, 0xf3dcb230,
    0xec0d8652, 0xd077c1e3, 0x6c2bb316, 0x99a970b9, 0xfa119448, 0x2247e964,
    0xc4a8fc8c, 0x1aa0f03f, 0xd8567d2c, 0xef223390, 0xc787494e, 0xc1d938d1,
    0xfe8ccaa2, 0x3698d40b, 0xcfa6f581, 0x28a57ade, 0x26dab78e, 0xa43fadbf,
    0xe42c3a9d, 0x0d507892, 0x9b6a5fcc, 0x62547e46, 0xc2f68d13, 0xe890d8b8,
    0x5e2e39f7, 0xf582c3af, 0xbe9f5d80, 0x7c69d093, 0xa96fd52d, 0xb3cf2512,
    0x3bc8ac99, 0xa710187d, 0x6ee89c63, 0x7bdb3bbb, 0x09cd2678, 0xf46e5918,
    0x01ec9ab7, 0xa8834f9a, 0x65e6956e, 0x7eaaffe6, 0x0821bccf, 0xe6ef15e8,
    0xd9bae79b, 0xce4a6f36, 0xd4ea9f09, 0xd629b07c, 0xaf31a4b2, 0x312a3f23,
    0x30c6a594, 0xc035a266, 0x37744ebc, 0xa6fc82ca, 0xb0e090d0, 0x1533a7d8,
    0x4af10498, 0xf741ecda, 0x0e7fcd50, 0x2f1791f6, 0x8d764dd6, 0x4d43efb0,
    0x54ccaa4d, 0xdfe49604, 0xe39ed1b5, 0x1b4c6a88, 0xb8c12c1f, 0x7f466551,
    0x049d5eea, 0x5d018c35, 0x73fa8774, 0x2efb0b41, 0x5ab3671d, 0x5292dbd2,
    0x33e91056, 0x136dd647, 0x8c9ad761, 0x7a37a10c, 0x8e59f814, 0x89eb133c,
    0xeecea927, 0x35b761c9, 0xede11ce5, 0x3c7a47b1, 0x599cd2df, 0x3f55f273,
    0x791814ce, 0xbf73c737, 0xea53f7cd, 0x5b5ffdaa, 0x14df3d6f, 0x867844db,
    0x81caaff3, 0x3eb968c4, 0x2c382434, 0x5fc2a340, 0x72161dc3, 0x0cbce225,
    0x8b283c49, 0x41ff0d95, 0x7139a801, 0xde080cb3, 0x9cd8b4e4, 0x906456c1,
    0x617bcb84, 0x70d532b6, 0x74486c5c, 0x42d0b857,
};
extern "C" const u32 data_ov001_02228734[256] = {
    0x63a5c663, 0x7c84f87c, 0x7799ee77, 0x7b8df67b, 0xf20dfff2, 0x6bbdd66b,
    0x6fb1de6f, 0xc55491c5, 0x30506030, 0x01030201, 0x67a9ce67, 0x2b7d562b,
    0xfe19e7fe, 0xd762b5d7, 0xabe64dab, 0x769aec76, 0xca458fca, 0x829d1f82,
    0xc94089c9, 0x7d87fa7d, 0xfa15effa, 0x59ebb259, 0x47c98e47, 0xf00bfbf0,
    0xadec41ad, 0xd467b3d4, 0xa2fd5fa2, 0xafea45af, 0x9cbf239c, 0xa4f753a4,
    0x7296e472, 0xc05b9bc0, 0xb7c275b7, 0xfd1ce1fd, 0x93ae3d93, 0x266a4c26,
    0x365a6c36, 0x3f417e3f, 0xf702f5f7, 0xcc4f83cc, 0x345c6834, 0xa5f451a5,
    0xe534d1e5, 0xf108f9f1, 0x7193e271, 0xd873abd8, 0x31536231, 0x153f2a15,
    0x040c0804, 0xc75295c7, 0x23654623, 0xc35e9dc3, 0x18283018, 0x96a13796,
    0x050f0a05, 0x9ab52f9a, 0x07090e07, 0x12362412, 0x809b1b80, 0xe23ddfe2,
    0xeb26cdeb, 0x27694e27, 0xb2cd7fb2, 0x759fea75, 0x091b1209, 0x839e1d83,
    0x2c74582c, 0x1a2e341a, 0x1b2d361b, 0x6eb2dc6e, 0x5aeeb45a, 0xa0fb5ba0,
    0x52f6a452, 0x3b4d763b, 0xd661b7d6, 0xb3ce7db3, 0x297b5229, 0xe33edde3,
    0x2f715e2f, 0x84971384, 0x53f5a653, 0xd168b9d1, 0x00000000, 0xed2cc1ed,
    0x20604020, 0xfc1fe3fc, 0xb1c879b1, 0x5bedb65b, 0x6abed46a, 0xcb468dcb,
    0xbed967be, 0x394b7239, 0x4ade944a, 0x4cd4984c, 0x58e8b058, 0xcf4a85cf,
    0xd06bbbd0, 0xef2ac5ef, 0xaae54faa, 0xfb16edfb, 0x43c58643, 0x4dd79a4d,
    0x33556633, 0x85941185, 0x45cf8a45, 0xf910e9f9, 0x02060402, 0x7f81fe7f,
    0x50f0a050, 0x3c44783c, 0x9fba259f, 0xa8e34ba8, 0x51f3a251, 0xa3fe5da3,
    0x40c08040, 0x8f8a058f, 0x92ad3f92, 0x9dbc219d, 0x38487038, 0xf504f1f5,
    0xbcdf63bc, 0xb6c177b6, 0xda75afda, 0x21634221, 0x10302010, 0xff1ae5ff,
    0xf30efdf3, 0xd26dbfd2, 0xcd4c81cd, 0x0c14180c, 0x13352613, 0xec2fc3ec,
    0x5fe1be5f, 0x97a23597, 0x44cc8844, 0x17392e17, 0xc45793c4, 0xa7f255a7,
    0x7e82fc7e, 0x3d477a3d, 0x64acc864, 0x5de7ba5d, 0x192b3219, 0x7395e673,
    0x60a0c060, 0x81981981, 0x4fd19e4f, 0xdc7fa3dc, 0x22664422, 0x2a7e542a,
    0x90ab3b90, 0x88830b88, 0x46ca8c46, 0xee29c7ee, 0xb8d36bb8, 0x143c2814,
    0xde79a7de, 0x5ee2bc5e, 0x0b1d160b, 0xdb76addb, 0xe03bdbe0, 0x32566432,
    0x3a4e743a, 0x0a1e140a, 0x49db9249, 0x060a0c06, 0x246c4824, 0x5ce4b85c,
    0xc25d9fc2, 0xd36ebdd3, 0xacef43ac, 0x62a6c462, 0x91a83991, 0x95a43195,
    0xe437d3e4, 0x798bf279, 0xe732d5e7, 0xc8438bc8, 0x37596e37, 0x6db7da6d,
    0x8d8c018d, 0xd564b1d5, 0x4ed29c4e, 0xa9e049a9, 0x6cb4d86c, 0x56faac56,
    0xf407f3f4, 0xea25cfea, 0x65afca65, 0x7a8ef47a, 0xaee947ae, 0x08181008,
    0xbad56fba, 0x7888f078, 0x256f4a25, 0x2e725c2e, 0x1c24381c, 0xa6f157a6,
    0xb4c773b4, 0xc65197c6, 0xe823cbe8, 0xdd7ca1dd, 0x749ce874, 0x1f213e1f,
    0x4bdd964b, 0xbddc61bd, 0x8b860d8b, 0x8a850f8a, 0x7090e070, 0x3e427c3e,
    0xb5c471b5, 0x66aacc66, 0x48d89048, 0x03050603, 0xf601f7f6, 0x0e121c0e,
    0x61a3c261, 0x355f6a35, 0x57f9ae57, 0xb9d069b9, 0x86911786, 0xc15899c1,
    0x1d273a1d, 0x9eb9279e, 0xe138d9e1, 0xf813ebf8, 0x98b32b98, 0x11332211,
    0x69bbd269, 0xd970a9d9, 0x8e89078e, 0x94a73394, 0x9bb62d9b, 0x1e223c1e,
    0x87921587, 0xe920c9e9, 0xce4987ce, 0x55ffaa55, 0x28785028, 0xdf7aa5df,
    0x8c8f038c, 0xa1f859a1, 0x89800989, 0x0d171a0d, 0xbfda65bf, 0xe631d7e6,
    0x42c68442, 0x68b8d068, 0x41c38241, 0x99b02999, 0x2d775a2d, 0x0f111e0f,
    0xb0cb7bb0, 0x54fca854, 0xbbd66dbb, 0x163a2c16,
};
extern "C" const u32 data_ov001_02227334[256] = {
    0xa75051f4, 0x65537e41, 0xa4c31a17, 0x5e963a27, 0x6bcb3bab, 0x45f11f9d,
    0x58abacfa, 0x03934be3, 0xfa552030, 0x6df6ad76, 0x769188cc, 0x4c25f502,
    0xd7fc4fe5, 0xcbd7c52a, 0x44802635, 0xa38fb562, 0x5a49deb1, 0x1b6725ba,
    0x0e9845ea, 0xc0e15dfe, 0x7502c32f, 0xf012814c, 0x97a38d46, 0xf9c66bd3,
    0x5fe7038f, 0x9c951592, 0x7aebbf6d, 0x59da9552, 0x832dd4be, 0x21d35874,
    0x692949e0, 0xc8448ec9, 0x896a75c2, 0x7978f48e, 0x3e6b9958, 0x71dd27b9,
    0x4fb6bee1, 0xad17f088, 0xac66c920, 0x3ab47dce, 0x4a1863df, 0x3182e51a,
    0x33609751, 0x7f456253, 0x77e0b164, 0xae84bb6b, 0xa01cfe81, 0x2b94f908,
    0x68587048, 0xfd198f45, 0x6c8794de, 0xf8b7527b, 0xd323ab73, 0x02e2724b,
    0x8f57e31f, 0xab2a6655, 0x2807b2eb, 0xc2032fb5, 0x7b9a86c5, 0x08a5d337,
    0x87f23028, 0xa5b223bf, 0x6aba0203, 0x825ced16, 0x1c2b8acf, 0xb492a779,
    0xf2f0f307, 0xe2a14e69, 0xf4cd65da, 0xbed50605, 0x621fd134, 0xfe8ac4a6,
    0x539d342e, 0x55a0a2f3, 0xe132058a, 0xeb75a4f6, 0xec390b83, 0xefaa4060,
    0x9f065e71, 0x1051bd6e, 0x8af93e21, 0x063d96dd, 0x05aedd3e, 0xbd464de6,
    0x8db59154, 0x5d0571c4, 0xd46f0406, 0x15ff6050, 0xfb241998, 0xe997d6bd,
    0x43cc8940, 0x9e7767d9, 0x42bdb0e8, 0x8b880789, 0x5b38e719, 0xeedb79c8,
    0x0a47a17c, 0x0fe97c42, 0x1ec9f884, 0x00000000, 0x86830980, 0xed48322b,
    0x70ac1e11, 0x724e6c5a, 0xfffbfd0e, 0x38560f85, 0xd51e3dae, 0x3927362d,
    0xd9640a0f, 0xa621685c, 0x54d19b5b, 0x2e3a2436, 0x67b10c0a, 0xe70f9357,
    0x96d2b4ee, 0x919e1b9b, 0xc54f80c0, 0x20a261dc, 0x4b695a77, 0x1a161c12,
    0xba0ae293, 0x2ae5c0a0, 0xe0433c22, 0x171d121b, 0x0d0b0e09, 0xc7adf28b,
    0xa8b92db6, 0xa9c8141e, 0x198557f1, 0x074caf75, 0xddbbee99, 0x60fda37f,
    0x269ff701, 0xf5bc5c72, 0x3bc54466, 0x7e345bfb, 0x29768b43, 0xc6dccb23,
    0xfc68b6ed, 0xf163b8e4, 0xdccad731, 0x85104263, 0x22401397, 0x112084c6,
    0x247d854a, 0x3df8d2bb, 0x3211aef9, 0xa16dc729, 0x2f4b1d9e, 0x30f3dcb2,
    0x52ec0d86, 0xe3d077c1, 0x166c2bb3, 0xb999a970, 0x48fa1194, 0x642247e9,
    0x8cc4a8fc, 0x3f1aa0f0, 0x2cd8567d, 0x90ef2233, 0x4ec78749, 0xd1c1d938,
    0xa2fe8cca, 0x0b3698d4, 0x81cfa6f5, 0xde28a57a, 0x8e26dab7, 0xbfa43fad,
    0x9de42c3a, 0x920d5078, 0xcc9b6a5f, 0x4662547e, 0x13c2f68d, 0xb8e890d8,
    0xf75e2e39, 0xaff582c3, 0x80be9f5d, 0x937c69d0, 0x2da96fd5, 0x12b3cf25,
    0x993bc8ac, 0x7da71018, 0x636ee89c, 0xbb7bdb3b, 0x7809cd26, 0x18f46e59,
    0xb701ec9a, 0x9aa8834f, 0x6e65e695, 0xe67eaaff, 0xcf0821bc, 0xe8e6ef15,
    0x9bd9bae7, 0x36ce4a6f, 0x09d4ea9f, 0x7cd629b0, 0xb2af31a4, 0x23312a3f,
    0x9430c6a5, 0x66c035a2, 0xbc37744e, 0xcaa6fc82, 0xd0b0e090, 0xd81533a7,
    0x984af104, 0xdaf741ec, 0x500e7fcd, 0xf62f1791, 0xd68d764d, 0xb04d43ef,
    0x4d54ccaa, 0x04dfe496, 0xb5e39ed1, 0x881b4c6a, 0x1fb8c12c, 0x517f4665,
    0xea049d5e, 0x355d018c, 0x7473fa87, 0x412efb0b, 0x1d5ab367, 0xd25292db,
    0x5633e910, 0x47136dd6, 0x618c9ad7, 0x0c7a37a1, 0x148e59f8, 0x3c89eb13,
    0x27eecea9, 0xc935b761, 0xe5ede11c, 0xb13c7a47, 0xdf599cd2, 0x733f55f2,
    0xce791814, 0x37bf73c7, 0xcdea53f7, 0xaa5b5ffd, 0x6f14df3d, 0xdb867844,
    0xf381caaf, 0xc43eb968, 0x342c3824, 0x405fc2a3, 0xc372161d, 0x250cbce2,
    0x498b283c, 0x9541ff0d, 0x017139a8, 0xb3de080c, 0xe49cd8b4, 0xc1906456,
    0x84617bcb, 0xb670d532, 0x5c74486c, 0x5742d0b8,
};
extern "C" const u32 data_ov001_02227734[256] = {
    0xf4a75051, 0x4165537e, 0x17a4c31a, 0x275e963a, 0xab6bcb3b, 0x9d45f11f,
    0xfa58abac, 0xe303934b, 0x30fa5520, 0x766df6ad, 0xcc769188, 0x024c25f5,
    0xe5d7fc4f, 0x2acbd7c5, 0x35448026, 0x62a38fb5, 0xb15a49de, 0xba1b6725,
    0xea0e9845, 0xfec0e15d, 0x2f7502c3, 0x4cf01281, 0x4697a38d, 0xd3f9c66b,
    0x8f5fe703, 0x929c9515, 0x6d7aebbf, 0x5259da95, 0xbe832dd4, 0x7421d358,
    0xe0692949, 0xc9c8448e, 0xc2896a75, 0x8e7978f4, 0x583e6b99, 0xb971dd27,
    0xe14fb6be, 0x88ad17f0, 0x20ac66c9, 0xce3ab47d, 0xdf4a1863, 0x1a3182e5,
    0x51336097, 0x537f4562, 0x6477e0b1, 0x6bae84bb, 0x81a01cfe, 0x082b94f9,
    0x48685870, 0x45fd198f, 0xde6c8794, 0x7bf8b752, 0x73d323ab, 0x4b02e272,
    0x1f8f57e3, 0x55ab2a66, 0xeb2807b2, 0xb5c2032f, 0xc57b9a86, 0x3708a5d3,
    0x2887f230, 0xbfa5b223, 0x036aba02, 0x16825ced, 0xcf1c2b8a, 0x79b492a7,
    0x07f2f0f3, 0x69e2a14e, 0xdaf4cd65, 0x05bed506, 0x34621fd1, 0xa6fe8ac4,
    0x2e539d34, 0xf355a0a2, 0x8ae13205, 0xf6eb75a4, 0x83ec390b, 0x60efaa40,
    0x719f065e, 0x6e1051bd, 0x218af93e, 0xdd063d96, 0x3e05aedd, 0xe6bd464d,
    0x548db591, 0xc45d0571, 0x06d46f04, 0x5015ff60, 0x98fb2419, 0xbde997d6,
    0x4043cc89, 0xd99e7767, 0xe842bdb0, 0x898b8807, 0x195b38e7, 0xc8eedb79,
    0x7c0a47a1, 0x420fe97c, 0x841ec9f8, 0x00000000, 0x80868309, 0x2bed4832,
    0x1170ac1e, 0x5a724e6c, 0x0efffbfd, 0x8538560f, 0xaed51e3d, 0x2d392736,
    0x0fd9640a, 0x5ca62168, 0x5b54d19b, 0x362e3a24, 0x0a67b10c, 0x57e70f93,
    0xee96d2b4, 0x9b919e1b, 0xc0c54f80, 0xdc20a261, 0x774b695a, 0x121a161c,
    0x93ba0ae2, 0xa02ae5c0, 0x22e0433c, 0x1b171d12, 0x090d0b0e, 0x8bc7adf2,
    0xb6a8b92d, 0x1ea9c814, 0xf1198557, 0x75074caf, 0x99ddbbee, 0x7f60fda3,
    0x01269ff7, 0x72f5bc5c, 0x663bc544, 0xfb7e345b, 0x4329768b, 0x23c6dccb,
    0xedfc68b6, 0xe4f163b8, 0x31dccad7, 0x63851042, 0x97224013, 0xc6112084,
    0x4a247d85, 0xbb3df8d2, 0xf93211ae, 0x29a16dc7, 0x9e2f4b1d, 0xb230f3dc,
    0x8652ec0d, 0xc1e3d077, 0xb3166c2b, 0x70b999a9, 0x9448fa11, 0xe9642247,
    0xfc8cc4a8, 0xf03f1aa0, 0x7d2cd856, 0x3390ef22, 0x494ec787, 0x38d1c1d9,
    0xcaa2fe8c, 0xd40b3698, 0xf581cfa6, 0x7ade28a5, 0xb78e26da, 0xadbfa43f,
    0x3a9de42c, 0x78920d50, 0x5fcc9b6a, 0x7e466254, 0x8d13c2f6, 0xd8b8e890,
    0x39f75e2e, 0xc3aff582, 0x5d80be9f, 0xd0937c69, 0xd52da96f, 0x2512b3cf,
    0xac993bc8, 0x187da710, 0x9c636ee8, 0x3bbb7bdb, 0x267809cd, 0x5918f46e,
    0x9ab701ec, 0x4f9aa883, 0x956e65e6, 0xffe67eaa, 0xbccf0821, 0x15e8e6ef,
    0xe79bd9ba, 0x6f36ce4a, 0x9f09d4ea, 0xb07cd629, 0xa4b2af31, 0x3f23312a,
    0xa59430c6, 0xa266c035, 0x4ebc3774, 0x82caa6fc, 0x90d0b0e0, 0xa7d81533,
    0x04984af1, 0xecdaf741, 0xcd500e7f, 0x91f62f17, 0x4dd68d76, 0xefb04d43,
    0xaa4d54cc, 0x9604dfe4, 0xd1b5e39e, 0x6a881b4c, 0x2c1fb8c1, 0x65517f46,
    0x5eea049d, 0x8c355d01, 0x877473fa, 0x0b412efb, 0x671d5ab3, 0xdbd25292,
    0x105633e9, 0xd647136d, 0xd7618c9a, 0xa10c7a37, 0xf8148e59, 0x133c89eb,
    0xa927eece, 0x61c935b7, 0x1ce5ede1, 0x47b13c7a, 0xd2df599c, 0xf2733f55,
    0x14ce7918, 0xc737bf73, 0xf7cdea53, 0xfdaa5b5f, 0x3d6f14df, 0x44db8678,
    0xaff381ca, 0x68c43eb9, 0x24342c38, 0xa3405fc2, 0x1dc37216, 0xe2250cbc,
    0x3c498b28, 0x0d9541ff, 0xa8017139, 0x0cb3de08, 0xb4e49cd8, 0x56c19064,
    0xcb84617b, 0x32b670d5, 0x6c5c7448, 0xb85742d0,
};
extern "C" const u32 data_ov001_02227b34[256] = {
    0x52525252, 0x09090909, 0x6a6a6a6a, 0xd5d5d5d5, 0x30303030, 0x36363636,
    0xa5a5a5a5, 0x38383838, 0xbfbfbfbf, 0x40404040, 0xa3a3a3a3, 0x9e9e9e9e,
    0x81818181, 0xf3f3f3f3, 0xd7d7d7d7, 0xfbfbfbfb, 0x7c7c7c7c, 0xe3e3e3e3,
    0x39393939, 0x82828282, 0x9b9b9b9b, 0x2f2f2f2f, 0xffffffff, 0x87878787,
    0x34343434, 0x8e8e8e8e, 0x43434343, 0x44444444, 0xc4c4c4c4, 0xdededede,
    0xe9e9e9e9, 0xcbcbcbcb, 0x54545454, 0x7b7b7b7b, 0x94949494, 0x32323232,
    0xa6a6a6a6, 0xc2c2c2c2, 0x23232323, 0x3d3d3d3d, 0xeeeeeeee, 0x4c4c4c4c,
    0x95959595, 0x0b0b0b0b, 0x42424242, 0xfafafafa, 0xc3c3c3c3, 0x4e4e4e4e,
    0x08080808, 0x2e2e2e2e, 0xa1a1a1a1, 0x66666666, 0x28282828, 0xd9d9d9d9,
    0x24242424, 0xb2b2b2b2, 0x76767676, 0x5b5b5b5b, 0xa2a2a2a2, 0x49494949,
    0x6d6d6d6d, 0x8b8b8b8b, 0xd1d1d1d1, 0x25252525, 0x72727272, 0xf8f8f8f8,
    0xf6f6f6f6, 0x64646464, 0x86868686, 0x68686868, 0x98989898, 0x16161616,
    0xd4d4d4d4, 0xa4a4a4a4, 0x5c5c5c5c, 0xcccccccc, 0x5d5d5d5d, 0x65656565,
    0xb6b6b6b6, 0x92929292, 0x6c6c6c6c, 0x70707070, 0x48484848, 0x50505050,
    0xfdfdfdfd, 0xedededed, 0xb9b9b9b9, 0xdadadada, 0x5e5e5e5e, 0x15151515,
    0x46464646, 0x57575757, 0xa7a7a7a7, 0x8d8d8d8d, 0x9d9d9d9d, 0x84848484,
    0x90909090, 0xd8d8d8d8, 0xabababab, 0x00000000, 0x8c8c8c8c, 0xbcbcbcbc,
    0xd3d3d3d3, 0x0a0a0a0a, 0xf7f7f7f7, 0xe4e4e4e4, 0x58585858, 0x05050505,
    0xb8b8b8b8, 0xb3b3b3b3, 0x45454545, 0x06060606, 0xd0d0d0d0, 0x2c2c2c2c,
    0x1e1e1e1e, 0x8f8f8f8f, 0xcacacaca, 0x3f3f3f3f, 0x0f0f0f0f, 0x02020202,
    0xc1c1c1c1, 0xafafafaf, 0xbdbdbdbd, 0x03030303, 0x01010101, 0x13131313,
    0x8a8a8a8a, 0x6b6b6b6b, 0x3a3a3a3a, 0x91919191, 0x11111111, 0x41414141,
    0x4f4f4f4f, 0x67676767, 0xdcdcdcdc, 0xeaeaeaea, 0x97979797, 0xf2f2f2f2,
    0xcfcfcfcf, 0xcececece, 0xf0f0f0f0, 0xb4b4b4b4, 0xe6e6e6e6, 0x73737373,
    0x96969696, 0xacacacac, 0x74747474, 0x22222222, 0xe7e7e7e7, 0xadadadad,
    0x35353535, 0x85858585, 0xe2e2e2e2, 0xf9f9f9f9, 0x37373737, 0xe8e8e8e8,
    0x1c1c1c1c, 0x75757575, 0xdfdfdfdf, 0x6e6e6e6e, 0x47474747, 0xf1f1f1f1,
    0x1a1a1a1a, 0x71717171, 0x1d1d1d1d, 0x29292929, 0xc5c5c5c5, 0x89898989,
    0x6f6f6f6f, 0xb7b7b7b7, 0x62626262, 0x0e0e0e0e, 0xaaaaaaaa, 0x18181818,
    0xbebebebe, 0x1b1b1b1b, 0xfcfcfcfc, 0x56565656, 0x3e3e3e3e, 0x4b4b4b4b,
    0xc6c6c6c6, 0xd2d2d2d2, 0x79797979, 0x20202020, 0x9a9a9a9a, 0xdbdbdbdb,
    0xc0c0c0c0, 0xfefefefe, 0x78787878, 0xcdcdcdcd, 0x5a5a5a5a, 0xf4f4f4f4,
    0x1f1f1f1f, 0xdddddddd, 0xa8a8a8a8, 0x33333333, 0x88888888, 0x07070707,
    0xc7c7c7c7, 0x31313131, 0xb1b1b1b1, 0x12121212, 0x10101010, 0x59595959,
    0x27272727, 0x80808080, 0xecececec, 0x5f5f5f5f, 0x60606060, 0x51515151,
    0x7f7f7f7f, 0xa9a9a9a9, 0x19191919, 0xb5b5b5b5, 0x4a4a4a4a, 0x0d0d0d0d,
    0x2d2d2d2d, 0xe5e5e5e5, 0x7a7a7a7a, 0x9f9f9f9f, 0x93939393, 0xc9c9c9c9,
    0x9c9c9c9c, 0xefefefef, 0xa0a0a0a0, 0xe0e0e0e0, 0x3b3b3b3b, 0x4d4d4d4d,
    0xaeaeaeae, 0x2a2a2a2a, 0xf5f5f5f5, 0xb0b0b0b0, 0xc8c8c8c8, 0xebebebeb,
    0xbbbbbbbb, 0x3c3c3c3c, 0x83838383, 0x53535353, 0x99999999, 0x61616161,
    0x17171717, 0x2b2b2b2b, 0x04040404, 0x7e7e7e7e, 0xbabababa, 0x77777777,
    0xd6d6d6d6, 0x26262626, 0xe1e1e1e1, 0x69696969, 0x14141414, 0x63636363,
    0x55555555, 0x21212121, 0x0c0c0c0c, 0x7d7d7d7d,
};
extern "C" const u32 data_ov001_02227f34[256] = {
    0xc66363a5, 0xf87c7c84, 0xee777799, 0xf67b7b8d, 0xfff2f20d, 0xd66b6bbd,
    0xde6f6fb1, 0x91c5c554, 0x60303050, 0x02010103, 0xce6767a9, 0x562b2b7d,
    0xe7fefe19, 0xb5d7d762, 0x4dababe6, 0xec76769a, 0x8fcaca45, 0x1f82829d,
    0x89c9c940, 0xfa7d7d87, 0xeffafa15, 0xb25959eb, 0x8e4747c9, 0xfbf0f00b,
    0x41adadec, 0xb3d4d467, 0x5fa2a2fd, 0x45afafea, 0x239c9cbf, 0x53a4a4f7,
    0xe4727296, 0x9bc0c05b, 0x75b7b7c2, 0xe1fdfd1c, 0x3d9393ae, 0x4c26266a,
    0x6c36365a, 0x7e3f3f41, 0xf5f7f702, 0x83cccc4f, 0x6834345c, 0x51a5a5f4,
    0xd1e5e534, 0xf9f1f108, 0xe2717193, 0xabd8d873, 0x62313153, 0x2a15153f,
    0x0804040c, 0x95c7c752, 0x46232365, 0x9dc3c35e, 0x30181828, 0x379696a1,
    0x0a05050f, 0x2f9a9ab5, 0x0e070709, 0x24121236, 0x1b80809b, 0xdfe2e23d,
    0xcdebeb26, 0x4e272769, 0x7fb2b2cd, 0xea75759f, 0x1209091b, 0x1d83839e,
    0x582c2c74, 0x341a1a2e, 0x361b1b2d, 0xdc6e6eb2, 0xb45a5aee, 0x5ba0a0fb,
    0xa45252f6, 0x763b3b4d, 0xb7d6d661, 0x7db3b3ce, 0x5229297b, 0xdde3e33e,
    0x5e2f2f71, 0x13848497, 0xa65353f5, 0xb9d1d168, 0x00000000, 0xc1eded2c,
    0x40202060, 0xe3fcfc1f, 0x79b1b1c8, 0xb65b5bed, 0xd46a6abe, 0x8dcbcb46,
    0x67bebed9, 0x7239394b, 0x944a4ade, 0x984c4cd4, 0xb05858e8, 0x85cfcf4a,
    0xbbd0d06b, 0xc5efef2a, 0x4faaaae5, 0xedfbfb16, 0x864343c5, 0x9a4d4dd7,
    0x66333355, 0x11858594, 0x8a4545cf, 0xe9f9f910, 0x04020206, 0xfe7f7f81,
    0xa05050f0, 0x783c3c44, 0x259f9fba, 0x4ba8a8e3, 0xa25151f3, 0x5da3a3fe,
    0x804040c0, 0x058f8f8a, 0x3f9292ad, 0x219d9dbc, 0x70383848, 0xf1f5f504,
    0x63bcbcdf, 0x77b6b6c1, 0xafdada75, 0x42212163, 0x20101030, 0xe5ffff1a,
    0xfdf3f30e, 0xbfd2d26d, 0x81cdcd4c, 0x180c0c14, 0x26131335, 0xc3ecec2f,
    0xbe5f5fe1, 0x359797a2, 0x884444cc, 0x2e171739, 0x93c4c457, 0x55a7a7f2,
    0xfc7e7e82, 0x7a3d3d47, 0xc86464ac, 0xba5d5de7, 0x3219192b, 0xe6737395,
    0xc06060a0, 0x19818198, 0x9e4f4fd1, 0xa3dcdc7f, 0x44222266, 0x542a2a7e,
    0x3b9090ab, 0x0b888883, 0x8c4646ca, 0xc7eeee29, 0x6bb8b8d3, 0x2814143c,
    0xa7dede79, 0xbc5e5ee2, 0x160b0b1d, 0xaddbdb76, 0xdbe0e03b, 0x64323256,
    0x743a3a4e, 0x140a0a1e, 0x924949db, 0x0c06060a, 0x4824246c, 0xb85c5ce4,
    0x9fc2c25d, 0xbdd3d36e, 0x43acacef, 0xc46262a6, 0x399191a8, 0x319595a4,
    0xd3e4e437, 0xf279798b, 0xd5e7e732, 0x8bc8c843, 0x6e373759, 0xda6d6db7,
    0x018d8d8c, 0xb1d5d564, 0x9c4e4ed2, 0x49a9a9e0, 0xd86c6cb4, 0xac5656fa,
    0xf3f4f407, 0xcfeaea25, 0xca6565af, 0xf47a7a8e, 0x47aeaee9, 0x10080818,
    0x6fbabad5, 0xf0787888, 0x4a25256f, 0x5c2e2e72, 0x381c1c24, 0x57a6a6f1,
    0x73b4b4c7, 0x97c6c651, 0xcbe8e823, 0xa1dddd7c, 0xe874749c, 0x3e1f1f21,
    0x964b4bdd, 0x61bdbddc, 0x0d8b8b86, 0x0f8a8a85, 0xe0707090, 0x7c3e3e42,
    0x71b5b5c4, 0xcc6666aa, 0x904848d8, 0x06030305, 0xf7f6f601, 0x1c0e0e12,
    0xc26161a3, 0x6a35355f, 0xae5757f9, 0x69b9b9d0, 0x17868691, 0x99c1c158,
    0x3a1d1d27, 0x279e9eb9, 0xd9e1e138, 0xebf8f813, 0x2b9898b3, 0x22111133,
    0xd26969bb, 0xa9d9d970, 0x078e8e89, 0x339494a7, 0x2d9b9bb6, 0x3c1e1e22,
    0x15878792, 0xc9e9e920, 0x87cece49, 0xaa5555ff, 0x50282878, 0xa5dfdf7a,
    0x038c8c8f, 0x59a1a1f8, 0x09898980, 0x1a0d0d17, 0x65bfbfda, 0xd7e6e631,
    0x844242c6, 0xd06868b8, 0x824141c3, 0x299999b0, 0x5a2d2d77, 0x1e0f0f11,
    0x7bb0b0cb, 0xa85454fc, 0x6dbbbbd6, 0x2c16163a,
};
extern "C" const u32 data_ov001_02228334[256] = {
    0xa5c66363, 0x84f87c7c, 0x99ee7777, 0x8df67b7b, 0x0dfff2f2, 0xbdd66b6b,
    0xb1de6f6f, 0x5491c5c5, 0x50603030, 0x03020101, 0xa9ce6767, 0x7d562b2b,
    0x19e7fefe, 0x62b5d7d7, 0xe64dabab, 0x9aec7676, 0x458fcaca, 0x9d1f8282,
    0x4089c9c9, 0x87fa7d7d, 0x15effafa, 0xebb25959, 0xc98e4747, 0x0bfbf0f0,
    0xec41adad, 0x67b3d4d4, 0xfd5fa2a2, 0xea45afaf, 0xbf239c9c, 0xf753a4a4,
    0x96e47272, 0x5b9bc0c0, 0xc275b7b7, 0x1ce1fdfd, 0xae3d9393, 0x6a4c2626,
    0x5a6c3636, 0x417e3f3f, 0x02f5f7f7, 0x4f83cccc, 0x5c683434, 0xf451a5a5,
    0x34d1e5e5, 0x08f9f1f1, 0x93e27171, 0x73abd8d8, 0x53623131, 0x3f2a1515,
    0x0c080404, 0x5295c7c7, 0x65462323, 0x5e9dc3c3, 0x28301818, 0xa1379696,
    0x0f0a0505, 0xb52f9a9a, 0x090e0707, 0x36241212, 0x9b1b8080, 0x3ddfe2e2,
    0x26cdebeb, 0x694e2727, 0xcd7fb2b2, 0x9fea7575, 0x1b120909, 0x9e1d8383,
    0x74582c2c, 0x2e341a1a, 0x2d361b1b, 0xb2dc6e6e, 0xeeb45a5a, 0xfb5ba0a0,
    0xf6a45252, 0x4d763b3b, 0x61b7d6d6, 0xce7db3b3, 0x7b522929, 0x3edde3e3,
    0x715e2f2f, 0x97138484, 0xf5a65353, 0x68b9d1d1, 0x00000000, 0x2cc1eded,
    0x60402020, 0x1fe3fcfc, 0xc879b1b1, 0xedb65b5b, 0xbed46a6a, 0x468dcbcb,
    0xd967bebe, 0x4b723939, 0xde944a4a, 0xd4984c4c, 0xe8b05858, 0x4a85cfcf,
    0x6bbbd0d0, 0x2ac5efef, 0xe54faaaa, 0x16edfbfb, 0xc5864343, 0xd79a4d4d,
    0x55663333, 0x94118585, 0xcf8a4545, 0x10e9f9f9, 0x06040202, 0x81fe7f7f,
    0xf0a05050, 0x44783c3c, 0xba259f9f, 0xe34ba8a8, 0xf3a25151, 0xfe5da3a3,
    0xc0804040, 0x8a058f8f, 0xad3f9292, 0xbc219d9d, 0x48703838, 0x04f1f5f5,
    0xdf63bcbc, 0xc177b6b6, 0x75afdada, 0x63422121, 0x30201010, 0x1ae5ffff,
    0x0efdf3f3, 0x6dbfd2d2, 0x4c81cdcd, 0x14180c0c, 0x35261313, 0x2fc3ecec,
    0xe1be5f5f, 0xa2359797, 0xcc884444, 0x392e1717, 0x5793c4c4, 0xf255a7a7,
    0x82fc7e7e, 0x477a3d3d, 0xacc86464, 0xe7ba5d5d, 0x2b321919, 0x95e67373,
    0xa0c06060, 0x98198181, 0xd19e4f4f, 0x7fa3dcdc, 0x66442222, 0x7e542a2a,
    0xab3b9090, 0x830b8888, 0xca8c4646, 0x29c7eeee, 0xd36bb8b8, 0x3c281414,
    0x79a7dede, 0xe2bc5e5e, 0x1d160b0b, 0x76addbdb, 0x3bdbe0e0, 0x56643232,
    0x4e743a3a, 0x1e140a0a, 0xdb924949, 0x0a0c0606, 0x6c482424, 0xe4b85c5c,
    0x5d9fc2c2, 0x6ebdd3d3, 0xef43acac, 0xa6c46262, 0xa8399191, 0xa4319595,
    0x37d3e4e4, 0x8bf27979, 0x32d5e7e7, 0x438bc8c8, 0x596e3737, 0xb7da6d6d,
    0x8c018d8d, 0x64b1d5d5, 0xd29c4e4e, 0xe049a9a9, 0xb4d86c6c, 0xfaac5656,
    0x07f3f4f4, 0x25cfeaea, 0xafca6565, 0x8ef47a7a, 0xe947aeae, 0x18100808,
    0xd56fbaba, 0x88f07878, 0x6f4a2525, 0x725c2e2e, 0x24381c1c, 0xf157a6a6,
    0xc773b4b4, 0x5197c6c6, 0x23cbe8e8, 0x7ca1dddd, 0x9ce87474, 0x213e1f1f,
    0xdd964b4b, 0xdc61bdbd, 0x860d8b8b, 0x850f8a8a, 0x90e07070, 0x427c3e3e,
    0xc471b5b5, 0xaacc6666, 0xd8904848, 0x05060303, 0x01f7f6f6, 0x121c0e0e,
    0xa3c26161, 0x5f6a3535, 0xf9ae5757, 0xd069b9b9, 0x91178686, 0x5899c1c1,
    0x273a1d1d, 0xb9279e9e, 0x38d9e1e1, 0x13ebf8f8, 0xb32b9898, 0x33221111,
    0xbbd26969, 0x70a9d9d9, 0x89078e8e, 0xa7339494, 0xb62d9b9b, 0x223c1e1e,
    0x92158787, 0x20c9e9e9, 0x4987cece, 0xffaa5555, 0x78502828, 0x7aa5dfdf,
    0x8f038c8c, 0xf859a1a1, 0x80098989, 0x171a0d0d, 0xda65bfbf, 0x31d7e6e6,
    0xc6844242, 0xb8d06868, 0xc3824141, 0xb0299999, 0x775a2d2d, 0x111e0f0f,
    0xcb7bb0b0, 0xfca85454, 0xd66dbbbb, 0x3a2c1616,
};
extern "C" u8 *data_ov001_0222c898 = 0;
extern "C" s32 sSimpleStartStackSize = 0x800;
extern "C" SimpleStartAllocFunc sSimpleStartAllocFunc = 0;
extern "C" s32 sSimpleStartStep = 0;
extern "C" u32 data_ov001_0222c864 = 0;
extern "C" u8 sSimpleStartSetupSsid[7] = {0x2a, 0x2a, 0x2a, 0x2a, 0x2a, 0x2a, 0};
extern "C" u8 data_ov001_0222a548[7] = {6, 0, 1, 2, 3, 4, 5};
extern "C" char data_ov001_0222a550[12] = "NINTENDO-DS";
extern "C" u8 sSimpleStartMd5Padding[64] = {0x80};
// the socket layer's setup: static addresses 192.168.0.176/24, gateway .200, DNS .32 (DHCP mode 0)
extern "C" SockStartupConfig sSimpleStartSocConfig = {0x1000000, SimpleStart_SocAlloc, SimpleStart_SocFree, 0, 0xb000a8c0, 0xffffff, 0xc800a8c0, 0x2000a8c0, 0, 0x1000, 0x1000, 0, 0, {0, 0, 0, 0, 0}, data_ov001_0222a550, 0x4, {0, 0}};
extern "C" u8 data_ov001_0222c850 = 0;
extern "C" u8 *data_ov001_0222c894 = 0;
extern "C" u32 sSimpleStartDeadline = 0xffffffff;
extern "C" u8 *data_ov001_0222c89c = 0;
extern "C" s32 data_ov001_0222c868 = 0;
extern "C" u8 *data_ov001_0222a52c = sSimpleStartTlvBuf + 8;
extern "C" s32 data_ov001_0222a53c = 0x40;
extern "C" u32 data_ov001_0222c884 = 0;
extern "C" SimpleStartWcmNotifyFunc sSimpleStartWcmNotifyCb = 0;
extern "C" SimpleStartStatusFunc sSimpleStartStatusCb = 0;
extern "C" s32 sSimpleStartResultCode = 0;
extern "C" s32 data_ov001_0222c8c4 = 0;
extern "C" s32 sSimpleStartMaxAps = 0;
extern "C" s32 sSimpleStartMsgTail = 0;
extern "C" s32 data_ov001_0222c8b4 = 0;
extern "C" s32 data_ov001_0222a530 = 0x1;
extern "C" u8 *data_ov001_0222c8ac = 0;
extern "C" s32 data_ov001_0222c870 = 0;
extern "C" s32 data_ov001_0222c8a8 = 0;
extern "C" u8 *data_ov001_0222c8a4 = 0;
extern "C" s32 data_ov001_0222c8a0 = 0;
extern "C" WifiLinkConfig *data_ov001_0222c858 = 0;
extern "C" s32 data_ov001_0222c874 = 0;
extern "C" s32 sSimpleStartCancel = 0;
extern "C" u8 *data_ov001_0222c890 = 0;
extern "C" u8 *data_ov001_0222c88c = 0;
extern "C" s32 sSimpleStartState = 0;
extern "C" SimpleStartFreeFunc sSimpleStartFreeFunc = 0;
extern "C" u8 *data_ov001_0222c880 = 0;
extern "C" s32 sSimpleStartWcmState = 0;
extern "C" s32 sSimpleStartMsgHead = 0;
extern "C" u8 *data_ov001_0222c878 = 0;
extern "C" s32 data_ov001_0222c8b0 = 0;
extern "C" u8 data_ov001_0222c8d4[6] = {0};
extern "C" u8 data_ov001_0222c8dc[6] = {0};
extern "C" u8 data_ov001_0222c8e4[8] = {0};
extern "C" u32 sSimpleStartMsgQueue[4] = {0};
extern "C" u8 data_ov001_0222c8fc[0x10] = {0};
extern "C" OSMutex sSimpleStartAllocMutex = {};
extern "C" u8 data_ov001_0222c944[0x20] = {0};
extern "C" u8 data_ov001_0222c924[0x20] = {0};
extern "C" u8 data_ov001_0222c964[0x24] = {0};
extern "C" OSThread sSimpleStartThread = {};
extern "C" u32 sSimpleStartResult[58] = {0};
extern "C" u32 data_ov001_0222cb30[149] = {0};
extern "C" u8 sSimpleStartTlvBuf[0x800] = {0};
extern "C" u8 sSimpleStartPacketBuf[0x800] = {0};



struct SimpleStartKeyWrapBlock { u8 b[8]; };

struct SimpleStartSecurityList {
    u8 b[7];
};

static inline u32 Unk_ov001_02206ef8_AL(u32 v, u32 a) {
    return (v + a - 1) & ~(a - 1);
}

enum Unk_ov001_02206ef8_Mask { UNK_OV001_02206EF8_MASK = 0x20 - 1 };

static inline void Unk_ov001_02205e18_Cp(SimpleStartPsk *d, SimpleStartPsk *s) { *d = *s; }


static inline u32 Unk_ov001_0220607c_Swap(u8 *p)
{
    u16 v = *(u16 *)p;
    return (u16)(((v >> 8) & 0xff) | ((v << 8) & 0xff00));
}


struct SimpleStartProtocolLocals { SockAddrIn req; SockAddrIn from; u32 w54; u32 w58; u32 w5c; };

extern "C" {

void SimpleStart_WcmCallback(WifiLinkEvent *p) {
    if (p == 0) {
        return;
    }
    switch (p->request) {
    case 1:
        if (p->result == 0) {
            s32 s = sSimpleStartWcmState;
            if (s == 4) {
                sSimpleStartWcmState = 3;
                if (sSimpleStartWcmNotifyCb != 0) {
                    sSimpleStartWcmNotifyCb(6, 0);
                }
                return;
            } else if (s == 6) {
                if (WifiLink_SearchAsync(data_ov001_0222c894, data_ov001_0222c898, data_ov001_0222c8b0) == 3) {
                    return;
                }
                sSimpleStartWcmState = 3;
                if (sSimpleStartWcmNotifyCb != 0) {
                    sSimpleStartWcmNotifyCb(2, 0);
                }
                return;
            } else if (s == 8) {
                if (WifiLink_ConnectAsync(data_ov001_0222c89c, data_ov001_0222c878, data_ov001_0222c8a8) == 3) {
                    return;
                }
                sSimpleStartWcmState = 3;
                if (sSimpleStartWcmNotifyCb != 0) {
                    sSimpleStartWcmNotifyCb(2, 0);
                }
                return;
            }
            return;
        } else {
            sSimpleStartWcmState = 1;
            if (sSimpleStartWcmNotifyCb != 0) {
                sSimpleStartWcmNotifyCb(2, 0);
            }
            return;
        }
    case 3:
        if (p->result == 0) {
            if (sSimpleStartWcmState == 6) {
                sSimpleStartWcmState = 5;
                if (sSimpleStartWcmNotifyCb != 0) {
                    sSimpleStartWcmNotifyCb(8, 0);
                }
            }
        } else {
            sSimpleStartWcmState = 3;
            if (sSimpleStartWcmNotifyCb != 0) {
                sSimpleStartWcmNotifyCb(9, 0);
            }
        }
        return;
    case 5:
        if (p->result == 0) {
            if (sSimpleStartWcmState == 8) {
                sSimpleStartWcmState = 7;
                if (sSimpleStartWcmNotifyCb != 0) {
                    sSimpleStartWcmNotifyCb(12, 0);
                }
            }
        } else {
            sSimpleStartWcmState = 3;
            if (sSimpleStartWcmNotifyCb != 0) {
                sSimpleStartWcmNotifyCb(13, 0);
            }
        }
        return;
    case 4:
        if (p->result == 0) {
            s32 s = sSimpleStartWcmState;
            if (s == 4) {
                sSimpleStartWcmState = 3;
                if (sSimpleStartWcmNotifyCb != 0) {
                    sSimpleStartWcmNotifyCb(10, 0);
                }
            } else if (s == 6) {
                if (WifiLink_SearchAsync(data_ov001_0222c894, data_ov001_0222c898, data_ov001_0222c8b0) != 3) {
                    sSimpleStartWcmState = 3;
                    if (sSimpleStartWcmNotifyCb != 0) {
                        sSimpleStartWcmNotifyCb(2, 0);
                    }
                }
            } else if (s == 2) {
                if (WifiLink_CleanupAsync() != 3) {
                    sSimpleStartWcmState = 3;
                    if (sSimpleStartWcmNotifyCb != 0) {
                        sSimpleStartWcmNotifyCb(2, 0);
                    }
                }
            } else if (s == 8) {
                if (WifiLink_ConnectAsync(data_ov001_0222c89c, data_ov001_0222c878, data_ov001_0222c8a8) != 3) {
                    sSimpleStartWcmState = 3;
                    if (sSimpleStartWcmNotifyCb != 0) {
                        sSimpleStartWcmNotifyCb(2, 0);
                    }
                }
            }
        } else {
            sSimpleStartWcmState = 3;
            if (sSimpleStartWcmNotifyCb != 0) {
                sSimpleStartWcmNotifyCb(11, 0);
            }
        }
        return;
    case 6:
        if (p->result == 0) {
            s32 s = sSimpleStartWcmState;
            if (s == 4) {
                sSimpleStartWcmState = 3;
                if (sSimpleStartWcmNotifyCb != 0) {
                    sSimpleStartWcmNotifyCb(14, 0);
                }
            } else if (s == 6) {
                if (WifiLink_SearchAsync(data_ov001_0222c894, data_ov001_0222c898, data_ov001_0222c8b0) != 3) {
                    sSimpleStartWcmState = 3;
                    if (sSimpleStartWcmNotifyCb != 0) {
                        sSimpleStartWcmNotifyCb(2, 0);
                    }
                }
            } else if (s == 2) {
                if (WifiLink_CleanupAsync() != 3) {
                    sSimpleStartWcmState = 3;
                    if (sSimpleStartWcmNotifyCb != 0) {
                        sSimpleStartWcmNotifyCb(2, 0);
                    }
                }
            } else if (s == 8) {
                if (WifiLink_ConnectAsync(data_ov001_0222c89c, data_ov001_0222c878, data_ov001_0222c8a8) != 3) {
                    sSimpleStartWcmState = 3;
                    if (sSimpleStartWcmNotifyCb != 0) {
                        sSimpleStartWcmNotifyCb(2, 0);
                    }
                }
            } else if (s == 7) {
                sSimpleStartWcmState = 3;
            }
        } else {
            sSimpleStartWcmState = 3;
            if (sSimpleStartWcmNotifyCb != 0) {
                sSimpleStartWcmNotifyCb(15, 0);
            }
        }
        return;
    case 2:
        if (p->result == 0) {
            if (sSimpleStartWcmState == 2) {
                WifiLink_Finish();
                sSimpleStartWcmState = 0;
                if (sSimpleStartWcmNotifyCb != 0) {
                    sSimpleStartWcmNotifyCb(0x14, 0);
                }
            }
        } else {
            sSimpleStartWcmState = 3;
            if (sSimpleStartWcmNotifyCb != 0) {
                sSimpleStartWcmNotifyCb(2, 0);
            }
        }
        return;
    case 7:
        if (sSimpleStartWcmState == 5) {
            if (sSimpleStartWcmNotifyCb != 0) {
                sSimpleStartWcmNotifyCb(5, 0);
            }
        }
        return;
    case 8:
        if (sSimpleStartWcmNotifyCb != 0) {
            sSimpleStartWcmNotifyCb(4, 0);
        }
        return;
    case 9:
        sSimpleStartWcmState = 0;
        if (sSimpleStartWcmNotifyCb != 0) {
            sSimpleStartWcmNotifyCb(3, 0);
        }
        return;
    default:
        if (sSimpleStartWcmNotifyCb != 0) {
            sSimpleStartWcmNotifyCb(1, 0);
        }
        return;
    }
}

s32 SimpleStart_WcmToIdle(void) {
    switch (sSimpleStartWcmState) {
    case 5:
        if (WifiLink_SearchAsync(0, 0, 0) != 3) {
            return 0;
        }
        break;
    case 7:
        if (WifiLink_DisconnectAsync() != 3) {
            return 0;
        }
        break;
    case 1:
        if (WifiLink_StartupAsync(data_ov001_0222c858, SimpleStart_WcmCallback) != 3) {
            return 0;
        }
        break;
    case 0:
    case 2:
    case 3:
    case 4:
    case 6:
    case 8:
    default:
        return 0;
    }
    return 1;
}

s32 SimpleStart_WcmReadApList(u8 *buf, s32 n) {
    s32 cnt;
    s32 i;
    WifiLink_LockApList(1);
    cnt = WifiLink_GetApListCount();
    if (cnt > 0) {
        for (i = 0; i < cnt; i++, buf += 0xc0) {
            if (i >= n) {
                break;
            }
            MIi_CpuCopy32(WifiLink_GetApListEntry((u16)i), buf, 0xc0);
        }
    }
    WifiLink_LockApList(0);
    return cnt;
}

s32 SimpleStart_WcmStartSearch(u8 *a, u8 *b, s32 c, s32 d) {
    s32 e = OS_DisableInterrupts();
    u8 *p;
    s32 i;
    data_ov001_0222c8b0 = d;
    p = data_ov001_0222c8e4;
    data_ov001_0222c894 = p;
    if (a != 0) {
        i = 0;
        do {
            *p++ = *a++;
            i++;
        } while (i < 6);
    } else {
        MI_CpuFill8(p, 0xff, 6);
        data_ov001_0222c894 = gWifiLinkAnyBssid;
    }
    p = data_ov001_0222c944;
    data_ov001_0222c898 = p;
    if (b != 0 && c > 0 && c < 0x20) {
        i = 0;
        if (c > 0) {
            do {
                *p++ = *b++;
                i++;
            } while (i < c);
        }
        if (i < 0x20) {
            p = data_ov001_0222c944 + i;
            do {
                *p++ = 0;
                i++;
            } while (i < 0x20);
        }
    } else {
        MI_CpuFill8(data_ov001_0222c944, 0xff, 0x20);
        data_ov001_0222c898 = gWifiLinkAnySsid;
    }
    if (sSimpleStartWcmState == 3) {
        if (WifiLink_SearchAsync(data_ov001_0222c8e4, data_ov001_0222c898, data_ov001_0222c8b0) == 3) {
            sSimpleStartWcmState = 6;
            OS_RestoreInterrupts(e);
            return 1;
        }
    } else if (SimpleStart_WcmToIdle() == 1) {
        sSimpleStartWcmState = 6;
        OS_RestoreInterrupts(e);
        return 1;
    }
    OS_RestoreInterrupts(e);
    return 0;
}

s32 SimpleStart_WcmDisconnect(void) {
    s32 e = OS_DisableInterrupts();
    if (sSimpleStartWcmState == 7) {
        if (WifiLink_DisconnectAsync() == 3) {
            sSimpleStartWcmState = 4;
            OS_RestoreInterrupts(e);
            return 1;
        }
    }
    OS_RestoreInterrupts(e);
    return 0;
}

s32 SimpleStart_WcmCleanup(void) {
    s32 e = OS_DisableInterrupts();
    if (sSimpleStartWcmState == 3) {
        if (WifiLink_CleanupAsync() != 3) {
            OS_RestoreInterrupts(e);
            return 0;
        }
        sSimpleStartWcmState = 2;
        OS_RestoreInterrupts(e);
        return 1;
    }
    if (SimpleStart_WcmToIdle() == 1) {
        sSimpleStartWcmState = 2;
        OS_RestoreInterrupts(e);
        return 1;
    }
    OS_RestoreInterrupts(e);
    return 0;
}

s32 SimpleStart_WcmConnect(void *p, void *x, s32 y) {
    s32 e = OS_DisableInterrupts();
    data_ov001_0222c8a8 = y;
    if (x != 0) {
        MI_CpuCopy8(x, data_ov001_0222c878, 0x60);
    } else {
        MI_CpuFill8(data_ov001_0222c878, 0, 0x60);
    }
    MIi_CpuCopy32(p, data_ov001_0222c89c, 0xc0);
    if (SimpleStart_WcmToIdle() == 1) {
        sSimpleStartWcmState = 8;
        OS_RestoreInterrupts(e);
        return 1;
    }
    if (sSimpleStartWcmState == 3) {
        if (WifiLink_ConnectAsync(data_ov001_0222c89c, data_ov001_0222c878, data_ov001_0222c8a8) == 3) {
            sSimpleStartWcmState = 8;
            OS_RestoreInterrupts(e);
            return 1;
        }
    }
    OS_RestoreInterrupts(e);
    return 0;
}

// the work area: 0x60 bytes of connection data, the WifiLink configuration, WifiLink's 0x2300-byte work (32-byte
// aligned), the connect target's BSS descriptor (0xc0) and the rest for the BSS descriptor buffer
s32 SimpleStart_WcmInit(SimpleStartWcmNotifyFunc cb, u8 *buf, s32 size) {
    s32 e = OS_DisableInterrupts();
    uptr a, b;
    data_ov001_0222c878 = buf;
    a = ((uptr)buf + 0x63) & ~(uptr)3;
    data_ov001_0222c858 = (WifiLinkConfig *)a;
    b = (a + 0x2f) & ~(uptr)0x1f;
    data_ov001_0222c88c = (u8 *)b;
    b = (b + WIFI_LINK_WORK_SIZE + 0x1f) & ~(uptr)0x1f;
    data_ov001_0222c89c = (u8 *)b;
    b += 0xdf;
    b &= ~(uptr)0x1f;
    data_ov001_0222c858->bssDescBuffer = (void *)b;
    data_ov001_0222c858->bssDescBufferSize = (buf + size) - (u8 *)b;
    data_ov001_0222c858->bssDescMode = 0;
    data_ov001_0222c858->dmaNo = 3;
    sSimpleStartWcmNotifyCb = cb;
    if (sSimpleStartWcmState == 0) {
        if (WifiLink_Init(data_ov001_0222c88c, WIFI_LINK_WORK_SIZE) != 0) {
            OS_RestoreInterrupts(e);
            return 0;
        }
        sSimpleStartWcmState = 1;
    }
    if (sSimpleStartWcmState == 1) {
        if (WifiLink_StartupAsync(data_ov001_0222c858, SimpleStart_WcmCallback) != 3) {
            OS_RestoreInterrupts(e);
            return 0;
        }
        sSimpleStartWcmState = 4;
        OS_RestoreInterrupts(e);
        return 1;
    }
    OS_RestoreInterrupts(e);
    return 0;
}

s32 SimpleStart_WcmGetState(void) {
    return sSimpleStartWcmState;
}

void *SimpleStart_SocAlloc(s32 a, u32 size) {
    s32 n = size;
    void *r;
    if (n > 0) {
        OS_LockMutex(&sSimpleStartAllocMutex);
        r = sSimpleStartAllocFunc(n);
        OS_UnlockMutex(&sSimpleStartAllocMutex);
        return r;
    }
    return 0;
}

void SimpleStart_SocFree(s32 a, void *p, u32 size) {
    s32 n = size;
    if (p != 0 && n > 0) {
        OS_LockMutex(&sSimpleStartAllocMutex);
        sSimpleStartFreeFunc(p);
        OS_UnlockMutex(&sSimpleStartAllocMutex);
    }
}

void SimpleStart_ClearMsgQueue(void) {
    s32 i;
    u32 *p;
    s32 e = OS_DisableInterrupts();
    sSimpleStartMsgTail = 0;
    sSimpleStartMsgHead = 0;
    p = sSimpleStartMsgQueue;
    for (i = 0; i < 4; i++) {
        *p++ = 0;
    }
    OS_RestoreInterrupts(e);
}

s32 SimpleStart_PopMsg(void) {
    s32 e = OS_DisableInterrupts();
    s32 head = sSimpleStartMsgHead;
    s32 r;
    if (sSimpleStartMsgTail == head) {
        r = 0;
    } else {
        r = sSimpleStartMsgQueue[head];
        head++;
        sSimpleStartMsgHead = head;
        if (head >= 4) {
            sSimpleStartMsgHead = 0;
        }
    }
    OS_RestoreInterrupts(e);
    return r;
}

void SimpleStart_PushMsg(u32 v, u32 w) {
    u32 head = sSimpleStartMsgHead;
    u32 tail = sSimpleStartMsgTail;
    u32 nt = tail + 1;
    if (nt == head || tail == head + 3) {
        return;
    }
    sSimpleStartMsgQueue[tail] = v;
    sSimpleStartMsgTail = nt;
    if ((s32)nt >= 4) {
        sSimpleStartMsgTail = 0;
    }
}

// (the alarm's argument is the message to push)
void SimpleStart_AlarmCallback(void *arg) {
    SimpleStart_PushMsg((u32)(uptr)arg, 0);
}

s32 SimpleStart_WlanStartup(s32 n) {
    s32 cont = 1;
    s32 res;
    u32 o;
    u8 *q;
    u8 *r;
    sSimpleStartMaxAps = n;
    SimpleStart_ClearMsgQueue();
    o = n * 0xd0;
    q = (u8 *)sSimpleStartAllocFunc(o + 0x24d0 + n * 0xc0);
    data_ov001_0222c8a4 = q;
    if (q == 0) {
        return -1;
    }
    Unk_ov001_02206ef8_Mask m = UNK_OV001_02206EF8_MASK;
    u8 *al = (u8 *)(((uptr)q + m) & ~(uptr)(0x20 - 1));
    data_ov001_0222c890 = al;
    u32 t = o + 0x2490;
    u8 *s2 = al + t;
    r = (u8 *)(((uptr)s2 + m) & ~(uptr)(0x20 - 1));
    data_ov001_0222c8ac = r;
    if (SimpleStart_WcmInit(SimpleStart_PushMsg, al, t) == 0) {
        return -2;
    }
    do {
        OS_Sleep(10);
        s32 m = SimpleStart_PopMsg();
        if (m != 0) {
            do {
                if (m == 4 || m == 5) {
                } else if (m == 6) {
                    cont = 0;
                    res = 1;
                } else {
                    cont = 0;
                    res = -2;
                }
                m = SimpleStart_PopMsg();
            } while (m != 0);
        }
    } while (cont != 0);
    return res;
}

s32 SimpleStart_WlanShutdown(void) {
    s32 cont = 1;
    if (SimpleStart_WcmCleanup() != 0) {
        do {
            OS_Sleep(10);
            s32 m = SimpleStart_PopMsg();
            if (m != 0) {
                do {
                    switch (m) {
                    case 4:
                    case 5:
                        break;
                    case 20:
                        cont = 0;
                        break;
                    default:
                        cont = 0;
                        break;
                    }
                    m = SimpleStart_PopMsg();
                } while (m != 0);
            }
        } while (cont != 0);
    }
    if (data_ov001_0222c8a4 != 0) {
        sSimpleStartFreeFunc(data_ov001_0222c8a4);
        data_ov001_0222c8a4 = 0;
    }
    return 1;
}

s32 SimpleStart_ConnectAp(void) {
    s32 cont = 1;
    s32 res = -2;
    u8 *p = data_ov001_0222c8ac + data_ov001_0222c868 * 0xc0;
    OSAlarm alarm;
    if (p == 0) {
        return 0;
    }
    if (SimpleStart_WcmConnect(p, 0, 0x30000) == 0) {
        return -2;
    }
    OS_CreateAlarm(&alarm);
    OS_SetAlarm(&alarm, 0x3fec42, SimpleStart_AlarmCallback, (void *)0x12);
    s32 z = 0;
    s32 k = -8;
    do {
        if ((u32)SimpleStart_GetMilliseconds() >= sSimpleStartDeadline) {
            res = -3;
            break;
        }
        if (sSimpleStartCancel != 0) {
            res = -8;
            break;
        }
        OS_Sleep(10);
        s32 m = SimpleStart_PopMsg();
        if (m != 0) {
            do {
                switch (m) {
                case 12:
                    cont = 0;
                    res = 1;
                    break;
                case 4:
                case 5:
                case 18:
                case 19:
                    break;
                case 13:
                    if (sSimpleStartCancel != 0) {
                        cont = z;
                        res = k;
                    } else if (SimpleStart_WcmConnect(p, 0, 0x30000) == 0) {
                        return res;
                    }
                    break;
                default:
                    cont = z;
                    break;
                }
                m = SimpleStart_PopMsg();
            } while (m != 0);
        }
    } while (cont != 0);
    OS_CancelAlarm(&alarm);
    while (SimpleStart_PopMsg() != 0) {
    }
    if (res > 0) {
        data_ov001_0222c870 = 1;
        if (Shared_Sock_Startup(&sSimpleStartSocConfig) < 0) {
            res = -2;
        } else {
            data_ov001_0222c874 = 1;
        }
    }
    return res;
}

void SimpleStart_Disconnect(void) {
    s32 cont = 1;
    s32 m;
    if (data_ov001_0222c870 != 0) {
        if (SimpleStart_WcmDisconnect() != 0) {
            do {
                OS_Sleep(10);
                m = SimpleStart_PopMsg();
                if (m != 0) {
                    do {
                        switch (m) {
                        case 4:
                        case 5:
                            break;
                        case 14:
                            cont = 0;
                            break;
                        default:
                            cont = 0;
                            break;
                        }
                        m = SimpleStart_PopMsg();
                    } while (m != 0);
                }
            } while (cont != 0);
        }
        data_ov001_0222c870 = 0;
    }
    if (data_ov001_0222c874 != 0) {
        data_ov001_0222c874 = 0;
        Sock_Cleanup();
    }
}

BOOL SimpleStart_FindSetupAp(SimpleStartApList *a, SimpleStartApList *b, s32 *out) {
    BOOL found = FALSE;
    BOOL ret = FALSE;
    BOOL flagA;
    BOOL flagB;
    SimpleStartApEntry *e1 = a->e;
    SimpleStartApEntry *e2 = b->e;
    u32 cnt;
    u32 i = found;
    if (a->count != 0) {
        do {
            u8 buf[0x22] = {0};
            memcpy(buf, e1->name, 0x20);
            buf[e1->len] = 0;
            u32 j = 0;
            cnt = b->count;
            if (cnt != 0) {
                u32 len = e1->len;
                do {
                    if (len == 0 || len > 0x20) break;
                    if (len == 1) {
                        u32 c = e1->name[0];
                        if (c == 0 || c == 0x20) break;
                    }
                    u32 n = strlen((const char *)buf);
                    if (memcmp(buf, e2->name, n) == 0 && memcmp(e1->bssid, e2->bssid, 4) == 0 &&
                        e1->privacy != e2->privacy && e1->privacy == 0) {
                        found = TRUE;
                        break;
                    }
                    e2++;
                    j++;
                } while (j < cnt);
            }
            if (found != 0) break;
            e1++;
            e2 = b->e;
            i++;
        } while (i < a->count);
    }
    if (found == 0) {
        u8 buf2[0x22] = {0};
        flagB = flagA = FALSE;
        e1 = a->e;
        e2 = b->e;
        u32 m = flagA;
        if (b->count != 0) {
            do {
                memcpy(buf2, e2->name, 0x20);
                buf2[e2->len] = flagA;
                u32 n = strlen((const char *)sSimpleStartSetupSsid);
                if (memcmp(buf2, sSimpleStartSetupSsid, n) == 0 && e2->privacy == 0) {
                    flagB = TRUE;
                    break;
                }
                e2++;
                m++;
            } while (m < b->count);
        }
        i = 0;
        u32 zz = i;
        if (a->count != 0) {
            do {
                memcpy(buf2, e1->name, 0x20);
                buf2[e1->len] = zz;
                u32 x = strlen((const char *)buf2);
                u32 y = strlen((const char *)sSimpleStartSetupSsid);
                if (x == y) {
                    u32 n = strlen((const char *)sSimpleStartSetupSsid);
                    if (memcmp(buf2, sSimpleStartSetupSsid, n) == 0 && e1->privacy == 0) {
                        flagA = TRUE;
                        break;
                    }
                }
                e1++;
                i++;
            } while (i < a->count);
        }
        if (flagA != 0 && flagB == 0) found = TRUE;
    }
    if (found != 0) {
        *out = i;
        ret = TRUE;
    }
    return ret;
}

s32 SimpleStart_SearchAp() {
    s32 result = -1;
    s32 iter;
    s32 i;
    s32 j;
    u32 size;
    SimpleStartApList *buf1;
    SimpleStartApList *buf2 = 0;
    WMBssDesc *src;
    s32 idx = 0;
    u8 unkbuf[0x20];
    OSAlarm alarm;
    s32 cont;
    s32 t;
    s32 sc;

    size = sSimpleStartMaxAps * 0x30 + 0x34;
    buf1 = (SimpleStartApList *)SimpleStart_Calloc(1, size);
    if (buf1 == 0) goto cleanup;
    buf2 = (SimpleStartApList *)SimpleStart_Calloc(1, size);
    if (buf2 == 0) goto cleanup;
    for (iter = 0; iter < 0x1e && sSimpleStartCancel == 0; iter++) {
        if (SimpleStart_GetMilliseconds() >= sSimpleStartDeadline) break;
        if (SimpleStart_WcmStartSearch(0, 0, 0, 0x30bffe) == 0) {
            result = -2;
            goto cleanup;
        }
        OS_CreateAlarm(&alarm);
        OS_SetAlarm(&alarm, 0xffb10, SimpleStart_AlarmCallback, (void *)0x13);
        cont = 1;
        result = 0;
        do {
            OS_Sleep(10);
            if (SimpleStart_GetMilliseconds() >= sSimpleStartDeadline) break;
            if (sSimpleStartCancel != 0) break;
            while ((t = SimpleStart_PopMsg()) != 0) {
                switch (t) {
                case 0x13:
                    cont = 0;
                    break;
                case 5:
                    sc = SimpleStart_WcmReadApList(data_ov001_0222c8ac, sSimpleStartMaxAps);
                    if (sc > result) {
                        result = sc;
                        OS_CancelAlarm(&alarm);
                        OS_SetAlarm(&alarm, 0xffb10, SimpleStart_AlarmCallback, (void *)0x13);
                    }
                    break;
                case 10:
                    cont = 0;
                    break;
                case 0: case 1: case 2: case 3: case 6: case 7: case 9:
                case 11: case 12: case 13: case 14: case 15: case 16: case 17:
                default:
                    cont = 0;
                    break;
                case 4: case 8: case 0x12:
                    break;
                }
            }
        } while (cont != 0);
        OS_CancelAlarm(&alarm);
        while (SimpleStart_PopMsg() != 0) {}
        if (sSimpleStartCancel != 0) break;
        j = 0;
        if (result >= sSimpleStartMaxAps) {
            result = -6;
            goto cleanup;
        }
        for (i = 0, src = (WMBssDesc *)data_ov001_0222c8ac; i < result; i++, src++) {
            memcpy(buf1->e[j].name, src->ssid, 0x20);
            buf1->e[j].len = src->ssidLength;
            buf1->e[j].name[src->ssidLength] = 0;
            buf1->e[j].privacy = (src->capaInfo & 0x10) ? 1 : 0;
            {
                const u8 *ms = src->bssid;
                u8 *md = buf1->e[j].bssid;
                md[0] = ms[0]; md[1] = ms[1]; md[2] = ms[2]; md[3] = ms[3]; md[4] = ms[4]; md[5] = ms[5];
            }
            j++;
        }
        buf1->count = result;
        if (sSimpleStartState != 1) {
            if (SimpleStart_FindSetupAp(buf1, buf2, &idx) != 0) {
                SimpleStartApEntry *e = &buf1->e[idx];
                data_ov001_0222c868 = idx;
                strcpy((char *)data_ov001_0222c964, (const char *)e->name);
                {
                    u8 *md = data_ov001_0222c8dc;
                    const u8 *ms = e->bssid;
                    md[0] = ms[0]; md[1] = ms[1]; md[2] = ms[2]; md[3] = ms[3]; md[4] = ms[4]; md[5] = ms[5];
                }
                SimpleStart_FormatMac((char *)unkbuf, (s8 *)data_ov001_0222c8dc);
                break;
            }
        }
        memcpy(buf2, buf1, size);
        sSimpleStartState = 2;
        SimpleStart_NotifyStatus();
    }
    if (iter >= 0x1e || SimpleStart_GetMilliseconds() > sSimpleStartDeadline) {
        result = -3;
    } else if (sSimpleStartCancel != 0) {
        result = -8;
    } else {
        result = 1;
    }
cleanup:
    if (buf1 != 0) SimpleStart_Free((u8 *)buf1);
    if (buf2 != 0) SimpleStart_Free((u8 *)buf2);
    return result;
}

s32 SimpleStart_SendTo(sptr a, SockAddrIn *hdr, u8 *out, u8 *d, u32 e) {
    s32 r = Sock_SendTo(a, d, e, 0, hdr);
    if (r < 0) r = -4;
    return r;
}

s32 SimpleStart_SendBroadcast(sptr a, u8 *b, u32 c) {
    SockAddrIn h;
    u8 out[4];
    h.len = 8;
    h.family = 2;
    h.addr = -1;
    h.port = 0x1e6;
    IpAddr_StoreBe32(SockCore_GetHostIp(), out);
    return SimpleStart_SendTo(a, &h, out, b, c);
}

s32 SimpleStart_SendPacket(sptr a, SockAddrIn *b, u8 *c, u32 d) {
    return SimpleStart_SendBroadcast(a, c, d);
}

u8 *SimpleStart_CheckPacket(u8 *pkt, s32 *type, s32 *len) {
    u32 sum = 0;
    *type = Bswap(*(u16 *)pkt);
    *len = Bswap(*(u16 *)(pkt + 2));
    u8 *end = pkt + 6 + *len;
    u8 *p;
    for (p = pkt; p < end; p++) sum += *p;
    if ((u16)sum != Bswap(*(u16 *)end)) return 0;
    return pkt + 6;
}

u8 *SimpleStart_NextTlv(u8 **cur, u8 *end, s32 *type, s32 *len) {
    u8 *p = *cur;
    if (p >= end) return 0;
    *type = Bswap(*(u16 *)p);
    *len = Bswap(*(u16 *)(p + 2));
    p += 4;
    *cur = p + ((((*len) + 11) & ~7) - 4);
    return p;
}

u8 *SimpleStart_FirstTlv(u8 *pkt, s32 *a, s32 *b) {
    u8 *cur = pkt + 8;
    return SimpleStart_NextTlv(&cur, cur + Bswap(*(u16 *)pkt), a, b);
}

s32 SimpleStart_BuildPacket(u8 *pkt, u32 type, u8 *hdr, u32 len, u8 *extra) {
    u8 *q = pkt;
    u32 sum = 0;
    hdr[0] = 0; hdr[1] = 0; hdr[2] = 0; hdr[3] = 0; hdr[4] = 0; hdr[5] = 0; hdr[6] = 0; hdr[7] = 0;
    *(u16 *)hdr = Bswap((u16)(len - 8));
    if (extra != 0) {
        SimpleStart_AesKeyWrap(pkt + 6, hdr, len, extra, 0x10);
        len += 8;
    } else {
        memcpy(pkt + 6, hdr, len);
    }
    pkt[0] = 0; pkt[1] = 0; pkt[2] = 0; pkt[3] = 0; pkt[4] = 0; pkt[5] = 0;
    *(u16 *)pkt = Bswap((u16)type);
    *(u16 *)(pkt + 2) = Bswap((u16)len);
    q = q + 6;
    q = q + len;
    u8 *p;
    for (p = pkt; p < q; p++) sum += *p;
    *(u16 *)q = Bswap((u16)sum);
    return (s32)(q + 2 - pkt);
}

u8 *SimpleStart_PutTlv(u8 *out, u32 id0, u8 *data, u32 len) {
    u32 pad;
    out[0] = 0; out[1] = 0; out[2] = 0; out[3] = 0;
    *(u16 *)out = Bswap(id0);
    pad = ((len + 11) & ~7) - 4;
    *(u16 *)(out + 2) = Bswap((u16)len);
    out += 4;
    memset(out, 0, pad);
    memcpy(out, data, len);
    out += pad;
    return out;
}

s32 SimpleStart_BuildTlvBlock(u8 *p, u32 id, u8 *data, u32 len) {
    p[0] = 0; p[1] = 0; p[2] = 0; p[3] = 0; p[4] = 0; p[5] = 0; p[6] = 0; p[7] = 0;
    s32 t = (s32)(SimpleStart_PutTlv(p + 8, id, data, len) - p);
    *(u16 *)p = (u16)(t - 8);
    return t;
}

BOOL SimpleStart_ParseHelloReply(u8 *pkt, s32 *out) {
    volatile u32 v2, v3;
    s32 type, len, t, l;
    u8 *cur;
    u8 *r = SimpleStart_CheckPacket(pkt, &type, &len);
    cur = r;
    u32 v1 = 0;
    v2 = 0;
    v3 = 0;
    if (r == 0) return v1;
    if (type != 1) return v1;
    u8 *end = r + len;
    cur = r + 8;
    u8 *p = SimpleStart_NextTlv(&cur, end, &t, &l);
    if (p != 0) {
        do {
            switch (t) {
            case 1:
                v1 = Bswap(*(u16 *)p);
                break;
            case 2:
                v2 = Bswap(*(u16 *)p);
                break;
            case 5:
                v3 = Bswap(*(u16 *)p);
                break;
            }
            p = SimpleStart_NextTlv(&cur, end, &t, &l);
        } while (p != 0);
    }
    if (v1 != 1 || v2 != 1) {
        return FALSE;
    }
    if ((s32)v3 >= 1) *out = 1; else *out = 0;
    return TRUE;
}

s32 SimpleStart_RecvPacket(u8 *pkt, s32 type, u8 *dst, u8 *extra) {
    s32 t, n;
    u8 *r = SimpleStart_CheckPacket(pkt, &t, &n);
    if (r == 0) return 0;
    if (t != type) return 0;
    if (extra != 0) {
        SimpleStart_AesKeyUnwrap(dst, r, n, extra, 0x10);
        n -= 8;
    } else {
        memcpy(dst, r, n);
    }
    return n;
}

u32 SimpleStart_BuildHelloPacket(u8 *pkt) {
    u8 t[9];
    *(u16 *)t = 0x100;
    *(SimpleStartSecurityList *)(t + 2) = *(SimpleStartSecurityList *)data_ov001_0222a548;
    u8 *r = SimpleStart_PutTlv(data_ov001_0222a52c, 1, t, 2);
    r = SimpleStart_PutTlv(r, 2, t, 2);
    if (data_ov001_0222c8a0 != 0) {
        r = SimpleStart_PutTlv(r, 5, t, 2);
    }
    r = SimpleStart_PutTlv(r, 3, t + 2, 7);
    if (data_ov001_0222c8a0 != 0) {
        r = SimpleStart_PutTlv(r, 4, data_ov001_0222c8d4, 6);
    }
    return SimpleStart_BuildPacket(pkt, 2, sSimpleStartTlvBuf, r - data_ov001_0222a52c + 8, 0);
}

BOOL SimpleStart_GetOwnMac(u8 *mac) {
    OS_GetMacAddress(mac);
    return TRUE;
}

s32 SimpleStart_BuildSessionId(SimpleStartSessionId *out, void *unused)
{
    u8 m0[6];
    u8 m1[6];
    char s0[0x20];
    char s1[0x20];
    CP4(out->tag, "WARP");
    CP6(m0, data_ov001_0222c8dc);
    m0[0] &= 0xfd;
    SimpleStart_GetOwnMac(m1);
    CP6(data_ov001_0222c8d4, m1);
    if (memcmp(m0, m1, 6) <= 0) {
        CP6(out->higherMac, m1);
        CP6(out->lowerMac, m0);
    } else {
        CP6(out->higherMac, m0);
        CP6(out->lowerMac, m1);
    }
    if (data_ov001_0222a530 != 0) {
        SimpleStart_FormatMac(s0, (s8 *)m1);
        SimpleStart_FormatMac(s1, (s8 *)m0);
    }
    return 1;
}

s32 SimpleStart_ParseConfigTlvs(u8 *p)
{
    u8 *end;
    s32 res;
    u8 *cur;
    u32 type, len;
    u8 *r;
    s32 i;
    cur = p + 8;
    res = 0;
    end = cur + Unk_ov001_0220607c_Swap(p);
    while ((r = (u8 *)SimpleStart_NextTlv(&cur, end, (s32 *)&type, (s32 *)&len)) != 0) {
        switch (type) {
        case 0x201:
            __builtin__clear(&data_ov001_0222cc30, 0x20);
            memcpy(&data_ov001_0222cc30, r, len);
            res = 1;
            break;
        case 0x202:
            data_ov001_0222cc30.securityType = Unk_ov001_0220607c_Swap(r);
            break;
        case 0x203: {
            u32 v = Unk_ov001_0220607c_Swap(r);
            u8 *q;
            for (i = 0, q = (u8 *)data_ov001_0222cb30; i < 4; q += 0x28, i++) *(u32 *)(q + 0x15c) = v;
            break;
        }
        case 0x204: {
            u32 v = Unk_ov001_0220607c_Swap(r);
            u8 *q;
            for (i = 0, q = (u8 *)data_ov001_0222cb30; i < 4; q += 0x28, i++) *(u32 *)(q + 0x160) = v;
            break;
        }
        case 0x205:
            data_ov001_0222cc30.unk_30 = Unk_ov001_0220607c_Swap(r);
            break;
        case 0x206: case 0x207: case 0x208: case 0x209: {
            memset(data_ov001_0222cc94 + (type - 0x206) * 0x28, 0, 0x20);
            if (data_ov001_0222cc30.unk_5c == 1) {
                u8 *dd = data_ov001_0222cc94 + (type - 0x206) * 0x28;
                for (i = 0; i < (s32)len; i++) {
                    dd += SimpleStart_FormatHexByte((char *)dd, *(s8 *)r++);
                }
            } else {
                memcpy(data_ov001_0222cc94 + (type - 0x206) * 0x28, r, len);
            }
            break;
        }
        case 0x20a:
            __builtin__clear(&data_ov001_0222cd2c, 0x48);
            memcpy(&data_ov001_0222cd2c, r, len);
            break;
        }
    }
    return res;
}

s32 SimpleStart_ParseHex(u8 *dst, s8 *src, s32 n)
{
    s32 acc = 0;
    s32 i;
    for (i = 0; i < n; i++) {
        s32 c = src[i];
        switch (c) {
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
            acc += c - '0';
            break;
        case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
            acc += c - 'a' + 10;
            break;
        case 'A': case 'B': case 'C': case 'D': case 'E': case 'F':
            acc += c - 'A' + 10;
            break;
        default:
            return 0;
        }
        if (i % 2 == 0) {
            acc <<= 4;
        } else {
            dst[i / 2] = acc;
            acc = 0;
        }
    }
    return 1;
}

s32 SimpleStart_BuildResult()
{
    s32 ret = 1;

    strcpy((char *)&CA48_A, (const char *)&data_ov001_0222cc30);
    switch (data_ov001_0222cc30.securityType) {
    case 0:
        CA48_A.securityType = 0;
        break;
    case 1: {
        if (data_ov001_0222cc30.unk_30 == 0) {
            ret = -7;
            break;
        }
        SimpleStartResult *a = &CA48_A;
        a->unk_24 = data_ov001_0222cc30.unk_30;
        s32 i = 0;
        {
            u8 buf[0x21];
            u8 *bp = buf;
            u8 *src = data_ov001_0222cc94;
            u8 *dst = data_ov001_0222ca70;
            for (; i < 4; i++) {
                memcpy(bp, src, 0x20);
                buf[0x20] = 0;
                switch (strlen((const char *)bp)) {
                case 5:
                    a->securityType = 1;
                    *(SimpleStartWep40Key *)dst = *(SimpleStartWep40Key *)bp;
                    break;
                case 10:
                    a->securityType = 1;
                    SimpleStart_ParseHex(dst, (s8 *)bp, 10);
                    break;
                case 13:
                    a->securityType = 2;
                    *(SimpleStartWep104Key *)dst = *(SimpleStartWep104Key *)bp;
                    break;
                case 26:
                    a->securityType = 2;
                    SimpleStart_ParseHex(dst, (s8 *)bp, 26);
                    break;
                case 16:
                    a->securityType = 3;
                    *(SimpleStartWep128Key *)dst = *(SimpleStartWep128Key *)bp;
                    break;
                case 32:
                    a->securityType = 3;
                    SimpleStart_ParseHex(dst, (s8 *)bp, 32);
                    break;
                case 0:
                    break;
                default:
                    ret = -7;
                    break;
                }
                src += 0x28;
                dst += 0x20;
            }
        }
        break;
    }
    case 2:
        CA48_A.securityType = 4;
        *(ret ? &data_ov001_0222caf0 : &data_ov001_0222caf0) = data_ov001_0222cd2c;
        break;
    case 3:
        CA48_A.securityType = 5;
        *(ret ? &data_ov001_0222caf0 : &data_ov001_0222caf0) = data_ov001_0222cd2c;
        break;
    default:
        ret = -7;
        break;
    }
    return ret;
}

s32 SimpleStart_RunProtocol()
{
    sptr h = 0;
    s32 result = -5;
    u32 t40 = 0;
    s32 retries = 0;
    s32 done = 0;
    SimpleStartProtocolLocals l;
    sSimpleStartStep = 1;
    while (done == 0 && sSimpleStartCancel == 0) {
        OS_Sleep(500);
        switch (sSimpleStartStep) {
        case 0:
            break;
        case 1:
            result = SimpleStart_SearchAp();
            if (result != 1) {
                done = 1;
                break;
            }
            sSimpleStartState = 3;
            SimpleStart_NotifyStatus();
            sSimpleStartStep = 2;
            break;
        case 2:
            result = SimpleStart_ConnectAp();
            if (result != 1) {
                done = 1;
                break;
            }
            sSimpleStartStep = 3;
            break;
        case 3:
            h = Sock_Create(2, 2);
            if (h < 0) {
                result = -2;
                done = 1;
                break;
            }
            __builtin__clear(&l.req, 8);
            l.req.len = 8;
            l.req.family = 2;
            l.req.port = 0x1e6;
            l.req.addr = 0;
            result = Shared_Sock_Bind(h, &l.req);
            if (result < 0) {
                result = -2;
                done = 1;
                break;
            }
            sSimpleStartStep = 4;
            break;
        case 4:
            if (SimpleStart_GetMilliseconds() >= sSimpleStartDeadline) {
                Sock_Close(h);
                result = -3;
                done = 1;
                break;
            }
            l.from.len = 8;
            SimpleStart_BuildSessionId((SimpleStartSessionId *)data_ov001_0222c8fc, &l.from);
            if (Sock_RecvFrom(h, sSimpleStartPacketBuf, 0x800, 4, &l.from) > 0) {
                if (SimpleStart_ParseHelloReply(sSimpleStartPacketBuf, &data_ov001_0222c8a0) != 0) {
                    sSimpleStartDeadline = SimpleStart_GetMilliseconds() + 30000;
                    sSimpleStartStep = 5;
                    sSimpleStartState = 4;
                    SimpleStart_NotifyStatus();
                }
            }
            break;
        case 5:
            data_ov001_0222c864 = SimpleStart_BuildHelloPacket(sSimpleStartPacketBuf);
            SimpleStart_SendPacket(h, &l.from, sSimpleStartPacketBuf, data_ov001_0222c864);
            t40 = SimpleStart_GetMilliseconds();
            sSimpleStartStep = 6;
            break;
        case 6:
            if (SimpleStart_GetMilliseconds() >= sSimpleStartDeadline) {
                Sock_Close(h);
                result = -4;
                done = 1;
                break;
            }
            if (Sock_RecvFrom(h, sSimpleStartPacketBuf, 0x800, 4, &l.from) > 0
                && SimpleStart_RecvPacket(sSimpleStartPacketBuf, 3, sSimpleStartTlvBuf, data_ov001_0222c8fc) != 0) {
                u8 *q = SimpleStart_FirstTlv(sSimpleStartTlvBuf, (s32 *)&l.w54, (s32 *)&l.w58);
                if (l.w54 != 0x101) break;
                l.w5c = SimpleStart_GetMilliseconds();
                C924_B8 = *(SimpleStartExchangeKey *)q;
                SimpleStart_Md5(data_ov001_0222c92c, &l.w5c, 4);
                retries = 0;
                sSimpleStartStep = 7;
                sSimpleStartState = 5;
                sSimpleStartDeadline = -1;
                SimpleStart_NotifyStatus();
                break;
            }
            if (SimpleStart_GetMilliseconds() >= t40 + 1000) sSimpleStartStep = 5;
            break;
        case 7:
            data_ov001_0222c884 = SimpleStart_BuildTlvBlock(sSimpleStartTlvBuf, 0x102, data_ov001_0222c92c, 8);
            data_ov001_0222c864 = SimpleStart_BuildPacket(sSimpleStartPacketBuf, 4, sSimpleStartTlvBuf, data_ov001_0222c884, data_ov001_0222c8fc);
            SimpleStart_SendPacket(h, &l.from, sSimpleStartPacketBuf, data_ov001_0222c864);
            t40 = SimpleStart_GetMilliseconds();
            __builtin__clear(data_ov001_0222cb30, 0x254);
            sSimpleStartStep = 8;
            break;
        case 8:
            if (Sock_RecvFrom(h, sSimpleStartPacketBuf, 0x800, 4, &l.from) > 0) {
                data_ov001_0222c884 = SimpleStart_RecvPacket(sSimpleStartPacketBuf, 5, sSimpleStartTlvBuf, C924_B8.v);
                if (data_ov001_0222c884 != 0 && SimpleStart_ParseConfigTlvs(sSimpleStartTlvBuf) != 0) {
                    if (*(s8 *)&data_ov001_0222cc30 != 0) data_ov001_0222c850 = 1;
                    else data_ov001_0222c850 = 0;
                    retries = 0;
                    sSimpleStartStep = 9;
                    break;
                }
            }
            if (SimpleStart_GetMilliseconds() >= t40 + 1000) {
                retries++;
                if (retries >= 10) {
                    Sock_Close(h);
                    result = -2;
                    done = 1;
                } else {
                    sSimpleStartStep = 7;
                }
            }
            break;
        case 9:
            data_ov001_0222c884 = SimpleStart_BuildTlvBlock(sSimpleStartTlvBuf, 0x301, &data_ov001_0222c850, 1);
            data_ov001_0222c864 = SimpleStart_BuildPacket(sSimpleStartPacketBuf, 6, sSimpleStartTlvBuf, data_ov001_0222c884, C924_B8.v);
            if (SimpleStart_WcmGetState() != 7) {
                t40 = SimpleStart_GetMilliseconds() + 1000;
                retries = 10;
                sSimpleStartStep = 10;
            } else {
                SimpleStart_SendPacket(h, &l.from, sSimpleStartPacketBuf, data_ov001_0222c864);
                t40 = SimpleStart_GetMilliseconds();
                sSimpleStartStep = 10;
            }
            break;
        case 10:
            if (SimpleStart_GetMilliseconds() >= t40 + 1000) {
                retries++;
                if (retries >= 10) {
                    done = 1;
                    result = SimpleStart_BuildResult();
                } else {
                    sSimpleStartStep = 9;
                }
            }
            break;
        }
    }
    if (h != 0) Sock_Close(h);
    if (sSimpleStartCancel != 0) result = -8;
    return result;
}

void SimpleStart_ThreadMain(void *arg)
{
    s32 r = SimpleStart_RunProtocol();
    sSimpleStartResultCode = r;
    SimpleStart_Disconnect();
    if (r == 1) {
        sSimpleStartState = 6;
    } else {
        sSimpleStartState = 7;
    }
    sSimpleStartDeadline = -1;
    SimpleStart_NotifyStatus();
}

s32 SimpleStart_AesKeyWrap(u8 *out, const u8 *in, u32 inlen, const u8 *key, s32 keylen)
{
    s32 i, j, n, nr;
    u64 nn;
    u64 r;
    u64 tb;
    SimpleStartKeyWrapBlock iv;
    SimpleStartKeyWrapBlock a;
    SimpleStartKeyWrapBlock b;
    u32 rk[81];
    u64 t;
    *(u32 *)&iv.b[0] = 0xa6a6a6a6;
    *(u32 *)&iv.b[4] = 0xa6a6a6a6;
    if ((inlen & 7) != 0 || (keylen & 7) != 0) {
        return 0;
    }
    n = inlen >> 3;
    if (n < 2) {
        return 0;
    }
    nr = SimpleStart_AesKeySetupEnc(rk, key, keylen << 3);
    memcpy(out + 8, in, inlen);
    { SimpleStartKeyWrapBlock *pa = &a; *pa = iv; }
    for (j = 0; j < 6; j++) {
        nn = n;
        i = 1;
        if (i <= n) do {
            b = *(SimpleStartKeyWrapBlock *)(out + ((u32)i << 3));
            SimpleStart_AesEncryptBlock(rk, nr, a.b, a.b);
            t = r = nn * j;
            t = i + r;
            tb = ((t & 0x00000000000000ffULL) << 56) | ((t & 0x000000000000ff00ULL) << 40) |
                 ((t & 0x0000000000ff0000ULL) << 24) | ((t & 0x00000000ff000000ULL) << 8) |
                 ((t & 0x000000ff00000000ULL) >> 8) | ((t & 0x0000ff0000000000ULL) >> 24) |
                 ((t & 0x00ff000000000000ULL) >> 40) | ((t & 0xff00000000000000ULL) >> 56);
            SimpleStart_Xor8(a.b, (u8 *)&tb, a.b);
            *(SimpleStartKeyWrapBlock *)(out + ((u32)i << 3)) = b;
        } while (++i <= n);
    }
    *(SimpleStartKeyWrapBlock *)out = a;
    return 1;
}

s32 SimpleStart_AesKeyUnwrap(u8 *out, const u8 *in, u32 inlen, const u8 *key, s32 keylen)
{
    s32 i, j, n, nr;
    s32 ok = 1;
    u64 nn;
    u64 r;
    u64 tb;
    SimpleStartKeyWrapBlock iv;
    SimpleStartKeyWrapBlock a;
    SimpleStartKeyWrapBlock b;
    u32 rk[81];
    u64 t;
    *(u32 *)&iv.b[0] = 0xa6a6a6a6;
    *(u32 *)&iv.b[4] = 0xa6a6a6a6;
    if ((inlen & 7) != 0 || (keylen & 7) != 0) {
        return 0;
    }
    n = (inlen - 1) >> 3;
    if (n < 2) {
        return 0;
    }
    nr = SimpleStart_AesKeySetupDec(rk, key, keylen << 3);
    { SimpleStartKeyWrapBlock *pa = &a; *pa = *(const SimpleStartKeyWrapBlock *)in; }
    in += 8;
    memcpy(out, in, inlen - 1);
    for (j = 5; j >= 0; j--) {
        nn = n;
        i = n;
        if (i > 0) do {
            t = r = nn * j;
            t = i + r;
            tb = ((t & 0x00000000000000ffULL) << 56) | ((t & 0x000000000000ff00ULL) << 40) |
                 ((t & 0x0000000000ff0000ULL) << 24) | ((t & 0x00000000ff000000ULL) << 8) |
                 ((t & 0x000000ff00000000ULL) >> 8) | ((t & 0x0000ff0000000000ULL) >> 24) |
                 ((t & 0x00ff000000000000ULL) >> 40) | ((t & 0xff00000000000000ULL) >> 56);
            SimpleStart_Xor8(a.b, (u8 *)&tb, a.b);
            u8 *p = out + (i - 1) * 8;
            b = *(SimpleStartKeyWrapBlock *)p;
            SimpleStart_AesDecryptBlock(rk, nr, a.b, a.b);
            *(SimpleStartKeyWrapBlock *)p = b;
        } while (--i > 0);
    }
    if (memcmp(&iv, &a, 8) != 0) ok = 0;
    return ok;
}

void SimpleStart_Xor8(const u8 *a, const u8 *b, u8 *c)
{
    c[0] = a[0] ^ b[0];
    c[1] = a[1] ^ b[1];
    c[2] = a[2] ^ b[2];
    c[3] = a[3] ^ b[3];
    c[4] = a[4] ^ b[4];
    c[5] = a[5] ^ b[5];
    c[6] = a[6] ^ b[6];
    c[7] = a[7] ^ b[7];
}

s32 SimpleStart_AesKeySetupEnc(u32 *rk, const u8 *cipherKey, s32 keyBits)
{
    s32 i = 0;
    u32 temp;
    const u32 *rp;

    rk[0] = GETU32(cipherKey);
    rk[1] = GETU32(cipherKey + 4);
    rk[2] = GETU32(cipherKey + 8);
    rk[3] = GETU32(cipherKey + 12);
    if (keyBits == 128) {
        rp = rcon;
        for (;;) {
            temp = rk[3];
            rk[4] = rk[0] ^ (Te4[(temp >> 16) & 0xff] & 0xff000000) ^
                    (Te4[(temp >> 8) & 0xff] & 0x00ff0000) ^
                    (Te4[(temp) & 0xff] & 0x0000ff00) ^
                    (Te4[(temp >> 24)] & 0x000000ff) ^ *rp;
            rk[5] = rk[1] ^ rk[4];
            rk[6] = rk[2] ^ rk[5];
            rk[7] = rk[3] ^ rk[6];
            rp++;
            if (++i == 10) {
                return 10;
            }
            rk += 4;
        }
    }
    rk[4] = GETU32(cipherKey + 16);
    rk[5] = GETU32(cipherKey + 20);
    if (keyBits == 192) {
        rp = rcon;
        for (;;) {
            temp = rk[5];
            rk[6] = rk[0] ^ (Te4[(temp >> 16) & 0xff] & 0xff000000) ^
                    (Te4[(temp >> 8) & 0xff] & 0x00ff0000) ^
                    (Te4[(temp) & 0xff] & 0x0000ff00) ^
                    (Te4[(temp >> 24)] & 0x000000ff) ^ *rp;
            rk[7] = rk[1] ^ rk[6];
            rk[8] = rk[2] ^ rk[7];
            rk[9] = rk[3] ^ rk[8];
            rp++;
            if (++i == 8) {
                return 12;
            }
            rk[10] = rk[4] ^ rk[9];
            rk[11] = rk[5] ^ rk[10];
            rk += 6;
        }
    }
    rk[6] = GETU32(cipherKey + 24);
    rk[7] = GETU32(cipherKey + 28);
    if (keyBits == 256) {
        rp = rcon;
        for (;;) {
            temp = rk[7];
            rk[8] = rk[0] ^ (Te4[(temp >> 16) & 0xff] & 0xff000000) ^
                    (Te4[(temp >> 8) & 0xff] & 0x00ff0000) ^
                    (Te4[(temp) & 0xff] & 0x0000ff00) ^
                    (Te4[(temp >> 24)] & 0x000000ff) ^ *rp;
            rk[9] = rk[1] ^ rk[8];
            rk[10] = rk[2] ^ rk[9];
            rk[11] = rk[3] ^ rk[10];
            rp++;
            if (++i == 7) {
                return 14;
            }
            temp = rk[11];
            rk[12] = rk[4] ^ (Te4[(temp >> 24)] & 0xff000000) ^
                     (Te4[(temp >> 16) & 0xff] & 0x00ff0000) ^
                     (Te4[(temp >> 8) & 0xff] & 0x0000ff00) ^
                     (Te4[(temp) & 0xff] & 0x000000ff);
            rk[13] = rk[5] ^ rk[12];
            rk[14] = rk[6] ^ rk[13];
            rk[15] = rk[7] ^ rk[14];
            rk += 8;
        }
    }
    return 0;
}

s32 SimpleStart_AesKeySetupDec(u32 *rk, const u8 *key, s32 bits)
{
    s32 Nr, i, j;
    u32 temp;
    Nr = SimpleStart_AesKeySetupEnc(rk, key, bits);
    for (i = 0, j = 4 * Nr; i < j; i += 4, j -= 4) {
        temp = rk[i]; rk[i] = rk[j]; rk[j] = temp;
        temp = rk[i + 1]; rk[i + 1] = rk[j + 1]; rk[j + 1] = temp;
        temp = rk[i + 2]; rk[i + 2] = rk[j + 2]; rk[j + 2] = temp;
        temp = rk[i + 3]; rk[i + 3] = rk[j + 3]; rk[j + 3] = temp;
    }
    for (i = 1; i < Nr; i++) {
        rk += 4;
        rk[0] = Td0[Te4[(rk[0] >> 24)] & 0xff] ^ Td1[Te4[(rk[0] >> 16) & 0xff] & 0xff] ^
                Td2[Te4[(rk[0] >> 8) & 0xff] & 0xff] ^ Td3[Te4[(rk[0]) & 0xff] & 0xff];
        rk[1] = Td0[Te4[(rk[1] >> 24)] & 0xff] ^ Td1[Te4[(rk[1] >> 16) & 0xff] & 0xff] ^
                Td2[Te4[(rk[1] >> 8) & 0xff] & 0xff] ^ Td3[Te4[(rk[1]) & 0xff] & 0xff];
        rk[2] = Td0[Te4[(rk[2] >> 24)] & 0xff] ^ Td1[Te4[(rk[2] >> 16) & 0xff] & 0xff] ^
                Td2[Te4[(rk[2] >> 8) & 0xff] & 0xff] ^ Td3[Te4[(rk[2]) & 0xff] & 0xff];
        rk[3] = Td0[Te4[(rk[3] >> 24)] & 0xff] ^ Td1[Te4[(rk[3] >> 16) & 0xff] & 0xff] ^
                Td2[Te4[(rk[3] >> 8) & 0xff] & 0xff] ^ Td3[Te4[(rk[3]) & 0xff] & 0xff];
    }
    return Nr;
}

void SimpleStart_AesEncryptBlock(const u32 *rk, s32 Nr, const u8 *ct, u8 *pt)
{
    u32 s0, s1, s2, s3, t0, t1, t2, t3;
    s32 r;
    s0 = GETU32(ct) ^ rk[0];
    s1 = GETU32(ct + 4) ^ rk[1];
    s2 = GETU32(ct + 8) ^ rk[2];
    s3 = GETU32(ct + 12) ^ rk[3];
    r = Nr >> 1;
    for (;;) {
        t0 = data_ov001_02227f34[s0 >> 24] ^ data_ov001_02228334[(s1 >> 16) & 0xff] ^ data_ov001_02228734[(s2 >> 8) & 0xff] ^ data_ov001_02228b34[s3 & 0xff] ^ rk[4];
        t1 = data_ov001_02227f34[s1 >> 24] ^ data_ov001_02228334[(s2 >> 16) & 0xff] ^ data_ov001_02228734[(s3 >> 8) & 0xff] ^ data_ov001_02228b34[s0 & 0xff] ^ rk[5];
        t2 = data_ov001_02227f34[s2 >> 24] ^ data_ov001_02228334[(s3 >> 16) & 0xff] ^ data_ov001_02228734[(s0 >> 8) & 0xff] ^ data_ov001_02228b34[s1 & 0xff] ^ rk[6];
        t3 = data_ov001_02227f34[s3 >> 24] ^ data_ov001_02228334[(s0 >> 16) & 0xff] ^ data_ov001_02228734[(s1 >> 8) & 0xff] ^ data_ov001_02228b34[s2 & 0xff] ^ rk[7];
        rk += 8;
        if (--r == 0) break;
        s0 = data_ov001_02227f34[t0 >> 24] ^ data_ov001_02228334[(t1 >> 16) & 0xff] ^ data_ov001_02228734[(t2 >> 8) & 0xff] ^ data_ov001_02228b34[t3 & 0xff] ^ rk[0];
        s1 = data_ov001_02227f34[t1 >> 24] ^ data_ov001_02228334[(t2 >> 16) & 0xff] ^ data_ov001_02228734[(t3 >> 8) & 0xff] ^ data_ov001_02228b34[t0 & 0xff] ^ rk[1];
        s2 = data_ov001_02227f34[t2 >> 24] ^ data_ov001_02228334[(t3 >> 16) & 0xff] ^ data_ov001_02228734[(t0 >> 8) & 0xff] ^ data_ov001_02228b34[t1 & 0xff] ^ rk[2];
        s3 = data_ov001_02227f34[t3 >> 24] ^ data_ov001_02228334[(t0 >> 16) & 0xff] ^ data_ov001_02228734[(t1 >> 8) & 0xff] ^ data_ov001_02228b34[t2 & 0xff] ^ rk[3];
    }
    s0 = (data_ov001_02228f34[t0 >> 24] & 0xff000000) ^ (data_ov001_02228f34[(t1 >> 16) & 0xff] & 0x00ff0000) ^ (data_ov001_02228f34[(t2 >> 8) & 0xff] & 0x0000ff00) ^ (data_ov001_02228f34[t3 & 0xff] & 0x000000ff) ^ rk[0];
    PUTU32(pt, s0);
    s1 = (data_ov001_02228f34[t1 >> 24] & 0xff000000) ^ (data_ov001_02228f34[(t2 >> 16) & 0xff] & 0x00ff0000) ^ (data_ov001_02228f34[(t3 >> 8) & 0xff] & 0x0000ff00) ^ (data_ov001_02228f34[t0 & 0xff] & 0x000000ff) ^ rk[1];
    PUTU32(pt + 4, s1);
    s2 = (data_ov001_02228f34[t2 >> 24] & 0xff000000) ^ (data_ov001_02228f34[(t3 >> 16) & 0xff] & 0x00ff0000) ^ (data_ov001_02228f34[(t0 >> 8) & 0xff] & 0x0000ff00) ^ (data_ov001_02228f34[t1 & 0xff] & 0x000000ff) ^ rk[2];
    PUTU32(pt + 8, s2);
    s3 = (data_ov001_02228f34[t3 >> 24] & 0xff000000) ^ (data_ov001_02228f34[(t0 >> 16) & 0xff] & 0x00ff0000) ^ (data_ov001_02228f34[(t1 >> 8) & 0xff] & 0x0000ff00) ^ (data_ov001_02228f34[t2 & 0xff] & 0x000000ff) ^ rk[3];
    PUTU32(pt + 12, s3);
}

void SimpleStart_AesDecryptBlock(const u32 *rk, s32 Nr, const u8 *ct, u8 *pt)
{
    u32 s0, s1, s2, s3, t0, t1, t2, t3;
    s32 r;
    s0 = GETU32(ct) ^ rk[0];
    s1 = GETU32(ct + 4) ^ rk[1];
    s2 = GETU32(ct + 8) ^ rk[2];
    s3 = GETU32(ct + 12) ^ rk[3];
    r = Nr >> 1;
    for (;;) {
        t0 = data_ov001_02229334[s0 >> 24] ^ data_ov001_02229734[(s3 >> 16) & 0xff] ^ data_ov001_02227334[(s2 >> 8) & 0xff] ^ data_ov001_02227734[s1 & 0xff] ^ rk[4];
        t1 = data_ov001_02229334[s1 >> 24] ^ data_ov001_02229734[(s0 >> 16) & 0xff] ^ data_ov001_02227334[(s3 >> 8) & 0xff] ^ data_ov001_02227734[s2 & 0xff] ^ rk[5];
        t2 = data_ov001_02229334[s2 >> 24] ^ data_ov001_02229734[(s1 >> 16) & 0xff] ^ data_ov001_02227334[(s0 >> 8) & 0xff] ^ data_ov001_02227734[s3 & 0xff] ^ rk[6];
        t3 = data_ov001_02229334[s3 >> 24] ^ data_ov001_02229734[(s2 >> 16) & 0xff] ^ data_ov001_02227334[(s1 >> 8) & 0xff] ^ data_ov001_02227734[s0 & 0xff] ^ rk[7];
        rk += 8;
        if (--r == 0) break;
        s0 = data_ov001_02229334[t0 >> 24] ^ data_ov001_02229734[(t3 >> 16) & 0xff] ^ data_ov001_02227334[(t2 >> 8) & 0xff] ^ data_ov001_02227734[t1 & 0xff] ^ rk[0];
        s1 = data_ov001_02229334[t1 >> 24] ^ data_ov001_02229734[(t0 >> 16) & 0xff] ^ data_ov001_02227334[(t3 >> 8) & 0xff] ^ data_ov001_02227734[t2 & 0xff] ^ rk[1];
        s2 = data_ov001_02229334[t2 >> 24] ^ data_ov001_02229734[(t1 >> 16) & 0xff] ^ data_ov001_02227334[(t0 >> 8) & 0xff] ^ data_ov001_02227734[t3 & 0xff] ^ rk[2];
        s3 = data_ov001_02229334[t3 >> 24] ^ data_ov001_02229734[(t2 >> 16) & 0xff] ^ data_ov001_02227334[(t1 >> 8) & 0xff] ^ data_ov001_02227734[t0 & 0xff] ^ rk[3];
    }
    s0 = (data_ov001_02227b34[t0 >> 24] & 0xff000000) ^ (data_ov001_02227b34[(t3 >> 16) & 0xff] & 0x00ff0000) ^ (data_ov001_02227b34[(t2 >> 8) & 0xff] & 0x0000ff00) ^ (data_ov001_02227b34[t1 & 0xff] & 0x000000ff) ^ rk[0];
    PUTU32(pt, s0);
    s1 = (data_ov001_02227b34[t1 >> 24] & 0xff000000) ^ (data_ov001_02227b34[(t0 >> 16) & 0xff] & 0x00ff0000) ^ (data_ov001_02227b34[(t3 >> 8) & 0xff] & 0x0000ff00) ^ (data_ov001_02227b34[t2 & 0xff] & 0x000000ff) ^ rk[1];
    PUTU32(pt + 4, s1);
    s2 = (data_ov001_02227b34[t2 >> 24] & 0xff000000) ^ (data_ov001_02227b34[(t1 >> 16) & 0xff] & 0x00ff0000) ^ (data_ov001_02227b34[(t0 >> 8) & 0xff] & 0x0000ff00) ^ (data_ov001_02227b34[t3 & 0xff] & 0x000000ff) ^ rk[2];
    PUTU32(pt + 8, s2);
    s3 = (data_ov001_02227b34[t3 >> 24] & 0xff000000) ^ (data_ov001_02227b34[(t2 >> 16) & 0xff] & 0x00ff0000) ^ (data_ov001_02227b34[(t1 >> 8) & 0xff] & 0x0000ff00) ^ (data_ov001_02227b34[t0 & 0xff] & 0x000000ff) ^ rk[3];
    PUTU32(pt + 12, s3);
}

void SimpleStart_Md5Init(SimpleStartMd5Ctx *ctx)
{
    ctx->count[0] = ctx->count[1] = 0;
    ctx->state[0] = 0x67452301;
    ctx->state[1] = 0xefcdab89;
    ctx->state[2] = 0x98badcfe;
    ctx->state[3] = 0x10325476;
}

void SimpleStart_Md5Update(SimpleStartMd5Ctx *ctx, const u8 *data, u32 len)
{
    u32 i;
    u32 idx;
    idx = (ctx->count[0] >> 3) & 0x3f;
    ctx->count[0] += len << 3;
    if (ctx->count[0] < (len << 3)) {
        ctx->count[1]++;
    }
    ctx->count[1] += len >> 29;
    i = 64 - idx;
    if (len >= i) {
        SimpleStart_Md5Memcpy(&ctx->buffer[idx], (u8 *)data, i);
        SimpleStart_Md5Transform((u32 *)ctx, ctx->buffer);
        if (i + 63 < len) {
            do {
                SimpleStart_Md5Transform((u32 *)ctx, (u8 *)&data[i]);
                i += 64;
            } while (i + 63 < len);
        }
        idx = 0;
    } else {
        i = 0;
    }
    SimpleStart_Md5Memcpy(&ctx->buffer[idx], (u8 *)&data[i], len - i);
}

void SimpleStart_Md5Final(u8 *out, SimpleStartMd5Ctx *ctx)
{
    u8 bits[8];
    u32 idx;
    u32 pad;
    SimpleStart_Md5Encode(bits, &ctx->count[0], 8);
    idx = (ctx->count[0] >> 3) & 0x3f;
    if (idx < 0x38) {
        pad = 0x38 - idx;
    } else {
        pad = 0x78 - idx;
    }
    SimpleStart_Md5Update(ctx, sSimpleStartMd5Padding, pad);
    SimpleStart_Md5Update(ctx, bits, 8);
    SimpleStart_Md5Encode(out, (u32 *)ctx, 0x10);
    SimpleStart_Md5Memset(ctx, 0, 0x58);
}

void SimpleStart_Md5Transform(u32 *state, u8 *block) {
    u32 a = state[0], b = state[1], c = state[2], d = state[3];
    u32 x[16];
    SimpleStart_Md5Decode(x, block, 64);
    STEP(F, a, b, c, d, x[0], 7, 0xd76aa478u);
    STEP(F, d, a, b, c, x[1], 12, 0xe8c7b756u);
    STEP(F, c, d, a, b, x[2], 17, 0x242070dbu);
    STEP(F, b, c, d, a, x[3], 22, 0xc1bdceeeu);
    STEP(F, a, b, c, d, x[4], 7, 0xf57c0fafu);
    STEP(F, d, a, b, c, x[5], 12, 0x4787c62au);
    STEP(F, c, d, a, b, x[6], 17, 0xa8304613u);
    STEP(F, b, c, d, a, x[7], 22, 0xfd469501u);
    STEP(F, a, b, c, d, x[8], 7, 0x698098d8u);
    STEP(F, d, a, b, c, x[9], 12, 0x8b44f7afu);
    STEP(F, c, d, a, b, x[10], 17, 0xffff5bb1u);
    STEP(F, b, c, d, a, x[11], 22, 0x895cd7beu);
    STEP(F, a, b, c, d, x[12], 7, 0x6b901122u);
    STEP(F, d, a, b, c, x[13], 12, 0xfd987193u);
    STEP(F, c, d, a, b, x[14], 17, 0xa679438eu);
    STEP(F, b, c, d, a, x[15], 22, 0x49b40821u);
    STEP(G, a, b, c, d, x[1], 5, 0xf61e2562u);
    STEP(G, d, a, b, c, x[6], 9, 0xc040b340u);
    STEP(G, c, d, a, b, x[11], 14, 0x265e5a51u);
    STEP(G, b, c, d, a, x[0], 20, 0xe9b6c7aau);
    STEP(G, a, b, c, d, x[5], 5, 0xd62f105du);
    STEP(G, d, a, b, c, x[10], 9, 0x02441453u);
    STEP(G, c, d, a, b, x[15], 14, 0xd8a1e681u);
    STEP(G, b, c, d, a, x[4], 20, 0xe7d3fbc8u);
    STEP(G, a, b, c, d, x[9], 5, 0x21e1cde6u);
    STEP(G, d, a, b, c, x[14], 9, 0xc33707d6u);
    STEP(G, c, d, a, b, x[3], 14, 0xf4d50d87u);
    STEP(G, b, c, d, a, x[8], 20, 0x455a14edu);
    STEP(G, a, b, c, d, x[13], 5, 0xa9e3e905u);
    STEP(G, d, a, b, c, x[2], 9, 0xfcefa3f8u);
    STEP(G, c, d, a, b, x[7], 14, 0x676f02d9u);
    STEP(G, b, c, d, a, x[12], 20, 0x8d2a4c8au);
    STEP(H, a, b, c, d, x[5], 4, 0xfffa3942u);
    STEP(H, d, a, b, c, x[8], 11, 0x8771f681u);
    STEP(H, c, d, a, b, x[11], 16, 0x6d9d6122u);
    STEP(H, b, c, d, a, x[14], 23, 0xfde5380cu);
    STEP(H, a, b, c, d, x[1], 4, 0xa4beea44u);
    STEP(H, d, a, b, c, x[4], 11, 0x4bdecfa9u);
    STEP(H, c, d, a, b, x[7], 16, 0xf6bb4b60u);
    STEP(H, b, c, d, a, x[10], 23, 0xbebfbc70u);
    STEP(H, a, b, c, d, x[13], 4, 0x289b7ec6u);
    STEP(H, d, a, b, c, x[0], 11, 0xeaa127fau);
    STEP(H, c, d, a, b, x[3], 16, 0xd4ef3085u);
    STEP(H, b, c, d, a, x[6], 23, 0x04881d05u);
    STEP(H, a, b, c, d, x[9], 4, 0xd9d4d039u);
    STEP(H, d, a, b, c, x[12], 11, 0xe6db99e5u);
    STEP(H, c, d, a, b, x[15], 16, 0x1fa27cf8u);
    STEP(H, b, c, d, a, x[2], 23, 0xc4ac5665u);
    STEP(I, a, b, c, d, x[0], 6, 0xf4292244u);
    STEP(I, d, a, b, c, x[7], 10, 0x432aff97u);
    STEP(I, c, d, a, b, x[14], 15, 0xab9423a7u);
    STEP(I, b, c, d, a, x[5], 21, 0xfc93a039u);
    STEP(I, a, b, c, d, x[12], 6, 0x655b59c3u);
    STEP(I, d, a, b, c, x[3], 10, 0x8f0ccc92u);
    STEP(I, c, d, a, b, x[10], 15, 0xffeff47du);
    STEP(I, b, c, d, a, x[1], 21, 0x85845dd1u);
    STEP(I, a, b, c, d, x[8], 6, 0x6fa87e4fu);
    STEP(I, d, a, b, c, x[15], 10, 0xfe2ce6e0u);
    STEP(I, c, d, a, b, x[6], 15, 0xa3014314u);
    STEP(I, b, c, d, a, x[13], 21, 0x4e0811a1u);
    STEP(I, a, b, c, d, x[4], 6, 0xf7537e82u);
    STEP(I, d, a, b, c, x[11], 10, 0xbd3af235u);
    STEP(I, c, d, a, b, x[2], 15, 0x2ad7d2bbu);
    STEP(I, b, c, d, a, x[9], 21, 0xeb86d391u);
    state[0] += a;
    state[1] += b;
    state[2] += c;
    state[3] += d;
    SimpleStart_Md5Memset(x, 0, 64);
}

void SimpleStart_Md5Encode(u8 *dst, u32 *src, u32 n) {
    u32 i;
    for (i = 0; i < n; i += 4) {
        dst[i] = *src;
        dst[i + 1] = *src >> 8;
        dst[i + 2] = *src >> 16;
        dst[i + 3] = *src >> 24;
        src++;
    }
}

void SimpleStart_Md5Decode(u32 *dst, u8 *src, u32 n) {
    u32 i;
    for (i = 0; i < n; i += 4) {
        *dst++ = ((u32)src[i]) | ((u32)src[i + 1] << 8) | ((u32)src[i + 2] << 16) | ((u32)src[i + 3] << 24);
    }
}

void SimpleStart_Md5Memcpy(u8 *dst, u8 *src, u32 n) {
    u32 i;
    for (i = 0; i < n; i++) {
        dst[i] = src[i];
    }
}

void SimpleStart_Md5Memset(void *p, s32 c, u32 n) {
    s8 *q = (s8 *)p;
    u32 i;
    for (i = 0; i < n; i++) {
        *q = (s8)c;
        q++;
    }
}

void SimpleStart_Md5(void *out, void *p, u32 n) {
    u32 ctx[22];
    SimpleStart_Md5Init((SimpleStartMd5Ctx *)ctx);
    SimpleStart_Md5Update((SimpleStartMd5Ctx *)ctx, (const u8 *)p, n);
    SimpleStart_Md5Final((u8 *)out, (SimpleStartMd5Ctx *)ctx);
}

u32 SimpleStart_GetMilliseconds() {
    return (OS_GetTick() << 6) / 0x82ea;
}

void *SimpleStart_Calloc(u32 n, u32 m) {
    u32 size = n * m;
    void *p = sSimpleStartAllocFunc(size);
    if (p != NULL) {
        memset(p, 0, size);
    }
    return p;
}

void SimpleStart_Free(void *p) {
    sSimpleStartFreeFunc(p);
}

s32 SimpleStart_FormatHexByte(char *buf, u32 v) {
    char *p = buf;
    s32 b = (u8)v;
    s32 i, hi, lo;
    hi = (b & 0xf0) >> 4;
    i = 0;
    lo = b & 0xf;
    do {
        if (hi <= 9) {
            *p = hi + 0x30;
            p++;
        } else {
            *p = hi + 0x37;
            p++;
        }
        hi = lo;
        i++;
    } while (i < 2);
    *p = 0;
    return p - buf;
}

s32 SimpleStart_FormatMac(char *dst, s8 *src) {
    char *start = dst;
    char *cur = dst;
    s32 i;
    for (i = 0; i < 6; i++) {
        cur += SimpleStart_FormatHexByte(cur, *src++);
        if (i < 5) {
            *cur++ = ':';
        }
    }
    *cur = 0;
    return cur - start;
}

s32 SimpleStart_Start(u32 a0, s32 a1, SimpleStartStatusFunc a2, SimpleStartAllocFunc a3, SimpleStartFreeFunc a4, s32 a5) {
    s32 r;
    if (sSimpleStartState >= 1 && sSimpleStartState <= 5) {
        return -10;
    }
    data_ov001_0222a53c = a1;
    sSimpleStartState = 7;
    sSimpleStartStatusCb = a2;
    sSimpleStartAllocFunc = a3;
    sSimpleStartFreeFunc = a4;
    sSimpleStartStackSize = a5;
    r = SimpleStart_WlanStartup(a1);
    data_ov001_0222c8c4 = 1;
    if (r < 0) {
        sSimpleStartResultCode = r;
        return r;
    }
    data_ov001_0222c880 = (u8 *)sSimpleStartAllocFunc(sSimpleStartStackSize);
    if (data_ov001_0222c880 == 0) {
        r = -1;
        sSimpleStartResultCode = r;
        return r;
    }
    if (OS_IsThreadAvailable() != 1) {
        r = -9;
        sSimpleStartResultCode = r;
        return r;
    }
    OS_CreateThread(&sSimpleStartThread, SimpleStart_ThreadMain, 0, data_ov001_0222c880 + (sSimpleStartStackSize & ~7), sSimpleStartStackSize, a0);
    sSimpleStartState = 1;
    sSimpleStartDeadline = SimpleStart_GetMilliseconds() + 60000;
    sSimpleStartCancel = 0;
    __builtin__clear(sSimpleStartResult, 0xe8);
    SimpleStart_NotifyStatus();
    OS_WakeupThreadDirect(&sSimpleStartThread);
    data_ov001_0222c8b4 = 1;
    return 1;
}

s32 SimpleStart_Stop(void) {
    if (data_ov001_0222c8b4 != 0) {
        s32 prev = sSimpleStartState;
        sSimpleStartCancel = 1;
        while (sSimpleStartState >= 1 && sSimpleStartState <= 5) {
            OS_Sleep(100);
        }
        OS_Sleep(500);
        if (OS_IsThreadTerminated(&sSimpleStartThread) == 0) {
            do {
                OS_WakeupThreadDirect(&sSimpleStartThread);
                OS_JoinThread(&sSimpleStartThread);
            } while (OS_IsThreadTerminated(&sSimpleStartThread) == 0);
        }
        if (data_ov001_0222c880 != 0) {
            sSimpleStartFreeFunc(data_ov001_0222c880);
            data_ov001_0222c880 = 0;
        }
        data_ov001_0222c8b4 = 0;
        if (prev != sSimpleStartState) {
            SimpleStart_NotifyStatus();
        }
    }
    if (data_ov001_0222c8c4 > 0) {
        s32 r = SimpleStart_WlanShutdown();
        data_ov001_0222c8c4 = 0;
        return r;
    }
    return -10;
}

s32 SimpleStart_GetStatus(SimpleStartStatus *out) {
    out->state = sSimpleStartState;
    if (sSimpleStartDeadline == -1) {
        out->remainingMs = -1;
    } else {
        out->remainingMs = sSimpleStartDeadline - SimpleStart_GetMilliseconds();
    }
    out->result = sSimpleStartResultCode;
    return TRUE;
}

s32 SimpleStart_GetResult(void *dst) {
    memcpy(dst, sSimpleStartResult, 0xe8);
    return TRUE;
}

void SimpleStart_NotifyStatus(void) {
    SimpleStartStatus buf;
    SimpleStart_GetStatus(&buf);
    sSimpleStartStatusCb(&buf);
}





}

// ---- overlay variables (generated: tools/host_overlay_vars.py) ----
#include "host_overlay_vars.h"

HOST_OVERLAY_VARS(unk_ov001_02203b1c) {
    HOST_OVERLAY_VAR(data_ov001_0222c898),
    HOST_OVERLAY_VAR(sSimpleStartStackSize),
    HOST_OVERLAY_VAR(sSimpleStartAllocFunc),
    HOST_OVERLAY_VAR(sSimpleStartStep),
    HOST_OVERLAY_VAR(data_ov001_0222c864),
    HOST_OVERLAY_VAR(sSimpleStartSetupSsid),
    HOST_OVERLAY_VAR(data_ov001_0222a548),
    HOST_OVERLAY_VAR(data_ov001_0222a550),
    HOST_OVERLAY_VAR(sSimpleStartMd5Padding),
    HOST_OVERLAY_VAR(sSimpleStartSocConfig),
    HOST_OVERLAY_VAR(data_ov001_0222c850),
    HOST_OVERLAY_VAR(data_ov001_0222c894),
    HOST_OVERLAY_VAR(sSimpleStartDeadline),
    HOST_OVERLAY_VAR(data_ov001_0222c89c),
    HOST_OVERLAY_VAR(data_ov001_0222c868),
    HOST_OVERLAY_VAR(data_ov001_0222a52c),
    HOST_OVERLAY_VAR(data_ov001_0222a53c),
    HOST_OVERLAY_VAR(data_ov001_0222c884),
    HOST_OVERLAY_VAR(sSimpleStartWcmNotifyCb),
    HOST_OVERLAY_VAR(sSimpleStartStatusCb),
    HOST_OVERLAY_VAR(sSimpleStartResultCode),
    HOST_OVERLAY_VAR(data_ov001_0222c8c4),
    HOST_OVERLAY_VAR(sSimpleStartMaxAps),
    HOST_OVERLAY_VAR(sSimpleStartMsgTail),
    HOST_OVERLAY_VAR(data_ov001_0222c8b4),
    HOST_OVERLAY_VAR(data_ov001_0222a530),
    HOST_OVERLAY_VAR(data_ov001_0222c8ac),
    HOST_OVERLAY_VAR(data_ov001_0222c870),
    HOST_OVERLAY_VAR(data_ov001_0222c8a8),
    HOST_OVERLAY_VAR(data_ov001_0222c8a4),
    HOST_OVERLAY_VAR(data_ov001_0222c8a0),
    HOST_OVERLAY_VAR(data_ov001_0222c858),
    HOST_OVERLAY_VAR(data_ov001_0222c874),
    HOST_OVERLAY_VAR(sSimpleStartCancel),
    HOST_OVERLAY_VAR(data_ov001_0222c890),
    HOST_OVERLAY_VAR(data_ov001_0222c88c),
    HOST_OVERLAY_VAR(sSimpleStartState),
    HOST_OVERLAY_VAR(sSimpleStartFreeFunc),
    HOST_OVERLAY_VAR(data_ov001_0222c880),
    HOST_OVERLAY_VAR(sSimpleStartWcmState),
    HOST_OVERLAY_VAR(sSimpleStartMsgHead),
    HOST_OVERLAY_VAR(data_ov001_0222c878),
    HOST_OVERLAY_VAR(data_ov001_0222c8b0),
    HOST_OVERLAY_VAR(data_ov001_0222c8d4),
    HOST_OVERLAY_VAR(data_ov001_0222c8dc),
    HOST_OVERLAY_VAR(data_ov001_0222c8e4),
    HOST_OVERLAY_VAR(sSimpleStartMsgQueue),
    HOST_OVERLAY_VAR(data_ov001_0222c8fc),
    HOST_OVERLAY_VAR(sSimpleStartAllocMutex),
    HOST_OVERLAY_VAR(data_ov001_0222c944),
    HOST_OVERLAY_VAR(data_ov001_0222c924),
    HOST_OVERLAY_VAR(data_ov001_0222c964),
    HOST_OVERLAY_VAR(sSimpleStartThread),
    HOST_OVERLAY_VAR(sSimpleStartResult),
    HOST_OVERLAY_VAR(data_ov001_0222cb30),
    HOST_OVERLAY_VAR(sSimpleStartTlvBuf),
    HOST_OVERLAY_VAR(sSimpleStartPacketBuf),
    HOST_OVERLAY_VARS_END
};
