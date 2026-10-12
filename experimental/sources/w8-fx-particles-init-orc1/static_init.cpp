// Observed _GLOBAL_$I$orc1ResourceMap at 0x0022bad0, 32 bytes, calling the file-local initializer at 0x0022b440
// (268 bytes) with (1, 0xffff). With those arguments the initializer makes
// the calls below; _GLOBAL_$D$orc1ResourceMap at 0x0022baf0 calls the same initializer with (0, 0xffff).
// The file-local initializer and the objects listed here are file-local in the executable unless noted. Argument
// tuples were taken from the image; floats that are not short decimals are given as bit patterns. What the
// particle fields mean is not established here. Fresh compilation is not an original-byte match.
//
//   slamParticle             0x007d7af0  80 bytes  file-local, defined here
extern "C" {
extern unsigned _3Orc$m_commanderList[];  // 0x0042ce10, 20 words, ListHead entries; defined elsewhere (class static member)
void __11ParticleDefiiiiiiiifffiiiPFPv_v(void* self, int a, int b, int c, int d, int e, int f, int g, int h,
    float x, float y, float z, int i, int j, int k, void (*callback)(void*));
void listDestroy__FP8ListHead(void* head);
}

static unsigned char slamParticle[80];

static void __static_initialization_and_destruction_0(int initialize, int priority) {
    if (priority != 0xffff) return;
    if (initialize == 1) {
        for (unsigned n = 0u; n < 20u; ++n) _3Orc$m_commanderList[n] = 0u;
        __11ParticleDefiiiiiiiifffiiiPFPv_v(slamParticle, 0, 15, 225, 255, 0, 5, 150, 0, -8.0f, 15.0f, -0.0f, 2, 2,
                6, 0);
    }
    if (initialize == 0) {
        for (unsigned n = 20u; n-- > 0u;) listDestroy__FP8ListHead(&_3Orc$m_commanderList[n]);
    }
}

extern "C" void _GLOBAL_$I$orc1ResourceMap() {
    __static_initialization_and_destruction_0(1, 0xffff);
}

extern "C" void _GLOBAL_$D$orc1ResourceMap() {
    __static_initialization_and_destruction_0(0, 0xffff);
}
