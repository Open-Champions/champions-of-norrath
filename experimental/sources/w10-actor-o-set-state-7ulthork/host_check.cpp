// Host contract for setState__7Ulthorki; not an EE input. 32-bit words are offsets from a host base;
// callees log. The authored function was also compared with the original in a private R5900 run
// (900 random objects over states 0..14 and out-of-range values; every original word reached except
// padding).
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
static void* allocAnswer;
static std::string num(double v) { char b[40]; std::snprintf(b, sizeof b, "%g", v); return b; }
static std::string hex(unsigned v) { char b[16]; std::snprintf(b, sizeof b, "%x", v); return b; }
static std::string fbits(float f) { unsigned u; std::memcpy(&u, &f, 4); return hex(u); }
static unsigned off(const void* p) { return static_cast<unsigned>(static_cast<const unsigned char*>(p) - arena); }
static std::string pt(const Point3& p) { return num(p.x) + ":" + num(p.y) + ":" + num(p.z); }
extern "C" {
int setStateProcessing__8CreaturePi(void*, int* s) { calls += "proc" + num(*s) + ";"; if (procNew != -1000) *s = procNew; return procResult; }
void resetRouting__8Creature(void*) { calls += "reset;"; }
unsigned eRand__10GameObject(void*) { calls += "rand;"; return roll; }
void getSmartRandomDestination__8Creaturei(void* r, void*, int range) { calls += "smart" + num(range) + ";"; float p[3] = {1, 2, 3}; std::memcpy(r, p, 12); }
void animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(void*, void* h, float t, int f) {
    calls += "trans(" + num(off(h) - 0x2000) + "," + fbits(t) + "," + num(f) + ");";
}
void animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(void*, void* h, float a, float b, int f) {
    calls += "shot(" + num(off(h) - 0x2000) + "," + fbits(a) + "," + fbits(b) + "," + num(f) + ");";
}
void animGlobalSync__FP14AnimationState(void*) { calls += "sync;"; }
void setParameters__8Creaturefffffi(void*, float a, float b, float c, float d, float e, int f) {
    calls += "params(" + fbits(a) + "," + fbits(b) + "," + num(c) + "," + num(d) + "," + num(e) + "," + num(f) + ");";
}
void objectAddToRunList__FP10GameObject(void*) { calls += "addrun;"; }
void dropTreasure__8CreatureG6Point3iffffffPc(void*, Point3 p, int a, float b, float c, float d, float e, float f, float g, char* name) {
    calls += "drop(" + pt(p) + "," + num(a) + "," + num(b) + "," + num(c) + "," + fbits(d) + "," + fbits(e) + "," + num(f) + "," + num(g) + "," + (name ? name : "null") + ");";
}
void modelGetCharAttachmentPos__FP12_modelHeaderG6Point3sP14AnimationStateiP6Point3T5f(
    void* m, Point3 p, short yaw, void* an, int index, Point3* first, Point3* second, float scale) {
    calls += "pos(" + num(off(m)) + "," + pt(p) + "," + num(yaw) + "," + num(off(an) - 0x1000) + "," + num(index) + "," + pt(*first) + "," + num(scale) + ");";
    *first = Point3{7, 8, 9}; *second = Point3{0, 0, 0};
}
void* blockAlloc__Fi(int size) { calls += "alloc" + num(size) + ";"; return allocAnswer; }
void __6PhPropG6Point3PcN22liPQ24item11descriptionf(void* self, Point3 p, char* a, char* b, char* c, long d, int e, void* desc, float scale) {
    calls += "prop(" + num(self == allocAnswer) + "," + pt(p) + "," + num(off(a)) + "," + num(off(b)) + "," + num(off(c)) + "," + num(d) + "," + num(e) + "," + (desc ? "desc" : "null") + "," + num(scale) + ");";
}
}
unsigned w10_actor_o_host_call(unsigned fw, void* adjusted, int n, unsigned long a1, unsigned long) {
    calls += "v" + num(fw) + "(" + num(off(adjusted) - 0x1000) + "," + num(n) + "," + num(a1) + ");"; return 0;
}
static void put32(unsigned o, unsigned v) { std::memcpy(obj + o, &v, 4); }
static void putA(unsigned a, unsigned v) { std::memcpy(arena + a, &v, 4); }
static unsigned get32(unsigned o) { unsigned v; std::memcpy(&v, obj + o, 4); return v; }
static short get16(unsigned o) { short v; std::memcpy(&v, obj + o, 2); return v; }
static void reset(short current) {
    std::memset(arena, 0, sizeof arena); w10_actor_o_host_base = arena; calls.clear();
    procResult = 0; procNew = -1000; roll = 0; allocAnswer = arena + 0x3000;
    put32(36, 0x300); put32(72, 0x800); putA(0x800 + 192, 8); putA(0x800 + 196, 0x78);
    for (unsigned i = 0; i < 200; ++i) putA(0x300 + 4 * i, 0x2000 + 4 * i);       // R[offset] = 0x2000 + offset
    std::memcpy(obj + 572, &current, 2);
    float p[3] = {4, 5, 6}; std::memcpy(obj + 16, p, 12);
    short h = 9; std::memcpy(obj + 56, &h, 2);
    put32(808, 1000);
    float s = 2.5f; std::memcpy(obj + 224, &s, 4);
}
static int check(const char* label, const std::string& want) {
    if (calls == want) return 0;
    std::printf("fail %s\n  want %s\n  got  %s\n", label, want.c_str(), calls.c_str()); return 1;
}
int main() {
    int bad = 0;
    auto p = [](int n) { return "proc" + num(n) + ";"; };
    const std::string slow = "3e999999", fast = "3dcccccc";
    // 0 and 14
    reset(0); roll = 250; setState__7Ulthorki(obj, 0);
    bad |= check("0", p(0) + "rand;trans(116," + slow + ",3);");
    if (get32(256) != 1000 + 250 % 120 || get16(572) != 0) { std::puts("fail 0 stores"); bad = 1; }
    for (int b1104 : {2, 0}) {
        for (unsigned r : {0u, 1u, 250u}) {
            reset(5); roll = r; obj[1104] = static_cast<unsigned char>(b1104); put32(12, 0xffffffffu); put32(160, 0); put32(164, 0);
            setState__7Ulthorki(obj, 14);
            const std::string want = p(14) + "params(3fb33333,3f999999,1,1,1,1);" + (b1104 == 2 ? "" : "rand;") + "addrun;rand;trans(116," + slow + ",3);";
            bad |= check("14", want);
            if (get32(12) != 0xfffffff6u || get32(164) != 1 || get32(960) != (b1104 == 2 ? 23u : 19u + 2 * (r & 1)) || get16(572) != 0) { std::puts("fail 14 stores"); bad = 1; }
            short expect[] = {0, 128, 255, 50, 0, 128, 255, 150};
            for (int i = 0; i < 8; ++i) if (get16(140 + 2 * i) != expect[i]) { std::puts("fail 14 halfwords"); bad = 1; }
        }
    }
    reset(0); setState__7Ulthorki(obj, 1);
    bad |= check("1", p(1) + "reset;smart200;rand;trans(140," + slow + ",2051);sync;");
    if (get32(256) != 1300) { std::puts("fail 1 wait"); bad = 1; }
    { float x, y, z; std::memcpy(&x, obj + 272, 4); std::memcpy(&y, obj + 276, 4); std::memcpy(&z, obj + 280, 4); if (x != 1 || y != 2 || z != 3) { std::puts("fail 1 destination"); bad = 1; } }
    for (short h : {3, 6, 5, 0}) {
        reset(h); setState__7Ulthorki(obj, 3);
        bad |= check("3", p(3) + "reset;" + ((h == 3 || h == 6) ? "" : "trans(132," + fast + ",2051);sync;"));
    }
    reset(0); setState__7Ulthorki(obj, 5); bad |= check("5", p(5) + "trans(124," + fast + ",3);");
    for (unsigned r : {0u, 1u, 2u, 3u, 5u}) {
        reset(0); roll = r; setState__7Ulthorki(obj, 6);
        bad |= check("6", p(6) + "rand;shot(" + num(44 + (r % 3) * 8) + "," + fast + "," + fast + ",8);");
    }
    reset(0); roll = 90; setState__7Ulthorki(obj, 13);
    bad |= check("13", p(13) + "trans(124," + fast + ",3);shot(148," + fast + "," + fast + ",8);rand;");
    { const float w = static_cast<float>(1180) + static_cast<float>(90) * (1.0f / 65536.0f) * 180.0f; if (static_cast<int>(get32(256)) != static_cast<int>(w)) { std::puts("fail 13 wait"); bad = 1; } }
    // 8
    for (short h : {0, 6, 9, 5}) {
        for (unsigned bit : {0u, 8u}) {
            for (unsigned r : {0u, 3u, 7u}) {
                reset(h); roll = r; { unsigned short b = static_cast<unsigned short>(bit); std::memcpy(obj + 294, &b, 2); }
                setState__7Ulthorki(obj, 8);
                const bool keep = (h == 6 && bit) || h == 9;
                bad |= check("8", p(8) + (keep ? "" : "rand;shot(" + num(84 + (r & 3) * 8) + "," + fast + "," + fast + ",16);"));
                if (get16(572) != (keep ? h : 8)) { std::puts("fail 8 state"); bad = 1; }
            }
        }
    }
    reset(0); setState__7Ulthorki(obj, 9); bad |= check("9", p(9) + "trans(116," + slow + ",3);");
    // 10
    for (unsigned d160 : {0u, 1u}) {
        for (unsigned w12 : {0u, 2u}) {
            for (unsigned w960 : {0u, 3u}) {
                for (unsigned r : {0u, 1u}) {
                    for (void* alloc : {static_cast<void*>(nullptr), static_cast<void*>(arena + 0x3000)}) {
                        reset(0); roll = r; put32(164, d160); put32(12, w12); put32(960, w960); allocAnswer = alloc;
                        setState__7Ulthorki(obj, 10);
                        std::string want = p(10);
                        if (!d160) want += "drop(4:5:6,9,36,2,3fe66666,3f199999,2,0.25,null);";
                        want += "rand;trans(" + num(68 + 8 * (r & 1)) + "," + slow + ",6);";
                        if (!w12 && w960) {
                            want += "pos(8204,4:5:6,-16375,288,0,4:5:6,1);alloc584;";
                            if (alloc) want += "prop(1,7:8:9," + num(0x2000) + "," + num(0x2000 + w960 * 8) + "," + num(0x2000 + w960 * 8 + 8) + ",1,16641,null,2.5);";
                        }
                        want += "v120(8,0,0);";
                        bad |= check("10", want);
                        if (get32(960) != ((!w12 && w960) ? 0u : w960)) { std::puts("fail 10 word 960"); bad = 1; }
                    }
                }
            }
        }
    }
    for (int s : {2, 4, 7, 11, 12, 15, 16, 100, -1}) { reset(5); setState__7Ulthorki(obj, s); bad |= check("none", p(s)); if (get16(572) != s) { std::puts("fail none state"); bad = 1; } }
    reset(5); procResult = 1; setState__7Ulthorki(obj, 1); bad |= check("proc", p(1)); if (get16(572) != 5) { std::puts("fail proc state"); bad = 1; }
    reset(5); procNew = 5; setState__7Ulthorki(obj, 100); bad |= check("changed", p(100) + "trans(124," + fast + ",3);"); if (get16(572) != 5) { std::puts("fail changed state"); bad = 1; }
    return bad;
}
