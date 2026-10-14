#define NORRATH_CTORS_LAVA_WRAITH_HOST 1
#include "../lava_monster.cpp"
#include "../../ctors-lava-wraith-detail/host_support.hpp"
#include <strings.h>
using namespace hs;
unsigned char norrath_actor_view_4844a4[4], norrath_ctors_lava_wraith_view_42b010[4], norrath_ctors_lava_wraith_view_44b810[4];
unsigned char norrath_ctors_lava_wraith_view_46d7b8[16] = "lb_block", norrath_ctors_lava_wraith_view_34b474[4], norrath_creature_view_484cac[4];
static unsigned char* obj;
static unsigned char model36[256], model_g[8], chunk_hdr[8], owner[128], vtable[256], header_a[8], header_b[8];
static unsigned model_low, rands[4], rand_calls;
static float f(unsigned u) { float x; std::memcpy(&x, &u, 4); return x; }
static int method_stub(void* self, int a, int b) { event("virtual self=%d a=%d b=%d", int((unsigned char*)self - owner), a, b); return 1000; }
extern "C" int norrath_game_object_e_rand(void*) { event("rand"); return (int)rands[rand_calls++ & 3]; }
extern "C" unsigned norrath_creature_construct(void* object, const void* point, short type, void* map, float scale, char** arguments) {
    event("Creature type=%d map=%d scale=%g args=%d", type, map == norrath_ctors_lava_wraith_view_42b010, scale, arguments != nullptr);
    obj = (unsigned char*)object; std::memcpy(obj + 16, point, 12);
    put(obj + 36, model_low, 4); put(obj + 160, 0x21, 4); put(obj + 164, 0x4000, 4);
    put(obj + 560, fbits(-7.9f), 4);
    return 1;
}
extern "C" int norrath_factory_strcasecmp(const char* l, const char* r) { event("strcasecmp"); return strcasecmp(l, r); }
extern "C" void norrath_creature_set_parameters(void*, float a, float b, float c, float d, float e, int flag) { event("params %g %08x %g %g %g %d b1048=%u", a, fbits(b), c, d, e, flag, word_at(obj + 1048)); }
extern "C" void* norrath_lava_monster_get_model(void*) { event("getModel b244=%u", word_at(obj + 244)); return model_g; }
extern "C" unsigned long long norrath_actor_model_active_chunks(void* m, int n) { event("chunks %d %d", m == model_g, n); return 0x0102030405060708ull; }
extern "C" void norrath_actor_animation_initialize(void* s) { event("animInit %d b552=%08x", int((unsigned char*)s - obj), word_at(obj + 552)); }
extern "C" void norrath_actor_animation_add(void* s, void* h, int fl) { event("animAdd %d %d %d b572=%u", int((unsigned char*)s - obj), h == header_a ? 1 : h == header_b ? 2 : 0, fl, obj[572] | obj[573] << 8); }
extern "C" unsigned norrath_actor_add_slow_run_list(void*) { event("slow run"); return 3; }
extern "C" void norrath_object_update_in_grid(void*) { event("updateInGrid"); }
extern "C" void norrath_creature_set_anim_event_flags(void*, int a, float b, int c, float d, int e, float ff, int g, float h, int i) {
    event("events %d %08x %d %08x %d %g %d %g %d", a, fbits(b), c, fbits(d), e, ff, g, h, i);
}
extern "C" unsigned norrath_creature_finish_construction(void*) { event("finish b964=%u b968=%08x", word_at(obj + 964), word_at(obj + 968)); return 0x77; }
static void run_case(const char* name, unsigned r0, unsigned r1, unsigned r2, unsigned r3, float game_float) {
    calls.clear(); for (int i = 0; i < 4; ++i) {} rands[0] = r0; rands[1] = r1; rands[2] = r2; rands[3] = r3; rand_calls = 0;
    Image<1100> o, want;
    unsigned char pt[12] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    char* args[] = {nullptr};
    namespace nc = norrath_ctors_lava_wraith;
    nc::host_note(model36); nc::host_note(model_g); nc::host_note(chunk_hdr); nc::host_note(owner); nc::host_note(vtable);
    nc::host_note(header_a); nc::host_note(header_b);
    nc::host_note(reinterpret_cast<const void*>(&method_stub));
    model_low = low(model36);
    put(model36 + 220, low(header_a), 4); put(model36 + 204, low(header_b), 4);
    put(norrath_ctors_lava_wraith_view_34b474, low(owner), 4);
    put(owner + 96, low(vtable), 4);
    put(vtable + 96, 8u, 2); put(vtable + 100, low(reinterpret_cast<const void*>(&method_stub)), 4);
    put(norrath_creature_view_484cac, fbits(game_float), 4);
    put(norrath_actor_view_4844a4, 0xcafef00du, 4);
    char nm[16]; std::strcpy(nm, name);
    const unsigned r = norrath_construct_LavaMonster(o.b, pt, (short)-4, args, nm);
    const bool block = f(0) == 0.0f && ((float)(int)r2 * f(0x37800000)) < f(0x3f8cccccu);
    const bool lb = strcasecmp(name, "lb_block") == 0;
    float spread = (float)(int)r0 * f(0x37800000); spread = spread * f(0x3e4cccccu); spread = spread + f(0x3f666666u);
    const unsigned pick = r1 % 15u;
    char b[200];
    std::vector<std::string> w{"Creature type=-4 map=1 scale=10 args=1", "rand", "strcasecmp"};
    std::snprintf(b, sizeof b, "params 3 3f999999 1 1 1 1 b1048=%u", lb ? 1u : 0u); w.push_back(b);
    w.push_back("rand");
    std::snprintf(b, sizeof b, "getModel b244=%u", pick); w.push_back(b);
    std::snprintf(b, sizeof b, "chunks 1 %u", pick); w.push_back(b);
    w.push_back("animInit 288 b552=05060708");
    std::snprintf(b, sizeof b, "animAdd 288 %d 1 b572=%u", lb ? 1 : 2, lb ? 0u : 13u); w.push_back(b);
    w.push_back("rand");
    const int amount = (int)game_float;
    int base = 1000, total = 0;
    if (block) {
        std::snprintf(b, sizeof b, "virtual self=8 a=0 b=%d", amount); w.push_back(b);
        w.push_back("rand");
        float extra = (float)base; extra = extra * f(0x3e4cccccu); float roll = (float)(int)r3 * f(0x37800000); extra = extra * roll;
        total = base + (int)extra;
    }
    w.push_back("slow run");
    for (const char* s : {"events 17 3f199999 1 00000000 0 0 0 0 0", "events 18 3f199999 1 00000000 0 0 0 0 0", "events 19 3f000000 1 3f800000 1 0 0 0 0",
                          "events 20 3f000000 1 00000000 0 0 0 0 0", "events 21 3f000000 1 00000000 0 0 0 0 0", "events 23 3f800000 1 40400000 2 0 0 0 0",
                          "events 24 3f800000 1 40466666 2 0 0 0 0", "events 5 3f8ccccc 4 00000000 0 0 0 0 0", "events 6 3f000000 4 00000000 0 0 0 0 0", "updateInGrid"}) w.push_back(s);
    const int whole = (int)(-7.9f);
    const int halved = (whole + (int)((unsigned)whole >> 31)) >> 1;
    std::snprintf(b, sizeof b, "finish b964=%u b968=%08x", (unsigned)halved, fbits(1.0f)); w.push_back(b);
    expect_log(w, "lava monster");
    for (int i = 0; i < 12; ++i) want.b[16 + i] = pt[i];
    want.w(36, model_low); want.w(560, fbits(-7.9f)); want.w(72, low(norrath_ctors_lava_wraith_view_44b810));
    for (unsigned i = 0; i < 4; ++i) want.w(1036 - 8 * i, 0xffff, 2);
    want.w(1008, 0xff, 1); want.w(1009, 0xff, 1);
    for (unsigned i = 0; i < 4; ++i) { want.w(992 + 2 * i, 0, 2); want.w(1000 + 2 * i, 0, 2); }
    want.w(40, 40, 1); want.w(160, 0x21 | 0x00020800u); want.w(164, 0x4000 | 0x40u); want.w(172, 11, 1); want.w(28, fbits(30.0f));
    want.w(248, 0xcafef00du); want.w(92, 0xbe4cccccu); want.w(168, 0x3f999999u); want.w(88, fbits(1.0f));
    want.w(176, 0); want.w(192, 0); want.w(236, 0); want.w(572, lb ? 0 : 13, 2); want.w(960, 0);
    want.w(224, fbits(spread)); want.w(1048, lb ? 1 : 0); want.w(244, pick); want.w(552, 0x05060708u); want.w(556, 0x01020304u);
    if (block) { want.w(1012, 21, 2); want.w(1018, 0, 2); want.w(1014, (unsigned)base, 2); want.w(1016, (unsigned)total, 2); }
    want.w(968, fbits(1.0f)); want.w(964, (unsigned)halved);
    expect_image(o, want, "lava monster image");
    expect(r == 0x77u, "lava monster return");
}
int main() {
    run_case("lb_block", 100, 33, 1000, 40000, 5.9f);
    run_case("LB_BLOCK", 0xfffffff0u, 14, 70000, 123, 0.5f);
    run_case("other", 5, 15, 500, 65535, -3.5f);
    run_case("", 7, 29, 0, 0, 1.0f);
    run_case("lb", 0x7fffffff, 3, 0x7fffffff, 3, 2.0f);
    return finish("lava monster constructor");
}
