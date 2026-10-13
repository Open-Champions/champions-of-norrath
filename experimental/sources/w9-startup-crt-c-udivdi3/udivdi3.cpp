// Observed __udivdi3 at 0x002f8f60, 1404 bytes. Unsigned 64-bit quotient (arguments in a1:a0 and
// a3:a2 registers, 64-bit result in v0) computed entirely with 32-bit word operations and no
// call. Observed paths, by the high word d1 and low word d0 of the divisor and the dividend
// words n1:n0: (d1 == 0, d0 > n1) one two-word division after normalising; (d1 == 0, d0 <= n1)
// two chained two-word divisions, with d0 == 0 turned into the hardware divide-by-zero trap by
// dividing 1 by it; (d1 != 0, d1 > n1) quotient 0; (d1 != 0, d1 <= n1) quotient 0 or 1, found by
// a normalised estimate corrected against a full 32x32 product. The shape matches the
// widely published generic 64-bit unsigned divide routine for 32-bit words; this file restates
// that observed behaviour independently. Nothing here was executed on the EE.
#include "../w9-startup-crt-c-libgcc-div-common/udivmod.hpp"

extern "C" DoubleWord __udivdi3(DoubleWord n, DoubleWord d) {
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
