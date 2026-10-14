// Host contract for __12SpellBlessediP8CreatureP5Skill; not an EE input. Stored 32-bit addresses are offsets into an
// arena (NORRATH_HOST_ARENA). eRand is scripted, so the check replays the same draws through an independent model: the six
// entries, the rejection sampling (including a forced retry), the x offset by parity, the light block and the frame word.
#define NORRATH_HOST_ARENA 1
#include "ctor.cpp"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

alignas(16) static unsigned char arena[0x4000];
static const unsigned base = 0x100000u;
unsigned char vt_Spell[8];
unsigned char vt_Blessed[8];
unsigned syncRandState;
unsigned frameNm;
static const unsigned fnAddress = 0x7f0000u, spellAddress = 0x7f0010u, blessedAddress = 0x7f0020u;
static std::string calls;
static int slotResult; static unsigned char* slotSeenThis; static int slotSeenA, slotSeenB;
static unsigned char* lightSeen; static unsigned char lightCopy[48];
static std::vector<int> script; static size_t scriptPos; static bool badSelf; static unsigned char* expectSelf;

static int slotTarget(void* self, int a, int b) {
    calls += "slot;"; slotSeenThis = static_cast<unsigned char*>(self); slotSeenA = a; slotSeenB = b; return slotResult;
}
extern "C" void lightAdd__FP5Light(void* light) { calls += "light;"; lightSeen = static_cast<unsigned char*>(light); std::memcpy(lightCopy, light, 48); }
extern "C" int eRand__5Spell(void* spell) {
    calls += "r;"; if (spell != expectSelf) badSelf = true;
    if (scriptPos > 4000) { std::puts("fail: rejection loop does not end"); std::exit(1); }
    return script[scriptPos++ % script.size()];
}
extern "C" unsigned char* norrath_host_pointer(unsigned a) {
    if (a == fnAddress) return reinterpret_cast<unsigned char*>(slotTarget);
    return arena + (a - base);
}
extern "C" unsigned norrath_host_address(const void* p) {
    const unsigned char* q = static_cast<const unsigned char*>(p);
    if (q >= arena && q < arena + sizeof arena) return base + static_cast<unsigned>(q - arena);
    if (q == vt_Spell) return spellAddress;
    if (q == vt_Blessed) return blessedAddress;
    return 0xdeadbeefu;
}
static unsigned word(const unsigned char* p) { unsigned v; std::memcpy(&v, p, 4); return v; }
static unsigned half(const unsigned char* p) { unsigned short v; std::memcpy(&v, p, 2); return v; }
static float flt(const unsigned char* p) { float v; std::memcpy(&v, p, 4); return v; }
static unsigned addr(const unsigned char* p) { return norrath_host_address(p); }

int main() {
    int bad = 0;
    unsigned char* const object = arena + 0x100;     // 256 bytes
    unsigned char* const creature = arena + 0x400;
    unsigned char* const skill = arena + 0x800;
    unsigned char* const table = arena + 0xc00;
    float k; { unsigned kb = 0x37800000u; std::memcpy(&k, &kb, 4); }
    volatile float zero = 0.0f; const float largest = 1.0f / zero;
    unsigned largestBits; std::memcpy(&largestBits, &largest, 4);
    // Draws: positive values keep points inside the sphere when small; the 0xffff entries force retries.
    const std::vector<int> scripts[] = {
        std::vector<int>{0x8000, 0x8000, 0x8000, 100, 7, 55, 3, 0x7000},
        std::vector<int>{0xffff, 0xffff, 0xffff, 0x8000, 0x8000, 0x8000, 0, 0xffff, 0x4000, 10, 0x9000, 0x1234},
        std::vector<int>{9, 8, 65536, 32768, 32768, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000},  // first sum is exactly 1.0
        std::vector<int>{0, 1, 2, 0xffff, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0xbeef},
    };
    const unsigned seeds[] = {0x12345678u, 0xffffffffu};
    const int ranks[] = {0x1a7, -1, 5};
    const short adjusts[] = {-24, 64};
    const int results[] = {0, 5, -3};
    const float c28s[] = {1.0f, -37.5f, 1000.0f};
    for (unsigned seed : seeds) for (int rank : ranks) for (short adj : adjusts) for (int result : results)
    for (const std::vector<int>& sc : scripts) for (float c28 : c28s) {
        std::memset(arena, 0x5a, sizeof arena);
        for (int i = 0; i < 12; ++i) creature[16 + i] = static_cast<unsigned char>(0x30 + i);
        const float c32 = c28 * -0.25f + 3.0f;
        std::memcpy(creature + 28, &c28, 4); std::memcpy(creature + 32, &c32, 4);
        const unsigned t = addr(table);
        std::memcpy(skill + 96, &t, 4); std::memcpy(table + 88, &adj, 2); std::memcpy(table + 92, &fnAddress, 4);
        calls.clear(); script = sc; scriptPos = 0; badSelf = false; expectSelf = object;
        syncRandState = seed; frameNm = 0x777u + seed % 13; slotResult = result; slotSeenThis = nullptr;
        const unsigned next = seed * 0x19660du + 0x3c6ef35fu;
        __12SpellBlessediP8CreatureP5Skill(object, rank, creature, skill);

        if (syncRandState != next || word(object + 16) != (next >> 16)) { std::puts("fail: random"); bad = 1; }
        if (object[0] != static_cast<unsigned char>(rank) || word(object + 4) != 0u) { std::puts("fail: base"); bad = 1; }
        if (word(object + 8) != addr(skill) || word(object + 12) != addr(creature) || word(object + 20) != blessedAddress) { std::puts("fail: fields"); bad = 1; }
        if (std::memcmp(object + 240, creature + 16, 12) != 0 || std::memcmp(object + 168, creature + 16, 12) != 0) { std::puts("fail: position copies"); bad = 1; }
        if (lightSeen != object + 192 || word(lightCopy + 32) != largestBits || word(lightCopy + 40) != largestBits ||
            word(lightCopy + 28) != 0u || word(lightCopy + 36) != 0u || word(lightCopy + 44) != 0x3f266677u) { std::puts("fail: light floats"); bad = 1; }
        if (half(lightCopy + 10) != 75 || half(lightCopy + 12) != 75 || half(lightCopy + 18) != 75 || half(lightCopy + 20) != 75 ||
            half(lightCopy + 14) != 200 || half(lightCopy + 22) != 200 || half(lightCopy + 16) != 255 || half(lightCopy + 24) != 255 ||
            lightCopy[8] != 0 || lightCopy[9] != 0) { std::puts("fail: light halves"); bad = 1; }
        if (slotSeenThis != skill + adj || slotSeenA != 1 || slotSeenB != static_cast<signed char>(rank)) { std::puts("fail: slot args"); bad = 1; }
        if (word(object + 180) != frameNm + 60u * static_cast<unsigned>(result)) { std::puts("fail: frame arithmetic"); bad = 1; }
        if (badSelf) { std::puts("fail: eRand receiver"); bad = 1; }
        // independent model of the draws
        size_t pos = 0;
        auto draw = [&]() { return static_cast<float>(sc[pos++ % sc.size()]); };
        for (int i = 0; i < 6; ++i) {
            const float angle = static_cast<float>(i * 10922);
            const float second = draw() * k * c28;
            const float third = c32 * 0.5f + draw() * k * (c32 * 0.5f);
            float x, y, z;
            for (;;) {
                x = draw() * k; x = x + x - 1.0f;
                y = draw() * k; y = y + y - 1.0f;
                z = draw() * k; z = z + z - 1.0f;
                float sum = x * x + y * y; sum = sum + z * z;
                if (!(1.0f < sum)) break;
            }
            x = (i & 1) == 0 ? x - 160.0f : x + 160.0f;
            const unsigned char* e = object + 24 + 12 * i; const unsigned char* v = object + 96 + 12 * i;
            if (flt(e) != angle || flt(e + 4) != second + second || flt(e + 8) != third) { std::printf("fail: entry %d\n", i); bad = 1; }
            if (flt(v) != x || flt(v + 4) != y || flt(v + 8) != z) { std::printf("fail: vector %d\n", i); bad = 1; }
        }
        if (pos != scriptPos) { std::puts("fail: draw count"); bad = 1; }
        if (calls.compare(0, 11, "light;slot;") != 0) { std::printf("fail: calls %s\n", calls.c_str()); bad = 1; }
    }
    if (bad == 0) std::puts("ok");
    return bad;
}
