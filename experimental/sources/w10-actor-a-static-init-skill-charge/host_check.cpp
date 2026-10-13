// Host contract for _GLOBAL_$I$getRankDescription__11SkillChargePUsi. Includes the source; not an EE input.
// Expected rows: eight integers, three float bit patterns, three integers.
#include "skill_charge_static_init.cpp"
#include "../w8-spell-static-init-a-common/host_record.hpp"

namespace {
const w8_host::Row expected[] = {
    {{112, 133, 134, 127, 59, 32, 207, 116}, {0x41b00000u, 0x3e4cccccu, 0x3f000000u}, {2, 1, 8}},
    {{35, 17, 114, 124, 55, 58, 99, 166}, {0x42b80000u, 0x3f666666u, 0x3e199999u}, {2, 3, 8}},
    {{112, 133, 207, 127, 207, 32, 32, 116}, {0x41b00000u, 0x00000000u, 0xbf000000u}, {2, 1, 8}},
};
void* const objects[] = {&chargeParticle, &chargeHeat, &chargeHit};
}  // namespace

int main() {
    int bad = 0;
    bad |= w8_host::check_wrapper(_GLOBAL_$I$getRankDescription__11SkillChargePUsi, expected, objects, 3);
    bad |= w8_host::check_quiet_paths(static_initialization_and_destruction_0);
    return bad;
}
