#include "../level-anim-load-common/mem.hpp"
// Observed animSetToEndFullyBlended__FP14AnimationState at 0x00163c58, 84 bytes. A nonzero
// word at state+0 is a pointer: state+0 is cleared and the word at +1552 of the pointed-to
// record is cleared. With a zero signed byte 4 the routine returns. Otherwise the last
// component, at state+8+32*(count-1), receives 1.0f at +16 and +8, and at +24 the
// float conversion of the signed integer word at header+4, where the header is the word
// at the component's offset 0. The stores run in the order +16, +8, +24.
// Inferred: this is animSetToEnd without the link adjustment, jumping the last
// animation to its final frame at full weight. Unknown: the meaning of the +8 and +16 floats.
extern "C" void animSetToEndFullyBlended__FP14AnimationState(unsigned char* state) {
    using namespace norrath_lal;
    unsigned char* result = load_pointer(state + 0);
    if (result != nullptr) {
        store_word(state + 0, 0);
        store_word(result + 1552, 0);
    }
    const int count = load_signed_byte(state + 4);
    if (count == 0) return;
    unsigned char* component = state + 8 + (count - 1) * 32;
    unsigned char* header = load_pointer(component);
    store_float(component + 16, 1.0f);
    const float frames = static_cast<float>(static_cast<int>(load_word(header + 4)));
    store_float(component + 8, 1.0f);
    store_float(component + 24, frames);
}
