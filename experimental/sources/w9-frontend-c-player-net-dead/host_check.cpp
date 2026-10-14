#include "player_net_dead.cpp"
int nmPlayersAlive;
int playersAlive[4];
static int localMask;
static int calls;
int netPlayerIsLocal__Fi(int index) { ++calls; return (localMask >> index) & 1; }
int main() {
    nmPlayersAlive = 0;
    if (playerNetDead__Fv() != 1 || calls != 0) return 1;
    nmPlayersAlive = 3;
    playersAlive[0] = 0; playersAlive[1] = 2; playersAlive[2] = 3;
    localMask = 0x2;  // player 1 is local but not alive
    if (playerNetDead__Fv() != 1 || calls != 3) return 2;
    localMask = 0x4;  // player 2 local: stops at second entry
    calls = 0;
    if (playerNetDead__Fv() != 0 || calls != 2) return 3;
    localMask = 0x1;
    calls = 0;
    if (playerNetDead__Fv() != 0 || calls != 1) return 4;
    nmPlayersAlive = 2;  // entry 3 is beyond the count
    localMask = 0x8;
    if (playerNetDead__Fv() != 1) return 5;
    return 0;
}
