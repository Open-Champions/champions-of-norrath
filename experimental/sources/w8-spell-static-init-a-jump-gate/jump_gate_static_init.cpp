#include "../w8-spell-static-init-a-common/particle_def.hpp"

// _GLOBAL_$I$__8JumpGateG6Point3sPPc at 0x2848f8, 32 bytes. It calls the unit-local
// static initializer (0x2842f8, 344 bytes) with (1, 0xffff). That initializer
// constructs 3 particle definition records in the order below. Each
// construction passes eight integers, three single floats, three more integers
// and a null callback. The records are 80-byte zero-initialized objects.
using w8_static_init::ParticleDefStorage;
using w8_static_init::construct_particle_def;

namespace {
// Local to the original unit.
ParticleDefStorage jumpParticleOn;
ParticleDefStorage jumpParticleJump;
ParticleDefStorage jumpParticle2;

void static_initialization_and_destruction_0(int initialize, int priority) {
    if (initialize == 1 && priority == 0xffff) {
        construct_particle_def(jumpParticleOn,
            151, 17, 47, 124, 55, 129, 99, 166, 18.0f, 0.0f, -1.0f, 2, 1, 8);
        construct_particle_def(jumpParticleJump,
            52, 74, 259, 132, 34, 87, 40, 0, 8.0f, 14.0f, 1.0f, 3, 1, 1);
        construct_particle_def(jumpParticle2,
            151, 17, 47, 124, 55, 129, 99, 166, 18.0f, 0.0f, 2.0f, 2, 1, 8);
    }
}
}  // namespace

extern "C" void _GLOBAL_$I$__8JumpGateG6Point3sPPc() {
    static_initialization_and_destruction_0(1, 0xffff);
}
