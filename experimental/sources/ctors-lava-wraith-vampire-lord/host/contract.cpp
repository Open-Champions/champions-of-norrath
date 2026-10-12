#define NORRATH_CTORS_LAVA_WRAITH_HOST 1
#include "../vampire_lord.cpp"
#include "../../ctors-lava-wraith-detail/host_support.hpp"
using namespace hs;
unsigned char norrath_actor_view_4844a4[4], norrath_ctors_lava_wraith_view_42d700[4], norrath_ctors_lava_wraith_view_44fd08[4];
unsigned char norrath_ctors_lava_wraith_view_4858c0[8], norrath_ctors_lava_wraith_view_4858c8[8];
static unsigned char* obj;
static unsigned char model[160], header[8], chunk_src[8];
static unsigned model_low;
static int wings;
extern "C" unsigned norrath_creature_construct(void* object, const void* point, short type, void* map, float scale, char** arguments) {
    event("Creature type=%d map=%d scale=%g args=%d", type, map == norrath_ctors_lava_wraith_view_42d700, scale, arguments != nullptr);
    obj = (unsigned char*)object; std::memcpy(obj + 16, point, 12);
    put(obj + 36, model_low, 4); put(obj + 160, 0x21, 4); put(obj + 164, 0x4000, 4); put(obj + 808, 0x1000, 4);
    return 1;
}
extern "C" void norrath_creature_set_parameters(void*, float a, float b, float c, float d, float e, int flag) { event("params %g %08x %08x %08x %g %d", a, fbits(b), fbits(c), fbits(d), e, flag); }
extern "C" void norrath_actor_animation_initialize(void* s) { event("animInit %d", int((unsigned char*)s - obj)); }
extern "C" void norrath_actor_animation_add(void* s, void* h, int fl) { event("animAdd %d %d %d", int((unsigned char*)s - obj), h == header, fl); }
extern "C" void norrath_actor_animation_randomize_time(void* s) { event("randomize %d", int((unsigned char*)s - obj)); }
extern "C" unsigned norrath_actor_add_slow_run_list(void*) { event("slow run"); return 3; }
extern "C" void* norrath_lump_load(char* n) { event("lumpLoad %d", n == (char*)norrath_ctors_lava_wraith_view_4858c0); return nullptr; }
extern "C" void norrath_creature_set_anim_event_flags(void*, int a, float b, int c, float d, int e, float ff, int g, float h, int i) {
    event("events %d %08x %d %08x %d %08x %d %08x %d", a, fbits(b), c, fbits(d), e, fbits(ff), g, fbits(h), i);
}
extern "C" int norrath_game_object_find_tag_int(void*, char* t, int n) { event("tagInt %d %d b244=%u b960=%u", t == (char*)norrath_ctors_lava_wraith_view_4858c8, n, word_at(obj + 244), word_at(obj + 960)); return wings; }
extern "C" unsigned long long norrath_actor_model_active_chunks(void* m, int n) { event("chunks %d %d", m == chunk_src, n); return 0x0102030405060708ull; }
extern "C" unsigned norrath_creature_finish_construction(void*) { event("finish b552=%08x", word_at(obj + 552)); return 0x77; }
static void run_case(int wings_value) {
    calls.clear(); wings = wings_value;
    Image<1180> o, want;
    unsigned char pt[12] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    char* args[] = {nullptr};
    norrath_ctors_lava_wraith::host_note(model); norrath_ctors_lava_wraith::host_note(header); norrath_ctors_lava_wraith::host_note(chunk_src);
    model_low = low(model);
    put(model + 44, low(header), 4); put(model + 12, low(chunk_src), 4);
    put(norrath_actor_view_4844a4, 0xcafef00du, 4);
    const unsigned r = norrath_construct_VampireLord(o.b, pt, (short)-9, args);
    const unsigned mask = wings_value == 0 ? 2u : 0u;
    char b[200];
    std::vector<std::string> w{"Creature type=-9 map=1 scale=10 args=1", "params 24 3fb33333 3ff99999 40866666 10.5 1", "animInit 288", "animAdd 288 1 1", "randomize 288", "slow run", "lumpLoad 1"};
    struct Ev { int a; unsigned b; int c; unsigned d; int e; unsigned f; int g; unsigned h; int i; };
    const Ev evs[] = {
        {18, 0x3e999999u, 1, 0x3fc00000u, 2, 0, 0, 0, 0}, {13, 0x3e999999u, 1, 0x3effffdeu, 2, 0, 0, 0, 0},
        {14, 0x3e4cccccu, 1, 0x3f1999aau, 2, 0, 0, 0, 0}, {15, 0x3f333333u, 1, 0x3f999913u, 2, 0, 0, 0, 0},
        {12, 0x3f666666u, 1, 0x3f999999u, 2, 0, 0, 0, 0}, {9, 0x3f000000u, 1, 0x40000000u, 2, 0x3e999999u, 4, 0x3eccccccu, 8},
        {8, 0x3f999999u, 1, 0x3fe66666u, 2, 0, 0, 0, 0}, {4, 0x3f333333u, 1, 0x3f800000u, 2, 0, 0, 0, 0},
        {20, 0x40000000u, 64, 0x40066666u, 128, 0, 0, 0, 0}};
    for (const Ev& e : evs) { std::snprintf(b, sizeof b, "events %d %08x %d %08x %d %08x %d %08x %d", e.a, e.b, e.c, e.d, e.e, e.f, e.g, e.h, e.i); w.push_back(b); }
    w.push_back("tagInt 1 0 b244=0 b960=0");
    std::snprintf(b, sizeof b, "chunks 1 %u", mask); w.push_back(b);
    w.push_back("finish b552=05060708");
    expect_log(w, "vampire lord");
    for (int i = 0; i < 12; ++i) want.b[16 + i] = pt[i];
    want.w(36, model_low); want.w(808, 0x1000); want.w(1028, 0); want.w(72, low(norrath_ctors_lava_wraith_view_44fd08));
    for (unsigned i = 0; i < 4; ++i) want.w(1156 - 8 * i, 0xffff, 2);
    want.w(1128, 0xff, 1); want.w(1129, 0xff, 1);
    for (unsigned i = 0; i < 4; ++i) { want.w(1112 + 2 * i, 0, 2); want.w(1120 + 2 * i, 0, 2); }
    want.w(160, 0x21 | 0x20000000u); want.w(164, 0x4000 | 0x204u);
    want.w(988, 0x1000 + 300); want.w(976, 0x1000 + 300);
    want.w(172, 16, 1); want.w(248, 0xcafef00du); want.w(168, 0x3fa66666u); want.w(28, fbits(18.0f)); want.w(88, 0xbe4cccccu);
    want.w(104, 0x3e999999u); want.w(100, fbits(1.0f)); want.w(96, fbits(1.0f));
    for (unsigned off : {176u, 236u, 964u, 968u, 980u, 984u, 992u, 1008u, 1012u, 1016u, 1020u, 1024u, 960u}) want.w(off, 0);
    want.w(572, 0, 2); want.w(244, mask); want.w(552, 0x05060708u); want.w(556, 0x01020304u);
    expect_image(o, want, "vampire lord image");
    expect(r == 0x77u, "vampire lord return");
}
int main() { run_case(0); run_case(1); run_case(-5); return finish("vampire lord constructor"); }
