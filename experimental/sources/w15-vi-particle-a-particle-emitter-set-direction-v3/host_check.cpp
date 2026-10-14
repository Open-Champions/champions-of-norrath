// Host check only; not a build input.
#include "set_direction.cpp"
#include <cmath>
#include <cstring>
static float got[9]; static int calls, mcalls; static int dtor_flag, ctor_seen;
static void norm(float* v) { float l = std::sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]); v[0] /= l; v[1] /= l; v[2] /= l; }
extern "C" void* Normalize__7VIVect3(void* v) { norm(static_cast<float*>(v)); calls++; return v; }
extern "C" void __10VIMatrix33(void* m) { ctor_seen++; std::memset(m, 0, 36); }
extern "C" void _$_10VIMatrix33(void*, int f) { dtor_flag = f; }
extern "C" void SetDirection__17VIParticleEmitterRC10VIMatrix33(void*, const void* m) { std::memcpy(got, m, 36); mcalls++; }
int main() {
    float dirs[4][3] = {{0, 0, 1}, {1, 0, 0}, {0, 1, 0}, {0.3f, -0.4f, 0.8f}};
    for (auto& d : dirs) {
        calls = mcalls = ctor_seen = 0;
        SetDirection__17VIParticleEmitterRC7VIVect3(nullptr, d);
        if (calls != 3 || mcalls != 1 || ctor_seen != 1 || dtor_flag != 2) return 1;
        float len = std::sqrt(d[0] * d[0] + d[1] * d[1] + d[2] * d[2]);
        for (int i = 0; i < 3; i++) if (std::fabs(got[6 + i] - d[i] / len) > 1e-5f) return 2;
        for (int a = 0; a < 3; a++) {
            float l = std::sqrt(got[3 * a] * got[3 * a] + got[3 * a + 1] * got[3 * a + 1] + got[3 * a + 2] * got[3 * a + 2]);
            if (std::fabs(l - 1.0f) > 1e-5f) return 3;
            for (int b = a + 1; b < 3; b++) {
                float dot = got[3 * a] * got[3 * b] + got[3 * a + 1] * got[3 * b + 1] + got[3 * a + 2] * got[3 * b + 2];
                if (std::fabs(dot) > 1e-5f) return 4;
            }
        }
    }
    // d = +Y uses the alternate up vector (0,0,1): first = (0,0,1) x (0,1,0) = (-1,0,0)
    float up[3] = {0, 1, 0};
    SetDirection__17VIParticleEmitterRC7VIVect3(nullptr, up);
    if (std::fabs(got[0] + 1.0f) > 1e-5f || std::fabs(got[1]) > 1e-5f || std::fabs(got[2]) > 1e-5f) return 5;
    // d = +Z uses the default up (0,1,0): first = (0,1,0) x (0,0,1) = (1,0,0)
    float z[3] = {0, 0, 1};
    SetDirection__17VIParticleEmitterRC7VIVect3(nullptr, z);
    if (std::fabs(got[0] - 1.0f) > 1e-5f) return 6;
    return 0;
}
