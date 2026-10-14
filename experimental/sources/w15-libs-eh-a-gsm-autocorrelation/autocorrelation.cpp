// Observed Autocorrelation at 0x002ecee0, 1468 bytes (a local routine of the GSM codec), pinned SLUS_205.65.
// Arguments: 160 input words s and a 9-word output acf. All words are 64-bit.
//   * smax = the largest |s[k]| (a signed compare of the negated value for negatives). The scaling is 0 when smax is 0
//     and otherwise 4 - gsm_norm(smax << 16) (0x002ed9e0).
//   * A working copy of the 160 words is taken on the stack, shifted right arithmetically by the scaling when it is
//     positive and copied unchanged otherwise. The input array is not modified (the image keeps no restore step).
//   * acf[0..8] start at zero; for each position i = 0..159 and each lag k = 0..min(i, 8), acf[k] += c[i] * c[i - k]
//     (the image unrolls this, using the shorter lag range for the first eight positions). Finally every acf[k] is
//     doubled.
// The products go through __muldi3 (0x002f81c0). Unknown: the callee bodies. Nothing was executed on the EE.
#include "../w15-libs-eh-a-gsm-common/gsm.hpp"

extern "C" {
int gsm_norm(long long value);

void Autocorrelation(const w15gsm::word* s, w15gsm::word* acf) {
    using w15gsm::word;
    word smax = 0;
    for (int k = 0; k < 160; ++k) {
        const word magnitude = s[k] < 0 ? -s[k] : s[k];
        if (smax < magnitude) smax = magnitude;
    }
    word scaling = 0;
    if (smax != 0) scaling = 4 - gsm_norm(smax << 16);
    word c[160];
    if (scaling > 0) {
        const int shift = static_cast<int>(scaling);
        for (int k = 0; k < 160; ++k) c[k] = s[k] >> shift;
    } else {
        for (int k = 0; k < 160; ++k) c[k] = s[k];
    }
    for (int k = 0; k < 9; ++k) acf[k] = 0;
    for (int i = 0; i < 160; ++i) {
        const int lags = i < 8 ? i : 8;
        for (int k = 0; k <= lags; ++k) acf[k] += c[i] * c[i - k];
    }
    for (int k = 8; k >= 0; --k) acf[k] <<= 1;
}
}
