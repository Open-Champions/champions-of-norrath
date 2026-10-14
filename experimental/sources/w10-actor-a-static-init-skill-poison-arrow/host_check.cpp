// Host contract for _GLOBAL_$I$getRankDescription__16SkillPoisonArrowPUsi. Includes the source; not an EE input.
// Expected rows: eight integers, three float bit patterns, three integers.
#include "skill_poison_arrow_static_init.cpp"
#include "../w8-spell-static-init-a-common/host_record.hpp"

namespace {
const w8_host::Row expected[] = {
    {{41, 107, 5, 104, 28, 100, 49, 0}, {0x42180000u, 0x4258ccccu, 0x3f666666u}, {0, 1, 2}},
    {{103, 119, 31, 100, 9, 9, 9, 33}, {0x40e00000u, 0x40a00000u, 0x00000000u}, {1, 2, 6}},
    {{58, 127, 0, 143, 38, 141, 3, 11}, {0x41100000u, 0x41919999u, 0x00000000u}, {3, 2, 6}},
};
void* const objects[] = {&poisonSmoke, &poisonFleck, &poisonPoof};
}  // namespace

int main() {
    int bad = 0;
    bad |= w8_host::check_wrapper(_GLOBAL_$I$getRankDescription__16SkillPoisonArrowPUsi, expected, objects, 3);
    bad |= w8_host::check_quiet_paths(static_initialization_and_destruction_0);
    return bad;
}
