// Host contract for gameAutoMapMode. Not an EE plan input.
#include "game_auto_map_mode.cpp"

int main() {
    if (gameAutoMapMode != 0) return 1;
    if (sizeof gameAutoMapMode != 4u) return 2;
    gameAutoMapMode = gameAutoMapMode + 1;
    if (gameAutoMapMode != 0 + 1) return 3;
    return 0;
}
