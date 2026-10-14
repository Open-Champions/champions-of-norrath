// Observed Short_term_synthesis_filtering at 0x002efce0, 992 bytes (a local routine of the GSM codec), pinned
// SLUS_205.65. Arguments: the codec state (filter memory v = 8 words kept from +2480), 8 reflection coefficients rrp, a
// sample count, the input array wt (64-bit words) and the output array of 16-bit samples. For each input word, with
// sri = the input, the stages i = 7 down to 0 do
//   sri = sri - ((rrp[i] * v[i]) >> 15); v[i + 1] = v[i] + ((rrp[i] * sri) >> 15)
// in 64-bit arithmetic with no rounding term. At stage 0 only, sri is saturated to [-32768, 32767] right after the
// subtraction, so the v[1] update uses the saturated value; it is stored as v[0] and written to the output as the low
// 16 bits. The image keeps v in registers and drops the store of v[8], whose value
// is never read; the nine-word field at +2480 still has that slot. After the last sample every v[0..7] is saturated to
// the same range and written back (also for a zero count).
// Multiplication goes through __muldi3 (0x002f81c0). Unknown: the callee body. Nothing was executed on the EE.
#include "../w15-libs-eh-a-gsm-common/gsm.hpp"

extern "C" {
void Short_term_synthesis_filtering(w15gsm::State* state, const w15gsm::word* rrp, int count, const w15gsm::word* input,
                                    short* output) {
    using w15gsm::word;
    word v[8], r[8];
    for (int i = 0; i < 8; ++i) {
        v[i] = state->v[i];
        r[i] = rrp[i];
    }
    for (int n = 0; n < count; ++n) {
        word sri = input[n];
        for (int i = 7; i >= 0; --i) {
            sri = sri - ((r[i] * v[i]) >> 15);
            if (i == 0) sri = w15gsm::clamp16(sri);
            if (i < 7) v[i + 1] = v[i] + ((r[i] * sri) >> 15);
        }
        v[0] = sri;
        output[n] = static_cast<short>(sri);
    }
    for (int i = 0; i < 8; ++i) state->v[i] = w15gsm::clamp16(v[i]);
}
}
