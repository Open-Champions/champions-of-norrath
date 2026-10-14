#include "reset_lightning.cpp"
#include <string.h>

static int rolls[4], nroll, recompute; static void* rarg;
extern "C" int rand(void) { return rolls[nroll++]; }
extern "C" void RecomputeLightningPatches__12VIAtmosphere(void* a) { ++recompute; rarg = a; }
static float fb(unsigned b) { float f; memcpy(&f, &b, 4); return f; }

int main() {
    alignas(4) static unsigned char a[4700];
    const float scale = fb(0x3b808080), lo = fb(0x3dcccccc), hi = fb(0x3f4ccccc), k = fb(0x3f199999);
    int cases[][2] = {{0, 0}, {1, 255}, {2, 7}, {1000003, 31}};
    for (auto& c : cases) {
        memset(a, 0x5a, sizeof a); nroll = 0; recompute = 0; rolls[0] = c[0]; rolls[1] = c[1];
        ResetLightning__12VIAtmosphere(a);
        if (nroll != 2 || recompute != 1 || rarg != a) return 1;
        float u = (float)((131 * c[0]) & 0xff) * scale;
        float level = u > lo ? u : lo; level = level < hi ? level : hi;
        float v = (float)((131 * c[1]) & 0xff) * scale;
        if (*(float*)(a + 4504) != 8.0f || *(float*)(a + 4520) != level || *(float*)(a + 4508) != level * 6.0f) return 2;
        if (*(float*)(a + 4516) != v * k + k) return 3;
        for (int i = 0; i < 4700; ++i) if (!((i >= 4504 && i < 4508) || (i >= 4508 && i < 4512) || (i >= 4516 && i < 4524)) && a[i] != 0x5a) return 4;
    }
    for (int r = 0; r < 256; ++r) {                        // every byte value, both clamps included
        memset(a, 0, sizeof a); nroll = 0; rolls[0] = r; rolls[1] = 255 - r;
        ResetLightning__12VIAtmosphere(a);
        float u = (float)((131 * r) & 0xff) * scale;
        float level = u > lo ? u : lo; level = level < hi ? level : hi;
        float v = (float)((131 * (255 - r)) & 0xff) * scale;
        if (*(float*)(a + 4520) != level || *(float*)(a + 4516) != v * k + k) return 8;
    }
    // concrete values: r = 1 -> byte 131 -> 131/255 = 0.51372546; clamp not hit
    memset(a, 0, sizeof a); nroll = 0; rolls[0] = 1; rolls[1] = 0;
    ResetLightning__12VIAtmosphere(a);
    float got = *(float*)(a + 4520);
    if (got < 0.5137f || got > 0.5138f) return 5;
    if (*(float*)(a + 4516) != k) return 6;               // v = 0
    memset(a, 0, sizeof a); nroll = 0; rolls[0] = 0; rolls[1] = 0;
    ResetLightning__12VIAtmosphere(a);
    if (*(float*)(a + 4520) != lo) return 7;               // lower clamp
    return 0;
}
