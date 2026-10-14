#include "async_save_config.cpp"

#include <cstdio>
#include <cstring>

int ranConfig, delayCount, frontEndRunning, syncState, errorCondition, MCA_Result, asyncMode, oldAsyncMode;
int actionCoolOff, padBackButton = 0x100, padBackButtonFontChar = 42;
signed char MC_KickProtect, netNetworkMode;
char levelName[322], configFileName[16];
unsigned char MC_GameConfig[252];
alignas(4) unsigned char netLocalPad[352];
unsigned short hudHeaderUni[128], hudFooterUni[128];

static unsigned short text_buf[8][1];
static char text_key[8][40];
static int text_n;
static int cmds, cmd_a[4], cmd_third[4], cmd_fourth[4];
static const void* cmd_text[4];
static int run_result, run_calls, run_mode, run_extra, busy, busy_calls;
static int pops, pop_arg, pop_net;
static int shows, show_a, show_b, show_c;
static unsigned short* show_title;
static unsigned short* show_text;
static unsigned short* show_footer;
static int titles;
static unsigned short* title_arg;
static float fmt_a, fmt_rest;
static int fmt_calls;
static unsigned short* fmt_dest;

extern "C" unsigned short* textGetString__FPc(char* key) {
    std::strcpy(text_key[text_n], key);
    return text_buf[text_n++];
}
extern "C" void usprintf__FPUsT0ffffffff(unsigned short* d, unsigned short* f, float a, float b, float c, float dd,
                                         float e, float ff, float g, float h) {
    ++fmt_calls; fmt_dest = d; fmt_a = a; fmt_rest = b + c + dd + e + ff + g + h; (void)f;
}
extern "C" int runConfigure__FiPUsi(int m, unsigned short*, int x) { ++run_calls; run_mode = m; run_extra = x; return run_result; }
extern "C" int MCA_Busy__Fv() { ++busy_calls; return busy; }
extern "C" void MCA_Cmd__Fiiii(int c, const void* t, int a, int b) {
    if (cmds < 4) { cmd_a[cmds] = c; cmd_text[cmds] = t; cmd_third[cmds] = a; cmd_fourth[cmds] = b; }
    ++cmds;
}
extern "C" void memcardTitle__FPUs(unsigned short* t) { ++titles; title_arg = t; }
extern "C" void hudShowMsg__FPUsN20iii(unsigned short* t, unsigned short* x, unsigned short* f, int a, int b, int c) {
    ++shows; show_title = t; show_text = x; show_footer = f; show_a = a; show_b = b; show_c = c;
}
extern "C" void popPause__Fi(int clear) { ++pops; pop_arg = clear; pop_net = netNetworkMode; }

static void reset() {
    ranConfig = 1; delayCount = 0; frontEndRunning = 0; syncState = 0; errorCondition = 0; MCA_Result = 0;
    asyncMode = 3; oldAsyncMode = 0; actionCoolOff = 0; MC_KickProtect = 0; netNetworkMode = 0;
    levelName[0] = 0; std::memset(netLocalPad, 0, sizeof netLocalPad);
    text_n = 0; cmds = 0; run_calls = 0; busy = 0; busy_calls = 0; pops = 0; shows = 0; titles = 0; fmt_calls = 0;
}

int main() {
    // Config screen results.
    reset(); ranConfig = 0; run_result = 5;
    asyncSaveConfig__Fv();
    if (run_calls != 1 || run_mode != 0 || run_extra != 0 || std::strcmp(text_key[0], "MemcardSaveConfigPost") != 0) return 1;
    if (ranConfig != 0 || busy_calls != 0 || shows != 0 || pops != 0) return 2;
    reset(); ranConfig = 0; run_result = -1; syncState = 4;
    asyncSaveConfig__Fv();
    if (ranConfig != 1 || asyncMode != -1 || oldAsyncMode != 3 || syncState != 0 || pops != 1 || pop_arg != 0) return 3;
    if (busy_calls != 0 || shows != 0 || delayCount != 0) return 4;
    reset(); ranConfig = 0; run_result = 1; delayCount = 9;
    asyncSaveConfig__Fv();
    if (ranConfig != 1 || delayCount != 8 || busy_calls != 1) return 5;

    // Busy, or delay still positive: no dispatch; header unless state 9.
    reset(); busy = 1; delayCount = 5; frontEndRunning = 1; syncState = 1;
    asyncSaveConfig__Fv();
    if (delayCount != 3 || syncState != 1 || shows != 1 || cmds != 0 || oldAsyncMode != 0) return 6;
    if (title_arg != hudHeaderUni || show_title != hudHeaderUni || show_footer != 0 || show_a != 128 || show_b != 0 || show_c != 0) return 7;
    if (std::strcmp(text_key[0], "MemcardSaveConfig1") != 0) return 8;
    reset(); busy = 0; delayCount = 5; syncState = 9;
    asyncSaveConfig__Fv();
    if (delayCount != 4 || shows != 0 || titles != 0) return 9;
    reset(); busy = 1; delayCount = -7; syncState = 9;
    asyncSaveConfig__Fv();
    if (shows != 0 || delayCount != -8) return 10;

    // State 0.
    reset(); delayCount = 1; syncState = 0;
    asyncSaveConfig__Fv();
    if (delayCount != 180 || syncState != 1 || MC_KickProtect != 1 || cmds != 3) return 11;
    if (cmd_a[0] != 0 || cmd_text[0] != configFileName || cmd_third[0] != 2 || cmd_fourth[0] != 0) return 12;
    if (cmd_a[1] != 5 || cmd_text[1] != MC_GameConfig || cmd_third[1] != 252 || cmd_fourth[1] != 0) return 13;
    if (cmd_a[2] != 1 || cmd_text[2] != 0 || cmd_third[2] != 0 || cmd_fourth[2] != 0) return 14;
    if (shows != 1) return 15;

    // State 1.
    reset(); syncState = 1; errorCondition = 4;
    asyncSaveConfig__Fv();
    if (syncState != 9 || shows != 0 || pops != 0 || oldAsyncMode != 0) return 16;
    reset(); syncState = 1; MCA_Result = -1;
    asyncSaveConfig__Fv();
    if (syncState != 9 || shows != 0 || pops != 0) return 17;
    reset(); syncState = 1; MCA_Result = 2; MC_KickProtect = 1;
    asyncSaveConfig__Fv();
    if (syncState != 0 || asyncMode != -1 || oldAsyncMode != 3 || pops != 1 || MC_KickProtect != 0 || shows != 1) return 18;

    // State 9.
    reset(); syncState = 9; MC_KickProtect = 1;
    asyncSaveConfig__Fv();
    if (fmt_calls != 1 || fmt_dest != hudFooterUni || fmt_a != 42.0f || fmt_rest != 0.0f) return 19;
    if (std::strcmp(text_key[0], "MemcardBack") != 0 || std::strcmp(text_key[1], "MemcardCantSaveConfig") != 0) return 20;
    if (shows != 1 || show_footer != hudFooterUni || show_title == hudHeaderUni || show_a != 128) return 21;
    if (syncState != 9 || pops != 0 || actionCoolOff != 0 || MC_KickProtect != 1) return 22;
    reset(); syncState = 9; netNetworkMode = 1;  // network without a level name
    asyncSaveConfig__Fv();
    if (syncState != 9 || pops != 0) return 23;
    reset(); syncState = 9; netNetworkMode = 1; levelName[0] = 'a';
    asyncSaveConfig__Fv();
    if (syncState != 0 || actionCoolOff != 15 || pops != 1 || pop_net != 1 || asyncMode != -1 || oldAsyncMode != 3) return 24;
    if (shows != 2 || show_title != hudHeaderUni) return 25;  // footer message, then the header
    reset(); syncState = 9; MC_KickProtect = 1;
    *reinterpret_cast<unsigned*>(netLocalPad + 12) = 0x100;
    asyncSaveConfig__Fv();
    if (syncState != 0 || actionCoolOff != 15 || pops != 1 || MC_KickProtect != 0) return 26;
    reset(); syncState = 9;
    *reinterpret_cast<unsigned*>(netLocalPad + 12) = 0xfeff;
    asyncSaveConfig__Fv();
    if (syncState != 9 || pops != 0) return 27;
    reset(); syncState = 9;
    *reinterpret_cast<unsigned*>(netLocalPad + 8) = 0x100;
    *reinterpret_cast<unsigned*>(netLocalPad + 16) = 0x100;
    asyncSaveConfig__Fv();
    if (syncState != 9 || pops != 0) return 28;

    // Any other state only shows the header.
    reset(); syncState = 4;
    asyncSaveConfig__Fv();
    if (syncState != 4 || shows != 1 || cmds != 0 || pops != 0) return 29;
    std::puts("async save config passed");
    return 0;
}
