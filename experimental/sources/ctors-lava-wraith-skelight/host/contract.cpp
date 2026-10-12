#define NORRATH_CTORS_LAVA_WRAITH_HOST 1
#include "../skelight.cpp"
#include "../../ctors-lava-wraith-detail/host_support.hpp"
using namespace hs;
unsigned char norrath_actor_view_4844a4[4], norrath_ctors_lava_wraith_view_42dbf0[4], norrath_ctors_lava_wraith_view_44fdf8[4];
unsigned char norrath_creature_view_484cac[4];
static unsigned char* obj;
static unsigned char model[160], header[8], chunk_src[8];
static unsigned model_low, rand_value;
extern "C" int norrath_game_object_e_rand(void*) { event("rand"); return (int)rand_value; }
extern "C" unsigned norrath_creature_construct(void* object, const void* point, short type, void* map, float scale, char** arguments) {
    event("Creature type=%d map=%d scale=%g args=%d", type, map == norrath_ctors_lava_wraith_view_42dbf0, scale, arguments != nullptr);
    obj = (unsigned char*)object; std::memcpy(obj + 16, point, 12);
    put(obj + 36, model_low, 4); put(obj + 160, 0x21, 4); put(obj + 164, 0x4000, 4);
    return 1;
}
extern "C" void norrath_skelight_init_weapon(void*) { event("initWeapon b208=%u", word_at(obj + 208)); }
extern "C" void norrath_creature_set_parameters(void*, float a, float b, float c, float d, float e, int flag) {
    event("params %g %g %g %g %g %d g=%08x b1104=%u", a, b, c, d, e, flag, word_at(norrath_creature_view_484cac), word_at(obj + 1104));
}
extern "C" unsigned long long norrath_actor_model_active_chunks(void* m, int n) { event("chunks %d %d b244=%u", m == chunk_src, n, word_at(obj + 244)); return 0x0102030405060708ull; }
extern "C" void norrath_actor_animation_initialize(void* s) { event("animInit %d", int((unsigned char*)s - obj)); }
extern "C" void norrath_actor_animation_add(void* s, void* h, int fl) { event("animAdd %d %d %d", int((unsigned char*)s - obj), h == header, fl); }
extern "C" void norrath_skelight_set_state(void*, int s) { event("setState %d", s); }
extern "C" unsigned norrath_actor_add_run_list(void*) { event("run"); return 3; }
extern "C" void norrath_creature_set_anim_event_flags(void*, int a, float b, int c, float d, int e, float ff, int g, float h, int i) { event("events %d %08x %d %g %d %g %d %g %d", a, fbits(b), c, d, e, ff, g, h, i); }
extern "C" unsigned norrath_creature_finish_construction(void*) { event("finish b236=%u", word_at(obj + 236)); return 0x77; }
static void run_case(signed char flag, int value, unsigned rnd) {
    calls.clear(); rand_value = rnd;
    Image<1112> o, want;
    unsigned char pt[12] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    char* args[] = {nullptr};
    norrath_ctors_lava_wraith::host_note(model); norrath_ctors_lava_wraith::host_note(header); norrath_ctors_lava_wraith::host_note(chunk_src);
    model_low = low(model);
    put(model + 60, low(header), 4); put(model + 12, low(chunk_src), 4);
    put(norrath_actor_view_4844a4, 0xcafef00du, 4);
    put(norrath_creature_view_484cac, 0x40490fdbu, 4);
    const unsigned r = norrath_construct_Skelight(o.b, pt, (short)44, args, flag, value);
    const unsigned b208 = flag ? rnd % 3u + 1u : 0x5a5a5a5au;
    char b[160];
    std::vector<std::string> w{"Creature type=44 map=1 scale=10 args=1"};
    if (flag) w.push_back("rand");
    std::snprintf(b, sizeof b, "initWeapon b208=%u", b208); w.push_back(b);
    std::snprintf(b, sizeof b, "params 1 1 1 1 1 1 g=%08x b1104=%u", fbits((float)value), (unsigned)value); w.push_back(b);
    w.push_back("chunks 1 0 b244=0"); w.push_back("animInit 288"); w.push_back("animAdd 288 1 12"); w.push_back("setState 13"); w.push_back("run");
    w.push_back("events 3 3e4ccccc 1 0 0 0 0 0 0"); w.push_back("events 4 3e4ccccc 1 1 1 0 0 0 0"); w.push_back("finish b236=0");
    expect_log(w, "skelight");
    expect(word_at(norrath_creature_view_484cac) == 0x40490fdbu, "global restored");
    for (int i = 0; i < 12; ++i) want.b[16 + i] = pt[i];
    want.w(36, model_low); want.w(72, low(norrath_ctors_lava_wraith_view_44fdf8));
    for (unsigned i = 0; i < 4; ++i) want.w(1092 - 8 * i, 0xffff, 2);
    want.w(1064, 0xff, 1); want.w(1065, 0xff, 1);
    for (unsigned i = 0; i < 4; ++i) { want.w(1048 + 2 * i, 0, 2); want.w(1056 + 2 * i, 0, 2); }
    want.w(40, 20, 1); want.w(160, 0x21 | 0x10000u); want.w(164, 0x4000); want.w(168, 0x3f999999u); want.w(28, fbits(15.0f));
    want.w(248, 0xcafef00du); want.w(960, 4); want.w(176, 0); want.w(192, 0); want.w(236, 1); want.w(572, 0, 2);
    want.w(100, fbits(1.0f)); want.w(96, fbits(1.0f)); if (flag) want.w(208, b208);
    want.w(1104, (unsigned)value); want.w(244, 0); want.w(552, 0x05060708u); want.w(556, 0x01020304u); want.w(792, 0xffff, 2);
    expect_image(o, want, "skelight image");
    expect(r == 0x77u, "skelight return");
}
int main() { run_case(1, 7, 100); run_case(0, -3, 5); run_case(-1, 0x7fffffff, 0xfffffff0u); run_case(2, 0, 2); return finish("skelight constructor"); }
