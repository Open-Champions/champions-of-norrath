// Host contract for the FireStorm constructor; not an EE input. Stubs log the call order with their
// arguments and the object words visible at each call. The skill table entry returns scripted values.
// The host divide 1/0 gives infinity; the target's divide-by-zero result is not modelled here.
#define W11_HOST 1
#include "../w11-ctor-a-common/host_support.hpp"
#include "ctor.cpp"
#include <cmath>
#include <cstdio>
unsigned char vt_FireStorm[8];
int frameNm;
using namespace w11_host;
static unsigned char* the_obj;
static unsigned char light[64];
static bool alloc_ok;
static std::vector<int> scripted;
static size_t scripted_index;
static float fl(const unsigned char* p) { float f; std::memcpy(&f, p, 4); return f; }
static short sh(const unsigned char* p) { short s; std::memcpy(&s, p, 2); return s; }
static std::string num(float f) { return std::to_string(f); }
extern "C" void game_object_construct(void* self, Point3 p, void* map, char** names) {
    calls() += "go(" + std::to_string(self == the_obj) + "," + num(p.x) + "," + num(p.y) + "," + num(p.z) + "," + std::to_string(map == nullptr) + "," +
               std::to_string(names == nullptr) + ");";
    std::memset(self, 0x21, 512);
    w11::u32(self, 12) = 0x100;
}
extern "C" void lightAdd__FP5Light(void* l) {
    calls() += "light(" + std::to_string(static_cast<unsigned char*>(l) - the_obj) + ");";
    std::memcpy(light, l, 64);
}
extern "C" void listOnewayLink__FP8ListHeadT0Pv(void* head, void* link, void* data) {
    calls() += "link(" + std::to_string(static_cast<unsigned char*>(head) - the_obj) + "," + std::to_string(w11::word(link)) + "," + std::to_string(link == data) + ")[" +
               std::to_string(w11::u32(the_obj, 12)) + "];";
}
extern "C" void* blockAlloc__Fi(int size) { calls() += "alloc(" + std::to_string(size) + ");"; return alloc_ok ? at(0xc00) : nullptr; }
extern "C" void light_effect_construct(void* self, Point3 p, float a, float b, float c, float d, float e, float f, float g, float h,
                                       float i, float j, float k, float l, void* owner, int flag) {
    calls() += "le(" + std::to_string(self == at(0xc00)) + "," + num(p.x) + "," + num(p.y) + "," + num(p.z) + "," + num(a) + "," + num(b) + "," + num(c) + "," + num(d) + "," +
               num(e) + "," + num(f) + "," + num(g) + "," + num(h) + "," + num(i) + "," + num(j) + "," + num(k) + "," + num(l) + "," + std::to_string(owner == nullptr) + "," +
               std::to_string(flag) + ");";
}
extern "C" void lightUpdatePosition__FP5Light(void* l) { calls() += "upd(" + std::to_string(static_cast<unsigned char*>(l) - the_obj) + ");"; }
extern "C" void objectAddToRunList__FP10GameObject(void* o) {
    calls() += "run" + std::to_string(o == the_obj) + "[" + std::to_string(w11::i32(the_obj, 148)) + "," + std::to_string(w11::u32(the_obj, 160)) + "];";
}
extern "C" void* lumpFindResource__FPcT0(char* l, char* n) { calls() += std::string("lump(") + l + "," + n + ");"; return at(0x800); }
extern "C" int soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* h, void* o, float v) {
    calls() += "snd(" + std::to_string(h == at(0x800)) + "," + std::to_string(o == the_obj) + "," + num(v) + ");";
    return 0x66;
}
extern "C" void soundMakePersistant__Fi(int handle) { calls() += "persist(" + std::to_string(handle) + ");"; }
int main() {
    the_obj = at(0x100);
    unsigned char* skill = at(0x600);
    unsigned char* owner = at(0x900);
    int bad = 0;
    struct Case { unsigned owner_flags; bool alloc; std::vector<int> results; };
    const Case cases[] = {
        {0x20, true, {3, 4}},
        {0x20, false, {1, 5}},
        {0x60, true, {2, -7}},
    };
    for (const Case& k : cases) {
        std::memset(arena, 0, sizeof arena);
        std::memset(light, 0xee, sizeof light);
        calls().clear();
        frameNm = 70;
        alloc_ok = k.alloc;
        scripted = k.results;
        scripted_index = 0;
        w11::u32(owner, 12) = k.owner_flags;
        w11::u32(skill, 96) = w11::word(at(0x700));
        w11::i16(at(0x700), 112) = 16;
        w11::u32(at(0x700), 116) = 9;
        functions()[9] = [&](void* self, int a, int b, int c) {
            const int r = scripted.at(scripted_index++);
            calls() += "skill(" + std::to_string(static_cast<unsigned char*>(self) - skill) + "," + std::to_string(a) + "," + std::to_string(b) + "," +
                       std::to_string(c == static_cast<int>(w11::word(the_obj + 76))) + ");";
            return r;
        };
        __9FireStormiP10GameObjectG6Point3P5Skill(the_obj, 7, owner, Point3{1.0f, -2.0f, 3.5f}, skill);
        const bool match = (k.owner_flags & 0x60) == 0x20;
        const int word156 = k.results[0] * 12;
        const int word152 = match ? static_cast<int>(70.0f + __builtin_bit_cast(float, 0x4221ffffu)) : 0;
        const int word148 = (match ? word152 : 70) + k.results[1] * 60;
        std::string want = "go(1,1.000000,-2.000000,3.500000,1,1);light(80);link(76," + std::to_string(w11::word(owner)) + ",1)[" + std::to_string(0x100 | 0x421b) + "];skill(16,1,7,1);";
        if (match) {
            want += "alloc(188);";
            if (k.alloc)
                want += "le(1,1.000000,-2.000000,15.500000,255.000000,128.000000,0.000000,100.000000,255.000000,128.000000,0.000000,100.000000," + num(static_cast<float>(word156)) + "," +
                        num(static_cast<float>(word156 * 2)) + "," + num(__builtin_bit_cast(float, 0x3f266666u)) + "," + num(__builtin_bit_cast(float, 0x40019999u)) + ",1,1);";
        }
        want += "skill(16,2,7,1);upd(80);run1[" + std::to_string(word148) + ",0];lump(fx.lmp,spel_ew_firestorm.anm);snd(1,1,1.000000);persist(102);";
        if (calls() != want) { std::puts(calls().c_str()); std::puts(want.c_str()); bad = 1; }
        unsigned bits = 0x3f004156u; float c44; std::memcpy(&c44, &bits, 4);
        if (light[8] != 0 || light[9] != 0 || sh(light + 10) != 255 || sh(light + 12) != 128 || sh(light + 14) != 0 || sh(light + 16) != -16 ||
            sh(light + 18) != 255 || sh(light + 20) != 128 || sh(light + 22) != 0 || sh(light + 24) != -16 || fl(light + 28) != 0.0f || fl(light + 36) != 0.0f ||
            !std::isinf(fl(light + 32)) || !std::isinf(fl(light + 40)) || fl(light + 44) != c44 || fl(light + 48) != 1.0f || fl(light + 52) != -2.0f || fl(light + 56) != 3.5f)
            { std::puts("light"); bad = 1; }
        if (w11::u32(the_obj, 72) != w11::word(vt_FireStorm) || w11::u32(the_obj, 76) != 0 || w11::u32(the_obj, 140) != 7 || fl(the_obj + 16) != 1.0f ||
            fl(the_obj + 20) != -2.0f || fl(the_obj + 24) != 3.5f || w11::u32(the_obj, 144) != w11::word(skill) || w11::i32(the_obj, 156) != word156 ||
            w11::i32(the_obj, 152) != word152 || w11::i32(the_obj, 148) != word148 || w11::u32(the_obj, 160) != 0 || w11::i32(the_obj, 164) != 0x66 ||
            the_obj[511] != 0x21)
            { std::puts("fields"); bad = 1; }
    }
    return bad;
}
