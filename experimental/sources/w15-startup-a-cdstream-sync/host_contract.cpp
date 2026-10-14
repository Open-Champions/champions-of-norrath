#include "sync.cpp"

#include <cstdio>

volatile int norrath_cdstream_pending;
static int rotations, rotate_arg, clear_after;

int RotateThreadReadyQueue(int priority) {
    rotate_arg = priority;
    if (++rotations == clear_after) norrath_cdstream_pending = 0;
    return 0;
}

int main() {
    norrath_cdstream_pending = 0;
    if (VICdStreamSync__Fi(0) != 0 || rotations != 0) return 1;
    if (VICdStreamSync__Fi(5) != 0 || rotations != 0) return 2;
    norrath_cdstream_pending = 1;
    if (VICdStreamSync__Fi(5) != 1 || VICdStreamSync__Fi(-1) != 1 || rotations != 0) return 3;
    norrath_cdstream_pending = 7;
    clear_after = 3;
    if (VICdStreamSync__Fi(0) != 0 || rotations != 3 || rotate_arg != 1) return 4;
    if (norrath_cdstream_pending != 0) return 5;
    std::puts("VICdStreamSync contract passed");
    return 0;
}
