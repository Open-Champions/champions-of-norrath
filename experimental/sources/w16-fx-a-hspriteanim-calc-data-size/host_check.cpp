// Host check only; not a build input.
#include "calc_data_size.cpp"
#include <cstring>
int main() {
    alignas(8) unsigned char o[64];
    std::memset(o, 0xee, sizeof o);
    int* w = reinterpret_cast<int*>(o);
    w[2] = 0; w[3] = 3; w[4] = 4; w[5] = 5;
    if (CalcDataSize__13VIHSpriteAnim(o) != 0) return 1;
    if (w[9] != 0 || w[10] != 24 || w[11] != 24 + 3 * 4 * 32 || w[12] != 24 + 3 * 4 * 32 + 40) return 2;
    w[2] = 1;
    if (CalcDataSize__13VIHSpriteAnim(o) != 0 || w[11] != 24 + 3 * 4 * 16 || w[12] != w[11] + 40) return 3;
    w[2] = 2; w[10] = 1234;
    if (CalcDataSize__13VIHSpriteAnim(o) != -1 || w[10] != 1234) return 4;
    return 0;
}
