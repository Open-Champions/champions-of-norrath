#include "object_draw_enable.cpp"
int main() {
    if (objectDrawEnable != 1) return 1;
    objectDrawEnable = 0;
    if (objectDrawEnable != 0) return 2;
    return 0;
}
