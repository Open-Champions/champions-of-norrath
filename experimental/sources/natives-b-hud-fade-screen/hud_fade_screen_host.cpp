#include <cstdio>
extern "C" int amxHudFadeScreen__FP5__amxPi(void*, int*);
static int seen = -1;
extern "C" int hudFadeScreen__Fi(int value) {
    seen = value;
    return 77;
}

int main() {
    int cells[] = {4, 3};
    if (amxHudFadeScreen__FP5__amxPi(nullptr, cells) != 77) return 1;
    if (seen != 3) return 2;
    cells[1] = 0;
    if (amxHudFadeScreen__FP5__amxPi(nullptr, cells) != 77) return 3;
    if (seen != 0) return 4;
    std::puts("amxHudFadeScreen passed");
    return 0;
}
