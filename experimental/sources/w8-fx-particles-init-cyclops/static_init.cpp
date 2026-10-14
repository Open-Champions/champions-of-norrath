// Observed _GLOBAL_$I$cyclopsResourceMap at 0x00203cd8, 32 bytes, calling the file-local initializer at 0x00203b50
// (116 bytes) with (1, 0xffff). With those arguments the initializer makes
// the calls below.
// The file-local initializer and the objects listed here are file-local in the executable unless noted. Argument
// tuples were taken from the image; floats that are not short decimals are given as bit patterns. What the
// particle fields mean is not established here. Fresh compilation is not an original-byte match.
//
//   hitParticleDust2         0x007d70c0  80 bytes  file-local, defined here
extern "C" {
void __11ParticleDefiiiiiiiifffiiiPFPv_v(void* self, int a, int b, int c, int d, int e, int f, int g, int h,
    float x, float y, float z, int i, int j, int k, void (*callback)(void*));
}

static unsigned char hitParticleDust2[80];

static float bits(unsigned value) { return __builtin_bit_cast(float, value); }

static void __static_initialization_and_destruction_0(int initialize, int priority) {
    if (priority != 0xffff) return;
    if (initialize == 1) {
        __11ParticleDefiiiiiiiifffiiiPFPv_v(hitParticleDust2, 92, 89, 64, 20, 92, 85, 64, 0, bits(0x4272b851),
                bits(0x42f11999), bits(0xc041eb85), 2, 0, 2, 0);
    }
}

extern "C" void _GLOBAL_$I$cyclopsResourceMap() {
    __static_initialization_and_destruction_0(1, 0xffff);
}
