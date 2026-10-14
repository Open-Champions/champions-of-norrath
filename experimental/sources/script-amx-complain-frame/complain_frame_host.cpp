#include <cstdio>
extern "C" unsigned char norrath_script_complain_frame_word_view[4];
int main() {
    if (sizeof norrath_script_complain_frame_word_view != 4) return 1;
    const unsigned char expected[] = {0xf0, 0xd8, 0xff, 0xff};
    for (unsigned i = 0; i < 4; ++i)
        if (norrath_script_complain_frame_word_view[i] != expected[i]) return 2;
    const unsigned value = unsigned(expected[0]) | (unsigned(expected[1]) << 8)
        | (unsigned(expected[2]) << 16) | (unsigned(expected[3]) << 24);
    if (value != 0xffffd8f0u) return 3;
    std::puts("complain frame passed");
}
