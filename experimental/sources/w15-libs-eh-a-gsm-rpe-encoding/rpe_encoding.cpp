// Observed Gsm_RPE_Encoding at 0x002ee0f0, 764 bytes, pinned SLUS_205.65: the regular-pulse-excitation encoder of the
// GSM 06.10 coder, built with 64-bit "word" values. Arguments: e (the 40 residual words; five words before it and
// five after are read, and it is overwritten at the end), a pointer to the block maximum code xmaxc, a pointer to the
// grid position Mc, and a 13-word output xMc. In order:
//   1. Weighting filter into a stack array x[0..39]: x[k] = (4096 + sum) >> 13 with a 64-bit sum of e[k + j] times
//      -128, -256, 0, 2048, 5120, 8192, 5120, 2048, 0, -256, -128 for j = -5..5 (the image forms every product with
//      shifts and adds); the result is saturated to [-32768, 32767].
//   2. RPE_grid_selection (0x002edae8) on (x, a 13-word stack array xM, Mc).
//   3. APCM_quantization (0x002edee8) on (xM, xMc, &mant, &expon, xmaxc), with mant and expon two stack words.
//   4. The decoder's inverse quantisation, inline, exactly as Gsm_RPE_Decoding does it for each of the 13 values:
//      s = 6 - expon saturated to 16 bits, a rounding word 1 << (s - 1) (the low five bits of the clamped s - 1, or
//      0xffffffff80000000 when s - 1 is 32767 or more), t = (gsm_FAC[mant] * ((xMc << 13) - 28672)) >> 15 (product
//      through __muldi3 at 0x002f81c0), saturate (t + rounding), shift right arithmetically by the low six bits of s.
//   5. The 13 decoded values are written back into e at positions Mc + 3 * i with zeros elsewhere, using the same
//      interleave and tail padding as Gsm_RPE_Decoding (so a grid outside 0..3 is handled the same way).
// Unknown: the bodies of RPE_grid_selection and APCM_quantization; nothing was executed on the EE.
#include "../w15-libs-eh-a-gsm-common/gsm.hpp"

extern "C" {
extern w15gsm::word gsm_FAC[8];
void RPE_grid_selection(const w15gsm::word* x, w15gsm::word* xm, w15gsm::word* mc);
void APCM_quantization(const w15gsm::word* xm, w15gsm::word* xmc, w15gsm::word* mant, w15gsm::word* expon, w15gsm::word* xmaxc);

void Gsm_RPE_Encoding(w15gsm::word* e, w15gsm::word* xmaxc, w15gsm::word* mc, w15gsm::word* xmc) {
    using w15gsm::word;
    word x[40];
    for (int k = 0; k < 40; ++k) {
        const word sum = 4096 - (e[k - 5] << 7) - (e[k - 4] << 8) + (e[k - 2] << 11) + ((e[k - 1] << 10) + (e[k - 1] << 12)) +
                         (e[k] << 13) + ((e[k + 1] << 10) + (e[k + 1] << 12)) + (e[k + 2] << 11) - (e[k + 4] << 8) -
                         (e[k + 5] << 7);
        x[k] = w15gsm::clamp16(sum >> 13);
    }

    word xm[13], mant, expon;
    RPE_grid_selection(x, xm, mc);
    APCM_quantization(xm, xmc, &mant, &expon, xmaxc);

    const word factor = gsm_FAC[static_cast<int>(mant)];
    const word shift = w15gsm::clamp16(6 - expon);
    const word before = shift - 1;
    word rounding;
    if (before < 32767) {
        const word clamped = before < -32768 ? -32768 : before;
        rounding = static_cast<int>(1u << (static_cast<unsigned>(clamped) & 31u));
    } else {
        rounding = static_cast<int>(0x80000000u);
    }
    word decoded[13];
    for (int i = 0; i < 13; ++i) {
        word t = (xmc[i] << 13) - 28672;
        t = (factor * t) >> 15;
        decoded[i] = w15gsm::clamp16(t + rounding) >> (static_cast<unsigned>(shift) & 63u);
    }

    word grid = *mc;
    const word* source = decoded;
    word* dest = e;
    int count = 13;
    switch (grid) {
    case 3:
        *dest++ = 0;
        // fall through
    case 2:
        do {
            *dest++ = 0;
            // fall through
    case 1:
            *dest++ = 0;
            // fall through
    case 0:
            *dest++ = *source++;
        } while (--count);
    }
    while (++grid < 4) *dest++ = 0;
}
}
