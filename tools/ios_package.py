#!/usr/bin/env python3
"""Package the iOS build (tools/configure_host.py --os ios) as an app and an unsigned .ipa for sideloading.

    python3 tools/ios_package.py build/hostios_usa/acww build/ios/ACWW.ipa [--version 1.0] [--build 1] [--sign-adhoc]

Writes <out dir>/Payload/ACWW.app (the program, Info.plist, the icons, PkgInfo) and zips it as the .ipa. The app is
not signed with a developer certificate: the sideloading tool (AltStore, SideStore, Sideloadly, ...) signs it with the
installing Apple ID. --sign-adhoc gives it an ad-hoc signature first (codesign, on a Mac), which those tools replace.
Only the Python standard library is used (the icons are drawn here, see draw_icon). See docs/ios.md.
"""
import argparse
import os
import plistlib
import shutil
import stat
import struct
import subprocess
import sys
import time
import zipfile
import zlib
from pathlib import Path

APP_NAME = "ACWW"
EXECUTABLE = "acww"
BUNDLE_ID = "io.github.acww-native.acww"
MIN_IOS = "14.0"

# the icons: (file name, pixels); iOS picks them by CFBundleIconFiles' base names and the @2x/@3x/~ipad suffixes
ICONS = [
    ("AppIcon60x60@2x.png", 120),
    ("AppIcon60x60@3x.png", 180),
    ("AppIcon76x76@2x~ipad.png", 152),
    ("AppIcon83.5x83.5@2x~ipad.png", 167),
]


def info_plist(version: str, build: str) -> dict:
    orient = ["UIInterfaceOrientationPortrait", "UIInterfaceOrientationLandscapeLeft",
              "UIInterfaceOrientationLandscapeRight"]
    return {
        "CFBundleDevelopmentRegion": "en",
        "CFBundleDisplayName": APP_NAME,
        "CFBundleExecutable": EXECUTABLE,
        "CFBundleIdentifier": BUNDLE_ID,
        "CFBundleInfoDictionaryVersion": "6.0",
        "CFBundleName": APP_NAME,
        "CFBundlePackageType": "APPL",
        "CFBundleShortVersionString": version,
        "CFBundleVersion": build,
        "CFBundleSupportedPlatforms": ["iPhoneOS"],
        "CFBundleIcons": {"CFBundlePrimaryIcon": {"CFBundleIconFiles": ["AppIcon60x60"]}},
        "CFBundleIcons~ipad": {"CFBundlePrimaryIcon": {"CFBundleIconFiles": ["AppIcon60x60", "AppIcon76x76",
                                                                              "AppIcon83.5x83.5"]}},
        "DTPlatformName": "iphoneos",
        "LSRequiresIPhoneOS": True,
        "MinimumOSVersion": MIN_IOS,
        "UIDeviceFamily": [1, 2],
        "UIRequiredDeviceCapabilities": ["arm64"],
        # (a launch screen: without one iOS runs the app in an old phone's resolution, letterboxed)
        "UILaunchScreen": {},
        "UIRequiresFullScreen": True,
        "UIStatusBarHidden": True,
        "UISupportedInterfaceOrientations": orient,
        "UISupportedInterfaceOrientations~ipad": orient[:1] + ["UIInterfaceOrientationPortraitUpsideDown"] + orient[1:],
        # the Documents folder in the Files app (On My iPhone > ACWW): the game goes there, the save is written there
        "UIFileSharingEnabled": True,
        "LSSupportsOpeningDocumentsInPlace": True,
        # (DS-to-DS play looks for other copies on the local network)
        "NSLocalNetworkUsageDescription": "DS-to-DS play looks for other copies of the game on your local network.",
        # (SDL can reach some controllers over Bluetooth itself; the app uses Apple's controller support instead,
        # but iOS ends any app that touches Bluetooth without this text)
        "NSBluetoothAlwaysUsageDescription": "Bluetooth is used for game controllers.",
    }


# ---- the icon: two rounded screens, as the DS's, on a green ground (drawn here: no image libraries needed)
def draw_icon(n: int) -> bytes:
    def rr(x, y, x0, y0, x1, y1, r):  # inside a rounded rectangle: coverage 0..1 (2x2 samples)
        c = 0
        for sy in (0.25, 0.75):
            for sx in (0.25, 0.75):
                px, py = x + sx, y + sy
                qx = max(x0 + r - px, 0, px - (x1 - r))
                qy = max(y0 + r - py, 0, py - (y1 - r))
                if px >= x0 and px <= x1 and py >= y0 and py <= y1 and qx * qx + qy * qy <= r * r:
                    c += 1
        return c / 4

    rows = []
    s = n / 120.0
    w_scr, h_scr = 64 * s, 40 * s
    x0 = (n - w_scr) / 2
    top = (17 * s, 17 * s + h_scr)
    bottom = (63 * s, 63 * s + h_scr)
    for y in range(n):
        row = bytearray([0])  # (PNG filter: none)
        for x in range(n):
            t = y / n
            # the ground: a soft green, lighter at the top
            r, g, b = 86 + (40 * (1 - t)), 168 + (40 * (1 - t)), 96 + (30 * (1 - t))
            for (ya, yb), (cr, cg, cb) in ((top, (255, 252, 236)), (bottom, (255, 252, 236))):
                frame = rr(x, y, x0, ya, x0 + w_scr, yb, 7 * s)
                if frame:
                    r, g, b = r + (60 - r) * frame * 0.9, g + (92 - g) * frame * 0.9, b + (70 - b) * frame * 0.9
                inner = rr(x, y, x0 + 5 * s, ya + 5 * s, x0 + w_scr - 5 * s, yb - 5 * s, 3 * s)
                if inner:
                    r, g, b = r + (cr - r) * inner, g + (cg - g) * inner, b + (cb - b) * inner
            row += bytes((int(min(255, max(0, r))), int(min(255, max(0, g))), int(min(255, max(0, b)))))
        rows.append(bytes(row))

    def chunk(kind, data):
        return struct.pack(">I", len(data)) + kind + data + struct.pack(">I", zlib.crc32(kind + data) & 0xffffffff)

    return (b"\x89PNG\r\n\x1a\n" + chunk(b"IHDR", struct.pack(">IIBBBBB", n, n, 8, 2, 0, 0, 0))
            + chunk(b"IDAT", zlib.compress(b"".join(rows), 9)) + chunk(b"IEND", b""))


def main():
    p = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    p.add_argument("binary", help="the iOS build's program (build/hostios_usa/acww)")
    p.add_argument("ipa", help="the .ipa to write")
    p.add_argument("--version", default="1.0", help="CFBundleShortVersionString")
    p.add_argument("--build", default="1", help="CFBundleVersion")
    p.add_argument("--sign-adhoc", action="store_true", help="an ad-hoc signature (codesign, macOS only)")
    args = p.parse_args()

    binary = Path(args.binary)
    if not binary.is_file():
        sys.exit(f"ios_package.py: no {binary} (build it first: docs/ios.md)")
    with open(binary, "rb") as f:
        magic = f.read(4)
    if magic != b"\xcf\xfa\xed\xfe":  # (MH_MAGIC_64, little-endian: an arm64 Mach-O)
        sys.exit(f"ios_package.py: {binary} isn't a 64-bit Mach-O program")

    ipa = Path(args.ipa)
    stage = ipa.parent / "Payload"
    app = stage / f"{APP_NAME}.app"
    if stage.exists():
        shutil.rmtree(stage)
    app.mkdir(parents=True)
    shutil.copyfile(binary, app / EXECUTABLE)
    os.chmod(app / EXECUTABLE, 0o755)
    with open(app / "Info.plist", "wb") as f:
        plistlib.dump(info_plist(args.version, args.build), f, fmt=plistlib.FMT_XML)
    (app / "PkgInfo").write_bytes(b"APPL????")
    for name, n in ICONS:
        (app / name).write_bytes(draw_icon(n))

    if args.sign_adhoc:
        subprocess.run(["codesign", "--force", "--sign", "-", "--timestamp=none", str(app)], check=True)

    # the .ipa: a zip of Payload/ (the program keeps its executable permission)
    if ipa.exists():
        ipa.unlink()
    now = time.localtime()[:6]
    with zipfile.ZipFile(ipa, "w", zipfile.ZIP_DEFLATED) as z:
        for root, dirs, files in os.walk(stage):
            dirs.sort()
            rel_root = Path(root).relative_to(ipa.parent)
            zi = zipfile.ZipInfo(str(rel_root).replace(os.sep, "/") + "/", date_time=now)
            zi.external_attr = (stat.S_IFDIR | 0o755) << 16
            z.writestr(zi, b"")
            for name in sorted(files):
                path = Path(root) / name
                zi = zipfile.ZipInfo(str(rel_root / name).replace(os.sep, "/"), date_time=now)
                mode = os.stat(path).st_mode & 0o777
                zi.external_attr = (stat.S_IFREG | mode) << 16
                zi.compress_type = zipfile.ZIP_DEFLATED
                z.writestr(zi, path.read_bytes())
    print(f"ios_package.py: {ipa} ({ipa.stat().st_size // 1024} KB)")


if __name__ == "__main__":
    main()
