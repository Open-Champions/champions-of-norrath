// Host contract for __17UndeadKnightCloneG6Point3sPPc; not an EE input.
#define NORRATH_HOST_CHECK 1
#include "undead_knight_clone_ctor.cpp"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>

unsigned char vt_UndeadKnightClone[8], udkResourceMap[8];
char levelName[200];
int frameNm;
static std::string calls; static unsigned char* theObject;
static Point3 seenPos; static short seenType; static void* seenMap; static float seenScale; static char** seenArgs;
static unsigned char lightAtAdd[48]; static void* lightPtr;
static float setParams[5]; static int setParamsFlag; static void *animState, *animHeader, *slowObject; static int animFlags;
static unsigned char blockStorage[128];
static unsigned char headerTarget[8];
struct Ev { int a; float b; int c; float d; int e; float f; int g; float h; int i; };
static Ev events[5]; static int eventCount;

extern "C" unsigned __8CreatureG6Point3sPQ210GameObject11resourceMapfPPc(void* s, Point3 p, short t, void* m, float sc, char** a) {
    calls += "creature;"; seenPos = p; seenType = t; seenMap = m; seenScale = sc; seenArgs = a;
    unsigned char* o = static_cast<unsigned char*>(s);
    const unsigned block = low(blockStorage), header = low(headerTarget);
    std::memcpy(o + 36, &block, 4);
    std::memcpy(blockStorage + 84, &header, 4);
    return 0;
}
extern "C" void lightAdd__FP5Light(void* l) { calls += "light;"; lightPtr = l; std::memcpy(lightAtAdd, l, 48); }
extern "C" void setParameters__8Creaturefffffi(void*, float a, float b, float c, float d, float e, int flag) {
    calls += "params;"; setParams[0] = a; setParams[1] = b; setParams[2] = c; setParams[3] = d; setParams[4] = e; setParamsFlag = flag;
}
extern "C" void animInitState__FP14AnimationState(void* s) { calls += "animinit;"; animState = s; }
extern "C" void animAdd__FP14AnimationStateP15AnimationHeaderi(void* s, void* h, int f) { calls += "animadd;"; animHeader = h; animFlags = f; (void)s; }
extern "C" void objectAddToSlowRunList__FP10GameObject(void* o) { calls += "slow;"; slowObject = o; }
extern "C" void setAnimEventFlags__8Creatureififififi(void*, int a, float b, int c, float d, int e, float f, int g, float h, int i) {
    calls += "event;"; events[eventCount++] = Ev{a, b, c, d, e, f, g, h, i};
}
extern "C" unsigned finishConstruction__8Creature(void*) { calls += "finish;"; return 0x777; }

extern "C" void* host_resolve(unsigned word) {
    if (word == low(blockStorage)) return blockStorage;
    if (word == low(headerTarget)) return headerTarget;
    std::puts("fail: unexpected address word"); std::exit(2);
}
static float fb(unsigned bits) { float f; std::memcpy(&f, &bits, 4); return f; }
static unsigned fbits(float f) { unsigned u; std::memcpy(&u, &f, 4); return u; }

static int run(const char* level, short type) {
    int bad = 0;
    alignas(16) unsigned char raw[2800];
    std::memset(raw, 0xee, sizeof raw);
    theObject = raw; calls.clear(); eventCount = 0;
    std::memset(levelName, 0, sizeof levelName); std::strcpy(levelName + 81, level);
    frameNm = 9999;
    std::memset(raw + 160, 0, 8);
    const unsigned long long before = 0x0123000000000001ull;
    std::memcpy(raw + 160, &before, 8);
    char* argList[1] = {nullptr};
    Point3 given{1.5f, -2.5f, 3.25f};
    const unsigned result = __17UndeadKnightCloneG6Point3sPPc(raw, given, type, argList);
    const std::string want = "creature;light;params;animinit;animadd;slow;event;event;event;event;event;finish;";
    if (calls != want) { std::printf("fail: call order %s\n", calls.c_str()); bad = 1; }
    if (result != 0x777u) { std::puts("fail: result"); bad = 1; }
    if (seenPos.x != 1.5f || seenPos.y != -2.5f || seenPos.z != 3.25f || seenType != type || seenMap != udkResourceMap || seenScale != 10.0f || seenArgs != argList) { std::puts("fail: creature arguments"); bad = 1; }
    unsigned w72; std::memcpy(&w72, raw + 72, 4);
    if (w72 != low(vt_UndeadKnightClone)) { std::puts("fail: table"); bad = 1; }
    // light record
    unsigned char want_light[48]; std::memset(want_light, 0xee, sizeof want_light);
    want_light[8] = 0; want_light[9] = 0;
    const short hs[8] = {255, 128, 128, -16, 255, 128, 128, -16};
    for (int i = 0; i < 8; ++i) std::memcpy(want_light + 10 + 2 * i, &hs[i], 2);
    const float f28 = 120.0f, f32 = fb(0x3891a2b3u); const unsigned w44 = 0;
    std::memcpy(want_light + 28, &f28, 4); std::memcpy(want_light + 32, &f32, 4);
    std::memcpy(want_light + 36, &f28, 4); std::memcpy(want_light + 40, &f32, 4); std::memcpy(want_light + 44, &w44, 4);
    if (lightPtr != raw + 2496) { std::puts("fail: light address"); bad = 1; }
    for (int i = 0; i < 48; ++i) {
        const bool untouched = i < 8 || i == 26 || i == 27;
        if (untouched ? lightAtAdd[i] != 0xee : lightAtAdd[i] != want_light[i]) { std::printf("fail: light byte %d\n", i); bad = 1; }
    }
    Point3 p2544; std::memcpy(&p2544, raw + 2544, 12);
    if (p2544.x != 1.5f || p2544.y != -2.5f || p2544.z != 3.25f) { std::puts("fail: point at +2544"); bad = 1; }
    // description-like record
    short h;
    for (int k = 0; k < 4; ++k) { std::memcpy(&h, raw + 2684 - 8 * k, 2); if (h != -1) { std::printf("fail: half %d\n", k); bad = 1; } }
    if (raw[2656] != 0xff || raw[2657] != 0xff) { std::puts("fail: bytes +2656/+2657"); bad = 1; }
    for (int k = 0; k < 4; ++k) {
        std::memcpy(&h, raw + 2640 + 2 * k, 2); if (h != 0) { std::puts("fail: zero half low"); bad = 1; }
        std::memcpy(&h, raw + 2648 + 2 * k, 2); if (h != 0) { std::puts("fail: zero half high"); bad = 1; }
    }
    // scalars
    unsigned long long q160; std::memcpy(&q160, raw + 160, 8);
    if (q160 != (before | 0x0000022020020000ull)) { std::puts("fail: doubleword +160"); bad = 1; }
    unsigned w;
    if (raw[40] != 20 || raw[172] != 16) { std::puts("fail: bytes"); bad = 1; }
    struct F { unsigned off; unsigned bits; } fs[] = {{28, fbits(20.0f)}, {168, 0x3f999999u}, {88, 0xbe4ccccc}, {96, 0x3f199999u}, {104, 0x3e999999u}, {224, 0x3fa66666u}, {100, fbits(1.0f)}, {248, 9999u},
                                                      {176, 0}, {192, 0}, {236, 0}, {2696, 1}, {2556, 1}};
    for (const F& f : fs) { std::memcpy(&w, raw + f.off, 4); if (w != f.bits) { std::printf("fail: word +%u\n", f.off); bad = 1; } }
    std::memcpy(&h, raw + 572, 2); if (h != 0) { std::puts("fail: half +572"); bad = 1; }
    // parameters
    const bool same = std::strcmp(level, "battle3") == 0;
    const float want5 = same ? fb(0x3c23d70au) : fb(0x3f999999u);
    if (setParams[0] != fb(0x3fb33333u) || setParams[1] != fb(0x3f999999u) || setParams[2] != 1.0f || setParams[3] != 1.0f || setParams[4] != want5 || setParamsFlag != 1) { std::puts("fail: setParameters"); bad = 1; }
    if (animState != raw + 288 || animHeader != headerTarget || animFlags != 1 || slowObject != raw) { std::puts("fail: animation calls"); bad = 1; }
    const Ev want_ev[5] = {{3, fb(0x3e4ccccc), 1, fb(0x3f199999u), 1, 0, 0, 0, 0}, {4, fb(0x3e4ccccc), 1, fb(0x3f199999u), 2, 1.0f, 2, 0, 0},
                           {5, fb(0x3e999999u), 4, 0, 0, 0, 0, 0, 0}, {6, fb(0x3e999999u), 1, fb(0x3f4ccccc), 1, fb(0x3fcccccc), 4, 0, 0},
                           {14, 0.5f, 1, 0, 0, 0, 0, 0, 0}};
    if (eventCount != 5 || std::memcmp(events, want_ev, sizeof events) != 0) { std::puts("fail: anim event flags"); bad = 1; }
    for (unsigned i = 2700; i < sizeof raw; ++i) if (raw[i] != 0xee) { std::puts("fail: tail written"); bad = 1; break; }
    return bad;
}
int main() {
    int bad = 0;
    bad |= run("battle3", 5);
    bad |= run("other", -3);
    bad |= run("battle", 0x7fff);
    bad |= run("battle33", -32768);
    return bad;
}
