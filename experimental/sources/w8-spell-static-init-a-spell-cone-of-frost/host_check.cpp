// Host contract for _GLOBAL_$I$__16SpellConeOfFrostiP8CreatureP5Skill. Includes the source; not an EE input.
// Expected rows: eight integers, three float bit patterns, three integers.
#include "spell_cone_of_frost_static_init.cpp"
#include "../w8-spell-static-init-a-common/host_record.hpp"

namespace {
const w8_host::Row expected[] = {
    {{36, 30, 150, 51, 103, 123, 143, 0}, {0x42040000u, 0x41c80000u, 0xbf83d70au}, {1, 1, 5}},
    {{110, 108, 105, 123, 63, 56, 141, 0}, {0x42580000u, 0x428c0000u, 0x3d8f5c28u}, {1, 0, 6}},
    {{100, 154, 238, 103, 120, 117, 114, 4}, {0x41000000u, 0x41000000u, 0xc0c0f5c2u}, {2, 0, 8}},
};
void* const objects[] = {&primary, &puff, &ice};
}  // namespace

int main() {
    int bad = 0;
    bad |= w8_host::check_wrapper(_GLOBAL_$I$__16SpellConeOfFrostiP8CreatureP5Skill, expected, objects, 3);
    bad |= w8_host::check_quiet_paths(static_initialization_and_destruction_0);
    return bad;
}
