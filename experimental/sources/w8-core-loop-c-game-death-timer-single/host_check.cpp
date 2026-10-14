// Host contract for gameDeathTimerSingle. Not an EE plan input.
#include "game_death_timer_single.cpp"

int main() {
    for (int i = 0; i < 4; ++i) {
        if (gameDeathTimerSingle[i] != 0) return 1 + i;
    }
    if (sizeof gameDeathTimerSingle != 8u) return 40;
    gameDeathTimerSingle[4-1] = 5;
    if (gameDeathTimerSingle[4-1] != 5) return 41;
    return 0;
}
