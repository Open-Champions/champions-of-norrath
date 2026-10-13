// Host comparison only. Not an EE build input.
#include "particle_storage.cpp"

#include <cstdio>

int main() {
    if (sizeof wigglyTab != 544u) return 1;
    if (sizeof descListMark != 128u) return 2;
    // Both start cleared, as the nobits image starts cleared.
    for (int i = 0; i < 544; ++i) {
        if (wigglyTab[i] != 0) return 3;
    }
    for (int i = 0; i < 128; ++i) {
        if (descListMark[i] != 0) return 4;
    }
    // Writable across the initializer's whole span: 512 bytes of pairs and
    // 32 mark words.
    for (int i = 0; i < 512; ++i) wigglyTab[i] = 0x3c;
    for (int i = 0; i < 32; ++i) {
        descListMark[i * 4u] = 0xff;
        descListMark[i * 4u + 1u] = 0xff;
        descListMark[i * 4u + 2u] = 0xff;
        descListMark[i * 4u + 3u] = 0xff;
    }
    for (int i = 0; i < 512; ++i) {
        if (wigglyTab[i] != 0x3c) return 5;
    }
    for (int i = 512; i < 544; ++i) {
        if (wigglyTab[i] != 0) return 6;
    }
    return 0;
}
