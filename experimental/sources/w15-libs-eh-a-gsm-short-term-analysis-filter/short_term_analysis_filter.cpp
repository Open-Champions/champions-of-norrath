// Observed Gsm_Short_Term_Analysis_Filter at 0x002f00c0, 956 bytes, pinned SLUS_205.65: the per-frame short-term
// analysis stage of the GSM encoder (gsm_encode calls it as (state, LARc, samples)). The frame index word at +2456 is read,
// then replaced by its exclusive-or with 1; LARpp[old] is "current" and LARpp[new] is "previous" (layout in
// ../w15-libs-eh-a-gsm-common/gsm.hpp). In order:
//   Decoding_of_the_coded_Log_Area_Ratios (0x002ef5c8) turns the 8 transmitted codes into the current set;
//   four coefficient sets are formed and mapped (larp_to_rp) and each drives Short_term_analysis_filtering (0x002ef8e0)
//   over a part of the 160 samples: 13 samples at s, 14 at s + 13, 13 at s + 27 and 120 at s + 40, with the first set
//   (previous >> 2, current >> 2, previous >> 1), the second (previous >> 1, current >> 1), the third (current >> 2,
//   previous >> 2, current >> 1) and the fourth the current set itself. All sums are plain 64-bit adds.
// The arrays use the index (int) of the old and new frame word: the old index is read as a 32-bit word.
// Unknown: nothing beyond the callees' bodies; nothing was executed on the EE.
#include "../w15-libs-eh-a-gsm-common/gsm.hpp"

extern "C" {
void Decoding_of_the_coded_Log_Area_Ratios(const w15gsm::word* codes, w15gsm::word* output);
void Short_term_analysis_filtering(w15gsm::State* state, const w15gsm::word* rp, int count, w15gsm::word* samples);

void Gsm_Short_Term_Analysis_Filter(w15gsm::State* state, const w15gsm::word* codes, w15gsm::word* samples) {
    using w15gsm::word;
    word* const current = state->LARpp[static_cast<int>(state->j)];
    const word next_index = state->j ^ 1;
    state->j = next_index;
    word* const previous = state->LARpp[static_cast<int>(next_index)];
    word rp[8];
    Decoding_of_the_coded_Log_Area_Ratios(codes, current);

    w15gsm::coefficients_first(previous, current, rp);
    w15gsm::larp_to_rp(rp);
    Short_term_analysis_filtering(state, rp, 13, samples);

    w15gsm::coefficients_second(previous, current, rp);
    w15gsm::larp_to_rp(rp);
    Short_term_analysis_filtering(state, rp, 14, samples + 13);

    w15gsm::coefficients_third(previous, current, rp);
    w15gsm::larp_to_rp(rp);
    Short_term_analysis_filtering(state, rp, 13, samples + 27);

    for (int i = 0; i < 8; ++i) rp[i] = current[i];
    w15gsm::larp_to_rp(rp);
    Short_term_analysis_filtering(state, rp, 120, samples + 40);
}
}
