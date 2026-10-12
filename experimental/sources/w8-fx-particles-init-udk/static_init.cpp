// Observed _GLOBAL_$I$udkResourceMap at 0x0023fa58, 32 bytes, calling the file-local initializer at 0x0023f7a8
// (332 bytes) with (1, 0xffff). With those arguments the initializer makes
// the calls below.
// The file-local initializer and the objects listed here are file-local in the executable unless noted. Argument
// tuples were taken from the image; floats that are not short decimals are given as bit patterns. What the
// particle fields mean is not established here. Fresh compilation is not an original-byte match.
//
//   udkParticle2             0x007d7e30  80 bytes  file-local, defined here
//   udkParticle3             0x007d7e80  80 bytes  file-local, defined here
//   udkParticle4             0x007d7ed0  80 bytes  file-local, defined here
extern "C" {
void __11ParticleDefiiiiiiiifffiiiPFPv_v(void* self, int a, int b, int c, int d, int e, int f, int g, int h,
    float x, float y, float z, int i, int j, int k, void (*callback)(void*));
}

static unsigned char udkParticle2[80];
static unsigned char udkParticle3[80];
static unsigned char udkParticle4[80];

static float bits(unsigned value) { return __builtin_bit_cast(float, value); }

static void __static_initialization_and_destruction_0(int initialize, int priority) {
    if (priority != 0xffff) return;
    if (initialize == 1) {
        __11ParticleDefiiiiiiiifffiiiPFPv_v(udkParticle2, 51, 46, 25, 50, 99, 31, 16, 167, 5.0f, 5.0f, 0.0f, 2, 0,
                9, 0);
        __11ParticleDefiiiiiiiifffiiiPFPv_v(udkParticle3, 51, 46, 25, 50, 99, 31, 16, 167, bits(0x41200000), 6.0f,
                0.0f, 0, 1, 9, 0);
        __11ParticleDefiiiiiiiifffiiiPFPv_v(udkParticle4, 51, 46, 25, 50, 99, 31, 16, 50, 5.0f, 5.0f, 0.0f, 3, 0,
                9, 0);
    }
}

extern "C" void _GLOBAL_$I$udkResourceMap() {
    __static_initialization_and_destruction_0(1, 0xffff);
}
