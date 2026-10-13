#include "set_profiles.cpp"

#include <cstring>

static unsigned word_at(const unsigned char* base, int offset) {
    unsigned v;
    std::memcpy(&v, base + offset, 4);
    return v;
}

int main() {
    alignas(8) static unsigned char world[1024];
    std::memset(world, 0xa5, sizeof world);
    if (SetProfiles__7VIWorld(reinterpret_cast<VIWorld*>(world)) != 0) return 1;
    // The bytes before +88, from +968 on and the unwritten pair of the last row stay untouched.
    for (int i = 0; i < 88; ++i) if (world[i] != 0xa5) return 2;
    for (int i = 968; i < 1024; ++i) if (world[i] != 0xa5) return 3;
    // Spot checks against values read from the image's stores.
    if (word_at(world, 88) != 0x437a0000u || word_at(world, 92) != 0x44480000u) return 4;
    if (word_at(world, 96) != 0x3f800000u || word_at(world, 108) != 0x3f800000u) return 5;
    if (word_at(world, 112) != 1u || word_at(world, 120) != 9u || word_at(world, 132) != 0x43c80000u) return 6;
    if (word_at(world, 136) != 0x3f52d2d2u || word_at(world, 140) != 0x3f66e6e6u) return 7;
    if (word_at(world, 144) != 0x3f34b4b4u || word_at(world, 148) != 0x3f7fffffu) return 8;
    if (word_at(world, 884) != 0x3f800000u || word_at(world, 872) != 2u) return 9;
    if (word_at(world, 936) != 0x3f0c8c8cu || word_at(world, 964) != 0u) return 10;
    // A hash over the whole written range, taken from the image's stores.
    unsigned h = 2166136261u;
    for (int i = 88; i < 968; ++i) h = (h ^ world[i]) * 16777619u;
    if (h != 0x9d01e3bcu) return 11;
    return 0;
}
