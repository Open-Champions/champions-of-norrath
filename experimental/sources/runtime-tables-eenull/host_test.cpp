// Host comparison only. Not an EE build input.
#include "eenull.cpp"

#include <cstdio>

int main() {
    if (sizeof eenull != 40u) return 1;
    // The blob opens with the observed setup sequence; the tail is zero.
    if (eenull[0] != 0x08 || eenull[1] != 0x00 || eenull[2] != 0x1d || eenull[3] != 0x3c) return 2;
    if (eenull[4] != 0x09 || eenull[5] != 0xf8 || eenull[6] != 0x60 || eenull[7] != 0x00) return 3;
    if (eenull[8] != 0xc0 || eenull[12] != 0xf8 || eenull[16] != 0x0c) return 4;
    for (int i = 20; i < 40; ++i) {
        if (eenull[i] != 0) return 5;
    }
    return 0;
}
