#include <string.h>
static float g_y[4];
static int g_calls;
extern "C" float powf(float x, float y) {
    if (x != 2.0f || g_calls >= 3) g_calls = 99;
    g_y[g_calls++] = y;
    return y == 3.0f ? 8.0f : y == 0.0f ? 1.0f : 1024.0f;
}
#include "set_packing.cpp"

int main() {
    unsigned char o[80];
    memset(o, 0x44, sizeof o);
    unsigned char want[80];
    memcpy(want, o, sizeof o);
    float f[3] = {0.125f, 1.0f, 1.0f / 1024.0f};
    memcpy(want + 48, f, 12);
    g_calls = 0;
    if (SetPacking__12VIPrimBufferiii(o, 3, 0, 10) != 0) return 1;
    if (g_calls != 3 || g_y[0] != 3.0f || g_y[1] != 0.0f || g_y[2] != 10.0f) return 2;
    if (memcmp(o, want, sizeof o) != 0) return 3;
    return 0;
}
