// Host contract for _GLOBAL_$I$cast__13SkillDarknessiP8CreatureG6Point3s. Includes the source; not an EE input.
// Expected rows: eight integers, three float bit patterns, three integers.
#include "skill_darkness_static_init.cpp"
#include "../w8-spell-static-init-a-common/host_record.hpp"

namespace {
const w8_host::Row expected[] = {
    {{33, 33, 33, 63, 7, 6, 7, 63}, {0x42ba0000u, 0x3f666666u, 0x3f666666u}, {1, 0, 6}},
};
void* const objects[] = {&darkParticle};
}  // namespace

int main() {
    int bad = 0;
    bad |= w8_host::check_wrapper(_GLOBAL_$I$cast__13SkillDarknessiP8CreatureG6Point3s, expected, objects, 1);
    bad |= w8_host::check_quiet_paths(static_initialization_and_destruction_0);
    return bad;
}
