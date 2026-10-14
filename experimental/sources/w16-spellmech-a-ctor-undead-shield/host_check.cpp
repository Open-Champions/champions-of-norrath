// Host contract for __17SpellUndeadShieldiP8CreatureP5Skill; not an EE input. Stored 32-bit addresses are offsets into an
// arena (NORRATH_HOST_ARENA). Checks the stored fields, the 48-byte light block, the call order and arguments, the frame
// arithmetic, the position copy and the sound handle. The two division-by-zero floats are compared with the same run-time
// division on the host, so only "stored from 1.0f / 0.0f" is checked here, not the EE bit pattern.
#define NORRATH_HOST_ARENA 1
#include "ctor.cpp"
#include <cstdio>
#include <cstring>
#include <string>

alignas(16) static unsigned char arena[0x4000];
static const unsigned base = 0x100000u;
unsigned char vt_Spell[8];
unsigned char vt_UndeadShield[8];
unsigned syncRandState;
unsigned frameNm;
static std::string calls;
static int slotResult;
static unsigned char* slotSeenThis;
static int slotSeenA, slotSeenB;
static unsigned char* lightSeen;
static unsigned char lightCopy[48];
static std::string lumpName, lumpArg;
static unsigned char* soundHeader; static Point3 soundPoint; static float soundVolume; static int soundResult, persistSeen;

static int slotTarget(void* self, int a, int b) {
    calls += "slot;"; slotSeenThis = static_cast<unsigned char*>(self); slotSeenA = a; slotSeenB = b;
    return slotResult;
}
extern "C" void* P_CreateParticleSystem__Fv() { calls += "particle;"; return arena + 0x3000; }
extern "C" void lightAdd__FP5Light(void* light) {
    calls += "light;"; lightSeen = static_cast<unsigned char*>(light); std::memcpy(lightCopy, light, 48);
}
extern "C" void* lumpFindResource__FPcT0(char* lump, char* name) { calls += "lump;"; lumpName = lump; lumpArg = name; return arena + 0x3400; }
extern "C" int soundPlaySequence__FP15AnimationHeaderG6Point3f(void* header, Point3 point, float volume) {
    calls += "sound;"; soundHeader = static_cast<unsigned char*>(header); soundPoint = point; soundVolume = volume; return soundResult;
}
extern "C" void soundMakePersistant__Fi(int sound) { calls += "persist;"; persistSeen = sound; }
static const unsigned fnAddress = 0x7f0000u, spellAddress = 0x7f0010u, shieldAddress = 0x7f0020u;
extern "C" unsigned char* norrath_host_pointer(unsigned a) {
    if (a == fnAddress) return reinterpret_cast<unsigned char*>(slotTarget);
    return arena + (a - base);
}
extern "C" unsigned norrath_host_address(const void* p) {
    const unsigned char* q = static_cast<const unsigned char*>(p);
    if (q >= arena && q < arena + sizeof arena) return base + static_cast<unsigned>(q - arena);
    if (q == vt_Spell) return spellAddress;
    if (q == vt_UndeadShield) return shieldAddress;
    return 0xdeadbeefu;
}
static unsigned word(const unsigned char* p) { unsigned v; std::memcpy(&v, p, 4); return v; }
static unsigned bits(const unsigned char* p) { return word(p); }
static unsigned half(const unsigned char* p) { unsigned short v; std::memcpy(&v, p, 2); return v; }
static unsigned addr(const unsigned char* p) { return norrath_host_address(p); }

int main() {
    int bad = 0;
    unsigned char* const object = arena + 0x100;
    unsigned char* const creature = arena + 0x400;
    unsigned char* const skill = arena + 0x800;
    unsigned char* const table = arena + 0xc00;
    volatile float zero = 0.0f;
    const float largest = 1.0f / zero;
    unsigned largestBits; std::memcpy(&largestBits, &largest, 4);
    const unsigned seeds[] = {0x12345678u, 0u, 0xffffffffu};
    const int ranks[] = {0x1a7, 0x80, -1, 7};
    const short adjusts[] = {-24, 0, 64};
    const int results[] = {0, 5, -3, 100000};
    const unsigned frames[] = {0u, 1000u, 0xfffffff0u};
    for (unsigned seed : seeds) for (int rank : ranks) for (short adj : adjusts) for (int result : results) for (unsigned frame : frames) {
        std::memset(arena, 0x5a, sizeof arena);
        for (int i = 0; i < 12; ++i) creature[16 + i] = static_cast<unsigned char>(0x30 + i);
        const unsigned t = addr(table);
        std::memcpy(skill + 96, &t, 4);
        std::memcpy(table + 88, &adj, 2);
        std::memcpy(table + 92, &fnAddress, 4);
        calls.clear(); syncRandState = seed; frameNm = frame; slotResult = result; slotSeenThis = nullptr; lightSeen = nullptr;
        soundResult = 40 + result; persistSeen = 0;
        const unsigned next = seed * 0x19660du + 0x3c6ef35fu;
        __17SpellUndeadShieldiP8CreatureP5Skill(object, rank, creature, skill);

        if (syncRandState != next || word(object + 16) != (next >> 16)) { std::puts("fail: random"); bad = 1; }
        if (object[0] != static_cast<unsigned char>(rank) || word(object + 4) != 0u) { std::puts("fail: base"); bad = 1; }
        if (word(object + 8) != addr(skill) || word(object + 12) != addr(creature)) { std::puts("fail: skill/creature"); bad = 1; }
        if (word(object + 20) != shieldAddress) { std::puts("fail: table"); bad = 1; }
        if (std::memcmp(object + 80, creature + 16, 12) != 0) { std::puts("fail: position copy"); bad = 1; }
        if (calls != "light;slot;particle;lump;sound;persist;") { std::printf("fail: calls %s\n", calls.c_str()); bad = 1; }
        if (lightSeen != object + 32) { std::puts("fail: light pointer"); bad = 1; }
        if (bits(lightCopy + 32) != largestBits || bits(lightCopy + 40) != largestBits) { std::puts("fail: light huge"); bad = 1; }
        if (bits(lightCopy + 28) != 0u || bits(lightCopy + 36) != 0u || bits(lightCopy + 44) != 0x3ecccceeu) { std::puts("fail: light floats"); bad = 1; }
        if (half(lightCopy + 10) != 250 || half(lightCopy + 18) != 250 || half(lightCopy + 12) != 200 || half(lightCopy + 20) != 200 ||
            half(lightCopy + 14) != 100 || half(lightCopy + 22) != 100 || half(lightCopy + 16) != 150 || half(lightCopy + 24) != 150) { std::puts("fail: light halves"); bad = 1; }
        if (lightCopy[8] != 0 || lightCopy[9] != 0) { std::puts("fail: light bytes"); bad = 1; }
        if (slotSeenThis != skill + adj || slotSeenA != 1 || slotSeenB != static_cast<signed char>(rank)) { std::puts("fail: slot args"); bad = 1; }
        if (word(object + 28) != 85u || word(object + 24) != frame + 60u * static_cast<unsigned>(result)) { std::puts("fail: frame arithmetic"); bad = 1; }
        if (word(object + 92) != addr(arena + 0x3000)) { std::puts("fail: particle"); bad = 1; }
        if (lumpName != "fx.lmp" || lumpArg != "spel_he_unshield.anm" || soundHeader != arena + 0x3400 ||
            std::memcmp(&soundPoint, creature + 16, 12) != 0 || soundVolume != 1.0f) { std::puts("fail: sound"); bad = 1; }
        if (word(object + 96) != static_cast<unsigned>(soundResult) || persistSeen != soundResult) { std::puts("fail: handle"); bad = 1; }
    }
    if (bad == 0) std::puts("ok");
    return bad;
}
