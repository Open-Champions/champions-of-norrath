// Host contract for _GLOBAL_$I$run__19SpellHornOmenEffectG6Point3s. Includes the source; not an EE input.
// Expected rows: eight integers, three float bit patterns, three integers.
#include "spell_horn_omen_effect_static_init.cpp"
#include "../w8-spell-static-init-a-common/host_record.hpp"

namespace {
const w8_host::Row expected[] = {
    {{225, 20, 0, 255, 115, 10, 0, 255}, {0x41a00000u, 0x00000000u, 0x3f800000u}, {0, 2, 15}},
};
void* const objects[] = {&hornomenParticle1};
}  // namespace

int main() {
    int bad = 0;
    bad |= w8_host::check_wrapper(_GLOBAL_$I$run__19SpellHornOmenEffectG6Point3s, expected, objects, 1);
    bad |= w8_host::check_quiet_paths(static_initialization_and_destruction_0);
    return bad;
}
