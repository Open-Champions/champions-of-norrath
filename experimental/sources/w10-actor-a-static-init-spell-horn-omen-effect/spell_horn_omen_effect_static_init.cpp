#include "../w8-spell-static-init-a-common/particle_def.hpp"

// _GLOBAL_$I$run__19SpellHornOmenEffectG6Point3s (32-byte wrapper in the original). It calls the unit-local
// static initializer (the local text symbol next to it) with (1, 0xffff). That initializer
// constructs 1 particle definition record in the order below. Each
// construction passes eight integers, three single floats, three more integers
// and a null callback. The records are 80-byte zero-initialized objects.
using w8_static_init::ParticleDefStorage;
using w8_static_init::construct_particle_def;

namespace {
// Local to the original unit.
ParticleDefStorage hornomenParticle1;

void static_initialization_and_destruction_0(int initialize, int priority) {
    if (initialize == 1 && priority == 0xffff) {
        construct_particle_def(hornomenParticle1,
            225, 20, 0, 255, 115, 10, 0, 255, 2e+01f, 0.0f, 1.0f, 0, 2, 15);
    }
}
}  // namespace

extern "C" void _GLOBAL_$I$run__19SpellHornOmenEffectG6Point3s() {
    static_initialization_and_destruction_0(1, 0xffff);
}
