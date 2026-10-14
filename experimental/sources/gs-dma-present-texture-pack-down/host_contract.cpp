#include "texture_pack_down.cpp"

#include <cstdio>

extern "C" unsigned char textureUncompBufferLocked = 0;
static int clears;

extern "C" void cacheClear__Fv() { clears += 1; }

static int expect(unsigned char start, unsigned char after) {
    textureUncompBufferLocked = start;
    const int before = clears;
    texturePackDown__Fv();
    if (clears != before + 1) return 1;
    if (textureUncompBufferLocked != after) return 1;
    return 0;
}

int main() {
    clears = 0;
    if (expect(0, 0)) return 1;
    if (expect(1, 2)) return 2;
    if (expect(9, 10)) return 3;
    if (expect(10, 10)) return 4;
    if (expect(127, 127)) return 5;
    if (expect(128, 129)) return 6;
    if (expect(255, 0)) return 7;
    if (clears != 7) return 8;
    std::puts("texture pack down contract passed");
    return 0;
}
