#include "../w8-spell-static-init-a-common/particle_def.hpp"

// _GLOBAL_$I$cast__19SkillSummonSkeletoniP8CreatureG6Point3s (32-byte wrapper in the original). It calls the unit-local
// static initializer (the local text symbol next to it) with (1, 0xffff). That initializer
// constructs 2 particle definition records in the order below. Each
// construction passes eight integers, three single floats, three more integers
// and a null callback. The records are 80-byte zero-initialized objects.
using w8_static_init::ParticleDefStorage;
using w8_static_init::construct_particle_def;

namespace {
// Local to the original unit.
ParticleDefStorage summonskeletonParticle1;
ParticleDefStorage summonskeletonParticle2;

void static_initialization_and_destruction_0(int initialize, int priority) {
    if (initialize == 1 && priority == 0xffff) {
        construct_particle_def(summonskeletonParticle1,
            22, 21, 6, 200, 20, 19, 6, 0, 1e+02f, -3.0f, 0.0f, 1, 0, 4);
        construct_particle_def(summonskeletonParticle2,
            50, 50, 60, 255, 0, 0, 0, 255, 5e+01f, 5e+01f, 0.0f, 1, 2, 6);
    }
}
}  // namespace

extern "C" void _GLOBAL_$I$cast__19SkillSummonSkeletoniP8CreatureG6Point3s() {
    static_initialization_and_destruction_0(1, 0xffff);
}
