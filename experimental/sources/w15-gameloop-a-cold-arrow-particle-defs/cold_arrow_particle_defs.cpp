// Successor of the unit it is copied from, with the record(s) named below given external linkage and their real symbol
// names. The original file keeps these records file-local, but other functions of the same original file are separate
// units here and name them as imports, so they must be linkable. The initializer and its constructor calls are unchanged.
// The three float arguments that the source wrote as decimal literals are given here as the bit patterns the image
// passes: the reviewed compiler rounds such a decimal literal one unit toward zero, which the paired run caught.
// Observed/inferred/unknown are those of the source unit. Select either this unit or the one it copies, never both.
#include "../w8-spell-static-init-a-common/particle_def.hpp"

// _GLOBAL_$I$__19ColdArrowProjectileG6Point3sP10GameObjectiP5Skill at 0x29fe00, 32 bytes. It calls the unit-local
// static initializer (0x29f9f8, 328 bytes) with (1, 0xffff). That initializer
// constructs 3 particle definition records in the order below. Each
// construction passes eight integers, three single floats, three more integers
// and a null callback. The records are 80-byte zero-initialized objects.
using w8_static_init::ParticleDefStorage;
using w8_static_init::construct_particle_def;

extern "C" {
// Exported so that other units (which the original keeps in one file) can name them.
ParticleDefStorage coldSmoke;
ParticleDefStorage coldSpeck;
}

namespace {
constexpr float bits(unsigned word) { return __builtin_bit_cast(float, word); }
// Local to the original unit.
ParticleDefStorage pack;

void static_initialization_and_destruction_0(int initialize, int priority) {
    if (initialize == 1 && priority == 0xffff) {
        construct_particle_def(coldSmoke,
            65, 65, 144, 64, 28, 39, 49, 0, 23.0f, 72.2f, -5.5f, 1, 1, 6);
        construct_particle_def(coldSpeck,
            77, 89, 115, 129, 88, 74, 115, 128, 6.0f, bits(0x400cccccu), -5.1f, 3, 0, 5);
        construct_particle_def(pack,
            111, 119, 215, 128, 76, 86, 210, 50, 5.0f, bits(0x3d4cccccu), bits(0xbdccccccu), 3, 0, 4);
    }
}
}  // namespace

extern "C" void _GLOBAL_$I$__19ColdArrowProjectileG6Point3sP10GameObjectiP5Skill() {
    static_initialization_and_destruction_0(1, 0xffff);
}
