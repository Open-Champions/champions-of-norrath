// Host contract for _$_13HammerOfWrath; not an EE input. Callees record their arguments; the object is a local array.
#include "dtor.cpp"
#include <cstdio>
#include <cstring>
#include <string>

static unsigned char object[0x200];
static std::string trace;
static int failures;
unsigned char vt_13HammerOfWrath[8];
unsigned char wrathHammerCount;
static const char* table_name(unsigned word) {
    if (word == static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(vt_13HammerOfWrath))) return "13HammerOfWrath";
    return word == 0x3c3c3c3cu ? "-" : "?";
}
static std::string here() {
    unsigned w;
    std::memcpy(&w, object + 72, 4);
    return std::string("[") + table_name(w) + "]";
}
static long off(const void* p) { return static_cast<const unsigned char*>(p) - object; }
void game_object_destructor(void* s, int f) { trace += "go(" + std::to_string(off(s)) + "," + std::to_string(f) + ")" + here() + ";"; }
void light_destructor(void* s, int f) { trace += "lt(" + std::to_string(off(s)) + "," + std::to_string(f) + ")" + here() + ";"; }
void listDestroy__FP8ListHead(void* s) { trace += "ld(" + std::to_string(off(s)) + ")" + here() + ";"; }
void P_DeleteParticleSystem__Fi(int id) { trace += "pd(" + std::to_string(id) + ")" + here() + ";"; }
void soundStopSound__Fi(int id) { trace += "sn(" + std::to_string(id) + ")" + here() + ";"; }
void blockFree__FPv(void* s) { trace += "fr(" + std::to_string(off(s)) + ")" + here() + ";"; }

struct Row { unsigned flags; unsigned char wrath_in; const char* want; unsigned char wrath_after; };
static const Row rows[] = {
    {0x0u, 0, "ld(80)[13HammerOfWrath];ld(76)[13HammerOfWrath];go(0,0)[13HammerOfWrath];", 0},
    {0x0u, 1, "ld(80)[13HammerOfWrath];ld(76)[13HammerOfWrath];go(0,0)[13HammerOfWrath];", 0},
    {0x0u, 2, "ld(80)[13HammerOfWrath];ld(76)[13HammerOfWrath];go(0,0)[13HammerOfWrath];", 1},
    {0x0u, 127, "ld(80)[13HammerOfWrath];ld(76)[13HammerOfWrath];go(0,0)[13HammerOfWrath];", 126},
    {0x0u, 128, "ld(80)[13HammerOfWrath];ld(76)[13HammerOfWrath];go(0,0)[13HammerOfWrath];", 127},
    {0x0u, 129, "ld(80)[13HammerOfWrath];ld(76)[13HammerOfWrath];go(0,0)[13HammerOfWrath];", 0},
    {0x0u, 254, "ld(80)[13HammerOfWrath];ld(76)[13HammerOfWrath];go(0,0)[13HammerOfWrath];", 0},
    {0x0u, 255, "ld(80)[13HammerOfWrath];ld(76)[13HammerOfWrath];go(0,0)[13HammerOfWrath];", 0},
    {0x1u, 0, "ld(80)[13HammerOfWrath];ld(76)[13HammerOfWrath];go(0,0)[13HammerOfWrath];fr(0)[13HammerOfWrath];", 0},
    {0x1u, 1, "ld(80)[13HammerOfWrath];ld(76)[13HammerOfWrath];go(0,0)[13HammerOfWrath];fr(0)[13HammerOfWrath];", 0},
    {0x1u, 2, "ld(80)[13HammerOfWrath];ld(76)[13HammerOfWrath];go(0,0)[13HammerOfWrath];fr(0)[13HammerOfWrath];", 1},
    {0x1u, 127, "ld(80)[13HammerOfWrath];ld(76)[13HammerOfWrath];go(0,0)[13HammerOfWrath];fr(0)[13HammerOfWrath];", 126},
    {0x1u, 128, "ld(80)[13HammerOfWrath];ld(76)[13HammerOfWrath];go(0,0)[13HammerOfWrath];fr(0)[13HammerOfWrath];", 127},
    {0x1u, 129, "ld(80)[13HammerOfWrath];ld(76)[13HammerOfWrath];go(0,0)[13HammerOfWrath];fr(0)[13HammerOfWrath];", 0},
    {0x1u, 254, "ld(80)[13HammerOfWrath];ld(76)[13HammerOfWrath];go(0,0)[13HammerOfWrath];fr(0)[13HammerOfWrath];", 0},
    {0x1u, 255, "ld(80)[13HammerOfWrath];ld(76)[13HammerOfWrath];go(0,0)[13HammerOfWrath];fr(0)[13HammerOfWrath];", 0},
    {0x2u, 0, "ld(80)[13HammerOfWrath];ld(76)[13HammerOfWrath];go(0,0)[13HammerOfWrath];", 0},
    {0x2u, 1, "ld(80)[13HammerOfWrath];ld(76)[13HammerOfWrath];go(0,0)[13HammerOfWrath];", 0},
    {0x2u, 2, "ld(80)[13HammerOfWrath];ld(76)[13HammerOfWrath];go(0,0)[13HammerOfWrath];", 1},
    {0x2u, 127, "ld(80)[13HammerOfWrath];ld(76)[13HammerOfWrath];go(0,0)[13HammerOfWrath];", 126},
    {0x2u, 128, "ld(80)[13HammerOfWrath];ld(76)[13HammerOfWrath];go(0,0)[13HammerOfWrath];", 127},
    {0x2u, 129, "ld(80)[13HammerOfWrath];ld(76)[13HammerOfWrath];go(0,0)[13HammerOfWrath];", 0},
    {0x2u, 254, "ld(80)[13HammerOfWrath];ld(76)[13HammerOfWrath];go(0,0)[13HammerOfWrath];", 0},
    {0x2u, 255, "ld(80)[13HammerOfWrath];ld(76)[13HammerOfWrath];go(0,0)[13HammerOfWrath];", 0},
    {0x3u, 0, "ld(80)[13HammerOfWrath];ld(76)[13HammerOfWrath];go(0,0)[13HammerOfWrath];fr(0)[13HammerOfWrath];", 0},
    {0x3u, 1, "ld(80)[13HammerOfWrath];ld(76)[13HammerOfWrath];go(0,0)[13HammerOfWrath];fr(0)[13HammerOfWrath];", 0},
    {0x3u, 2, "ld(80)[13HammerOfWrath];ld(76)[13HammerOfWrath];go(0,0)[13HammerOfWrath];fr(0)[13HammerOfWrath];", 1},
    {0x3u, 127, "ld(80)[13HammerOfWrath];ld(76)[13HammerOfWrath];go(0,0)[13HammerOfWrath];fr(0)[13HammerOfWrath];", 126},
    {0x3u, 128, "ld(80)[13HammerOfWrath];ld(76)[13HammerOfWrath];go(0,0)[13HammerOfWrath];fr(0)[13HammerOfWrath];", 127},
    {0x3u, 129, "ld(80)[13HammerOfWrath];ld(76)[13HammerOfWrath];go(0,0)[13HammerOfWrath];fr(0)[13HammerOfWrath];", 0},
    {0x3u, 254, "ld(80)[13HammerOfWrath];ld(76)[13HammerOfWrath];go(0,0)[13HammerOfWrath];fr(0)[13HammerOfWrath];", 0},
    {0x3u, 255, "ld(80)[13HammerOfWrath];ld(76)[13HammerOfWrath];go(0,0)[13HammerOfWrath];fr(0)[13HammerOfWrath];", 0},
    {0x4u, 0, "ld(80)[13HammerOfWrath];ld(76)[13HammerOfWrath];go(0,0)[13HammerOfWrath];", 0},
    {0x4u, 1, "ld(80)[13HammerOfWrath];ld(76)[13HammerOfWrath];go(0,0)[13HammerOfWrath];", 0},
    {0x4u, 2, "ld(80)[13HammerOfWrath];ld(76)[13HammerOfWrath];go(0,0)[13HammerOfWrath];", 1},
    {0x4u, 127, "ld(80)[13HammerOfWrath];ld(76)[13HammerOfWrath];go(0,0)[13HammerOfWrath];", 126},
    {0x4u, 128, "ld(80)[13HammerOfWrath];ld(76)[13HammerOfWrath];go(0,0)[13HammerOfWrath];", 127},
    {0x4u, 129, "ld(80)[13HammerOfWrath];ld(76)[13HammerOfWrath];go(0,0)[13HammerOfWrath];", 0},
    {0x4u, 254, "ld(80)[13HammerOfWrath];ld(76)[13HammerOfWrath];go(0,0)[13HammerOfWrath];", 0},
    {0x4u, 255, "ld(80)[13HammerOfWrath];ld(76)[13HammerOfWrath];go(0,0)[13HammerOfWrath];", 0},
    {0x5u, 0, "ld(80)[13HammerOfWrath];ld(76)[13HammerOfWrath];go(0,0)[13HammerOfWrath];fr(0)[13HammerOfWrath];", 0},
    {0x5u, 1, "ld(80)[13HammerOfWrath];ld(76)[13HammerOfWrath];go(0,0)[13HammerOfWrath];fr(0)[13HammerOfWrath];", 0},
    {0x5u, 2, "ld(80)[13HammerOfWrath];ld(76)[13HammerOfWrath];go(0,0)[13HammerOfWrath];fr(0)[13HammerOfWrath];", 1},
    {0x5u, 127, "ld(80)[13HammerOfWrath];ld(76)[13HammerOfWrath];go(0,0)[13HammerOfWrath];fr(0)[13HammerOfWrath];", 126},
    {0x5u, 128, "ld(80)[13HammerOfWrath];ld(76)[13HammerOfWrath];go(0,0)[13HammerOfWrath];fr(0)[13HammerOfWrath];", 127},
    {0x5u, 129, "ld(80)[13HammerOfWrath];ld(76)[13HammerOfWrath];go(0,0)[13HammerOfWrath];fr(0)[13HammerOfWrath];", 0},
    {0x5u, 254, "ld(80)[13HammerOfWrath];ld(76)[13HammerOfWrath];go(0,0)[13HammerOfWrath];fr(0)[13HammerOfWrath];", 0},
    {0x5u, 255, "ld(80)[13HammerOfWrath];ld(76)[13HammerOfWrath];go(0,0)[13HammerOfWrath];fr(0)[13HammerOfWrath];", 0},
    {0x80000001u, 0, "ld(80)[13HammerOfWrath];ld(76)[13HammerOfWrath];go(0,0)[13HammerOfWrath];fr(0)[13HammerOfWrath];", 0},
    {0x80000001u, 1, "ld(80)[13HammerOfWrath];ld(76)[13HammerOfWrath];go(0,0)[13HammerOfWrath];fr(0)[13HammerOfWrath];", 0},
    {0x80000001u, 2, "ld(80)[13HammerOfWrath];ld(76)[13HammerOfWrath];go(0,0)[13HammerOfWrath];fr(0)[13HammerOfWrath];", 1},
    {0x80000001u, 127, "ld(80)[13HammerOfWrath];ld(76)[13HammerOfWrath];go(0,0)[13HammerOfWrath];fr(0)[13HammerOfWrath];", 126},
    {0x80000001u, 128, "ld(80)[13HammerOfWrath];ld(76)[13HammerOfWrath];go(0,0)[13HammerOfWrath];fr(0)[13HammerOfWrath];", 127},
    {0x80000001u, 129, "ld(80)[13HammerOfWrath];ld(76)[13HammerOfWrath];go(0,0)[13HammerOfWrath];fr(0)[13HammerOfWrath];", 0},
    {0x80000001u, 254, "ld(80)[13HammerOfWrath];ld(76)[13HammerOfWrath];go(0,0)[13HammerOfWrath];fr(0)[13HammerOfWrath];", 0},
    {0x80000001u, 255, "ld(80)[13HammerOfWrath];ld(76)[13HammerOfWrath];go(0,0)[13HammerOfWrath];fr(0)[13HammerOfWrath];", 0},
    {0xfffffffeu, 0, "ld(80)[13HammerOfWrath];ld(76)[13HammerOfWrath];go(0,0)[13HammerOfWrath];", 0},
    {0xfffffffeu, 1, "ld(80)[13HammerOfWrath];ld(76)[13HammerOfWrath];go(0,0)[13HammerOfWrath];", 0},
    {0xfffffffeu, 2, "ld(80)[13HammerOfWrath];ld(76)[13HammerOfWrath];go(0,0)[13HammerOfWrath];", 1},
    {0xfffffffeu, 127, "ld(80)[13HammerOfWrath];ld(76)[13HammerOfWrath];go(0,0)[13HammerOfWrath];", 126},
    {0xfffffffeu, 128, "ld(80)[13HammerOfWrath];ld(76)[13HammerOfWrath];go(0,0)[13HammerOfWrath];", 127},
    {0xfffffffeu, 129, "ld(80)[13HammerOfWrath];ld(76)[13HammerOfWrath];go(0,0)[13HammerOfWrath];", 0},
    {0xfffffffeu, 254, "ld(80)[13HammerOfWrath];ld(76)[13HammerOfWrath];go(0,0)[13HammerOfWrath];", 0},
    {0xfffffffeu, 255, "ld(80)[13HammerOfWrath];ld(76)[13HammerOfWrath];go(0,0)[13HammerOfWrath];", 0},
    {0xffffffffu, 0, "ld(80)[13HammerOfWrath];ld(76)[13HammerOfWrath];go(0,0)[13HammerOfWrath];fr(0)[13HammerOfWrath];", 0},
    {0xffffffffu, 1, "ld(80)[13HammerOfWrath];ld(76)[13HammerOfWrath];go(0,0)[13HammerOfWrath];fr(0)[13HammerOfWrath];", 0},
    {0xffffffffu, 2, "ld(80)[13HammerOfWrath];ld(76)[13HammerOfWrath];go(0,0)[13HammerOfWrath];fr(0)[13HammerOfWrath];", 1},
    {0xffffffffu, 127, "ld(80)[13HammerOfWrath];ld(76)[13HammerOfWrath];go(0,0)[13HammerOfWrath];fr(0)[13HammerOfWrath];", 126},
    {0xffffffffu, 128, "ld(80)[13HammerOfWrath];ld(76)[13HammerOfWrath];go(0,0)[13HammerOfWrath];fr(0)[13HammerOfWrath];", 127},
    {0xffffffffu, 129, "ld(80)[13HammerOfWrath];ld(76)[13HammerOfWrath];go(0,0)[13HammerOfWrath];fr(0)[13HammerOfWrath];", 0},
    {0xffffffffu, 254, "ld(80)[13HammerOfWrath];ld(76)[13HammerOfWrath];go(0,0)[13HammerOfWrath];fr(0)[13HammerOfWrath];", 0},
    {0xffffffffu, 255, "ld(80)[13HammerOfWrath];ld(76)[13HammerOfWrath];go(0,0)[13HammerOfWrath];fr(0)[13HammerOfWrath];", 0},
};

int main() {
    const unsigned fill = 0x3c3c3c3cu;
    for (const Row& r : rows) {
        std::memset(object, 0x3c, sizeof object);
        wrathHammerCount = r.wrath_in;
        trace.clear();
        destructor_13HammerOfWrath(object, static_cast<int>(r.flags));
        if (trace != r.want) { std::fprintf(stderr, "fail: flags %%x got %%s want %%s\n", r.flags, trace.c_str(), r.want); ++failures; }
        if (wrathHammerCount != r.wrath_after) { std::fprintf(stderr, "fail: wrath in %%u got %%u want %%u\n", r.wrath_in, wrathHammerCount, r.wrath_after); ++failures; }
    }
    (void)fill;
    if (!failures) std::puts("w16-m-dtor-a-dtor-13hammerofwrath ok");
    return failures ? 1 : 0;
}
