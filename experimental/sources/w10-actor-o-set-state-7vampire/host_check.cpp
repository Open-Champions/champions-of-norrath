// Host contract for setState__7Vampirei; not an EE input. 32-bit words are offsets from a host base;
// callees log. The authored function was also compared with the original in a private R5900 run
// (600 random objects over all states plus 90 runs of state 15 on the boundary values of its angle
// ranges; all words of the original reached except padding).
#define W10_ACTOR_O_HOST 1
#include <cstdio>
#include <cstring>
#include <string>
#include <initializer_list>
unsigned char* w10_actor_o_host_base;
#include "set_state.cpp"

static unsigned char arena[0x8000] __attribute__((aligned(16)));
static unsigned char* const obj = arena + 0x1000;
static std::string calls;
static int procResult, procNew = -1000;
static unsigned roll;
static short clearPath;
static std::string num(double v) { char b[40]; std::snprintf(b, sizeof b, "%g", v); return b; }
static unsigned off(const void* p) { return static_cast<unsigned>(static_cast<const unsigned char*>(p) - arena); }
extern "C" {
int setStateProcessing__8CreaturePi(void*, int* s) { calls += "proc" + num(*s) + ";"; if (procNew != -1000) *s = procNew; return procResult; }
void resetRouting__8Creature(void*) { calls += "reset;"; }
unsigned eRand__10GameObject(void*) { calls += "rand;"; return roll; }
void getSmartRandomDestination__8Creaturei(void* r, void*, int range) { calls += "smart" + num(range) + ";"; float p[3] = {1, 2, 3}; std::memcpy(r, p, 12); }
void animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(void*, void* h, float t, int f) {
    unsigned b; std::memcpy(&b, &t, 4);
    char buf[16]; std::snprintf(buf, sizeof buf, "%x", b);
    calls += "trans(" + num(off(h) - 0x2000) + "," + buf + "," + num(f) + ");";
}
void animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(void*, void* h, float a, float b, int f) {
    unsigned x, y; std::memcpy(&x, &a, 4); std::memcpy(&y, &b, 4);
    char buf[32]; std::snprintf(buf, sizeof buf, "%x,%x", x, y);
    calls += "shot(" + num(off(h) - 0x2000) + "," + buf + "," + num(f) + ");";
}
void animGlobalSync__FP14AnimationState(void*) { calls += "sync;"; }
short findClearPath__8CreatureG6Point3scf(void*, Point3 p, short heading, char flag, float range) {
    calls += "clear(" + num(p.x) + ":" + num(p.y) + ":" + num(p.z) + "," + num(heading) + "," + num(flag) + "," + num(range) + ");";
    return clearPath;
}
void dropTreasure__8CreatureG6Point3iffffffPc(void*, Point3 p, int a, float b, float c, float d, float e, float f, float g, char* name) {
    calls += "drop(" + num(p.x) + ":" + num(p.y) + ":" + num(p.z) + "," + num(a) + "," + num(b) + "," + num(c) + "," + num(d) + "," + num(e) + "," + num(f) + "," + num(g) + "," + (name ? name : "null") + ");";
}
}
unsigned w10_actor_o_host_call(unsigned fw, void* adjusted, int n, unsigned long a1, unsigned long) {
    calls += "v" + num(fw) + "(" + num(off(adjusted)) + "," + num(n) + "," + num(a1) + ");"; return 0;
}
static void put32(unsigned o, unsigned v) { std::memcpy(obj + o, &v, 4); }
static void putA(unsigned a, unsigned v) { std::memcpy(arena + a, &v, 4); }
static unsigned get32(unsigned o) { unsigned v; std::memcpy(&v, obj + o, 4); return v; }
static short get16(unsigned o) { short v; std::memcpy(&v, obj + o, 2); return v; }
static void reset(short current) {
    std::memset(arena, 0, sizeof arena); w10_actor_o_host_base = arena; calls.clear();
    procResult = 0; procNew = -1000; roll = 0; clearPath = 0;
    put32(36, 0x300); put32(72, 0x400); putA(0x400 + 192, 8); putA(0x400 + 196, 0x78);
    for (unsigned i = 0; i < 100; ++i) putA(0x300 + 4 * i, 0x2000 + 4 * i);       // R[offset] = 0x2000 + offset
    std::memcpy(obj + 572, &current, 2);
    float pt[3] = {4, 5, 6}; std::memcpy(obj + 16, pt, 12);
    short h = 9; std::memcpy(obj + 56, &h, 2);
    put32(808, 1000);
}
static int check(const char* label, const std::string& want) {
    if (calls == want) return 0;
    std::printf("fail %s\n  want %s\n  got  %s\n", label, want.c_str(), calls.c_str()); return 1;
}
int main() {
    int bad = 0;
    auto p = [](int n) { return "proc" + num(n) + ";"; };
    const std::string fast = "3dcccccc", slow = "3e999999";
    const std::string fastshot = fast + "," + fast + ",";
    // 0: wait = base + roll % 90 + 90
    reset(0); roll = 200; setState__7Vampirei(obj, 0);
    bad |= check("0", p(0) + "rand;trans(132," + slow + ",3);");
    if (get32(256) != 1000 + 200 % 90 + 90 || get16(572) != 0) { std::puts("fail 0 stores"); bad = 1; }
    reset(0); setState__7Vampirei(obj, 1);
    bad |= check("1", p(1) + "reset;smart300;trans(52," + slow + ",2051);sync;");
    { float x, y, z; std::memcpy(&x, obj + 272, 4); std::memcpy(&y, obj + 276, 4); std::memcpy(&z, obj + 280, 4); if (x != 1 || y != 2 || z != 3) { std::puts("fail 1 destination"); bad = 1; } }
    reset(5); setState__7Vampirei(obj, 13); bad |= check("13", p(13) + "reset;trans(52," + slow + ",2051);sync;");
    reset(5); setState__7Vampirei(obj, 3); bad |= check("3 other", p(3) + "trans(52," + fast + ",2051);sync;");
    reset(3); setState__7Vampirei(obj, 3); bad |= check("3 same", p(3) + "sync;");
    reset(0); setState__7Vampirei(obj, 5); bad |= check("5", p(5) + "trans(132," + fast + ",3);");
    reset(0); setState__7Vampirei(obj, 6); bad |= check("6", p(6) + "trans(132," + fast + ",3);shot(44," + fastshot.substr(0, fastshot.size() - 1) + ",2056);");
    if (get32(256) != 20) { std::puts("fail 6 wait"); bad = 1; }
    for (unsigned r : {0u, 1u, 2u, 3u}) {
        reset(0); roll = r; setState__7Vampirei(obj, 16);
        bad |= check("16", p(16) + "trans(124," + fast + ",3);rand;shot(" + num(28 + (r & 1) * 8) + "," + fast + "," + fast + ",2056);");
        if (get32(256) != 20) { std::puts("fail 16 wait"); bad = 1; }
    }
    reset(0); roll = 300; setState__7Vampirei(obj, 14);
    bad |= check("14", p(14) + "trans(148," + fast + ",3);rand;");
    if (get32(256) != 1000 + 300 % 240 + 240) { std::puts("fail 14 wait"); bad = 1; }
    // 8 and 9
    for (int s : {8, 9}) {
        for (short h : {0, 6, 16, 15}) {
            for (unsigned bit : {0u, 8u}) {
                for (unsigned r : {0u, 1u}) {
                    reset(h); roll = r; { unsigned short b = static_cast<unsigned short>(bit); std::memcpy(obj + 294, &b, 2); }
                    setState__7Vampirei(obj, s);
                    const bool skip = h == 16 && bit;
                    std::string want = p(s);
                    if (!skip) want += "rand;shot(" + num(108 + (r & 1) * 8) + "," + fast + "," + fast + ",2064);";
                    bad |= check("8/9", want);
                    const int final = skip ? 16 : (h == 6 && bit) ? 6 : 8;
                    if (get16(572) != final) { std::printf("fail 8/9 state s=%d h=%d bit=%u got %d want %d\n", s, h, bit, get16(572), final); bad = 1; }
                }
            }
        }
    }
    // 10
    reset(0); roll = 7; setState__7Vampirei(obj, 10);
    bad |= check("10", p(10) + "drop(4:5:6,9,36,2,1.6,0.6,1.4,0.25,vampirefang);rand;trans(" + num(60 + (7 % 3) * 8) + "," + slow + ",2054);v120(" + num(0x1000 + 8) + ",0,0);");
    if (get32(256) != 1000 || (get32(12) & 0x4000) == 0) { std::puts("fail 10 stores"); bad = 1; }
    // 15: angle difference picks the animation
    struct Row { short path; int index; };
    for (Row row : {Row{9 + 8192, 10}, Row{9 + 24575, 10}, Row{9 + 24576, 12}, Row{9 + 8191, 12}, Row{9, 12}, Row{9 - 8192, 12},
                    Row{static_cast<short>(9 - 8193), 11}, Row{static_cast<short>(9 - 24575), 11}, Row{static_cast<short>(9 - 24576), 12}}) {
        reset(0); roll = 3; clearPath = row.path; setState__7Vampirei(obj, 15);
        bad |= check("15", p(15) + "trans(132," + fast + ",3);rand;clear(4:5:6,9,1,3);shot(" + num(row.index * 8 + 4) + ",3c23d70a," + fast + ",2056);");
    }
    for (int s : {2, 4, 7, 11, 12, 17, 18, 100}) { reset(5); setState__7Vampirei(obj, s); bad |= check("none", p(s)); if (get16(572) != s) { std::puts("fail none state"); bad = 1; } }
    reset(5); procResult = 1; setState__7Vampirei(obj, 1); bad |= check("proc", p(1)); if (get16(572) != 5) { std::puts("fail proc state"); bad = 1; }
    reset(5); procNew = 5; setState__7Vampirei(obj, 100); bad |= check("changed", p(100) + "trans(132," + fast + ",3);"); if (get16(572) != 5) { std::puts("fail changed state"); bad = 1; }
    return bad;
}
