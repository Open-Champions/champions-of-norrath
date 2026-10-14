#include "../w8-spell-static-init-a-common/particle_def.hpp"

// _GLOBAL_$I$getRankDescription__10SkillShockPUsi (32-byte wrapper in the original). It calls the unit-local
// static initializer (the local text symbol next to it) with (1, 0xffff). That initializer
// constructs 2 particle definition records in the order below. Each
// construction passes eight integers, three single floats, three more integers
// and a null callback. The records are 80-byte zero-initialized objects.
using w8_static_init::ParticleDefStorage;
using w8_static_init::construct_particle_def;

namespace {
// Local to the original unit.
ParticleDefStorage ballParticle;
ParticleDefStorage ballSmokeParticle;

void static_initialization_and_destruction_0(int initialize, int priority) {
    if (initialize == 1 && priority == 0xffff) {
        construct_particle_def(ballParticle,
            164, 148, 343, 90, 111, 117, 216, 0, 6.0f, 3.0f, 0.03f, 0, 1, 4);
        construct_particle_def(ballSmokeParticle,
            111, 112, 154, 80, 33, 30, 28, 0, 8.0f, 75.0f, 0x1.999998p-4f, 2, 0, 6);
    }
}
}  // namespace

extern "C" void _GLOBAL_$I$getRankDescription__10SkillShockPUsi() {
    static_initialization_and_destruction_0(1, 0xffff);
}
