#include "pad_change_index.cpp"

#include <cstdio>

extern "C" unsigned char padIndexMap[4] = {0, 1, 2, 3};

static bool is(unsigned a, unsigned b, unsigned c, unsigned d) {
    return padIndexMap[0] == a && padIndexMap[1] == b && padIndexMap[2] == c && padIndexMap[3] == d;
}

int main() {
    padChangeIndex__Fii(0, 3);
    if (!is(3, 1, 2, 0)) return 1;
    padChangeIndex__Fii(1, 2);
    if (!is(3, 2, 1, 0)) return 2;
    padChangeIndex__Fii(2, 2);
    if (!is(3, 2, 1, 0)) return 3;
    padChangeIndex__Fii(3, 0);
    if (!is(0, 2, 1, 3)) return 4;
    padIndexMap[1] = 0xf0;  // a byte with the top bit set must come through unchanged
    padChangeIndex__Fii(1, 2);
    if (!is(0, 1, 0xf0, 3)) return 5;
    std::puts("pad change index contract passed");
    return 0;
}
