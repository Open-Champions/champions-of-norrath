#define NORRATH_CTORS_LAVA_WRAITH_HOST 1
#include "../vampire.cpp"
#include "../../ctors-lava-wraith-detail/host_support.hpp"
using namespace hs;
unsigned char norrath_actor_view_4844a4[4], norrath_ctors_lava_wraith_view_42d650[4], norrath_ctors_lava_wraith_view_44b080[4];
static unsigned char* obj;
static unsigned char model[160], header[8], chunk_src[8];
static unsigned model_low, rand_value;
extern "C" int norrath_game_object_e_rand(void*) { event("rand"); return (int)rand_value; }
extern "C" unsigned norrath_creature_construct(void* object, const void* point, short type, void* map, float scale, char** arguments) {
    event("Creature type=%d map=%d scale=%g args=%d", type, map == norrath_ctors_lava_wraith_view_42d650, scale, arguments != nullptr);
    obj = (unsigned char*)object; std::memcpy(obj + 16, point, 12);
    put(obj + 36, model_low, 4); put(obj + 160, 0x21, 4); put(obj + 164, 0x4000, 4); put(obj + 244, 0x100, 4);
    return 1;
}
extern "C" void norrath_creature_set_parameters(void*, float a, float b, float c, float d, float e, int flag) { event("params %08x %g %08x %g %g %d", fbits(a), b, fbits(c), d, e, flag); }
extern "C" void norrath_actor_animation_initialize(void* s) { event("animInit %d", int((unsigned char*)s - obj)); }
extern "C" void norrath_actor_animation_add(void* s, void* h, int fl) { event("animAdd %d %d %d", int((unsigned char*)s - obj), h == header, fl); }
extern "C" void norrath_actor_animation_randomize_time(void* s) { event("randomize %d", int((unsigned char*)s - obj)); }
extern "C" unsigned norrath_actor_add_slow_run_list(void*) { event("slow run"); return 3; }
extern "C" void norrath_creature_set_anim_event_flags(void*, int a, float b, int c, float d, int e, float ff, int g, float h, int i) { event("events %d %08x %d %g %d %g %d %g %d", a, fbits(b), c, d, e, ff, g, h, i); }
extern "C" unsigned long long norrath_actor_model_active_chunks(void* m, int n) { event("chunks %d %08x", m == chunk_src, n); return 0x0102030405060708ull; }
extern "C" unsigned norrath_creature_finish_construction(void*) { event("finish b552=%08x", word_at(obj + 552)); return 0x77; }
static void run_case(unsigned rnd) {
    calls.clear(); rand_value = rnd;
    Image<1000> o, want;
    unsigned char pt[12] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    char* args[] = {nullptr};
    norrath_ctors_lava_wraith::host_note(model); norrath_ctors_lava_wraith::host_note(header); norrath_ctors_lava_wraith::host_note(chunk_src);
    model_low = low(model);
    put(model + 124, low(header), 4); put(model + 12, low(chunk_src), 4);
    put(norrath_actor_view_4844a4, 0xcafef00du, 4);
    const unsigned r = norrath_construct_Vampire(o.b, pt, (short)-6, args);
    const unsigned mask = 0x100u | (1u << (rnd & 7u));
    char b[120];
    std::vector<std::string> w{"Creature type=-6 map=1 scale=10 args=1", "params 3f4ccccc 1 3f4ccccc 1 1 1", "animInit 288", "animAdd 288 1 1", "randomize 288", "slow run"};
    for (int n : {3, 4, 5, 5}) { std::snprintf(b, sizeof b, "events %d 3dcccccc 1 0 0 0 0 0 0", n); w.push_back(b); }
    w.push_back("rand");
    std::snprintf(b, sizeof b, "chunks 1 %08x", mask); w.push_back(b);
    w.push_back("finish b552=0506070" "8" + std::string()); w.back() = "finish b552=05060708";
    expect_log(w, "vampire");
    for (int i = 0; i < 12; ++i) want.b[16 + i] = pt[i];
    want.w(36, model_low); want.w(160, 0x21); want.w(164, 0x4000 | 4u); want.w(244, mask);
    want.w(248, 0xcafef00du); want.w(168, 0x3f333333u); want.w(28, fbits(18.0f)); want.w(88, 0xbe4cccccu); want.w(104, 0x3e999999u);
    want.w(100, fbits(1.0f)); want.w(96, fbits(1.0f)); want.w(72, low(norrath_ctors_lava_wraith_view_44b080)); want.w(172, 16, 1);
    want.w(176, 0); want.w(236, 0); want.w(572, 0, 2); want.w(552, 0x05060708u); want.w(556, 0x01020304u);
    expect_image(o, want, "vampire image");
    expect(r == 0x77u, "vampire return");
}
int main() { run_case(0); run_case(7); run_case(0xfffffffbu); run_case(13); return finish("vampire constructor"); }
