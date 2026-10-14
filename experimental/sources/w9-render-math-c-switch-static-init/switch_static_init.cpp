// Observed _GLOBAL_$I$__6SwitchG6Point3PPc at 0x0021df04 (a 32-byte wrapper) calling the file-local
// initializer at 0x0021b430 (2188 bytes) with (1, 0xffff). With those arguments the initializer makes
// the 23 calls below, in this order, each to the ParticleDef constructor
// __11ParticleDefiiiiiiiifffiiiPFPv_v at 0x00180d78 with a zero callback. Integer tuples and float bit
// patterns were taken by running the image's code under a private interpreter with the constructor
// replaced by a recorder. The names and addresses are the executable's own symbol names. What the
// particle fields mean is not established here. Fresh compilation is not an original-byte match.
//
// Objects (80 bytes each; the global ones have zero bytes in the file image and are exported, the
// others are file-local in the executable and kept file-local here, except the two the Container
// code reads, which keep the placeholder names its authored source uses):
//   fireTrailParticle 0x0042c4d0   trackfireTrailParticle 0x0042c520   saveParticle 0x0042c590
//   saveParticle2 0x0042c5e0       teleportParticle1 0x0042c670        teleportParticle2 0x0042c6c0
//   rainParticle 0x007d73f0  rainSplashParticle 0x007d7440  snowParticle 0x007d7490  sparkParticle 0x007d74f0
//   trailParticle 0x007d7540  fireSmokeParticle 0x007d7590  heatParticle 0x007d75e0  fireHitParticle 0x007d7630
//   fireSmoke 0x007d7680  iceBallParticle1 0x007d76d0
//   norrath_container_msg_run_fuseParticle_bytes 0x007d7720 (symbol fuseParticle)
//   norrath_container_msg_run_smokeParticle_bytes 0x007d7770 (symbol smokeParticle)
//   rootParticle 0x007d77c0  oniRedParticle 0x007d7810  oniGreenParticle 0x007d7860  oniBlueParticle 0x007d78b0
//   unholyAuraParticle 0x007d7900
// Unknown: which translation unit the initializer belongs to beyond the Switch constructor's wrapper name.
extern "C" {
void __11ParticleDefiiiiiiiifffiiiPFPv_v(void* self, int a, int b, int c, int d, int e, int f, int g, int h,
    float x, float y, float z, int i, int j, int k, void (*callback)(void*));
unsigned char fireTrailParticle[80];
unsigned char trackfireTrailParticle[80];
unsigned char saveParticle[80];
unsigned char saveParticle2[80];
unsigned char teleportParticle1[80];
unsigned char teleportParticle2[80];
unsigned char norrath_container_msg_run_fuseParticle_bytes[80];
unsigned char norrath_container_msg_run_smokeParticle_bytes[80];
}
static unsigned char rainParticle[80];
static unsigned char rainSplashParticle[80];
static unsigned char snowParticle[80];
static unsigned char sparkParticle[80];
static unsigned char trailParticle[80];
static unsigned char fireSmokeParticle[80];
static unsigned char heatParticle[80];
static unsigned char fireHitParticle[80];
static unsigned char fireSmoke[80];
static unsigned char iceBallParticle1[80];
static unsigned char rootParticle[80];
static unsigned char oniRedParticle[80];
static unsigned char oniGreenParticle[80];
static unsigned char oniBlueParticle[80];
static unsigned char unholyAuraParticle[80];

static float bits(unsigned value) { return __builtin_bit_cast(float, value); }

__attribute__((noinline)) static void __static_initialization_and_destruction_0(int initialize, int priority) {
    if (priority != 0xffff) return;
    if (initialize == 1) {
        __11ParticleDefiiiiiiiifffiiiPFPv_v(fireTrailParticle, 231, 87, 100, 88, 264, 63, 4, 0,
                bits(0x41f33333u), bits(0x409eb851u), bits(0x3f6e147au), 2, 1, 0, nullptr);
        __11ParticleDefiiiiiiiifffiiiPFPv_v(trackfireTrailParticle, 42, 87, 100, 88, -11, 171, 328, 0,
                bits(0x41f33333u), bits(0x409eb851u), bits(0x3f6e147au), 2, 1, 0, nullptr);
        __11ParticleDefiiiiiiiifffiiiPFPv_v(saveParticle, 62, 80, 280, 108, 34, 87, 193, 0,
                bits(0x41000000u), bits(0x41700000u), bits(0x3e6147aeu), 3, 1, 6, nullptr);
        __11ParticleDefiiiiiiiifffiiiPFPv_v(saveParticle2, 78, 139, 136, 97, 103, 103, 206, 0,
                bits(0x41066666u), bits(0x41c00000u), bits(0x40600000u), 2, 0, 9, nullptr);
        __11ParticleDefiiiiiiiifffiiiPFPv_v(rainParticle, 20, 20, 40, 0, 20, 20, 40, 175,
                bits(0x409ad0e5u), 0.0f, 0.0f, 1, 2, 4, nullptr);
        __11ParticleDefiiiiiiiifffiiiPFPv_v(rainSplashParticle, 50, 50, 100, 100, 170, 200, 250, 0,
                bits(0x3f800000u), bits(0x41200000u), 0.0f, 1, 1, 7, nullptr);
        __11ParticleDefiiiiiiiifffiiiPFPv_v(snowParticle, 200, 200, 200, 0, 200, 200, 200, 80,
                bits(0x40a00000u), bits(0x3fccccccu), 0.0f, 2, 1, 6, nullptr);
        __11ParticleDefiiiiiiiifffiiiPFPv_v(teleportParticle1, 2, 83, 74, 224, 31, 43, 31, 214,
                bits(0x42a4570au), bits(0x42956d91u), 0.0f, 3, 3, 2, nullptr);
        __11ParticleDefiiiiiiiifffiiiPFPv_v(teleportParticle2, 72, 51, 6, 25, 43, 36, 32, 5,
                bits(0x404ae147u), bits(0x42916d91u), 0.0f, 3, 0, 6, nullptr);
        __11ParticleDefiiiiiiiifffiiiPFPv_v(sparkParticle, 255, 255, 255, 255, 0, 0, 0, 0,
                bits(0x41d00000u), bits(0x41200000u), bits(0xc1700000u), 2, 1, 0, nullptr);
        __11ParticleDefiiiiiiiifffiiiPFPv_v(trailParticle, 255, 240, 119, 128, 0, 0, 0, -159,
                bits(0x420d5604u), bits(0x3fcb851eu), 0.0f, 1, 1, 8, nullptr);
        __11ParticleDefiiiiiiiifffiiiPFPv_v(fireSmokeParticle, 72, 31, 19, 128, 0, 0, 0, 0,
                bits(0x41efced9u), bits(0x426553f7u), 0.0f, 1, 1, 8, nullptr);
        __11ParticleDefiiiiiiiifffiiiPFPv_v(heatParticle, 5, 128, 128, 402, 128, 128, 128, -68,
                bits(0x42c80000u), bits(0x42c80000u), 0.0f, 2, 3, 0, nullptr);
        __11ParticleDefiiiiiiiifffiiiPFPv_v(fireHitParticle, 250, 126, 86, 255, 0, 0, 0, 0,
                bits(0x42700000u), bits(0x41200000u), bits(0xc1700000u), 1, 1, 9, nullptr);
        __11ParticleDefiiiiiiiifffiiiPFPv_v(fireSmoke, 90, 50, 40, 70, 2, 5, 5, 0,
                bits(0x422c0000u), bits(0x42c80000u), bits(0x40000000u), 1, 0, 6, nullptr);
        __11ParticleDefiiiiiiiifffiiiPFPv_v(iceBallParticle1, 100, 210, 238, 90, 4, 63, 105, 0,
                bits(0x41300000u), bits(0x42480000u), 0.0f, 1, 1, 4, nullptr);
        __11ParticleDefiiiiiiiifffiiiPFPv_v(norrath_container_msg_run_fuseParticle_bytes, 255, 255, 255, 255, 0, 0, 0, 0,
                bits(0x41000000u), bits(0x40400000u), bits(0xc0a00000u), 2, 1, 0, nullptr);
        __11ParticleDefiiiiiiiifffiiiPFPv_v(norrath_container_msg_run_smokeParticle_bytes, 100, 44, 0, 23, -77, -54, 0, 0,
                bits(0x41d00000u), bits(0x41a80000u), bits(0xc07851ebu), 2, 0, 5, nullptr);
        __11ParticleDefiiiiiiiifffiiiPFPv_v(rootParticle, 96, 210, 103, 85, 70, 242, 74, 0,
                bits(0x3f424dd2u), bits(0x41e478d4u), 0.0f, 1, 1, 15, nullptr);
        __11ParticleDefiiiiiiiifffiiiPFPv_v(oniRedParticle, 254, 30, 27, 128, 250, 35, 34, 0,
                bits(0x42187ef9u), bits(0x3fa16872u), 0.0f, 2, 1, 9, nullptr);
        __11ParticleDefiiiiiiiifffiiiPFPv_v(oniGreenParticle, 30, 254, 27, 128, 35, 250, 34, 0,
                bits(0x41a0fdf3u), bits(0x3fa16872u), 0.0f, 2, 1, 9, nullptr);
        __11ParticleDefiiiiiiiifffiiiPFPv_v(oniBlueParticle, 27, 30, 254, 128, 34, 35, 250, 0,
                bits(0x42187ef9u), bits(0x3fa16872u), 0.0f, 2, 1, 9, nullptr);
        __11ParticleDefiiiiiiiifffiiiPFPv_v(unholyAuraParticle, 250, 100, 250, 103, 255, 103, 238, 32,
                bits(0x42798624u), bits(0x42158624u), bits(0x4002e147u), 1, 2, 6, nullptr);
    }
}

extern "C" void _GLOBAL_$I$__6SwitchG6Point3PPc() {
    __static_initialization_and_destruction_0(1, 0xffff);
}
