// Host check only; not an EE build input.
#include <cstdio>
#include <cstring>
extern "C" const unsigned char norrath_player_scan_exponent_format_view[5];
int main() {
    if (sizeof norrath_player_scan_exponent_format_view != 5) return 1;
    if (std::memcmp(norrath_player_scan_exponent_format_view, "e%ld", 5) != 0) return 2;
    char out[16];
    std::snprintf(out, sizeof out, reinterpret_cast<const char*>(norrath_player_scan_exponent_format_view), -12L);
    if (std::strcmp(out, "e-12") != 0) return 3;
    std::puts("scan_exponent_format ok");
    return 0;
}
