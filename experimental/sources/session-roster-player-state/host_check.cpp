#include "player_state.cpp"
static bool zero(const unsigned char* p, unsigned n) {
    for (unsigned i = 0; i < n; ++i) if (p[i]) return false;
    return true;
}
int main() {
    if (sizeof playersAlive != 16 || !zero(playersAlive, 16)) return 1;
    if (sizeof startEntranceName != 32 || !zero(startEntranceName, 32)) return 2;
    if (sizeof syncRandState != 4 || syncRandState != 0) return 3;
    if (sizeof activePlayer != 4 || activePlayer != 0) return 4;
    if (sizeof nmPlayersAlive != 4 || nmPlayersAlive != 0) return 5;
    if (sizeof noMonsters != 1 || noMonsters != 0) return 6;
    if (sizeof nmSingle != 4 || nmSingle != 0) return 7;
    if (sizeof nmLines != 4 || nmLines != 0) return 8;
    if (sizeof invisible != 80 || !zero(invisible, 80)) return 9;
    return 0;
}
