#include "../w8-spell-static-init-a-common/particle_def.hpp"

// _GLOBAL_$I$__3NPCG6Point3sPcN33ifPPc at 0x233280, 32 bytes. It calls the unit-local
// static initializer (0x233110, 136 bytes) with (1, 0xffff). That initializer
// constructs 1 particle definition record in the order below. Each
// construction passes eight integers, three single floats, three more integers
// and a null callback. The records are 80-byte zero-initialized objects.
using w8_static_init::ParticleDefStorage;
using w8_static_init::construct_particle_def;

namespace {
// Local to the original unit.
ParticleDefStorage kingParticle;

void static_initialization_and_destruction_0(int initialize, int priority) {
    if (initialize == 1 && priority == 0xffff) {
        construct_particle_def(kingParticle,
            114, 108, 26, 118, 133, 73, 97, 1, 6.0f, 5.0f, 0.049999997f, 3, 1, 9);
    }
}
}  // namespace

extern "C" void _GLOBAL_$I$__3NPCG6Point3sPcN33ifPPc() {
    static_initialization_and_destruction_0(1, 0xffff);
}
