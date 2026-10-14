// Host contract for __17SpellDiseaseTrailG6Point3iP8CreatureP5Skill; not an EE input. Stored 32-bit addresses are offsets
// into an arena (NORRATH_HOST_ARENA). Checks the spell object's fields, the float rounding of the frame value, the spawned
// trail object (flags, table, owner link, copied point, constants, skill-driven frame word), the call order, the failed
// allocation path and the sound call. The null-creature branch is not exercised here: the source reads the creature's
// position afterwards, which a host build cannot do at address zero.
#define NORRATH_HOST_ARENA 1
#include "ctor.cpp"
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

alignas(16) static unsigned char arena[0x2000];
static const unsigned base = 0x100000u;
unsigned char vt_Spell[8];
unsigned char vt_DiseaseTrailSpell[8];
unsigned char vt_DiseaseTrail[8];
unsigned syncRandState;
int frameNm;
static const unsigned fnAddress = 0x7f0000u, spellAddress = 0x7f0010u, trailSpellAddress = 0x7f0020u, trailAddress = 0x7f0030u;
static std::string calls; static int allocSize; static bool failAlloc;
static unsigned char* baseSelf; static Point3 basePoint; static void* baseMap; static char** baseArgs;
static unsigned baseFlagsBefore;
struct Link { unsigned char* list; void* a; void* b; };
static std::vector<Link> links;
static unsigned char* gridSelf; static unsigned gridFlags, gridTable, gridList, gridSkill, gridRank; static unsigned char gridCopy[96];
static int slotResult; static unsigned char* slotSeenThis; static int slotSeenA, slotSeenB;
static unsigned runWord88; static unsigned char* runSelf;
static std::string lumpName, lumpArg; static void* soundHeader; static Point3 soundPoint; static float soundVolume;

static int slotTarget(void* self, int a, int b) {
    calls += "slot;"; slotSeenThis = static_cast<unsigned char*>(self); slotSeenA = a; slotSeenB = b; return slotResult;
}
extern "C" void* blockAlloc__Fi(int size) { calls += "alloc;"; allocSize = size; return failAlloc ? nullptr : arena + 0x800; }
extern "C" void* __10GameObjectG6Point3PQ210GameObject11resourceMapPPc(void* object, Point3 point, void* map, char** args) {
    calls += "base;"; baseSelf = static_cast<unsigned char*>(object); basePoint = point; baseMap = map; baseArgs = args;
    const unsigned flags = 0x00000100u; std::memcpy(baseSelf + 12, &flags, 4); baseFlagsBefore = flags;
    return object;
}
extern "C" void listOnewayLink__FP8ListHeadT0Pv(ListHead* list, void* a, void* b) {
    calls += "link;"; Link l = {reinterpret_cast<unsigned char*>(list), a, b}; links.push_back(l);
}
extern "C" void objectUpdateInGrid__FP10GameObject(void* object) {
    calls += "grid;"; gridSelf = static_cast<unsigned char*>(object); std::memcpy(gridCopy, gridSelf, 96);
    std::memcpy(&gridFlags, gridSelf + 12, 4); std::memcpy(&gridTable, gridSelf + 72, 4); std::memcpy(&gridList, gridSelf + 92, 4);
    std::memcpy(&gridRank, gridSelf + 96, 4); std::memcpy(&gridSkill, gridSelf + 100, 4);
}
extern "C" void objectAddToRunList__FP10GameObject(void* object) {
    calls += "run;"; runSelf = static_cast<unsigned char*>(object); std::memcpy(&runWord88, runSelf + 88, 4);
}
extern "C" void* lumpFindResource__FPcT0(char* lump, char* name) { calls += "lump;"; lumpName = lump; lumpArg = name; return arena + 0x1c00; }
extern "C" int soundPlaySequence__FP15AnimationHeaderG6Point3f(void* header, Point3 point, float volume) {
    calls += "sound;"; soundHeader = header; soundPoint = point; soundVolume = volume; return 0;
}
extern "C" unsigned char* norrath_host_pointer(unsigned a) {
    if (a == fnAddress) return reinterpret_cast<unsigned char*>(slotTarget);
    return arena + (a - base);
}
extern "C" unsigned norrath_host_address(const void* p) {
    const unsigned char* q = static_cast<const unsigned char*>(p);
    if (q >= arena && q < arena + sizeof arena) return base + static_cast<unsigned>(q - arena);
    if (q == vt_Spell) return spellAddress;
    if (q == vt_DiseaseTrailSpell) return trailSpellAddress;
    if (q == vt_DiseaseTrail) return trailAddress;
    return 0xdeadbeefu;
}
static unsigned word(const unsigned char* p) { unsigned v; std::memcpy(&v, p, 4); return v; }
static unsigned addr(const unsigned char* p) { return norrath_host_address(p); }

int main() {
    int bad = 0;
    unsigned char* const object = arena + 0x100;
    unsigned char* const creature = arena + 0x400;
    unsigned char* const skill = arena + 0x600;
    unsigned char* const table = arena + 0x700;
    unsigned char* const trail = arena + 0x800;
    const Point3 points[] = {{1.5f, -2.5f, 3.25f}, {1.0e5f, 0.0f, -7.0f}};
    const unsigned seeds[] = {0x12345678u, 0xffffffffu};
    const int ranks[] = {0x1a7, 0x80, -1};
    const short adjusts[] = {-24, 64};
    const int results[] = {0, 5, -3};
    const int frames[] = {0, 100, 12345678, -50, 16777217};
    for (const Point3& point : points) for (unsigned seed : seeds) for (int rank : ranks) for (short adj : adjusts)
    for (int result : results) for (int frame : frames) for (int fail = 0; fail < 2; ++fail) {
        std::memset(arena, 0x5a, sizeof arena);
        for (int i = 0; i < 12; ++i) creature[16 + i] = static_cast<unsigned char>(0x30 + i);
        const unsigned t = addr(table);
        std::memcpy(skill + 96, &t, 4); std::memcpy(table + 88, &adj, 2); std::memcpy(table + 92, &fnAddress, 4);
        calls.clear(); links.clear(); failAlloc = fail != 0; allocSize = 0; slotSeenThis = nullptr; gridSelf = nullptr; runSelf = nullptr;
        syncRandState = seed; frameNm = frame; slotResult = result;
        const unsigned next = seed * 0x19660du + 0x3c6ef35fu;
        __17SpellDiseaseTrailG6Point3iP8CreatureP5Skill(object, point, rank, creature, skill);

        if (syncRandState != next || word(object + 16) != (next >> 16)) { std::puts("fail: random"); bad = 1; }
        if (object[0] != static_cast<unsigned char>(rank) || word(object + 4) != 0u || word(object + 24) != 0u) { std::puts("fail: base"); bad = 1; }
        if (word(object + 8) != addr(skill) || word(object + 12) != addr(creature) || word(object + 20) != trailSpellAddress) { std::puts("fail: fields"); bad = 1; }
        if (std::memcmp(object + 28, &point, 12) != 0) { std::puts("fail: point copy"); bad = 1; }
        const float expectedFrame = static_cast<float>(static_cast<int>(static_cast<float>(frame) + 180.0f));
        if (std::memcmp(object + 44, &expectedFrame, 4) != 0) { std::puts("fail: frame float"); bad = 1; }
        if (allocSize != 104) { std::puts("fail: alloc size"); bad = 1; }
        if (fail) {
            if (calls != "alloc;") { std::printf("fail: failure calls %s\n", calls.c_str()); bad = 1; }
            continue;
        }
        if (calls != "alloc;base;link;grid;slot;run;link;lump;sound;") { std::printf("fail: calls %s\n", calls.c_str()); bad = 1; continue; }
        if (baseSelf != trail || std::memcmp(&basePoint, &point, 12) != 0 || baseMap != nullptr || baseArgs != nullptr) { std::puts("fail: base ctor args"); bad = 1; }
        if (links.size() != 2 || links[0].list != trail + 92 || links[0].a != creature || links[0].b != creature ||
            links[1].list != object + 24 || links[1].a != trail || links[1].b != trail) { std::puts("fail: links"); bad = 1; }
        if (gridSelf != trail || gridFlags != (baseFlagsBefore | 0x4421fu) || gridTable != trailAddress || gridList != 0u ||
            gridSkill != addr(skill) || gridRank != static_cast<unsigned>(static_cast<int>(static_cast<signed char>(rank)))) { std::puts("fail: object at grid call"); bad = 1; }
        if (std::memcmp(gridCopy + 16, &point, 12) != 0 || word(gridCopy + 28) != 0x42040000u || word(gridCopy + 32) != 0x42100000u) { std::puts("fail: grid fields"); bad = 1; }
        if (std::memcmp(trail + 76, &point, 12) != 0) { std::puts("fail: point at +76"); bad = 1; }
        if (slotSeenThis != skill + adj || slotSeenA != 1 || slotSeenB != static_cast<int>(static_cast<signed char>(rank))) { std::puts("fail: slot args"); bad = 1; }
        const unsigned want88 = static_cast<unsigned>(frame) + 60u * static_cast<unsigned>(result);
        if (word(trail + 88) != want88 || runWord88 != want88 || runSelf != trail) { std::puts("fail: frame word"); bad = 1; }
        if (lumpName != "fx.lmp" || lumpArg != "spel_de_distrail.anm" || soundHeader != arena + 0x1c00 ||
            std::memcmp(&soundPoint, creature + 16, 12) != 0 || soundVolume != 1.0f) { std::puts("fail: sound"); bad = 1; }
    }
    if (bad == 0) std::puts("ok");
    return bad;
}
