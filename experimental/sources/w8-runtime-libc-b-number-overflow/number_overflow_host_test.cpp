// Host check only; not an EE build input.
#include <cstdio>
#include <cstring>
#include <limits>
extern "C" const unsigned char norrath_player_number_overflow_view[8];
int main() {
    double value;
    std::memcpy(&value, norrath_player_number_overflow_view, 8);
    if (value != std::numeric_limits<double>::infinity()) return 1;
    if (sizeof norrath_player_number_overflow_view != 8) return 2;
    std::puts("number_overflow ok");
    return 0;
}
