// Observed Gsm_Short_Term_Synthesis_Filter at 0x002f0480, 988 bytes, pinned SLUS_205.65: the per-frame short-term
// synthesis stage of the GSM decoder (gsm_decode calls it as (state, LARc, wt, target)). It has the same frame-index and
// coefficient steps as the analysis stage (Gsm_Short_Term_Analysis_Filter, 0x002f00c0) and drives
// Short_term_synthesis_filtering (0x002efce0) instead: 13 words of wt at wt and 13 16-bit outputs at target, then 14 at
// +104 bytes / +26 bytes, 13 at +216 / +54, and 120 at +320 / +80. Coefficient sets and the mapping to reflection
// coefficients are the shared ones in ../w15-libs-eh-a-gsm-common/gsm.hpp.
// Unknown: nothing beyond the callees' bodies; nothing was executed on the EE.
#include "../w15-libs-eh-a-gsm-common/gsm.hpp"

extern "C" {
void Decoding_of_the_coded_Log_Area_Ratios(const w15gsm::word* codes, w15gsm::word* output);
void Short_term_synthesis_filtering(w15gsm::State* state, const w15gsm::word* rrp, int count, const w15gsm::word* input,
                                    short* output);

void Gsm_Short_Term_Synthesis_Filter(w15gsm::State* state, const w15gsm::word* codes, const w15gsm::word* wt, short* target) {
    using w15gsm::word;
    word* const current = state->LARpp[static_cast<int>(state->j)];
    const word next_index = state->j ^ 1;
    state->j = next_index;
    word* const previous = state->LARpp[static_cast<int>(next_index)];
    word rrp[8];
    Decoding_of_the_coded_Log_Area_Ratios(codes, current);

    w15gsm::coefficients_first(previous, current, rrp);
    w15gsm::larp_to_rp(rrp);
    Short_term_synthesis_filtering(state, rrp, 13, wt, target);

    w15gsm::coefficients_second(previous, current, rrp);
    w15gsm::larp_to_rp(rrp);
    Short_term_synthesis_filtering(state, rrp, 14, wt + 13, target + 13);

    w15gsm::coefficients_third(previous, current, rrp);
    w15gsm::larp_to_rp(rrp);
    Short_term_synthesis_filtering(state, rrp, 13, wt + 27, target + 27);

    for (int i = 0; i < 8; ++i) rrp[i] = current[i];
    w15gsm::larp_to_rp(rrp);
    Short_term_synthesis_filtering(state, rrp, 120, wt + 40, target + 40);
}
}
