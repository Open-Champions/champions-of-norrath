#include "mc_byte_copy.cpp"

#include <cstdio>

int main() {
    unsigned char source[8] = {1, 2, 3, 4, 5, 6, 7, 8};
    unsigned char dest[8] = {9, 9, 9, 9, 9, 9, 9, 9};
    MC_ByteCopy__FPvT0i(dest, source, 0);
    MC_ByteCopy__FPvT0i(dest, source, -5);
    for (unsigned char b : dest) if (b != 9) return 1;
    MC_ByteCopy__FPvT0i(dest, source, 5);
    for (int i = 0; i < 5; ++i) if (dest[i] != i + 1) return 2;
    for (int i = 5; i < 8; ++i) if (dest[i] != 9) return 3;
    // Overlapping copy forward by one repeats the first byte, which shows the copy order.
    unsigned char run[6] = {1, 2, 3, 4, 5, 6};
    MC_ByteCopy__FPvT0i(run + 1, run, 5);
    for (unsigned char b : run) if (b != 1) return 4;
    std::puts("mc byte copy passed");
    return 0;
}
