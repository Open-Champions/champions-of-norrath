// Host contract for _GLOBAL_$I$__8HolyBoltG6Point3iiP8CreatureP5Skill. Includes the source; not an EE input.
// Expected rows: eight integers, three float bit patterns, three integers.
#include "holy_bolt_static_init.cpp"
#include "../w8-spell-static-init-a-common/host_record.hpp"

namespace {
const w8_host::Row expected[] = {
    {{250, 200, 100, 255, 0, 0, 0, 0}, {0x42200000u, 0x41200000u, 0xc0a00000u}, {1, 1, 9}},
    {{255, 200, 170, 128, 250, 225, 135, 0}, {0x42180000u, 0x41a00000u, 0x3fb33333u}, {1, 2, 15}},
    {{86, 126, 250, 134, 4, 63, 105, 0}, {0x41a00000u, 0x00000000u, 0x3fb33333u}, {2, 2, 15}},
    {{86, 126, 250, 255, 0, 0, 0, 0}, {0x42700000u, 0x41200000u, 0xc1700000u}, {0, 1, 48}},
    {{86, 126, 250, 255, 0, 0, 0, 0}, {0x42700000u, 0x41200000u, 0xc1700000u}, {1, 1, 48}},
};
void* const objects[] = {&dismissundeadParticle1, &dismissundeadParticle2, &dismissundeadParticle3, &skullHitParticle, &skullHitParticleLong};
}  // namespace

int main() {
    int bad = 0;
    bad |= w8_host::check_wrapper(_GLOBAL_$I$__8HolyBoltG6Point3iiP8CreatureP5Skill, expected, objects, 5);
    bad |= w8_host::check_quiet_paths(static_initialization_and_destruction_0);
    return bad;
}
