#pragma once
#include "number_parser.hpp"
namespace norrath_parser_arithmetic {
struct QuotientRemainder { NorrathUnsigned64 quotient, remainder; };
inline QuotientRemainder divide_nonzero(NorrathUnsigned64 numerator, NorrathUnsigned64 denominator) {
    // Actual leaves raise a BREAK7 exception for zero divisor. This source
    // traps naturally; exact exception-code/handler equivalence is unclaimed.
    // The approved mathematical/parser domain requires nonzero denominator.
    if (denominator == 0ULL) __builtin_trap();
    NorrathUnsigned64 quotient = 0;
    NorrathUnsigned64 remainder = 0;
    for (unsigned remaining = 64; remaining != 0u; --remaining) {
        const bool carry = (remainder >> 63) != 0ULL;
        remainder = (remainder << 1) | ((numerator >> (remaining - 1u)) & 1ULL);
        const bool subtract = carry || remainder >= denominator;
        if (subtract) remainder -= denominator;
        quotient = (quotient << 1) | (subtract ? 1ULL : 0ULL);
    }
    return {quotient, remainder};
}
}
