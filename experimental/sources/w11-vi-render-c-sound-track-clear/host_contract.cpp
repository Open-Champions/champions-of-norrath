#include "sound_track_clear.cpp"

#include <cstring>

int main() {
    alignas(8) unsigned char object[160];
    std::memset(object, 0x5a, sizeof object);
    if (Clear__17VISceneSoundTrack(reinterpret_cast<VISceneSoundTrack*>(object)) != 0) return 1;
    unsigned char expect[160];
    std::memset(expect, 0x5a, sizeof expect);
    auto w32 = [&](int off, unsigned v) { std::memcpy(expect + off, &v, 4); };
    auto w64 = [&](int off, unsigned long long v) { std::memcpy(expect + off, &v, 8); };
    w32(0, 1); w32(4, 0x3f800000u); w32(8, ~0u); w64(16, 15000); w64(24, 0); w32(32, ~0u); w32(36, ~0u);
    w64(40, 0); w32(48, 0x3f800000u); w32(56, 0); w64(64, 0); w32(72, 0); w32(76, 0);
    w32(80, ~0u); w32(84, ~0u); w32(88, 0); w32(92, ~0u);
    w32(96, 0); w64(104, 0); w32(112, 0); w32(116, 0); w32(120, ~0u); w32(124, ~0u); w32(128, 0); w32(132, ~0u);
    return std::memcmp(object, expect, sizeof object) == 0 ? 0 : 2;
}
