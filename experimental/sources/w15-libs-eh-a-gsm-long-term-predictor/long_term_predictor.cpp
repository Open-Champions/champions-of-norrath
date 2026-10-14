// Observed Gsm_Long_Term_Predictor at 0x002ef410, 164 bytes, pinned SLUS_205.65: the long-term (pitch) predictor
// stage of the GSM encoder. Seven arguments: the coder state, the 40 current words d, the history dp, the output
// e, the output dpp, and pointers to the lag Nc and gain code bc (all words are 64-bit; the last three arguments
// arrive in registers t0..t2). In order:
//   * if the state word at byte 2464 is nonzero, Cut_Calculation_of_the_LTP_parameters (0x002ef028) is called as
//     (d, dp, bc, Nc); otherwise Calculation_of_the_LTP_parameters (0x002ee650) with the same arguments;
//   * then Long_term_analysis_filtering (0x002ef250) is tail-called as (*bc, *Nc, dp, d, dpp, e).
// Unknown: the callee bodies. Nothing was executed on the EE.
#include "../w15-libs-eh-a-gsm-common/gsm.hpp"

extern "C" {
void Cut_Calculation_of_the_LTP_parameters(const w15gsm::word* d, const w15gsm::word* dp, w15gsm::word* bc, w15gsm::word* nc);
void Calculation_of_the_LTP_parameters(const w15gsm::word* d, const w15gsm::word* dp, w15gsm::word* bc, w15gsm::word* nc);
void Long_term_analysis_filtering(w15gsm::word bc, w15gsm::word nc, const w15gsm::word* dp, const w15gsm::word* d,
                                  w15gsm::word* dpp, w15gsm::word* e);

void Gsm_Long_Term_Predictor(w15gsm::State* state, const w15gsm::word* d, const w15gsm::word* dp, w15gsm::word* e,
                             w15gsm::word* dpp, w15gsm::word* nc, w15gsm::word* bc) {
    if (state->ltp_cut != 0)
        Cut_Calculation_of_the_LTP_parameters(d, dp, bc, nc);
    else
        Calculation_of_the_LTP_parameters(d, dp, bc, nc);
    Long_term_analysis_filtering(*bc, *nc, dp, d, dpp, e);
}
}
