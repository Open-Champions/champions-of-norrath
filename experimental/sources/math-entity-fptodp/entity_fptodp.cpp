// Observed: megatileEntitize__FPci calls fptodp at 0x2fb320 (0x2694c8 and 0x269510).
// The body stores F12 and calls __unpack_f at 0x2ff4a8. A zero float exponent writes
// class word 2 and the sign, and does not write the exponent or fraction words.
// Any nonzero float exponent, including 255, writes class word 3, unbiased power
// (biased - 127), and a 32-bit fraction with the implicit bit at bit 30. fptodp
// then shifts that word left 30 and calls __make_dp / __pack_d. Class 2 packs to a
// signed zero. Class 3 with this power packs through the normal arm: field
// power+1023 and the fraction shifted right 8, with the implicit bit falling
// outside the 52-bit field.
// Inferred: norrath_entity_fptodp is that F12-to-GPR64 result. Float infinities and
// NaNs stay finite powers of 128; they are not IEEE double infinities or NaNs.
// Unknown: unwritten stack slots on the zero-exponent path are unused by class 2,
// and the pack routine's class 0/1/4 arms are not reached from this unpack.
static_assert(__FLT_MANT_DIG__ == 24, "binary32 mantissa");
static_assert(sizeof(float) == 4 && sizeof(unsigned long long) == 8, "float and GPR64");

static unsigned float_bits(float value) {
    union { float f; unsigned u; } bits;
    bits.f = value;
    return bits.u;
}

extern "C" unsigned long long norrath_entity_fptodp(float value) {
    const unsigned bits = float_bits(value);
    const unsigned sign = bits >> 31;
    const unsigned biased = (bits >> 23) & 0xffu;
    const unsigned fraction = bits & 0x7fffffu;
    if (biased == 0u) return static_cast<unsigned long long>(sign) << 63;
    const int power = static_cast<int>(biased) - 127;
    const unsigned word = (fraction << 7) | 0x40000000u;
    unsigned long long packed = static_cast<unsigned long long>(word) << 30;
    unsigned exponent = 0u;
    if (power < -1022) packed = 0ull;
    else if (power >= 1024) {
        exponent = 2047u;
        packed = 0ull;
    } else {
        exponent = static_cast<unsigned>(power) + 1023u;
        packed >>= 8;
    }
    return (packed & 0x000fffffffffffffULL) |
           (static_cast<unsigned long long>(exponent & 2047u) << 52) |
           (static_cast<unsigned long long>(sign) << 63);
}
