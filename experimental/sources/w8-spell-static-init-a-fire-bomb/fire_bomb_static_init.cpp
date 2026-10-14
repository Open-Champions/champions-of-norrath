#include "../w8-spell-static-init-a-common/particle_def.hpp"

// _GLOBAL_$I$__8FireBombG6Point3sPPc at 0x28b838, 32 bytes. It calls the unit-local
// static initializer (0x28b718, 136 bytes) with (1, 0xffff). That initializer
// constructs 1 particle definition record in the order below. Each
// construction passes eight integers, three single floats, three more integers
// and a null callback. The records are 80-byte zero-initialized objects.
using w8_static_init::ParticleDefStorage;
using w8_static_init::construct_particle_def;

namespace {
// Local to the original unit.
ParticleDefStorage sparkParticle;

void static_initialization_and_destruction_0(int initialize, int priority) {
    if (initialize == 1 && priority == 0xffff) {
        construct_particle_def(sparkParticle,
            295, 195, 111, 149, 226, 11, 14, 24, 4.0f, 6.0f, -3.3f, 2, 1, 0);
    }
}
}  // namespace

extern "C" void _GLOBAL_$I$__8FireBombG6Point3sPPc() {
    static_initialization_and_destruction_0(1, 0xffff);
}
