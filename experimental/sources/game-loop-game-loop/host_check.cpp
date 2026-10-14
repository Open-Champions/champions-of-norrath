// Host contract for gameLoop. Not an EE plan input.
#define NORRATH_HOST_CONTRACT
#include "game_loop.cpp"

#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <string>

static std::string log_text;

template <class... Args>
static void note(const char* name, Args... args) {
    log_text += name;
    if (sizeof...(Args) != 0) {
        log_text += '(';
        bool first = true;
        ((log_text += (first ? "" : ","), log_text += std::to_string(static_cast<long>(args)), first = false), ...);
        log_text += ')';
    }
    log_text += ';';
}

static void note_text(const char* name, const char* text, long a = -1) {
    log_text += name;
    log_text += '(';
    log_text += text;
    if (a != -1) log_text += "," + std::to_string(a);
    log_text += ");";
}

extern "C" {
GameLoopStateInit superGamestateInit;
signed char netNetworkMode, netBroadCastStart;
int netTrouble;
unsigned eRandState, syncRandState;
int frameNm, drawFrameNm;
unsigned char supersampleMode;
int gameBailFromGameLoop;
float coreInterfaceBrightness[4], coreInterfaceFadeUpVal[4];
int coreLevelCommentTimer;
signed char cutMode, interfaceOn;
int gamePause, nmPlayers, gamePlayersPresent, coreHeadsetResetTimer, commEnable;
signed char MC_KickProtect, MC_FrontEndImport, MC_SavingOrLoading, playerRecalled, recallEnter;
int musicTimer;
unsigned char* gameCam;
signed char particlesPaused, worldDrawEnable, gameCreatureThinkSpinner;
int coreLoading, coreLoadingStart;
signed char loadingScreenRequestExit, coreLoadingScreenRunning;
int loadThreadID, engineFullFrameMode, coreSwitchScene, coreSwitchSceneStart;
unsigned coreClearColor;
int engineLastAITime, engineDisplayDisable, bossMeterCurrent, bossMeterAlpha;
signed char cheatOn;
int world;
char levelName[322];
alignas(8) unsigned char player[0x176c0];
}

static unsigned timer0 = 0;
static unsigned cop0 = 1000;
static int frames_left = 2;
static int exit_value = 7;
static int ai_steps = 5;
static int dlg_active = 0, task_active = 0, mc_busy = 0;
static int tex_loading = 0, tex_decompressing = 0, async_mode = 0;
static int precache_result = 0;
static int draw_step = 0;
static int poke_brightness = 0;
static int lowest_loading = 1000;
static const char* hub_name = nullptr;
static int contact_after = 2;
static int contact_calls = 0;
static int thread_status_calls = 0;
static int lump_present = 0;
static GameLoopLumpEntry lump_entry;
static unsigned char texture_bytes[4];

extern "C" unsigned norrath_host_timer0_read() {
    timer0 += 7;
    return timer0;
}
extern "C" void norrath_host_timer0_clear() {
    timer0 = 0;
    note("timerclear");
}
extern "C" unsigned norrath_host_cop0_count() { return cop0++; }

extern "C" {
void gameInit__Fv() {
    note("gameInit");
    if (hub_name != nullptr) std::strcpy(levelName + 81, hub_name);
}
void engineAddTask__FPFv_viPci(void (*)(), int priority, const char* name, int flags) {
    log_text += "task(" + std::string(name) + "," + std::to_string(priority) + "," + std::to_string(flags) + ");";
}
void worldRunWaterTask__Fv() {}
void gameSetObjectVis__Fv() {}
void plotRouteTask__Fv() {}
void gameMiscTaskSync__Fv() {}
void textMessageProcess__Fv() {}
void textMessageDraw__Fv() {}
void commProcess__Fv() {}
void inventoryInit__Fv() { note("inventoryInit"); }
void engineSetDisplayDisable__Fi(int value) { note("displayDisable", value); }
void lightVU0StoppedProcessing__Fv() { note("lightVU0"); }
void scriptSetGameState__FPci(const char* name, int state) {
    note_text("gameState", name, state);
}
int DLG_Active__Fv() { return dlg_active; }
void playerPlaySound__Fii(int who, int sound) { note("playerSound", who, sound); }
int engineTaskActive__FPc(const char* name) {
    note_text("taskActive", name);
    return task_active;
}
int MC_Busy__Fv() { note("mcBusy"); return mc_busy; }
int subDrawLoop__FPFv_i(int (*callback)()) {
    note("subDrawLoop", callback == mcProcessKick__Fv);
    return 0;
}
int mcProcessKick__Fv() { return 0; }
void netWaitForPeersToCatchUp__Fv() { note("netWait"); }
void netInit__Fv() { note("netInit"); }
void netStartNetworkGameInit__Fv() { note("netStartInit"); }
int netStartNetworkGameFrame__Fv() { return 0; }
void simpleDrawLoop__FPFv_i(int (*callback)()) {
    note("simpleDrawLoop", callback == netStartNetworkGameFrame__Fv);
}
void netStart__Fv() { note("netStart"); }
int MC_GetAsyncMode__Fv() { note("asyncMode"); return async_mode; }
void Reset__6Cameraii(unsigned char* camera, int a, int b) { note("cameraReset", camera == gameCam, a, b); }
void _engineCheckStack__FPCci(const char* file, int line) {
    note("stack", line);
    if (std::strcmp(file, "core.cc") != 0) note("badfile");
}
void startNewWorld__Fv() { note("startNewWorld"); }
void enginePushBorderColor__FUi(unsigned color) { note("push", color); }
void enginePopBorderColor__Fv() { note("pop"); }
int scePrintf(const char* format, ...) {
    va_list args;
    va_start(args, format);
    if (std::strcmp(format, "to level %s\n") == 0) {
        const char* name = va_arg(args, const char*);
        note_text("print-level", name);
    } else if (std::strcmp(format, "still running %d min\n") == 0) {
        note("print-minutes", va_arg(args, int));
    } else if (std::strcmp(format, "preLevelStart %dk %dk\n") == 0) {
        unsigned first = va_arg(args, unsigned);
        unsigned second = va_arg(args, unsigned);
        note("print-pre", first, second);
    } else if (std::strlen(format) == 2) {
        char tag[16];
        std::snprintf(tag, sizeof tag, "print-%c", format[0]);
        note(tag);
    } else {
        note("print-other");
    }
    va_end(args);
    return 0;
}
void scriptRun__Fv() { note("scriptRun"); }
void netStartLevel__Fv() { note("netStartLevel"); }
void enumerateAlivePlayers__Fv() {
    note("enumerate");
    if (coreLoading < lowest_loading) lowest_loading = coreLoading;
}
void Update__6Camera(unsigned char*) { note("cameraUpdate"); }
void gameDrawWorld__Fv() {
    note("drawWorld");
    drawFrameNm += draw_step;
}
void cutRunFrame__Fb(int value) { note("cutFrame", value); }
void cdProcess__Fv() { note("cd"); }
void texProcessLoad__Fi(int value) { note("texLoad", value); }
void texProcessDecompress__Fv() { note("texDecompress"); }
int texIsLoading__Fv() { return tex_loading; }
int preCacheArmor__6Player(unsigned char* p) { note("preCache", p != nullptr); return precache_result; }
void netGameStep__FbT0(int a, int b) { note("netStep", a, b); }
void netClearContact__Fv() { note("netClear"); }
int netInContact__Fv() {
    ++contact_calls;
    if (contact_calls > contact_after) return 1;
    drawFrameNm += 1;
    return 0;
}
void shopStart__Fiii(int a, int b, int c) { note("shop", a, b, c); }
int ReferThreadStatus(int thread, void* status) {
    note("thread", thread, status != nullptr);
    return ++thread_status_calls < 3 ? 1 : 16;
}
void P_Clear__Fv() { note("pclear"); }
void drawListClear__Fv() { note("drawListClear"); }
void engineSetScreenColor__Fiiii(int r, int g, int b, int a) { note("screen", r, g, b, a); }
GameLoopLump* lumpQuery__FPc(const char*) {
    note("lumpQuery");
    return lump_present ? reinterpret_cast<GameLoopLump*>(&lump_entry) : nullptr;
}
GameLoopLumpEntry* lumpFindByType__FPvPci(GameLoopLump* lump, const char* type, int index) {
    note("lumpFind", lump == reinterpret_cast<GameLoopLump*>(&lump_entry), std::strcmp(type, "etex"), index);
    return &lump_entry;
}
void textureInitTexture__FP8_texture(unsigned char* t) { note("texInit", t == texture_bytes); }
void engineLoadEnvTex__FP8_texture(unsigned char* t) { note("envTex", t == texture_bytes); }
unsigned sceSifQueryMaxFreeMemSize() { return 5u << 10; }
unsigned sceSifQueryTotalFreeMemSize() { return 9u << 10; }
void engineFrameStart__FUib(unsigned color, int second) {
    note("frameStart", color, second);
    if (poke_brightness) coreInterfaceBrightness[0] = 1.5f;
    poke_brightness = 0;
}
void lumpCheck__Fv() { note("lumpCheck"); }
int engineGetAIStepCount__Fv() { return ai_steps; }
void padProcess__Fv() { note("pad"); }
void coreGameStep__Fii(int steps, int second) { note("coreStep", steps, second); }
void engineRunTasks__Fii(int a, int b) { note("runTasks", a, b); }
void engineSetBlur__Fi(int value) { note("blur", value); }
int texIsDecompressing__Fv() { return tex_decompressing; }
void engineSetVideoMode__Fb(int value) { note("videoMode", value); }
void runMusic__Fv() { note("music"); }
void soundFrame__Fv() { note("soundFrame"); }
void animFrame__Fv() { note("anim"); }
void picProcess__4itemv() { note("pic"); }
void engineFrameEnd__Fb(int value) {
    note("frameEnd", value);
    drawFrameNm += 1;
    if (--frames_left == 0) gameBailFromGameLoop = exit_value;
}
void scriptSetScript__FPv(void* s) { note("setScript", s != nullptr); }
void gameClear__Fi(int v) { note("gameClear", v); }
void soundResetStream__Fi(int v) { note("resetStream", v); }
void lumpClear__Fi(int v) { note("lumpClear", v); }
void playerDeallocateArmorCaches__Fv() { note("deallocArmor"); }
void debugClearStartMenu__Fv() { note("debugMenu"); }
void frontEndInit__Fi(int v) { note("frontEndInit", v); }
}

static int fail(int code, const char* what) {
    std::fprintf(stderr, "check %d failed: %s\nlog: %s\n", code, what, log_text.c_str());
    return code;
}

static void reset_world() {
    log_text.clear();
    std::memset(levelName, 0, sizeof levelName);
    std::memset(player, 0, sizeof player);
    netNetworkMode = 0;
    netBroadCastStart = 0;
    netTrouble = 0;
    eRandState = syncRandState = 0;
    frameNm = 77;
    drawFrameNm = 1000;
    supersampleMode = 0;
    gameBailFromGameLoop = 55;
    for (int i = 0; i < 4; ++i) {
        coreInterfaceBrightness[i] = 1.0f;
        coreInterfaceFadeUpVal[i] = 1.0f;
    }
    coreLevelCommentTimer = 0;
    cutMode = interfaceOn = 0;
    gamePause = 0;
    nmPlayers = 0;
    gamePlayersPresent = 0;
    coreHeadsetResetTimer = 0;
    commEnable = 0;
    MC_KickProtect = MC_FrontEndImport = MC_SavingOrLoading = playerRecalled = recallEnter = 0;
    musicTimer = 0;
    gameCam = nullptr;
    particlesPaused = 1;
    worldDrawEnable = 1;
    gameCreatureThinkSpinner = 1;
    coreLoading = 0;
    coreLoadingStart = 0;
    loadingScreenRequestExit = 0;
    coreLoadingScreenRunning = 1;
    loadThreadID = 33;
    engineFullFrameMode = 0;
    coreSwitchScene = 0;
    coreSwitchSceneStart = 0;
    coreClearColor = 0x12345678u;
    engineLastAITime = 0;
    engineDisplayDisable = 99;
    bossMeterCurrent = bossMeterAlpha = 5;
    cheatOn = 1;
    world = 5;
    superGamestateInit.name = nullptr;
    superGamestateInit.state = 0;
    timer0 = 0;
    cop0 = 1000;
    frames_left = 2;
    exit_value = 7;
    ai_steps = 5;
    dlg_active = task_active = mc_busy = 0;
    tex_loading = tex_decompressing = async_mode = 0;
    precache_result = 0;
    draw_step = 0;
    contact_after = 2;
    contact_calls = 0;
    thread_status_calls = 0;
    lump_present = 0;
    lump_entry.unknown = 0;
    lump_entry.resource = texture_bytes;
    cop0 = 1000;
}

static bool has(const char* text) { return log_text.find(text) != std::string::npos; }

static std::string frame_tail(int suppressDraw = 0) {
    (void)suppressDraw;
    return "music;soundFrame;anim;cd;texLoad(-1);pic;texDecompress;";
}

static std::string plain_frame(unsigned color, int steps) {
    return "frameStart(" + std::to_string(color) + ",0);lumpCheck;pad;coreStep(" + std::to_string(steps) +
           ",0);runTasks(0,1);" + frame_tail();
}

static const char* const kSetup =
    "gameInit;task(runwater,0,2);inventoryInit;displayDisable(1);lightVU0;"
    "task(gameSetObjectVis,0,0);task(plotroute,1000,0);task(gameMiscSync,1,0);"
    "task(textMessageProcess,123,0);task(textMessageDraw,125,1);task(commProcess,124,0);";
static const char* const kTail =
    "displayDisable(1);setScript(0);gameClear(1);resetStream(0);resetStream(1);resetStream(2);"
    "lumpClear(10);deallocArmor;debugMenu;";

static std::string loading_loop(int iterations) {
    std::string text;
    for (int i = 0; i < iterations; ++i)
        text += "enumerate;drawWorld;cd;texLoad(-1);timerclear;texDecompress;";
    return text;
}

int main() {
    // 1. No level. Two frames, then exit value 7.
    reset_world();
    coreLevelCommentTimer = 2;
    nmPlayers = 3;
    coreHeadsetResetTimer = 1;
    poke_brightness = 1;
    coreInterfaceFadeUpVal[2] = 0.5f;
    coreInterfaceFadeUpVal[3] = 0.95f;
    superGamestateInit.name = "boot";
    superGamestateInit.state = 9;
    gameLoop__Fv();
    {
        unsigned stepped = 1000u * 1664525u + 1013904223u;
        std::string expected = std::string(kSetup) + "gameState(boot,9);playerSound(" +
                               std::to_string(static_cast<int>(stepped >> 16) % 3) + ",2);" +
                               plain_frame(0x12345678u, 2) + "frameEnd(0);" + plain_frame(0x12345678u, 2) +
                               "frameEnd(0);" + kTail;
        if (log_text != expected) return fail(1, "no-level trace");
        if (eRandState != stepped || syncRandState != 1001u) return fail(2, "rand state");
    }
    if (frameNm != 0 || supersampleMode != 1) return fail(3, "setup state");
    if (superGamestateInit.name != nullptr) return fail(4, "game state cleared");
    if (commEnable != 1 || coreHeadsetResetTimer != 0) return fail(5, "headset timer");
    if (coreLevelCommentTimer != 0) return fail(6, "comment timer");
    if (coreInterfaceBrightness[0] < 1.31f || coreInterfaceBrightness[0] > 1.33f) return fail(7, "decay");
    if (coreInterfaceBrightness[1] != 1.0f) return fail(8, "brightness idle");
    if (coreInterfaceFadeUpVal[2] < 0.73f || coreInterfaceFadeUpVal[2] > 0.75f) return fail(9, "fade up");
    if (coreInterfaceFadeUpVal[3] != 1.0f) return fail(10, "fade clamp");
    if (engineDisplayDisable != 2 || bossMeterCurrent != 0 || bossMeterAlpha != 0 || cheatOn != 0 || world != 0)
        return fail(11, "tail state");
    if (particlesPaused != 0 || gamePause != 0) return fail(12, "tail flags");
    if (engineLastAITime != 7) return fail(13, "ai time");

    // 2. Comment timer holds while paused, cut, interface on or dialog open.
    for (int blocked = 0; blocked < 4; ++blocked) {
        reset_world();
        coreLevelCommentTimer = 2;
        nmPlayers = 1;
        frames_left = 1;
        if (blocked == 0) cutMode = 1;
        if (blocked == 1) gamePause = 1;
        if (blocked == 2) interfaceOn = 1;
        if (blocked == 3) dlg_active = 1;
        gameLoop__Fv();
        // gamePause is cleared by the tail of gameLoop.
        if (coreLevelCommentTimer != 2) return fail(14, "comment timer must hold");
        if (has("playerSound")) return fail(15, "no sound while blocked");
    }

    // 3. Level start, offline, hub name at levelName+81, camera kept.
    reset_world();
    std::strcpy(levelName, "x");
    hub_name = "gnomehub";
    gamePlayersPresent = 1;
    nmPlayers = 2;
    {
        float alive = 10.0f;
        std::memcpy(player + 23984 + 172, &alive, 4);
    }
    async_mode = 7;
    lump_present = 1;
    frames_left = 1;
    gameCam = texture_bytes;
    gameLoop__Fv();
    {
        std::string expected = std::string(kSetup) +
            "taskActive(frontend);mcBusy;"
            "print-level(x);displayDisable(1);asyncMode;stack(1638);startNewWorld;push(255);stack(1649);print-a;"
            "print-b;scriptRun;print-c;stack(1700);push(65535);" +
            loading_loop(10) +
            "pop;pop;push(65280);stack(1741);print-d;shop(0,1,2);thread(33,0);thread(33,0);thread(33,0);"
            "pclear;drawListClear;displayDisable(6);screen(-128,-128,-128,0);"
            "lumpQuery;lumpQuery;lumpFind(1,0,0);texInit(1);envTex(1);stack(1792);print-pre(5,9);pop;" +
            plain_frame(0x12345678u, 2) + "frameEnd(0);" + kTail;
        // gameCam is non-null so Camera::Update runs in every loading pass.
        std::string loop;
        for (int i = 0; i < 10; ++i) loop += "enumerate;cameraUpdate;drawWorld;cd;texLoad(-1);timerclear;texDecompress;";
        size_t from = expected.find(loading_loop(10));
        expected.replace(from, loading_loop(10).size(), loop);
        if (log_text != expected) return fail(16, "offline level trace");
    }
    hub_name = nullptr;
    if (levelName[0] != 'x') return fail(17, "level name kept");
    if (frameNm != 0 || coreLoadingScreenRunning != 0 || loadingScreenRequestExit != 1) return fail(18, "level state");
    if (worldDrawEnable != 1 || particlesPaused != 0 || gameCreatureThinkSpinner != 0) return fail(19, "level flags");
    if (musicTimer != 60) return fail(20, "music timer");
    if (coreLoading != 0) return fail(21, "loading ended");

    // 4. Camera reset when the async mode is not 7, and when the front end import is set.
    for (int variant = 0; variant < 4; ++variant) {
        reset_world();
        std::strcpy(levelName, "x");
        gameCam = texture_bytes;
        frames_left = 1;
        async_mode = (variant & 1) ? 3 : 7;
        MC_FrontEndImport = (variant & 2) ? 1 : 0;
        gameLoop__Fv();
        bool reset = has("cameraReset(1,1,0)");
        bool expectReset = async_mode != 7 || MC_FrontEndImport;
        if (reset != expectReset) return fail(22, "camera reset rule");
    }
    reset_world();
    std::strcpy(levelName, "x");
    gameCam = texture_bytes;
    MC_SavingOrLoading = 1;
    frames_left = 1;
    async_mode = 3;
    gameLoop__Fv();
    if (has("asyncMode") || has("cameraReset")) return fail(23, "saving skips camera reset");

    // 5. Hub names and the elf pair.
    {
        const char* const names[] = {"kelethin", "gnomehub", "beachhub", "elfhub", "elfhubn", "airhub", "other"};
        const int hubs[] = {1, 2, 3, 4, 4, 5, 0};
        for (int i = 0; i < 7; ++i) {
            reset_world();
            std::strcpy(levelName, "x");
            frames_left = 1;
            // levelName+81 is cleared by gameLoop before the loop, so a scenario
            // sets it from the game's own hook.
            hub_name = names[i];
            gameLoop__Fv();
            char expected[32];
            std::snprintf(expected, sizeof expected, "shop(0,1,%d)", hubs[i]);
            if (hubs[i] == 0 ? has("shop(") : !has(expected)) return fail(24, "hub name");
        }
        hub_name = nullptr;
    }

    // 6. Player death check.
    reset_world();
    std::strcpy(levelName, "x");
    gamePlayersPresent = 1;
    nmPlayers = 2;
    frames_left = 1;
    gameLoop__Fv();
    if (has("print-level") || levelName[0] != 0 || !has("taskActive(frontend);mcBusy;")) return fail(25, "all players down");
    reset_world();
    std::strcpy(levelName, "x");
    gamePlayersPresent = 1;
    nmPlayers = 2;
    task_active = 1;
    frames_left = 1;
    gameLoop__Fv();
    if (!has("print-level") || has("mcBusy")) return fail(26, "frontend task keeps level");
    reset_world();
    std::strcpy(levelName, "x");
    gamePlayersPresent = 1;
    nmPlayers = 2;
    mc_busy = 1;
    frames_left = 1;
    gameLoop__Fv();
    if (!has("print-level") || !has("mcBusy;")) return fail(27, "busy memory card keeps level");
    reset_world();
    std::strcpy(levelName, "x");
    gamePlayersPresent = 0;
    nmPlayers = 2;
    frames_left = 1;
    gameLoop__Fv();
    if (!has("print-level") || has("taskActive")) return fail(28, "no players present keeps level");

    // 7. Armor caching holds the loading counter at 3 until the 301 frame cap.
    reset_world();
    std::strcpy(levelName, "x");
    nmPlayers = 2;
    {
        unsigned char* record = texture_bytes;
        std::memcpy(player + 23984 + 21872, &record, sizeof record);
    }
    precache_result = 1;
    lowest_loading = 1000;
    draw_step = 30;
    frames_left = 1;
    gameLoop__Fv();
    {
        int count = 0;
        for (size_t at = log_text.find("preCache(1)"); at != std::string::npos; at = log_text.find("preCache(1)", at + 1))
            ++count;
        if (count != 11) return fail(29, "precache bump");
        if (has("preCache(0)")) return fail(30, "records without a player are skipped");
        if (lowest_loading != 3) return fail(53, "counter held at 3");
    }

    // 8. Network level start and the contact wait; netInit runs only once.
    reset_world();
    std::strcpy(levelName, "x");
    netNetworkMode = 1;
    netBroadCastStart = 1;
    MC_KickProtect = 1;
    MC_SavingOrLoading = 1;
    recallEnter = 1;
    frames_left = 1;
    lump_present = 0;
    gameLoop__Fv();
    if (eRandState != 0) return fail(31, "net rand state");
    if (!has("subDrawLoop(1);netWait;print-level(x);netInit;netStartInit;simpleDrawLoop(1);netStart;displayDisable(1);"))
        return fail(32, "net start");
    if (musicTimer != 0) return fail(33, "recall keeps music timer");
    if (!has("netStartLevel;push(65535);") || !has("netStep(1,0);netClear;netStep(1,1);netStep(1,1);thread(33,0)"))
        return fail(34, "net contact wait");
    if (has("pad;") || !has("netStep(0,0);runTasks(0,1)")) return fail(35, "net frame");
    reset_world();
    std::strcpy(levelName, "x");
    netNetworkMode = 1;
    netBroadCastStart = 1;
    frames_left = 1;
    gameLoop__Fv();
    if (has("netInit") || has("simpleDrawLoop")) return fail(36, "net init once");
    reset_world();
    std::strcpy(levelName, "x");
    netNetworkMode = 1;
    netBroadCastStart = 0;
    frames_left = 1;
    gameLoop__Fv();
    if (has("netInit") || !has("netWait;print-level(x);")) return fail(37, "no broadcast");
    // The contact wait gives up when netTrouble passes 600.
    reset_world();
    std::strcpy(levelName, "x");
    netNetworkMode = 1;
    frames_left = 1;
    contact_after = 1000;
    netTrouble = 601;
    gameLoop__Fv();
    if (contact_calls != 1) return fail(38, "trouble limit");

    // 9. Frame bookkeeping.
    reset_world();
    coreSwitchScene = 2;
    coreSwitchSceneStart = 100;
    drawFrameNm = 1000;
    frames_left = 1;
    ai_steps = 1;
    gameLoop__Fv();
    if (!has("frameStart(2147483648,0);lumpCheck;pad;coreStep(1,0);runTasks(0,1);blur(64);blur(64);drawListClear;music;"))
        return fail(39, "scene switch timeout");
    if (coreSwitchScene != 0) return fail(40, "scene switch ends");
    reset_world();
    coreSwitchScene = 2;
    coreSwitchSceneStart = 900;
    drawFrameNm = 1000 - 500 + 1;
    tex_loading = 0;
    frames_left = 1;
    gameLoop__Fv();
    if (!has("blur(64);drawListClear;music;") || has("blur(64);blur(64)") || coreSwitchScene != 1)
        return fail(41, "scene switch step");
    reset_world();
    coreSwitchScene = 2;
    coreSwitchSceneStart = 900;
    drawFrameNm = 1100;
    tex_loading = 1;
    frames_left = 1;
    gameLoop__Fv();
    if (has("blur") || coreSwitchScene != 2 || !has("drawListClear")) return fail(42, "scene switch waits for textures");
    // Decompress: the video mode is forced after 22 frames.
    reset_world();
    tex_decompressing = 1;
    frames_left = 23;
    gameLoop__Fv();
    {
        int count = 0;
        for (size_t at = log_text.find("videoMode(1)"); at != std::string::npos; at = log_text.find("videoMode(1)", at + 1))
            ++count;
        if (count != 2) return fail(43, "video mode after 22 frames");
    }
    reset_world();
    tex_decompressing = 1;
    engineFullFrameMode = 1;
    frames_left = 23;
    gameLoop__Fv();
    if (has("videoMode")) return fail(44, "full frame mode blocks video mode");
    // Minute message.
    reset_world();
    drawFrameNm = 7200;
    frames_left = 1;
    gameLoop__Fv();
    if (!has("texDecompress;print-minutes(2);frameEnd(0)")) return fail(45, "minute message");
    reset_world();
    drawFrameNm = 7201;
    frames_left = 1;
    gameLoop__Fv();
    if (has("print-minutes")) return fail(46, "minute message only on 600");
    // Loading counter in the frame.
    reset_world();
    coreLoading = 1;
    coreLoadingStart = 900;
    drawFrameNm = 1000;
    engineDisplayDisable = 99;
    frames_left = 1;
    gameLoop__Fv();
    if (coreLoading != 0 || !has("frameStart(2147483648,0)") ||
        !has("drawListClear;screen(-128,-128,-128,0);frameEnd(0)"))
        return fail(47, "loading ends");
    reset_world();
    coreLoading = 5;
    coreLoadingStart = 900;
    frames_left = 1;
    gameLoop__Fv();
    if (has("screen(") || !has("drawListClear;frameEnd(0)")) return fail(48, "loading continues");
    reset_world();
    coreLoading = 5;
    coreLoadingStart = 900;
    tex_loading = 1;
    frames_left = 1;
    gameLoop__Fv();
    if (has("screen(") || !has("drawListClear;frameEnd(0)")) return fail(49, "texture loading re-arms");
    reset_world();
    coreLoading = 5;
    coreLoadingStart = 500;
    drawFrameNm = 1000;
    frames_left = 1;
    gameLoop__Fv();
    if (!has("screen(-128,-128,-128,0);frameEnd(0)")) return fail(50, "loading frame cap");
    reset_world();
    coreLoading = 5;
    drawFrameNm = 2;
    coreLoadingStart = 0;
    frames_left = 1;
    gameLoop__Fv();
    if (has("drawListClear") || has("screen(")) return fail(51, "early frames skip draw list clear");

    // AI steps above two are clamped to two.
    for (int steps = 1; steps <= 4; ++steps) {
        reset_world();
        ai_steps = steps;
        frames_left = 1;
        gameLoop__Fv();
        char expected[32];
        std::snprintf(expected, sizeof expected, "coreStep(%d,0)", steps > 2 ? 2 : steps);
        if (!has(expected)) return fail(54, "ai step clamp");
    }

    // 10. Exit value 101 starts the front end.
    reset_world();
    exit_value = 101;
    frames_left = 1;
    gameLoop__Fv();
    if (!has("debugMenu;frontEndInit(1);")) return fail(52, "front end on 101");
    return 0;
}
