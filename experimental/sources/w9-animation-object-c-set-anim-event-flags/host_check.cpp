// Host contract for setAnimEventFlags__8Creatureififififi. Not an EE plan input.
// Build: c++ -std=c++17 -ffp-contract=off -DNORRATH_HOST_ARENA host_check.cpp
#ifndef NORRATH_HOST_ARENA
#define NORRATH_HOST_ARENA
#endif
#include "../w9-animation-object-c-common/host_arena.hpp"
#include "../w9-animation-object-c-common/host_check.hpp"
#include "set_anim_event_flags.cpp"
int main() {
    unsigned char* creature = arena_at(0x100);
    unsigned char* table = arena_at(0x400);
    unsigned char* rec[3] = {arena_at(0x800), arena_at(0x900), arena_at(0xa00)};
    std::memset(arena_at(0), 0x33, 0x1000);
    sw_(creature + 36, norrath_host_address(table));
    for (int i = 0; i != 3; ++i) sw_(table + i * 8 + 4, norrath_host_address(rec[i]));
    unsigned char before[0x1000];
    std::memcpy(before, arena_at(0), 0x1000);
    setAnimEventFlags__8Creatureififififi(creature, 2, 1.5f, 0x1ff, -2.0f, 7, 3.25f, 0x80, 4.5f, 0);
    // Expected: only record 2 changed, eight stores.
    unsigned char want[0x1000];
    std::memcpy(want, before, 0x1000);
    unsigned char* r = want + 0xa00;
    sf_(r + 20, 1.5f); r[24] = 0xff;
    sf_(r + 28, -2.0f); r[32] = 7;
    sf_(r + 36, 3.25f); r[40] = 0x80;
    sf_(r + 44, 4.5f); r[48] = 0;
    if (same("record 2 only", arena_at(0), want, 0x1000)) return 1;
    // Index 0 goes to record 0 and leaves the others alone.
    std::memcpy(want, arena_at(0), 0x1000);
    r = want + 0x800;
    sf_(r + 20, 9.0f); r[24] = 1; sf_(r + 28, 8.0f); r[32] = 2; sf_(r + 36, 7.0f); r[40] = 3; sf_(r + 44, 6.0f); r[48] = 4;
    setAnimEventFlags__8Creatureififififi(creature, 0, 9.0f, 1, 8.0f, 2, 7.0f, 3, 6.0f, 4);
    if (same("record 0 only", arena_at(0), want, 0x1000)) return 2;
    std::printf("setAnimEventFlags check passed\n");
    return 0;
}
