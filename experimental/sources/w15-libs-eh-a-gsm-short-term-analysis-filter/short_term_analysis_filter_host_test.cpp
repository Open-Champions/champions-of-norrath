// Host check only; not an EE build input. Build the source with -DGsm_Short_Term_Analysis_Filter=w15_gsm_analysis;
// this file is compiled without it. The two callees are stubs that record their arguments; the decoded set is made
// predictable (each output word is 1000 * code + 7) so the four coefficient sets can be checked by hand against the
// rules in the source comment.
#include <cstdio>
#include <cstring>
#include "../w15-libs-eh-a-gsm-common/gsm.hpp"
using w15gsm::State;
using w15gsm::word;
static word seen_codes[8]; static word* seen_decode_out; static const word* seen_decode_in;
extern "C" void Decoding_of_the_coded_Log_Area_Ratios(const word* codes, word* output) {
    seen_decode_in = codes; seen_decode_out = output;
    for (int i = 0; i < 8; ++i) { seen_codes[i] = codes[i]; output[i] = codes[i] * 1000 + 7; }
}
static int calls; static int counts[4]; static word* sample_ptrs[4]; static word rps[4][8]; static State* states[4];
extern "C" void Short_term_analysis_filtering(State* state, const word* rp, int count, word* samples) {
    states[calls] = state; counts[calls] = count; sample_ptrs[calls] = samples;
    for (int i = 0; i < 8; ++i) rps[calls][i] = rp[i];
    ++calls;
}
extern "C" void w15_gsm_analysis(State* state, const word* codes, word* samples);
static word map(word x) {
    word t = x < 0 ? -x : x;
    word m = t < 11059 ? t * 2 : (t < 20070 ? t + 11059 : t / 4 + 26112);
    return x < 0 ? -m : m;
}
int main() {
    static State s;
    word codes[8] = {10, 20, 30, 40, 50, 60, 70, 80};
    static word samples[160];
    for (int start_j = 0; start_j < 2; ++start_j) {
        calls = 0;
        s.j = start_j;
        for (int k = 0; k < 16; ++k) s.LARpp[k / 8][k % 8] = (k % 2 ? 1 : -1) * (500 + 3000 * k);   // varied sizes and signs
        word saved_prev[8];
        for (int i = 0; i < 8; ++i) saved_prev[i] = s.LARpp[start_j ^ 1][i];
        w15_gsm_analysis(&s, codes, samples);
        if (calls != 4 || s.j != (start_j ^ 1)) return 1;
        if (seen_decode_in != codes || seen_decode_out != s.LARpp[start_j]) return 2;     // decoded into the old index
        if (counts[0] != 13 || counts[1] != 14 || counts[2] != 13 || counts[3] != 120) return 3;
        if (sample_ptrs[0] != samples || sample_ptrs[1] != samples + 13 || sample_ptrs[2] != samples + 27 || sample_ptrs[3] != samples + 40) return 4;
        for (int c = 0; c < 4; ++c) if (states[c] != &s) return 5;
        for (int i = 0; i < 8; ++i) {
            const word prev = saved_prev[i], cur = codes[i] * 1000 + 7;
            const word want[4] = {map((prev >> 2) + (cur >> 2) + (prev >> 1)), map((prev >> 1) + (cur >> 1)),
                                  map((cur >> 2) + (prev >> 2) + (cur >> 1)), map(cur)};
            for (int c = 0; c < 4; ++c) if (rps[c][i] != want[c]) return 6;
        }
        if (std::memcmp(saved_prev, s.LARpp[start_j ^ 1], sizeof saved_prev) != 0) return 7;   // the previous set is untouched
    }
    // the sign rules of the mapping
    if (map(-11058) != -22116 || map(11059) != 22118 || map(-20070) != -(20070 / 4 + 26112) || map(0) != 0) return 8;
    std::puts("gsm_short_term_analysis_filter ok");
    return 0;
}
