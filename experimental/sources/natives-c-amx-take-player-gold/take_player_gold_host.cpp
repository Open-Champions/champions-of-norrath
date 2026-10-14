#include <cstdio>
#include <cstring>

extern "C" int nmPlayers = 0;
extern "C" unsigned char player[80000] = {};
extern "C" int amxTakePlayerGold__FP5__amxPi(void*, int*);

static void put_gold(int index, unsigned value) {
    std::memcpy(player + 232 + index * 23984, &value, 4);
}

static unsigned gold(int index) {
    unsigned value = 0;
    std::memcpy(&value, player + 232 + index * 23984, 4);
    return value;
}

int main() {
    std::memset(player, 0, sizeof player);
    int params[2] = {4, 10};
    nmPlayers = 0;
    put_gold(0, 100);
    if (amxTakePlayerGold__FP5__amxPi(nullptr, params) != 0) return 1;
    if (gold(0) != 100) return 2;
    nmPlayers = -3;
    if (amxTakePlayerGold__FP5__amxPi(nullptr, params) != 0 || gold(0) != 100) return 3;
    nmPlayers = 2;
    put_gold(0, 100);
    put_gold(1, 40);
    if (amxTakePlayerGold__FP5__amxPi(nullptr, params) != 0) return 4;
    if (gold(0) != 95 || gold(1) != 35) return 5;
    put_gold(0, 3);
    put_gold(1, 1);
    params[1] = 100;
    if (amxTakePlayerGold__FP5__amxPi(nullptr, params) != 0) return 6;
    if (gold(0) != 0 || gold(1) != 0) return 7;
    put_gold(0, 0);
    params[1] = -4;
    if (amxTakePlayerGold__FP5__amxPi(nullptr, params) != 0) return 8;
    if (gold(0) != 2) return 9;
    std::printf("take player gold passed\n");
    return 0;
}
