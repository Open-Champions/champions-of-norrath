// Observed _GLOBAL_$I$spiderResourceMap at 0x0020ff28, 32 bytes, calling the file-local initializer at 0x0020f720
// (228 bytes) with (1, 0xffff). With those arguments the initializer makes
// the calls below.
// The file-local initializer and the objects listed here are file-local in the executable unless noted. Argument
// tuples were taken from the image; floats that are not short decimals are given as bit patterns. What the
// particle fields mean is not established here. Fresh compilation is not an original-byte match.
//
//   ectopileParticle1        0x007d7210  80 bytes  file-local, defined here
//   particlePoisonDrop       0x007d7260  80 bytes  file-local, defined here
extern "C" {
void __11ParticleDefiiiiiiiifffiiiPFPv_v(void* self, int a, int b, int c, int d, int e, int f, int g, int h,
    float x, float y, float z, int i, int j, int k, void (*callback)(void*));
}

static unsigned char ectopileParticle1[80];
static unsigned char particlePoisonDrop[80];

static float bits(unsigned value) { return __builtin_bit_cast(float, value); }

static void __static_initialization_and_destruction_0(int initialize, int priority) {
    if (priority != 0xffff) return;
    if (initialize == 1) {
        __11ParticleDefiiiiiiiifffiiiPFPv_v(ectopileParticle1, 50, 50, 20, 150, 40, 40, 10, 0, bits(0x4203b6ba),
                bits(0x423954a5), bits(0xbf9accf6), 2, 0, 4, 0);
        __11ParticleDefiiiiiiiifffiiiPFPv_v(particlePoisonDrop, 10, 65, 10, 53, 10, 65, 10, 0, 20.5f, 1.0f,
                bits(0xc10a6666), 0, 0, 6, 0);
    }
}

extern "C" void _GLOBAL_$I$spiderResourceMap() {
    __static_initialization_and_destruction_0(1, 0xffff);
}
