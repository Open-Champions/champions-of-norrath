#include "bail.cpp"
#include <cstdio>
static_assert(sizeof(gameBailFromGameLoop) == 4, "gameBailFromGameLoop is one word");
int main() {
    if (gameBailFromGameLoop != 0) return 1;
    gameBailFromGameLoop = 1;
    if (gameBailFromGameLoop != 1) return 2;
    std::puts("gameBailFromGameLoop contract passed");
    return 0;
}
