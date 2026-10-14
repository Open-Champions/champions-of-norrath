// Host comparison only. Not an EE build input.
#include "nan_parts.cpp"

#include <cstdio>

int main() {
    if (sizeof norrath_player_softdouble_nan_parts_view != 24u) return 1;
    if ((reinterpret_cast<unsigned long long>(norrath_player_softdouble_nan_parts_view) & 7ull) != 0ull) {
        return 1;
    }
    for (unsigned index = 0; index < 24u; ++index) {
        if (norrath_player_softdouble_nan_parts_view[index] != 0) {
            std::printf("nan byte %u is %u\n", index, norrath_player_softdouble_nan_parts_view[index]);
            return 1;
        }
    }
    const unsigned category = static_cast<unsigned>(norrath_player_softdouble_nan_parts_view[0]) |
                              (static_cast<unsigned>(norrath_player_softdouble_nan_parts_view[1]) << 8) |
                              (static_cast<unsigned>(norrath_player_softdouble_nan_parts_view[2]) << 16) |
                              (static_cast<unsigned>(norrath_player_softdouble_nan_parts_view[3]) << 24);
    // Existing add/compare treat category < 2 as the exceptional NaN record.
    if (!(category < 2u)) return 1;
    return 0;
}
