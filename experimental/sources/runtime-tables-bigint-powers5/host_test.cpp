// Host comparison only. Not an EE build input.
#include "bigint_powers5.cpp"

#include <cstdio>
#include <cstring>

static unsigned word_at(const unsigned char* base, unsigned index) {
    return static_cast<unsigned>(base[index * 4u]) |
           (static_cast<unsigned>(base[index * 4u + 1u]) << 8) |
           (static_cast<unsigned>(base[index * 4u + 2u]) << 16) |
           (static_cast<unsigned>(base[index * 4u + 3u]) << 24);
}

int main() {
    if (sizeof norrath_player_bigint_small_powers5_view != 12u) return 1;
    // Three native words only: 5^1, 5^2, 5^3. The growth path reads index
    // remainder - 1 and continues from 625 by squaring.
    if (word_at(norrath_player_bigint_small_powers5_view, 0) != 5u) return 2;
    if (word_at(norrath_player_bigint_small_powers5_view, 1) != 25u) return 3;
    if (word_at(norrath_player_bigint_small_powers5_view, 2) != 125u) return 4;
    if (word_at(norrath_player_bigint_small_powers5_view, 2) != 5u * 5u * 5u) return 5;
    return 0;
}
