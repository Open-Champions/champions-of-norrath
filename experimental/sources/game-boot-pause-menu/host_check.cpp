// Host contract for debugCheckForPauseMenuStart. Not an EE plan input.
#define NORRATH_HOST_CONTRACT 1
#include "debug_check.cpp"

#include <cstdio>
#include <cstring>

extern "C" {
unsigned glistp = 0;
unsigned gstart = 0;
unsigned GLISTSIZE = 3000u;
unsigned coreLoading = 0;
unsigned char recallOn = 0;
unsigned char cutMode = 0;
unsigned char hudNetWaiting = 0;
unsigned char netNetworkMode = 0;
unsigned char camera_bytes[64];
unsigned char* gameCam = camera_bytes;
unsigned char player[95936];
unsigned nmPlayers = 0;
unsigned char pad[512];
unsigned char startMenuState = 0;
unsigned char mainMenu[4];
unsigned char superMainMenu[4];
unsigned char raceMainMenu[4];
unsigned char* menu = nullptr;
unsigned menuPlayerNm = 99u;
unsigned gamePause = 9u;
unsigned char particlesPaused = 3;
unsigned char MC_Debug = 0;
char levelName[322];
unsigned char useRaceMenu = 7;
unsigned menuNm = 8u;
unsigned char debugPauseMenuOn = 0;
unsigned char waitNetExitCount = 4;
unsigned char exitToMenuMode = 5;
}

static int failures = 0;
static int paused_calls = 0;
static int jump_calls = 0;
static int dialog_calls = 0;
static int local_calls = 0;
static int local_script[4];
static int local_script_used = 0;
static int is_local_calls = 0;
static int local_mask = 0;
static int busy_calls = 0;
static int hud_calls = 0;
static int hud_player = 0;
static int hud_busy = -1;
static unsigned pause_at_hud = 0;
static unsigned char state_at_hud = 9;
static unsigned char debug_at_local = 9;
static int remove_calls = 0;
static int add_calls = 0;
static int add_priority = 0;
static int add_flag = -1;
static char add_name[16];
static void (*added)() = nullptr;
static void (*removed)() = nullptr;
static int flare_calls = 0;
static int setpos_calls = 0;
static unsigned char* setpos_menu = nullptr;
static int setpos_arg = -1;
static unsigned menu_nm_at_setpos = 0;
static unsigned char debug_at_flare = 9;
static unsigned char debug_at_add = 9;
static unsigned char wait_at_add = 9;
static int status_calls = 0;
static int fade_calls = 0;
static int save_calls = 0;
static int save_a = -1;
static int save_b = -1;
static int stream_status = 0;
static int paused_result = 0;
static int jump_result = 0;
static int dialog_result = 0;
static int busy_result = 0;
static int local_player_result = 0;
static unsigned seen_entity_word = 0;
static unsigned char entity[16];
static unsigned char empty_entity[16];

static void expect(bool ok, const char* message) {
    if (!ok) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}
static void write_u32(unsigned char* p, unsigned value) {
    p[0] = static_cast<unsigned char>(value);
    p[1] = static_cast<unsigned char>(value >> 8);
    p[2] = static_cast<unsigned char>(value >> 16);
    p[3] = static_cast<unsigned char>(value >> 24);
}
static void reset_calls() {
    paused_calls = jump_calls = dialog_calls = 0;
    local_calls = local_script_used = is_local_calls = busy_calls = 0;
    hud_calls = remove_calls = add_calls = flare_calls = setpos_calls = 0;
    status_calls = fade_calls = save_calls = 0;
    seen_entity_word = 0;
    hud_busy = -1;
}
static void reset_world() {
    std::memset(player, 0, sizeof player);
    std::memset(pad, 0, sizeof pad);
    std::memset(camera_bytes, 0, sizeof camera_bytes);
    std::memset(levelName, 0, sizeof levelName);
    std::memset(entity, 0, sizeof entity);
    std::memset(empty_entity, 0, sizeof empty_entity);
    glistp = gstart = 0;
    GLISTSIZE = 3000u;
    coreLoading = recallOn = cutMode = hudNetWaiting = netNetworkMode = 0;
    gameCam = camera_bytes;
    nmPlayers = 0;
    startMenuState = 0;
    menu = nullptr;
    menuPlayerNm = 99u;
    gamePause = 9u;
    particlesPaused = 3;
    MC_Debug = 0;
    useRaceMenu = 7;
    menuNm = 8u;
    debugPauseMenuOn = 0;
    waitNetExitCount = 4;
    exitToMenuMode = 5;
    paused_result = jump_result = dialog_result = busy_result = 0;
    local_player_result = 0;
    local_mask = 0;
    stream_status = 0;
    reset_calls();
}
static void allow_play() {
    write_u32(camera_bytes + 52, 1u);
}

extern "C" unsigned char* norrath_host_word_pointer(unsigned word) {
    seen_entity_word = word;
    return word == 0x1000u ? entity : empty_entity;
}
extern "C" int gameIsPaused__Fv() {
    ++paused_calls;
    return paused_result;
}
extern "C" int jumpGateJumping__Fv() {
    ++jump_calls;
    return jump_result;
}
extern "C" int DLG_Active__Fv() {
    ++dialog_calls;
    return dialog_result;
}
extern "C" int netGetLocalPlayerNm__Fi(int) {
    const int value = local_calls < local_script_used ? local_script[local_calls] : local_player_result;
    ++local_calls;
    debug_at_local = debugPauseMenuOn;
    return value;
}
extern "C" int netPlayerIsLocal__Fi(int index) {
    ++is_local_calls;
    return (local_mask & (1 << index)) != 0 ? 1 : 0;
}
extern "C" int MC_Busy__Fv() {
    ++busy_calls;
    return busy_result;
}
extern "C" void msgHudSetPlayerBusy__Fcc(int player_index, int busy) {
    ++hud_calls;
    hud_player = player_index;
    hud_busy = busy;
    pause_at_hud = gamePause;
    state_at_hud = startMenuState;
}
extern "C" int strcmp(const char* left, const char* right) {
    while (*left != 0 && *left == *right) {
        ++left;
        ++right;
    }
    return static_cast<unsigned char>(*left) - static_cast<unsigned char>(*right);
}
extern "C" void menuSetPos__FP4Menui(unsigned char* menu_pointer, int position) {
    ++setpos_calls;
    setpos_menu = menu_pointer;
    setpos_arg = position;
    menu_nm_at_setpos = menuNm;
}
extern "C" void coreInterfaceFlare__Fi(int) {
    ++flare_calls;
    debug_at_flare = debugPauseMenuOn;
}
extern "C" void engineAddTask__FPFv_viPci(void (*callback)(), int priority, char* name, int flag) {
    ++add_calls;
    added = callback;
    add_priority = priority;
    add_flag = flag;
    std::snprintf(add_name, sizeof add_name, "%s", name);
    debug_at_add = debugPauseMenuOn;
    wait_at_add = waitNetExitCount;
}
extern "C" void pauseMenuFrameRun__Fv() {}
extern "C" void engineRemoveTask__FPFv_v(void (*callback)()) {
    ++remove_calls;
    removed = callback;
}
extern "C" int soundGetStreamPlayStatus__Fi(int stream) {
    ++status_calls;
    return stream == 0 ? stream_status : 0;
}
extern "C" void soundFadeStream__Fi(int) { ++fade_calls; }
extern "C" void gameMaybeSaveConfig__Fii(int first, int second) {
    ++save_calls;
    save_a = first;
    save_b = second;
}

static void arm_player(int index, unsigned health) {
    local_mask |= 1 << index;
    write_u32(pad + static_cast<unsigned>(index) * 88u + 12u, 0x800u);
    write_u32(player + static_cast<unsigned>(index) * 23984u + 172u, health);
}

int main() {
    reset_world();
    GLISTSIZE = 2999u;
    debugCheckForPauseMenuStart__Fv();
    expect(paused_calls == 0 && local_calls == 0, "short display list returns immediately");

    reset_world();
    coreLoading = 1u;
    debugCheckForPauseMenuStart__Fv();
    expect(paused_calls == 0, "loading returns after the list check");

    reset_world();
    glistp = 8u;
    debugCheckForPauseMenuStart__Fv();
    expect(paused_calls == 0, "eight consumed list bytes drop the remainder below 3000");

    reset_world();
    gstart = 8u;
    GLISTSIZE = 2999u;
    paused_result = 1;
    debugCheckForPauseMenuStart__Fv();
    expect(paused_calls == 1 && jump_calls == 0, "arithmetic shift of a negative list delta still reaches the pause check");

    reset_world();
    recallOn = 1;
    debugCheckForPauseMenuStart__Fv();
    expect(paused_calls == 0, "recall returns before the pause check");

    reset_world();
    paused_result = 1;
    debugCheckForPauseMenuStart__Fv();
    expect(paused_calls == 1 && jump_calls == 0 && local_calls == 0, "pause with a clear menu state returns");

    reset_world();
    cutMode = 1;
    debugCheckForPauseMenuStart__Fv();
    expect(jump_calls == 0, "cut mode returns before jump");

    reset_world();
    jump_result = 1;
    hudNetWaiting = 1;
    debugCheckForPauseMenuStart__Fv();
    expect(jump_calls == 1 && dialog_calls == 0, "jumping returns before dialog");

    reset_world();
    debugCheckForPauseMenuStart__Fv();
    expect(dialog_calls == 0 && local_calls == 0, "camera word other than one returns before dialog");

    reset_world();
    allow_play();
    dialog_result = 1;
    debugCheckForPauseMenuStart__Fv();
    expect(dialog_calls == 1 && local_calls == 0, "active dialog returns");

    reset_world();
    allow_play();
    debugCheckForPauseMenuStart__Fv();
    expect(local_calls == 1 && is_local_calls == 0, "null entity and no players stops after one local-player call");

    reset_world();
    allow_play();
    write_u32(player + 21872, 0x1000u);
    write_u32(entity + 12, 0x400000u);
    local_script[0] = 0;
    local_script[1] = 0;
    local_script_used = 2;
    debugCheckForPauseMenuStart__Fv();
    expect(local_calls == 2 && seen_entity_word == 0x1000u && hud_calls == 0, "pause-blocking player bit returns");

    reset_world();
    paused_result = 1;
    startMenuState = 1;
    debugCheckForPauseMenuStart__Fv();
    expect(local_calls == 0, "a blocked call with menu state set skips the entity test");

    reset_world();
    allow_play();
    nmPlayers = 1;
    arm_player(0, 0x3f800000u);
    local_mask = 0;
    debugCheckForPauseMenuStart__Fv();
    expect(is_local_calls == 1 && busy_calls == 0, "a remote player is skipped");

    reset_world();
    allow_play();
    nmPlayers = 1;
    arm_player(0, 0x3f800000u);
    write_u32(pad + 12, 0u);
    debugCheckForPauseMenuStart__Fv();
    expect(busy_calls == 0, "the pad bit is required");

    reset_world();
    allow_play();
    nmPlayers = 1;
    arm_player(0, 0u);
    debugCheckForPauseMenuStart__Fv();
    expect(busy_calls == 0, "zero health skips the player");

    reset_world();
    allow_play();
    nmPlayers = 1;
    arm_player(0, 0x80000000u);
    debugCheckForPauseMenuStart__Fv();
    expect(busy_calls == 0, "negative zero health skips the player");

    reset_world();
    allow_play();
    nmPlayers = 1;
    arm_player(0, 0x3f800000u);
    busy_result = 1;
    debugCheckForPauseMenuStart__Fv();
    expect(busy_calls == 1 && hud_calls == 0, "a busy memory card skips the player");

    reset_world();
    allow_play();
    nmPlayers = 1;
    arm_player(0, 0x7fc00000u);
    debugCheckForPauseMenuStart__Fv();
    expect(hud_calls == 1 && hud_player == 0 && hud_busy == 1, "NaN health can open the menu");
    expect(state_at_hud == 1 && pause_at_hud == 9u, "menu state is set before the hud call and pause is not");
    expect(gamePause == 1u && particlesPaused == 1, "offline open pauses the game");
    expect(menu == mainMenu && useRaceMenu == 0, "default menu is main");
    expect(setpos_calls == 1 && setpos_menu == mainMenu && setpos_arg == 0, "menu position uses the final menu");
    expect(menu_nm_at_setpos == 8u && menuNm == 0u, "menu number clears after positioning");
    expect(debug_at_flare == 0 && debugPauseMenuOn == 1, "pause flag is set after the flare");
    expect(wait_at_add == 0 && add_calls == 1 && added == pauseMenuFrameRun__Fv, "pause task is registered");
    expect(add_priority == 52 && add_flag == 1 && std::strcmp(add_name, "pauseMenu") == 0, "pause task arguments");
    expect(remove_calls == 0 && save_calls == 0, "opening does not remove or save");

    reset_world();
    allow_play();
    nmPlayers = 1;
    arm_player(0, 0x3f800000u);
    std::memcpy(levelName + 81, "lava3", 6);
    debugCheckForPauseMenuStart__Fv();
    expect(menu == raceMainMenu && useRaceMenu == 1 && setpos_menu == raceMainMenu, "lava3 selects the race menu");

    reset_world();
    allow_play();
    nmPlayers = 2;
    local_mask = 2;
    write_u32(pad + 8, 2051u);
    write_u32(pad + 96, 0u);
    write_u32(pad + 100, 0x800u);
    write_u32(player + 23984u + 172u, 0x3f800000u);
    MC_Debug = 1;
    debugCheckForPauseMenuStart__Fv();
    expect(menuPlayerNm == 1u && hud_player == 1 && menu == superMainMenu, "debug pad word 2051 selects the super menu for player one");

    reset_world();
    allow_play();
    nmPlayers = 1;
    arm_player(0, 0x3f800000u);
    MC_Debug = 1;
    write_u32(pad + 8, 2050u);
    debugCheckForPauseMenuStart__Fv();
    expect(menu == mainMenu, "a different debug pad word keeps the main menu");

    reset_world();
    allow_play();
    nmPlayers = 1;
    arm_player(0, 0x3f800000u);
    MC_Debug = 1;
    write_u32(pad + 8, 2051u);
    std::memcpy(levelName + 81, "lava3", 6);
    debugCheckForPauseMenuStart__Fv();
    expect(menu == raceMainMenu, "lava3 replaces the super menu");

    reset_world();
    netNetworkMode = 1;
    nmPlayers = 1;
    arm_player(0, 0u);
    debugCheckForPauseMenuStart__Fv();
    expect(hud_calls == 1 && gamePause == 9u && particlesPaused == 3, "network open skips health and leaves the pause flags");

    reset_world();
    allow_play();
    nmPlayers = 1;
    arm_player(0, 0x3f800000u);
    startMenuState = 1;
    menu = superMainMenu;
    debugCheckForPauseMenuStart__Fv();
    expect(menuPlayerNm == 99u && hud_calls == 0 && remove_calls == 0, "a different menu tries the next player");

    reset_world();
    allow_play();
    nmPlayers = 1;
    arm_player(0, 0x3f800000u);
    startMenuState = 1;
    menu = mainMenu;
    debugPauseMenuOn = 1;
    local_script[0] = 0;
    local_script[1] = 0x1ff;
    local_script_used = 2;
    stream_status = 0x41;
    debugCheckForPauseMenuStart__Fv();
    expect(hud_calls == 1 && hud_player == -1 && hud_busy == 0, "close sign-extends the local player byte");
    expect(debug_at_local == 0, "debug flag clears before the local-player call");
    expect(gamePause == 0u && particlesPaused == 0 && exitToMenuMode == 0, "offline close clears pause state");
    expect(startMenuState == 0 && debugPauseMenuOn == 0, "close clears the menu state");
    expect(remove_calls == 1 && removed == pauseMenuFrameRun__Fv && add_calls == 0, "close removes the pause task");
    expect(fade_calls == 1 && save_calls == 1 && save_a == 0 && save_b == 0, "bit 0x40 fades and then saves");

    reset_world();
    netNetworkMode = 1;
    nmPlayers = 1;
    arm_player(0, 0u);
    startMenuState = 1;
    menu = mainMenu;
    debugPauseMenuOn = 1;
    stream_status = 1;
    debugCheckForPauseMenuStart__Fv();
    expect(gamePause == 9u && particlesPaused == 3 && exitToMenuMode == 0, "network close leaves the pause flags");
    expect(fade_calls == 0 && save_calls == 1, "a status without bit 0x40 does not fade");

    reset_world();
    allow_play();
    nmPlayers = 1;
    arm_player(0, 0x3f800000u);
    startMenuState = 1;
    menu = mainMenu;
    debugPauseMenuOn = 0;
    stream_status = 0x40;
    debugCheckForPauseMenuStart__Fv();
    expect(hud_calls == 0 && remove_calls == 0 && startMenuState == 0, "a clear debug flag only drops the menu state");
    expect(gamePause == 9u && fade_calls == 1 && save_calls == 1, "that path still fades and saves");

    if (failures != 0) return 1;
    std::puts("pause menu contract passed");
    return 0;
}
