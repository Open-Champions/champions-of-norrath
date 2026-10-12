#include "../w8-spell-static-init-a-common/particle_def.hpp"

// _GLOBAL_$I$__10StaticFireG6Point3fif at 0x28f5e8, 32 bytes. It calls the unit-local
// static initializer (0x28f348, 328 bytes) with (1, 0xffff). That initializer
// constructs 3 particle definition records in the order below. Each
// construction passes eight integers, three single floats, three more integers
// and a null callback. The records are 80-byte zero-initialized objects.
using w8_static_init::ParticleDefStorage;
using w8_static_init::construct_particle_def;

namespace {
// Local to the original unit.
ParticleDefStorage fireParticle;
ParticleDefStorage fireParticleCut;
ParticleDefStorage sparkParticle;

void static_initialization_and_destruction_0(int initialize, int priority) {
    if (initialize == 1 && priority == 0xffff) {
        construct_particle_def(fireParticle,
            300, 200, 48, 88, 244, 56, 4, 26, 26.0f, 11.0f, 2.3f, 1, 1, 0);
        construct_particle_def(fireParticleCut,
            136, 131, 92, 136, 391, 46, 67, 0, 25.0f, 5.0f, 0.5f, 1, 1, 9);
        construct_particle_def(sparkParticle,
            375, 267, 92, 128, 55, 55, 4, 0, 5.0f, 8.0f, -0.36999997f, 2, 2, 0);
    }
}
}  // namespace

extern "C" void _GLOBAL_$I$__10StaticFireG6Point3fif() {
    static_initialization_and_destruction_0(1, 0xffff);
}
