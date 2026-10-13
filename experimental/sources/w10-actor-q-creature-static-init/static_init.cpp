// Observed _GLOBAL_$I$numInRegion__8creatureG6Point3f at 0x00113cc0, 32 bytes. It calls the
// file-local initializer at 0x001127b0 (244 bytes) with (1, 0xffff). With those arguments the
// initializer constructs two 80-byte file-local particle definitions with the ParticleDef
// constructor, in this order:
//   trailParticleSpark (0x004e2080) and slamParticle (0x004e20d0).
// Argument tuples were taken from the image; the third float of slamParticle is the literal word
// 0x80000000. There is no matching destructor wrapper. What the particle fields mean is not
// established here. Fresh compilation is not an original-byte match.
extern "C" {
void __11ParticleDefiiiiiiiifffiiiPFPv_v(void* self, int a, int b, int c, int d, int e, int f, int g, int h,
                                         float x, float y, float z, int i, int j, int k, void (*callback)(void*));
}

static unsigned char trailParticleSpark[80];
static unsigned char slamParticle[80];

static void __static_initialization_and_destruction_0(int initialize, int priority) {
    if (priority != 0xffff) return;
    if (initialize != 1) return;
    __11ParticleDefiiiiiiiifffiiiPFPv_v(trailParticleSpark, 200, 200, 150, 300, 200, 175, 100, 0, 2.0f, 5.0f, -20.0f,
                                        1, 2, 2, 0);
    __11ParticleDefiiiiiiiifffiiiPFPv_v(slamParticle, 228, 150, 150, 150, 255, 0, 0, 0, -8.0f, 15.0f,
                                        __builtin_bit_cast(float, 0x80000000u), 2, 2, 6, 0);
}

extern "C" void _GLOBAL_$I$numInRegion__8creatureG6Point3f() {
    __static_initialization_and_destruction_0(1, 0xffff);
}
