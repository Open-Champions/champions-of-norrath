// Host contract for _GLOBAL_$I$getRankDescription__12SkillRipostePUsi. Includes the source; not an EE input.
// Expected rows: eight integers, three float bit patterns, three integers.
#include "skill_riposte_static_init.cpp"
#include "../w8-spell-static-init-a-common/host_record.hpp"

namespace {
const w8_host::Row expected[] = {
    {{150, 150, 150, 150, 255, 0, 0, 0}, {0x41c00000u, 0x00000000u, 0x00000000u}, {1, 1, 6}},
    {{200, 200, 200, 50, 255, 0, 0, 0}, {0x42100000u, 0xbf800000u, 0x00000000u}, {1, 1, 6}},
};
void* const objects[] = {&riposteParticle1, &riposteParticle2};
}  // namespace

int main() {
    int bad = 0;
    bad |= w8_host::check_wrapper(_GLOBAL_$I$getRankDescription__12SkillRipostePUsi, expected, objects, 2);
    bad |= w8_host::check_quiet_paths(static_initialization_and_destruction_0);
    return bad;
}
