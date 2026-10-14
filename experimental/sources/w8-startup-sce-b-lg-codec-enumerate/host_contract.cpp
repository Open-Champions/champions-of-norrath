#include "lg_codec_enumerate.cpp"

#include <cstdio>
#include <cstring>

int main() {
    if (lgCodecEnumerate(0) != &g_codecs[0] || lgCodecEnumerate(1) != &g_codecs[1]) return 1;
    if (lgCodecEnumerate(2) != 0 || lgCodecEnumerate(-1) != 0 || lgCodecEnumerate(1000) != 0) return 2;
    if (lgCodecEnumerate(0)->identifier != 10 || std::strcmp(lgCodecEnumerate(0)->name, "PCM") != 0) return 3;
    if (lgCodecEnumerate(1)->identifier != 0x12c || std::strcmp(lgCodecEnumerate(1)->name, "GSM") != 0) return 4;
    if (lgCodecEnumerate(1)->word_2 != 0x32c8u || lgCodecEnumerate(1)->word_4 != 0x21u) return 5;
    if (lgCodecEnumerate(0)->word_2 != 0x1f400u || lgCodecEnumerate(0)->word_3 != 0x140u) return 6;
    std::puts("lgCodecEnumerate passed");
    return 0;
}
