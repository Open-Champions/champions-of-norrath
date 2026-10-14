#define NORRATH_CTORS_LAVA_WRAITH_HOST 1
#include "../wraith.cpp"
#include "../../ctors-lava-wraith-detail/host_support.hpp"
using namespace hs;
unsigned char norrath_actor_view_4844a4[4], norrath_ctors_lava_wraith_view_42bcb8[4], norrath_ctors_lava_wraith_view_44aea0[4];
unsigned char norrath_ctors_lava_wraith_view_42bd18[64], norrath_ctors_lava_wraith_view_4852a8[4];
static unsigned char* obj;
static unsigned char model[96], header[8];
static unsigned model_low, rand_value;
extern "C" int norrath_game_object_e_rand(void*) { event("rand"); return (int)rand_value; }
extern "C" unsigned norrath_creature_construct(void* object, const void* point, short type, void* map, float scale, char** arguments) {
    event("Creature type=%d map=%d scale=%g args=%d", type, map == norrath_ctors_lava_wraith_view_42bcb8, scale, arguments != nullptr);
    obj = (unsigned char*)object; std::memcpy(obj + 16, point, 12);
    put(obj + 36, model_low, 4); put(obj + 160, 0x1, 4); put(obj + 164, 0x2000, 4);
    return 1;
}
extern "C" void norrath_creature_set_parameters(void*, float a, float b, float c, float d, float e, int flag) { event("params %08x %08x %08x %g %g %d", fbits(a), fbits(b), fbits(c), d, e, flag); }
extern "C" void norrath_actor_animation_initialize(void* s) { event("animInit %d", int((unsigned char*)s - obj)); }
extern "C" void norrath_actor_animation_add(void* s, void* h, int f) { event("animAdd %d %d %d", int((unsigned char*)s - obj), h == header, f); }
extern "C" unsigned norrath_actor_add_slow_run_list(void*) { event("slow run"); return 3; }
extern "C" void norrath_creature_set_anim_event_flags(void*, int a, float b, int c, float d, int e, float f, int g, float h, int i) {
    event("events %d %08x %d %g %d %g %d %g %d", a, fbits(b), c, d, e, f, g, h, i);
}
extern "C" void* norrath_list_oneway_link(void* head, void* object_head, void* value) {
    event("link %d %d %d count=%u", int((unsigned char*)head - norrath_ctors_lava_wraith_view_42bd18), object_head == obj, value == obj,
          word_at(norrath_ctors_lava_wraith_view_4852a8));
    return nullptr;
}
extern "C" unsigned norrath_creature_finish_construction(void*) { event("finish count=%u", word_at(norrath_ctors_lava_wraith_view_4852a8)); return 0x77; }
static void run_case(unsigned rnd, unsigned count) {
    calls.clear(); rand_value = rnd;
    Image<1000> o, want;
    unsigned char pt[12] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    char* args[] = {nullptr};
    norrath_ctors_lava_wraith::host_note(model); norrath_ctors_lava_wraith::host_note(header);
    model_low = low(model);
    put(model + 76, low(header), 4);
    put(norrath_actor_view_4844a4, 0xcafef00du, 4);
    put(norrath_ctors_lava_wraith_view_4852a8, count, 4);
    const unsigned r = norrath_construct_Wraith(o.b, pt, (short)0x8003, args);
    const float scale = (float)(int)(rnd % 14u + 4u);
    char b[160];
    std::vector<std::string> w{"rand"};
    std::snprintf(b, sizeof b, "Creature type=-32765 map=1 scale=%g args=1", scale); w.push_back(b);
    w.push_back("params 3fb33333 3fb33333 3fa66666 1 1 1");
    w.push_back("animInit 288"); w.push_back("animAdd 288 1 1"); w.push_back("slow run");
    w.push_back("events 3 3e999999 1 0 0 0 0 0 0"); w.push_back("events 5 3ecccccc 1 0 0 0 0 0 0");
    std::snprintf(b, sizeof b, "link %u 1 1 count=%u", count * 4, count); w.push_back(b);
    std::snprintf(b, sizeof b, "finish count=%u", count + 1); w.push_back(b);
    expect_log(w, "wraith");
    for (int i = 0; i < 12; ++i) want.b[16 + i] = pt[i];
    want.w(36, model_low); want.w(160, 0x1 | 0x00200000u); want.w(164, 0x2000 | 4u);
    want.w(40, 20, 1); want.w(176, fbits(140.0f)); want.w(168, 0x3f999999u); want.w(28, fbits(18.0f));
    want.w(960, 100, 1); want.w(968, 0); want.w(72, low(norrath_ctors_lava_wraith_view_44aea0)); want.w(248, 0xcafef00du);
    want.w(192, 0); want.w(100, fbits(1.0f)); want.w(96, fbits(1.0f)); want.w(104, 0x3e999999u);
    want.w(236, 0); want.w(572, 0, 2); want.w(788, fbits(600.0f)); want.w(172, 16, 1);
    expect_image(o, want, "wraith image");
    expect(word_at(norrath_ctors_lava_wraith_view_4852a8) == count + 1, "count incremented");
    expect(r == 0x77u, "wraith return");
}
int main() {
    run_case(100, 0); run_case(0xfffffff1u, 3); run_case(13, 7);
    return finish("wraith constructor");
}
