#include "combat_camera.cpp"
#include <cstdio>
static_assert(sizeof(gameCombatCamera) == 4, "gameCombatCamera is one word");
int main() {
    if (gameCombatCamera != 0) return 1;
    gameCombatCamera = 2;
    if (gameCombatCamera != 2) return 2;
    std::puts("gameCombatCamera contract passed");
    return 0;
}
