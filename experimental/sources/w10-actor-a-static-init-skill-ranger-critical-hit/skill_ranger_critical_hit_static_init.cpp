#include "../w8-spell-static-init-a-common/particle_def.hpp"

// _GLOBAL_$I$getRankDescription__22SkillRangerCriticalHitPUsi (32-byte wrapper in the original). It calls the unit-local
// static initializer (the local text symbol next to it) with (1, 0xffff). That initializer
// constructs 1 particle definition record in the order below. Each
// construction passes eight integers, three single floats, three more integers
// and a null callback. The records are 80-byte zero-initialized objects.
using w8_static_init::ParticleDefStorage;
using w8_static_init::construct_particle_def;

namespace {
// Local to the original unit.
ParticleDefStorage aura;

void static_initialization_and_destruction_0(int initialize, int priority) {
    if (initialize == 1 && priority == 0xffff) {
        construct_particle_def(aura,
            107, 210, 209, 140, 198, 199, 244, 147, 2.0f, 1.0f, -2.0f, 2, 1, 5);
    }
}
}  // namespace

extern "C" void _GLOBAL_$I$getRankDescription__22SkillRangerCriticalHitPUsi() {
    static_initialization_and_destruction_0(1, 0xffff);
}
