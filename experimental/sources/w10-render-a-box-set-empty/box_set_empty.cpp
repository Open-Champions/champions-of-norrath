#include "../level-anim-load-common/mem.hpp"
// Observed boxSetEmpty__FP3Box at 0x0015a8b8, 36 bytes. Six floats are stored through the
// argument: words 0, 1 and 2 receive the single-precision literal 1.0e7 (bits 0x4b189680,
// read from the global-pointer area) and words 3, 4 and 5 receive -1.0e7 (0xcb189680).
// The stores run in the order 2, 5, 0, 1, 3, 4.
// Inferred: the record is an axis-aligned box with the minimum corner in words 0..2 and
// the maximum corner in words 3..5, so these values make it empty for any later
// extension. Unknown: whether the record has fields beyond the first 24 bytes.
extern "C" void boxSetEmpty__FP3Box(unsigned char* box) {
    using namespace norrath_lal;
    const float high = 10000000.0f;
    const float low = -10000000.0f;
    store_float(box + 8, high);
    store_float(box + 20, low);
    store_float(box + 0, high);
    store_float(box + 4, high);
    store_float(box + 12, low);
    store_float(box + 16, low);
}
