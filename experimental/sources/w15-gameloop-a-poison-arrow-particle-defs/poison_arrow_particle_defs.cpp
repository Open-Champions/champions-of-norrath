// Successor of the unit it is copied from, with the record(s) named below given external linkage and their real symbol
// names. The original file keeps these records file-local, but other functions of the same original file are separate
// units here and name them as imports, so they must be linkable. The initializer and its constructor calls are unchanged.
// Observed/inferred/unknown are those of the source unit. Select either this unit or the one it copies, never both.
#include "../w8-spell-static-init-a-common/particle_def.hpp"

// _GLOBAL_$I$getRankDescription__16SkillPoisonArrowPUsi (32-byte wrapper in the original). It calls the unit-local
// static initializer (the local text symbol next to it) with (1, 0xffff). That initializer
// constructs 3 particle definition records in the order below. Each
// construction passes eight integers, three single floats, three more integers
// and a null callback. The records are 80-byte zero-initialized objects.
using w8_static_init::ParticleDefStorage;
using w8_static_init::construct_particle_def;

extern "C" {
// Exported so that other units (which the original keeps in one file) can name them.
ParticleDefStorage poisonPoof;
}

namespace {
// Local to the original unit.
ParticleDefStorage poisonSmoke;
ParticleDefStorage poisonFleck;

void static_initialization_and_destruction_0(int initialize, int priority) {
    if (initialize == 1 && priority == 0xffff) {
        construct_particle_def(poisonSmoke,
            41, 107, 5, 104, 28, 100, 49, 0, 38.0f, 0x1.b19998p+5f, 0.9f, 0, 1, 2);
        construct_particle_def(poisonFleck,
            103, 119, 31, 100, 9, 9, 9, 33, 7.0f, 5.0f, 0.0f, 1, 2, 6);
        construct_particle_def(poisonPoof,
            58, 127, 0, 143, 38, 141, 3, 11, 9.0f, 0x1.233332p+4f, 0.0f, 3, 2, 6);
    }
}
}  // namespace

extern "C" void _GLOBAL_$I$getRankDescription__16SkillPoisonArrowPUsi() {
    static_initialization_and_destruction_0(1, 0xffff);
}
