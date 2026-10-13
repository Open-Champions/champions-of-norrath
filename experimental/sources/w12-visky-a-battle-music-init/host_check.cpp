// Host check only; not a build input. Native build: c++ -std=c++17 -ffp-contract=off host_check.cpp
#include "init.cpp"
#include <cstring>

int main() {
    alignas(8) unsigned char music[64], want[64];
    std::memset(music, 0x77, sizeof music);
    std::memcpy(want, music, sizeof want);
    // A fake scene address that fits a 32-bit word.
    if (Init__13VIBattleMusicP7VIScene(music, reinterpret_cast<unsigned char*>(0x12345670u)) != 0) return 1;
    const unsigned scene = 0x12345670u, tick = 0x3c23d70au;
    const int minus1 = -1, zero = 0;
    std::memcpy(want + 0, &scene, 4); std::memcpy(want + 4, &minus1, 4);
    std::memcpy(want + 8, &zero, 4); std::memcpy(want + 12, &zero, 4);
    std::memcpy(want + 16, &tick, 4); std::memcpy(want + 24, &minus1, 4); std::memcpy(want + 28, &minus1, 4);
    if (std::memcmp(music, want, sizeof want) != 0) return 2;
    float f;
    std::memcpy(&f, music + 16, 4);
    return f == 0.01f ? 0 : 3;
}
