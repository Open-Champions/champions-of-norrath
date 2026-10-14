#define NORRATH_CTORS_LAVA_WRAITH_HOST 1
#include "../ulthork.cpp"
#include "../../ctors-lava-wraith-detail/host_support.hpp"
using namespace hs;
unsigned char norrath_actor_view_4844a4[4], norrath_ctors_lava_wraith_view_42b478[4], norrath_ctors_lava_wraith_view_44dfe0[4];
static unsigned char* obj;
static unsigned char model[160], header[8], chunk_src[8];
static unsigned model_low, rands[3], rand_calls;
static float f(unsigned u) { float x; std::memcpy(&x, &u, 4); return x; }
extern "C" int norrath_game_object_e_rand(void*) { event("rand"); return (int)rands[rand_calls++]; }
extern "C" unsigned norrath_creature_construct(void* object, const void* point, short type, void* map, float scale, char** arguments) {
    event("Creature type=%d map=%d scale=%g args=%d", type, map == norrath_ctors_lava_wraith_view_42b478, scale, arguments != nullptr);
    obj = (unsigned char*)object; std::memcpy(obj + 16, point, 12);
    put(obj + 36, model_low, 4); put(obj + 160, 0x21, 4); put(obj + 164, 0x4000, 4);
    return 1;
}
extern "C" void norrath_creature_set_parameters(void*, float a, float b, float c, float d, float e, int flag) {
    event("params %g %08x %g %g %g %d b244=%u b960=%u", a, fbits(b), c, d, e, flag, word_at(obj + 244), word_at(obj + 960));
}
extern "C" unsigned long long norrath_actor_model_active_chunks(void* m, int n) { event("chunks %d %d", m == chunk_src, n); return 0x0102030405060708ull; }
extern "C" void norrath_actor_animation_initialize(void* s) { event("animInit %d", int((unsigned char*)s - obj)); }
extern "C" void norrath_actor_animation_add(void* s, void* h, int fl) { event("animAdd %d %d %d", int((unsigned char*)s - obj), h == header, fl); }
extern "C" unsigned norrath_actor_add_slow_run_list(void*) { event("slow run"); return 3; }
extern "C" void norrath_creature_set_anim_event_flags(void*, int a, float b, int c, float d, int e, float ff, int g, float h, int i) { event("events %d %08x %d %08x %d %g %d %g %d", a, fbits(b), c, fbits(d), e, ff, g, h, i); }
extern "C" unsigned norrath_creature_finish_construction(void*) { event("finish"); return 0x77; }
static void run_case(unsigned r0, unsigned r1, unsigned r2) {
    calls.clear(); rands[0] = r0; rands[1] = r1; rands[2] = r2; rand_calls = 0;
    Image<1112> o, want;
    unsigned char pt[12] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    char* args[] = {nullptr};
    norrath_ctors_lava_wraith::host_note(model); norrath_ctors_lava_wraith::host_note(header); norrath_ctors_lava_wraith::host_note(chunk_src);
    model_low = low(model);
    put(model + 116, low(header), 4); put(model + 12, low(chunk_src), 4);
    put(norrath_actor_view_4844a4, 0xcafef00du, 4);
    const unsigned r = norrath_construct_Ulthork(o.b, pt, (short)-3, args);
    const unsigned pick = r1 % 3u;
    char b[200];
    std::vector<std::string> w{"Creature type=-3 map=1 scale=10 args=1", "rand", "rand"};
    if (pick != 2) w.push_back("rand");
    std::snprintf(b, sizeof b, "params 8 3f999999 2 1 1 1 b244=%u b960=%u", pick == 2 ? 1u : 0u, pick == 2 ? 23u : 19u + 2 * (r2 & 1u)); w.push_back(b);
    std::snprintf(b, sizeof b, "chunks 1 %d", pick == 2 ? 1 : 0); w.push_back(b);
    w.push_back("animInit 288"); w.push_back("animAdd 288 1 1"); w.push_back("slow run");
    for (int n : {5, 6, 7}) { std::snprintf(b, sizeof b, "events %d 3e4ccccc 1 3ecccccc 2 0 0 0 0", n); w.push_back(b); }
    w.push_back("events 18 3e999999 1 00000000 0 0 0 0 0");
    w.push_back("finish");
    expect_log(w, "ulthork");
    for (int i = 0; i < 12; ++i) want.b[16 + i] = pt[i];
    want.w(36, model_low); want.w(72, low(norrath_ctors_lava_wraith_view_44dfe0));
    for (unsigned i = 0; i < 4; ++i) want.w(1092 - 8 * i, 0xffff, 2);
    want.w(1064, 0xff, 1); want.w(1065, 0xff, 1);
    for (unsigned i = 0; i < 4; ++i) { want.w(1048 + 2 * i, 0, 2); want.w(1056 + 2 * i, 0, 2); }
    want.w(40, 90, 1); want.w(160, 0x21 | 0x00040000u); want.w(164, 0x4000 | 2u);
    want.w(168, 0x3f999999u); want.w(248, 0xcafef00du); want.w(28, fbits(35.0f)); want.w(92, fbits(1.0f)); want.w(88, 0xbe999999u);
    for (unsigned off : {176u, 192u, 236u, 256u}) want.w(off, 0);
    want.w(572, 0, 2);
    float spread = (float)(int)r0 * f(0x37800000); spread = spread * f(0x3e999999); spread = spread + f(0x3f4ccccc);
    want.w(224, fbits(spread));
    want.w(1104, pick, 1);
    if (pick == 2) { want.w(156, 6, 1); want.w(960, 23); want.w(244, 1); } else { want.w(960, 19 + 2 * (r2 & 1u)); want.w(244, 0); }
    want.w(552, 0x05060708u); want.w(556, 0x01020304u);
    expect_image(o, want, "ulthork image");
    expect(r == 0x77u, "ulthork return");
}
int main() {
    run_case(100, 2, 0); run_case(0xfffffff0u, 4, 1); run_case(65536, 3, 6); run_case(7, 0xffffffffu, 1);
    return finish("ulthork constructor");
}
