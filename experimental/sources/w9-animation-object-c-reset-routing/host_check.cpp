// Host contract for resetRouting__8Creature. Not an EE plan input.
// Build: c++ -std=c++17 -ffp-contract=off -DNORRATH_HOST_ARENA host_check.cpp
#ifndef NORRATH_HOST_ARENA
#define NORRATH_HOST_ARENA
#endif
#include "../w9-animation-object-c-common/host_arena.hpp"
#include "../w9-animation-object-c-common/host_check.hpp"
#include "reset_routing.cpp"
int main() {
    unsigned char* c = arena_at(0x100);
    unsigned char want[1024];
    std::memset(c, 0x5a, 1024);
    std::memcpy(want, c, 1024);
    sf_(want + 260, -1.0f); sf_(want + 264, -1.0f);
    sw_(want + 580, 0xffffffffu); sw_(want + 584, 0xffffffffu);
    sw_(want + 708, 0); sw_(want + 716, 0);
    resetRouting__8Creature(c);
    if (same("all stores", c, want, 1024)) return 1;
    // From other contents only the six words change (no reads of the creature).
    std::memset(c, 0xa5, 1024);
    std::memset(want, 0xa5, 1024);
    sf_(want + 260, -1.0f); sf_(want + 264, -1.0f);
    sw_(want + 580, 0xffffffffu); sw_(want + 584, 0xffffffffu);
    sw_(want + 708, 0); sw_(want + 716, 0);
    resetRouting__8Creature(c);
    if (same("second call", c, want, 1024)) return 1;
    std::printf("resetRouting check passed\n");
    return 0;
}
