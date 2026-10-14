// Observed Decoding_of_the_coded_Log_Area_Ratios at 0x002ef5c8, 788 bytes (a local routine of the GSM codec), pinned
// SLUS_205.65. Arguments: the 8 transmitted log-area-ratio codes and an 8-word output. Output i is
//   clamp16(2 * ((INVA[i] * (((code[i] + MIC[i]) << 10) - 2 * B[i])) >> 15))
// in 64-bit arithmetic, with no rounding term, where the image folds each constant multiply into shifts and adds. The
// constants, by index 0..7 as the folded constants decode:
//   MIC  -32 -32 -16 -16  -8   -8   -4    -4
//   B      0   0 2048 -2560 94 -1792 -341 -1144
//   INVA 13107 13107 13107 13107 19223 17476 31454 29708
// (the products folded into the shifts and adds are 13107 = 3 * 17 * 257, and the others as listed). The results are
// saturated to [-32768, 32767] after the doubling.
// Unknown: nothing beyond the arithmetic; nothing was executed on the EE.
#include "../w15-libs-eh-a-gsm-common/gsm.hpp"

extern "C" {
void Decoding_of_the_coded_Log_Area_Ratios(const w15gsm::word* codes, w15gsm::word* output) {
    using w15gsm::word;
    static const word kMic[8] = {-32, -32, -16, -16, -8, -8, -4, -4};
    static const word kB[8] = {0, 0, 2048, -2560, 94, -1792, -341, -1144};
    static const word kInva[8] = {13107, 13107, 13107, 13107, 19223, 17476, 31454, 29708};
    for (int i = 0; i < 8; ++i) {
        word t = ((codes[i] + kMic[i]) << 10) - 2 * kB[i];
        t = (kInva[i] * t) >> 15;
        output[i] = w15gsm::clamp16(t * 2);
    }
}
}
