// Observed newWind__10StaticFirei at 0x0028eff8, 288 bytes. With c = icos(angle) and s = isin(angle),
// the pair (c, s) is normalized by one reciprocal square root (rsqrt.s, numerator 1.0) of c*c + s*s.
// The halfword at +114 is set to (int)(normalized c * 3.7333331) and the halfword at +116 to
// (int)(normalized s * 3.7333331), both truncated to 16 bits. Then eRand(this) is drawn once, and
// the records of the ring buffer (10 bytes each, base = word at +1188) are visited from the index in
// the halfword at +122 up to, but not including, the index in the halfword at +120; the index wraps
// to 0 when it reaches the halfword at +118. For each visited index whose low two bits equal the low
// two bits of the roll, the record's halfword at +6 gains the halfword at +114 and its halfword at
// +8 gains the halfword at +116 (16-bit wraparound). When the two bounds are equal nothing is
// visited. Returns nothing. Unknown: what the records are; the names suggest wind but that is not
// established here. Nothing here ran on the EE.
#include "../w10-actor-n-common/access.hpp"

extern "C" {
float icos__Fi(int angle);
float isin__Fi(int angle);
unsigned eRand__10GameObject(void* self);

namespace {
__attribute__((always_inline)) inline float reciprocal_sqrt(float length_squared) {
#if defined(__mips__)
    float result;
    float one = 1.0f;
    __asm__ volatile("rsqrt.s %0, %1, %2" : "=f"(result) : "f"(one), "f"(length_squared));
    return result;
#else
    return 1.0f / __builtin_sqrtf(length_squared);
#endif
}
}  // namespace

void newWind__10StaticFirei(void* self, int angle) {
    using namespace w10n;
    unsigned char* const o = static_cast<unsigned char*>(self);
    const float c = icos__Fi(angle);
    const float s = isin__Fi(angle);
    int index = half(o, 122);
    const float scale = reciprocal_sqrt(c * c + s * s);
    const float k = bits(0x406eeeeeu);
    const float s_n = s * scale;
    const float c_n = c * scale;
    half(o, 116) = static_cast<short>(static_cast<int>(s_n * k));
    half(o, 114) = static_cast<short>(static_cast<int>(c_n * k));
    const unsigned roll = eRand__10GameObject(self);
    if (index == half(o, 120)) return;
    const unsigned lane = roll & 3u;
    do {
        unsigned char* const record = from_word(word(o, 1188)) + index * 10;
        const bool pick = (static_cast<unsigned>(index) & 3u) == lane;
        ++index;
        if (pick) {
            half(record, 6) = static_cast<short>(static_cast<unsigned short>(half(record, 6)) + static_cast<unsigned short>(half(o, 114)));
            half(record, 8) = static_cast<short>(static_cast<unsigned short>(half(record, 8)) + static_cast<unsigned short>(half(o, 116)));
        }
        if (index == half(o, 118)) index = 0;
    } while (index != half(o, 120));
}
}
