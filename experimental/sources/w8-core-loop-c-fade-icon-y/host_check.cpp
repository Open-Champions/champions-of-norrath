// Host contract for fadeIconY. Not an EE plan input.
#include "fade_icon_y.cpp"

int main() {
    for (int i = 0; i < 28; ++i) {
        if (fadeIconY[i] != 0) return 1 + i;
    }
    if (sizeof fadeIconY != 112u) return 40;
    fadeIconY[28-1] = 5;
    if (fadeIconY[28-1] != 5) return 41;
    return 0;
}
