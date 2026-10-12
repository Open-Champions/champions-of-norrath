#include "get_free_ram.cpp"
#include <cstdio>
#include <cstring>
extern "C" int effectsBufferSize = 0;
extern "C" int nmSounds = 0;
extern "C" unsigned char soundList[16 * 4] = {};
int main() {
    effectsBufferSize = 0x1000;
    if (soundGetFreeRam__Fv() != 0x200000 - 0x1000) return 1;
    int v = 0x4000, other = 0x77777;
    std::memcpy(soundList + 16 + 8, &v, 4);   // record 1 offset 8
    std::memcpy(soundList + 8, &other, 4);    // record 0 offset 8
    std::memcpy(soundList + 32 + 8, &other, 4);
    nmSounds = 2;
    if (soundGetFreeRam__Fv() != 0x200000 - 0x1000 - 0x4000) return 2;
    nmSounds = 1;
    if (soundGetFreeRam__Fv() != 0x200000 - 0x1000 - 0x77777) return 3;
    std::puts("get_free_ram passed");
    return 0;
}
