#include "../w8-spell-static-init-a-common/particle_def.hpp"

// _GLOBAL_$I$__8HolyBoltG6Point3iiP8CreatureP5Skill at 0x2bbd58, 32 bytes. It calls the unit-local
// static initializer (0x2bba08, 528 bytes) with (1, 0xffff). That initializer
// constructs 5 particle definition records in the order below. Each
// construction passes eight integers, three single floats, three more integers
// and a null callback. The records are 80-byte zero-initialized objects.
using w8_static_init::ParticleDefStorage;
using w8_static_init::construct_particle_def;

namespace {
// Local to the original unit.
ParticleDefStorage dismissundeadParticle1;
ParticleDefStorage dismissundeadParticle2;
ParticleDefStorage dismissundeadParticle3;
ParticleDefStorage skullHitParticle;
ParticleDefStorage skullHitParticleLong;

void static_initialization_and_destruction_0(int initialize, int priority) {
    if (initialize == 1 && priority == 0xffff) {
        construct_particle_def(dismissundeadParticle1,
            250, 200, 100, 255, 0, 0, 0, 0, 4e+01f, 1e+01f, -5.0f, 1, 1, 9);
        construct_particle_def(dismissundeadParticle2,
            255, 200, 170, 128, 250, 225, 135, 0, 38.0f, 2e+01f, 1.4f, 1, 2, 15);
        construct_particle_def(dismissundeadParticle3,
            86, 126, 250, 134, 4, 63, 105, 0, 2e+01f, 0.0f, 1.4f, 2, 2, 15);
        construct_particle_def(skullHitParticle,
            86, 126, 250, 255, 0, 0, 0, 0, 6e+01f, 1e+01f, -15.0f, 0, 1, 48);
        construct_particle_def(skullHitParticleLong,
            86, 126, 250, 255, 0, 0, 0, 0, 6e+01f, 1e+01f, -15.0f, 1, 1, 48);
    }
}
}  // namespace

extern "C" void _GLOBAL_$I$__8HolyBoltG6Point3iiP8CreatureP5Skill() {
    static_initialization_and_destruction_0(1, 0xffff);
}
