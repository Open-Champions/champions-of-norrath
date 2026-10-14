#pragma once
// Raw lower-64-bit EABI carriers. Signed division/remainder interpret the
// two's-complement bit patterns explicitly; no narrowing to a 32-bit word.
namespace norrath_software_arithmetic {
using Bits = unsigned long long;
static_assert(sizeof(Bits) == 8 && sizeof(unsigned) == 4, "fixed EE scalar widths");
}
extern "C" norrath_software_arithmetic::Bits __muldi3(
    norrath_software_arithmetic::Bits left, norrath_software_arithmetic::Bits right);
extern "C" norrath_software_arithmetic::Bits __divdi3(
    norrath_software_arithmetic::Bits numerator, norrath_software_arithmetic::Bits denominator);
extern "C" norrath_software_arithmetic::Bits __moddi3(
    norrath_software_arithmetic::Bits numerator, norrath_software_arithmetic::Bits denominator);
