// Host comparison only. Not an EE build input.
#include "number_refine.cpp"

#include <cstdio>
#include <cstring>

static double slot(const unsigned char* base) {
    double value;
    std::memcpy(&value, base, 8);
    return value;
}

int main() {
    if (sizeof norrath_player_number_half_lower_view != 8u) return 1;
    if (sizeof norrath_player_number_half_upper_view != 8u) return 2;
    if (sizeof norrath_player_number_quarter_lower_view != 8u) return 3;
    // Exact observed values: just below and above one half, and just below
    // one quarter. A zeroed or swapped table cannot pass these.
    if (slot(norrath_player_number_half_lower_view) != 0.4999999) return 4;
    if (slot(norrath_player_number_half_upper_view) != 0.5000001) return 5;
    if (slot(norrath_player_number_quarter_lower_view) != 0.24999995) return 6;
    // The bounds must straddle one half in the order the loop tests them:
    // lower < 0.5 < upper, and the quarter bound below the half bound.
    const double lower = slot(norrath_player_number_half_lower_view);
    const double upper = slot(norrath_player_number_half_upper_view);
    const double quarter = slot(norrath_player_number_quarter_lower_view);
    if (!(lower < 0.5 && 0.5 < upper)) return 7;
    if (!(quarter < lower && quarter < 0.25)) return 8;
    // The overflow word is positive infinity, the ERANGE result.
    if (sizeof norrath_player_number_overflow_view != 8u) return 9;
    double overflow;
    std::memcpy(&overflow, norrath_player_number_overflow_view, 8);
    if (!(overflow > 1.0e308 && overflow * 2.0 == overflow)) return 10;
    return 0;
}
