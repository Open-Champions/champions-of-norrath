// initNoiseTable__Fv at 0x001581b8, 452 bytes, read from the pinned
// SLUS-20565 image. Fresh compilation is not an original-byte match. The
// noise table has not been built on the EE.
//
// Four powers are powf(8, -1), powf(4, -1), powf(2, -1), and powf(1, -1).
// eRandState at 0x00483e58 is the seed. Each generated sample uses
// seed = seed * 0x19660d + 0x3c6ef35f, then (seed >> 16) * 2^-16 - 0.5.
// Samples at output indexes 241 through 255 use 0 in place of that centered
// value. Octave n is skipped when the output index's low n bits are not all
// zero; a skipped octave adds its previous sample again. The four octave
// accumulators are multiplied by the four powers, then by the .lit4 word
// 0x471c4000 (40000) at 0x0047fef8. cvt.w.s stores 256 halfwords at noiseTable,
// 0x006021c8. The updated seed is written back to eRandState.
//
// The original accumulator words above the first, and the sample words, are
// not cleared before the first read. Those stack words are unknown. This
// source starts them at zero. That is a stand-in, not a recovered value.
extern "C" {
extern int eRandState;
extern unsigned char noiseTable[];
float powf(float base, float exponent);

static unsigned float_bits(float value) {
    const auto* bytes = reinterpret_cast<const unsigned char*>(&value);
    return static_cast<unsigned>(bytes[0]) | (static_cast<unsigned>(bytes[1]) << 8) |
           (static_cast<unsigned>(bytes[2]) << 16) | (static_cast<unsigned>(bytes[3]) << 24);
}

static float bits_float(unsigned bits) {
    unsigned char bytes[4] = {
        static_cast<unsigned char>(bits), static_cast<unsigned char>(bits >> 8),
        static_cast<unsigned char>(bits >> 16), static_cast<unsigned char>(bits >> 24)};
    float value = 0.f;
    auto* raw = reinterpret_cast<unsigned char*>(&value);
    raw[0] = bytes[0];
    raw[1] = bytes[1];
    raw[2] = bytes[2];
    raw[3] = bytes[3];
    return value;
}

static int round_to_int(float value) {
#if defined(NORRATH_PARTICLES_HOST)
    const unsigned bits = float_bits(value);
    const unsigned raw_exp = (bits >> 23) & 0xffu;
    const unsigned sign = bits >> 31;
    if (raw_exp == 0u || raw_exp == 255u) return static_cast<int>(value);
    const int exponent = static_cast<int>(raw_exp) - 127;
    const unsigned magnitude = (bits & 0x7fffffu) | 0x800000u;
    if (exponent >= 31) return static_cast<int>(value);
    if (exponent < 0) {
        if (exponent < -1 || magnitude == 0x800000u) return 0;
        return sign != 0u ? -1 : 1;
    }
    int whole = 0;
    if (exponent >= 23) whole = static_cast<int>(magnitude << (exponent - 23));
    else {
        const unsigned shift = static_cast<unsigned>(23 - exponent);
        unsigned truncated = magnitude >> shift;
        const unsigned remainder = magnitude & ((1u << shift) - 1u);
        const unsigned half = 1u << (shift - 1u);
        if (remainder > half || (remainder == half && (truncated & 1u) != 0u)) ++truncated;
        whole = static_cast<int>(truncated);
    }
    return sign != 0u ? -whole : whole;
#else
    return static_cast<int>(value);
#endif
}

static void store_half(unsigned char* slot, unsigned short value) {
    slot[0] = static_cast<unsigned char>(value);
    slot[1] = static_cast<unsigned char>(value >> 8);
}

void initNoiseTable__Fv() {
    const float negative_one = bits_float(0xbf800000u);
    float powers[4];
    for (int index = 0; index < 4; ++index) {
        const float base = static_cast<float>(1 << (3 - index));
        powers[index] = powf(base, negative_one);
    }
    const float unit_scale = bits_float(0x37800000u);
    const float half = bits_float(0x3f000000u);
    const float one = bits_float(0x3f800000u);
    const float coefficient = bits_float(0x471c4000u);
    float accum[4] = {};
    float samples[4] = {};
    unsigned seed = static_cast<unsigned>(eRandState);
    for (int output = 0; output < 256; ++output) {
        for (int octave = 0; octave < 4; ++octave) {
            const unsigned frequency = 1u << octave;
            if ((static_cast<unsigned>(output) & (frequency - 1u)) == 0u) {
                seed = seed * 0x19660du + 0x3c6ef35fu;
                const float unit = static_cast<float>(seed >> 16) * unit_scale;
                float centered = 0.f;
                if (output < 241) centered = unit - half;
                const float amplitude = one / static_cast<float>(frequency);
                const float delta = centered - accum[octave];
                samples[octave] = delta * amplitude;
            }
            accum[octave] = accum[octave] + samples[octave];
        }
        float dot = 0.f;
        for (int octave = 0; octave < 4; ++octave) {
            const float term = accum[octave] * powers[octave];
            dot = dot + term;
        }
        const float scaled = dot * coefficient;
        const unsigned low = static_cast<unsigned>(round_to_int(scaled));
        store_half(noiseTable + output * 2, static_cast<unsigned short>(low));
    }
    eRandState = static_cast<int>(seed);
}
}
