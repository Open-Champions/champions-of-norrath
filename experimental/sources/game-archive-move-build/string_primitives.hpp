#pragma once
static_assert(__CHAR_BIT__ == 8 && sizeof(unsigned) == 4 && sizeof(int) == 4,
              "target32 scalar representation");
static_assert(sizeof(void*) == 4 && sizeof(__UINTPTR_TYPE__) == 4 &&
              sizeof(unsigned long long) == 8 &&
              __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__, "target32 byte addresses");
extern "C" {
unsigned norrath_factory_strlen(const char* text);
int norrath_factory_strncmp(const char* left, const char* right, unsigned count);
}
namespace norrath_string_detail {
using Address = unsigned;
using Word64 = unsigned long long;
struct Block16 { Word64 low; Word64 high; };
inline Address address(const void* p) {
    return static_cast<Address>(reinterpret_cast<__UINTPTR_TYPE__>(p));
}
inline const volatile unsigned char* bytes(Address a) {
    return reinterpret_cast<const volatile unsigned char*>(static_cast<__UINTPTR_TYPE__>(a));
}
inline unsigned char read_byte(Address a) { return *bytes(a); }
// Volatile byte accesses retain the complete required original block, including
// bytes after a NUL and the eager left16 tail read. Scope is stable ordinary RAM;
// neither original LD/LQ atomicity nor a device/fault/concurrency trace is claimed.
inline Word64 read8(Address a) {
    Word64 value = 0;
    for (unsigned byte = 0; byte < 8u; ++byte)
        value |= static_cast<Word64>(read_byte(a + byte)) << (8u * byte);
    return value;
}
inline Block16 read16(Address a) {
    const Word64 low = read8(a);
    const Word64 high = read8(a + 8u);
    return Block16{low, high};
}
// Borrow propagation can set additional bits after a zero byte; only the
// existence of a zero is used. This is also equivalent to the original PSUBB
// per-byte mask for the boolean question asked by the 16-byte paths.
inline bool contains_zero(Word64 value) {
    return ((value - 0x0101010101010101ULL) & ~value & 0x8080808080808080ULL) != 0;
}
inline bool contains_zero(Block16 value) {
    return contains_zero(value.low) || contains_zero(value.high);
}
inline bool equal(Block16 left, Block16 right) {
    return left.low == right.low && left.high == right.high;
}
inline int byte_compare(Address left, Address right, unsigned count) {
    if (count == 0u) return 0;
    for (;;) {
        const unsigned char a = read_byte(left);
        const unsigned char b = read_byte(right);
        // The original branch delay also reloads the left byte as unsigned.
        const unsigned char current_left = read_byte(left);
        if (a != b) {
            const unsigned char fresh_right = read_byte(right);
            const unsigned char fresh_left = read_byte(left);
            return static_cast<int>(fresh_left) - static_cast<int>(fresh_right);
        }
        count -= 1u;
        if (count == 0u || current_left == 0u) return 0;
        left += 1u;
        right += 1u;
    }
}
}
