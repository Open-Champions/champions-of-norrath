// Host contract for tobcd. Not a build-plan input.
//   c++ -std=c++17 -o check host_contract.cpp && ./check
// For 0..99 the result must have the decimal digits in its nibbles; for all 256 inputs it must equal
// byte + 6 * (byte / 10) truncated to a byte.
#include <cstdio>
#include "tobcd.cpp"
int main() {
    int bad = 0;
    for (int v = 0; v < 256; ++v) {
        unsigned char r = tobcd(static_cast<unsigned char>(v));
        if (r != static_cast<unsigned char>(v + 6 * (v / 10))) { std::printf("closed form %d\n", v); ++bad; }
        if (v < 100 && r != (((v / 10) << 4) | (v % 10))) { std::printf("digits %d\n", v); ++bad; }
    }
    if (tobcd(59) != 0x59 || tobcd(0) != 0 || tobcd(99) != 0x99) { std::puts("spot values"); ++bad; }
    return bad;
}
