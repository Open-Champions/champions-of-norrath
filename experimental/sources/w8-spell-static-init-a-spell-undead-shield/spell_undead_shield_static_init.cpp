#include "../w8-spell-static-init-a-common/particle_def.hpp"

// _GLOBAL_$I$__17SpellUndeadShieldiP8CreatureP5Skill at 0x2b1440, 32 bytes. It calls the unit-local
// static initializer (0x2b0fc8, 240 bytes) with (1, 0xffff). That initializer
// constructs 2 particle definition records in the order below. Each
// construction passes eight integers, three single floats, three more integers
// and a null callback. The records are 80-byte zero-initialized objects.
using w8_static_init::ParticleDefStorage;
using w8_static_init::construct_particle_def;

namespace {
// Local to the original unit.
ParticleDefStorage undeadshieldParticle1;
ParticleDefStorage undeadshieldParticle2;

void static_initialization_and_destruction_0(int initialize, int priority) {
    if (initialize == 1 && priority == 0xffff) {
        construct_particle_def(undeadshieldParticle1,
            86, 126, 250, 255, 4, 63, 105, 255, 2e+01f, 0.0f, 1.0f, 0, 2, 15);
        construct_particle_def(undeadshieldParticle2,
            200, 200, 200, 50, 255, 0, 0, 0, 36.0f, -1.0f, 0.0f, 1, 1, 6);
    }
}
}  // namespace

extern "C" void _GLOBAL_$I$__17SpellUndeadShieldiP8CreatureP5Skill() {
    static_initialization_and_destruction_0(1, 0xffff);
}
