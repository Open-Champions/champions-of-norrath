// Observed gameLoop at 0x002cf048, 2812 bytes. Its one caller is main.
// Setup: frameNm is cleared. A nonzero netNetworkMode clears eRandState;
// otherwise eRandState and syncRandState are two reads of the COP0 count
// register. levelName[242] and levelName[81] are cleared, then gameInit runs
// and seven tasks are registered with engineAddTask (function, priority,
// name, flags): runwater 0/2, gameSetObjectVis 0/0, plotroute 1000/0,
// gameMiscSync 1/0, textMessageProcess 123/0, textMessageDraw 125/1 and
// commProcess 124/0. runwater is added before inventoryInit; the others
// follow once supersampleMode is 1, display disable is 1, gameBailFromGameLoop
// is 0 and coreInterfaceBrightness[0..1] are 1.0f. A nonzero first word of
// superGamestateInit is passed with the second word to scriptSetGameState and
// cleared.
// Each pass of the loop (until gameBailFromGameLoop is nonzero):
//  - While coreLevelCommentTimer is positive and cutMode, gamePause,
//    interfaceOn and DLG_Active are all zero, it decreases; reaching 1 with
//    nmPlayers positive steps eRandState (* 1664525 + 1013904223) and plays
//    sound 2 for player (eRandState >> 16) % nmPlayers.
//  - With levelName[0] set, gamePlayersPresent nonzero, no "frontend" task,
//    nmPlayers positive and MC_Busy zero, levelName[0] is cleared unless some
//    player record has a float above zero at +172 (records are 23984 bytes).
//  - coreHeadsetResetTimer counts down and sets commEnable when it reaches 0.
//  - With levelName[0] set, the level start runs: peers wait when networked,
//    "to level %s", start-network game when broadcasting, the Camera reset
//    unless MC_GetAsyncMode is 7 and MC_FrontEndImport is clear, startNewWorld,
//    scriptRun, the loading loop (cap 301 frames, texIsLoading re-arms
//    coreLoading at 10, preCacheArmor holds it at 3), network contact wait,
//    the shopStart hub names at levelName+81, the loading thread wait, and the
//    "env.lmp"/"etex" environment texture.
//  - The frame: engineFrameStart, lumpCheck, at most two AI steps, network or
//    pad step, tasks, scene-switch / loading / decompress bookkeeping, music,
//    sound, anim, cd, texture, item picture work, a minute message every 600
//    frames, engineFrameEnd, then the interface brightness and fade values
//    move toward 1.0.
// After the loop: display disable 2, boss meter words and cheatOn cleared,
// script, game, streams 0-2 and lump 10 cleared, and frontEndInit(1) when the
// exit value is 101.
// Unknown: what the loading-thread flag read at 0x004866d4 is for (one byte
// is read once before the wait and not used); the types of bossMeterCurrent,
// bossMeterAlpha and world (only zero is stored); the Player layout beyond
// the words used here; the meaning of the 1638/1649/1700/1741/1792 line numbers
// beyond being passed to _engineCheckStack; the strings "a\n".."d\n".
extern "C" {
void gameInit__Fv();
void engineAddTask__FPFv_viPci(void (*task)(), int priority, const char* name, int flags);
void worldRunWaterTask__Fv();
void gameSetObjectVis__Fv();
void plotRouteTask__Fv();
void gameMiscTaskSync__Fv();
void textMessageProcess__Fv();
void textMessageDraw__Fv();
void commProcess__Fv();
void inventoryInit__Fv();
void engineSetDisplayDisable__Fi(int value);
void lightVU0StoppedProcessing__Fv();
void scriptSetGameState__FPci(const char* name, int state);
int DLG_Active__Fv();
void playerPlaySound__Fii(int player, int sound);
int engineTaskActive__FPc(const char* name);
int MC_Busy__Fv();
int subDrawLoop__FPFv_i(int (*callback)());
int mcProcessKick__Fv();
void netWaitForPeersToCatchUp__Fv();
void netInit__Fv();
void netStartNetworkGameInit__Fv();
int netStartNetworkGameFrame__Fv();
void simpleDrawLoop__FPFv_i(int (*callback)());
void netStart__Fv();
int MC_GetAsyncMode__Fv();
void Reset__6Cameraii(unsigned char* camera, int first, int second);
void _engineCheckStack__FPCci(const char* file, int line);
void startNewWorld__Fv();
void enginePushBorderColor__FUi(unsigned color);
void enginePopBorderColor__Fv();
int scePrintf(const char* format, ...);
void scriptRun__Fv();
void netStartLevel__Fv();
void enumerateAlivePlayers__Fv();
void Update__6Camera(unsigned char* camera);
void gameDrawWorld__Fv();
void cutRunFrame__Fb(int value);
void cdProcess__Fv();
void texProcessLoad__Fi(int value);
void texProcessDecompress__Fv();
int texIsLoading__Fv();
int preCacheArmor__6Player(unsigned char* player);
void netGameStep__FbT0(int first, int second);
void netClearContact__Fv();
int netInContact__Fv();
int strcmp(const char* left, const char* right);
void shopStart__Fiii(int first, int second, int third);
int ReferThreadStatus(int thread, void* status);
void P_Clear__Fv();
void drawListClear__Fv();
void engineSetScreenColor__Fiiii(int red, int green, int blue, int alpha);
struct GameLoopLump;
struct GameLoopLumpEntry {
    int unknown;
    unsigned char* resource;
};
GameLoopLump* lumpQuery__FPc(const char* name);
GameLoopLumpEntry* lumpFindByType__FPvPci(GameLoopLump* lump, const char* type, int index);
void textureInitTexture__FP8_texture(unsigned char* texture);
void engineLoadEnvTex__FP8_texture(unsigned char* texture);
unsigned sceSifQueryMaxFreeMemSize();
unsigned sceSifQueryTotalFreeMemSize();
void engineFrameStart__FUib(unsigned color, int second);
void lumpCheck__Fv();
int engineGetAIStepCount__Fv();
void padProcess__Fv();
void coreGameStep__Fii(int steps, int second);
void engineRunTasks__Fii(int first, int second);
void engineSetBlur__Fi(int value);
int texIsDecompressing__Fv();
void engineSetVideoMode__Fb(int value);
void runMusic__Fv();
void soundFrame__Fv();
void animFrame__Fv();
void picProcess__4itemv();
void engineFrameEnd__Fb(int value);
void scriptSetScript__FPv(void* script);
void gameClear__Fi(int value);
void soundResetStream__Fi(int stream);
void lumpClear__Fi(int value);
void playerDeallocateArmorCaches__Fv();
void debugClearStartMenu__Fv();
void frontEndInit__Fi(int value);

struct GameLoopStateInit {
    const char* name;
    int state;
};
extern GameLoopStateInit superGamestateInit;
extern signed char netNetworkMode;
extern signed char netBroadCastStart;
extern int netTrouble;
extern unsigned eRandState;
extern unsigned syncRandState;
extern int frameNm;
extern int drawFrameNm;
extern unsigned char supersampleMode;
extern int gameBailFromGameLoop;
extern float coreInterfaceBrightness[4];
extern float coreInterfaceFadeUpVal[4];
extern int coreLevelCommentTimer;
extern signed char cutMode;
extern int gamePause;
extern signed char interfaceOn;
extern int nmPlayers;
extern int gamePlayersPresent;
extern int coreHeadsetResetTimer;
extern int commEnable;
extern signed char MC_KickProtect;
extern signed char MC_FrontEndImport;
extern signed char MC_SavingOrLoading;
extern signed char playerRecalled;
extern signed char recallEnter;
extern int musicTimer;
extern unsigned char* gameCam;
extern signed char particlesPaused;
extern signed char worldDrawEnable;
extern signed char gameCreatureThinkSpinner;
extern int coreLoading;
extern int coreLoadingStart;
extern signed char loadingScreenRequestExit;
extern signed char coreLoadingScreenRunning;
extern int loadThreadID;
extern int engineFullFrameMode;
extern int coreSwitchScene;
extern int coreSwitchSceneStart;
extern unsigned coreClearColor;
extern int engineLastAITime;
extern int engineDisplayDisable;
extern int bossMeterCurrent;
extern int bossMeterAlpha;
extern signed char cheatOn;
extern int world;
extern char levelName[];
extern unsigned char player[];
}

#if defined(NORRATH_HOST_CONTRACT)
extern "C" unsigned norrath_host_timer0_read();
extern "C" void norrath_host_timer0_clear();
extern "C" unsigned norrath_host_cop0_count();
#define NORRATH_TIMER0_READ() norrath_host_timer0_read()
#define NORRATH_TIMER0_CLEAR() norrath_host_timer0_clear()
#define NORRATH_COP0_COUNT() norrath_host_cop0_count()
#else
#define NORRATH_TIMER0_READ() (*reinterpret_cast<volatile unsigned*>(0x10000000u))
#define NORRATH_TIMER0_CLEAR() (*reinterpret_cast<volatile unsigned*>(0x10000000u) = 0u)
static inline unsigned norrath_cop0_count() {
    unsigned value;
    asm volatile("mfc0 %0, $9" : "=r"(value));
    return value;
}
#define NORRATH_COP0_COUNT() norrath_cop0_count()
#endif

namespace {
const unsigned kPlayerStride = 23984u;
const unsigned kPlayerFloatOffset = 172u;
const unsigned kPlayerPointerOffset = 21872u;

void startLevel() {
    if (netNetworkMode) {
        if (MC_KickProtect) subDrawLoop__FPFv_i(mcProcessKick__Fv);
        if (netNetworkMode) netWaitForPeersToCatchUp__Fv();
    }
    scePrintf("to level %s\n", levelName);
    frameNm = 0;
    if (netNetworkMode && netBroadCastStart) {
        static int initialized;
        if (!initialized) {
            initialized = 1;
            netInit__Fv();
            netStartNetworkGameInit__Fv();
            simpleDrawLoop__FPFv_i(netStartNetworkGameFrame__Fv);
            netStart__Fv();
        }
    }
    if (!playerRecalled && !recallEnter) musicTimer = 60;
    engineSetDisplayDisable__Fi(1);
    if (!MC_SavingOrLoading) {
        if (MC_GetAsyncMode__Fv() != 7 || MC_FrontEndImport) Reset__6Cameraii(gameCam, 1, 0);
    }
    particlesPaused = 0;
    worldDrawEnable = 0;
    gameCreatureThinkSpinner = 0;
    _engineCheckStack__FPCci("core.cc", 1638);
    startNewWorld__Fv();
    enginePushBorderColor__FUi(255);
    _engineCheckStack__FPCci("core.cc", 1649);
    scePrintf("a\n");
    coreLoading = 10;
    coreLoadingStart = drawFrameNm;
    scePrintf("b\n");
    scriptRun__Fv();
    scePrintf("c\n");
    _engineCheckStack__FPCci("core.cc", 1700);
    if (netNetworkMode) netStartLevel__Fv();
    enginePushBorderColor__FUi(0xffff);
    while (coreLoading != 0) {
        enumerateAlivePlayers__Fv();
        if (gameCam != nullptr) Update__6Camera(gameCam);
        gameDrawWorld__Fv();
        if (cutMode) cutRunFrame__Fb(1);
        cdProcess__Fv();
        texProcessLoad__Fi(-1);
        NORRATH_TIMER0_CLEAR();
        texProcessDecompress__Fv();
        if (texIsLoading__Fv()) coreLoading = 10;
        else coreLoading = coreLoading - 1;
        for (int i = 0; i < nmPlayers; ++i) {
            unsigned char* record =
                *reinterpret_cast<unsigned char* const*>(player + kPlayerPointerOffset + i * kPlayerStride);
            if (record != nullptr && preCacheArmor__6Player(record) && coreLoading < 3) coreLoading = 3;
        }
        if (!(drawFrameNm - coreLoadingStart < 301)) coreLoading = 0;
    }
    enginePopBorderColor__Fv();
    enginePopBorderColor__Fv();
    enginePushBorderColor__FUi(0xff00);
    _engineCheckStack__FPCci("core.cc", 1741);
    scePrintf("d\n");
    if (netNetworkMode) {
        netGameStep__FbT0(1, 0);
        netClearContact__Fv();
        int seen = drawFrameNm;
        do {
            if (netInContact__Fv()) break;
            if (drawFrameNm != seen) {
                netGameStep__FbT0(1, 1);
                seen = drawFrameNm;
            }
        } while (netTrouble <= 600);
    }
    const char* hub = levelName + 81;
    if (strcmp("kelethin", hub) == 0) shopStart__Fiii(0, 1, 1);
    else if (strcmp("gnomehub", hub) == 0) shopStart__Fiii(0, 1, 2);
    else if (strcmp("beachhub", hub) == 0) shopStart__Fiii(0, 1, 3);
    else if (strcmp("elfhub", hub) == 0 || strcmp("elfhubn", hub) == 0) shopStart__Fiii(0, 1, 4);
    else if (strcmp("airhub", hub) == 0) shopStart__Fiii(0, 1, 5);
    loadingScreenRequestExit = 1;
    while (ReferThreadStatus(loadThreadID, 0) != 16) {
    }
    coreLoadingScreenRunning = 0;
    P_Clear__Fv();
    drawListClear__Fv();
    engineSetDisplayDisable__Fi(6);
    engineSetScreenColor__Fiiii(-128, -128, -128, 0);
    worldDrawEnable = 1;
    if (lumpQuery__FPc("env.lmp") != nullptr) {
        GameLoopLumpEntry* entry = lumpFindByType__FPvPci(lumpQuery__FPc("env.lmp"), "etex", 0);
        unsigned char* texture = entry->resource;
        textureInitTexture__FP8_texture(texture);
        engineLoadEnvTex__FP8_texture(texture);
    }
    _engineCheckStack__FPCci("core.cc", 1792);
    frameNm = 0;
    unsigned largest = sceSifQueryMaxFreeMemSize();
    unsigned total = sceSifQueryTotalFreeMemSize();
    scePrintf("preLevelStart %dk %dk\n", largest >> 10, total >> 10);
    enginePopBorderColor__Fv();
}
}

extern "C" void gameLoop__Fv() {
    static int decompCount;
    frameNm = 0;
    if (netNetworkMode) {
        eRandState = 0;
    } else {
        eRandState = NORRATH_COP0_COUNT();
        syncRandState = NORRATH_COP0_COUNT();
    }
    levelName[242] = 0;
    levelName[81] = 0;
    gameInit__Fv();
    engineAddTask__FPFv_viPci(worldRunWaterTask__Fv, 0, "runwater", 2);
    inventoryInit__Fv();
    supersampleMode = 1;
    engineSetDisplayDisable__Fi(1);
    gameBailFromGameLoop = 0;
    lightVU0StoppedProcessing__Fv();
    coreInterfaceBrightness[1] = 1.0f;
    coreInterfaceBrightness[0] = 1.0f;
    engineAddTask__FPFv_viPci(gameSetObjectVis__Fv, 0, "gameSetObjectVis", 0);
    engineAddTask__FPFv_viPci(plotRouteTask__Fv, 1000, "plotroute", 0);
    engineAddTask__FPFv_viPci(gameMiscTaskSync__Fv, 1, "gameMiscSync", 0);
    engineAddTask__FPFv_viPci(textMessageProcess__Fv, 123, "textMessageProcess", 0);
    engineAddTask__FPFv_viPci(textMessageDraw__Fv, 125, "textMessageDraw", 1);
    engineAddTask__FPFv_viPci(commProcess__Fv, 124, "commProcess", 0);
    if (superGamestateInit.name != nullptr) {
        scriptSetGameState__FPci(superGamestateInit.name, superGamestateInit.state);
        superGamestateInit.name = nullptr;
    }
    while (gameBailFromGameLoop == 0) {
        if (coreLevelCommentTimer > 0 && !cutMode && !gamePause && !interfaceOn && !DLG_Active__Fv()) {
            coreLevelCommentTimer = coreLevelCommentTimer - 1;
            if (coreLevelCommentTimer == 1 && nmPlayers > 0) {
                eRandState = eRandState * 1664525u + 1013904223u;
                playerPlaySound__Fii(static_cast<int>(eRandState >> 16) % nmPlayers, 2);
            }
        }
        if (levelName[0] != 0 && gamePlayersPresent != 0 && !engineTaskActive__FPc("frontend") &&
            nmPlayers > 0 && !MC_Busy__Fv()) {
            bool anyAbove = false;
            for (int i = 0; i < nmPlayers; ++i) {
                if (0.0f < *reinterpret_cast<const float*>(player + kPlayerFloatOffset + i * kPlayerStride))
                    anyAbove = true;
            }
            if (!anyAbove) levelName[0] = 0;
        }
        if (coreHeadsetResetTimer != 0) {
            coreHeadsetResetTimer = coreHeadsetResetTimer - 1;
            if (coreHeadsetResetTimer == 0) commEnable = 1;
        }
        if (levelName[0] != 0) startLevel();
        engineFrameStart__FUib((!interfaceOn && !coreLoading && !coreSwitchScene) ? coreClearColor : 0x80000000u, 0);
        lumpCheck__Fv();
        unsigned startTime = NORRATH_TIMER0_READ();
        int steps = engineGetAIStepCount__Fv();
        if (steps > 2) steps = 2;
        if (netNetworkMode) {
            netGameStep__FbT0(0, 0);
        } else {
            padProcess__Fv();
            coreGameStep__Fii(steps, 0);
        }
        engineLastAITime = static_cast<int>(NORRATH_TIMER0_READ() - startTime);
        engineRunTasks__Fii(0, 1);
        if (coreSwitchScene != 0) {
            if (!texIsLoading__Fv()) {
                coreSwitchScene = coreSwitchScene - 1;
                engineSetBlur__Fi(64);
            }
            if (!(drawFrameNm - coreSwitchSceneStart < 421)) {
                engineSetBlur__Fi(64);
                coreSwitchScene = 0;
            }
            drawListClear__Fv();
        } else if (!texIsDecompressing__Fv()) {
            decompCount = 0;
        } else {
            int previous = decompCount;
            decompCount = previous + 1;
            if (!(previous < 21) && engineFullFrameMode == 0) engineSetVideoMode__Fb(1);
        }
        runMusic__Fv();
        soundFrame__Fv();
        animFrame__Fv();
        cdProcess__Fv();
        texProcessLoad__Fi(-1);
        picProcess__4itemv();
        texProcessDecompress__Fv();
        if (drawFrameNm % 600 == 0) scePrintf("still running %d min\n", drawFrameNm / 3600);
        if (coreLoading != 0) {
            int next;
            if (texIsLoading__Fv()) {
                coreLoading = 10;
                next = 10;
            } else {
                next = coreLoading - 1;
            }
            if (!(drawFrameNm - coreLoadingStart < 421)) next = 0;
            coreLoading = next;
            if (!(drawFrameNm < 3)) {
                drawListClear__Fv();
                if (coreLoading == 0) {
                    engineSetScreenColor__Fiiii(-128, -128, -128, 0);
                    engineDisplayDisable = 0;
                }
            }
        }
        engineFrameEnd__Fb(0);
        for (int i = 0; i < 4; ++i) {
            float brightness = coreInterfaceBrightness[i];
            if (1.0f < brightness) coreInterfaceBrightness[i] = (brightness - 1.0f) * 0.8f + 1.0f;
            float fade = coreInterfaceFadeUpVal[i];
            if (fade < 1.0f) {
                fade = fade + 0.12f;
                coreInterfaceFadeUpVal[i] = fade;
                if (1.0f < fade) coreInterfaceFadeUpVal[i] = 1.0f;
            }
        }
    }
    engineDisplayDisable = 2;
    bossMeterCurrent = 0;
    bossMeterAlpha = 0;
    engineSetDisplayDisable__Fi(1);
    cheatOn = 0;
    scriptSetScript__FPv(nullptr);
    gameClear__Fi(1);
    soundResetStream__Fi(0);
    soundResetStream__Fi(1);
    soundResetStream__Fi(2);
    lumpClear__Fi(10);
    playerDeallocateArmorCaches__Fv();
    debugClearStartMenu__Fv();
    particlesPaused = 0;
    gamePause = 0;
    world = 0;
    if (gameBailFromGameLoop == 101) frontEndInit__Fi(1);
}
