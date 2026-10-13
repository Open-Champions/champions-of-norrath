// Host contract for setState__12UndeadKnighti; not an EE input. 32-bit words are offsets from a host
// base; callees log. The authored function was also compared with the original in a private R5900 run
// (1200 random objects over states 0..20 and out-of-range values; every original word reached except
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
float icos__Fi(int a) { calls += "cos" + num(a) + ";"; return 0.5f; }
float isin__Fi(int a) { calls += "sin" + num(a) + ";"; return -0.25f; }
void* blockAlloc__Fi(int size) { calls += "alloc" + num(size) + ";"; return allocAnswer; }
void __17UndeadKnightCloneG6Point3sPPc(void* self, Point3 p, short a, char** args) {
    calls += "clone(" + num(self == allocAnswer) + "," + pt(p) + "," + num(a) + "," + (args ? "args" : "null") + ");";
}
void* listOnewayLink__FP8ListHeadT0Pv(void* head, void* second, void* value) {
    calls += "link(" + num(off(head) - 0x1000) + "," + num(second == allocAnswer) + "," + num(value == allocAnswer) + ");"; return nullptr;
}
void dropTreasure__8CreatureG6Point3iffffffPc(void*, Point3 p, int a, float b, float c, float d, float e, float f, float g, char* name) {
    calls += "drop(" + pt(p) + "," + num(a) + "," + num(b) + "," + num(c) + "," + fbits(d) + "," + fbits(e) + "," + num(f) + "," + num(g) + "," + (name ? name : "null") + ");";
}
void gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(void* t, void* s, float a, float b, int c, int d, int e, int f, void* desc, bool flag) {
    calls += "attack(" + num(off(t)) + "," + num(off(s) - 0x1000) + "," + num(a) + "," + num(b) + "," + num(c) + "," + num(d) + "," + num(e) + "," + num(f) + "," + (desc ? "desc" : "null") + "," + num(flag) + ");";
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
    put32(36, 0x300); put32(72, 0x800); putA(0x800 + 184, 0); putA(0x800 + 188, 0x77); putA(0x800 + 192, 8); putA(0x800 + 196, 0x78);
    for (unsigned i = 0; i < 200; ++i) putA(0x300 + 4 * i, 0x2000 + 4 * i);       // R[offset] = 0x2000 + offset
    std::memcpy(obj + 572, &current, 2);
    float p[3] = {4, 5, 6}; std::memcpy(obj + 16, p, 12);
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
    const std::string fast = "3dcccccc";
    const std::string tr3 = "," + fast + ",3);", shot = "," + fast + "," + fast + ",2056);";
    // bit 9 of the doubleword at +160 is cleared on every call that passes the first step
    reset(0); put32(160, 0xffffffffu); put32(164, 0xffffffffu); setState__12UndeadKnighti(obj, 100);
    if (get32(160) != 0xfffffdffu || get32(164) != 0xffffffffu) { std::puts("fail bit 9 clear"); bad = 1; }
    reset(0); put32(160, 0xffffffffu); procResult = 1; setState__12UndeadKnighti(obj, 100);
    if (get32(160) != 0xffffffffu) { std::puts("fail bit 9 clear after refusal"); bad = 1; }
    reset(0); roll = 250; setState__12UndeadKnighti(obj, 0);
    bad |= check("0", p(0) + "rand;trans(84" + tr3);
    if (get32(256) != 1000 + 250 % 120) { std::puts("fail 0 wait"); bad = 1; }
    reset(0); setState__12UndeadKnighti(obj, 1);
    bad |= check("1", p(1) + "reset;smart200;trans(108," + fast + ",2051);sync;");
    { float x, y, z; std::memcpy(&x, obj + 272, 4); std::memcpy(&y, obj + 276, 4); std::memcpy(&z, obj + 280, 4); if (x != 1 || y != 2 || z != 3) { std::puts("fail 1 destination"); bad = 1; } }
    reset(0); setState__12UndeadKnighti(obj, 3); bad |= check("3", p(3) + "reset;trans(108," + fast + ",2051);sync;");
    reset(0); setState__12UndeadKnighti(obj, 5); bad |= check("5", p(5) + "trans(60" + tr3);
    reset(0); setState__12UndeadKnighti(obj, 13); bad |= check("13", p(13) + "shot(116" + shot);
    struct Row { int state, trans, shot; };
    for (Row row : {Row{14, 60, 28}, Row{15, 60, 36}, Row{16, 60, 44}, Row{17, 60, 52}}) {
        reset(0); roll = 47; setState__12UndeadKnighti(obj, row.state);
        bad |= check("14..17", p(row.state) + "trans(" + num(row.trans) + tr3 + "shot(" + num(row.shot) + shot + "rand;");
        if (get32(256) != 47 % 20) { std::puts("fail wait"); bad = 1; }
    }
    reset(0); put32(160, 0); setState__12UndeadKnighti(obj, 18); bad |= check("18", p(18) + "trans(60" + tr3); if (get32(160) != 0x200u) { std::puts("fail 18 bit"); bad = 1; }
    reset(0); setState__12UndeadKnighti(obj, 20); bad |= check("20", p(20) + "trans(60" + tr3 + "shot(100" + shot);
    // 19: the point is the target plus 36 times the trig pair (28:11:30 from 10:20:30), or the object position plus 120 times it
    // (64:-25:6 from 4:5:6); a block is allocated and linked
    for (int enemy : {0, 1}) {
        for (void* alloc : {static_cast<void*>(nullptr), static_cast<void*>(arena + 0x3000)}) {
            reset(0); allocAnswer = alloc;
            if (enemy) { put32(576, 0x500); putA(0x500 + 12, 0x600); float q[3] = {10, 20, 30}; std::memcpy(arena + 0x600 + 16, q, 12); }
            setState__12UndeadKnighti(obj, 19);
            std::string want = p(19) + (enemy ? "cos-32758;sin-32758;" : "cos9;sin9;") + "alloc2704;";
            if (alloc) want += "clone(1," + std::string(enemy ? "28:11:30" : "64:-25:6") + ",9,null);link(964,1,1);";
            want += "trans(84" + tr3 + "shot(68" + shot;
            bad |= check("19", want);
        }
    }
    // 8
    for (short h : {0, 6, 9, 5}) {
        for (unsigned bit : {0u, 8u}) {
            for (unsigned w960 : {0u, 1u}) {
                reset(h); put32(960, w960); { unsigned short b = static_cast<unsigned short>(bit); std::memcpy(obj + 294, &b, 2); }
                setState__12UndeadKnighti(obj, 8);
                const bool keep = (h == 6 && bit) || h == 9 || w960;
                bad |= check("8", p(8) + (keep ? "" : "shot(76," + fast + "," + fast + ",16);"));
                if (get16(572) != (keep ? h : 8)) { std::puts("fail 8 state"); bad = 1; }
            }
        }
    }
    for (unsigned w960 : {0u, 1u}) {
        reset(4); put32(960, w960); setState__12UndeadKnighti(obj, 9);
        bad |= check("9", p(9) + (w960 ? "" : "trans(84,3e999999,3);"));
        if (get16(572) != (w960 ? 4 : 9)) { std::puts("fail 9 state"); bad = 1; }
    }
    // 10
    for (unsigned w964 : {0u, 0x500u}) {
        reset(0); put32(964, w964); putA(0x500 + 12, 0x700);
        setState__12UndeadKnighti(obj, 10);
        std::string want = p(10) + "drop(4:5:6,9,36,2,3fe66666,3f199999,2,0.25,null);trans(124," + fast + ",6);";
        if (w964) want += "attack(1792,0,5000,5000,10000,0,0,64,null,0);";
        want += "v120(8,0,0);";
        bad |= check("10", want);
    }
    // everything else calls the table entry at +184 with 3 and keeps the state
    for (int s : {2, 4, 6, 7, 11, 12, 21, 100, -1}) {
        reset(5); setState__12UndeadKnighti(obj, s);
        bad |= check("default", p(s) + "v119(0,1,3);");
        if (get16(572) != s) { std::puts("fail default state"); bad = 1; }
    }
    reset(5); procResult = 1; setState__12UndeadKnighti(obj, 1); bad |= check("proc", p(1)); if (get16(572) != 5) { std::puts("fail proc state"); bad = 1; }
    reset(5); procNew = 5; setState__12UndeadKnighti(obj, 100); bad |= check("changed", p(100) + "trans(60" + tr3); if (get16(572) != 5) { std::puts("fail changed state"); bad = 1; }
    return bad;
}
