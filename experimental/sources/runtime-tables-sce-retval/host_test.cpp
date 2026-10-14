// Host comparison only. Not an EE build input.
#include "sce_retval.cpp"

#include <cstdio>

int main() {
    if (sizeof retval != 64u) return 1;
    // Starts cleared, as the nobits image starts cleared.
    for (int i = 0; i < 64; ++i) {
        if (retval[i] != 0) return 2;
    }
    // Writable at the version words the card initializer reads.
    retval[0] = 1;
    retval[4] = 2;
    retval[8] = 3;
    if (retval[0] != 1 || retval[4] != 2 || retval[8] != 3) return 3;
    return 0;
}
