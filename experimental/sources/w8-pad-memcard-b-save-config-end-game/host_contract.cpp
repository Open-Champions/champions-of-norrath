#include "save_config_end_game.cpp"

int main() {
    if (sizeof saveConfigEndGame != 1u) return 1;
    if (saveConfigEndGame != 0) return 2;
    return 0;
}
