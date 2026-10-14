// Observed amxSetParticle__FP5__amxPi at 0x0013c108, 140 bytes. Cell 1
// selects a pointer from scriptParticles. Cells 2 through 9 are the eight
// integer arguments. Cells 10 through 12 are converted from signed words to
// floats. Cells 13 through 15 are the trailing integers, and the callback
// is null. The native returns 0.
extern "C" void* scriptParticles[];
extern "C" void P_UpdateParticleType__FP11ParticleDefiiiiiiiifffiiiPFPv_v(
    void* definition, int first, int second, int third, int fourth, int fifth,
    int sixth, int seventh, int eighth, float amount, float rise, float sink,
    int mode, int extra, int repeat, void (*callback)(void*));

extern "C" int amxSetParticle__FP5__amxPi(void*, int* params) {
    const unsigned index = static_cast<unsigned>(params[1]);
    P_UpdateParticleType__FP11ParticleDefiiiiiiiifffiiiPFPv_v(
        scriptParticles[index], params[2], params[3], params[4], params[5], params[6],
        params[7], params[8], params[9], static_cast<float>(params[10]),
        static_cast<float>(params[11]), static_cast<float>(params[12]), params[13],
        params[14], params[15], nullptr);
    return 0;
}
