#include "../w8-spell-static-init-a-common/particle_def.hpp"

// _GLOBAL_$I$msg_run__13DiseaseCorpse (32-byte wrapper at 0x00297638 in the verified image). It calls the
// unit-local static initializer at 0x00297370 (328 bytes) with (1, 0xffff). That initializer constructs three
// 80-byte particle definition records, in the order below, each time with a null callback. The records are
// unit-local zero-initialized storage in the image (three consecutive 80-byte slots from 0x008d6a50).
// Argument rows were read from the original's code and then compared under a private paired run.
// What the fields mean is not established here.
using w8_static_init::ParticleDefStorage;
using w8_static_init::construct_particle_def;

namespace {
ParticleDefStorage diseaseCorpseParticle1;
ParticleDefStorage diseaseCorpseParticle2;
ParticleDefStorage diseaseCorpseParticle3;

float bits(unsigned value) { return __builtin_bit_cast(float, value); }

void static_initialization_and_destruction_0(int initialize, int priority) {
    if (initialize == 1 && priority == 0xffff) {
        construct_particle_def(diseaseCorpseParticle1,
            98, 121, 66, 50, 63, 85, 30, 0, bits(0x41700000u), bits(0x41a00000u), bits(0xc0000000u), 2, 0, 2);
        construct_particle_def(diseaseCorpseParticle2,
            125, 179, 15, 50, 65, 116, 0, 0, bits(0x41700000u), bits(0x41a00000u), bits(0xc0000000u), 2, 0, 2);
        construct_particle_def(diseaseCorpseParticle3,
            0, 0, 0, 54, 25, 25, 25, 0, bits(0x41200000u), bits(0x41200000u), bits(0x00000000u), 2, 0, 9);
    }
}
}  // namespace

extern "C" void _GLOBAL_$I$msg_run__13DiseaseCorpse() {
    static_initialization_and_destruction_0(1, 0xffff);
}
