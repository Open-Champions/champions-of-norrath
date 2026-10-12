// Observed _GLOBAL_$I$playerStartPos at 0x001d12b8, 32 bytes. It calls the
// file-local initializer at 0x001d09e0 with (1, 0xffff). With those arguments
// that initializer constructs the global at 0x007d0b20 (trailParticleSpark, 80
// bytes) by calling the ParticleDef constructor with eight ints
// 200, 200, 150, 300, 200, 175, 100, 0, three floats 2.0, 5.0, -20.0, three ints
// 1, 2, 2 and a null callback. The two arguments are kept as in the original
// call; the initializer does nothing for other argument pairs.
extern "C" {
extern unsigned char trailParticleSpark[];
void __11ParticleDefiiiiiiiifffiiiPFPv_v(void* self, int a, int b, int c, int d, int e, int f, int g, int h,
                                         float x, float y, float z, int i, int j, int k, void (*callback)(void*));

static void localInitialize(int initialize, int priority) {
    if (priority == 0xffff) {
        if (initialize == 1) {
            __11ParticleDefiiiiiiiifffiiiPFPv_v(trailParticleSpark, 200, 200, 150, 300, 200, 175, 100, 0, 2.0f,
                                                5.0f, -20.0f, 1, 2, 2, 0);
        }
    }
}

void _GLOBAL_$I$playerStartPos() {
    localInitialize(1, 0xffff);
}
}
