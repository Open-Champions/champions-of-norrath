// Host contract for _GLOBAL_$I$__10PoisonRainiP10GameObjectG6Point3P5SkillP15AnimationHeader. Includes the source; not an EE input.
// Expected rows: eight integers, three float bit patterns, three integers.
#include "poison_rain_static_init.cpp"
#include "../w8-spell-static-init-a-common/host_record.hpp"

namespace {
const w8_host::Row expected[] = {
    {{20, 80, 20, 0, 20, 80, 20, 175}, {0x409ad0e5u, 0x00000000u, 0x00000000u}, {1, 2, 4}},
    {{50, 200, 50, 100, 100, 250, 100, 0}, {0x3f800000u, 0x41200000u, 0x00000000u}, {1, 1, 7}},
};
void* const objects[] = {&poisonrainParticle1, &poisonrainParticle2};
}  // namespace

int main() {
    int bad = 0;
    bad |= w8_host::check_wrapper(_GLOBAL_$I$__10PoisonRainiP10GameObjectG6Point3P5SkillP15AnimationHeader, expected, objects, 2);
    bad |= w8_host::check_quiet_paths(static_initialization_and_destruction_0);
    return bad;
}
