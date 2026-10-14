// Observed Gsm_LPC_Analysis at 0x002ed940, 156 bytes, pinned SLUS_205.65: the linear-prediction stage of the GSM encoder
// (gsm_encode calls it as (samples, LARc) with 160 sample words and an 8-word result). In order:
//   Autocorrelation (0x002ecee0) on the samples into 9 words on the stack;
//   Reflection_coefficients (0x002ed4a0) from those into LARc;
//   the log-area-ratio transformation, inline: for each of the 8 values x, with t = |x|: t < 22118 gives t >> 1;
//   t < 31130 gives t - 11059; otherwise (t - 26112) << 2; the sign of x is restored (64-bit, no saturation);
//   Quantization_and_coding (0x002ed700) on LARc.
// Unknown: nothing beyond the callees' bodies; nothing was executed on the EE.
#include "../w15-libs-eh-a-gsm-common/gsm.hpp"

extern "C" {
void Autocorrelation(const w15gsm::word* samples, w15gsm::word* acf);
void Reflection_coefficients(const w15gsm::word* acf, w15gsm::word* r);
void Quantization_and_coding(w15gsm::word* values);

void Gsm_LPC_Analysis(const w15gsm::word* samples, w15gsm::word* lar) {
    using w15gsm::word;
    word acf[9];
    Autocorrelation(samples, acf);
    Reflection_coefficients(acf, lar);
    for (int i = 0; i < 8; ++i) {
        const word x = lar[i];
        word t = x < 0 ? -x : x;
        if (t < 22118) t >>= 1;
        else if (t < 31130) t -= 11059;
        else t = (t - 26112) << 2;
        lar[i] = x < 0 ? -t : t;
    }
    Quantization_and_coding(lar);
}
}
