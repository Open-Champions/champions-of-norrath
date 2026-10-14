// Host check only; not a build input.
#define NORRATH_HOST_ARENA
static unsigned char arena[1 << 12];
static const unsigned arena_base = 0x100000u;
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + (address - arena_base); }
#include "set_node_ref_id.cpp"
#include <cstring>
int main() {
    alignas(8) unsigned char object[64] = {};
    *reinterpret_cast<unsigned*>(object + 36) = arena_base + 0x40;
    std::memset(arena, 0x5a, sizeof arena);
    if (SetNodeRefID__13VIHSpriteAnimii(object, 3, -7) != 0) return 1;
    if (*reinterpret_cast<int*>(arena + 0x40 + 24) != -7) return 2;
    if (arena[0x40 + 23] != 0x5a || arena[0x40 + 28] != 0x5a || *reinterpret_cast<int*>(arena + 0x40 + 28) != 0x5a5a5a5a) return 3;
    if (SetNodeRefID__13VIHSpriteAnimii(object, 0, 9) != 0 || *reinterpret_cast<int*>(arena + 0x40) != 9) return 4;
    return 0;
}
