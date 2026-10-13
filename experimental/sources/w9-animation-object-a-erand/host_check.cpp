// Host contract for eRand__10GameObject. Not an EE plan input.
// Build: c++ -std=c++17 -DNORRATH_HOST_ARENA host_check.cpp
#define NORRATH_HOST_ARENA
#include "../w9-animation-object-a-common/host_arena.hpp"
#include "../w9-animation-object-a-common/host_check.hpp"
#include "erand.cpp"
int main() {
    unsigned char* o = arena_at(0x100);
    std::memset(o, 0xa5, 128);
    // Independent arithmetic on 64-bit values for a spread of seeds.
    const unsigned seeds[] = {0u, 1u, 0xffffffffu, 0x12345678u, 0x80000000u, 0x3c6ef35fu};
    for (unsigned seed : seeds) {
        unsigned char before[128];
        std::memcpy(before, o, 128);
        sw_(o + 68, seed);
        sw_(before + 68, 0);
        const unsigned long long wide = (unsigned long long)seed * 1664525ull + 1013904223ull;
        const unsigned want = (unsigned)(wide & 0xffffffffull);
        const unsigned r = eRand__10GameObject(o);
        if (r != (want >> 16) || r > 0xffffu) { std::printf("seed %08x: got %u\n", seed, r); return 1; }
        if (hw(o + 68) != want) { std::printf("seed %08x: state %08x\n", seed, hw(o + 68)); return 2; }
        sw_(before + 68, want);
        if (same("neighbours", o, before, 128)) return 3;
    }
    // Chained calls follow the recurrence.
    sw_(o + 68, 7);
    unsigned s = 7;
    for (int i = 0; i < 1000; ++i) {
        s = s * 1664525u + 1013904223u;
        if (eRand__10GameObject(o) != (s >> 16)) { std::printf("chain %d\n", i); return 4; }
    }
    std::printf("eRand check passed\n");
    return 0;
}
