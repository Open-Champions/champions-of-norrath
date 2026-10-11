#include "lump_find.hpp"
namespace {
using Word = unsigned;
Word address(const char* p) {
    return static_cast<Word>(reinterpret_cast<__UINTPTR_TYPE__>(p));
}
const unsigned char* bytes(Word a) {
    return reinterpret_cast<const unsigned char*>(static_cast<__UINTPTR_TYPE__>(a));
}
void read_block(unsigned char* out, Word at, Word width) {
    for (Word i = 0; i != width; ++i) out[i] = bytes(at + i)[0];
}
bool equal_block(const unsigned char* left, const unsigned char* right, Word width) {
    for (Word i = 0; i != width; ++i) if (left[i] != right[i]) return false;
    return true;
}
bool contains_zero(const unsigned char* block, Word width) {
    for (Word i = 0; i != width; ++i) if (block[i] == 0) return true;
    return false;
}
int compare_bytes(Word left, Word right) {
    while (bytes(left)[0] != 0) {
        if (bytes(left)[0] != bytes(right)[0]) break;
        ++left;
        ++right;
    }
    // The original returns the actual unsigned-byte difference, not only its sign.
    return static_cast<int>(bytes(left)[0]) - static_cast<int>(bytes(right)[0]);
}
}
extern "C" int norrath_string_compare(const char* left_string, const char* right_string) {
    Word left = address(left_string), right = address(right_string);
    const Word alignment = left | right;
    if ((alignment & 7u) == 0) {
        alignas(16) unsigned char left_block[16]{}, right_block[16]{};
        // This initial eight-byte right read also occurs on the original16-byte path.
        read_block(right_block, right, 8u);
        const Word width = (alignment & 15u) == 0 ? 16u : 8u;
        read_block(left_block, left, width);
        if (width == 16u) read_block(right_block, right, width);
        if (equal_block(left_block, right_block, width)) {
            // The original reloads left after its first equal-block comparison.
            read_block(left_block, left, width);
            for (;;) {
                // Original128-bit packed-byte or64-bit borrow detection tests any zero.
                if (contains_zero(left_block, width)) return 0;
                left += width;
                right += width;
                read_block(left_block, left, width);
                read_block(right_block, right, width);
                if (!equal_block(left_block, right_block, width)) break;
            }
        }
    }
    return compare_bytes(left, right);
}
