// Host contract for the FrostStormSpell constructor; not an EE input. Stubs log the call order and the
// object words visible at each call; the GameObject stub fills the object with 0x21 and sets +12 to
// 0x100. Light records are compared field by field at the moment lightAdd runs. On the host min.s and
// max.s are replaced by comparisons.
#define W11_HOST 1
#include "../w11-ctor-a-common/host_support.hpp"
#include "ctor.cpp"
#include <cmath>
#include <cstdio>
unsigned char vt_FrostStormSpell[8];
unsigned char vt_LightEffect[8];
int frameNm;
using namespace w11_host;
static unsigned char* the_obj;
static unsigned char* storage_obj;
static unsigned char light_a[64], light_b[64];
static int light_calls;
static bool alloc_ok;
static float fl(const unsigned char* p) { float f; std::memcpy(&f, p, 4); return f; }
static short sh(const unsigned char* p) { short s; std::memcpy(&s, p, 2); return s; }
static std::string num(float f) { return std::to_string(f); }
extern "C" void game_object_construct(void* self, Point3 p, void* map, char** names) {
    calls() += "go(" + std::string(self == the_obj ? "self" : self == storage_obj ? "storage" : "?") + "," + num(p.x) + "," + num(p.y) + "," + num(p.z) + "," +
               std::to_string(map == nullptr) + "," + std::to_string(names == nullptr) + ");";
    std::memset(self, 0x21, 512);
    w11::u32(self, 12) = 0x100;
}
extern "C" void lightAdd__FP5Light(void* l) {
    const bool outer = l == the_obj + 112;
    calls() += std::string("light(") + (outer ? "self" : "storage") + ");";
    std::memcpy(outer ? light_a : light_b, l, 64);
}
extern "C" void* blockAlloc__Fi(int size) {
    calls() += "alloc(" + std::to_string(size) + ")[" + std::to_string(w11::i32(the_obj, 76)) + "," + std::to_string(w11::i32(the_obj, 80)) + "];";
    return alloc_ok ? storage_obj : nullptr;
}
extern "C" void listOnewayLink__FP8ListHeadT0Pv(void* head, void* link, void* data) {
    calls() += "link(" + std::to_string(static_cast<unsigned char*>(head) - the_obj) + "," + std::to_string(w11::word(link)) + "," + std::to_string(link == data) + ");";
}
extern "C" void objectAddToRunList__FP10GameObject(void* o) {
    if (o == storage_obj) calls() += "run(storage)[" + std::to_string(w11::u32(storage_obj, 184)) + "," + std::to_string(w11::u32(storage_obj, 12)) + "];";
    else calls() += "run(self)[" + std::to_string(w11::i32(the_obj, 92)) + "];";
}
static bool fields(const unsigned char* light, short s12, short s14, short s16, float big, float q, float f44, float px, float py, float pz) {
    return light[8] == 0 && light[9] == 0 && sh(light + 10) == 0 && sh(light + 12) == s12 && sh(light + 14) == s14 && sh(light + 16) == s16 && sh(light + 18) == 0 &&
           sh(light + 20) == s12 && sh(light + 22) == s14 && sh(light + 24) == s16 && fl(light + 28) == big && fl(light + 36) == big && fl(light + 32) == q &&
           fl(light + 40) == q && fl(light + 44) == f44 && fl(light + 48) == px && fl(light + 52) == py && fl(light + 56) == pz;
}
int main() {
    the_obj = at(0x100);
    storage_obj = at(0x1000);
    unsigned char* owner = at(0x900);
    int bad = 0;
    struct Case { unsigned flags; bool alloc; float f; int a; int b; };
    const Case cases[] = {
        {0x20, true, 2.0f, 4, 0x77},      // 2 * 1.71596 -> 3.43 -> clamped up to 79
        {0x20, true, 400.0f, -9, 5},      // clamped down to 300
        {0x20, true, 60.0f, 3, 9},        // inside the range: 102.96
        {0x20, false, 60.0f, 3, 9},       // allocation fails
        {0x60, true, 60.0f, 3, 9},        // flag test fails
    };
    for (const Case& k : cases) {
        std::memset(arena, 0, sizeof arena);
        std::memset(light_a, 0xee, sizeof light_a);
        std::memset(light_b, 0xee, sizeof light_b);
        calls().clear();
        light_calls = 0;
        frameNm = 70;
        alloc_ok = k.alloc;
        w11::u32(owner, 12) = k.flags;
        __15FrostStormSpellG6Point3ifiP10GameObjectT1(the_obj, Point3{1.0f, -2.0f, 3.5f}, k.a, k.f, k.b, owner, Point3{60.0f, 120.0f, 180.0f});
        float scaled = k.f * __builtin_bit_cast(float, 0x3fdba483u);
        float c = scaled < 300.0f ? scaled : 300.0f;
        c = c > 79.0f ? c : 79.0f;
        const float q = 1.0f / (c * c);
        const bool match = (k.flags & 0x60) == 0x20;
        const int frame_based = static_cast<int>(70.0f + __builtin_bit_cast(float, 0x4221ffffu));
        std::string want = "go(self,1.000000,-2.000000,3.500000,1,1);light(self);";
        if (match) {
            want += "alloc(188)[" + std::to_string(frame_based + k.a) + "," + std::to_string(frame_based) + "];";
            if (k.alloc) want += "go(storage,1.000000,-2.000000,15.500000,1,1);light(storage);run(storage)[0," + std::to_string(0x103) + "];";
        }
        want += "link(108," + std::to_string(w11::word(owner)) + ",1);run(self)[" + std::to_string(k.b) + "];";
        // The storage run-list call happens before +184 is set, so the logged word is the 0x21 pattern.
        const std::string stale = std::to_string(0x21212121u);
        size_t pos = want.find("run(storage)[0,");
        if (pos != std::string::npos) want.replace(pos, std::string("run(storage)[0,").size(), "run(storage)[" + stale + ",");
        if (calls() != want) { std::puts(calls().c_str()); std::puts(want.c_str()); bad = 1; }
        if (!fields(light_a, 128, 255, 150, c, q, __builtin_bit_cast(float, 0x3f333333u), 1.0f, -2.0f, 3.5f)) { std::puts("outer light"); bad = 1; }
        if (w11::u32(the_obj, 72) != w11::word(vt_FrostStormSpell) || w11::u32(the_obj, 12) != 0x101 || fl(the_obj + 84) != k.f || w11::u32(the_obj, 108) != 0 ||
            w11::u32(the_obj, 28) != 0 || fl(the_obj + 88) != 1.0f || w11::u32(the_obj, 92) != static_cast<unsigned>(k.b) || fl(the_obj + 96) != 60.0f * __builtin_bit_cast(float, 0x3c888888u) ||
            fl(the_obj + 100) != 120.0f * __builtin_bit_cast(float, 0x3c888888u) || fl(the_obj + 104) != 180.0f * __builtin_bit_cast(float, 0x3c888888u) ||
            w11::i32(the_obj, 80) != (match ? frame_based : 0) || w11::i32(the_obj, 76) != (match ? frame_based : 70) + k.a || the_obj[511] != 0x21)
            { std::puts("fields"); bad = 1; }
        if (match && k.alloc) {
            const float g = 2.0f * k.f;
            const float gq = 1.0f / (g * g);
            unsigned char* s = storage_obj;
            if (!fields(light_b, 128, 255, 100, g, gq, __builtin_bit_cast(float, 0x3f266666u), 1.0f, -2.0f, 15.5f)) { std::puts("inner light"); bad = 1; }
            if (w11::u32(s, 72) != w11::word(vt_LightEffect) || w11::u32(s, 136) != 0 || w11::u32(s, 12) != 0x103 || w11::i32(s, 180) != 150 || fl(s + 144) != 128.0f ||
                fl(s + 148) != 255.0f || fl(s + 152) != 100.0f || fl(s + 168) != 0.0f || fl(s + 176) != -g * __builtin_bit_cast(float, 0x3c4ccccc) || fl(s + 140) != 0.0f ||
                fl(s + 156) != 0.0f || fl(s + 160) != 0.0f || fl(s + 164) != 0.0f || fl(s + 172) != g || w11::u32(s, 184) != 1)
                { std::puts("storage fields"); bad = 1; }
        }
    }
    return bad;
}
