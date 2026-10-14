// Observed __moddi3 at 0x002f88d0, 1680 bytes. Signed 64-bit remainder, which takes the sign of
// the dividend, computed with 32-bit word operations and no call. A negative dividend is
// replaced by its 64-bit negation and a flag is set; a negative divisor is negated too
// but sets no flag. The magnitudes go through the same word-by-word unsigned remainder as
// __umoddi3 at 0x002f94e0 (case split on the divisor words, the range-test-plus-256-byte-table
// leading-zero count at 0x0047db08 and the two-stage 16-bit-digit correction; restated in the
// shared header of the w9-startup-crt-c package), the remainder passes through a stack slot, and
// it is negated when the flag is set. A zero divisor ends in a divide instruction whose zero
// check traps (break 7); here it is a divide of 1 by zero, as in the unsigned routine.
// Unknown: which instruction form the original source used for the trap. Nothing was executed
// on the EE.
#include "../w9-startup-crt-c-libgcc-div-common/udivmod.hpp"

namespace {
DoubleWord unsigned_remainder(DoubleWord n, DoubleWord d) {
    using namespace norrath_div;
    Word d0 = static_cast<Word>(d);
    Word d1 = static_cast<Word>(d >> 32);
    Word n0 = static_cast<Word>(n);
    Word n1 = static_cast<Word>(n >> 32);
    Word q, low, high;

    if (d1 == 0) {
        Word bm;
        if (d0 > n1) {
            bm = leading_zeros(d0);
            if (bm != 0) {
                d0 <<= bm;
                n1 = (n1 << bm) | (n0 >> (32 - bm));
                n0 <<= bm;
            }
            divide_two_words(q, n0, n1, n0, d0);
        } else {
            if (d0 == 0) d0 = 1u / d0;
            bm = leading_zeros(d0);
            if (bm == 0) {
                n1 -= d0;
            } else {
                Word b = 32 - bm;
                d0 <<= bm;
                Word n2 = n1 >> b;
                n1 = (n1 << bm) | (n0 >> b);
                n0 <<= bm;
                divide_two_words(q, n1, n2, n1, d0);
            }
            divide_two_words(q, n0, n1, n0, d0);
        }
        low = n0 >> bm;
        high = 0;
    } else if (d1 > n1) {
        low = n0;
        high = n1;
    } else {
        Word bm = leading_zeros(d1);
        if (bm == 0) {
            if (n1 > d1 || n0 >= d0) {
                Word borrow = n0 < d0;
                n0 -= d0;
                n1 = n1 - d1 - borrow;
            }
            low = n0;
            high = n1;
        } else {
            Word b = 32 - bm;
            d1 = (d1 << bm) | (d0 >> b);
            d0 <<= bm;
            Word n2 = n1 >> b;
            n1 = (n1 << bm) | (n0 >> b);
            n0 <<= bm;
            divide_two_words(q, n1, n2, n1, d1);
            Word m1, m0;
            multiply_words(m1, m0, q, d0);
            if (m1 > n1 || (m1 == n1 && m0 > n0)) {
                Word borrow = m0 < d0;
                m0 -= d0;
                m1 = m1 - d1 - borrow;
            }
            Word borrow = n0 < m0;
            n0 -= m0;
            n1 = n1 - m1 - borrow;
            low = (n1 << b) | (n0 >> bm);
            high = n1 >> bm;
        }
    }
    return (static_cast<DoubleWord>(high) << 32) | low;
}
}  // namespace

extern "C" long long __moddi3(long long n, long long d) {
    bool negate = false;
    DoubleWord un = static_cast<DoubleWord>(n);
    DoubleWord ud = static_cast<DoubleWord>(d);
    if (n < 0) {
        un = 0 - un;
        negate = true;
    }
    if (d < 0) ud = 0 - ud;
    DoubleWord r = unsigned_remainder(un, ud);
    if (negate) r = 0 - r;
    return static_cast<long long>(r);
}
