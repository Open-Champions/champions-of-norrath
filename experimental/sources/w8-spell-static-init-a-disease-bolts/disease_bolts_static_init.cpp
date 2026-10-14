#include "../w8-spell-static-init-a-common/particle_def.hpp"

// _GLOBAL_$I$__11DiseaseBoltsiG6Point3P10GameObjectP5Skill at 0x2938a8, 32 bytes. It calls the unit-local
// static initializer (0x2934d0, 344 bytes) with (1, 0xffff). That initializer
// constructs 3 particle definition records in the order below. Each
// construction passes eight integers, three single floats, three more integers
// and a null callback. The records are 80-byte zero-initialized objects.
using w8_static_init::ParticleDefStorage;
using w8_static_init::construct_particle_def;

namespace {
// Local to the original unit.
ParticleDefStorage diseaseParticle1;
ParticleDefStorage diseaseParticle2;
ParticleDefStorage diseaseParticle3;

void static_initialization_and_destruction_0(int initialize, int priority) {
    if (initialize == 1 && priority == 0xffff) {
        construct_particle_def(diseaseParticle1,
            110, 100, 23, 241, 8, 40, 16, 5, 35.0f, 16.0f, 0.0f, 0, 0, 6);
        construct_particle_def(diseaseParticle2,
            78, 80, 30, 50, 78, 66, 30, 5, 28.0f, 5e+01f, -2.0f, 2, 0, 9);
        construct_particle_def(diseaseParticle3,
            0, 0, 0, 72, 36, 25, 20, 125, 12.0f, 1.0f, 0.0f, 2, 0, 9);
    }
}
}  // namespace

extern "C" void _GLOBAL_$I$__11DiseaseBoltsiG6Point3P10GameObjectP5Skill() {
    static_initialization_and_destruction_0(1, 0xffff);
}
