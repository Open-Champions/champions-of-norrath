// Host check only; not an EE build input. Build the source with -DGsm_Long_Term_Synthesis_Filtering=w15_lt_synthesis;
// this file is compiled without it and defines gsm_QLB with the observed four values. A reference written with a
// scratch copy of the whole buffer (so the in-place update order cannot matter) is compared over every lag from -5
// to 130 and every gain code, together with the lag-replacement rule and the history move.
#include <cstdio>
#include "../w15-libs-eh-a-gsm-common/gsm.hpp"
using w15gsm::State;
using w15gsm::word;
extern "C" {
word gsm_QLB[4] = {3277, 11469, 21299, 32767};
void w15_lt_synthesis(State* state, word lag, word gain_code, const word* erp, word* drp);
}
enum { kBase = 200, kSize = 260 };
int main() {
    static State s;
    word erp[40];
    for (int i = 0; i < 40; ++i) erp[i] = 100 * i - 1900;
    int checked = 0;
    for (word lag = -5; lag <= 130; ++lag) {
        for (word code = 0; code < 4; ++code) {
            word buffer[kSize], want[kSize];
            for (int i = 0; i < kSize; ++i) buffer[i] = want[i] = ((i * 37 + lag * 11 + code) % 2001) - 1000;
            s.nrp = 61;
            word used = (lag >= 40 && lag <= 120) ? lag : 61;
            for (int k = 0; k < 40; ++k) want[kBase + k] = erp[k] + ((gsm_QLB[code] * want[kBase + k - used]) >> 15);
            word copy[kSize];
            for (int i = 0; i < kSize; ++i) copy[i] = want[i];
            for (int k = 0; k < 120; ++k) want[kBase - 120 + k] = copy[kBase - 80 + k];
            w15_lt_synthesis(&s, lag, code, erp, buffer + kBase);
            if (s.nrp != used) { std::printf("lag %lld nrp %lld\n", lag, s.nrp); return 1; }
            for (int i = 0; i < kSize; ++i) if (buffer[i] != want[i]) { std::printf("lag %lld code %lld index %d\n", lag, code, i); return 2; }
            ++checked;
        }
    }
    if (checked != 136 * 4) return 3;
    std::puts("gsm_long_term_synthesis_filtering ok");
    return 0;
}
