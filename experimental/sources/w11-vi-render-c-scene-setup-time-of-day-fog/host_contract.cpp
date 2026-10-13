#include "setup_time_of_day_fog.cpp"

static float overcast_value, near_out, far_out;
static int fog_calls;
static unsigned fog_raster;
static int fog_a;
static float fog_near, fog_far, fog_colour[4];

extern "C" float GetOvercastInterp__12VIAtmosphere(void*) { return overcast_value; }
extern "C" int CalcTimeOfDayClipPlanes__7VISceneR21VITerrainProfileBlendRfT2(VIScene*, VITerrainProfileBlend*, float* n, float* f) {
    *n = near_out; *f = far_out; return 0;
}
extern "C" void SetFog__8VIRasteriffRC10VIColor32F(unsigned raster, int a, float n, float f, const float* c) {
    ++fog_calls; fog_raster = raster; fog_a = a; fog_near = n; fog_far = f;
    for (int i = 0; i < 4; ++i) fog_colour[i] = c[i];
}

alignas(16) static char scene[20000];
static VITerrainProfileBlend blend;

static void put(int offset, float v) { *reinterpret_cast<float*>(scene + offset) = v; }
static bool near_eq(float a, float b) { float d = a - b; return d < 0.0001f && d > -0.0001f; }

int main() {
    VIScene* s = reinterpret_cast<VIScene*>(scene);
    put(8160, 2.5f);
    put(10848, 50.0f);
    *reinterpret_cast<unsigned*>(scene + 19356) = 0x1234;
    for (int i = 0; i < 4; ++i) put(8164 + 4 * i, 0.1f + 0.2f * i);
    near_out = 3.0f; far_out = 100.0f; overcast_value = 0.5f;

    // bit 0: channels raised to 0.4, red zero, green halved, far limited to 20
    if (SetupTimeOfDayFog__7VISceneR21VITerrainProfileBlendUi(s, &blend, 1) != 0) return 1;
    if (fog_calls != 1 || fog_raster != 0x1234 || fog_a != 1) return 2;
    if (fog_near != 0.0f || fog_far != 20.0f) return 3;
    if (!near_eq(fog_colour[0], 0.0f) || !near_eq(fog_colour[1], 0.2f) || !near_eq(fog_colour[2], 0.5f) || !near_eq(fog_colour[3], 0.7f)) return 4;

    // bit 2 beats bit 1: solid red, limited by the scene's own far limit when smaller
    put(10848, 12.0f);
    if (SetupTimeOfDayFog__7VISceneR21VITerrainProfileBlendUi(s, &blend, 6) != 0) return 5;
    if (fog_far != 12.0f || fog_near != 0.0f) return 6;
    if (fog_colour[0] != 1.0f || fog_colour[1] != 0.0f || fog_colour[2] != 0.0f || fog_colour[3] != 1.0f) return 7;

    // bit 3 limits far to 5
    put(10848, 50.0f);
    if (SetupTimeOfDayFog__7VISceneR21VITerrainProfileBlendUi(s, &blend, 8) != 0) return 8;
    if (fog_far != 5.0f || !near_eq(fog_colour[0], 70.0f / 255.0f * 0.4f)) return 9;

    // no flags: blend of profile records, then hour records interpolated by the fraction, then overcast
    blend.profile[0] = 1; blend.weight[0] = 1.0f;
    for (int i = 1; i < 4; ++i) { blend.profile[i] = 0; blend.weight[i] = 0.0f; }
    for (int i = 0; i < 4; ++i) {
        put(100 + 40 + 4 * i, 0.5f);                 // profile record 1
        put(108 * 2 + 8256 + 4 * i, 0.2f);           // hour 2 first colour
        put(108 * 3 + 8256 + 4 * i, 0.6f);           // hour 3 first colour
        put(108 * 2 + 8272 + 4 * i, 1.0f);           // hour 2 second colour
        put(108 * 3 + 8272 + 4 * i, 3.0f);           // hour 3 second colour
    }
    if (SetupTimeOfDayFog__7VISceneR21VITerrainProfileBlendUi(s, &blend, 0) != 0) return 10;
    // B = 0.4, C = 2.0, A = ((2.0 - 0.4) * 0.5 + 0.4) * 0.5 = 0.6
    if (!near_eq(fog_colour[0], 0.6f) || !near_eq(fog_colour[3], 0.6f)) return 11;
    if (fog_near != 3.0f || fog_far != 50.0f) return 12;
    return 0;
}
