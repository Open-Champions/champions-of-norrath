// Host contract for gameCreatureThinkSpinner. Not an EE plan input.
#include "game_creature_think_spinner.cpp"

int main() {
    if (gameCreatureThinkSpinner != 0) return 1;
    if (sizeof gameCreatureThinkSpinner != 1u) return 2;
    gameCreatureThinkSpinner = gameCreatureThinkSpinner + 1;
    if (gameCreatureThinkSpinner != 0 + 1) return 3;
    return 0;
}
