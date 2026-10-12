#include "async_save_game.cpp"

#include <cstdio>
#include <cstring>
#include <string>

int ranConfig, ranBrowser, syncState, errorCondition, flareAlpha, flareDelta, browserSlot, sortedDirCount;
int actionCoolOff, gameControlPaused, padAcceptButtonFontChar = 11, padBackButtonFontChar = 22;
unsigned padAcceptButton = 0x40, padBackButton = 0x80;
char sortedDir[64 * 8 + 64];
signed char configFormatted, MC_SavingOrLoading, MC_KickProtect, netNetworkMode, whichPad, cutMode;
unsigned char MC_LastModeSuccessful;
char levelName[322];
alignas(4) unsigned char netLocalPad[352];
unsigned char ramSlot[16];
unsigned char MCA_DirContents[4096];
unsigned char quickLoadStamp[64];
unsigned short hudFooterUni[128];

static void copy_str(char* d, const char* s) { while ((*d++ = *s++)) {} }
static char log_[2048];
static void ev(const char* text, int a = -1000, int b = -1000, int c = -1000, int d = -1000) {
    char tmp[96];
    int n = std::snprintf(tmp, sizeof tmp, "%s", text);
    if (a != -1000) n += std::snprintf(tmp + n, sizeof tmp - n, "(%d", a);
    if (b != -1000) n += std::snprintf(tmp + n, sizeof tmp - n, ",%d", b);
    if (c != -1000) n += std::snprintf(tmp + n, sizeof tmp - n, ",%d", c);
    if (d != -1000) n += std::snprintf(tmp + n, sizeof tmp - n, ",%d", d);
    if (a != -1000) std::snprintf(tmp + n, sizeof tmp - n, ")");
    std::strcat(log_, tmp);
    std::strcat(log_, ";");
}
static bool is(const char* expected) { return std::strcmp(log_, expected) == 0; }

static unsigned short texts[16][1];
static char keys[16][40];
static int text_n;
static int run_config_result = 1, busy, dlg;
static int browser_result[4], browser_n;
static float fmt_a, fmt_rest;
static unsigned short* fmt_dest;
static unsigned short *show_title, *show_text, *show_footer;
static int show_a, show_b, show_c;
static const void* cmd_text[4];
static int cmd_cmd[4], cmd_third[4], cmd_fourth[4], cmd_n;
static int net_cycle = 0x1234;
static char level_name_buffer_seen;
static char best_slot_fill[] = "BEST";

extern "C" unsigned norrath_host_cycle_count() { return net_cycle; }
extern "C" unsigned short* textGetString__FPc(char* key) {
    copy_str(keys[text_n], key);
    return texts[text_n++];
}
extern "C" void usprintf__FPUsT0ffffffff(unsigned short* d, unsigned short*, float a, float b, float c, float dd,
                                         float e, float ff, float g, float h) {
    ev("FMT"); fmt_dest = d; fmt_a = a; fmt_rest = b + c + dd + e + ff + g + h;
}
extern "C" int runConfigure__FiPUsi(int m, unsigned short*, int x) { ev("RC", m, x); return run_config_result; }
extern "C" int runBrowser__FPUs(unsigned short*) {
    ev("RB");
    return browser_result[browser_n < 3 ? browser_n++ : 3];
}
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
extern "C" void popPause__Fi(int c) { ev("POP", c); }
extern "C" int newMode__Fii(int m, int k) { ev("NM", m, k); return 0; }
extern "C" void resetWithClose__Fv() { ev("RWC"); }
extern "C" void startProgress__Fv() { ev("START"); }
extern "C" void drawProgress__Fv() { ev("DRAW"); }
extern "C" void gameGetFullLevelName__FPc(char* n) { ev("FULL"); n[0] = 'x'; level_name_buffer_seen = 1; ev("KICK", MC_KickProtect); }
extern "C" void prepareBrowserUndo__Fv() { ev("PREP"); }
extern "C" void undoBrowser__Fv() { ev("UNDO", MC_KickProtect); }
extern "C" void getBestSlotName__FPc(char* n) { ev("BEST"); copy_str(n, best_slot_fill); }
extern "C" char* strcpy(char* d, const char* s) { ev("STRCPY"); char* r = d; while ((*d++ = *s++)) {} return r; }
extern "C" void modifyRamSlotForNetSave__Fv() { ev("MOD"); }
extern "C" void unModifyRamSlotForNetSave__Fv() { ev("UNMOD"); }
extern "C" void updateRamSlot__Fii(int a, int b) { ev("UPD", a, b); }
extern "C" void msgHudSetPlayerBusy__Fcc(int p, int b) { ev("PBUSY", p, b); }
extern "C" int DLG_Active__Fv() { ev("DLG"); return dlg; }

static void reset() {
    log_[0] = 0; text_n = 0; cmd_n = 0; browser_n = 0;
    ranConfig = 1; ranBrowser = 1; syncState = 0; errorCondition = 0; flareAlpha = 50; flareDelta = 0;
    browserSlot = 0; sortedDirCount = 0; configFormatted = 0; MC_SavingOrLoading = 0; MC_KickProtect = 0;
    MC_LastModeSuccessful = 0; netNetworkMode = 0; whichPad = 2; cutMode = 0; actionCoolOff = 0; gameControlPaused = 0;
    levelName[0] = 0; busy = 0; dlg = 0; run_config_result = 1;
    std::memset(netLocalPad, 0, sizeof netLocalPad);
    for (int& r : browser_result) r = 0;
}
static void pad(unsigned word) { *reinterpret_cast<unsigned*>(netLocalPad + 88 * whichPad + 12) = word; }
#define RUN() asyncSaveGame__Fv()
#define CHECK(n, cond) do { if (!(cond)) { std::printf("fail %d log=%s\n", n, log_); return n; } } while (0)

int main() {
    const char* cancel2 = "PBUSY(2,0);NM(-1,0);POP(0);";
    (void)cancel2;

    // Entry: configuration screen.
    reset(); ranConfig = 0; run_config_result = 5; RUN();
    CHECK(1, is("RC(0,0);") && ranConfig == 0 && std::strcmp(keys[0], "menu_savegame") == 0);
    reset(); ranConfig = 0; run_config_result = -1; RUN();
    CHECK(2, ranConfig == 1 && std::strcmp(log_, "RC(0,0);PBUSY(2,0);NM(-1,0);POP(0);") == 0);
    reset(); ranConfig = 0; run_config_result = -1; netNetworkMode = 1; RUN();
    CHECK(3, is("RC(0,0);PBUSY(2,0);NM(-1,0);"));
    reset(); ranConfig = 0; run_config_result = 1; busy = 1; RUN();
    CHECK(4, ranConfig == 1);
    CHECK(5, is("RC(0,0);DRAW;BUSY;"));

    // Browser.
    reset(); ranBrowser = 0; browser_result[0] = -1; RUN();
    CHECK(6, is("RB;PBUSY(2,0);NM(-1,0);POP(0);") && ranBrowser == 0);
    CHECK(7, std::strcmp(keys[0], "MemcardSave") == 0);
    reset(); ranBrowser = 0; browser_result[0] = -2; browser_result[1] = 0; RUN();
    CHECK(8, is("RB;RC(0,0);RB;") && ranConfig == 1 && ranBrowser == 0);
    reset(); ranBrowser = 0; browser_result[0] = 0; RUN();
    CHECK(9, is("RB;") && MC_SavingOrLoading == 0 && flareAlpha == 50);
    reset(); ranBrowser = 0; browser_result[0] = 1; flareDelta = 3; syncState = 7; busy = 1; RUN();
    CHECK(10, is("RB;START;DRAW;BUSY;") && MC_SavingOrLoading == 1 && flareDelta == 8 && ranBrowser == 1 && syncState == 0);
    CHECK(11, flareAlpha == 58);
    reset(); ranBrowser = 0; configFormatted = 1; flareDelta = 3; syncState = 7; busy = 1; RUN();
    CHECK(12, is("START;DRAW;BUSY;") && MC_SavingOrLoading == 1 && flareDelta == 8 && syncState == 0 && ranBrowser == 1);
    reset(); ranBrowser = 0; browser_result[0] = 3; flareDelta = 3; busy = 1; RUN();
    CHECK(13, is("RB;DRAW;BUSY;") && ranBrowser == 0 && flareDelta == 3 && flareAlpha == 53 && MC_SavingOrLoading == 0);
    reset(); ranBrowser = 0; browser_result[0] = -5; flareDelta = -3; busy = 1; RUN();
    CHECK(14, is("RB;DRAW;BUSY;") && ranBrowser == 0 && flareAlpha == 47);

    // Alpha clamp and drawProgress rules.
    reset(); flareAlpha = 120; flareDelta = 8; busy = 1; RUN();
    CHECK(15, flareAlpha == 127);
    reset(); flareAlpha = 126; flareDelta = 1; busy = 1; RUN();
    CHECK(16, flareAlpha == 127);
    reset(); flareAlpha = 5; flareDelta = -16; busy = 1; RUN();
    CHECK(17, flareAlpha == 0);
    reset(); flareAlpha = 100; flareDelta = -16; busy = 1; RUN();
    CHECK(18, flareAlpha == 84);
    reset(); syncState = 9; busy = 1; RUN();
    CHECK(19, is("BUSY;"));
    reset(); syncState = 4; busy = 1; RUN();
    CHECK(20, is("BUSY;"));
    reset(); syncState = 3; busy = 1; flareDelta = 5; RUN();
    CHECK(21, is("DRAW;BUSY;") && MC_LastModeSuccessful == 0);

    // Error handling.
    reset(); syncState = 2; errorCondition = 1; RUN();
    CHECK(22, std::strncmp(log_, "DRAW;BUSY;RWC;FMT;", 18) == 0 && syncState == 9);
    reset(); syncState = 9; errorCondition = 1; RUN();
    CHECK(23, std::strncmp(log_, "BUSY;FMT;", 9) == 0 && std::strstr(log_, "RWC") == 0);

    // State 0.
    reset(); syncState = 0; configFormatted = 1; RUN();
    CHECK(24, is("DRAW;BUSY;FULL;KICK(1);PREP;BEST;") && MC_KickProtect == 1 && syncState == 1 && cycleStart == 0x1234);
    CHECK(25, std::strcmp(localSlotFileName, "BEST") == 0);
    reset(); browserSlot = 3; sortedDirCount = 3; RUN();
    CHECK(26, is("DRAW;BUSY;FULL;KICK(1);PREP;BEST;") && syncState == 1);
    reset(); browserSlot = 2; sortedDirCount = 5; copy_str(sortedDir + 32 + 64 * 2, "Slot Two"); log_[0] = 0; RUN();
    CHECK(27, is("DRAW;BUSY;FULL;KICK(1);PREP;STRCPY;") && std::strcmp(localSlotFileName, "Slot Two") == 0 && syncState == 1);

    // State 1.
    reset(); syncState = 1; RUN();
    CHECK(28, is("DRAW;BUSY;UPD(2,0);") && syncState == 2);
    reset(); syncState = 1; netNetworkMode = 1; RUN();
    CHECK(29, is("DRAW;BUSY;MOD;") && syncState == 2);

    // State 2.
    reset(); syncState = 2; RUN();
    CHECK(30, cmd_n == 4 && syncState == 3 && is("DRAW;BUSY;CMD(0,514,0);CMD(5,731592,0);CMD(1,0,0);CMD(6,0,0);"));
    CHECK(31, cmd_text[0] == localSlotFileName && cmd_text[1] == ramSlot && cmd_text[2] == 0 && cmd_text[3] == localSlotFileName);

    // State 3.
    reset(); syncState = 3; flareDelta = 4;
    for (int i = 0; i < 64; ++i) MCA_DirContents[i] = static_cast<unsigned char>(i + 1);
    MCA_DirContents[64] = 0xee; MC_KickProtect = 1; RUN();
    CHECK(32, is("DRAW;BUSY;") && quickLoadStamp[0] == 1 && quickLoadStamp[63] == 64 && MC_LastModeSuccessful == 1);
    CHECK(33, flareDelta == -16 && MC_KickProtect == 0 && syncState == 3 && flareAlpha == 54 && quickLoadStamp[0] == 1);
    reset(); syncState = 3; flareDelta = 4; netNetworkMode = 1; flareAlpha = 90; RUN();
    CHECK(34, is("DRAW;BUSY;UNMOD;") && flareAlpha == 94);
    reset(); syncState = 3; flareDelta = 4; netNetworkMode = 1; levelName[0] = 'L'; flareAlpha = 90; RUN();
    CHECK(35, flareAlpha == 0);
    reset(); syncState = 3; flareDelta = 4; levelName[0] = 'L'; flareAlpha = 90; RUN();
    CHECK(36, flareAlpha == 94);
    reset(); syncState = 3; flareDelta = 0; RUN();
    CHECK(37, flareDelta == 8 && syncState == 4 && MC_LastModeSuccessful == 0);
    reset(); syncState = 3; flareDelta = -16; RUN();
    CHECK(38, flareDelta == -16 && syncState == 3 && MC_LastModeSuccessful == 0);

    // State 4.
    reset(); syncState = 4; flareDelta = 0; flareAlpha = 77; RUN();
    CHECK(39, std::strcmp(keys[0], "MemcardContinue") == 0 && std::strcmp(keys[1], "MemcardSaveComplete") == 0 &&
                  std::strcmp(keys[2], "MemcardSaveGood") == 0);
    CHECK(40, fmt_dest == hudFooterUni && fmt_a == 11.0f && fmt_rest == 0.0f && show_footer == hudFooterUni);
    CHECK(41, show_a == 77 && show_b == 0 && show_c == 0 && show_title == texts[1] && show_text == texts[2]);
    CHECK(42, is("BUSY;FMT;SHOW;DLG;POP(0);NM(-1,0);PBUSY(2,0);") && MC_SavingOrLoading == 0 && gameControlPaused == 15 && actionCoolOff == 15);
    reset(); syncState = 4; flareDelta = 0; netNetworkMode = 1; MC_SavingOrLoading = 1; RUN();
    CHECK(43, is("BUSY;FMT;SHOW;DLG;NM(-1,0);PBUSY(2,0);") && MC_SavingOrLoading == 0);
    reset(); syncState = 4; flareDelta = 6; RUN();
    CHECK(44, is("BUSY;FMT;SHOW;DLG;") && flareDelta == 6 && MC_SavingOrLoading == 0);
    reset(); syncState = 4; flareDelta = 6; pad(0x40); RUN();
    CHECK(45, is("BUSY;FMT;SHOW;DLG;") && flareDelta == -16 && gameControlPaused == 0);
    reset(); syncState = 4; flareDelta = 6; pad(0x3f | 0xffffff80u); RUN();
    CHECK(46, flareDelta == 6);
    reset(); syncState = 4; flareDelta = 6; whichPad = 1; pad(0x40); whichPad = 2; RUN();
    CHECK(47, flareDelta == 6);
    reset(); syncState = 4; flareDelta = -16; RUN();
    CHECK(48, is("BUSY;FMT;SHOW;DLG;") && flareDelta == -16);
    reset(); syncState = 4; flareDelta = -16; dlg = 1; RUN();
    CHECK(49, is("BUSY;FMT;SHOW;DLG;POP(0);NM(-1,0);PBUSY(2,0);"));
    reset(); syncState = 4; flareDelta = 6; cutMode = 1; RUN();
    CHECK(50, is("BUSY;FMT;SHOW;POP(0);NM(-1,0);PBUSY(2,0);"));
    reset(); syncState = 4; flareDelta = 6; netNetworkMode = 1; levelName[0] = 'L'; RUN();
    CHECK(51, is("BUSY;FMT;SHOW;NM(-1,0);PBUSY(2,0);"));
    reset(); syncState = 4; flareDelta = 6; netNetworkMode = 1; RUN();
    CHECK(52, is("BUSY;FMT;SHOW;DLG;") && flareDelta == 6);

    // State 9.
    reset(); syncState = 9; MC_KickProtect = 1; RUN();
    CHECK(53, is("BUSY;FMT;TITLE;SHOW;") && std::strcmp(keys[0], "MemcardBack") == 0 && std::strcmp(keys[1], "MemcardCantSave") == 0);
    CHECK(54, fmt_a == 22.0f && show_footer == hudFooterUni && show_a == 128 && show_b == 0 && show_c == 0 && MC_KickProtect == 1);
    reset(); syncState = 9; pad(0x80); MC_KickProtect = 1; RUN();
    {
        std::string want = std::string("BUSY;FMT;TITLE;SHOW;PBUSY(2,0);UNDO(0);") + cancel2;
        CHECK(55, want == log_ && actionCoolOff == 15 && MC_KickProtect == 0);
    }
    reset(); syncState = 9; pad(0x80); netNetworkMode = 1; RUN();
    CHECK(56, is("BUSY;FMT;TITLE;SHOW;PBUSY(2,0);UNDO(0);PBUSY(2,0);NM(-1,0);"));
    reset(); syncState = 9; netNetworkMode = 1; levelName[0] = 'L'; RUN();
    CHECK(57, std::strstr(log_, "UNDO(0)") != 0 && actionCoolOff == 15);
    reset(); syncState = 9; netNetworkMode = 1; RUN();
    CHECK(58, is("BUSY;FMT;TITLE;SHOW;") && actionCoolOff == 0);
    reset(); syncState = 9; levelName[0] = 'L'; pad(0x40); RUN();
    CHECK(59, is("BUSY;FMT;TITLE;SHOW;"));

    // Other states do nothing but the shared work.
    for (int s : {5, 6, 7, 8, 10, 11, 1000, -1}) {
        reset(); syncState = s; RUN();
        CHECK(60, is(s == 9 ? "" : "DRAW;BUSY;") && syncState == s);
    }
    std::puts("async save game passed");
    return 0;
}
