#include "game_config.cpp"

#include <cstdio>

int main() {
    if (sizeof MC_GameConfig != 252) return 1;
    for (int index = 0; index < 252; ++index) {
        if (MC_GameConfig[index] != 0) return 2;
    }
    MC_GameConfig[0] = 0x14;
    MC_GameConfig[251] = 0x20;
    if (MC_GameConfig[0] != 0x14 || MC_GameConfig[251] != 0x20 || MC_GameConfig[1] != 0) return 3;
    std::puts("game config passed");
    return 0;
}
