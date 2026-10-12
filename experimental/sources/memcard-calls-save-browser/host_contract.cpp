#define NORRATH_HOST_ARENA
#include "save_browser.cpp"

#include <cstdio>
#include <cstring>

extern "C" int nmPlayers = 0;
extern "C" {
unsigned char player[23984 * 2];
}
extern "C" int disableMC = 0;
extern "C" int asyncMode = -1;
extern "C" int actionCoolOff = 0;
extern "C" signed char netNetworkMode = 0;
extern "C" signed char whichPad = 0;
extern "C" int ranConfig = 1;
extern "C" int ranBrowser = 1;
extern "C" unsigned char MC_LastModeSuccessful = 1;

static unsigned char objects[2][32];
extern "C" const unsigned char* norrath_host_objects[3] = {nullptr, objects[0], objects[1]};

static int local_calls;
static int local_argument;
static int pauses;
static int hud_player;
static int hud_busy;
static int last_at_hud;
static int mode;

void store_word(unsigned char* at, unsigned bits) {
    at[0] = static_cast<unsigned char>(bits);
    at[1] = static_cast<unsigned char>(bits >> 8);
    at[2] = static_cast<unsigned char>(bits >> 16);
    at[3] = static_cast<unsigned char>(bits >> 24);
}

void set_player(int index, unsigned object_index, unsigned flags, unsigned float_bits) {
    unsigned char* record = player + 172 + index * 23984;
    store_word(record, float_bits);
    store_word(record + 21700, object_index);
    store_word(objects[object_index - 1] + 12, flags);
}

extern "C" int netPlayerIsLocal__Fi(int player_index) {
    ++local_calls;
    local_argument = player_index;
    return player_index == 0x180 ? 1 : 0;
}
extern "C" void pushPause__Fv() { ++pauses; }
extern "C" void msgHudSetPlayerBusy__Fcc(int player_index, int busy) {
    hud_player = player_index;
    hud_busy = busy;
    last_at_hud = MC_LastModeSuccessful;
}
extern "C" int newMode__Fii(int next, int) { mode = next; return 0; }

int main() {
    std::memset(player, 0, sizeof player);
    nmPlayers = 0;
    MC_SaveGameBrowser__Fi(1);
    if (pauses != 0) return 1;

    nmPlayers = 1;
    set_player(0, 1, 0, 0);
    MC_SaveGameBrowser__Fi(1);
    if (pauses != 0) return 2;

    set_player(0, 1, 8, 0x3f800000u);
    MC_SaveGameBrowser__Fi(1);
    if (pauses != 0) return 3;

    set_player(0, 1, 0, 0x3f800000u);
    disableMC = 2;
    MC_SaveGameBrowser__Fi(1);
    if (pauses != 0) return 4;
    disableMC = 0;
    asyncMode = 1;
    MC_SaveGameBrowser__Fi(1);
    if (pauses != 0) return 5;
    asyncMode = -1;
    actionCoolOff = 1;
    MC_SaveGameBrowser__Fi(1);
    if (pauses != 0) return 6;
    actionCoolOff = 0;

    netNetworkMode = 1;
    local_calls = 0;
    MC_SaveGameBrowser__Fi(4);
    if (local_calls != 1 || local_argument != 4 || pauses != 0) return 7;

    ranConfig = 5;
    ranBrowser = 5;
    MC_LastModeSuccessful = 5;
    MC_SaveGameBrowser__Fi(0x180);
    if (pauses != 1 || whichPad != -128 || hud_player != -128 || hud_busy != 1) return 8;
    if (local_argument != 0x180 || last_at_hud != 0 || ranConfig != 0 || ranBrowser != 0) return 9;
    if (mode != 2) return 10;

    netNetworkMode = 0;
    local_calls = 0;
    MC_SaveGameBrowser__Fi(2);
    if (local_calls != 0 || pauses != 2 || whichPad != 2 || hud_player != 2) return 11;
    std::puts("save browser passed");
    return 0;
}
