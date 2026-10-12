// _GLOBAL_$I$msg_run__12DiseaseTrail at 0x2955a0, 32 bytes, read from the pinned SLUS-20565 image.
// Observed: it calls the translation unit's own static-initialization routine
// (0x295168, 332 bytes) with the pair (1, 0xffff). That routine does nothing unless the
// priority is 0xffff, and then constructs 3 ParticleDef-named objects, each only when
// the first argument is 1. Each object is 80 bytes in .bss starting at 0x8d68c0;
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

alignas(16) unsigned char norrath_diseasetrail_msg_run_diseaseTrailParticle1_bytes[80];
alignas(16) unsigned char norrath_diseasetrail_msg_run_diseaseTrailParticle2_bytes[80];
alignas(16) unsigned char norrath_diseasetrail_msg_run_diseaseTrailParticle3_bytes[80];

__attribute__((noipa)) static void norrath_static_init(int initialize_p, int priority) {
    if (priority != 0xffff) return;
    if (initialize_p == 1)
        __11ParticleDefiiiiiiiifffiiiPFPv_v(norrath_diseasetrail_msg_run_diseaseTrailParticle1_bytes,
            110, 100, 23, 100, 78, 66, 30, 5,
            35.0f, 45.0f, 0.0f,
            2, 0, 2, nullptr);
    if (initialize_p == 1)
        __11ParticleDefiiiiiiiifffiiiPFPv_v(norrath_diseasetrail_msg_run_diseaseTrailParticle2_bytes,
            12, 12, 0, 78, 8, 40, 16, 5,
            20.0f, 15.0f, 0.0f,
            3, 0, 8, nullptr);
    if (initialize_p == 1)
        __11ParticleDefiiiiiiiifffiiiPFPv_v(norrath_diseasetrail_msg_run_diseaseTrailParticle3_bytes,
            0, 0, 0, 254, 25, 25, 25, 0,
            5.0f, 5.0f, 0.0f,
            2, 0, 9, nullptr);
}

void _GLOBAL_$I$msg_run__12DiseaseTrail() { norrath_static_init(1, 0xffff); }
}
