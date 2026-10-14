// Host check only; not a build input. Stubs record the call order and the soft-double helpers use the host's double.
#define NORRATH_HOST_ARENA 1
#include "render.cpp"
#include <cmath>
#include <cstring>
#include <string>
static unsigned char scene_block[20000];
unsigned char* _14VIEffectVolume$mScene = scene_block;
static std::string tr; static float mag; static float last_offset[3]; static float last_mul_a[16], last_mul_b[16];
static float modelview[16];
static unsigned long long pack(double d) { unsigned long long v; std::memcpy(&v, &d, 8); return v; }
static double unpack(unsigned long long v) { double d; std::memcpy(&d, &v, 8); return d; }
extern "C" void AllocParticles__14VIEffectVolume(void*) { tr += "Alloc;"; }
extern "C" void sceVu0CopyMatrix(void* d, const void* s) { std::memcpy(d, s, 64); tr += "Copy;"; }
extern "C" void* Transpose__10VIMatrix44RC10VIMatrix44(void* d, const void* s) {
    const float* in = static_cast<const float*>(s); float* out = static_cast<float*>(d);
    for (int r = 0; r < 4; r++) for (int c = 0; c < 4; c++) out[4 * r + c] = in[4 * c + r];
    tr += "Transpose;"; return d;
}
extern "C" const void* GetModelView__C8VIRaster(void*) { tr += "ModelView;"; return modelview; }
extern "C" void* Mul__10VIMatrix44RC10VIMatrix44T1(void* d, const void* a, const void* b) {
    std::memcpy(last_mul_a, a, 64); std::memcpy(last_mul_b, b, 64);
    float* o = static_cast<float*>(d);
    for (int i = 0; i < 16; i++) o[i] = last_mul_a[i] + last_mul_b[i];
    tr += "Mul;"; return d;
}
extern "C" float Magnitude__C7VIVect3(const void* v) { std::memcpy(last_offset, v, 12); tr += "Mag;"; return mag; }
extern "C" unsigned long long fptodp(float v) { return pack(v); }
extern "C" int dpcmp(unsigned long long a, unsigned long long b) { double x = unpack(a), y = unpack(b); return x < y ? -1 : x > y ? 1 : 0; }
extern "C" unsigned long long dpsub(unsigned long long a, unsigned long long b) { return pack(unpack(a) - unpack(b)); }
extern "C" float dptofp(unsigned long long v) { return static_cast<float>(unpack(v)); }
extern "C" void _$_10VIMatrix44(void*, int f) { tr += "Dtor" + std::to_string(f) + ";"; }
extern "C" void RenderFallingLeaves__14VIEffectVolumeR8VICameraR10VIMatrix44(void*, void*, void*) { tr += "R1;"; }
extern "C" void RenderLightShafts__14VIEffectVolumeR8VICameraR10VIMatrix44(void*, void*, void*) { tr += "R2;"; }
extern "C" void RenderGroundFog__14VIEffectVolumeR8VICameraR10VIMatrix44(void*, void*, void*) { tr += "R3;"; }
extern "C" void RenderDustStorm__14VIEffectVolumeR8VICameraR10VIMatrix44(void*, void*, void*) { tr += "R4;"; }
extern "C" void* norrath_host_pointer(unsigned a) { static unsigned char arena[16]; return arena + a % 16; }

static float rd(unsigned char* v, int off) { float f; std::memcpy(&f, v + off, 4); return f; }
static void wr(unsigned char* v, int off, float f) { std::memcpy(v + off, &f, 4); }
int main() {
    alignas(16) static unsigned char object[256];
    alignas(16) static unsigned char camera[256];
    static unsigned char raster_block[16];
    unsigned raster_word = 0;
    (void)raster_word;
    auto run = [&](unsigned type, float radius, float dist, float a, float b, float now) {
        std::memset(object, 0, sizeof object);
        std::memcpy(object + 36, &type, 4); wr(object, 48, radius); wr(object, 52, a); wr(object, 56, b);
        wr(object, 92, -7.0f); wr(object, 96, -7.0f);
        mag = dist; tr.clear();
        wr(scene_block, 8160, now);
        std::memset(scene_block + 19356, 0, 4);
        return Render__14VIEffectVolumeR8VICameraR10VIMatrix44(object, camera, nullptr);
    };
    for (int i = 0; i < 16; i++) { wr(camera, 48 + 4 * i, static_cast<float>(i + 1)); modelview[i] = static_cast<float>(100 + i); }
    wr(camera, 28, 10.0f); wr(camera, 32, 20.0f); wr(camera, 36, 30.0f);
    (void)raster_block;
    // distance fade
    if (run(0, 10.0f, 5.0f, 6.0f, 6.1f, 0) != 0 || rd(object, 96) != 1.0f) return 1;
    run(0, 10.0f, 7.5f, 6.0f, 6.1f, 0); if (rd(object, 96) != 1.0f - 0.0f / 2.5f) return 2;
    run(0, 10.0f, 9.0f, 6.0f, 6.1f, 0); if (std::fabs(rd(object, 96) - 0.4f) > 1e-5f) return 3;
    run(0, 10.0f, 10.0f, 6.0f, 6.1f, 0); if (rd(object, 96) != 0.0f || tr.find("Dtor2;Dtor2;Dtor2;") == std::string::npos) return 4;
    run(1, 10.0f, 10.5f, 6.0f, 6.1f, 0); if (rd(object, 96) != 0.0f || tr.find("R1") != std::string::npos || rd(object, 92) != -7.0f) return 5;
    // matrices: B row 3 is the negated dot products; the last column is cleared before Mul
    run(0, 10.0f, 1.0f, 6.0f, 6.1f, 0);
    float tb[16]; for (int r = 0; r < 4; r++) for (int c = 0; c < 4; c++) tb[4 * r + c] = static_cast<float>(c * 4 + r + 1);
    float e12 = -(tb[3] * tb[0] + tb[7] * tb[4] + tb[11] * tb[8]);
    if (last_mul_b[12] != e12 || last_mul_b[3] != 0 || last_mul_b[7] != 0 || last_mul_b[11] != 0 || last_mul_a[0] != 100.0f) return 6;
    // offset passed to Magnitude: camera position minus D[12..14] where D = A + B
    if (last_offset[0] != 10.0f - (100.0f + 12 + last_mul_b[12]) ) return 7;
    // time ramp
    run(0, 10.0f, 1.0f, 6.0f, 6.1f, 3.0f); if (rd(object, 92) != 0.0f) return 8;      // narrow window with a < b: 0
    run(0, 10.0f, 1.0f, 6.1f, 6.0f, 3.0f); if (rd(object, 92) != 1.0f) return 9;      // narrow window with b < a: 1
    run(0, 10.0f, 1.0f, 0.0f, 23.9f, 3.0f); if (rd(object, 92) != 1.0f) return 10;
    run(0, 10.0f, 1.0f, 23.9f, 0.0f, 3.0f); if (rd(object, 92) != 0.0f) return 11;
    run(0, 10.0f, 1.0f, 8.0f, 18.0f, 12.0f); if (rd(object, 92) != 1.0f) return 12;
    run(0, 10.0f, 1.0f, 8.0f, 18.0f, 7.95f); if (std::fabs(rd(object, 92) - 0.4f) > 1e-3f) return 13;
    run(0, 10.0f, 1.0f, 8.0f, 18.0f, 18.05f); if (std::fabs(rd(object, 92) - 0.4f) > 1e-3f) return 14;
    run(0, 10.0f, 1.0f, 8.0f, 18.0f, 3.0f); if (rd(object, 92) != 0.0f) return 15;
    run(0, 10.0f, 1.0f, 20.0f, 6.0f, 3.0f); if (rd(object, 92) != 1.0f) return 16;      // wrapped window
    run(0, 10.0f, 1.0f, 20.0f, 6.0f, 12.0f); if (rd(object, 92) != 0.0f) return 17;
    // dispatch order after the fades
    run(2, 10.0f, 1.0f, 8.0f, 18.0f, 12.0f);
    if (tr != "Copy;Transpose;ModelView;Copy;Mul;Mag;R2;Dtor2;Dtor2;Dtor2;") return 18;
    return 0;
}
