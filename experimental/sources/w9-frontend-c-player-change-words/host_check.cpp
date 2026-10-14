#include "player_change_words.cpp"
int main() {
    static_assert(sizeof(pcNmPlayers) == 1 && sizeof(pcLaunch) == 1, "bytes");
    static_assert(sizeof(playerChangeOldYaw) == 8, "four halfwords");
    static_assert(sizeof(playerChangeOldPos) == 48, "four positions");
    static_assert(sizeof(playerStatus) == 16 && sizeof(playerRace) == 16, "four words");
#if defined(__mips__)
    static_assert(sizeof(pcTitle) == 4, "target pointer");
#endif
    if (pcNmPlayers || pcLaunch || pcTitle) return 1;
    for (int i = 0; i < 4; ++i) {
        if (playerChangeOldYaw[i] || playerStatus[i] || playerRace[i]) return 2;
        for (int j = 0; j < 3; ++j) if (playerChangeOldPos[i][j] != 0.0f) return 3;
    }
    return 0;
}
