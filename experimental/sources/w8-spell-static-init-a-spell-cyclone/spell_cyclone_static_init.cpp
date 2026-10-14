#include "../w8-spell-static-init-a-common/particle_def.hpp"

// _GLOBAL_$I$__12SpellCycloneiP8CreatureP5Skill at 0x29a9e0, 32 bytes. It calls the unit-local
// static initializer (0x29a618, 248 bytes) with (1, 0xffff). That initializer
// constructs 2 particle definition records in the order below. Each
// construction passes eight integers, three single floats, three more integers
// and a null callback. The records are 80-byte zero-initialized objects.
using w8_static_init::ParticleDefStorage;
using w8_static_init::construct_particle_def;

namespace {
// Local to the original unit.
ParticleDefStorage swirlParticle;
ParticleDefStorage spark;

void static_initialization_and_destruction_0(int initialize, int priority) {
    if (initialize == 1 && priority == 0xffff) {
        construct_particle_def(swirlParticle,
            35, 37, 20, 52, 129, 83, 55, 9, 47.0f, 3e+01f, -3.3999999f, 0, 1, 6);
        construct_particle_def(spark,
            306, 317, 75, 111, 99, 168, 82, 91, 62.0f, 1.0f, -6.6f, 2, 1, 8);
    }
}
}  // namespace

extern "C" void _GLOBAL_$I$__12SpellCycloneiP8CreatureP5Skill() {
    static_initialization_and_destruction_0(1, 0xffff);
}
