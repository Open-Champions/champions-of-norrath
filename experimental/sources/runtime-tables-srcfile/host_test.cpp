// Host comparison only. Not an EE build input.
#include "srcfile.cpp"

#include <cstdio>

int main() {
    if (sizeof srcfile != 1856u) return 1;
    // The blob opens with the observed loader setup words; a zeroed or
    // truncated copy cannot pass these.
    if (srcfile[0] != 0x07 || srcfile[1] != 0x80 || srcfile[2] != 0x02 || srcfile[3] != 0x3c) return 2;
    unsigned nonzero = 0;
    for (int i = 0; i < 1856; ++i) {
        if (srcfile[i] != 0) ++nonzero;
    }
    if (nonzero != 1347u) return 3;
    // Last bytes of the observed span.
    if (srcfile[1854] != 0x07 || srcfile[1855] != 0x80) return 4;
    return 0;
}
