#include "../w8-spell-static-init-a-common/particle_def.hpp"

// _GLOBAL_$I$__13HammerOfWrathG6Point3sP10GameObjectiP5Skill at 0x2b5878, 32 bytes. It calls the unit-local
// static initializer (0x2b54d0, 320 bytes) with (1, 0xffff). That initializer
// constructs 3 particle definition records in the order below. Each
// construction passes eight integers, three single floats, three more integers
// and a null callback. The records are 80-byte zero-initialized objects.
using w8_static_init::ParticleDefStorage;
using w8_static_init::construct_particle_def;

namespace {
// Local to the original unit.
ParticleDefStorage hammerofwrathParticle1;
ParticleDefStorage hammerofwrathParticle2;
ParticleDefStorage hammerofwrathParticle3;

void static_initialization_and_destruction_0(int initialize, int priority) {
    if (initialize == 1 && priority == 0xffff) {
        construct_particle_def(hammerofwrathParticle1,
            114, 127, 139, 75, 31, 43, 66, 0, 6.1f, 8.0f, 10.299999f, 1, 1, 6);
        construct_particle_def(hammerofwrathParticle2,
            255, 200, 170, 128, 250, 225, 135, 0, 38.0f, 2e+01f, 1.4f, 1, 2, 15);
        construct_particle_def(hammerofwrathParticle3,
            86, 126, 250, 134, 4, 63, 105, 0, 2e+01f, 0.0f, 1.4f, 2, 2, 15);
    }
}
}  // namespace

extern "C" void _GLOBAL_$I$__13HammerOfWrathG6Point3sP10GameObjectiP5Skill() {
    static_initialization_and_destruction_0(1, 0xffff);
}
