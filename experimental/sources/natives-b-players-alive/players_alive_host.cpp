#include <cstdio>
extern "C" int amxPlayersAlive__FP5__amxPi(void*, int*);
extern "C" int nmPlayersAlive = 0;

int main() {
    nmPlayersAlive = 0;
    if (amxPlayersAlive__FP5__amxPi(nullptr, nullptr) != 0) return 1;
    nmPlayersAlive = -4;
    if (amxPlayersAlive__FP5__amxPi(nullptr, nullptr) != -4) return 2;
    nmPlayersAlive = 9;
    if (amxPlayersAlive__FP5__amxPi(nullptr, nullptr) != 9) return 3;
    std::puts("amxPlayersAlive passed");
    return 0;
}
