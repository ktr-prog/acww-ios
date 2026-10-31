// Host build: the optional features (host_features.h).
#include "host_features.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const struct {
    const char *key, *name;
} kFeatures[FEAT_COUNT] = {
    [FEAT_CIRCLE_PAD] = {"circle_pad", "CIRCLE PAD"},
    [FEAT_FPS] = {"fps", "FPS COUNTER"},
    [FEAT_CPU3D] = {"cpu3d", "CPU 3D (RESTART)"},
};
static int sOn[FEAT_COUNT];
static char sPath[1024];

static void save(void) {
    if (!sPath[0]) return;
    char tmp[1100];
    snprintf(tmp, sizeof tmp, "%s.tmp", sPath);
    FILE *f = fopen(tmp, "w");
    if (!f) { fprintf(stderr, "features: can't write %s\n", tmp); return; }
    fprintf(f, "# Animal Crossing: Wild World (native): optional features, 1 on, 0 off (the settings screen)\n");
    for (int i = 0; i < FEAT_COUNT; i++) fprintf(f, "%s=%d\n", kFeatures[i].key, sOn[i]);
    if (fclose(f) != 0 || rename(tmp, sPath) != 0) fprintf(stderr, "features: can't write %s\n", sPath);
}

void features_init(const char *path) {
    snprintf(sPath, sizeof sPath, "%s", path ? path : "");
    FILE *f = sPath[0] ? fopen(sPath, "r") : NULL;
    if (f) {
        char line[256];
        while (fgets(line, sizeof line, f)) {
            if (line[0] == '#') continue;
            line[strcspn(line, "\r\n")] = 0;
            char *eq = strchr(line, '=');
            if (!eq) continue;
            *eq = 0;
            for (int i = 0; i < FEAT_COUNT; i++)
                if (!strcmp(line, kFeatures[i].key)) sOn[i] = atoi(eq + 1) != 0;
        }
        fclose(f);
    }
    for (int i = 0; i < FEAT_COUNT; i++) fprintf(stderr, "features: %s %s\n", kFeatures[i].key, sOn[i] ? "on" : "off");
}

int feature_in_file(const char *path, int f) {
    FILE *fp = path && f >= 0 && f < FEAT_COUNT ? fopen(path, "r") : NULL;
    int on = 0;
    if (!fp) return 0;
    char line[256];
    size_t n = strlen(kFeatures[f].key);
    while (fgets(line, sizeof line, fp))
        if (!strncmp(line, kFeatures[f].key, n) && line[n] == '=') on = atoi(line + n + 1) != 0;
    fclose(fp);
    return on;
}

int feature_on(int f) { return f >= 0 && f < FEAT_COUNT && sOn[f]; }

static int env_off(const char *name) { return getenv(name) && atoi(getenv(name)) == 0; }
int host_local_play_on(void) { return !env_off("ACWW_LOCAL_PLAY"); }
int host_wiimmfi_on(void) { return !env_off("ACWW_WIIMMFI"); }

void feature_set(int f, int on) {
    if (f < 0 || f >= FEAT_COUNT || sOn[f] == (on != 0)) return;
    sOn[f] = on != 0;
    fprintf(stderr, "features: %s %s\n", kFeatures[f].key, sOn[f] ? "on" : "off");
    save();
}

const char *feature_name(int f) { return f >= 0 && f < FEAT_COUNT ? kFeatures[f].name : ""; }
