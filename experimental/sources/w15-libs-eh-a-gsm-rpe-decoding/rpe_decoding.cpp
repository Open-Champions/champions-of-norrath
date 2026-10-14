// Observed Gsm_RPE_Decoding at 0x002ee3f0, 604 bytes, pinned SLUS_205.65: the regular-pulse-excitation decoder of
// a GSM 06.10 full-rate decoder, built with 64-bit "word" values (every table entry and sample is a doubleword).
// Called by gsm_decode (0x002eb7e0 region) as Gsm_RPE_Decoding(xmaxc, grid, pulses, out) with the block maximum code,
// the grid position 0..3, the 13 decoded pulse codes and a 40-word output. Three steps, in order:
//   1. Exponent and mantissa from xmaxc: exp = xmaxc > 15 ? (xmaxc >> 3) - 1 : 0, mant = xmaxc - 8 * exp. A zero
//      mantissa gives exp -4, mant 7; otherwise the mantissa is doubled with a low one bit (and exp lowered) while it is
//      below 8, then lowered by 8.
//   2. For each of the 13 pulses: t = (pulse << 13) - 28672 (that is (2 * pulse - 7) << 12); t = (gsm_FAC[mant] * t) >> 15
//      (no rounding term); t is added to a rounding word and saturated to the 16-bit range; the result is shifted right
//      arithmetically by s. Here s = 6 - exp and the rounding word is 1 << (s - 1), the first saturated to [-32768, 32767]
//      before use (the lower clamp applies after the add of -1), and the shift uses the low six bits of s (no
//      range-limited shift helper). When (s - 1) before the lower clamp is 32767 or more, the rounding word is
//      0xffffffff80000000.
//   3. The 13 values go to output positions grid + 3 * i (i = 0..12), with zeros between them: grid zeros first (a
//      grid of 3 gives three), then each value followed by two zeros, and finally a loop that stores zeros while
//      ++grid < 4, which pads the tail to 40 words. A grid outside 0..3 skips the interleave: nothing is stored
//      for the values, and the padding loop stores one zero per step from grid + 1 up to 3 (none for grid >= 3).
// Inferred: gsm_FAC (0x00447fb8, eight doublewords) is the decoder's mantissa table; field and helper names are mine.
// Unknown: the bodies of the callees (__muldi3 for the product); nothing was executed on the EE.
extern "C" {
typedef long long word;
extern word gsm_FAC[8];

void Gsm_RPE_Decoding(word xmaxc, word grid, const word* pulses, word* out) {
    word exp = 0;
    if (xmaxc > 15) exp = (xmaxc >> 3) - 1;
    word mant = xmaxc - (exp << 3);
    if (mant == 0) {
        exp = -4;
        mant = 7;
    } else {
        while (mant <= 7) {
            mant = mant * 2 + 1;
            exp -= 1;
        }
        mant -= 8;
    }

    const word factor = gsm_FAC[mant];
    word shift = 6 - exp;
    shift = shift < 32767 ? (shift < -32768 ? -32768 : shift) : 32767;
    word before = shift - 1;
    word rounding;
    if (before < 32767) {
        const word clamped = before < -32768 ? -32768 : before;
        rounding = static_cast<int>(1u << (static_cast<unsigned>(clamped) & 31u));
    } else {
        rounding = static_cast<int>(0x80000000u);
    }

    word decoded[13];
    for (int i = 0; i < 13; ++i) {
        word t = (pulses[i] << 13) - 28672;
        t = (factor * t) >> 15;
        word sum = t + rounding;
        if (sum >= 32767) sum = 32767;
        else if (sum < -32768) sum = -32768;
        decoded[i] = sum >> (static_cast<unsigned>(shift) & 63u);
    }

    const word* source = decoded;
    word* dest = out;
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
