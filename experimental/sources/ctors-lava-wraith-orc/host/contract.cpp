#define NORRATH_CTORS_LAVA_WRAITH_HOST 1
#include "../orc.cpp"
#include "../../ctors-lava-wraith-detail/host_support.hpp"
#include <strings.h>
using namespace hs;
unsigned char norrath_actor_view_4844a4[4], norrath_ctors_lava_wraith_view_483f88[4], norrath_ctors_lava_wraith_view_44b720[4];
unsigned char norrath_ctors_lava_wraith_view_42c8e0[4], norrath_ctors_lava_wraith_view_42ca58[4];
alignas(8) unsigned char norrath_ctors_lava_wraith_view_42cbd0[128], norrath_ctors_lava_wraith_view_42ccf0[128];
#define TXT(n, t) unsigned char norrath_ctors_lava_wraith_view_##n[16] = t
TXT(4718e8, "orcrecruit"); TXT(4718f8, "orcsoldier"); TXT(471908, "orcleader1"); TXT(471918, "orcleader"); TXT(471928, "orcarcher");
TXT(471938, "orcshaman"); TXT(471948, "orc2recruit"); TXT(471958, "orc2soldier"); TXT(471968, "orc2leader1"); TXT(471978, "orc2leader");
TXT(471988, "orc2archer"); TXT(471998, "orc2shaman");
static const char* texts[12] = {"orcrecruit", "orcsoldier", "orcleader1", "orcleader", "orcarcher", "orcshaman",
                                "orc2recruit", "orc2soldier", "orc2leader1", "orc2leader", "orc2archer", "orc2shaman"};
static const unsigned kinds[12] = {0, 1, 3, 2, 4, 5, 0, 1, 3, 2, 4, 5};
static unsigned char* obj;
static unsigned char model[320], chunk_src[8], headers[3][8];
static char lump[16] = "orc.lmp", res_x[8] = "x", name_a[8] = "resA", name_b[8] = "resB";
static unsigned model_low, rands[24], rand_calls;
static float f(unsigned u) { float x; std::memcpy(&x, &u, 4); return x; }
extern "C" int norrath_game_object_e_rand(void*) { event("rand"); return (int)rands[rand_calls++ % 24]; }
extern "C" unsigned norrath_creature_construct(void* object, const void* point, short type, void* map, float scale, char** arguments) {
    event("Creature type=%d map=%s scale=%g args=%d", type, map == norrath_ctors_lava_wraith_view_42c8e0 ? "A" : map == norrath_ctors_lava_wraith_view_42ca58 ? "B" : "?", scale, arguments != nullptr);
    obj = (unsigned char*)object; std::memcpy(obj + 16, point, 12);
    put(obj + 36, model_low, 4); put(obj + 160, 0x21, 4); put(obj + 164, 0x4000, 4);
    return 1;
}
extern "C" void* norrath_lump_find_resource(char* a, char* b) { event("lump %s %s", a, b); return res_x; }
extern "C" int norrath_strticmp(const char* l, const char* r) { return strcasecmp(l, r); }
extern "C" void norrath_orc_set_weapon(void*) { event("setWeapon b244=%08x", word_at(obj + 244)); }
extern "C" void norrath_orc_init_type(void*, int fl) { event("initType %d", fl); }
extern "C" unsigned long long norrath_actor_model_active_chunks(void* m, int n) { event("chunks %d %08x", m == chunk_src, n); return 0x0102030405060708ull; }
extern "C" void norrath_actor_animation_initialize(void* s) { event("animInit %d b552=%08x", int((unsigned char*)s - obj), word_at(obj + 552)); }
extern "C" void norrath_actor_animation_add(void* s, void* h, int fl) {
    int which = -1; for (int i = 0; i < 3; ++i) if (h == headers[i]) which = i;
    event("animAdd %d header=%d %d", int((unsigned char*)s - obj), which, fl);
}
extern "C" unsigned norrath_actor_add_slow_run_list(void*) { event("slow run"); return 3; }
extern "C" void norrath_creature_set_anim_event_flags(void*, int a, float b, int c, float d, int e, float ff, int g, float h, int i) {
    event("events %d %08x %d %08x %d %08x %d %08x %d", a, fbits(b), c, fbits(d), e, fbits(ff), g, fbits(h), i);
}
extern "C" unsigned norrath_creature_finish_construction(void*) { event("finish"); return 0x77; }
static void setup_table(unsigned char* t, unsigned stamp) {
    namespace nc = norrath_ctors_lava_wraith;
    std::memset(t, 0, 128);
    nc::host_note(lump); nc::host_note(res_x); nc::host_note(name_a); nc::host_note(name_b);
    put(t, low(lump), 4); put(t + 4, stamp, 4); put(t + 8, low(name_a), 4); put(t + 16, low(name_b), 4);
}
static void run_case(const char* name, int variant, unsigned stamp, unsigned base) {
    calls.clear();
    for (unsigned i = 0; i < 24; ++i) rands[i] = base * (i % 5 + 1) + i * 12345u + (i & 1);
    rand_calls = 0;
    setup_table(norrath_ctors_lava_wraith_view_42cbd0, stamp); setup_table(norrath_ctors_lava_wraith_view_42ccf0, stamp);
    put(norrath_ctors_lava_wraith_view_483f88, 9, 4);
    Image<1160> o, want;
    unsigned char pt[12] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    char* args[] = {nullptr};
    namespace nc = norrath_ctors_lava_wraith;
    nc::host_note(model); nc::host_note(chunk_src);
    model_low = low(model);
    put(model + 12, low(chunk_src), 4);
    for (int i = 0; i < 3; ++i) { nc::host_note(headers[i]); put(model + 252 + 16 * i, low(headers[i]), 4); }
    put(norrath_actor_view_4844a4, 0xcafef00du, 4);
    char nm[40]; std::strcpy(nm, name);
    const unsigned r = norrath_construct_Orc(o.b, pt, (short)-6, args, nm, (signed char)variant);
    // ---- independent model
    unsigned used = 0; auto draw = [&]() { return rands[used++ % 24]; };
    const unsigned d0 = draw();
    const float scale = (float)(int)(d0 % 14u + 4u);
    unsigned pick = draw() % 6u; if (pick == 3) pick = 0;
    for (int i = 0; i < 12; ++i) if (strcasecmp(name, texts[i]) == 0) { pick = kinds[i]; break; }
    const unsigned s = draw();
    unsigned mask = 0, p208 = 0, p1112 = 0, p1124 = 0; bool w208 = false, w1112 = false, w1124 = false;
    const float sc = f(0x37800000), thr = f(0x3dcccccc);
    auto roll = [&]() { return (float)(int)draw() * sc < thr; };
    if (pick == 0 || pick == 4) {
        if (s & 1) mask |= (s & 2) ? 1u : 2u; else if (s & 2) mask |= 0x100; else if (s & 4) mask |= 0x200; else mask |= 0x400;
        if (pick == 4) { if (roll()) { p208 = draw() % 3u + 1; w208 = true; } p1124 = 1; w1124 = true; }
        else if (s & 0x32) { if ((draw() & 1u) == 0) { p1112 = 8; w1112 = true; } }
    } else if (pick == 1) {
        if (roll()) { p208 = (draw() & 1u) + 1; w208 = true; }
        mask |= 0x400000; if (s & 1) mask |= 0x800000; if (s & 2) mask |= 0x200000;
        mask |= (s & 4) ? 0x2010u : (s & 8) ? 0x2008u : 0x2004u;
        mask |= 0x2000; mask |= 1u << ((draw() & 3u) + 15);
        if (s & 0x16c) { p1112 = (draw() & 1u) + 9; w1112 = true; }
    } else if (pick == 2 || pick == 3) {
        mask |= 1u << ((draw() & 1u) + 5); if ((mask & 0x20) && (s & 1)) mask |= 0x1000;
        mask |= 0x00e04000; mask |= 1u << ((draw() & 1u) + 19);
        p1112 = (draw() & 1u) + 11; w1112 = true;
        if (roll()) { p208 = (draw() & 3u) + 1; w208 = true; }
    } else if (pick == 5) mask |= 0x20000000;
    const unsigned hp = draw() % 3u;
    // ---- expected calls
    std::vector<std::string> w; char b[200];
    w.push_back("rand");
    std::snprintf(b, sizeof b, "Creature type=-6 map=%s scale=%g args=1", variant == 0 ? "A" : "B", scale); w.push_back(b);
    if ((variant == 0 || variant == 1) && stamp != 9) { w.push_back("lump orc.lmp resA"); w.push_back("lump orc.lmp resB"); }
    for (unsigned i = 1; i < used - 1; ++i) w.push_back("rand");
    std::snprintf(b, sizeof b, "setWeapon b244=%08x", mask); w.push_back(b);
    w.push_back("initType 0");
    std::snprintf(b, sizeof b, "chunks 1 %08x", mask); w.push_back(b);
    w.push_back("animInit 288 b552=05060708");
    w.push_back("rand");
    std::snprintf(b, sizeof b, "animAdd 288 header=%u 1", hp); w.push_back(b);
    w.push_back("slow run");
    struct Ev { int idx; unsigned f12; int a; unsigned f13; int c; unsigned f14; int t0; };
    const Ev evs[14] = {{19,0x3e999999,1,0,0,0,0},{20,0x3e999999,1,0,0,0,0},{21,0x3e999999,1,0,0,0,0},{22,0x3f800000,1,0x40000000,2,0,0},
        {23,0x3e999999,1,0,0,0,0},{24,0x3e999999,2,0x3f199999,1,0,0},{25,0x3e4ccccc,2,0x3f000000,2,0x3f800000,1},{26,0x3e4ccccc,2,0x3f000000,2,0x3f800000,1},
        {3,0x40400000,1,0x3fc00000,2,0,0},{4,0x3f800000,1,0x3fc00000,2,0,0},{5,0x3f800000,1,0x3fc00000,2,0,0},{6,0x3ecccccc,1,0x3fc00000,2,0,0},
        {7,0x3f800000,1,0x3fc00000,2,0,0},{8,0x3f800000,1,0x3fc00000,2,0,0}};
    for (const Ev& e : evs) {
        std::snprintf(b, sizeof b, "events %d %08x %d %08x %d %08x %d 00000000 0", e.idx, e.f12, e.a, e.f13, e.c, e.f14, e.t0); w.push_back(b);
    }
    w.push_back("finish");
    // the draws consumed before setWeapon were pushed generically; the call order is rand x N then the rest
    expect_log(w, "orc");
    for (int i = 0; i < 12; ++i) want.b[16 + i] = pt[i];
    want.w(36, model_low); want.w(72, low(norrath_ctors_lava_wraith_view_44b720)); want.w(1128, 0);
    for (unsigned i = 0; i < 4; ++i) want.w(1100 - 8 * i, 0xffff, 2);
    want.w(1072, 0xff, 1); want.w(1073, 0xff, 1);
    for (unsigned i = 0; i < 4; ++i) { want.w(1056 + 2 * i, 0, 2); want.w(1064 + 2 * i, 0, 2); }
    unsigned char* table = variant == 0 ? norrath_ctors_lava_wraith_view_42cbd0 : norrath_ctors_lava_wraith_view_42ccf0;
    if (variant == 0 || variant == 1) want.w(1136, low(table));
    want.w(40, 40, 1); want.w(160, 0x21 | 0x00040400u); want.w(164, 0x4000); want.w(176, fbits(80.0f)); want.w(168, 0x3f999999u); want.w(28, fbits(18.0f));
    want.w(248, 0xcafef00du); want.w(236, 0); want.w(572, 0, 2); want.w(1120, 0); want.w(1116, pick); want.w(244, mask); want.w(972, 0);
    if (w208) want.w(208, p208); if (w1112) want.w(1112, p1112); if (w1124) want.w(1124, p1124);
    want.w(552, 0x05060708u); want.w(556, 0x01020304u);
    expect_image(o, want, "orc image");
    expect(r == 0x77u, "orc return");
    if ((variant == 0 || variant == 1) && stamp != 9) {
        expect(word_at(table + 4) == 9u && word_at(table + 12) == low(res_x) && word_at(table + 20) == low(res_x), "table stamped and resources stored");
    }
}
int main() {
    for (const char* t : texts) for (int v : {0, 1, 2}) run_case(t, v, v == 1 ? 9 : 0, 100 + v);
    for (const char* n : {"x", "", "ORCSHAMAN"}) for (int v : {0, 1, 5, -1}) for (unsigned base : {0u, 1u, 7u, 3000u, 70001u}) run_case(n, v, 0, base);
    return finish("orc constructor");
}
