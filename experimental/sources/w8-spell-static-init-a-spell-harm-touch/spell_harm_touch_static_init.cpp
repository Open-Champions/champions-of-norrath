#include "../w8-spell-static-init-a-common/particle_def.hpp"

// _GLOBAL_$I$attack__14SpellHarmTouchP10GameObjectPiT2 at 0x29b218, 32 bytes. It calls the unit-local
// static initializer (0x29adb0, 248 bytes) with (1, 0xffff). That initializer
// constructs 2 particle definition records in the order below. Each
// construction passes eight integers, three single floats, three more integers
// and a null callback. The records are 80-byte zero-initialized objects.
using w8_static_init::ParticleDefStorage;
using w8_static_init::construct_particle_def;

namespace {
// Local to the original unit.
ParticleDefStorage harm1;
ParticleDefStorage harm2;

void static_initialization_and_destruction_0(int initialize, int priority) {
    if (initialize == 1 && priority == 0xffff) {
        construct_particle_def(harm1,
            136, 198, 107, 98, 54, 118, 33, 34, 5e+01f, 0.099999994f, 0.049999997f, 1, 1, 8);
        construct_particle_def(harm2,
            163, 127, 92, 135, 59, 7, 3, 58, 1e+01f, 2.0f, 5.0f, 2, 1, 6);
    }
}
}  // namespace

extern "C" void _GLOBAL_$I$attack__14SpellHarmTouchP10GameObjectPiT2() {
    static_initialization_and_destruction_0(1, 0xffff);
}
