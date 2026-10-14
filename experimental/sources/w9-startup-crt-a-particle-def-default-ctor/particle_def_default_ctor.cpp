// Observed __11ParticleDef at 0x00180d08, 112 bytes: the ParticleDef default constructor (object address as the
// only argument). It stores 0xffffffff in the word at +4 and zero in the word at +8, then calls
// setParticleDef__FP11ParticleDefiiiiiiiifffiiiPFPv_v (0x00180820) with the object, eight integer arguments that are
// all zero, the three floats 1.0, 1.0 and 0.0, the three integers 3, 1 and 1, and a null callback. The word stores
// happen before that call starts. Nothing is returned. The argument names follow the sibling constructor
// (w8-fx-particles-particle-def-ctor): the eight zeros are the start and end quadruples. Why these defaults, and
// what the object's other words hold, are not established here. Fresh compilation is not an original-byte match.
extern "C" {
void setParticleDef__FP11ParticleDefiiiiiiiifffiiiPFPv_v(void*, int, int, int, int, int, int, int, int, float, float,
                                                          float, int, int, int, void (*)(void*));

void __11ParticleDef(void* definition) {
    int* words = static_cast<int*>(definition);
    words[1] = -1;
    words[2] = 0;
    setParticleDef__FP11ParticleDefiiiiiiiifffiiiPFPv_v(definition, 0, 0, 0, 0, 0, 0, 0, 0, 1.0f, 1.0f, 0.0f, 3, 1, 1,
                                                         nullptr);
}
}
