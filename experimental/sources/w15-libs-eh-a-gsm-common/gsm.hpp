// Shared definitions for the w15-libs-eh-a GSM 06.10 sources. Nothing here is an EE input by itself.
// Observed from the Gsm_ routines at 0x002ecee0..0x002f0860: every sample, coefficient and table entry is a 64-bit
// word, and the coder/decoder state is one record whose fields these routines reach at fixed byte offsets:
//      0  280 words  delay line (gsm_encode points dp0 at it and dp at +960)
//   2240  word       first preprocessing state (read by gsm_encode, outside this package)
//   2248  word       second preprocessing state
//   2256  int        last output of the preprocessing stage
//   2264  8 words    short-term analysis filter memory (u)
//   2328  2 x 8 words  the two interpolated log-area-ratio sets, selected by the word at 2456
//   2456  word       index (0 or 1) of the set used for the current frame; flipped once per frame
//   2464  word       long-term-predictor "cut" switch: nonzero selects the shorter search
//   2472  word       last long-term lag (set by Gsm_Long_Term_Synthesis_Filtering)
//   2480  9 words    short-term synthesis filter memory (v)
// Names are mine; nothing in the image names these fields.
#ifndef W15_LIBS_EH_A_GSM_HPP
#define W15_LIBS_EH_A_GSM_HPP

namespace w15gsm {

typedef long long word;

struct State {
    word dp0[280];
    word z1;
    word L_z2;
    int mp;
    int pad_2260;
    word u[8];
    word LARpp[2][8];
    word j;
    word ltp_cut;
    word nrp;
    word v[9];
};
static_assert(sizeof(word) == 8, "64-bit word");
static_assert(__builtin_offsetof(State, z1) == 2240 && __builtin_offsetof(State, mp) == 2256, "state layout");
static_assert(__builtin_offsetof(State, u) == 2264 && __builtin_offsetof(State, LARpp) == 2328, "state layout");
static_assert(__builtin_offsetof(State, j) == 2456 && __builtin_offsetof(State, ltp_cut) == 2464, "state layout");
static_assert(__builtin_offsetof(State, nrp) == 2472 && __builtin_offsetof(State, v) == 2480, "state layout");

// Saturate to the signed 16-bit range, as the image does with a pair of slt tests.
inline word clamp16(word value) {
    if (value >= 32767) return 32767;
    return value < -32768 ? -32768 : value;
}

// Interpolation of the two log-area-ratio sets into the eight coefficients used for one part of a frame, as the
// short-term filters (analysis at 0x002f00c0, synthesis at 0x002f0480) form them: plain 64-bit shifts and adds, no
// saturation. `previous` is the set of the other frame index (LARpp[j ^ 1]) and `current` is LARpp[j].
inline void coefficients_first(const word* previous, const word* current, word* out) {      // samples 0..12
    for (int i = 0; i < 8; ++i) out[i] = (previous[i] >> 2) + (current[i] >> 2) + (previous[i] >> 1);
}
inline void coefficients_second(const word* previous, const word* current, word* out) {     // samples 13..26
    for (int i = 0; i < 8; ++i) out[i] = (previous[i] >> 1) + (current[i] >> 1);
}
inline void coefficients_third(const word* previous, const word* current, word* out) {      // samples 27..39
    for (int i = 0; i < 8; ++i) out[i] = (current[i] >> 2) + (previous[i] >> 2) + (current[i] >> 1);
}

// Conversion of eight log-area ratios to reflection coefficients, applied in place. A negative value is negated
// (in 64 bits, with no special case for the most negative 16-bit value), mapped, and negated back. The map is
// 2t below 11059, t + 11059 below 20070, and (t >> 2) + 26112 otherwise, with no saturation.
inline void larp_to_rp(word* values) {
    for (int i = 0; i < 8; ++i) {
        const word x = values[i];
        const word t = x < 0 ? -x : x;
        const word mapped = t < 11059 ? t * 2 : (t < 20070 ? t + 11059 : (t >> 2) + 26112);
        values[i] = x < 0 ? -mapped : mapped;
    }
}

}  // namespace w15gsm

#endif
