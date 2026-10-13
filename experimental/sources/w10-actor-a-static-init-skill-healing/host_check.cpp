// Host contract for _GLOBAL_$I$getRankDescription__12SkillHealingPUsi. Includes the source; not an EE input.
// Expected rows: eight integers, three float bit patterns, three integers.
#include "skill_healing_static_init.cpp"
#include "../w8-spell-static-init-a-common/host_record.hpp"

namespace {
const w8_host::Row expected[] = {
    {{83, 82, 6, 57, 18, 31, 8, 40}, {0x40c00000u, 0x3f800000u, 0xbf800000u}, {2, 1, 8}},
};
void* const objects[] = {&aura};
}  // namespace

int main() {
    int bad = 0;
    bad |= w8_host::check_wrapper(_GLOBAL_$I$getRankDescription__12SkillHealingPUsi, expected, objects, 1);
    bad |= w8_host::check_quiet_paths(static_initialization_and_destruction_0);
    return bad;
}
