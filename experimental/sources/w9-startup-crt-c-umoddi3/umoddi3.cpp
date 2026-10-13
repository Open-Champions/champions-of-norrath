// Observed __umoddi3 at 0x002f94e0, 1384 bytes. Unsigned 64-bit remainder, computed with 32-bit
// word operations and no call. It has the same case split as __udivdi3 (by the divisor words
// d1, d0 and dividend words n1, n0), but each path ends by building a remainder instead of a
// quotient: the divisor-fits-one-word paths leave the remainder in the low word shifted back
// down by the normalisation count, the "divisor larger than dividend" and top-bit-set paths
// leave n1:n0 (minus the divisor when the quotient is 1), and the normalised-estimate path
// subtracts the corrected product from n1:n0 and shifts the result down. The remainder is written
// to a stack slot whose address is tested against null before the store and is then returned.
// This file restates that observed behaviour independently. Nothing here was executed on the EE.
#include "../w9-startup-crt-c-libgcc-div-common/udivmod.hpp"

extern "C" DoubleWord __umoddi3(DoubleWord n, DoubleWord d) {
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
