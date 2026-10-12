#define NORRATH_CTORS_LAVA_WRAITH_HOST 1
#include "../soul.cpp"
#include "../../ctors-lava-wraith-detail/host_support.hpp"
#include <strings.h>
using namespace hs;
char levelName[322];
unsigned char norrath_actor_view_4844a4[4], norrath_ctors_lava_wraith_view_42b3d8[4], norrath_ctors_lava_wraith_view_44def0[4];
unsigned char norrath_ctors_lava_wraith_view_485240[8] = {'a', 'i', 'r', 'h', 'u', 'b', 0, 0};
static unsigned char* obj;
static unsigned char model[128], headers[4][8];
static unsigned model_low;
extern "C" unsigned norrath_creature_construct(void* object, const void* point, short type, void* map, float scale, char** arguments) {
    event("Creature type=%d map=%d scale=%g args=%d", type, map == norrath_ctors_lava_wraith_view_42b3d8, scale, arguments != nullptr);
    obj = (unsigned char*)object; std::memcpy(obj + 16, point, 12);
    put(obj + 36, model_low, 4); put(obj + 160, 0x11, 4); put(obj + 164, 0x100, 4); put(obj + 12, 0x40, 4);
    return 1;
}
extern "C" void norrath_creature_set_parameters(void*, float a, float b, float c, float d, float e, int flag) { event("params %08x %08x %g %g %g %d", fbits(a), fbits(b), c, d, e, flag); }
extern "C" void norrath_actor_animation_initialize(void* s) { event("animInit %d", int((unsigned char*)s - obj)); }
extern "C" void norrath_actor_animation_add(void* s, void* h, int f) {
    int which = -1; for (int i = 0; i < 4; ++i) if (h == headers[i]) which = i;
    event("animAdd %d header=%d %d", int((unsigned char*)s - obj), which, f);
}
extern "C" void* norrath_list_oneway_link(void* head, void* oh, void* v) { event("link %d %d %d flags=%08x", int((unsigned char*)head - obj), oh == (void*)0x1234, v == (void*)0x1234, word_at(obj + 12)); return nullptr; }
extern "C" int norrath_factory_strcasecmp(const char* l, const char* r) { event("strcasecmp %d", int(l - levelName)); return strcasecmp(l, r); }
extern "C" void norrath_soul_set_state(void*, int s) { event("setState %d", s); }
extern "C" unsigned norrath_actor_add_run_list(void*) { event("run b236=%u", word_at(obj + 236)); return 3; }
extern "C" void norrath_creature_set_anim_event_flags(void*, int a, float b, int c, float d, int e, float f, int g, float h, int i) { event("events %d %08x %d %g %d %g %d %g %d", a, fbits(b), c, d, e, f, g, h, i); }
extern "C" unsigned norrath_creature_finish_construction(void*) { event("finish"); return 0x77; }
static void run_case(void* minion, const char* level) {
    calls.clear(); std::memset(levelName, 0, sizeof levelName); std::strcpy(levelName + 81, level);
    Image<1000> o, want;
    unsigned char pt[12] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    char* args[] = {nullptr};
    norrath_ctors_lava_wraith::host_note(model);
    for (auto& h : headers) norrath_ctors_lava_wraith::host_note(h);
    model_low = low(model);
    put(model + 108, low(headers[0]), 4); put(model + 52, low(headers[1]), 4); put(model + 60, low(headers[2]), 4);
    put(norrath_actor_view_4844a4, 0xcafef00du, 4);
    const unsigned r = norrath_construct_Soul(o.b, pt, (short)0x7001, args, minion);
    const bool airhub = !minion && strcasecmp(level, "airhub") == 0;
    std::vector<std::string> w{"Creature type=28673 map=1 scale=10 args=1", "params 40599999 3f999999 1 1 1 1", "animInit 288"};
    char b[100];
    if (minion) { w.push_back("link 960 1 1 flags=00000049"); w.push_back("animAdd 288 header=0 1"); }
    else { w.push_back("strcasecmp 0"); if (airhub) { w.push_back("animAdd 288 header=1 1"); w.push_back("setState 24"); } else w.push_back("animAdd 288 header=2 1"); }
    w.push_back("run b236=1"); w.push_back("events 11 3f333333 1 0 0 0 0 0 0"); w.push_back("finish");
    (void)b;
    // strcasecmp reports the offset into levelName passed by the constructor (81)
    for (auto& s : w) if (s == "strcasecmp 0") s = "strcasecmp 81";
    expect_log(w, "soul");
    for (int i = 0; i < 12; ++i) want.b[16 + i] = pt[i];
    want.w(36, model_low); want.w(960, 0); want.w(40, 30, 1);
    want.w(160, 0x11 | 0x04880000u); want.w(164, 0x100 | 2u);
    want.w(248, 0xcafef00du); want.w(72, low(norrath_ctors_lava_wraith_view_44def0)); want.w(168, 0x3f999999u); want.w(28, fbits(15.0f));
    want.w(792, 0xffff, 2);
    for (unsigned off : {964u, 176u, 192u, 256u}) want.w(off, 0);
    want.w(572, 0, 2); want.w(236, 1);
    for (unsigned off : {88u, 92u, 96u, 100u, 104u}) want.w(off, fbits(1.0f));
    want.w(12, minion ? (0x40u | 9u) : 0x40u);
    expect_image(o, want, "soul image");
    expect(r == 0x77u, "soul return");
}
int main() {
    run_case((void*)0x1234, "airhub");
    run_case(nullptr, "AirHub"); run_case(nullptr, "lava3"); run_case(nullptr, "");
    return finish("soul constructor");
}
