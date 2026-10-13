// Host contract for the PoisonArrowProjectile constructor; not an EE input. Stubs log the call order with their
// arguments. The Projectile stub fills the object with 0x21 and sets the +76 word; the skill table
// entry returns values derived from its arguments, so a swapped or dropped argument shows in the
// stored velocity. The light record at +112 is compared field by field.
#define W11_HOST 1
#include "../w11-ctor-a-common/host_support.hpp"
#include "ctor.cpp"
#include <cstdio>
unsigned char vt_PoisonArrowProjectile[8];
int frameNm;
using namespace w11_host;
static unsigned char* the_obj;
static unsigned char* light_snapshot;
extern "C" void projectile_construct(void* self, Point3 p, void* owner, void* map) {
    calls() += "proj(" + std::to_string(self == the_obj) + "," + std::to_string(p.x) + "," + std::to_string(p.y) + "," +
               std::to_string(p.z) + "," + std::to_string(w11::word(owner)) + "," + std::to_string(map == nullptr) + ");";
    std::memset(self, 0x21, 256);
}
extern "C" void lightAdd__FP5Light(void* light) {
    calls() += "light(" + std::to_string(static_cast<unsigned char*>(light) - the_obj) + ");";
    std::memcpy(light_snapshot, light, 64);
}
extern "C" float icos__Fi(int a) { calls() += "cos(" + std::to_string(a) + ");"; return 0.5f; }
extern "C" float isin__Fi(int a) { calls() += "sin(" + std::to_string(a) + ");"; return 0.25f; }
extern "C" void objectAddToRunList__FP10GameObject(void* o) { calls() += "run" + std::to_string(o == the_obj) + ";"; }
extern "C" void* lumpFindResource__FPcT0(char* l, char* n) { calls() += std::string("lump(") + l + "," + n + ");"; return at(0x800); }
extern "C" int soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* h, void* o, float v) {
    calls() += "snd(" + std::to_string(h == at(0x800)) + "," + std::to_string(o == the_obj) + "," + std::to_string(v) + ");";
    return 0;
}
static float fl(const unsigned char* p) { float f; std::memcpy(&f, p, 4); return f; }
static short sh(const unsigned char* p) { short s; std::memcpy(&s, p, 2); return s; }
int main() {
    the_obj = at(0x100);
    unsigned char light[64];
    light_snapshot = light;
    unsigned char* skill = at(0x200);
    int bad = 0;
    for (int amount : {3, -4}) {
        std::memset(arena, 0, sizeof arena);
        std::memset(light, 0xee, sizeof light);
        calls().clear();
        w11::u32(skill, 96) = w11::word(at(0x300));
        w11::i16(at(0x300), 88) = 16;
        w11::u32(at(0x300), 92) = 7;
        functions3()[7] = [&](void* self, int a, int b) {
            calls() += "skill(" + std::to_string(static_cast<unsigned char*>(self) - skill) + "," + std::to_string(a) + "," + std::to_string(b) + ");";
            return 100 * a + b;
        };
        __21PoisonArrowProjectileG6Point3sP10GameObjectiP5Skill(the_obj, Point3{1.0f, -2.0f, 3.5f}, -300, at(0x600), amount, skill);
        const float scale = static_cast<float>(100 * 1 + amount);
        std::string want = "proj(1,1.000000,-2.000000,3.500000," + std::to_string(w11::word(at(0x600))) + ",1);light(112);skill(16,1," +
                           std::to_string(amount) + ");cos(-300);sin(-300);run1;lump(fx.lmp,spel_we_poisarrow.anm);snd(1,1,1.000000);";
        if (calls() != want) { std::puts(calls().c_str()); std::puts(want.c_str()); bad = 1; }
        float f84 = 0.5f * scale, f88 = 0.25f * scale;
        const unsigned char* o = the_obj;
        if (fl(o + 84) != f84 || fl(o + 88) != f88 || fl(o + 28) != 5.0f || sh(o + 100) != -300 || w11::u32(the_obj, 104) != static_cast<unsigned>(amount) ||
            w11::u32(the_obj, 172) != w11::word(skill) || w11::u32(the_obj, 92) != 0 || w11::u32(the_obj, 108) != 0 || w11::u32(the_obj, 96) != 0 ||
            o[255] != 0x21 || o[200] != 0x21) bad = 1;
        // The light record as it stood when lightAdd ran.
        if (light[8] != 0 || light[9] != 0 || sh(light + 10) != 60 || sh(light + 12) != 132 || sh(light + 14) != 68 || sh(light + 16) != 0 ||
            sh(light + 18) != 60 || sh(light + 20) != 132 || sh(light + 22) != 68 || sh(light + 24) != 0 ||
            fl(light + 28) != 120.0f || fl(light + 36) != 120.0f || fl(light + 32) != fl(light + 40) ||
            fl(light + 44) != 0.0f || fl(light + 48) != 1.0f || fl(light + 52) != -2.0f || fl(light + 56) != 3.5f) { std::puts("light"); bad = 1; }
        { unsigned bits = 0x3891a2b3u; float want_small; std::memcpy(&want_small, &bits, 4); if (fl(light + 32) != want_small) { std::puts("small"); bad = 1; } }
    }
    return bad;
}
