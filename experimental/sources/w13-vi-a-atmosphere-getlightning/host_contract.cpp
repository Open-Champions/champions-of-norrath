#include "get_lightning.cpp"
#include <string.h>

static int rolls[16], nroll, recompute;
extern "C" int rand(void) { return rolls[nroll++]; }
extern "C" void RecomputeLightningPatches__12VIAtmosphere(void*) { ++recompute; }
static float fb(unsigned b) { float f; memcpy(&f, &b, 4); return f; }
#define F(off) (*(float*)(a + (off)))

static unsigned char a[4700];
static void setup(int m1, int m2, float t, float step, float dur, float level) {
    memset(a, 0x5a, sizeof a); nroll = 0; recompute = 0;
    *(int*)(a + 1516) = m1; *(int*)(a + 1520) = m2; F(4512) = t; F(1192) = step; F(4516) = dur; F(4520) = level;
}
static bool near(float x, float y) { float d = x - y; return d < 0 ? -d < 1e-6f : d < 1e-6f; }   // the host may fuse multiply-adds
static float clamp01(float v) { v = v > 0.0f ? v : 0.0f; return v < 1.0f ? v : 1.0f; }

int main() {
    const float A = fb(0x3e4ccccc), B = fb(0x3e999999), C = fb(0x3f4ccccc), S = fb(0x3b808080), R = fb(0x30000000);
    float out = -7.0f, ex;
    // flags: either int not 1 resets t and returns 0 without touching the output slots
    setup(2, 1, 0.3f, 0.1f, 1.0f, 0.5f);
    if (GetLightning__12VIAtmospherePf(a, &out) != 0 || F(4512) != 0.0f || out != -7.0f || nroll != 0) return 1;
    setup(1, 0, 0.3f, 0.1f, 1.0f, 0.5f);
    if (GetLightning__12VIAtmospherePf(a, &out) != 0 || F(4512) != 0.0f) return 2;
    // t == 0: output copy cleared, time untouched
    setup(1, 1, 0.0f, 0.1f, 1.0f, 0.5f); F(3912) = 3.0f;
    if (GetLightning__12VIAtmospherePf(a, &out) != 0 || F(3912) != 0.0f || F(4512) != 0.0f || out != -7.0f) return 3;
    // ramp (now < A) without noise
    setup(1, 1, 0.1f, 0.05f, 1.0f, 0.25f); rolls[0] = rolls[1] = 77;
    if (GetLightning__12VIAtmospherePf(a, &out) != 1 || nroll != 2 || recompute != 0) return 4;
    float now = 0.1f + 0.05f;
    ex = clamp01(now * 5.0f + 0.0f) * (1.0f - 0.25f) * C;
    if (out != ex || F(3912) != ex || F(4512) != now) return 5;
    // ramp with noise (first - second) * R * A: +A when r1 - r2 = 2^31 as a float, clamped at the top
    setup(1, 1, 0.1f, 0.05f, 1.0f, 0.0f); rolls[0] = 0x7fffffff; rolls[1] = 0;
    GetLightning__12VIAtmospherePf(a, &out);
    ex = clamp01(now * 5.0f + (fb(0x4f000000) - 0.0f) * R * A) * 1.0f * C;
    if (!near(out, ex)) return 6;
    setup(1, 1, 0.1f, 0.05f, 1.0f, 0.0f); rolls[0] = 0; rolls[1] = 0x7fffffff;      // negative noise
    GetLightning__12VIAtmospherePf(a, &out);
    ex = clamp01(now * 5.0f + (0.0f - fb(0x4f000000)) * R * A) * 1.0f * C;
    if (!near(out, ex)) return 7;
    // decay (now >= A): t = 0.5, no step: value = (now - A) / (d - A) + noise; third roll & 15 >= 2 keeps it
    setup(1, 1, 0.5f, 0.0f, 1.0f, 0.5f); rolls[0] = rolls[1] = 9; rolls[2] = 0x35;
    GetLightning__12VIAtmospherePf(a, &out);
    ex = clamp01((0.5f - A) / (1.0f - A)) * (1.0f - 0.5f) * C;
    if (nroll != 3 || recompute != 0 || !near(out, ex)) return 8;
    // flicker: third roll & 15 < 2 replaces the value from a fourth roll
    setup(1, 1, 0.5f, 0.0f, 1.0f, 0.0f); rolls[0] = rolls[1] = 9; rolls[2] = 0x31; rolls[3] = 1;
    GetLightning__12VIAtmospherePf(a, &out);
    ex = clamp01((float)((131 * 1) & 0xff) * S * A + C) * 1.0f * C;
    if (nroll != 4 || !near(out, ex)) return 9;
    // noise in the decay branch uses B
    setup(1, 1, 0.5f, 0.0f, 1.0f, 0.0f); rolls[0] = 0x7fffffff; rolls[1] = 0; rolls[2] = 0x35;
    GetLightning__12VIAtmospherePf(a, &out);
    ex = clamp01((0.5f - A) / (1.0f - A) + (fb(0x4f000000) - 0.0f) * R * B) * 1.0f * C;
    if (!near(out, ex)) return 10;
    // crossing the midpoint: t 0.4 -> 0.6 of d = 1.0; the extra roll decides the patch recompute
    setup(1, 1, 0.4f, 0.2f, 1.0f, 0.0f); rolls[0] = rolls[1] = 9; rolls[2] = 0x35; rolls[3] = 5;
    GetLightning__12VIAtmospherePf(a, &out);
    if (nroll != 4 || recompute != 1) return 11;
    setup(1, 1, 0.4f, 0.2f, 1.0f, 0.0f); rolls[0] = rolls[1] = 9; rolls[2] = 0x35; rolls[3] = 6;
    GetLightning__12VIAtmospherePf(a, &out);
    if (nroll != 4 || recompute != 0) return 12;
    // not crossing: t 0.6 -> 0.7 consumes no extra roll; t 0.3 -> 0.4 neither
    setup(1, 1, 0.6f, 0.1f, 1.0f, 0.0f); rolls[0] = rolls[1] = 9; rolls[2] = 0x35;
    GetLightning__12VIAtmospherePf(a, &out);
    if (nroll != 3 || recompute != 0) return 13;
    setup(1, 1, 0.3f, 0.1f, 1.0f, 0.0f); rolls[0] = rolls[1] = 9; rolls[2] = 0x35;
    GetLightning__12VIAtmospherePf(a, &out);
    if (nroll != 3 || recompute != 0) return 14;
    // the end of the flash resets t to zero (stored value reaches d)
    setup(1, 1, 0.9f, 0.2f, 1.0f, 0.0f); rolls[0] = rolls[1] = 9; rolls[2] = 0x35;
    GetLightning__12VIAtmospherePf(a, &out);
    if (F(4512) != 0.0f || nroll != 3) return 15;
    setup(1, 1, 0.5f, 0.2f, 1.0f, 0.0f); rolls[0] = rolls[1] = 9; rolls[2] = 0x35; rolls[3] = 9;
    GetLightning__12VIAtmospherePf(a, &out);
    if (F(4512) != 0.5f + 0.2f) return 16;
    return 0;
}
