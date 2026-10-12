// Observed _GLOBAL_$I$soulResourceMap at 0x001f5030, 32 bytes, calling the file-local initializer at 0x001f4e60
// (132 bytes) with (1, 0xffff). With those arguments the initializer makes
// the calls below.
// The file-local initializer and the objects listed here are file-local in the executable unless noted. Argument
// tuples were taken from the image; floats that are not short decimals are given as bit patterns. What the
// particle fields mean is not established here. Fresh compilation is not an original-byte match.
//
//   soulParticle             0x007d6d80  80 bytes  file-local, defined here
extern "C" {
void __11ParticleDefiiiiiiiifffiiiPFPv_v(void* self, int a, int b, int c, int d, int e, int f, int g, int h,
    float x, float y, float z, int i, int j, int k, void (*callback)(void*));
}

static unsigned char soulParticle[80];

static float bits(unsigned value) { return __builtin_bit_cast(float, value); }

static void __static_initialization_and_destruction_0(int initialize, int priority) {
    if (priority != 0xffff) return;
    if (initialize == 1) {
        __11ParticleDefiiiiiiiifffiiiPFPv_v(soulParticle, 25, 36, 87, 125, 22, 24, 143, 24, bits(0x4208ae14),
                bits(0x3f36c8b4), 5.0f, 3, 1, 8, 0);
    }
}

extern "C" void _GLOBAL_$I$soulResourceMap() {
    __static_initialization_and_destruction_0(1, 0xffff);
}
