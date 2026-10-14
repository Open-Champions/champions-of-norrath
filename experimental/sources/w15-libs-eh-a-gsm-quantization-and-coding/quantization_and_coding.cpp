// Observed Quantization_and_coding at 0x002ed700, 572 bytes (a local routine of the GSM codec), pinned SLUS_205.65.
// It quantises the 8 log-area ratios in place, one table row per index i (all arithmetic is 64-bit and unsaturated):
//   t = (((A[i] * x) >> 15) + B[i] + 256) >> 9 (arithmetic); x = t > MAC[i] ? MAC[i] - MIC[i] : (t < MIC[i] ? 0 : t - MIC[i])
// with, by index 0..7 (the image folds each multiplier into shifts and adds):
//   A    20480 20480 20480 20480 13964 15360 8534 9036
//   B        0     0  2048 -2560    94 -1792 -341 -1144
//   MAC     31    31    15    15     7     7    3     3
//   MIC    -32   -32   -16   -16    -8    -8   -4    -4
// The comparison against MAC is "t >= MAC + 1" as written in the image (slti MAC + 1); the result is then MAC - MIC.
// Unknown: nothing beyond the arithmetic; nothing was executed on the EE.
#include "../w15-libs-eh-a-gsm-common/gsm.hpp"

extern "C" {
void Quantization_and_coding(w15gsm::word* values) {
    using w15gsm::word;
    static const word kA[8] = {20480, 20480, 20480, 20480, 13964, 15360, 8534, 9036};
    static const word kB[8] = {0, 0, 2048, -2560, 94, -1792, -341, -1144};
    static const word kMac[8] = {31, 31, 15, 15, 7, 7, 3, 3};
    static const word kMic[8] = {-32, -32, -16, -16, -8, -8, -4, -4};
    for (int i = 0; i < 8; ++i) {
        word t = (((kA[i] * values[i]) >> 15) + kB[i] + 256) >> 9;
        values[i] = t > kMac[i] ? kMac[i] - kMic[i] : (t < kMic[i] ? 0 : t - kMic[i]);
    }
}
}
