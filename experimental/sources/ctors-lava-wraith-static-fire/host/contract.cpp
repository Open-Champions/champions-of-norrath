#define NORRATH_CTORS_LAVA_WRAITH_HOST 1
#include "../static_fire.cpp"
#include "../../ctors-lava-wraith-detail/host_support.hpp"
using namespace hs;
#define V(n) unsigned char norrath_ctors_lava_wraith_view_##n[8]
V(449310); V(486118); V(4778c8); V(4778d8); V(486120); V(486128); V(486130);
static unsigned char* obj;
static bool present[8];
static int value[8];
static int slot(const void* t) {
    const void* v[] = {norrath_ctors_lava_wraith_view_486118, norrath_ctors_lava_wraith_view_4778c8, norrath_ctors_lava_wraith_view_4778d8,
                       norrath_ctors_lava_wraith_view_486120, norrath_ctors_lava_wraith_view_486128, norrath_ctors_lava_wraith_view_486130};
    for (int i = 0; i < 6; ++i) if (v[i] == t) return i;
    return 7;
}
static const char* names[] = {"big", "small", "wind", "radius", "density", "fire", "?", "?"};
extern "C" unsigned norrath_game_object_construct(void* object, const void* point, void* map, char** arguments) {
    event("GameObject map=%d args=%d", map != nullptr, arguments != nullptr);
    obj = (unsigned char*)object; std::memcpy(obj + 16, point, 12); return 1;
}
extern "C" void norrath_light_add(void* l) {
    event("lightAdd %d", int((unsigned char*)l - obj));
    expect(obj[136] == 0 && obj[137] == 0 && word_at(obj + 164) == fbits(120.0f), "light complete at lightAdd");
}
extern "C" char* norrath_actor_find_tag_string(void*, char* t, int) { event("str %s", names[slot(t)]); return present[slot(t)] ? t : nullptr; }
extern "C" int norrath_game_object_find_tag_int(void*, char* t, int) { event("int %s", names[slot(t)]); return value[slot(t)]; }
extern "C" void norrath_static_fire_init(void*) { event("init b111=%u b112=%u", obj[111], obj[112]); }
static void run_case(unsigned mask) {
    calls.clear();
    int v = 300;  // exercises the byte truncation
    for (int i = 0; i < 6; ++i) { present[i] = (mask >> i) & 1u; value[i] = v + i * 7; }
    Image<256> o, want;
    unsigned char pt[12];
    put(pt, fbits(-0.0f), 4); put(pt + 4, fbits(5.5f), 4); put(pt + 8, fbits(10.25f), 4);
    char* args[] = {nullptr};
    const unsigned r = norrath_construct_StaticFire(o.b, pt, 9, args);
    std::vector<std::string> w{"GameObject map=0 args=1", "lightAdd 128", "str big"};
    if (present[0]) w.push_back("int big");
    w.push_back("str small"); if (present[1]) w.push_back("int small");
    char b[100];
    std::snprintf(b, sizeof b, "init b111=%u b112=%u", present[0] ? (unsigned)(value[0] & 0xff) : 0u, present[1] ? (unsigned)(value[1] & 0xff) : 0u); w.push_back(b);
    for (int i = 2; i < 6; ++i) { w.push_back(std::string("str ") + names[i]); if (present[i]) w.push_back(std::string("int ") + names[i]); }
    expect_log(w, "static fire");
    for (int i = 0; i < 12; ++i) want.b[16 + i] = pt[i];
    want.w(72, low(norrath_ctors_lava_wraith_view_449310));
    want.w(176, fbits(-0.0f + 0.0f)); want.w(180, fbits(5.5f)); want.w(184, fbits(10.25f + 36.0f));
    want.w(137, 0, 1); want.w(136, 0, 1);
    want.w(164, fbits(120.0f)); want.w(168, 0x3891a2b3u); want.w(172, 0); want.w(156, fbits(120.0f)); want.w(160, 0x3891a2b3u);
    want.w(146, 255, 2); want.w(148, 128, 2); want.w(152, 0xfff0, 2); want.w(138, 255, 2); want.w(140, 128, 2); want.w(142, 0, 2);
    want.w(144, 0xfff0, 2); want.w(150, 0, 2);
    want.w(111, present[0] ? (unsigned)(value[0] & 0xff) : 0u, 1); want.w(112, present[1] ? (unsigned)(value[1] & 0xff) : 0u, 1);
    if (present[2]) want.w(108, (unsigned)(value[2] & 0xff), 1);
    if (present[3]) want.w(28, fbits((float)value[3]));
    if (present[4]) want.w(84, fbits((float)value[4]));
    want.w(110, present[5] ? (unsigned)(value[5] & 0xff) : 1u, 1);
    expect_image(o, want, "static fire image");
    expect(r == (present[5] ? (unsigned)value[5] : 1u), "static fire return");
}
int main() {
    for (unsigned m = 0; m < 64; ++m) run_case(m);
    return finish("static fire constructor");
}
