// _GLOBAL_$I$getRankDescription__18SkillAncestralCallPUsi at 0x2a8690, 32 bytes, read from the pinned SLUS-20565 image.
// Observed: it calls the translation unit's own static-initialization routine
// (0x2a8330, 132 bytes) with the pair (1, 0xffff). That routine does nothing unless the
// priority is 0xffff, and then constructs 1 ParticleDef-named objects, each only when
// the first argument is 1. Each object is 80 bytes in .bss starting at 0x8d7360;
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

alignas(16) unsigned char norrath_skillancestralcall_getrankdescription_callParticle_bytes[80];

__attribute__((noipa)) static void norrath_static_init(int initialize_p, int priority) {
    if (priority != 0xffff) return;
    if (initialize_p == 1)
        __11ParticleDefiiiiiiiifffiiiPFPv_v(norrath_skillancestralcall_getrankdescription_callParticle_bytes,
            74, 72, 166, 72, 48, 9, 299, 64,
            6.5f, 0.0f, 0.0f,
            2, 1, 7, nullptr);
}

void _GLOBAL_$I$getRankDescription__18SkillAncestralCallPUsi() { norrath_static_init(1, 0xffff); }
}
