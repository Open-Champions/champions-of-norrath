// Host contract for __16SpellFireWeaponsiP8CreatureP5Skill; not an EE input. Stored 32-bit addresses are offsets into an arena
// (NORRATH_HOST_ARENA). Checks the spell object's words, the four skill-table calls and their arguments, the inline light block
// and its lightAdd call, the weapon-record update (an independent model of the type-21 search, the rounding and the
// first-empty-record store, for both weapon blocks, including null blocks and a block that already has a non-zero type-21
// entry) and the sound call.
#define NORRATH_HOST_ARENA 1
#include "ctor.cpp"
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

alignas(16) static unsigned char arena[0x8000];
static const unsigned base = 0x100000u;
unsigned char vt_Spell[8];
unsigned char vt_FireWeapons[8];
unsigned char player[4 * 23984];
unsigned syncRandState;
int frameNm;
static const unsigned fnAddress = 0x7f0000u, spellAddress = 0x7f0010u, fireAddress = 0x7f0020u;
static std::string calls;
static int slotResults[4]; static int slotCalls; static unsigned char* slotSeenThis; static int slotSeenA[4], slotSeenB[4];
static unsigned char* lightSeen; static unsigned char lightCopy[48]; static unsigned char posAtLight[12];
static std::string lumpName, lumpArg; static void* soundHeader; static Point3 soundPoint; static float soundVolume;

static int slotTarget(void* self, int a, int b) {
    calls += "slot;"; slotSeenThis = static_cast<unsigned char*>(self);
    slotSeenA[slotCalls] = a; slotSeenB[slotCalls] = b; return slotResults[slotCalls++];
}
extern "C" void lightAdd__FP5Light(void* l) {
    calls += "light;"; lightSeen = static_cast<unsigned char*>(l); std::memcpy(lightCopy, l, 48); std::memcpy(posAtLight, lightSeen - 32 + 80, 12);
}
extern "C" void* lumpFindResource__FPcT0(char* lump, char* name) { calls += "lump;"; lumpName = lump; lumpArg = name; return arena + 0x7000; }
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
    if (q == vt_FireWeapons) return fireAddress;
    return 0xdeadbeefu;
}
static unsigned word(const unsigned char* p) { unsigned v; std::memcpy(&v, p, 4); return v; }
static short sh(const unsigned char* p) { short v; std::memcpy(&v, p, 2); return v; }
static unsigned addr(const unsigned char* p) { return norrath_host_address(p); }

// Independent model of one weapon block update.
static void model(unsigned char* w, int r0, short v, float k) {
    int found = -1;
    for (int j = 0; j < 4; ++j) if (sh(w + 36 + 8 * j) == 21) { found = j; break; }
    if (found >= 0 && sh(w + 38 + 8 * found) != 0) return;
    float f = static_cast<float>(r0); f = f + f * k;
    const float g = f >= 0.0f ? f + 0.5f : f - 0.5f;
    const short gi = static_cast<short>(static_cast<int>(g));
    for (int j = 0; j < 4; ++j) {
        const short t = sh(w + 36 + 8 * j);
        if (t == -1 || t == 21) {
            const short n21 = 21, r = static_cast<short>(r0);
            std::memcpy(w + 36 + 8 * j, &n21, 2); std::memcpy(w + 38 + 8 * j, &r, 2);
            std::memcpy(w + 40 + 8 * j, &gi, 2); std::memcpy(w + 42 + 8 * j, &v, 2);
            return;
        }
    }
}

int main() {
    int bad = 0;
    unsigned char* const object = arena + 0x100;
    unsigned char* const creature = arena + 0x400;    // 1408+ bytes
    unsigned char* const skill = arena + 0x1000;
    unsigned char* const table = arena + 0x1100;
    unsigned char* const weapon1 = arena + 0x1200;
    unsigned char* const weapon2 = arena + 0x1400;
    unsigned char* const owner = arena + 0x1600;
    unsigned char* const names = arena + 0x1700;
    float k; { unsigned kb = 0x3e4cccccu; std::memcpy(&k, &kb, 4); }
    const unsigned seeds[] = {0x12345678u, 0xffffffffu};
    const short adjusts[] = {-24, 64};
    const int r0s[] = {0, 5, -3, 40000, -40000};
    const int r1s[] = {0, 2, -3};
    const int frames[] = {0, 100, 12345678};
    // record type patterns for the four records: 0 = empty(-1), 1 = type 21 with value, 2 = type 21 with zero value, 3 = other
    const unsigned patterns1[] = {0u, 0x55u, 0xaau, 0x27u, 0xe4u, 0xffu};
    const int selects[] = {0, 3, -1};   // never 2: that slot is the second pointer's own word
    const bool nulls[] = {false, true};
    for (unsigned seed : seeds) for (short adj : adjusts) for (int r0 : r0s) for (int r1 : r1s) for (int frame : frames)
    for (unsigned p1 : patterns1) for (unsigned p2 : patterns1) for (int sel : selects) for (bool nullFirst : nulls) {
        std::memset(arena, 0x5a, sizeof arena);
        std::memset(player, 0x11, sizeof player);
        for (int i = 0; i < 12; ++i) creature[16 + i] = static_cast<unsigned char>(0x30 + i);
        const unsigned index = 2;
        std::memcpy(creature + 960, &index, 4);
        unsigned char* entry = player + index * 23984u;
        entry[23572] = static_cast<unsigned char>(sel);
        const unsigned w1 = addr(weapon1), w2 = addr(weapon2);
        const unsigned zeroWord = 0;
        std::memcpy(entry + 21960 + 4 * sel, nullFirst ? &zeroWord : &w1, 4);
        std::memcpy(entry + 21968, &w2, 4);
        for (int w = 0; w < 2; ++w) {
            unsigned char* wb = w ? weapon2 : weapon1; const unsigned pat = w ? p2 : p1;
            for (int j = 0; j < 4; ++j) {
                const int kind = (pat >> (2 * j)) & 3;
                short type = kind == 0 ? -1 : kind == 3 ? 5 : 21;
                short value = kind == 1 ? 0x77 : 0;
                std::memcpy(wb + 36 + 8 * j, &type, 2); std::memcpy(wb + 38 + 8 * j, &value, 2);
                const short filler = static_cast<short>(0x1111 * (j + 1));
                std::memcpy(wb + 40 + 8 * j, &filler, 2); std::memcpy(wb + 42 + 8 * j, &filler, 2);
            }
        }
        unsigned char expect1[128], expect2[128];
        std::memcpy(expect1, weapon1, 128); std::memcpy(expect2, weapon2, 128);
        const bool firstLive = !nullFirst;
        if (firstLive) model(expect1, r0, static_cast<short>(60 * r1), k);
        model(expect2, r0, static_cast<short>(60 * r1), k);
        const unsigned t = addr(table), ow = addr(owner), nm = addr(names);
        std::memcpy(skill + 96, &t, 4); std::memcpy(table + 88, &adj, 2); std::memcpy(table + 92, &fnAddress, 4);
        std::memcpy(creature + 36, &ow, 4); std::memcpy(owner, &nm, 4); std::memcpy(names, "fx.lmp", 7);
        calls.clear(); slotCalls = 0; slotSeenThis = nullptr; lightSeen = nullptr;
        slotResults[0] = r1; slotResults[1] = 3; slotResults[2] = r0; slotResults[3] = r1;
        syncRandState = seed; frameNm = frame;
        const unsigned next = seed * 0x19660du + 0x3c6ef35fu;
        __16SpellFireWeaponsiP8CreatureP5Skill(object, 0x1a7, creature, skill);

        if (syncRandState != next || word(object + 16) != (next >> 16)) { std::puts("fail: random"); bad = 1; }
        if (object[0] != 0xa7 || word(object + 4) != 0u) { std::puts("fail: base"); bad = 1; }
        if (word(object + 8) != addr(skill) || word(object + 12) != addr(creature) || word(object + 20) != fireAddress) { std::puts("fail: fields"); bad = 1; }
        if (slotCalls != 4 || slotSeenThis != skill + adj || slotSeenA[0] != 1 || slotSeenA[1] != 2 || slotSeenA[2] != 0 || slotSeenA[3] != 1) { std::puts("fail: slot calls"); bad = 1; continue; }
        for (int i = 0; i < 4; ++i) if (slotSeenB[i] != static_cast<signed char>(0xa7)) { std::puts("fail: slot byte"); bad = 1; }
        if (word(object + 24) != static_cast<unsigned>(frame) + 60u * static_cast<unsigned>(r1)) { std::puts("fail: frame word"); bad = 1; }
        if (word(object + 28) != 12u * 3u || word(object + 92) != static_cast<unsigned>(frame)) { std::puts("fail: words 28 and 92"); bad = 1; }
        if (std::memcmp(weapon2, expect2, 128) != 0) { std::puts("fail: second weapon"); bad = 1; }
        if (std::memcmp(weapon1, expect1, 128) != 0) { std::puts("fail: first weapon"); bad = 1; }
        if (calls != "light;slot;slot;slot;slot;lump;sound;") { std::printf("fail: calls %s\n", calls.c_str()); bad = 1; continue; }
        unsigned largestBits; { volatile float zero = 0.0f; const float largest = 1.0f / zero; std::memcpy(&largestBits, &largest, 4); }
        if (lightSeen != object + 32 || word(lightCopy + 32) != largestBits || word(lightCopy + 40) != largestBits ||
            word(lightCopy + 28) != 0u || word(lightCopy + 36) != 0u || word(lightCopy + 44) != 0x3f266655u) { std::puts("fail: light floats"); bad = 1; }
        if (sh(lightCopy + 10) != 255 || sh(lightCopy + 18) != 255 || sh(lightCopy + 12) != 128 || sh(lightCopy + 20) != 128 ||
            sh(lightCopy + 14) != 0 || sh(lightCopy + 16) != 0 || sh(lightCopy + 22) != 0 || sh(lightCopy + 24) != 0 ||
            lightCopy[8] != 0 || lightCopy[9] != 0) { std::puts("fail: light halves"); bad = 1; }
        if (std::memcmp(posAtLight, creature + 16, 12) != 0 || std::memcmp(object + 80, creature + 16, 12) != 0) { std::puts("fail: position copy"); bad = 1; }
        if (lumpName != "fx.lmp" || lumpArg != "spel_ew_heatwpn.anm" || soundHeader != arena + 0x7000 ||
            std::memcmp(&soundPoint, creature + 16, 12) != 0 || soundVolume != 1.0f) { std::puts("fail: sound"); bad = 1; }
    }
    if (bad == 0) std::puts("ok");
    return bad;
}
