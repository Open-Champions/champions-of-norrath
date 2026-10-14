#include "init_name_pad.cpp"

#include <cstdio>
#include <cstring>

unsigned char fontGrid[128];
int nameGridCount = 99;
int nameGridW = 99;

int main() {
    std::memset(fontGrid, 0x77, sizeof fontGrid);
    initNamePad__Fv();
    if (nameGridCount != 64 || nameGridW != 10) return 1;
    for (int i = 0; i < 26; ++i) if (fontGrid[i] != 'a' + i) return 2;
    for (int i = 0; i < 10; ++i) if (fontGrid[26 + i] != '0' + i) return 3;
    static const unsigned char extra[28] = {0xc1,0xc4,0xc8,0xc9,0xd3,0xd6,0xdc,0xdf,0xe0,0xe1,0xe2,0xe4,0xe7,0xe8,
                                            0xe9,0xea,0xec,0xed,0xef,0xf1,0xf2,0xf3,0xf4,0xf6,0xf9,0xfa,0xfb,0xfc};
    if (std::memcmp(fontGrid + 36, extra, 28) != 0) return 4;
    if (fontGrid[64] != 0x77 || fontGrid[127] != 0x77) return 5;
    std::puts("init name pad passed");
    return 0;
}
