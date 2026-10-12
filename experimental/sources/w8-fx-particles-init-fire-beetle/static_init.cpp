// Observed _GLOBAL_$I$fireBeetleResourceMap at 0x002126a0, 32 bytes, calling the file-local initializer at 0x00212428
// (236 bytes) with (1, 0xffff). With those arguments the initializer makes
// the calls below.
// The file-local initializer and the objects listed here are file-local in the executable unless noted. Argument
// tuples were taken from the image; floats that are not short decimals are given as bit patterns. What the
// particle fields mean is not established here. Fresh compilation is not an original-byte match.
//
//   fuseParticle             0x007d72b0  80 bytes  file-local, defined here
//   smokeParticle            0x007d7300  80 bytes  file-local, defined here
extern "C" {
void __11ParticleDefiiiiiiiifffiiiPFPv_v(void* self, int a, int b, int c, int d, int e, int f, int g, int h,
    float x, float y, float z, int i, int j, int k, void (*callback)(void*));
}

static unsigned char fuseParticle[80];
static unsigned char smokeParticle[80];

static float bits(unsigned value) { return __builtin_bit_cast(float, value); }

static void __static_initialization_and_destruction_0(int initialize, int priority) {
    if (priority != 0xffff) return;
    if (initialize == 1) {
        __11ParticleDefiiiiiiiifffiiiPFPv_v(fuseParticle, 255, 255, 255, 255, 0, 0, 0, 0, 8.0f, 3.0f, -5.0f, 1, 1,
                0, 0);
        __11ParticleDefiiiiiiiifffiiiPFPv_v(smokeParticle, 100, 44, 0, 23, -77, -54, 0, 0, 26.0f, 21.0f,
                bits(0xc07851eb), 2, 0, 5, 0);
    }
}

extern "C" void _GLOBAL_$I$fireBeetleResourceMap() {
    __static_initialization_and_destruction_0(1, 0xffff);
}
