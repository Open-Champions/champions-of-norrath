#include "mc_get_temp_player_storage.cpp"

#include <cstdio>

unsigned char tempPlayerStruct[4 * 23984];

int main() {
    if (MC_GetTempPlayerStorage__Fi(0) != tempPlayerStruct) return 1;
    if (MC_GetTempPlayerStorage__Fi(1) != tempPlayerStruct + 23984) return 2;
    if (MC_GetTempPlayerStorage__Fi(3) != tempPlayerStruct + 71952) return 3;
    if (MC_GetTempPlayerStorage__Fi(-1) != tempPlayerStruct - 23984) return 4;
    std::puts("mc get temp player storage passed");
    return 0;
}
