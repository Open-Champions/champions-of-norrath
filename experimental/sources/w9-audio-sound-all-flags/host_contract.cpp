#include "sound_all_flags.cpp"
#include <cstdio>
#include <cstring>
extern "C" alignas(16) unsigned char stream[3 * 452] = {};
static void put(int rec, int off, int v) { std::memcpy(stream + rec * 452 + off, &v, 4); }
int main() {
    if (soundAllFlags__Fi(1) != 0) return 1;
    put(1, 32, 0x10);                       // ignored while +20 is zero
    if (soundAllFlags__Fi(1) != 0) return 2;
    put(1, 20, 7);
    if (soundAllFlags__Fi(1) != 0x10) return 3;
    put(1, 448, 2); put(1, 108, 0x1); put(1, 156, 0x4); put(1, 204, 0x80);   // third is past the count
    if (soundAllFlags__Fi(1) != 0x15) return 4;
    put(1, 448, 3);
    if (soundAllFlags__Fi(1) != 0x95) return 5;
    put(1, 20, 0);                          // blocks count even without the +20 word
    if (soundAllFlags__Fi(1) != 0x85) return 6;
    put(1, 448, -4);
    if (soundAllFlags__Fi(1) != 0) return 7;
    put(0, 108, 0x400); put(2, 108, 0x800); put(2, 448, 1);
    if (soundAllFlags__Fi(0) != 0 || soundAllFlags__Fi(2) != 0x800) return 8;
    std::puts("sound_all_flags passed");
    return 0;
}
