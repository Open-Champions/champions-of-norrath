// Host contract for the DiseaseBolt constructor; not an EE input. Stubs log the call order with their
// arguments. The Projectile stub fills the object with 0x21 and writes a position; the sorted-list
// stub builds a node list in the arena; the owner's table entry accepts creatures whose address has
// bit 5 set. The contract fails on a wrong value, order, argument or filter.
#define W11_HOST 1
#include "../w11-ctor-a-common/host_support.hpp"
#include "ctor.cpp"
#include <cstdio>
unsigned char vt_DiseaseBolt[8];
int frameNm;
using namespace w11_host;
static unsigned char* the_obj;
static int node_count;
static float fl(const unsigned char* p) { float f; std::memcpy(&f, p, 4); return f; }
static short sh(const unsigned char* p) { short s; std::memcpy(&s, p, 2); return s; }
extern "C" void projectile_construct(void* self, Point3 p, void* owner, void* map) {
    calls() += "proj(" + std::to_string(self == the_obj) + "," + std::to_string(p.x) + "," + std::to_string(p.y) + "," + std::to_string(p.z) + "," +
               std::to_string(w11::word(owner)) + "," + std::to_string(map == nullptr) + ");";
    std::memset(self, 0x21, 256);
    w11::f32(self, 16) = 10.0f; w11::f32(self, 20) = -20.0f; w11::f32(self, 24) = 1.5f;
}
extern "C" float icos__Fi(int a) { calls() += "cos(" + std::to_string(a) + ");"; return 0.5f; }
extern "C" float isin__Fi(int a) { calls() += "sin(" + std::to_string(a) + ");"; return 0.25f; }
extern "C" void listOnewayLink__FP8ListHeadT0Pv(void* head, void* link, void* data) {
    calls() += "link(" + std::to_string(static_cast<unsigned char*>(head) - the_obj) + "," + std::to_string(w11::word(link)) + "," + std::to_string(link == data) + ");";
}
extern "C" void objectGetSortedCreatureList__FG6Point3sffP8ListHeadP8Creature(Point3 p, short h, float a, float b, void* list, void* creature) {
    calls() += "sorted(" + std::to_string(p.x) + "," + std::to_string(p.y) + "," + std::to_string(p.z) + "," + std::to_string(h) + "," + std::to_string(a) + "," +
               std::to_string(b) + "," + std::to_string(creature == nullptr) + ");";
    unsigned* head = static_cast<unsigned*>(list);
    for (int i = 0; i < node_count; ++i) {
        unsigned char* node = at(0x1000 + 16 * i);
        w11::u32(node, 0) = i + 1 < node_count ? w11::word(at(0x1000 + 16 * (i + 1))) : 0;
        w11::u32(node, 12) = w11::word(at(0x2000 + 0x20 * i));
    }
    *head = node_count ? w11::word(at(0x1000)) : 0;
}
extern "C" void listDestroy__FP8ListHead(void* list) { calls() += "destroy(" + std::to_string(*static_cast<unsigned*>(list)) + ");"; }
extern "C" void objectAddToRunList__FP10GameObject(void* o) { calls() += "run" + std::to_string(o == the_obj) + ";"; }
extern "C" void* lumpFindResource__FPcT0(char* l, char* n) { calls() += std::string("lump(") + l + "," + n + ");"; return at(0x800); }
extern "C" int soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* h, void* o, float v) {
    calls() += "snd(" + std::to_string(h == at(0x800)) + "," + std::to_string(o == the_obj) + "," + std::to_string(v) + ");";
    return 0;
}
int main() {
    the_obj = at(0x100);
    unsigned char* owner_obj = at(0x900);
    int bad = 0;
    for (int variant = 0; variant < 4; ++variant) {
        node_count = variant == 3 ? 3 : variant;
        void* owner = variant == 2 ? nullptr : owner_obj;
        std::memset(arena, 0, sizeof arena);
        calls().clear();
        frameNm = 70;
        w11::u32(owner_obj, 72) = w11::word(at(0xa00));
        w11::i16(at(0xa00), 160) = 8;
        w11::u32(at(0xa00), 164) = 5;
        functions()[5] = [&](void* self, int creature, int, int) {
            const bool accept = (static_cast<unsigned>(creature) >> 5) & 1;
            calls() += "owner(" + std::to_string(static_cast<unsigned char*>(self) - owner_obj) + "," + std::to_string(accept) + ");";
            return accept ? 1 : 0;
        };
        void* const skill = at(0xc00);
        __11DiseaseBoltsiG6Point3P10GameObjectP5Skill(the_obj, 300, 0x105, Point3{1.0f, -2.0f, 3.5f}, owner, skill);
        std::string want = "proj(1,1.000000,-2.000000,3.500000," + std::to_string(w11::word(owner)) + ",1);cos(300);sin(300);";
        if (owner) want += "link(84," + std::to_string(w11::word(owner)) + ",1);";
        if (owner) {
            want += "sorted(10.000000,-20.000000,1.500000,300,279.999878,60.000000,1);";
            for (int i = 0; i < node_count; ++i) {
                const unsigned creature = w11::word(at(0x2000 + 0x20 * i));
                const bool accept = (creature >> 5) & 1;
                want += "owner(8," + std::to_string(accept) + ");";
                if (accept) want += "link(88," + std::to_string(creature) + ",1);";
            }
            want += "destroy(" + std::to_string(node_count ? w11::word(at(0x1000)) : 0u) + ");";
        }
        want += "run1;lump(fx.lmp,spel_de_disbolt.anm);snd(1,1,1.000000);";
        if (calls() != want) { std::puts(calls().c_str()); std::puts(want.c_str()); bad = 1; }
        if (w11::u32(the_obj, 84) != 0 || w11::u32(the_obj, 88) != 0 || w11::u32(the_obj, 108) != 0 || w11::u32(the_obj, 120) != 0 ||
            fl(the_obj + 100) != 175.0f || fl(the_obj + 104) != 87.5f || sh(the_obj + 56) != 300 || sh(the_obj + 96) != 0x105 ||
            w11::u32(the_obj, 92) != w11::word(skill) || w11::u32(the_obj, 112) != 370u || w11::u32(the_obj, 116) != 80u ||
            w11::u32(the_obj, 72) != w11::word(vt_DiseaseBolt) || the_obj[255] != 0x21 || the_obj[124] != 0x21) { std::puts("fields"); bad = 1; }
    }
    return bad;
}
