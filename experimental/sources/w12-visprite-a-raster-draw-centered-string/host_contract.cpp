#define NORRATH_HOST_CONTRACT 1
#include "draw_centered_string.cpp"
#include <cstring>
#include <string>
#include <vector>

alignas(8) static unsigned char arena[1 << 16];
extern "C" char* norrath_host_translate(unsigned address) { return reinterpret_cast<char*>(arena) + address; }
static void put(unsigned off, unsigned v) { std::memcpy(arena + off, &v, 4); }
static void putf(unsigned off, float v) { std::memcpy(arena + off, &v, 4); }
alignas(16) static unsigned char raster[20000];
static void rputf(unsigned off, float v) { std::memcpy(raster + off, &v, 4); }
static void rputi(unsigned off, int v) { std::memcpy(raster + off, &v, 4); }

struct Ev { std::string name; float a, b, c, d, e; };
static std::vector<Ev> ev;
static int cache_count, add_result;
static void push(const char* n, float a = 0, float b = 0, float c = 0, float d = 0, float e = 0) { ev.push_back({n, a, b, c, d, e}); }
static float rf(unsigned off) { float v; std::memcpy(&v, raster + off, 4); return v; }

extern "C" int Cache__7VIRFontR8VIRasterPCciPi(void* font, void*, const char* text, int limit, int* out) {
    push("cache", reinterpret_cast<unsigned char*>(font) == arena + 0x1100, limit, std::strcmp(text, "hi") == 0);
    for (int i = 0; i < cache_count; ++i) out[i] = i;
    return cache_count;
}
extern "C" int AddMaterial__8VIRasteriPP10_sceDmaTag(void*, int material, void* slot) {
    push("addmat", material, slot == raster + 19492);
    return add_result;
}
extern "C" void __ml__FRC7VIVect3RC10VIMatrix44(void* res, const void* v, const void* m) {
    const float* in = static_cast<const float*>(v);
    push("ml3", in[0], in[1], in[2], reinterpret_cast<unsigned long>(m));
    float out[4] = {1, 2, 3, 1};
    std::memcpy(res, out, 16);
}
extern "C" void __ml__FRC7VIVect4RC10VIMatrix44(void* res, const void* v, const void* m) {
    const float* in = static_cast<const float*>(v);
    push("ml4", in[0], in[1], in[2], in[3], reinterpret_cast<unsigned long>(m));
    float out[4] = {0.5f, 0.25f, 4.0f, 2.0f};
    std::memcpy(res, out, 16);
}
extern "C" void Store2D__8VIRaster(void*) { push("store2d"); }
extern "C" void Restore2D__8VIRaster(void*) { push("restore2d"); }
extern "C" void Identity__10VIMatrix44(void* m) { push("ident", reinterpret_cast<unsigned long>(m)); }
extern "C" void UploadRasterMicro__8VIRasterPP10_sceDmaTag(void*, void*) { push("micro"); }
extern "C" void CalcMatrices__8VIRaster(void*) { push("calc"); }
extern "C" void UploadMatrices__8VIRasterPP10_sceDmaTag(void*, void*) { push("upmat"); }
extern "C" void Vertex__8VIRasterRC7VIVect3(void*, const void* v) {
    const float* in = static_cast<const float*>(v);
    push("vertex", in[0], in[1], in[2], rf(12), rf(16));
}
extern "C" void EndTriFan__8VIRaster(void*) {
    unsigned a, b;
    std::memcpy(&a, raster + 56, 4); std::memcpy(&b, raster + 52, 4);
    push("endfan", a, b);
}

static bool is(const Ev& e, const char* n) { return e.name == n; }

int main() {
    std::memset(arena, 0, sizeof arena);
    std::memset(raster, 0, sizeof raster);
    // font table at 0x800: entry 2 holds the font pointer 0x1100
    put(0x800 + 12 * 2, 0x1100);
    put(0x1100 + 16, 9); put(0x1100 + 20, 16); put(0x1100 + 64, 0x1400); put(0x1100 + 68, 0x1800);
    // glyph records (12 bytes): halfword at +8 selects the rectangle, byte at +10 the advance
    arena[0x1400 + 8] = 0; arena[0x1400 + 10] = 7;
    arena[0x1400 + 12 + 8] = 1; arena[0x1400 + 12 + 10] = 9;
    // rectangles (56 bytes) with texcoords at +24..+52
    for (int rct = 0; rct < 2; ++rct)
        for (int k = 0; k < 8; ++k) putf(0x1800 + 56 * rct + 24 + 4 * k, 10.0f * rct + k);
    put(0, 0);
    std::memcpy(raster + 19516, (const unsigned[]){0x800}, 4);
    rputf(17732, 1.0f); rputf(17736, 2.0f); rputf(17740, 2.0f); rputf(17744, 3.0f);
    rputi(17748, 640); rputi(17752, 448); rputi(19532, 0);
    float pos[3] = {5, 6, 7};
    unsigned color[4] = {1, 2, 3, 4};
    cache_count = 2; add_result = 0;
    if (DrawCenteredString__8VIRasteriRC7VIVect3RC10VIColor32FPCc(raster, 2, pos, color, "hi") != 0) return 1;
    // set-up events
    if (ev.size() != 7 + 2 * 8 + 1) return 2;
    if (!is(ev[0], "cache") || ev[0].a != 1 || ev[0].b != 256 || ev[0].c != 1) return 3;
    if (!is(ev[1], "addmat") || ev[1].a != 9 || ev[1].b != 1) return 4;
    if (!is(ev[2], "ml3") || ev[2].a != 5 || ev[2].b != 6 || ev[2].c != 7) return 5;
    if (!is(ev[3], "ml4") || ev[3].a != 1 || ev[3].b != 2 || ev[3].c != 3 || ev[3].d != 1) return 6;
    if (!is(ev[4], "store2d") || !is(ev[5], "ident") || !is(ev[6], "ident")) return 7;
    if (ev[5].a != static_cast<float>(0x70000000) || ev[6].a != static_cast<float>(0x70000040)) return 8;
    // first glyph: micro (word is not 1), calc, upmat, four vertices, endfan
    const char* order[] = {"micro", "calc", "upmat", "vertex", "vertex", "vertex", "vertex", "endfan"};
    for (int g = 0; g < 2; ++g)
        for (int k = 0; k < 8; ++k)
            if (!is(ev[7 + 8 * g + k], order[k])) return 9;
    // exact vertex coordinates of the first glyph: centre (400, 196), pen (391, 188), bottom 204
    const Ev* v = &ev[10];
    if (!is(v[0], "vertex") || v[0].a != 781.0f || v[0].b != -559.5f || v[0].c != 2.0f || v[0].d != 0.0f || v[0].e != 1.0f) return 10;
    if (v[1].a != 795.0f || v[1].b != -559.5f || v[1].d != 2.0f || v[1].e != 3.0f) return 11;
    if (v[2].a != 795.0f || v[2].b != -607.5f || v[2].d != 4.0f || v[2].e != 5.0f) return 12;
    if (v[3].a != 781.0f || v[3].b != -607.5f || v[3].d != 6.0f || v[3].e != 7.0f) return 13;
    // second glyph starts at 399 (right edge 398 plus one) with rectangle 1
    const Ev* w = &ev[10 + 8];
    if (!is(w[0], "vertex") || w[0].a != (399 - 1 + 0.5f) * 2 || w[0].d != 10.0f + 0 || w[0].e != 11.0f) return 14;
    if (w[1].a != (408 - 1 + 0.5f) * 2) return 15;
    if (!is(ev.back(), "restore2d")) return 16;
    // colour copied to raster + 32; stage words
    unsigned got[4]; std::memcpy(got, raster + 32, 16);
    if (std::memcmp(got, color, 16) != 0) return 17;
    int stage; std::memcpy(&stage, raster + 17884, 4);
    int flag; std::memcpy(&flag, raster + 17888, 4);
    if (stage != 2 || flag != 1) return 18;
    // failures and empty strings
    ev.clear(); cache_count = -1;
    if (DrawCenteredString__8VIRasteriRC7VIVect3RC10VIColor32FPCc(raster, 2, pos, color, "hi") != -1 || ev.size() != 1) return 19;
    ev.clear(); cache_count = 2; add_result = -4;
    if (DrawCenteredString__8VIRasteriRC7VIVect3RC10VIColor32FPCc(raster, 2, pos, color, "hi") != -1 || ev.size() != 2) return 20;
    ev.clear(); cache_count = 0; add_result = 0;
    if (DrawCenteredString__8VIRasteriRC7VIVect3RC10VIColor32FPCc(raster, 2, pos, color, "hi") != 0) return 21;
    if (ev.size() != 8 || !is(ev.back(), "restore2d")) return 22;
    return 0;
}
