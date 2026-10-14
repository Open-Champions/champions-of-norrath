#include "rpc_initialized.cpp"

#include <cstdio>
#include <cstring>

int main() {
    static_assert(sizeof norrath_sound_rpc_initialized_word_view == 4, "size");
    static const unsigned char expected[] = {0, 0, 0, 0};
    static_assert(sizeof expected == 4, "expected size");
    if (std::memcmp(norrath_sound_rpc_initialized_word_view, expected, 4) != 0) return 1;
    std::puts("rpc_initialized passed");
    return 0;
}
