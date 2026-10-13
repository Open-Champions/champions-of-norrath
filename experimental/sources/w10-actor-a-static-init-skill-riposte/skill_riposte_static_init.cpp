#include "../w8-spell-static-init-a-common/particle_def.hpp"

// _GLOBAL_$I$getRankDescription__12SkillRipostePUsi (32-byte wrapper in the original). It calls the unit-local
// static initializer (the local text symbol next to it) with (1, 0xffff). That initializer
// constructs 2 particle definition records in the order below. Each
// construction passes eight integers, three single floats, three more integers
// and a null callback. The records are 80-byte zero-initialized objects.
using w8_static_init::ParticleDefStorage;
using w8_static_init::construct_particle_def;

namespace {
// Local to the original unit.
ParticleDefStorage riposteParticle1;
ParticleDefStorage riposteParticle2;

void static_initialization_and_destruction_0(int initialize, int priority) {
    if (initialize == 1 && priority == 0xffff) {
        construct_particle_def(riposteParticle1,
            150, 150, 150, 150, 255, 0, 0, 0, 24.0f, 0.0f, 0.0f, 1, 1, 6);
        construct_particle_def(riposteParticle2,
            200, 200, 200, 50, 255, 0, 0, 0, 36.0f, -1.0f, 0.0f, 1, 1, 6);
    }
}
}  // namespace

extern "C" void _GLOBAL_$I$getRankDescription__12SkillRipostePUsi() {
    static_initialization_and_destruction_0(1, 0xffff);
}
