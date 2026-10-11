#include "save_config_silent.cpp"

#include <cstdio>

extern "C" int MC_GameConfig[63] = {};
extern "C" int disableMC = 0;
extern "C" int gameBailFromGameLoop = 0;
extern "C" unsigned char saveConfigEndGame = 0;
extern "C" int ranConfig = 1;
extern "C" int delayCount = 1;

static int pauses;
static int delay_at_pause;
static int mode;

extern "C" void pushPause__Fv() {
    ++pauses;
    delay_at_pause = delayCount;
}
extern "C" int newMode__Fii(int next, int) { mode = next; return 17; }

int main() {
    MC_GameConfig[0] = 0x10;
    MC_GameConfig[1] = 0x55;
    disableMC = 1;
    gameBailFromGameLoop = 0;
    if (MC_SaveConfigSilent__Fii(0x4, 0) != 101 || pauses != 0) return 1;
    if (MC_GameConfig[0] != 0x14 || MC_GameConfig[1] != 0x55 || gameBailFromGameLoop != 0) return 2;
    if (MC_SaveConfigSilent__Fii(0x1, 3) != 101 || gameBailFromGameLoop != 101) return 3;
    if (MC_GameConfig[0] != 0x15 || pauses != 0) return 4;

    disableMC = 0;
    ranConfig = 8;
    delayCount = 8;
    saveConfigEndGame = 0;
    if (MC_SaveConfigSilent__Fii(0x20, 0x105) != 17) return 5;
    if (MC_GameConfig[0] != 0x35 || saveConfigEndGame != 5) return 6;
    if (pauses != 1 || delay_at_pause != 0 || ranConfig != 0 || mode != 5) return 7;
    std::puts("save config silent passed");
    return 0;
}
