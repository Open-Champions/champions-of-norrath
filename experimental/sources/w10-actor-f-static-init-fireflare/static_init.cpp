#include "../w8-spell-static-init-a-common/particle_def.hpp"

// _GLOBAL_$I$msg_run__9FireFlare (32-byte wrapper at 0x002a49d8 in the verified image). It calls the
// unit-local static initializer at 0x002a4528 (328 bytes) with (1, 0xffff). That initializer constructs three
// 80-byte particle definition records, in the order below, each time with a null callback. The records are
// unit-local zero-initialized storage in the image (symbols fireflaresParticle1..3, three consecutive
// 80-byte slots from 0x008d70e0).
// Argument rows were read from the original's code and then compared under a private paired run.
// What the fields mean is not established here.
using w8_static_init::ParticleDefStorage;
using w8_static_init::construct_particle_def;

namespace {
ParticleDefStorage fireflaresParticle1;
ParticleDefStorage fireflaresParticle2;
ParticleDefStorage fireflaresParticle3;

float bits(unsigned value) { return __builtin_bit_cast(float, value); }

void static_initialization_and_destruction_0(int initialize, int priority) {
    if (initialize == 1 && priority == 0xffff) {
        construct_particle_def(fireflaresParticle1,
            295, 195, 111, 413, 226, 11, 14, -11, bits(0x40800000u), bits(0x40c00000u), bits(0x00000000u), 2, 1, 0);
        construct_particle_def(fireflaresParticle2,
            130, 116, 76, 87, 194, 120, 74, 0, bits(0x42000000u), bits(0x40a00000u), bits(0x00000000u), 1, 1, 2);
        construct_particle_def(fireflaresParticle3,
            0, 0, 0, 64, 34, 30, 21, 0, bits(0x41f00000u), bits(0x42480000u), bits(0x40400000u), 1, 0, 6);
    }
}
}  // namespace

extern "C" void _GLOBAL_$I$msg_run__9FireFlare() {
    static_initialization_and_destruction_0(1, 0xffff);
}
