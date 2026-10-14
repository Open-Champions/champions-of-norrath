// Host contract for __15SpellWizardBeamiP8CreatureP5Skill; not an EE input. Stored 32-bit addresses are offsets into an arena
// (NORRATH_HOST_ARENA). eRand, hsv, fmodf, P_StartTrail and the skill table entry are mocks; the check replays the expected
// call sequence (every trail's arguments and the handle stored for it), the three colours derived from the hue, the float and
// word stores around the skill table calls, the limit of six on word +184, and the sound call.
#define NORRATH_HOST_ARENA 1
#include "ctor.cpp"
#include <cmath>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

alignas(16) static unsigned char arena[0x2000];
static const unsigned base = 0x100000u;
unsigned char vt_Spell[8];
unsigned char vt_WizardBeam[8];
unsigned syncRandState;
static const unsigned fnAddress = 0x7f0000u, spellAddress = 0x7f0010u, beamAddress = 0x7f0020u;
static std::string calls;
static unsigned rollValue; static void* randSelf;
struct Hsv { float h, s, v; };
static std::vector<Hsv> hsvCalls; static std::vector<std::pair<float, float> > fmodCalls;
struct Trail { int a, b; float c; int d, e, f, g, h; };
static std::vector<Trail> trails;
static int slotResults[3]; static int slotCalls; static unsigned char* slotSeenThis; static int slotSeenA[3], slotSeenB[3];
static std::string lumpName, lumpArg; static void* soundHeader; static Point3 soundPoint; static float soundVolume;

static int slotTarget(void* self, int a, int b) {
    calls += "slot;"; slotSeenThis = static_cast<unsigned char*>(self);
    slotSeenA[slotCalls] = a; slotSeenB[slotCalls] = b; return slotResults[slotCalls++];
}
extern "C" int eRand__5Spell(void* spell) { calls += "rand;"; randSelf = spell; return static_cast<int>(rollValue); }
extern "C" unsigned hsv__Ffff(float h, float s, float v) {
    calls += "hsv;"; Hsv x = {h, s, v}; hsvCalls.push_back(x);
    unsigned hb, sb; std::memcpy(&hb, &h, 4); std::memcpy(&sb, &s, 4);
    return (hb * 31u + sb * 17u + 0x01020304u) ^ 0x00a5c3e1u;
}
extern "C" float fmodf(float x, float y) { calls += "fmod;"; fmodCalls.push_back(std::make_pair(x, y)); return static_cast<float>(std::fmod(static_cast<double>(x), static_cast<double>(y))); }
extern "C" int P_StartTrail__Fiifiiiii(int a, int b, float c, int d, int e, int f, int g, int h) {
    calls += "trail;"; Trail t = {a, b, c, d, e, f, g, h}; trails.push_back(t); return 0x500 + static_cast<int>(trails.size());
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
    if (q == vt_WizardBeam) return beamAddress;
    return 0xdeadbeefu;
}
static unsigned word(const unsigned char* p) { unsigned v; std::memcpy(&v, p, 4); return v; }
static float flt(const unsigned char* p) { float v; std::memcpy(&v, p, 4); return v; }
static unsigned addr(const unsigned char* p) { return norrath_host_address(p); }
static unsigned colour(float h, float s) {
    unsigned hb, sb; std::memcpy(&hb, &h, 4); std::memcpy(&sb, &s, 4);
    return (hb * 31u + sb * 17u + 0x01020304u) ^ 0x00a5c3e1u;
}

int main() {
    int bad = 0;
    unsigned char* const object = arena + 0x100;      // 0x400 bytes
    unsigned char* const creature = arena + 0x600;
    unsigned char* const skill = arena + 0x800;
    unsigned char* const table = arena + 0x900;
    float k; { unsigned kb = 0x37800000u; std::memcpy(&k, &kb, 4); }
    float s1, s2, s3; { unsigned a = 0x3f4cccccu, b = 0x3f666666u, c = 0x3f199999u; std::memcpy(&s1, &a, 4); std::memcpy(&s2, &b, 4); std::memcpy(&s3, &c, 4); }
    const unsigned seeds[] = {0x12345678u, 0xffffffffu};
    const int ranks[] = {0x1a7, -1, 4};
    const short adjusts[] = {-24, 64};
    const unsigned rolls[] = {0u, 1u, 32768u, 65535u, 12345u};
    const int firsts[] = {0, 5, -3};
    const int thirds[] = {0, 3, -2};
    const int seconds[] = {-1, 0, 1, 5, 6, 7, 100};
    for (unsigned seed : seeds) for (int rank : ranks) for (short adj : adjusts) for (unsigned roll : rolls)
    for (int r1 : firsts) for (int r3 : thirds) for (int r2 : seconds) {
        std::memset(arena, 0x5a, sizeof arena);
        for (int i = 0; i < 12; ++i) creature[16 + i] = static_cast<unsigned char>(0x30 + i);
        const unsigned t = addr(table);
        std::memcpy(skill + 96, &t, 4); std::memcpy(table + 88, &adj, 2); std::memcpy(table + 92, &fnAddress, 4);
        calls.clear(); hsvCalls.clear(); fmodCalls.clear(); trails.clear(); slotCalls = 0; slotSeenThis = nullptr; randSelf = nullptr;
        slotResults[0] = r1; slotResults[1] = r3; slotResults[2] = r2; rollValue = roll;
        syncRandState = seed;
        const unsigned next = seed * 0x19660du + 0x3c6ef35fu;
        __15SpellWizardBeamiP8CreatureP5Skill(object, rank, creature, skill);

        if (syncRandState != next || word(object + 16) != (next >> 16)) { std::puts("fail: random"); bad = 1; }
        if (object[0] != static_cast<unsigned char>(rank) || word(object + 4) != 0u || object[25] != 0 || object[26] != 0) { std::puts("fail: base"); bad = 1; }
        if (word(object + 8) != addr(skill) || word(object + 12) != addr(creature) || word(object + 20) != beamAddress) { std::puts("fail: fields"); bad = 1; }
        if (randSelf != object) { std::puts("fail: eRand receiver"); bad = 1; }
        // colours
        const float hue = static_cast<float>(static_cast<int>(roll)) * k;
        const float shifted = hue + 0.5f;
        const float wrapped = static_cast<float>(std::fmod(static_cast<double>(shifted), 1.0));
        const unsigned c1 = colour(hue, s1), c2 = colour(wrapped, s2), c3 = colour(wrapped, s3);
        if (hsvCalls.size() != 3 || hsvCalls[0].h != hue || hsvCalls[0].s != s1 || hsvCalls[0].v != 1.0f ||
            hsvCalls[1].h != wrapped || hsvCalls[1].s != s2 || hsvCalls[2].h != wrapped || hsvCalls[2].s != s3 ||
            fmodCalls.size() != 2 || fmodCalls[0].first != shifted || fmodCalls[0].second != 1.0f || fmodCalls[1].first != shifted) { std::puts("fail: colour derivation"); bad = 1; continue; }
        // expected trails
        std::vector<Trail> want;
        auto add = [&](int a, int b, unsigned c, int f) {
            Trail t = {a, b, 2.0f, 5, static_cast<int>(c & 0xff), static_cast<int>((c >> 8) & 0xff), static_cast<int>((c >> 16) & 0xff), f};
            want.push_back(t);
        };
        add(1, 200, c1, 128); add(1, 100, c2, 128); add(1, 100, c3, 128);
        for (int i = 0; i < 5; ++i) add(6, 4000, c1, 32);
        add(1, 300, c1, 128);
        const int limit = r2 >= 7 ? 6 : r2;
        for (int i = 0; i < limit; ++i) { add(1, 100, c1, 128); add(1, 100, c2, 128); }
        if (trails.size() != want.size()) { std::printf("fail: trail count %zu vs %zu\n", trails.size(), want.size()); bad = 1; continue; }
        for (size_t i = 0; i < want.size(); ++i) {
            const Trail& g = trails[i]; const Trail& w = want[i];
            if (g.a != w.a || g.b != w.b || g.c != w.c || g.d != w.d || g.e != w.e || g.f != w.f || g.g != w.g || g.h != w.h) { std::printf("fail: trail %zu\n", i); bad = 1; }
        }
        // handles
        if (word(object + 28) != 0x501u || word(object + 32) != 0x502u || word(object + 36) != 0x503u) { std::puts("fail: first handles"); bad = 1; }
        for (int i = 0; i < 5; ++i) {
            if (word(object + 848 + 8 * i) != static_cast<unsigned>(0x500 + 4 + i) || word(object + 852 + 8 * i) != 0xfffe7960u) { std::puts("fail: ring handles"); bad = 1; }
        }
        if (word(object + 892) != 0x509u) { std::puts("fail: handle 892"); bad = 1; }
        for (int i = 0; i < limit; ++i)
            if (word(object + 188 + 4 * i) != static_cast<unsigned>(0x500 + 10 + 2 * i) || word(object + 212 + 4 * i) != static_cast<unsigned>(0x500 + 11 + 2 * i)) { std::puts("fail: pair handles"); bad = 1; }
        if (word(object + 888) != c1) { std::puts("fail: colour word"); bad = 1; }
        if (slotSeenThis != skill + adj || slotSeenA[0] != 1 || slotSeenA[1] != 3 || slotSeenA[2] != 2) { std::puts("fail: slot args"); bad = 1; }
        for (int i = 0; i < 3; ++i) if (slotSeenB[i] != static_cast<signed char>(rank)) { std::puts("fail: slot rank"); bad = 1; }
        if (flt(object + 840) != static_cast<float>(r1 * 2) || flt(object + 844) != static_cast<float>(r1 * 2)) { std::puts("fail: doubled floats"); bad = 1; }
        if (flt(object + 836) != static_cast<float>(r3 * 3)) { std::puts("fail: tripled float"); bad = 1; }
        for (int i = 0; i < 6; ++i) if (flt(object + 812 + 4 * i) != static_cast<float>(r3 * 3)) { std::puts("fail: copied floats"); bad = 1; }
        if (word(object + 184) != static_cast<unsigned>(r2 >= 7 ? 6 : r2)) { std::puts("fail: limit word"); bad = 1; }
        if (lumpName != "fx.lmp" || lumpArg != "spel_ew_wbeam.anm" || soundHeader != arena + 0x1c00 ||
            std::memcmp(&soundPoint, creature + 16, 12) != 0 || soundVolume != 1.0f) { std::puts("fail: sound"); bad = 1; }
    }
    if (bad == 0) std::puts("ok");
    return bad;
}
