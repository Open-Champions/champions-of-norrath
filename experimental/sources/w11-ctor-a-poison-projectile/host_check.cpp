// Host contract for the PoisonProjectile constructor; not an EE input. Stubs log the call order and
// the object words visible at each call; the Projectile stub fills the object with 0x21 and points
// the word at +36 at a table of eight-byte entries whose second words are header tags.
#define W11_HOST 1
#include "../w11-ctor-a-common/host_support.hpp"
#include "ctor.cpp"
#include <cstdio>
unsigned char vt_PoisonProjectile[8];
int frameNm;
using namespace w11_host;
static unsigned char* the_obj;
static std::string snap() {
    return "[" + std::to_string(w11::u32(the_obj, 164)) + "," + std::to_string(w11::u32(the_obj, 156)) + "," + std::to_string(w11::u32(the_obj, 12)) + "]";
}
extern "C" void projectile_construct(void* self, Point3 p, void* owner, void* map) {
    calls() += "proj(" + std::to_string(self == the_obj) + "," + std::to_string(p.x) + "," + std::to_string(p.y) + "," + std::to_string(p.z) + "," +
               std::to_string(w11::word(owner)) + "," + std::to_string(w11::word(map)) + ");";
    std::memset(self, 0x21, 256);
    w11::u32(self, 12) = 0x100;
    w11::u32(self, 36) = w11::word(at(0x800));
}
extern "C" void animInitState__FP14AnimationState(void* s) {
    calls() += "init(" + std::to_string(static_cast<unsigned char*>(s) - the_obj) + "," + std::to_string(the_obj[105]) + "," + std::to_string(w11::u32(the_obj, 84)) + "," +
               std::to_string(w11::u32(the_obj, 88)) + "," + std::to_string(w11::u32(the_obj, 92)) + "," + std::to_string(w11::u32(the_obj, 96)) + ");";
}
extern "C" void animAdd__FP14AnimationStateP15AnimationHeaderi(void* s, void* h, int flags) {
    calls() += "add(" + std::to_string(static_cast<unsigned char*>(s) - the_obj) + "," + std::to_string(w11::word(h)) + "," + std::to_string(flags) + ");";
}
extern "C" void animRandomizeTime__FP14AnimationState(void* s) { calls() += "rand(" + std::to_string(static_cast<unsigned char*>(s) - the_obj) + ");"; }
extern "C" void objectAddToRunList__FP10GameObject(void* o) { calls() += "run" + std::to_string(o == the_obj) + snap() + ";"; }
static float fl(const unsigned char* p) { float f; std::memcpy(&f, p, 4); return f; }
int main() {
    the_obj = at(0x100);
    int bad = 0;
    int idx = 0;
    for (int c : {0, 2}) {
        std::memset(arena, 0, sizeof arena);
        calls().clear();
        for (int j = 0; j < 4; ++j) w11::u32(at(0x800), 8 * j + 4) = 1000 + j;
        frameNm = 50 + idx++;
        __16PoisonProjectilePQ210GameObject11resourceMapiiiiG6Point3T6ifP8Creature(the_obj, at(0x700), 11, 22, c, 44, Point3{1.0f, -2.0f, 3.5f},
                                                                                Point3{10.0f, 20.0f, 30.0f}, -5, 2.5f, at(0x900));
        // before the run-list call the words at +164 and +156 are still the 0x21 pattern
        const std::string pat = std::to_string(0x21212121u);
        const std::string w2 = "proj(1,1.000000,-2.000000,3.500000," + std::to_string(w11::word(at(0x900))) + "," + std::to_string(w11::word(at(0x700))) +
             ");init(100,1,11,22," + std::to_string(c) + ",44);add(100," + std::to_string(1000 + c) + ",1);rand(100);run1[" + pat + "," + pat + ",256];";
        if (calls() != w2) { std::puts(calls().c_str()); std::puts(w2.c_str()); bad = 1; }
        short h; std::memcpy(&h, the_obj + 56, 2);
        if (fl(the_obj + 160) != 2.5f || fl(the_obj + 164) != 3.5f || fl(the_obj + 140) != 10.0f || fl(the_obj + 144) != 20.0f || fl(the_obj + 148) != 30.0f ||
            h != -5 || w11::u32(the_obj, 152) != 0 || w11::u32(the_obj, 12) != 0x101 || w11::u32(the_obj, 156) != static_cast<unsigned>(frameNm + 300) ||
            w11::u32(the_obj, 72) != w11::word(vt_PoisonProjectile) || the_obj[255] != 0x21 || the_obj[168] != 0x21)
            bad = 1;
    }
    return bad;
}
