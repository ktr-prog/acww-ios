// Host build: the Wi-Fi access point's network for Nintendo Wi-Fi Connection through Wiimmfi (net_nat.c): the frames
// the DS sends in infrastructure mode (wm7.c's DCF) in, the frames to it out. A frame is the DS's DCF data: an 802.2
// LLC/SNAP header (AA AA 03 00 00 00), the ethertype, the payload.
#ifndef HOST_NET_NAT_H
#define HOST_NET_NAT_H

#include "host.h"

#define NAT_AP_SSID "ACWW Wiimmfi"  // the access point's name (wm7.c's scans, the Wi-Fi settings pxi7.c puts in)
extern const u8 kNatGatewayMac[6];  // the access point's BSSID and the gateway's MAC address

int nat_supported(void);              // 0: no Wiimmfi in this build (Windows)
void nat_start(const u8 dsMac[6]);    // the DS connected to the access point
void nat_stop(void);                  // disconnected: every flow closed
void nat_from_ds(const u8 dst[6], const u8 *llc, u32 len);
void nat_poll(void);                  // the host's sockets (from the event loop)
int nat_take(u8 *buf, u32 cap, u32 *len);  // the next frame for the DS, if any

#endif
