// _GLOBAL_$I$msg_run__9FireFlare at 0x2a49d8, 32 bytes, read from the pinned SLUS-20565 image.
// Observed: it calls the translation unit's own static-initialization routine
// (0x2a4528, 328 bytes) with the pair (1, 0xffff). That routine does nothing unless the
// priority is 0xffff, and then constructs 3 ParticleDef-named objects, each only when
// the first argument is 1. Each object is 80 bytes in .bss starting at 0x8d70e0;
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

alignas(16) unsigned char norrath_fireflare_msg_run_fireflaresParticle1_bytes[80];
alignas(16) unsigned char norrath_fireflare_msg_run_fireflaresParticle2_bytes[80];
alignas(16) unsigned char norrath_fireflare_msg_run_fireflaresParticle3_bytes[80];

__attribute__((noipa)) static void norrath_static_init(int initialize_p, int priority) {
    if (priority != 0xffff) return;
    if (initialize_p == 1)
        __11ParticleDefiiiiiiiifffiiiPFPv_v(norrath_fireflare_msg_run_fireflaresParticle1_bytes,
            295, 195, 111, 413, 226, 11, 14, -11,
            4.0f, 6.0f, 0.0f,
            2, 1, 0, nullptr);
    if (initialize_p == 1)
        __11ParticleDefiiiiiiiifffiiiPFPv_v(norrath_fireflare_msg_run_fireflaresParticle2_bytes,
            130, 116, 76, 87, 194, 120, 74, 0,
            32.0f, 5.0f, 0.0f,
            1, 1, 2, nullptr);
    if (initialize_p == 1)
        __11ParticleDefiiiiiiiifffiiiPFPv_v(norrath_fireflare_msg_run_fireflaresParticle3_bytes,
            0, 0, 0, 64, 34, 30, 21, 0,
            30.0f, 50.0f, 3.0f,
            1, 0, 6, nullptr);
}

void _GLOBAL_$I$msg_run__9FireFlare() { norrath_static_init(1, 0xffff); }
}
