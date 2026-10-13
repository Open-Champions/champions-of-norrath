#include "../w8-spell-static-init-a-common/particle_def.hpp"

// _GLOBAL_$I$msg_run__22ExplodeArrowProjectile (32-byte wrapper at 0x002a2b18 in the verified image). It calls the
// unit-local static initializer at 0x002a25c8 (128 bytes) with (1, 0xffff). That initializer constructs one
// 80-byte particle definition record with a null callback. The record is unit-local zero-initialized storage in
// the image (0x008d7090). The second and third float arguments come from two single-float constants in the
// image's small-data area. What the fields mean is not established here.
using w8_static_init::ParticleDefStorage;
using w8_static_init::construct_particle_def;

namespace {
ParticleDefStorage explodeArrowParticle;

float bits(unsigned value) { return __builtin_bit_cast(float, value); }

void static_initialization_and_destruction_0(int initialize, int priority) {
    if (initialize == 1 && priority == 0xffff) {
        construct_particle_def(explodeArrowParticle,
            193, 112, 39, 61, 54, 51, 33, 14, bits(0x41800000u), bits(0x3e999999u), bits(0xbeccccccu), 1, 2, 2);
    }
}
}  // namespace

extern "C" void _GLOBAL_$I$msg_run__22ExplodeArrowProjectile() {
    static_initialization_and_destruction_0(1, 0xffff);
}
