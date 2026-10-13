#include "run_configure.cpp"

#include <cstdio>

// Recording stubs for every callee and data object the unit imports.
int configState, delayCount, disableMC, flareAlpha, flareDelta, wasFormatting, frontEndRunning;
int asyncMode, errorCondition, MCA_Port, MCA_Slot, MCA_Result, MCA_Present[8], MCA_Formatted[8];
int MCA_LastDirCount, lastCardPort, lastCardSlot, blocksFree, totalClusters, sessionMark;
int padAcceptButtonFontChar, padBackButtonFontChar;
signed char configFormatted, netNetworkMode, MC_Debug, whichPad;
unsigned char MCA_DirContents[4096], iconHeader[964], MC_GameConfig[252], tempBuffer[0x9000], netLocalPad[352];
char slotsCorrupted[27], markFileName[16], configFileName[32];
unsigned padAcceptButton = 0x4000000, padBackButton = 0x2000000;
void* smallFont;
unsigned short hudHeaderUni[8], hudFooterUni[8];

static char trace[4096];
static int traceLen;
static void note(const char* text) {
    for (; *text && traceLen < 4090; ++text) trace[traceLen++] = *text;
    trace[traceLen++] = ';';
    trace[traceLen] = 0;
}
static bool traceIs(const char* want) {
    int i = 0;
    for (; want[i] && trace[i] == want[i]; ++i) {}
    return want[i] == 0 && trace[i] == 0;
}
static bool traceHas(const char* part) {
    for (int i = 0; trace[i]; ++i) {
        int j = 0;
        while (part[j] && trace[i + j] == part[j]) ++j;
        if (!part[j]) return true;
    }
    return false;
}

extern "C" {
unsigned short* textGetString__FPc(char* k) { return reinterpret_cast<unsigned short*>(k); }
void usprintf__FPUsT0ffffffff(unsigned short*, unsigned short*, float, float, float, float, float, float, float, float) {}
void hudShowMsg__FPUsN20iii(unsigned short*, unsigned short*, unsigned short*, int, int, int) { note("show"); }
int hudRunYesNo__FPUsiiii(unsigned short*, int, int, int, int) { note("yesno"); return 2; }
void MCA_Cmd__Fiiii(int c, long, long, long) { char b[8] = {'c', char('0' + c / 10), char('0' + c % 10), 0}; note(b); }
static int busyCalls;
int MCA_Busy__Fv() { return busyCalls++ == 0 ? 0 : 1; }
void MCA_Reset__Fv() { note("reset"); }
void pushPause__Fv() { note("push"); }
void popPause__Fi(int) { note("pop"); }
void resetWithClose__Fv() { note("close"); }
void resetQuickLoad__Fv() { note("quick"); }
int dirEntryIsQuickLoad__FP14sceMcTblGetDir(void*) { return 0; }
char* getSlotName__FiiPc(int, int, char*) { return 0; }
int sceMtapPortOpen(int) { return 0; }
int scePadGetSlotMax(int) { return 4; }
void* lumpFindResource__FPcT0(char*, char*) { return 0; }
void fontStart__FP4FontiUii(void*, int, unsigned, int) {}
void fontEnd__Fv() {}
int fontPrint__FiiPci(int, int, char*, int) { return 0; }
void rectangleDraw__FiiiiiUi(int, int, int, int, int, unsigned) {}
float evalHermite__Ffffff(float t, float, float, float, float) { return t; }
void cdLoadAsync__FPcPvT0ii(const char*, void*, void*, int, int) {}
char* strstr(const char* h, const char* n) {
    for (; *h; ++h) {
        int i = 0;
        while (n[i] && h[i] == n[i]) ++i;
        if (!n[i]) return const_cast<char*>(h);
    }
    return 0;
}
int strcmp(const char* a, const char* b) {
    while (*a && *a == *b) { ++a; ++b; }
    return static_cast<unsigned char>(*a) - static_cast<unsigned char>(*b);
}
void* memset(void* d, int v, unsigned n) {
    for (unsigned i = 0; i < n; ++i) static_cast<unsigned char*>(d)[i] = static_cast<unsigned char>(v);
    return d;
}
}

#define CHECK(c) do { if (!(c)) { std::printf("FAIL line %d\n", __LINE__); return 1; } } while (0)

static void reset() {
    traceLen = 0; trace[0] = 0; busyCalls = 0;
    configState = 0; delayCount = 0; disableMC = 0; flareAlpha = 0; flareDelta = 0; wasFormatting = 0;
    frontEndRunning = 0; asyncMode = 0; errorCondition = 0; MCA_Port = -1; MCA_Slot = -1; MCA_Result = 0;
    for (int i = 0; i < 8; ++i) MCA_Present[i] = MCA_Formatted[i] = 0;
    MCA_LastDirCount = 0; lastCardPort = lastCardSlot = 0; blocksFree = totalClusters = 0;
    configFormatted = 0; netNetworkMode = 0; MC_Debug = 0; whichPad = 0;
    for (unsigned i = 0; i < sizeof MCA_DirContents; ++i) MCA_DirContents[i] = 0;
    for (unsigned i = 0; i < sizeof netLocalPad; ++i) netLocalPad[i] = 0;
    for (int i = 0; i < 27; ++i) slotsCorrupted[i] = 0;
    const char* cfg = "BASLUS-20565_config";
    for (int i = 0; i < 20; ++i) configFileName[i] = cfg[i];
}

static void entry(int index, const char* name, int size) {
    unsigned char* base = MCA_DirContents + index * 64;
    *reinterpret_cast<int*>(base + 16) = size;
    int i = 0;
    for (; name[i]; ++i) base[32 + i] = static_cast<unsigned char>(name[i]);
    base[32 + i] = 0;
}

int main() {
    // Cancelled from the first pass: disableMC selects state 30.
    reset(); configState = -1; disableMC = 1;
    CHECK(runConfigure__FiPUsi(0, 0, 0) == -1);
    CHECK(configState == -1 && lastCardPort == -1);
    CHECK(traceIs("push;pop;quick;show;"));
    CHECK(flareDelta == 8 && delayCount == -1);

    // Finished with a card chosen: state 29 returns 1 and remembers the card.
    reset(); configState = 29; MCA_Port = 1; MCA_Slot = 2; netNetworkMode = 1;
    CHECK(runConfigure__FiPUsi(0, 0, 0) == 1);
    CHECK(configState == -1 && lastCardPort == 1 && lastCardSlot == 2);
    CHECK(traceIs("show;"));

    // Delay and fade bookkeeping while the front end runs.
    reset(); configState = 33; frontEndRunning = 1; delayCount = 10; flareAlpha = 100; flareDelta = 50;
    CHECK(runConfigure__FiPUsi(0, 0, 0) == 0 && delayCount == 8 && flareAlpha == 127);
    reset(); configState = 33; flareAlpha = 5; flareDelta = -16;
    CHECK(runConfigure__FiPUsi(0, 0, 0) == 0 && flareAlpha == 0 && delayCount == -1);

    // State 2: a single formatted card is taken directly.
    reset(); configState = 2; MCA_Present[5] = 1; MCA_Formatted[5] = 1;
    CHECK(runConfigure__FiPUsi(1, 0, 0) == 0);
    CHECK(configState == 7 && MCA_Port == 1 && MCA_Slot == 1);
    reset(); configState = 2; MCA_Present[2] = MCA_Present[6] = 1;
    runConfigure__FiPUsi(0, 0, 0);
    CHECK(configState == 3);
    reset(); configState = 2;
    runConfigure__FiPUsi(0, 0, 0);
    CHECK(configState == 31);
    reset(); configState = 2; MCA_Present[0] = 1; asyncMode = 4;
    runConfigure__FiPUsi(0, 0, 0);
    CHECK(configState == 10);

    // State 16: size checks on the config file, icon.sys, icon.ico and the slot files.
    reset(); configState = 16; MCA_Result = 4;
    entry(0, "BASLUS-20565_config", 252); entry(1, "icon.sys", 964); entry(2, "icon.ico", 0x1f6e0); entry(3, "slot1", 0xb29c8);
    runConfigure__FiPUsi(1, 0, 0);
    CHECK(configState == 26);
    reset(); configState = 16; MCA_Result = 4;
    entry(0, "BASLUS-20565_config", 252); entry(1, "icon.sys", 964); entry(2, "icon.ico", 0x1f6e0); entry(3, "slot1", 0xb29c8);
    runConfigure__FiPUsi(0, 0, 0);
    CHECK(configState == 29);
    reset(); configState = 16; MCA_Result = 3;
    entry(0, "slot0", 1); entry(1, "slot1", 0xb29c8); entry(2, "slot2", 5);
    runConfigure__FiPUsi(0, 0, 0);
    CHECK(configState == 17 && slotsCorrupted[0] == 1 && slotsCorrupted[1] == 0 && slotsCorrupted[2] == 1);
    CHECK(traceIs("show;"));
    reset(); configState = 16; MCA_Result = 2; entry(0, "icon.sys", 963);
    runConfigure__FiPUsi(0, 0, 0);
    CHECK(configState == 19);
    reset(); configState = 16; MCA_Result = 1; entry(0, "icon.ico", 5);
    runConfigure__FiPUsi(0, 0, 0);
    CHECK(configState == 19);
    reset(); configState = 16; MCA_Result = 1; entry(0, "icon.ico", 5); MC_Debug = 1;
    runConfigure__FiPUsi(0, 0, 0);
    CHECK(configState == 29);

    // State 15: counting the slot files decides between 16, 19 and 21.
    reset(); configState = 15; MCA_Result = 6;
    for (int i = 0; i < 6; ++i) entry(i, "other", 1);
    runConfigure__FiPUsi(0, 0, 0);
    CHECK(configState == 16);
    reset(); configState = 15; MCA_Result = 2;
    entry(0, "a", 1); entry(1, "b", 1);
    runConfigure__FiPUsi(0, 0, 0);
    CHECK(configState == 21);

    // State 17: accept deletes the corrupt slot files and waits.
    reset(); configState = 17; MCA_LastDirCount = 2;
    entry(0, "slot0", 1); entry(1, "slot1", 1); slotsCorrupted[1] = 1;
    *reinterpret_cast<unsigned*>(netLocalPad + 12) = padAcceptButton;
    runConfigure__FiPUsi(0, 0, 0);
    CHECK(configState == 18 && delayCount == 20 && traceHas("c07;"));
    CHECK(!traceHas("c07;c07;"));

    // State 5: the choice moves with the down mask, accept picks the second card.
    reset(); configState = 5; MCA_Present[1] = MCA_Present[4] = 1; flareAlpha = 100; flareDelta = 0;
    *reinterpret_cast<unsigned*>(netLocalPad + 12) = 0x204000;
    runConfigure__FiPUsi(0, 0, 0);
    CHECK(configState == 5 && MCA_Port == -1);
    std::puts("ok");
    return 0;
}
