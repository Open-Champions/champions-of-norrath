// Host contract for gameLevelLoading. Not an EE plan input.
#include "game_level_loading.cpp"

int main() {
    if (gameLevelLoading != 0) return 1;
    if (sizeof gameLevelLoading != 4u) return 2;
    gameLevelLoading = gameLevelLoading + 1;
    if (gameLevelLoading != 0 + 1) return 3;
    return 0;
}
