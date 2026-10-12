#include "../w8-spell-static-init-a-common/particle_def.hpp"

// _GLOBAL_$I$__17SpellConvertEnemyiP8CreatureP5Skill at 0x298ce0, 32 bytes. It calls the unit-local
// static initializer (0x2989f0, 136 bytes) with (1, 0xffff). That initializer
// constructs 1 particle definition record in the order below. Each
// construction passes eight integers, three single floats, three more integers
// and a null callback. The records are 80-byte zero-initialized objects.
using w8_static_init::ParticleDefStorage;
using w8_static_init::construct_particle_def;

namespace {
// Local to the original unit.
ParticleDefStorage convertP;

void static_initialization_and_destruction_0(int initialize, int priority) {
    if (initialize == 1 && priority == 0xffff) {
        construct_particle_def(convertP,
            280, 186, 56, 125, 15, 19, 211, 63, 7e+01f, 2.0f, 0.39999998f, 3, 1, 9);
    }
}
}  // namespace

extern "C" void _GLOBAL_$I$__17SpellConvertEnemyiP8CreatureP5Skill() {
    static_initialization_and_destruction_0(1, 0xffff);
}
