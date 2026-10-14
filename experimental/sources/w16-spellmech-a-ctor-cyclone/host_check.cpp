// Host contract for __12SpellCycloneiP8CreatureP5Skill; not an EE input. Stored 32-bit addresses are offsets into an
// arena (NORRATH_HOST_ARENA). Checks the stored fields, the 48-byte light block, the animation edits and the float product,
// the early return on the creature flag, the two random halfwords, and the link walk over lists of zero to four nodes.
#define NORRATH_HOST_ARENA 1
#include "ctor.cpp"
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

alignas(16) static unsigned char arena[0x10000];
static const unsigned base = 0x100000u;
unsigned char vt_Spell[8];
unsigned char vt_Cyclone[8];
unsigned syncRandState;
unsigned frameNm;
static std::string calls;
static int slotResult;
static unsigned char* slotSeenThis;
static int slotSeenA, slotSeenB;
static unsigned char* lightSeen; static unsigned char lightCopy[48];
static std::string lumpName[2], lumpArg[2]; static int lumpCount;
static unsigned char* animState; static unsigned char* animHeader; static int animFlags;
static int playHandle, playId;
static void* soundHeader; static void* soundObject; static float soundVolume;
static Point3 radiusPoint; static short radiusAngle; static float radiusValue; static ListHead* radiusList, *destroyedList;
static unsigned char* chain[4]; static int chainCount;
static std::vector<std::pair<void*, void*> > links; static std::vector<void*> linkLists;
static unsigned rolls[2]; static int rollCount;
static void* eRandSeen[2];

static int slotTarget(void* self, int a, int b) {
    calls += "slot;"; slotSeenThis = static_cast<unsigned char*>(self); slotSeenA = a; slotSeenB = b; return slotResult;
}
extern "C" void lightAdd__FP5Light(void* light) { calls += "light;"; lightSeen = static_cast<unsigned char*>(light); std::memcpy(lightCopy, light, 48); }
extern "C" void* lumpFindResource__FPcT0(char* lump, char* name) {
    calls += "lump;"; lumpName[lumpCount] = lump; lumpArg[lumpCount] = name; return lumpCount++ == 0 ? arena + 0x5000 : arena + 0x5400;
}
extern "C" void animAdd__FP14AnimationStateP15AnimationHeaderi(void* state, void* header, int flags) {
    calls += "anim;"; animState = static_cast<unsigned char*>(state); animHeader = static_cast<unsigned char*>(header); animFlags = flags;
}
extern "C" unsigned eRand__5Spell(void* spell) { calls += "rand;"; eRandSeen[rollCount & 1] = spell; return rolls[rollCount++ & 1]; }
extern "C" void playerPlaySound__Fii(int handle, int id) { calls += "play;"; playHandle = handle; playId = id; }
extern "C" void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* sound, void* object, float volume) {
    calls += "sound;"; soundHeader = sound; soundObject = object; soundVolume = volume;
}
extern "C" void objectGetListInRadius__FG6Point3sfP8ListHead(Point3 point, short angle, float radius, ListHead* list) {
    calls += "radius;"; radiusPoint = point; radiusAngle = angle; radiusValue = radius; radiusList = list;
    list->first = chainCount ? chain[0] : nullptr;
}
extern "C" void listOnewayLink__FP8ListHeadT0Pv(ListHead* list, void* a, void* b) { calls += "link;"; links.push_back(std::make_pair(a, b)); linkLists.push_back(list); }
extern "C" void listDestroy__FP8ListHead(ListHead* list) { calls += "destroy;"; destroyedList = list; }
static const unsigned fnAddress = 0x7f0000u, spellAddress = 0x7f0010u, cycloneAddress = 0x7f0020u;
extern "C" unsigned char* norrath_host_pointer(unsigned a) {
    if (a == fnAddress) return reinterpret_cast<unsigned char*>(slotTarget);
    return arena + (a - base);
}
extern "C" unsigned norrath_host_address(const void* p) {
    const unsigned char* q = static_cast<const unsigned char*>(p);
    if (q >= arena && q < arena + sizeof arena) return base + static_cast<unsigned>(q - arena);
    if (q == vt_Spell) return spellAddress;
    if (q == vt_Cyclone) return cycloneAddress;
    return 0xdeadbeefu;
}
static unsigned word(const unsigned char* p) { unsigned v; std::memcpy(&v, p, 4); return v; }
static unsigned half(const unsigned char* p) { unsigned short v; std::memcpy(&v, p, 2); return v; }
static float flt(const unsigned char* p) { float v; std::memcpy(&v, p, 4); return v; }
static unsigned addr(const unsigned char* p) { return norrath_host_address(p); }

int main() {
    int bad = 0;
    unsigned char* const object = arena + 0x100;      // 128 bytes
    unsigned char* const creature = arena + 0x400;    // 1408 bytes
    unsigned char* const skill = arena + 0xa00;
    unsigned char* const table = arena + 0xc00;
    unsigned char* const owner = arena + 0xe00;
    unsigned char* const names = arena + 0xf00;
    unsigned char* const nodes = arena + 0x1000;
    unsigned char* const targets = arena + 0x2000;
    const unsigned seeds[] = {0x12345678u, 0xffffffffu};
    const short adjusts[] = {-24, 64};
    const int results[] = {0, 5, -3};
    const int anmInts[] = {0, 100, -100, 12345};
    const unsigned flagsList[] = {0u, 0x20000u, 0xffffffffu, 0xfffdffffu, 0x1u};
    for (unsigned seed : seeds) for (short adj : adjusts) for (int result : results) for (int anmInt : anmInts)
    for (unsigned ownerFlags : flagsList) for (int n = 0; n <= 4; ++n) {
        std::memset(arena, 0x5a, sizeof arena);
        for (int i = 0; i < 12; ++i) creature[16 + i] = static_cast<unsigned char>(0x30 + i);
        const unsigned t = addr(table), ow = addr(owner), nm = addr(names);
        std::memcpy(skill + 96, &t, 4);
        std::memcpy(table + 88, &adj, 2);
        std::memcpy(table + 92, &fnAddress, 4);
        std::memcpy(creature + 36, &ow, 4);
        std::memcpy(owner, &nm, 4);
        std::memcpy(names, "fx.lmp", 7);
        const unsigned creatureAddr = addr(creature);
        std::memcpy(object + 12, &creatureAddr, 4);
        std::memcpy(creature + 12, &ownerFlags, 4);
        const unsigned handle = 0xabcd1234u; std::memcpy(creature + 960, &handle, 4);
        const short angle = -0x321; std::memcpy(creature + 56, &angle, 2);
        std::memcpy(arena + 0x5000 + 4, &anmInt, 4);
        chainCount = n;
        for (int i = 0; i < n; ++i) {
            chain[i] = nodes + 32 * i;
            const unsigned target = addr(targets + 32 * i);
            std::memcpy(chain[i] + 12, &target, 4);
            const unsigned next = i + 1 < n ? addr(nodes + 32 * (i + 1)) : 0u;
            std::memcpy(chain[i], &next, 4);
        }
        calls.clear(); links.clear(); linkLists.clear(); lumpCount = 0; rollCount = 0;
        rolls[0] = 0x12345u; rolls[1] = 0xabcdefu; soundObject = nullptr;
        syncRandState = seed; frameNm = 0x1357u + seed % 7; slotResult = result; slotSeenThis = nullptr;
        const unsigned next = seed * 0x19660du + 0x3c6ef35fu;
        __12SpellCycloneiP8CreatureP5Skill(object, 0x1a7, creature, skill);

        const bool early = (ownerFlags & 0x20000u) != 0;
        if (syncRandState != next || word(object + 16) != (next >> 16)) { std::puts("fail: random"); bad = 1; }
        if (object[0] != 0xa7 || word(object + 4) != 0u || word(object + 24) != 0u) { std::puts("fail: base"); bad = 1; }
        if (word(object + 8) != addr(skill) || word(object + 12) != creatureAddr || word(object + 20) != cycloneAddress) { std::puts("fail: fields"); bad = 1; }
        if (std::memcmp(object + 76, creature + 16, 12) != 0 || std::memcmp(object + 108, creature + 16, 12) != 0) { std::puts("fail: position copies"); bad = 1; }
        if (lightSeen != object + 28 || flt(lightCopy + 28) != 36.0f || flt(lightCopy + 36) != 36.0f ||
            word(lightCopy + 32) != 0x3a4a4587u || word(lightCopy + 40) != 0x3a4a4587u || word(lightCopy + 44) != 0u) { std::puts("fail: light floats"); bad = 1; }
        if (half(lightCopy + 10) != 158 || half(lightCopy + 18) != 158 || half(lightCopy + 12) != 111 || half(lightCopy + 20) != 111 ||
            half(lightCopy + 14) != 67 || half(lightCopy + 22) != 67 || half(lightCopy + 16) != 0 || half(lightCopy + 24) != 0 ||
            lightCopy[8] != 0 || lightCopy[9] != 0) { std::puts("fail: light halves"); bad = 1; }
        if (word(creature + 1400) != 6u || word(object + 88) != frameNm || word(object + 120) != addr(arena + 0x5000)) { std::puts("fail: creature/frame/anm words"); bad = 1; }
        if (slotSeenThis != skill + adj || slotSeenA != 1 || slotSeenB != static_cast<signed char>(0xa7)) { std::puts("fail: slot args"); bad = 1; }
        float k; unsigned kb = 0x3c888888u; std::memcpy(&k, &kb, 4);
        const float want = static_cast<float>(anmInt) * k * static_cast<float>(result);
        if (std::memcmp(&want, object + 104, 4) != 0) { std::puts("fail: product"); bad = 1; }
        if (arena[0x5000 + 24] != 1 || word(arena + 0x5000 + 20) != 0x3d4cccccu || animState != creature + 288 || animHeader != arena + 0x5000 || animFlags != 35) { std::puts("fail: anim"); bad = 1; }
        if (word(object + 92) != 0u || word(object + 96) != 0u || half(object + 100) != 0x2345u || half(object + 102) != 0xcdefu ||
            eRandSeen[0] != object || eRandSeen[1] != object) { std::puts("fail: rands"); bad = 1; }
        if (lumpName[0] != "fx.lmp" || lumpArg[0] != "cyclone1.anm") { std::puts("fail: first lump"); bad = 1; }
        std::string wantCalls = "light;lump;slot;anim;rand;rand;";
        if (!early) {
            wantCalls += "play;lump;sound;radius;";
            for (int i = 0; i < n; ++i) wantCalls += "link;";
            wantCalls += "destroy;";
            if (playHandle != static_cast<int>(handle) || playId != 10) { std::puts("fail: play args"); bad = 1; }
            if (lumpCount != 2 || lumpName[1] != "fx.lmp" || lumpArg[1] != "spel_bw_cyclone.anm" || soundHeader != arena + 0x5400 ||
                soundObject != creature || soundVolume != 1.0f) { std::puts("fail: sound"); bad = 1; }
            if (std::memcmp(&radiusPoint, creature + 16, 12) != 0 || radiusAngle != angle || radiusValue != 480.0f || radiusList != destroyedList) { std::puts("fail: radius"); bad = 1; }
            if (static_cast<int>(links.size()) != n) { std::puts("fail: link count"); bad = 1; }
            else for (int i = 0; i < n; ++i)
                if (links[i].first != targets + 32 * i || links[i].second != links[i].first || linkLists[i] != object + 24) { std::puts("fail: link args"); bad = 1; }
        } else if (lumpCount != 1) { std::puts("fail: early lump count"); bad = 1; }
        if (calls != wantCalls) { std::printf("fail: calls %s\n", calls.c_str()); bad = 1; }
    }
    if (bad == 0) std::puts("ok");
    return bad;
}
