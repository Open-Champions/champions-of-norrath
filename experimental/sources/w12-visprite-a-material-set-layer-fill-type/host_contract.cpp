#include "set_layer_fill_type.cpp"
#include <initializer_list>
#include <string.h>

int main() {
    for (int layer = 0; layer < 4; ++layer) {
        for (int type = -1; type < 4; ++type) {
            for (unsigned start : {0u, 0xffffffffu, 0x20u, 0x5au}) {
                unsigned char object[700];
                memset(object, 0x5a, sizeof object);
                memcpy(object + 52 + 144 * layer, &start, 4);
                unsigned char want[700];
                memcpy(want, object, sizeof object);
                unsigned expect = type == 1 ? (start | 0x20u) : (start & ~0x20u);
                memcpy(want + 52 + 144 * layer, &expect, 4);
                int one = 1;
                memcpy(want + 32, &one, 4);
                if (SetLayerFillType__10VIMateriali18VIMaterialFillType(object, layer, type) != 0) return 1;
                if (memcmp(want, object, sizeof object) != 0) return 2;
            }
        }
    }
    return 0;
}
