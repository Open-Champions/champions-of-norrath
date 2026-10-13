// Host contract for frombcd. Not a build-plan input.
//   c++ -std=c++17 -o check host_contract.cpp && ./check
// Checks every byte value against an independent decimal-digit decomposition for valid packed decimal,
// and against the closed form byte - 6 * (byte >> 4) for all 256 inputs.
#include <cstdio>
#include "frombcd.cpp"
int main() {
    int bad = 0;
    for (int v = 0; v < 256; ++v) {
        unsigned char want = static_cast<unsigned char>(v - 6 * (v >> 4));
        if (frombcd(static_cast<unsigned char>(v)) != want) { std::printf("closed form %d\n", v); ++bad; }
        if ((v & 15) < 10 && (v >> 4) < 10 && frombcd(static_cast<unsigned char>(v)) != (v >> 4) * 10 + (v & 15)) {
            std::printf("digits %02x\n", v); ++bad;
        }
    }
    if (frombcd(0x99) != 99 || frombcd(0x00) != 0 || frombcd(0x1f) != 25) { std::puts("spot values"); ++bad; }
    return bad;
}
