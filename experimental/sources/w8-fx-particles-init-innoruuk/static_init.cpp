// Observed _GLOBAL_$I$innoruukResourceMap at 0x001ff800, 32 bytes, calling the file-local initializer at 0x001ff0d0
// (900 bytes) with (1, 0xffff). With those arguments the initializer makes
// the calls below.
// The file-local initializer and the objects listed here are file-local in the executable unless noted. Argument
// tuples were taken from the image; floats that are not short decimals are given as bit patterns. What the
// particle fields mean is not established here. Fresh compilation is not an original-byte match.
//
//   innoruukParticle1        0x007d6de0  80 bytes  file-local, defined here
//   innoruukParticle2        0x007d6e30  80 bytes  file-local, defined here
//   innoruukParticle3        0x007d6e80  80 bytes  file-local, defined here
//   summonhammerParticle     0x007d6ed0  80 bytes  file-local, defined here
//   shockwaveInnoruukParticle 0x007d6f20  80 bytes  file-local, defined here
//   shockwaveInnoruukParticle2 0x007d6f70  80 bytes  file-local, defined here
//   hateballParticle         0x007d6fc0  80 bytes  file-local, defined here
//   hateballSmokeParticle    0x007d7010  80 bytes  file-local, defined here
//   disintoInnoruukParticle  0x007d7060  80 bytes  file-local, defined here
extern "C" {
void __11ParticleDefiiiiiiiifffiiiPFPv_v(void* self, int a, int b, int c, int d, int e, int f, int g, int h,
    float x, float y, float z, int i, int j, int k, void (*callback)(void*));
}

static unsigned char innoruukParticle1[80];
static unsigned char innoruukParticle2[80];
static unsigned char innoruukParticle3[80];
static unsigned char summonhammerParticle[80];
static unsigned char shockwaveInnoruukParticle[80];
static unsigned char shockwaveInnoruukParticle2[80];
static unsigned char hateballParticle[80];
static unsigned char hateballSmokeParticle[80];
static unsigned char disintoInnoruukParticle[80];

static float bits(unsigned value) { return __builtin_bit_cast(float, value); }

static void __static_initialization_and_destruction_0(int initialize, int priority) {
    if (priority != 0xffff) return;
    if (initialize == 1) {
        __11ParticleDefiiiiiiiifffiiiPFPv_v(innoruukParticle1, 0, 255, 0, 255, 0, 255, 0, 255, bits(0x42a00000),
                bits(0x41f00000), 0.0f, 2, 3, 0, 0);
        __11ParticleDefiiiiiiiifffiiiPFPv_v(innoruukParticle2, 255, 0, 0, 255, 255, 0, 0, 255, bits(0x42a00000),
                bits(0x41f00000), 0.0f, 2, 3, 6, 0);
        __11ParticleDefiiiiiiiifffiiiPFPv_v(innoruukParticle3, 255, 0, 0, 255, 255, 0, 0, 255, bits(0x42a00000),
                bits(0x41f00000), 0.0f, 1, 3, 6, 0);
        __11ParticleDefiiiiiiiifffiiiPFPv_v(summonhammerParticle, 220, 15, 0, 90, 33, 30, 28, 0, bits(0x41200000),
                bits(0x41a00000), 0.0f, 1, 1, 4, 0);
        __11ParticleDefiiiiiiiifffiiiPFPv_v(shockwaveInnoruukParticle, 142, 108, 51, 110, 108, 5, 0, 0,
                bits(0x41a00000), 8.0f, 0.0f, 1, 1, 2, 0);
        __11ParticleDefiiiiiiiifffiiiPFPv_v(shockwaveInnoruukParticle2, 255, 255, 255, 255, 255, 255, 255, 0, 5.0f,
                5.0f, 0.0f, 1, 1, 2, 0);
        __11ParticleDefiiiiiiiifffiiiPFPv_v(hateballParticle, 220, 15, 0, 90, 115, 10, 10, 0, 3.0f, 3.0f, 0.0f, 0,
                1, 4, 0);
        __11ParticleDefiiiiiiiifffiiiPFPv_v(hateballSmokeParticle, 0, 0, 0, 80, 33, 30, 28, 0, 8.0f, 75.0f, 0.0f,
                2, 0, 6, 0);
        __11ParticleDefiiiiiiiifffiiiPFPv_v(disintoInnoruukParticle, 261, 290, 64, 257, 267, 20, 25, 0,
                bits(0x41199999), bits(0x41099999), 0.0f, 3, 1, 8, 0);
    }
}

extern "C" void _GLOBAL_$I$innoruukResourceMap() {
    __static_initialization_and_destruction_0(1, 0xffff);
}
