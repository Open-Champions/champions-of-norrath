#include <initializer_list>
extern "C" unsigned playerGetMarkerColor__Fii(int, int);
int main() {
    const unsigned rgb[] = {0x3a2fb6, 0x1ed918, 0x21dbd8, 0xd13d47};
    for (int i = 0; i < 4; ++i)
        for (int a : {0, 1, 0x80, 0xff, 0x1ff, -1})
            if (playerGetMarkerColor__Fii(i, a) != ((static_cast<unsigned>(a) << 24) | rgb[i])) return 1 + i;
    for (int i : {4, 5, -1, 100, 7})
        if (playerGetMarkerColor__Fii(i, 0x40) != (0x40000000u | 0x808080u)) return 10;
    return 0;
}
