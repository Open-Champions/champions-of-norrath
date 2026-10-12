// _GLOBAL_$I$cast__9SkillRootiP8CreatureG6Point3s at 0x2ad178, 32 bytes, read from the pinned SLUS-20565 image.
// Observed: it calls the translation unit's own static-initialization routine
// (0x2acd38, 332 bytes) with the pair (1, 0xffff). That routine does nothing unless the
// priority is 0xffff, and then constructs 3 ParticleDef-named objects, each only when
// the first argument is 1. Each object is 80 bytes in .bss starting at 0x8d75e0;
// the argument words, floats and trailing counts below are the values the image
// passes, in constructor order. The callback argument is null in every call.
// Inferred: the object type from the constructor's mangled name. The storage is
// given external names here because the code that reads these objects lives in
// other units; the original names are local symbols in the image.
// Unknown: which fields the 14 numeric arguments set, and who reads the objects.
// Fresh compilation is not an original-byte match; nothing here has run on the EE.
extern "C" {
void __11ParticleDefiiiiiiiifffiiiPFPv_v(void* self,
    int a, int b, int c, int d, int e, int f, int g, int h,
    float x, float y, float z, int i, int j, int k, void (*callback)(void*));

alignas(16) unsigned char norrath_skillroot_cast_roots_bytes[80];
alignas(16) unsigned char norrath_skillroot_cast_magic_bytes[80];
alignas(16) unsigned char norrath_skillroot_cast_miss_bytes[80];

__attribute__((noipa)) static void norrath_static_init(int initialize_p, int priority) {
    if (priority != 0xffff) return;
    if (initialize_p == 1)
        __11ParticleDefiiiiiiiifffiiiPFPv_v(norrath_skillroot_cast_roots_bytes,
            2, 85, 2, 114, 0, 80, 0, 99,
            12.0f, 11.0f, -0x1.999998p+0f,
            3, 2, 2, nullptr);
    if (initialize_p == 1)
        __11ParticleDefiiiiiiiifffiiiPFPv_v(norrath_skillroot_cast_magic_bytes,
            42, 52, 217, 50, 255, 285, 0, 43,
            28.0f, 0.0f, 24.0f,
            1, 1, 8, nullptr);
    if (initialize_p == 1)
        __11ParticleDefiiiiiiiifffiiiPFPv_v(norrath_skillroot_cast_miss_bytes,
            105, 100, 2, 139, 121, 23, 0, 0,
            12.0f, 12.0f, -0x1.999998p+0f,
            1, 2, 8, nullptr);
}

void _GLOBAL_$I$cast__9SkillRootiP8CreatureG6Point3s() { norrath_static_init(1, 0xffff); }
}
