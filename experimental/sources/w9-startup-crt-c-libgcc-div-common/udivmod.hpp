// Word-size helpers shared by the two 64-bit unsigned division routines (__udivdi3 at
// 0x002f8f60 and __umoddi3 at 0x002f94e0). Observed in both bodies: every division works on
// 32-bit words, the leading-zero count is a 16/8-bit range test plus a 256-byte lookup table
// (a local read-only copy of the table sits in the image at 0x0047dd08 and four more copies
// nearby; the bytes of that one equal the bit length of the index), and the 64-by-32 step
// divides 16 bits at a time with a two-step correction. The helpers below restate those
// observed steps. Nothing here is a claim about the original source text.
#pragma once

typedef unsigned Word;
typedef unsigned long long DoubleWord;

namespace norrath_div {

struct BitLengthTable {
    unsigned char bytes[256];
    constexpr BitLengthTable() : bytes() {
        for (int i = 1; i < 256; ++i) bytes[i] = static_cast<unsigned char>(bytes[i >> 1] + 1);
    }
};
static constexpr BitLengthTable bit_length_table;

// Number of leading zero bits of a nonzero word, found as the original does: pick a shift of
// 0, 8, 16 or 24 by range tests, then add the table entry for the shifted value.
static inline Word leading_zeros(Word x) {
    Word shift;
    if (x < 0x10000u)
        shift = x < 0x100u ? 0u : 8u;
    else
        shift = x < 0x1000000u ? 16u : 24u;
    return 32u - (bit_length_table.bytes[x >> shift] + shift);
}

// Divide the two-word value n1:n0 by d, where n1 < d and d has its top bit set. Quotient and
// remainder come back through the references. 16 bits at a time.
static inline void divide_two_words(Word& quotient, Word& remainder, Word n1, Word n0, Word d) {
    Word d1 = d >> 16;
    Word d0 = d & 0xffffu;
    Word q1 = n1 / d1;
    Word r1 = n1 % d1;
    Word m = q1 * d0;
    r1 = (r1 << 16) | (n0 >> 16);
    if (r1 < m) {
        --q1;
        r1 += d;
        if (r1 >= d && r1 < m) {
            --q1;
            r1 += d;
        }
    }
    r1 -= m;
    Word q0 = r1 / d1;
    Word r0 = r1 % d1;
    m = q0 * d0;
    r0 = (r0 << 16) | (n0 & 0xffffu);
    if (r0 < m) {
        --q0;
        r0 += d;
        if (r0 >= d && r0 < m) {
            --q0;
            r0 += d;
        }
    }
    r0 -= m;
    quotient = (q1 << 16) | q0;
    remainder = r0;
}

// 32x32 -> 64-bit product as high and low words.
static inline void multiply_words(Word& high, Word& low, Word a, Word b) {
    Word a1 = a >> 16, a0 = a & 0xffffu;
    Word b1 = b >> 16, b0 = b & 0xffffu;
    Word p00 = a0 * b0;
    Word p01 = a0 * b1;
    Word p10 = a1 * b0;
    Word p11 = a1 * b1;
    p01 += p00 >> 16;
    p01 += p10;
    if (p01 < p10) p11 += 0x10000u;
    high = p11 + (p01 >> 16);
    low = (p01 << 16) + (p00 & 0xffffu);
}

}  // namespace norrath_div
