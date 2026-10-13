// Host contract for __10SpawnMummyG6Point3PcP10GameObject; not an EE input.
#include <cstdio>
#include <cstring>
#include <string>
#include "spawn_mummy_ctor.cpp"
unsigned char vt_DustDevil[8], vt_SpawnMummy[8];
int frameNm;
static std::string calls; static unsigned char* theObject;
static Point3 baseSeen; static void* baseMap = &calls; static char** baseArgs = reinterpret_cast<char**>(&calls);
static unsigned w12AtRun, w72AtParticle, w72AtRun, w88AtLink, w72AtLink, w100AtLink; static void *linkHead, *linkLink, *linkData;
extern "C" unsigned __10GameObjectG6Point3PQ210GameObject11resourceMapPPc(void* s, Point3 p, void* m, char** a) {
    calls += "base;"; baseSeen = p; baseMap = m; baseArgs = a;
    std::memset(static_cast<unsigned char*>(s) + 12, 0, 4); return 0;
}
extern "C" void* P_CreateParticleSystem__Fv() {
    calls += "particles;"; std::memcpy(&w72AtParticle, theObject + 72, 4); return reinterpret_cast<void*>(0x5150);
}
extern "C" void objectAddToRunList__FP10GameObject(void*) {
    calls += "run;"; std::memcpy(&w12AtRun, theObject + 12, 4); std::memcpy(&w72AtRun, theObject + 72, 4);
}
extern "C" void listOnewayLink__FP8ListHeadT0Pv(void* h, void* l, void* d) {
    calls += "link;"; linkHead = h; linkLink = l; linkData = d;
    std::memcpy(&w88AtLink, theObject + 88, 4); std::memcpy(&w72AtLink, theObject + 72, 4); std::memcpy(&w100AtLink, theObject + 100, 4);
}
static unsigned lo(const void* p) { return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(p)); }
int main() {
    int bad = 0;
    alignas(16) unsigned char object[128]; theObject = object;
    std::memset(object, 0xcc, sizeof object);
    frameNm = 4000;
    char text[4] = "x"; int list;
    Point3 given{9, 8, 7};
    __10SpawnMummyG6Point3PcP10GameObject(object, given, text, &list);
    unsigned w12, w72, w76, w80, w84, w88, w92, w96, w100;
    std::memcpy(&w12, object + 12, 4); std::memcpy(&w72, object + 72, 4); std::memcpy(&w76, object + 76, 4);
    std::memcpy(&w80, object + 80, 4); std::memcpy(&w84, object + 84, 4); std::memcpy(&w88, object + 88, 4);
    std::memcpy(&w92, object + 92, 4); std::memcpy(&w96, object + 96, 4); std::memcpy(&w100, object + 100, 4);
    if (calls != "base;particles;run;link;") { std::printf("fail: call order %s\n", calls.c_str()); bad = 1; }
    if (baseSeen.x != 9 || baseSeen.y != 8 || baseSeen.z != 7 || baseMap != nullptr || baseArgs != nullptr) { std::puts("fail: base arguments"); bad = 1; }
    if (w12 != 9u || w12AtRun != 9u) { std::puts("fail: flag word"); bad = 1; }
    if (w72AtParticle != lo(vt_DustDevil) || w72AtRun != lo(vt_DustDevil)) { std::puts("fail: parent table before the link"); bad = 1; }
    if (w72 != lo(vt_SpawnMummy) || w72AtLink != lo(vt_SpawnMummy)) { std::puts("fail: own table"); bad = 1; }
    if (w84 != 0x5150u || w76 != 0u || w80 != 4060u) { std::puts("fail: +76/+80/+84"); bad = 1; }
    if (w88 != lo(text) || w88AtLink != lo(text)) { std::puts("fail: +88"); bad = 1; }
    if (w92 != 0u || w96 != 0u || w100 != 0u || w100AtLink != 0u) { std::puts("fail: cleared words"); bad = 1; }
    if (linkHead != object + 100 || linkLink != &list || linkData != &list) { std::puts("fail: link arguments"); bad = 1; }
    return bad;
}
