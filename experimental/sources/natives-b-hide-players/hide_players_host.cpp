#include <cstdio>
extern "C" int amxHidePlayers__FP5__amxPi(void*, int*);
extern "C" unsigned char gameHidePlayers = 0;

int main() {
    int cells[] = {4, static_cast<int>(0x12345689u)};
    if (amxHidePlayers__FP5__amxPi(nullptr, cells) != 0) return 1;
    if (gameHidePlayers != 0x89) return 2;
    cells[1] = 0;
    if (amxHidePlayers__FP5__amxPi(nullptr, cells) != 0) return 3;
    if (gameHidePlayers != 0) return 4;
    std::puts("amxHidePlayers passed");
    return 0;
}
