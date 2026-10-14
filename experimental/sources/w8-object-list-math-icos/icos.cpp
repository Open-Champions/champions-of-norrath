// Observed icos__Fi at 0x15a070, extent 144. Cosine of an integer angle in
// 65536-per-turn units. Only the low 16 bits of the argument matter. See
// trig_core.hpp for the folding and polynomial, which are observed.
#include "../w8-object-list-math-common/trig_core.hpp"

extern "C" float icos__Fi(int angle) {
    return trig_core::evaluate(static_cast<short>(angle));
}
