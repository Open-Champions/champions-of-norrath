#include "control_paused.cpp"
#include <cstdio>
static_assert(sizeof(gameControlPaused) == 4, "gameControlPaused is one word");
int main() {
    if (gameControlPaused != 0) return 1;
    gameControlPaused = 3;
    if (gameControlPaused != 3) return 2;
    std::puts("gameControlPaused contract passed");
    return 0;
}
