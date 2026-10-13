// Host comparison only. Not an EE build input.
#include "services_word.cpp"

#include <cstdio>
#include <cstring>

int main() {
    if (sizeof norrath_player_services_view_48068c != 4u) return 1;
    float value;
    std::memcpy(&value, norrath_player_services_view_48068c, 4);
    // The observed coefficient is one ulp below the nearest float to 0.4,
    // the historical rounding of this table; pin the exact bits.
    unsigned bits;
    std::memcpy(&bits, &value, 4);
    if (bits != 0x3eccccccu) return 2;
    double close = value > 0.3999999 && value < 0.4000001 ? 1.0 : 0.0;
    if (close != 1.0) return 3;
    return 0;
}
