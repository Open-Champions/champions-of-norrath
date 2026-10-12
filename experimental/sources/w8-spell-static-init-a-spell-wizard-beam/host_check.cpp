// Host contract for _GLOBAL_$I$__15SpellWizardBeamiP8CreatureP5Skill. Includes the source; not an EE input.
// Expected rows: eight integers, three float bit patterns, three integers.
#include "spell_wizard_beam_static_init.cpp"
#include "../w8-spell-static-init-a-common/host_record.hpp"

namespace {
const w8_host::Row expected[] = {
    {{295, 195, 111, 413, 226, 11, 14, -11}, {0x40800000u, 0x40c00000u, 0x00000000u}, {2, 1, 0}},
    {{130, 116, 76, 87, 194, 120, 74, 0}, {0x41c80000u, 0x42aa0000u, 0x00000000u}, {1, 1, 6}},
    {{0, 0, 0, 64, 34, 30, 21, 0}, {0x00000000u, 0x42a00000u, 0x00000000u}, {2, 0, 6}},
};
void* const objects[] = {&wizardbeamParticle1, &wizardbeamParticle2, &wizardbeamParticle3};
}  // namespace

int main() {
    int bad = 0;
    bad |= w8_host::check_wrapper(_GLOBAL_$I$__15SpellWizardBeamiP8CreatureP5Skill, expected, objects, 3);
    bad |= w8_host::check_quiet_paths(static_initialization_and_destruction_0);
    return bad;
}
