// Host comparison only. Not an EE build input.
#include <cstdio>

#include "powers5.cpp"

int main() {
    unsigned expected = 1u;
    for (int i = 0; i < 3; ++i) {
        expected *= 5u;
        const unsigned char* p = norrath_player_bigint_small_powers5_view + i * 4;
        const unsigned word = unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
        if (word != expected) { std::printf("word %d is %u, expected %u\n", i, word, expected); return 1; }
    }
    if (sizeof(norrath_player_bigint_small_powers5_view) != 12) { std::printf("size wrong\n"); return 2; }
    std::printf("powers of five cases passed\n");
    return 0;
}
