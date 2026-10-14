#define NORRATH_CTORS_LAVA_WRAITH_HOST 1
#include "../minion.cpp"
#include "../../ctors-lava-wraith-detail/host_support.hpp"
using namespace hs;
unsigned char norrath_actor_view_4844a4[4], norrath_ctors_lava_wraith_view_42d138[4], norrath_ctors_lava_wraith_view_44f5b8[4];
static unsigned char* obj;
static unsigned char model[160], header[8];
static unsigned model_low;
extern "C" unsigned norrath_creature_construct(void* object, const void* point, short type, void* map, float scale, char** arguments) {
    event("Creature type=%d map=%d scale=%g args=%d", type, map == norrath_ctors_lava_wraith_view_42d138, scale, arguments != nullptr);
    obj = (unsigned char*)object; std::memcpy(obj + 16, point, 12);
    put(obj + 36, model_low, 4); put(obj + 160, 0x21, 4); put(obj + 164, 0x4000, 4);
    return 1;
}
extern "C" void norrath_creature_set_parameters(void*, float a, float b, float c, float d, float e, int flag) { event("params %g %08x %g %08x %g %d b984=%u", a, fbits(b), c, fbits(d), e, flag, word_at(obj + 984)); }
extern "C" void norrath_actor_animation_initialize(void* s) { event("animInit %d", int((unsigned char*)s - obj)); }
extern "C" void norrath_actor_animation_add(void* s, void* h, int fl) { event("animAdd %d %d %d", int((unsigned char*)s - obj), h == header, fl); }
extern "C" unsigned norrath_actor_add_slow_run_list(void*) { event("slow run"); return 3; }
extern "C" void norrath_creature_set_anim_event_flags(void*, int a, float b, int c, float d, int e, float ff, int g, float h, int i) {
    event("events %d %08x %d %08x %d %08x %d %g %d", a, fbits(b), c, fbits(d), e, fbits(ff), g, h, i);
}
extern "C" unsigned norrath_creature_finish_construction(void*) { event("finish"); return 0x77; }
int main() {
    Image<1000> o, want;
    unsigned char pt[12] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    char* args[] = {nullptr};
    norrath_ctors_lava_wraith::host_note(model); norrath_ctors_lava_wraith::host_note(header);
    model_low = low(model);
    put(model + 108, low(header), 4);
    put(norrath_actor_view_4844a4, 0xcafef00du, 4);
    const unsigned r = norrath_construct_Minion(o.b, pt, (short)-300, args);
    std::vector<std::string> w{"Creature type=-300 map=1 scale=10 args=1", "params 34 3fb33333 2 40866666 10.5 1 b984=0", "animInit 288", "animAdd 288 1 1", "slow run"};
    char b[120];
    for (int n : {4, 5, 6, 7, 3}) { std::snprintf(b, sizeof b, "events %d 3e999999 1 00000000 0 00000000 0 0 0", n); w.push_back(b); }
    for (const char* s : {"events 8 40000000 1 40200000 2 00000000 0 0 0", "events 16 3f333333 1 00000000 0 00000000 0 0 0",
                          "events 15 3f000000 1 00000000 0 00000000 0 0 0", "events 17 3fd33333 1 3c23d70a 64 3fd33333 128 0 0",
                          "events 19 3c23d70a 16 40800000 32 00000000 0 0 0", "finish"}) w.push_back(s);
    expect_log(w, "minion");
    for (int i = 0; i < 12; ++i) want.b[16 + i] = pt[i];
    want.w(36, model_low); want.w(40, 100, 1); want.w(41, 50, 1); want.w(160, 0x21 | 0x20040000u); want.w(164, 0x4000 | 2u);
    want.w(72, low(norrath_ctors_lava_wraith_view_44f5b8)); want.w(168, 0x3f999999u); want.w(28, fbits(45.0f)); want.w(248, 0xcafef00du);
    want.w(224, 0x3fa66666u); want.w(964, 1);
    for (unsigned off : {88u, 92u, 96u, 100u, 104u}) want.w(off, fbits(0.5f));
    for (unsigned off : {960u, 968u, 176u, 192u, 236u, 256u, 980u, 984u}) want.w(off, 0);
    want.w(572, 0, 2);
    expect_image(o, want, "minion image");
    expect(r == 0x77u, "minion return");
    return finish("minion constructor");
}
