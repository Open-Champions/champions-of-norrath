// Observed _GLOBAL_$I$scriptParticles at 0x0013c508, 32 bytes, calling the file-local initializer at 0x0013a578
// (296 bytes) with (1, 0xffff). With those arguments the initializer makes
// the calls below.
// The file-local initializer and the objects listed here are file-local in the executable unless noted. Argument
// tuples were taken from the image; floats that are not short decimals are given as bit patterns. What the
// particle fields mean is not established here. Fresh compilation is not an original-byte match.
//
//   scriptParticle0          0x004f3f20  80 bytes  defined elsewhere in the draft
//   scriptParticle1          0x004f3f70  80 bytes  defined elsewhere in the draft
//   scriptParticle2          0x004f3fc0  80 bytes  defined elsewhere in the draft
extern "C" {
void __11ParticleDefiiiiiiiifffiiiPFPv_v(void* self, int a, int b, int c, int d, int e, int f, int g, int h,
    float x, float y, float z, int i, int j, int k, void (*callback)(void*));
extern unsigned char scriptParticle0[];
extern unsigned char scriptParticle1[];
extern unsigned char scriptParticle2[];
}

static void __static_initialization_and_destruction_0(int initialize, int priority) {
    if (priority != 0xffff) return;
    if (initialize == 1) {
        __11ParticleDefiiiiiiiifffiiiPFPv_v(scriptParticle0, 0, 0, 0, 0, 0, 0, 0, 0, 0.0f, 0.0f, 0.0f, 0, 0, 2, 0);
        __11ParticleDefiiiiiiiifffiiiPFPv_v(scriptParticle1, 0, 0, 0, 0, 0, 0, 0, 0, 0.0f, 0.0f, 0.0f, 0, 0, 2, 0);
        __11ParticleDefiiiiiiiifffiiiPFPv_v(scriptParticle2, 0, 0, 0, 0, 0, 0, 0, 0, 0.0f, 0.0f, 0.0f, 0, 0, 2, 0);
    }
}

extern "C" void _GLOBAL_$I$scriptParticles() {
    __static_initialization_and_destruction_0(1, 0xffff);
}
