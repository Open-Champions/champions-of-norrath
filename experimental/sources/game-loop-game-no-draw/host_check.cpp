#include "game_no_draw.cpp"
int main() {
    if (gameNoDraw != 0) return 1;
    gameNoDraw = 1;
    if (gameNoDraw != 1) return 2;
    return 0;
}
