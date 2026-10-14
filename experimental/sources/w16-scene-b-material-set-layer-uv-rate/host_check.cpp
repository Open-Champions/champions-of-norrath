#include "set_layer_uv_rate.cpp"
#include <string.h>

int main() {
    const float nan = __builtin_nanf("");
    const float vals[][2] = {{0, 0}, {0, 1}, {2, 0}, {-1, -1}, {nan, 0}, {0, nan}};
    const bool flag[] = {false, true, true, true, true, true};
    for (int i = 0; i < 6; ++i) {
        for (int layer = 0; layer < 3; ++layer) {
            unsigned char m[600];
            memset(m, 0x11, sizeof m);
            unsigned start = 0x100;
            memcpy(m + 12, &start, 4);
            unsigned char want[600];
            memcpy(want, m, sizeof m);
            memcpy(want + 104 + 144 * layer, vals[i], 8);
            unsigned f = flag[i] ? 0x104u : 0x100u;
            memcpy(want + 12, &f, 4);
            unsigned one = 1;
            memcpy(want + 32, &one, 4);
            if (SetLayerUVRate__10VIMaterialiRC7VIVect2(m, layer, vals[i]) != 0) return 1;
            if (memcmp(want, m, sizeof m) != 0) return 2;
        }
    }
    return 0;
}
