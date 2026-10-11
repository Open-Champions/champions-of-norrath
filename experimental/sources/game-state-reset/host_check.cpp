// Host contract for gameResetGameState. Not an EE plan input.
#define memset norrath_host_memset
#include "reset.cpp"
#undef memset

#include <cstdint>
#include <cstdio>
#include <cstring>

extern "C" unsigned char startEntranceName[32] = {};
extern "C" unsigned char objectDrawEnable = 0;
extern "C" unsigned char engineCurrentLevelName[80] = {};
extern "C" unsigned char levelName[322] = {};
extern "C" float gamePlayerLightAttenuate = 0.0f;
extern "C" unsigned char floorDrawEnable = 0;
extern "C" int gameAutoMapMode = 0;
extern "C" int gameRecallInhibit = 7;
extern "C" unsigned char cheatOn = 1;
extern "C" int gameNoDraw = 3;
extern "C" unsigned char hudNoDraw = 1;
extern "C" unsigned char gameOver = 9;
extern "C" unsigned char levelSeeds[4096] = {};

static int failures = 0;
static char order[32];
static int order_used = 0;
static int new_game_argument = -1;
static unsigned memset_count = 0;
static unsigned char* memset_destination = 0;
static int memset_value = -1;
static char state_name[32];
static int state_value = -1;
static unsigned char game_over_at_new = 9;
static unsigned char level_81_at_new = 9;
static unsigned char level_242_at_new = 9;

static void expect(int ok, const char* message) {
    if (!ok) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}

static void mark(char step) {
    if (order_used < 31) order[order_used++] = step;
    order[order_used] = 0;
}

extern "C" void MC_NewGame__Fi(int value) {
    mark('N');
    new_game_argument = value;
    game_over_at_new = gameOver;
    level_81_at_new = levelName[81];
    level_242_at_new = levelName[242];
    gameOver = 4;
}
extern "C" void textInit__Fv() { mark('T'); }
extern "C" void* norrath_host_memset(void* destination, int value, unsigned count) {
    mark('M');
    memset_destination = static_cast<unsigned char*>(destination);
    memset_value = value;
    memset_count = count;
    std::memset(destination, value, count);
    return destination;
}
extern "C" void commInit__Fv() { mark('C'); }
extern "C" void removeAllQuestsFromLog__Fv() { mark('Q'); }
extern "C" void playerGlobalInit__Fv() { mark('P'); }
extern "C" void scriptSuperClear__Fv() { mark('S'); }
extern "C" void scriptClearAllGameState__Fv() { mark('A'); }
extern "C" void scriptSetGameState__FPci(char* name, int value) {
    mark('G');
    std::snprintf(state_name, sizeof state_name, "%s", name);
    state_value = value;
}

static std::uint32_t float_bits(float value) {
    std::uint32_t out = 0;
    std::memcpy(&out, &value, 4);
    return out;
}

int main() {
    std::memset(startEntranceName, 0x11, sizeof startEntranceName);
    std::memset(engineCurrentLevelName, 0x22, sizeof engineCurrentLevelName);
    std::memset(levelName, 0x33, sizeof levelName);
    std::memset(levelSeeds, 0x44, sizeof levelSeeds);
    norrath_game_state_byte_00484eb8 = 0x5a;
    objectDrawEnable = 0;
    floorDrawEnable = 0;
    gamePlayerLightAttenuate = 4.0f;
    gameAutoMapMode = 0;
    gameRecallInhibit = 7;
    cheatOn = 1;
    gameNoDraw = 3;
    hudNoDraw = 1;
    gameOver = 9;
    gameResetGameState__Fv();

    expect(std::strcmp(order, "NTMCQPSAG") == 0, "call order");
    expect(new_game_argument == 0, "MC_NewGame receives 0");
    expect(game_over_at_new == 0 && gameOver == 4, "gameOver is cleared before MC_NewGame");
    expect(level_81_at_new == 0 && level_242_at_new == 0, "level name bytes are cleared first");
    expect(startEntranceName[0] == 0x5a && startEntranceName[1] == 0x11, "only the first entrance byte is copied");
    expect(objectDrawEnable == 1 && floorDrawEnable == 1, "draw enables are set");
    expect(engineCurrentLevelName[0] == 0 && engineCurrentLevelName[1] == 0x22, "only the current level's first byte is cleared");
    expect(levelName[80] == 0x33 && levelName[81] == 0 && levelName[82] == 0x33, "only levelName byte 81");
    expect(levelName[241] == 0x33 && levelName[242] == 0 && levelName[243] == 0x33, "only levelName byte 242");
    expect(float_bits(gamePlayerLightAttenuate) == 0x3f800000u, "player light is 1.0f");
    expect(gameAutoMapMode == 2, "automap mode is 2");
    expect(gameRecallInhibit == 0 && cheatOn == 0 && gameNoDraw == 0 && hudNoDraw == 0, "inhibit, cheat, and draw flags clear");
    expect(memset_destination == levelSeeds && memset_value == 0 && memset_count == 4096u, "level seeds are cleared for 4096 bytes");
    expect(levelSeeds[0] == 0 && levelSeeds[4095] == 0, "both ends of the seed block are cleared");
    expect(std::strcmp(state_name, "needPlayers") == 0 && state_value == 1, "needPlayers is set to 1");

    norrath_game_state_byte_00484eb8 = 0;
    startEntranceName[0] = 9;
    gameResetGameState__Fv();
    expect(startEntranceName[0] == 0, "a zero source byte is copied");

    if (failures != 0) return 1;
    std::puts("gameResetGameState contract passed");
    return 0;
}
