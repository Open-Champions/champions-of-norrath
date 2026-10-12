#include "lg_codec_init.cpp"

#include <cstdio>

int main() {
    if (lgCodecInit() != 0) return 1;
    std::puts("lgCodecInit passed");
    return 0;
}
