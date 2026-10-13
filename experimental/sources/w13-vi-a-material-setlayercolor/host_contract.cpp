#include "set_layer_color.cpp"
#include <string.h>

int main() {
    alignas(4) unsigned char m[144 * 4 + 64];
    memset(m, 0x55, sizeof m);
    unsigned char c[7] = {0, 1, 2, 3, 4, 5, 6};
    if (SetLayerColor__10VIMaterialiRC9VIColor32(m, 2, (const VIColor32Bytes*)(c + 1)) != 0) return 1;
    for (int i = 0; i < 4; ++i) if (m[288 + 56 + i] != c[1 + i]) return 2;
    if (*(int*)(m + 32) != 1) return 3;
    for (unsigned i = 0; i < sizeof m; ++i) {
        bool w = (i >= 344 && i < 348) || (i >= 32 && i < 36);
        if (!w && m[i] != 0x55) return 4;
    }
    return 0;
}
