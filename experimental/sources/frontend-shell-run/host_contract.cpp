#define NORRATH_HOST_CONTRACT
#include <cstdint>
#include <cstdio>
#include <cstring>

// The source under test declares the libc names with its own signatures.
// Keep host copies before renaming them so the declarations do not collide.
static int h_strcmp(const char* a, const char* b) { return std::strcmp(a, b); }
static void* h_memset(void* d, int v, unsigned long n) { return std::memset(d, v, n); }
static int h_memcmp(const void* a, const void* b, unsigned long n) { return std::memcmp(a, b, n); }
#define strcmp norrath_host_strcmp
#define memset norrath_host_memset
#define sprintf norrath_host_sprintf

static int error;
static int calls;
static char seen[48][48];
static int music_mode[4];
static char music_name[4][40];
static int music_count;
static int isin_angle;
static int title_x;
static int title_y;
static int browser_a;
static int browser_b;
static int attempt_during_browser;
static int line_shift_during_clear;
static int cleanup_arg;
static int cleanup_saw_clear_flag;
static int setpos_count;
static unsigned char* setpos_target[4];
static unsigned font_colors[4];
static int font_color_count;
static int param_calls;
static int player_inits;
static char player_name[16];
static int rect_w;
static int rect_h;
static unsigned rect_color;

static void fail(int code) {
    if (error == 0) error = code;
}

static void note(const char* name) {
    if (calls < 48) std::snprintf(seen[calls], sizeof seen[calls], "%s", name);
    ++calls;
}

static int saw(const char* name) {
    for (int index = 0; index < calls; ++index) {
        if (h_strcmp(seen[index], name) == 0) return 1;
    }
    return 0;
}

extern "C" {
struct AnimationHeader;
struct GameObject;
int musicPlaying;
int musicStart;
unsigned char gameHidePlayers;
unsigned char frontEndStartInCredits;
unsigned char MC_Debug;
void* smallFont;
int drawFrameNm;
int frontendControllerConfigRunning;
int menuNm;
unsigned char netAttemptLaunch;
unsigned char menuModeRebootQuestion;
unsigned char* menu;
unsigned char netNetworkMode;
unsigned char coreClearNetLogFuck;
unsigned char netGameLaunchInfo[328];
int nmPlayers;
int gamePause;
unsigned char levelName[322];
int padAcceptButtonFontChar;
int padBackButtonFontChar;
unsigned short hudFooterUni[128];
unsigned char pad[352];
unsigned padAcceptButton = 1u;
unsigned padBackButton = 2u;
unsigned char* statusPtr;
unsigned char coreLaunchSRS;
int gameBailFromGameLoop;
void* mediumFont;
float soundSfxVolume;
float soundMusicVolume;
int frontend_end_timer_1104 __asm__("endTimer.1104");
int endInit;
int endTimer;
int creditsPosY;
int lineShift;
int curFocus;
unsigned char creditsText[64];
int frameNm;
int gameDifficulty;
int isNetGame;
unsigned char netBroadCastStart;
unsigned MC_GameConfig;
unsigned char playerSelectEnforce;
unsigned char playerSelectCount;
unsigned char playerSelectStartPlayer;
unsigned char playerSelectNextName[64];
unsigned char playerSelectPrevName[64];
unsigned short* playerSelectExitMessage;
unsigned char norrath_frontend_byte_484ba8;
unsigned char attemptLoadGame;
float expWarnAlpha;
unsigned char coreBeenInSRS;
unsigned char startMenuState;
unsigned char MC_LastModeSuccessful;
unsigned char playerSelectSuccessful;
unsigned char frontMenu[80];
unsigned char creditMenu[16];
unsigned char gameTypeMenu[16];
unsigned char playerMenu[80];
unsigned char menuNetClientServer[16];
unsigned char menuNetImportOrNewPlayer[16];
unsigned char menuNetImportOrNewGame[16];
unsigned char optionMenu[16];
unsigned char difficultyMenu[16];
unsigned char preConfigure[80];
unsigned char commMenu[16];
unsigned char texture_bytes[8];
unsigned char menu_records[80];
int busy_value;
int launch_value;
int save_value;
int yes_answer;
int param_value;
int menu_run_value;
int menu_run_out;
int stream_status;

void soundResetStream__Fi(int) { note("reset"); }
void soundStartGlobalMusic__FPci(char* name, int mode) {
    note("music");
    if (music_count < 4) {
        music_mode[music_count] = mode;
        std::snprintf(music_name[music_count], sizeof music_name[music_count], "%s", name);
        ++music_count;
    }
}
int soundGetStreamAmountPlayed__Fi(int) {
    note("amount");
    if (musicPlaying != 1) fail(50);
    return 77;
}
void hudRunNoControllers__Fi(int value) {
    note("nocontroller");
    if (value != 1) fail(51);
}
int sprintf(char* dest, const char* format, ...) {
    note("sprintf");
    if (h_strcmp(format, "Build: %s,  %s\n") != 0) fail(52);
    dest[0] = 0;
    return 0;
}
void fontStart__FP4FontiUii(void*, int, unsigned, int) { note("fontStart"); }
void fontPrintCentered__FiiPci(int, int, char*, int) { note("print"); }
void fontEnd__Fv() { note("fontEnd"); }
int isin__Fi(int angle) {
    note("isin");
    isin_angle = angle;
    return 0;
}
void frontendRunControllerConfig__Fv() { note("runConfig"); }
int MC_Busy__Fv() {
    note("busy");
    return busy_value;
}
void* lumpFindResource__FPcT0(char*, char*) {
    note("lump");
    texture_bytes[0] = 100;
    texture_bytes[1] = 0;
    return texture_bytes;
}
void textureDrawColor__FP8_textureiiiUi(void*, int x, int y, int mode, unsigned color) {
    note("title");
    title_x = x;
    title_y = y;
    if (mode != 5 || color != 0x80808080u) fail(53);
}
unsigned short* textGetString__FPc(char* key) {
    note(key);
    return reinterpret_cast<unsigned short*>(key);
}
int hudRunYesNo__FPUsiiii(unsigned short*, int, int, int, int d) {
    note("yesno");
    if (d != 120) fail(54);
    return yes_answer;
}
void engineSetDisplayDisable__Fi(int) { note("display"); }
void netInit__Fv() { note("netInit"); }
void netFree__Fb(int down) {
    note(down ? "netFree1" : "netFree0");
}
void engineReboot__Fv() { note("reboot"); }
int netDoNewLaunch__Fv() {
    note("newLaunch");
    return launch_value;
}
void frontEndCleanUp__Fi(int value) {
    note("cleanup");
    cleanup_arg = value;
    cleanup_saw_clear_flag = coreClearNetLogFuck;
}
void gameDeathStart__Fi(int) { note("death"); }
void gameResetGameState__Fv() { note("reset"); }
void netStart__Fv() { note("netStart"); }
int gameMaybeSaveConfig__Fii(int, int second) {
    note(second ? "save1" : "save0");
    return save_value;
}
void usprintf__FPUsT0ffffffff(unsigned short*, unsigned short*, float, float, float, float, float, float, float,
                              float) {
    note("usprintf");
}
void hudShowMsg__FPUsN20iii(unsigned short*, unsigned short*, unsigned short*, int, int, int) { note("show"); }
void gamePopConfig__Fv() { note("pop"); }
void startNetLaunch__Fv() { note("startNet"); }
void setSRSInfo__Fv() { note("srs"); }
void fontPrintCenteredu__FiiPUsi(int, int, unsigned short*, int) { note("printu"); }
void menuDraw__FiiP4MenuUiUiP4FontP8_texturei(int, int, unsigned char*, unsigned, unsigned, void*, void*, int) {
    note("menuDraw");
}
int menuRun__FP4MenuP3padPi(unsigned char*, unsigned char*, int* out_value) {
    note("menuRun");
    *out_value = menu_run_out;
    return menu_run_value;
}
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(AnimationHeader*, GameObject*, float) { note("sequence"); }
int soundGetStreamPlayStatus__Fi(int) { return stream_status; }
void soundUpdateMusicVolume__Fv() { note("vol"); }
void soundUpdateAdjustments__Fv() { note("adjust"); }
void soundFadeStream__Fi(int) { note("fade"); }
void fontColor__FUiii(unsigned color, int, int) {
    if (font_color_count < 4) font_colors[font_color_count++] = color;
}
int strcmp(const char* left, const char* right) { return h_strcmp(left, right); }
void stopCredits__Fv() { note("stopCredits"); }
void rectangleDraw__FiiiiiUi(int, int, int width, int height, int, unsigned color) {
    note("rect");
    rect_w = width;
    rect_h = height;
    rect_color = color;
}
int paramRunning__Fv() {
    note("param");
    ++param_calls;
    return param_value;
}
void textureSmoothBorder__FiiiiiUiiii(int, int, int, int, int, unsigned, int, int, int, int) { note("border"); }
int fontStringSizeu__FP4FontPUsi(void*, unsigned short*, int) { return 20; }
void soundClearStreamQue__Fi(int) {
    note("clearQue");
    line_shift_during_clear = lineShift;
}
void gamePushConfig__Fv() { note("push"); }
void playerInit__FiiPcScScScScT2(int, int, char* name, signed char, signed char, signed char, signed char, char*) {
    note("player");
    ++player_inits;
    std::snprintf(player_name, sizeof player_name, "%s", name);
}
void memset(void* dest, int value, unsigned size) {
    note("memset");
    if (size != 328u || value != 0) fail(55);
    h_memset(dest, value, size);
}
void launchPlayerSelect__Fv() { note("launchSelect"); }
void MC_ChangePlayers__FiPUsiii(int, unsigned short*, int, int, int) { note("changePlayers"); }
void MC_LoadGameBrowser__Fii(int first, int second) {
    note("browser");
    browser_a = first;
    browser_b = second;
    attempt_during_browser = attemptLoadGame;
}
void menuSetPos__FP4Menui(unsigned char* records, int position) {
    note("setpos");
    if (setpos_count < 4) setpos_target[setpos_count] = records;
    ++setpos_count;
    if (position != 0) fail(56);
}
void frontendInitControllerConfig__Fi(int) { note("initConfig"); }
void engineSyncVBlank__Fv() { note("vblank"); }
}

static void store_ptr(unsigned char* slot, const void* pointer) {
    h_memset(slot, 0, 16);
    std::memcpy(slot, &pointer, sizeof pointer);
}

static void reset() {
    calls = 0;
    music_count = 0;
    isin_angle = -1;
    title_x = -1;
    font_color_count = 0;
    param_calls = 0;
    player_inits = 0;
    setpos_count = 0;
    busy_value = 0;
    launch_value = 0;
    save_value = 0;
    yes_answer = 0;
    param_value = 0;
    menu_run_value = 0;
    menu_run_out = 0;
    stream_status = 0;
    musicPlaying = 1;
    musicStart = 0;
    gameHidePlayers = 0;
    frontEndStartInCredits = 0;
    MC_Debug = 0;
    drawFrameNm = 3;
    frontendControllerConfigRunning = 0;
    menuNm = 0;
    netAttemptLaunch = 0;
    menuModeRebootQuestion = 0;
    netNetworkMode = 9;
    coreClearNetLogFuck = 0;
    h_memset(netGameLaunchInfo, 0, sizeof netGameLaunchInfo);
    nmPlayers = 1;
    gamePause = 7;
    h_memset(levelName, 0, sizeof levelName);
    h_memset(pad, 0, sizeof pad);
    menu = menu_records;
    h_memset(menu_records, 0, sizeof menu_records);
    statusPtr = &MC_LastModeSuccessful;
    MC_LastModeSuccessful = 0;
    coreLaunchSRS = 0;
    gameBailFromGameLoop = 0;
    endInit = 0;
    endTimer = 0;
    creditsPosY = 0;
    lineShift = 4;
    attemptLoadGame = 0;
    expWarnAlpha = 0;
    coreBeenInSRS = 0;
    startMenuState = 1;
    frontend_end_timer_1104 = 3;
    gameDifficulty = 0;
    isNetGame = 9;
    h_memset(creditsText, 0, sizeof creditsText);
    h_memset(seen, 0, sizeof seen);
}

#include "run_front_end.cpp"

static void expect(int condition, int code) {
    if (!condition) fail(code);
}

int main() {
    reset();
    musicPlaying = 0;
    frontEndStartInCredits = 0;
    runFrontEnd__Fv();
    expect(gameHidePlayers == 1, 1);
    expect(music_count == 2, 2);
    expect(h_strcmp(music_name[0], "mx_maintheme_l.vag") == 0 && music_mode[0] == 2, 3);
    expect(h_strcmp(music_name[1], "mx_mainmenu_l.vag") == 0 && music_mode[1] == 5, 4);
    expect(musicPlaying == 1 && musicStart == 77, 5);
    expect(isin_angle == (3 << 9), 6);
    expect(title_x == 270 && title_y == 36, 7);

    reset();
    frontEndStartInCredits = 1;
    musicPlaying = 0;
    menuNm = 60;
    endTimer = 1;
    runFrontEnd__Fv();
    expect(music_count == 1 && music_mode[0] == 1, 8);
    expect(h_strcmp(music_name[0], "mx_endcredits_ext_l.vag") == 0, 9);
    expect(!saw("title"), 10);

    reset();
    frontendControllerConfigRunning = 1;
    runFrontEnd__Fv();
    expect(saw("runConfig") && !saw("busy"), 11);
    expect(gameHidePlayers == 1, 12);

    reset();
    busy_value = 1;
    menuModeRebootQuestion = 1;
    runFrontEnd__Fv();
    expect(!saw("yesno") && !saw("reboot"), 13);

    reset();
    menuModeRebootQuestion = 1;
    yes_answer = 2;
    runFrontEnd__Fv();
    expect(saw("reboot") && menuModeRebootQuestion == 1, 14);
    reset();
    menuModeRebootQuestion = 1;
    yes_answer = 1;
    runFrontEnd__Fv();
    expect(!saw("reboot") && menuModeRebootQuestion == 0, 15);

    reset();
    netAttemptLaunch = 1;
    launch_value = 0;
    runFrontEnd__Fv();
    expect(netAttemptLaunch == 1 && !saw("cleanup"), 16);
    reset();
    netAttemptLaunch = 1;
    launch_value = 1;
    runFrontEnd__Fv();
    expect(netAttemptLaunch == 0 && saw("death") && cleanup_saw_clear_flag == 1 && cleanup_arg == 0, 17);
    reset();
    netAttemptLaunch = 1;
    launch_value = 2;
    netGameLaunchInfo[4] = 0xfe;
    runFrontEnd__Fv();
    expect(nmPlayers == -2 && gamePause == 0 && h_memcmp(levelName, "kelethin", 9) == 0, 18);
    expect(levelName[80] == 0xff, 19);
    reset();
    netAttemptLaunch = 1;
    launch_value = -3;
    runFrontEnd__Fv();
    expect(menu == menuNetClientServer && menuNm == 2 && saw("netFree0"), 20);

    reset();
    menuNm = 55;
    save_value = 0;
    runFrontEnd__Fv();
    expect(saw("pop") && menu == frontMenu && menuNm == 0, 21);
    reset();
    menuNm = 55;
    save_value = 1;
    pad[12] = 1;
    runFrontEnd__Fv();
    expect(saw("save0") && menu == frontMenu && menuNm == 0, 22);
    reset();
    menuNm = 55;
    save_value = 1;
    runFrontEnd__Fv();
    expect(!saw("pop") && !saw("save0") && menuNm == 55, 23);

    reset();
    menuNm = 12;
    statusPtr[0] = 1;
    runFrontEnd__Fv();
    expect(saw("startNet") && !saw("cleanup"), 24);

    reset();
    menuNm = 17;
    statusPtr[0] = 1;
    runFrontEnd__Fv();
    expect(cleanup_arg == 1 && coreLaunchSRS == 1 && gameBailFromGameLoop == 1 && menuNm == 18, 25);
    expect(!saw("menuDraw"), 26);

    reset();
    store_ptr(menu_records, "menu_startnewgame");
    pad[12] = 1;
    runFrontEnd__Fv();
    expect(menu == gameTypeMenu && menuNm == 1 && setpos_count == 1, 27);

    reset();
    store_ptr(menu_records, "menu_startnewgame");
    pad[12 + 92] = 1;
    runFrontEnd__Fv();
    expect(menu == menu_records && menuNm == 0 && setpos_count == 0, 28);

    reset();
    store_ptr(menu_records, "menu_startnewgame");
    pad[12] = 3;
    runFrontEnd__Fv();
    expect(menu == frontMenu && menuNm == 0 && param_calls == 2, 29);

    reset();
    store_ptr(menu_records, "menu_loadsavedgame");
    pad[12] = 1;
    attemptLoadGame = 0;
    runFrontEnd__Fv();
    expect(browser_a == 0 && browser_b == 1 && attempt_during_browser == 0 && attemptLoadGame == 1, 30);

    reset();
    store_ptr(menu_records, "menu_credits");
    pad[12] = 1;
    lineShift = 9;
    runFrontEnd__Fv();
    expect(menu == creditMenu && menuNm == 60 && creditsPosY == 448, 31);
    expect(line_shift_during_clear == 0, 32);

    reset();
    menuNm = 60;
    menu = creditMenu;
    creditsPosY = 200;
    creditsText[0] = 1;
    store_ptr(creditsText + 4, "Hello");
    endTimer = 0;
    runFrontEnd__Fv();
    expect(font_color_count == 2 && font_colors[0] == 0x20000000u && font_colors[1] == 0x5c1a444cu, 33);
    expect(saw("stopCredits") && param_calls == 0, 34);

    reset();
    menu = difficultyMenu;
    menuNm = 4;
    expWarnAlpha = 0;
    menu_run_out = 0;
    runFrontEnd__Fv();
    union {
        unsigned u;
        float f;
    } bits;
    bits.f = expWarnAlpha;
    expect(bits.u == 0x3d84a444u, 35);
    reset();
    menu = difficultyMenu;
    menuNm = 4;
    bits.u = 0x3f7d70a4u;
    expWarnAlpha = bits.f;
    runFrontEnd__Fv();
    expect(expWarnAlpha == 1.0f, 36);

    reset();
    menuNm = 50;
    menu = optionMenu;
    pad[12] = 2;
    endTimer = 0;
    runFrontEnd__Fv();
    expect(menuNm == 55 && menu == optionMenu, 37);

    reset();
    menuNm = 60;
    menu = creditMenu;
    endTimer = 4;
    frontEndStartInCredits = 0;
    pad[12] = 2;
    runFrontEnd__Fv();
    expect(menuNm == 0 && saw("stopCredits"), 38);

    reset();
    menuNm = 60;
    menu = creditMenu;
    endTimer = 4;
    frontEndStartInCredits = 1;
    pad[12] = 2;
    runFrontEnd__Fv();
    expect(rect_w == 640 && rect_h == 512 && rect_color == 0x80000000u && menuNm == 60, 39);

    reset();
    menu = playerMenu;
    menuNm = 20;
    pad[12] = 1;
    menu_run_value = 1;
    runFrontEnd__Fv();
    expect(setpos_count == 2 && setpos_target[0] == difficultyMenu && nmPlayers == 2 && menuNm == 21, 40);

    reset();
    store_ptr(menu_records, "Start Net Game");
    pad[12] = 1;
    runFrontEnd__Fv();
    expect(player_inits == 4 && h_strcmp(player_name, "Toonces") == 0, 41);
    expect(h_memcmp(levelName, "kelethin", 9) == 0 && param_calls == 1, 42);

    reset();
    menu = difficultyMenu;
    menuNm = 21;
    nmPlayers = 3;
    norrath_frontend_byte_484ba8 = 0x5a;
    pad[12] = 1;
    menu_run_value = 0;
    runFrontEnd__Fv();
    expect(std::memcmp(playerSelectNextName, "kelsel", 7) == 0 && playerSelectNextName[48] == 0, 43);
    expect(playerSelectPrevName[0] == 0x5a && playerSelectPrevName[1] == 0, 44);
    expect(playerSelectEnforce == 1 && playerSelectCount == 3 && std::memcmp(levelName, "kelsel", 7) == 0, 45);

    if (error != 0) {
        std::printf("frontend contract failed: %d\n", error);
        return error;
    }
    return 0;
}
