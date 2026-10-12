// Host check only; not an EE build input.
#include <cstdio>
#include <cstring>
extern "C" const unsigned char norrath_player_number_tiny_tens_view[40];
int main() {
    const double expected[5] = {1e-16, 9.999999999999999e-33, 1e-64, 9.999999999999999e-129, 1e-256};
    if (sizeof norrath_player_number_tiny_tens_view != 40) return 1;
    for (int i = 0; i < 5; ++i) {
        double value;
        std::memcpy(&value, norrath_player_number_tiny_tens_view + 8 * i, 8);
        if (value != expected[i]) return 2 + i;
    }
    // The 1e-32 and 1e-128 slots differ from the nearest-rounded literal by one ulp.
    double near32; std::memcpy(&near32, norrath_player_number_tiny_tens_view + 8, 8);
    if (near32 == 1e-32) return 7;
    std::puts("number_tiny_tens ok");
    return 0;
}
