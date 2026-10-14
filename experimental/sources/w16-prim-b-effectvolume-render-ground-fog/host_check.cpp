// Host check only; not a build input. Pointer words hold arena offsets (NORRATH_HOST_ARENA).
#define NORRATH_HOST_ARENA 1
#include <initializer_list>
#include <math.h>
#include <string.h>
#include <string>
#include <vector>
unsigned _14VIEffectVolume$mScene __asm__("_14VIEffectVolume$mScene");
unsigned char sFogCard[384] __asm__("sFogCard");
#include "render_ground_fog.cpp"

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
static float* g_m;
static float g_matrix_seen[16];
extern "C" void SetMaterial__8VIRasteri(void* r, int m) { add("material", r, nullptr, 0, m); }
extern "C" void SetSolidFillUIAVGMaterial__8VIRaster(void* r) { add("solid", r, nullptr, 0); }
extern "C" void Color__8VIRasterRC10VIColor32F(void* r, const float* c) { add("color", r, c, 4); }
extern "C" void BeginTriFan__8VIRaster(void* r) { add("begin", r, nullptr, 0); }
extern "C" void UV__8VIRasterRC7VIVect2(void* r, const float* c) { add("uv", r, c, 2); }
extern "C" void Vertex__8VIRasterRC7VIVect3(void* r, const float* c) { add("vertex", r, c, 3); }
extern "C" void EndTriFan__8VIRaster(void* r) { add("end", r, nullptr, 0); }
extern "C" float sinf(float x) { return x * 2.0f; }
extern "C" void PrepareBillboardTransform__14VIEffectVolumeRC10VIMatrix44T1P10VIMatrix44(float* result, void*, const float* cam, const float* mat, float* out) {
    add("prepare", nullptr, nullptr, 0);
    g_m = out;
    for (int i = 0; i < 16; ++i) if (out[i] != 0.0f) add("badzero", nullptr, nullptr, 0);
    for (int i = 0; i < 16; ++i) { out[i] = (i % 5 == 0) ? 1.0f : 0.0f; result[i] = 9.0f; }
    events.back().f[0] = cam[0];
    events.back().f[1] = mat[0];
}
extern "C" void Mul__C10VIMatrix44RC7VIVect3R7VIVect3(const float* m, const float* in, float* out) {
    add("mul", nullptr, nullptr, 0);
    out[0] = in[0] + m[12];
    out[1] = in[1] + m[13];
    out[2] = in[2] + m[14];
}
extern "C" void _$_10VIMatrix44(float* m, int flags) {
    add(m == g_m ? "dtorM" : "dtorTemp", nullptr, nullptr, 0, flags);
}

static void put32(unsigned char* p, unsigned v) { memcpy(p, &v, 4); }
static void putf(unsigned char* p, float v) { memcpy(p, &v, 4); }

int main() {
    for (int i = 0; i < 384 / 4; ++i) putf(sFogCard + 4 * i, 1.0f + i);
    const unsigned scene = 0x2000, raster = 0x3000, particles = 0x100;
    void* rasterPtr = arena + raster;
    float camera[100], matrixArg[16];
    for (int i = 0; i < 100; ++i) camera[i] = 0.0f;
    camera[44] = 7.0f;   // camera + 176 bytes
    for (int i = 0; i < 16; ++i) matrixArg[i] = 5.0f;
    for (int mode = 0; mode < 7; ++mode) {
        memset(arena, 0, sizeof arena);
        _14VIEffectVolume$mScene = scene;
        put32(arena + scene + 19356, raster);
        unsigned char* o = arena + 0x1000;
        putf(o + 92, mode == 1 ? 0.0f : 2.0f);
        putf(o + 96, mode == 2 ? 0.0f : 0.5f);
        putf(o + 44, 0.5f);
        putf(o + 40, 3.0f);
        put32(o + 88, mode == 3 ? 0 : particles);
        put32(o + 84, mode == 4 ? 0 : 2);
        put32(o + 76, mode == 5 ? 7 : static_cast<unsigned>(-1));
        unsigned char* p = arena + particles;
        put32(p, 1); putf(p + 4, 10.0f); putf(p + 8, 20.0f); putf(p + 12, 30.0f);
        put32(p + 16, 1); putf(p + 20, 2.0f); putf(p + 24, 3.0f); putf(p + 28, 1.0f); putf(p + 32, 4.0f);
        put32(p + 36, 0);
        events.clear();
        int r = RenderGroundFog__14VIEffectVolumeR8VICameraR10VIMatrix44(o, reinterpret_cast<unsigned char*>(camera), matrixArg);
        if (mode == 1 || mode == 2) { if (r != 0 || !events.empty()) return 1; continue; }
        if (mode == 3) { if (r != -1 || !events.empty()) return 2; continue; }
        if (r != 0) return 3;
        if (events[0].name != (mode == 5 ? "material" : "solid")) return 4;
        if (events[1].name != "prepare" || events[1].f[0] != 7.0f || events[1].f[1] != 5.0f) return 5;
        if (events[2].name != "dtorTemp" || events[2].material != 2) return 6;
        if (events.back().name != "dtorM" || events.back().material != 2) return 7;
        if (mode == 4) { if (events.size() != 4) return 8; continue; }
        // one active particle: 4 muls, color, begin, 6 uv/vertex pairs, end
        if (events.size() != 3 + 4 + 1 + 1 + 12 + 1 + 1) return 9;
        float phase = (1.0f - 0.5f) * 0.5f + 0.5f;
        float s = (1.0f / 4.0f * 0x1.921fb4p+1f) * 2.0f * phase;
        const Event& color = events[3 + 4];
        if (color.name != "color" || color.f[3] != 3.0f * 2.0f * (0.5f * s)) return 10;
        const float* card = reinterpret_cast<const float*>(sFogCard + 48);
        // vertex 0 is corner 0: card corner 0 scaled by (2, 3) then moved by the translation row
        const Event& v0 = events[3 + 4 + 2 + 1];
        if (v0.name != "vertex") return 11;
        if (v0.f[0] != card[0] * 2.0f + 10.0f || v0.f[1] != card[1] * 3.0f + (20.0f + s * 1.5f) || v0.f[2] != card[2] + 30.0f) return 12;
    }
    return 0;
}
