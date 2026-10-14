#include "set_layer_uv_set.cpp"
#include <string.h>

int main() {
    for (int layer = 0; layer < 3; ++layer) {
        for (int set = 0; set < 2; ++set) {
            unsigned char m[600];
            memset(m, 0x5a, sizeof m);
            unsigned start = 0x12345678;
            memcpy(m + 52 + 144 * layer, &start, 4);
            unsigned char want[600];
            memcpy(want, m, sizeof m);
            unsigned expect = set ? 0xffffffefu : (start & ~0x10u);
            memcpy(want + 52 + 144 * layer, &expect, 4);
            unsigned one = 1;
            memcpy(want + 32, &one, 4);
            if (SetLayerUVSet__10VIMaterialii(m, layer, set) != 0) return 1;
            if (memcmp(want, m, sizeof m) != 0) return 2;
        }
    }
    return 0;
}
