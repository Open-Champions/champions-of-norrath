// Host check for MC_SetRamNmPlayers__Fi (source: ../w10-front-a-mc-set-ram-nm-players).
// Build: c++ -std=c++17 -Wall -Wextra -Werror -fno-builtin host_check.cpp
// Fails on a wrong header offset, wrong copy offset or size, wrong value, or a changed
// call order of the two footprint queries (camera first, then jump gate).
#include "../w10-front-a-mc-set-ram-nm-players/mc_set_ram_nm_players.cpp"

#include <cstdio>
#include <cstring>
#include <initializer_list>

extern "C" unsigned char ramSlot[];
extern "C" void* gameCam;
alignas(8) unsigned char ramSlot[1 << 16];
int camTag;
void* gameCam = &camTag;

static int order[4], norder;
static int copyCount, copyBytes;
static unsigned char* copyDest;
static int copyValue;

extern "C" int GetSaveFootPrint__6Camera(void* camera) {
    if (camera != &camTag) return -9999;
    order[norder++] = 1;
    return 1000;
}
extern "C" int jumpGateSaveFootPrint__Fv() {
    order[norder++] = 2;
    return 200;
}
extern "C" void MC_ByteCopy__FPvT0i(void* destination, void* source, int count) {
    ++copyCount;
    copyDest = static_cast<unsigned char*>(destination);
    copyBytes = count;
    std::memcpy(&copyValue, source, 4);
    std::memcpy(destination, source, static_cast<unsigned>(count));
}

int main() {
    for (int n : {0, 1, 4, 0x12345, -1}) {
        std::memset(ramSlot, 0x5a, sizeof ramSlot);
        norder = copyCount = 0;
        unsigned char before[sizeof ramSlot];
        std::memcpy(before, ramSlot, sizeof ramSlot);
        MC_SetRamNmPlayers__Fi(n);
        if (norder != 2 || order[0] != 1 || order[1] != 2) return 1;
        if (copyCount != 1 || copyBytes != 4 || copyValue != n) return 2;
        if (copyDest != ramSlot + 112 + 1000 + 200 + 4100) return 3;
        // expected image: halfword at 106, four bytes at 112+1200+4100
        before[106] = static_cast<unsigned char>(n);
        before[107] = static_cast<unsigned char>(n >> 8);
        std::memcpy(before + 112 + 1200 + 4100, &n, 4);
        if (std::memcmp(before, ramSlot, sizeof ramSlot) != 0) return 4;
    }
    std::puts("set ram nm players passed");
    return 0;
}
