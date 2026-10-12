#define NORRATH_CTORS_LAVA_WRAITH_HOST 1
#include "../push_trigger.cpp"
#include "../../ctors-lava-wraith-detail/host_support.hpp"
using namespace hs;
#define V(n) unsigned char norrath_ctors_lava_wraith_view_##n[8]
V(44c710); V(46cd20); V(484fa0); V(484fa8); V(46cd30); V(46cd40); V(46cd50); V(46cd60); V(46cd70);
static unsigned char* obj;
static int int_tag, name_mode;  // name_mode: 0 none, 1 equals 484fa8, 2 equals 46cd30, 3 other
static char other[8] = "other";
static const char* tag_name(const void* t) {
    if (t == norrath_ctors_lava_wraith_view_46cd20) return "hasPhysics";
    if (t == norrath_ctors_lava_wraith_view_484fa0) return "icon";
    if (t == norrath_ctors_lava_wraith_view_46cd40) return "off";
    if (t == norrath_ctors_lava_wraith_view_46cd50) return "half";
    if (t == norrath_ctors_lava_wraith_view_46cd60) return "val";
    if (t == norrath_ctors_lava_wraith_view_46cd70) return "script";
    return "?";
}
extern "C" unsigned norrath_game_object_construct(void* object, const void* point, void* map, char** arguments) {
    event("GameObject map=%d args=%d", map != nullptr, arguments != nullptr);
    obj = (unsigned char*)object; std::memcpy(obj + 16, point, 12); return 1;
}
extern "C" int norrath_game_object_find_tag_int(void*, char* t, int n) { event("int %s %d flags=%08x", tag_name(t), n, word_at(obj + 12)); return int_tag; }
extern "C" char* norrath_actor_find_tag_string(void*, char* t, int n) {
    event("str %s %d", tag_name(t), n);
    if ((const void*)t == norrath_ctors_lava_wraith_view_484fa0) {
        switch (name_mode) { case 0: return nullptr; case 1: return (char*)norrath_ctors_lava_wraith_view_484fa8;
            case 2: return (char*)norrath_ctors_lava_wraith_view_46cd30; default: return other; }
    }
    return (char*)t + 1;  // distinct non-null pointer per tag
}
extern "C" int strcmp(const char* l, const char* r) { event("strcmp %s %s", l == other ? "name" : (l == (const char*)norrath_ctors_lava_wraith_view_484fa8 ? "up" : "down"),
    r == (const char*)norrath_ctors_lava_wraith_view_484fa8 ? "up" : "down"); return l == r ? 0 : 1; }
extern "C" unsigned norrath_map_add_icon(const void* p, int kind, char* name, int type, char* a, char* b, char* c, char* d) {
    event("icon kind=%d name=%d type=%d p=%02x a=%d b=%d c=%d d=%d", kind, name != nullptr, type, ((const unsigned char*)p)[0],
          a == (char*)norrath_ctors_lava_wraith_view_46cd40 + 1, b == (char*)norrath_ctors_lava_wraith_view_46cd50 + 1,
          c == (char*)norrath_ctors_lava_wraith_view_46cd60 + 1, d == (char*)norrath_ctors_lava_wraith_view_46cd70 + 1);
    return 0xabc;
}
static void run_case(int tag_int, int mode, int type) {
    calls.clear(); int_tag = tag_int; name_mode = mode;
    Image<96> o, want; o.w(12, 0x100); want.w(12, 0x100);
    unsigned char pt[12] = {9, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    char* args[] = {nullptr};
    const unsigned r = norrath_construct_PushTrigger(o.b, pt, type, args);
    std::vector<std::string> w{"GameObject map=0 args=1"};
    char b[160];
    std::snprintf(b, sizeof b, "int hasPhysics 0 flags=%08x", 0x1010bu); w.push_back(b);
    w.push_back("str icon 0");
    if (mode != 0) {
        if (mode == 1) w.push_back("strcmp up up");
        else { w.push_back(mode == 2 ? "strcmp down up" : "strcmp name up"); w.push_back(mode == 2 ? "strcmp down down" : "strcmp name down"); }
        w.push_back("str off 0"); w.push_back("str half 0"); w.push_back("str val 0"); w.push_back("str script 0");
        const int kind = mode == 3 ? 1 : 0;
        std::snprintf(b, sizeof b, "icon kind=%d name=1 type=%d p=09 a=1 b=1 c=1 d=1", kind, type); w.push_back(b);
    }
    expect_log(w, "push trigger");
    for (int i = 0; i < 12; ++i) want.b[16 + i] = pt[i];
    want.w(12, 0x1010b & (tag_int ? ~1u : ~0u)); want.w(72, low(norrath_ctors_lava_wraith_view_44c710)); want.w(56, (unsigned)type, 2);
    expect_image(o, want, "push trigger image");
    expect(r == (mode ? 0xabcu : 0u), "push trigger return");
}
int main() {
    for (int t = 0; t < 2; ++t) for (int m = 0; m < 4; ++m) run_case(t, m, m == 3 ? -2 : 77);
    return finish("push trigger constructor");
}
