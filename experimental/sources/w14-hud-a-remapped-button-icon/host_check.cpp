#include <cstdio>
#include <cstring>
extern "C" unsigned char MC_GameConfig[252];
unsigned char MC_GameConfig[252];
#include "remapped_button_icon.cpp"

int main() {
    int bad = 0;
    const int masks[12] = {32, 16, 128, 64, 32768, 8192, 4096, 16384, 4, 1, 8, 2};
    for (int p = 0; p < 2; ++p)
        for (int s = 0; s < 9; ++s)
            for (int i = 0; i < 12; ++i) {
                std::memset(MC_GameConfig, 0, sizeof MC_GameConfig);
                std::memcpy(MC_GameConfig + 52 + (p * 9 + s) * 4, &masks[i], 4);
                if (hudGetRemappedButtonIcon__Fii(p, s) != 123 + i) bad++;
                // the same word one slot over must not be seen
                if (s < 8 && hudGetRemappedButtonIcon__Fii(p, s + 1) != 0) bad++;
            }
    std::memset(MC_GameConfig, 0, sizeof MC_GameConfig);
    int none = 3;
    std::memcpy(MC_GameConfig + 52, &none, 4);
    if (hudGetRemappedButtonIcon__Fii(0, 0) != 0) bad++;
    if (!bad) std::puts("hudGetRemappedButtonIcon passed");
    return bad;
}
