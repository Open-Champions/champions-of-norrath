#include <string.h>
static const void* g_dst;
static const void* g_src;
extern "C" void* __as__10VIMatrix33RC10VIMatrix33(void* self, const void* other) {
    g_dst = self;
    g_src = other;
    return self;
}
#include "set_layer_uv_transform.cpp"

int main() {
    for (int layer = 0; layer < 3; ++layer) {
        unsigned char m[600];
        memset(m, 0x22, sizeof m);
        float src[9] = {};
        g_dst = g_src = 0;
        if (SetLayerUVTransform__10VIMaterialiRC10VIMatrix33(m, layer, src) != 0) return 1;
        if (g_dst != m + 64 + 144 * layer || g_src != src) return 2;
        for (unsigned i = 0; i < sizeof m; ++i) {
            unsigned char want = (i == 32) ? 1 : (i > 32 && i < 36) ? 0 : 0x22;
            if (m[i] != want) return 3;
        }
    }
    return 0;
}
