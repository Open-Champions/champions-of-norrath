#include "../level-anim-load-common/mem.hpp"
// Observed boxAdd__FP3BoxT0 at 0x0015a6b8, 128 bytes. Both arguments address six floats.
// For words 0, 1 and 2 of the first record the routine stores the second record's word
// when it compares strictly less (c.lt.s other < mine). For words 3, 4 and 5 it stores
// the second record's word when it compares strictly greater. The second record is
// not written and nothing is returned. A NaN in either operand compares false, so the
// first record keeps its value.
// Inferred: the records are axis-aligned boxes (minimum corner then maximum corner) and
// this grows the first to contain the second. Unknown: the layout past 24 bytes.
extern "C" void boxAdd__FP3BoxT0(unsigned char* box, const unsigned char* other) {
    using namespace norrath_lal;
    for (unsigned i = 0; i != 3; ++i) {
        const float candidate = load_float(other + 4 * i);
        if (candidate < load_float(box + 4 * i)) store_float(box + 4 * i, candidate);
    }
    for (unsigned i = 3; i != 6; ++i) {
        const float candidate = load_float(other + 4 * i);
        if (load_float(box + 4 * i) < candidate) store_float(box + 4 * i, candidate);
    }
}
