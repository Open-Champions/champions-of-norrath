// Host contract for the ShockOfFrost constructor; not an EE input. Stubs log the call order with the
// arguments; the Projectile stub fills the object with 0x21. The light record is compared field by
// field as it stood when lightAdd ran.
#define W11_HOST 1
#include "../w11-ctor-a-common/host_support.hpp"
#include "ctor.cpp"
#include <cstdio>
unsigned char vt_ShockOfFrost[8];
int frameNm;
using namespace w11_host;
static unsigned char* the_obj;
static unsigned char light[64];
static float fl(const unsigned char* p) { float f; std::memcpy(&f, p, 4); return f; }
static short sh(const unsigned char* p) { short s; std::memcpy(&s, p, 2); return s; }
extern "C" void projectile_construct(void* self, Point3 p, void* owner, void* map) {
    calls() += "proj(" + std::to_string(self == the_obj) + "," + std::to_string(p.x) + "," + std::to_string(p.y) + "," + std::to_string(p.z) + "," +
               std::to_string(w11::word(owner)) + "," + std::to_string(map == nullptr) + ");";
    std::memset(self, 0x21, 512);
}
extern "C" void lightAdd__FP5Light(void* l) {
    calls() += "light(" + std::to_string(static_cast<unsigned char*>(l) - the_obj) + ");";
    std::memcpy(light, l, 64);
}
extern "C" void listOnewayLink__FP8ListHeadT0Pv(void* head, void* link, void* data) {
    calls() += "link(" + std::to_string(static_cast<unsigned char*>(head) - the_obj) + "," + std::to_string(w11::word(link)) + "," + std::to_string(link == data) + ");";
}
extern "C" float icos__Fi(int a) { calls() += "cos(" + std::to_string(a) + ");"; return 0.5f; }
extern "C" float isin__Fi(int a) { calls() += "sin(" + std::to_string(a) + ");"; return 0.25f; }
extern "C" void objectAddToRunList__FP10GameObject(void* o) { calls() += "run" + std::to_string(o == the_obj) + ";"; }
extern "C" void* lumpFindResource__FPcT0(char* l, char* n) { calls() += std::string("lump(") + l + "," + n + ");"; return at(0x800); }
extern "C" int soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* h, void* o, float v) {
    calls() += "snd(" + std::to_string(h == at(0x800)) + "," + std::to_string(o == the_obj) + "," + std::to_string(v) + ");";
    return 0;
}
int main() {
    the_obj = at(0x100);
    int bad = 0;
    for (void* owner : {static_cast<void*>(nullptr), static_cast<void*>(at(0x900))}) {
        std::memset(arena, 0, sizeof arena);
        std::memset(light, 0xee, sizeof light);
        calls().clear();
        frameNm = 70;
        void* const skill = at(0xa00);
        __12ShockOfFrostG6Point3siP10GameObjectP5Skill(the_obj, Point3{1.0f, -2.0f, 3.5f}, -300, 0x10005, owner, skill);
        std::string want = "proj(1,1.000000,-2.000000,3.500000," + std::to_string(w11::word(owner)) + ",1);light(88);";
        if (owner) want += "link(84," + std::to_string(w11::word(owner)) + ",1);";
        want += "cos(-300);sin(-300);run1;lump(fx.lmp,spel_ew_shockfrost.anm);snd(1,1,1.000000);";
        if (calls() != want) { std::puts(calls().c_str()); std::puts(want.c_str()); bad = 1; }
        unsigned a_bits = 0x38be37c6u, b_bits = 0x3f4d0e45u; float a, b;
        std::memcpy(&a, &a_bits, 4); std::memcpy(&b, &b_bits, 4);
        if (light[8] != 0 || light[9] != 0 || sh(light + 10) != 0 || sh(light + 12) != 128 || sh(light + 14) != 255 || sh(light + 16) != 150 ||
            sh(light + 18) != 0 || sh(light + 20) != 128 || sh(light + 22) != 255 || sh(light + 24) != 150 || fl(light + 28) != 105.0f ||
            fl(light + 36) != 105.0f || fl(light + 32) != a || fl(light + 40) != a || fl(light + 44) != b ||
            fl(light + 48) != 1.0f || fl(light + 52) != -2.0f || fl(light + 56) != 3.5f) { std::puts("light"); bad = 1; }
        if (w11::u32(the_obj, 72) != w11::word(vt_ShockOfFrost) || w11::u32(the_obj, 84) != 0 || sh(the_obj + 148) != 5 || sh(the_obj + 56) != -300 ||
            fl(the_obj + 156) != 175.0f || fl(the_obj + 160) != 87.5f || fl(the_obj + 164) != 0.0f || w11::u32(the_obj, 200) != w11::word(skill) ||
            w11::u32(the_obj, 196) != 0 || w11::u32(the_obj, 192) != 190u || w11::u32(the_obj, 204) != 0 || the_obj[511] != 0x21 || the_obj[168] != 0x21)
            { std::puts("fields"); bad = 1; }
    }
    return bad;
}
