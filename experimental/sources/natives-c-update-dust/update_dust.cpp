// Observed fxUpdateDust__Fiiiiiiii at 0x001e6410, 128 bytes. All eight
// integers are forwarded to P_UpdateParticleType with fxDust. The three
// floats are the .lit4 word 0x41a4cccc, 9.5, and -5. The trailing words
// are 2, 0, 2, and a null callback.
extern "C" void P_UpdateParticleType__FP11ParticleDefiiiiiiiifffiiiPFPv_v(
    void* definition, int first, int second, int third, int fourth, int fifth,
    int sixth, int seventh, int eighth, float amount, float rise, float sink,
    int mode, int unused, int repeat, void (*callback)(void*));
extern unsigned char fxDust[];

namespace {
float bits_to_float(unsigned bits) {
    float value;
    auto* out = reinterpret_cast<unsigned char*>(&value);
    out[0] = static_cast<unsigned char>(bits);
    out[1] = static_cast<unsigned char>(bits >> 8);
    out[2] = static_cast<unsigned char>(bits >> 16);
    out[3] = static_cast<unsigned char>(bits >> 24);
    return value;
}
}

extern "C" void fxUpdateDust__Fiiiiiiii(int first, int second, int third, int fourth,
                                        int fifth, int sixth, int seventh, int eighth) {
    P_UpdateParticleType__FP11ParticleDefiiiiiiiifffiiiPFPv_v(
        fxDust, first, second, third, fourth, fifth, sixth, seventh, eighth,
        bits_to_float(0x41a4ccccu), bits_to_float(0x41180000u), bits_to_float(0xc0a00000u),
        2, 0, 2, nullptr);
}
