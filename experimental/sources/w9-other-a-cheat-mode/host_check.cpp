#include "cheat_mode.cpp"
int main() {
    if (sizeof(cheatMode) != 4) return 1;
    if (cheatMode != 0) return 2;
    cheatMode = 1;
    if (cheatMode != 1) return 3;
    return 0;
}
