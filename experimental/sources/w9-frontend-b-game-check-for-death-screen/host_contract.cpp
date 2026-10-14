#include "game_check_for_death_screen.cpp"

#include <cstdio>
#include <cstdlib>
#include <cstring>

signed char cutMode;
int gamePlayersPresent, nmPlayersAlive, nmPlayers, padFilter;
short gameDeathTimer, gameDeathTimerSingle[4];
char levelName[322];
char groundType[512];
unsigned char player[23984 * 4];
static int paused, kicks, starts, states, state_val;
static char state_name[16];
extern "C" int gameIsPaused__Fv() { return paused; }
extern "C" void gameNetKick__Fi(int a) { ++kicks; if (a != 0) std::abort(); }
extern "C" void gameDeathStart__Fi(int a) { ++starts; if (a != 0 || kicks != starts) std::abort(); }
extern "C" void scriptSetGameState__FPci(char* n, int v) { ++states; std::strcpy(state_name, n); state_val = v; }

static void setHealth(int i, float f) { std::memcpy(player + 172 + 23984 * i, &f, 4); }

static void reset() {
    cutMode = 0; gamePlayersPresent = 1; nmPlayersAlive = 0; nmPlayers = 1; padFilter = 0;
    gameDeathTimer = 2; paused = 0; kicks = starts = states = 0;
    for (int i = 0; i < 4; ++i) { gameDeathTimerSingle[i] = 3; setHealth(i, 1.0f); }
    std::strcpy(levelName + 81, "hate3");
    std::strcpy(groundType + 256, "hate3");
    levelName[0] = 'q'; levelName[80] = 1;
}

int main() {
    reset(); cutMode = 1;
    gameCheckForDeathScreen__Fv();
    if (padFilter != 0 || gameDeathTimer != 2) return 1;
    reset();
    gameCheckForDeathScreen__Fv();  // timer 2 -> 1: nothing else yet
    if (padFilter != 10 || gameDeathTimer != 1 || kicks || states) return 2;
    gameCheckForDeathScreen__Fv();  // timer reaches 0, same level, one player
    if (gameDeathTimer != 0 || gamePlayersPresent != 0 || states != 1) return 3;
    if (std::strcmp(state_name, "whichAct") != 0 || state_val != -1) return 4;
    if (std::strcmp(levelName, "actintro") != 0 || levelName[80] != -1 || kicks || starts) return 5;
    reset(); gameDeathTimer = 1; nmPlayers = 2;
    gameCheckForDeathScreen__Fv();  // two players: kick and start
    if (kicks != 1 || starts != 1 || gamePlayersPresent != 1 || states) return 6;
    reset(); gameDeathTimer = 1; std::strcpy(levelName + 81, "other");
    gameCheckForDeathScreen__Fv();
    if (kicks != 1 || starts != 1) return 7;
    reset(); paused = 1;
    gameCheckForDeathScreen__Fv();
    if (padFilter != 0 || gameDeathTimer != 2) return 8;
    reset(); nmPlayersAlive = 1;
    gameCheckForDeathScreen__Fv();
    if (gameDeathTimer != 2 || padFilter != 0) return 9;
    reset(); nmPlayers = 0;
    gameCheckForDeathScreen__Fv();
    if (gameDeathTimer != 2) return 10;
    reset(); gameDeathTimer = 0;
    gameCheckForDeathScreen__Fv();
    if (padFilter != 10 || gameDeathTimer != 0 || kicks) return 11;
    // single-player timers: players at or below zero tick down, others keep
    reset(); gamePlayersPresent = 0; nmPlayers = 3; nmPlayersAlive = 1;
    setHealth(0, 0.0f); setHealth(1, 5.0f); setHealth(2, -1.0f);
    gameDeathTimerSingle[2] = 0;
    gameCheckForDeathScreen__Fv();
    if (gameDeathTimerSingle[0] != 2 || gameDeathTimerSingle[1] != 3) return 12;
    if (gameDeathTimerSingle[2] != 0 || gameDeathTimerSingle[3] != 3) return 13;
    reset(); gamePlayersPresent = 0; nmPlayers = 3; nmPlayersAlive = 3;
    setHealth(0, 0.0f);
    gameCheckForDeathScreen__Fv();
    if (gameDeathTimerSingle[0] != 3) return 14;
    std::puts("game check for death screen passed");
    return 0;
}
