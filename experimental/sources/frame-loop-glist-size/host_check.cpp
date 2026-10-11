#include <cstdio>
#include <cstring>

#include "glist_size.cpp"

int main() {
    const unsigned char expected[4] = {0xb8, 0x88, 0x00, 0x00};
    unsigned char bytes[4];
    std::memcpy(bytes, &GLISTSIZE, 4);
    if (std::memcmp(bytes, expected, 4) != 0 || GLISTSIZE != 35000u) {
        std::fprintf(stderr, "fail: GLISTSIZE bytes\n");
        return 1;
    }
    bytes[0] ^= 1u;
    unsigned changed;
    std::memcpy(&changed, bytes, 4);
    if (changed == GLISTSIZE) return 1;
    std::printf("GLISTSIZE checks passed\n");
    return 0;
}
