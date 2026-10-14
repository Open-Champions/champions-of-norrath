// Host contract for __5MummyG6Point3sPPcPc; not an EE input. Stored 32-bit addresses are offsets into an arena
// (NORRATH_HOST_ARENA). Checks the base constructor arguments, the initial stores, the name-to-kind mapping for every listed
// name (and case variants, near misses and the empty name), the kind-specific weapon mask and the optional second random
// step from scripted draws, the model-chunk call, the fixed call sequence and the twelve animation-flag calls.
#define NORRATH_HOST_ARENA 1
#include "ctor.cpp"
#include <cstdio>
#include <cstring>
#include <strings.h>
#include <string>
#include <vector>

alignas(16) static unsigned char arena[0x4000];
static const unsigned base = 0x100000u;
unsigned char vt_Mummy[8];
unsigned char mummyResourceMap[8];
int frameNm;
static const unsigned mummyAddress = 0x7f0010u;
static std::string calls;
static unsigned char* baseSelf; static Point3 basePoint; static short baseType; static void* baseMap; static float baseScale; static char** baseArgs;
static const unsigned modelOffset = 0x2000, headerOffset = 0x2100;
static std::vector<std::string> compared;
static std::vector<unsigned> rolls; static size_t rollPos; static bool rollSelfBad;
static void* chunkHeader; static int chunkMask; static unsigned long long chunkResult;
static unsigned char* sawSelf[6]; static int sawArg[6];
struct Anim { int id; float f1; int a; float f2; int b; float f3; int c; float f4; int d; };
static std::vector<Anim> anims;
static unsigned long long qwordAtSetWeapon;

extern "C" void* __8CreatureG6Point3sPQ210GameObject11resourceMapfPPc(void* object, Point3 point, short type, void* map, float scale, char** args) {
    calls += "creature;"; baseSelf = static_cast<unsigned char*>(object); basePoint = point; baseType = type; baseMap = map; baseScale = scale; baseArgs = args;
    const unsigned m = base + modelOffset, h = base + headerOffset;
    std::memcpy(baseSelf + 36, &m, 4); std::memcpy(arena + modelOffset + 12, &h, 4);
    return object;
}
extern "C" int strticmp__FPCcT0(const char* left, const char* right) { calls += "cmp;"; compared.push_back(right); return strcasecmp(left, right); }
extern "C" unsigned eRand__10GameObject(void* object) {
    calls += "rand;"; if (object != baseSelf) rollSelfBad = true;
    return rolls[rollPos++ % rolls.size()];
}
extern "C" unsigned long long modelGetActiveChunks__FP12_modelHeaderi(void* header, int mask) {
    calls += "chunks;"; chunkHeader = header; chunkMask = mask; chunkResult = 0x1122334455667788ull + static_cast<unsigned>(mask); return chunkResult;
}
extern "C" void setWeapon__5Mummy(void* o) { calls += "weapon;"; std::memcpy(&qwordAtSetWeapon, static_cast<unsigned char*>(o) + 552, 8); }
extern "C" void initType__5Mummyb(void* o, int flag) { calls += "type;"; (void)o; sawArg[0] = flag; }
extern "C" void animInitState__FP14AnimationState(void* state) { calls += "animinit;"; sawSelf[1] = static_cast<unsigned char*>(state); }
extern "C" void setState__5Mummyi(void* o, int state) { calls += "state;"; (void)o; sawArg[2] = state; }
extern "C" void objectAddToSlowRunList__FP10GameObject(void* o) { calls += "slowrun;"; sawSelf[3] = static_cast<unsigned char*>(o); }
extern "C" void setAnimEventFlags__8Creatureififififi(void* o, int id, float a, int b, float c, int d, float e, int f, float g, int h) {
    calls += "anim;"; (void)o; Anim x = {id, a, b, c, d, e, f, g, h}; anims.push_back(x);
}
extern "C" void finishConstruction__8Creature(void* o) { calls += "finish;"; sawSelf[5] = static_cast<unsigned char*>(o); }
extern "C" unsigned char* norrath_host_pointer(unsigned a) { return arena + (a - base); }
extern "C" unsigned norrath_host_address(const void* p) {
    const unsigned char* q = static_cast<const unsigned char*>(p);
    if (q >= arena && q < arena + sizeof arena) return base + static_cast<unsigned>(q - arena);
    if (q == vt_Mummy) return mummyAddress;
    return 0xdeadbeefu;
}
static unsigned word(const unsigned char* p) { unsigned v; std::memcpy(&v, p, 4); return v; }
static unsigned half(const unsigned char* p) { unsigned short v; std::memcpy(&v, p, 2); return v; }
static float flt(const unsigned char* p) { float v; std::memcpy(&v, p, 4); return v; }

struct Kind { const char* name; unsigned kind; };

int main() {
    int bad = 0;
    unsigned char* const object = arena + 0x100;      // 1200+ bytes
    const Kind names[] = {{"mummytemplesoldier", 2}, {"mummytso", 2}, {"MUMMYTSO", 2}, {"mummydesertshaman", 1}, {"MummyDsh", 1},
                          {"mummytempleshaman", 3}, {"mummytsh", 3}, {"mummyelitesoldier", 4}, {"mummyeso", 4}, {"mummyeliteshaman", 5},
                          {"mummyesh", 5}, {"mummy", 0}, {"", 0}, {"mummytso2", 0}};
    const std::vector<unsigned> rollSets[] = {
        {0u, 1u, 2u, 3u, 4u, 5u}, {5u, 6552u, 1u, 7u, 9u, 11u}, {100000u, 100u, 3u, 4u, 5u, 6u},
        {2u, 6554u, 8u, 1u, 3u, 7u}, {0xffffffffu, 0x7fffffffu, 0x80000000u, 9u, 0xfffffffdu, 4u}, {7u, 0u, 4u, 5u, 10u, 3u}};
    const Point3 point = {1.5f, -2.0f, 3.25f};
    const int types[] = {0, 7, -1, 0x7fff, -32768};
    for (const Kind& k : names) for (const std::vector<unsigned>& rs : rollSets) for (int type : types) for (int frame : {0, 1234, -5}) {
        std::memset(arena, 0x5a, sizeof arena);
        const unsigned long long flagsBefore = 0x00000002000000ffull;
        std::memcpy(object + 160, &flagsBefore, 8);
        calls.clear(); compared.clear(); anims.clear(); rolls = rs; rollPos = 0; rollSelfBad = false; frameNm = frame;
        char* args = reinterpret_cast<char*>(arena + 0x3000);
        char** const argList = &args;
        __5MummyG6Point3sPPcPc(object, point, type, argList, const_cast<char*>(k.name));

        if (baseSelf != object || std::memcmp(&basePoint, &point, 12) != 0 || baseType != static_cast<short>(type) || baseMap != mummyResourceMap ||
            baseScale != 10.0f || baseArgs != argList) { std::puts("fail: creature args"); bad = 1; }
        if (word(object + 72) != mummyAddress) { std::puts("fail: table"); bad = 1; }
        for (int i = 0; i < 4; ++i) if (half(object + 1068 + 8 * i) != 0xffff) { std::puts("fail: halfwords -1"); bad = 1; }
        if (object[1064] != 0xff || object[1065] != 0xff) { std::puts("fail: bytes -1"); bad = 1; }
        for (int i = 1048; i < 1064; ++i) if (object[i] != 0) { std::puts("fail: zero run"); bad = 1; }
        const unsigned long long flagsWant = flagsBefore | 0x0000000400010000ull;
        unsigned long long flagsNow; std::memcpy(&flagsNow, object + 160, 8);
        if (object[40] != 30 || flagsNow != flagsWant || word(object + 168) != 0x3f999999u || word(object + 248) != static_cast<unsigned>(frame) ||
            flt(object + 28) != 15.0f || object[172] != 16) { std::puts("fail: initial stores"); bad = 1; }
        if (word(object + 176) != 0 || word(object + 192) != 0 || word(object + 236) != 0 || half(object + 572) != 0 ||
            word(object + 1104) != 0 || word(object + 1108) != 0 || word(object + 1112) != 0) { std::puts("fail: zero stores"); bad = 1; }
        if (word(object + 960) != k.kind) { std::printf("fail: kind for %s\n", k.name); bad = 1; continue; }
        // model of the kind-specific draws
        size_t pos = 0; auto next = [&]() { return rs[pos++ % rs.size()]; };
        unsigned mask = 0;
        bool setSecond = false; unsigned second = 0;
        const float scale = 1.0f / 65536.0f, tenth = [] { unsigned b = 0x3dccccccu; float f; std::memcpy(&f, &b, 4); return f; }();
        switch (k.kind) {
        case 0: { mask = 1u << (next() % 3u); if (static_cast<float>(static_cast<int>(next())) * scale < tenth) { setSecond = true; second = (next() & 1u) + 1u; } break; }
        case 1: mask = 8; break;
        case 2: { mask = 1u << ((next() & 3u) + 4u); if (static_cast<float>(static_cast<int>(next())) * scale < tenth) { setSecond = true; second = (next() & 1u) + 1u; } break; }
        case 3: mask = 256; break;
        case 4: { mask = 1u << ((next() & 1u) + 9u); if (static_cast<float>(static_cast<int>(next())) * scale < tenth) { setSecond = true; second = (next() & 3u) + 1u; } break; }
        case 5: mask = 2048; break;
        }
        if (word(object + 244) != mask || chunkMask != static_cast<int>(mask) || pos != rollPos) { std::printf("fail: mask kind %u\n", k.kind); bad = 1; }
        if (setSecond && word(object + 208) != second) { std::puts("fail: word 208"); bad = 1; }
        if (!setSecond && word(object + 208) != 0x5a5a5a5au) { std::puts("fail: word 208 touched"); bad = 1; }
        if (rollSelfBad) { std::puts("fail: eRand receiver"); bad = 1; }
        if (chunkHeader != arena + headerOffset) { std::puts("fail: chunk header"); bad = 1; }
        unsigned long long chunkStored; std::memcpy(&chunkStored, object + 552, 8);
        if (chunkStored != chunkResult || qwordAtSetWeapon != chunkResult) { std::puts("fail: chunk store"); bad = 1; }
        if (sawArg[0] != 0 || sawArg[2] != 0 || sawSelf[1] != object + 288 || sawSelf[3] != object || sawSelf[5] != object) { std::puts("fail: call args"); bad = 1; }
        // call sequence: the creature call, the name comparisons, then the rest
        if (calls.find("cmp;") != 9) { std::puts("fail: compare start"); bad = 1; }
        const size_t lastCmp = calls.rfind("cmp;");
        const std::string tail = calls.substr(lastCmp + 4);
        std::string wantTail;
        const size_t draws = pos;
        for (size_t i = 0; i < draws; ++i) wantTail += "rand;";
        wantTail += "chunks;weapon;type;animinit;state;slowrun;";
        for (int i = 0; i < 12; ++i) wantTail += "anim;";
        wantTail += "finish;";
        if (tail != wantTail) { std::printf("fail: tail calls %s\n", tail.c_str()); bad = 1; }
        // the compared literal names, in order, stop at the first match
        const char* const order[] = {"mummytemplesoldier", "mummytso", "mummydesertshaman", "mummydsh", "mummytempleshaman", "mummytsh",
                                     "mummyelitesoldier", "mummyeso", "mummyeliteshaman", "mummyesh"};
        size_t expectedCompares = 0;
        for (size_t i = 0; i < 10; ++i) { ++expectedCompares; if (strcasecmp(k.name, order[i]) == 0) break; }
        if (compared.size() != expectedCompares) { std::puts("fail: compare count"); bad = 1; }
        else for (size_t i = 0; i < compared.size(); ++i) if (compared[i] != order[i]) { std::puts("fail: compare literal"); bad = 1; }
        // animation flags
        const struct { int id; unsigned f1; unsigned f2; int b; } wantA[12] = {
            {3, 0x3dccccccu, 0, 0}, {4, 0x3e199999u, 0, 0}, {32, 0x3e4cccccu, 0, 0}, {33, 0x3fc00000u, 0, 0}, {34, 0x3e4cccccu, 0, 0},
            {14, 0x3dccccccu, 0, 0}, {15, 0x3dccccccu, 0, 0}, {16, 0x3dccccccu, 0x3f000000u, 1}, {17, 0x3dccccccu, 0, 0},
            {18, 0x3dccccccu, 0, 0}, {19, 0x3dccccccu, 0x3f000000u, 1}, {43, 0x3e4cccccu, 0x3f4cccccu, 2}};
        if (anims.size() != 12) { std::puts("fail: anim count"); bad = 1; continue; }
        for (int i = 0; i < 12; ++i) {
            const Anim& a = anims[i];
            unsigned f1, f2, f3, f4; std::memcpy(&f1, &a.f1, 4); std::memcpy(&f2, &a.f2, 4); std::memcpy(&f3, &a.f3, 4); std::memcpy(&f4, &a.f4, 4);
            if (a.id != wantA[i].id || f1 != wantA[i].f1 || a.a != 1 || f2 != wantA[i].f2 || a.b != wantA[i].b || f3 != 0 || a.c != 0 || f4 != 0 || a.d != 0) {
                std::printf("fail: anim flags %d\n", i); bad = 1;
            }
        }
    }
    if (bad == 0) std::puts("ok");
    return bad;
}
