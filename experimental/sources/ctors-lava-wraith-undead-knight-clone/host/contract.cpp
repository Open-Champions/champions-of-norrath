#define NORRATH_CTORS_LAVA_WRAITH_HOST 1
#include "../undead_knight_clone.cpp"
#include "../../ctors-lava-wraith-detail/host_support.hpp"
using namespace hs;
char levelName[322];
unsigned char norrath_actor_view_4844a4[4], norrath_ctors_lava_wraith_view_42d578[4], norrath_ctors_lava_wraith_view_44fb28[4];
unsigned char norrath_ctors_lava_wraith_view_4858b8[8] = {'b', 'a', 't', 't', 'l', 'e', '3', 0};
static unsigned char* obj;
static unsigned char model[160], header[8];
static unsigned model_low;
extern "C" int strcmp(const char* a, const char* b) { while (*a && *a == *b) { ++a; ++b; } return (unsigned char)*a - (unsigned char)*b; }
extern "C" unsigned norrath_creature_construct(void* object, const void* point, short type, void* map, float scale, char** arguments) {
    event("Creature type=%d map=%d scale=%g args=%d", type, map == norrath_ctors_lava_wraith_view_42d578, scale, arguments != nullptr);
    obj = (unsigned char*)object; std::memcpy(obj + 16, point, 12);
    put(obj + 36, model_low, 4); put(obj + 160, 0x21, 4); put(obj + 164, 0x4000, 4);
    return 1;
}
extern "C" void norrath_light_add(void* l) {
    event("lightAdd %d", int((unsigned char*)l - obj));
    expect(obj[2504] == 0 && obj[2505] == 0 && word_at(obj + 2532) == fbits(120.0f) && word_at(obj + 2544) == word_at(obj + 16), "light complete");
}
extern "C" void norrath_creature_set_parameters(void*, float a, float b, float c, float d, float e, int flag) { event("params %08x %08x %g %g %08x %d b572=%u", fbits(a), fbits(b), c, d, fbits(e), flag, obj[572]); }
extern "C" void norrath_actor_animation_initialize(void* s) { event("animInit %d", int((unsigned char*)s - obj)); }
extern "C" void norrath_actor_animation_add(void* s, void* h, int fl) { event("animAdd %d %d %d", int((unsigned char*)s - obj), h == header, fl); }
extern "C" unsigned norrath_actor_add_slow_run_list(void*) { event("slow run"); return 3; }
extern "C" void norrath_creature_set_anim_event_flags(void*, int a, float b, int c, float d, int e, float ff, int g, float h, int i) {
    event("events %d %08x %d %08x %d %08x %d %g %d", a, fbits(b), c, fbits(d), e, fbits(ff), g, h, i);
}
extern "C" unsigned norrath_creature_finish_construction(void*) { event("finish"); return 0x77; }
static void run_case(const char* level) {
    calls.clear(); std::memset(levelName, 0, sizeof levelName); std::strcpy(levelName + 81, level);
    Image<2704> o, want;
    unsigned char pt[12] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    char* args[] = {nullptr};
    norrath_ctors_lava_wraith::host_note(model); norrath_ctors_lava_wraith::host_note(header);
    model_low = low(model);
    put(model + 84, low(header), 4);
    put(norrath_actor_view_4844a4, 0xcafef00du, 4);
    const unsigned r = norrath_construct_UndeadKnightClone(o.b, pt, (short)21, args);
    const bool same = std::strcmp(level, "battle3") == 0;
    std::vector<std::string> w{"Creature type=21 map=1 scale=10 args=1", "lightAdd 2496"};
    w.push_back(same ? "params 3fb33333 3f999999 1 1 3c23d70a 1 b572=0" : "params 3fb33333 3f999999 1 1 3f999999 1 b572=0");
    for (const char* s : {"animInit 288", "animAdd 288 1 1", "slow run",
                          "events 3 3e4ccccc 1 3f199999 1 00000000 0 0 0", "events 4 3e4ccccc 1 3f199999 2 3f800000 2 0 0",
                          "events 5 3e999999 4 00000000 0 00000000 0 0 0", "events 6 3e999999 1 3f4ccccc 1 3fcccccc 4 0 0",
                          "events 14 3f000000 1 00000000 0 00000000 0 0 0", "finish"}) w.push_back(s);
    expect_log(w, "undead knight clone");
    for (int i = 0; i < 12; ++i) { want.b[16 + i] = pt[i]; want.b[2544 + i] = pt[i]; }
    want.w(36, model_low); want.w(72, low(norrath_ctors_lava_wraith_view_44fb28));
    want.w(2505, 0, 1); want.w(2504, 0, 1);
    want.w(2514, 255, 2); want.w(2518, 128, 2); want.w(2506, 255, 2); want.w(2508, 128, 2); want.w(2510, 128, 2); want.w(2516, 128, 2);
    want.w(2532, fbits(120.0f)); want.w(2536, 0x3891a2b3u); want.w(2520, 0xfff0, 2); want.w(2524, fbits(120.0f)); want.w(2528, 0x3891a2b3u);
    want.w(2540, 0); want.w(2512, 0xfff0, 2);
    for (unsigned i = 0; i < 4; ++i) want.w(2684 - 8 * i, 0xffff, 2);
    want.w(2656, 0xff, 1); want.w(2657, 0xff, 1);
    for (unsigned i = 0; i < 4; ++i) { want.w(2640 + 2 * i, 0, 2); want.w(2648 + 2 * i, 0, 2); }
    want.w(40, 20, 1); want.w(160, 0x21 | 0x20020000u); want.w(164, 0x4000 | 0x22u); want.w(172, 16, 1); want.w(168, 0x3f999999u);
    want.w(248, 0xcafef00du); want.w(28, fbits(20.0f)); want.w(2556, 1); want.w(2696, 1); want.w(88, 0xbe4cccccu); want.w(96, 0x3f199999u);
    want.w(100, fbits(1.0f)); want.w(104, 0x3e999999u); want.w(224, 0x3fa66666u);
    for (unsigned off : {176u, 192u, 236u}) want.w(off, 0);
    want.w(572, 0, 2);
    expect_image(o, want, "undead knight clone image");
    expect(r == 0x77u, "undead knight clone return");
}
int main() { run_case("battle3"); run_case("battle2"); run_case(""); run_case("battle3x"); return finish("undead knight clone constructor"); }
