// Host contract for __15SpellShieldBashiP8CreatureP5Skill; not an EE input. Stored 32-bit addresses are offsets into an
// arena (NORRATH_HOST_ARENA). Checks the stored fields, the animation setup, the sorted-list query arguments, the
// target selection over lists of zero to four nodes, and the call order.
#define NORRATH_HOST_ARENA 1
#include "ctor.cpp"
#include <cstdio>
#include <cstring>
#include <string>

alignas(16) static unsigned char arena[0x8000];
static const unsigned base = 0x100000u;
unsigned char vt_Spell[8];
unsigned char vt_SpellShieldBash[8];
unsigned syncRandState;
static const unsigned spellAddress = 0x7f0010u, bashAddress = 0x7f0020u;

static std::string calls;
static std::string lumpName, lumpArg;
static unsigned char* animState; static void* animHeader; static float animA, animB; static int animId;
static Point3 sortedPoint; static short sortedAngle; static float sortedA, sortedB;
static ListHead* sortedList; static void* sortedExclude;
static unsigned char* chain[4]; static int chainCount;
static unsigned destroyedFirst; static ListHead* destroyedList;
static unsigned randAtLump;

extern "C" unsigned char* norrath_host_pointer(unsigned a) { return arena + (a - base); }
extern "C" unsigned norrath_host_address(const void* p) {
    const unsigned char* q = static_cast<const unsigned char*>(p);
    if (q >= arena && q < arena + sizeof arena) return base + static_cast<unsigned>(q - arena);
    if (q == vt_Spell) return spellAddress;
    if (q == vt_SpellShieldBash) return bashAddress;
    return 0xdeadbeefu;
}
static unsigned word(const unsigned char* p) { unsigned v; std::memcpy(&v, p, 4); return v; }
static float flt(const unsigned char* p) { float v; std::memcpy(&v, p, 4); return v; }
static unsigned addr(const unsigned char* p) { return norrath_host_address(p); }

extern "C" void* lumpFindResource__FPcT0(char* lump, char* name) {
    calls += "lump;"; lumpName = lump; lumpArg = name; randAtLump = syncRandState;
    return arena + 0x7000;
}
extern "C" void animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(void* state, void* header, float a, float b, int id) {
    calls += "anim;"; animState = static_cast<unsigned char*>(state); animHeader = header; animA = a; animB = b; animId = id;
}
extern "C" void objectGetSortedCreatureList__FG6Point3sffP8ListHeadP8Creature(Point3 point, short angle, float a, float b, ListHead* list, void* exclude) {
    calls += "sorted;"; sortedPoint = point; sortedAngle = angle; sortedA = a; sortedB = b; sortedList = list; sortedExclude = exclude;
    list->first = chainCount ? chain[0] : nullptr;
}
extern "C" void listDestroy__FP8ListHead(ListHead* list) {
    calls += "destroy;"; destroyedList = list; destroyedFirst = list->first ? 1u : 0u;
}

int main() {
    int bad = 0;
    unsigned char* const object = arena + 0x100;
    unsigned char* const creature = arena + 0x400;  // 1408 bytes
    unsigned char* const lumpOwner = arena + 0xa00;
    unsigned char* const nodes = arena + 0x1000;
    unsigned char* const targets = arena + 0x2000;
    unsigned char* const anm = arena + 0x7000;
    unsigned char* const names = arena + 0x7800;
    const unsigned seeds[] = {0x12345678u, 0u, 0xffffffffu};
    const int ranks[] = {0x1a7, 0x80, -1, 7};
    const unsigned flags[] = {0u, 0x01000000u, 0x01000005u, 0x2000u};
    for (unsigned seed : seeds) for (int rank : ranks) for (int n = 0; n <= 4; ++n) for (unsigned pattern = 0; pattern < (1u << (n ? n : 1)) * (n ? 1 : 1); ++pattern) {
        std::memset(arena, 0x5a, sizeof arena);
        for (int i = 0; i < 12; ++i) creature[16 + i] = static_cast<unsigned char>(0x20 + i);
        const unsigned owner = addr(lumpOwner), names_a = addr(names);
        std::memcpy(creature + 36, &owner, 4);
        std::memcpy(lumpOwner, &names_a, 4);
        std::memcpy(names, "fx.lmp", 7);
        const short angle = 0x1234; std::memcpy(creature + 56, &angle, 2);
        chainCount = n;
        bool expectStore = false;
        for (int i = 0; i < n; ++i) {
            chain[i] = nodes + 32 * i;
            const unsigned target = addr(targets + 32 * i);
            std::memcpy(chain[i] + 12, &target, 4);
            const unsigned next = i + 1 < n ? addr(nodes + 32 * (i + 1)) : 0u;
            std::memcpy(chain[i], &next, 4);
            const unsigned flag = flags[((pattern >> i) & 1) ? 1 : (i % 2 ? 3 : 0)];
            std::memcpy(targets + 32 * i + 12, &flag, 4);
            if ((flag & 0x01000000u) == 0) expectStore = true;
        }
        calls.clear(); syncRandState = seed; animState = nullptr; sortedList = nullptr; destroyedList = nullptr;
        const unsigned next = seed * 0x19660du + 0x3c6ef35fu;
        __15SpellShieldBashiP8CreatureP5Skill(object, rank, creature, arena + 0x600);

        if (syncRandState != next || word(object + 16) != (next >> 16) || randAtLump != next) { std::puts("fail: random"); bad = 1; }
        if (object[0] != static_cast<unsigned char>(rank) || word(object + 4) != 0u) { std::puts("fail: base"); bad = 1; }
        if (word(object + 8) != addr(arena + 0x600) || word(object + 12) != addr(creature)) { std::puts("fail: skill/creature"); bad = 1; }
        if (word(object + 20) != bashAddress) { std::puts("fail: table"); bad = 1; }
        if (word(creature + 1400) != 6u) { std::puts("fail: creature state"); bad = 1; }
        if (calls != "lump;anim;sorted;destroy;" || lumpName != "fx.lmp" || lumpArg != "bash.anm") { std::printf("fail: calls %s\n", calls.c_str()); bad = 1; }
        if (animState != creature + 288 || animHeader != anm || animA != animB || animA != flt(reinterpret_cast<const unsigned char*>("\xcc\xcc\xcc\x3d")) || animId != 2066) { std::puts("fail: anim"); bad = 1; }
        if (anm[32] != 2 || flt(anm + 28) != flt(reinterpret_cast<const unsigned char*>("\x40\x10\x18\x3f"))) { std::puts("fail: anm edit"); bad = 1; }
        if (std::memcmp(&sortedPoint, creature + 16, 12) != 0 || sortedAngle != angle || sortedA != 100.0f || sortedB != 90.0f ||
            sortedList != destroyedList || sortedExclude != nullptr) { std::puts("fail: query"); bad = 1; }
        if (word(object + 24) != 0u || word(object + 28) != 0u || word(object + 32) != 0u) { std::puts("fail: zero words"); bad = 1; }
        const unsigned want = expectStore ? addr(targets) : 0u;
        if (word(object + 36) != want) { std::printf("fail: target n=%d pattern=%u\n", n, pattern); bad = 1; }
    }
    if (bad == 0) std::puts("ok");
    return bad;
}
