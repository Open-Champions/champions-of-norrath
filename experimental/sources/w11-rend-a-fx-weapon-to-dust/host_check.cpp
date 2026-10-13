#include <cstdio>
#include <cstring>
#include <vector>
int eRandState;
unsigned char fxDust[80];
static int shadowCount;
struct Particle { float pos[3], vel[3], arg; const void* def; };
static std::vector<Particle> particles;
static std::vector<int> shadowIndices;
static float baseX = 100.0f;
extern "C" {
void modelGetAttachmentMatrix__FP12_modelHeaderP14AnimationStateiP8Matrix34f(unsigned char*, unsigned char*, int, float* m, float) { for (int i = 0; i != 12; ++i) m[i] = float(i); }
void matMakeTransYaw__FP8Matrix34G6Point3s(float* m, const float*, short) { for (int i = 0; i != 12; ++i) m[i] = 0; }
void matMul__FP8Matrix34N20(float* d, float*, float*) { for (int i = 0; i != 12; ++i) d[i] = float(i); }
int modelGetNmShadowPoints__FP12_modelHeader(unsigned char*) { return shadowCount; }
void modelGetShadowPos__FP12_modelHeaderP8Matrix34iP6Point3Pf(unsigned char*, float*, int index, float* position, float* height) {
    shadowIndices.push_back(index);
    position[0] = baseX + 1000.0f * index; position[1] = 200.0f; position[2] = 300.0f; *height = 0;
}
void P_AddParticle__FP11ParticleDefP6Point3T1f(const void* def, float* position, float* velocity, float arg) {
    Particle p; std::memcpy(p.pos, position, 12); std::memcpy(p.vel, velocity, 12); p.arg = arg; p.def = def;
    particles.push_back(p);
}
void fxWeaponToDust__FP12_modelHeaderT0P14AnimationStateG6Point3sffi(unsigned char*, unsigned char*, unsigned char*, const float*, short, float, float, int);
}
static unsigned lcg(unsigned s) { return s * 1664525u + 1013904223u; }
int main() {
    unsigned char m1[32] = {}, m2[32] = {}, st[8] = {};
    float point[3] = {0, 0, 0};
    shadowCount = 0;
    eRandState = 1234;
    fxWeaponToDust__FP12_modelHeaderT0P14AnimationStateG6Point3sffi(m1, m2, st, point, 0, 1.0f, 2.0f, 0);
    if (!particles.empty() || eRandState != 1234) return 1;
    shadowCount = 2;
    fxWeaponToDust__FP12_modelHeaderT0P14AnimationStateG6Point3sffi(m1, m2, st, point, 0, 1.0f, 2.0f, 0);
    if (particles.size() != 6 || shadowIndices.size() != 2) return 2;
    // Reproduce the generator by hand for the first particle.
    unsigned s = 1234;
    float v[3];
    for (;;) {
        float r[3];
        for (int k = 0; k != 3; ++k) { s = lcg(s); r[k] = float(int(s >> 16)) * (1.0f / 65536.0f); r[k] = (r[k] + r[k]) - 1.0f; }
        std::memcpy(v, r, 12);
        if (!(1.0f < r[0] * r[0] + r[1] * r[1] + r[2] * r[2])) break;
    }
    const float K = 0x1.b99998p+1f;
    s = lcg(s); unsigned r1 = (s >> 16) % 6;
    s = lcg(s); unsigned r2 = (s >> 16) % 6;
    s = lcg(s); unsigned r3 = (s >> 16) % 6;
    const Particle& p = particles[0];
    if (p.vel[0] != v[0] * K || p.vel[1] != v[1] * K || p.vel[2] != v[2] * K - 4.0f) return 3;
    if (p.pos[0] != float(int(r1) - 3) + baseX || p.pos[1] != float(int(r2) - 3) + 200.0f || p.pos[2] != float(int(r3) - 3) + 300.0f) return 4;
    if (p.arg != 0.0f || p.def != fxDust) return 5;
    // Every velocity lies inside the scaled ball, every position offset in [-3, 2].
    for (int i = 0; i != 6; ++i) {
        const Particle& q = particles[i];
        float vx = q.vel[0] / K, vy = q.vel[1] / K, vz = (q.vel[2] + 4.0f) / K;
        if (vx * vx + vy * vy + vz * vz > 1.001f) return 6;
        float base = i < 3 ? baseX : baseX + 1000.0f;
        float dx = q.pos[0] - base;
        if (dx < -3.0f || dx > 2.0f) return 7;
    }
    if (shadowIndices[0] != 0 || shadowIndices[1] != 1) return 8;
    std::puts("fxWeaponToDust contract passed");
    return 0;
}
