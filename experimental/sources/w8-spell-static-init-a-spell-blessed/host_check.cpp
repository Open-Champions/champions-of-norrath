// Host contract for _GLOBAL_$I$__12SpellBlessediP8CreatureP5Skill. Includes the source; not an EE input.
// Expected rows: eight integers, three float bit patterns, three integers.
#include "spell_blessed_static_init.cpp"
#include "../w8-spell-static-init-a-common/host_record.hpp"

namespace {
const w8_host::Row expected[] = {
    {{150, 150, 150, 150, 105, 105, 253, 0}, {0x40e00000u, 0x00000000u, 0x00000000u}, {3, 0, 8}},
    {{0, 0, 255, 190, 150, 150, 255, 0}, {0x41f00000u, 0x00000000u, 0xc0900000u}, {2, 2, 8}},
};
void* const objects[] = {&blessedParticle1, &blessedParticle2};
}  // namespace

int main() {
    int bad = 0;
    bad |= w8_host::check_wrapper(_GLOBAL_$I$__12SpellBlessediP8CreatureP5Skill, expected, objects, 2);
    bad |= w8_host::check_quiet_paths(static_initialization_and_destruction_0);
    return bad;
}
