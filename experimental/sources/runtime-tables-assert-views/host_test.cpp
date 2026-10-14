// Host comparison only. Not an EE build input.
#include "assert_views.cpp"

#include <cstdio>
#include <cstring>

int main() {
    if (sizeof norrath_assert_format_view != 49u) return 1;
    if (sizeof norrath_assert_context_word_view != 4u) return 2;
    // The exact observed format, terminator included.
    if (std::memcmp(norrath_assert_format_view, "Assertion \"%s\" failed in %s:%d  context:%s (%x)\n",
                    49) != 0)
        return 3;
    if (norrath_assert_format_view[48] != 0) return 4;
    // The context word images the address of the initial "none" string.
    const unsigned char* word = norrath_assert_context_word_view;
    const unsigned value = static_cast<unsigned>(word[0]) | (static_cast<unsigned>(word[1]) << 8) |
                           (static_cast<unsigned>(word[2]) << 16) | (static_cast<unsigned>(word[3]) << 24);
    if (value != 0x004840e8u) return 5;
    return 0;
}
