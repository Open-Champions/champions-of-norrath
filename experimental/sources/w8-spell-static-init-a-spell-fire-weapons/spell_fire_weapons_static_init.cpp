#include "../w8-spell-static-init-a-common/particle_def.hpp"

// _GLOBAL_$I$__16SpellFireWeaponsiP8CreatureP5Skill at 0x2ab528, 32 bytes. It calls the unit-local
// static initializer (0x2ab1b8, 328 bytes) with (1, 0xffff). That initializer
// constructs 3 particle definition records in the order below. Each
// construction passes eight integers, three single floats, three more integers
// and a null callback. The records are 80-byte zero-initialized objects.
using w8_static_init::ParticleDefStorage;
using w8_static_init::construct_particle_def;

namespace {
// Local to the original unit.
ParticleDefStorage fireweaponsParticle1;
ParticleDefStorage fireweaponsParticle2;
ParticleDefStorage fireweaponsParticle3;

void static_initialization_and_destruction_0(int initialize, int priority) {
    if (initialize == 1 && priority == 0xffff) {
        construct_particle_def(fireweaponsParticle1,
            295, 195, 111, 413, 226, 11, 14, -11, 4.0f, 6.0f, 0.0f, 2, 1, 0);
        construct_particle_def(fireweaponsParticle2,
            130, 116, 76, 87, 194, 120, 74, 0, 32.0f, 5.0f, 0.0f, 1, 1, 2);
        construct_particle_def(fireweaponsParticle3,
            0, 0, 0, 64, 34, 30, 21, 0, 3e+01f, 5e+01f, 3.0f, 1, 0, 6);
    }
}
}  // namespace

extern "C" void _GLOBAL_$I$__16SpellFireWeaponsiP8CreatureP5Skill() {
    static_initialization_and_destruction_0(1, 0xffff);
}
