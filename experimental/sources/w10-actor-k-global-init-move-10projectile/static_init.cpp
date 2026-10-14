// Observed _GLOBAL_$I$move__10ProjectileG6Point3 at 0x00155fa0, 32 bytes, calling the file-local initializer
// __static_initialization_and_destruction_0 at 0x00153940 (1636 bytes) with (1, 0xffff). The
// initializer does nothing unless its second argument is 0xffff and its first is 1; then it makes
// these calls in this order. Fifteen ParticleDef constructions (the object, eight ints, three floats,
// three ints and a null callback each) and four ResourceMap constructions (the object, the lump
// name "fx.lmp" and a resource name). Argument tuples were read from the instruction stream; floats
// are given as bit patterns. What the particle fields mean is not established.
//
// File-local objects in the executable (80 bytes each unless noted), defined here as file-local:
//   trailParticleFire 0x600b40, trailParticleSmoke 0x600b90, ballSparkParticle 0x600be0,
//   snowParticle1 0x600c30, trailParticleSpark 0x600c80, trailParticleFire2 0x600cd0,
//   waveMainParticle 0x600d60, waveMainAlternate 0x600db0, waveRandomFoam 0x600e00,
//   waveTrail 0x600e50, waveSplashBig 0x600ea0, waveSplashSmall 0x600ef0, waveFroth 0x600f40,
//   goldRestModel 0x600d50 (16 bytes).
// Defined here as global because another unit calls them by name: morpheffectParticle1 0x600f90 and
// morpheffectParticle2 0x600fe0 (80 bytes each). The ResourceMaps goldAnm, goldModel and goldTex
// are 16-byte objects defined by other units and only referenced here.
// Nothing here ran on the EE.
extern "C" {
void __11ParticleDefiiiiiiiifffiiiPFPv_v(void* self, int a, int b, int c, int d, int e, int f, int g, int h,
    float x, float y, float z, int i, int j, int k, void (*callback)(void*));
void __11ResourceMapPcT1(void* self, const char* lump, const char* name);
extern unsigned char goldAnm[];
extern unsigned char goldModel[];
extern unsigned char goldTex[];
unsigned char morpheffectParticle1[80];
unsigned char morpheffectParticle2[80];
}

static unsigned char trailParticleFire[80];
static unsigned char trailParticleSmoke[80];
static unsigned char ballSparkParticle[80];
static unsigned char snowParticle1[80];
static unsigned char trailParticleSpark[80];
static unsigned char trailParticleFire2[80];
static unsigned char goldRestModel[16];
static unsigned char waveMainParticle[80];
static unsigned char waveMainAlternate[80];
static unsigned char waveRandomFoam[80];
static unsigned char waveTrail[80];
static unsigned char waveSplashBig[80];
static unsigned char waveSplashSmall[80];
static unsigned char waveFroth[80];

static float bits(unsigned value) { return __builtin_bit_cast(float, value); }

static void __static_initialization_and_destruction_0(int initialize, int priority) {
    if (priority != 0xffff) return;
    if (initialize == 1) {
        __11ParticleDefiiiiiiiifffiiiPFPv_v(trailParticleFire, 238, 210, 100, 180, 105, 63, 4, -100,
                bits(0x41700000u), bits(0xc0200000u), bits(0x3f733333u), 1, 1, 0, nullptr);
        __11ParticleDefiiiiiiiifffiiiPFPv_v(trailParticleSmoke, 100, 44, 0, 23, -77, -54, 0, 0,
                bits(0x41d00000u), bits(0x41f00000u), bits(0x00000000u), 2, 0, 5, nullptr);
        __11ParticleDefiiiiiiiifffiiiPFPv_v(ballSparkParticle, 100, 111, 358, 128, 122, 106, 349, 0,
                bits(0x40be147au), bits(0x41091eb8u), bits(0xc1151eb8u), 1, 2, 0, nullptr);
        __11ParticleDefiiiiiiiifffiiiPFPv_v(snowParticle1, 100, 210, 238, 90, 4, 63, 105, 0,
                bits(0x4033d70au), bits(0x40380000u), bits(0xc0000000u), 3, 1, 4, nullptr);
        __11ParticleDefiiiiiiiifffiiiPFPv_v(trailParticleSpark, 200, 200, 150, 300, 200, 175, 100, 0,
                bits(0x40000000u), bits(0x40a00000u), bits(0xc1a00000u), 1, 2, 2, nullptr);
        __11ParticleDefiiiiiiiifffiiiPFPv_v(trailParticleFire2, 238, 210, 100, 180, 105, 63, 4, 0,
                bits(0x41f00000u), bits(0x40a00000u), bits(0x3f733333u), 2, 1, 0, nullptr);
        __11ResourceMapPcT1(goldAnm, "fx.lmp", "goldpile_fall.anm");
        __11ResourceMapPcT1(goldModel, "fx.lmp", "goldpile.vif");
        __11ResourceMapPcT1(goldTex, "fx.lmp", "goldpile.tex");
        __11ResourceMapPcT1(goldRestModel, "fx.lmp", "goldpile_rest.vif");
        __11ParticleDefiiiiiiiifffiiiPFPv_v(waveMainParticle, 103, 118, 143, 80, 99, 116, 121, 24,
                bits(0x41d64236u), bits(0x42a7833du), bits(0xbf3f3882u), 1, 0, 2, nullptr);
        __11ParticleDefiiiiiiiifffiiiPFPv_v(waveMainAlternate, 65, 80, 93, 84, 103, 148, 111, 15,
                bits(0x428e0000u), bits(0x43369d57u), bits(0xc06167dbu), 2, 0, 2, nullptr);
        __11ParticleDefiiiiiiiifffiiiPFPv_v(waveRandomFoam, 79, 85, 74, 62, 67, 99, 89, 0,
                bits(0x41e06e65u), bits(0x43127b83u), bits(0xc0654fa8u), 3, 1, 2, nullptr);
        __11ParticleDefiiiiiiiifffiiiPFPv_v(waveTrail, 118, 127, 128, 81, 129, 132, 137, 0,
                bits(0x4109368fu), bits(0x40b7167eu), bits(0x00000000u), 3, 0, 2, nullptr);
        __11ParticleDefiiiiiiiifffiiiPFPv_v(waveSplashBig, 104, 103, 130, 93, 72, 76, 74, 0,
                bits(0x40e00000u), bits(0x41a00000u), bits(0xc0c00000u), 3, 1, 2, nullptr);
        __11ParticleDefiiiiiiiifffiiiPFPv_v(waveSplashSmall, 77, 67, 93, 85, 72, 76, 74, 0,
                bits(0x40e00000u), bits(0x41a00000u), bits(0xbfad0d41u), 2, 1, 2, nullptr);
        __11ParticleDefiiiiiiiifffiiiPFPv_v(waveFroth, 123, 124, 127, 97, 82, 84, 83, 0,
                bits(0x40613715u), bits(0x41a40bd3u), bits(0x00000000u), 3, 1, 5, nullptr);
        __11ParticleDefiiiiiiiifffiiiPFPv_v(morpheffectParticle1, 220, 20, 0, 150, 220, 20, 0, 150,
                bits(0x41700000u), bits(0x41700000u), bits(0x00000000u), 2, 1, 8, nullptr);
        __11ParticleDefiiiiiiiifffiiiPFPv_v(morpheffectParticle2, 255, 255, 255, 255, 255, 255, 255, 255,
                bits(0x41f00000u), bits(0x00000000u), bits(0x00000000u), 0, 1, 8, nullptr);
    }
}

extern "C" void _GLOBAL_$I$move__10ProjectileG6Point3() {
    __static_initialization_and_destruction_0(1, 0xffff);
}
