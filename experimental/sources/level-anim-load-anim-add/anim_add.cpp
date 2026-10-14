#include "../level-anim-load-common/mem.hpp"
// Observed animAdd__FP14AnimationStateP15AnimationHeaderi at 0x00163830, 132 bytes.
// A null header or a full state (signed byte 4 not below signed byte 5) returns with
// no stores. Otherwise the next component, at state+8+32*count, is filled and byte 4
// advances. The state halfword at +6 gains the flag bits. In the component: word +0
// is the header, +24 and halfword +30 are cleared, halfword +28 holds the flags,
// floats +8 and +16 are 1.0 and floats +12 and +20 are 0.0. A nonzero pointer left
// in word +4 has its own word +20 cleared, and word +4 is cleared.
// Inferred: the +4 pointer is a link to a record that points back at this component.
// Unknown: the meaning of the individual float slots; the compile is not byte-matched.
extern "C" void animAdd__FP14AnimationStateP15AnimationHeaderi(
    unsigned char* state, unsigned char* header, int flags) {
    using namespace norrath_lal;
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
    store_float(component + 12, 0.0f);
    store_float(component + 8, 1.0f);
    store_float(component + 16, 1.0f);
    store_float(component + 20, 0.0f);
}
