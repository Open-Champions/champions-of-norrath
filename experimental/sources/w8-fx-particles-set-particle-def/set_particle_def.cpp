// Observed setParticleDef__FP11ParticleDefiiiiiiiifffiiiPFPv_v at 0x00180820,
// 668 bytes (a local symbol in the executable; published here with external
// linkage because other units call it by this name). Arguments in order: the
// definition address, eight integers (call them start0..3 and end0..3), three
// floats (p, q, r), three integers (mode, tier, grid) and a function pointer.
//  1. A nonzero function pointer calls __assert__FPCcUiT0i with the file text
//     "particle.cc", line 502, the text "!terminateCallBackIn" and the caller's
//     return address, and then continues.
//  2. mode and tier are clamped to 0..3: below zero becomes zero, four or more
//     becomes three. lifeTimes[mode] (0x0037f720) is read as a signed integer.
//  3. Stores in the definition (word indexes are byte offsets over four):
//       +0   tier * 4 + mode                  +12  function pointer
//       +24  r * 2^-8                          +40  the definition's own address
//       +44  p * 100                           +28  see below
//     tier == 2: +16 takes r * 2^-8 and +28 takes p * 100 * 0.01f.
//     otherwise: +28 takes (q * 100 - p * 100) * (1.0f / float(lifeTimes[mode])),
//     and the grid integer fills four words: +16 = ((grid & 15) * 32 + 1) * 16,
//     +20 = ((grid >> 4) * 32 + 1) * 16, +32 = ((grid & 15) * 32 + 31) * 16,
//     +36 = ((grid >> 4) * 32 + 31) * 16 (arithmetic shift).
//     +48..+60 take float(start0..3); unless tier == 3, 8388608.0f is added to
//     each first. +64..+76 take float(end_n - start_n) * (1.0f / float(lifeTimes[mode])).
// What the fields mean beyond these offsets is not established here. Fresh
// compilation is not an original-byte match.
extern "C" {
extern unsigned lifeTimes[];
void __assert__FPCcUiT0i(const char* file, unsigned line, const char* text, int caller);
}

#if defined(NORRATH_HOST_CONTRACT)
extern "C" void* norrath_host_pointer(unsigned address);
#endif

namespace {
using Address = __UINTPTR_TYPE__;
unsigned char* bytes(void* definition) { return static_cast<unsigned char*>(definition); }
void write_word(unsigned char* base, unsigned offset, unsigned value) {
    base[offset] = static_cast<unsigned char>(value);
    base[offset + 1] = static_cast<unsigned char>(value >> 8);
    base[offset + 2] = static_cast<unsigned char>(value >> 16);
    base[offset + 3] = static_cast<unsigned char>(value >> 24);
}
void write_float(unsigned char* base, unsigned offset, float value) {
    union { float f; unsigned u; } bits;
    bits.f = value;
    write_word(base, offset, bits.u);
}
int clamp_to_three(int value) {
    if (value < 0) value = 0;
    if (value >= 4) value = 3;
    return value;
}
}

extern "C" void setParticleDef__FP11ParticleDefiiiiiiiifffiiiPFPv_v(
    void* definition, int start0, int start1, int start2, int start3, int end0, int end1, int end2, int end3,
    float p, float q, float r, int mode, int tier, int grid, void (*callback)(void*)) {
    unsigned char* d = bytes(definition);
    if (callback != nullptr) {
        __assert__FPCcUiT0i("particle.cc", 502u, "!terminateCallBackIn",
                            static_cast<int>(reinterpret_cast<Address>(__builtin_return_address(0))));
    }
    mode = clamp_to_three(mode);
    tier = clamp_to_three(tier);
    const float scaled_r = r * 0.00390625f;
    const float inverse = 1.0f / static_cast<float>(static_cast<int>(lifeTimes[mode]));
    const float scaled_q = q * 100.0f;
    const float scaled_p = p * 100.0f;
    write_word(d, 0, static_cast<unsigned>(tier * 4 + mode));
    write_word(d, 12, static_cast<unsigned>(reinterpret_cast<Address>(callback)));
    write_float(d, 24, scaled_r);
    float rate;
    if (tier == 2) {
        write_float(d, 16, scaled_r);
        rate = scaled_p * 0.01f;
    } else {
        rate = (scaled_q - scaled_p) * inverse;
        const int low = grid & 15;
        const int high = grid >> 4;
        write_word(d, 16, static_cast<unsigned>(((low * 32) + 1) * 16));
        write_word(d, 20, static_cast<unsigned>(((high * 32) + 1) * 16));
        write_word(d, 32, static_cast<unsigned>(((low * 32) + 31) * 16));
        write_word(d, 36, static_cast<unsigned>(((high * 32) + 31) * 16));
    }
    write_float(d, 28, rate);
    write_word(d, 40, static_cast<unsigned>(reinterpret_cast<Address>(definition)));
    write_float(d, 44, scaled_p);
    if (tier == 3) {
        write_float(d, 48, static_cast<float>(start0));
        write_float(d, 52, static_cast<float>(start1));
        write_float(d, 56, static_cast<float>(start2));
        write_float(d, 60, static_cast<float>(start3));
    } else {
        write_float(d, 48, static_cast<float>(start0) + 8388608.0f);
        write_float(d, 52, static_cast<float>(start1) + 8388608.0f);
        write_float(d, 56, static_cast<float>(start2) + 8388608.0f);
        write_float(d, 60, static_cast<float>(start3) + 8388608.0f);
    }
    write_float(d, 64, static_cast<float>(end0 - start0) * inverse);
    write_float(d, 68, static_cast<float>(end1 - start1) * inverse);
    write_float(d, 72, static_cast<float>(end2 - start2) * inverse);
    write_float(d, 76, static_cast<float>(end3 - start3) * inverse);
}
