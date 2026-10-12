// Observed isin__Fi at 0x15a100, extent 148. Sine of an integer angle in
// 65536-per-turn units, computed as the cosine of (angle - 16384). Only the low
// 16 bits of the argument matter. See trig_core.hpp.
#include "../w8-object-list-math-common/trig_core.hpp"

extern "C" float isin__Fi(int angle) {
    return trig_core::evaluate(static_cast<short>(angle - 16384));
}
