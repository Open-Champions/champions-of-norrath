#include "../w8-spell-static-init-a-common/particle_def.hpp"

// _GLOBAL_$I$__12SpellBlessediP8CreatureP5Skill at 0x2a7048, 32 bytes. It calls the unit-local
// static initializer (0x2a6c30, 240 bytes) with (1, 0xffff). That initializer
// constructs 2 particle definition records in the order below. Each
// construction passes eight integers, three single floats, three more integers
// and a null callback. The records are 80-byte zero-initialized objects.
using w8_static_init::ParticleDefStorage;
using w8_static_init::construct_particle_def;

namespace {
// Local to the original unit.
ParticleDefStorage blessedParticle1;
ParticleDefStorage blessedParticle2;

void static_initialization_and_destruction_0(int initialize, int priority) {
    if (initialize == 1 && priority == 0xffff) {
        construct_particle_def(blessedParticle1,
            150, 150, 150, 150, 105, 105, 253, 0, 7.0f, 0.0f, 0.0f, 3, 0, 8);
        construct_particle_def(blessedParticle2,
            0, 0, 255, 190, 150, 150, 255, 0, 3e+01f, 0.0f, -4.5f, 2, 2, 8);
    }
}
}  // namespace

extern "C" void _GLOBAL_$I$__12SpellBlessediP8CreatureP5Skill() {
    static_initialization_and_destruction_0(1, 0xffff);
}
