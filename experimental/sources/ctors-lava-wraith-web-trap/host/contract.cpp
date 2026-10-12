#define NORRATH_CTORS_LAVA_WRAITH_HOST 1
#include "../web_trap.cpp"
#include "../../ctors-lava-wraith-detail/host_support.hpp"
using namespace hs;
unsigned char norrath_ctors_lava_wraith_view_449c20[4], norrath_ctors_lava_wraith_view_485330[8], norrath_ctors_lava_wraith_view_470358[16];
unsigned char norrath_factory_world_pointer_word[4];
static unsigned char* obj;
static float scripted_t;
static unsigned char resource[8];
static float f(unsigned u) { float x; std::memcpy(&x, &u, 4); return x; }
extern "C" unsigned norrath_game_object_construct(void* object, const void* point, void* map, char** arguments) {
    event("GameObject map=%d args=%d", map != nullptr, arguments != nullptr);
    obj = (unsigned char*)object; std::memcpy(obj + 16, point, 12);
    put(obj + 84, fbits(1.0f), 4); put(obj + 88, fbits(2.0f), 4); put(obj + 92, fbits(3.0f), 4);
    return 1;
}
extern "C" float norrath_integer_sine(int a) { event("sin %d", a); return 0.25f; }
extern "C" float norrath_integer_cosine(int a) { event("cos %d", a); return -0.5f; }
extern "C" float norrath_world_hitscan(void* w, const void* from, const void* dir, float dist, int mode) {
    const unsigned char* fp = (const unsigned char*)from; const unsigned char* dp = (const unsigned char*)dir;
    event("hitscan w=%d from=%08x,%08x,%08x dir=%08x,%08x,%08x dist=%g mode=%d", w == (void*)(std::uintptr_t)0x20004444u,
          word_at(fp), word_at(fp + 4), word_at(fp + 8), word_at(dp), word_at(dp + 4), word_at(dp + 8), dist, mode);
    return scripted_t;
}
extern "C" unsigned norrath_object_add_to_delete_list(void*) { event("delete"); return 0x55; }
extern "C" void* norrath_lump_find_resource(char* a, char* b) {
    event("lump %d %d", a == (char*)norrath_ctors_lava_wraith_view_485330, b == (char*)norrath_ctors_lava_wraith_view_470358); return resource;
}
extern "C" unsigned norrath_actor_add_run_list(void*) { event("run"); return 0x66; }
static void run_case(float t) {
    calls.clear(); scripted_t = t;
    Image<256> o, want;
    unsigned char pt[12];
    put(pt, fbits(10.0f), 4); put(pt + 4, fbits(20.0f), 4); put(pt + 8, fbits(30.0f), 4);
    char* args[] = {nullptr};
    norrath_ctors_lava_wraith::host_note(resource);
    put(norrath_factory_world_pointer_word, 0x20004444u, 4);
    o.w(12, 0x100); want.w(12, 0x100);
    const unsigned r = norrath_construct_WebTrap(o.b, pt, (short)0x8123, args);
    char b[250];
    std::vector<std::string> w{"GameObject map=0 args=1", "sin -32477", "cos -32477"};
    std::snprintf(b, sizeof b, "hitscan w=1 from=%08x,%08x,%08x dir=%08x,%08x,%08x dist=1200 mode=0", fbits(10.0f), fbits(20.0f), fbits(54.0f),
                  fbits(0.25f * 500.0f), fbits(-0.5f * 500.0f), fbits(0.0f)); w.push_back(b);
    const bool hit = t > 0.0f && t <= 1.0f;
    if (!hit) w.push_back("delete"); else { w.push_back("lump 1 1"); w.push_back("run"); }
    expect_log(w, "web trap");
    expect(r == (hit ? 0x66u : 0x55u), "web trap return");
    for (int i = 0; i < 12; ++i) want.b[16 + i] = pt[i];
    want.w(72, low(norrath_ctors_lava_wraith_view_449c20)); want.w(76, 0); want.w(136, 0);
    want.w(84, fbits(1.0f)); want.w(88, fbits(2.0f)); want.w(92, fbits(3.0f));
    if (hit) {
        const float hx = 10.0f + 125.0f * t, hy = 20.0f + -250.0f * t, hz = 54.0f + 0.0f * t;
        const float nx = 1.0f + hx, ny = 2.0f + hy, nz = 3.0f + hz;
        want.w(84, fbits(nx)); want.w(88, fbits(ny)); want.w(92, fbits(nz));
        want.w(12, 0x101); want.w(152, 1); want.w(80, 0xffffffffu); want.w(148, 0);
        want.w(40, 0xff, 1); want.w(41, 0xff, 1); want.w(132, 50, 1); want.w(156, 0);
        const float mx = (10.0f + nx) * 0.5f, my = (20.0f + ny) * 0.5f, mz = (30.0f + nz) * 0.5f;
        want.w(108, fbits(mx)); want.w(112, fbits(my)); want.w(116, fbits(mz));
        want.w(120, fbits(mx)); want.w(124, fbits(my)); want.w(128, fbits(mz));
        want.w(140, low(resource)); want.w(144, 0);
    }
    expect_image(o, want, "web trap image");
    (void)f;
}
int main() {
    run_case(0.5f); run_case(1.0f); run_case(0.125f); run_case(0.0f); run_case(1.5f); run_case(-1.0f);
    return finish("web trap constructor");
}
