#include <cstdio>
extern "C" int amxHudResetFadeScreen__FP5__amxPi(void*, int*);
static int seen = -1;
extern "C" int hudResetFadeScreen__Fi(int value) {
    seen = value;
    return 9;
}

int main() {
    int cells[] = {4, 12};
    if (amxHudResetFadeScreen__FP5__amxPi(nullptr, cells) != 0) return 1;
    if (seen != 12) return 2;
    std::puts("amxHudResetFadeScreen passed");
    return 0;
}
