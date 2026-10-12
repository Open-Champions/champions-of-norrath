// Observed _GLOBAL_$I$worldTileEntityLightScale at 0x0017dd70 and
// _GLOBAL_$D$worldTileEntityLightScale at 0x0017dd90, 32 bytes each. They call
// the translation unit's file-local static initialization routine at 0x0017ce90
// (844 bytes) with (1, 0xffff) and (0, 0xffff) respectively. Both are listed
// among the static constructors and destructors of the image.
// The routine does nothing unless its second argument is 0xffff. With the first
// argument 1 it runs, in this order:
//  1. ParticleDef::ParticleDef() (0x00180d08) on the +72 member of each of the 16
//     152-byte records of ParticleEmitterArray (0x006e93e0), first to last;
//  2. clears bytes +16 and +17 of each of the 16 68-byte records of
//     DynamicLightArray (0x006e9d60), which is what the Light constructor does to
//     the member at +8;
//  3. clears the word at +40 of each of the 128 48-byte records of routeQueue
//     (0x006ea1a0);
//  4. the full ParticleDef constructor (0x00180d78) on droplet (0x006ebaa0), then
//     on rainSplashParticle (0x006ebaf0);
//  5. sets byte +5 to 1 in each of the 64 48-byte records of worldAnimation
//     (0x006ebb40);
//  6. the full ParticleDef constructor on ParticleEmitterTestParticle (0x006ec740).
// (The first argument 0 path is added with the destruction wrapper.)
// The constructor arguments are listed at the calls below; three floats of the
// first one come from the unit's constant pool (10.68, 8.55, -60.03).
// Inferred: the unit's globals are the objects above; their ParticleDef names
// are the image symbols. Which constructor argument is which field is not
// established. listDestroy is the already published norrath_list_destroy.
// Unknown: worldTileEntityLightScale, the object the wrapper names, has no
// dynamic initialization in this routine.
extern "C" {
extern unsigned char ParticleEmitterArray[];
extern unsigned char DynamicLightArray[];
extern unsigned char routeQueue[];
extern unsigned char droplet[];
extern unsigned char rainSplashParticle[];
extern unsigned char worldAnimation[];
extern unsigned char ParticleEmitterTestParticle[];

void __11ParticleDef(void* self);
void __11ParticleDefiiiiiiiifffiiiPFPv_v(void* self, int a, int b, int c, int d, int e, int f, int g, int h, float x,
                                         float y, float z, int i, int j, int k, void (*callback)(void*));
}

#ifdef __clang__
#define NORRATH_W8_NOIPA
#else
#define NORRATH_W8_NOIPA __attribute__((noipa))
#endif

static float float_bits(unsigned bits) {
    return __builtin_bit_cast(float, bits);
}

static void NORRATH_W8_NOIPA static_initialization(int constructing, int priority) {
    if (priority != 0xffff) return;
    if (constructing == 1) {
        for (int index = 0; index < 16; ++index) __11ParticleDef(ParticleEmitterArray + 152 * index + 72);
        for (int index = 0; index < 16; ++index) {
            DynamicLightArray[68 * index + 17] = 0;
            DynamicLightArray[68 * index + 16] = 0;
        }
        for (int index = 0; index < 128; ++index) *reinterpret_cast<unsigned*>(routeQueue + 48 * index + 40) = 0;
        __11ParticleDefiiiiiiiifffiiiPFPv_v(droplet, 70, 70, 150, 50, 60, 60, 170, 230, float_bits(0x412ae147u),
                                            float_bits(0x4108ccccu), float_bits(0xc2701eb8u), 3, 2, 0, nullptr);
        __11ParticleDefiiiiiiiifffiiiPFPv_v(rainSplashParticle, 50, 50, 100, 100, 170, 200, 250, 0, 1.0f, 10.0f, 0.0f, 1,
                                            1, 7, nullptr);
        for (int index = 0; index < 64; ++index) worldAnimation[48 * index + 5] = 1;
        __11ParticleDefiiiiiiiifffiiiPFPv_v(ParticleEmitterTestParticle, 130, 130, 130, 0, 100, 100, 100, 205, 5.0f,
                                            5.0f, 1.0f, 3, 1, 4, nullptr);
    }
}

extern "C" void _GLOBAL_$I$worldTileEntityLightScale() {
    static_initialization(1, 0xffff);
}
