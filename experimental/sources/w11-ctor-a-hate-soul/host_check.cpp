// Host contract for the HateSoul constructor; not an EE input. Stubs log the call order with the
// arguments and the object words visible at each call. The random stub returns a value the test sets
// so both outcomes of the final check are reached.
#define W11_HOST 1
#include "../w11-ctor-a-common/host_support.hpp"
#include "ctor.cpp"
#include <cstdio>
unsigned char vt_HateSoul[8];
int frameNm;
using namespace w11_host;
static unsigned char* the_obj;
static unsigned char light[64];
static unsigned roll_value;
static float fl(const unsigned char* p) { float f; std::memcpy(&f, p, 4); return f; }
static short sh(const unsigned char* p) { short s; std::memcpy(&s, p, 2); return s; }
static std::string snap() {
    return "[" + std::to_string(w11::u32(the_obj, 12)) + "," + std::to_string(w11::u32(the_obj, 160)) + "," + std::to_string(w11::u32(the_obj, 164)) + "," +
           std::to_string(w11::u32(the_obj, 84)) + "]";
}
extern "C" void projectile_construct(void* self, Point3 p, void* owner, void* map) {
    calls() += "proj(" + std::to_string(self == the_obj) + "," + std::to_string(p.x) + "," + std::to_string(p.y) + "," + std::to_string(p.z) + "," +
               std::to_string(owner == nullptr) + "," + std::to_string(map == nullptr) + ");";
    std::memset(self, 0x21, 512);
    w11::u32(self, 12) = 0x100;
    w11::f32(self, 16) = 10.0f; w11::f32(self, 20) = -20.0f; w11::f32(self, 24) = 1.5f;
}
extern "C" void lightAdd__FP5Light(void* l) {
    calls() += "light(" + std::to_string(static_cast<unsigned char*>(l) - the_obj) + ");";
    std::memcpy(light, l, 64);
}
extern "C" unsigned eRand__10GameObject(void* o) { calls() += "rand" + std::to_string(o == the_obj) + snap() + ";"; return roll_value; }
extern "C" void objectAddToRunList__FP10GameObject(void* o) { calls() += "run" + std::to_string(o == the_obj) + snap() + ";"; }
extern "C" int P_StartTrail__Fiifiiiii(int a, int b, float c, int d, int e, int f, int g, int h) {
    calls() += "start(" + std::to_string(a) + "," + std::to_string(b) + "," + std::to_string(c) + "," + std::to_string(d) + "," + std::to_string(e) + "," +
               std::to_string(f) + "," + std::to_string(g) + "," + std::to_string(h) + ");";
    return 0x2301;
}
extern "C" void P_ExtendTrail__FiG6Point3(int t, Point3 p) {
    calls() += "ext(" + std::to_string(t) + "," + std::to_string(p.x) + "," + std::to_string(p.y) + "," + std::to_string(p.z) + ");";
}
extern "C" void lightDeactivate__FP5Light(void* l) { calls() += "deact(" + std::to_string(static_cast<unsigned char*>(l) - the_obj) + ");"; }
int main() {
    the_obj = at(0x100);
    int bad = 0;
    struct Case { unsigned roll; int c; bool deactivate; };
    for (Case k : {Case{2, 0, false}, Case{3, 0, true}, Case{3, 5, false}, Case{1, 7, false}}) {
        std::memset(arena, 0, sizeof arena);
        std::memset(light, 0xee, sizeof light);
        calls().clear();
        frameNm = 70;
        roll_value = k.roll;
        __8HateSoulG6Point3T1iii(the_obj, Point3{1.0f, -2.0f, 3.5f}, Point3{7.0f, 8.0f, 9.0f}, 0x10005, 99, k.c);
        const std::string pat = std::to_string(0x21212121u);
        const std::string flagged = std::to_string(0x4103);
        std::string want = "proj(1,1.000000,-2.000000,3.500000,1,1);light(100);rand1[256," + pat + "," + pat + "," + pat + "];run1[" + flagged + "," + std::to_string(k.roll & 1) +
                           "," + std::to_string(k.c) + ",70];start(8,35,0.322000,4,255,135,100,171);ext(8961,10.000000,-20.000000,1.500000);";
        if (k.deactivate) want += "deact(100);";
        if (calls() != want) { std::puts(calls().c_str()); std::puts(want.c_str()); bad = 1; }
        unsigned bits = 0x3891a2b3u; float a; std::memcpy(&a, &bits, 4);
        if (light[8] != 0 || light[9] != 0 || sh(light + 10) != 200 || sh(light + 12) != 30 || sh(light + 14) != 0 || sh(light + 16) != -32 ||
            sh(light + 18) != 200 || sh(light + 20) != 30 || sh(light + 22) != 0 || sh(light + 24) != -32 || fl(light + 28) != 120.0f ||
            fl(light + 36) != 120.0f || fl(light + 32) != a || fl(light + 40) != a || fl(light + 44) != 0.0f ||
            fl(light + 48) != 1.0f || fl(light + 52) != -2.0f || fl(light + 56) != 3.5f) { std::puts("light"); bad = 1; }
        if (w11::u32(the_obj, 72) != w11::word(vt_HateSoul) || fl(the_obj + 28) != 0.0f || sh(the_obj + 56) != 5 || fl(the_obj + 88) != 7.0f ||
            fl(the_obj + 92) != 8.0f || fl(the_obj + 96) != 9.0f || w11::u32(the_obj, 168) != 0x2301 || the_obj[511] != 0x21 || the_obj[172] != 0x21)
            { std::puts("fields"); bad = 1; }
    }
    return bad;
}
