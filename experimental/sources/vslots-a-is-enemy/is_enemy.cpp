// Observed isEnemy__8CreatureP10GameObject at 0x0010b4a8, 176 bytes.
// Returns 0, 1, or 2. The other object's word at +12 supplies the flag bits.
// Bit 0x8, a missing 0x20/0x40 bit, a null other, or the same object returns 0.
// Halfwords at +792 on both objects select the remaining paths. When bit 0x20
// is set, bits 0x04000000 and 0x800 of the doubleword at other+160 also return 0.
// Reaching the end returns 2 when bit 0x40 is set and 1 when it is clear.

namespace {
unsigned read_word(const unsigned char* p) {
    return unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}
int read_half(const unsigned char* p) {
    unsigned value = unsigned(p[0]) | (unsigned(p[1]) << 8);
    return value < 0x8000u ? static_cast<int>(value) : static_cast<int>(value) - 0x10000;
}
unsigned long long read_double(const unsigned char* p) {
    unsigned long long value = 0;
    for (unsigned i = 0; i < 8u; ++i) value |= static_cast<unsigned long long>(p[i]) << (8u * i);
    return value;
}
}

extern "C" int isEnemy__8CreatureP10GameObject(void* self, void* other) {
    if (other == nullptr || other == self) return 0;
    const auto* creature = static_cast<const unsigned char*>(self);
    const auto* target = static_cast<const unsigned char*>(other);
    const unsigned flags = read_word(target + 12);
    if ((flags & 0x60u) == 0u || (flags & 0x8u) != 0u) return 0;
    const int own = read_half(creature + 792);
    if (own != 0) {
        if ((flags & 0x40u) != 0u) return 0;
        if (read_half(target + 792) != 0) return 0;
    } else if ((flags & 0x40u) == 0u) {
        if (read_half(target + 792) == 0) return 0;
    }
    if ((flags & 0x20u) != 0u) {
        const unsigned long long bits = read_double(target + 160);
        if ((bits & 0x04000000ull) != 0ull || (bits & 0x800ull) != 0ull) return 0;
    }
    return (flags & 0x40u) != 0u ? 2 : 1;
}
