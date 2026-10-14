#include <cstdio>
#include <cstring>

#include "clear_color.cpp"

int main() {
    const unsigned char expected[4] = {0x00, 0x00, 0x00, 0x80};
    unsigned char bytes[4];
    std::memcpy(bytes, &coreClearColor, 4);
    if (std::memcmp(bytes, expected, 4) != 0 || coreClearColor != 0x80000000u) {
        std::fprintf(stderr, "fail: coreClearColor bytes\n");
        return 1;
    }
    bytes[3] ^= 1u;
    unsigned changed;
    std::memcpy(&changed, bytes, 4);
    if (changed == coreClearColor) return 1;
    std::printf("coreClearColor checks passed\n");
    return 0;
}
