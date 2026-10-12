#define NORRATH_HOST_CONTRACT
#include "pause_menu.cpp"
#include <cassert>
#include <cstdarg>
#include <cstdio>
#include <cstring>

extern "C" {
unsigned char* menu;
unsigned menuNm, menuPlayerNm, lastWarpPos;
unsigned char kickMenuList[16], kickMenu[96], preConfigure[80], raceMainMenu[80];
unsigned short hudFooterUni[128];
int endTimer;
unsigned char startMenuState, useRaceMenu, exitToMenuMode, waitNetExitCount;
unsigned gameControlPaused, gamePause, gameBailFromGameLoop;
unsigned char particlesPaused, cutMode, MC_Debug, netNetworkMode;
unsigned nmPlayers;
unsigned char player[4 * 23984], netLocalPad[352];
char levelName[322];
unsigned char* gameCam;
unsigned padAcceptButton, padBackButton;
int padAcceptButtonFontChar, padBackButtonFontChar;
char* defaultFont;
unsigned drawFrameNm;
int frontendControllerConfigRunning;
float soundSfxVolume, soundMusicVolume;
unsigned char packetBuffer[560000];
void* mediumFont;
}

static unsigned char* T1() { return packetBuffer + 0xf7c0; }
static unsigned char* T2() { return packetBuffer + 0xf940; }
static unsigned char* T3() { return packetBuffer + 0xf610; }
static unsigned char* T4() { return packetBuffer + 0xf700; }

static void store_ptr(unsigned char* at, void* p) { memcpy(at, &p, sizeof p); }
static void* load_ptr(unsigned char* at) { void* p = nullptr; memcpy(&p, at, sizeof p); return p; }
static void store_u32(unsigned char* at, unsigned v) {
    at[0] = (unsigned char)v; at[1] = (unsigned char)(v >> 8);
    at[2] = (unsigned char)(v >> 16); at[3] = (unsigned char)(v >> 24);
}
static unsigned load_u32(unsigned char* at) {
    return (unsigned)at[0] | ((unsigned)at[1] << 8) | ((unsigned)at[2] << 16) | ((unsigned)at[3] << 24);
}

static int asyncResult = -1, coordsCalls = 0, cfgCalls = 0;
static int showMsgCalls = 0, yesnoResult = 0, yesnoCalls = 0;
static int hudExits = 0, restores = 0, clears = 0;
static int saveResult = 0, saveCalls = 0, saveA0 = -1, saveA1 = -1;
static int pushes = 0, pops = 0, zones = 0, dlgActive = 0, tasks = 0;
static int draws = 0, runs = 0, pickedResult = 0;
static int drawArgs[4];
static void *drawMenuArg, *drawFontArg, *drawTexArg;
static unsigned drawCA, drawCB;
static int runPadOff = -1, setposCalls = 0;
static void* setposMenu = nullptr;
static int setposPos = -1, statusResult = 0, statusCalls = 0;
static int fadeCalls = 0, musCalls = 0, adjCalls = 0;
static int seqCalls = 0, musicCalls = 0;
static void *seqTex = nullptr, *musicName = nullptr;
static int musicIdx = -1, localNm = 0, connectedMap[8] = {0, 0, 0, 0, 0, 0, 0, 0};
static int browsers = 0, browserArg = -1, loads = 0, changes = 0;
static int recalls = 0, recallArg = 0, warps = 0, cleans = 0, colorCalls = 0;
static int kicks = 0, kickArg = -1, flares = 0, inits = 0, initArg = -1;
static int shops = 0;
static char textKey[64], lumpA[64], lumpB[64];
static int usprintfCalls = 0;
static int printCenterCalls = 0;
static float sfxDuringRun = 0, musDuringRun = 0;
static int changeSfx = 0, changeMus = 0;

static void reset_stubs() {
    asyncResult = -1; coordsCalls = cfgCalls = 0;
    showMsgCalls = yesnoResult = yesnoCalls = 0;
    hudExits = restores = clears = 0;
    saveResult = saveCalls = 0; saveA0 = saveA1 = -1;
    pushes = pops = zones = dlgActive = tasks = 0;
    draws = runs = 0; pickedResult = 0;
    drawMenuArg = drawFontArg = drawTexArg = nullptr;
    memset(drawArgs, 0, sizeof drawArgs); drawCA = drawCB = 0;
    runPadOff = -1; setposCalls = 0; setposMenu = nullptr; setposPos = -1;
    statusResult = statusCalls = fadeCalls = 0;
    musCalls = adjCalls = seqCalls = musicCalls = 0;
    seqTex = musicName = nullptr; musicIdx = -1;
    localNm = 0; browsers = 0; browserArg = -1; loads = changes = 0;
    changeSfx = changeMus = 0; sfxDuringRun = musDuringRun = 0;
    recalls = 0; recallArg = 0; warps = 0; cleans = colorCalls = 0;
    kicks = 0; kickArg = -1; flares = inits = 0; initArg = -1; shops = 0;
    memset(textKey, 0, sizeof textKey); memset(lumpA, 0, sizeof lumpA); memset(lumpB, 0, sizeof lumpB);
    usprintfCalls = printCenterCalls = 0;
    memset(connectedMap, 0, sizeof connectedMap);
}
static void reset_state() {
    menu = nullptr; menuNm = menuPlayerNm = lastWarpPos = 0;
    endTimer = 0;
    startMenuState = useRaceMenu = exitToMenuMode = waitNetExitCount = 0;
    gameControlPaused = gamePause = gameBailFromGameLoop = 0;
    particlesPaused = cutMode = MC_Debug = netNetworkMode = 0;
    nmPlayers = 0;
    padAcceptButton = padBackButton = 0;
    padAcceptButtonFontChar = padBackButtonFontChar = 0;
    drawFrameNm = 0; frontendControllerConfigRunning = 0;
    soundSfxVolume = soundMusicVolume = 0;
    memset(kickMenuList, 0, sizeof kickMenuList); memset(kickMenu, 0, sizeof kickMenu);
    memset(preConfigure, 0, sizeof preConfigure); memset(raceMainMenu, 0, sizeof raceMainMenu);
    memset(hudFooterUni, 0, sizeof hudFooterUni);
    memset(player, 0, sizeof player); memset(netLocalPad, 0, sizeof netLocalPad);
    memset(levelName, 0, sizeof levelName);
    memset(packetBuffer, 0, sizeof packetBuffer);
    reset_stubs();
}

extern "C" {
int MC_GetAsyncMode__Fv() { return asyncResult; }
void showCoords__Fv() { ++coordsCalls; }
void frontendRunControllerConfig__Fv() { ++cfgCalls; }
void frontendInitControllerConfig__Fi(int i) { ++inits; initArg = i; }
void hudShowMsg__FPUsN20iii(unsigned short*, unsigned short*, int, int, int, int) { ++showMsgCalls; }
int hudRunYesNo__FPUsiiii(unsigned short*, int, int, int, int) { ++yesnoCalls; return yesnoResult; }
void msgHudPlayerExit__Fv() { ++hudExits; }
void netRestoreButtonConfigs__Fv() { ++restores; }
void debugClearStartMenu__Fv() { ++clears; }
int gameMaybeSaveConfig__Fii(int a, int b) { ++saveCalls; saveA0 = a; saveA1 = b; return saveResult; }
void gamePushConfig__Fv() { ++pushes; }
void gamePopConfig__Fv() { ++pops; }
int gameCheckNoSaveZone__Fv() { return zones; }
int DLG_Active__Fv() { return dlgActive; }
int engineTaskActive__FPc(char*) { ++tasks; return 0; }
void* lumpFindResource__FPcT0(char* l, char* n) {
    strncpy(lumpA, l, 63); strncpy(lumpB, n, 63);
    static unsigned char t[8]; return t;
}
void menuSetPos__FP4Menui(unsigned char* m, int p) { ++setposCalls; setposMenu = m; setposPos = p; }
void menuDraw__FiiP4MenuUiUiP4FontP8_texturei(int x, int y, void* m, unsigned a, unsigned b,
                                             void* f, void* t, int e) {
    ++draws; drawArgs[0] = x; drawArgs[1] = y; drawMenuArg = m;
    drawCA = a; drawCB = b; drawFontArg = f; drawTexArg = t; drawArgs[2] = e;
}
int menuRun__FP4MenuP3padPi(void* m, void* p, int f) {
    ++runs; (void)m; runPadOff = (int)((unsigned char*)p - netLocalPad); (void)f;
    if (changeSfx) soundSfxVolume = sfxDuringRun;
    if (changeMus) soundMusicVolume = musDuringRun;
    return pickedResult;
}
unsigned short* textGetString__FPc(char* k) {
    strncpy(textKey, k, 63); static unsigned short s[] = {'K', 0}; return s;
}
void usprintf__FPUsT0ffffffff(unsigned short*, unsigned short*, float, float, float, float,
                              float, float, float, float) { ++usprintfCalls; }
void fontStart__FP4FontiUii(void*, int, unsigned, int) {}
int fontPrintCenteredu__FiiPUsi(int, int, unsigned short*, int) { ++printCenterCalls; return 0; }
void fontEnd__Fv() {}
void textureDrawColor__FP8_textureiiiUi(void*, int, int, int, unsigned) {}
int soundGetStreamPlayStatus__Fi(int) { ++statusCalls; return statusResult; }
void soundFadeStream__Fi(int) { ++fadeCalls; }
void soundUpdateMusicVolume__Fv() { ++musCalls; }
void soundUpdateAdjustments__Fv() { ++adjCalls; }
int soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* h, void*, float) {
    ++seqCalls; seqTex = h; return 0;
}
void soundStartGlobalMusic__FPci(char* n, int i) { ++musicCalls; musicName = n; musicIdx = i; }
int netGetLocalPlayerNm__Fi(int) { return localNm; }
int netPlayerIsConnected__Fi(int i) { return (i >= 0 && i < 8) ? connectedMap[i] : 0; }
void MC_SaveGameBrowser__Fi(int p) { ++browsers; browserArg = p; }
void MC_LoadGameBrowser__Fii(int p, int) { ++loads; browserArg = p; }
void MC_ChangePlayers__FiPUsiii(int, unsigned short*, int, int, int) { ++changes; }
void msgHudRecall__Fc(signed char i) { ++recalls; recallArg = i; }
void hudNetLevelWarp__FPc(char*) { ++warps; }
void cleanUp__Fv() { ++cleans; }
void colorLevelNames__FP4Menu(void*) { ++colorCalls; }
void netKickPlayer__Fi(int i) { ++kicks; kickArg = i; }
void coreInterfaceFlare__Fi(int) { ++flares; }
void shopStart__Fiii(int, int, int) { ++shops; }
}

static unsigned char gameCamBuf[64];
static unsigned char menubuf[8 * 16];
static unsigned char casebuf[8 * 16];

int main() {
    gameCam = gameCamBuf;
    mediumFont = (void*)0x11;
    defaultFont = (char*)0x22;
    // Async mode returns after the table fixup.
    {
        reset_state();
        store_ptr(T1(), (void*)"menu_exittomenu");
        store_u32(T1() + 12, 0);
        store_ptr(T1() + 16, nullptr);
        asyncResult = 5;
        menu = menubuf;
        pauseMenuFrameRun__Fv();
        assert(strcmp((char*)load_ptr(T1()), "menu_exitmultiplayer") == 0);
        assert(draws == 0);
    }
    // Fixup swaps and kick bits; the full path then draws and returns.
    {
        reset_state();
        store_ptr(T1(), (void*)"menu_exittomenu");
        store_ptr(T1() + 16, (void*)"menu_kickplayers");
        store_u32(T1() + 16 + 12, 0);
        store_ptr(T1() + 32, (void*)"other");
        store_u32(T1() + 32 + 12, 9);
        store_ptr(T1() + 48, nullptr);
        store_ptr(menubuf, nullptr);
        menu = menubuf; menuNm = 99;
        pauseMenuFrameRun__Fv();
        assert(strcmp((char*)load_ptr(T1()), "menu_exitmultiplayer") == 0);
        assert(load_u32(T1() + 16 + 12) == 1u);
        assert(load_u32(T1() + 32 + 12) == 9u);
        assert(draws == 1 && runs == 1 && musCalls == 1 && adjCalls == 1);
        assert(drawArgs[0] == 320 && drawArgs[1] == 240 && drawMenuArg == (void*)menubuf);
        assert(drawCA == 0x4e646868u && drawCB == 0x7d4c6a73u);
        assert(drawFontArg == (void*)0x22 && drawArgs[2] == 0);
        assert(strcmp(lumpA, "hud.lmp") == 0 && strcmp(lumpB, "helpbar.tex") == 0);
        assert(printCenterCalls == 1);
    }
    // Networked kick bit clears with two connected players.
    {
        reset_state();
        netNetworkMode = 1; localNm = 0; nmPlayers = 4;
        connectedMap[1] = 1; connectedMap[2] = 1;
        store_ptr(T1(), (void*)"menu_kickplayers");
        store_u32(T1() + 12, 3);
        store_ptr(T1() + 16, nullptr);
        store_ptr(menubuf, nullptr);
        menu = menubuf; menuNm = 99;
        pauseMenuFrameRun__Fv();
        assert(load_u32(T1() + 12) == 2u);
        reset_state();
        netNetworkMode = 1; localNm = 0; nmPlayers = 4;
        connectedMap[1] = 0;
        store_ptr(T1(), (void*)"menu_kickplayers");
        store_u32(T1() + 12, 2);
        store_ptr(T1() + 16, nullptr);
        store_ptr(menubuf, nullptr);
        menu = menubuf; menuNm = 99;
        pauseMenuFrameRun__Fv();
        assert(load_u32(T1() + 12) == 3u);
        // The fourth player slot counts too.
        reset_state();
        netNetworkMode = 1; localNm = 0; nmPlayers = 5;
        connectedMap[4] = 1;
        store_ptr(T1(), (void*)"menu_kickplayers");
        store_u32(T1() + 12, 3);
        store_ptr(T1() + 16, nullptr);
        store_ptr(menubuf, nullptr);
        menu = menubuf; menuNm = 99;
        pauseMenuFrameRun__Fv();
        assert(load_u32(T1() + 12) == 2u);
    }
    // Coordinates and controller config gates.
    {
        reset_state();
        MC_Debug = 1;
        store_ptr(menubuf, nullptr);
        menu = menubuf; menuNm = 99;
        pauseMenuFrameRun__Fv();
        assert(coordsCalls == 1);
        reset_state();
        MC_Debug = 1;
        store_ptr(menubuf, nullptr);
        menu = T2(); menuNm = 99;
        pauseMenuFrameRun__Fv();
        assert(coordsCalls == 0);
        reset_state();
        frontendControllerConfigRunning = 1;
        store_ptr(menubuf, nullptr);
        menu = menubuf; menuNm = 99;
        pauseMenuFrameRun__Fv();
        assert(cfgCalls == 1 && draws == 0);
    }
    // Exit countdown ticks down, then bails out.
    {
        reset_state();
        waitNetExitCount = 3;
        store_ptr(menubuf, nullptr);
        menu = menubuf;
        pauseMenuFrameRun__Fv();
        assert(waitNetExitCount == 2 && showMsgCalls == 1 && strcmp(textKey, "netDisconnectTitle") == 0);
        assert(hudExits == 0 && gameBailFromGameLoop == 0u);
        waitNetExitCount = 1;
        pauseMenuFrameRun__Fv();
        assert(hudExits == 1 && restores == 1 && clears == 1);
        assert(netNetworkMode == 0 && gameBailFromGameLoop == 1u);
    }
    // Yes/no answers clear, arm, or return.
    {
        reset_state();
        exitToMenuMode = 1; netNetworkMode = 1;
        store_ptr(menubuf, nullptr);
        menu = menubuf;
        yesnoResult = 1;
        pauseMenuFrameRun__Fv();
        assert(strcmp(textKey, "exitMultiplayerMsg") == 0);
        assert(exitToMenuMode == 0 && waitNetExitCount == 0 && hudExits == 0);
        exitToMenuMode = 1; yesnoResult = 2;
        pauseMenuFrameRun__Fv();
        assert(waitNetExitCount == 120 && hudExits == 1 && exitToMenuMode == 0);
        exitToMenuMode = 1; yesnoResult = 7;
        pauseMenuFrameRun__Fv();
        assert(exitToMenuMode == 1 && hudExits == 1);
        reset_state();
        exitToMenuMode = 1; netNetworkMode = 0;
        store_ptr(menubuf, nullptr);
        menu = menubuf;
        yesnoResult = 2;
        pauseMenuFrameRun__Fv();
        assert(strcmp(textKey, "exitToMenuMsg") == 0 && waitNetExitCount == 1);
    }
    // Save state 51: fail pops, accept resets, back pops, neither returns.
    {
        reset_state();
        menuNm = 51; saveResult = 0;
        store_ptr(menubuf, nullptr);
        menu = menubuf;
        pauseMenuFrameRun__Fv();
        assert(saveCalls == 1 && pops == 1 && menuNm == 0u && draws == 0);
        reset_state();
        menuNm = 51; saveResult = 1; localNm = 0;
        padAcceptButton = 0x40;
        store_u32(netLocalPad + 12, 0x40);
        store_ptr(menubuf, nullptr);
        menu = menubuf;
        pauseMenuFrameRun__Fv();
        assert(menu == T1() && menuNm == 0u && saveCalls == 2);
        assert(saveA0 == 0 && saveA1 == 0 && usprintfCalls == 1 && showMsgCalls == 1);
        reset_state();
        menuNm = 51; saveResult = 1; localNm = 0;
        padAcceptButton = 0x40; padBackButton = 0x120;
        store_u32(netLocalPad + 12, 0x120);
        store_ptr(menubuf, nullptr);
        menu = menubuf;
        pauseMenuFrameRun__Fv();
        assert(pops == 1 && menu == (void*)menubuf);
        reset_state();
        menuNm = 51; saveResult = 1; localNm = 0;
        padAcceptButton = 0x40; padBackButton = 0x120;
        store_u32(netLocalPad + 12, 0);
        store_ptr(menubuf, nullptr);
        menu = menubuf;
        pauseMenuFrameRun__Fv();
        assert(menuNm == 51u && pops == 0 && draws == 0);
    }
    // Menu walk enables entries by mode and checks.
    {
        reset_state();
        netNetworkMode = 1;
        store_ptr(menubuf, (void*)"x");
        store_ptr(menubuf + 16, (void*)"menu_loadgame");
        store_ptr(menubuf + 32, (void*)"menu_controls");
        store_ptr(menubuf + 48, (void*)"menu_recall");
        store_ptr(menubuf + 64, (void*)"menu_changeplayers");
        store_ptr(menubuf + 80, nullptr);
        menu = menubuf; menuNm = 99;
        // Recall path: hurt set, dialog idle, jumpgate idle, camera word
        // set, but no entity.
        float hurt = 5.0f;
        memcpy(player + 172, &hurt, 4);
        store_u32(gameCamBuf + 52, 1);
        padBackButton = 0x120;
        pauseMenuFrameRun__Fv();
        assert(load_u32(menubuf + 16 + MENU_FLAG_OFF) == 5u);
        assert(load_u32(menubuf + 32 + MENU_FLAG_OFF) == 5u);
        assert(load_u32(menubuf + 48 + MENU_FLAG_OFF) == 0u);
        assert(load_u32(menubuf + 64 + MENU_FLAG_OFF) == 5u);
        assert(draws == 1); // changeplayers ends the walk through the draw
    }
    // Offline walk disables, and hurt alone enables recall.
    {
        reset_state();
        netNetworkMode = 0;
        store_ptr(menubuf, (void*)"menu_loadgame");
        store_ptr(menubuf + 16, (void*)"menu_recall");
        store_ptr(menubuf + 32, nullptr);
        menu = menubuf; menuNm = 99;
        float hurt = -1.0f;
        memcpy(player + 172, &hurt, 4);
        padBackButton = 0x120;
        pauseMenuFrameRun__Fv();
        assert(load_u32(menubuf + MENU_FLAG_OFF) == 0u);
        assert(load_u32(menubuf + 16 + MENU_FLAG_OFF) == 5u);
        assert(tasks == 0); // hurt short-circuits the remaining checks
    }
    // Pre-configuration menu copies player names.
    {
        reset_state();
        static unsigned char n0[32], n1[32];
        strcpy((char*)n0, "old0"); strcpy((char*)n1, "old1");
        store_ptr(preConfigure, n0);
        store_ptr(preConfigure + 16, n1);
        menu = preConfigure; menuNm = 99;
        nmPlayers = 2;
        strcpy((char*)player, "Al");
        strcpy((char*)player + 23984, "Bo");
        padBackButton = 0x120;
        pauseMenuFrameRun__Fv();
        assert(strcmp((char*)n0, "Al") == 0 && strcmp((char*)n1, "Bo") == 0);
        assert(load_u32(preConfigure + 32) == 0u);
    }
    // Music, timer, and accept-sound gates around the run.
    {
        reset_state();
        store_ptr(menubuf, nullptr);
        menu = menubuf; menuNm = 99; menuPlayerNm = 0;
        pickedResult = 2;
        soundSfxVolume = 1.0f; soundMusicVolume = 2.0f;
        drawFrameNm = 1;
        padAcceptButton = 0x40;
        store_u32(netLocalPad + 12, 0x40);
        pauseMenuFrameRun__Fv();
        assert(seqCalls == 1 && strcmp(lumpB, "menu_main_accept.anm") == 0);
        assert(runPadOff == 0 && endTimer == 1);
        // Volumes changed mid-frame replay the tab sound.
        reset_stubs();
        changeSfx = 1; sfxDuringRun = 9.0f;
        store_u32(netLocalPad + 12, 0);
        pauseMenuFrameRun__Fv();
        assert(seqCalls == 1 && strcmp(lumpB, "menu_shop_tab.anm") == 0);
        // Changed music with the stream flag restarts the music.
        reset_stubs();
        changeMus = 1; musDuringRun = 9.0f;
        statusResult = 0x20;
        pauseMenuFrameRun__Fv();
        assert(musicCalls == 1 && strcmp((char*)musicName, "mx_main_l.vag") == 0 && musicIdx == 65);
        assert(endTimer == 1); // restarted by the music, then ticked once
        // A hot timer fades the stream.
        reset_stubs();
        endTimer = 241;
        statusResult = 0x40;
        pauseMenuFrameRun__Fv();
        assert(endTimer == 241 && fadeCalls == 1);
        reset_stubs();
        statusResult = 0;
        pauseMenuFrameRun__Fv();
        assert(fadeCalls == 0);
    }
    // Back button returns before transitions.
    {
        reset_state();
        store_ptr(menubuf, nullptr);
        menu = menubuf; menuNm = 7; menuPlayerNm = 0;
        padBackButton = 0x120;
        store_u32(netLocalPad + 12, 0);
        pauseMenuFrameRun__Fv();
        assert(flares == 0 && menuNm == 7u);
    }
    // State-0 transitions clear or take the race menu.
    {
        reset_state();
        store_ptr(menubuf, (void*)"zzz");
        store_ptr(menubuf + 16, nullptr);
        menu = menubuf; menuNm = 0; menuPlayerNm = 0;
        padBackButton = 0x120;
        store_u32(netLocalPad + 12, 0x120);
        statusResult = 0x40;
        pauseMenuFrameRun__Fv();
        assert(clears == 1 && gameControlPaused == 60u && fadeCalls == 1);
        reset_state();
        store_ptr(T1(), (void*)"zzz");
        store_ptr(T1() + 16, nullptr);
        menu = T1(); menuNm = 0; menuPlayerNm = 0; useRaceMenu = 1;
        padBackButton = 0x120;
        store_u32(netLocalPad + 12, 0x120);
        pauseMenuFrameRun__Fv();
        assert(menu == (void*)raceMainMenu && flares == 1 && clears == 0);
        // Other states rotate: 3 and 5 become 2, 2 becomes 51, rest reset.
        reset_state();
        store_ptr(menubuf, nullptr);
        menu = menubuf; menuNm = 3; menuPlayerNm = 0;
        padBackButton = 0x120;
        store_u32(netLocalPad + 12, 0x120);
        pauseMenuFrameRun__Fv();
        assert(menuNm == 2u && menu == (void*)T3() && flares == 1 && seqCalls == 1);
        assert(strcmp(lumpB, "menu_main_back.anm") == 0);
        store_ptr(T3(), (void*)"zzz");
        store_ptr(T3() + 16, nullptr);
        menuNm = 2;
        pauseMenuFrameRun__Fv();
        assert(menuNm == 51u);
        menuNm = 4;
        pauseMenuFrameRun__Fv();
        assert(menuNm == 0u && menu == (void*)T1());
    }
    // Dispatch: return to game clears, pauses, fades, and repositions.
    {
        reset_state();
        store_ptr(casebuf, (void*)"menu_returntogame");
        store_ptr(casebuf + 16, nullptr);
        menu = casebuf; menuNm = 0; menuPlayerNm = 0;
        pickedResult = 0;
        padBackButton = 0x120;
        store_u32(netLocalPad + 12, 0x120);
        statusResult = 0x40;
        pauseMenuFrameRun__Fv();
        // The case body and the state-0 tail both clear and fade.
        assert(clears == 2 && gameControlPaused == 60u && fadeCalls == 2);
        assert(setposCalls == 0);
        // Save browser forwards the player index.
        reset_state();
        store_ptr(casebuf, (void*)"menu_savegame");
        store_ptr(casebuf + 16, nullptr);
        menu = casebuf; menuNm = 0; menuPlayerNm = 3;
        pauseMenuFrameRun__Fv();
        assert(browsers == 1 && browserArg == 3);
        // Shop starts and clears.
        reset_state();
        store_ptr(casebuf, (void*)"Enter Shop");
        store_ptr(casebuf + 16, nullptr);
        menu = casebuf; menuNm = 0;
        pauseMenuFrameRun__Fv();
        assert(shops == 1 && clears == 1);
        // The submenu entry parks the first table without repositioning.
        reset_state();
        store_ptr(casebuf, (void*)"menu_moremenu");
        store_ptr(casebuf + 16, nullptr);
        menu = casebuf; menuNm = 0;
        pauseMenuFrameRun__Fv();
        assert(menu == (void*)T1());
        assert(setposCalls == 1 && setposMenu == (void*)T1() && setposPos == 0);
    }
    // Dispatch: level select positions the warp and skips the tail setpos.
    {
        reset_state();
        store_ptr(casebuf, (void*)"menu_levelwarp");
        store_ptr(casebuf + 16, nullptr);
        store_ptr(T2(), (void*)"zzz");
        store_ptr(T2() + 16, (void*)"abc");
        store_ptr(T2() + 32, nullptr);
        strcpy(levelName + 81, "abc");
        menu = casebuf; menuNm = 0;
        pickedResult = 0;
        padBackButton = 0x120;
        store_u32(netLocalPad + 12, 0);
        pauseMenuFrameRun__Fv();
        assert(menu == (void*)T2() && menuNm == 1u && colorCalls == 1);
        assert(setposCalls == 1 && setposMenu == T2() && setposPos == 1);
        // Without a match the stored warp position is used.
        reset_state();
        store_ptr(casebuf, (void*)"menu_levelwarp");
        store_ptr(casebuf + 16, nullptr);
        store_ptr(T2(), (void*)"zzz");
        store_ptr(T2() + 16, nullptr);
        strcpy(levelName + 81, "abc");
        lastWarpPos = 7;
        menu = casebuf; menuNm = 0;
        padBackButton = 0x120;
        store_u32(netLocalPad + 12, 0);
        pauseMenuFrameRun__Fv();
        assert(setposPos == 7);
    }
    // Dispatch: warp names, controls, controller init, and kicks.
    {
        reset_state();
        store_ptr(casebuf, (void*)"whatever");
        store_ptr(casebuf + 16, nullptr);
        menu = casebuf; menuNm = 1;
        pickedResult = 0; cutMode = 1;
        pauseMenuFrameRun__Fv();
        assert(warps == 0); // cutscene mode skips everything
        cutMode = 0; netNetworkMode = 0;
        pauseMenuFrameRun__Fv();
        assert(strcmp(levelName, "whatever") == 0 && levelName[80] == (char)0xff);
        assert(gameControlPaused == 60u && gamePause == 0u && startMenuState == 0);
        reset_state();
        store_ptr(casebuf, (void*)"whatever");
        store_ptr(casebuf + 16, nullptr);
        menu = casebuf; menuNm = 1;
        pickedResult = 0; cutMode = 0; netNetworkMode = 1;
        pauseMenuFrameRun__Fv();
        assert(lastWarpPos == 0u && warps == 1);
        reset_state();
        store_ptr(casebuf, (void*)"menu_controls");
        store_ptr(casebuf + 16, nullptr);
        menu = casebuf; menuNm = 2;
        pickedResult = 0;
        pauseMenuFrameRun__Fv();
        assert(menuNm == 3u && menu == (void*)preConfigure);
        reset_state();
        store_ptr(casebuf, (void*)"menu_comm");
        store_ptr(casebuf + 16, nullptr);
        menu = casebuf; menuNm = 2;
        pauseMenuFrameRun__Fv();
        assert(menuNm == 5u && menu == (void*)T4());
        reset_state();
        menu = casebuf; menuNm = 3;
        pickedResult = 6;
        pauseMenuFrameRun__Fv();
        assert(inits == 1 && initArg == 6);
        reset_state();
        menu = casebuf; menuNm = 4;
        pickedResult = 1;
        kickMenuList[1] = 5;
        pauseMenuFrameRun__Fv();
        assert(kicks == 1 && kickArg == 5 && clears == 1);
    }
    // Dispatch: kick setup collects connected players.
    {
        reset_state();
        store_ptr(casebuf, (void*)"menu_kickplayers");
        store_ptr(casebuf + 16, nullptr);
        menu = casebuf; menuNm = 0;
        pickedResult = 0; nmPlayers = 3;
        connectedMap[1] = 1; connectedMap[2] = 0;
        pauseMenuFrameRun__Fv();
        assert(kickMenuList[1] == 1 && menuNm == 4u && menu == (void*)kickMenu);
        assert(setposCalls == 1 && setposMenu == kickMenu && setposPos == 0);
    }
    return 0;
}
