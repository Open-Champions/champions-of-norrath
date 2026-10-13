// Host contract for the HateBeam constructor; not an EE input. Stubs log the call order with the
// object words visible at each call; the GameObject stub fills the object with 0x21 and sets +12.
// The light record is compared field by field as it stood when lightAdd ran.
#define W11_HOST 1
#include "../w11-ctor-a-common/host_support.hpp"
#include "ctor.cpp"
#include <cstdio>
unsigned char vt_HateBeam[8];
int frameNm;
using namespace w11_host;
static unsigned char* the_obj;
static unsigned char light[64];
static std::string snap() {
    return "[" + std::to_string(w11::u32(the_obj, 12)) + "," + std::to_string(w11::u32(the_obj, 168)) + "," + std::to_string(w11::u32(the_obj, 164)) + "," + std::to_string(w11::u32(the_obj, 172)) + "]";
}
extern "C" void game_object_construct(void* self, Point3 p, void* map, char** names) {
    calls() += "go(" + std::to_string(self == the_obj) + "," + std::to_string(p.x) + "," + std::to_string(p.y) + "," + std::to_string(p.z) + "," +
               std::to_string(map == nullptr) + "," + std::to_string(names == nullptr) + ");";
    std::memset(self, 0x21, 256);
    w11::u32(self, 12) = 0x100;
}
extern "C" void lightAdd__FP5Light(void* l) {
    calls() += "light(" + std::to_string(static_cast<unsigned char*>(l) - the_obj) + ");";
    std::memcpy(light, l, 64);
}
extern "C" void listOnewayLink__FP8ListHeadT0Pv(void* head, void* link, void* data) {
    calls() += "link(" + std::to_string(static_cast<unsigned char*>(head) - the_obj) + "," + std::to_string(w11::word(link)) + "," + std::to_string(link == data) + ")" + snap() + ";";
}
extern "C" void objectAddToRunList__FP10GameObject(void* o) { calls() += "run" + std::to_string(o == the_obj) + snap() + ";"; }
static float fl(const unsigned char* p) { float f; std::memcpy(&f, p, 4); return f; }
static short sh(const unsigned char* p) { short s; std::memcpy(&s, p, 2); return s; }
int main() {
    the_obj = at(0x100);
    int bad = 0;
    for (void* creature : {static_cast<void*>(nullptr), static_cast<void*>(at(0x900))}) {
        std::memset(arena, 0, sizeof arena);
        std::memset(light, 0xee, sizeof light);
        calls().clear();
        frameNm = 70;
        void* const skill = at(0xa00);
        __8HateBeamiG6Point3T2P8CreatureP5Skill(the_obj, 33, Point3{1.0f, -2.0f, 3.5f}, Point3{9.0f, 9.0f, 9.0f}, creature, skill);
        const std::string pat = std::to_string(0x21212121u);
        std::string want = "go(1,1.000000,-2.000000,3.500000,1,1);light(104);";
        if (creature) want += "link(76," + std::to_string(w11::word(creature)) + ",1)[" + std::to_string(0x100 | 0x421b) + ",33," + pat + "," + pat + "];";
        want += "run1[" + std::to_string(0x100 | 0x421b) + ",33," + std::to_string(w11::word(skill)) + "," + std::to_string(250) + "];";
        if (calls() != want) { std::puts(calls().c_str()); std::puts(want.c_str()); bad = 1; }
        if (light[8] != 0 || light[9] != 0 || sh(light + 10) != 115 || sh(light + 12) != 10 || sh(light + 14) != 0 || sh(light + 16) != -32 ||
            sh(light + 18) != 115 || sh(light + 20) != 10 || sh(light + 22) != 0 || sh(light + 24) != -32 || fl(light + 28) != 156.0f ||
            fl(light + 36) != 156.0f || fl(light + 32) != fl(light + 40) || w11::u32(light, 44) != 0 ||
            fl(light + 48) != 1.0f || fl(light + 52) != -2.0f || fl(light + 56) != 3.5f) { std::puts("light"); bad = 1; }
        { unsigned bits = 0x382c598au; float f; std::memcpy(&f, &bits, 4); if (fl(light + 32) != f) { std::puts("small"); bad = 1; } }
        if (w11::u32(the_obj, 72) != w11::word(vt_HateBeam) || w11::u32(the_obj, 76) != 0 || the_obj[255] != 0x21 || the_obj[200] != 0x21) bad = 1;
    }
    return bad;
}
