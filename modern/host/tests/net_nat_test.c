// net_nat.c against real sockets on this machine: a made-up DS (DHCP, ARP, a UDP echo, a TCP exchange of 300 KB each
// way with a small window and frames lost both ways). A local echo server stands in for the internet.
//   cc -O1 -Imodern/host -Iinclude modern/host/tests/net_nat_test.c modern/host/net_nat.c -o net_nat_test
#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <signal.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>

#include "net_nat.h"

uint64_t host_plat_time_ns(void) {
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return (uint64_t)t.tv_sec * 1000000000u + (uint64_t)t.tv_nsec;
}

static void put16(u8 *p, u32 v) { p[0] = (u8)(v >> 8), p[1] = (u8)v; }
static void put32(u8 *p, u32 v) { put16(p, v >> 16), put16(p + 2, v); }
static u32 get16(const u8 *p) { return (u32)p[0] << 8 | p[1]; }
static u32 get32(const u8 *p) { return get16(p) << 16 | get16(p + 2); }
static u32 csum_add(u32 s, const u8 *p, u32 n) {
    for (u32 i = 0; i + 1 < n; i += 2) s += get16(p + i);
    if (n & 1) s += (u32)p[n - 1] << 8;
    return s;
}
static u16 fold(u32 s) { while (s >> 16) s = (s & 0xffff) + (s >> 16); return (u16)~s; }

static const u8 kDsMac[6] = {0, 9, 0xbf, 1, 2, 3};
static int fails;
#define CHECK(c, ...) do { if (!(c)) { fails++; printf("FAIL: " __VA_ARGS__); printf("\n"); } } while (0)

static void send_frame(u16 type, const u8 *payload, u32 n) {
    u8 f[1600] = {0xaa, 0xaa, 3, 0, 0, 0};
    put16(f + 6, type);
    memcpy(f + 8, payload, n);
    nat_from_ds(kNatGatewayMac, f, 8 + n);
}
static void send_ip(u8 proto, u32 src, u32 dst, u8 *l4, u32 n) {
    u8 ip[1600];
    memset(ip, 0, 20);
    ip[0] = 0x45, put16(ip + 2, 20 + n), ip[8] = 64, ip[9] = proto;
    put32(ip + 12, src), put32(ip + 16, dst);
    put16(ip + 10, fold(csum_add(0, ip, 20)));
    u8 ph[12];
    put32(ph, src), put32(ph + 4, dst), ph[8] = 0, ph[9] = proto, put16(ph + 10, n);
    int off = proto == 6 ? 16 : 6;
    put16(l4 + off, 0);
    put16(l4 + off, fold(csum_add(csum_add(0, ph, 12), l4, n)));
    memcpy(ip + 20, l4, n);
    send_frame(0x0800, ip, 20 + n);
}
// the next frame from the router (polling a while); its payload after the LLC header
static int recv_frame(u8 *buf, u32 *len, u16 *type, int ms) {
    u8 f[1600];
    u32 n;
    for (int i = 0; i < ms; i++) {
        nat_poll();
        if (nat_take(f, sizeof f, &n)) {
            *type = (u16)get16(f + 6);
            memcpy(buf, f + 8, n - 8);
            *len = n - 8;
            return 1;
        }
        usleep(1000);
    }
    return 0;
}

// ---- the internet: an echo server on 127.0.0.1 (TCP and UDP)
static int sTcpPort, sUdpPort;
static void *tcp_server(void *arg) {
    int l = *(int *)arg;
    int c = accept(l, 0, 0);
    u8 b[8192];
    ssize_t r;
    while ((r = recv(c, b, sizeof b, 0)) > 0) {
        for (ssize_t i = 0; i < r; i++) b[i] ^= 0x5a;  // (echoed changed: the DS must see the server's bytes)
        for (ssize_t w = 0; w < r;) { ssize_t k = send(c, b + w, (size_t)(r - w), 0); if (k <= 0) return 0; w += k; }
    }
    close(c);
    return 0;
}
static void *udp_server(void *arg) {
    int s = *(int *)arg;
    struct timeval tv = {3, 0};  // (never waits for good: a datagram that doesn't come fails the test, not hangs it)
    setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof tv);
    u8 b[2048];
    struct sockaddr_in from;
    socklen_t fl = sizeof from;
    ssize_t r = recvfrom(s, b, sizeof b, 0, (struct sockaddr *)&from, &fl);
    if (r > 0) sendto(s, b, (size_t)r, 0, (struct sockaddr *)&from, fl);
    printf("udp server: %s\n", r > 0 ? "a datagram, echoed" : "nothing came");
    return 0;
}
static int listener(int type, int *port) {
    int s = socket(AF_INET, type, 0);
    struct sockaddr_in a = {0};
    a.sin_family = AF_INET, a.sin_addr.s_addr = htonl(0x7f000001);
    bind(s, (struct sockaddr *)&a, sizeof a);
    socklen_t al = sizeof a;
    getsockname(s, (struct sockaddr *)&a, &al);
    *port = ntohs(a.sin_port);
    if (type == SOCK_STREAM) listen(s, 1);
    return s;
}

static void timeout_handler(int sig) {
    (void)sig;
    static const char m[] = "FAIL: the test took too long\n";
    if (write(1, m, sizeof m - 1) < 0) _exit(3);
    _exit(2);
}

int main(void) {
    setvbuf(stdout, NULL, _IONBF, 0);
    signal(SIGALRM, timeout_handler);
    alarm(90);
    const u32 DS = 0x0a00000a, GW = 0x0a000001, LO = 0x7f000001;
    u8 b[1600];
    u32 n;
    u16 type;
    nat_start(kDsMac);
    // DHCP discover -> offer with the DNS server
    u8 udp[600];
    memset(udp, 0, sizeof udp);
    put16(udp, 68), put16(udp + 2, 67);
    u8 *d = udp + 8;
    d[0] = 1, d[1] = 1, d[2] = 6, put32(d + 4, 0x1234), memcpy(d + 28, kDsMac, 6), put32(d + 236, 0x63825363);
    d[240] = 53, d[241] = 1, d[242] = 1, d[243] = 255;
    put16(udp + 4, 8 + 244);
    send_ip(17, 0, 0xffffffff, udp, 8 + 244);
    CHECK(recv_frame(b, &n, &type, 100) && type == 0x0800 && b[9] == 17, "no DHCP offer");
    CHECK(get32(b + 20 + 8 + 16) == DS, "DHCP: the DS's address");
    {
        int dns = 0;
        for (u32 i = 20 + 8 + 240; i + 5 < n; i += 2 + b[i + 1])
            if (b[i] == 6) dns = (int)(get32(b + i + 2) == 0xb23e2bd4);  // (178.62.43.212)
        CHECK(dns, "DHCP: Wiimmfi's DNS server");
    }
    // ARP for the gateway
    u8 arp[28] = {0, 1, 8, 0, 6, 4, 0, 1};
    memcpy(arp + 8, kDsMac, 6), put32(arp + 14, DS), put32(arp + 24, GW);
    send_frame(0x0806, arp, 28);
    CHECK(recv_frame(b, &n, &type, 100) && type == 0x0806 && !memcmp(b + 8, kNatGatewayMac, 6), "no ARP reply");
    // UDP to the echo server
    int us = listener(SOCK_DGRAM, &sUdpPort);
    pthread_t th;
    pthread_create(&th, 0, udp_server, &us);
    memset(udp, 0, 8);
    put16(udp, 2000), put16(udp + 2, (u32)sUdpPort), put16(udp + 4, 8 + 5);
    memcpy(udp + 8, "hello", 5);
    send_ip(17, DS, LO, udp, 13);
    CHECK(recv_frame(b, &n, &type, 1000) && b[9] == 17 && get32(b + 12) == LO && get16(b + 22) == 2000 &&
              !memcmp(b + 28, "hello", 5),
          "no UDP answer");
    pthread_join(th, 0);
    // TCP: 300 KB to the echo server and back, the DS's window 2920, every 29th frame lost each way
    printf("UDP done\n");
    int ls = listener(SOCK_STREAM, &sTcpPort);
    pthread_create(&th, 0, tcp_server, &ls);
    enum { TOTAL = 300000 };
    u8 *sendData = malloc(TOTAL), *got = malloc(TOTAL);
    for (int i = 0; i < TOTAL; i++) sendData[i] = (u8)(i * 7 + i / 251);
    u32 iss = 1000, snd = iss + 1, sndUna = iss + 1, rcv = 0;
    u32 gotLen = 0, wnd = 2920, frames = 0;
    u8 seg[1600];
    memset(seg, 0, 24);
    put16(seg, 3000), put16(seg + 2, (u32)sTcpPort), put32(seg + 4, iss);
    seg[12] = 6 << 4, seg[13] = 2, put16(seg + 14, wnd), seg[20] = 2, seg[21] = 4, put16(seg + 22, 1360);
    send_ip(6, DS, LO, seg, 24);
    int ok = recv_frame(b, &n, &type, 2000) && (b[20 + 13] & 0x12) == 0x12;
    CHECK(ok, "no SYN-ACK");
    rcv = get32(b + 20 + 4) + 1;
    u64 last = host_plat_time_ns();
    while (ok && (gotLen < TOTAL || sndUna < iss + 1 + TOTAL)) {
        // send what the window of 4 segments allows
        while (snd - (iss + 1) < TOTAL && snd - sndUna < 4 * 1360) {
            u32 off = snd - (iss + 1), len = TOTAL - off < 1360 ? TOTAL - off : 1360;
            memset(seg, 0, 20);
            put16(seg, 3000), put16(seg + 2, (u32)sTcpPort), put32(seg + 4, snd), put32(seg + 8, rcv);
            seg[12] = 5 << 4, seg[13] = 0x18, put16(seg + 14, wnd);
            memcpy(seg + 20, sendData + off, len);
            if (++frames % 29) send_ip(6, DS, LO, seg, 20 + len);  // (every 29th lost)
            snd += len;
        }
        if (!recv_frame(b, &n, &type, 300)) {
            if (host_plat_time_ns() - last > 20000000000ull) break;
            snd = sndUna;  // (the DS's own resend)
            continue;
        }
        if (++frames % 29 == 0) continue;  // (lost on the way to the DS)
        last = host_plat_time_ns();
        const u8 *t = b + 20;
        u32 hl = (u32)(t[12] >> 4) * 4, len = get16(b + 2) - 20 - hl, seq = get32(t + 4), ack = get32(t + 8);
        if (t[13] & 4) { CHECK(0, "TCP reset"); break; }
        if ((t[13] & 16) && (s32)(ack - sndUna) > 0) sndUna = ack;
        if (len && seq == rcv && gotLen + len <= TOTAL) {
            memcpy(got + gotLen, t + hl, len);
            gotLen += len;
            rcv += len;
        }
        if (len) {  // acknowledge (the window as said)
            memset(seg, 0, 20);
            put16(seg, 3000), put16(seg + 2, (u32)sTcpPort), put32(seg + 4, snd), put32(seg + 8, rcv);
            seg[12] = 5 << 4, seg[13] = 0x10, put16(seg + 14, wnd);
            send_ip(6, DS, LO, seg, 20);
        }
    }
    u32 bad = 0;
    for (u32 i = 0; i < gotLen; i++) bad += got[i] != (sendData[i] ^ 0x5a);
    printf("TCP: %u of %u bytes back, %u wrong, %u frames\n", gotLen, TOTAL, bad, frames);
    CHECK(gotLen == TOTAL && !bad, "TCP data");
    nat_stop();
    printf(fails ? "FAILED\n" : "ok\n");
    return fails != 0;
}
