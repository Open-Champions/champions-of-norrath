// Host comparison only. Not an EE build input.
#include "scan_views.cpp"

#include <cstdio>
#include <cstring>

static int read_half(const unsigned char* base, unsigned index) {
    const int low = base[index * 2u] | (base[index * 2u + 1u] << 8);
    return (low & 0x8000) != 0 ? low - 0x10000 : low;
}

int main() {
    if (sizeof norrath_player_scan_base_normalization_view != 34u) return 1;
    if (sizeof norrath_player_scan_exponent_format_view != 5u) return 2;
    // Entry 0 normalizes the undetermined %i base to decimal; every other
    // entry is the identity, preserving octal and hexadecimal scanning.
    if (read_half(norrath_player_scan_base_normalization_view, 0) != 10) return 3;
    for (unsigned base = 1; base < 17u; ++base) {
        if (read_half(norrath_player_scan_base_normalization_view, base) != static_cast<int>(base)) {
            std::printf("base %u not identity\n", base);
            return 4;
        }
    }
    // The exponent rewrite format is the NUL-terminated string "e%ld".
    if (std::memcmp(norrath_player_scan_exponent_format_view, "e%ld", 5) != 0) return 5;
    if (norrath_player_scan_exponent_format_view[4] != 0) return 6;
    return 0;
}
