// Observed _GLOBAL_$I$mummyResourceMap at 0x0023c310, 32 bytes, calling the file-local initializer at 0x0023bc68
// (132 bytes) with (1, 0xffff). With those arguments the initializer makes
// the calls below.
// The file-local initializer and the objects listed here are file-local in the executable unless noted. Argument
// tuples were taken from the image; floats that are not short decimals are given as bit patterns. What the
// particle fields mean is not established here. Fresh compilation is not an original-byte match.
//
//   SnakeParticle            0x0042d528  80 bytes  global, defined here (file bytes zero)
extern "C" {
void __11ParticleDefiiiiiiiifffiiiPFPv_v(void* self, int a, int b, int c, int d, int e, int f, int g, int h,
    float x, float y, float z, int i, int j, int k, void (*callback)(void*));
unsigned char SnakeParticle[80];
}

static float bits(unsigned value) { return __builtin_bit_cast(float, value); }

static void __static_initialization_and_destruction_0(int initialize, int priority) {
    if (priority != 0xffff) return;
    if (initialize == 1) {
        __11ParticleDefiiiiiiiifffiiiPFPv_v(SnakeParticle, 5, 78, 5, 180, 0, 5, 0, 60, bits(0x41200000), 1.5f,
                0.0f, 1, 0, 2, 0);
    }
}

extern "C" void _GLOBAL_$I$mummyResourceMap() {
    __static_initialization_and_destruction_0(1, 0xffff);
}
