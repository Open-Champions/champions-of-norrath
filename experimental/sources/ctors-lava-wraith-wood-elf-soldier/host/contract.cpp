#define NORRATH_CTORS_LAVA_WRAITH_HOST 1
#include "../wood_elf_soldier.cpp"
#include "../../ctors-lava-wraith-detail/host_support.hpp"
#include <strings.h>
using namespace hs;
unsigned char norrath_actor_view_4844a4[4], norrath_ctors_lava_wraith_view_42dc40[4], norrath_ctors_lava_wraith_view_44af90[4];
unsigned char norrath_ctors_lava_wraith_view_474540[16] = "woodelfsoldier", norrath_ctors_lava_wraith_view_474550[16] = "woodelfarcher";
unsigned char norrath_ctors_lava_wraith_view_474560[16] = "woodelfspear", norrath_ctors_lava_wraith_view_474570[16] = "woodelfscout";
static unsigned char* obj;
static unsigned char model[160], chunk_src[8];
static unsigned model_low, rands[3], rand_calls;
extern "C" int norrath_game_object_e_rand(void*) { event("rand"); return (int)rands[rand_calls++ % 3]; }
extern "C" unsigned norrath_creature_construct(void* object, const void* point, short type, void* map, float scale, char** arguments) {
    event("Creature type=%d map=%d scale=%g args=%d", type, map == norrath_ctors_lava_wraith_view_42dc40, scale, arguments != nullptr);
    obj = (unsigned char*)object; std::memcpy(obj + 16, point, 12);
    put(obj + 36, model_low, 4); put(obj + 160, 0x21, 4); put(obj + 164, 0x4000, 4); put(obj + 12, 0x40, 4); put(obj + 244, 0x3, 4);
    return 1;
}
extern "C" void norrath_creature_set_parameters(void*, float a, float b, float c, float d, float e, int flag) { event("params %g %g %08x %08x %g %d b988=%u", a, b, fbits(c), fbits(d), e, flag, word_at(obj + 988)); }
extern "C" int norrath_strticmp(const char* l, const char* r) { event("strticmp %s", r); return strcasecmp(l, r); }
extern "C" void norrath_wood_elf_soldier_init_type(void*) { event("initType b964=%u", word_at(obj + 964)); }
extern "C" unsigned long long norrath_actor_model_active_chunks(void* m, int n) { event("chunks %d %d", m == chunk_src, n); return 0x0102030405060708ull; }
extern "C" void norrath_actor_animation_initialize(void* s) { event("animInit %d b552=%08x", int((unsigned char*)s - obj), word_at(obj + 552)); }
extern "C" void norrath_wood_elf_soldier_set_state(void*, int s) { event("setState %d", s); }
extern "C" unsigned norrath_actor_add_run_list(void*) { event("run b236=%u b256=%u", word_at(obj + 236), word_at(obj + 256)); return 3; }
extern "C" unsigned norrath_actor_add_slow_run_list(void*) { event("slow run b236=%u", word_at(obj + 236)); return 3; }
extern "C" void norrath_creature_set_anim_event_flags(void*, int a, float b, int c, float d, int e, float ff, int g, float h, int i) {
    event("events %d %08x %d %08x %d %08x %d %g %d", a, fbits(b), c, fbits(d), e, fbits(ff), g, h, i);
}
extern "C" unsigned norrath_creature_finish_construction(void*) { event("finish"); return 0x77; }
static void run_case(const char* name, unsigned r0, unsigned r1, unsigned r2) {
    calls.clear(); rands[0] = r0; rands[1] = r1; rands[2] = r2; rand_calls = 0;
    Image<1000> o, want;
    unsigned char pt[12] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    char* args[] = {nullptr};
    norrath_ctors_lava_wraith::host_note(chunk_src); norrath_ctors_lava_wraith::host_note(model);
    model_low = low(model);
    put(model + 12, low(chunk_src), 4);
    put(norrath_actor_view_4844a4, 0xcafef00du, 4);
    char nm[32]; std::strcpy(nm, name);
    const unsigned r = norrath_construct_WoodElfSoldier(o.b, pt, (short)-2, args, nm);
    const float scale = (float)(int)(r0 % 14u + 4u);
    unsigned pick = r1 % 3u & 0xffffu; unsigned draws = 2;
    if (pick != 0) { ++draws; if (r2 % 3u != 0u) pick = 0; }
    int matched = -1;
    const char* names[] = {"woodelfsoldier", "woodelfarcher", "woodelfspear", "woodelfscout"};
    std::vector<std::string> w;
    char b[160];
    w.push_back("rand");
    std::snprintf(b, sizeof b, "Creature type=-2 map=1 scale=%g args=1", scale); w.push_back(b);
    w.push_back("params 2 2 3e999999 3e999999 1 1 b988=0");
    for (unsigned i = 1; i < draws; ++i) w.push_back("rand");
    for (int i = 0; i < 4 && matched < 0; ++i) { w.push_back(std::string("strticmp ") + names[i]); if (strcasecmp(name, names[i]) == 0) matched = i; }
    if (matched >= 0) pick = (unsigned)matched;
    std::snprintf(b, sizeof b, "initType b964=%u", pick); w.push_back(b);
    w.push_back("chunks 1 3");
    w.push_back("animInit 288 b552=05060708");
    w.push_back("setState 0");
    if (pick == 3) w.push_back("run b236=1 b256=0"); else w.push_back("slow run b236=0");
    for (const char* s : {"events 17 3e999999 1 00000000 0 00000000 0 0 0", "events 24 3ef5c28f 1 3f8ccccc 2 00000000 0 0 0",
                          "events 25 40499999 1 40799999 2 00000000 0 0 0", "events 34 3e999999 1 3fc00000 2 00000000 0 0 0",
                          "events 27 3e999999 1 00000000 0 00000000 0 0 0", "events 35 3e999999 1 00000000 0 00000000 0 0 0",
                          "events 38 3f000000 16 00000000 0 00000000 0 0 0", "finish"}) w.push_back(s);
    expect_log(w, "wood elf soldier");
    for (int i = 0; i < 12; ++i) want.b[16 + i] = pt[i];
    want.w(36, model_low); want.w(40, 40, 1); want.w(160, 0x21 | 0x00040000u); want.w(164, 0x4000 | 8u); want.w(12, 0x40 | 0x01000000u);
    want.w(244, 3); want.w(248, 0xcafef00du); want.w(176, fbits(100.0f)); want.w(168, 0x3f999999u); want.w(72, low(norrath_ctors_lava_wraith_view_44af90));
    want.w(28, fbits(20.0f)); want.w(236, pick == 3 ? 1u : 0u); want.w(572, 0, 2); want.w(960, 0); want.w(988, 0);
    want.w(792, 0xffff, 2); want.w(968, 0); want.w(964, pick); want.w(552, 0x05060708u); want.w(556, 0x01020304u); want.w(256, 0);
    expect_image(o, want, "wood elf soldier image");
    expect(r == 0x77u, "wood elf soldier return");
}
int main() {
    const char* all[] = {"woodelfsoldier", "WoodElfArcher", "woodelfspear", "WOODELFSCOUT", "other", ""};
    for (const char* n : all) { run_case(n, 100, 0, 0); run_case(n, 13, 2, 1); run_case(n, 5, 1, 3); run_case(n, 0xfffffff0u, 4, 2); }
    return finish("wood elf soldier constructor");
}
