// Observed __divdi3 at 0x002f8238, 1684 bytes. Signed 64-bit quotient, truncating toward zero,
// computed with 32-bit word operations and no call. Each negative operand is replaced by its
// 64-bit negation (a sign flag flips once per negative operand); the magnitudes then go through
// the same word-by-word unsigned divide as __udivdi3 at 0x002f8f60 (same case split on the
// divisor words, the range-test-plus-256-byte-table leading-zero count at 0x0047db08 and the
// two-stage 16-bit-digit correction; restated in the shared header of the w9-startup-crt-c
// package), and the quotient is negated when the flag is set. A zero divisor ends in a divide
// instruction whose zero check traps (break 7); here it is a divide of 1 by zero, as in
// the unsigned routine.
// Unknown: which instruction form the original source used for the trap. Nothing was executed
// on the EE.
#include "../w9-startup-crt-c-libgcc-div-common/udivmod.hpp"

namespace {
DoubleWord unsigned_quotient(DoubleWord n, DoubleWord d) {
    using namespace norrath_div;
    Word d0 = static_cast<Word>(d);
    Word d1 = static_cast<Word>(d >> 32);
    Word n0 = static_cast<Word>(n);
    Word n1 = static_cast<Word>(n >> 32);
    Word q0, q1;

    if (d1 == 0) {
        if (d0 > n1) {
            Word bm = leading_zeros(d0);
            if (bm != 0) {
                d0 <<= bm;
                n1 = (n1 << bm) | (n0 >> (32 - bm));
                n0 <<= bm;
            }
            divide_two_words(q0, n0, n1, n0, d0);
            q1 = 0;
        } else {
            if (d0 == 0) d0 = 1u / d0;
            Word bm = leading_zeros(d0);
            if (bm == 0) {
                n1 -= d0;
                q1 = 1;
            } else {
                Word b = 32 - bm;
                d0 <<= bm;
                Word n2 = n1 >> b;
                n1 = (n1 << bm) | (n0 >> b);
                n0 <<= bm;
                divide_two_words(q1, n1, n2, n1, d0);
            }
            divide_two_words(q0, n0, n1, n0, d0);
        }
    } else if (d1 > n1) {
        q0 = 0;
        q1 = 0;
    } else {
        Word bm = leading_zeros(d1);
        if (bm == 0) {
            q0 = (n1 > d1 || n0 >= d0) ? 1u : 0u;
            q1 = 0;
        } else {
            Word b = 32 - bm;
            d1 = (d1 << bm) | (d0 >> b);
            d0 <<= bm;
            Word n2 = n1 >> b;
            n1 = (n1 << bm) | (n0 >> b);
            n0 <<= bm;
            divide_two_words(q0, n1, n2, n1, d1);
            Word m1, m0;
            multiply_words(m1, m0, q0, d0);
            if (m1 > n1 || (m1 == n1 && m0 > n0)) --q0;
            q1 = 0;
        }
    }
    return (static_cast<DoubleWord>(q1) << 32) | q0;
}
}  // namespace

extern "C" long long __divdi3(long long n, long long d) {
    bool negate = false;
    DoubleWord un = static_cast<DoubleWord>(n);
    DoubleWord ud = static_cast<DoubleWord>(d);
    if (n < 0) {
        un = 0 - un;
        negate = true;
    }
    if (d < 0) {
        ud = 0 - ud;
        negate = !negate;
    }
    DoubleWord q = unsigned_quotient(un, ud);
    if (negate) q = 0 - q;
    return static_cast<long long>(q);
}
