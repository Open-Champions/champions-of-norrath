#include "multiplayer_markers.cpp"
#include <cstdio>
static_assert(sizeof(gameMultiPlayerMarkersOn) == 4, "gameMultiPlayerMarkersOn is one word");
int main() {
    if (gameMultiPlayerMarkersOn != 0) return 1;
    gameMultiPlayerMarkersOn = 1;
    if (gameMultiPlayerMarkersOn != 1) return 2;
    std::puts("gameMultiPlayerMarkersOn contract passed");
    return 0;
}
