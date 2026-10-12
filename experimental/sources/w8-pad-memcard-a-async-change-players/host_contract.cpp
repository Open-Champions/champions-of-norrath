#include "async_change_players.cpp"

#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <string>

int syncState, asyncMode, oldAsyncMode, errorCondition, delayCount, flareAlpha, flareDelta, browserSlot;
int MCA_Port, MCA_Slot, MCA_FileOpen, MCA_Result, lastCardPort, lastCardSlot, drawFrameNm, nmPlayers;
signed char whichPad, pcNmPlayers, pcLaunch, MC_FrontEndImport, keepPlayerPositions, netNetworkMode;
unsigned char MC_LastModeSuccessful;
unsigned short* pcTitle;
int frontEndRunning;
void* smallFont = reinterpret_cast<void*>(0x2000);
void* gameCam = reinterpret_cast<void*>(0x3000);
unsigned padAcceptButton = 0x40, padBackButton = 0x100;
int padAcceptButtonFontChar = 5, padBackButtonFontChar = 6;
int playerStatus[4], playerRace[4];
alignas(8) unsigned char player[4 * 23984];
alignas(8) unsigned char pad[352];
alignas(8) unsigned char netLocalPad[352];
alignas(8) unsigned char tempBuffer[4 * 23984];
alignas(8) unsigned char tempPlayerStruct[4 * 23984];
unsigned char slotInfoHeaders[112 * 8];
char sortedDir[64 * 8 + 64];
char levelName[322];
char playerSelectNextName[64], playerSelectPrevName[64];
unsigned char playerSelectInfo[41 * 4 + 64];
unsigned short* playerSelectExitMessage;
signed char playerSelectCount, playerSelectStartPlayer, playerSelectEnforce, playerSelectSuccessful;
float playerChangeOldPos[4][3];
unsigned short playerChangeOldYaw[4];
char* texStrings[16];
char* raceStrings[16];
unsigned short hudHeaderUni[128], hudFooterUni[128];

static std::string log_;
static void ev(const char* fmt, ...) {
    char tmp[256];
    va_list ap;
    va_start(ap, fmt);
    std::vsnprintf(tmp, sizeof tmp, fmt, ap);
    va_end(ap);
    log_ += tmp;
    log_ += ";";
}

static unsigned short texts[256][1];
static char text_keys[256][48];
static int text_n;
static unsigned short title_text[2] = {0x41, 0};
static int config_result, browser_result, busy, mca_sync_calls;
static char resource_names[64][48];
static int resource_n;

extern "C" unsigned short* textGetString__FPc(char* key) {
    ev("TGS(%s)", key);
    std::strcpy(text_keys[text_n], key);
    texts[text_n][0] = static_cast<unsigned short>(0xa000 + text_n);
    return texts[text_n++];
}
static const char* key_of(const unsigned short* t) {
    if (t == 0) return "null";
    if (t[0] == 0xb000) return "buf";
    if (t[0] >= 0xa000 && t[0] < 0xa100) return text_keys[t[0] - 0xa000];
    return "other";
}
extern "C" void usprintf__FPUsT0ffffffff(unsigned short* d, unsigned short* f, float a, float b, float c, float dd,
                                         float e, float ff, float g, float h) {
    ev("FMT(%s,%s,%g,%g,%g,%g,%g,%g,%g,%g)", d == hudFooterUni ? "footer" : (d == hudHeaderUni ? "header" : "buf"),
       key_of(f), a, b, c, dd, e, ff, g, h);
    d[0] = 0xb000;
}
extern "C" void ustrcpy__FPUsT0(unsigned short* d, unsigned short* s) {
    ev("USTRCPY(%s,%s)", d == hudHeaderUni ? "header" : "buf", s == pcTitle ? "pcTitle" : key_of(s));
    d[0] = s[0];
}
extern "C" int runConfigure__FiPUsi(int m, unsigned short* t, int x) { ev("RC(%d,%s,%d)", m, key_of(t), x); return config_result; }
extern "C" int runBrowser__FPUs(unsigned short* t) { ev("RB(%s)", key_of(t)); return browser_result; }
extern "C" int MCA_Busy__Fv() { ev("BUSY"); return busy; }
extern "C" void MCA_Cmd__Fiiii(int c, const void* t, int a, int b) {
    ev("CMD(%d,%s,%d,%d)", c, t == 0 ? "0" : (t == sortedDir + 32 + 64 * browserSlot ? "name" : "?"), a, b);
}
extern "C" void MCA_Reset__Fv() { ev("MCARESET"); }
extern "C" int MCA_Sync__FPi(int* e) { ev("SYNC(%d)", e == &errorCondition); ++mca_sync_calls; return 0; }
extern "C" void loadPlayerInfo__FiPiP12playerStruct(int c, int* u, void* d) { ev("LOADINFO(%d,%d,%d)", c, u == 0, d == tempBuffer); }
extern "C" void memcardTitle__FPUs(unsigned short* t) { ev("TITLE(%s)", t == hudHeaderUni ? "header" : "buf"); }
extern "C" void hudShowMsg__FPUsN20iii(unsigned short* t, unsigned short* x, unsigned short* f, int a, int b, int c) {
    ev("SHOW(%s,%s,%s,%d,%d,%d)", t == hudHeaderUni ? "header" : "?", key_of(x), f == hudFooterUni ? "footer" : "?", a, b, c);
}
extern "C" void popPause__Fi(int c) { ev("POP(%d)", c); }
extern "C" void pushPause__Fv() { ev("PUSH"); }
extern "C" void* lumpFindResource__FPcT0(char* g, char* n) {
    ev("LFR(%s,%s)", g, n);
    std::strcpy(resource_names[resource_n], n);
    return resource_names[resource_n++];
}
extern "C" void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* s, void* o, float v) {
    ev("SND(%s,%d,%g)", static_cast<char*>(s), o == 0, v);
}
extern "C" void frontEndCleanUp__Fi(int m) { ev("CLEAN(%d)", m); }
extern "C" void gameGetFullLevelName__FPc(char* n) {
    ev("FULLNAME(%s)", n == playerSelectNextName ? "next" : (n == playerSelectPrevName ? "prev" : (n == lname ? "lname" : "stack")));
    std::strcpy(n, "levelA");
}
extern "C" void gameResetGameState__Fv() { ev("RESETSTATE"); }
extern "C" void scriptSetGameState__FPci(char* n, int v) { ev("SETSTATE(%s,%d)", n, v); }
extern "C" void engineSetDisplayDisable__Fi(int v) { ev("DISPDIS(%d)", v); }
extern "C" void engineSyncVBlank__Fv() { ev("VBLANK"); }
extern "C" void Push__6Camera(void* c, int v) { ev("CAMPUSH(%d,%d)", c == gameCam, v); }
extern "C" unsigned playerGetMarkerColor__Fii(int p, int a) { ev("MARKER(%d,%d)", p, a); return 0xabc0000u + p; }
extern "C" void fontStart__FP4FontiUii(void* f, int a, unsigned c, int b) { ev("FS(%d,%d,%x,%d)", f == smallFont, a, c, b); }
extern "C" void fontEnd__Fv() { ev("FE"); }
extern "C" void fontColor__FUiii(unsigned c, int a, int b) { ev("FC(%x,%d,%d)", c, a, b); }
extern "C" void fontPrintu__FiiPUsi(int x, int y, unsigned short* t, int w) { ev("FP(%d,%d,%s,%d)", x, y, key_of(t), w); }
extern "C" void fontPrintCenteredu__FiiPUsi(int x, int y, unsigned short* t, int w) { ev("FPC(%d,%d,%s,%d)", x, y, key_of(t), w); }
extern "C" void fontPrintCentered__FiiPci(int x, int y, char* t, int w) { ev("FPCS(%d,%d,%s,%d)", x, y, t, w); }
extern "C" void fontPrintWrapCenteredu__FiiPUsiiii(int x, int y, unsigned short* t, int a, int b, int c, int d) {
    ev("FPWCU(%d,%d,%s,%d,%d,%d,%d)", x, y, key_of(t), a, b, c, d);
}
extern "C" void fontPrintWrapCentered__FiiPciiii(int x, int y, char* t, int a, int b, int c, int d) {
    ev("FPWC(%d,%d,%s,%d,%d,%d,%d)", x, y, t, a, b, c, d);
}
extern "C" void textureDrawBorder__FiiiiiUiPcii(int x, int y, int w, int h, int a, unsigned c, char* g, int b, int d) {
    ev("TDB(%d,%d,%d,%d,%d,%x,%s,%d,%d)", x, y, w, h, a, c, g ? g : "-", b, d);
}
extern "C" void textureDrawColor__FP8_textureiiiUi(void* t, int x, int y, int z, unsigned c) {
    ev("TDC(%s,%d,%d,%d,%x)", static_cast<char*>(t), x, y, z, c);
}

static bool has(const char* part) { return log_.find(part) != std::string::npos; }
static int count(const char* part) { int n = 0; for (size_t p = log_.find(part); p != std::string::npos; p = log_.find(part, p + 1)) ++n; return n; }
static bool same(const char* a, const char* b) { return std::strcmp(a, b) == 0; }
static bool is(const char* want) { return log_ == want; }
static bool starts(const char* want) { return log_.compare(0, std::strlen(want), want) == 0; }

static void put(unsigned char* base, int record, int offset, unsigned v) { *reinterpret_cast<unsigned*>(base + record * 88 + offset) = v; }
static void setPad(unsigned buttons, unsigned edges) { put(pad, whichPad, 12, buttons); put(pad, whichPad, 16, edges); }
static unsigned char objects[4][64];

static void reset() {
    log_.clear(); text_n = 0; resource_n = 0; mca_sync_calls = 0;
    syncState = 8; asyncMode = 3; oldAsyncMode = 0; errorCondition = 0; delayCount = 9; flareAlpha = 50; flareDelta = 0;
    browserSlot = 0; MCA_Port = 1; MCA_Slot = 2; MCA_FileOpen = 0; MCA_Result = 0; lastCardPort = -1; lastCardSlot = -1;
    drawFrameNm = 0; nmPlayers = 2; whichPad = 1; pcNmPlayers = 2; pcLaunch = 0; MC_FrontEndImport = 0;
    keepPlayerPositions = 0; netNetworkMode = 0; MC_LastModeSuccessful = 0; frontEndRunning = 0; pcTitle = title_text;
    std::memset(playerStatus, 0, sizeof playerStatus); std::memset(playerRace, 0, sizeof playerRace);
    std::memset(player, 0, sizeof player); std::memset(pad, 0, sizeof pad); std::memset(netLocalPad, 0, sizeof netLocalPad);
    std::memset(tempBuffer, 0, sizeof tempBuffer); std::memset(tempPlayerStruct, 0, sizeof tempPlayerStruct);
    std::memset(slotInfoHeaders, 0, sizeof slotInfoHeaders); std::memset(sortedDir, 0, sizeof sortedDir);
    std::memset(levelName, 0, sizeof levelName); std::memset(playerSelectNextName, 0, 64); std::memset(playerSelectPrevName, 0, 64);
    std::memset(playerSelectInfo, 0, sizeof playerSelectInfo);
    playerSelectExitMessage = 0; playerSelectCount = playerSelectStartPlayer = playerSelectEnforce = playerSelectSuccessful = 0;
    std::memset(playerChangeOldPos, 0, sizeof playerChangeOldPos); std::memset(playerChangeOldYaw, 0, sizeof playerChangeOldYaw);
    for (int i = 0; i < 16; ++i) { texStrings[i] = const_cast<char*>("tex?"); raceStrings[i] = const_cast<char*>("race?"); }
    static const char* tex[4] = {"t_a.tex", "t_b.tex", "t_c.tex", "t_d.tex"};
    static const char* race[4] = {"RaceA", "RaceB", "RaceC", "RaceD"};
    for (int i = 0; i < 4; ++i) { texStrings[i] = const_cast<char*>(tex[i]); raceStrings[i] = const_cast<char*>(race[i]); }
    std::memset(hudHeaderUni, 0, sizeof hudHeaderUni); std::memset(hudFooterUni, 0xff, sizeof hudFooterUni);
    busy = 0; config_result = 0; browser_result = 0;
    glowOffset = 0; curPlayer = 0; needReset = 0; importPlayerCount = 0; curImportPlayer = 0; originalPort = 7; originalSlot = 8;
    boxWidth = 300; boxHeight = 220; std::memset(lname, 0, sizeof lname);
    std::memset(objects, 0, sizeof objects);
}
static unsigned char* oldPosBytes() { return reinterpret_cast<unsigned char*>(playerChangeOldPos); }
static void connect(int record) { put(netLocalPad, record, 4, 1); }
#define RUN() asyncChangePlayers__Fv()
#define CHECK(n, cond) do { if (!(cond)) { std::printf("fail %d\n log=%s\n", n, log_.c_str()); return n; } } while (0)

int main() {
    // Prologue: glow direction, alpha update, header and footer.
    reset(); drawFrameNm = 0x20; flareAlpha = 120; flareDelta = 8; RUN();
    CHECK(1, glowOffset == 1 && flareAlpha == 127 && hudFooterUni[0] == 0 && hudHeaderUni[0] == 0x41);
    CHECK(2, is("USTRCPY(header,pcTitle);SHOW(header,null,footer,127,300,220);"));
    reset(); drawFrameNm = 0x1f; flareAlpha = 5; flareDelta = -16; RUN();
    CHECK(3, glowOffset == -1 && flareAlpha == 0);
    reset(); flareAlpha = 100; flareDelta = 20; RUN();
    CHECK(4, flareAlpha == 120);
    reset(); flareAlpha = 100; flareDelta = 28; RUN();
    CHECK(5, flareAlpha == 127);
    for (int s : {2, 3}) { reset(); syncState = s; flareAlpha = 100; flareDelta = 8; RUN(); CHECK(6, flareAlpha == 100); }
    for (int s : {13, 100, -1}) {
        reset(); syncState = s; RUN();
        CHECK(7, syncState == s && is("USTRCPY(header,pcTitle);SHOW(header,null,footer,50,300,220);"));
    }
    reset(); syncState = 8; RUN();
    CHECK(8, is("USTRCPY(header,pcTitle);SHOW(header,null,footer,50,300,220);") && syncState == 8);

    // State 0.
    reset(); syncState = 0; nmPlayers = 2; MC_FrontEndImport = 0;
    *reinterpret_cast<void**>(player + 21872 + 23984 * 2) = objects[0];
    *reinterpret_cast<int*>(player + 84 + 23984 * 0) = 11; *reinterpret_cast<int*>(player + 84 + 23984 * 1) = 12;
    *reinterpret_cast<int*>(player + 84 + 23984 * 2) = 13; *reinterpret_cast<int*>(player + 84 + 23984 * 3) = 14;
    flareDelta = 3; curPlayer = 3; needReset = 1; delayCount = 9; MCA_Port = 4; MCA_Slot = 5; RUN();
    CHECK(10, playerStatus[0] == 1 && playerStatus[1] == 1 && playerStatus[2] == 1 && playerStatus[3] == 0);
    CHECK(11, playerRace[0] == 11 && playerRace[1] == 12 && playerRace[2] == 13 && playerRace[3] == -1);
    CHECK(12, curPlayer == 0 && glowOffset == 32);
    CHECK(13, flareDelta == 8 && originalPort == 4 && originalSlot == 5 && needReset == 0 && delayCount == 0 && flareAlpha == 0 && syncState == 1);
    CHECK(14, has("SHOW(header,null,footer,0,") && has("FMT(buf,MemcardImportPWho,1,") && !has("FMT(footer"));
    reset(); syncState = 0; MC_FrontEndImport = 1; nmPlayers = 4; RUN();
    CHECK(15, playerStatus[0] == 0 && playerStatus[3] == 0 && playerRace[0] == -1);
    reset(); syncState = 0; MC_FrontEndImport = 1; *reinterpret_cast<void**>(player + 21872 + 23984) = objects[0];
    *reinterpret_cast<int*>(player + 84 + 23984) = 3; RUN();
    CHECK(16, playerStatus[1] == 1 && playerRace[1] == 3 && playerStatus[0] == 0);

    // State 1 footers.
    reset(); syncState = 1; pcNmPlayers = 3; flareDelta = 0; flareAlpha = 9; playerStatus[0] = 1; playerStatus[1] = 1; playerStatus[2] = 1; RUN();
    CHECK(20, boxHeight == 211 && boxWidth == 3 * 80 + 292);
    reset(); syncState = 1; MC_FrontEndImport = 1; pcNmPlayers = 2; playerStatus[0] = playerStatus[1] = 1; RUN();
    CHECK(21, has("FMT(footer,ImportAcceptBack,124,5,6,0,0,0,0,0)"));
    reset(); syncState = 1; MC_FrontEndImport = 1; pcNmPlayers = 2; playerStatus[0] = 1; RUN();
    CHECK(22, has("FMT(footer,MemcardImportBack,124,6,0,0,0,0,0,0)"));
    reset(); syncState = 1; playerStatus[0] = 0; RUN();
    CHECK(23, has("FMT(footer,NewImportAcceptBack,124,5,6,0,0,0,0,0)") && hudFooterUni[0] == 133);
    reset(); syncState = 1; playerStatus[0] = 1; playerStatus[1] = 1; curPlayer = 0; RUN();
    CHECK(24, has("FMT(footer,ImportDeleteAcceptBack,124,125,5,6,0,0,0,0)") && hudFooterUni[0] == 0xb000);
    reset(); syncState = 1; playerStatus[0] = 1; curPlayer = 0; RUN();
    CHECK(25, has("FMT(footer,ImportAcceptBack,124,5,6,0,0,0,0,0)"));
    reset(); syncState = 1; playerStatus[0] = 1; playerStatus[1] = 1; curPlayer = 1; playerStatus[1] = 0; RUN();
    CHECK(26, has("FMT(footer,NewImportAcceptBack"));
    reset(); syncState = 1; playerStatus[0] = 1; playerStatus[1] = 1; playerStatus[2] = 1; curPlayer = 5 - 5; MC_FrontEndImport = 0; RUN();
    CHECK(27, has("ImportDeleteAcceptBack"));
    // Display mode 0 panels follow (count > 0), and hudShowMsg ends the pass.
    CHECK(28, has("SHOW(header,null,footer,9,") == false && has("SHOW(header,null,footer,50,"));

    // State 1 navigation. Controllers 0 to 3 present.
    for (int i = 0; i < 4; ++i) connect(i);
    reset(); for (int i = 0; i < 4; ++i) connect(i); syncState = 1; pcNmPlayers = 4; flareDelta = 8; curPlayer = 1; playerStatus[1] = 1;
    setPad(0, 0x802000); RUN();
    CHECK(30, curPlayer == 2 && count("SND(menu_main_toggle.anm,1,1)") == 1);
    reset(); for (int i = 0; i < 4; ++i) connect(i); syncState = 1; pcNmPlayers = 4; flareDelta = 8; curPlayer = 2; playerStatus[2] = 1;
    setPad(0, 0x408000); RUN();
    CHECK(31, curPlayer == 1 && count("SND(menu_main_toggle.anm") == 1);
    reset(); for (int i = 0; i < 4; ++i) connect(i); syncState = 1; pcNmPlayers = 4; flareDelta = 8; curPlayer = 3; playerStatus[3] = 1;
    setPad(0, 0x802000); RUN();
    CHECK(32, curPlayer == 3 && !has("SND("));
    reset(); for (int i = 0; i < 4; ++i) connect(i); syncState = 1; pcNmPlayers = 4; flareDelta = 8; curPlayer = 0; playerStatus[0] = 1;
    setPad(0, 0x408000); RUN();
    CHECK(33, curPlayer == 0 && !has("SND("));
    // Absent controllers are skipped, with a sound for each step.
    reset(); connect(0); connect(3); syncState = 1; pcNmPlayers = 4; flareDelta = 8; curPlayer = 0; playerStatus[0] = 1;
    setPad(0, 0x802000); RUN();
    CHECK(34, curPlayer == 3 && count("SND(menu_main_toggle.anm") == 3);
    reset(); connect(0); connect(3); syncState = 1; pcNmPlayers = 4; flareDelta = 8; curPlayer = 3; playerStatus[3] = 1;
    setPad(0, 0x408000); RUN();
    CHECK(35, curPlayer == 0 && count("SND(menu_main_toggle.anm") == 3);
    reset(); connect(0); syncState = 1; pcNmPlayers = 4; flareDelta = 8; curPlayer = 0; playerStatus[0] = 1;
    setPad(0, 0x802000); RUN();
    CHECK(36, curPlayer == 0 && count("SND(menu_main_toggle.anm") == 3);  // walks to the end, finds nothing
    // Both directions at once.
    reset(); for (int i = 0; i < 4; ++i) connect(i); syncState = 1; pcNmPlayers = 4; flareDelta = 8; curPlayer = 1; playerStatus[1] = 1;
    setPad(0, 0x408000 | 0x802000); RUN();
    CHECK(37, curPlayer == 1 && count("SND(menu_main_toggle.anm") == 2);
    // Accept.
    reset(); for (int i = 0; i < 4; ++i) connect(i); syncState = 1; flareDelta = 8; playerStatus[0] = 1; setPad(padAcceptButton, 0); RUN();
    CHECK(40, syncState == 11 && has("SND(menu_main_accept.anm,1,1)"));
    reset(); for (int i = 0; i < 4; ++i) connect(i); syncState = 1; flareDelta = 8; MC_FrontEndImport = 1; pcNmPlayers = 2;
    playerStatus[0] = 1; setPad(padAcceptButton, 0); RUN();
    CHECK(41, syncState == 1 && flareDelta == 8 && !has("SND("));
    reset(); for (int i = 0; i < 4; ++i) connect(i); syncState = 1; flareDelta = 8; MC_FrontEndImport = 1; pcNmPlayers = 2;
    playerStatus[0] = playerStatus[1] = 1; setPad(padAcceptButton, 0); RUN();
    CHECK(42, syncState == 11);
    reset(); for (int i = 0; i < 4; ++i) connect(i); syncState = 1; flareDelta = 8; MC_FrontEndImport = 1; pcNmPlayers = 2;
    playerStatus[0] = 1; setPad(padAcceptButton | padBackButton, 0); RUN();
    CHECK(43, flareDelta == -16 && syncState == 1);
    // Back, other buttons.
    reset(); for (int i = 0; i < 4; ++i) connect(i); syncState = 1; flareDelta = 8; playerStatus[0] = 1; setPad(padBackButton, 0); RUN();
    CHECK(44, flareDelta == -16 && !has("SND("));
    reset(); for (int i = 0; i < 4; ++i) connect(i); syncState = 1; flareDelta = 8; playerStatus[0] = 1; setPad(0x10, 0); RUN();
    CHECK(45, syncState == 2 && has("SND(menu_main_accept.anm,1,1)"));
    reset(); for (int i = 0; i < 4; ++i) connect(i); syncState = 1; flareDelta = 8; playerStatus[0] = 1; setPad(padBackButton | 0x10, 0); RUN();
    CHECK(46, flareDelta == -16 && syncState == 1);
    reset(); for (int i = 0; i < 4; ++i) connect(i); syncState = 1; flareDelta = 8; playerStatus[0] = playerStatus[1] = 1; setPad(0x80, 0); RUN();
    CHECK(47, syncState == 6 && count("SND(menu_main_accept.anm") == 1);
    reset(); for (int i = 0; i < 4; ++i) connect(i); syncState = 1; flareDelta = 8; playerStatus[0] = 1; setPad(0x80, 0); RUN();
    CHECK(48, syncState == 1 && !has("SND("));
    reset(); for (int i = 0; i < 4; ++i) connect(i); syncState = 1; flareDelta = 8; playerStatus[0] = playerStatus[1] = 1; setPad(0x80 | 0x10, 0); RUN();
    CHECK(49, syncState == 2);
    // Button 8 starts the in-game player select only on the "new" footer.
    reset(); for (int i = 0; i < 4; ++i) connect(i); syncState = 1; flareDelta = 8; playerStatus[0] = 1; setPad(0x8, 0); RUN();
    CHECK(50, syncState == 1 && !has("SETSTATE"));
    reset(); for (int i = 0; i < 4; ++i) connect(i); syncState = 1; flareDelta = 8; playerStatus[0] = 0; curPlayer = 0; nmPlayers = 2;
    setPad(0x8, 0);
    *reinterpret_cast<void**>(player + 21872) = objects[0];
    objects[0][16] = 1; objects[0][17] = 2; objects[0][27] = 3; objects[0][28] = 9;
    *reinterpret_cast<unsigned short*>(objects[0] + 56) = 0x1234;
    RUN();
    CHECK(51, syncState == 7 && has("SND(menu_main_accept.anm,1,1);LFR(fx.lmp,menu_main_accept.anm)") == false);
    CHECK(52, has("FULLNAME(next);FULLNAME(prev);TGS(selectExitInGame);SETSTATE(needPlayers,1);DISPDIS(10);VBLANK;CAMPUSH(1,-1);POP(0);"));
    CHECK(53, oldPosBytes()[0] == 1 && oldPosBytes()[1] == 2 && oldPosBytes()[11] == 3);
    CHECK(54, oldPosBytes()[12] == 0 && playerChangeOldYaw[0] == 0x1234 && playerChangeOldYaw[1] == 0);
    CHECK(55, playerSelectCount == 1 && playerSelectStartPlayer == 0 && playerSelectEnforce == 0 && same(levelName, "kelsel2") && levelName[80] == -1);
    CHECK(56, playerSelectExitMessage != 0 && same(playerSelectNextName, "levelA") && same(playerSelectPrevName, "levelA"));
    reset(); for (int i = 0; i < 4; ++i) connect(i); syncState = 1; flareDelta = 8; playerStatus[0] = 0; frontEndRunning = 1; netNetworkMode = 1;
    std::strcpy(playerSelectNextName, "zzz"); std::strcpy(playerSelectPrevName, "yyy"); setPad(0x8, 0); RUN();
    CHECK(57, has("CLEAN(1);TGS(selectExitInGame);SETSTATE(needPlayers,1);DISPDIS(10);VBLANK;CAMPUSH(1,-1);") && !has("FULLNAME") && !has("POP("));
    CHECK(58, playerSelectNextName[0] == 0 && playerSelectPrevName[0] == 0 && playerSelectNextName[1] == 'z');
    // The start player is the selection, not necessarily curPlayer.
    reset(); connect(0); connect(2); syncState = 1; flareDelta = 8; pcNmPlayers = 3; curPlayer = 0; playerStatus[0] = 0; setPad(0x8, 0x802000); RUN();
    CHECK(59, curPlayer == 2 && playerSelectStartPlayer == 2);
    // flareDelta not positive: a faded-out screen goes to state 10.
    reset(); syncState = 1; flareDelta = -16; flareAlpha = 40; RUN();
    CHECK(60, syncState == 1 && flareAlpha == 24);
    reset(); syncState = 1; flareDelta = -16; flareAlpha = 10; RUN();
    CHECK(61, syncState == 10 && flareAlpha == 0);
    reset(); syncState = 1; flareDelta = 0; flareAlpha = 0; RUN();
    CHECK(62, syncState == 10);
    reset(); syncState = 1; flareDelta = 0; flareAlpha = 5; RUN();
    CHECK(63, syncState == 1);

    // State 2.
    reset(); syncState = 2; config_result = 1; RUN();
    CHECK(70, syncState == 3 && starts("USTRCPY(header,pcTitle);TGS(MemcardChangePlayers);RC(0,MemcardChangePlayers,1);") && !has("SHOW("));
    reset(); syncState = 2; config_result = -1; RUN();
    CHECK(71, syncState == 1);
    reset(); syncState = 2; config_result = 5; RUN();
    CHECK(72, syncState == 2);
    // State 3.
    reset(); syncState = 3; browser_result = -1; flareDelta = 0; RUN();
    CHECK(73, flareDelta == 8 && syncState == 1 && starts("USTRCPY(header,pcTitle);TGS(MemcardChangePlayers);RB(MemcardChangePlayers);") && !has("SHOW("));
    reset(); syncState = 3; browser_result = 0; RUN();
    CHECK(74, syncState == 3 && flareDelta == 0);
    reset(); syncState = 3; browser_result = 1; browserSlot = 2; flareAlpha = 40;
    *reinterpret_cast<unsigned short*>(slotInfoHeaders + 106 + 112 * 2) = 3; RUN();
    CHECK(75, syncState == 4 && flareDelta == 8 && curImportPlayer == -1 && flareAlpha == 0 && importPlayerCount == 3);
    CHECK(76, has("CMD(0,name,1,0);LOADINFO(3,1,1);CMD(1,0,0,0);"));

    // State 4.
    reset(); syncState = 4; busy = 1; importPlayerCount = 2; flareAlpha = 30; RUN();
    CHECK(80, is("USTRCPY(header,pcTitle);BUSY;") && flareAlpha == 0 && syncState == 4);
    reset(); syncState = 4; importPlayerCount = 2; flareDelta = 8; curImportPlayer = -1; RUN();
    CHECK(81, boxWidth == 460 && boxHeight == 245 && curImportPlayer == 0 && has("TGS(MemcardImportAvail)") && syncState == 4);
    CHECK(82, has("SHOW(header,null,footer,") && has(",460,245)"));
    reset(); syncState = 4; importPlayerCount = 2; curImportPlayer = 1; setPad(0, 0x408000); flareDelta = 8; RUN();
    CHECK(83, curImportPlayer == 0);
    reset(); syncState = 4; importPlayerCount = 2; curImportPlayer = 0; setPad(0, 0x408000); flareDelta = 8; RUN();
    CHECK(84, curImportPlayer == 0);
    reset(); syncState = 4; importPlayerCount = 2; curImportPlayer = 0; setPad(0, 0x802000); flareDelta = 8; RUN();
    CHECK(85, curImportPlayer == 1);
    reset(); syncState = 4; importPlayerCount = 2; curImportPlayer = 1; setPad(0, 0x802000); flareDelta = 8; RUN();
    CHECK(86, curImportPlayer == 1);
    reset(); syncState = 4; importPlayerCount = 3; curImportPlayer = 0; setPad(0, 0x408000 | 0x802000); flareDelta = 8; RUN();
    CHECK(87, curImportPlayer == 1);  // left with nothing to the left falls to the right edge
    reset(); syncState = 4; importPlayerCount = 3; curImportPlayer = 1; setPad(0, 0x408000 | 0x802000); flareDelta = 8; RUN();
    CHECK(88, curImportPlayer == 0);
    reset(); syncState = 4; importPlayerCount = 2; flareDelta = 8; setPad(padAcceptButton, 0); RUN();
    CHECK(89, syncState == 5);
    reset(); syncState = 4; importPlayerCount = 2; flareDelta = 8; setPad(padBackButton, 0); RUN();
    CHECK(90, syncState == 4 && flareDelta == -16);
    reset(); syncState = 4; importPlayerCount = 2; flareDelta = 8; setPad(padBackButton | padAcceptButton, 0); RUN();
    CHECK(91, syncState == 5 && flareDelta == 8);
    reset(); syncState = 4; importPlayerCount = 2; flareDelta = 0; RUN();
    CHECK(92, syncState == 3);
    reset(); syncState = 4; importPlayerCount = 2; flareDelta = -16; RUN();
    CHECK(93, syncState == 4);
    reset(); syncState = 4; importPlayerCount = 2; flareDelta = 0; MCA_Result = -5; MCA_FileOpen = 1; RUN();
    CHECK(94, has("BUSY;MCARESET;CMD(1,0,0,0);SYNC(1);") && syncState == 3 && mca_sync_calls == 1);  // state 9 is overwritten by the flareDelta test
    reset(); syncState = 4; importPlayerCount = 2; flareDelta = 8; MCA_Result = -5; MCA_FileOpen = 0; RUN();
    CHECK(95, has("BUSY;MCARESET;") && !has("SYNC") && syncState == 9);

    // State 5.
    for (int s = 0; s < 4; ++s) for (unsigned i = 0; i < 23984; ++i) tempBuffer[s * 23984 + i] = static_cast<unsigned char>(i * 3 + s * 17 + 1);
    reset(); for (int s = 0; s < 4; ++s) for (unsigned i = 0; i < 23984; ++i) tempBuffer[s * 23984 + i] = static_cast<unsigned char>(i * 3 + s * 17 + 1);
    syncState = 5; curPlayer = 1; curImportPlayer = 2; playerStatus[1] = 0; RUN();
    CHECK(100, playerStatus[1] == 3 && needReset == 1 && syncState == 1 && playerRace[1] == *reinterpret_cast<int*>(tempBuffer + 2 * 23984 + 84));
    CHECK(101, std::memcmp(tempPlayerStruct + 23984, tempBuffer + 2 * 23984, 21872) == 0 &&
                   std::memcmp(tempPlayerStruct + 23984 + 21880, tempBuffer + 2 * 23984 + 21880, 23984 - 21880) == 0 &&
                   *reinterpret_cast<int*>(tempPlayerStruct + 23984 + 21872) == 0);
    CHECK(102, tempPlayerStruct[0] == 0 && tempPlayerStruct[2 * 23984] == 0 && has("SHOW(header,null,footer,"));
    reset(); syncState = 5; curPlayer = 0; playerStatus[0] = 1; RUN();
    CHECK(103, syncState == 5 && has("FMT(footer,MemcardContinueBack,5,6,0,0,0,0,0,0)") &&
                   has("FMT(buf,MemcardImportWarn,1,0,0,0,0,0,0,0)") && has("SHOW(header,buf,footer,"));
    reset(); syncState = 5; curPlayer = 2; playerStatus[2] = 1; setPad(0, 0); setPad(padBackButton, 0); RUN();
    CHECK(104, syncState == 4);
    reset(); syncState = 5; curPlayer = 2; playerStatus[2] = 1; setPad(padAcceptButton, 0); curImportPlayer = 0; RUN();
    CHECK(105, syncState == 1 && playerStatus[2] == 3 && has("FMT(footer,MemcardContinueBack") && has("MemcardImportWarn,3,"));
    // State 6.
    reset(); syncState = 6; curPlayer = 1; playerStatus[1] = 1; playerRace[1] = 4; RUN();
    CHECK(110, syncState == 6 && has("FMT(footer,MemcardContinueBack") && has("MemcardImportWarn,2,"));
    reset(); syncState = 6; curPlayer = 1; playerStatus[1] = 1; setPad(padAcceptButton, 0); RUN();
    CHECK(111, syncState == 1 && playerStatus[1] == 0 && playerRace[1] == -1 && needReset == 1);
    reset(); syncState = 6; curPlayer = 1; playerStatus[1] = 1; setPad(padBackButton, 0); RUN();
    CHECK(112, syncState == 1 && playerStatus[1] == 1 && needReset == 0);
    reset(); syncState = 6; curPlayer = 1; playerStatus[1] = 2; RUN();
    CHECK(113, syncState == 1 && playerStatus[1] == 0 && playerRace[1] == -1 && !has("MemcardImportWarn"));
    // State 7.
    reset(); syncState = 7; playerSelectSuccessful = 1; playerSelectStartPlayer = 2; curPlayer = 1; playerSelectInfo[32 + 41 * 2] = 0xfe; std::strcpy(playerSelectNextName, "levelA"); RUN();
    CHECK(120, syncState == 1 && needReset == 1 && playerStatus[1] == 2 && playerRace[1] == -2 && is("USTRCPY(header,pcTitle);FULLNAME(stack);PUSH;"));
    reset(); syncState = 7; playerSelectSuccessful = 0; netNetworkMode = 1; std::strcpy(playerSelectNextName, "levelA"); RUN();
    CHECK(121, syncState == 1 && needReset == 0 && !has("PUSH") && playerStatus[0] == 0);
    reset(); syncState = 7; std::strcpy(playerSelectNextName, "other"); RUN();
    CHECK(122, syncState == 7 && keepPlayerPositions == 1 && !has("PUSH"));
    reset(); syncState = 7; std::strcpy(playerSelectNextName, "levelA"); levelName[0] = 'x'; RUN();
    CHECK(123, syncState == 7 && keepPlayerPositions == 1);
    reset(); syncState = 7; keepPlayerPositions = 0; std::strcpy(playerSelectNextName, "levelA"); RUN();
    CHECK(124, keepPlayerPositions == 0);
    // State 9.
    reset(); syncState = 9; flareAlpha = 3; RUN();
    CHECK(130, flareAlpha == 127 && syncState == 9 && is("USTRCPY(header,pcTitle);TITLE(header);TGS(MemcardBack);FMT(footer,MemcardBack,6,0,0,0,0,0,0,0);TGS(MemcardNotFound);FMT(buf,MemcardNotFound,137,0,0,0,0,0,0,0);SHOW(header,buf,footer,127,300,220);"));
    reset(); syncState = 9; setPad(padBackButton, 0); RUN();
    CHECK(131, syncState == 1);
    // State 10 / 11 / 12.
    reset(); syncState = 10; originalPort = 3; originalSlot = 4; asyncMode = 7; RUN();
    CHECK(140, lastCardPort == 3 && lastCardSlot == 4 && MCA_Port == 3 && MCA_Slot == 4 && asyncMode == -1 && oldAsyncMode == 7 && syncState == 0);
    CHECK(141, is("USTRCPY(header,pcTitle);POP(0);"));
    reset(); syncState = 10; netNetworkMode = 1; RUN();
    CHECK(142, is("USTRCPY(header,pcTitle);") && syncState == 0);
    reset(); syncState = 11; originalPort = 3; originalSlot = 4; pcLaunch = 0; asyncMode = 7; RUN();
    CHECK(143, MC_LastModeSuccessful == 1 && lastCardPort == 3 && MCA_Slot == 4 && syncState == 0 && oldAsyncMode == 7 && asyncMode == -1 && is("USTRCPY(header,pcTitle);POP(0);"));
    reset(); syncState = 11; pcLaunch = 1; needReset = 0; asyncMode = 7; delayCount = 9; RUN();
    CHECK(144, syncState == 0 && delayCount == 0 && oldAsyncMode == 7 && asyncMode == -1);
    reset(); syncState = 11; pcLaunch = 1; needReset = 1; MC_FrontEndImport = 1; RUN();
    CHECK(145, is("USTRCPY(header,pcTitle);POP(0);RESETSTATE;DISPDIS(10);VBLANK;CLEAN(0);") && syncState == 12 && same(lname, "kelethin") && same(levelName, "kelethin") && levelName[80] == -1);
    CHECK(146, keepPlayerPositions == 0);
    reset(); syncState = 11; pcLaunch = 1; needReset = 1; MC_FrontEndImport = 0; nmPlayers = 1;
    *reinterpret_cast<void**>(player + 21872) = objects[1]; objects[1][16] = 7; *reinterpret_cast<unsigned short*>(objects[1] + 56) = 0x55; RUN();
    CHECK(147, is("USTRCPY(header,pcTitle);POP(0);FULLNAME(lname);") && keepPlayerPositions == 1 && syncState == 12 && same(levelName, "levelA") && levelName[80] == -1);
    CHECK(148, oldPosBytes()[0] == 7 && playerChangeOldYaw[0] == 0x55);
    reset(); syncState = 11; pcLaunch = 1; needReset = 1; netNetworkMode = 1; MC_FrontEndImport = 1; RUN();
    CHECK(149, !has("POP(") && syncState == 12);
    reset(); syncState = 12; std::strcpy(lname, "levelA"); asyncMode = 4; RUN();
    CHECK(150, syncState == 0 && asyncMode == -1 && oldAsyncMode == 4 && is("USTRCPY(header,pcTitle);FULLNAME(stack);SHOW(header,null,footer,50,300,220);"));
    reset(); syncState = 12; std::strcpy(lname, "levelB"); RUN();
    CHECK(151, syncState == 12);

    // Display mode 0: one panel per local player.
    reset(); syncState = 1; pcNmPlayers = 2; flareDelta = 0; flareAlpha = 9; curPlayer = 0; glowOffset = 40; drawFrameNm = 0x20;
    connect(0); playerStatus[0] = 1; playerRace[0] = 2; playerStatus[1] = 0; connect(1);
    std::strcpy(reinterpret_cast<char*>(player), "Hero A B");
    RUN();
    CHECK(160, has("FS(1,6,9323c3c,-1);TGS(MemcardImportPWho);FMT(buf,MemcardImportPWho,1,0,0,0,0,0,0,0);FPC(254,105,buf,1000000);FE;"));
    // glow is 40 + 1 + 96 = 137 = 0x89 for the current player.
    CHECK(162, has("TDB(199,135,112,105,6,9898989,-,1,0);LFR(hud.lmp,t_c.tex);TDC(t_c.tex,205,136,6,9898989);"));
    CHECK(163, has("TDB(330,135,112,105,6,9404040,-,1,0);TDB(330,135,112,108,6,9404040,-,0,1);"));
    CHECK(164, has("FPWC(254,250,Hero A B,105,-100,580,15)") && has("FPWCU(385,250,MemcardImportAvailablePost,117,-100,580,21)"));
    CHECK(166, has("MARKER(0,9)") && has("FC(abc0000,-1,0)"));
    // Controller missing for a panel.
    reset(); syncState = 8; pcNmPlayers = 1; connect(0); RUN();
    CHECK(167, !has("TDB(") );
    reset(); syncState = 1; pcNmPlayers = 2; flareDelta = 0; connect(0); playerStatus[0] = 1; playerStatus[1] = 1; std::strcpy(reinterpret_cast<char*>(player), "A"); RUN();
    CHECK(168, has("FPWCU(385,250,MemcardImportNoControllerPost,117,-100,580,21)"));
    // Name statuses and truncation, front-end colour.
    reset(); syncState = 1; pcNmPlayers = 3; flareDelta = 0; for (int i = 0; i < 3; ++i) connect(i);
    frontEndRunning = 1; flareAlpha = 7;
    playerStatus[0] = 1; playerStatus[1] = 2; playerStatus[2] = 3;
    std::strcpy(reinterpret_cast<char*>(player), "Verylongnamewithoutspace");
    std::strcpy(reinterpret_cast<char*>(playerSelectInfo + 41), "Ab Cd");
    std::strcpy(reinterpret_cast<char*>(tempPlayerStruct + 2 * 23984), "Averyveryverylonglongfirst Name");
    RUN();
    CHECK(170, has("FC(7235657,-1,0)") && !has("MARKER("));
    CHECK(171, has("FPWC(") && has(",250,Verylongnamewit...,105,-100,580,15)"));
    CHECK(172, has(",250,Ab Cd,105,-100,580,15)"));
    CHECK(173, has(",250,Averyveryverylo...,105,-100,580,15)"));
    // Mode 1 (warning) and the drawing call.
    reset(); syncState = 8; RUN();
    CHECK(174, !has("TDB("));

    // Display mode 3: import list. Two players.
    reset(); syncState = 4; importPlayerCount = 2; flareDelta = 8; flareAlpha = 40; glowOffset = 10; curImportPlayer = 1; drawFrameNm = 0;
    for (int s = 0; s < 2; ++s) {
        unsigned char* r = tempBuffer + 23984 * s;
        std::strcpy(reinterpret_cast<char*>(r), s ? "Bo Bo" : "Alexandria Longname");
        *reinterpret_cast<int*>(r + 84) = s ? 3 : 0;
        r[89] = static_cast<unsigned char>(s ? 12 : 7);
        *reinterpret_cast<int*>(r + 92) = 1500 + s;
        *reinterpret_cast<int*>(r + 232) = 99 + s;
        *reinterpret_cast<float*>(r + 172) = s ? 100.0f : 63.5f;
    }
    RUN();
    // first = 380 - 120 = 260; captions at 157; panel x: 260, 380.
    CHECK(180, has("TGS(MemcardImportAvail);USTRCPY(header,MemcardImportAvail);"));
    CHECK(181, has("FMT(footer,MemcardImportBack,5,6,0,0,0,0,0,0)"));
    CHECK(182, has("FS(1,6,30235657,-1);TGS(MemcardRace);FP(157,235,MemcardRace,1000000);TGS(MemcardLevel);FP(157,271,MemcardLevel,1000000);"));
    CHECK(183, has("FP(157,289,MemcardExp,1000000);TGS(MemcardGold);FP(157,307,MemcardGold,1000000);TGS(MemcardHealth);FP(157,325,MemcardHealth,1000000);FE;"));
    CHECK(184, has("TDB(254,94,112,105,6,30404040,-,1,0);LFR(hud.lmp,t_a.tex);TDC(t_a.tex,260,95,6,30404040);"));
    CHECK(185, has("TDB(374,94,112,105,6,30696969,-,1,0);LFR(hud.lmp,t_d.tex);TDC(t_d.tex,380,95,6,30696969);"));
    CHECK(186, has("FPWC(310,199,Alexandria Longname,117,-100,580,21)"));
    CHECK(187, has("FC(30323c3c,-1,0);") && has("TGS(RaceA);USTRCPY(buf,RaceA);FPWCU(310,235,RaceA,117,-100,580,18)"));
    CHECK(188, has("FPCS(310,271,7,10000);FPCS(310,289,1500,10000);FPCS(310,307,99,10000);FPCS(310,325,64,10000);"));
    CHECK(189, has("FPWC(430,199,Bo Bo,117,-100,580,21)") && has("FPCS(430,271,12,10000);FPCS(430,289,1501,10000);FPCS(430,307,100,10000);FPCS(430,325,100,10000);"));
    CHECK(190, has("SHOW(header,null,footer,48,460,245)"));
    // Mode 6 hudShowMsg uses the buffer title; mode 5 omits the message.
    reset(); syncState = 10; RUN();
    CHECK(191, !has("SHOW("));
    reset(); syncState = 2; RUN();
    CHECK(192, !has("SHOW("));

    std::puts("async change players passed");
    return 0;
}
