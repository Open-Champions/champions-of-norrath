#include "../w8-spell-static-init-a-common/particle_def.hpp"

// _GLOBAL_$I$__16SpellConeOfFrostiP8CreatureP5Skill at 0x2aff88, 32 bytes. It calls the unit-local
// static initializer (0x2afb58, 328 bytes) with (1, 0xffff). That initializer
// constructs 3 particle definition records in the order below. Each
// construction passes eight integers, three single floats, three more integers
// and a null callback. The records are 80-byte zero-initialized objects.
using w8_static_init::ParticleDefStorage;
using w8_static_init::construct_particle_def;

namespace {
// Local to the original unit.
ParticleDefStorage primary;
ParticleDefStorage puff;
ParticleDefStorage ice;

void static_initialization_and_destruction_0(int initialize, int priority) {
    if (initialize == 1 && priority == 0xffff) {
        construct_particle_def(primary,
            36, 30, 150, 51, 103, 123, 143, 0, 33.0f, 25.0f, -1.03f, 1, 1, 5);
        construct_particle_def(puff,
            110, 108, 105, 123, 63, 56, 141, 0, 54.0f, 7e+01f, 0.06999999f, 1, 0, 6);
        construct_particle_def(ice,
            100, 154, 238, 103, 120, 117, 114, 4, 8.0f, 8.0f, -6.0299997f, 2, 0, 8);
    }
}
}  // namespace

extern "C" void _GLOBAL_$I$__16SpellConeOfFrostiP8CreatureP5Skill() {
    static_initialization_and_destruction_0(1, 0xffff);
}
