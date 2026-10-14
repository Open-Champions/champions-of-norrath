// Host contract for _$_12SpellHealing; not an EE input. Callees record their arguments; the object is a local array.
#include "dtor.cpp"
#include <cstdio>
#include <cstring>
#include <string>

static unsigned char object[0x200];
static std::string trace;
static int failures;
unsigned char vt_5Spell[8];
static const char* table_name(unsigned word) {
    if (word == static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(vt_5Spell))) return "5Spell";
    return word == 0x3c3c3c3cu ? "-" : "?";
}
static std::string here() {
    unsigned w;
    std::memcpy(&w, object + 20, 4);
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
    {0x0u, 0, "ld(4)[5Spell];", 0},
    {0x1u, 0, "ld(4)[5Spell];fr(0)[5Spell];", 0},
    {0x2u, 0, "ld(4)[5Spell];", 0},
    {0x3u, 0, "ld(4)[5Spell];fr(0)[5Spell];", 0},
    {0x4u, 0, "ld(4)[5Spell];", 0},
    {0x5u, 0, "ld(4)[5Spell];fr(0)[5Spell];", 0},
    {0x80000001u, 0, "ld(4)[5Spell];fr(0)[5Spell];", 0},
    {0xfffffffeu, 0, "ld(4)[5Spell];", 0},
    {0xffffffffu, 0, "ld(4)[5Spell];fr(0)[5Spell];", 0},
};

int main() {
    const unsigned fill = 0x3c3c3c3cu;
    for (const Row& r : rows) {
        std::memset(object, 0x3c, sizeof object);
        trace.clear();
        destructor_12SpellHealing(object, static_cast<int>(r.flags));
        if (trace != r.want) { std::fprintf(stderr, "fail: flags %%x got %%s want %%s\n", r.flags, trace.c_str(), r.want); ++failures; }
    }
    (void)fill;
    if (!failures) std::puts("w16-m-dtor-a-dtor-12spellhealing ok");
    return failures ? 1 : 0;
}
