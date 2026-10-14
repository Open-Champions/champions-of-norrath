// Host contract for engineSetBlur. Not an EE plan input.
#include "engine_set_blur.cpp"

extern "C" {
int compositeAlpha = 0;
}

int main() {
    compositeAlpha = 100;
    engineSetBlur__Fi(0);       // 128 is not smaller than 100
    if (compositeAlpha != 100) return 1;
    engineSetBlur__Fi(28);      // 100 is not smaller than 100
    if (compositeAlpha != 100) return 2;
    engineSetBlur__Fi(29);      // 99 is smaller
    if (compositeAlpha != 99) return 3;
    engineSetBlur__Fi(200);     // negative limits compare signed
    if (compositeAlpha != -72) return 4;
    engineSetBlur__Fi(-50);
    if (compositeAlpha != -72) return 5;
    return 0;
}
