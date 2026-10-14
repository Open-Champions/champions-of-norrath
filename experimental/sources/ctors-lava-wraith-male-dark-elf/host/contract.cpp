#define NORRATH_CTORS_LAVA_WRAITH_HOST 1
#include "../male_dark_elf.cpp"
#include "../../ctors-lava-wraith-detail/host_support.hpp"
#include <strings.h>
using namespace hs;
unsigned char norrath_actor_view_4844a4[4], norrath_ctors_lava_wraith_view_438c70[4], norrath_ctors_lava_wraith_view_450440[4];
#define TXT(n, t) unsigned char norrath_ctors_lava_wraith_view_##n[24] = t
TXT(477568, "maledarkelfarcher"); TXT(477580, "maledarkelfspear"); TXT(477598, "maledarkelfsoldier"); TXT(4775b0, "maledarkelfknight");
TXT(4775c8, "maledarkelfstar"); TXT(4775d8, "maledarkelfassassin");
static const char* texts[6] = {"maledarkelfarcher", "maledarkelfspear", "maledarkelfsoldier", "maledarkelfknight", "maledarkelfstar", "maledarkelfassassin"};
static unsigned char* obj;
static unsigned char model[160], chunk_src[8];
static unsigned model_low, rands[8], rand_calls;
static float f(unsigned u) { float x; std::memcpy(&x, &u, 4); return x; }
extern "C" int norrath_game_object_e_rand(void*) { event("rand"); return (int)rands[rand_calls++ & 7]; }
extern "C" unsigned norrath_creature_construct(void* object, const void* point, short type, void* map, float scale, char** arguments) {
    event("Creature type=%d map=%d scale=%g args=%d", type, map == norrath_ctors_lava_wraith_view_438c70, scale, arguments != nullptr);
    obj = (unsigned char*)object; std::memcpy(obj + 16, point, 12);
    put(obj + 36, model_low, 4);
    return 1;
}
extern "C" int norrath_strticmp(const char* l, const char* r) { event("strticmp %s", r); return strcasecmp(l, r); }
extern "C" void norrath_male_dark_elf_init_type(void*, int fl) { event("initType %d", fl); }
extern "C" unsigned long long norrath_actor_model_active_chunks(void* m, int n) { event("chunks %d %d", m == chunk_src, n); return 0x0102030405060708ull; }
extern "C" void norrath_actor_animation_initialize(void* s) { event("animInit %d b552=%08x", int((unsigned char*)s - obj), word_at(obj + 552)); }
extern "C" void norrath_male_dark_elf_set_state(void*, int s) { event("setState %d", s); }
extern "C" unsigned norrath_actor_add_slow_run_list(void*) { event("slow run"); return 3; }
extern "C" void norrath_creature_set_anim_event_flags(void*, int a, float b, int c, float d, int e, float ff, int g, float h, int i) {
    event("events %d %08x %d %08x %d %08x %d %g %d", a, fbits(b), c, fbits(d), e, fbits(ff), g, h, i);
}
extern "C" unsigned norrath_creature_finish_construction(void*) { event("finish"); return 0x77; }
static void run_case(const char* name, unsigned base) {
    calls.clear();
    for (unsigned i = 0; i < 8; ++i) rands[i] = base * (i + 3) + i * 40000u + 1u;
    rand_calls = 0;
    Image<1128> o, want;
    unsigned char pt[12] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    char* args[] = {nullptr};
    norrath_ctors_lava_wraith::host_note(model); norrath_ctors_lava_wraith::host_note(chunk_src);
    model_low = low(model);
    put(model + 12, low(chunk_src), 4);
    put(norrath_actor_view_4844a4, 0xcafef00du, 4);
    char nm[40]; std::strcpy(nm, name);
    const unsigned r = norrath_construct_MaleDarkElf(o.b, pt, (short)-8, args, nm);
    std::vector<std::string> w{"Creature type=-8 map=1 scale=10 args=1", "rand"};
    unsigned used = 1; auto draw = [&]() { w.push_back("rand"); return rands[used++ & 7]; };
    unsigned variant = rands[0] % 6u; bool matched = false;
    for (int i = 0; i < 6 && !matched; ++i) { w.push_back(std::string("strticmp ") + texts[i]); if (strcasecmp(name, texts[i]) == 0) { variant = i; matched = true; } }
    const float thr = f(0x3dcccccc), sc = f(0x37800000);
    unsigned mask = 0, p964 = 0, p968 = 0, p1112 = 0, p208 = 0;
    auto roll = [&](void) { return (float)(int)draw() * sc < thr; };
    switch (variant) {
    case 0: { p964 = 64; mask = (1u << (draw() & 3u)) | 0x08000000u; if (roll()) p208 = draw() % 5u + 1u; break; }
    case 1: { p964 = 52; mask = 1u << (draw() % 6u + 4u); if (roll()) p208 = (draw() & 3u) + 1u; break; }
    case 2: { p964 = 62; mask = 1u << (draw() % 5u + 16u); p1112 = (draw() % 3u) * 2u + 46u; if (roll()) p208 = (draw() & 3u) + 1u; break; }
    case 3: { p964 = 58; mask = 1u << (draw() % 6u + 10u); if (roll()) p208 = (draw() & 3u) + 1u; break; }
    case 4: { p964 = 54; p968 = 56; mask = 1u << (draw() % 6u + 21u); break; }
    default: { p964 = 62; p968 = 60; mask = 1u << (draw() % 6u + 21u); if (roll()) p208 = draw() % 3u + 1u; break; }
    }
    char b[100];
    w.push_back("initType 0");
    std::snprintf(b, sizeof b, "chunks 1 %u", mask); w.push_back(b);
    w.push_back("animInit 288 b552=05060708"); w.push_back("setState 0"); w.push_back("slow run");
    for (const char* s : {"events 19 3dcccccc 1 00000000 0 00000000 0 0 0", "events 20 3dcccccc 1 00000000 0 00000000 0 0 0", "events 21 3dcccccc 1 00000000 0 00000000 0 0 0",
                          "events 13 3dcccccc 1 00000000 0 00000000 0 0 0", "events 14 3dcccccc 1 00000000 0 00000000 0 0 0", "events 15 3dcccccc 1 00000000 0 00000000 0 0 0",
                          "events 28 3dcccccc 1 00000000 0 00000000 0 0 0", "events 29 3dcccccc 1 00000000 0 00000000 0 0 0",
                          "events 34 3e4ccccc 1 3f8ccccc 2 00000000 0 0 0", "events 11 3f000000 2 3f199999 4 3f800000 1 0 0",
                          "events 12 3e4ccccc 2 3f199999 1 00000000 0 0 0", "events 3 3dcccccc 1 3e4ccccc 1 3ecccccc 1 0 0",
                          "events 4 3dcccccc 1 00000000 0 00000000 0 0 0", "events 38 3f800000 16 00000000 0 00000000 0 0 0",
                          "events 40 3f800000 16 00000000 0 00000000 0 0 0", "events 42 3f800000 16 00000000 0 00000000 0 0 0",
                          "events 44 3f800000 16 00000000 0 00000000 0 0 0", "finish"}) w.push_back(s);
    expect_log(w, "male dark elf");
    for (int i = 0; i < 12; ++i) want.b[16 + i] = pt[i];
    want.w(36, model_low); want.w(72, low(norrath_ctors_lava_wraith_view_450440));
    for (unsigned i = 0; i < 4; ++i) want.w(1100 - 8 * i, 0xffff, 2);
    want.w(1072, 0xff, 1); want.w(1073, 0xff, 1);
    for (unsigned i = 0; i < 4; ++i) { want.w(1056 + 2 * i, 0, 2); want.w(1064 + 2 * i, 0, 2); }
    want.w(40, 20, 1); want.w(168, 0x3f999999u); want.w(172, 9, 1); want.w(28, fbits(15.0f)); want.w(248, 0xcafef00du);
    for (unsigned off : {176u, 192u, 236u}) want.w(off, 0);
    want.w(572, 0, 2); want.w(960, variant, 1); want.w(244, mask); want.w(964, p964); want.w(968, p968); want.w(1112, p1112); want.w(208, p208);
    want.w(552, 0x05060708u); want.w(556, 0x01020304u);
    expect_image(o, want, "male dark elf image");
    expect(r == 0x77u, "male dark elf return");
}
int main() {
    const char* names[] = {"maledarkelfarcher", "MaleDarkElfSpear", "maledarkelfsoldier", "maledarkelfknight", "maledarkelfstar", "maledarkelfassassin", "other", ""};
    unsigned bases[] = {1, 7, 100, 33333, 0x7ffffff0u};
    for (const char* n : names) for (unsigned base : bases) run_case(n, base);
    return finish("male dark elf constructor");
}
