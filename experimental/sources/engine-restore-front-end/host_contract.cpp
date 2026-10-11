#include <cstdint>
#include <cstdio>
#include <cstring>

struct Menu {
    unsigned char storage;
};

#include "front_end_init.cpp"

static int error;
static int calls;
static char seen[8][32];

static void fail(int code) {
    if (error == 0) error = code;
}

static void note(const char* name) {
    if (calls < 8) std::snprintf(seen[calls], sizeof seen[calls], "%s", name);
    ++calls;
}

unsigned char frontEndSuspend;
unsigned char hudNoDraw;
unsigned char netNetworkMode;
int nmPlayers;
int musicPlaying;
unsigned char attemptLoadGame;
unsigned char MC_LastModeSuccessful;
int menuNm;
Menu* menu;
Menu frontMenu;
Menu creditMenu;
int autodemoTimer;
unsigned char levelName[322];
unsigned char frontEndStartInCredits;
int frontendControllerConfigRunning;
int frontEndRunning;
int lineShift;
int expWarnAlpha;
int gamePause;
int gameNoDraw;
unsigned char floorDrawEnable;
unsigned char objectDrawEnable;
int creditsPosY;
int engineHoldMpeg;
int emergencyMusicWaitAbort;

static int menu_nm_at_browser;
static int attempt_at_browser;
static Menu* menu_at_set;
static int menu_nm_at_set;
static int suspend_at_task;
static unsigned char credits_at_task;
static int running_at_task;
static int pause_at_task;
static unsigned char level_at_task[6];
static unsigned char level80_at_task;
static int task_second;
static char* task_text;
static int task_fourth;
static void (*task_callback)();
static char* lumps[2];
static int lump_count;
static int hold_before_credits;
static int nm_at_task;

extern "C" void MC_GetReadyForMainMenu__Fv() {
    note("ready");
    if (hudNoDraw != 9 || nmPlayers != 9 || musicPlaying != 9) fail(1);
}
extern "C" void MC_LoadGameBrowser__Fii(int first, int second) {
    note("browser");
    menu_nm_at_browser = menuNm;
    attempt_at_browser = attemptLoadGame;
    if (first != 0 || second != 1) fail(2);
}
extern "C" void menuSetPos__FP4Menui(Menu* target, int position) {
    note("set");
    menu_at_set = menu;
    menu_nm_at_set = menuNm;
    if (target != &frontMenu || position != 0 || menu != &frontMenu) fail(3);
}
extern "C" void padInitPadIndexMap__Fv() { note("pads"); }
extern "C" void runFrontEnd__Fv() {}
extern "C" void engineAddTask__FPFv_viPci(void (*callback)(), int second, char* third, int fourth) {
    note("task");
    suspend_at_task = frontEndSuspend;
    credits_at_task = frontEndStartInCredits;
    running_at_task = frontEndRunning;
    pause_at_task = gamePause;
    nm_at_task = menuNm;
    std::memcpy(level_at_task, levelName, 6);
    level80_at_task = levelName[80];
    task_callback = callback;
    task_second = second;
    task_text = third;
    task_fourth = fourth;
    if (floorDrawEnable != 1 || objectDrawEnable != 1 || gameNoDraw != 0) fail(4);
    if (frontendControllerConfigRunning != 0 || lineShift != 0 || expWarnAlpha != 0) fail(5);
    if (autodemoTimer != 0) fail(6);
}
extern "C" void lumpLoad__FPc(char* name) {
    if (lump_count < 2) lumps[lump_count] = name;
    ++lump_count;
    note("lump");
}

static void reset(unsigned char suspend, unsigned char attempt, unsigned char last, int nm, int argument_hold) {
    std::memset(levelName, 0x5a, sizeof levelName);
    frontEndSuspend = suspend;
    hudNoDraw = 9;
    netNetworkMode = 9;
    nmPlayers = 9;
    musicPlaying = 9;
    attemptLoadGame = attempt;
    MC_LastModeSuccessful = last;
    menuNm = nm;
    menu = reinterpret_cast<Menu*>(static_cast<std::uintptr_t>(0x1111u));
    autodemoTimer = 9;
    frontEndStartInCredits = 9;
    frontendControllerConfigRunning = 9;
    frontEndRunning = 9;
    lineShift = 9;
    expWarnAlpha = 9;
    gamePause = 9;
    gameNoDraw = 9;
    floorDrawEnable = 9;
    objectDrawEnable = 9;
    creditsPosY = 9;
    engineHoldMpeg = argument_hold;
    emergencyMusicWaitAbort = 9;
    calls = 0;
    lump_count = 0;
    menu_nm_at_browser = -1;
    attempt_at_browser = -1;
    menu_at_set = nullptr;
    menu_nm_at_set = -1;
    hold_before_credits = argument_hold;
}

static int expect_tail(unsigned char credit_byte, int old_nm, int took_menu) {
    if (error != 0) return error;
    if (std::strcmp(seen[0], "ready") != 0) return 10;
    if (hudNoDraw != 1 || netNetworkMode != 0 || nmPlayers != 1 || musicPlaying != 0) return 11;
    if (suspend_at_task != 0 || running_at_task != 1 || pause_at_task != 0) return 12;
    if (credits_at_task != credit_byte) return 13;
    if (task_callback != runFrontEnd__Fv || task_second != 25 || task_fourth != 0) return 14;
    if (task_text != norrath_frontend_task_name) return 15;
    if (std::memcmp(level_at_task, "intro", 6) != 0 || level80_at_task != 0xffu) return 16;
    if (lump_count != 2 || lumps[0] != norrath_frontend_fx_lump || lumps[1] != norrath_frontend_lump) return 17;
    if (credit_byte != 0) {
        if (creditsPosY != 448 || menu != &creditMenu || menuNm != 60) return 18;
        if (engineHoldMpeg != hold_before_credits) return 19;
    } else {
        if (engineHoldMpeg != 1 || emergencyMusicWaitAbort != 0) return 20;
        if (took_menu) {
            if (menu != &frontMenu) return 21;
        }
        (void)old_nm;
    }
    return 0;
}

int main() {
    reset(1, 1, 0, 7, 4);
    frontEndInit__Fi(0);
    if (calls != 5) return 30;
    if (std::strcmp(seen[1], "pads") != 0 || std::strcmp(seen[2], "task") != 0) return 31;
    if (menu != reinterpret_cast<Menu*>(static_cast<std::uintptr_t>(0x1111u))) return 32;
    if (nm_at_task != 7 || attemptLoadGame != 1) return 33;
    int status = expect_tail(0, 7, 0);
    if (status != 0) return status;

    reset(0, 0, 1, 4, 4);
    frontEndInit__Fi(1);
    if (std::strcmp(seen[1], "set") != 0 || menu_nm_at_set != 4) return 34;
    if (std::strcmp(seen[2], "pads") != 0 || nm_at_task != 0) return 35;
    status = expect_tail(1, 0, 1);
    if (status != 0) return status;

    reset(0, 1, 1, 4, 4);
    frontEndInit__Fi(0);
    if (std::strcmp(seen[1], "set") != 0) return 36;
    status = expect_tail(0, 0, 1);
    if (status != 0) return status;

    reset(0, 1, 0, 0, 4);
    frontEndInit__Fi(256);
    if (std::strcmp(seen[1], "browser") != 0) return 37;
    if (menu_nm_at_browser != 0 || attempt_at_browser != 0) return 38;
    if (nm_at_task != 0) return 39;
    status = expect_tail(0, 0, 0);
    if (status != 0) return status;
    if (menuNm != 0 || menu == &creditMenu) return 40;

    reset(0, 1, 0, 7, 4);
    frontEndInit__Fi(1);
    if (menu_nm_at_browser != 15 || nm_at_task != 15) return 41;
    status = expect_tail(1, 15, 0);
    if (status != 0) return status;

    reset(0, 0, 0, 3, 8);
    frontEndStartInCredits = 0;
    frontEndInit__Fi(2);
    if (credits_at_task != 2) return 42;
    if (creditsPosY != 448 || menuNm != 60 || menu != &creditMenu) return 43;
    return error;
}
