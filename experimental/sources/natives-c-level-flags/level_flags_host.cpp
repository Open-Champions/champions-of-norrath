#include <cstdio>

extern "C" unsigned levelFlags;

int main() {
    if (levelFlags != 0u) return 1;
    levelFlags |= 0x20000000u;
    if (levelFlags != 0x20000000u) return 2;
    levelFlags = 0u;
    if (levelFlags != 0u) return 3;
    std::printf("level flags passed\n");
    return 0;
}
