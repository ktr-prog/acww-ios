// Host build, iOS: the app's entry point and its few Objective-C needs (ios.h).
//
// main() hands the process to UIKit through SDL (SDL_UIKitRunApp), which calls ios_main once the app has launched,
// on the main thread; ios_main never returns while the game runs. The game is a .nds file in the app's Documents
// folder: with UIFileSharingEnabled and LSSupportsOpeningDocumentsInPlace (Info.plist) the Files app shows that folder
// as "On My iPhone / On My iPad > ACWW", and a Mac's Finder shows it under the device's Files tab. The save file is
// written next to the game (<name>.sav, pxi7.c), so it is in the same folder, where it can be backed up.
#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_syswm.h>

#import <CommonCrypto/CommonDigest.h>
#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

#include <stdio.h>
#include <string.h>

#include "ios.h"

// the supported version (README.md): USA (Rev 1)
static NSString *const kRomSha1 = @"77fde3e30e1e6068395d1f96ea63be569b61c351";
static NSString *const kReadmeName = @"Put your Animal Crossing Wild World .nds file here.txt";

void ios_safe_area(SDL_Window *window, float out[4]) {
    out[0] = out[1] = out[2] = out[3] = 0;
    SDL_SysWMinfo wm;
    SDL_VERSION(&wm.version);
    if (!window || !SDL_GetWindowWMInfo(window, &wm) || wm.subsystem != SDL_SYSWM_UIKIT) return;
    UIWindow *w = wm.info.uikit.window;
    if (!w) return;
    UIEdgeInsets e = w.safeAreaInsets;
    out[0] = (float)e.left, out[1] = (float)e.top, out[2] = (float)e.right, out[3] = (float)e.bottom;
}

void ios_fatal_message(const char *text) {
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "ACWW", text, NULL);
}

// a message box with up to three buttons; the index of the one pressed (the first: the default)
static int ask(SDL_MessageBoxFlags kind, const char *title, const char *text, const char *const *buttons, int n) {
    SDL_MessageBoxButtonData b[3];
    memset(b, 0, sizeof b);
    for (int i = 0; i < n && i < 3; i++) {
        b[i].buttonid = i;
        b[i].text = buttons[i];
        b[i].flags = i == 0 ? SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT : 0;
    }
    SDL_MessageBoxData d;
    memset(&d, 0, sizeof d);
    d.flags = kind;
    d.title = title;
    d.message = text;
    d.numbuttons = n;
    d.buttons = b;
    int pressed = 0;
    if (SDL_ShowMessageBox(&d, &pressed) != 0 || pressed < 0) pressed = 0;
    return pressed;
}

static NSString *sha1_of(NSString *path) {
    NSData *data = [NSData dataWithContentsOfFile:path options:NSDataReadingMappedIfSafe error:nil];
    if (!data) return nil;
    unsigned char md[CC_SHA1_DIGEST_LENGTH];
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
    CC_SHA1(data.bytes, (CC_LONG)data.length, md);  // (only to recognise the game's dump, not for security)
#pragma clang diagnostic pop
    NSMutableString *s = [NSMutableString stringWithCapacity:40];
    for (int i = 0; i < CC_SHA1_DIGEST_LENGTH; i++) [s appendFormat:@"%02x", md[i]];
    return s;
}

// the .nds files in the folder, by name
static NSArray<NSString *> *roms_in(NSString *dir) {
    NSArray<NSString *> *names = [[NSFileManager defaultManager] contentsOfDirectoryAtPath:dir error:nil];
    NSMutableArray<NSString *> *roms = [NSMutableArray array];
    for (NSString *n in names) {
        if ([n.pathExtension caseInsensitiveCompare:@"nds"] == NSOrderedSame && ![n hasPrefix:@"."])
            [roms addObject:[dir stringByAppendingPathComponent:n]];
    }
    return [roms sortedArrayUsingSelector:@selector(localizedStandardCompare:)];
}

// the game's path, once the player has put it in the Documents folder; asks until then
static NSString *find_rom(void) {
    NSString *docs = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES).firstObject;
    if (!docs) docs = [NSHomeDirectory() stringByAppendingPathComponent:@"Documents"];
    NSFileManager *fm = [NSFileManager defaultManager];
    [fm createDirectoryAtPath:docs withIntermediateDirectories:YES attributes:nil error:nil];
    // (a file in the folder, so that the Files app lists it from the start)
    NSString *readme = [docs stringByAppendingPathComponent:kReadmeName];
    if (![fm fileExistsAtPath:readme]) {
        NSString *text = @"Copy your own dump of Animal Crossing: Wild World (USA, Rev 1) into this folder - one "
                         @"file ending in .nds - then go back to the app.\n\nThe game saves next to it (same name, "
                         @"ending in .sav). Keep a copy of that file somewhere safe.\n";
        [text writeToFile:readme atomically:YES encoding:NSUTF8StringEncoding error:nil];
    }
    for (;;) {
        NSArray<NSString *> *roms = roms_in(docs);
        if (roms.count == 0) {
            static const char *const kButtons[] = {"I've added it - check again"};
            ask(SDL_MESSAGEBOX_INFORMATION, "Add your game",
                "No game found.\n\nOpen the Files app, go to On My iPhone (or On My iPad) > ACWW, and copy your "
                "Animal Crossing: Wild World (USA) .nds file there. Then come back here.",
                kButtons, 1);
            continue;
        }
        for (NSString *r in roms) {
            NSString *sha = sha1_of(r);
            if (sha && [sha isEqualToString:kRomSha1]) return r;
        }
        // none is the supported dump: say so, and let the player try the first one anyway
        NSString *text = [NSString stringWithFormat:
            @"\"%@\" isn't the supported version of the game (USA, Rev 1 - SHA-1 %@). Other versions or modified "
            @"dumps will most likely not work.", roms[0].lastPathComponent, kRomSha1];
        static const char *const kButtons[] = {"Check again", "Try it anyway"};
        if (ask(SDL_MESSAGEBOX_WARNING, "Unsupported game file", text.UTF8String, kButtons, 2) == 1) return roms[0];
    }
}

static int ios_main(int argc, char *argv[]) {
    (void)argc;
    NSString *rom;
    @autoreleasepool {
        rom = find_rom();
    }
    static char path[4096];
    snprintf(path, sizeof path, "%s", rom.fileSystemRepresentation);
    static char *args[3];
    args[0] = argv[0];
    args[1] = path;
    args[2] = NULL;
    fprintf(stderr, "ios: the game: %s\n", path);
    return host_main(2, args);
}

int main(int argc, char *argv[]) {
    return SDL_UIKitRunApp(argc, argv, ios_main);
}
