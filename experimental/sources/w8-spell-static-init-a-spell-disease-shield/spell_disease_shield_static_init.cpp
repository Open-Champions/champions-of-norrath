#include "../w8-spell-static-init-a-common/particle_def.hpp"

// _GLOBAL_$I$__18SpellDiseaseShieldiP8CreatureP5Skill at 0x296910, 32 bytes. It calls the unit-local
// static initializer (0x296668, 248 bytes) with (1, 0xffff). That initializer
// constructs 2 particle definition records in the order below. Each
// construction passes eight integers, three single floats, three more integers
// and a null callback. The records are 80-byte zero-initialized objects.
using w8_static_init::ParticleDefStorage;
using w8_static_init::construct_particle_def;

namespace {
// Local to the original unit.
ParticleDefStorage diseaseshieldParticle1;
ParticleDefStorage diseaseshieldParticle2;

void static_initialization_and_destruction_0(int initialize, int priority) {
    if (initialize == 1 && priority == 0xffff) {
        construct_particle_def(diseaseshieldParticle1,
            110, 100, 23, 100, 78, 66, 30, 5, 5e+01f, 6e+01f, 0.0f, 2, 0, 2);
        construct_particle_def(diseaseshieldParticle2,
            12, 12, 0, 78, 8, 40, 16, 5, 25.0f, 2e+01f, 0.0f, 3, 0, 8);
    }
}
}  // namespace

extern "C" void _GLOBAL_$I$__18SpellDiseaseShieldiP8CreatureP5Skill() {
    static_initialization_and_destruction_0(1, 0xffff);
}
