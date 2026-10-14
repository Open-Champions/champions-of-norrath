// Host contract for dumpMem__FPvi. Not an EE plan input. The nops are replaced by a counter
// so the number of loop steps can be checked on the host (32-bit address model is emulated
// by passing offsets inside a buffer).
#include <cstdio>
#include <cstdint>
static long steps;
#define asm steps++, (void)
#define volatile
#include "dump_mem.cpp"

static long run(unsigned off, int n) {
    alignas(16) static unsigned char buf[256];
    steps = 0;
    dumpMem__FPvi(buf + off, n);
    return steps;
}
int main() {
    // steps = 4 per iteration; iterations = end - (start & ~15)
    if (run(0, 0) != 0) return 1;
    if (run(0, 16) != 16 * 4) return 2;
    if (run(5, 3) != (5 + 3 - 0) * 4) return 3;    // start rounds down to 0
    if (run(17, 1) != (17 + 1 - 16) * 4) return 4; // start rounds down to 16
    if (run(20, -4) != 0) return 5;                // end below aligned start
    if (run(20, -5) != 0) return 6;
    return 0;
}
