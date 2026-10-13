// Host contract for __9dustCloudG6Point3iiii; not an EE input.
#include <cstdio>
#include <cstring>
#include <string>
#include "dust_cloud_ctor.cpp"
unsigned char vt_dustCloud[8];
int frameNm;
static std::string calls; static Point3 baseSeen; static void* baseMap = &calls; static char** baseArgs = reinterpret_cast<char**>(&calls);
static unsigned char* theObject; static unsigned wordAt12Seen; static unsigned wordAt92Seen;
extern "C" unsigned __10GameObjectG6Point3PQ210GameObject11resourceMapPPc(void* s, Point3 p, void* m, char** a) {
    calls += "base;"; baseSeen = p; baseMap = m; baseArgs = a;
    std::memset(static_cast<unsigned char*>(s) + 12, 0, 4);  // the base sets the flag word
    return 0;
}
extern "C" void objectAddToRunList__FP10GameObject(void*) {
    calls += "run;"; std::memcpy(&wordAt12Seen, theObject + 12, 4); std::memcpy(&wordAt92Seen, theObject + 92, 4);
}
static int rc(int a, int b, int c, int d, int expect92) {
    int bad = 0;
    alignas(16) unsigned char object[128]; theObject = object;
    std::memset(object, 0xcc, sizeof object);
    calls.clear(); frameNm = 1000;
    Point3 given{9, 8, 7};
    __9dustCloudG6Point3iiii(object, given, a, b, c, d);
    unsigned w12; int w88, w92, w100, w104; float f96; Point3 p76; unsigned w72;
    std::memcpy(&w12, object + 12, 4); std::memcpy(&w88, object + 88, 4); std::memcpy(&w92, object + 92, 4);
    std::memcpy(&w100, object + 100, 4); std::memcpy(&w104, object + 104, 4); std::memcpy(&f96, object + 96, 4);
    std::memcpy(&p76, object + 76, 12); std::memcpy(&w72, object + 72, 4);
    if (calls != "base;run;") { std::printf("fail: call order %s\n", calls.c_str()); bad = 1; }
    if (baseSeen.x != 9 || baseSeen.y != 8 || baseSeen.z != 7 || baseMap != nullptr || baseArgs != nullptr) { std::puts("fail: base arguments"); bad = 1; }
    if (p76.x != 9 || p76.y != 8 || p76.z != 7) { std::puts("fail: position copy at +76"); bad = 1; }
    if (w12 != 3u) { std::puts("fail: flag word"); bad = 1; }
    if (wordAt12Seen != w12) { std::puts("fail: flag ORed in before the run list call"); bad = 1; }
    if (w88 != 1000 + a) { std::puts("fail: +88"); bad = 1; }
    if (w92 != expect92 || static_cast<int>(wordAt92Seen) != expect92) { std::puts("fail: +92 clamp"); bad = 1; }
    if (w100 != c || w104 != d) { std::puts("fail: +100/+104"); bad = 1; }
    if (f96 != 10.0f) { std::puts("fail: +96"); bad = 1; }
    if (w72 != static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(vt_dustCloud))) { std::puts("fail: table"); bad = 1; }
    return bad;
}
int main() {
    int bad = 0;
    bad |= rc(30, 5, 0x1234, -7, 10);    // below the floor of 10
    bad |= rc(-4, 10, 11, 12, 10);       // exactly 10
    bad |= rc(0, 250, 1, 2, 250);        // above
    bad |= rc(7, -3, 1, 2, 10);          // negative is clamped too (signed compare)
    return bad;
}
