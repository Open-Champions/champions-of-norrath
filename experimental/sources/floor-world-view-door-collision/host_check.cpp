#include <cstdio>
#include <cstring>

extern "C" int norrath_world_set_door_collision(void* world, int index, int enabled);
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
unsigned half_at(const unsigned char* address) {
    return address[0] | (unsigned(address[1]) << 8);
}
}

int main() {
    unsigned char header[88];
    std::memset(header, 0xa5, sizeof header);
    std::memset(arena, 0xa5, sizeof arena);
    if (norrath_world_set_door_collision(header, -3, 0) != 0) return 1;
    if (arena[16] != 0xa5) return 2;

    unsigned char* doors = arena + 16;
    unsigned char* items = arena + 128;
    std::memset(doors, 0, 56);
    set_word(header + 84, 16u);
    set_word(header + 32, 128u);
    doors[24] = 0;
    doors[25] = 0;
    items[16] = 0xf1;
    items[17] = 0x00;
    norrath_world_set_door_collision(header, 0, 1);
    if (half_at(items + 16) != 0x00f1u) return 3;
    norrath_world_set_door_collision(header, 0, 0);
    if (half_at(items + 16) != 0x00f3u) return 4;
    norrath_world_set_door_collision(header, 0, 9);
    if (half_at(items + 16) != 0x00f1u) return 5;
    if (items[18] != 0xa5) return 6;
    return 0;
}
