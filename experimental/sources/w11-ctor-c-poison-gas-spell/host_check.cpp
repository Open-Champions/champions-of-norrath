// Host contract for __14poisonGasSpellG6Point3iiiP10GameObjecti; not an EE input.
#include <cstdio>
#include <cstring>
#include <string>
#include "poison_gas_spell_ctor.cpp"
unsigned char vt_poisonGasSpell[8];
int frameNm;
static std::string calls; static Point3 baseSeen; static void* baseMap = &calls; static char** baseArgs = reinterpret_cast<char**>(&calls);
static unsigned char* theObject; static unsigned wordAt12Seen; static int wordAt96AtLink; static unsigned wordAt76AtLink;
static void* linkHead; static void* linkLink; static void* linkData;
extern "C" unsigned __10GameObjectG6Point3PQ210GameObject11resourceMapPPc(void* s, Point3 p, void* m, char** a) {
    calls += "base;"; baseSeen = p; baseMap = m; baseArgs = a;
    std::memset(static_cast<unsigned char*>(s) + 12, 0, 4);
    return 0;
}
extern "C" void listOnewayLink__FP8ListHeadT0Pv(void* h, void* l, void* d) {
    calls += "link;"; linkHead = h; linkLink = l; linkData = d;
    std::memcpy(&wordAt96AtLink, theObject + 96, 4); std::memcpy(&wordAt76AtLink, theObject + 76, 4);
}
extern "C" void objectAddToRunList__FP10GameObject(void*) {
    calls += "run;"; std::memcpy(&wordAt12Seen, theObject + 12, 4);
}
static int rc(int a, int b, int c, void* list, int d, int expect96) {
    int bad = 0;
    alignas(16) unsigned char object[128]; theObject = object;
    std::memset(object, 0xcc, sizeof object);
    calls.clear(); frameNm = 2000; linkHead = linkLink = linkData = nullptr;
    Point3 given{9, 8, 7};
    __14poisonGasSpellG6Point3iiiP10GameObjecti(object, given, a, b, c, list, d);
    unsigned w12, w76, w72; int w92, w96, w104, w108; float f100; Point3 p80;
    std::memcpy(&w12, object + 12, 4); std::memcpy(&w76, object + 76, 4); std::memcpy(&w72, object + 72, 4);
    std::memcpy(&w92, object + 92, 4); std::memcpy(&w96, object + 96, 4); std::memcpy(&w104, object + 104, 4);
    std::memcpy(&w108, object + 108, 4); std::memcpy(&f100, object + 100, 4); std::memcpy(&p80, object + 80, 12);
    const std::string want = list ? "base;link;run;" : "base;run;";
    if (calls != want) { std::printf("fail: call order %s\n", calls.c_str()); bad = 1; }
    if (baseSeen.x != 9 || baseSeen.y != 8 || baseSeen.z != 7 || baseMap != nullptr || baseArgs != nullptr) { std::puts("fail: base arguments"); bad = 1; }
    if (p80.x != 9 || p80.y != 8 || p80.z != 7) { std::puts("fail: position copy at +80"); bad = 1; }
    if (w76 != 0u) { std::puts("fail: +76"); bad = 1; }
    if (list && (linkHead != object + 76 || linkLink != list || linkData != list || wordAt76AtLink != 0u || wordAt96AtLink != b)) { std::puts("fail: link call"); bad = 1; }
    if (w12 != 3u || wordAt12Seen != w12) { std::puts("fail: flags"); bad = 1; }
    if (w92 != 2000 + a || w104 != c || w108 != d) { std::puts("fail: +92/+104/+108"); bad = 1; }
    if (w96 != expect96) { std::puts("fail: +96 clamp"); bad = 1; }
    if (f100 != 10.0f) { std::puts("fail: +100"); bad = 1; }
    if (w72 != static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(vt_poisonGasSpell))) { std::puts("fail: table"); bad = 1; }
    return bad;
}
int main() {
    int bad = 0;
    int target;
    bad |= rc(30, 5, 0x1234, &target, -7, 10);
    bad |= rc(-4, 10, 11, nullptr, 12, 10);
    bad |= rc(0, 250, 1, &target, 2, 250);
    bad |= rc(7, -3, 1, nullptr, 2, 10);
    return bad;
}
