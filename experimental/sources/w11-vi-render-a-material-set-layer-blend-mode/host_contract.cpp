#include "set_layer_blend_mode.cpp"
#include <string.h>

int main() {
    for (int layer = 0; layer < 4; ++layer) {
        unsigned char object[700];
        memset(object, 0x5a, sizeof object);
        unsigned char before[700];
        memcpy(before, object, sizeof object);
        int result = SetLayerBlendMode__10VIMateriali16VIAlphaBlendMode(object, layer, 0x1234 + layer);
        if (result != 0) return 1;
        int flag, mode;
        memcpy(&flag, object + 32, 4);
        memcpy(&mode, object + 60 + 144 * layer, 4);
        if (flag != 1 || mode != 0x1234 + layer) return 2;
        memcpy(before + 32, &flag, 4);
        memcpy(before + 60 + 144 * layer, &mode, 4);
        if (memcmp(before, object, sizeof object) != 0) return 3;
    }
    return 0;
}
