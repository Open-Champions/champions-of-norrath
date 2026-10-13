#include "../w8-spell-static-init-a-common/particle_def.hpp"

// _GLOBAL_$I$getRankDescription__16SkillGroundPoundPUsi (32-byte wrapper in the original). It calls the unit-local
// static initializer (the local text symbol next to it) with (1, 0xffff). That initializer
// constructs 7 particle definition records in the order below. Each
// construction passes eight integers, three single floats, three more integers
// and a null callback. The records are 80-byte zero-initialized objects.
using w8_static_init::ParticleDefStorage;
using w8_static_init::construct_particle_def;

namespace {
// Local to the original unit.
ParticleDefStorage dustParticle;
ParticleDefStorage sparkParticle;
ParticleDefStorage smokeParticle;
ParticleDefStorage splashParticle;
ParticleDefStorage groundpoundParticle0;
ParticleDefStorage groundpoundParticle1;
ParticleDefStorage groundpoundParticle2;

void static_initialization_and_destruction_0(int initialize, int priority) {
    if (initialize == 1 && priority == 0xffff) {
        construct_particle_def(dustParticle,
            74, 80, 141, 119, 44, 55, 297, 0, 21.0f, 0x1.999998p-3f, -6.0f, 3, 1, 9);
        construct_particle_def(sparkParticle,
            283, 164, 79, 127, 148, 100, 52, 0, 22.0f, 1.3f, -6.0f, 3, 1, 8);
        construct_particle_def(smokeParticle,
            87, 58, 31, 191, 9, 9, 9, 0, 26.0f, 55.0f, 1.5f, 3, 0, 6);
        construct_particle_def(splashParticle,
            138, 163, 141, 93, 111, 161, 150, 0, 17.0f, 0x1.999998p-4f, -0x1.b99998p+2f, 3, 0, 6);
        construct_particle_def(groundpoundParticle0,
            255, 0, 0, 255, 255, 0, 0, 255, 8e+01f, 3e+01f, 0.0f, 0, 3, 6);
        construct_particle_def(groundpoundParticle1,
            255, 0, 0, 255, 255, 0, 0, 255, 8e+01f, 3e+01f, 0.0f, 1, 3, 6);
        construct_particle_def(groundpoundParticle2,
            255, 0, 0, 255, 255, 0, 0, 255, 8e+01f, 3e+01f, 0.0f, 2, 3, 6);
    }
}
}  // namespace

extern "C" void _GLOBAL_$I$getRankDescription__16SkillGroundPoundPUsi() {
    static_initialization_and_destruction_0(1, 0xffff);
}
