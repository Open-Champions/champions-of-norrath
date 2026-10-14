// Host contract for eRand__5Spell; not an EE input. Includes the source.
#include "erand.cpp"
#include <cstdio>
#include <cstring>

int main() {
    unsigned char o[64];
    int bad = 0;
    std::memset(o, 0x5a, sizeof o);
    const unsigned seeds[] = {0u, 1u, 0x12345678u, 0xffffffffu, 0x80000000u};
    for (unsigned seed : seeds) {
        unsigned char before[64];
        std::memcpy(o + 16, &seed, 4);
        std::memcpy(before, o, sizeof o);
        const unsigned want = seed * 1664525u + 1013904223u;
        const unsigned got = eRand__5Spell(o);
        unsigned stored;
        std::memcpy(&stored, o + 16, 4);
        if (got != (want >> 16) || stored != want) { std::printf("fail: seed %u\n", seed); bad = 1; }
        std::memcpy(before + 16, &want, 4);
        if (std::memcmp(before, o, sizeof o) != 0) { std::puts("fail: other bytes written"); bad = 1; }
    }
    unsigned seed0 = 0, one;
    std::memcpy(o + 16, &seed0, 4);
    eRand__5Spell(o);
    one = eRand__5Spell(o);
    if (one != ((1013904223u * 1664525u + 1013904223u) >> 16)) { std::puts("fail: second step"); bad = 1; }
    return bad;
}
