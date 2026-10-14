// Host contract for _GLOBAL_$I$canCast__18SkillConvertUndeadP8Creature. Includes the source; not an EE input.
// Expected rows: eight integers, three float bit patterns, three integers.
#include "skill_convert_undead_static_init.cpp"
#include "../w8-spell-static-init-a-common/host_record.hpp"

namespace {
const w8_host::Row expected[] = {
    {{32, 90, 200, 125, 128, 19, 15, 63}, {0x428c0000u, 0x40000000u, 0x3eccccccu}, {3, 1, 9}},
};
void* const objects[] = {&convertP};
}  // namespace

int main() {
    int bad = 0;
    bad |= w8_host::check_wrapper(_GLOBAL_$I$canCast__18SkillConvertUndeadP8Creature, expected, objects, 1);
    bad |= w8_host::check_quiet_paths(static_initialization_and_destruction_0);
    return bad;
}
