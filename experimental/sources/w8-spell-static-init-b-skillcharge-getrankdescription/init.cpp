// _GLOBAL_$I$getRankDescription__11SkillChargePUsi at 0x292158, 32 bytes, read from the pinned SLUS-20565 image.
// Observed: it calls the translation unit's own static-initialization routine
// (0x291e40, 344 bytes) with the pair (1, 0xffff). That routine does nothing unless the
// priority is 0xffff, and then constructs 3 ParticleDef-named objects, each only when
// the first argument is 1. Each object is 80 bytes in .bss starting at 0x8d66e0;
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

alignas(16) unsigned char norrath_skillcharge_getrankdescription_chargeParticle_bytes[80];
alignas(16) unsigned char norrath_skillcharge_getrankdescription_chargeHeat_bytes[80];
alignas(16) unsigned char norrath_skillcharge_getrankdescription_chargeHit_bytes[80];

__attribute__((noipa)) static void norrath_static_init(int initialize_p, int priority) {
    if (priority != 0xffff) return;
    if (initialize_p == 1)
        __11ParticleDefiiiiiiiifffiiiPFPv_v(norrath_skillcharge_getrankdescription_chargeParticle_bytes,
            112, 133, 134, 127, 59, 32, 207, 116,
            22.0f, 0.19999999f, 0.5f,
            2, 1, 8, nullptr);
    if (initialize_p == 1)
        __11ParticleDefiiiiiiiifffiiiPFPv_v(norrath_skillcharge_getrankdescription_chargeHeat_bytes,
            35, 17, 114, 124, 55, 58, 99, 166,
            92.0f, 0.9f, 0x1.333332p-3f,
            2, 3, 8, nullptr);
    if (initialize_p == 1)
        __11ParticleDefiiiiiiiifffiiiPFPv_v(norrath_skillcharge_getrankdescription_chargeHit_bytes,
            112, 133, 207, 127, 207, 32, 32, 116,
            22.0f, 0.0f, -0.5f,
            2, 1, 8, nullptr);
}

void _GLOBAL_$I$getRankDescription__11SkillChargePUsi() { norrath_static_init(1, 0xffff); }
}
