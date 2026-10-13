#include "init_time_of_day.cpp"

static int sin_calls, cos_calls;
static float last_angle;
extern "C" float sinf(float a) { ++sin_calls; last_angle = a; return 0.25f; }
extern "C" float cosf(float a) { ++cos_calls; return 0.5f; }

alignas(16) static char scene[12000];
static float at(int offset) { return *reinterpret_cast<float*>(scene + offset); }
static bool near_eq(float a, float b) { float d = a - b; return d < 0.0001f && d > -0.0001f; }

int main() {
    for (unsigned i = 0; i < sizeof scene; ++i) scene[i] = static_cast<char>(0x5a);
    if (InitTimeOfDay__7VIScene(reinterpret_cast<VIScene*>(scene)) != 0) return 1;
    if (sin_calls != 24 || cos_calls != 24) return 2;
    if (at(8160) != 10.0f) return 3;
    // hour 0: first table 75 75 100, second table 70 0 190, copied second table, grey, zero and fog slots
    if (!near_eq(at(8180), 75.0f / 255.0f) || !near_eq(at(8188), 100.0f / 255.0f) || at(8192) != 0.0f) return 4;
    if (!near_eq(at(8224), 70.0f / 255.0f) || at(8228) != 0.0f || !near_eq(at(8232), 190.0f / 255.0f)) return 5;
    for (int i = 0; i < 4; ++i) if (at(8256 + 4 * i) != at(8224 + 4 * i)) return 6;
    if (!near_eq(at(8196), 75.0f / 255.0f * 0.9f) || at(8200) != at(8196) || at(8204) != at(8196) || at(8208) != 0.0f) return 7;
    for (int i = 0; i < 4; ++i) if (at(8240 + 4 * i) != 0.0f) return 8;
    // hour 0 is 12 hours from noon: v = max((1 - 1) * 0.7, 0.2); hour 12 is 0.7
    if (!near_eq(at(8272), 0.2f) || !near_eq(at(8276), 0.2f) || !near_eq(at(8280), 0.2f) || at(8284) != 0.0f) return 9;
    if (!near_eq(at(8272 + 108 * 12), 0.7f)) return 10;
    if (!near_eq(at(8272 + 108 * 6), 0.35f)) return 11;
    // direction slot: (-sin, cos, 0)
    if (at(8212) != -0.25f || at(8216) != 0.5f || at(8220) != 0.0f) return 12;
    if (at(8212 + 108 * 23) != -0.25f) return 13;
    // noon first table entry is 102, second 255 250 240
    if (!near_eq(at(8180 + 108 * 12), 102.0f / 255.0f) || !near_eq(at(8224 + 108 * 12 + 8), 240.0f / 255.0f)) return 14;
    // bytes between records that the function does not own stay untouched (8164..8179)
    if (scene[8179] != 0x5a || scene[8164] != 0x5a) return 15;
    return 0;
}
