#include "../w8-spell-static-init-a-common/particle_def.hpp"

// _GLOBAL_$I$msg_run__12DiseaseTrail (32-byte wrapper at 0x002955a0 in the verified image). It calls the
// unit-local static initializer at 0x00295168 (332 bytes) with (1, 0xffff). That initializer constructs three
// 80-byte particle definition records, in the order below, each time with a null callback. The records are
// unit-local zero-initialized storage in the image (three consecutive 80-byte slots from 0x008d68c0).
// Argument rows were read from the original's code and then compared under a private paired run.
// What the fields mean is not established here.
using w8_static_init::ParticleDefStorage;
using w8_static_init::construct_particle_def;

namespace {
ParticleDefStorage diseaseTrailParticle1;
ParticleDefStorage diseaseTrailParticle2;
ParticleDefStorage diseaseTrailParticle3;

float bits(unsigned value) { return __builtin_bit_cast(float, value); }

void static_initialization_and_destruction_0(int initialize, int priority) {
    if (initialize == 1 && priority == 0xffff) {
        construct_particle_def(diseaseTrailParticle1,
            110, 100, 23, 100, 78, 66, 30, 5, bits(0x420c0000u), bits(0x42340000u), bits(0x00000000u), 2, 0, 2);
        construct_particle_def(diseaseTrailParticle2,
            12, 12, 0, 78, 8, 40, 16, 5, bits(0x41a00000u), bits(0x41700000u), bits(0x00000000u), 3, 0, 8);
        construct_particle_def(diseaseTrailParticle3,
            0, 0, 0, 254, 25, 25, 25, 0, bits(0x40a00000u), bits(0x40a00000u), bits(0x00000000u), 2, 0, 9);
    }
}
}  // namespace

extern "C" void _GLOBAL_$I$msg_run__12DiseaseTrail() {
    static_initialization_and_destruction_0(1, 0xffff);
}
