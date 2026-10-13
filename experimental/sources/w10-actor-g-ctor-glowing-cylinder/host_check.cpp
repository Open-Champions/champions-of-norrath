// Host contract for __15GlowingCylinderG6Point3iiii; not an EE input.
#include <cstdio>
#include <cstring>
#include <string>
#include "glowing_cylinder_ctor.cpp"
unsigned char vt_GlowingCylinder[8];
static std::string calls; static int starts;
static float lastParam; static Point3 baseSeen; static void *baseMap = &calls; static char** baseArgs = reinterpret_cast<char**>(&calls);
static unsigned char* theObject; static unsigned wordAt12Seen; static unsigned char byte40AtRun;
extern "C" unsigned __10GameObjectG6Point3PQ210GameObject11resourceMapPPc(void* s, Point3 p, void* m, char** a) {
    calls += "base;"; baseSeen = p; baseMap = m; baseArgs = a; std::memset(static_cast<unsigned char*>(s) + 12, 0x01, 4); return 0;
}
extern "C" void objectAddToRunList__FP10GameObject(void*) {
    calls += "run;"; std::memcpy(&wordAt12Seen, theObject + 12, 4); byte40AtRun = theObject[40];
}
extern "C" int P_StartTrail__Fiifiiiii(int a, int b, float c, int d, int e, int f, int g, int h) {
    ++starts; lastParam = c;
    if (a != 1 || b != 244 || d != 4 || e != 86 || f != 126 || g != 250 || h != 200) calls += "BADSTART;";
    calls += "start;"; return 500 + starts;
}
extern "C" void P_ExtendTrail__FiG6Point3(int t, Point3 p) {
    char b[64]; std::snprintf(b, sizeof b, "ext%d(%g,%g,%g);", t, p.x, p.y, p.z); calls += b;
}
int main() {
    int bad = 0;
    alignas(16) unsigned char object[128]; theObject = object;
    std::memset(object, 0, sizeof object);
    float pos[3] = {1.5f, 2.5f, 3.5f}; std::memcpy(object + 16, pos, 12);
    Point3 given{9, 8, 7};
    __15GlowingCylinderG6Point3iiii(object, given, 0x11223344, 0x12345, -2, 65535);
    unsigned w12, w76, w88, w92, w72; short h80, h82, h84;
    std::memcpy(&w12, object + 12, 4); std::memcpy(&w76, object + 76, 4); std::memcpy(&w88, object + 88, 4); std::memcpy(&w92, object + 92, 4);
    std::memcpy(&w72, object + 72, 4); std::memcpy(&h80, object + 80, 2); std::memcpy(&h82, object + 82, 2); std::memcpy(&h84, object + 84, 2);
    if (calls != "base;run;start;ext501(1.5,2.5,3.5);start;ext502(1.5,2.5,3.5);") { std::printf("fail: call order %s\n", calls.c_str()); bad = 1; }
    if (baseSeen.x != 9 || baseSeen.y != 8 || baseSeen.z != 7 || baseMap != nullptr || baseArgs != nullptr) { std::puts("fail: base arguments"); bad = 1; }
    if ((w12 | 0x4009u) != w12 || wordAt12Seen != w12) { std::puts("fail: flags ORed in before the run list call"); bad = 1; }
    if (w76 != 0x11223344u || h80 != 0x2345 || h82 != -2 || h84 != -1) { std::puts("fail: parameters (word, then three truncated halfwords)"); bad = 1; }
    if (w88 != 501u || w92 != 502u) { std::puts("fail: trail handles"); bad = 1; }
    if (object[40] != 48 || byte40AtRun == 48) { std::puts("fail: byte 40 set after the run list call"); bad = 1; }
    unsigned bits; std::memcpy(&bits, &lastParam, 4);
    if (bits != 0x3ecccccc) { std::puts("fail: trail parameter bits"); bad = 1; }
    if (w72 != static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(vt_GlowingCylinder))) { std::puts("fail: table pointer"); bad = 1; }
    if (!bad) std::puts("glowing cylinder ctor ok");
    return bad;
}
