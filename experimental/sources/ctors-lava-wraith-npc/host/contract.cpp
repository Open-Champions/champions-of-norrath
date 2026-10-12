#define NORRATH_CTORS_LAVA_WRAITH_HOST 1
#include "../npc.cpp"
#include "../../ctors-lava-wraith-detail/host_support.hpp"
#include <cstdarg>
using namespace hs;
#define TXT(n, text) unsigned char norrath_ctors_lava_wraith_view_##n[16] = text
TXT(44adb0, "vt"); TXT(4857e8, "name"); TXT(4857f0, "npc"); TXT(472050, "lumpname"); TXT(4857f8, "%s.lmp");
TXT(485800, "npc.lmp"); TXT(485808, "wakname"); TXT(485810, "%s.vif"); TXT(485818, "rawname"); TXT(485820, "%s.tex");
TXT(485828, "idle"); TXT(472060, "change%d"); TXT(472040, "showName");
static unsigned char* obj;
static const char *tag_name, *tag_lump, *tag_wak, *tag_raw;
static bool tag_show, idle_present;
static unsigned change_mask;
static unsigned char model_res[8], texture_res[8], idle_header[8];
static std::string trace;
extern "C" int sprintf(char* d, const char* f, ...) {
    va_list l; va_start(l, f); const int n = vsnprintf(d, 64, f, l); va_end(l); return n;
}
extern "C" char* strcpy(char* d, const char* s) { char* r = d; while ((*d++ = *s++)) {} return r; }
extern "C" int strcmp(const char* a, const char* b) { while (*a && *a == *b) { ++a; ++b; } return (unsigned char)*a - (unsigned char)*b; }
extern "C" unsigned norrath_creature_construct(void* object, const void* point, short type, void* map, float scale, char** arguments) {
    event("Creature type=%d map=%d scale=%g args=%d", type, map != nullptr, scale, arguments != nullptr);
    obj = (unsigned char*)object; std::memcpy(obj + 16, point, 12); put(obj + 12, 0x40, 4);
    return 1;
}
extern "C" char* norrath_actor_find_tag_string(void*, char* t, int n) {
    const void* v = t;
    const char* r = nullptr;
    if (v == norrath_ctors_lava_wraith_view_4857e8) r = tag_name;
    else if (v == norrath_ctors_lava_wraith_view_472050) r = tag_lump;
    else if (v == norrath_ctors_lava_wraith_view_485808) r = tag_wak;
    else if (v == norrath_ctors_lava_wraith_view_485818) r = tag_raw;
    else if (v == norrath_ctors_lava_wraith_view_472040) r = tag_show ? "yes" : nullptr;
    else { int i; if (std::sscanf(t, "change%d", &i) == 1) r = (change_mask >> i & 1u) ? "on" : nullptr; else r = nullptr; }
    trace += r ? '+' : '-';
    (void)n;
    return (char*)r;
}
extern "C" void* norrath_lump_find_resource(char* a, char* b) { event("lump %s %s", a, b); return std::strcmp(b, "") ? (std::strstr(b, ".vif") ? model_res : texture_res) : nullptr; }
extern "C" void norrath_actor_animation_initialize(void* s) { event("animInit %d", int((unsigned char*)s - obj)); }
extern "C" void* norrath_object_find_animation(void*, char* n) { event("findAnim %s", n); return idle_present ? idle_header : nullptr; }
extern "C" void norrath_actor_animation_add(void* s, void* h, int f) { event("animAdd %d %d %d", int((unsigned char*)s - obj), h == idle_header, f); }
extern "C" void norrath_actor_animation_randomize_time(void* s) { event("randomize %d", int((unsigned char*)s - obj)); }
extern "C" unsigned long long norrath_actor_model_active_chunks(void* m, int n) { event("chunks %d %d", m == model_res, n); return 0x0102030405060708ull; }
extern "C" unsigned norrath_actor_add_run_list(void*) { event("run"); return 3; }
extern "C" unsigned norrath_object_add_to_delete_list(void*) { event("delete"); return 0x55; }
extern "C" unsigned norrath_creature_finish_construction(void*) { event("finish"); return 0x77; }
static void run_case(const char* name, const char* lump, const char* wak, const char* raw, unsigned mask, bool show, bool idle) {
    calls.clear(); trace.clear();
    tag_name = name; tag_lump = lump; tag_wak = wak; tag_raw = raw; change_mask = mask; tag_show = show; idle_present = idle;
    Image<1160> o, want;
    unsigned char pt[12] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    char* args[] = {nullptr};
    norrath_ctors_lava_wraith::host_note(model_res); norrath_ctors_lava_wraith::host_note(texture_res);
    std::string base = name ? name : "npc";
    std::string lump_name = (lump ? std::string(lump) : base) + ".lmp";
    const bool deleted = lump_name == "npc.lmp";
    const unsigned r = norrath_construct_NPC(o.b, pt, (short)-4, args);
    char b[200];
    std::vector<std::string> w{"Creature type=-4 map=0 scale=8 args=1"};
    const std::string model_n = (wak ? std::string(wak) : base) + ".vif", tex_n = (raw ? std::string(raw) : base) + ".tex";
    if (deleted) w.push_back("delete");
    else {
        w.push_back("lump " + lump_name + " " + model_n);
        w.push_back("lump " + lump_name + " " + tex_n);
        w.push_back("animInit 288"); w.push_back("findAnim idle");
        if (idle) { w.push_back("animAdd 288 1 1"); w.push_back("randomize 288"); }
        std::snprintf(b, sizeof b, "chunks 1 %d", (int)mask); w.push_back(b);
        w.push_back("run"); w.push_back("finish");
    }
    expect_log(w, "npc");
    // tag query order: name, (name), lump, (lump), wak, (wak), raw, (raw), 32 changes, showName
    std::string t;
    t += name ? "++" : "-"; t += lump ? "++" : "-";
    if (!deleted) {
        t += wak ? "++" : "-"; t += raw ? "++" : "-";
        for (int i = 0; i < 32; ++i) t += (mask >> i & 1u) ? '+' : '-';
        t += show ? '+' : '-';
    }
    expect(trace == t, "tag query sequence");
    expect(r == (deleted ? 0x55u : 0x77u), "npc return");
    for (int i = 0; i < 12; ++i) want.b[16 + i] = pt[i];
    unsigned flags = 0x40 | 0x01100008u;
    want.w(40, 36, 1); want.w(72, low(norrath_ctors_lava_wraith_view_44adb0));
    want.w(28, fbits(20.0f)); want.w(168, fbits(0.5f)); want.w(176, fbits(60.0f)); want.w(572, 0, 2); want.w(1136, 0);
    std::memset(want.b + 1068, 0, 0); 
    // base name at +1068: either the tag copy or the 4 bytes "npc\0"
    for (size_t i = 0; i <= base.size() && !name; ++i) want.b[1068 + i] = (unsigned char)"npc"[i];
    if (name) for (size_t i = 0; i <= base.size(); ++i) want.b[1068 + i] = (unsigned char)name[i];
    for (size_t i = 0; i <= lump_name.size(); ++i) want.b[960 + i] = (unsigned char)lump_name.c_str()[i];
    if (!deleted) {
        for (size_t i = 0; i <= model_n.size(); ++i) want.b[992 + i] = (unsigned char)model_n.c_str()[i];
        for (size_t i = 0; i <= tex_n.size(); ++i) want.b[1024 + i] = (unsigned char)tex_n.c_str()[i];
        want.w(1056, low(model_res)); want.w(1060, low(texture_res));
        want.w(244, mask); want.w(552, 0x05060708u); want.w(556, 0x01020304u); want.w(236, 1);
        if (!show) flags &= 0xffefffffu;
    }
    want.w(12, flags);
    // the change buffer is on the constructor's stack; only the object image is compared
    expect_image(o, want, "npc image");
}
int main() {
    run_case("Bob", "lmpX", "wk", "rw", 0x80000021u, true, true);
    run_case(nullptr, nullptr, nullptr, nullptr, 0, true, true);
    run_case("Bob", nullptr, nullptr, nullptr, 0, false, false);
    run_case(nullptr, "other", nullptr, "tx", 2, false, true);
    return finish("npc constructor");
}
