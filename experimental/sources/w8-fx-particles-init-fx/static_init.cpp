// Observed _GLOBAL_$I$fxResourceMap at 0x001e6b10, 32 bytes, calling the file-local initializer at 0x001e3c70
// (5076 bytes) with (1, 0xffff). With those arguments the initializer makes
// the calls below.
// The file-local initializer and the objects listed here are file-local in the executable unless noted. Argument
// tuples were taken from the image; floats that are not short decimals are given as bit patterns. What the
// particle fields mean is not established here. Fresh compilation is not an original-byte match.
//
//   fxDust                   0x0042ad70  80 bytes  defined elsewhere in the draft
//   particleBloodDrop        0x007d5cf0  80 bytes  file-local, defined here
//   particleEctoDrop         0x007d5d40  80 bytes  file-local, defined here
//   particleSlimeDrop        0x007d5d90  80 bytes  file-local, defined here
//   snowParticle2            0x0042adc0  80 bytes  global, defined here (file bytes zero)
//   hitParticleBlood         0x007d5de0  80 bytes  file-local, defined here
//   hitParticleBigBlood      0x007d5e30  80 bytes  file-local, defined here
//   BurningHandsParticle1    0x007d5e80  80 bytes  file-local, defined here
//   LineParticle1            0x007d5ed0  80 bytes  file-local, defined here
//   coneOfColdParticle1      0x007d5f20  80 bytes  file-local, defined here
//   snowParticle1            0x007d5f70  80 bytes  file-local, defined here
//   fireParticle             0x007d5fc0  80 bytes  file-local, defined here
//   ballParticle             0x007d6010  80 bytes  file-local, defined here
//   trailParticle            0x007d6060  80 bytes  file-local, defined here
//   disintoParticle          0x007d60b0  80 bytes  file-local, defined here
//   blueDisintoParticle      0x007d6100  80 bytes  file-local, defined here
//   crispyCritterParticle    0x007d6150  80 bytes  file-local, defined here
//   cFireParticle            0x007d61a0  80 bytes  file-local, defined here
//   cSmokeParticle           0x007d61f0  80 bytes  file-local, defined here
//   glowParticle             0x007d6240  80 bytes  file-local, defined here
//   shimmerElementalParticle 0x007d6290  80 bytes  file-local, defined here
//   fireElementalBurstParticle 0x007d62e0  80 bytes  file-local, defined here
//   ballSparkParticle        0x007d6330  80 bytes  file-local, defined here
//   ballSmokeParticle        0x007d6380  80 bytes  file-local, defined here
//   lightningSwordParticle   0x007d63d0  80 bytes  file-local, defined here
//   flameSwordParticle       0x007d6420  80 bytes  file-local, defined here
//   newFlameSwordParticle1   0x007d6470  80 bytes  file-local, defined here
//   newFlameSwordParticle2   0x007d64c0  80 bytes  file-local, defined here
//   newFlameSwordParticle3   0x007d6510  80 bytes  file-local, defined here
//   iceSwordParticle         0x007d6560  80 bytes  file-local, defined here
//   newIceSwordParticle1     0x007d65b0  80 bytes  file-local, defined here
//   newIceSwordParticle2     0x007d6600  80 bytes  file-local, defined here
//   particleWeaponBlood      0x007d6650  80 bytes  file-local, defined here
//   particleWeaponEcto       0x007d66a0  80 bytes  file-local, defined here
//   particleWeaponSlime      0x007d66f0  80 bytes  file-local, defined here
//   flyingIcePuffParticle    0x007d6740  80 bytes  file-local, defined here
//   iceHitParticle           0x007d6790  80 bytes  file-local, defined here
//   vaporParticle            0x007d67e0  80 bytes  file-local, defined here
//   splashParticle           0x0042ae10  80 bytes  global, defined here (file bytes zero)
//   sparkParticle            0x007d6830  80 bytes  file-local, defined here
//   flameParticle            0x007d6880  80 bytes  file-local, defined here
//   smokeParticle            0x007d68d0  80 bytes  file-local, defined here
//   smokeParticleSmall       0x007d6920  80 bytes  file-local, defined here
//   gasParticle              0x007d6970  80 bytes  file-local, defined here
//   hitParticleDust2         0x007d69c0  80 bytes  file-local, defined here
//   froststormParticle1      0x007d6a10  80 bytes  file-local, defined here
//   froststormParticle2      0x007d6a60  80 bytes  file-local, defined here
//   froststormParticle3      0x007d6ab0  80 bytes  file-local, defined here
//   froststormParticle4      0x007d6b00  80 bytes  file-local, defined here
//   rainoffireCloud          0x007d6b50  80 bytes  file-local, defined here
//   rainoffireDrops          0x007d6ba0  80 bytes  file-local, defined here
//   rainoffireSplashParticle 0x007d6bf0  80 bytes  file-local, defined here
extern "C" {
void __11ParticleDefiiiiiiiifffiiiPFPv_v(void* self, int a, int b, int c, int d, int e, int f, int g, int h,
    float x, float y, float z, int i, int j, int k, void (*callback)(void*));
extern unsigned char fxDust[];
unsigned char snowParticle2[80];
unsigned char splashParticle[80];
}

static unsigned char particleBloodDrop[80];
static unsigned char particleEctoDrop[80];
static unsigned char particleSlimeDrop[80];
static unsigned char hitParticleBlood[80];
static unsigned char hitParticleBigBlood[80];
static unsigned char BurningHandsParticle1[80];
static unsigned char LineParticle1[80];
static unsigned char coneOfColdParticle1[80];
static unsigned char snowParticle1[80];
static unsigned char fireParticle[80];
static unsigned char ballParticle[80];
static unsigned char trailParticle[80];
static unsigned char disintoParticle[80];
static unsigned char blueDisintoParticle[80];
static unsigned char crispyCritterParticle[80];
static unsigned char cFireParticle[80];
static unsigned char cSmokeParticle[80];
static unsigned char glowParticle[80];
static unsigned char shimmerElementalParticle[80];
static unsigned char fireElementalBurstParticle[80];
static unsigned char ballSparkParticle[80];
static unsigned char ballSmokeParticle[80];
static unsigned char lightningSwordParticle[80];
static unsigned char flameSwordParticle[80];
static unsigned char newFlameSwordParticle1[80];
static unsigned char newFlameSwordParticle2[80];
static unsigned char newFlameSwordParticle3[80];
static unsigned char iceSwordParticle[80];
static unsigned char newIceSwordParticle1[80];
static unsigned char newIceSwordParticle2[80];
static unsigned char particleWeaponBlood[80];
static unsigned char particleWeaponEcto[80];
static unsigned char particleWeaponSlime[80];
static unsigned char flyingIcePuffParticle[80];
static unsigned char iceHitParticle[80];
static unsigned char vaporParticle[80];
static unsigned char sparkParticle[80];
static unsigned char flameParticle[80];
static unsigned char smokeParticle[80];
static unsigned char smokeParticleSmall[80];
static unsigned char gasParticle[80];
static unsigned char hitParticleDust2[80];
static unsigned char froststormParticle1[80];
static unsigned char froststormParticle2[80];
static unsigned char froststormParticle3[80];
static unsigned char froststormParticle4[80];
static unsigned char rainoffireCloud[80];
static unsigned char rainoffireDrops[80];
static unsigned char rainoffireSplashParticle[80];

static float bits(unsigned value) { return __builtin_bit_cast(float, value); }

static void __static_initialization_and_destruction_0(int initialize, int priority) {
    if (priority != 0xffff) return;
    if (initialize == 1) {
        __11ParticleDefiiiiiiiifffiiiPFPv_v(fxDust, 92, 90, 64, 34, 92, 85, 64, 0, bits(0x41a570a3),
                bits(0x4118cccc), bits(0xc0a0f5c2), 2, 0, 2, 0);
        __11ParticleDefiiiiiiiifffiiiPFPv_v(particleBloodDrop, 48, 6, 9, 204, 30, 6, 6, 0, 10.5f, bits(0x3f999999),
                bits(0xc094cccc), 3, 0, 6, 0);
        __11ParticleDefiiiiiiiifffiiiPFPv_v(particleEctoDrop, 80, 70, 5, 204, 60, 40, 5, 0, 10.5f,
                bits(0x3f999999), bits(0xc094cccc), 3, 0, 6, 0);
        __11ParticleDefiiiiiiiifffiiiPFPv_v(particleSlimeDrop, 39, 108, 40, 53, 40, 87, 19, 0, 10.5f,
                bits(0x3f999999), bits(0xc094cccc), 3, 0, 6, 0);
        __11ParticleDefiiiiiiiifffiiiPFPv_v(snowParticle2, 100, 210, 238, 90, 4, 63, 105, 0, bits(0x40b3d70a),
                5.75f, -2.0f, 1, 1, 4, 0);
        __11ParticleDefiiiiiiiifffiiiPFPv_v(hitParticleBlood, 48, 6, 9, 204, 30, 6, 6, 0, 4.5f, bits(0x3f999999),
                bits(0xc0299999), 1, 0, 6, 0);
        __11ParticleDefiiiiiiiifffiiiPFPv_v(hitParticleBigBlood, 48, 6, 9, 204, 30, 6, 6, 0, 4.5f,
                bits(0x3f999999), bits(0xc0299999), 3, 0, 6, 0);
        __11ParticleDefiiiiiiiifffiiiPFPv_v(BurningHandsParticle1, 213, 211, 49, 90, 438, 63, 4, 0,
                bits(0x4139fbe7), bits(0x42bad70a), 0.0f, 1, 1, 4, 0);
        __11ParticleDefiiiiiiiifffiiiPFPv_v(LineParticle1, 225, 169, 125, 118, 163, 131, 102, 19, bits(0x407cfdf3),
                bits(0x4298ab02), 0.0f, 1, 2, 5, 0);
        __11ParticleDefiiiiiiiifffiiiPFPv_v(coneOfColdParticle1, 100, 210, 238, 90, 4, 63, 105, 0,
                bits(0x4139eb85), 51.75f, 0.0f, 1, 1, 4, 0);
        __11ParticleDefiiiiiiiifffiiiPFPv_v(snowParticle1, 100, 210, 238, 90, 4, 63, 105, 0, bits(0x40b3d70a),
                5.75f, -2.0f, 3, 1, 4, 0);
        __11ParticleDefiiiiiiiifffiiiPFPv_v(fireParticle, 238, 210, 100, 180, 105, 63, 4, 0, bits(0x41f00000),
                5.0f, bits(0x3f733333), 2, 1, 0, 0);
        __11ParticleDefiiiiiiiifffiiiPFPv_v(ballParticle, 261, 290, 64, 257, 267, 20, 25, 0, 66.0f,
                bits(0x430c0000), 0.0f, 1, 1, 2, 0);
        __11ParticleDefiiiiiiiifffiiiPFPv_v(trailParticle, 248, 283, -21, 128, 251, 144, 11, 32, 58.0f, 37.0f,
                0.0f, 1, 1, 6, 0);
        __11ParticleDefiiiiiiiifffiiiPFPv_v(disintoParticle, 261, 290, 64, 257, 267, 20, 25, 0, bits(0x41199999),
                bits(0x41099999), 0.0f, 2, 1, 8, 0);
        __11ParticleDefiiiiiiiifffiiiPFPv_v(blueDisintoParticle, 64, 261, 290, 257, 25, 20, 267, 0,
                bits(0x41199999), bits(0x41099999), 0.0f, 2, 1, 8, 0);
        __11ParticleDefiiiiiiiifffiiiPFPv_v(crispyCritterParticle, 20, 20, 20, 100, 15, 15, 15, 0, 8.0f,
                bits(0x40b33333), 0.0f, 2, 0, 9, 0);
        __11ParticleDefiiiiiiiifffiiiPFPv_v(cFireParticle, 201, 191, 103, 70, 129, 20, 74, 0, 24.0f,
                bits(0x41200000), bits(0x40ae6666), 1, 1, 8, 0);
        __11ParticleDefiiiiiiiifffiiiPFPv_v(cSmokeParticle, 64, 39, 15, 82, 0, 0, 0, 0, 15.0f, 63.0f, 1.5f, 3, 0,
                6, 0);
        __11ParticleDefiiiiiiiifffiiiPFPv_v(glowParticle, 100, 72, 40, 41, 145, 89, 25, 0, 24.5f, 8.75f, 0.0f, 1,
                1, 4, 0);
        __11ParticleDefiiiiiiiifffiiiPFPv_v(shimmerElementalParticle, 2, 83, 74, 224, 31, 43, 31, 214,
                bits(0x41b95c28), bits(0x41bdb645), 0.0f, 3, 3, 2, 0);
        __11ParticleDefiiiiiiiifffiiiPFPv_v(fireElementalBurstParticle, 220, 62, 30, 41, 250, 79, 15, 0, 30.5f,
                24.75f, 0.0f, 1, 1, 4, 0);
        __11ParticleDefiiiiiiiifffiiiPFPv_v(ballSparkParticle, 333, 358, 287, 128, 122, 106, 349, 0,
                bits(0x40be147a), bits(0x41091eb8), bits(0xc1151eb8), 1, 2, 0, 0);
        __11ParticleDefiiiiiiiifffiiiPFPv_v(ballSmokeParticle, 111, 112, 154, 80, 33, 30, 28, 0, 8.0f, 75.0f,
                bits(0x3dcccccc), 2, 0, 6, 0);
        __11ParticleDefiiiiiiiifffiiiPFPv_v(lightningSwordParticle, 111, 112, 154, 60, 33, 30, 28, -30, 15.0f,
                bits(0x41f00000), bits(0x3dcccccc), 0, 0, 6, 0);
        __11ParticleDefiiiiiiiifffiiiPFPv_v(flameSwordParticle, 231, 87, 100, 29, 264, 63, 4, 0, bits(0x41a8b851),
                bits(0x419acccc), bits(0x3f9851eb), 0, 1, 0, 0);
        __11ParticleDefiiiiiiiifffiiiPFPv_v(newFlameSwordParticle1, 295, 195, 111, 413, 226, 11, 14, -11, 4.0f,
                3.0f, -6.0f, 1, 1, 0, 0);
        __11ParticleDefiiiiiiiifffiiiPFPv_v(newFlameSwordParticle2, 130, 116, 76, 87, 194, 120, 74, 0,
                bits(0x41a00000), 0.0f, bits(0x3fb33333), 0, 1, 6, 0);
        __11ParticleDefiiiiiiiifffiiiPFPv_v(newFlameSwordParticle3, 0, 0, 0, 64, 34, 30, 21, -32, 15.0f,
                bits(0x41f00000), 3.0f, 1, 0, 6, 0);
        __11ParticleDefiiiiiiiifffiiiPFPv_v(iceSwordParticle, 100, 210, 238, 80, 4, 63, 105, -50, 12.0f, -12.0f,
                -7.0f, 0, 1, 8, 0);
        __11ParticleDefiiiiiiiifffiiiPFPv_v(newIceSwordParticle1, 100, 210, 235, 140, 198, 199, 244, 140, 4.0f,
                0.0f, -2.0f, 2, 1, 5, 0);
        __11ParticleDefiiiiiiiifffiiiPFPv_v(newIceSwordParticle2, 100, 139, 255, 255, 198, 199, 244, 255, 7.0f,
                0.0f, bits(0xc104cccc), 2, 0, 5, 0);
        __11ParticleDefiiiiiiiifffiiiPFPv_v(particleWeaponBlood, 48, 6, 9, 204, 30, 6, 6, 0, 10.5f,
                bits(0x3f999999), bits(0xc094cccc), 1, 0, 6, 0);
        __11ParticleDefiiiiiiiifffiiiPFPv_v(particleWeaponEcto, 80, 70, 5, 204, 60, 40, 5, 0, 10.5f,
                bits(0x3f999999), bits(0xc094cccc), 1, 0, 6, 0);
        __11ParticleDefiiiiiiiifffiiiPFPv_v(particleWeaponSlime, 39, 108, 40, 53, 40, 87, 19, 0, 10.5f,
                bits(0x3f999999), bits(0xc094cccc), 1, 0, 6, 0);
        __11ParticleDefiiiiiiiifffiiiPFPv_v(flyingIcePuffParticle, 68, 136, 144, 40, 137, 160, 158, 0,
                bits(0x41a00000), bits(0x437a0000), 2.0f, 2, 0, 6, 0);
        __11ParticleDefiiiiiiiifffiiiPFPv_v(iceHitParticle, 111, 151, 154, 40, 60, 120, 127, 40, bits(0x41200000),
                bits(0x41200000), -5.0f, 2, 0, 6, 0);
        __11ParticleDefiiiiiiiifffiiiPFPv_v(vaporParticle, 68, 136, 144, 40, 137, 160, 158, 0, bits(0x43160000),
                bits(0x43960000), 4.5f, 2, 0, 6, 0);
        __11ParticleDefiiiiiiiifffiiiPFPv_v(splashParticle, 121, 127, 126, 145, 66, 87, 68, 0, 5.5f,
                bits(0x40a851eb), bits(0xc1cc51eb), 2, 0, 2, 0);
        __11ParticleDefiiiiiiiifffiiiPFPv_v(sparkParticle, 295, 195, 111, 149, 226, 11, 14, 24, 4.0f, 6.0f,
                bits(0xc0533333), 2, 1, 0, 0);
        __11ParticleDefiiiiiiiifffiiiPFPv_v(flameParticle, 130, 116, 76, 87, 194, 120, 74, 0, 4.0f, 44.0f,
                bits(0x402ccccc), 0, 1, 2, 0);
        __11ParticleDefiiiiiiiifffiiiPFPv_v(smokeParticle, 0, 0, 0, 64, 34, 30, 22, 0, 14.0f, 77.0f, 1.0f, 3, 0, 6,
                0);
        __11ParticleDefiiiiiiiifffiiiPFPv_v(smokeParticleSmall, 0, 0, 0, 64, 34, 30, 22, 0, bits(0x41f00000),
                bits(0x42480000), 1.0f, 2, 0, 6, 0);
        __11ParticleDefiiiiiiiifffiiiPFPv_v(gasParticle, 36, 89, 49, 56, 65, 144, 85, 0, bits(0x41cd9999),
                bits(0x42a00000), 0.0f, 2, 1, 6, 0);
        __11ParticleDefiiiiiiiifffiiiPFPv_v(hitParticleDust2, 92, 89, 64, 20, 92, 85, 64, 0, bits(0x4272b851),
                bits(0x42f11999), bits(0xc041eb85), 2, 0, 2, 0);
        __11ParticleDefiiiiiiiifffiiiPFPv_v(froststormParticle1, 100, 210, 235, 140, 198, 199, 244, 140, 4.0f,
                0.0f, -7.0f, 3, 1, 5, 0);
        __11ParticleDefiiiiiiiifffiiiPFPv_v(froststormParticle2, 100, 139, 255, 255, 198, 199, 244, 0, 8.0f, 6.0f,
                bits(0xc104cccc), 3, 0, 5, 0);
        __11ParticleDefiiiiiiiifffiiiPFPv_v(froststormParticle3, 144, 148, 173, 298, 120, 117, 114, 4,
                bits(0x4104cccc), bits(0x41033333), bits(0xbfa66666), 2, 1, 8, 0);
        __11ParticleDefiiiiiiiifffiiiPFPv_v(froststormParticle4, 117, 137, 215, 121, 148, 147, 223, 5, 6.0f,
                bits(0x41a00000), 0.0f, 3, 1, 6, 0);
        __11ParticleDefiiiiiiiifffiiiPFPv_v(rainoffireCloud, 15, 10, 10, 14, 0, 0, 0, 0, bits(0x428ccccc),
                bits(0x42493333), 0.0f, 1, 0, 6, 0);
        __11ParticleDefiiiiiiiifffiiiPFPv_v(rainoffireDrops, 237, 50, 10, 40, 217, 50, 10, 125, 26.5f, 5.0f,
                bits(0xc1a00000), 1, 2, 5, 0);
        __11ParticleDefiiiiiiiifffiiiPFPv_v(rainoffireSplashParticle, 117, 137, 215, 121, 148, 147, 223, 0,
                bits(0x40571a9f), bits(0x421b9999), bits(0xbeb43958), 3, 2, 7, 0);
    }
}

extern "C" void _GLOBAL_$I$fxResourceMap() {
    __static_initialization_and_destruction_0(1, 0xffff);
}
