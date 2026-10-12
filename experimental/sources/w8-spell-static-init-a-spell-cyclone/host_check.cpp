// Host contract for _GLOBAL_$I$__12SpellCycloneiP8CreatureP5Skill. Includes the source; not an EE input.
// Expected rows: eight integers, three float bit patterns, three integers.
#include "spell_cyclone_static_init.cpp"
#include "../w8-spell-static-init-a-common/host_record.hpp"

namespace {
const w8_host::Row expected[] = {
    {{35, 37, 20, 52, 129, 83, 55, 9}, {0x423c0000u, 0x41f00000u, 0xc0599999u}, {0, 1, 6}},
    {{306, 317, 75, 111, 99, 168, 82, 91}, {0x42780000u, 0x3f800000u, 0xc0d33333u}, {2, 1, 8}},
};
void* const objects[] = {&swirlParticle, &spark};
}  // namespace

int main() {
    int bad = 0;
    bad |= w8_host::check_wrapper(_GLOBAL_$I$__12SpellCycloneiP8CreatureP5Skill, expected, objects, 2);
    bad |= w8_host::check_quiet_paths(static_initialization_and_destruction_0);
    return bad;
}
