// Host contract for _GLOBAL_$I$cast__14SkillMultiFireiP8CreatureG6Point3s. Includes the source; not an EE input.
// Expected rows: eight integers, three float bit patterns, three integers.
#include "skill_multi_fire_static_init.cpp"
#include "../w8-spell-static-init-a-common/host_record.hpp"

namespace {
const w8_host::Row expected[] = {
    {{114, 132, 215, 28, 80, 45, 170, 0}, {0x411cccccu, 0x40666666u, 0xc0400000u}, {1, 2, 4}},
};
void* const objects[] = {&trail};
}  // namespace

int main() {
    int bad = 0;
    bad |= w8_host::check_wrapper(_GLOBAL_$I$cast__14SkillMultiFireiP8CreatureG6Point3s, expected, objects, 1);
    bad |= w8_host::check_quiet_paths(static_initialization_and_destruction_0);
    return bad;
}
