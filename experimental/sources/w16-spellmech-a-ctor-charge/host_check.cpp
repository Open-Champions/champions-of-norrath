// Host contract for __11SpellChargeiP8CreatureP5Skill; not an EE input. Stored 32-bit addresses are offsets into an
// arena (NORRATH_HOST_ARENA). Checks the stored fields, the 48-byte light block, the sound call, the angle lookups, and the
// ten trail starts/extensions: handle words, constant arguments, and the position plus offset arithmetic in float.
#define NORRATH_HOST_ARENA 1
#include "ctor.cpp"
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

alignas(16) static unsigned char arena[0x4000];
static const unsigned base = 0x100000u;
unsigned char vt_Spell[8];
unsigned char vt_Charge[8];
unsigned syncRandState;
unsigned frameNm;
static const unsigned spellAddress = 0x7f0010u, chargeAddress = 0x7f0020u;
static std::string calls;
static unsigned char* lightSeen; static unsigned char lightCopy[48];
static std::string lumpName, lumpArg;
static void* soundHeader; static void* soundObject; static float soundVolume;
static std::vector<int> angles; static float cosValue, sinValue;
struct Start { int a, b; float c; int d, e, f, g, h; };
static std::vector<Start> starts;
struct Extend { int handle; Point3 point; };
static std::vector<Extend> extends;

extern "C" void lightAdd__FP5Light(void* light) { calls += "light;"; lightSeen = static_cast<unsigned char*>(light); std::memcpy(lightCopy, light, 48); }
extern "C" void* lumpFindResource__FPcT0(char* lump, char* name) { calls += "lump;"; lumpName = lump; lumpArg = name; return arena + 0x3000; }
extern "C" void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* sound, void* object, float volume) {
    calls += "sound;"; soundHeader = sound; soundObject = object; soundVolume = volume;
}
extern "C" float icos__Fi(int angle) { calls += "cos;"; angles.push_back(angle); return cosValue; }
extern "C" float isin__Fi(int angle) { calls += "sin;"; angles.push_back(angle); return sinValue; }
extern "C" int P_StartTrail__Fiifiiiii(int a, int b, float c, int d, int e, int f, int g, int h) {
    calls += "start;"; Start s = {a, b, c, d, e, f, g, h}; starts.push_back(s); return 0x500 + static_cast<int>(starts.size());
}
extern "C" void P_ExtendTrail__FiG6Point3(int handle, Point3 point) { calls += "extend;"; Extend e = {handle, point}; extends.push_back(e); }
extern "C" unsigned char* norrath_host_pointer(unsigned a) { return arena + (a - base); }
extern "C" unsigned norrath_host_address(const void* p) {
    const unsigned char* q = static_cast<const unsigned char*>(p);
    if (q >= arena && q < arena + sizeof arena) return base + static_cast<unsigned>(q - arena);
    if (q == vt_Spell) return spellAddress;
    if (q == vt_Charge) return chargeAddress;
    return 0xdeadbeefu;
}
static unsigned word(const unsigned char* p) { unsigned v; std::memcpy(&v, p, 4); return v; }
static unsigned half(const unsigned char* p) { unsigned short v; std::memcpy(&v, p, 2); return v; }
static float flt(const unsigned char* p) { float v; std::memcpy(&v, p, 4); return v; }
static unsigned addr(const unsigned char* p) { return norrath_host_address(p); }

int main() {
    int bad = 0;
    unsigned char* const object = arena + 0x100;    // 144 bytes
    unsigned char* const creature = arena + 0x400;
    unsigned char* const skill = arena + 0x800;
    const unsigned seeds[] = {0x12345678u, 0xffffffffu};
    const int ranks[] = {0x1a7, -1};
    const short angles_in[] = {0, 0x4000, -0x4000, 0x1234};
    const float cosines[] = {1.0f, -0.5f, 0.0f};
    const float sines[] = {0.0f, 0.866f, -1.0f};
    const float positions[][3] = {{0.0f, 0.0f, 0.0f}, {100.5f, -300.25f, 77.0f}, {-1.0e6f, 3.0f, 1.0e-3f}};
    for (unsigned seed : seeds) for (int rank : ranks) for (short angle : angles_in) for (int ci = 0; ci < 3; ++ci) for (int pi = 0; pi < 3; ++pi) {
        std::memset(arena, 0x5a, sizeof arena);
        std::memcpy(creature + 16, positions[pi], 12);
        std::memcpy(creature + 56, &angle, 2);
        const unsigned creatureAddr = addr(creature);
        calls.clear(); starts.clear(); extends.clear(); angles.clear(); soundObject = nullptr;
        cosValue = cosines[ci]; sinValue = sines[(ci + pi) % 3];
        syncRandState = seed; frameNm = 0x2468u + seed % 11;
        const unsigned next = seed * 0x19660du + 0x3c6ef35fu;
        __11SpellChargeiP8CreatureP5Skill(object, rank, creature, skill);

        if (syncRandState != next || word(object + 16) != (next >> 16)) { std::puts("fail: random"); bad = 1; }
        if (object[0] != static_cast<unsigned char>(rank) || word(object + 4) != 0u) { std::puts("fail: base"); bad = 1; }
        if (word(object + 8) != addr(skill) || word(object + 12) != creatureAddr || word(object + 20) != chargeAddress) { std::puts("fail: fields"); bad = 1; }
        if (std::memcmp(object + 124, creature + 16, 12) != 0) { std::puts("fail: position copy"); bad = 1; }
        if (lightSeen != object + 76 || flt(lightCopy + 28) != 103.0f || flt(lightCopy + 36) != 103.0f ||
            word(lightCopy + 32) != 0x38c5ad3bu || word(lightCopy + 40) != 0x38c5ad3bu || word(lightCopy + 44) != 0u) { std::puts("fail: light floats"); bad = 1; }
        if (half(lightCopy + 10) != 108 || half(lightCopy + 18) != 108 || half(lightCopy + 12) != 102 || half(lightCopy + 20) != 102 ||
            half(lightCopy + 14) != 208 || half(lightCopy + 22) != 208 || half(lightCopy + 16) != 0 || half(lightCopy + 24) != 0 ||
            lightCopy[8] != 0 || lightCopy[9] != 0) { std::puts("fail: light halves"); bad = 1; }
        if (lumpName != "fx.lmp" || lumpArg != "spel_bw_bull.anm" || soundHeader != arena + 0x3000 || soundObject != creature || soundVolume != 1.0f) { std::puts("fail: sound"); bad = 1; }
        if (word(object + 68) != frameNm || word(object + 72) != 0u) { std::puts("fail: frame words"); bad = 1; }
        if (angles.size() != 2 || angles[0] != angle || angles[1] != angle) { std::puts("fail: angles"); bad = 1; }
        std::string want = "light;lump;sound;cos;sin;";
        for (int i = 0; i < 10; ++i) want += "start;extend;";
        if (calls != want || starts.size() != 10 || extends.size() != 10) { std::printf("fail: calls %s\n", calls.c_str()); bad = 1; continue; }
        const float x = cosValue * 10.0f, y = sinValue * 10.0f;
        float k08, k6; unsigned b08 = 0x3f4cccccu, b6 = 0x40bfffffu; std::memcpy(&k08, &b08, 4); std::memcpy(&k6, &b6, 4);
        for (int i = 0; i < 10; ++i) {
            const Start& s = starts[i];
            if (s.a != 1 || s.b != 800 || s.c != k08 || s.d != 4 || s.e != 64 || s.f != 64 || s.g != 128 || s.h != 32) { std::puts("fail: start args"); bad = 1; }
            if (word(object + 28 + 4 * i) != static_cast<unsigned>(0x500 + i + 1)) { std::puts("fail: handle word"); bad = 1; }
            const float z = static_cast<float>(i) * k6 + 12.0f;
            const float px = positions[pi][0] + x, py = positions[pi][1] + y, pz = positions[pi][2] + z;
            if (extends[i].handle != 0x500 + i + 1 || std::memcmp(&extends[i].point.x, &px, 4) != 0 ||
                std::memcmp(&extends[i].point.y, &py, 4) != 0 || std::memcmp(&extends[i].point.z, &pz, 4) != 0) { std::puts("fail: extend"); bad = 1; }
        }
    }
    if (bad == 0) std::puts("ok");
    return bad;
}
