// Host check only; not a build input.
#include "static_bounds.cpp"
#include <cstdio>
#include <cstring>
static int bound_calls;
extern "C" int Store__C6VIBBoxP7VIVect3(const float* box, float* out) {
    for (int k = 0; k < 8; ++k)
        for (int a = 0; a < 3; ++a) out[3 * k + a] = box[a + 3 * ((k >> a) & 1)];
    return 0;
}
extern "C" void __ml__FRC7VIVect3RC10VIMatrix33(void* result, const void* vector, const void*) { std::memcpy(result, vector, 12); }
extern "C" void Bound__6VIBBoxiRC7VIVect3(float* box, int first, const float* p) {
    ++bound_calls;
    for (int a = 0; a < 3; ++a) {
        if (first) { box[a] = p[a]; box[3 + a] = p[a]; continue; }
        if (p[a] < box[a]) box[a] = p[a];
        if (p[a] > box[3 + a]) box[3 + a] = p[a];
    }
}
int main() {
    alignas(16) unsigned char emitter[1024];
    std::memset(emitter, 0, sizeof emitter);
    float* location = reinterpret_cast<float*>(emitter + 16);
    float* box = reinterpret_cast<float*>(emitter + 916);
    location[0] = 10.0f; location[1] = 20.0f; location[2] = 30.0f;
    box[0] = -1.0f; box[1] = -2.0f; box[2] = -3.0f; box[3] = 4.0f; box[4] = 5.0f; box[5] = 6.0f;
    float result[6] = {9, 9, 9, 9, 9, 9};
    void* r = StaticBounds__C17VIParticleEmitter(result, emitter);
    const float want[6] = {9.0f, 18.0f, 27.0f, 14.0f, 25.0f, 36.0f};
    bool ok = r == result && bound_calls == 8;
    for (int i = 0; i < 6; ++i) if (result[i] != want[i]) ok = false;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
