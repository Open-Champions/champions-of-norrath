#include <cstdio>
#include <cstring>

extern "C" int norrath_world_get_door_height(void* world, int index);
extern unsigned char* norrath_door_arena;

unsigned char arena[256];
unsigned char* norrath_door_arena = arena;

namespace {
void set_word(unsigned char* address, unsigned value) {
    address[0] = static_cast<unsigned char>(value);
    address[1] = static_cast<unsigned char>(value >> 8);
    address[2] = static_cast<unsigned char>(value >> 16);
    address[3] = static_cast<unsigned char>(value >> 24);
}
}

int main() {
    unsigned char header[88];
    std::memset(header, 0, sizeof header);
    std::memset(arena, 0xcd, sizeof arena);
    if (norrath_world_get_door_height(header, -1) != 0) return 1;
    if (arena[0] != 0xcd) return 2;

    unsigned char* doors = arena + 16;
    unsigned char* items = arena + 128;
    set_word(header + 84, 16u);
    set_word(header + 32, 128u);
    doors[2 * 28 + 24] = 1;
    doors[2 * 28 + 25] = 0;
    set_word(items + 36 + 28, 0x1234u);
    if (norrath_world_get_door_height(header, 2) != 0x1234) return 3;

    doors[24] = 0xff;
    doors[25] = 0xff;
    set_word(items + 28, 9u);
    set_word(items - 8, 0x55u);
    if (norrath_world_get_door_height(header, 0) != 0x55) return 4;
    return 0;
}
