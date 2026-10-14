#include "player_local_busy.cpp"
#include <cstring>
unsigned char player[4 * 23984];
int nmPlayersAlive;
int playersAlive[4];
static int localMask;
int netPlayerIsLocal__Fi(int index) { return (localMask >> index) & 1; }
static void setObject(int slot, unsigned* object) {
    std::memcpy(player + slot * 23984 + 21872, &object, sizeof object);
}
int main() {
    unsigned words[4][4] = {{0}};
    words[0][3] = 0x400000;     // busy, not local
    words[1][3] = 0x400000;     // busy, local
    words[2][3] = ~0x400000u;   // every other bit set, not busy, local
    words[3][3] = 0x200000;     // not busy
    for (int i = 0; i < 4; ++i) setObject(i, words[i]);
    nmPlayersAlive = 0;
    localMask = 0xf;
    if (playerLocalPlayerBusy__Fv() != 0) return 1;
    nmPlayersAlive = 3; playersAlive[0] = 0; playersAlive[1] = 1; playersAlive[2] = 2;
    localMask = 0x2;
    if (playerLocalPlayerBusy__Fv() != 1) return 2;
    localMask = 0x4;
    if (playerLocalPlayerBusy__Fv() != 0) return 3;
    localMask = 0x6;  // first local entry decides
    if (playerLocalPlayerBusy__Fv() != 1) return 4;
    localMask = 0x8;  // player 3 local but not in the alive list
    if (playerLocalPlayerBusy__Fv() != 0) return 5;
    playersAlive[0] = 3; localMask = 0x8;
    if (playerLocalPlayerBusy__Fv() != 0) return 6;
    return 0;
}
