// Host check for writeMap__3mapPUc. Not an EE plan input.
#include "write_map.cpp"
#include <cstring>
unsigned char _3map$maptiles[93312];
int main() {
    for (int i = 0; i < 93312; ++i) _3map$maptiles[i] = (unsigned char)(i * 37 + (i >> 8) * 11 + 3);
    unsigned char before[93312];
    std::memcpy(before, _3map$maptiles, sizeof before);
    unsigned char out[5832 + 16];
    std::memset(out, 0xcc, sizeof out);
    writeMap__3mapPUc(out);
    if (std::memcmp(before, _3map$maptiles, sizeof before) != 0) return 1;
    for (int i = 5832; i < 5832 + 16; ++i) if (out[i] != 0xcc) return 2;
    // each output byte, derived from the layout rather than the loop shape
    for (int row = 0; row < 108; ++row) {
        for (int col = 0; col < 54; ++col) {
            const int left = 432 * (2 * row) + 2 * (4 * col) + 1;
            const int right = 432 * (2 * row) + 2 * (4 * col + 2) + 1;
            const unsigned char want = (unsigned char)((before[left] & 0xf0) | (before[right] >> 4));
            if (out[row * 54 + col] != want) return 3;
        }
    }
    // low nibble of the left tile and high nibble of the right tile must not leak the wrong way
    std::memset(_3map$maptiles, 0, sizeof _3map$maptiles);
    _3map$maptiles[1] = 0xaf;
    _3map$maptiles[5] = 0x5f;
    _3map$maptiles[432 * 2 + 1] = 0x0f;
    writeMap__3mapPUc(out);
    if (out[0] != 0xa5 || out[54] != 0x00) return 4;
    return 0;
}
