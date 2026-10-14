// Host check only; not a build input.
#define NORRATH_HOST_ARENA
static unsigned char arena[1 << 12];
static const unsigned arena_base = 0x100000u;
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + (address - arena_base); }
#include "set_keyframe.cpp"
#include <cstring>
int main() {
    alignas(8) unsigned char object[64] = {};
    *reinterpret_cast<unsigned*>(object + 44) = arena_base + 0x40;
    std::memset(arena, 0x5a, sizeof arena);
    if (SetKeyframe__13VIHSpriteAnimiif(object, 2, 77, 1.5f) != 0) return 1;
    if (*reinterpret_cast<float*>(arena + 0x40 + 16) != 1.5f || *reinterpret_cast<int*>(arena + 0x40 + 20) != 77) return 2;
    if (*reinterpret_cast<int*>(arena + 0x40 + 12) != 0x5a5a5a5a || *reinterpret_cast<int*>(arena + 0x40 + 24) != 0x5a5a5a5a) return 3;
    return 0;
}
