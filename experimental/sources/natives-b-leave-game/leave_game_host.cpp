#include <cstdio>
extern "C" int amxLeaveGame__FP5__amxPi(void*, int*);
extern "C" int gameBailFromGameLoop = 0;

int main() {
    int cells[] = {4, 0};
    if (amxLeaveGame__FP5__amxPi(nullptr, cells) != 0) return 1;
    if (gameBailFromGameLoop != 1) return 2;
    cells[1] = 1;
    if (amxLeaveGame__FP5__amxPi(nullptr, cells) != 0) return 3;
    if (gameBailFromGameLoop != 101) return 4;
    cells[1] = -1;
    if (amxLeaveGame__FP5__amxPi(nullptr, cells) != 0) return 5;
    if (gameBailFromGameLoop != 101) return 6;
    std::puts("amxLeaveGame passed");
    return 0;
}
