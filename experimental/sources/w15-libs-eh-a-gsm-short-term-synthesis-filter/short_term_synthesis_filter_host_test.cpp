// Host check only; not an EE build input. Build the source with -DGsm_Short_Term_Synthesis_Filter=w15_gsm_synthesis;
// this file is compiled without it. Same approach as the analysis stage test: stub callees, predictable decoded set.
#include <cstdio>
#include <cstring>
#include "../w15-libs-eh-a-gsm-common/gsm.hpp"
using w15gsm::State;
using w15gsm::word;
extern "C" void Decoding_of_the_coded_Log_Area_Ratios(const word* codes, word* output) {
    for (int i = 0; i < 8; ++i) output[i] = codes[i] * 1000 + 7;
}
static int calls; static int counts[4]; static const word* input_ptrs[4]; static short* output_ptrs[4]; static word rps[4][8];
extern "C" void Short_term_synthesis_filtering(State* state, const word* rrp, int count, const word* input, short* output) {
    counts[calls] = count; input_ptrs[calls] = input; output_ptrs[calls] = output;
    for (int i = 0; i < 8; ++i) rps[calls][i] = rrp[i];
    ++calls;
}
extern "C" void w15_gsm_synthesis(State* state, const word* codes, const word* wt, short* target);
static word map(word x) {
    word t = x < 0 ? -x : x;
    word m = t < 11059 ? t * 2 : (t < 20070 ? t + 11059 : t / 4 + 26112);
    return x < 0 ? -m : m;
}
int main() {
    static State s;
    word codes[8] = {10, 20, 30, 40, 50, 60, 70, 80};
    static word wt[160];
    static short target[160];
    for (int start_j = 0; start_j < 2; ++start_j) {
        calls = 0;
        s.j = start_j;
        for (int k = 0; k < 16; ++k) s.LARpp[k / 8][k % 8] = (k % 2 ? 1 : -1) * (500 + 3000 * k);
        word saved_prev[8];
        for (int i = 0; i < 8; ++i) saved_prev[i] = s.LARpp[start_j ^ 1][i];
        w15_gsm_synthesis(&s, codes, wt, target);
        if (calls != 4 || s.j != (start_j ^ 1)) return 1;
        if (counts[0] != 13 || counts[1] != 14 || counts[2] != 13 || counts[3] != 120) return 2;
        if (input_ptrs[0] != wt || input_ptrs[1] != wt + 13 || input_ptrs[2] != wt + 27 || input_ptrs[3] != wt + 40) return 3;
        if (output_ptrs[0] != target || output_ptrs[1] != target + 13 || output_ptrs[2] != target + 27 || output_ptrs[3] != target + 40) return 4;
        for (int i = 0; i < 8; ++i) {
            const word prev = saved_prev[i], cur = codes[i] * 1000 + 7;
            const word want[4] = {map((prev >> 2) + (cur >> 2) + (prev >> 1)), map((prev >> 1) + (cur >> 1)),
                                  map((cur >> 2) + (prev >> 2) + (cur >> 1)), map(cur)};
            for (int c = 0; c < 4; ++c) if (rps[c][i] != want[c]) return 5;
        }
    }
    std::puts("gsm_short_term_synthesis_filter ok");
    return 0;
}
