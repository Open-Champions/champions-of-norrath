#include "../w8-spell-static-init-a-common/particle_def.hpp"

// _GLOBAL_$I$msg_draw__10FrostShard (32-byte wrapper at 0x002be198 in the verified image). It calls the
// unit-local static initializer at 0x002bdc70 (324 bytes) with (1, 0xffff). That initializer constructs three
// 80-byte particle definition records, in the order below, each time with a null callback. The records are
// unit-local zero-initialized storage in the image (symbols shockoffrostParticle1..3, three consecutive
// 80-byte slots from 0x008d80a0). Rows two and three take their third float from two adjacent words of the
// image's small-constant area; both hold the bit pattern 0xc104cccc.
// Argument rows were read from the original's code and then compared under a private paired run.
// What the fields mean is not established here.
using w8_static_init::ParticleDefStorage;
using w8_static_init::construct_particle_def;

namespace {
ParticleDefStorage shockoffrostParticle1;
ParticleDefStorage shockoffrostParticle2;
ParticleDefStorage shockoffrostParticle3;

float bits(unsigned value) { return __builtin_bit_cast(float, value); }

void static_initialization_and_destruction_0(int initialize, int priority) {
    if (initialize == 1 && priority == 0xffff) {
        construct_particle_def(shockoffrostParticle1,
            100, 210, 235, 140, 198, 199, 244, 140, bits(0x40800000u), bits(0x00000000u), bits(0xc0000000u), 1, 1, 5);
        construct_particle_def(shockoffrostParticle2,
            100, 139, 255, 255, 198, 199, 244, 0, bits(0x40e00000u), bits(0x40a00000u), bits(0xc104ccccu), 1, 0, 5);
        construct_particle_def(shockoffrostParticle3,
            255, 255, 255, 255, 150, 150, 150, 0, bits(0x40e00000u), bits(0x40a00000u), bits(0xc104ccccu), 1, 1, 5);
    }
}
}  // namespace

extern "C" void _GLOBAL_$I$msg_draw__10FrostShard() {
    static_initialization_and_destruction_0(1, 0xffff);
}
