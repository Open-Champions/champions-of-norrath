#include "async_save_config_silent.cpp"

#include <cstdio>
#include <cstring>

int ranConfig, syncState, errorCondition, MCA_Result, asyncMode, oldAsyncMode, actionCoolOff, gameBailFromGameLoop;
int padBackButton = 0x100, padBackButtonFontChar = 42;
signed char saveConfigEndGame, MC_KickProtect, netNetworkMode;
char levelName[322], configFileName[16];
unsigned char MC_GameConfig[252];
alignas(4) unsigned char netLocalPad[352];
unsigned short hudFooterUni[128];

static unsigned short text_buf[8][1];
static char text_key[8][40];
static int text_n, cmds, cmd_a[4], cmd_third[4], cmd_fourth[4];
static const void* cmd_text[4];
static int run_result, run_calls, run_mode, run_extra, busy, busy_calls, pops, pop_arg, shows, titles, fmt_calls;
static unsigned short *show_title, *show_footer, *fmt_dest;
static int show_a, show_b, show_c;
static float fmt_a, fmt_rest;

extern "C" unsigned short* textGetString__FPc(char* key) {
    std::strcpy(text_key[text_n], key);
    return text_buf[text_n++];
}
extern "C" void usprintf__FPUsT0ffffffff(unsigned short* d, unsigned short*, float a, float b, float c, float dd,
                                         float e, float ff, float g, float h) {
    ++fmt_calls; fmt_dest = d; fmt_a = a; fmt_rest = b + c + dd + e + ff + g + h;
}
extern "C" int runConfigure__FiPUsi(int m, unsigned short*, int x) { ++run_calls; run_mode = m; run_extra = x; return run_result; }
extern "C" int MCA_Busy__Fv() { ++busy_calls; return busy; }
extern "C" void MCA_Cmd__Fiiii(int c, const void* t, int a, int b) {
    if (cmds < 4) { cmd_a[cmds] = c; cmd_text[cmds] = t; cmd_third[cmds] = a; cmd_fourth[cmds] = b; }
    ++cmds;
}
extern "C" void memcardTitle__FPUs(unsigned short*) { ++titles; }
extern "C" void hudShowMsg__FPUsN20iii(unsigned short* t, unsigned short*, unsigned short* f, int a, int b, int c) {
    ++shows; show_title = t; show_footer = f; show_a = a; show_b = b; show_c = c;
}
extern "C" void popPause__Fi(int clear) { ++pops; pop_arg = clear; }

static void reset() {
    ranConfig = 1; syncState = 0; errorCondition = 0; MCA_Result = 0; asyncMode = 3; oldAsyncMode = 0;
    actionCoolOff = 0; gameBailFromGameLoop = 0; saveConfigEndGame = 0; MC_KickProtect = 0; netNetworkMode = 0;
    levelName[0] = 0; std::memset(netLocalPad, 0, sizeof netLocalPad);
    text_n = 0; cmds = 0; run_calls = 0; busy = 0; busy_calls = 0; pops = 0; shows = 0; titles = 0; fmt_calls = 0;
}

int main() {
    reset(); ranConfig = 0; run_result = 5; saveConfigEndGame = 1;
    asyncSaveConfigSilent__Fv();
    if (run_calls != 1 || run_mode != 0 || run_extra != 0 || std::strcmp(text_key[0], "MemcardEndGame") != 0) return 1;
    if (ranConfig != 0 || busy_calls != 0 || pops != 0 || gameBailFromGameLoop != 0) return 2;
    reset(); ranConfig = 0; run_result = -1; syncState = 4; saveConfigEndGame = 0;
    asyncSaveConfigSilent__Fv();
    if (ranConfig != 1 || asyncMode != -1 || oldAsyncMode != 3 || syncState != 0 || pops != 1 || pop_arg != 0) return 3;
    if (gameBailFromGameLoop != 0 || busy_calls != 0 || shows != 0) return 4;
    reset(); ranConfig = 0; run_result = -1; saveConfigEndGame = 2;
    asyncSaveConfigSilent__Fv();
    if (gameBailFromGameLoop != 101 || ranConfig != 1) return 5;
    reset(); ranConfig = 0; run_result = 1;
    asyncSaveConfigSilent__Fv();
    if (ranConfig != 1 || busy_calls != 1 || cmds != 3) return 6;  // state 0 ran

    reset(); busy = 1; syncState = 1;
    asyncSaveConfigSilent__Fv();
    if (syncState != 1 || cmds != 0 || pops != 0 || shows != 0) return 7;

    reset(); syncState = 0;
    asyncSaveConfigSilent__Fv();
    if (syncState != 1 || MC_KickProtect != 1 || cmds != 3) return 8;
    if (cmd_a[0] != 0 || cmd_text[0] != configFileName || cmd_third[0] != 2 || cmd_fourth[0] != 0) return 9;
    if (cmd_a[1] != 5 || cmd_text[1] != MC_GameConfig || cmd_third[1] != 252 || cmd_fourth[1] != 0) return 10;
    if (cmd_a[2] != 1 || cmd_text[2] != 0 || cmd_third[2] != 0 || cmd_fourth[2] != 0) return 11;
    if (shows != 0) return 12;

    reset(); syncState = 1; errorCondition = 1;
    asyncSaveConfigSilent__Fv();
    if (syncState != 9 || pops != 0) return 13;
    reset(); syncState = 1; MCA_Result = -2;
    asyncSaveConfigSilent__Fv();
    if (syncState != 9 || pops != 0) return 14;
    reset(); syncState = 1; MCA_Result = 0; MC_KickProtect = 1;
    asyncSaveConfigSilent__Fv();
    if (syncState != 0 || asyncMode != -1 || oldAsyncMode != 3 || pops != 1 || MC_KickProtect != 0 || gameBailFromGameLoop != 0) return 15;
    reset(); syncState = 1; saveConfigEndGame = 1;
    asyncSaveConfigSilent__Fv();
    if (gameBailFromGameLoop != 101 || syncState != 0) return 16;

    reset(); syncState = 9; MC_KickProtect = 1;
    asyncSaveConfigSilent__Fv();
    if (fmt_calls != 1 || fmt_dest != hudFooterUni || fmt_a != 42.0f || fmt_rest != 0.0f) return 17;
    if (std::strcmp(text_key[0], "MemcardBack") != 0 || std::strcmp(text_key[1], "MemcardCantSaveConfig") != 0) return 18;
    if (shows != 1 || show_footer != hudFooterUni || show_a != 128 || show_b != 0 || show_c != 0) return 19;
    if (syncState != 9 || pops != 0 || actionCoolOff != 0 || MC_KickProtect != 1) return 20;
    reset(); syncState = 9; netNetworkMode = 1;
    asyncSaveConfigSilent__Fv();
    if (syncState != 9 || pops != 0) return 21;
    reset(); syncState = 9; netNetworkMode = 1; levelName[0] = 'a'; MC_KickProtect = 1;
    asyncSaveConfigSilent__Fv();
    if (syncState != 0 || actionCoolOff != 15 || pops != 1 || MC_KickProtect != 0) return 22;
    reset(); syncState = 9;
    *reinterpret_cast<unsigned*>(netLocalPad + 12) = 0x100;
    asyncSaveConfigSilent__Fv();
    if (syncState != 0 || actionCoolOff != 15 || pops != 1) return 23;
    reset(); syncState = 9;
    *reinterpret_cast<unsigned*>(netLocalPad + 12) = 0xfeff;
    asyncSaveConfigSilent__Fv();
    if (syncState != 9 || pops != 0) return 24;

    reset(); syncState = 5;
    asyncSaveConfigSilent__Fv();
    if (syncState != 5 || cmds != 0 || pops != 0 || shows != 0) return 25;
    std::puts("async save config silent passed");
    return 0;
}
