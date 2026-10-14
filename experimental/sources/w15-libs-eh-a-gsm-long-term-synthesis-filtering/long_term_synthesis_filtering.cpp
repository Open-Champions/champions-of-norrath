// Observed Gsm_Long_Term_Synthesis_Filtering at 0x002ef4b8, 272 bytes, pinned SLUS_205.65: the long-term (pitch)
// synthesis stage of the GSM decoder (gsm_decode calls it as (state, Nr, bcr, erp, drp)). In order:
//   * The lag Nr is kept when 40 <= Nr <= 120 (tested as an unsigned 64-bit difference from 40 below 81) and otherwise
//     replaced by the previous lag, the doubleword at state +2472; the lag in use is stored there.
//   * The gain is gsm_QLB[(int) bcr] (0x00448090, four doublewords; the index is taken as a signed 32-bit value).
//   * For k = 0..39: drp[k] = erp[k] + ((gain * drp[k - lag]) >> 15), in 64-bit arithmetic with no rounding term and no
//     saturation. drp points at the 40 words being produced; the words before it are the earlier output.
//   * Then the history is moved: drp[-120 + k] = drp[-80 + k] for k = 0..119.
// The product goes through __muldi3 (0x002f81c0). Unknown: that callee's body. Nothing was executed on the EE.
#include "../w15-libs-eh-a-gsm-common/gsm.hpp"

extern "C" {
extern w15gsm::word gsm_QLB[4];

void Gsm_Long_Term_Synthesis_Filtering(w15gsm::State* state, w15gsm::word lag, w15gsm::word gain_code,
                                       const w15gsm::word* erp, w15gsm::word* drp) {
    using w15gsm::word;
    if (static_cast<unsigned long long>(lag - 40) >= 81ull) lag = state->nrp;
    state->nrp = lag;
    const word gain = gsm_QLB[static_cast<int>(gain_code)];
    const int lag_words = static_cast<int>(lag);
    for (int k = 0; k < 40; ++k) drp[k] = erp[k] + ((gain * drp[k - lag_words]) >> 15);
    for (int k = 0; k < 120; ++k) drp[-120 + k] = drp[-80 + k];
}
}
