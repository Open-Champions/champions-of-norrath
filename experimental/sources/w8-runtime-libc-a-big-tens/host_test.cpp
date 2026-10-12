// Host comparison only. Not an EE build input.
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstring>

#include "big_tens.cpp"

int main() {
    static const int exponents[5] = {16, 32, 64, 128, 256};
    if (sizeof(norrath_player_number_big_tens_view) != 40) { std::printf("size wrong\n"); return 1; }
    for (int i = 0; i < 5; ++i) {
        double value;
        std::memcpy(&value, norrath_player_number_big_tens_view + i * 8, 8);
        const double exact = std::pow(10.0, exponents[i]);
        std::uint64_t got_bits, exact_bits;
        std::memcpy(&got_bits, &value, 8);
        std::memcpy(&exact_bits, &exact, 8);
        // Entry 0 is exact; the others sit within one unit of the rounded power.
        const std::uint64_t difference = got_bits > exact_bits ? got_bits - exact_bits : exact_bits - got_bits;
        if (i == 0 ? difference != 0 : difference > 1) {
            std::printf("entry %d is %.17g, off the power by %llu units\n", i, value, (unsigned long long)difference);
            return 2;
        }
    }
    // Entries 1 to 4 are specifically one unit low in the image.
    double second, exact_second;
    std::memcpy(&second, norrath_player_number_big_tens_view + 8, 8);
    exact_second = std::pow(10.0, 32);
    if (!(second < exact_second)) { std::printf("entry 1 must sit below 1e32\n"); return 3; }
    std::printf("big tens cases passed\n");
    return 0;
}
