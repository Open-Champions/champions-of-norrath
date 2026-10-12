// Host contract for gameDeathTimer. Not an EE plan input.
#include "game_death_timer.cpp"

int main() {
    if (gameDeathTimer != -1) return 1;
    if (sizeof gameDeathTimer != 2u) return 2;
    gameDeathTimer = gameDeathTimer + 1;
    if (gameDeathTimer != -1 + 1) return 3;
    return 0;
}
