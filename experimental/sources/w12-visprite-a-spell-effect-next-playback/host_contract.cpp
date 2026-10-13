#define NORRATH_HOST_CONTRACT 1
#include "next_playback.cpp"
#include <cstring>

alignas(8) static unsigned char arena[1 << 16];
extern "C" char* norrath_host_translate(unsigned address) { return reinterpret_cast<char*>(arena) + address; }
static void put(unsigned off, unsigned v) { std::memcpy(arena + off, &v, 4); }

int main() {
    std::memset(arena, 0, sizeof arena);
    put(0x1000, 0x1040); put(0x1040, 0x1080); put(0x1080, 0);
    char* base = reinterpret_cast<char*>(arena);
    if (NextPlayback__19VISpellEffectSystemPQ219VISpellEffectSystem16VISpellEffectPbi(nullptr, base + 0x1000) != base + 0x1040) return 1;
    if (NextPlayback__19VISpellEffectSystemPQ219VISpellEffectSystem16VISpellEffectPbi(nullptr, base + 0x1040) != nullptr) return 2;
    return 0;
}
