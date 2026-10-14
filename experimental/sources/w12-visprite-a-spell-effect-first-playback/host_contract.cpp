#define NORRATH_HOST_CONTRACT 1
#include "first_playback.cpp"
#include <cstring>

alignas(8) static unsigned char arena[1 << 16];
extern "C" char* norrath_host_translate(unsigned address) { return reinterpret_cast<char*>(arena) + address; }
static void put(unsigned off, unsigned v) { std::memcpy(arena + off, &v, 4); }

int main() {
    const unsigned sys = 0x100;
    std::memset(arena, 0, sizeof arena);
    // real first node (link non-zero) -> returned
    put(sys + 100, 0x1000); put(0x1000, 0x1040); put(0x1040, 0);
    if (FirstPlayback__19VISpellEffectSystem(arena + sys) != reinterpret_cast<char*>(arena) + 0x1000) return 1;
    // empty list: the node is the end marker (link zero) -> null
    put(sys + 100, 0x1040);
    if (FirstPlayback__19VISpellEffectSystem(arena + sys) != nullptr) return 2;
    return 0;
}
