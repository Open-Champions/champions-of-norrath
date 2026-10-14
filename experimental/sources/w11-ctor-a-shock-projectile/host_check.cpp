// Host contract for the ShockProjectile constructor; not an EE input. Stubs log the call order with
// arguments and object words. The reciprocal square root is the host expression 1/sqrt(x) (the EE
// build uses rsqrt.s, which the host cannot run). The skill table entry returns a value derived from
// its arguments, so a dropped or reordered argument shows in byte +394.
#define W11_HOST 1
#include "../w11-ctor-a-common/host_support.hpp"
#include "ctor.cpp"
#include <cstdio>
unsigned char vt_ShockProjectile[8];
int frameNm;
using namespace w11_host;
static unsigned char* the_obj;
static unsigned char light[64];
static float fl(const unsigned char* p) { float f; std::memcpy(&f, p, 4); return f; }
static short sh(const unsigned char* p) { short s; std::memcpy(&s, p, 2); return s; }
extern "C" void game_object_construct(void* self, Point3 p, void* map, char** names) {
    calls() += "go(" + std::to_string(self == the_obj) + "," + std::to_string(p.x) + "," + std::to_string(p.y) + "," + std::to_string(p.z) + "," +
               std::to_string(map == nullptr) + "," + std::to_string(names == nullptr) + ");";
    std::memset(self, 0x21, 512);
    w11::u32(self, 12) = 0x100;
}
extern "C" void lightAdd__FP5Light(void* l) {
    calls() += "light(" + std::to_string(static_cast<unsigned char*>(l) - the_obj) + ");";
    std::memcpy(light, l, 64);
}
extern "C" void listOnewayLink__FP8ListHeadT0Pv(void* head, void* link, void* data) {
    calls() += "link(" + std::to_string(static_cast<unsigned char*>(head) - the_obj) + "," + std::to_string(w11::word(link)) + "," + std::to_string(link == data) + ");";
}
extern "C" float icos__Fi(int a) { calls() += "cos(" + std::to_string(a) + ");"; return 0.6f; }
extern "C" float isin__Fi(int a) { calls() += "sin(" + std::to_string(a) + ");"; return 0.5f; }
extern "C" void objectAddToRunList__FP10GameObject(void* o) { calls() += "run" + std::to_string(o == the_obj) + ";"; }
extern "C" void* lumpFindResource__FPcT0(char* l, char* n) { calls() += std::string("lump(") + l + "," + n + ");"; return at(0x800); }
extern "C" int soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* h, void* o, float v) {
    calls() += "snd(" + std::to_string(h == at(0x800)) + "," + std::to_string(o == the_obj ? 1 : w11::word(o)) + "," + std::to_string(v) + ");";
    return 0;
}
int main() {
    the_obj = at(0x100);
    unsigned char* skill = at(0x600);
    int bad = 0;
    for (void* owner : {static_cast<void*>(nullptr), static_cast<void*>(at(0x900))}) {
        std::memset(arena, 0, sizeof arena);
        std::memset(light, 0xee, sizeof light);
        calls().clear();
        frameNm = 70;
        w11::u32(skill, 96) = w11::word(at(0x700));
        w11::i16(at(0x700), 112) = 16;
        w11::u32(at(0x700), 116) = 9;
        functions()[9] = [&](void* self, int a, int b, int c) {
            calls() += "skill(" + std::to_string(static_cast<unsigned char*>(self) - skill) + "," + std::to_string(a) + "," + std::to_string(b) + "," +
                       std::to_string(c == static_cast<int>(w11::word(the_obj + 404))) + ");";
            return 40 + b;
        };
        __15ShockProjectileG6Point3sP10GameObjectP5Skilli(the_obj, Point3{1.0f, -2.0f, 3.5f}, -300, owner, skill, 0x85);
        // N = 0x85 is stored as a byte and read back as signed: -123
        std::string want = "go(1,1.000000,-2.000000,3.500000,1,1);light(332);lump(fx.lmp,spel_ew_shock.anm);snd(1," + std::to_string(w11::word(owner)) + ",1.000000);";
        if (owner) want += "link(404," + std::to_string(w11::word(owner)) + ",1);";
        want += "cos(-300);sin(-300);skill(16,1,-123,1);run1;lump(fx.lmp,spel_ew_shock.anm);snd(1,1,1.000000);";
        if (calls() != want) { std::puts(calls().c_str()); std::puts(want.c_str()); bad = 1; }
        unsigned a_bits = 0x382c598au; float a; std::memcpy(&a, &a_bits, 4);
        if (light[8] != 0 || light[9] != 0 || sh(light + 10) != 0 || sh(light + 12) != 225 || sh(light + 14) != 320 || sh(light + 16) != -32 ||
            sh(light + 18) != 0 || sh(light + 20) != 225 || sh(light + 22) != 320 || sh(light + 24) != -32 || fl(light + 28) != 156.0f ||
            fl(light + 36) != 156.0f || fl(light + 32) != a || fl(light + 40) != a || fl(light + 44) != 0.0f ||
            fl(light + 48) != 1.0f || fl(light + 52) != -2.0f || fl(light + 56) != 3.5f) { std::puts("light"); bad = 1; }
        const float rs = 1.0f / __builtin_sqrtf(0.6f * 0.6f + 0.5f * 0.5f);
        unsigned step_bits = 0x3c888888u, k_bits = 0x43aa8000u; float step, k400;
        std::memcpy(&step, &step_bits, 4); std::memcpy(&k400, &k_bits, 4);
        const float x = (0.6f * rs) * (k400 * step), y = (0.5f * rs) * (k400 * step);
        if (w11::u32(the_obj, 72) != w11::word(vt_ShockProjectile) || w11::u32(the_obj, 80) != 0 || w11::u32(the_obj, 84) != 0 || w11::u32(the_obj, 404) != 0 ||
            w11::u32(the_obj, 12) != 0x101 || sh(the_obj + 56) != -300 || w11::u32(the_obj, 88) != 70 || w11::u32(the_obj, 76) != w11::word(skill) ||
            the_obj[392] != 0x85 || the_obj[393] != 0 || the_obj[394] != static_cast<unsigned char>(40 - 123 + 1) || fl(the_obj + 28) != 12.0f ||
            fl(the_obj + 400) != k400 || fl(the_obj + 296) != 0.0f || fl(the_obj + 288) != x || fl(the_obj + 292) != y || the_obj[511] != 0x21)
            { std::puts("fields"); bad = 1; }
    }
    return bad;
}
