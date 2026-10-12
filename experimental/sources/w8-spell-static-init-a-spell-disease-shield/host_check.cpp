// Host contract for _GLOBAL_$I$__18SpellDiseaseShieldiP8CreatureP5Skill. Includes the source; not an EE input.
// Expected rows: eight integers, three float bit patterns, three integers.
#include "spell_disease_shield_static_init.cpp"
#include "../w8-spell-static-init-a-common/host_record.hpp"

namespace {
const w8_host::Row expected[] = {
    {{110, 100, 23, 100, 78, 66, 30, 5}, {0x42480000u, 0x42700000u, 0x00000000u}, {2, 0, 2}},
    {{12, 12, 0, 78, 8, 40, 16, 5}, {0x41c80000u, 0x41a00000u, 0x00000000u}, {3, 0, 8}},
};
void* const objects[] = {&diseaseshieldParticle1, &diseaseshieldParticle2};
}  // namespace

int main() {
    int bad = 0;
    bad |= w8_host::check_wrapper(_GLOBAL_$I$__18SpellDiseaseShieldiP8CreatureP5Skill, expected, objects, 2);
    bad |= w8_host::check_quiet_paths(static_initialization_and_destruction_0);
    return bad;
}
