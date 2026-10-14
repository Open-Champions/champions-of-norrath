#include "software_arithmetic.hpp"
#include "native_accumulators.hpp"
namespace norrath_software_arithmetic {
namespace {
using Word = unsigned;
inline Bits word_lane(Word value) {
    return (value & 0x80000000u) ? (Bits(value) | 0xffffffff00000000ULL) : Bits(value);
}
inline Bits magnitude(Bits value) { return (value >> 63) ? Bits(0) - value : value; }
inline Bits join(Word high, Word low) { return (Bits(high) << 32) | Bits(low); }
// Four base-2^16 partial products. Every multiply expression has only 32-bit
// operands; no 64-bit product/division/remainder can recurse into these roots.
inline Bits unsigned_product(Word left, Word right) {
    const Word ll = (left & 65535u) * (right & 65535u);
    const Word lh = (left & 65535u) * (right >> 16);
    const Word hl = (left >> 16) * (right & 65535u);
    const Word hh = (left >> 16) * (right >> 16);
    return Bits(ll) + (Bits(lh) << 16) + (Bits(hl) << 16) + (Bits(hh) << 32);
}
inline Bits signed_product(Word left, Word right) {
    Bits product = unsigned_product(left, right);
    if (left >> 31) product -= Bits(right) << 32;
    if (right >> 31) product -= Bits(left) << 32;
    return product;
}
inline void record_product0(Accumulators& state, Bits product) {
    state.lo0 = word_lane(Word(product));
    state.hi0 = word_lane(Word(product >> 32));
}
inline Word multiply_signed0(Accumulators& state, Word left, Word right) {
    const Bits product = signed_product(left, right);
    record_product0(state, product);
    return Word(product);
}
struct WordDivision { Word quotient, remainder; };
inline WordDivision divide_word(Accumulators& state, Word numerator, Word denominator) {
    // Caller normalization proves denominator's high digit is nonzero.
    const Word quotient = numerator / denominator;
    const Word remainder = numerator % denominator;
    state.lo0 = word_lane(quotient);
    state.hi0 = word_lane(remainder);
    return {quotient, remainder};
}
inline unsigned normalization(Word nonzero) {
    unsigned shift = 0;
    while ((nonzero & 0x80000000u) == 0) { nonzero <<= 1; ++shift; }
    return shift;
}
struct Digit { Word quotient, remainder; };
// Estimate a base-65536 digit from the divisor's high half, then correct its
// overestimate once or twice. The native MULT effect records the UNCORRECTED
// estimate. Corrections never multiply again or overwrite the tracked banks.
inline Digit quotient_digit(Accumulators& state, Word high, Word low_digit,
                            Word divisor) {
    const WordDivision estimate = divide_word(state, high, divisor >> 16);
    Word quotient = estimate.quotient;
    const Word product = multiply_signed0(state, quotient, divisor & 65535u);
    Word residue = (estimate.remainder << 16) | low_digit;
    if (residue < product) {
        --quotient;
        residue += divisor;
        // A wrapped sum already represents the carry that cancels subtraction.
        if (residue >= divisor && residue < product) {
            --quotient;
            residue += divisor;
        }
    }
    return {quotient, Word(residue - product)};
}
struct PairDivision { Word quotient, remainder; };
inline PairDivision divide_pair(Accumulators& state, Word high, Word low,
                                Word normalized_divisor) {
    const Digit upper = quotient_digit(state, high, low >> 16, normalized_divisor);
    const Digit lower = quotient_digit(state, upper.remainder, low & 65535u,
                                      normalized_divisor);
    return {Word((upper.quotient << 16) | lower.quotient), lower.remainder};
}
struct Division { Bits quotient, remainder; };
// This is the recovered original word-normalization and base-65536 algorithm.
// Lookup-table reads are replaced by arithmetic leading-zero counting; this
// packet does not claim original lookup/read/fault/IRQ trace equivalence.
inline Division divide_magnitudes(Accumulators& state, Bits numerator, Bits denominator) {
    Word n1 = Word(numerator >> 32), n0 = Word(numerator);
    Word d1 = Word(denominator >> 32), d0 = Word(denominator);
    if (d1 == 0) {
        if (d0 == 0) zero_denominator(state);
        const unsigned shift = normalization(d0);
        Word q1 = 0;
        if (n1 < d0) {
            if (shift != 0) {
                n1 = Word((n1 << shift) | (n0 >> (32 - shift)));
                n0 <<= shift;
                d0 <<= shift;
            }
        } else if (shift == 0) {
            // The high limb needs exactly one subtraction, with no MDU effect.
            n1 -= d0;
            q1 = 1;
        } else {
            const Word n2 = n1 >> (32 - shift);
            n1 = Word((n1 << shift) | (n0 >> (32 - shift)));
            n0 <<= shift;
            d0 <<= shift;
            const PairDivision upper = divide_pair(state, n2, n1, d0);
            q1 = upper.quotient;
            n1 = upper.remainder;
        }
        const PairDivision lower = divide_pair(state, n1, n0, d0);
        return {join(q1, lower.quotient), Bits(lower.remainder >> shift)};
    }
    if (n1 < d1) return {0, numerator}; // Both incoming banks survive.
    const unsigned shift = normalization(d1);
    if (shift == 0) {
        // No multiply or divide in either direct comparison/subtraction path.
        const bool subtract = n1 > d1 || n0 >= d0;
        return {Bits(subtract), subtract ? numerator - denominator : numerator};
    }
    const Word n2 = n1 >> (32 - shift);
    n1 = Word((n1 << shift) | (n0 >> (32 - shift)));
    n0 <<= shift;
    d1 = Word((d1 << shift) | (d0 >> (32 - shift)));
    d0 <<= shift;
    const PairDivision estimate = divide_pair(state, n2, n1, d1);
    Word quotient = estimate.quotient;
    Bits product = unsigned_product(quotient, d0);
    // This final MULTU precedes the quotient/product correction in BOTH roots.
    record_product0(state, product);
    const Bits residue = join(estimate.remainder, n0);
    if (product > residue) {
        --quotient;
        product -= join(d1, d0);
    }
    return {Bits(quotient), (residue - product) >> shift};
}
}
}
extern "C" norrath_software_arithmetic::Bits __muldi3(
    norrath_software_arithmetic::Bits left, norrath_software_arithmetic::Bits right) {
    using namespace norrath_software_arithmetic;
    Accumulators state = capture(left, right);
    const Word left_low = Word(left), right_low = Word(right);
    const Word left_high = Word(left >> 32), right_high = Word(right >> 32);
    const Bits low_product = unsigned_product(left_low, right_low);
    record_product0(state, low_product);
    const Bits cross0 = signed_product(left_low, right_high);
    record_product0(state, cross0);
    const Bits cross1 = signed_product(left_high, right_low);
    state.lo1 = word_lane(Word(cross1));
    state.hi1 = word_lane(Word(cross1 >> 32));
    const Word high = Word(low_product >> 32) + Word(cross0) + Word(cross1);
    return commit(state, join(high, Word(low_product)));
}
extern "C" norrath_software_arithmetic::Bits __divdi3(
    norrath_software_arithmetic::Bits numerator,
    norrath_software_arithmetic::Bits denominator) {
    using namespace norrath_software_arithmetic;
    Accumulators state = capture(numerator, denominator);
    const Division result = divide_magnitudes(state, magnitude(numerator), magnitude(denominator));
    const Bits quotient = ((numerator ^ denominator) >> 63)
        ? Bits(0) - result.quotient : result.quotient;
    return commit(state, quotient);
}
extern "C" norrath_software_arithmetic::Bits __moddi3(
    norrath_software_arithmetic::Bits numerator,
    norrath_software_arithmetic::Bits denominator) {
    using namespace norrath_software_arithmetic;
    Accumulators state = capture(numerator, denominator);
    const Division result = divide_magnitudes(state, magnitude(numerator), magnitude(denominator));
    const Bits remainder = (numerator >> 63) ? Bits(0) - result.remainder : result.remainder;
    return commit(state, remainder);
}
