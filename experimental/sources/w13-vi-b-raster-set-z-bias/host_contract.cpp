#include "set_z_bias.cpp"
#include <string.h>

static void* seen;
static int calls;
static float atCall;
extern "C" int CalcClipToScreen__8VIRaster(void* raster) {
    ++calls; seen = raster;
    atCall = *reinterpret_cast<float*>(static_cast<unsigned char*>(raster) + 17544);
    return 31;
}

int main() {
    static unsigned char object[18000];
    struct Case { int bias; float expect; } cases[] = {{1, 166.0f}, {3, 498.0f}, {100, 16600.0f}, {0, 0.0f}, {-5, 0.0f}};
    for (const Case& c : cases) {
        memset(object, 0x6b, sizeof object);
        static unsigned char want[18000];
        memcpy(want, object, sizeof object);
        memcpy(want + 17544, &c.expect, 4);
        want[17888] = 1; want[17889] = 0; want[17890] = 0; want[17891] = 0;
        calls = 0;
        if (SetZBias__8VIRasteri(object, c.bias) != 31 || calls != 1 || seen != object) return 1;
        if (atCall != c.expect) return 2;
        if (memcmp(object, want, sizeof object) != 0) return 3;
    }
    return 0;
}
