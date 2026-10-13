#define NORRATH_HOST_CONTRACT 1
int norrath_trail_material_def __asm__("VITrailFx$sMaterialHandle") = 7;
#include "render.cpp"
#include <cstring>
#include <string>
#include <vector>

alignas(8) static unsigned char arena[1 << 16];
extern "C" char* norrath_host_translate(unsigned address) { return reinterpret_cast<char*>(arena) + address; }
static void put(unsigned off, unsigned v) { std::memcpy(arena + off, &v, 4); }
static void putf(unsigned off, float v) { std::memcpy(arena + off, &v, 4); }
static float getf(unsigned off) { float v; std::memcpy(&v, arena + off, 4); return v; }
static unsigned off_of(const void* p) { return static_cast<unsigned>(static_cast<const unsigned char*>(p) - arena); }

static std::vector<std::string> ev;
static int list_length;
static std::string tag(const char* n, unsigned a) { return std::string(n) + ":" + std::to_string(a); }
extern "C" int Length__C7VILList(void* l) { ev.push_back(tag("Len", off_of(l))); return list_length; }
extern "C" void SetModelView__8VIRasterRC10VIMatrix44(void*, void* m) { ev.push_back(tag("MV", off_of(m))); }
extern "C" void SetMaterial__8VIRasteri(void*, int h) { ev.push_back(tag("Mat", h)); }
extern "C" void BeginTriStrip__8VIRaster(void*) { ev.push_back("B"); }
extern "C" void EndTriStrip__8VIRaster(void*) { ev.push_back("E"); }
extern "C" void Color__8VIRasterRC10VIColor32F(void*, void* c) { ev.push_back(tag("C", off_of(c))); }
extern "C" void Vertex__8VIRasterRC7VIVect3(void*, void* v) { ev.push_back(tag("V", off_of(v))); }

static void build(int n, float age) {
    std::memset(arena, 0, sizeof arena);
    const unsigned trail = 0x100;
    put(trail + 28, 0x1000);
    putf(trail + 24, 2.0f);
    for (int i = 0; i < n; ++i) {
        unsigned a = 0x1000 + 0x100 * i;
        put(a, i + 1 < n ? a + 0x100 : 0x1f00);
        putf(a + 72, age * i);
    }
    list_length = n;
    ev.clear();
}

// reference model of one segment's calls
static void model_node(std::vector<std::string>& w, unsigned a, bool fwd) {
    w.push_back(tag("C", a + (fwd ? 40 : 56))); w.push_back(tag("V", a + (fwd ? 16 : 28)));
    w.push_back(tag("C", a + (fwd ? 56 : 40))); w.push_back(tag("V", a + (fwd ? 28 : 16)));
}

int main() {
    const unsigned trail = 0x100;
    char* raster = reinterpret_cast<char*>(0);
    // fewer than two segments: only the length query
    build(1, 0.5f);
    list_length = 1;
    if (Render__9VITrailFxR8VIRasterR8VICamera(arena + trail, raster, arena + 0x800) != 0) return 1;
    if (ev.size() != 1 || ev[0] != tag("Len", trail + 28)) return 2;
    // 13 segments: each strip holds 12, so the first pass restarts on node 11
    build(13, 0.25f);
    Render__9VITrailFxR8VIRasterR8VICamera(arena + trail, raster, arena + 0x800);
    std::vector<std::string> w = {tag("Len", trail + 28), tag("MV", 0x800 + 48), tag("Mat", 7), "B"};
    for (int pass = 0; pass < 2; ++pass) {
        bool fwd = pass == 0;
        for (int i = 0; i < 12; ++i) model_node(w, 0x1000 + 0x100 * i, fwd);
        w.push_back("E"); w.push_back("B"); model_node(w, 0x1000 + 0x100 * 11, fwd);
        model_node(w, 0x1000 + 0x100 * 12, fwd);
        w.push_back("E");
        if (pass == 0) w.push_back("B");
    }
    if (ev != w) return 3;
    // alpha: age/lifetime = 0.5 * i / 2 ...: node 12 age 3.0 -> 1 - 1.5 < 0 -> 0; node 4 age 1.0 -> 0.5; node 0 -> 1
    if (getf(0x1000 + 68) != 1.0f || getf(0x1000 + 52) != 1.0f) return 4;
    if (getf(0x1400 + 68) != 0.5f || getf(0x1400 + 52) != 0.5f) return 5;
    if (getf(0x1c00 + 68) != 0.0f) return 6;
    // exactly 12 segments: the strip ends on the last node with no restart
    build(12, 0.0f);
    Render__9VITrailFxR8VIRasterR8VICamera(arena + trail, raster, arena + 0x800);
    w = {tag("Len", trail + 28), tag("MV", 0x800 + 48), tag("Mat", 7), "B"};
    for (int pass = 0; pass < 2; ++pass) {
        for (int i = 0; i < 12; ++i) model_node(w, 0x1000 + 0x100 * i, pass == 0);
        w.push_back("E");
        if (pass == 0) w.push_back("B");
    }
    if (ev != w) return 7;
    // two segments: one short strip per pass
    build(2, 1.0f);
    Render__9VITrailFxR8VIRasterR8VICamera(arena + trail, raster, arena + 0x800);
    w = {tag("Len", trail + 28), tag("MV", 0x800 + 48), tag("Mat", 7), "B"};
    for (int pass = 0; pass < 2; ++pass) {
        for (int i = 0; i < 2; ++i) model_node(w, 0x1000 + 0x100 * i, pass == 0);
        w.push_back("E");
        if (pass == 0) w.push_back("B");
    }
    if (ev != w) return 8;
    return 0;
}
