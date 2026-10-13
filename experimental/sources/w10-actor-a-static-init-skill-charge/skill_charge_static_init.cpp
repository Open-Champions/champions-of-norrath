#include "../w8-spell-static-init-a-common/particle_def.hpp"

// _GLOBAL_$I$getRankDescription__11SkillChargePUsi (32-byte wrapper in the original). It calls the unit-local
// static initializer (the local text symbol next to it) with (1, 0xffff). That initializer
// constructs 3 particle definition records in the order below. Each
// construction passes eight integers, three single floats, three more integers
// and a null callback. The records are 80-byte zero-initialized objects.
using w8_static_init::ParticleDefStorage;
using w8_static_init::construct_particle_def;

namespace {
// Local to the original unit.
ParticleDefStorage chargeParticle;
ParticleDefStorage chargeHeat;
ParticleDefStorage chargeHit;

void static_initialization_and_destruction_0(int initialize, int priority) {
    if (initialize == 1 && priority == 0xffff) {
        construct_particle_def(chargeParticle,
            112, 133, 134, 127, 59, 32, 207, 116, 22.0f, 0x1.999998p-3f, 0.5f, 2, 1, 8);
        construct_particle_def(chargeHeat,
            35, 17, 114, 124, 55, 58, 99, 166, 92.0f, 0.9f, 0x1.333332p-3f, 2, 3, 8);
        construct_particle_def(chargeHit,
            112, 133, 207, 127, 207, 32, 32, 116, 22.0f, 0.0f, -0.5f, 2, 1, 8);
    }
}
}  // namespace

extern "C" void _GLOBAL_$I$getRankDescription__11SkillChargePUsi() {
    static_initialization_and_destruction_0(1, 0xffff);
}
