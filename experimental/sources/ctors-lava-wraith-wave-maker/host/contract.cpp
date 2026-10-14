#define NORRATH_CTORS_LAVA_WRAITH_HOST 1
#include "../wave_maker.cpp"
#include "../../ctors-lava-wraith-detail/host_support.hpp"
#include <cmath>
using namespace hs;
unsigned char norrath_ctors_lava_wraith_view_44c7a0[4], norrath_ctors_lava_wraith_view_483e28[8] = "wave";
unsigned char norrath_factory_world_pointer_word[4];
alignas(8) unsigned char norrath_ctors_lava_wraith_view_601fd0[512], norrath_ctors_lava_wraith_view_601030[4096];
static unsigned char* obj;
alignas(8) static unsigned char line[64], points[64 * 6];
static float heights[8]; static unsigned height_i; static float water_level;
static const void* const world = reinterpret_cast<const void*>(static_cast<std::uintptr_t>(0x20004444u));
static float f(unsigned u) { float x; std::memcpy(&x, &u, 4); return x; }
static void setfl(unsigned char* p, float x) { std::memcpy(p, &x, 4); }
extern "C" unsigned norrath_game_object_construct(void* object, const void* pt, void* map, char** arguments) {
    event("GameObject map=%d args=%d", map != nullptr, arguments != nullptr);
    obj = (unsigned char*)object; std::memcpy(obj + 16, pt, 12); return 1;
}
extern "C" unsigned norrath_actor_add_run_list(void*) { event("run b12=%08x b72=%08x", word_at(obj + 12), word_at(obj + 72)); return 3; }
extern "C" int norrath_game_line_get(char* name) { event("lineGet %s", name); return 7; }
extern "C" void* norrath_game_line_get_ptr(int h) { event("lineGetPtr %d", h); return line; }
extern "C" float norrath_world_find_height(void* w, const void* p, float radius) {
    event("height %d %g", w == world, radius);
    float h = heights[height_i++ % 8]; return h;
}
extern "C" int norrath_world_check_for_water(void* w, const void* p, float radius, float* out) {
    event("water %d %08x,%08x,%08x r=%g", w == world, word_at((const unsigned char*)p), word_at((const unsigned char*)p + 4), word_at((const unsigned char*)p + 8), radius);
    *out = water_level; return 1;
}
extern "C" void norrath_wave_maker_get_wavelet_mat(void*, int index, float fraction, void* m) {
    event("wavelet %d %08x", index, fbits(fraction));
    float* mm = (float*)m; for (int k = 0; k < 9; ++k) mm[k] = 1.0f + 0.25f * (float)k;
}
extern "C" void norrath_mat_transform(void* out, void* m, void* in) {
    const float* a = (const float*)m; const float* v = (const float*)in; float* o = (float*)out;
    o[0] = a[0] * v[0] + a[1] * v[1] + a[2] * v[2]; o[1] = a[3] * v[0] + a[4] * v[1] + a[5] * v[2]; o[2] = a[6] * v[0] + a[7] * v[1] + a[8] * v[2];
}
static void run_case(int nsegs, unsigned closed, int scenario) {
    calls.clear(); height_i = 0;
    water_level = scenario == 2 ? 50.0f : 5.0f;
    const float seq_a[8] = {2.0f, 30.0f, 30.0f, 30.0f, 30.0f, 30.0f, 30.0f, 30.0f};
    const float seq_b[8] = {-10.0f, -10.0f, 1.0f, 18.0f, 18.0f, 0.0f, 3.0f, 9.0f};
    const float seq_c[8] = {-100.0f, -100.0f, -100.0f, -100.0f, -100.0f, -100.0f, -100.0f, -100.0f};
    for (int i = 0; i < 8; ++i) heights[i] = scenario == 0 ? seq_a[i] : scenario == 1 ? seq_b[i] : seq_c[i];
    std::memset(norrath_ctors_lava_wraith_view_601fd0, 0xee, 512);
    std::memset(norrath_ctors_lava_wraith_view_601030, 0x11, 4096);
    std::memset(line, 0, sizeof line); std::memset(points, 0, sizeof points);
    put(line + 4, nsegs, 4); put(line + 8, closed, 4);
    namespace nc = norrath_ctors_lava_wraith;
    nc::host_note(points); put(line + 20, low(points), 4);
    for (int i = 0; i < 6; ++i) {
        unsigned char* r = points + 64 * i;
        setfl(r + 0, 1.0f * i); setfl(r + 4, 2.0f); setfl(r + 8, 3.0f);
        setfl(r + 12, 0.5f); setfl(r + 16, -0.25f); setfl(r + 20, 1.5f);
        setfl(r + 36, 3.0f + i); setfl(r + 40, 4.0f); setfl(r + 44, -2.0f - i);
        setfl(r + 48, i == 1 ? 34.0f : i == 3 ? -4.0f : 40.0f);
    }
    put(norrath_factory_world_pointer_word, 0x20004444u, 4);
    Image<1920> o, want;
    unsigned char pt[12] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    char* args[] = {nullptr};
    o.w(12, 0x100);
    const unsigned r = norrath_construct_WaveMaker(o.b, pt, (short)7, args);
    // ------------------------------------------------ model
    std::vector<std::string> w; char b[256];
    w.push_back("GameObject map=0 args=1");
    std::snprintf(b, sizeof b, "run b12=%08x b72=%08x", 0x100u, low(norrath_ctors_lava_wraith_view_44c7a0)); w.push_back(b);
    w.push_back("lineGet wave"); w.push_back("lineGetPtr 7");
    unsigned hi = 0;
    auto height = [&](float radius) { std::snprintf(b, sizeof b, "height 1 %g", radius); w.push_back(b); return heights[hi++ % 8]; };
    const float first[3] = {0.0f, 2.0f, 3.0f};
    const float seed_z = height(32.0f);
    std::snprintf(b, sizeof b, "water 1 %08x,%08x,%08x r=32", fbits(first[0]), fbits(first[1]), fbits(seed_z)); w.push_back(b);
    for (int i = 0; i < nsegs; ++i) {
        const float dx = 3.0f + i, dy = 4.0f, dz = -2.0f - i;
        const float cx = dy * 1.0f - dz * 0.0f, cy = dz * 0.0f - dx * 1.0f, cz = dx * 0.0f - dy * 0.0f;
        float sq = cx * cx; sq = sq + cy * cy; sq = sq + cz * cz;
        const float inv = 1.0f / std::sqrt(sq);
        const unsigned char* slot = o.b + 76 + 36 * i;
        (void)slot;
        want.w(76 + 36 * i, fbits(cx * inv)); want.w(80 + 36 * i, fbits(cy * inv)); want.w(84 + 36 * i, fbits(cz * inv));
        want.w(88 + 36 * i, fbits(-dx)); want.w(92 + 36 * i, fbits(-dy)); want.w(96 + 36 * i, fbits(-dz));
        want.w(100 + 36 * i, fbits(0.0f)); want.w(104 + 36 * i, fbits(0.0f)); want.w(108 + 36 * i, fbits(1.0f));
    }
    const int segments = nsegs - (closed == 0 ? 1 : 0);
    unsigned char mask[512]; unsigned char flags[4096];
    std::memset(mask, 0, 504); std::memset(mask + 504, 0xee, 8); std::memset(flags, 0x11, 4096);
    unsigned c = 0;
    for (int i = 0; i < segments; ++i) {
        float px = 1.0f * i, py = 2.0f, pz = 3.0f;
        const float sx = 0.5f * 17.0f, sy = -0.25f * 17.0f, sz = 1.5f * 17.0f;
        const float len = i == 1 ? 34.0f : i == 3 ? -4.0f : 40.0f;
        float t = 0.0f;
        if (!(0.0f <= len)) continue;
        do {
            std::snprintf(b, sizeof b, "wavelet %d %08x", i, fbits(t / len)); w.push_back(b);
            flags[c] = 0;
            float qx = px, qy = py, qz = pz;
            for (;;) {
                const float h = height(8.0f);
                if (water_level - h < 4.0f) { if (12.0f < h - water_level) *(unsigned*)(mask + (c >> 5) * 4) |= 1u << (c & 31); break; }
                qx = qx + (-30.0f); qy = qy + (-48.0f); qz = qz + (-66.0f);
                unsigned taken = (unsigned)flags[c] + 1; flags[c] = (unsigned char)taken;
                if ((taken & 0xff) >= 255) break;
            }
            if (flags[c] == 255) flags[c] = 0;
            px = px + sx; t = t + 17.0f; ++c; py = py + sy; pz = pz + sz;
        } while (t <= len);
    }
    expect_log(w, "wave maker");
    for (int i = 0; i < 12; ++i) want.b[16 + i] = pt[i];
    want.w(12, 0x101); want.w(72, low(norrath_ctors_lava_wraith_view_44c7a0));
    want.w(1888, fbits(water_level));
    want.w(1892, low(line)); 
    const float sc = (float)(int)c * f(0x3dcccccc);
    const int rounded = sc >= 0.0f ? (int)(sc + 0.5f) : (int)(sc - 0.5f);
    want.w(1884, (unsigned)rounded); want.w(1878, 1192, 2); want.w(1880, 0);
    expect_image(o, want, "wave maker image");
    expect(std::memcmp(norrath_ctors_lava_wraith_view_601fd0, mask, 512) == 0, "visited bit buffer");
    expect(std::memcmp(norrath_ctors_lava_wraith_view_601030, flags, 4096) == 0, "step counters");
    expect(r == 1192u, "wave maker return");
}
int main() {
    for (int ns : {0, 1, 2, 3, 5}) for (unsigned closed : {0u, 1u}) for (int sc = 0; sc < 3; ++sc) run_case(ns, closed, sc);
    return finish("wave maker constructor");
}
