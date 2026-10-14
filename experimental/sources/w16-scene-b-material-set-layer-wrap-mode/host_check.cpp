#include "set_layer_wrap_mode.cpp"
#include <initializer_list>
#include <string.h>

int main() {
    for (int layer = 0; layer < 3; ++layer) {
        for (int mode = -1; mode < 5; ++mode) {
            for (unsigned start : {0u, 0xffffffffu, 0x2u, 0x4u, 0x10u}) {
                unsigned char m[600];
                memset(m, 0x44, sizeof m);
                memcpy(m + 52 + 144 * layer, &start, 4);
                unsigned char want[600];
                memcpy(want, m, sizeof m);
                bool valid = mode >= 0 && mode <= 3;
                unsigned expect = start;
                if (mode == 0) expect = start & ~6u;
                if (mode == 1) expect = (start & ~6u) | 2u;
                if (mode == 2) expect = (start & ~6u) | 4u;
                if (mode == 3) expect = start | 6u;
                memcpy(want + 52 + 144 * layer, &expect, 4);
                if (valid) {
                    unsigned one = 1;
                    memcpy(want + 32, &one, 4);
                }
                int r = SetLayerWrapMode__10VIMateriali18VIMaterialWrapMode(m, layer, mode);
                if (r != (valid ? 0 : -1)) return 1;
                if (memcmp(want, m, sizeof m) != 0) return 2;
            }
        }
    }
    return 0;
}
