// Observed SetupTimeOfDayFog__7VISceneR21VITerrainProfileBlendUi at 0x01140e40, 2088 bytes. Arguments:
// scene, a terrain profile blend (four profile ids at +0..12 and four weights at +16..28, as filled in by
// CalcProfileBlend) and an unsigned `flags`. The result is zero. Everything is computed in locals and
// handed to the raster; the scene is only read. Scene words used: the float at +8160 (time of day,
// 0 <= t < 24), a 16-byte colour at +8164, the atmosphere at +1600, the float at +10848, the raster
// word at +19356, 108-byte hour records from +0 whose colours are at +8256 and +8272, and 40-byte
// profile records from +100 (four floats each).
//   overcast = GetOvercastInterp__12VIAtmosphere(scene + 1600); hour = int(t), frac = t - hour,
//   next = (hour + 1) wrapped to 0 at 24; CalcTimeOfDayClipPlanes__7VISceneR21VITerrainProfileBlendRfT2(
//   scene, blend, &near, &far) fills two floats ("near"/"far" are inferred from how SetFog uses them).
//   The colour D starts as zero and A (the result colour) as zero. By flag, tested in this order:
//   bit 0: D = the 16 bytes at scene + 8164 with each channel raised to at least 0.4;
//          A = (0, D.y * 0.5, D.z, D.w); near = 0; far = min(far, 20).
//   bit 2: A = (1, 0, 0, 1); near = 0; far = min(far, 20).
//   bit 1: D as in bit 0; A = (D.x * 0.18, D.y * 0.56, D.z * 0.313, D.w); near = 0; far = min(far, 20).
//   bit 3: D as in bit 0; A = ((70 * (1/255)) * D.x, D.y * (65/255), D.z * (50/255), D.w * (255/255));
//          near = 0; far = min(far, 5).
//   none:  D = sum of the four profile records weighted by the blend; B = the hour record colour at +8256
//          interpolated between `hour` and `next` by frac as (next - cur) * frac + cur; C the same for the
//          colour at +8272; A = C; then each channel A = ((A - B) * overcast + B) * D.
//   Finally SetFog__8VIRasteriffRC10VIColor32F(raster, 1, near, min(far, scene[10848]), &A).
// The float constants 0.4, 0.18, 0.56, 0.313, 1/255, 65/255, 50/255 and the last scale are in the image as the
// bit patterns 0x3ecccccc, 0x3e3851eb, 0x3f0f5c28, 0x3ea04189, 0x3b808080, 0x3e828282, 0x3e48c8c8 and 0x3f7fffff
// (the compiler truncates decimal literals, so the source uses short decimals; the last is 0.99999995f);
// The product 70 * (1/255) is a run-time multiply in the image, so it is kept as one.
// Inferred: colours are (r, g, b, a) channels and the flag bits select weather or special-effect fog;
// Unknown: what each flag bit stands for, and what the two clip-plane values are besides distances.
extern "C" {
struct VIScene;
struct VITerrainProfileBlend { int profile[4]; float weight[4]; };

float GetOvercastInterp__12VIAtmosphere(void* atmosphere);
int CalcTimeOfDayClipPlanes__7VISceneR21VITerrainProfileBlendRfT2(VIScene* scene, VITerrainProfileBlend* blend, float* near_value,
                                                                  float* far_value);
void SetFog__8VIRasteriffRC10VIColor32F(unsigned raster, int a, float near_value, float far_value, const float* colour);

static float float_at(const char* address) { return *reinterpret_cast<const float*>(address); }
static float at_least(float lower, float value) { return lower > value ? lower : value; }
static float at_most(float value, float upper) { return value < upper ? value : upper; }

int SetupTimeOfDayFog__7VISceneR21VITerrainProfileBlendUi(VIScene* scene_, VITerrainProfileBlend* blend, unsigned flags) {
    char* scene = reinterpret_cast<char*>(scene_);
    alignas(16) float a[4] = {0.0f, 0.0f, 0.0f, 0.0f};
    alignas(16) float b[4] = {0.0f, 0.0f, 0.0f, 0.0f};
    alignas(16) float c[4] = {0.0f, 0.0f, 0.0f, 0.0f};
    alignas(16) float d[4] = {0.0f, 0.0f, 0.0f, 0.0f};
    float near_value = 0.0f;
    float far_value = 0.0f;
    float overcast = GetOvercastInterp__12VIAtmosphere(scene + 1600);
    float hour_value = float_at(scene + 8160);
    int hour = static_cast<int>(hour_value);
    float frac = hour_value - static_cast<float>(hour);
    int next = hour + 1;
    if (next == 24) next = 0;
    CalcTimeOfDayClipPlanes__7VISceneR21VITerrainProfileBlendRfT2(scene_, blend, &near_value, &far_value);
    const float floor_value = 0.4f;
    if (flags & 1) {
        __builtin_memcpy(d, scene + 8164, 16);
        d[0] = at_least(floor_value, d[0]);
        float far_limited = at_most(far_value, 20.0f);
        d[1] = at_least(floor_value, d[1]);
        d[2] = at_least(floor_value, d[2]);
        d[3] = at_least(floor_value, d[3]);
        a[0] = 0.0f;
        a[1] = d[1] * 0.5f;
        a[2] = d[2];
        a[3] = d[3];
        near_value = 0.0f;
        far_value = far_limited;
    } else if (flags & 4) {
        far_value = at_most(far_value, 20.0f);
        near_value = 0.0f;
        a[0] = 1.0f;
        a[1] = 0.0f;
        a[2] = 0.0f;
        a[3] = 1.0f;
    } else if (flags & 2) {
        __builtin_memcpy(d, scene + 8164, 16);
        d[0] = at_least(floor_value, d[0]);
        d[1] = at_least(floor_value, d[1]);
        d[2] = at_least(floor_value, d[2]);
        d[3] = at_least(floor_value, d[3]);
        a[0] = d[0] * 0.18f;
        a[1] = d[1] * 0.56f;
        a[2] = d[2] * 0.313f;
        a[3] = d[3];
        far_value = at_most(far_value, 20.0f);
        near_value = 0.0f;
    } else if (flags & 8) {
        __builtin_memcpy(d, scene + 8164, 16);
        d[0] = at_least(floor_value, d[0]);
        d[1] = at_least(floor_value, d[1]);
        d[2] = at_least(floor_value, d[2]);
        d[3] = at_least(floor_value, d[3]);
        float red = 70.0f;
#ifdef __mips__
        __asm__("" : "+f"(red));
#endif
        red = red * (1.0f / 255.0f);
        a[2] = d[2] * (50.0f / 255.0f);
        a[1] = d[1] * (65.0f / 255.0f);
        a[3] = d[3] * 0.99999995f;
        a[0] = red * d[0];
        far_value = at_most(far_value, 5.0f);
        near_value = 0.0f;
    } else {
        const float* record0 = reinterpret_cast<const float*>(scene + 100 + 40 * blend->profile[0]);
        const float* record1 = reinterpret_cast<const float*>(scene + 100 + 40 * blend->profile[1]);
        const float* record2 = reinterpret_cast<const float*>(scene + 100 + 40 * blend->profile[2]);
        const float* record3 = reinterpret_cast<const float*>(scene + 100 + 40 * blend->profile[3]);
        for (int i = 0; i < 4; ++i) d[i] = record0[i] * blend->weight[0];
        for (int i = 0; i < 4; ++i) d[i] = d[i] + record1[i] * blend->weight[1];
        for (int i = 0; i < 4; ++i) d[i] = d[i] + record2[i] * blend->weight[2];
        for (int i = 0; i < 4; ++i) d[i] = d[i] + record3[i] * blend->weight[3];
        const float* now1 = reinterpret_cast<const float*>(scene + 108 * hour + 8256);
        const float* now2 = reinterpret_cast<const float*>(scene + 108 * hour + 8272);
        __builtin_memcpy(b, scene + 108 * next + 8256, 16);
        for (int i = 0; i < 4; ++i) b[i] = b[i] - now1[i];
        for (int i = 0; i < 4; ++i) b[i] = b[i] * frac;
        for (int i = 0; i < 4; ++i) b[i] = b[i] + now1[i];
        __builtin_memcpy(c, scene + 108 * next + 8272, 16);
        for (int i = 0; i < 4; ++i) c[i] = c[i] - now2[i];
        for (int i = 0; i < 4; ++i) c[i] = c[i] * frac;
        for (int i = 0; i < 4; ++i) c[i] = c[i] + now2[i];
        for (int i = 0; i < 4; ++i) a[i] = c[i];
        for (int i = 0; i < 4; ++i) a[i] = ((a[i] - b[i]) * overcast + b[i]) * d[i];
    }
    float far_final = at_most(far_value, float_at(scene + 10848));
    far_value = far_final;
    SetFog__8VIRasteriffRC10VIColor32F(*reinterpret_cast<unsigned*>(scene + 19356), 1, near_value, far_final, a);
    return 0;
}
}
