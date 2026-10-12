// Observed _GLOBAL_$I$antResourceMap at 0x00208d60, 32 bytes, calling the file-local initializer at 0x00208738
// (228 bytes) with (1, 0xffff). With those arguments the initializer makes
// the calls below.
// The file-local initializer and the objects listed here are file-local in the executable unless noted. Argument
// tuples were taken from the image; floats that are not short decimals are given as bit patterns. What the
// particle fields mean is not established here. Fresh compilation is not an original-byte match.
//
//   ectopileParticle1        0x007d7170  80 bytes  file-local, defined here
//   AntWaveParticle1         0x0042c028  80 bytes  global, defined here (file bytes zero)
extern "C" {
void __11ParticleDefiiiiiiiifffiiiPFPv_v(void* self, int a, int b, int c, int d, int e, int f, int g, int h,
    float x, float y, float z, int i, int j, int k, void (*callback)(void*));
unsigned char AntWaveParticle1[80];
}

static unsigned char ectopileParticle1[80];

static float bits(unsigned value) { return __builtin_bit_cast(float, value); }

static void __static_initialization_and_destruction_0(int initialize, int priority) {
    if (priority != 0xffff) return;
    if (initialize == 1) {
        __11ParticleDefiiiiiiiifffiiiPFPv_v(ectopileParticle1, 50, 50, 20, 150, 40, 40, 10, 0, bits(0x4203b6ba),
                bits(0x423954a5), bits(0xbf9accf6), 2, 0, 4, 0);
        __11ParticleDefiiiiiiiifffiiiPFPv_v(AntWaveParticle1, 2, 83, 74, 224, 31, 43, 31, 214, bits(0x42a4570a),
                bits(0x42956d91), 0.0f, 1, 3, 2, 0);
    }
}

extern "C" void _GLOBAL_$I$antResourceMap() {
    __static_initialization_and_destruction_0(1, 0xffff);
}
