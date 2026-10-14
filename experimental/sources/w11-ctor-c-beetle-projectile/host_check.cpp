// Host contract for __16beetleProjectileG6Point3T1iP8CreatureiiiiiP15AnimationHeader; not an EE input.
#include <cstdio>
#include <cstring>
#include <string>
#include "beetle_projectile_ctor.cpp"
unsigned char vt_beetleProjectile[8];
static std::string calls; static unsigned char* theObject;
static void* seenSelf; static int seenZero = -1; static void *seenModel = &calls, *seenTex = &calls, *seenVagA = &calls, *seenVagB = &calls, *seenDesc = &calls;
static Point3 seenP, seenQ; static int seenN, seenI[5], seenFour; static void* seenCreature;
static unsigned wordAt72AtLink; static void *linkHead, *linkLink, *linkData;
extern "C" unsigned __13MissileWeaponiP12_modelHeaderP8_textureG6Point3T4iP8CreatureP10_vagHeaderT8iiiiiiPQ24item11description(
    void* self, int zero, void* modelHeader, void* texture, Point3 p, Point3 q, int n, void* creature,
    void* vagA, void* vagB, int i1, int i2, int i3, int i4, int i5, int four, void* description) {
    calls += "base;"; seenSelf = self; seenZero = zero; seenModel = modelHeader; seenTex = texture; seenP = p; seenQ = q;
    seenN = n; seenCreature = creature; seenVagA = vagA; seenVagB = vagB;
    seenI[0] = i1; seenI[1] = i2; seenI[2] = i3; seenI[3] = i4; seenI[4] = i5; seenFour = four; seenDesc = description;
    std::memset(theObject + 72, 0x77, 4); std::memset(theObject + 452, 0x77, 8);  // base-constructed bytes
    return 0;
}
extern "C" void listOnewayLink__FP8ListHeadT0Pv(void* h, void* l, void* d) {
    calls += "link;"; linkHead = h; linkLink = l; linkData = d;
    std::memcpy(&wordAt72AtLink, theObject + 72, 4);
}
static int rc(void* creature, void* header) {
    int bad = 0;
    alignas(16) unsigned char object[512]; theObject = object;
    std::memset(object, 0xcc, sizeof object);
    calls.clear(); linkHead = linkLink = linkData = nullptr;
    Point3 p{1, 2, 3}, q{4, 5, 6};
    __16beetleProjectileG6Point3T1iP8CreatureiiiiiP15AnimationHeader(object, p, q, 99, creature, 11, 22, 33, 44, 55, header);
    unsigned w72, w452, w456;
    std::memcpy(&w72, object + 72, 4); std::memcpy(&w452, object + 452, 4); std::memcpy(&w456, object + 456, 4);
    const std::string want = creature ? "base;link;" : "base;";
    if (calls != want) { std::printf("fail: call order %s\n", calls.c_str()); bad = 1; }
    if (seenSelf != object || seenZero != 0 || seenModel || seenTex || seenVagA || seenVagB || seenDesc) { std::puts("fail: null/zero base arguments"); bad = 1; }
    if (seenP.x != 1 || seenP.y != 2 || seenP.z != 3 || seenQ.x != 4 || seenQ.y != 5 || seenQ.z != 6) { std::puts("fail: positions"); bad = 1; }
    if (seenN != 99 || seenCreature != creature) { std::puts("fail: n/creature"); bad = 1; }
    if (seenI[0] != 11 || seenI[1] != 22 || seenI[2] != 33 || seenI[3] != 44 || seenI[4] != 55 || seenFour != 4) { std::puts("fail: ints"); bad = 1; }
    if (w72 != static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(vt_beetleProjectile))) { std::puts("fail: table"); bad = 1; }
    if (w452 != 0u) { std::puts("fail: +452 cleared"); bad = 1; }
    if (creature && (linkHead != object + 452 || linkLink != creature || linkData != creature)) { std::puts("fail: link arguments"); bad = 1; }
    if (w456 != static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(header))) { std::puts("fail: +456"); bad = 1; }
    if (creature && wordAt72AtLink != static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(vt_beetleProjectile))) { std::puts("fail: table set before link"); bad = 1; }
    return bad;
}
int main() {
    int bad = 0;
    bad |= rc(reinterpret_cast<void*>(0x1000), reinterpret_cast<void*>(0x2000));
    bad |= rc(nullptr, reinterpret_cast<void*>(0x3000));
    return bad;
}
