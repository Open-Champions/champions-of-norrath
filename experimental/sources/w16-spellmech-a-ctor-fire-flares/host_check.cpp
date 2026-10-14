// Host contract for __15SpellFireFlaresiP8CreatureG6Point3P5Skill; not an EE input. Stored 32-bit addresses are offsets into
// an arena (NORRATH_HOST_ARENA). Checks the spell object's fields and light block, the arithmetic on the rank, both skill-table
// calls, the flare count limit and angle step (angles are logged), the per-flare object fields and call order, the failed
// allocation path, and the final sound handle. Ranks that make the integer quotient (2r+65)/12 zero are not used.
#define NORRATH_HOST_ARENA 1
#include "ctor.cpp"
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

alignas(16) static unsigned char arena[0x8000];
static const unsigned base = 0x100000u;
unsigned char vt_Spell[8];
unsigned char vt_FireFlares[8];
unsigned char vt_FireFlare[8];
unsigned syncRandState;
unsigned frameNm;
static const unsigned fnAddress = 0x7f0000u, spellAddress = 0x7f0010u, flaresAddress = 0x7f0020u, flareAddress = 0x7f0030u;
static std::string calls;
static unsigned char* lightSeen; static unsigned char lightCopy[48]; static unsigned posWordAtLight[3];
static unsigned char* lightUpdSeen; static float posZAtUpdate; static unsigned frameWordAtUpdate;
static int slotResults[2]; static int slotCalls; static unsigned char* slotSeenThis; static int slotSeenA[2], slotSeenB[2];
static std::vector<int> iangles, sangles; static float cosValue, sinValue;
static int allocSizes[32]; static int allocCalls; static int failEvery;
struct Flare { unsigned char* block; Point3 at; void* map; char** args; };
static std::vector<Flare> flares;
static std::vector<unsigned char*> gridBlocks, runBlocks;
struct Link { unsigned char* list; void* a; void* b; };
static std::vector<Link> links;
static std::string lumpName, lumpArg; static void* soundHeader; static Point3 soundPoint; static float soundVolume; static int soundResult, persistSeen;

static int slotTarget(void* self, int a, int b) {
    calls += "slot;"; slotSeenThis = static_cast<unsigned char*>(self);
    slotSeenA[slotCalls] = a; slotSeenB[slotCalls] = b; return slotResults[slotCalls++];
}
extern "C" void lightAdd__FP5Light(void* light) {
    calls += "light;"; lightSeen = static_cast<unsigned char*>(light); std::memcpy(lightCopy, light, 48);
    std::memcpy(posWordAtLight, lightSeen - 44 + 92, 12);
}
extern "C" void lightUpdatePosition__FP5Light(void* light) {
    calls += "lightupd;"; lightUpdSeen = static_cast<unsigned char*>(light);
    std::memcpy(&posZAtUpdate, lightUpdSeen - 44 + 100, 4); std::memcpy(&frameWordAtUpdate, lightUpdSeen - 44 + 40, 4);
}
extern "C" float icos__Fi(int angle) { calls += "cos;"; iangles.push_back(angle); return cosValue; }
extern "C" float isin__Fi(int angle) { calls += "sin;"; sangles.push_back(angle); return sinValue; }
extern "C" void* blockAlloc__Fi(int size) {
    calls += "alloc;"; allocSizes[allocCalls] = size; ++allocCalls;
    if (failEvery && allocCalls % failEvery == 0) return nullptr;
    return arena + 0x2000 + 0x100 * allocCalls;
}
extern "C" void* __10GameObjectG6Point3PQ210GameObject11resourceMapPPc(void* object, Point3 point, void* map, char** args) {
    calls += "base;"; Flare f = {static_cast<unsigned char*>(object), point, map, args}; flares.push_back(f);
    const unsigned flags = 0x00000100u; std::memcpy(static_cast<unsigned char*>(object) + 12, &flags, 4);
    return object;
}
extern "C" void objectUpdateInGrid__FP10GameObject(void* object) { calls += "grid;"; gridBlocks.push_back(static_cast<unsigned char*>(object)); }
extern "C" void objectAddToRunList__FP10GameObject(void* object) { calls += "run;"; runBlocks.push_back(static_cast<unsigned char*>(object)); }
extern "C" void listOnewayLink__FP8ListHeadT0Pv(ListHead* list, void* a, void* b) {
    calls += "link;"; Link l = {reinterpret_cast<unsigned char*>(list), a, b}; links.push_back(l);
}
extern "C" void* lumpFindResource__FPcT0(char* lump, char* name) { calls += "lump;"; lumpName = lump; lumpArg = name; return arena + 0x1c00; }
extern "C" int soundPlaySequence__FP15AnimationHeaderG6Point3f(void* header, Point3 point, float volume) {
    calls += "sound;"; soundHeader = header; soundPoint = point; soundVolume = volume; return soundResult;
}
extern "C" void soundMakePersistant__Fi(int sound) { calls += "persist;"; persistSeen = sound; }
extern "C" unsigned char* norrath_host_pointer(unsigned a) {
    if (a == fnAddress) return reinterpret_cast<unsigned char*>(slotTarget);
    return arena + (a - base);
}
extern "C" unsigned norrath_host_address(const void* p) {
    const unsigned char* q = static_cast<const unsigned char*>(p);
    if (q >= arena && q < arena + sizeof arena) return base + static_cast<unsigned>(q - arena);
    if (q == vt_Spell) return spellAddress;
    if (q == vt_FireFlares) return flaresAddress;
    if (q == vt_FireFlare) return flareAddress;
    return 0xdeadbeefu;
}
static unsigned word(const unsigned char* p) { unsigned v; std::memcpy(&v, p, 4); return v; }
static unsigned half(const unsigned char* p) { unsigned short v; std::memcpy(&v, p, 2); return v; }
static float flt(const unsigned char* p) { float v; std::memcpy(&v, p, 4); return v; }
static unsigned addr(const unsigned char* p) { return norrath_host_address(p); }

int main() {
    int bad = 0;
    unsigned char* const object = arena + 0x100;      // 144 bytes
    unsigned char* const creature = arena + 0x400;
    unsigned char* const skill = arena + 0x600;
    unsigned char* const table = arena + 0x700;
    volatile float zero = 0.0f; const float largest = 1.0f / zero;
    unsigned largestBits; std::memcpy(&largestBits, &largest, 4);
    const Point3 point = {12.5f, -4.0f, 300.0f};
    const unsigned seeds[] = {0x12345678u, 0xffffffffu};
    const int ranks[] = {0, 3, 7, 0x1a7, -1};
    const short adjusts[] = {-24, 64};
    const int firstResults[] = {0, 5, -3};
    const int counts[] = {-1, 0, 1, 2, 3, 5, 16, 17, 40};
    const int failures[] = {0, 2, 3};
    for (unsigned seed : seeds) for (int rank : ranks) for (short adj : adjusts) for (int r1 : firstResults)
    for (int n : counts) for (int failure : failures) {
        std::memset(arena, 0x5a, sizeof arena);
        for (int i = 0; i < 12; ++i) creature[16 + i] = static_cast<unsigned char>(0x30 + i);
        const unsigned t = addr(table);
        std::memcpy(skill + 96, &t, 4); std::memcpy(table + 88, &adj, 2); std::memcpy(table + 92, &fnAddress, 4);
        calls.clear(); iangles.clear(); sangles.clear(); flares.clear(); gridBlocks.clear(); runBlocks.clear(); links.clear();
        slotCalls = 0; slotResults[0] = r1; slotResults[1] = n; slotSeenThis = nullptr; allocCalls = 0; failEvery = failure;
        cosValue = 0.5f; sinValue = -0.25f; soundResult = 0x321; persistSeen = 0;
        syncRandState = seed; frameNm = 0x999u + seed % 7;
        const unsigned next = seed * 0x19660du + 0x3c6ef35fu;
        __15SpellFireFlaresiP8CreatureG6Point3P5Skill(object, rank, creature, point, skill);

        if (syncRandState != next || word(object + 16) != (next >> 16)) { std::puts("fail: random"); bad = 1; }
        if (object[0] != static_cast<unsigned char>(rank) || word(object + 4) != 0u || word(object + 24) != 0u) { std::puts("fail: base"); bad = 1; }
        if (word(object + 8) != addr(skill) || word(object + 12) != addr(creature) || word(object + 20) != flaresAddress) { std::puts("fail: fields"); bad = 1; }
        if (word(object + 28) != addr(creature) || word(object + 32) != static_cast<unsigned>(rank) || word(object + 36) != addr(skill) || word(object + 112) != 0u) { std::puts("fail: words 28 to 36"); bad = 1; }
        const int scaled = rank * 2 + 65;
        if (word(object + 104) != static_cast<unsigned>(scaled) || word(object + 108) != static_cast<unsigned>(4500 / (scaled / 12))) { std::puts("fail: rank arithmetic"); bad = 1; }
        if (std::memcmp(posWordAtLight, &point, 12) != 0) { std::puts("fail: point at light time"); bad = 1; }
        if (lightSeen != object + 44 || word(lightCopy + 32) != largestBits || word(lightCopy + 40) != largestBits ||
            word(lightCopy + 28) != 0u || word(lightCopy + 36) != 0u || word(lightCopy + 44) != 0x3f09fbd6u) { std::puts("fail: light floats"); bad = 1; }
        if (half(lightCopy + 10) != 255 || half(lightCopy + 18) != 255 || half(lightCopy + 12) != 128 || half(lightCopy + 20) != 128 ||
            half(lightCopy + 16) != 0xfff0 || half(lightCopy + 24) != 0xfff0 || half(lightCopy + 14) != 0 || half(lightCopy + 22) != 0 ||
            lightCopy[8] != 0 || lightCopy[9] != 0) { std::puts("fail: light halves"); bad = 1; }
        float constant; { unsigned kb = 0x37b88ca3u; std::memcpy(&constant, &kb, 4); }
        if (slotSeenThis != skill + adj || slotSeenA[0] != 2 || slotSeenB[0] != rank || slotSeenA[1] != 3 || slotSeenB[1] != rank) { std::puts("fail: slot args"); bad = 1; }
        if (word(object + 40) != frameNm + 60u * static_cast<unsigned>(r1) || frameWordAtUpdate != word(object + 40)) { std::puts("fail: frame word"); bad = 1; }
        if (flt(object + 100) != point.z + constant || posZAtUpdate != flt(object + 100) || lightUpdSeen != object + 44) { std::puts("fail: light update"); bad = 1; }
        const int count = n > 16 ? 16 : n;
        const int step = count > 0 ? static_cast<short>(0xfffe / count) : 0;
        const int made = count > 0 ? count : 0;
        if (static_cast<int>(iangles.size()) != made || static_cast<int>(sangles.size()) != made) { std::puts("fail: angle count"); bad = 1; continue; }
        for (int i = 0; i < made; ++i) if (iangles[i] != step * i || sangles[i] != step * i) { std::puts("fail: angle value"); bad = 1; }
        size_t liveIndex = 0;
        for (int i = 0; i < made; ++i) {
            const bool failed = failure && (i + 1) % failure == 0;
            if (allocSizes[i] != 108) { std::puts("fail: flare alloc size"); bad = 1; }
            if (failed) continue;
            if (liveIndex >= flares.size()) { std::puts("fail: missing flare"); bad = 1; break; }
            const Flare& f = flares[liveIndex];
            unsigned char* b = f.block;
            if (b != arena + 0x2000 + 0x100 * (i + 1) || f.map != nullptr || f.args != nullptr) { std::puts("fail: flare ctor args"); bad = 1; }
            const float x = 0.5f * 0.0f + point.x, y = -0.25f * 0.0f + point.y, z = 0.0f + point.z;
            if (std::memcmp(&f.at.x, &x, 4) != 0 || std::memcmp(&f.at.y, &y, 4) != 0 || std::memcmp(&f.at.z, &z, 4) != 0) { std::puts("fail: flare position"); bad = 1; }
            if (word(b + 12) != (0x100u | 0x421du) || word(b + 72) != flareAddress || word(b + 76) != addr(creature) ||
                word(b + 80) != static_cast<unsigned>(rank) || word(b + 84) != addr(skill) || word(b + 88) != 0u ||
                half(b + 92) != static_cast<unsigned>((step * i) & 0xffff) || half(b + 94) != 0 || word(b + 28) != 0x41c00000u ||
                std::memcmp(b + 16, &f.at, 12) != 0) { std::printf("fail: flare fields (flare %d)\n", i); bad = 1; }
            if (liveIndex >= links.size() || links[liveIndex].list != object + 24 || links[liveIndex].a != b || links[liveIndex].b != b) { std::puts("fail: flare link"); bad = 1; }
            ++liveIndex;
        }
        if (flares.size() != liveIndex || gridBlocks.size() != liveIndex || runBlocks.size() != liveIndex || links.size() != liveIndex) { std::puts("fail: flare counts"); bad = 1; }
        if (lumpName != "fx.lmp" || lumpArg != "spel_ew_flares.anm" || soundHeader != arena + 0x1c00 ||
            std::memcmp(&soundPoint, creature + 16, 12) != 0 || soundVolume != 1.0f) { std::puts("fail: sound"); bad = 1; }
        if (word(object + 116) != 0x321u || persistSeen != 0x321) { std::puts("fail: sound handle"); bad = 1; }
        if (calls.compare(0, 20, "light;slot;lightupd;") != 0) { std::printf("fail: call prefix %s\n", calls.c_str()); bad = 1; }
    }
    if (bad == 0) std::puts("ok");
    return bad;
}
