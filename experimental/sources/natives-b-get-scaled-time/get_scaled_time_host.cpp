#include <cstdio>
extern "C" int amxGetScaledTime__FP5__amxPi(void*, int*);

int main() {
    int cells[] = {4, 0};
    if (amxGetScaledTime__FP5__amxPi(nullptr, cells) != 0) return 1;
    cells[1] = 1;
    if (amxGetScaledTime__FP5__amxPi(nullptr, cells) != 1) return 2;
    cells[1] = 2;
    if (amxGetScaledTime__FP5__amxPi(nullptr, cells) != 2) return 3;
    cells[1] = -1;
    if (amxGetScaledTime__FP5__amxPi(nullptr, cells) != -1) return 4;
    cells[1] = 60;
    if (amxGetScaledTime__FP5__amxPi(nullptr, cells) != 60) return 5;
    cells[1] = 123456;
    if (amxGetScaledTime__FP5__amxPi(nullptr, cells) != 123456) return 6;
    std::puts("amxGetScaledTime passed");
    return 0;
}
