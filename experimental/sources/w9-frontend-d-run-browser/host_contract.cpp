#define sprintf w9_sprintf
#include "run_browser.cpp"
#undef sprintf

#include <cstdio>

// Recording stubs for every callee and data object the unit imports.
int browserState, browserSlot, browserDelete, browserOverwrite, delayCount, flareAlpha, flareDelta;
int errorCondition, frontEndRunning, asyncMode, MCA_Port, MCA_Slot, MCA_Result, blocksFree, slotClusters;
int sortedDirCount, sortedPlayerRace[40], padAcceptButtonFontChar, padBackButtonFontChar;
signed char netNetworkMode, fullLaunchOK, MC_Debug, language, whichPad;
unsigned char netLocalPad[352], pad[352], slotInfoHeaders[1120], sortedDir[640], sortedTimes[160];
char* raceStrings[10];
unsigned padAcceptButton = 0x4000000, padBackButton = 0x2000000;
void* smallFont;
unsigned short hudHeaderUni[8];

static char trace[8192];
static int traceLen;
static void note(const char* text) {
    for (; *text && traceLen < 8180; ++text) trace[traceLen++] = *text;
    trace[traceLen++] = ';';
    trace[traceLen] = 0;
}
static bool traceHas(const char* part) {
    for (int i = 0; trace[i]; ++i) {
        int j = 0;
        while (part[j] && trace[i + j] == part[j]) ++j;
        if (!part[j]) return true;
    }
    return false;
}

static int busyCalls, busyFirst;
extern "C" {
unsigned short* textGetString__FPc(char* k) { return reinterpret_cast<unsigned short*>(k); }
void usprintf__FPUsT0ffffffff(unsigned short*, unsigned short*, float, float, float, float, float, float, float, float) {}
void usprintf__FPUsPcffffffff(unsigned short*, char*, float, float, float, float, float, float, float, float) {}
void hudShowMsg__FPUsN20iii(unsigned short*, unsigned short*, unsigned short*, int, int, int) { note("show"); }
void MCA_Cmd__Fiiii(int c, long, long, long) { char b[8] = {'c', char('0' + c / 10), char('0' + c % 10), 0}; note(b); }
int MCA_Busy__Fv() { return busyCalls++ < busyFirst; }
int cdIsBusy__Fv() { return 0; }
void pushPause__Fv() { note("push"); }
void popPause__Fi(int) { note("pop"); }
void resetWithClose__Fv() { note("close"); }
void resetQuickLoad__Fv() { note("quick"); }
int dirEntryIsQuickLoad__FP14sceMcTblGetDir(void*) { return 0; }
void generateSortedListAndInfoHeaders__Fv() { note("gen"); }
void patchSortedListWithLocalTime__Fv() { note("patch"); }
char* getSlotName__FiiPc(int, int, char*) { return 0; }
unsigned short* getLongLevelName__FPc(char*) { return 0; }
void memcardTitle__FPUs(unsigned short*) {}
void* lumpFindResource__FPcT0(char*, char*) { return 0; }
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void*, void*, float) { note("sound"); }
void fontStart__FP4FontiUii(void*, int, unsigned, int) {}
void fontEnd__Fv() {}
void fontColor__FUiii(unsigned, int, int) {}
int fontPrint__FiiPci(int, int, char*, int) { return 0; }
int fontPrintu__FiiPUsi(int, int, unsigned short*, int) { return 0; }
int fontPrintCenteredu__FiiPUsi(int, int, unsigned short*, int) { return 0; }
int fontPrintWrapCenteredu__FiiPUsiiii(int, int, unsigned short*, int, int, int, int) { return 0; }
void textureDrawBorder__FiiiiiUiPcii(int, int, int, int, int, unsigned, char*, int, int) {}
void rectangleDraw__FiiiiiUi(int, int, int, int, int, unsigned) {}
float evalHermite__Ffffff(float t, float, float, float, float) { return t; }
int w9_sprintf(char* b, const char*, ...) { b[0] = 'x'; b[1] = 0; return 1; }
int strcmp(const char* a, const char* b) {
    while (*a && *a == *b) { ++a; ++b; }
    return static_cast<unsigned char>(*a) - static_cast<unsigned char>(*b);
}
void ustrcpy__FPUsPc(unsigned short*, char*) {}
void ustrcat__FPUsT0(unsigned short*, unsigned short*) {}
void ustrcat__FPUsPc(unsigned short*, char*) {}
}

#define CHECK(c) do { if (!(c)) { std::printf("FAIL line %d\n", __LINE__); return 1; } } while (0)

static void reset(int state) {
    traceLen = 0; trace[0] = 0; busyCalls = 0; busyFirst = 0;
    browserState = state; browserSlot = 0; browserDelete = browserOverwrite = 0; delayCount = 0;
    flareAlpha = 0; flareDelta = 0; errorCondition = 0; frontEndRunning = 0; asyncMode = 0;
    MCA_Port = MCA_Slot = 0; MCA_Result = 0; blocksFree = slotClusters = 0; sortedDirCount = 0;
    netNetworkMode = 0; fullLaunchOK = 0; MC_Debug = 0; language = 0; whichPad = 0;
    for (unsigned i = 0; i < sizeof netLocalPad; ++i) netLocalPad[i] = pad[i] = 0;
}

static void press(unsigned bits) {
    *reinterpret_cast<unsigned*>(netLocalPad + 12) = bits;
}

int main() {
    // State 0 starts the browser: pause, delay 60, next state, hud message only.
    reset(0);
    CHECK(runBrowser__FPUs(0) == 0);
    CHECK(browserState == 1 && delayCount == 60 && flareAlpha == 127 && flareDelta == 8);
    CHECK(traceHas("push;") && traceHas("show;"));

    // State 1 waits while MCA is busy, then queues the directory read.
    reset(1); busyFirst = 1; delayCount = 5;
    runBrowser__FPUs(0);
    CHECK(browserState == 1 && !traceHas("c10;"));
    reset(1); delayCount = 0;
    runBrowser__FPUs(0);
    CHECK(browserState == 2 && traceHas("c10;c06;"));

    // State 2: a negative result goes to the error screen, otherwise the list is built.
    reset(2); MCA_Result = -1;
    runBrowser__FPUs(0);
    CHECK(browserState == 15 && !traceHas("gen;"));
    reset(2); MCA_Result = 3; delayCount = 0;
    runBrowser__FPUs(0);
    CHECK(browserState == 3 && traceHas("gen;"));

    // Close paths.
    reset(12); flareDelta = 0; netNetworkMode = 0;
    CHECK(runBrowser__FPUs(0) == 1 && browserState == 0 && traceHas("pop;"));
    reset(12); flareDelta = 8; flareAlpha = 50;
    CHECK(runBrowser__FPUs(0) == 0 && browserState == 12);
    reset(13); netNetworkMode = 1;
    CHECK(runBrowser__FPUs(0) == -2 && browserState == 0 && !traceHas("pop;"));
    reset(14);
    CHECK(runBrowser__FPUs(0) == -1 && browserState == 0);
    reset(14); busyFirst = 1;
    CHECK(runBrowser__FPUs(0) == 0 && browserState == 14);

    // An error condition forces state 15 and the reset.
    reset(4); errorCondition = 1;
    runBrowser__FPUs(0);
    CHECK(browserState == 15 && traceHas("close;"));
    // State 15: accept re-reads the card and goes to 13, back goes to 14.
    reset(15); press(padAcceptButton);
    runBrowser__FPUs(0);
    CHECK(browserState == 13 && flareAlpha == 128 && traceHas("c08;quick;"));
    reset(15); press(padBackButton);
    runBrowser__FPUs(0);
    CHECK(browserState == 14 && traceHas("quick;"));

    // State 9: accept starts the fade, then the delete is queued and the browser restarts.
    reset(9); flareDelta = 8; flareAlpha = 100; press(padAcceptButton);
    runBrowser__FPUs(0);
    CHECK(browserState == 9 && flareDelta == -16);
    reset(9); flareDelta = -16; flareAlpha = 0; browserSlot = 2;
    runBrowser__FPUs(0);
    CHECK(browserState == 1 && delayCount == 240 && browserDelete == 1 && traceHas("c07;"));

    // State 5: accept moves to 11 with the overwrite flag; back returns to 4.
    reset(5); flareDelta = 8; flareAlpha = 100; press(padAcceptButton);
    runBrowser__FPUs(0);
    CHECK(browserState == 11 && browserOverwrite == 1 && flareDelta == -16);
    reset(5); press(padBackButton);
    runBrowser__FPUs(0);
    CHECK(browserState == 4);
    std::puts("ok");
    return 0;
}
