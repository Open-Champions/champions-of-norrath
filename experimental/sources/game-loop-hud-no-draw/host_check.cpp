#include "hud_no_draw.cpp"
int main() {
    if (hudNoDraw != 0) return 1;
    hudNoDraw = 1;
    if (hudNoDraw != 1) return 2;
    return 0;
}
