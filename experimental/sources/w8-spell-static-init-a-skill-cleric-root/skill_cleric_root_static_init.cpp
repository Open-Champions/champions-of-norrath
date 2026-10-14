#include "../w8-spell-static-init-a-common/particle_def.hpp"

// _GLOBAL_$I$cast__15SkillClericRootiP8CreatureG6Point3s at 0x2c6440, 32 bytes. It calls the unit-local
// static initializer (0x2c6008, 336 bytes) with (1, 0xffff). That initializer
// constructs 3 particle definition records in the order below. Each
// construction passes eight integers, three single floats, three more integers
// and a null callback. The records are 80-byte zero-initialized objects.
using w8_static_init::ParticleDefStorage;
using w8_static_init::construct_particle_def;

namespace {
// Local to the original unit.
ParticleDefStorage clericroots;
ParticleDefStorage magic;
ParticleDefStorage miss;

void static_initialization_and_destruction_0(int initialize, int priority) {
    if (initialize == 1 && priority == 0xffff) {
        construct_particle_def(clericroots,
            2, 85, 2, 114, 0, 80, 0, 99, 12.0f, 11.0f, -1.5999999f, 3, 2, 2);
        construct_particle_def(magic,
            42, 52, 217, 50, 255, 285, 0, 43, 28.0f, 0.0f, 24.0f, 1, 1, 8);
        construct_particle_def(miss,
            105, 100, 2, 139, 121, 23, 0, 0, 12.0f, 12.0f, -1.5999999f, 1, 2, 8);
    }
}
}  // namespace

extern "C" void _GLOBAL_$I$cast__15SkillClericRootiP8CreatureG6Point3s() {
    static_initialization_and_destruction_0(1, 0xffff);
}
