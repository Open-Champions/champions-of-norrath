#include "player_state_words.cpp"
int main() {
    if (sizeof(activePlayer) != 4 || sizeof(isNetGame) != 4 || sizeof(gamePlayersPresent) != 4) return 1;
    if (sizeof(gameHidePlayers) != 1 || sizeof(wrathHammerCount) != 1 || sizeof(playerSelectSuccessful) != 1) return 2;
    if (activePlayer != 0 || isNetGame != 0 || gamePlayersPresent != 0 || gameHidePlayers != 0 || wrathHammerCount != 0) return 3;
    if (playerSelectSuccessful != 1) return 4;
    if (globalCharLightScale != 1.0f) return 5;
    if (sizeof(playerStartPos) != 48) return 6;
    for (unsigned i = 0; i < sizeof(playerStartPos); ++i) if (playerStartPos[i]) return 7;
    return 0;
}
