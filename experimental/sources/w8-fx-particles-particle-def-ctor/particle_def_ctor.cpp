// Observed __11ParticleDefiiiiiiiifffiiiPFPv_v at 0x00180d78, 36 bytes: the
// ParticleDef constructor. It stores 0xffffffff in the word at +4 and zero in the
// word at +8, then jumps to setParticleDef__FP11ParticleDefiiiiiiiifffiiiPFPv_v
// (0x00180820) with every register and stack argument unchanged. Its own result
// is the callee's. The arguments are described at that function. Fresh
// compilation is not an original-byte match.
extern "C" {
void setParticleDef__FP11ParticleDefiiiiiiiifffiiiPFPv_v(void*, int, int, int, int, int, int, int, int, float, float,
                                                          float, int, int, int, void (*)(void*));

void __11ParticleDefiiiiiiiifffiiiPFPv_v(void* definition, int start0, int start1, int start2, int start3, int end0,
                                          int end1, int end2, int end3, float p, float q, float r, int mode, int tier,
                                          int grid, void (*callback)(void*)) {
    unsigned char* d = static_cast<unsigned char*>(definition);
    d[4] = 0xffu; d[5] = 0xffu; d[6] = 0xffu; d[7] = 0xffu;
    d[8] = 0u; d[9] = 0u; d[10] = 0u; d[11] = 0u;
    setParticleDef__FP11ParticleDefiiiiiiiifffiiiPFPv_v(definition, start0, start1, start2, start3, end0, end1, end2,
                                                         end3, p, q, r, mode, tier, grid, callback);
}
}
