// Host check only; not an EE build input. Build the source with -DShort_term_analysis_filtering=w15_st_analysis;
// this file is compiled without it. __muldi3 is not needed on the host (the compiler multiplies directly).
#include <cstdio>
#include "../w15-libs-eh-a-gsm-common/gsm.hpp"
using w15gsm::State;
using w15gsm::word;
extern "C" void w15_st_analysis(State* state, const word* rp, int count, word* samples);
static word sat(word v) { return v > 32767 ? 32767 : (v < -32768 ? -32768 : v); }
int main() {
    static State a, b;
    word rp[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    word in[10] = {5, -7, 100, 32767, -32768, 40000, -40000, 1, 2, 3};
    word out[10];
    for (int i = 0; i < 10; ++i) out[i] = in[i];
    w15_st_analysis(&a, rp, 10, out);
    // zero reflection coefficients: the output is the saturated input and u holds the last eight inputs, newest first
    for (int i = 0; i < 10; ++i) if (out[i] != sat(in[i])) return 1;
    for (int i = 0; i < 8; ++i) if (a.u[i] != sat(in[9 - i])) return 2;
    // a zero count only saturates the stored memory
    for (int i = 0; i < 8; ++i) b.u[i] = (i % 2 ? 50000 : -50000) + i;
    word untouched[1] = {77};
    w15_st_analysis(&b, rp, 0, untouched);
    for (int i = 0; i < 8; ++i) if (b.u[i] != (i % 2 ? 32767 : -32768)) return 3;
    if (untouched[0] != 77) return 4;
    // a reference with the stage equations spelled out, over a pseudo-random run
    unsigned long long s = 88172645463325252ull;
    static State c;
    word ref_u[8] = {0};
    for (int round = 0; round < 3000; ++round) {
        word r[8], samples[13], want[13];
        for (int i = 0; i < 8; ++i) { s ^= s << 13; s ^= s >> 7; s ^= s << 17; r[i] = static_cast<word>(s % 65536) - 32768; }
        for (int i = 0; i < 13; ++i) { s ^= s << 13; s ^= s >> 7; s ^= s << 17; samples[i] = static_cast<word>(s % 80000) - 40000; want[i] = samples[i]; }
        for (int n = 0; n < 13; ++n) {
            word di = want[n], sav = di;
            for (int i = 0; i < 8; ++i) {
                const word ui = ref_u[i];
                ref_u[i] = sav;
                sav = ui + (r[i] * di >> 15);
                di = di + (r[i] * ui >> 15);
            }
            want[n] = sat(di);
        }
        for (int i = 0; i < 8; ++i) ref_u[i] = sat(ref_u[i]);
        w15_st_analysis(&c, r, 13, samples);
        for (int i = 0; i < 13; ++i) if (samples[i] != want[i]) return 5;
        for (int i = 0; i < 8; ++i) if (c.u[i] != ref_u[i]) return 6;
    }
    std::puts("short_term_analysis_filtering ok");
    return 0;
}
