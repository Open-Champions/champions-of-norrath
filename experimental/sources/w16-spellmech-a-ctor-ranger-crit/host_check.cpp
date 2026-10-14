// Host contract for __22SpellRangerCriticalHitiP8CreatureP5Skill; not an EE input. Stored 32-bit addresses are
// offsets into an arena (NORRATH_HOST_ARENA). Checks stored fields, the light block, call order and arguments,
// the signed byte argument, the integer-to-float conversion and the particle handle.
#define NORRATH_HOST_ARENA 1
#include "ctor.cpp"
#include <cstdio>
#include <cstring>
#include <string>

alignas(16) static unsigned char arena[0x4000];
static const unsigned base = 0x100000u;
unsigned char vt_Spell[8];
unsigned char vt_RangerCriticalHit[8];
unsigned syncRandState;
static std::string calls;
static int slotResult;
static unsigned char* slotSeenThis;
static int slotSeenA, slotSeenB;
static unsigned char* lightSeen;
static unsigned char lightCopy[48];

static int slotTarget(void* self, int a, int b) {
    calls += "slot;"; slotSeenThis = static_cast<unsigned char*>(self); slotSeenA = a; slotSeenB = b;
    return slotResult;
}
extern "C" void* P_CreateParticleSystem__Fv() { calls += "particle;"; return arena + 0x3000; }
extern "C" void lightAdd__FP5Light(void* light) {
    calls += "light;"; lightSeen = static_cast<unsigned char*>(light); std::memcpy(lightCopy, light, 48);
}
static const unsigned fnAddress = 0x7f0000u, spellAddress = 0x7f0010u, critAddress = 0x7f0020u;
extern "C" unsigned char* norrath_host_pointer(unsigned a) {
    if (a == fnAddress) return reinterpret_cast<unsigned char*>(slotTarget);
    return arena + (a - base);
}
extern "C" unsigned norrath_host_address(const void* p) {
    const unsigned char* q = static_cast<const unsigned char*>(p);
    if (q >= arena && q < arena + sizeof arena) return base + static_cast<unsigned>(q - arena);
    if (q == vt_Spell) return spellAddress;
    if (q == vt_RangerCriticalHit) return critAddress;
    return 0xdeadbeefu;
}
static unsigned word(const unsigned char* p) { unsigned v; std::memcpy(&v, p, 4); return v; }
static float flt(const unsigned char* p) { float v; std::memcpy(&v, p, 4); return v; }
static unsigned half(const unsigned char* p) { unsigned short v; std::memcpy(&v, p, 2); return v; }
static unsigned addr(const unsigned char* p) { return norrath_host_address(p); }

int main() {
    int bad = 0;
    unsigned char* const object = arena + 0x100;
    unsigned char* const creature = arena + 0x400;
    unsigned char* const skill = arena + 0x800;
    unsigned char* const table = arena + 0xc00;
    const unsigned seeds[] = {0x12345678u, 0u, 0xffffffffu};
    const int ranks[] = {0x1a7, 0x80, -1, 7};
    const short adjusts[] = {-24, 0, 64};
    const int results[] = {0, 5, -3, 100000};
    for (unsigned seed : seeds) for (int rank : ranks) for (short adj : adjusts) for (int result : results) {
        std::memset(arena, 0x5a, sizeof arena);
        for (int i = 0; i < 12; ++i) creature[16 + i] = static_cast<unsigned char>(0x30 + i);
        const unsigned t = addr(table);
        std::memcpy(skill + 96, &t, 4);
        std::memcpy(table + 88, &adj, 2);
        std::memcpy(table + 92, &fnAddress, 4);
        calls.clear(); syncRandState = seed; slotResult = result; slotSeenThis = nullptr; lightSeen = nullptr;
        const unsigned next = seed * 0x19660du + 0x3c6ef35fu;
        __22SpellRangerCriticalHitiP8CreatureP5Skill(object, rank, creature, skill);

        if (syncRandState != next || word(object + 16) != (next >> 16)) { std::puts("fail: random"); bad = 1; }
        if (object[0] != static_cast<unsigned char>(rank) || word(object + 4) != 0u) { std::puts("fail: base"); bad = 1; }
        if (word(object + 8) != addr(skill) || word(object + 12) != addr(creature)) { std::puts("fail: skill/creature"); bad = 1; }
        if (word(object + 20) != critAddress) { std::puts("fail: table"); bad = 1; }
        if (std::memcmp(object + 76, creature + 16, 12) != 0) { std::puts("fail: position copy"); bad = 1; }
        if (calls != "light;slot;particle;") { std::printf("fail: calls %s\n", calls.c_str()); bad = 1; }
        if (lightSeen != object + 28) { std::puts("fail: light pointer"); bad = 1; }
        if (flt(lightCopy + 28) != 70.0f || flt(lightCopy + 36) != 70.0f) { std::puts("fail: light 70"); bad = 1; }
        if (flt(lightCopy + 32) != flt(lightCopy + 40) || !(flt(lightCopy + 32) > 0.0f && flt(lightCopy + 32) < 0.001f)) { std::puts("fail: light small"); bad = 1; }
        if (half(lightCopy + 10) != 69 || half(lightCopy + 18) != 69 || half(lightCopy + 14) != 32 || half(lightCopy + 22) != 32) { std::puts("fail: light halves"); bad = 1; }
        if (half(lightCopy + 12) != 0 || half(lightCopy + 16) != 0 || half(lightCopy + 20) != 0 || half(lightCopy + 24) != 0 ||
            lightCopy[8] != 0 || lightCopy[9] != 0 || word(lightCopy + 44) != 0) { std::puts("fail: light zeros"); bad = 1; }
        if (slotSeenThis != skill + adj || slotSeenA != 2 || slotSeenB != static_cast<signed char>(rank)) { std::puts("fail: slot args"); bad = 1; }
        if (flt(object + 24) != static_cast<float>(result)) { std::puts("fail: result float"); bad = 1; }
        if (word(object + 88) != addr(arena + 0x3000)) { std::puts("fail: particle"); bad = 1; }
    }
    if (bad == 0) std::puts("ok");
    return bad;
}
