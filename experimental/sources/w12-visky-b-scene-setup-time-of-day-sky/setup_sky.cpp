// Observed: SetupTimeOfDaySky__7VISceneR21VITerrainProfileBlend at 0x01141780, 1780 bytes. Arguments:
// the scene and a profile blend (four profile indices at +0 .. +12, four float weights at +16 ..
// +28). The result is zero. The atmosphere object is the scene + 1600. Offsets are bytes.
//   1. overcast = GetOvercastInterp__12VIAtmosphere(scene + 1600).
//   2. B = sum over k = 0..3 of the four floats at scene + 100 + 40 * profile[k], each scaled by
//      weight[k], summed in the order ((k0 + k1) + k2) + k3 per component.
//   3. time = the float at scene + 8160; hour = (int)time; next = hour + 1, or 0 when that is 24;
//      frac = time - (float)hour. Hour records are 108 bytes apart: record h has a four-float
//      vector at +8256 and another at +8272 (from the scene start).
//   4. C = A0 + (A1 - A0) * frac with A0, A1 the +8256 vectors of the hour and next-hour records,
//      and D likewise from the +8272 vectors (per component: subtract, multiply by frac, add back).
//   5. S = ((D - C) * overcast + C), then multiplied component-wise by B.
//   6. A colour of four bytes, each (unsigned char)(unsigned)(S[i] * 255.0f + 0.5f), is passed to
//      SetSkyColor__12VIAtmosphereR7VISceneRC9VIColor32(scene + 1600, scene, &colour).
//   7. L = ((f0 * w0 + f1 * w1) + f2 * w2) + f3 * w3, where f_k is the float at
//      scene + 128 + 40 * profile[k]; SetLavastormIntensity__12VIAtmospheref(scene + 1600, L).
// Inferred: the scene keeps a table of 40-byte terrain profile records at +100 whose first four
// floats multiply the sky colour and whose float at +28 feeds the lava-storm intensity; the 24
// hour records carry two colour vectors that are blended across the hour and then by overcast.
// Unknown: which of the two +8256 / +8272 vectors is which sky state, and the meaning of the other
// floats of a profile record.
extern "C" {
struct VIScene;
struct VITerrainProfileBlend { int profile[4]; float weight[4]; };

float GetOvercastInterp__12VIAtmosphere(void* atmosphere);
void SetSkyColor__12VIAtmosphereR7VISceneRC9VIColor32(void* atmosphere, void* scene, const void* color);
void SetLavastormIntensity__12VIAtmospheref(void* atmosphere, float intensity);

struct Vec4 { float v[4]; };

static unsigned char to_byte(float value) {
    return static_cast<unsigned char>(static_cast<unsigned>(value * 255.0f + 0.5f));
}

int SetupTimeOfDaySky__7VISceneR21VITerrainProfileBlend(VIScene* scene_, VITerrainProfileBlend* blend) {
    unsigned char* scene = reinterpret_cast<unsigned char*>(scene_);
    void* atmosphere = scene + 1600;
    Vec4 sky;
    unsigned char color[4] = {0, 0, 0, 255};
    Vec4 tint;
    Vec4 c, d;
    for (int i = 0; i < 4; ++i) { sky.v[i] = 0.0f; tint.v[i] = 0.0f; c.v[i] = 0.0f; d.v[i] = 0.0f; }

    float overcast = GetOvercastInterp__12VIAtmosphere(atmosphere);
    float time = *reinterpret_cast<float*>(scene + 8160);
    int hour = static_cast<int>(time);

    for (int k = 0; k < 4; ++k) {
        const float* profile = reinterpret_cast<const float*>(scene + 100 + 40 * blend->profile[k]);
        float weight = blend->weight[k];
        for (int i = 0; i < 4; ++i) {
            if (k == 0) tint.v[i] = profile[i] * weight;
            else tint.v[i] = tint.v[i] + profile[i] * weight;
        }
    }

    int next = hour + 1;
    if (next == 24) next = 0;
    float frac = time - static_cast<float>(hour);
    const float* hour_c = reinterpret_cast<const float*>(scene + 108 * hour + 8256);
    const float* hour_d = reinterpret_cast<const float*>(scene + 108 * hour + 8272);
    const float* next_c = reinterpret_cast<const float*>(scene + 108 * next + 8256);
    const float* next_d = reinterpret_cast<const float*>(scene + 108 * next + 8272);
    for (int i = 0; i < 4; ++i) {
        c.v[i] = next_c[i];
        c.v[i] = c.v[i] - hour_c[i];
        c.v[i] = c.v[i] * frac;
        c.v[i] = c.v[i] + hour_c[i];
    }
    for (int i = 0; i < 4; ++i) {
        d.v[i] = next_d[i];
        d.v[i] = d.v[i] - hour_d[i];
        d.v[i] = d.v[i] * frac;
        d.v[i] = d.v[i] + hour_d[i];
    }
    for (int i = 0; i < 4; ++i) {
        sky.v[i] = d.v[i];
        sky.v[i] = sky.v[i] - c.v[i];
        sky.v[i] = sky.v[i] * overcast;
        sky.v[i] = sky.v[i] + c.v[i];
        sky.v[i] = sky.v[i] * tint.v[i];
    }
    for (int i = 0; i < 4; ++i) color[i] = to_byte(sky.v[i]);
    SetSkyColor__12VIAtmosphereR7VISceneRC9VIColor32(atmosphere, scene, color);

    float lava0 = *reinterpret_cast<float*>(scene + 128 + 40 * blend->profile[0]) * blend->weight[0];
    float lava1 = *reinterpret_cast<float*>(scene + 128 + 40 * blend->profile[1]) * blend->weight[1];
    float lava2 = *reinterpret_cast<float*>(scene + 128 + 40 * blend->profile[2]) * blend->weight[2];
    float lava3 = *reinterpret_cast<float*>(scene + 128 + 40 * blend->profile[3]) * blend->weight[3];
    SetLavastormIntensity__12VIAtmospheref(atmosphere, ((lava0 + lava1) + lava2) + lava3);
    return 0;
}
}
