#include "../level-anim-load-common/mem.hpp"
// Observed animResetFramestates__FP14AnimationState at 0x001640d0, 96 bytes. A nonzero
// word at state+0 is a pointer; its word at +1552 is cleared and state+0 is cleared.
// Then, for each of the signed-byte-4 components at state+8+32*i, a nonzero word at
// component+4 is a pointer whose word +20 is cleared, and component+4 is cleared.
// Inferred: state+0 points at a 1568-byte result record (animInit links animResults
// in records of that stride and reserves +1552 ahead of its list words).
// Unknown: whether the component loop observes a count changed by the stores; the
// stores never touch byte 4 of the state.
extern "C" void animResetFramestates__FP14AnimationState(unsigned char* state) {
    using namespace norrath_lal;
    unsigned char* result = load_pointer(state + 0);
    if (result != nullptr) {
        store_word(result + 1552, 0);
        store_word(state + 0, 0);
    }
    const int count = load_signed_byte(state + 4);
    for (int index = 0; index < count; ++index) {
        unsigned char* slot = state + 12 + index * 32;
        unsigned char* link = load_pointer(slot);
        if (link != nullptr) {
            store_word(link + 20, 0);
            store_word(slot, 0);
        }
    }
}
