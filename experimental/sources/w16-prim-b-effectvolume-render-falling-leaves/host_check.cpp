// Host check only; not a build input. Pointer words hold arena offsets (NORRATH_HOST_ARENA).
#define NORRATH_HOST_ARENA 1
#include <initializer_list>
#include <string.h>
#include <string>
#include <vector>
unsigned _14VIEffectVolume$mScene __asm__("_14VIEffectVolume$mScene");
unsigned char sFallEffect[2880] __asm__("sFallEffect");
#include "render_falling_leaves.cpp"

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
extern "C" void SetMaterial__8VIRasteri(void* r, int m) { add("material", r, nullptr, 0, m); }
extern "C" void SetSolidFillUIAVGMaterial__8VIRaster(void* r) { add("solid", r, nullptr, 0); }
extern "C" void Color__8VIRasterRC10VIColor32F(void* r, const float* c) { add("color", r, c, 4); }
extern "C" void BeginTriFan__8VIRaster(void* r) { add("begin", r, nullptr, 0); }
extern "C" void UV__8VIRasterRC7VIVect2(void* r, const float* c) { add("uv", r, c, 2); }
extern "C" void Vertex__8VIRasterRC7VIVect3(void* r, const float* c) { add("vertex", r, c, 3); }
extern "C" void EndTriFan__8VIRaster(void* r) { add("end", r, nullptr, 0); }

static void put32(unsigned char* p, unsigned v) { memcpy(p, &v, 4); }
static void putf(unsigned char* p, float v) { memcpy(p, &v, 4); }

int main() {
    for (int i = 0; i < 2880 / 4; ++i) putf(sFallEffect + 4 * i, 0.25f * i);
    const unsigned scene = 0x2000, raster = 0x3000, particles = 0x100;
    void* rasterPtr = arena + raster;
    for (int mode = 0; mode < 6; ++mode) {
        memset(arena, 0, sizeof arena);
        _14VIEffectVolume$mScene = scene;
        put32(arena + scene + 19356, raster);
        unsigned char* o = arena + 0x1000;
        float f92 = mode == 1 ? 0.0f : 0.5f, f96 = mode == 2 ? 0.0f : 0.25f;
        putf(o + 92, f92);
        putf(o + 96, f96);
        put32(o + 88, mode == 3 ? 0 : particles);
        put32(o + 84, mode == 4 ? 0 : 5);   // count = (int)(0.5 * 5) = 2 (mode 4: 0)
        put32(o + 76, mode == 5 ? 7 : static_cast<unsigned>(-1));
        // particle 0 active, type 1, frame 3.9; particle 1 inactive
        unsigned char* p = arena + particles;
        put32(p, 1); put32(p + 4, 1); putf(p + 8, 3.9f);
        putf(p + 12, 10.0f); putf(p + 16, 20.0f); putf(p + 20, 30.0f); putf(p + 32, 0.5f);
        put32(p + 36, 0); put32(p + 40, 1);
        events.clear();
        int r = RenderFallingLeaves__14VIEffectVolumeR8VICameraR10VIMatrix44(o, nullptr, nullptr);
        if (mode == 1 || mode == 2 || mode == 4) {
            if (r != 0 || !events.empty() && mode != 4) return 1;
            if (mode == 4 && events.size() != 1) return 2;   // material only, no particles drawn
            continue;
        }
        if (mode == 3) { if (r != -1 || !events.empty()) return 3; continue; }
        if (r != 0) return 4;
        if (events.size() != 1 + 1 + 1 + 12 + 1) return 5;   // material, color, begin, 6 uv+vertex, end
        if (events[0].name != (mode == 5 ? "material" : "solid")) return 6;
        if (mode == 5 && events[0].material != 7) return 7;
        for (const Event& e : events) if (e.raster != rasterPtr) return 8;
        if (events[1].name != "color" || events[1].f[0] != 1.0f || events[1].f[1] != 1.0f || events[1].f[2] != 1.0f || events[1].f[3] != 0.5f * 0.25f) return 9;
        if (events[2].name != "begin" || events.back().name != "end") return 10;
        const float uv[6][2] = {{0, 0}, {1, 0}, {1, 1}, {0, 1}, {1, 1}, {1, 0}};
        const int order[6] = {0, 1, 2, 3, 2, 1};
        const float* record = reinterpret_cast<const float*>(sFallEffect + 1 * 1440 + 3 * 48);
        for (int i = 0; i < 6; ++i) {
            const Event& u = events[3 + 2 * i];
            const Event& v = events[4 + 2 * i];
            if (u.name != "uv" || u.f[0] != uv[i][0] || u.f[1] != uv[i][1]) return 11;
            if (v.name != "vertex") return 12;
            if (v.f[0] != record[3 * order[i]] + 10.0f || v.f[1] != record[3 * order[i] + 1] + 20.0f || v.f[2] != record[3 * order[i] + 2] + 30.0f) return 13;
        }
    }
    return 0;
}
