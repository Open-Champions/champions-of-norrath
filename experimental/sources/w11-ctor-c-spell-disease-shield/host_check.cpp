// Host contract for __18SpellDiseaseShieldiP8CreatureP5Skill; not an EE input.
#include "spell_disease_shield_ctor.cpp"
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

unsigned char vt_Spell[8], vt_SpellDiseaseShield[8];
unsigned syncRandState;
int frameNm;
static unsigned char* theObject;
static std::string calls; static std::vector<int> entryResults; static int entryCount;
static void* entrySelf[2]; static int entryA[2], entryB[2]; static void* entryC[2];
static void* playHeader; static Point3 playPos; static float playVolume; static unsigned char w408[12];

static int fakeEntry(void* self, int a, int b, void* c) {
    entrySelf[entryCount] = self; entryA[entryCount] = a; entryB[entryCount] = b; entryC[entryCount] = c;
    calls += "entry;";
    return entryResults[entryCount++];
}
extern "C" int eRand__5Spell(void* self) {
    unsigned* word = reinterpret_cast<unsigned*>(static_cast<unsigned char*>(self) + 16);
    *word = *word * 1664525u + 1013904223u;
    return static_cast<int>(*word >> 16);
}
extern "C" void* lumpFindResource__FPcT0(char* lump, char* resource) {
    calls += "lump:"; calls += lump; calls += ":"; calls += resource; calls += ";";
    std::memcpy(w408, theObject + 408, 12);
    return reinterpret_cast<void*>(0x7007);
}
extern "C" int soundPlaySequence__FP15AnimationHeaderG6Point3f(void* h, Point3 p, float v) {
    calls += "play;"; playHeader = h; playPos = p; playVolume = v; return 0;
}

int main() {
    int bad = 0;
    alignas(16) unsigned char raw[512];
    std::memset(raw, 0xee, sizeof raw);
    theObject = raw;
    alignas(16) unsigned char skillBlock[128], tableBlock[128], creature[64];
    std::memset(skillBlock, 0, sizeof skillBlock); std::memset(tableBlock, 0, sizeof tableBlock); std::memset(creature, 0, sizeof creature);
    Entry entry{16, 0, fakeEntry};
    std::memcpy(tableBlock + 104, &entry, sizeof entry);
    char* tablePtr = reinterpret_cast<char*>(tableBlock);
    std::memcpy(skillBlock + 96, &tablePtr, sizeof tablePtr);
    const Point3 where{1.5f, -2.5f, 3.25f};
    const float radius = 2.5f;
    std::memcpy(creature + 16, &where, 12);
    std::memcpy(creature + 28, &radius, 4);
    entryResults = {5, 7};
    syncRandState = 0x12345678u;
    frameNm = 1000;
    __18SpellDiseaseShieldiP8CreatureP5Skill(raw, 0x1a7, creature, skillBlock);

    // Reference: the base step stores the high half of the stepped sync state at +16; the 32 accepted
    // points (a scaled and an unscaled one, 16 times) then step that word.
    const unsigned state = 0x12345678u * 1664525u + 1013904223u;
    if (syncRandState != state) { std::puts("fail: sync state"); bad = 1; }
    unsigned objectState; std::memcpy(&objectState, raw + 16, 4);
    unsigned word = state >> 16;
    float refA[16][3], refB[16][3];
    for (int i = 0; i < 16; ++i) {
        for (int pass = 0; pass < 2; ++pass) {
            float p[3];
            do {
                for (int c = 0; c < 3; ++c) {
                    word = word * 1664525u + 1013904223u;
                    p[c] = static_cast<float>(static_cast<int>(word >> 16)) * 0x1p-16f * 2.0f - 1.0f;
                }
            } while (1.0f < p[0] * p[0] + p[1] * p[1] + p[2] * p[2]);
            if (pass == 0) { for (int c = 0; c < 3; ++c) refA[i][c] = p[c] * radius; }
            else { for (int c = 0; c < 3; ++c) refB[i][c] = p[c]; }
        }
    }
    if (objectState != word) { std::puts("fail: random word after the draws"); bad = 1; }
    for (int i = 0; i < 16; ++i) {
        float a[3], b[3];
        std::memcpy(a, raw + 24 + 12 * i, 12); std::memcpy(b, raw + 216 + 12 * i, 12);
        if (std::memcmp(a, refA[i], 12) != 0) { std::printf("fail: scaled point %d\n", i); bad = 1; }
        if (std::memcmp(b, refB[i], 12) != 0) { std::printf("fail: unit point %d\n", i); bad = 1; }
    }
    int w420, w424; std::memcpy(&w420, raw + 420, 4); std::memcpy(&w424, raw + 424, 4);
    if (w420 != 1000 + 5 * 60 || w424 != 7 * 12) { std::puts("fail: +420/+424"); bad = 1; }
    if (entryCount != 2 || entryA[0] != 2 || entryA[1] != 1 || entryB[0] != -89 || entryB[1] != -89 ||
        entryC[0] != creature || entryC[1] != creature || entrySelf[0] != skillBlock + 16 || entrySelf[1] != skillBlock + 16) {
        std::puts("fail: entry calls"); bad = 1;
    }
    unsigned w4, w8, w12, w20; std::memcpy(&w4, raw + 4, 4); std::memcpy(&w8, raw + 8, 4); std::memcpy(&w12, raw + 12, 4); std::memcpy(&w20, raw + 20, 4);
    if (raw[0] != 0xa7 || w4 != 0u || w8 != low(skillBlock) || w12 != low(creature)) { std::puts("fail: base fields"); bad = 1; }
    if (w20 != low(vt_SpellDiseaseShield)) { std::puts("fail: table"); bad = 1; }
    Point3 p408; std::memcpy(&p408, raw + 408, 12);
    if (p408.x != where.x || p408.y != where.y || p408.z != where.z) { std::puts("fail: point at +408"); bad = 1; }
    if (calls != "entry;entry;lump:fx.lmp:spel_de_dishield.anm;play;") { std::printf("fail: calls %s\n", calls.c_str()); bad = 1; }
    if (playHeader != reinterpret_cast<void*>(0x7007) || playPos.x != where.x || playPos.y != where.y || playPos.z != where.z || playVolume != 1.0f) { std::puts("fail: play arguments"); bad = 1; }
    if (std::memcmp(w408, &where, 12) != 0) { std::puts("fail: +408 not set before the lump call"); bad = 1; }
    for (int i = 428; i < 512; ++i) if (raw[i] != 0xee) { std::puts("fail: tail written"); bad = 1; break; }
    return bad;
}
