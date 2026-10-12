// _GLOBAL_$I$getRankDescription__16SkillPoisonArrowPUsi at 0x2a0d38, 32 bytes, read from the pinned SLUS-20565 image.
// Observed: it calls the translation unit's own static-initialization routine
// (0x2a0a30, 332 bytes) with the pair (1, 0xffff). That routine does nothing unless the
// priority is 0xffff, and then constructs 3 ParticleDef-named objects, each only when
// the first argument is 1. Each object is 80 bytes in .bss starting at 0x8d6f50;
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

alignas(16) unsigned char norrath_skillpoisonarrow_getrankdescription_poisonSmoke_bytes[80];
alignas(16) unsigned char norrath_skillpoisonarrow_getrankdescription_poisonFleck_bytes[80];
alignas(16) unsigned char norrath_skillpoisonarrow_getrankdescription_poisonPoof_bytes[80];

__attribute__((noipa)) static void norrath_static_init(int initialize_p, int priority) {
    if (priority != 0xffff) return;
    if (initialize_p == 1)
        __11ParticleDefiiiiiiiifffiiiPFPv_v(norrath_skillpoisonarrow_getrankdescription_poisonSmoke_bytes,
            41, 107, 5, 104, 28, 100, 49, 0,
            38.0f, 54.199997f, 0.9f,
            0, 1, 2, nullptr);
    if (initialize_p == 1)
        __11ParticleDefiiiiiiiifffiiiPFPv_v(norrath_skillpoisonarrow_getrankdescription_poisonFleck_bytes,
            103, 119, 31, 100, 9, 9, 9, 33,
            7.0f, 5.0f, 0.0f,
            1, 2, 6, nullptr);
    if (initialize_p == 1)
        __11ParticleDefiiiiiiiifffiiiPFPv_v(norrath_skillpoisonarrow_getrankdescription_poisonPoof_bytes,
            58, 127, 0, 143, 38, 141, 3, 11,
            9.0f, 18.199999f, 0.0f,
            3, 2, 6, nullptr);
}

void _GLOBAL_$I$getRankDescription__16SkillPoisonArrowPUsi() { norrath_static_init(1, 0xffff); }
}
