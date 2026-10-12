// Host check only; not an EE build input.
#include <cstdio>
#include <cstring>
extern "C" const unsigned char norrath_player_number_tens_view[200];
int main() {
    const double expected[25] = {1e0, 1e1, 1e2, 1e3, 1e4, 1e5, 1e6, 1e7, 1e8, 1e9, 1e10, 1e11, 1e12,
                                 1e13, 1e14, 1e15, 1e16, 1e17, 1e18, 1e19, 1e20, 1e21, 1e22, 1e23, 1e24};
    if (sizeof norrath_player_number_tens_view != 200) return 1;
    for (int i = 0; i < 25; ++i) {
        double value;
        std::memcpy(&value, norrath_player_number_tens_view + 8 * i, 8);
        if (value != expected[i]) return 2 + i;
    }
    std::puts("number_tens ok");
    return 0;
}
