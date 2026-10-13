// Host comparison only. Not an EE build input.
#include "anim_identity.cpp"

#include <cstdio>

int main() {
    if (sizeof identityResult != 1600u) return 1;
    // Starts cleared, as the nobits image starts cleared.
    for (int i = 0; i < 1600; ++i) {
        if (identityResult[i] != 0) return 2;
    }
    // Writable across the initializer's whole span: the word at 4, the
    // halfword at 8, and 64 groups of 24 bytes from offset 16.
    identityResult[4] = 0x00;
    identityResult[5] = 0x80;
    identityResult[6] = 0xc1;
    identityResult[7] = 0x6d;
    identityResult[8] = 64;
    for (int group = 0; group < 64; ++group) {
        identityResult[16 + group * 24 + 2] = 0x10;
    }
    if (identityResult[7] != 0x6d || identityResult[8] != 64) return 3;
    if (identityResult[16 + 63 * 24 + 2] != 0x10) return 4;
    if (identityResult[1599] != 0) return 5;
    return 0;
}
