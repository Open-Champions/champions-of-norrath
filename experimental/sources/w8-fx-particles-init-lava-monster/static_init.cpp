// Observed _GLOBAL_$I$lavaMonsterResourceMap at 0x001ee370, 32 bytes, calling the file-local initializer at 0x001ee0a0
// (328 bytes) with (1, 0xffff). With those arguments the initializer makes
// the calls below.
// The file-local initializer and the objects listed here are file-local in the executable unless noted. Argument
// tuples were taken from the image; floats that are not short decimals are given as bit patterns. What the
// particle fields mean is not established here. Fresh compilation is not an original-byte match.
//
//   lavamonsterParticle1     0x007d6c90  80 bytes  file-local, defined here
//   lavamonsterParticle2     0x007d6ce0  80 bytes  file-local, defined here
//   lavaSparkParticle        0x007d6d30  80 bytes  file-local, defined here
extern "C" {
void __11ParticleDefiiiiiiiifffiiiPFPv_v(void* self, int a, int b, int c, int d, int e, int f, int g, int h,
    float x, float y, float z, int i, int j, int k, void (*callback)(void*));
}

static unsigned char lavamonsterParticle1[80];
static unsigned char lavamonsterParticle2[80];
static unsigned char lavaSparkParticle[80];

static float bits(unsigned value) { return __builtin_bit_cast(float, value); }

static void __static_initialization_and_destruction_0(int initialize, int priority) {
    if (priority != 0xffff) return;
    if (initialize == 1) {
        __11ParticleDefiiiiiiiifffiiiPFPv_v(lavamonsterParticle1, 295, 195, 111, 413, 226, 11, 14, -11, 4.0f, 6.0f,
                -6.0f, 3, 1, 0, 0);
        __11ParticleDefiiiiiiiifffiiiPFPv_v(lavamonsterParticle2, 255, 0, 0, 255, 255, 0, 0, 255, bits(0x42a00000),
                bits(0x42a00000), 0.0f, 1, 3, 6, 0);
        __11ParticleDefiiiiiiiifffiiiPFPv_v(lavaSparkParticle, 302, 195, 291, 385, 206, 58, 27, 0,
                bits(0x40199999), bits(0x41e13333), -14.5f, 1, 2, 0, 0);
    }
}

extern "C" void _GLOBAL_$I$lavaMonsterResourceMap() {
    __static_initialization_and_destruction_0(1, 0xffff);
}
