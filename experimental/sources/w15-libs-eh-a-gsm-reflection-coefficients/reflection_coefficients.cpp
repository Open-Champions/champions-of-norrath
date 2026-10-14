// Observed Reflection_coefficients at 0x002ed4a0, 608 bytes (a local routine of the GSM codec), pinned SLUS_205.65.
// Arguments: the 9 autocorrelation values acf and an 8-word output r. All words are 64-bit and the arithmetic is
// unsaturated, with no rounding term on products.
//   * acf[0] == 0: r[0..7] = 0, return.
//   * n = gsm_norm(acf[0]) (0x002ed9e0), taken as a 32-bit value; the working copy is w[i] = (acf[i] << n) >> 16
//     (arithmetic) for i = 0..8. P[0..8] = w[0..8] and K[1..7] = w[1..7].
//   * For step k = 1..8 (writing r[k - 1]): t = |P[1]|. If P[0] < t the remaining r[k - 1..7] are set to 0 and the
//     routine returns. Otherwise r = (t << 15) / P[0] (a signed 64-bit division through __divdi3, 0x002f8238,
//     skipped when t << 15 is zero), negated when P[1] > 0. At k == 8 the routine returns; otherwise the recursion runs:
//     P[0] += (P[1] * r) >> 15, then for m = 1..8 - k: P[m] = P[m + 1] + ((K[m] * r) >> 15) and
//     K[m] = K[m] + ((P[m + 1] * r) >> 15), the second line using the P[m + 1] from before the first line's store to P[m].
// The product goes through __muldi3 (0x002f81c0). Unknown: the callees' bodies. Nothing was executed on the EE.
#include "../w15-libs-eh-a-gsm-common/gsm.hpp"

extern "C" {
int gsm_norm(long long value);

void Reflection_coefficients(const w15gsm::word* acf, w15gsm::word* r) {
    using w15gsm::word;
    if (acf[0] == 0) {
        for (int i = 0; i < 8; ++i) r[i] = 0;
        return;
    }
    const int shift = gsm_norm(acf[0]);
    word w[9], P[9], K[9];
    for (int i = 0; i < 9; ++i) w[i] = (acf[i] << shift) >> 16;
    for (int i = 1; i < 8; ++i) K[i] = w[i];
    for (int i = 0; i < 9; ++i) P[i] = w[i];

    for (int k = 1; k <= 8; ++k) {
        const word p1 = P[1];
        const word t = p1 < 0 ? -p1 : p1;
        if (P[0] < t) {
            for (int i = k; i <= 8; ++i) r[i - 1] = 0;
            return;
        }
        word value = 0;
        if ((t << 15) != 0) value = (t << 15) / P[0];
        r[k - 1] = value;
        if (p1 > 0) r[k - 1] = -value;
        if (k == 8) return;
        const word coefficient = r[k - 1];
        P[0] = P[0] + ((p1 * coefficient) >> 15);
        for (int m = 1; m <= 8 - k; ++m) {
            const word next = P[m + 1];
            P[m] = next + ((K[m] * coefficient) >> 15);
            K[m] = K[m] + ((next * coefficient) >> 15);
        }
    }
}
}
