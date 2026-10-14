#define NORRATH_CTORS_LAVA_WRAITH_HOST 1
#include "../skeleton.cpp"
#include "../../ctors-lava-wraith-detail/host_support.hpp"
#include <strings.h>
using namespace hs;
unsigned char norrath_actor_view_4844a4[4], norrath_ctors_lava_wraith_view_483f88[4], norrath_ctors_lava_wraith_view_44fee8[4];
alignas(8) unsigned char norrath_ctors_lava_wraith_view_42d7d0[1024], norrath_ctors_lava_wraith_view_42d8d0[640];
#define TXT(n, t) unsigned char norrath_ctors_lava_wraith_view_##n[32] = t
TXT(473f20, "skeletonred"); TXT(473f30, "skeletonblue"); TXT(473f40, "skeletonswordohwred"); TXT(473f58, "skeletonswordohwblue");
TXT(473f70, "skeletonsworddhwred"); TXT(473f88, "skeletonsworddhwblue"); TXT(473fa0, "skeletonaxered"); TXT(473fb0, "skeletonaxeblue");
TXT(473fc0, "skeletonspearred"); TXT(473fd8, "skeletonspearblue"); TXT(473ff0, "skeletonbowred"); TXT(474000, "skeletonbowblue");
TXT(474010, "skeletonswordohw"); TXT(474028, "skeletonsworddhw"); TXT(474040, "skeletonaxe"); TXT(474050, "skeletonspear"); TXT(474060, "skeletonbow");
static const char* texts[17] = {"skeletonred", "skeletonblue", "skeletonswordohwred", "skeletonswordohwblue", "skeletonsworddhwred", "skeletonsworddhwblue",
    "skeletonaxered", "skeletonaxeblue", "skeletonspearred", "skeletonspearblue", "skeletonbowred", "skeletonbowblue", "skeletonswordohw",
    "skeletonsworddhw", "skeletonaxe", "skeletonspear", "skeletonbow"};
// expected (+972 byte, +968 word or -1 for keep) per text
static const int exp972[17] = {2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 2, 0, 0, 0, 0, 0};
static const int exp968[17] = {-1, -1, 0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 0, 1, 2, 3, 4};
static unsigned char* obj;
static unsigned char model[160], chunk_src[8], headers[5][8];
static char lump[16] = "skeleton.lmp", res_a[8] = "a", res_b[8] = "b";
static unsigned model_low, rands[16], rand_calls;
static float f(unsigned u) { float x; std::memcpy(&x, &u, 4); return x; }
extern "C" int norrath_game_object_e_rand(void*) { event("rand"); return (int)rands[rand_calls++ & 15]; }
extern "C" unsigned norrath_creature_construct(void* object, const void* point, short type, void* map, float scale, char** arguments) {
    event("Creature type=%d map=%d scale=%g args=%d", type, (int)((unsigned char*)map - norrath_ctors_lava_wraith_view_42d7d0), scale, arguments != nullptr);
    obj = (unsigned char*)object; std::memcpy(obj + 16, point, 12);
    put(obj + 36, model_low, 4); put(obj + 160, 0x21, 4); put(obj + 164, 0x4000, 4);
    return 1;
}
extern "C" void* norrath_lump_find_resource(char* a, char* b) { event("lump %s %s", a, b); return std::strcmp(b, "resA") == 0 ? res_a : res_b; }
extern "C" int norrath_strticmp(const char* l, const char* r) { return strcasecmp(l, r); }
extern "C" unsigned long long norrath_actor_model_active_chunks(void* m, int n) { event("chunks %d %08x", m == chunk_src, n); return 0x0102030405060708ull; }
extern "C" void norrath_skeleton_init_type(void*, int fl) { event("initType %d b552=%08x", fl, word_at(obj + 552)); }
extern "C" void norrath_actor_animation_initialize(void* s) { event("animInit %d", int((unsigned char*)s - obj)); }
extern "C" void norrath_actor_animation_add(void* s, void* h, int fl) {
    int which = -1; for (int i = 0; i < 5; ++i) if (h == headers[i]) which = i;
    event("animAdd %d header=%d %d", int((unsigned char*)s - obj), which, fl);
}
extern "C" void norrath_skeleton_set_state(void*, int s) { event("setState %d", s); }
extern "C" unsigned norrath_actor_add_slow_run_list(void*) { event("slow run"); return 3; }
extern "C" void norrath_creature_set_anim_event_flags_res_map(void*, void* map, int a, int b, int c, int d, int e, float f12, float f13, float f14, float f15) {
    event("events %d %d %d %d %d %d %08x %08x %08x %08x", map == norrath_ctors_lava_wraith_view_42d8d0, a, b, c, d, e, fbits(f12), fbits(f13), fbits(f14), fbits(f15));
}
extern "C" unsigned norrath_creature_finish_construction(void*) { event("finish"); return 0x77; }
static void setup_table(unsigned stamp_in_table, unsigned global_stamp) {
    namespace nc = norrath_ctors_lava_wraith;
    std::memset(norrath_ctors_lava_wraith_view_42d8d0, 0, 640);
    unsigned char* t = norrath_ctors_lava_wraith_view_42d8d0;
    nc::host_note(lump); nc::host_note(res_a); nc::host_note(res_b);
    static char name_a[8] = "resA", name_b[8] = "resB";
    nc::host_note(name_a); nc::host_note(name_b);
    put(t, low(lump), 4); put(t + 4, stamp_in_table, 4);
    put(t + 8, low(name_a), 4); put(t + 16, low(name_b), 4); put(t + 24, low(name_a), 4);   // pairs 1..3, pair 4 is the null terminator
    for (int v = 0; v < 5; ++v) { nc::host_note(headers[v]); put(t + 104 * v + 68, low(headers[v]), 4); }
    put(norrath_ctors_lava_wraith_view_483f88, global_stamp, 4);
}
static void run_case(const char* name, int kind, unsigned stamp_in_table, unsigned r0, unsigned r1) {
    calls.clear();
    for (unsigned i = 0; i < 16; ++i) rands[i] = (i & 1 ? r1 : r0) + i * 7777u;
    rand_calls = 0;
    setup_table(stamp_in_table, 5);
    Image<1136> o, want;
    unsigned char pt[12] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    char* args[] = {nullptr};
    namespace nc = norrath_ctors_lava_wraith;
    nc::host_note(model); nc::host_note(chunk_src);
    model_low = low(model);
    put(model + 12, low(chunk_src), 4);
    put(norrath_actor_view_4844a4, 0xcafef00du, 4);
    char nm[40]; std::strcpy(nm, name);
    const unsigned r = norrath_construct_Skeleton(o.b, pt, (short)-6, args, nm, kind);
    // --- independent model of the name table and the random draws
    int match = -1; for (int i = 0; i < 17; ++i) if (strcasecmp(name, texts[i]) == 0) { match = i; break; }
    unsigned used = 0; auto draw = [&]() { return rands[used++ & 15]; };
    const unsigned pick = draw() % 5u;
    unsigned b968 = pick, b972 = 0;
    if (match >= 0) { b972 = exp972[match]; if (exp968[match] >= 0) b968 = exp968[match]; }
    unsigned p208 = 0, p976 = 0x5a5a5a5au, p1120 = 0x5a5a5a5au, p1124 = 0x5a5a5a5au;
    const float sc = f(0x37800000);
    if (b968 == 0) { if ((float)(int)draw() * sc < f(0x3dccca2d)) p208 = 1; p976 = (draw() & 1u) ? 77 : 79; p1120 = ((draw() & 1u) << 1) + 91; }
    else if (b968 == 1) { if ((float)(int)draw() * sc < f(0x3dccca2d)) p208 = 1; p976 = 89; }
    else if (b968 == 2) { if ((float)(int)draw() * sc < f(0x3dcccab3)) p208 = 1; p976 = (draw() & 1u) ? 85 : 87; }
    else if (b968 == 3) { if ((float)(int)draw() * sc < f(0x3dcccccc)) p208 = (draw() & 1u) + 1; p976 = (draw() & 1u) ? 95 : 97; }
    else if (b968 == 4) { if ((float)(int)draw() * sc < f(0x3dcccbc0)) p208 = (draw() & 3u) + 1; p1124 = 0; p976 = 83; }
    unsigned mask = 0; const unsigned bits = draw();
    if (bits & 1u) mask |= 1u << (draw() % 3u);
    if (b968 == 4) mask |= 0x80u; else if (bits & 2u) mask |= 1u << (draw() % 12u + 9u);
    if (!(mask & 0x4000u)) mask |= 1u << (draw() % 5u + 21u);
    mask |= 1u << ((draw() & 1u) + 5u);
    // --- expected call sequence
    std::vector<std::string> w; char b[200];
    std::snprintf(b, sizeof b, "Creature type=-6 map=%d scale=10 args=1", kind * 64); w.push_back(b);
    if (stamp_in_table != 5) { w.push_back("lump skeleton.lmp resA"); w.push_back("lump skeleton.lmp resB"); w.push_back("lump skeleton.lmp resA"); }
    for (unsigned i = 0; i < used; ++i) w.push_back("rand");
    std::snprintf(b, sizeof b, "chunks 1 %08x", mask); w.push_back(b);
    w.push_back("initType 0 b552=05060708"); w.push_back("animInit 288");
    std::snprintf(b, sizeof b, "animAdd 288 header=%u 1", b968); w.push_back(b);
    w.push_back("setState 0"); w.push_back("slow run");
    struct Ev { int a, b, c; unsigned f12, f13; };
    const Ev evs[15] = {{1,1,0,0,0},{2,1,0,0,0},{3,1,0,0,0},{14,1,0,0,0},{15,1,0,0,0},{16,1,0,0,0},{27,1,1,0,0x3f666666},{28,1,1,0,0x3f666666},{29,1,0,0,0},
                        {40,1,0,0,0},{41,1,0,0,0},{42,1,0,0,0},{53,2,1,1,0x3fa66666},{54,2,1,1,0x3fa66666},{55,2,1,1,0x3fa66666}};
    for (const Ev& e : evs) {
        std::snprintf(b, sizeof b, "events 1 %d %d %d 0 0 %08x %08x 00000000 00000000", e.a, e.b, e.c, e.f12 ? 0x3e4ccccc : 0x3e999999, e.f13);
        w.push_back(b);
    }
    w.push_back("finish");
    expect_log(w, "skeleton");
    for (int i = 0; i < 12; ++i) want.b[16 + i] = pt[i];
    want.w(36, model_low); want.w(960, 0); want.w(964, 0); want.w(72, low(norrath_ctors_lava_wraith_view_44fee8));
    for (unsigned i = 0; i < 4; ++i) want.w(1108 - 8 * i, 0xffff, 2);
    want.w(1080, 0xff, 1); want.w(1081, 0xff, 1);
    for (unsigned i = 0; i < 4; ++i) { want.w(1064 + 2 * i, 0, 2); want.w(1072 + 2 * i, 0, 2); }
    want.w(40, 20, 1); want.w(160, 0x21 | 0x10000u); want.w(164, 0x4000); want.w(172, 16, 1); want.w(168, 0x3f999999u); want.w(28, fbits(18.0f));
    want.w(248, 0xcafef00du); want.w(96, fbits(1.0f)); want.w(100, fbits(1.0f));
    for (unsigned off : {176u, 192u, 236u}) want.w(off, 0);
    want.w(572, 0, 2); want.w(968, b968); want.w(972, b972, 1); want.w(208, p208);
    if (p976 != 0x5a5a5a5au) want.w(976, p976);
    if (p1120 != 0x5a5a5a5au) want.w(1120, p1120);
    if (p1124 != 0x5a5a5a5au) want.w(1124, p1124);
    want.w(244, mask); want.w(552, 0x05060708u); want.w(556, 0x01020304u);
    expect_image(o, want, "skeleton image");
    expect(r == 0x77u, "skeleton return");
    unsigned char* t = norrath_ctors_lava_wraith_view_42d8d0;
    if (stamp_in_table != 5) {
        expect(word_at(t + 4) == 5u, "stamp rewritten");
        expect(word_at(t + 12) == low(res_a) && word_at(t + 20) == low(res_b) && word_at(t + 28) == low(res_a), "resources stored");
    } else {
        expect(word_at(t + 12) == 0 && word_at(t + 20) == 0, "table untouched when stamped");
    }
}
int main() {
    for (const char* t : texts) { run_case(t, 1, 0, 100, 3); run_case(t, 2, 5, 70000, 9); }
    for (const char* n : {"SkeletonBow", "x", "", "skeletonred "}) { run_case(n, 3, 0, 0, 0); run_case(n, 0, 5, 12345, 40000); run_case(n, 1, 7, 7, 65000); }
    return finish("skeleton constructor");
}
