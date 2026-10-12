// Host contract for fadeTimer. Not an EE plan input.
#include "fade_timer.cpp"

int main() {
    for (int i = 0; i < 28; ++i) {
        if (fadeTimer[i] != 0) return 1 + i;
    }
    if (sizeof fadeTimer != 112u) return 40;
    fadeTimer[28-1] = 5;
    if (fadeTimer[28-1] != 5) return 41;
    return 0;
}
