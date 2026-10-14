// Host check only; not an EE build input.
#include <cstdio>
#include <cstring>
extern "C" const unsigned char norrath_player_number_quarter_lower_view[8];
int main() {
    double value;
    std::memcpy(&value, norrath_player_number_quarter_lower_view, 8);
    if (value != 0.24999995) return 1;
    if (sizeof norrath_player_number_quarter_lower_view != 8) return 2;
    std::puts("quarter-lower ok");
    return 0;
}
