// Host contract for the PoisonRain constructor; not an EE input. Stubs log the call order with their
// arguments. The skill table entry (byte 112) returns scripted values in call order, so a wrong K, a
// dropped argument or a swapped result shows in the logged calls and the stored words. The LightEffect
// stub records all fourteen arguments.
#define W11_HOST 1
#include "../w11-ctor-a-common/host_support.hpp"
#include "ctor.cpp"
#include <cstdio>
unsigned char vt_PoisonRain[8];
int frameNm;
using namespace w11_host;
static unsigned char* the_obj;
static bool alloc_ok;
static std::vector<int> scripted;
static size_t scripted_index;
static float fl(const unsigned char* p) { float f; std::memcpy(&f, p, 4); return f; }
static std::string num(float f) { return std::to_string(f); }
extern "C" void game_object_construct(void* self, Point3 p, void* map, char** names) {
    calls() += "go(" + std::to_string(self == the_obj) + "," + num(p.x) + "," + num(p.y) + "," + num(p.z) + "," + std::to_string(map == nullptr) + "," +
               std::to_string(names == nullptr) + ");";
    std::memset(self, 0x21, 512);
    w11::u32(self, 12) = 0x100;
}
extern "C" void listOnewayLink__FP8ListHeadT0Pv(void* head, void* link, void* data) {
    calls() += "link(" + std::to_string(static_cast<unsigned char*>(head) - the_obj) + "," + std::to_string(w11::word(link)) + "," + std::to_string(link == data) + ")[" +
               std::to_string(w11::u32(the_obj, 12)) + "," + std::to_string(w11::u32(the_obj, 92)) + "," + std::to_string(w11::u32(the_obj, 96)) + "," +
               std::to_string(w11::u32(the_obj, 100)) + "];";
}
extern "C" void* blockAlloc__Fi(int size) { calls() += "alloc(" + std::to_string(size) + ");"; return alloc_ok ? at(0xc00) : nullptr; }
extern "C" void light_effect_construct(void* self, Point3 p, float a, float b, float c, float d, float e, float f, float g, float h,
                                       float i, float j, float k, float l, void* owner, int flag) {
    calls() += "le(" + std::to_string(self == at(0xc00)) + "," + num(p.x) + "," + num(p.y) + "," + num(p.z) + "," + num(a) + "," + num(b) + "," + num(c) + "," + num(d) + "," +
               num(e) + "," + num(f) + "," + num(g) + "," + num(h) + "," + num(i) + "," + num(j) + "," + num(k) + "," + num(l) + "," + std::to_string(owner == nullptr) + "," +
               std::to_string(flag) + ");";
}
extern "C" int soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* h, void* o, float v) {
    calls() += "snd(" + std::to_string(w11::word(h)) + "," + std::to_string(o == the_obj) + "," + num(v) + ");";
    return 0x66;
}
extern "C" void objectAddToRunList__FP10GameObject(void* o) { calls() += "run" + std::to_string(o == the_obj) + ";"; }
int main() {
    the_obj = at(0x100);
    unsigned char* skill = at(0x600);
    unsigned char* owner = at(0x900);
    int bad = 0;
    struct Case { unsigned owner_flags; bool alloc; bool header; std::vector<int> results; };
    const Case cases[] = {
        {0x20, true, true, {3, 4, 5, 6}},   // flags test passes, allocation works, header present
        {0x60, false, false, {1, -2, 0}},   // flags test fails, allocation fails, no header (the third skill call is skipped)
    };
    for (const Case& k : cases) {
        std::memset(arena, 0, sizeof arena);
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
                       std::to_string(c == static_cast<int>(w11::word(the_obj + 100))) + ")[" + std::to_string(w11::u32(the_obj, 80)) + "];";
            return r;
        };
        void* const header = k.header ? at(0xa00) : nullptr;
        __10PoisonRainiP10GameObjectG6Point3P5SkillP15AnimationHeader(the_obj, 7, owner, Point3{1.0f, -2.0f, 3.5f}, skill, header);
        const std::string pat = std::to_string(0x21212121u);
        const bool match = (k.owner_flags & 0x60) == 0x20;
        const int word80 = match ? static_cast<int>(70.0f + __builtin_bit_cast(float, 0x4221ffffu)) : 0;
        std::string want = "go(1,1.000000,-2.000000,3.500000,1,1);link(100," + std::to_string(w11::word(owner)) + ",1)[" + std::to_string(0x100 | 0x421b) + ",7," +
                           std::to_string(w11::word(skill)) + ",0];";
        want += "skill(16,1,7,1)[" + pat + "];skill(16,2,7,1)[" + std::to_string(word80) + "];";
        want += "alloc(188);";
        const float base = static_cast<float>(k.results[0] * 12);
        if (k.alloc)
            want += "skill(16,2,7,1)[" + std::to_string(word80) + "];le(1,1.000000,-2.000000,15.500000,0.000000,255.000000,0.000000,100.000000,0.000000,255.000000,0.000000,100.000000," +
                    num(base) + "," + num(base + base) + "," + num(__builtin_bit_cast(float, 0x3f266666u)) + "," +
                    num(static_cast<float>(k.results[2]) + __builtin_bit_cast(float, 0x3f2cccccu)) + ",1,1);";
        want += "skill(16,0,7,1)[" + std::to_string(word80) + "];";
        if (header) want += "snd(" + std::to_string(w11::word(header)) + ",1,1.000000);";
        want += "run1;";
        // The Case with allocation failing has only 3 scripted results; the fourth call (K = 0) is the third result there.
        if (calls() != want && !(!k.alloc)) { std::puts(calls().c_str()); std::puts(want.c_str()); bad = 1; }
        if (!k.alloc) {
            // Rebuild the expected log for the no-allocation path: calls are K=1, K=2, alloc, K=0.
            std::string w2 = "go(1,1.000000,-2.000000,3.500000,1,1);link(100," + std::to_string(w11::word(owner)) + ",1)[" + std::to_string(0x100 | 0x421b) + ",7," +
                             std::to_string(w11::word(skill)) + ",0];skill(16,1,7,1)[" + pat + "];skill(16,2,7,1)[" + std::to_string(word80) + "];alloc(188);skill(16,0,7,1)[" +
                             std::to_string(word80) + "];run1;";
            if (calls() != w2) { std::puts(calls().c_str()); std::puts(w2.c_str()); bad = 1; }
        }
        const int r2 = k.results[1];
        if (w11::u32(the_obj, 72) != w11::word(vt_PoisonRain) || fl(the_obj + 84) != base || w11::i32(the_obj, 80) != word80 ||
            w11::i32(the_obj, 76) != word80 + 60 * r2 || w11::i32(the_obj, 88) != (k.alloc ? k.results[3] : k.results[2]) ||
            w11::i32(the_obj, 104) != (header ? 0x66 : -1) || the_obj[511] != 0x21)
            { std::puts("fields"); bad = 1; }
    }
    return bad;
}
