#define NORRATH_CTORS_LAVA_WRAITH_HOST 1
#include "../mummy_king.cpp"
#include "../../ctors-lava-wraith-detail/host_support.hpp"
using namespace hs;
unsigned char norrath_actor_view_4844a4[4], norrath_ctors_lava_wraith_view_42d480[4], norrath_ctors_lava_wraith_view_44f798[4];
static unsigned char* obj;
static unsigned char model[160], header[8];
static unsigned model_low;
extern "C" unsigned norrath_creature_construct(void* object, const void* point, short type, void* map, float scale, char** arguments) {
    event("Creature type=%d map=%d scale=%g args=%d", type, map == norrath_ctors_lava_wraith_view_42d480, scale, arguments != nullptr);
    obj = (unsigned char*)object; std::memcpy(obj + 16, point, 12);
    put(obj + 36, model_low, 4); put(obj + 160, 0x21, 4); put(obj + 164, 0x4000, 4);
    return 1;
}
extern "C" void norrath_creature_set_parameters(void*, float a, float b, float c, float d, float e, int flag) { event("params %g %08x %g %08x %g %d b1104=%u", a, fbits(b), c, fbits(d), e, flag, word_at(obj + 1104)); }
extern "C" void norrath_creature_set_anim_event_flags(void*, int a, float b, int c, float d, int e, float ff, int g, float h, int i) { event("events %d %08x %d %08x %d %g %d %g %d", a, fbits(b), c, fbits(d), e, ff, g, h, i); }
extern "C" void norrath_actor_animation_initialize(void* s) { event("animInit %d", int((unsigned char*)s - obj)); }
extern "C" void norrath_actor_animation_add(void* s, void* h, int fl) { event("animAdd %d %d %d", int((unsigned char*)s - obj), h == header, fl); }
extern "C" unsigned norrath_actor_add_slow_run_list(void*) { event("slow run"); return 0x66; }
int main() {
    Image<1160> o, want;
    unsigned char pt[12] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    char* args[] = {nullptr};
    norrath_ctors_lava_wraith::host_note(model); norrath_ctors_lava_wraith::host_note(header);
    model_low = low(model);
    put(model + 68, low(header), 4);
    put(norrath_actor_view_4844a4, 0xcafef00du, 4);
    const unsigned r = norrath_construct_MummyKing(o.b, pt, (short)300, args);
    expect_log({"Creature type=300 map=1 scale=10 args=1", "params 34 3fb33333 2.5 40866666 10.5 1 b1104=0",
                "events 3 3e999999 1 3f666666 1 0 0 0 0", "events 4 3e999999 1 00000000 0 0 0 0 0", "events 10 3f4ccccc 1 00000000 0 0 0 0 0",
                "events 5 3dcccccc 1 00000000 0 0 0 0 0", "animInit 288", "animAdd 288 1 1", "slow run"}, "mummy king");
    for (int i = 0; i < 12; ++i) want.b[16 + i] = pt[i];
    want.w(36, model_low); want.w(72, low(norrath_ctors_lava_wraith_view_44f798));
    for (unsigned i = 0; i < 4; ++i) want.w(1092 - 8 * i, 0xffff, 2);
    want.w(1064, 0xff, 1); want.w(1065, 0xff, 1);
    for (unsigned i = 0; i < 4; ++i) { want.w(1048 + 2 * i, 0, 2); want.w(1056 + 2 * i, 0, 2); }
    for (unsigned i = 0; i < 4; ++i) { want.w(1108 + 4 * i, 0); want.w(1136 + 4 * i, 0); }
    want.w(1124, 0); want.w(40, 50, 1); want.w(41, 50, 1); want.w(160, 0x21 | 0x20010000u); want.w(164, 0x4000 | 8u);
    want.w(168, 0x3f999999u); want.w(28, fbits(25.0f)); want.w(88, 0xbe4cccccu); want.w(100, 0x3f333333u); want.w(96, 0x3f199999u);
    want.w(224, fbits(1.0f)); want.w(960, 14); want.w(248, 0xcafef00du); want.w(104, 0x3e999999u);
    for (unsigned off : {176u, 192u, 236u, 256u, 1132u, 1104u}) want.w(off, 0);
    want.w(572, 0, 2);
    expect_image(o, want, "mummy king image");
    expect(r == 0x66u, "mummy king return");
    return finish("mummy king constructor");
}
