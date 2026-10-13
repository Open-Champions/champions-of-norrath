// Host check only; not a build input. Native build: c++ -std=c++17 -ffp-contract=off host_check.cpp
#include "calc.cpp"
#include <cstring>

static void put(unsigned char* p, unsigned off, float f) { std::memcpy(p + off, &f, 4); }
static void puti(unsigned char* p, unsigned off, int v) { std::memcpy(p + off, &v, 4); }
static float get(const unsigned char* p, unsigned off) { float f; std::memcpy(&f, p + off, 4); return f; }

int main() {
    alignas(8) unsigned char scene[512], blend[32];
    std::memset(scene, 0, sizeof scene);
    for (int i = 0; i < 6; ++i) { put(scene, 92 + 40 * i, 1.5f * (i + 1)); put(scene, 96 + 40 * i, 0.25f * (i + 1) - 3.0f); }
    puti(blend, 0, 2); puti(blend, 4, 0); puti(blend, 8, 5); puti(blend, 12, 3);
    put(blend, 16, 0.5f); put(blend, 20, 0.25f); put(blend, 24, 0.125f); put(blend, 28, 0.0625f);
    float o1 = -77.0f, o2 = -88.0f;
    if (CalcTimeOfDayClipPlanes__7VISceneR21VITerrainProfileBlendRfT2(scene, blend, &o1, &o2) != 0) return 1;
    const float e1 = ((4.5f * 0.5f + 1.5f * 0.25f) + 9.0f * 0.125f) + 6.0f * 0.0625f;
    const float e2 = (((0.25f * 3 - 3.0f) * 0.5f + (0.25f - 3.0f) * 0.25f) + (0.25f * 6 - 3.0f) * 0.125f) + (0.25f * 4 - 3.0f) * 0.0625f;
    if (o1 != e1 || o2 != e2) return 2;
    // Inputs are only read.
    unsigned char scene2[512];
    std::memset(scene2, 0, sizeof scene2);
    for (int i = 0; i < 6; ++i) { put(scene2, 92 + 40 * i, 1.5f * (i + 1)); put(scene2, 96 + 40 * i, 0.25f * (i + 1) - 3.0f); }
    if (std::memcmp(scene, scene2, sizeof scene) != 0) return 3;
    // Same index four times with weights summing to one reproduces the table value.
    puti(blend, 0, 1); puti(blend, 4, 1); puti(blend, 8, 1); puti(blend, 12, 1);
    put(blend, 16, 0.25f); put(blend, 20, 0.25f); put(blend, 24, 0.25f); put(blend, 28, 0.25f);
    if (CalcTimeOfDayClipPlanes__7VISceneR21VITerrainProfileBlendRfT2(scene, blend, &o1, &o2) != 0 || o1 != 3.0f || o2 != -2.5f) return 4;
    // out1 shares storage with weight w1: the weight is re-read after the first store.
    puti(blend, 0, 0); puti(blend, 4, 0); puti(blend, 8, 0); puti(blend, 12, 0);
    put(blend, 16, 2.0f); put(blend, 20, 100.0f); put(blend, 24, 0.0f); put(blend, 28, 0.0f);
    float o3 = 0;
    CalcTimeOfDayClipPlanes__7VISceneR21VITerrainProfileBlendRfT2(scene, blend, reinterpret_cast<float*>(blend + 20), &o3);
    // first store: 1.5*2 = 3 lands in w1; then 3 + 1.5*3 = 7.5.
    if (get(blend, 20) != 7.5f) return 5;
    return 0;
}
