// Host contract for _GLOBAL_$I$cast__15SkillClericRootiP8CreatureG6Point3s. Includes the source; not an EE input.
// Expected rows: eight integers, three float bit patterns, three integers.
#include "skill_cleric_root_static_init.cpp"
#include "../w8-spell-static-init-a-common/host_record.hpp"

namespace {
const w8_host::Row expected[] = {
    {{2, 85, 2, 114, 0, 80, 0, 99}, {0x41400000u, 0x41300000u, 0xbfccccccu}, {3, 2, 2}},
    {{42, 52, 217, 50, 255, 285, 0, 43}, {0x41e00000u, 0x00000000u, 0x41c00000u}, {1, 1, 8}},
    {{105, 100, 2, 139, 121, 23, 0, 0}, {0x41400000u, 0x41400000u, 0xbfccccccu}, {1, 2, 8}},
};
void* const objects[] = {&clericroots, &magic, &miss};
}  // namespace

int main() {
    int bad = 0;
    bad |= w8_host::check_wrapper(_GLOBAL_$I$cast__15SkillClericRootiP8CreatureG6Point3s, expected, objects, 3);
    bad |= w8_host::check_quiet_paths(static_initialization_and_destruction_0);
    return bad;
}
