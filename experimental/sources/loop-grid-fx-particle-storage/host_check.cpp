// Host contract for the particle storage words. Not an EE plan input.
#include <cstdio>

extern "C" {
extern unsigned char pushLevel[4];
extern unsigned char freeBlocks[4];
extern unsigned char globalSystem[256];
extern unsigned char blockList[512];
extern unsigned char* currentPS;
}

int main() {
    if (currentPS != globalSystem) { std::fprintf(stderr, "fail: currentPS starts at globalSystem\n"); return 1; }
    for (unsigned n = 0; n < 4; ++n)
        if (pushLevel[n] != 0 || freeBlocks[n] != 0) { std::fprintf(stderr, "fail: words start zero\n"); return 2; }
    for (unsigned n = 0; n < sizeof globalSystem; ++n) if (globalSystem[n] != 0) return 3;
    for (unsigned n = 0; n < sizeof blockList; ++n) if (blockList[n] != 0) return 4;
    std::printf("particle-storage contract passed\n");
    return 0;
}
