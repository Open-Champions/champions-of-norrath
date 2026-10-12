#include "../w8-spell-static-init-a-common/particle_def.hpp"

// _GLOBAL_$I$__19ColdArrowProjectileG6Point3sP10GameObjectiP5Skill at 0x29fe00, 32 bytes. It calls the unit-local
// static initializer (0x29f9f8, 328 bytes) with (1, 0xffff). That initializer
// constructs 3 particle definition records in the order below. Each
// construction passes eight integers, three single floats, three more integers
// and a null callback. The records are 80-byte zero-initialized objects.
using w8_static_init::ParticleDefStorage;
using w8_static_init::construct_particle_def;

namespace {
// Local to the original unit.
ParticleDefStorage coldSmoke;
ParticleDefStorage coldSpeck;
ParticleDefStorage pack;

void static_initialization_and_destruction_0(int initialize, int priority) {
    if (initialize == 1 && priority == 0xffff) {
        construct_particle_def(coldSmoke,
            65, 65, 144, 64, 28, 39, 49, 0, 23.0f, 72.2f, -5.5f, 1, 1, 6);
        construct_particle_def(coldSpeck,
            77, 89, 115, 129, 88, 74, 115, 128, 6.0f, 2.1999998f, -5.1f, 3, 0, 5);
        construct_particle_def(pack,
            111, 119, 215, 128, 76, 86, 210, 50, 5.0f, 0.049999997f, -0.099999994f, 3, 0, 4);
    }
}
}  // namespace

extern "C" void _GLOBAL_$I$__19ColdArrowProjectileG6Point3sP10GameObjectiP5Skill() {
    static_initialization_and_destruction_0(1, 0xffff);
}
