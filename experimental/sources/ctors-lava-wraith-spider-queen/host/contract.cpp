#define NORRATH_CTORS_LAVA_WRAITH_HOST 1
#include "../spider_queen.cpp"
#include "../../ctors-lava-wraith-detail/host_support.hpp"
using namespace hs;
unsigned char norrath_actor_view_4844a4[4], norrath_ctors_lava_wraith_view_42c228[4], norrath_ctors_lava_wraith_view_44b350[4];
static unsigned char* obj;
static unsigned char model[160], header[8], chunk_src[8];
static unsigned model_low, rands[2], rand_calls;
extern "C" int norrath_game_object_e_rand(void*) { event("rand"); return (int)rands[rand_calls++ & 1]; }
extern "C" unsigned norrath_creature_construct(void* object, const void* point, short type, void* map, float scale, char** arguments) {
    event("Creature type=%d map=%d scale=%g args=%d", type, map == norrath_ctors_lava_wraith_view_42c228, scale, arguments != nullptr);
    obj = (unsigned char*)object; std::memcpy(obj + 16, point, 12);
    put(obj + 36, model_low, 4); put(obj + 160, 0x21, 4); put(obj + 164, 0x4000, 4);
    return 1;
}
extern "C" void norrath_web_construct(void* w, int a, int b) { event("Web %d %d %d b56=%u b72=%08x", int((unsigned char*)w - obj), a, b, obj[56] | obj[57] << 8, word_at(obj + 72)); }
extern "C" void norrath_creature_set_parameters(void*, float a, float b, float c, float d, float e, int flag) { event("params %g %08x %g %08x %g %d b964=%u", a, fbits(b), c, fbits(d), e, flag, word_at(obj + 964)); }
extern "C" void norrath_actor_animation_initialize(void* s) { event("animInit %d", int((unsigned char*)s - obj)); }
extern "C" void norrath_actor_animation_add(void* s, void* h, int fl) { event("animAdd %d %d %d", int((unsigned char*)s - obj), h == header, fl); }
extern "C" void norrath_actor_animation_randomize_time(void* s) { event("randomize %d", int((unsigned char*)s - obj)); }
extern "C" unsigned long long norrath_actor_model_active_chunks(void* m, int n) { event("chunks %d %d", m == chunk_src, n); return 0x0102030405060708ull; }
extern "C" unsigned norrath_actor_add_slow_run_list(void*) { event("slow run b552=%08x", word_at(obj + 552)); return 3; }
extern "C" void norrath_creature_set_anim_event_flags(void*, int a, float b, int c, float d, int e, float ff, int g, float h, int i) {
    event("events %d %08x %d %08x %d %08x %d %g %d", a, fbits(b), c, fbits(d), e, fbits(ff), g, h, i);
}
extern "C" unsigned norrath_creature_finish_construction(void*) { event("finish"); return 0x77; }
static void run_case(unsigned r0, unsigned r1) {
    calls.clear(); rands[0] = r0; rands[1] = r1; rand_calls = 0;
    Image<1100> o, want;
    unsigned char pt[12] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    char* args[] = {nullptr};
    norrath_ctors_lava_wraith::host_note(model); norrath_ctors_lava_wraith::host_note(header); norrath_ctors_lava_wraith::host_note(chunk_src);
    model_low = low(model);
    put(model + 92, low(header), 4); put(model + 12, low(chunk_src), 4);
    put(norrath_actor_view_4844a4, 0xcafef00du, 4);
    const unsigned r = norrath_construct_SpiderQueen(o.b, pt, (short)-70, args);
    const float scale = (float)(int)((r0 & 7u) + (r1 & 7u) + 32u);
    char b[160];
    std::vector<std::string> w{"rand", "rand"};
    std::snprintf(b, sizeof b, "Creature type=-70 map=1 scale=%g args=1", scale); w.push_back(b);
    std::snprintf(b, sizeof b, "Web 968 41 240 b56=23130 b72=%08x", low(norrath_ctors_lava_wraith_view_44b350)); w.push_back(b);
    w.push_back("params 68 3fb33333 4 40866666 10.5 1 b964=0"); w.push_back("animInit 288"); w.push_back("animAdd 288 1 1"); w.push_back("randomize 288");
    w.push_back("chunks 1 0"); w.push_back("slow run b552=05060708");
    for (const char* s : {"events 16 3e999999 1 3f000000 1 3f333333 1 0 0", "events 17 3fa66666 1 40200000 1 40266666 1 0 0",
                          "events 12 3e4ccccc 1 3f666666 2 3f8ccccc 1 0 0", "events 21 3ecccccc 1 00000000 0 00000000 0 0 0",
                          "events 13 3f000000 1 3fc00000 2 00000000 0 0 0", "events 14 3f000000 1 3fc00000 2 00000000 0 0 0",
                          "events 15 3f000000 1 3fc00000 2 00000000 0 0 0", "events 4 3e4ccccc 1 00000000 0 00000000 0 0 0",
                          "events 6 3e999999 1 3fa66666 2 40000000 4 0 0", "events 5 40c00000 8 00000000 0 00000000 0 0 0", "finish"}) w.push_back(s);
    expect_log(w, "spider queen");
    for (int i = 0; i < 12; ++i) want.b[16 + i] = pt[i];
    want.w(36, model_low); want.w(72, low(norrath_ctors_lava_wraith_view_44b350)); want.w(56, 0xffba, 2); want.w(40, 100, 1);
    want.w(160, 0x21 | 0x20002000u); want.w(164, 0x4000 | 0x80u); want.w(248, 0xcafef00du); want.w(172, 19, 1);
    want.w(176, fbits(120.0f)); want.w(168, 0x3f333333u); want.w(28, fbits(70.0f)); want.w(788, fbits(2000.0f)); want.w(228, fbits(-50.0f));
    want.w(96, fbits(1.0f)); want.w(224, fbits(1.0f)); want.w(236, 0); want.w(572, 0, 2); want.w(964, 0);
    for (unsigned off = 1028; off <= 1056; off += 4) want.w(off, 0);
    want.w(552, 0x05060708u); want.w(556, 0x01020304u);
    expect_image(o, want, "spider queen image");
    expect(r == 0x77u, "spider queen return");
}
int main() { run_case(0, 0); run_case(7, 15); run_case(0xfffffff9u, 3); return finish("spider queen constructor"); }
