#include "../w8-spell-static-init-a-common/particle_def.hpp"

// _GLOBAL_$I$__6SwitchG6Point3PPc at 0x21def8, 32 bytes. It calls the unit-local
// static initializer (0x21b430, 2192 bytes) with (1, 0xffff). That initializer
// constructs 23 particle definition records in the order below. Each
// construction passes eight integers, three single floats, three more integers
// and a null callback. The records are 80-byte zero-initialized objects.
using w8_static_init::ParticleDefStorage;
using w8_static_init::construct_particle_def;

// Global in the original symbol table.
ParticleDefStorage fireTrailParticle;
ParticleDefStorage trackfireTrailParticle;
ParticleDefStorage saveParticle;
ParticleDefStorage saveParticle2;
ParticleDefStorage teleportParticle1;
ParticleDefStorage teleportParticle2;

namespace {
// Local to the original unit.
ParticleDefStorage rainParticle;
ParticleDefStorage rainSplashParticle;
ParticleDefStorage snowParticle;
ParticleDefStorage sparkParticle;
ParticleDefStorage trailParticle;
ParticleDefStorage fireSmokeParticle;
ParticleDefStorage heatParticle;
ParticleDefStorage fireHitParticle;
ParticleDefStorage fireSmoke;
ParticleDefStorage iceBallParticle1;
ParticleDefStorage fuseParticle;
ParticleDefStorage smokeParticle;
ParticleDefStorage rootParticle;
ParticleDefStorage oniRedParticle;
ParticleDefStorage oniGreenParticle;
ParticleDefStorage oniBlueParticle;
ParticleDefStorage unholyAuraParticle;

void static_initialization_and_destruction_0(int initialize, int priority) {
    if (initialize == 1 && priority == 0xffff) {
        construct_particle_def(fireTrailParticle,
            231, 87, 100, 88, 264, 63, 4, 0, 30.4f, 4.9599996f, 0.92999995f, 2, 1, 0);
        construct_particle_def(trackfireTrailParticle,
            42, 87, 100, 88, -11, 171, 328, 0, 30.4f, 4.9599996f, 0.92999995f, 2, 1, 0);
        construct_particle_def(saveParticle,
            62, 80, 280, 108, 34, 87, 193, 0, 8.0f, 15.0f, 0.22f, 3, 1, 6);
        construct_particle_def(saveParticle2,
            78, 139, 136, 97, 103, 103, 206, 0, 8.4f, 24.0f, 3.5f, 2, 0, 9);
        construct_particle_def(rainParticle,
            20, 20, 40, 0, 20, 20, 40, 175, 4.838f, 0.0f, 0.0f, 1, 2, 4);
        construct_particle_def(rainSplashParticle,
            50, 50, 100, 100, 170, 200, 250, 0, 1.0f, 1e+01f, 0.0f, 1, 1, 7);
        construct_particle_def(snowParticle,
            200, 200, 200, 0, 200, 200, 200, 80, 5.0f, 1.5999999f, 0.0f, 2, 1, 6);
        construct_particle_def(teleportParticle1,
            2, 83, 74, 224, 31, 43, 31, 214, 82.17f, 74.714f, 0.0f, 3, 3, 2);
        construct_particle_def(teleportParticle2,
            72, 51, 6, 25, 43, 36, 32, 5, 3.1699998f, 72.714f, 0.0f, 3, 0, 6);
        construct_particle_def(sparkParticle,
            255, 255, 255, 255, 0, 0, 0, 0, 26.0f, 1e+01f, -15.0f, 2, 1, 0);
        construct_particle_def(trailParticle,
            255, 240, 119, 128, 0, 0, 0, -159, 35.334f, 1.5899999f, 0.0f, 1, 1, 8);
        construct_particle_def(fireSmokeParticle,
            72, 31, 19, 128, 0, 0, 0, 0, 29.976f, 57.331997f, 0.0f, 1, 1, 8);
        construct_particle_def(heatParticle,
            5, 128, 128, 402, 128, 128, 128, -68, 1e+02f, 1e+02f, 0.0f, 2, 3, 0);
        construct_particle_def(fireHitParticle,
            250, 126, 86, 255, 0, 0, 0, 0, 6e+01f, 1e+01f, -15.0f, 1, 1, 9);
        construct_particle_def(fireSmoke,
            90, 50, 40, 70, 2, 5, 5, 0, 43.0f, 1e+02f, 2.0f, 1, 0, 6);
        construct_particle_def(iceBallParticle1,
            100, 210, 238, 90, 4, 63, 105, 0, 11.0f, 5e+01f, 0.0f, 1, 1, 4);
        construct_particle_def(fuseParticle,
            255, 255, 255, 255, 0, 0, 0, 0, 8.0f, 3.0f, -5.0f, 2, 1, 0);
        construct_particle_def(smokeParticle,
            100, 44, 0, 23, -77, -54, 0, 0, 26.0f, 21.0f, -3.8799999f, 2, 0, 5);
        construct_particle_def(rootParticle,
            96, 210, 103, 85, 70, 242, 74, 0, 0.75899994f, 28.558998f, 0.0f, 1, 1, 15);
        construct_particle_def(oniRedParticle,
            254, 30, 27, 128, 250, 35, 34, 0, 38.123997f, 1.2609999f, 0.0f, 2, 1, 9);
        construct_particle_def(oniGreenParticle,
            30, 254, 27, 128, 35, 250, 34, 0, 20.123999f, 1.2609999f, 0.0f, 2, 1, 9);
        construct_particle_def(oniBlueParticle,
            27, 30, 254, 128, 34, 35, 250, 0, 38.123997f, 1.2609999f, 0.0f, 2, 1, 9);
        construct_particle_def(unholyAuraParticle,
            250, 100, 250, 103, 255, 103, 238, 32, 62.380997f, 37.380997f, 2.0449998f, 1, 2, 6);
    }
}
}  // namespace

extern "C" void _GLOBAL_$I$__6SwitchG6Point3PPc() {
    static_initialization_and_destruction_0(1, 0xffff);
}
