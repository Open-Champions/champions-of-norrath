#include <string.h>
static float g_x, g_y;
static int g_calls;
extern "C" float powf(float x, float y) {
    g_x = x; g_y = y; ++g_calls;
    return y == 3.0f ? 8.0f : y == 0.0f ? 1.0f : 1024.0f;
}
#include "set_packing.cpp"

int main() {
    struct { int bits; float want; } t[] = {{3, 0.125f}, {0, 1.0f}, {10, 1.0f / 1024.0f}};
    for (auto& c : t) {
        unsigned char o[40];
        memset(o, 0x44, sizeof o);
        unsigned char want[40];
        memcpy(want, o, sizeof o);
        memcpy(want + 16, &c.want, 4);
        g_calls = 0;
        if (SetPacking__12VICollBufferi(o, c.bits) != 0) return 1;
        if (g_calls != 1 || g_x != 2.0f || g_y != static_cast<float>(c.bits)) return 2;
        if (memcmp(o, want, sizeof o) != 0) return 3;
    }
    return 0;
}
