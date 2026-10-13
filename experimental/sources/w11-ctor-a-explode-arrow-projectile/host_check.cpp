// Host contract for the ExplodeArrowProjectile constructor; not an EE input. Stubs log the call
// order with their arguments and the object words at each call. The skill table entry is a
// function that returns a value derived from its arguments, so a swapped or dropped argument
// changes the result and the stored velocity.
#define W11_HOST 1
#include "../w11-ctor-a-common/host_support.hpp"
#include "ctor.cpp"
#include <cstdio>
unsigned char vt_ExplodeArrowProjectile[8];
int frameNm;
using namespace w11_host;
static unsigned char* the_obj;
static std::string snap() {
    unsigned a = w11::u32(the_obj, 72), b = w11::u32(the_obj, 84), c = w11::u32(the_obj, 88);
    return "[" + std::to_string(a == w11::word(vt_ExplodeArrowProjectile)) + "," + std::to_string(b) + "," + std::to_string(c) + "]";
}
extern "C" void projectile_construct(void* self, Point3 p, void* owner, void* map) {
    calls() += "proj(" + std::to_string(self == the_obj) + "," + std::to_string(p.x) + "," + std::to_string(p.y) + "," +
               std::to_string(p.z) + "," + std::to_string(w11::word(owner)) + "," + std::to_string(map == nullptr) + ");";
    std::memset(self, 0x21, 128);
    w11::u32(self, 76) = w11::word(at(0x400));
}
extern "C" float icos__Fi(int a) { calls() += "cos(" + std::to_string(a) + ");"; return 0.5f; }
extern "C" float isin__Fi(int a) { calls() += "sin(" + std::to_string(a) + ");"; return 0.25f; }
extern "C" void objectAddToRunList__FP10GameObject(void* o) { calls() += "run" + std::to_string(o == the_obj) + snap() + ";"; }
extern "C" void* lumpFindResource__FPcT0(char* l, char* n) { calls() += std::string("lump(") + l + "," + n + ");"; return at(0x800); }
extern "C" int soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* h, void* o, float v) {
    calls() += "snd(" + std::to_string(h == at(0x800)) + "," + std::to_string(o == the_obj) + "," + std::to_string(v) + ");";
    return 0;
}
int main() {
    the_obj = at(0x100);
    std::memset(arena, 0, sizeof arena);
    // Skill object at 0x200 with its table word at +96 pointing at 0x300; the entry at table+104 is
    // {adjust = 16, function word = 7}.
    unsigned char* skill = at(0x200);
    w11::u32(skill, 96) = w11::word(at(0x300));
    w11::i16(at(0x300), 104) = 16;
    w11::u32(at(0x300), 108) = 7;
    w11::u32(at(0x400), 12) = 99;  // the word read through object+76
    functions()[7] = [&](void* self, int a, int b, int c) {
        calls() += "skill(" + std::to_string(static_cast<unsigned char*>(self) - skill) + "," + std::to_string(a) + "," +
                   std::to_string(b) + "," + std::to_string(c) + ");";
        return 10 * b + c;
    };
    int bad = 0;
    for (int amount : {3, -4}) {
        calls().clear();
        std::memset(the_obj, 0, 256);
        __22ExplodeArrowProjectileG6Point3sP10GameObjectiP5Skill(the_obj, Point3{1.0f, -2.0f, 3.5f}, -300, at(0x600), amount, skill);
        const float scale = static_cast<float>(10 * amount + 99);
        std::string want = "proj(1,1.000000,-2.000000,3.500000," + std::to_string(w11::word(at(0x600))) + ",1);skill(16,1," +
                           std::to_string(amount) + ",99);cos(-300);sin(-300);";
        float f84 = 0.5f * scale, f88 = 0.25f * scale;
        unsigned b84, b88;
        std::memcpy(&b84, &f84, 4); std::memcpy(&b88, &f88, 4);
        want += "run1[1," + std::to_string(b84) + "," + std::to_string(b88) + "];lump(fx.lmp,spel_we_explarrow.anm);snd(1,1,1.000000);";
        short h; std::memcpy(&h, the_obj + 100, 2);
        float f28; std::memcpy(&f28, the_obj + 28, 4);
        if (calls() != want) { std::puts(calls().c_str()); std::puts(want.c_str()); bad = 1; }
        if (h != -300 || f28 != 8.0f || w11::u32(the_obj, 104) != static_cast<unsigned>(amount) ||
            w11::u32(the_obj, 112) != w11::word(skill) || w11::u32(the_obj, 92) != 0 || w11::u32(the_obj, 108) != 0 ||
            w11::u32(the_obj, 96) != 0 || the_obj[127] != 0x21 || the_obj[116] != 0x21) bad = 1;
    }
    return bad;
}
