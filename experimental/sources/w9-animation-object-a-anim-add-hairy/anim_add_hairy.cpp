#include "../w9-animation-object-a-common/mem.hpp"
// Observed animAddHairy__FP14AnimationStateP15AnimationHeaderfffffi at 0x001638b8, 120
// bytes. The five floats arrive in the first five float argument registers; the first
// is not read. A null header or a full state (signed byte 4 not below signed byte 5)
// stores nothing. Otherwise the next component, at state+8+32*count, is filled like
// animAdd fills it: byte 4 advances, the state halfword +6 gains the flag bits, component
// word +0 is the header, word +24 and halfword +30 are cleared, halfword +28 holds the
// flags, and a nonzero word at component+4 has that record's word +20 cleared and is
// itself cleared. The difference from animAdd: floats +8, +12, +16 and +20 receive the
// second to fifth float arguments instead of the constants 1.0, 0.0, 1.0 and 0.0.
// Inferred: the four floats are the same per-component slots animAdd initialises.
// Unknown: why the first float exists, what any slot means, and what "Hairy" refers to.
extern "C" void animAddHairy__FP14AnimationStateP15AnimationHeaderfffffi(
    unsigned char* state, unsigned char* header, float unused, float slot8, float slot12,
    float slot16, float slot20, int flags) {
    using namespace norrath_ao;
    (void)unused;
    if (header == nullptr) return;
    const int count = load_signed_byte(state + 4);
    if (!(count < load_signed_byte(state + 5))) return;
    unsigned char* component = state + 8 + count * 32;
    state[4] = static_cast<unsigned char>(count + 1);
    store_half(state + 6, load_half(state + 6) | static_cast<unsigned>(flags));
    unsigned char* link = load_pointer(component + 4);
    store_pointer(component + 0, header);
    store_word(component + 24, 0);
    store_half(component + 30, 0);
    if (link != nullptr) {
        store_word(link + 20, 0);
        store_word(component + 4, 0);
    }
    store_half(component + 28, static_cast<unsigned>(flags));
    store_float(component + 8, slot8);
    store_float(component + 12, slot12);
    store_float(component + 16, slot16);
    store_float(component + 20, slot20);
}
