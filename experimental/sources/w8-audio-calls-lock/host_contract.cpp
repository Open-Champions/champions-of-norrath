#include "sound_lock.cpp"
#include <cstdio>
extern "C" int nmSounds = 0;
extern "C" unsigned char soundList[16 * 6] = {};
static signed char& lk(int i) { return reinterpret_cast<signed char&>(soundList[i * 16 + 12]); }
int main() {
    lk(0) = 0; lk(1) = 5; lk(2) = -3; lk(3) = 1; lk(4) = 0;
    soundList[11] = 0x55; soundList[13] = 0x66;
    nmSounds = 0;
    soundLock__Fi(2);
    if (lk(0) != 0 || lk(2) != -3) return 1;
    nmSounds = -4;
    soundLock__Fi(2);
    if (lk(0) != 0) return 2;
    nmSounds = 4;
    soundLock__Fi(2);
    if (lk(0) != 2 || lk(1) != 5 || lk(2) != 2 || lk(3) != 2) return 3;
    if (lk(4) != 0) return 4;  // the fifth record is past the count
    if (soundList[11] != 0x55 || soundList[13] != 0x66) return 5;
    soundLock__Fi(-1);
    if (lk(0) != 2) return 6;
    std::puts("sound_lock passed");
    return 0;
}
