#include "../w8-spell-static-init-a-common/particle_def.hpp"

// _GLOBAL_$I$__5TorchG6Point3PPci at 0x2313a8, 32 bytes. It calls the unit-local
// static initializer (0x230d48, 704 bytes) with (1, 0xffff). That initializer
// constructs 7 particle definition records in the order below. Each
// construction passes eight integers, three single floats, three more integers
// and a null callback. The records are 80-byte zero-initialized objects.
using w8_static_init::ParticleDefStorage;
using w8_static_init::construct_particle_def;

namespace {
// Local to the original unit.
ParticleDefStorage torchParticle;
ParticleDefStorage torchSmokeParticle;
ParticleDefStorage heatParticle;
ParticleDefStorage fireSparkParticle;
ParticleDefStorage candleParticle;
ParticleDefStorage candle2Particle;
ParticleDefStorage lampParticle;

void static_initialization_and_destruction_0(int initialize, int priority) {
    if (initialize == 1 && priority == 0xffff) {
        construct_particle_def(torchParticle,
            132, 83, 45, 59, 108, 46, 13, 94, 5e+01f, 5.0f, 1.3f, 2, 1, 9);
        construct_particle_def(torchSmokeParticle,
            90, 74, 60, 48, 60, 50, 45, 0, 3e+01f, 6e+01f, 2.0f, 2, 0, 6);
        construct_particle_def(heatParticle,
            5, 128, 128, 402, 128, 128, 128, -68, 38.0f, 34.0f, 0.0f, 1, 3, 0);
        construct_particle_def(fireSparkParticle,
            402, 295, 391, 485, 126, 58, 27, 0, 2.3999999f, 28.15f, -14.5f, 1, 2, 0);
        construct_particle_def(candleParticle,
            128, 96, 26, 152, 119, 55, 1, 47, 2.6f, 0.47f, 0.13999999f, 1, 1, 4);
        construct_particle_def(candle2Particle,
            128, 96, 26, 152, 119, 55, 1, 47, 3.6f, 0.47f, 0.13999999f, 1, 1, 4);
        construct_particle_def(lampParticle,
            164, 120, 10, 790, 154, 55, 47, 0, 25.598f, 0.17199999f, 0.0f, 2, 1, 9);
    }
}
}  // namespace

extern "C" void _GLOBAL_$I$__5TorchG6Point3PPci() {
    static_initialization_and_destruction_0(1, 0xffff);
}
