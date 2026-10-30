// Host build: the access point and the internet behind it, for Nintendo Wi-Fi Connection through Wiimmfi (net_nat.h).
//
// The game's own network stack (ov065: the SDK's IP stack, its SSL, DWC, GameSpy) runs unchanged and sends Ethernet
// frames over the DS's infrastructure mode (WM's DCF, wm7.c). Here a small router answers them, as a home router and a
// NAT would: ARP for the gateway (proxy ARP: every address is behind it), DHCP (the DS gets 10.0.0.10, the gateway is
// 10.0.0.1, the DNS server Wiimmfi's), ICMP echo, and every UDP and TCP flow to the internet through the host's own
// sockets (non-blocking, polled from the runtime's event loop): a UDP socket per DS port; a TCP connection is ended
// here (the DS's segments acknowledged, its data written to the host's socket; the host's data sent back in segments
// the DS's window and MSS allow, resent when not acknowledged). The DNS server the DS is given is Wiimmfi's
// (167.86.108.126, ACWW_WIIMMFI_DNS another): the game's queries for nintendowifi.net and gamespy.com go there, which
// is how Wiimmfi takes a real DS's traffic too. Outside the save states' range (sockets are this launch's).
#include "net_nat.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "plat.h"

const u8 kNatGatewayMac[6] = {0x02, 0x41, 0x43, 0x57, 0x57, 0x01};  // (also the access point's BSSID)

#ifdef _WIN32
// (no Wiimmfi on Windows yet: the radio finds no access point)
int nat_supported(void) { return 0; }
void nat_start(const u8 dsMac[6]) { (void)dsMac; }
void nat_stop(void) {}
void nat_from_ds(const u8 dst[6], const u8 *llc, u32 len) { (void)dst, (void)llc, (void)len; }
void nat_poll(void) {}
int nat_take(u8 *buf, u32 cap, u32 *len) { (void)buf, (void)cap, (void)len; return 0; }
#else
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <poll.h>
#include <sys/socket.h>
#include <unistd.h>

#define GW_IP 0x0a000001u   // 10.0.0.1
#define DS_IP 0x0a00000au   // 10.0.0.10
#define NETMASK 0xffffff00u
#define DNS_IP_DEFAULT 0xa7566c7eu  // 167.86.108.126: Wiimmfi's DNS server
#define MSS_MAX 1360        // (the DS's MTU in its Wi-Fi settings is 1400)
#define TCP_BUF 65536
#define MAX_TCP 24
#define MAX_UDP 16
#define QUEUE 128

static int sOn, sLog = -1;
static u8 sDsMac[6];
static u32 sDnsIp;

static u64 now_ms(void) { return host_plat_time_ns() / 1000000u; }
#define LOG(...) do { if (sLog) fprintf(stderr, "nat: " __VA_ARGS__); } while (0)

int nat_supported(void) { return 1; }

// ---- frames to the DS: LLC/SNAP + ethertype + payload, in a ring
static struct { u32 len; u8 data[1520]; } sQ[QUEUE];
static int sQHead, sQCount;

int nat_take(u8 *buf, u32 cap, u32 *len) {
    if (!sQCount) return 0;
    u32 n = sQ[sQHead].len < cap ? sQ[sQHead].len : cap;
    memcpy(buf, sQ[sQHead].data, n);
    *len = n;
    sQHead = (sQHead + 1) % QUEUE;
    sQCount--;
    return 1;
}

static int queue_room(void) { return QUEUE - sQCount; }

static void put16(u8 *p, u32 v) { p[0] = (u8)(v >> 8), p[1] = (u8)v; }
static void put32(u8 *p, u32 v) { put16(p, v >> 16), put16(p + 2, v); }
static u32 get16(const u8 *p) { return (u32)p[0] << 8 | p[1]; }
static u32 get32(const u8 *p) { return get16(p) << 16 | get16(p + 2); }

static u32 csum_add(u32 sum, const u8 *p, u32 n) {
    for (u32 i = 0; i + 1 < n; i += 2) sum += get16(p + i);
    if (n & 1) sum += (u32)p[n - 1] << 8;
    return sum;
}
static u16 csum_fold(u32 sum) {
    while (sum >> 16) sum = (sum & 0xffff) + (sum >> 16);
    return (u16)~sum;
}

static u8 *frame(u16 type, u32 len) {  // a frame of `len` payload bytes queued; its payload
    if (sQCount == QUEUE || len > sizeof sQ[0].data - 8) return 0;
    u8 *f = sQ[(sQHead + sQCount) % QUEUE].data;
    static const u8 kLlc[6] = {0xaa, 0xaa, 0x03, 0, 0, 0};
    memcpy(f, kLlc, 6);
    put16(f + 6, type);
    sQ[(sQHead + sQCount) % QUEUE].len = 8 + len;
    sQCount++;
    return f + 8;
}

// an IPv4 packet to the DS from `src`: header + `plen` bytes of payload (already in place after the 20-byte header)
static u8 *ip_begin(u32 plen) { return frame(0x0800, 20 + plen); }
static void ip_finish(u8 *ip, u8 proto, u32 src, u32 dst, u32 plen) {
    static u16 id;
    ip[0] = 0x45, ip[1] = 0;
    put16(ip + 2, 20 + plen);
    put16(ip + 4, ++id);
    put16(ip + 6, 0x4000);  // don't fragment
    ip[8] = 64, ip[9] = proto;
    put16(ip + 10, 0);
    put32(ip + 12, src);
    put32(ip + 16, dst);
    put16(ip + 10, csum_fold(csum_add(0, ip, 20)));
}
static u16 l4_csum(u8 proto, u32 src, u32 dst, const u8 *seg, u32 len) {
    u8 ph[12];
    put32(ph, src), put32(ph + 4, dst);
    ph[8] = 0, ph[9] = proto;
    put16(ph + 10, len);
    return csum_fold(csum_add(csum_add(0, ph, 12), seg, len));
}

static void send_udp(u32 src, u16 sport, u32 dst, u16 dport, const u8 *data, u32 n) {
    u8 *ip = ip_begin(8 + n);
    if (!ip) return;
    u8 *u = ip + 20;
    put16(u, sport), put16(u + 2, dport), put16(u + 4, 8 + n), put16(u + 6, 0);
    memcpy(u + 8, data, n);
    u16 c = l4_csum(17, src, dst, u, 8 + n);
    put16(u + 6, c ? c : 0xffff);
    ip_finish(ip, 17, src, dst, 8 + n);
}

// ---- ARP: every address answered with the gateway's MAC (proxy ARP)
static void arp_in(const u8 *a, u32 n) {
    if (n < 28 || get16(a) != 1 || get16(a + 2) != 0x0800 || get16(a + 6) != 1) return;
    u32 spa = get32(a + 14), tpa = get32(a + 24);
    if (tpa == spa || tpa == DS_IP) return;  // (the DS's own address: gratuitous ARP, a duplicate address check)
    u8 *r = frame(0x0806, 28);
    if (!r) return;
    put16(r, 1), put16(r + 2, 0x0800), r[4] = 6, r[5] = 4, put16(r + 6, 2);
    memcpy(r + 8, kNatGatewayMac, 6);
    put32(r + 14, tpa);
    memcpy(r + 18, a + 8, 6);
    put32(r + 24, spa);
}

// ---- DHCP
static void dhcp_in(const u8 *d, u32 n) {
    if (n < 240 || d[0] != 1 || get32(d + 236) != 0x63825363) return;
    int type = 0;
    for (u32 i = 240; i + 1 < n && d[i] != 255;) {
        if (d[i] == 0) { i++; continue; }
        if (i + 2 + d[i + 1] > n) break;
        if (d[i] == 53 && d[i + 1] >= 1) type = d[i + 2];
        i += 2 + d[i + 1];
    }
    if (type != 1 && type != 3) return;  // DISCOVER, REQUEST
    u8 r[300];
    memset(r, 0, sizeof r);
    r[0] = 2, r[1] = 1, r[2] = 6;
    memcpy(r + 4, d + 4, 4);    // xid
    memcpy(r + 10, d + 10, 2);  // flags
    put32(r + 16, DS_IP);       // yiaddr
    put32(r + 20, GW_IP);       // siaddr
    memcpy(r + 28, d + 28, 16); // chaddr
    put32(r + 236, 0x63825363);
    u8 *o = r + 240;
    *o++ = 53, *o++ = 1, *o++ = type == 1 ? 2 : 5;  // OFFER / ACK
    *o++ = 54, *o++ = 4, put32(o, GW_IP), o += 4;    // server
    *o++ = 51, *o++ = 4, put32(o, 86400), o += 4;    // lease
    *o++ = 1, *o++ = 4, put32(o, NETMASK), o += 4;
    *o++ = 3, *o++ = 4, put32(o, GW_IP), o += 4;     // router
    *o++ = 6, *o++ = 4, put32(o, sDnsIp), o += 4;    // DNS: Wiimmfi's
    *o++ = 255;
    LOG("DHCP %s: the DS is 10.0.0.10\n", type == 1 ? "offer" : "ack");
    send_udp(GW_IP, 67, 0xffffffffu, 68, r, (u32)(o - r));
}

// ---- UDP: a host socket per DS port
static struct Udp { int fd; u16 dsPort; u64 used; } sUdp[MAX_UDP];

static void set_nonblock(int fd) {
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
    fcntl(fd, F_SETFD, FD_CLOEXEC);
#ifdef SO_NOSIGPIPE
    int one = 1;
    setsockopt(fd, SOL_SOCKET, SO_NOSIGPIPE, &one, sizeof one);
#endif
}

static void udp_out(u16 sport, u32 dst, u16 dport, const u8 *data, u32 n) {
    struct Udp *u = 0;
    for (int i = 0; i < MAX_UDP && !u; i++)
        if (sUdp[i].fd > 0 && sUdp[i].dsPort == sport) u = &sUdp[i];
    if (!u) {
        for (int i = 0; i < MAX_UDP && !u; i++)
            if (sUdp[i].fd <= 0) u = &sUdp[i];
        if (!u) {  // the least recently used one goes
            u = &sUdp[0];
            for (int i = 1; i < MAX_UDP; i++)
                if (sUdp[i].used < u->used) u = &sUdp[i];
            close(u->fd);
        }
        u->fd = socket(AF_INET, SOCK_DGRAM, 0);
        if (u->fd < 0) { u->fd = 0; return; }
        set_nonblock(u->fd);
        u->dsPort = sport;
    }
    u->used = now_ms();
    struct sockaddr_in a = {0};
    a.sin_family = AF_INET;
    a.sin_addr.s_addr = htonl(dst);
    a.sin_port = htons(dport);
    if (sendto(u->fd, data, n, 0, (struct sockaddr *)&a, sizeof a) < 0) LOG("UDP to %08x:%u: %s\n", dst, dport, strerror(errno));
}

static void udp_poll(void) {
    for (int i = 0; i < MAX_UDP; i++) {
        struct Udp *u = &sUdp[i];
        if (u->fd <= 0) continue;
        for (int k = 0; k < 16 && queue_room() > 8; k++) {
            u8 buf[1472];
            struct sockaddr_in from;
            socklen_t fl = sizeof from;
            ssize_t r = recvfrom(u->fd, buf, sizeof buf, 0, (struct sockaddr *)&from, &fl);
            if (r < 0) break;
            u->used = now_ms();
            send_udp(ntohl(from.sin_addr.s_addr), ntohs(from.sin_port), DS_IP, u->dsPort, buf, (u32)r);
        }
        if (now_ms() - u->used > 180000) { close(u->fd); u->fd = 0; }
    }
}

// ---- TCP: each connection ended here
enum { T_FREE, T_CONNECTING, T_SYNACK, T_OPEN, T_CLOSED };
#define F_FIN 1
#define F_SYN 2
#define F_RST 4
#define F_PSH 8
#define F_ACK 16
static struct Tcp {
    int state, fd;
    u16 dsPort, rPort;
    u32 rIp;
    u32 iss, sndUna, sndNxt, rcvNxt;  // (sequence numbers: ours to the DS, the DS's to us)
    u32 dsWnd, mss;
    u8 *out;      // the host's data from sndUna on (unacknowledged or not yet sent), outLen bytes
    u32 outLen;
    u8 *in;       // the DS's data not yet written to the host's socket
    u32 inLen;
    int hostEof, finSent, dsFin, shutWr;
    u64 rto, rtoAt;
    int retries, dupAcks;
} sTcp[MAX_TCP];

static void tcp_seg(struct Tcp *t, u8 flags, u32 seq, const u8 *data, u32 n) {
    u32 opt = (flags & F_SYN) ? 4 : 0;
    u8 *ip = ip_begin(20 + opt + n);
    if (!ip) return;
    u8 *s = ip + 20;
    put16(s, t->rPort), put16(s + 2, t->dsPort);
    put32(s + 4, seq), put32(s + 8, t->rcvNxt);
    s[12] = (u8)((20 + opt) / 4) << 4, s[13] = flags;
    u32 room = TCP_BUF - t->inLen;
    put16(s + 14, room > 0xffff ? 0xffff : room);
    put16(s + 16, 0), put16(s + 18, 0);
    if (opt) s[20] = 2, s[21] = 4, put16(s + 22, MSS_MAX);
    if (n) memcpy(s + 20 + opt, data, n);
    put16(s + 16, l4_csum(6, t->rIp, DS_IP, s, 20 + opt + n));
    ip_finish(ip, 6, t->rIp, DS_IP, 20 + opt + n);
}
static void tcp_rst_reply(u32 src, u16 sport, u32 dst, u16 dport, u32 seq, u32 ack, int withAck) {
    struct Tcp t = {0};
    t.rIp = dst, t.rPort = dport, t.dsPort = sport, t.rcvNxt = ack, t.inLen = TCP_BUF;
    (void)src;
    tcp_seg(&t, (u8)(F_RST | (withAck ? F_ACK : 0)), seq, 0, 0);
}
static void tcp_free(struct Tcp *t) {
    if (t->fd > 0) close(t->fd);
    free(t->out), free(t->in);
    memset(t, 0, sizeof *t);
}

// what may go to the DS now: unsent data within its window, the FIN after the last byte, resends
static void tcp_push(struct Tcp *t) {
    if (t->state != T_OPEN) return;
    u64 now = now_ms();
    if (t->sndNxt != t->sndUna && t->rtoAt && now >= t->rtoAt) {  // not acknowledged in time: from sndUna again
        if (++t->retries > 12) {
            LOG("TCP %u.%u: the DS stopped answering\n", t->rIp, t->rPort);
            tcp_seg(t, F_RST | F_ACK, t->sndNxt, 0, 0);
            tcp_free(t);
            return;
        }
        t->sndNxt = t->sndUna;
        if (t->finSent) t->finSent = 0;
        t->rto = t->rto * 2 > 8000 ? 8000 : t->rto * 2;
        t->rtoAt = 0;
    }
    for (;;) {
        u32 sent = t->sndNxt - t->sndUna, wnd = t->dsWnd ? t->dsWnd : 1;
        if (sent >= t->outLen || sent >= wnd || queue_room() < 4) break;
        u32 n = t->outLen - sent;
        if (n > t->mss) n = t->mss;
        if (n > wnd - sent) n = wnd - sent;
        tcp_seg(t, F_ACK | F_PSH, t->sndNxt, t->out + sent, n);
        t->sndNxt += n;
        if (!t->rtoAt) t->rtoAt = now + t->rto;
    }
    if (t->hostEof && !t->finSent && t->sndNxt - t->sndUna == t->outLen && queue_room() >= 4) {
        tcp_seg(t, F_FIN | F_ACK, t->sndNxt, 0, 0);
        t->sndNxt++;
        t->finSent = 1;
        if (!t->rtoAt) t->rtoAt = now + t->rto;
    }
}

static void tcp_in(u32 src, u32 dst, const u8 *s, u32 n) {
    if (n < 20) return;
    u16 sport = (u16)get16(s), dport = (u16)get16(s + 2);
    u32 seq = get32(s + 4), ack = get32(s + 8), hl = (u32)(s[12] >> 4) * 4;
    u8 fl = s[13];
    u32 wnd = get16(s + 14);
    if (hl < 20 || hl > n) return;
    const u8 *data = s + hl;
    u32 len = n - hl;
    struct Tcp *t = 0;
    for (int i = 0; i < MAX_TCP && !t; i++)
        if (sTcp[i].state && sTcp[i].dsPort == sport && sTcp[i].rIp == dst && sTcp[i].rPort == dport) t = &sTcp[i];
    if (fl & F_RST) {
        if (t) { LOG("TCP to %08x:%u: reset by the DS\n", dst, dport); tcp_free(t); }
        return;
    }
    if (!t) {
        if (!(fl & F_SYN) || (fl & F_ACK)) {
            if (!(fl & F_ACK)) tcp_rst_reply(src, sport, dst, dport, 0, seq + len + ((fl & F_SYN) ? 1 : 0), 1);
            else tcp_rst_reply(src, sport, dst, dport, ack, 0, 0);
            return;
        }
        for (int i = 0; i < MAX_TCP && !t; i++)
            if (!sTcp[i].state) t = &sTcp[i];
        if (!t) { tcp_rst_reply(src, sport, dst, dport, 0, seq + 1, 1); return; }
        memset(t, 0, sizeof *t);
        t->out = malloc(TCP_BUF), t->in = malloc(TCP_BUF);
        t->fd = socket(AF_INET, SOCK_STREAM, 0);
        if (!t->out || !t->in || t->fd < 0) { tcp_free(t); tcp_rst_reply(src, sport, dst, dport, 0, seq + 1, 1); return; }
        set_nonblock(t->fd);
        t->dsPort = sport, t->rIp = dst, t->rPort = dport;
        t->rcvNxt = seq + 1;
        t->iss = (u32)(now_ms() * 2654435761u);
        t->sndUna = t->sndNxt = t->iss;
        t->mss = 536;
        for (u32 i = 20; i < hl;) {  // the DS's MSS
            u8 k = s[i];
            if (k == 0) break;
            if (k == 1) { i++; continue; }
            if (i + 1 >= hl || s[i + 1] < 2) break;
            if (k == 2 && s[i + 1] == 4 && i + 4 <= hl) t->mss = get16(s + i + 2);
            i += s[i + 1];
        }
        if (t->mss > MSS_MAX) t->mss = MSS_MAX;
        t->dsWnd = wnd;
        t->rto = 600;
        struct sockaddr_in a = {0};
        a.sin_family = AF_INET;
        a.sin_addr.s_addr = htonl(dst);
        a.sin_port = htons(dport);
        int r = connect(t->fd, (struct sockaddr *)&a, sizeof a);
        if (r < 0 && errno != EINPROGRESS) {
            LOG("TCP to %08x:%u: %s\n", dst, dport, strerror(errno));
            tcp_rst_reply(src, sport, dst, dport, 0, seq + 1, 1);
            tcp_free(t);
            return;
        }
        t->state = T_CONNECTING;
        t->rtoAt = now_ms() + 15000;  // (the host's connection's time limit)
        LOG("TCP to %d.%d.%d.%d:%u from DS port %u\n", dst >> 24, dst >> 16 & 255, dst >> 8 & 255, dst & 255, dport,
            sport);
        return;
    }
    if (fl & F_SYN) {  // the DS resending its SYN: answer again once connected
        if (t->state == T_SYNACK) tcp_seg(t, F_SYN | F_ACK, t->iss, 0, 0);
        return;
    }
    if (t->state == T_CONNECTING) return;
    if (fl & F_ACK) {
        if (t->state == T_SYNACK && ack == t->iss + 1) {
            t->state = T_OPEN;
            t->sndUna = t->sndNxt = ack;
            t->rtoAt = 0, t->retries = 0;
        } else if (t->state == T_OPEN && (s32)(ack - t->sndUna) > 0 && (s32)(ack - t->sndNxt) <= 0) {
            u32 k = ack - t->sndUna;
            u32 dataAcked = k > t->outLen ? t->outLen : k;  // (the FIN takes a sequence number but no byte)
            memmove(t->out, t->out + dataAcked, t->outLen - dataAcked);
            t->outLen -= dataAcked;
            t->sndUna = ack;
            t->retries = 0;
            t->rtoAt = t->sndUna != t->sndNxt ? now_ms() + t->rto : 0;
            if (t->rto > 600) t->rto = 600;
            t->dupAcks = 0;
        } else if (t->state == T_OPEN && ack == t->sndUna && t->sndNxt != t->sndUna && !len && wnd == t->dsWnd &&
                   ++t->dupAcks == 3) {  // the DS has lost a segment and is still getting the later ones: resend now
            t->sndNxt = t->sndUna;
            if (t->finSent) t->finSent = 0;
            t->rtoAt = 0;
        }
        t->dsWnd = wnd;
    }
    if (t->state != T_OPEN) return;
    int ackNow = 0;
    if (len || (fl & F_FIN)) {
        if (seq == t->rcvNxt) {
            u32 room = TCP_BUF - t->inLen, take = len < room ? len : room;
            memcpy(t->in + t->inLen, data, take);
            t->inLen += take;
            t->rcvNxt += take;
            if ((fl & F_FIN) && take == len && !t->dsFin) {
                t->dsFin = 1;
                t->rcvNxt++;
            }
        } else if ((s32)(seq - t->rcvNxt) < 0 && (s32)(seq + len - t->rcvNxt) > 0) {  // a resend overlapping the new
            u32 skip = t->rcvNxt - seq, room = TCP_BUF - t->inLen, take = len - skip < room ? len - skip : room;
            memcpy(t->in + t->inLen, data + skip, take);
            t->inLen += take;
            t->rcvNxt += take;
        }
        ackNow = 1;  // (also for an old or early segment: the DS learns where we are)
    }
    if (ackNow) tcp_seg(t, F_ACK, t->sndNxt, 0, 0);
    tcp_push(t);
}

static void tcp_poll(void) {
    for (int i = 0; i < MAX_TCP; i++) {
        struct Tcp *t = &sTcp[i];
        if (!t->state) continue;
        if (t->state == T_CONNECTING) {
            struct pollfd p = {t->fd, POLLOUT, 0};
            if (poll(&p, 1, 0) > 0) {
                int err = 0;
                socklen_t el = sizeof err;
                getsockopt(t->fd, SOL_SOCKET, SO_ERROR, &err, &el);
                if (err) {
                    LOG("TCP to %08x:%u: %s\n", t->rIp, t->rPort, strerror(err));
                    tcp_seg(t, F_RST | F_ACK, t->iss, 0, 0);
                    tcp_free(t);
                    continue;
                }
                t->state = T_SYNACK;
                t->rtoAt = now_ms() + t->rto;
                tcp_seg(t, F_SYN | F_ACK, t->iss, 0, 0);
            } else if (now_ms() > t->rtoAt) {
                LOG("TCP to %08x:%u: no answer\n", t->rIp, t->rPort);
                tcp_seg(t, F_RST | F_ACK, t->iss, 0, 0);
                tcp_free(t);
            }
            continue;
        }
        if (t->state == T_SYNACK) {
            if (now_ms() >= t->rtoAt) {
                if (++t->retries > 8) { tcp_free(t); continue; }
                tcp_seg(t, F_SYN | F_ACK, t->iss, 0, 0);
                t->rtoAt = now_ms() + t->rto;
            }
            continue;
        }
        // the DS's data to the host
        while (t->inLen) {
            ssize_t w = send(t->fd, t->in, t->inLen, 0
#ifdef MSG_NOSIGNAL
                             | MSG_NOSIGNAL
#endif
            );
            if (w <= 0) {
                if (w < 0 && errno != EAGAIN && errno != EWOULDBLOCK) {
                    tcp_seg(t, F_RST | F_ACK, t->sndNxt, 0, 0);
                    tcp_free(t);
                }
                break;
            }
            u32 room0 = TCP_BUF - t->inLen;
            memmove(t->in, t->in + w, t->inLen - (u32)w);
            t->inLen -= (u32)w;
            if (room0 < 2 * t->mss) tcp_seg(t, F_ACK, t->sndNxt, 0, 0);  // (the window opened again)
        }
        if (!t->state) continue;
        if (t->dsFin && !t->inLen && !t->shutWr) {
            shutdown(t->fd, SHUT_WR);
            t->shutWr = 1;
        }
        // the host's data to the DS
        while (!t->hostEof && t->outLen < TCP_BUF) {
            ssize_t r = recv(t->fd, t->out + t->outLen, TCP_BUF - t->outLen, 0);
            if (r > 0) { t->outLen += (u32)r; continue; }
            if (r == 0) t->hostEof = 1;
            else if (errno != EAGAIN && errno != EWOULDBLOCK) {
                LOG("TCP to %08x:%u: %s\n", t->rIp, t->rPort, strerror(errno));
                tcp_seg(t, F_RST | F_ACK, t->sndNxt, 0, 0);
                tcp_free(t);
            }
            break;
        }
        if (!t->state) continue;
        tcp_push(t);
        if (t->state && t->finSent && t->sndUna == t->sndNxt && t->dsFin && !t->inLen) tcp_free(t);  // both ends done
    }
}

// ---- from the DS
static void ip_in(const u8 *ip, u32 n) {
    if (n < 20 || ip[0] >> 4 != 4) return;
    u32 hl = (u32)(ip[0] & 15) * 4, tot = get16(ip + 2);
    if (hl < 20 || tot < hl || tot > n) return;
    if (get16(ip + 6) & 0x3fff) return;  // (fragments: the DS sends none)
    u32 src = get32(ip + 12), dst = get32(ip + 16);
    const u8 *p = ip + hl;
    u32 len = tot - hl;
    switch (ip[9]) {
    case 1:  // ICMP: echo answered for any address
        if (len >= 8 && p[0] == 8) {
            u8 *r = ip_begin(len);
            if (!r) return;
            memcpy(r + 20, p, len);
            r[20] = 0, r[22] = 0, r[23] = 0;
            put16(r + 22, csum_fold(csum_add(0, r + 20, len)));
            ip_finish(r, 1, dst, src, len);
        }
        break;
    case 17: {
        if (len < 8) return;
        u16 sport = (u16)get16(p), dport = (u16)get16(p + 2);
        u32 ul = get16(p + 4);
        if (ul < 8 || ul > len) return;
        if (dport == 67) { dhcp_in(p + 8, ul - 8); return; }
        if (dst == GW_IP || dst == 0xffffffffu || (dst & ~NETMASK) == ~NETMASK) return;
        udp_out(sport, dst, dport, p + 8, ul - 8);
        break;
    }
    case 6:
        tcp_in(src, dst, p, len);
        break;
    }
}

void nat_from_ds(const u8 dst[6], const u8 *llc, u32 len) {
    (void)dst;
    if (!sOn || len < 8 || llc[0] != 0xaa || llc[1] != 0xaa) return;
    u32 type = get16(llc + 6);
    if (type == 0x0800) ip_in(llc + 8, len - 8);
    else if (type == 0x0806) arp_in(llc + 8, len - 8);
}

void nat_poll(void) {
    if (!sOn) return;
    udp_poll();
    tcp_poll();
}

void nat_start(const u8 dsMac[6]) {
    if (sLog < 0) sLog = getenv("ACWW_NAT_LOG") ? atoi(getenv("ACWW_NAT_LOG")) : 1;
    nat_stop();
    memcpy(sDsMac, dsMac, 6);
    const char *d = getenv("ACWW_WIIMMFI_DNS");
    struct in_addr a;
    sDnsIp = d && inet_pton(AF_INET, d, &a) == 1 ? ntohl(a.s_addr) : DNS_IP_DEFAULT;
    sOn = 1;
    LOG("the access point is up (Wiimmfi's DNS %d.%d.%d.%d)\n", sDnsIp >> 24, sDnsIp >> 16 & 255, sDnsIp >> 8 & 255,
        sDnsIp & 255);
}

void nat_stop(void) {
    if (!sOn) return;
    for (int i = 0; i < MAX_TCP; i++)
        if (sTcp[i].state) tcp_free(&sTcp[i]);
    for (int i = 0; i < MAX_UDP; i++)
        if (sUdp[i].fd > 0) close(sUdp[i].fd), sUdp[i].fd = 0;
    sQHead = sQCount = 0;
    sOn = 0;
    LOG("the access point is down\n");
}
#endif
