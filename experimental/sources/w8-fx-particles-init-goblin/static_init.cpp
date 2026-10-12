// Observed _GLOBAL_$I$goblinResourceMap at 0x0022f570, 32 bytes, calling the file-local initializer at 0x0022ef00
// (128 bytes) with (1, 0xffff). With those arguments the initializer makes
// the calls below.
// The file-local initializer and the objects listed here are file-local in the executable unless noted. Argument
// tuples were taken from the image; floats that are not short decimals are given as bit patterns. What the
// particle fields mean is not established here. Fresh compilation is not an original-byte match.
//
//   iceballParticle1         0x007d7b50  80 bytes  file-local, defined here
extern "C" {
void __11ParticleDefiiiiiiiifffiiiPFPv_v(void* self, int a, int b, int c, int d, int e, int f, int g, int h,
    float x, float y, float z, int i, int j, int k, void (*callback)(void*));
}

static unsigned char iceballParticle1[80];

static float bits(unsigned value) { return __builtin_bit_cast(float, value); }

static void __static_initialization_and_destruction_0(int initialize, int priority) {
    if (priority != 0xffff) return;
    if (initialize == 1) {
        __11ParticleDefiiiiiiiifffiiiPFPv_v(iceballParticle1, 100, 210, 238, 40, 4, 63, 105, 0, bits(0x42480000),
                bits(0x42c80000), 0.0f, 1, 1, 4, 0);
    }
}

extern "C" void _GLOBAL_$I$goblinResourceMap() {
    __static_initialization_and_destruction_0(1, 0xffff);
}
