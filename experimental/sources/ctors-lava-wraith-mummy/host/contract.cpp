#define NORRATH_CTORS_LAVA_WRAITH_HOST 1
#include "../mummy.cpp"
#include "../../ctors-lava-wraith-detail/host_support.hpp"
#include <strings.h>
using namespace hs;
unsigned char norrath_actor_view_4844a4[4], norrath_ctors_lava_wraith_view_42d2b0[4], norrath_ctors_lava_wraith_view_44fa38[4];
#define TXT(n, t) unsigned char norrath_ctors_lava_wraith_view_##n[24] = t
TXT(472c10, "mummytemplesoldier"); TXT(472c28, "mummytso"); TXT(472c38, "mummydesertshaman"); TXT(472c50, "mummydsh");
TXT(472c60, "mummytempleshaman"); TXT(472c78, "mummytsh"); TXT(472c88, "mummyelitesoldier"); TXT(472ca0, "mummyeso");
TXT(472cb0, "mummyeliteshaman"); TXT(472cc8, "mummyesh");
static unsigned char* obj;
static unsigned char model[160], chunk_src[8];
static unsigned model_low, rands[3], rand_calls;
extern "C" int norrath_game_object_e_rand(void*) { event("rand"); return (int)rands[rand_calls++ % 3]; }
extern "C" unsigned norrath_creature_construct(void* object, const void* point, short type, void* map, float scale, char** arguments) {
    event("Creature type=%d map=%d scale=%g args=%d", type, map == norrath_ctors_lava_wraith_view_42d2b0, scale, arguments != nullptr);
    obj = (unsigned char*)object; std::memcpy(obj + 16, point, 12);
    put(obj + 36, model_low, 4); put(obj + 160, 0x21, 4); put(obj + 164, 0x4000, 4);
    return 1;
}
extern "C" int norrath_strticmp(const char* l, const char* r) { event("strticmp %s", r); return strcasecmp(l, r); }
extern "C" unsigned long long norrath_actor_model_active_chunks(void* m, int n) { event("chunks %d %d", m == chunk_src, n); return 0x0102030405060708ull; }
extern "C" void norrath_mummy_set_weapon(void*) { event("setWeapon b552=%08x", word_at(obj + 552)); }
extern "C" void norrath_mummy_init_type(void*, int f) { event("initType %d", f); }
extern "C" void norrath_actor_animation_initialize(void* s) { event("animInit %d", int((unsigned char*)s - obj)); }
extern "C" void norrath_mummy_set_state(void*, int s) { event("setState %d", s); }
extern "C" unsigned norrath_actor_add_slow_run_list(void*) { event("slow run"); return 3; }
extern "C" void norrath_creature_set_anim_event_flags(void*, int a, float b, int c, float d, int e, float ff, int g, float h, int i) {
    event("events %d %08x %d %08x %d %g %d %g %d", a, fbits(b), c, fbits(d), e, ff, g, h, i);
}
extern "C" unsigned norrath_creature_finish_construction(void*) { event("finish"); return 0x77; }
static float f(unsigned u) { float x; std::memcpy(&x, &u, 4); return x; }
static const char* texts[10] = {"mummytemplesoldier", "mummytso", "mummydesertshaman", "mummydsh", "mummytempleshaman", "mummytsh", "mummyelitesoldier", "mummyeso", "mummyeliteshaman", "mummyesh"};
static void run_case(const char* name, unsigned r0, unsigned r1, unsigned r2) {
    calls.clear(); rands[0] = r0; rands[1] = r1; rands[2] = r2; rand_calls = 0;
    Image<1128> o, want;
    unsigned char pt[12] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    char* args[] = {nullptr};
    norrath_ctors_lava_wraith::host_note(model); norrath_ctors_lava_wraith::host_note(chunk_src);
    model_low = low(model);
    put(model + 12, low(chunk_src), 4);
    put(norrath_actor_view_4844a4, 0xcafef00du, 4);
    char nm[32]; std::strcpy(nm, name);
    const unsigned r = norrath_construct_Mummy(o.b, pt, (short)77, args, nm);
    // expected selector and call order
    std::vector<std::string> w{"Creature type=77 map=1 scale=10 args=1"};
    int sel = 0, cmp = 0;
    const int group_sel[5] = {2, 1, 3, 4, 5};
    for (int g = 0; g < 5 && sel == 0; ++g) {
        for (int k = 0; k < 2; ++k) {
            w.push_back(std::string("strticmp ") + texts[g * 2 + k]);
            if (strcasecmp(name, texts[g * 2 + k]) == 0) { sel = group_sel[g]; break; }
        }
    }
    (void)cmp;
    unsigned mask = 0, b208 = 0x5a5a5a5au; unsigned used = 0;
    const float thr = f(0x3dcccccc);
    auto draw = [&](void) { w.push_back("rand"); return rands[used++ % 3]; };
    if (sel == 0) { mask = 1u << (draw() % 3u); float roll = (float)(int)draw() * f(0x37800000); if (roll < thr) b208 = (draw() & 1u) + 1u; }
    else if (sel == 2) { mask = 1u << ((draw() & 3u) + 4u); float roll = (float)(int)draw() * f(0x37800000); if (roll < thr) b208 = (draw() & 1u) + 1u; }
    else if (sel == 1) mask = 8; else if (sel == 3) mask = 256;
    else if (sel == 4) { mask = 1u << ((draw() & 1u) + 9u); float roll = (float)(int)draw() * f(0x37800000); if (roll < thr) b208 = (draw() & 3u) + 1u; }
    else mask = 2048;
    char b[100];
    std::snprintf(b, sizeof b, "chunks 1 %u", mask); w.push_back(b);
    for (const char* s : {"setWeapon b552=05060708", "initType 0", "animInit 288", "setState 0", "slow run",
                          "events 3 3dcccccc 1 00000000 0 0 0 0 0", "events 4 3e199999 1 00000000 0 0 0 0 0", "events 32 3e4ccccc 1 00000000 0 0 0 0 0",
                          "events 33 3fc00000 1 00000000 0 0 0 0 0", "events 34 3e4ccccc 1 00000000 0 0 0 0 0", "events 14 3dcccccc 1 00000000 0 0 0 0 0",
                          "events 15 3dcccccc 1 00000000 0 0 0 0 0", "events 16 3dcccccc 1 3f000000 1 0 0 0 0", "events 17 3dcccccc 1 00000000 0 0 0 0 0",
                          "events 18 3dcccccc 1 00000000 0 0 0 0 0", "events 19 3dcccccc 1 3f000000 1 0 0 0 0", "events 43 3e4ccccc 1 3f4ccccc 2 0 0 0 0", "finish"}) w.push_back(s);
    expect_log(w, "mummy");
    for (int i = 0; i < 12; ++i) want.b[16 + i] = pt[i];
    want.w(36, model_low); want.w(72, low(norrath_ctors_lava_wraith_view_44fa38));
    for (unsigned i = 0; i < 4; ++i) want.w(1092 - 8 * i, 0xffff, 2);
    want.w(1064, 0xff, 1); want.w(1065, 0xff, 1);
    for (unsigned i = 0; i < 4; ++i) { want.w(1048 + 2 * i, 0, 2); want.w(1056 + 2 * i, 0, 2); }
    want.w(40, 30, 1); want.w(160, 0x21 | 0x10000u); want.w(164, 0x4000 | 4u); want.w(168, 0x3f999999u); want.w(248, 0xcafef00du);
    want.w(172, 16, 1); want.w(28, fbits(15.0f));
    for (unsigned off : {176u, 192u, 236u, 1104u, 1108u, 1112u}) want.w(off, 0);
    want.w(572, 0, 2); want.w(960, (unsigned)sel); want.w(244, mask); want.w(552, 0x05060708u); want.w(556, 0x01020304u);
    if (b208 != 0x5a5a5a5au) want.w(208, b208);
    expect_image(o, want, "mummy image");
    expect(r == 0x77u, "mummy return");
}
int main() {
    const char* names[] = {"mummytemplesoldier", "MUMMYTSO", "mummydesertshaman", "mummydsh", "mummytempleshaman", "mummytsh",
                           "mummyelitesoldier", "mummyeso", "mummyeliteshaman", "MummyEsh", "other", ""};
    for (const char* n : names) { run_case(n, 5, 100, 7); run_case(n, 0xfffffff0u, 70000, 2); run_case(n, 4, 65000, 6); }
    return finish("mummy constructor");
}
