#!/usr/bin/env python3
"""A stand-in for Nintendo WFC (Wiimmfi), for testing the game's connection on a machine without the real servers.

Serves, on this machine:
  - DNS (UDP 53 on 127.0.0.1): every name is this machine's address (--ip)
  - conntest.nintendowifi.net (HTTP, port 80)
  - nas.nintendowifi.net (HTTPS, port 443): SSL 3.0 with RC4, a certificate from a test "Nintendo CA"
    (--certdir: srv.key/srv.der and, for a chain, inter.der, made with openssl; the game trusts that CA with ACWW_TEST_CA)
  - GameSpy's login server, gpcm (TCP 29900), and its profile server, gpsp (TCP 29901): enough of them to log in
  - the availability check (UDP 27900)

Run as root (ports below 1024), then the game with
  ACWW_WIIMMFI_DNS=127.0.0.1 ACWW_TEST_CA=$(cat <certdir>/ca_modulus.hex) ACWW_WFC_TEST=<frame> ...
Everything that happens is written to stdout.
"""
import argparse, base64, hashlib, os, random, socket, struct, subprocess, threading, time, urllib.parse

ap = argparse.ArgumentParser()
ap.add_argument('--ip', default='192.0.2.2')
ap.add_argument('--certdir', default='.')
args = ap.parse_args()


def log(*a):
    print(time.strftime('%H:%M:%S'), *a, flush=True)


# ---- DNS
def dns():
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    s.bind(('127.0.0.1', 53))
    while True:
        d, a = s.recvfrom(512)
        i, name = 12, []
        while d[i]:
            name.append(d[i + 1:i + 1 + d[i]].decode())
            i += 1 + d[i]
        q = d[12:i + 5]
        log('dns', '.'.join(name))
        r = d[:2] + b'\x81\x80' + struct.pack('>HHHH', 1, 1, 0, 0) + q + b'\xc0\x0c' + struct.pack('>HHIH', 1, 1, 60, 4) \
            + socket.inet_aton(args.ip)
        s.sendto(r, a)


# ---- conntest
def http():
    s = socket.socket()
    s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    s.bind(('0.0.0.0', 80))
    s.listen(5)
    while True:
        c, a = s.accept()
        d = c.recv(4096)
        log('http', d[:120])
        body = b'<html><body>This is a test.</body></html>'
        c.sendall(b'HTTP/1.1 200 OK\r\nX-Organization: Nintendo\r\nContent-Type: text/html\r\nContent-Length: %d\r\n'
                  b'Connection: close\r\n\r\n' % len(body) + body)
        c.close()


# ---- SSL 3.0, RC4 (the DS's client offers RC4-MD5 and RC4-SHA)
def rsa_key(path):
    out = subprocess.run(['openssl', 'rsa', '-in', path, '-noout', '-text'], capture_output=True, text=True).stdout
    def field(name):
        lines = out.split(name + ':')[1].split('\n')[1:]
        h = ''
        for l in lines:
            if not l.startswith('    '):
                break
            h += l.strip().replace(':', '')
        return int(h, 16)
    return field('modulus'), field('privateExponent')


N, D = rsa_key(os.path.join(args.certdir, 'srv.key'))
CERTS = [open(os.path.join(args.certdir, 'srv.der'), 'rb').read()]
if os.path.exists(os.path.join(args.certdir, 'inter.der')):  # (a chain: the server's certificate, then its issuer's)
    CERTS.append(open(os.path.join(args.certdir, 'inter.der'), 'rb').read())


class RC4:
    def __init__(self, key):
        S = list(range(256))
        j = 0
        for i in range(256):
            j = (j + S[i] + key[i % len(key)]) & 255
            S[i], S[j] = S[j], S[i]
        self.S, self.i, self.j = S, 0, 0

    def __call__(self, data):
        S, i, j, out = self.S, self.i, self.j, bytearray()
        for b in data:
            i = (i + 1) & 255
            j = (j + S[i]) & 255
            S[i], S[j] = S[j], S[i]
            out.append(b ^ S[(S[i] + S[j]) & 255])
        self.i, self.j = i, j
        return bytes(out)


def md5(b): return hashlib.md5(b).digest()
def sha(b): return hashlib.sha1(b).digest()


def ssl3_prf(secret, r1, r2, n):
    out, k = b'', 0
    while len(out) < n:
        label = bytes([65 + k]) * (k + 1)
        out += md5(secret + sha(label + secret + r1 + r2))
        k += 1
    return out[:n]


class Ssl3:
    def __init__(self, c):
        self.c, self.buf, self.hs = c, b'', b''
        self.rd = self.wr = None
        self.rseq = self.wseq = 0

    def recv_record(self):
        while len(self.buf) < 5 or len(self.buf) < 5 + struct.unpack('>H', self.buf[3:5])[0]:
            d = self.c.recv(65536)
            if not d:
                raise EOFError
            self.buf += d
        t, n = self.buf[0], struct.unpack('>H', self.buf[3:5])[0]
        body, self.buf = self.buf[5:5 + n], self.buf[5 + n:]
        if self.rd:
            body = self.rd(body)
            body, mac = body[:-self.maclen], body[-self.maclen:]
            if mac != self.mac(self.cmac, self.rseq, t, body):
                log('ssl: bad MAC from the DS')
            self.rseq += 1
        return t, body

    def send_record(self, t, body):
        if self.wr:
            body = self.wr(body + self.mac(self.smac, self.wseq, t, body))
            self.wseq += 1
        self.c.sendall(bytes([t]) + b'\x03\x00' + struct.pack('>H', len(body)) + body)

    def mac(self, key, seq, t, data):
        h, p = (sha, 40) if self.maclen == 20 else (md5, 48)
        return h(key + b'\x5c' * p + h(key + b'\x36' * p + struct.pack('>QBH', seq, t, len(data)) + data))

    def finished(self, sender):
        m = self.master
        return (md5(m + b'\x5c' * 48 + md5(self.hs + sender + m + b'\x36' * 48)) +
                sha(m + b'\x5c' * 40 + sha(self.hs + sender + m + b'\x36' * 40)))

    def handshake(self):
        t, ch = self.recv_record()
        assert t == 22 and ch[0] == 1, 'not a ClientHello'
        self.hs += ch
        cr = ch[6:38]
        sl = ch[38]
        ncs = struct.unpack('>H', ch[39 + sl:41 + sl])[0]
        suites = [struct.unpack('>H', ch[41 + sl + 2 * i:43 + sl + 2 * i])[0] for i in range(ncs // 2)]
        suite = 5 if 5 in suites else 4
        self.maclen = 20 if suite == 5 else 16
        log('ssl: ClientHello, suites %s, using %#x' % ([hex(x) for x in suites], suite))
        sr = os.urandom(32)
        sh = b'\x03\x00' + sr + b'\x20' + os.urandom(32) + struct.pack('>HB', suite, 0)
        msgs = bytes([2]) + struct.pack('>I', len(sh))[1:] + sh
        cl = b''.join(struct.pack('>I', len(x))[1:] + x for x in CERTS)
        cl = struct.pack('>I', len(cl))[1:] + cl
        msgs += bytes([11]) + struct.pack('>I', len(cl))[1:] + cl
        msgs += b'\x0e\x00\x00\x00'
        self.hs += msgs
        self.send_record(22, msgs)
        # ClientKeyExchange
        t, cke = self.recv_record()
        if t != 22:
            log('ssl: the DS sent record type %d instead of its key (alert %s)' % (t, cke.hex()))
            raise EOFError
        self.hs += cke[:4 + int.from_bytes(cke[1:4], 'big')]
        enc = cke[4:4 + int.from_bytes(cke[1:4], 'big')]
        m = pow(int.from_bytes(enc, 'big'), D, N).to_bytes(128, 'big')
        assert m[0:2] == b'\x00\x02', 'bad premaster padding'
        pre = m[m.index(b'\x00', 2) + 1:]
        assert len(pre) == 48 and pre[:2] == b'\x03\x00', 'bad premaster'
        self.master = ssl3_prf(pre, cr, sr, 48)
        kb = ssl3_prf(self.master, sr, cr, 2 * self.maclen + 32)
        self.cmac, self.smac = kb[:self.maclen], kb[self.maclen:2 * self.maclen]
        ck, sk = kb[2 * self.maclen:2 * self.maclen + 16], kb[2 * self.maclen + 16:2 * self.maclen + 32]
        t, ccs = self.recv_record()
        assert t == 20, 'no ChangeCipherSpec'
        self.rd = RC4(ck)
        expect = self.finished(b'CLNT')
        t, fin = self.recv_record()
        ok = t == 22 and fin[0] == 20 and fin[4:] == expect
        log('ssl: the DS\'s Finished', 'is right' if ok else 'is WRONG (%s)' % fin.hex())
        self.hs += fin
        self.send_record(20, b'\x01')
        self.wr = RC4(sk)
        f = self.finished(b'SRVR')
        self.send_record(22, b'\x14\x00\x00\x24' + f)

    def read_http(self):
        data = b''
        while b'\r\n\r\n' not in data:
            t, b = self.recv_record()
            data += b
        head, body = data.split(b'\r\n\r\n', 1)
        n = 0
        for l in head.split(b'\r\n'):
            if l.lower().startswith(b'content-length:'):
                n = int(l.split(b':')[1])
        while len(body) < n:
            t, b = self.recv_record()
            body += b
        return head, body


def nas_b64(s):
    return base64.b64encode(s.encode() if isinstance(s, str) else s).decode().replace('=', '*')


def nas_unb64(s):
    return base64.b64decode(s.replace('*', '=')).decode('latin-1')


def nas_answer(fields):
    action = fields.get('action', '')
    now = time.strftime('%Y%m%d%H%M%S')
    if action == 'acctcreate':
        r = {'retry': '0', 'returncd': '002', 'userid': '1234567', 'datetime': now}
    elif action == 'login':
        r = {'challenge': 'TESTCHAL', 'locator': 'gamespy.com', 'retry': '0', 'returncd': '001',
             'token': 'NDS' + 'a' * 93, 'datetime': now}
    else:
        r = {'retry': '0', 'returncd': '001', 'datetime': now}
    return '&'.join('%s=%s' % (k, nas_b64(v)) for k, v in r.items()).encode()


def nas_conn(c):
    s = Ssl3(c)
    try:
        s.handshake()
        head, body = s.read_http()
        log('nas: request', head.split(b'\r\n')[0])
        fields = {k: nas_unb64(v) for k, v in urllib.parse.parse_qsl(body.decode())}
        log('nas: fields', fields)
        ans = nas_answer(fields)
        log('nas: answer', ans)
        resp = b'HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: %d\r\nConnection: close\r\n\r\n' % len(ans) + ans
        s.send_record(23, resp)
        time.sleep(1)
    except (EOFError, AssertionError, ConnectionError) as e:
        log('nas: ended:', repr(e))
    c.close()


def nas():
    s = socket.socket()
    s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    s.bind(('0.0.0.0', 443))
    s.listen(5)
    while True:
        c, a = s.accept()
        threading.Thread(target=nas_conn, args=(c,), daemon=True).start()


# ---- GameSpy
def gs_parse(msg):
    p = msg.split('\\')[1:]
    return dict(zip(p[0::2], p[1::2]))


def gs_md5(s):
    return hashlib.md5(s.encode('latin-1')).hexdigest()


PROFILE = {'lastname': ''}


def gpcm_conn(c):
    srvchal = ''.join(random.choice('ABCDEFGHIJKLMNOPQRSTUVWXYZ') for _ in range(10))
    c.sendall(('\\lc\\1\\challenge\\%s\\id\\1\\final\\' % srvchal).encode())
    buf = ''
    try:
        while True:
            d = c.recv(4096)
            if not d:
                break
            buf += d.decode('latin-1')
            while '\\final\\' in buf:
                msg, buf = buf.split('\\final\\', 1)
                f = gs_parse(msg)
                cmd = msg.split('\\')[1] if msg.count('\\') > 1 else ''
                log('gpcm: <-', msg[:300])
                if cmd == 'login':
                    PROFILE['lastname'] = ''  # (a new profile for each login: one stand-in profile, many test runs)
                    pw = gs_md5('TESTCHAL')  # (the NAS challenge is the password)
                    user = f.get('authtoken', f.get('uniquenick', ''))
                    proof = gs_md5(pw + ' ' * 48 + user + srvchal + f.get('challenge', '') + pw)
                    want = gs_md5(pw + ' ' * 48 + user + f.get('challenge', '') + srvchal + pw)
                    log('gpcm: the DS\'s response', 'is right' if want == f.get('response') else 'is WRONG')
                    r = ('\\lc\\2\\sesskey\\12345678\\proof\\%s\\userid\\1234567\\profileid\\7654321\\uniquenick\\testnick'
                         '\\lt\\%s\\id\\%s\\final\\' % (proof, 'x' * 22 + '__', f.get('id', '1')))
                    c.sendall(r.encode())
                    log('gpcm: ->', r)
                elif cmd == 'updatepro':
                    PROFILE.update({k: v for k, v in f.items() if k not in ('updatepro', 'sesskey', 'id')})
                elif cmd == 'getprofile':
                    r = ('\\pi\\\\profileid\\%s\\nick\\testnick\\userid\\1234567\\email\\test@test.com\\sig\\%s'
                         '\\uniquenick\\testnick\\pid\\0\\lastname\\%s\\lon\\0.000000\\lat\\0.000000\\loc\\\\id\\%s\\final\\'
                         % (f.get('profileid', '0'), '0' * 32, PROFILE['lastname'], f.get('id', '1')))
                    c.sendall(r.encode())
                    log('gpcm: ->', r)
                elif cmd == 'ka':
                    c.sendall(b'\\ka\\\\final\\')
    except ConnectionError as e:
        log('gpcm: ended:', repr(e))
    log('gpcm: closed')
    c.close()


def tcp_server(port, fn):
    s = socket.socket()
    s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    s.bind(('0.0.0.0', port))
    s.listen(5)
    while True:
        c, a = s.accept()
        log('tcp %d: connection' % port)
        threading.Thread(target=fn, args=(c,), daemon=True).start()


def gpsp_conn(c):
    try:
        while True:
            d = c.recv(4096)
            if not d:
                break
            log('gpsp: <-', d[:200])
            c.sendall(b'\\otherslist\\\\oldone\\\\final\\')
    except ConnectionError:
        pass
    c.close()


def udp_27900():
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    s.bind(('0.0.0.0', 27900))
    while True:
        d, a = s.recvfrom(2048)
        log('udp 27900 <-', d[:80])
        if d[:1] == b'\x09':  # the availability check
            s.sendto(b'\xfe\xfd\x09\x00\x00\x00\x00', a)
        elif d[:1] == b'\x03' or d[:1] == b'\x08':  # qr2 heartbeat / keepalive
            s.sendto(b'\xfe\xfd\x01' + d[1:5] + b'TESTCHALLENGE\x00', a)


for f in (dns, http, nas, udp_27900):
    threading.Thread(target=f, daemon=True).start()
threading.Thread(target=tcp_server, args=(29900, gpcm_conn), daemon=True).start()
threading.Thread(target=tcp_server, args=(29901, gpsp_conn), daemon=True).start()
log('serving (names answer %s)' % args.ip)
while True:
    time.sleep(60)
