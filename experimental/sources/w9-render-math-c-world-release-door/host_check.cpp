// Host contract for worldReleaseDoor__FP12_worldHeaderi. Not an EE plan input.
// Build with -DNORRATH_HOST_DOOR: the stored 32-bit address is an offset into an arena.
#include <cstdio>
#include <cstring>
alignas(8) unsigned char arena[28 * 4];
unsigned char* norrath_door_arena = arena;
extern "C" void worldReleaseDoor__FP12_worldHeaderi(void*, int);
int main() {
    alignas(8) unsigned char header[96] = {};
    *reinterpret_cast<unsigned*>(header + 84) = 0u;
    std::memset(arena, 0xff, sizeof arena);
    worldReleaseDoor__FP12_worldHeaderi(header, -1);
    for (unsigned char b : arena) if (b != 0xff) return 1;
    worldReleaseDoor__FP12_worldHeaderi(header, 2);
    if (*reinterpret_cast<unsigned short*>(arena + 56 + 6) != 0xfffe) return 2;
    for (int i = 0; i < int(sizeof arena); ++i) if (i != 62 && arena[i] != 0xff) return 3;
    std::printf("world release door contract passed\n");
    return 0;
}
