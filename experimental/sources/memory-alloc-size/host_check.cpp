#include <cstdio>
#include <cstring>

extern "C" unsigned char memAllocSize[4];

int main() {
    if (sizeof memAllocSize != 4) return 1;
    if (std::memcmp(memAllocSize, "\0\0\0\0", 4) != 0) return 2;
    memAllocSize[1] = 0x30;
    if (memAllocSize[1] != 0x30 || memAllocSize[0] != 0) return 3;
    std::printf("memAllocSize checks passed\n");
    return 0;
}
