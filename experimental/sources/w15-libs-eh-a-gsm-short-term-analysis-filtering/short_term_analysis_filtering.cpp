// Observed Short_term_analysis_filtering at 0x002ef8e0, 1024 bytes (a local routine of the GSM codec), pinned
// SLUS_205.65. Arguments: the codec state (filter memory u = 8 words at +2264), 8 reflection coefficients rp, a sample
// count and the sample array (64-bit words), filtered in place. The image keeps u and rp in registers and on the
// stack and unrolls the eight stages. For each sample, with di = sav = the sample, the stages i = 0..7 do
//   ui = u[i]; u[i] = sav; sav = ui + ((rp[i] * di) >> 15); di = di + ((rp[i] * ui) >> 15)
// in 64-bit arithmetic, with no rounding term and no per-stage saturation; the output sample is di saturated to
// [-32768, 32767]. After the last sample every u[i] is saturated to the same range and written back (so an
// out-of-range stored value is brought into range even when the count is zero).
// Multiplication goes through __muldi3 (0x002f81c0). Unknown: the callee body. Nothing was executed on the EE.
#include "../w15-libs-eh-a-gsm-common/gsm.hpp"

extern "C" {
void Short_term_analysis_filtering(w15gsm::State* state, const w15gsm::word* rp, int count, w15gsm::word* samples) {
    using w15gsm::word;
    word u[8], r[8];
    for (int i = 0; i < 8; ++i) {
        u[i] = state->u[i];
        r[i] = rp[i];
    }
    for (int n = 0; n < count; ++n) {
        word di = samples[n];
        word sav = di;
        for (int i = 0; i < 8; ++i) {
            const word ui = u[i];
            u[i] = sav;
            sav = ui + ((r[i] * di) >> 15);
            di = di + ((r[i] * ui) >> 15);
        }
        samples[n] = w15gsm::clamp16(di);
    }
    for (int i = 0; i < 8; ++i) state->u[i] = w15gsm::clamp16(u[i]);
}
}
