#include "iop_heap_bind.cpp"

#include <cstdio>
#include <cstring>

int main() {
    static_assert(sizeof norrath_sound_iop_heap_bind_word_view == 4, "size");
    static const unsigned char expected[] = {0xff, 0xff, 0xff, 0xff};
    static_assert(sizeof expected == 4, "expected size");
    if (std::memcmp(norrath_sound_iop_heap_bind_word_view, expected, 4) != 0) return 1;
    std::puts("iop_heap_bind passed");
    return 0;
}
