// _GLOBAL_$I$move__10ProjectileG6Point3 at 0x155fa0, 32 bytes, read from the pinned SLUS-20565 image.
// Observed: it calls the translation unit's own static-initialization routine
// (0x153940, 1636 bytes) with the pair (1, 0xffff). That routine does nothing unless the
// priority is 0xffff, and then constructs 15 ParticleDef-named objects (80 bytes each) and
// 4 ResourceMap-named objects (16 bytes each, built from two file names), each only when
// the first argument is 1. The storage is in .bss starting at 0x600b40;
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

void __11ResourceMapPcT1(void* self, const char* first, const char* second);

alignas(16) unsigned char norrath_projectile_move_trailParticleFire_bytes[80];
alignas(16) unsigned char norrath_projectile_move_trailParticleSmoke_bytes[80];
alignas(16) unsigned char norrath_projectile_move_ballSparkParticle_bytes[80];
alignas(16) unsigned char norrath_projectile_move_snowParticle1_bytes[80];
alignas(16) unsigned char norrath_projectile_move_trailParticleSpark_bytes[80];
alignas(16) unsigned char norrath_projectile_move_trailParticleFire2_bytes[80];
alignas(16) unsigned char norrath_projectile_move_goldAnm_bytes[16];
alignas(16) unsigned char norrath_projectile_move_goldModel_bytes[16];
alignas(16) unsigned char norrath_projectile_move_goldTex_bytes[16];
alignas(16) unsigned char norrath_projectile_move_goldRestModel_bytes[16];
alignas(16) unsigned char norrath_projectile_move_waveMainParticle_bytes[80];
alignas(16) unsigned char norrath_projectile_move_waveMainAlternate_bytes[80];
alignas(16) unsigned char norrath_projectile_move_waveRandomFoam_bytes[80];
alignas(16) unsigned char norrath_projectile_move_waveTrail_bytes[80];
alignas(16) unsigned char norrath_projectile_move_waveSplashBig_bytes[80];
alignas(16) unsigned char norrath_projectile_move_waveSplashSmall_bytes[80];
alignas(16) unsigned char norrath_projectile_move_waveFroth_bytes[80];
alignas(16) unsigned char norrath_projectile_move_morpheffectParticle1_bytes[80];
alignas(16) unsigned char norrath_projectile_move_morpheffectParticle2_bytes[80];

__attribute__((noipa)) static void norrath_static_init(int initialize_p, int priority) {
    if (priority != 0xffff) return;
    if (initialize_p == 1)
        __11ParticleDefiiiiiiiifffiiiPFPv_v(norrath_projectile_move_trailParticleFire_bytes,
            238, 210, 100, 180, 105, 63, 4, -100,
            15.0f, -2.5f, 0.95f,
            1, 1, 0, nullptr);
    if (initialize_p == 1)
        __11ParticleDefiiiiiiiifffiiiPFPv_v(norrath_projectile_move_trailParticleSmoke_bytes,
            100, 44, 0, 23, -77, -54, 0, 0,
            26.0f, 30.0f, 0.0f,
            2, 0, 5, nullptr);
    if (initialize_p == 1)
        __11ParticleDefiiiiiiiifffiiiPFPv_v(norrath_projectile_move_ballSparkParticle_bytes,
            100, 111, 358, 128, 122, 106, 349, 0,
            5.9399996f, 8.57f, -9.32f,
            1, 2, 0, nullptr);
    if (initialize_p == 1)
        __11ParticleDefiiiiiiiifffiiiPFPv_v(norrath_projectile_move_snowParticle1_bytes,
            100, 210, 238, 90, 4, 63, 105, 0,
            2.81f, 2.875f, -2.0f,
            3, 1, 4, nullptr);
    if (initialize_p == 1)
        __11ParticleDefiiiiiiiifffiiiPFPv_v(norrath_projectile_move_trailParticleSpark_bytes,
            200, 200, 150, 300, 200, 175, 100, 0,
            2.0f, 5.0f, -20.0f,
            1, 2, 2, nullptr);
    if (initialize_p == 1)
        __11ParticleDefiiiiiiiifffiiiPFPv_v(norrath_projectile_move_trailParticleFire2_bytes,
            238, 210, 100, 180, 105, 63, 4, 0,
            30.0f, 5.0f, 0.95f,
            2, 1, 0, nullptr);
    if (initialize_p == 1)
        __11ResourceMapPcT1(norrath_projectile_move_goldAnm_bytes, "fx.lmp", "goldpile_fall.anm");
    if (initialize_p == 1)
        __11ResourceMapPcT1(norrath_projectile_move_goldModel_bytes, "fx.lmp", "goldpile.vif");
    if (initialize_p == 1)
        __11ResourceMapPcT1(norrath_projectile_move_goldTex_bytes, "fx.lmp", "goldpile.tex");
    if (initialize_p == 1)
        __11ResourceMapPcT1(norrath_projectile_move_goldRestModel_bytes, "fx.lmp", "goldpile_rest.vif");
    if (initialize_p == 1)
        __11ParticleDefiiiiiiiifffiiiPFPv_v(norrath_projectile_move_waveMainParticle_bytes,
            103, 118, 143, 80, 99, 116, 121, 24,
            26.78233f, 83.756325f, -0.746956f,
            1, 0, 2, nullptr);
    if (initialize_p == 1)
        __11ParticleDefiiiiiiiifffiiiPFPv_v(norrath_projectile_move_waveMainAlternate_bytes,
            65, 80, 93, 84, 103, 148, 111, 15,
            71.0f, 182.61461f, -0x1.c2cfb6p+1f,
            2, 0, 2, nullptr);
    if (initialize_p == 1)
        __11ParticleDefiiiiiiiifffiiiPFPv_v(norrath_projectile_move_waveRandomFoam_bytes,
            79, 85, 74, 62, 67, 99, 89, 0,
            28.053904f, 146.48247f, -0x1.ca9f5p+1f,
            3, 1, 2, nullptr);
    if (initialize_p == 1)
        __11ParticleDefiiiiiiiifffiiiPFPv_v(norrath_projectile_move_waveTrail_bytes,
            118, 127, 128, 81, 129, 132, 137, 0,
            8.57582f, 5.72149563f, 0.0f,
            3, 0, 2, nullptr);
    if (initialize_p == 1)
        __11ParticleDefiiiiiiiifffiiiPFPv_v(norrath_projectile_move_waveSplashBig_bytes,
            104, 103, 130, 93, 72, 76, 74, 0,
            7.0f, 20.0f, -6.0f,
            3, 1, 2, nullptr);
    if (initialize_p == 1)
        __11ParticleDefiiiiiiiifffiiiPFPv_v(norrath_projectile_move_waveSplashSmall_bytes,
            77, 67, 93, 85, 72, 76, 74, 0,
            7.0f, 20.0f, -1.351967f,
            2, 1, 2, nullptr);
    if (initialize_p == 1)
        __11ParticleDefiiiiiiiifffiiiPFPv_v(norrath_projectile_move_waveFroth_bytes,
            123, 124, 127, 97, 82, 84, 83, 0,
            3.518987f, 20.505774f, 0.0f,
            3, 1, 5, nullptr);
    if (initialize_p == 1)
        __11ParticleDefiiiiiiiifffiiiPFPv_v(norrath_projectile_move_morpheffectParticle1_bytes,
            220, 20, 0, 150, 220, 20, 0, 150,
            15.0f, 15.0f, 0.0f,
            2, 1, 8, nullptr);
    if (initialize_p == 1)
        __11ParticleDefiiiiiiiifffiiiPFPv_v(norrath_projectile_move_morpheffectParticle2_bytes,
            255, 255, 255, 255, 255, 255, 255, 255,
            30.0f, 0.0f, 0.0f,
            0, 1, 8, nullptr);
}

void _GLOBAL_$I$move__10ProjectileG6Point3() { norrath_static_init(1, 0xffff); }
}
