#include "../w9-animation-object-a-common/mem.hpp"
// Observed animGlobalSync__FP14AnimationState at 0x001639e8, 248 bytes. For each of the
// signed-byte-4 components (32 bytes each, from state+8), in order:
//   1. component+24 = fmodf(frameNm * (1/60) * 60.0, length), where frameNm is the global
//      int at 0x004844a4 converted to float and length is the float conversion of the
//      integer word at offset 4 of the component's header (the word at component+0).
//   2. If bit 0x0800 of the halfword at component+28 is set, updateAnimation is called
//      on the component, then the record linked at component+4 (assumed nonzero) is
//      adjusted: with t = component[+24] - (float)(signed halfword at link+28), the
//      floats at link+36 and link+32 become v - (v + k*t) for v = link[+36], k = link[+48]
//      and v = link[+32], k = link[+44] respectively.
// Inferred: all components are placed at the same global time, wrapped to each clip's
// length; the linked record carries a per-component offset that the 0x0800 flag asks to
// be re-derived. No field names are claimed, and the 1/60 and 60.0 pair is copied as the
// original orders it (the multiplications are not folded).
// Unknown: what the adjustment at link+32/+36 represents (it reads as v - (v + k*t), kept
// literally because floating-point rounding makes that differ from -k*t), and which
// rounding of fmodf the original build used.
extern "C" {
extern int frameNm;
float fmodf(float, float);
void updateAnimation__FP13animComponent(unsigned char* component);
void animGlobalSync__FP14AnimationState(unsigned char* state);
}

extern "C" void animGlobalSync__FP14AnimationState(unsigned char* state) {
    using namespace norrath_ao;
    const int count = load_signed_byte(state + 4);
    for (int index = 0; index < count; ++index) {
        unsigned char* component = state + 8 + index * 32;
        const float now = static_cast<float>(frameNm) * (1.0f / 60.0f) * 60.0f;
        const float length = static_cast<float>(static_cast<int>(load_word(load_pointer(component) + 4)));
        store_float(component + 24, fmodf(now, length));
        if (load_half(component + 28) & 0x800u) {
            updateAnimation__FP13animComponent(component);
            unsigned char* link = load_pointer(component + 4);
            const float t = load_float(component + 24) - static_cast<float>(load_signed_half(link + 28));
            const float b = load_float(link + 36);
            const float a = load_float(link + 32);
            const float nb = b + load_float(link + 48) * t;
            const float na = a + load_float(link + 44) * t;
            store_float(link + 36, b - nb);
            store_float(link + 32, a - na);
        }
    }
}
