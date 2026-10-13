// Host contract for _GLOBAL_$I$getRankDescription__10SkillShockPUsi. Includes the source; not an EE input.
// Expected rows: eight integers, three float bit patterns, three integers.
#include "skill_shock_static_init.cpp"
#include "../w8-spell-static-init-a-common/host_record.hpp"

namespace {
const w8_host::Row expected[] = {
    {{164, 148, 343, 90, 111, 117, 216, 0}, {0x40c00000u, 0x40400000u, 0x3cf5c28fu}, {0, 1, 4}},
    {{111, 112, 154, 80, 33, 30, 28, 0}, {0x41000000u, 0x42960000u, 0x3dccccccu}, {2, 0, 6}},
};
void* const objects[] = {&ballParticle, &ballSmokeParticle};
}  // namespace

int main() {
    int bad = 0;
    bad |= w8_host::check_wrapper(_GLOBAL_$I$getRankDescription__10SkillShockPUsi, expected, objects, 2);
    bad |= w8_host::check_quiet_paths(static_initialization_and_destruction_0);
    return bad;
}
