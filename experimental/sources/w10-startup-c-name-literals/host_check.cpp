#include <cstdio>
#include <cstring>
#include "name_literals.cpp"
int main() {
    if (std::strcmp(norrath_w9_startup_crt_b_view_462260, "game_swish_chest%d.anm") != 0) return 1;
    if (std::strcmp(norrath_w9_startup_crt_b_view_4708d0, "spel_ew_shockfire.anm") != 0) return 2;
    if (std::strcmp(norrath_w9_startup_crt_b_view_11c3ae8, "VIStation::VIStation\n") != 0) return 3;
    char out[64];
    std::snprintf(out, sizeof out, norrath_w9_startup_crt_b_view_462260, 2);
    if (std::strcmp(out, "game_swish_chest2.anm") != 0) return 4;
    std::puts("name literals check passed");
    return 0;
}
