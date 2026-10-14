// Host check only; not a build input. Pointer words are 32-bit on the target, so the host maps them through a small arena.
#define NORRATH_HOST_ARENA
#include <cstdio>
#include <cstring>
static unsigned char arena[1 << 17];
static const unsigned arena_base = 0x100000u;
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + (address - arena_base); }
static long long now_value;
static int log_[64], nlog;
static unsigned char* log_arg[64];
static float last_vec[3];
extern "C" long long GetTime__7VIScene(unsigned char*) { log_[nlog++] = 1; return now_value; }
extern "C" void Mul__10VIMatrix44RC10VIMatrix44T1(unsigned char* d, const unsigned char*, const unsigned char*) {
    float* f = reinterpret_cast<float*>(d);
    for (int i = 0; i < 16; ++i) f[i] = static_cast<float>(i + 1);
    f[12] = 1.0f; f[13] = 2.0f; f[14] = 3.0f;
}
extern "C" void CalcTransAboutCenter__9VIHSpriteRC10VIMatrix44R7VISceneR10VIMatrix44(unsigned char*, unsigned char*, unsigned char*, unsigned char*) {}
extern "C" void Purge__17VIParticleEmitter(unsigned char* e) { log_[nlog] = 2; log_arg[nlog++] = e; }
extern "C" void SetLocation__17VIParticleEmitterRC7VIVect3(unsigned char* e, const unsigned char* v) { log_[nlog] = 3; log_arg[nlog++] = e; std::memcpy(last_vec, v, 12); }
extern "C" void SetDirection__17VIParticleEmitterRC7VIVect3(unsigned char* e, const unsigned char*) { log_[nlog] = 4; log_arg[nlog++] = e; }
extern "C" void SetDirection__17VIParticleEmitterRC10VIMatrix33(unsigned char* e, const unsigned char* m) { log_[nlog] = 5; log_arg[nlog++] = e; std::memcpy(last_vec, m + 12, 12); }
extern "C" void SetSpace__17VIParticleEmitterRC10VIMatrix44(unsigned char* e, const unsigned char*) { log_[nlog] = 6; log_arg[nlog++] = e; }
extern "C" void Update__17VIParticleEmitter(unsigned char* e) { log_[nlog] = 7; log_arg[nlog++] = e; }
extern "C" void __10VIMatrix33(unsigned char*) {}
extern "C" void _$_10VIMatrix33(unsigned char*, int) {}
extern "C" void _$_10VIMatrix44(unsigned char*, int) {}
#include "update_particle_emitters.cpp"
static int& I(unsigned off) { return *reinterpret_cast<int*>(arena + off); }
int main() {
    bool ok = true;
    unsigned char* self = arena; unsigned char* cam = arena + 0x1000; unsigned char* scene = arena + 0x2000;
    const unsigned ENT = 0x4000;
    std::memset(arena, 0, sizeof arena);
    float* c = reinterpret_cast<float*>(cam + 28); c[0] = 1.0f; c[1] = 2.0f; c[2] = 3.0f;
    I(4616) = 2; I(4620) = arena_base + ENT;
    I(ENT + 1056 + 1020) = 1;            // second entry has a direction matrix
    now_value = 7;
    *reinterpret_cast<long long*>(self + 4608) = 7;
    ok = ok && UpdateParticleEmitters__9VICSpriteR8VICameraR7VISceneR8VIRasterR10VIMatrix44(self, cam, scene, 0, arena + 0x3000) == 0 && nlog == 1;
    nlog = 0; now_value = 8;
    ok = ok && UpdateParticleEmitters__9VICSpriteR8VICameraR7VISceneR8VIRasterR10VIMatrix44(self, cam, scene, 0, arena + 0x3000) == 0;
    ok = ok && *reinterpret_cast<long long*>(self + 4608) == 8;
    // entry 0: SetLocation, SetDirection(vec), SetSpace, Update; entry 1: SetLocation, SetDirection(matrix), Update
    int want[] = {1, 1, 3, 4, 6, 7, 3, 5, 7};
    ok = ok && nlog == 1 + 1 + 3 + 1 + 3 + 0 + 0 + 0 && nlog >= 9;
    int k = 0;
    for (int i = 0; i < nlog && k < 9; ++i) if (log_[i] != 1) { ok = ok && log_[i] == want[k + 2]; ++k; }
    ok = ok && last_vec[0] == 5.0f && last_vec[1] == 6.0f && last_vec[2] == 7.0f;   // matrix rows 4..6 of B (floats 4,5,6 = 5,6,7)
    // far entry is purged
    nlog = 0; now_value = 9;
    Mul__10VIMatrix44RC10VIMatrix44T1(arena + 0x3800, 0, 0);
    c[0] = 1000.0f;
    UpdateParticleEmitters__9VICSpriteR8VICameraR7VISceneR8VIRasterR10VIMatrix44(self, cam, scene, 0, arena + 0x3000);
    int purges = 0; for (int i = 0; i < nlog; ++i) purges += log_[i] == 2;
    ok = ok && purges == 2;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
