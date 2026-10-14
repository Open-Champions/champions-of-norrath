// Host contract for bExitStatePersueIdle__8Creature. Not an EE plan input.
// Build: c++ -std=c++17 -DNORRATH_HOST_ARENA host_check.cpp
#define NORRATH_HOST_ARENA
#include "../w9-animation-object-a-common/host_arena.hpp"
#include "../w9-animation-object-a-common/host_check.hpp"
#include "b_exit_state_persue_idle.cpp"
int main() {
    unsigned char* c = arena_at(0x100);
    const unsigned values[] = {0u, 1u, 0x7fffu, 0x8000u, 0xffffu, 0x0100u, 0x00ffu};
    for (unsigned v : values) {
        std::memset(c, 0x5a, 1024);
        sh_(c + 794, v);
        unsigned char before[1024];
        std::memcpy(before, c, 1024);
        const int r = bExitStatePersueIdle__8Creature(c);
        if (r != (v == 0 ? 1 : 0)) { std::printf("value %04x: got %d\n", v, r); return 1; }
        if (same("no stores", c, before, 1024)) return 2;
    }
    // Neighbouring bytes do not count.
    std::memset(c, 0xff, 1024);
    sh_(c + 794, 0);
    if (bExitStatePersueIdle__8Creature(c) != 1) return 3;
    std::printf("bExitStatePersueIdle check passed\n");
    return 0;
}
