// Host check only; not an EE build input. Build the source with -DShort_term_synthesis_filtering=w15_st_synthesis;
// this file is compiled without it. The reference below spells the stage equations out and is compared over a
// pseudo-random run, with extra checks of the saturation points and the zero-count case.
#include <cstdio>
#include "../w15-libs-eh-a-gsm-common/gsm.hpp"
using w15gsm::State;
using w15gsm::word;
extern "C" void w15_st_synthesis(State* state, const word* rrp, int count, const word* input, short* output);
static word sat(word v) { return v > 32767 ? 32767 : (v < -32768 ? -32768 : v); }
int main() {
    static State a, b, c;
    word rrp[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    word in[6] = {5, -7, 40000, -40000, 100, 0};
    short out[6];
    w15_st_synthesis(&a, rrp, 6, in, out);
    // zero reflection coefficients: the output is the saturated input and v[0] is the last output
    for (int i = 0; i < 6; ++i) if (out[i] != sat(in[i])) return 1;
    if (a.v[0] != 0) return 2;
    for (int i = 0; i < 8; ++i) b.v[i] = (i % 2 ? 70000 : -70000);
    word keep[1] = {1};
    short keep_out[1] = {55};
    w15_st_synthesis(&b, rrp, 0, keep, keep_out);
    for (int i = 0; i < 8; ++i) if (b.v[i] != (i % 2 ? 32767 : -32768)) return 3;
    if (keep_out[0] != 55) return 4;
    unsigned long long s = 88172645463325252ull;
    word ref_v[8] = {0};
    for (int round = 0; round < 3000; ++round) {
        word r[8], samples[13];
        short got[13];
        for (int i = 0; i < 8; ++i) { s ^= s << 13; s ^= s >> 7; s ^= s << 17; r[i] = static_cast<word>(s % 65536) - 32768; }
        for (int i = 0; i < 13; ++i) { s ^= s << 13; s ^= s >> 7; s ^= s << 17; samples[i] = static_cast<word>(s % 80000) - 40000; }
        short want[13];
        for (int n = 0; n < 13; ++n) {
            word sri = samples[n];
            word nv[9];
            for (int i = 7; i >= 0; --i) {
                sri = sri - (r[i] * ref_v[i] >> 15);
                if (i == 0) sri = sat(sri);
                nv[i + 1] = ref_v[i] + (r[i] * sri >> 15);
            }
            for (int i = 1; i < 8; ++i) ref_v[i] = nv[i];
            ref_v[0] = sri;
            want[n] = static_cast<short>(sri);
        }
        for (int i = 0; i < 8; ++i) ref_v[i] = sat(ref_v[i]);
        w15_st_synthesis(&c, r, 13, samples, got);
        for (int i = 0; i < 13; ++i) if (got[i] != want[i]) return 5;
        for (int i = 0; i < 8; ++i) if (c.v[i] != ref_v[i]) return 6;
    }
    std::puts("short_term_synthesis_filtering ok");
    return 0;
}
