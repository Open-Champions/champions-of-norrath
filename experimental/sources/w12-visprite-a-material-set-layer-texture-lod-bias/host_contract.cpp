#include "set_layer_texture_lod_bias.cpp"
#include <initializer_list>
#include <string.h>

int main() {
    for (int layer = 0; layer < 4; ++layer) {
        unsigned char object[700];
        memset(object, 0x5a, sizeof object);
        unsigned char want[700];
        memcpy(want, object, sizeof object);
        float bias = -1.25f + layer;
        int one = 1;
        memcpy(want + 32, &one, 4);
        memcpy(want + 100 + 144 * layer, &bias, 4);
        if (SetLayerTextureLODBias__10VIMaterialif(object, layer, bias) != 0) return 1;
        if (memcmp(want, object, sizeof object) != 0) return 2;
    }
    return 0;
}
