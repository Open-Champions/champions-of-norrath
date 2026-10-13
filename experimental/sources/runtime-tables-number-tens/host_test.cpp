// Host comparison only. Not an EE build input.
#include "number_tens.cpp"

#include <cmath>
#include <cstdio>
#include <cstring>

static unsigned long long slot_bits(const unsigned char* base, unsigned index) {
    unsigned long long bits;
    std::memcpy(&bits, base + index * 8u, 8);
    return bits;
}

int main() {
    if (sizeof norrath_player_number_tens_view != 184u) return 1;
    if (sizeof norrath_player_number_big_tens_view != 40u) return 2;
    if (sizeof norrath_player_number_tiny_tens_view != 40u) return 3;
    // Exact bit images observed in the executable. The small table matches
    // host powers of ten; the large and tiny tables carry the historical
    // one-ulp-below rounding of several entries, so every entry is pinned.
    const unsigned long long tens_want[] = {
        0x3ff0000000000000ull, 0x4024000000000000ull, 0x4059000000000000ull,
        0x408f400000000000ull, 0x40c3880000000000ull, 0x40f86a0000000000ull,
        0x412e848000000000ull, 0x416312d000000000ull, 0x4197d78400000000ull,
        0x41cdcd6500000000ull, 0x4202a05f20000000ull, 0x42374876e8000000ull,
        0x426d1a94a2000000ull, 0x42a2309ce5400000ull, 0x42d6bcc41e900000ull,
        0x430c6bf526340000ull, 0x4341c37937e08000ull, 0x4376345785d8a000ull,
        0x43abc16d674ec800ull, 0x43e158e460913d00ull, 0x4415af1d78b58c40ull,
        0x444b1ae4d6e2ef50ull, 0x4480f0cf064dd592ull
    };
    const unsigned long long big_want[] = {
        0x4341c37937e08000ull, 0x4693b8b5b5056e16ull, 0x4d384f03e93ff9f4ull,
        0x5a827748f9301d31ull, 0x75154fdd7f73bf3bull
    };
    const unsigned long long tiny_want[] = {
        0x3c9cd2b297d889bcull, 0x3949f623d5a8a732ull, 0x32a50ffd44f4a73dull,
        0x255bba08cf8c979cull, 0x0ac8062864ac6f43ull
    };
    for (unsigned index = 0; index < 23u; ++index) {
        if (slot_bits(norrath_player_number_tens_view, index) != tens_want[index]) {
            std::printf("tens[%u] mismatch\n", index);
            return 4;
        }
    }
    for (unsigned index = 0; index < 5u; ++index) {
        if (slot_bits(norrath_player_number_big_tens_view, index) != big_want[index]) {
            std::printf("bigtens[%u] mismatch\n", index);
            return 5;
        }
        if (slot_bits(norrath_player_number_tiny_tens_view, index) != tiny_want[index]) {
            std::printf("tinytens[%u] mismatch\n", index);
            return 6;
        }
    }
    // Semantic anchors that do not depend on the historical rounding: the
    // small table opens at 1 and its 1e16 slot equals the large table head.
    double one;
    std::memcpy(&one, norrath_player_number_tens_view, 8);
    if (one != 1.0) return 7;
    if (slot_bits(norrath_player_number_tens_view, 16) != big_want[0]) return 8;
    return 0;
}
