// Host check only; not a build input. Native build: c++ -std=c++17 -ffp-contract=off host_check.cpp
#include "get_time.cpp"
#include <cstring>

int main() {
    alignas(8) unsigned char scene[1280];
    std::memset(scene, 0x5a, sizeof scene);
    const unsigned char value[8] = {0x44, 0x33, 0x22, 0x11, 0x01, 0x00, 0x00, 0x80};
    std::memcpy(scene + 1152, value, 8);
    if (GetTime__7VIScene(scene) != static_cast<long long>(0x8000000111223344ull)) return 1;
    // Neighbouring words must not leak into the result.
    std::memset(scene + 1152, 0, 8);
    if (GetTime__7VIScene(scene) != 0) return 2;
    scene[1159] = 0xff; scene[1151] = 0x7f; scene[1160] = 0x7f;
    if (GetTime__7VIScene(scene) != static_cast<long long>(0xff00000000000000ull)) return 3;
    return 0;
}
