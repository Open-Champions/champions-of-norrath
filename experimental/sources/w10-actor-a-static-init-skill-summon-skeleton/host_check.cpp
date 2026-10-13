// Host contract for _GLOBAL_$I$cast__19SkillSummonSkeletoniP8CreatureG6Point3s. Includes the source; not an EE input.
// Expected rows: eight integers, three float bit patterns, three integers.
#include "skill_summon_skeleton_static_init.cpp"
#include "../w8-spell-static-init-a-common/host_record.hpp"

namespace {
const w8_host::Row expected[] = {
    {{22, 21, 6, 200, 20, 19, 6, 0}, {0x42c80000u, 0xc0400000u, 0x00000000u}, {1, 0, 4}},
    {{50, 50, 60, 255, 0, 0, 0, 255}, {0x42480000u, 0x42480000u, 0x00000000u}, {1, 2, 6}},
};
void* const objects[] = {&summonskeletonParticle1, &summonskeletonParticle2};
}  // namespace

int main() {
    int bad = 0;
    bad |= w8_host::check_wrapper(_GLOBAL_$I$cast__19SkillSummonSkeletoniP8CreatureG6Point3s, expected, objects, 2);
    bad |= w8_host::check_quiet_paths(static_initialization_and_destruction_0);
    return bad;
}
