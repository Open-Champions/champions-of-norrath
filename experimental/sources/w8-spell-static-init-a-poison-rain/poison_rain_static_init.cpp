#include "../w8-spell-static-init-a-common/particle_def.hpp"

// _GLOBAL_$I$__10PoisonRainiP10GameObjectG6Point3P5SkillP15AnimationHeader at 0x2bf270, 32 bytes. It calls the unit-local
// static initializer (0x2bf0b0, 232 bytes) with (1, 0xffff). That initializer
// constructs 2 particle definition records in the order below. Each
// construction passes eight integers, three single floats, three more integers
// and a null callback. The records are 80-byte zero-initialized objects.
using w8_static_init::ParticleDefStorage;
using w8_static_init::construct_particle_def;

namespace {
// Local to the original unit.
ParticleDefStorage poisonrainParticle1;
ParticleDefStorage poisonrainParticle2;

void static_initialization_and_destruction_0(int initialize, int priority) {
    if (initialize == 1 && priority == 0xffff) {
        construct_particle_def(poisonrainParticle1,
            20, 80, 20, 0, 20, 80, 20, 175, 4.838f, 0.0f, 0.0f, 1, 2, 4);
        construct_particle_def(poisonrainParticle2,
            50, 200, 50, 100, 100, 250, 100, 0, 1.0f, 1e+01f, 0.0f, 1, 1, 7);
    }
}
}  // namespace

extern "C" void _GLOBAL_$I$__10PoisonRainiP10GameObjectG6Point3P5SkillP15AnimationHeader() {
    static_initialization_and_destruction_0(1, 0xffff);
}
