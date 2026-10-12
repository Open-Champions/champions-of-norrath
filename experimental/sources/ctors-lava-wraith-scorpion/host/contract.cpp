#define NORRATH_CTORS_LAVA_WRAITH_HOST 1
#include "../scorpion.cpp"
#include "../../ctors-lava-wraith-detail/host_support.hpp"
using namespace hs;
unsigned char norrath_actor_view_4844a4[4], norrath_ctors_lava_wraith_view_42b330[4], norrath_ctors_lava_wraith_view_44b630[4];
static unsigned char* obj;
static unsigned char model[160], header[8], chunk_src[8];
static unsigned model_low, rands[2], rand_calls;
static float f(unsigned u) { float x; std::memcpy(&x, &u, 4); return x; }
extern "C" int norrath_game_object_e_rand(void*) { event("rand"); return (int)rands[rand_calls++ & 1]; }
extern "C" unsigned norrath_creature_construct(void* object, const void* point, short type, void* map, float scale, char** arguments) {
    event("Creature type=%d map=%d scale=%g args=%d", type, map == norrath_ctors_lava_wraith_view_42b330, scale, arguments != nullptr);
    obj = (unsigned char*)object; std::memcpy(obj + 16, point, 12);
    put(obj + 36, model_low, 4); put(obj + 160, 0x21, 4); put(obj + 164, 0x4000, 4); put(obj + 244, 0x77, 4);
    return 1;
}
extern "C" void norrath_creature_set_parameters(void*, float a, float b, float c, float d, float e, int flag) { event("params %08x %08x %g %g %g %d b244=%u", fbits(a), fbits(b), c, d, e, flag, word_at(obj + 244)); }
static unsigned long long dp_of(float x) { return ((unsigned long long)fbits(x) << 8) | 1ull; }
extern "C" unsigned long long norrath_entity_fptodp(float x) { event("fptodp %08x", fbits(x)); return dp_of(x); }
extern "C" unsigned long long norrath_softdouble_add_2fb5c0(unsigned long long l, unsigned long long r) { event("dpadd %llx %llx", l, r); return l ^ (r >> 3); }
static float to_f(unsigned long long v) { return (float)((v & 0xffffff) + 1) * 0.5f; }
extern "C" float norrath_softdouble_to_float_2fbc20(unsigned long long v) { event("dptofp %llx", v); return to_f(v); }
extern "C" unsigned long long norrath_actor_model_active_chunks(void* m, int n) { event("chunks %d %d", m == chunk_src, n); return 0x0102030405060708ull; }
extern "C" void norrath_actor_animation_initialize(void* s) { event("animInit %d", int((unsigned char*)s - obj)); }
extern "C" void norrath_actor_animation_add(void* s, void* h, int fl) { event("animAdd %d %d %d", int((unsigned char*)s - obj), h == header, fl); }
extern "C" unsigned norrath_actor_add_slow_run_list(void*) { event("slow run"); return 3; }
extern "C" void norrath_creature_set_anim_event_flags(void*, int a, float b, int c, float d, int e, float ff, int g, float h, int i) { event("events %d %08x %d %g %d %g %d %g %d", a, fbits(b), c, d, e, ff, g, h, i); }
extern "C" unsigned norrath_creature_finish_construction(void*) { event("finish"); return 0x77; }
static void run_case(unsigned r0, unsigned r1) {
    calls.clear(); rands[0] = r0; rands[1] = r1; rand_calls = 0;
    Image<1000> o, want;
    unsigned char pt[12] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    char* args[] = {nullptr};
    norrath_ctors_lava_wraith::host_note(model); norrath_ctors_lava_wraith::host_note(header);
    model_low = low(model);
    put(model + 132, low(header), 4); norrath_ctors_lava_wraith::host_note(chunk_src); put(model + 12, low(chunk_src), 4);
    put(norrath_actor_view_4844a4, 0xcafef00du, 4);
    const unsigned r = norrath_construct_Scorpion(o.b, pt, (short)5, args);
    const float step = (float)(int)(r0 % 6u) * f(0x3d4ccccc);
    const unsigned long long wide = dp_of(step);
    const unsigned long long sum = wide ^ (0x3fe8000000000000ull >> 3);
    const float factor = to_f(sum);
    char b[200];
    std::vector<std::string> w{"Creature type=5 map=1 scale=10 args=1"};
    w.push_back("params 3fb33333 3f999999 1 1 1 1 b244=119");
    w.push_back("rand");
    std::snprintf(b, sizeof b, "fptodp %08x", fbits(step)); w.push_back(b);
    std::snprintf(b, sizeof b, "dpadd %llx 3fe8000000000000", wide); w.push_back(b);
    std::snprintf(b, sizeof b, "dptofp %llx", sum); w.push_back(b);
    w.push_back("rand"); w.push_back("chunks 1 0"); w.push_back("animInit 288"); w.push_back("animAdd 288 1 1"); w.push_back("slow run");
    for (int n : {5, 6, 7, 8}) { std::snprintf(b, sizeof b, "events %d 3e999999 1 0 0 0 0 0 0", n); w.push_back(b); }
    for (int n : {11, 12}) { std::snprintf(b, sizeof b, "events %d 3f000000 1 0 0 0 0 0 0", n); w.push_back(b); }
    w.push_back("finish");
    expect_log(w, "scorpion");
    for (int i = 0; i < 12; ++i) want.b[16 + i] = pt[i];
    want.w(36, model_low); want.w(40, 70, 1); want.w(160, 0x21 | 0x1000u); want.w(164, 0x4000 | 0x80u);
    want.w(248, 0xcafef00du); want.w(168, 0x3f999999u); want.w(216, 1); want.w(72, low(norrath_ctors_lava_wraith_view_44b630));
    want.w(28, fbits(30.0f * factor)); want.w(172, 18, 1); want.w(176, 0); want.w(192, 0); want.w(236, 0); want.w(572, 0, 2); want.w(244, 0);
    want.w(96, 0x3ecccccc); want.w(224, fbits(factor)); want.w(960, r1 & 1, 1);
    want.w(552, 0x05060708u); want.w(556, 0x01020304u);
    expect_image(o, want, "scorpion image");
    expect(r == 0x77u, "scorpion return");
}
int main() {
    run_case(100, 3); run_case(5, 2); run_case(0xfffffff0u, 0xffffffffu);
    return finish("scorpion constructor");
}
