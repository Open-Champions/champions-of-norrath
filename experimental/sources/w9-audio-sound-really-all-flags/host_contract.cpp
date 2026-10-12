#include "sound_really_all_flags.cpp"
#include <cstdio>
#include <cstring>
extern "C" alignas(16) unsigned char stream[3 * 452] = {};
static void put(int rec, int off, int v) { std::memcpy(stream + rec * 452 + off, &v, 4); }
int main() {
    if (soundReallyAllFlags__Fi(2) != 0) return 1;
    put(2, 32, 0x10);                       // counts although +20 is zero
    if (soundReallyAllFlags__Fi(2) != 0x10) return 2;
    put(2, 448, 2); put(2, 108, 0x1); put(2, 156, 0x4); put(2, 204, 0x80);
    if (soundReallyAllFlags__Fi(2) != 0x15) return 3;
    put(2, 448, 3);
    if (soundReallyAllFlags__Fi(2) != 0x95) return 4;
    put(2, 448, 0);
    if (soundReallyAllFlags__Fi(2) != 0x10) return 5;
    put(2, 448, -1);
    if (soundReallyAllFlags__Fi(2) != 0x10) return 6;
    put(0, 108, 0x400);
    if (soundReallyAllFlags__Fi(0) != 0) return 7;
    std::puts("sound_really_all_flags passed");
    return 0;
}
