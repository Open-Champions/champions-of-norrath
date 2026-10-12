#include "player_selection_state.cpp"
int main() {
    if (sizeof playerSelRunning != 4 || playerSelRunning != 0) return 1;
    if (sizeof playerSelectSoundLump != 32) return 2;
    for (unsigned i = 0; i < 32; ++i) if (playerSelectSoundLump[i]) return 3;
    return 0;
}
