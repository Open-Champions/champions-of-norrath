#define NORRATH_CTORS_LAVA_WRAITH_HOST 1
#include "../weapon_rack.cpp"
#include "../../ctors-lava-wraith-detail/host_support.hpp"
using namespace hs;
unsigned char norrath_ctors_lava_wraith_view_448fb0[4], norrath_creature_view_484cac[4], norrath_ctors_lava_wraith_view_4853d0[8], norrath_ctors_lava_wraith_view_4853d8[8];
static unsigned char* obj;
static float ramp, tag_float, global_float;
static int random_value, tag_present, create_result;
static float f(unsigned u) { float x; std::memcpy(&x, &u, 4); return x; }
extern "C" unsigned norrath_game_object_construct(void* object, const void* point, void* map, char** arguments) {
    event("GameObject map=%d args=%d", map != nullptr, arguments != nullptr);
    obj = (unsigned char*)object; std::memcpy(obj + 16, point, 12); return 1;
}
extern "C" float norrath_player_treasure_ramp() { event("ramp flags=%08x", word_at(obj + 12)); return ramp; }
extern "C" int norrath_game_object_e_rand(void*) { event("rand"); return random_value; }
extern "C" char* norrath_actor_find_tag_string(void*, char* t, int n) { event("tag %d %d", t == (char*)norrath_ctors_lava_wraith_view_4853d0, n); return tag_present ? t : nullptr; }
extern "C" float norrath_game_object_find_tag_float(void*, char* t, int n) { event("tagf %d %d", t == (char*)norrath_ctors_lava_wraith_view_4853d0, n); return tag_float; }
extern "C" int norrath_item_create_random(void* d, int level, int a, int b, int c) {
    event("random %d level=%d %d %d %d", int((unsigned char*)d - obj), level, a, b, c);
    // the description is already initialised when createRandom runs
    expect(obj[112] == 0xff && obj[113] == 0xff, "description marker bytes");
    for (int i = 96; i < 112; ++i) if (i != 112) expect(obj[i] == 0, "description zero halves");
    expect(obj[84] == 0x5a, "item type untouched before the fallback");
    return create_result;
}
extern "C" unsigned norrath_item_create(char* name, void* d, int a, int b) {
    event("create %d %d %d %d", name == (char*)norrath_ctors_lava_wraith_view_4853d8, int((unsigned char*)d - obj), a, b); return 5;
}
static void run_case(float r_ramp, float g, int rnd, bool tag, float tf, int created) {
    calls.clear(); ramp = r_ramp; global_float = g; random_value = rnd; tag_present = tag; tag_float = tf; create_result = created;
    put(norrath_creature_view_484cac, fbits(g), 4);
    Image<160> o, want; o.w(12, 0x44); want.w(12, 0x44 | 0x100001);
    unsigned char pt[12] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    char* args[] = {nullptr};
    const unsigned r = norrath_construct_WeaponRack(o.b, pt, -7, args);
    float level = r_ramp * (((float)rnd * f(0x37800000u)) * f(0x3dccccccu) + f(0x3f733333u));
    level = level * ((g * f(0x3e4cccccu) + f(0x3f4cccccu)) * 200.0f);
    if (tag) level = level * tf;
    std::vector<std::string> w{"GameObject map=0 args=1", "ramp flags=00100045", "rand", "tag 1 0"};
    char b[128];
    if (tag) w.push_back("tagf 1 0");
    std::snprintf(b, sizeof b, "random 80 level=%d 0 -1 1", (int)level); w.push_back(b);
    if (!created) { w.push_back("create 1 80 -1 0"); }
    expect_log(w, "weapon rack");
    for (int i = 0; i < 12; ++i) want.b[16 + i] = pt[i];
    want.w(72, low(norrath_ctors_lava_wraith_view_448fb0)); want.w(56, 0xfff9, 2); want.w(40, 40, 1);
    for (int i = 0; i < 4; ++i) want.w(116 + 8 * i, 0xffff, 2);
    want.w(112, 0xffff, 2);
    for (int i = 0; i < 4; ++i) { want.w(96 + 2 * i, 0, 2); want.w(104 + 2 * i, 0, 2); }
    if (!created) want.w(84, 40, 2);
    expect(r == (created ? (unsigned)created : 40u), "weapon rack return");
    expect_image(o, want, "weapon rack image");
}
int main() {
    run_case(2.5f, 1.0f, 0x12345, false, 1.0f, 3);
    run_case(1.75f, 0.5f, 0, true, 0.6f, 0);
    run_case(1.0f, 1.0f, 65535, true, 2.0f, -1);
    return finish("weapon rack constructor");
}
