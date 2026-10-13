#include "../w9-animation-object-a-common/mem.hpp"
// Observed animAddToEnd__FP14AnimationStateP15AnimationHeaderi at 0x00163930, 148 bytes.
// Like animAdd, but the new component starts where the previous one ends. A null header
// or a full state (signed byte 4 not below signed byte 5) stores nothing. Otherwise,
// with the new component at state+8+32*count (count = signed byte 4) and the previous
// one 32 bytes earlier: byte 4 advances; the halfword at state+6 gains the flag bits;
// new+24 receives (float at previous+24) minus (float conversion of the integer word at
// offset 4 of the previous component's header); new+0 is the header; a nonzero word at
// new+4 has that record's word +20 cleared and new+4 is cleared; halfword new+28 holds
// the flags; words new+20 and new+12 are cleared; floats new+16 and new+8 are 1.0.
// The halfword at new+30 is not touched.
// Inferred: new+24 is a start time and header+4 a length, so the sequence is queued
// after the last component. The previous slot is read only after the capacity test
// (the original reads it ahead of the test; no store depends on that order).
// Unknown: units of the +24 float and the real meaning of the header word at +4. With a
// count of zero the previous slot lies in front of the state and is not valid.
extern "C" void animAddToEnd__FP14AnimationStateP15AnimationHeaderi(
    unsigned char* state, unsigned char* header, int flags) {
    using namespace norrath_ao;
    if (header == nullptr) return;
    const int count = load_signed_byte(state + 4);
    if (!(count < load_signed_byte(state + 5))) return;
    unsigned char* previous = state + 8 + (count - 1) * 32;
    unsigned char* component = state + 8 + count * 32;
    const float length = static_cast<float>(static_cast<int>(load_word(load_pointer(previous) + 4)));
    const float end = load_float(previous + 24) - length;
    state[4] = static_cast<unsigned char>(count + 1);
    store_half(state + 6, load_half(state + 6) | static_cast<unsigned>(flags));
    unsigned char* link = load_pointer(component + 4);
    store_pointer(component + 0, header);
    store_float(component + 24, end);
    if (link != nullptr) {
        store_word(link + 20, 0);
        store_word(component + 4, 0);
    }
    store_half(component + 28, static_cast<unsigned>(flags));
    store_word(component + 20, 0);
    store_word(component + 12, 0);
    store_float(component + 16, 1.0f);
    store_float(component + 8, 1.0f);
}
