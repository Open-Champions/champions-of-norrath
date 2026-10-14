// Host contract for __17SpellConvertEnemyiP8CreatureP5Skill; not an EE input. Stored 32-bit addresses are offsets
// into an arena (NORRATH_HOST_ARENA). The expected link list and particle stores come from an independent model of the
// target filter (flag bit, boss test, float <= 0; the signed byte at +172 is not part of it) and the count bound, over
// lists of zero to five nodes.
#define NORRATH_HOST_ARENA 1
#include "ctor.cpp"
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

alignas(16) static unsigned char arena[0x10000];
static const unsigned base = 0x100000u;
unsigned char vt_Spell[8];
unsigned char vt_ConvertEnemy[8];
unsigned syncRandState;
unsigned frameNm;
static const unsigned spellAddress = 0x7f0010u, cuAddress = 0x7f0020u, fnAddress = 0x7f0030u;

static std::string calls;
static std::string lumpName, lumpArg;
static unsigned char* soundLump; static void* soundObject; static float soundVolume;
static Point3 sortedPoint; static short sortedAngle; static float sortedA, sortedB;
static ListHead* sortedList; static void* sortedExclude;
static unsigned char* chain[5]; static int chainCount;
static std::vector<std::pair<void*, void*> > links; static std::vector<void*> linkLists;
static std::vector<unsigned char*> bossAsked; static bool bossFlag[5]; static unsigned char* targetsBase;
static int particleCount;
static unsigned char* slotSeenThis; static int slotSeenA, slotSeenB, slotResult;
static ListHead* destroyed;

static int slotTarget(void* self, int a, int b) {
    calls += "slot;"; slotSeenThis = static_cast<unsigned char*>(self); slotSeenA = a; slotSeenB = b; return slotResult;
}
extern "C" unsigned char* norrath_host_pointer(unsigned a) {
    if (a == fnAddress) return reinterpret_cast<unsigned char*>(slotTarget);
    return arena + (a - base);
}
extern "C" unsigned norrath_host_address(const void* p) {
    const unsigned char* q = static_cast<const unsigned char*>(p);
    if (q >= arena && q < arena + sizeof arena) return base + static_cast<unsigned>(q - arena);
    if (q == vt_Spell) return spellAddress;
    if (q == vt_ConvertEnemy) return cuAddress;
    return 0xdeadbeefu;
}
static unsigned word(const unsigned char* p) { unsigned v; std::memcpy(&v, p, 4); return v; }
static unsigned addr(const unsigned char* p) { return norrath_host_address(p); }

extern "C" void* lumpFindResource__FPcT0(char* lump, char* name) {
    calls += "lump;"; lumpName = lump; lumpArg = name; return arena + 0xf000;
}
extern "C" void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* sound, void* object, float volume) {
    calls += "sound;"; soundLump = static_cast<unsigned char*>(sound); soundObject = object; soundVolume = volume;
}
extern "C" void objectGetSortedCreatureList__FG6Point3sffP8ListHeadP8Creature(Point3 point, short angle, float a, float b, ListHead* list, void* exclude) {
    calls += "sorted;"; sortedPoint = point; sortedAngle = angle; sortedA = a; sortedB = b; sortedList = list; sortedExclude = exclude;
    list->first = chainCount ? chain[0] : nullptr;
}
extern "C" int isBoss__FP10GameObject(void* object) {
    calls += "boss;"; unsigned char* t = static_cast<unsigned char*>(object); bossAsked.push_back(t);
    return bossFlag[(t - targetsBase) / 1024] ? 1 : 0;
}
extern "C" void listOnewayLink__FP8ListHeadT0Pv(ListHead* list, void* a, void* b) {
    calls += "link;"; links.push_back(std::make_pair(a, b)); linkLists.push_back(list);
}
extern "C" void* P_CreateParticleSystem__Fv() { calls += "particle;"; return arena + 0xe000 + 16 * particleCount++; }
extern "C" void listDestroy__FP8ListHead(ListHead* list) { calls += "destroy;"; destroyed = list; }

int main() {
    int bad = 0;
    unsigned char* const object = arena + 0x100;
    unsigned char* const creature = arena + 0x400;
    unsigned char* const skill = arena + 0x800;
    unsigned char* const table = arena + 0xc00;
    unsigned char* const nodes = arena + 0x1000;
    unsigned char* const targets = arena + 0x2000;
    targetsBase = targets;
    const unsigned seeds[] = {0x12345678u, 0xffffffffu};
    const int bounds[] = {-2, 0, 1, 2, 3, 9};
    const short adjusts[] = {-24, 40};
    for (unsigned seed : seeds) for (int bound : bounds) for (short adj : adjusts) for (int n = 0; n <= 5; ++n)
    for (unsigned pattern = 0; pattern < 32; ++pattern) {
        std::memset(arena, 0x5a, sizeof arena);
        for (int i = 0; i < 12; ++i) creature[16 + i] = static_cast<unsigned char>(0x20 + i);
        const short angle = 0x1234; std::memcpy(creature + 56, &angle, 2);
        const unsigned t = addr(table);
        std::memcpy(skill + 96, &t, 4);
        std::memcpy(table + 88, &adj, 2);
        std::memcpy(table + 92, &fnAddress, 4);
        chainCount = n;
        std::vector<int> expectLinked;
        int count = 0;
        for (int i = 0; i < n; ++i) {
            chain[i] = nodes + 32 * i;
            unsigned char* tg = targets + 1024 * i;
            const unsigned tgAddr = addr(tg);
            std::memcpy(chain[i] + 12, &tgAddr, 4);
            const unsigned next = i + 1 < n ? addr(nodes + 32 * (i + 1)) : 0u;
            std::memcpy(chain[i], &next, 4);
            // pattern bit i chooses a failing reason (cycle through four reasons) or a passing target
            const bool pass = ((pattern >> i) & 1) != 0;
            unsigned flag = 0; float f = (i + pattern) % 2 ? 0.0f : -1.0f; signed char kind = 16; bossFlag[i] = false;
            if (!pass) { const int why = (pattern / 3 + i) % 4; if (why == 0) flag = 0x01000000u; else if (why == 1) bossFlag[i] = true; else if (why == 2) kind = 15; else f = 0.5f; }
            std::memcpy(tg + 12, &flag, 4); tg[172] = static_cast<unsigned char>(kind); std::memcpy(tg + 876, &f, 4);
        }
        if (n > 0 && bound > 0) {
            for (int i = 0; i < n; ++i) {
                unsigned char* tg = targets + 1024 * i; float f; std::memcpy(&f, tg + 876, 4);
                if ((word(tg + 12) & 0x01000000u) == 0 && !bossFlag[i] && f <= 0.0f) { expectLinked.push_back(i); ++count; }
                if (count >= bound) break;
            }
        }
        // the filter above stops early only on the count, matching the walk's continue condition
        calls.clear(); links.clear(); linkLists.clear(); bossAsked.clear(); particleCount = 0;
        syncRandState = seed; frameNm = 777u + seed % 5; slotResult = bound;
        slotSeenThis = nullptr; sortedList = nullptr; destroyed = nullptr;
        const unsigned next = seed * 0x19660du + 0x3c6ef35fu;
        __17SpellConvertEnemyiP8CreatureP5Skill(object, 0x1a7, creature, skill);

        if (syncRandState != next || word(object + 16) != (next >> 16)) { std::puts("fail: random"); bad = 1; }
        if (object[0] != 0xa7 || word(object + 4) != 0u) { std::puts("fail: base"); bad = 1; }
        if (word(object + 8) != addr(skill) || word(object + 12) != addr(creature) || word(object + 20) != cuAddress) { std::puts("fail: fields"); bad = 1; }
        if (word(object + 28) != frameNm || (object[112] | object[113]) != 0 || word(object + 116) != 0u || word(object + 24) != 0u) { std::puts("fail: reset words"); bad = 1; }
        if (slotSeenThis != skill + adj || slotSeenA != 0 || slotSeenB != static_cast<signed char>(0xa7)) { std::puts("fail: slot"); bad = 1; }
        if (lumpName != "fx.lmp" || lumpArg != "spel_he_convert.anm" || soundLump != arena + 0xf000 || soundObject != creature || soundVolume != 1.0f) { std::puts("fail: sound"); bad = 1; }
        if (std::memcmp(&sortedPoint, creature + 16, 12) != 0 || sortedAngle != angle || sortedA != 333.0f || sortedB != 139.0f || sortedExclude != nullptr || sortedList != destroyed) { std::puts("fail: query"); bad = 1; }
        if (links.size() != expectLinked.size() || static_cast<int>(links.size()) != particleCount) { std::printf("fail: link count n=%d bound=%d pattern=%u\n", n, bound, pattern); bad = 1; }
        else for (size_t i = 0; i < links.size(); ++i) {
            if (links[i].first != targets + 1024 * expectLinked[i] || links[i].second != links[i].first || linkLists[i] != object + 24) { std::puts("fail: link args"); bad = 1; }
            if (word(object + 32 + 4 * i) != addr(arena + 0xe000 + 16 * i)) { std::puts("fail: particle slot"); bad = 1; }
        }
        if (calls.compare(0, 23, "slot;lump;sound;sorted;") != 0 || calls.substr(calls.size() - 8) != "destroy;") { std::printf("fail: calls %s\n", calls.c_str()); bad = 1; }
    }
    if (bad == 0) std::puts("ok");
    return bad;
}
