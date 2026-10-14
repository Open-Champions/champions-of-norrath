// Host check only; not a build input.
#define NORRATH_HOST_ARENA
static unsigned char arena[1 << 14];
static const unsigned arena_base = 0x100000u;
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + (address - arena_base); }
#include "set_node_frame.cpp"
#include <cstring>
int main() {
    alignas(8) unsigned w[16] = {};
    unsigned char src[16];
    for (int i = 0; i < 16; ++i) src[i] = static_cast<unsigned char>(0x30 + i);
    w[2] = 1; w[9] = arena_base + 0x100; w[10] = arena_base + 0x400;
    for (int k = 0; k < 4; ++k) *reinterpret_cast<int*>(arena + 0x100 + k * 8 + 4) = k * 5 + 1;
    std::memset(arena + 0x400, 0xee, 0x400);
    if (SetNodeFrame__13VIHSpriteAnimiiR18VIHSpritePackFrame(w, 2, 3, src) != 0) return 1;
    if (std::memcmp(arena + 0x400 + 14 * 16, src, 16) != 0) return 2;
    if (arena[0x400 + 14 * 16 - 1] != 0xee || arena[0x400 + 15 * 16] != 0xee) return 3;
    w[2] = 0;
    std::memset(arena + 0x400, 0xee, 0x400);
    if (SetNodeFrame__13VIHSpriteAnimiiR18VIHSpritePackFrame(w, 2, 3, src) != -1 || arena[0x400 + 14 * 16] != 0xee) return 4;
    return 0;
}
