#include "pause_word.cpp"
#include <cstdio>
static_assert(sizeof(gamePause) == 4, "gamePause is one word");
int main() {
    if (gamePause != 0) return 1;
    gamePause = -10000;
    if (gamePause != -10000) return 2;
    std::puts("gamePause contract passed");
    return 0;
}
