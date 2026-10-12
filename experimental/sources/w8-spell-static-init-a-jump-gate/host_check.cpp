// Host contract for _GLOBAL_$I$__8JumpGateG6Point3sPPc. Includes the source; not an EE input.
// Expected rows: eight integers, three float bit patterns, three integers.
#include "jump_gate_static_init.cpp"
#include "../w8-spell-static-init-a-common/host_record.hpp"

namespace {
const w8_host::Row expected[] = {
    {{151, 17, 47, 124, 55, 129, 99, 166}, {0x41900000u, 0x00000000u, 0xbf800000u}, {2, 1, 8}},
    {{52, 74, 259, 132, 34, 87, 40, 0}, {0x41000000u, 0x41600000u, 0x3f800000u}, {3, 1, 1}},
    {{151, 17, 47, 124, 55, 129, 99, 166}, {0x41900000u, 0x00000000u, 0x40000000u}, {2, 1, 8}},
};
void* const objects[] = {&jumpParticleOn, &jumpParticleJump, &jumpParticle2};
}  // namespace

int main() {
    int bad = 0;
    bad |= w8_host::check_wrapper(_GLOBAL_$I$__8JumpGateG6Point3sPPc, expected, objects, 3);
    bad |= w8_host::check_quiet_paths(static_initialization_and_destruction_0);
    return bad;
}
