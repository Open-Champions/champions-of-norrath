#include "../w9-animation-object-a-common/mem.hpp"
// Observed animFindComponentWithFlags__FP14AnimationStatei at 0x00164130, 76 bytes.
// The signed byte at state+4 is a component count; components are 32 bytes each,
// starting at state+8, and the halfword at component+28 holds that component's flags
// (the same slot animAdd fills). Returns the index of the first component for which
// (sign-extended flag halfword & mask) is nonzero, scanning from index 0. A count of
// zero or below, or no match, returns -1000000000 (0xc4653600).
// Inferred: -1000000000 is the "not found" value; callers' use of it is not examined.
// Unknown: the meaning of individual flag bits.
extern "C" int animFindComponentWithFlags__FP14AnimationStatei(unsigned char* state, int mask) {
    using namespace norrath_ao;
    const int count = load_signed_byte(state + 4);
    for (int index = 0; index < count; ++index) {
        if (load_signed_half(state + 8 + index * 32 + 28) & mask) return index;
    }
    return -1000000000;
}
