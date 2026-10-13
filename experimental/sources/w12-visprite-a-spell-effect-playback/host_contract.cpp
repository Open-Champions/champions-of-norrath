#define NORRATH_HOST_CONTRACT 1
#include "playback.cpp"
#include <cstring>

alignas(8) static unsigned char arena[1 << 16];
extern "C" char* norrath_host_translate(unsigned address) { return reinterpret_cast<char*>(arena) + address; }
static void put(unsigned off, unsigned v) { std::memcpy(arena + off, &v, 4); }

int main() {
    const unsigned sys = 0x100;
    std::memset(arena, 0, sizeof arena);
    char* base = reinterpret_cast<char*>(arena);
    // ids 5, 29 and 0x105 share slot 5; only the record in the table is examined
    put(sys + 4 + 4 * 5, 0x2000); put(0x2000 + 28, 29);
    if (Playback__19VISpellEffectSystemi(arena + sys, 29) != base + 0x2000) return 1;
    if (Playback__19VISpellEffectSystemi(arena + sys, 5) != nullptr) return 2;
    if (Playback__19VISpellEffectSystemi(arena + sys, 0x105) != nullptr) return 3;
    // empty slot
    if (Playback__19VISpellEffectSystemi(arena + sys, 6) != nullptr) return 4;
    // the hash uses only the low byte: id 0x11d (low byte 0x1d = 29) maps to slot 5 but compares unequal
    if (Playback__19VISpellEffectSystemi(arena + sys, 0x11d) != nullptr) return 5;
    // slot 23 is the last slot; slot index for 0x2f is 47 % 24 = 23
    put(sys + 4 + 4 * 23, 0x2100); put(0x2100 + 28, 47);
    if (Playback__19VISpellEffectSystemi(arena + sys, 47) != base + 0x2100) return 6;
    // negative ids compare as full ints
    put(sys + 4 + 4 * (0xff % 24), 0x2200); put(0x2200 + 28, -1);
    if (Playback__19VISpellEffectSystemi(arena + sys, -1) != base + 0x2200) return 7;
    return 0;
}
