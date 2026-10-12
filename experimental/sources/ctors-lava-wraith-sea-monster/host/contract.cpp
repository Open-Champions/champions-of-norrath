#define NORRATH_CTORS_LAVA_WRAITH_HOST 1
#include "../sea_monster.cpp"
#include "../../ctors-lava-wraith-detail/host_support.hpp"
using namespace hs;
unsigned char norrath_actor_view_4844a4[4], norrath_ctors_lava_wraith_view_42b7e8[4], norrath_ctors_lava_wraith_view_44e2b0[4];
static unsigned char* obj;
static unsigned char model[96], header[8];
static unsigned model_low, flag_low;
extern "C" unsigned norrath_creature_construct(void* object, const void* point, short type, void* map, float scale, char** arguments) {
    event("Creature type=%d map=%d scale=%g args=%d", type, map == norrath_ctors_lava_wraith_view_42b7e8, scale, arguments != nullptr);
    obj = (unsigned char*)object; std::memcpy(obj + 16, point, 12);
    put(obj + 36, model_low, 4); put(obj + 160, flag_low, 4); put(obj + 164, 0x2000, 4);
    return 1;
}
extern "C" void norrath_creature_set_parameters(void*, float a, float b, float c, float d, float e, int flag) { event("params %g %08x %g %g %g %d", a, fbits(b), c, d, e, flag); }
extern "C" void norrath_actor_animation_initialize(void* s) { event("animInit %d", int((unsigned char*)s - obj)); }
extern "C" void norrath_actor_animation_add(void* s, void* h, int f) { event("animAdd %d %d %d", int((unsigned char*)s - obj), h == header, f); }
extern "C" unsigned norrath_actor_add_slow_run_list(void*) { event("slow run"); return 3; }
extern "C" void norrath_creature_set_anim_event_flags(void*, int a, float b, int c, float d, int e, float f, int g, float h, int i) {
    event("events %d %08x %d %g %d %g %d %g %d", a, fbits(b), c, d, e, f, g, h, i);
}
extern "C" unsigned norrath_creature_finish_construction(void*) { event("finish"); return 0x77; }
static void run_case(unsigned flags0) {
    calls.clear(); flag_low = flags0;
    Image<1000> o, want;
    unsigned char pt[12] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    char* args[] = {nullptr};
    norrath_ctors_lava_wraith::host_note(model); norrath_ctors_lava_wraith::host_note(header);
    model_low = low(model);
    put(model + 60, low(header), 4);
    put(norrath_actor_view_4844a4, 0xcafef00du, 4);
    const unsigned r = norrath_construct_SeaMonster(o.b, pt, (short)-9, args);
    expect_log({"Creature type=-9 map=1 scale=10 args=1", "params 9 3f999999 2 1 1 1", "animInit 288", "animAdd 288 1 1", "slow run",
                "events 4 3e999999 1 0 0 0 0 0 0", "events 5 3e999999 1 0 0 0 0 0 0", "events 6 3e999999 1 0 0 0 0 0 0", "finish"}, "sea monster");
    for (int i = 0; i < 12; ++i) want.b[16 + i] = pt[i];
    want.w(36, model_low); want.w(40, 65, 1); want.w(41, 50, 1); want.w(72, low(norrath_ctors_lava_wraith_view_44e2b0));
    unsigned low_word = flags0 | 0x00040000u;
    want.w(164, 0x2000 | 0x80u);
    want.w(168, 0x3f999999u); want.w(224, fbits(1.0f)); want.w(248, 0xcafef00du); want.w(228, fbits(10.0f));
    for (unsigned off : {176u, 192u, 236u, 256u, 960u}) want.w(off, 0);
    want.w(572, 0, 2);
    want.w(28, fbits(35.0f));
    unsigned ret = 0;
    if (low_word & 1u) { low_word |= 0x20000000u; want.w(224, 0x3f933333u); want.w(28, fbits(40.0f)); ret = low_word; }
    want.w(160, low_word);
    expect_image(o, want, "sea monster image");
    expect(r == ret, "sea monster return");
}
int main() {
    run_case(0); run_case(1); run_case(0x80000000u); run_case(0x7);
    return finish("sea monster constructor");
}
