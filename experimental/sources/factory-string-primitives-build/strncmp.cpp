#include "string_primitives.hpp"
extern "C" int norrath_factory_strncmp(const char* left_text, const char* right_text,
                                      unsigned count) {
    using namespace norrath_string_detail;
    if (count == 0u) return 0;
    Address left = address(left_text);
    Address right = address(right_text);
    const Address combined = left | right;
    if ((combined & 7u) != 0u) return byte_compare(left, right, count);
    if ((combined & 15u) == 0u && count >= 16u) {
        Block16 left_block = read16(left);
        Block16 right_block = read16(right);
        if (!equal(left_block, right_block)) return byte_compare(left, right, count);
        count -= 16u;
        for (;;) {
            if (count == 0u) return 0;
            if (contains_zero(read16(left))) return 0;
            left += 16u;
            const bool short_tail = count < 16u;
            // The actual LQ at 30501c occurs even when the next branch takes
            // the short tail. The forced read helper preserves its envelope.
            left_block = read16(left);
            right += 16u;
            if (short_tail) return byte_compare(left, right, count);
            right_block = read16(right);
            if (!equal(left_block, right_block)) return byte_compare(left, right, count);
            count -= 16u;
        }
    }
    // The 8-byte mode stays selected; it never upgrades after advancing pointers.
    if (count < 8u) return byte_compare(left, right, count);
    Word64 left_word = read8(left);
    Word64 right_word = read8(right);
    if (left_word != right_word) return byte_compare(left, right, count);
    count -= 8u;
    for (;;) {
        if (count == 0u) return 0;
        if (contains_zero(read8(left))) return 0;
        left += 8u;
        right += 8u;
        if (count < 8u) return byte_compare(left, right, count);
        left_word = read8(left);
        right_word = read8(right);
        if (left_word != right_word) return byte_compare(left, right, count);
        count -= 8u;
    }
}
