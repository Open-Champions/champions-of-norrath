// Host check only; not an EE build input.
#include <cstdio>
#include <cstring>
extern "C" const unsigned char norrath_player_scan_base_normalization_view[34];
int main() {
    if (sizeof norrath_player_scan_base_normalization_view != 34) return 1;
    for (int i = 0; i < 17; ++i) {
        short value;
        std::memcpy(&value, norrath_player_scan_base_normalization_view + 2 * i, 2);
        if (value != (i == 0 ? 10 : i)) return 2 + i;
    }
    std::puts("scan_base_normalization ok");
    return 0;
}
