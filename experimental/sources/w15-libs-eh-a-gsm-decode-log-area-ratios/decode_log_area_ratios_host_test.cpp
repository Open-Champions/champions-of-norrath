// Host check only; not an EE build input. Build the source with -DDecoding_of_the_coded_Log_Area_Ratios=w15_decode_lar;
// this file is compiled without it. Worked values: the middle code of each field decodes to a value computed by hand
// from the formula in the source comment, then a saturation case and a sweep against a floating-point-free reference.
#include <cstdio>
#include "../w15-libs-eh-a-gsm-common/gsm.hpp"
using w15gsm::word;
extern "C" void w15_decode_lar(const word* codes, word* output);
static word sat(word v) { return v > 32767 ? 32767 : (v < -32768 ? -32768 : v); }
int main() {
    const word mic[8] = {-32, -32, -16, -16, -8, -8, -4, -4};
    const word b[8] = {0, 0, 2048, -2560, 94, -1792, -341, -1144};
    const word inva[8] = {13107, 13107, 13107, 13107, 19223, 17476, 31454, 29708};
    word codes[8] = {32, 32, 16, 16, 8, 8, 4, 4};   // code + mic == 0: only the -2*b term remains
    word out[8];
    w15_decode_lar(codes, out);
    for (int i = 0; i < 8; ++i) {
        word want = sat((inva[i] * (-2 * b[i])) >> 15) * 2;
        want = sat(want);
        if (out[i] != want) return 1;
    }
    word extreme[8] = {1 << 20, -(1 << 20), 1 << 20, -(1 << 20), 63, 0, 7, 0};
    w15_decode_lar(extreme, out);
    if (out[0] != 32767 || out[1] != -32768 || out[2] != 32767 || out[3] != -32768) return 2;
    for (word base = -70; base < 120; ++base) {
        for (int i = 0; i < 8; ++i) codes[i] = base + i;
        w15_decode_lar(codes, out);
        for (int i = 0; i < 8; ++i) {
            word t = ((codes[i] + mic[i]) * 1024) - 2 * b[i];
            t = (inva[i] * t) >> 15;
            if (out[i] != sat(t * 2)) return 3;
        }
    }
    std::puts("decode_log_area_ratios ok");
    return 0;
}
