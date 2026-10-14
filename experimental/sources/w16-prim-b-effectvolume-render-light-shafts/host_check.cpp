// Host check only; not a build input. Pointer words hold arena offsets (NORRATH_HOST_ARENA).
#define NORRATH_HOST_ARENA 1
#include <initializer_list>
#include <math.h>
#include <string.h>
#include <string>
#include <vector>
unsigned _14VIEffectVolume$mScene __asm__("_14VIEffectVolume$mScene");
#include "render_light_shafts.cpp"

alignas(16) static unsigned char arena[0x8000];
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + address; }

struct Event {
    std::string name;
    float f[4];
    int material;
    void* raster;
};
static std::vector<Event> events;
static void add(const char* n, void* r, const float* f, int count, int material = 0) {
    Event e{n, {0, 0, 0, 0}, material, r};
    for (int i = 0; i < count; ++i) e.f[i] = f[i];
    events.push_back(e);
}
static float *g_m1, *g_m2, *g_m3;
extern "C" void SetMaterial__8VIRasteri(void* r, int m) { add("material", r, nullptr, 0, m); }
extern "C" void SetSolidFillUIAVGMaterial__8VIRaster(void* r) { add("solid", r, nullptr, 0); }
extern "C" void Color__8VIRasterRC10VIColor32F(void* r, const float* c) { add("color", r, c, 4); }
extern "C" void BeginTriFan__8VIRaster(void* r) { add("begin", r, nullptr, 0); }
extern "C" void UV__8VIRasterRC7VIVect2(void* r, const float* c) { add("uv", r, c, 2); }
extern "C" void Vertex__8VIRasterRC7VIVect3(void* r, const float* c) { add("vertex", r, c, 3); }
extern "C" void EndTriFan__8VIRaster(void* r) { add("end", r, nullptr, 0); }
extern "C" float sinf(float x) { return x * 2.0f; }
extern "C" float atanf(float x) { return x + 100.0f; }
extern "C" void Identity__10VIMatrix44(float* m) {
    g_m2 = m;
    for (int i = 0; i < 16; ++i) if (m[i] != 0.0f) add("badzero", nullptr, nullptr, 0);
    for (int i = 0; i < 16; ++i) m[i] = (i % 5 == 0) ? 1.0f : 0.0f;
    add("identity", nullptr, nullptr, 0);
}
extern "C" void RotateY__10VIMatrix44f(float* m, float angle) {
    float a[1] = {angle};
    add("rotatey", nullptr, a, 1);
    if (m != g_m2) add("badrot", nullptr, nullptr, 0);
}
extern "C" void PrepareBillboardTransform__14VIEffectVolumeRC10VIMatrix44T1P10VIMatrix44(float* result, void*, const float* m2, const float* mat, float* out) {
    add("prepare", nullptr, nullptr, 0);
    if (m2 != g_m2) add("badprep", nullptr, nullptr, 0);
    g_m1 = out;
    g_m3 = result;
    for (int i = 0; i < 16; ++i) if (out[i] != 0.0f) add("badzero", nullptr, nullptr, 0);
    for (int i = 0; i < 16; ++i) { out[i] = (i % 5 == 0) ? 1.0f : 0.0f; result[i] = 2.0f; }
    events.back().f[0] = mat[0];
}
extern "C" void Mul__C10VIMatrix44RC7VIVect3R7VIVect3(const float* m, const float* in, float* out) {
    float a[3] = {in[0], in[1], in[2]};
    add(m == g_m3 ? "mulR" : "mul", nullptr, a, 3);
    out[0] = m == g_m3 ? in[0] * 4.0f : in[0];
    out[1] = m == g_m3 ? in[1] : in[1];
    out[2] = in[2];
}
extern "C" void _$_10VIMatrix44(float* m, int flags) {
    add(m == g_m3 ? "dtorR" : m == g_m2 ? "dtorM2" : "dtorM1", nullptr, nullptr, 0, flags);
}

static void put32(unsigned char* p, unsigned v) { memcpy(p, &v, 4); }
static void putf(unsigned char* p, float v) { memcpy(p, &v, 4); }

int main() {
    const unsigned scene = 0x2000, raster = 0x3000, particles = 0x100;
    void* rasterPtr = arena + raster;
    float camera[8] = {0, 0, 0.5f, 0.0f, 0.25f, 0, 0, 0};   // floats at byte offsets 8, 12, 16
    float matrixArg[16];
    for (int i = 0; i < 16; ++i) matrixArg[i] = 5.0f;
    // modes: 0 guard on 92, 1 guard on 96, 2 null particles, 3 beams with now before 9 (92 reset, return 0),
    // 4 beams at 9.5 (ramp), 5 no beams, 6 beams at 12 (hold), 7 beams at 14.5 (ramp down), 8 count 0
    for (int mode = 0; mode < 9; ++mode) {
        memset(arena, 0, sizeof arena);
        _14VIEffectVolume$mScene = scene;
        put32(arena + scene + 19356, raster);
        float nowValues[9] = {12, 12, 12, 8.0f, 9.5f, 12, 12, 14.5f, 12};
        putf(arena + scene + 8160, nowValues[mode]);
        unsigned char* o = arena + 0x1000;
        putf(o + 92, mode == 0 ? 0.0f : 2.0f);
        putf(o + 96, mode == 1 ? 0.0f : 0.5f);
        putf(o + 44, 0.5f);
        put32(o + 60, mode == 5 ? 0 : 2);
        put32(o + 88, mode == 2 ? 0 : particles);
        put32(o + 84, mode == 8 ? 0 : 1);
        put32(o + 76, static_cast<unsigned>(-1));
        unsigned char* p = arena + particles;
        put32(p, 1); putf(p + 4, 10.0f); putf(p + 8, 20.0f); putf(p + 12, 30.0f);
        putf(p + 16, 2.0f); putf(p + 20, 3.0f); putf(p + 24, 1.0f); putf(p + 28, 4.0f);
        events.clear();
        int r = RenderLightShafts__14VIEffectVolumeR8VICameraR10VIMatrix44(o, reinterpret_cast<unsigned char*>(camera) - 0, matrixArg);
        float intensity;
        memcpy(&intensity, o + 92, 4);
        if (mode == 0 || mode == 1) { if (r != 0 || !events.empty()) return 1; continue; }
        if (mode == 3) { if (r != 0 || intensity != 0.0f || !events.empty()) return 2; continue; }
        if (mode == 2) { if (r != -1 || !events.empty()) return 3; continue; }
        if (r != 0) return 4;
        if (mode == 4 && intensity != 0.5f) return 5;
        if (mode == 6 && intensity != 1.0f) return 6;
        if (mode == 7 && intensity != 0.5f) return 7;
        if (events.back().name != "dtorM1" || events[events.size() - 2].name != "dtorM2" || events[events.size() - 3].name != "dtorR") return 8;
        if (mode == 8) continue;
        // camera x = camera[2] = 0.5 > 0: angle = atanf(z / -x) - pi/2 with z = camera[4] = 0.25
        const Event* rot = nullptr;
        for (const Event& e : events) if (e.name == "rotatey") rot = &e;
        float want_angle = (0.25f / -0.5f + 100.0f) - 0x1.921fb4p+0f;
        if (!rot || rot->f[0] != want_angle) return 9;
        // find the four quad corners: vertices 0..3 of the first fan
        std::vector<const Event*> vertices;
        for (const Event& e : events) if (e.name == "vertex") vertices.push_back(&e);
        if (vertices.size() != 6) return 10;
        const float h = 3.0f;
        float dir = mode == 4 ? (9.5f - 9.0f) * 0x1.555554p-2f + -1.0f : mode == 6 ? (12.0f - 9.0f) * 0x1.555554p-2f + -1.0f : (14.5f - 9.0f) * 0x1.555554p-2f + -1.0f;
        if (mode == 5) dir = 0.0f;
        // corner 0: mul of (-2,0,0) by identity, plus p
        if (vertices[0]->f[0] != -2.0f + 10.0f || vertices[0]->f[1] != 0.0f + 20.0f || vertices[0]->f[2] != 30.0f) return 11;
        if (vertices[1]->f[0] != 2.0f + 10.0f) return 12;
        // corner 2 gets T * h added when beams are on: T = R * (dir, 0, 0) = (dir * 4, 0, 0)
        float tx = mode == 5 ? 0.0f : dir * 4.0f;
        if (vertices[2]->f[0] != 2.0f + 10.0f + tx * h || vertices[2]->f[1] != -3.0f + 20.0f) return 13;
    }
    return 0;
}
