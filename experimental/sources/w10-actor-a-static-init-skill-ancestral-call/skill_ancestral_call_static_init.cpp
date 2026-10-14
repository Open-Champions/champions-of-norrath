#include "../w8-spell-static-init-a-common/particle_def.hpp"

// _GLOBAL_$I$getRankDescription__18SkillAncestralCallPUsi (32-byte wrapper in the original). It calls the unit-local
// static initializer (the local text symbol next to it) with (1, 0xffff). That initializer
// constructs 1 particle definition record in the order below. Each
// construction passes eight integers, three single floats, three more integers
// and a null callback. The records are 80-byte zero-initialized objects.
using w8_static_init::ParticleDefStorage;
using w8_static_init::construct_particle_def;

namespace {
// Local to the original unit.
ParticleDefStorage callParticle;

void static_initialization_and_destruction_0(int initialize, int priority) {
    if (initialize == 1 && priority == 0xffff) {
        construct_particle_def(callParticle,
            74, 72, 166, 72, 48, 9, 299, 64, 6.5f, 0.0f, 0.0f, 2, 1, 7);
    }
}
}  // namespace

extern "C" void _GLOBAL_$I$getRankDescription__18SkillAncestralCallPUsi() {
    static_initialization_and_destruction_0(1, 0xffff);
}
