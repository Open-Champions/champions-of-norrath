// _GLOBAL_$I$getRankDescription__16SkillGroundPoundPUsi at 0x290b00, 32 bytes, read from the pinned SLUS-20565 image.
// Observed: it calls the translation unit's own static-initialization routine
// (0x2905a0, 740 bytes) with the pair (1, 0xffff). That routine does nothing unless the
// priority is 0xffff, and then constructs 7 ParticleDef-named objects, each only when
// the first argument is 1. Each object is 80 bytes in .bss starting at 0x8d64b0;
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

alignas(16) unsigned char norrath_skillgroundpound_getrankdescription_dustParticle_bytes[80];
alignas(16) unsigned char norrath_skillgroundpound_getrankdescription_sparkParticle_bytes[80];
alignas(16) unsigned char norrath_skillgroundpound_getrankdescription_smokeParticle_bytes[80];
alignas(16) unsigned char norrath_skillgroundpound_getrankdescription_splashParticle_bytes[80];
alignas(16) unsigned char norrath_skillgroundpound_getrankdescription_groundpoundParticle0_bytes[80];
alignas(16) unsigned char norrath_skillgroundpound_getrankdescription_groundpoundParticle1_bytes[80];
alignas(16) unsigned char norrath_skillgroundpound_getrankdescription_groundpoundParticle2_bytes[80];

__attribute__((noipa)) static void norrath_static_init(int initialize_p, int priority) {
    if (priority != 0xffff) return;
    if (initialize_p == 1)
        __11ParticleDefiiiiiiiifffiiiPFPv_v(norrath_skillgroundpound_getrankdescription_dustParticle_bytes,
            74, 80, 141, 119, 44, 55, 297, 0,
            21.0f, 0.19999999f, -6.0f,
            3, 1, 9, nullptr);
    if (initialize_p == 1)
        __11ParticleDefiiiiiiiifffiiiPFPv_v(norrath_skillgroundpound_getrankdescription_sparkParticle_bytes,
            283, 164, 79, 127, 148, 100, 52, 0,
            22.0f, 1.3f, -6.0f,
            3, 1, 8, nullptr);
    if (initialize_p == 1)
        __11ParticleDefiiiiiiiifffiiiPFPv_v(norrath_skillgroundpound_getrankdescription_smokeParticle_bytes,
            87, 58, 31, 191, 9, 9, 9, 0,
            26.0f, 55.0f, 1.5f,
            3, 0, 6, nullptr);
    if (initialize_p == 1)
        __11ParticleDefiiiiiiiifffiiiPFPv_v(norrath_skillgroundpound_getrankdescription_splashParticle_bytes,
            138, 163, 141, 93, 111, 161, 150, 0,
            17.0f, 0x1.999998p-4f, -6.89999962f,
            3, 0, 6, nullptr);
    if (initialize_p == 1)
        __11ParticleDefiiiiiiiifffiiiPFPv_v(norrath_skillgroundpound_getrankdescription_groundpoundParticle0_bytes,
            255, 0, 0, 255, 255, 0, 0, 255,
            80.0f, 30.0f, 0.0f,
            0, 3, 6, nullptr);
    if (initialize_p == 1)
        __11ParticleDefiiiiiiiifffiiiPFPv_v(norrath_skillgroundpound_getrankdescription_groundpoundParticle1_bytes,
            255, 0, 0, 255, 255, 0, 0, 255,
            80.0f, 30.0f, 0.0f,
            1, 3, 6, nullptr);
    if (initialize_p == 1)
        __11ParticleDefiiiiiiiifffiiiPFPv_v(norrath_skillgroundpound_getrankdescription_groundpoundParticle2_bytes,
            255, 0, 0, 255, 255, 0, 0, 255,
            80.0f, 30.0f, 0.0f,
            2, 3, 6, nullptr);
}

void _GLOBAL_$I$getRankDescription__16SkillGroundPoundPUsi() { norrath_static_init(1, 0xffff); }
}
