// Observed debugCheckForPauseMenuStart at 0x0017f800, 912 bytes.
// main registers it with engineAddTask and does not call it. The body returns
// before any later work when the signed display-list remainder is below 3000,
// coreLoading is nonzero, or recallOn is nonzero. Remainder is
// GLISTSIZE - arithmetic((glistp - gstart) >> 3).
// gameIsPaused, cutMode, jumpGateJumping, and hudNetWaiting are tried in that
// order and stop at the first hit. A hit continues only when startMenuState
// is nonzero. With netNetworkMode clear, word 52 of gameCam must be 1 and
// DLG_Active must return 0; otherwise the same startMenuState gate applies.
// Otherwise the local player's record in player, stride 23984, is read at
// offset 21872. A zero pointer skips the flag test. A second call's pointer
// is not checked again; its word at offset 12 is masked with 0x400000, and
// that bit returns only when startMenuState is zero.
// nmPlayers is a signed count. Each index calls netPlayerIsLocal and, for a
// local player, reads pad at index * 88 + 12. Bit 0x800 is required. Offline,
// the float at player offset 172 must not be ordered <= +0. MC_Busy must
// return 0. startMenuState zero opens the menu; nonzero closes it only when
// menu is mainMenu, and otherwise tries the next player.
// Opening stores the index in menuPlayerNm, sets startMenuState before
// msgHudSetPlayerBusy of that low signed byte and 1, and sets gamePause and
// particlesPaused only while offline. MC_Debug with pad word 8 equal to 2051
// selects superMainMenu; otherwise mainMenu. strcmp of levelName+81 and the
// unnamed bytes "lava3" at 0x004842f0 selects raceMainMenu and useRaceMenu.
// menuSetPos sees the final menu. menuNm is cleared after that call.
// coreInterfaceFlare(0) runs before debugPauseMenuOn is set. waitNetExitCount
// is cleared before engineAddTask(pauseMenuFrameRun, 52, "pauseMenu", 1).
// Closing clears startMenuState. While debugPauseMenuOn is set it also clears
// exitToMenuMode, clears the pause flags only while offline, clears
// debugPauseMenuOn, calls msgHudSetPlayerBusy with the sign-extended low byte
// of netGetLocalPlayerNm and 0, and removes pauseMenuFrameRun. Both close
// paths then read soundGetStreamPlayStatus(0) and fade stream 0 when bit
// 0x40 is set, then gameMaybeSaveConfig(0, 0).
// "lava3" and "pauseMenu" have no ELF symbol at the addresses used. The
// second menu compare in the close path repeats the mainMenu test; the taken
// path would clear startMenuState and return, and this source keeps that
// test. Nothing here was executed.
extern "C" int gameIsPaused__Fv();
extern "C" int jumpGateJumping__Fv();
extern "C" int DLG_Active__Fv();
extern "C" int netGetLocalPlayerNm__Fi(int value);
extern "C" int netPlayerIsLocal__Fi(int index);
extern "C" int MC_Busy__Fv();
extern "C" void msgHudSetPlayerBusy__Fcc(int player, int busy);
extern "C" int strcmp(const char* left, const char* right);
extern "C" void menuSetPos__FP4Menui(unsigned char* menu_pointer, int position);
extern "C" void coreInterfaceFlare__Fi(int value);
extern "C" void engineAddTask__FPFv_viPci(void (*callback)(), int priority, char* name, int flag);
extern "C" void pauseMenuFrameRun__Fv();
extern "C" void engineRemoveTask__FPFv_v(void (*callback)());
extern "C" int soundGetStreamPlayStatus__Fi(int stream);
extern "C" void soundFadeStream__Fi(int stream);
extern "C" void gameMaybeSaveConfig__Fii(int first, int second);

extern "C" unsigned glistp;
extern "C" unsigned gstart;
extern "C" unsigned GLISTSIZE;
extern "C" unsigned coreLoading;
extern "C" unsigned char recallOn;
extern "C" unsigned char cutMode;
extern "C" unsigned char hudNetWaiting;
extern "C" unsigned char netNetworkMode;
extern "C" unsigned char* gameCam;
extern "C" unsigned char player[];
extern "C" unsigned nmPlayers;
extern "C" unsigned char pad[];
extern "C" unsigned char startMenuState;
extern "C" unsigned char* menu;
extern "C" unsigned char mainMenu[];
extern "C" unsigned char superMainMenu[];
extern "C" unsigned char raceMainMenu[];
extern "C" unsigned menuPlayerNm;
extern "C" unsigned gamePause;
extern "C" unsigned char particlesPaused;
extern "C" unsigned char MC_Debug;
extern "C" char levelName[];
extern "C" unsigned char useRaceMenu;
extern "C" unsigned menuNm;
extern "C" unsigned char debugPauseMenuOn;
extern "C" unsigned char waitNetExitCount;
extern "C" unsigned char exitToMenuMode;

#if defined(NORRATH_HOST_CONTRACT)
extern "C" unsigned char* norrath_host_word_pointer(unsigned word);
#endif

namespace {
unsigned read_u32(const unsigned char* p) {
    return static_cast<unsigned>(p[0]) | (static_cast<unsigned>(p[1]) << 8) |
           (static_cast<unsigned>(p[2]) << 16) | (static_cast<unsigned>(p[3]) << 24);
}

unsigned char* object_at(unsigned word) {
#if defined(NORRATH_HOST_CONTRACT)
    return norrath_host_word_pointer(word);
#else
    return reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word));
#endif
}

int narrow_byte(int value) {
    const unsigned low = static_cast<unsigned>(value) & 0xffu;
    return low < 0x80u ? static_cast<int>(low) : static_cast<int>(low) - 256;
}

// c.le.s against +0 is false for NaN and true for both zeros and every negative value.
bool ordered_le_plus_zero(unsigned bits) {
    const unsigned exponent = (bits >> 23) & 0xffu;
    const unsigned fraction = bits & 0x7fffffu;
    if (exponent == 0xffu && fraction != 0u) return false;
    if ((bits & 0x80000000u) != 0u) return true;
    return (bits & 0x7fffffffu) == 0u;
}

void finish_sound() {
    const int status = soundGetStreamPlayStatus__Fi(0);
    if ((status & 0x40) != 0) soundFadeStream__Fi(0);
    gameMaybeSaveConfig__Fii(0, 0);
}

void close_menu() {
    if (debugPauseMenuOn != 0) {
        startMenuState = 0;
        exitToMenuMode = 0;
        if (netNetworkMode == 0) {
            gamePause = 0u;
            particlesPaused = 0;
        }
        debugPauseMenuOn = 0;
        const int local = netGetLocalPlayerNm__Fi(0);
        msgHudSetPlayerBusy__Fcc(narrow_byte(local), 0);
        engineRemoveTask__FPFv_v(pauseMenuFrameRun__Fv);
    } else {
        startMenuState = 0;
    }
    finish_sound();
}

void open_menu() {
    const int busy_player = narrow_byte(static_cast<int>(menuPlayerNm));
    startMenuState = 1;
    msgHudSetPlayerBusy__Fcc(busy_player, 1);
    if (netNetworkMode == 0) {
        gamePause = 1u;
        particlesPaused = 1;
    }
    unsigned char* chosen = mainMenu;
    if (MC_Debug != 0 && read_u32(pad + 8u) == 2051u) chosen = superMainMenu;
    menu = chosen;
    if (strcmp(levelName + 81, "lava3") != 0) useRaceMenu = 0;
    else useRaceMenu = 1;
    if (useRaceMenu != 0) menu = raceMainMenu;
    menuSetPos__FP4Menui(menu, 0);
    menuNm = 0u;
    coreInterfaceFlare__Fi(0);
    debugPauseMenuOn = 1;
    waitNetExitCount = 0;
    engineAddTask__FPFv_viPci(pauseMenuFrameRun__Fv, 52, const_cast<char*>("pauseMenu"), 1);
}
}

extern "C" void debugCheckForPauseMenuStart__Fv() {
    const unsigned difference = glistp - gstart;
    const int slots = static_cast<int>(difference) >> 3;
    const int remain = static_cast<int>(GLISTSIZE - static_cast<unsigned>(slots));
    if (remain < 3000) return;
    if (coreLoading != 0u) return;
    if (recallOn != 0) return;

    bool blocked = false;
    if (gameIsPaused__Fv() != 0) blocked = true;
    else if (cutMode != 0) blocked = true;
    else if (jumpGateJumping__Fv() != 0) blocked = true;
    else if (hudNetWaiting != 0) blocked = true;
    else if (netNetworkMode == 0) {
        const unsigned camera_word = read_u32(gameCam + 52);
        if ((camera_word ^ 1u) != 0u) blocked = true;
        else if (DLG_Active__Fv() != 0) blocked = true;
    }

    if (blocked) {
        if (startMenuState == 0) return;
    } else {
        const int first = netGetLocalPlayerNm__Fi(0);
        unsigned entity = read_u32(player + static_cast<unsigned>(first) * 23984u + 21872u);
        if (entity != 0u) {
            const int second = netGetLocalPlayerNm__Fi(0);
            entity = read_u32(player + static_cast<unsigned>(second) * 23984u + 21872u);
            const unsigned flags = read_u32(object_at(entity) + 12);
            if ((flags & 0x400000u) != 0u && startMenuState == 0) return;
        }
    }

    if (static_cast<int>(nmPlayers) <= 0) return;
    for (int index = 0; index < static_cast<int>(nmPlayers); ++index) {
        if (netPlayerIsLocal__Fi(index) == 0) continue;
        const unsigned buttons = read_u32(pad + static_cast<unsigned>(index) * 88u + 12u);
        if ((buttons & 0x800u) == 0u) continue;
        if (netNetworkMode == 0) {
            const unsigned health = read_u32(player + static_cast<unsigned>(index) * 23984u + 172u);
            if (ordered_le_plus_zero(health)) continue;
        }
        if (MC_Busy__Fv() != 0) continue;
        if (startMenuState != 0 && menu != mainMenu) continue;
        menuPlayerNm = static_cast<unsigned>(index);
        if (startMenuState != 0 && menu != mainMenu) {
            startMenuState = 0;
            return;
        }
        if (startMenuState != 0) {
            close_menu();
            return;
        }
        open_menu();
        return;
    }
}
