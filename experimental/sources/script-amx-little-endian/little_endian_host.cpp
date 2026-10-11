#include <cstdio>
extern "C" unsigned char norrath_amx_little_endian_word_view[4];
int main() {
    if (sizeof norrath_amx_little_endian_word_view != 4) return 1;
    for (unsigned i = 0; i < 4; ++i)
        if (norrath_amx_little_endian_word_view[i] != 0) return 2;
    norrath_amx_little_endian_word_view[0] = 1;
    if (norrath_amx_little_endian_word_view[0] != 1) return 3;
    std::puts("little-endian word passed");
}
