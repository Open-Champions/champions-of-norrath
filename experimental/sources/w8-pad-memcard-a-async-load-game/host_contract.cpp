// Host check: build with -fno-builtin so the memcpy below is the one called.
#include "async_load_game.cpp"

#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

int ranConfig, ranBrowser, syncState, errorCondition, flareAlpha, flareDelta, browserSlot;
int actionCoolOff, gameNoDraw, gameBailFromGameLoop, padBackButtonFontChar = 22;
unsigned coreClearColor, padBackButton = 0x80;
char sortedDir[64 * 8 + 64], loadName[64];
signed char MC_SavingOrLoading, whichPad, frontEndWasRunning, fullLaunchOK, floorDrawEnable, objectDrawEnable;
unsigned char MC_LastModeSuccessful;
alignas(4) unsigned char netLocalPad[352];
unsigned char quickLoadStamp[64];
unsigned short hudFooterUni[128];
static std::vector<unsigned char> lock_storage(0xb29c8 + 16, 0);
unsigned char ramSlot[0xb29c8 + 16];

static void copy_str(char* d, const char* s) { while ((*d++ = *s++)) {} }
static char log_[2048];
static void ev(const char* text, int a = -1000, int b = -1000, int c = -1000) {
    char tmp[96];
    int n = std::snprintf(tmp, sizeof tmp, "%s", text);
    if (a != -1000) n += std::snprintf(tmp + n, sizeof tmp - n, "(%d", a);
    if (b != -1000) n += std::snprintf(tmp + n, sizeof tmp - n, ",%d", b);
    if (c != -1000) n += std::snprintf(tmp + n, sizeof tmp - n, ",%d", c);
    if (a != -1000) std::snprintf(tmp + n, sizeof tmp - n, ")");
    std::strcat(log_, tmp);
    std::strcat(log_, ";");
}
static bool same(const char* a, const char* b) { while (*a && *a == *b) { ++a; ++b; } return *a == *b; }
static bool prefix(const char* p) { for (int i = 0; p[i]; ++i) if (log_[i] != p[i]) return false; return true; }
static bool is(const char* expected) { return same(log_, expected); }

static unsigned short texts[16][1];
static char keys[16][40];
static int text_n, busy, fade_result, browser_result[4], browser_n;
static unsigned char* lock_result;
static float fmt_a, fmt_rest;
static unsigned short *fmt_dest, *show_title, *show_text, *show_footer;
static int show_a, show_b, show_c, cmd_n, cmd_cmd[4], cmd_third[4], cmd_fourth[4], run_config_result = 1;
static const void* cmd_text[4];
static int pop_arg_last;
static void *cpy_dest;
static const void* cpy_src;
static unsigned long cpy_n;
static int level_calls;

extern "C" unsigned short* textGetString__FPc(char* key) { copy_str(keys[text_n], key); return texts[text_n++]; }
extern "C" void usprintf__FPUsT0ffffffff(unsigned short* d, unsigned short*, float a, float b, float c, float dd,
                                         float e, float ff, float g, float h) {
    ev("FMT"); fmt_dest = d; fmt_a = a; fmt_rest = b + c + dd + e + ff + g + h;
}
extern "C" int runConfigure__FiPUsi(int m, unsigned short*, int x) { ev("RC", m, x); return run_config_result; }
extern "C" int runBrowser__FPUs(unsigned short*) { ev("RB"); return browser_result[browser_n < 3 ? browser_n++ : 3]; }
extern "C" int MCA_Busy__Fv() { ev("BUSY"); return busy; }
extern "C" void MCA_Cmd__Fiiii(int c, const void* t, int a, int b) {
    ev("CMD", c, a, b);
    if (cmd_n < 4) { cmd_cmd[cmd_n] = c; cmd_text[cmd_n] = t; cmd_third[cmd_n] = a; cmd_fourth[cmd_n] = b; }
    ++cmd_n;
}
extern "C" void memcardTitle__FPUs(unsigned short*) { ev("TITLE"); }
extern "C" void hudShowMsg__FPUsN20iii(unsigned short* t, unsigned short* x, unsigned short* f, int a, int b, int c) {
    ev("SHOW"); show_title = t; show_text = x; show_footer = f; show_a = a; show_b = b; show_c = c;
}
extern "C" void popPause__Fi(int c) { ev("POP", c); pop_arg_last = c; }
extern "C" int newMode__Fii(int m, int k) { ev("NM", m, k); return 0; }
extern "C" void resetWithClose__Fv() { ev("RWC"); }
extern "C" void startProgress__Fv() { ev("START"); }
extern "C" void drawProgress__Fv() { ev("DRAW"); }
extern "C" void gameGetFullLevelName__FPc(char* n) { ev("FULL"); copy_str(n, "level-a"); ++level_calls; }
extern "C" void prepareBrowserUndo__Fv() { ev("PREP"); }
extern "C" void undoBrowser__Fv() { ev("UNDO"); }
extern "C" int strcmp(const char* a, const char* b) {
    ev("STRCMP");
    while (*a && *a == *b) { ++a; ++b; }
    return static_cast<unsigned char>(*a) - static_cast<unsigned char>(*b);
}
extern "C" void* memcpy(void* d, const void* s, unsigned long n) {
    if (n == 0xb29c8) { ev("MEMCPY"); cpy_dest = d; cpy_src = s; cpy_n = n; }  // smaller copies are struct copies
    unsigned char* dd = static_cast<unsigned char*>(d); const unsigned char* ss = static_cast<const unsigned char*>(s);
    for (unsigned long i = 0; i < n; ++i) dd[i] = ss[i];
    return d;
}
extern "C" void frontEndCleanUp__Fi(int m) { ev("CLEAN", m); }
extern "C" void hudResetFadeScreen__Fi(int m) { ev("RESETFADE", m); }
extern "C" int hudFadeScreen__Fi(int m) { ev("FADE", m); return fade_result; }
extern "C" void coreStartLoading__Fv() { ev("CORE"); }
extern "C" void texturePrepareToLockCacheBuffer__Fv() { ev("PREPLOCK"); }
extern "C" unsigned char* textureLockCacheBuffer__Fv() { ev("LOCK"); return lock_result; }
extern "C" void textureReleaseCacheBuffer__Fv() { ev("RELEASE"); }
extern "C" void loadRamSlot__Fv() { ev("LOADSLOT"); }
extern "C" void padInitPadIndexMap__Fv() { ev("PADMAP"); }

static void reset() {
    log_[0] = 0; text_n = 0; cmd_n = 0; browser_n = 0; level_calls = 0;
    ranConfig = 1; ranBrowser = 1; syncState = 0; errorCondition = 0; flareAlpha = 50; flareDelta = 0;
    browserSlot = 0; MC_SavingOrLoading = 0; MC_LastModeSuccessful = 0; whichPad = 2; actionCoolOff = 0;
    gameNoDraw = 7; gameBailFromGameLoop = 0; coreClearColor = 0x1111; floorDrawEnable = 5; objectDrawEnable = 5;
    frontEndWasRunning = 0; fullLaunchOK = 0; busy = 0; fade_result = 0; run_config_result = 1;
    std::memset(netLocalPad, 0, sizeof netLocalPad);
    for (int& r : browser_result) r = 0;
    copy_str(loadName, "level-a");
}
static void pad(unsigned word) { *reinterpret_cast<unsigned*>(netLocalPad + 88 * whichPad + 12) = word; }
#define RUN() asyncLoadGame__Fv()
#define CHECK(n, cond) do { if (!(cond)) { std::printf("fail %d log=%s\n", n, log_); return n; } } while (0)

int main() {
    // Entry.
    reset(); ranConfig = 0; run_config_result = 5; RUN();
    CHECK(1, is("RC(0,0);") && ranConfig == 0 && same(keys[0], "menu_loadgame"));
    reset(); ranConfig = 0; run_config_result = -1; RUN();
    CHECK(2, ranConfig == 1 && is("RC(0,0);NM(-1,0);POP(0);"));
    reset(); ranConfig = 0; run_config_result = 1; busy = 1; RUN();
    CHECK(3, ranConfig == 1 && is("RC(0,0);DRAW;BUSY;"));

    // Browser.
    reset(); ranBrowser = 0; browser_result[0] = -1; RUN();
    CHECK(4, is("RB;NM(-1,0);POP(0);") && ranBrowser == 0 && same(keys[0], "MemcardLoad"));
    reset(); ranBrowser = 0; browser_result[0] = -2; browser_result[1] = 0; RUN();
    CHECK(5, is("RB;RC(0,0);RB;") && ranConfig == 1);
    reset(); ranBrowser = 0; browser_result[0] = 0; RUN();
    CHECK(6, is("RB;") && flareAlpha == 50);
    reset(); ranBrowser = 0; browser_result[0] = 2; flareDelta = 3; busy = 1; RUN();
    CHECK(7, is("RB;DRAW;BUSY;") && ranBrowser == 0 && flareDelta == 3 && flareAlpha == 53);
    reset(); ranBrowser = 0; browser_result[0] = -7; busy = 1; RUN();
    CHECK(8, is("RB;DRAW;BUSY;") && ranBrowser == 0);
    reset(); ranBrowser = 0; browser_result[0] = 1; browserSlot = 3; flareDelta = 3; syncState = 6; busy = 1;
    for (int i = 0; i < 64; ++i) sortedDir[3 * 64 + i] = static_cast<char>(i + 1);
    sortedDir[4 * 64] = 0x77; RUN();
    CHECK(9, is("RB;RESETFADE(0);PREP;START;DRAW;BUSY;") && flareDelta == 16 && ranBrowser == 1 && syncState == 0 && flareAlpha == 66);
    CHECK(10, quickLoadStamp[0] == 1 && quickLoadStamp[63] == 64);
    reset(); ranBrowser = 0; browser_result[0] = 1; frontEndWasRunning = 1; busy = 1; RUN();
    CHECK(11, is("RB;RESETFADE(0);PREP;START;DRAW;BUSY;"));
    reset(); ranBrowser = 0; browser_result[0] = 1; frontEndWasRunning = 1; fullLaunchOK = 1; busy = 1; RUN();
    CHECK(12, is("RB;CLEAN(0);RESETFADE(0);PREP;START;DRAW;BUSY;"));
    reset(); ranBrowser = 0; browser_result[0] = 1; fullLaunchOK = 1; busy = 1; RUN();
    CHECK(13, is("RB;RESETFADE(0);PREP;START;DRAW;BUSY;"));

    // Alpha clamp, drawProgress and busy.
    reset(); flareAlpha = 120; flareDelta = 8; busy = 1; syncState = 3; RUN();
    CHECK(14, flareAlpha == 128 && is("DRAW;BUSY;"));
    reset(); flareAlpha = 125; flareDelta = 3; busy = 1; RUN();
    CHECK(15, flareAlpha == 128);
    reset(); flareAlpha = 126; flareDelta = 100; busy = 1; RUN();
    CHECK(16, flareAlpha == 128);
    reset(); flareAlpha = 5; flareDelta = -16; busy = 1; RUN();
    CHECK(17, flareAlpha == 0);
    reset(); flareAlpha = 100; flareDelta = -16; busy = 1; syncState = 9; RUN();
    CHECK(18, flareAlpha == 84 && is("BUSY;"));
    reset(); busy = 1; syncState = 4; RUN();
    CHECK(19, MC_SavingOrLoading == 0 && is("DRAW;BUSY;"));

    // Errors.
    reset(); syncState = 2; errorCondition = 1; RUN();
    CHECK(20, prefix("DRAW;BUSY;RWC;CORE;") && syncState == 9 && MC_SavingOrLoading == 1);
    CHECK(21, std::strstr(log_, "RELEASE") == 0);
    reset(); syncState = 2; errorCondition = 1; fade_result = 0; RUN();  // localBuffer is zero here
    reset(); syncState = 0; fade_result = 128; RUN();  // sets localBuffer to zero in state 0
    reset(); syncState = 2; lock_result = lock_storage.data(); RUN();  // localBuffer becomes nonzero
    reset(); syncState = 2; errorCondition = 1; RUN();
    CHECK(22, prefix("DRAW;BUSY;RELEASE;RWC;CORE;") && syncState == 9);
    reset(); syncState = 9; errorCondition = 1; RUN();
    CHECK(23, prefix("BUSY;FMT;") && std::strstr(log_, "RWC") == 0);

    // State 0.
    reset(); syncState = 0; fade_result = 127; RUN();
    CHECK(24, is("DRAW;BUSY;FADE(8);") && syncState == 0 && MC_SavingOrLoading == 1 && coreClearColor == 0x1111);
    reset(); syncState = 0; fade_result = 128; RUN();
    CHECK(25, is("DRAW;BUSY;FADE(8);RESETFADE(0);") && syncState == 1 && oldClearColor == 0x1111 && coreClearColor == 0x80000000u);
    CHECK(26, gameNoDraw == 1 && floorDrawEnable == 0 && objectDrawEnable == 0 && localBuffer == 0);

    // State 1.
    reset(); syncState = 1; flareAlpha = 100; RUN();
    CHECK(27, is("DRAW;BUSY;") && syncState == 1);
    reset(); syncState = 1; flareAlpha = 127; flareDelta = 1; RUN();
    CHECK(28, is("DRAW;BUSY;PREPLOCK;") && syncState == 2);

    // State 2.
    reset(); syncState = 2; lock_result = 0; localBuffer = lock_storage.data(); RUN();
    CHECK(29, is("DRAW;BUSY;LOCK;") && localBuffer == 0 && syncState == 2);
    reset(); syncState = 2; lock_result = lock_storage.data(); RUN();
    CHECK(30, is("DRAW;BUSY;LOCK;CMD(0,1,0);CMD(3,731592,0);CMD(1,0,0);") && syncState == 3 && localBuffer == lock_storage.data());
    CHECK(31, cmd_text[0] == quickLoadStamp + 32 && cmd_text[1] == lock_storage.data() && cmd_text[2] == 0);

    // State 3.
    reset(); syncState = 3; flareAlpha = 30; flareDelta = 0; RUN();
    CHECK(32, flareDelta == -16 && syncState == 3 && flareAlpha == 30);
    reset(); syncState = 3; flareAlpha = 5; flareDelta = -5; RUN();
    CHECK(33, flareAlpha == 0 && flareDelta == -16 && syncState == 4);

    // State 4 (copies the locked buffer into ramSlot).
    for (unsigned i = 0; i < 0xb29c8; ++i) lock_storage[i] = static_cast<unsigned char>(i * 7 + 1);
    lock_storage[0xb29c8] = 0x99;
    for (unsigned i = 0; i < sizeof ramSlot; ++i) ramSlot[i] = 0;
    reset(); syncState = 4; localBuffer = lock_storage.data(); oldClearColor = 0x2222; fullLaunchOK = 1; RUN();
    CHECK(34, is("DRAW;BUSY;MEMCPY;RELEASE;LOADSLOT;") && cpy_dest == ramSlot && cpy_src == lock_storage.data() && cpy_n == 0xb29c8);
    CHECK(35, ramSlot[0] == lock_storage[0] && ramSlot[0xb29c7] == lock_storage[0xb29c7] && ramSlot[0xb29c8] == 0);
    CHECK(36, coreClearColor == 0x2222 && gameNoDraw == 0 && floorDrawEnable == 1 && objectDrawEnable == 1 && syncState == 5);
    CHECK(37, MC_LastModeSuccessful == 0 && MC_SavingOrLoading == 1);
    reset(); syncState = 4; localBuffer = lock_storage.data(); oldClearColor = 0x2222; fullLaunchOK = 0; frontEndWasRunning = 1; RUN();
    CHECK(38, is("DRAW;BUSY;MEMCPY;RELEASE;POP(0);NM(-1,0);CORE;") && syncState == 4 && MC_LastModeSuccessful == 1);
    CHECK(39, frontEndWasRunning == -1 && MC_SavingOrLoading == 0 && gameNoDraw == 0);

    // State 5.
    reset(); syncState = 5; copy_str(loadName, "other"); MC_SavingOrLoading = 0; RUN();
    CHECK(40, is("DRAW;BUSY;FULL;STRCMP;") && MC_LastModeSuccessful == 0 && MC_SavingOrLoading == 1 && syncState == 5);
    reset(); syncState = 5; frontEndWasRunning = 1; RUN();
    CHECK(41, is("DRAW;BUSY;FULL;STRCMP;POP(1);NM(-1,0);PADMAP;") && frontEndWasRunning == -1 && MC_SavingOrLoading == 0 && MC_LastModeSuccessful == 1);

    // State 9.
    reset(); syncState = 9; oldClearColor = 0x3333; RUN();
    CHECK(42, is("BUSY;FMT;TITLE;SHOW;") && same(keys[0], "MemcardBack") && same(keys[1], "MemcardCantLoad"));
    CHECK(43, fmt_a == 22.0f && fmt_rest == 0.0f && fmt_dest == hudFooterUni && show_footer == hudFooterUni && show_a == 128 && show_b == 0 && show_c == 0);
    CHECK(44, MC_SavingOrLoading == 1 && gameNoDraw == 7 && actionCoolOff == 0);
    reset(); syncState = 9; oldClearColor = 0x3333; pad(0x80); frontEndWasRunning = 1; RUN();
    CHECK(45, is("BUSY;FMT;TITLE;SHOW;UNDO;NM(-1,0);POP(0);") && gameBailFromGameLoop == 1 && actionCoolOff == 15);
    CHECK(46, frontEndWasRunning == -1 && objectDrawEnable == 1 && floorDrawEnable == 1 && coreClearColor == 0x3333 && MC_SavingOrLoading == 0 && gameNoDraw == 0);
    reset(); syncState = 9; pad(0x80); frontEndWasRunning = 0; RUN();
    CHECK(47, gameBailFromGameLoop == 0 && frontEndWasRunning == -1);
    reset(); syncState = 9; pad(0x7f); RUN();
    CHECK(48, is("BUSY;FMT;TITLE;SHOW;") && actionCoolOff == 0);
    reset(); syncState = 9; whichPad = 1; pad(0x80); whichPad = 2; RUN();
    CHECK(49, actionCoolOff == 0);

    // Other states.
    for (int s : {6, 7, 8, 10, 11, 1000, -1}) {
        reset(); syncState = s; RUN();
        CHECK(50, is("DRAW;BUSY;") && syncState == s && MC_SavingOrLoading == 1);
    }
    std::puts("async load game passed");
    return 0;
}
