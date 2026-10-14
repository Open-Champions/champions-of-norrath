#include "../level-anim-load-common/mem.hpp"
// Observed animInitState__FP14AnimationState at 0x001637a8, 8 bytes: one byte store of
// zero at state+4, then return. Elsewhere in the animation code that signed byte is the
// number of components in use, and byte 5 is the capacity; the stores here and in the
// add routines agree with that reading.
// Unknown: whether the component records and the state word at +0 are left stale; this
// routine does not touch them.
extern "C" void animInitState__FP14AnimationState(unsigned char* state) {
    state[4] = 0;
}
