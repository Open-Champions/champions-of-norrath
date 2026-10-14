#include <cstdio>
#include <cstring>

extern "C" unsigned char memAllocPtr[4];

int main() {
    if (sizeof memAllocPtr != 4) return 1;
    if (std::memcmp(memAllocPtr, "\0\0\0\0", 4) != 0) return 2;
    memAllocPtr[0] = 0x10;
    memAllocPtr[3] = 0x20;
    if (memAllocPtr[0] != 0x10 || memAllocPtr[3] != 0x20) return 3;
    std::printf("memAllocPtr checks passed\n");
    return 0;
}
