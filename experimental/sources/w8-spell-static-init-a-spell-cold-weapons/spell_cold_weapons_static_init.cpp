#include "../w8-spell-static-init-a-common/particle_def.hpp"

// _GLOBAL_$I$__16SpellColdWeaponsiP8CreatureP5Skill at 0x2ac4d0, 32 bytes. It calls the unit-local
// static initializer (0x2ac240, 240 bytes) with (1, 0xffff). That initializer
// constructs 2 particle definition records in the order below. Each
// construction passes eight integers, three single floats, three more integers
// and a null callback. The records are 80-byte zero-initialized objects.
using w8_static_init::ParticleDefStorage;
using w8_static_init::construct_particle_def;

namespace {
// Local to the original unit.
ParticleDefStorage coldweaponsParticle1;
ParticleDefStorage coldweaponsParticle2;

void static_initialization_and_destruction_0(int initialize, int priority) {
    if (initialize == 1 && priority == 0xffff) {
        construct_particle_def(coldweaponsParticle1,
            100, 210, 235, 140, 198, 199, 244, 140, 4.0f, 0.0f, -2.0f, 1, 1, 5);
        construct_particle_def(coldweaponsParticle2,
            100, 139, 255, 255, 198, 199, 244, 255, 7.0f, 0.0f, -8.299999f, 1, 0, 5);
    }
}
}  // namespace

extern "C" void _GLOBAL_$I$__16SpellColdWeaponsiP8CreatureP5Skill() {
    static_initialization_and_destruction_0(1, 0xffff);
}
