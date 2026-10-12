#include "../w8-spell-static-init-a-common/particle_def.hpp"

// _GLOBAL_$I$cast__14SkillMultiFireiP8CreatureG6Point3s at 0x2a1e58, 32 bytes. It calls the unit-local
// static initializer (0x2a1b30, 128 bytes) with (1, 0xffff). That initializer
// constructs 1 particle definition record in the order below. Each
// construction passes eight integers, three single floats, three more integers
// and a null callback. The records are 80-byte zero-initialized objects.
using w8_static_init::ParticleDefStorage;
using w8_static_init::construct_particle_def;

namespace {
// Local to the original unit.
ParticleDefStorage trail;

void static_initialization_and_destruction_0(int initialize, int priority) {
    if (initialize == 1 && priority == 0xffff) {
        construct_particle_def(trail,
            114, 132, 215, 28, 80, 45, 170, 0, 9.799999f, 3.6f, -3.0f, 1, 2, 4);
    }
}
}  // namespace

extern "C" void _GLOBAL_$I$cast__14SkillMultiFireiP8CreatureG6Point3s() {
    static_initialization_and_destruction_0(1, 0xffff);
}
