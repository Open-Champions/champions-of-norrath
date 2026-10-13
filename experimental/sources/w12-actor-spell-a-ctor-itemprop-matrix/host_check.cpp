// Host contract for the ItemProp (description and matrix) constructor; not an EE input. Stubs log the call order; the
// item table lives in a host array whose pointer words are host-side offsets (see w11 host support).
#define W11_HOST 1
#include "../w11-ctor-a-common/host_support.hpp"
#define strcpy w12_strcpy
#include "ctor.cpp"
#undef strcpy
#include <cstdio>
unsigned char vt_Prop[8], vt_ItemProp[8];
unsigned char globalItem[16 * 8];
using namespace w11_host;
static unsigned char* the_obj;
static std::string id(const void* p) { return std::to_string(p == the_obj); }
static int ids[2], next_id;
extern "C" int getProxyMeshId__4itemPQ24item11description(void* d) { calls() += "proxy(" + std::to_string(d == at(0x1000)) + ");"; return ids[next_id++ % 2]; }
extern "C" void game_object_construct(void* self, Point3 p, void* map, char** names) {
    calls() += "go(" + id(self) + "," + std::to_string(p.x) + "," + std::to_string(p.z) + "," + std::to_string(map == nullptr) + "," + std::to_string(names == nullptr) + ");";
    std::memset(self, 0x21, 0x180);
    const unsigned flags = 0x100;
    std::memcpy(static_cast<unsigned char*>(self) + 12, &flags, 4);
    w11::f32(self, 16) = 9.0f;
}
extern "C" void objectUpdateInGrid__FP10GameObject(void* o) { calls() += "grid(" + id(o) + "," + std::to_string(w11::u32(o, 12)) + ");"; }
extern "C" void objectAddToRunList__FP10GameObject(void* o) { calls() += "run(" + id(o) + ");"; }
extern "C" char* w12_strcpy(char* to, const char* from) { calls() += "strcpy(" + std::to_string(to - reinterpret_cast<char*>(the_obj)) + "," + from + ");"; std::strcpy(to, from); return to; }
static int lump_calls, h320_after;
extern "C" void* lumpFindResource__FPcT0(char* lump, char* name) {
    calls() += "lump(" + std::to_string(lump - reinterpret_cast<char*>(the_obj)) + "," + std::to_string(name - reinterpret_cast<char*>(the_obj)) + "," + lump + "," + name + ");";
    if (++lump_calls == 2) w11::u16(the_obj, 320) = static_cast<unsigned short>(h320_after);
    return at(0x40 * lump_calls);
}
int main() {
    the_obj = at(0x100);
    int bad = 0;
    struct Case { int id1, id2, h320, bit39; };
    const Case cases[] = {{1, 2, 0, 0}, {3, 5, 2, 0}, {1, 4, 4, 1}, {5, 5, 6, 1}, {2, 3, 2, 1}, {7, 1, 0, 0}};
    unsigned char description[72], matrix[48];
    for (const Case& k : cases) {
        std::memset(arena, 0x5a, sizeof arena);
        for (int j = 0; j < 8; ++j) {
            char* a = reinterpret_cast<char*>(at(0x2000 + 0x40 * j));
            char* b = reinterpret_cast<char*>(at(0x2000 + 0x40 * j + 0x20));
            std::snprintf(a, 32, "lump%d.bin", j);
            std::snprintf(b, 32, "res%d.tex", j * 3);
            const unsigned wa = w11_host_word(a), wb = w11_host_word(b);
            std::memcpy(globalItem + 16 * j, &wa, 4);
            std::memcpy(globalItem + 16 * j + 4, &wb, 4);
        }
        for (int i = 0; i < 72; i++) description[i] = static_cast<unsigned char>(i + 1);
        if (k.bit39) description[12] |= 0x80; else description[12] &= 0x7f;  // bit 39 of the 64-bit word at +336 is bit 7 of byte +340 = description byte 12
        for (int i = 0; i < 48; i++) matrix[i] = static_cast<unsigned char>(0xa0 + i);
        std::memcpy(at(0x1000), description, 72);
        ids[0] = k.id1; ids[1] = k.id2; next_id = 0; lump_calls = 0; h320_after = k.h320;
        calls().clear();
        __8ItemPropG6Point3PQ24item11descriptionP8Matrix34(the_obj, Point3{1.0f, 2.0f, 3.0f}, at(0x1000), reinterpret_cast<Matrix34*>(matrix));
        const unsigned pre = (0x100u | 0x1809u);
        std::string want = "proxy(1);proxy(1);go(1,1.000000,3.000000,1,1);grid(1," + std::to_string(pre) + ");";
        want += "strcpy(100,lump" + std::to_string(k.id1) + ".bin);strcpy(164,res" + std::to_string(k.id2 * 3) + ".tex);";
        want += "lump(84,100,equipmnt.lmp,lump" + std::to_string(k.id1) + ".bin);lump(84,164,equipmnt.lmp,res" + std::to_string(k.id2 * 3) + ".tex);";
        unsigned f = pre;
        if (k.h320 & 2) { want += "run(1);"; f |= 0x4000u; }
        if (k.h320 & 4) f |= 0x1000u;
        f = (f | 0x102000u) & 0xffffe7ffu;
        if (k.bit39) f &= 0xffffcfffu;
        want += "grid(1," + std::to_string(f) + ");";
        if (calls() != want) { std::puts(calls().c_str()); std::puts(want.c_str()); bad = 1; }
        const unsigned long long m232 = *reinterpret_cast<unsigned long long*>(the_obj + 232);
        if (w11::f32(the_obj, 28) != 4.0f || w11::u16(the_obj, 320) != k.h320 || std::memcmp(the_obj + 272, matrix, 48) != 0 || std::memcmp(the_obj + 84, "equipmnt.lmp", 13) != 0 ||
            m232 != ~0ull || w11::u32(the_obj, 76) != w11_host_word(at(0x40)) || w11::u32(the_obj, 80) != w11_host_word(at(0x80)) || w11::u32(the_obj, 72) != w11::word(vt_ItemProp) ||
            w11::u32(the_obj, 12) != f) {
            std::puts("fields");
            bad = 1;
        }
        // The description copy overwrites the halfword defaults at 364..388 (offsets 36..60 of the description) and 344..358.
        if (std::memcmp(the_obj + 328, description, 72) != 0) { std::puts("copy"); bad = 1; }
        if (w11::u8(the_obj, 360) != 1 + 32 || the_obj[327] != 0x21 || the_obj[400] != 0x5a) { std::puts("stray"); bad = 1; }
    }
    if (!bad) std::puts("itemprop matrix ctor ok");
    return bad;
}
