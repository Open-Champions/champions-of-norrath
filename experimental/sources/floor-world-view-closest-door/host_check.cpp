#include <cstdio>
#include <cstring>

extern "C" int norrath_world_find_closest_door(void* world, const void* point);
extern unsigned char* norrath_door_arena;

unsigned char arena[512];
unsigned char* norrath_door_arena = arena;

namespace {
void set_word(unsigned char* address, unsigned value) {
    address[0] = static_cast<unsigned char>(value);
    address[1] = static_cast<unsigned char>(value >> 8);
    address[2] = static_cast<unsigned char>(value >> 16);
    address[3] = static_cast<unsigned char>(value >> 24);
}
void set_half(unsigned char* address, int value) {
    const unsigned bits = static_cast<unsigned>(value);
    address[0] = static_cast<unsigned char>(bits);
    address[1] = static_cast<unsigned char>(bits >> 8);
}
unsigned half_at(const unsigned char* address) {
    return address[0] | (unsigned(address[1]) << 8);
}
void set_point(unsigned char* point, unsigned x, unsigned y, unsigned z) {
    set_word(point, x);
    set_word(point + 4, y);
    set_word(point + 8, z);
}
}

int main() {
    unsigned char header[88];
    unsigned char point[12];
    std::memset(header, 0, sizeof header);
    std::memset(arena, 0, sizeof arena);
    set_word(header + 36, 200u);
    set_word(header + 84, 16u);
    set_word(header + 80, 0u);
    set_point(point, 0, 0, 0);
    if (norrath_world_find_closest_door(header, point) != -1) return 1;

    unsigned char* doors = arena + 16;
    unsigned char* elements = arena + 200;
    set_word(header + 80, 1u);
    set_word(doors + 16, 400u);
    set_point(point, 0, 0, 0);
    if (norrath_world_find_closest_door(header, point) != -1) return 2;
    if (half_at(doors + 6) != 0u) return 3;

    std::memset(doors, 0, 64);
    set_word(header + 80, 2u);
    if (norrath_world_find_closest_door(header, point) != 1) return 4;
    if (half_at(doors + 6) != 0u) return 5;
    if (half_at(doors + 28 + 6) != 1u) return 6;
    if (norrath_world_find_closest_door(header, point) != -1) return 7;
    if (half_at(doors + 28 + 6) != 1u) return 8;

    std::memset(doors, 0, 64);
    std::memset(elements, 0, 120);
    set_word(doors + 8, 8000u);
    set_word(doors + 12, 4000u);
    set_word(doors + 16, 16u);
    set_half(doors + 20, 20000);
    set_half(doors + 22, -8000);
    set_word(elements + 36, 0x40u);
    set_point(point, 0x44744068u, 0xc4895a52u, 0x40800000u);
    set_word(header + 80, 1u);
    if (norrath_world_find_closest_door(header, point) != 0) return 9;
    set_word(elements + 36, 0u);
    doors[6] = 0;
    if (norrath_world_find_closest_door(header, point) != -1) return 10;
    return 0;
}
