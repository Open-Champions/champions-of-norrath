#include "game_over.cpp"
int main() {
    if (gameOver != 0) return 1;
    gameOver = 1;
    if (gameOver != 1) return 2;
    return 0;
}
