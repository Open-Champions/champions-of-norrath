// Host check only; not a build input. The library stub records its arguments and writes known lanes.
#include "mul.cpp"
#include <cstdio>

static const float* seenMatrix;
static float seenPoint[4];
static int calls;
extern "C" void sceVu0ApplyMatrix(float* destination, const float* matrix, const float* vector) {
    ++calls;
    seenMatrix = matrix;
    for (int i = 0; i < 4; ++i) { seenPoint[i] = vector[i]; destination[i] = 10.0f + i; }
}

int main() {
    alignas(16) float m[16] = {};
    float v[3] = {1.5f, -2.5f, 3.5f};
    float out[5] = {-1, -1, -1, -1, -77};
    float* r = __ml__FRC7VIVect3RC10VIMatrix44(out, v, m);
    bool ok = r == out && calls == 1 && seenMatrix == m && seenPoint[0] == 1.5f && seenPoint[1] == -2.5f && seenPoint[2] == 3.5f &&
              seenPoint[3] == 1.0f && out[0] == 10.0f && out[1] == 11.0f && out[2] == 12.0f && out[3] == 13.0f && out[4] == -77.0f;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
