// Host contract for _GLOBAL_$I$getRankDescription__18SkillAncestralCallPUsi. Includes the source; not an EE input.
// Expected rows: eight integers, three float bit patterns, three integers.
#include "skill_ancestral_call_static_init.cpp"
#include "../w8-spell-static-init-a-common/host_record.hpp"

namespace {
const w8_host::Row expected[] = {
    {{74, 72, 166, 72, 48, 9, 299, 64}, {0x40d00000u, 0x00000000u, 0x00000000u}, {2, 1, 7}},
};
void* const objects[] = {&callParticle};
}  // namespace

int main() {
    int bad = 0;
    bad |= w8_host::check_wrapper(_GLOBAL_$I$getRankDescription__18SkillAncestralCallPUsi, expected, objects, 1);
    bad |= w8_host::check_quiet_paths(static_initialization_and_destruction_0);
    return bad;
}
