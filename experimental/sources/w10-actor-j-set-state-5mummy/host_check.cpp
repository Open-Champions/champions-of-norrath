// Host contract for setState__5Mummyi; not an EE input. 32-bit words are offsets from a host base;
// callees log. The authored function was also compared with the original in a private R5900 run
// (every state -7..20, with the word at +960 from 0 to 6 and 9).
#define W10_ACTOR_J_HOST 1
#include <cstdio>
#include <cstring>
#include <string>
unsigned char* w10_actor_j_host_base;
#include "set_state.cpp"

static unsigned char arena[0x8000] __attribute__((aligned(16)));
static unsigned char* const obj = arena + 0x1000;
static std::string calls;
static int procResult, procNew = -1000;
static unsigned rollValue, rollValue2;
static int rollCount;
static std::string num(double v) { char b[40]; std::snprintf(b, sizeof b, "%g", v); return b; }
static unsigned off(const void* p) { return static_cast<unsigned>(static_cast<const unsigned char*>(p) - arena); }
extern "C" {
int setStateProcessing__8CreaturePi(void*, int* s) { calls += "proc" + num(*s) + ";"; if (procNew != -1000) *s = procNew; return procResult; }
void terminateSpell__8CreatureRC9type_info(void*, const void* t) { calls += std::string("term") + (t == arena + 0x7000 ? "ok" : "bad") + ";"; }
void* __tf15SpellConeOfFire() { calls += "tf;"; return arena + 0x7000; }
void resetRouting__8Creature(void*) { calls += "reset;"; }
void getSmartRandomDestination__8Creaturei(void* r, void*, int range) { calls += "smart" + num(range) + ";"; float p[3] = {1, 2, 3}; std::memcpy(r, p, 12); }
unsigned eRand__10GameObject(void*) { calls += "rand;"; return rollCount++ ? rollValue2 : rollValue; }
void initType__5Mummyb(void*, bool f) { calls += "init" + num(f) + ";"; }
void objectAddToRunList__FP10GameObject(void*) { calls += "runlist;"; }
void objectAddToDeleteList__FP10GameObject(void*) { calls += "dellist;"; }
void animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(void* s, void* h, float t, int f) {
    calls += "trans(" + num(off(h) - 0x2000) + "," + num(t) + "," + num(f) + ");";
}
void animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(void* s, void* h, float a, float b, int f) {
    calls += "shot(" + num(off(h) - 0x2000) + "," + num(a) + "," + num(b) + "," + num(f) + ");";
}
void animGlobalSync__FP14AnimationState(void*) { calls += "sync;"; }
void fxCreatureToDust__FP12_modelHeaderP14AnimationStateG6Point3sff(void* m, void*, Point3 p, short a, float b, float c) {
    calls += "dust(" + num(off(m) - 0x2000) + "," + num(p.x) + ":" + num(p.y) + ":" + num(p.z) + "," + num(a) + "," + num(b) + "," + num(c) + ");";
}
void dropTreasure__8CreatureG6Point3iffffffPc(void*, Point3 p, int a, float b, float c, float d, float e, float f, float g, char* name) {
    calls += "drop(" + num(p.x) + ":" + num(p.y) + ":" + num(p.z) + "," + num(a) + "," + num(b) + "," + num(c) + "," + num(d) + "," + num(e) + "," + num(f) + "," + num(g) + "," + name + ");";
}
void dropWeapon__5Mummy(void*) { calls += "dropw;"; }
}
unsigned w10_actor_j_host_call(unsigned fw, void* adjusted, int n, unsigned long, unsigned long) {
    calls += "v" + num(fw) + "(" + num(off(adjusted)) + "," + num(n) + ");"; return 0;
}
static void put32(unsigned o, unsigned v) { std::memcpy(obj + o, &v, 4); }
static void putA(unsigned a, unsigned v) { std::memcpy(arena + a, &v, 4); }
static unsigned get32(unsigned o) { unsigned v; std::memcpy(&v, obj + o, 4); return v; }
static unsigned long long get64(unsigned o) { unsigned long long v; std::memcpy(&v, obj + o, 8); return v; }
static short get16(unsigned o) { short v; std::memcpy(&v, obj + o, 2); return v; }
static void reset(short current, unsigned kind) {
    std::memset(arena, 0, sizeof arena); w10_actor_j_host_base = arena; calls.clear();
    procResult = 0; procNew = -1000; rollValue = 1000; rollValue2 = 0x8000; rollCount = 0;
    put32(36, 0x300); put32(72, 0x400); putA(0x400 + 192, 8); putA(0x400 + 196, 0x77);
    for (unsigned i = 0; i < 100; ++i) putA(0x300 + 4 * i, 0x2000 + 4 * i);       // R[offset] = 0x2000 + offset
    std::memcpy(obj + 572, &current, 2);
    put32(808, 5000); put32(960, kind);
    float pt[3] = {4, 5, 6}; std::memcpy(obj + 16, pt, 12);
    short h = 9; std::memcpy(obj + 56, &h, 2);
}
static int check(const char* label, const std::string& want) {
    if (calls == want) return 0;
    std::printf("fail %s\n  want %s\n  got  %s\n", label, want.c_str(), calls.c_str()); return 1;
}
int main() {
    int bad = 0;
    const std::string head = "term";
    const std::string pre = "proc";
    auto prefix = [](int n) { return "proc" + num(n) + ";tf;termok;"; };
    // State 0: the header offset depends on the kind word (0 or 2: 84, 4: 204, other: 324).
    for (auto c : {std::pair<unsigned, int>{0, 84}, {2, 84}, {4, 204}, {1, 324}, {7, 324}}) {
        reset(0, c.first); setState__5Mummyi(obj, 0);
        bad |= check("0", prefix(0) + "trans(" + num(c.second) + ",0.1,3);rand;");
        if (get32(256) != 5000 + 1000 % 120 || get32(1104) != 0) { std::puts("fail 0 stores"); bad = 1; }
    }
    // State 16: initialisation, then the state 0 steps; n becomes 0.
    reset(2, 4); put32(12, 0xffffffffu); put32(964, 7); put32(160, 0); setState__5Mummyi(obj, 16);
    bad |= check("16", prefix(16) + "init0;runlist;trans(204,0.1,3);rand;");
    if (get32(12) != 0xfffffff6u || get32(964) != 46 || get64(160) != 0x100000000ull || get16(146) != 50 || get16(150) != 128 || get16(152) != 255 || get16(154) != 150 ||
        get16(140) != 0 || get16(142) != 128 || get16(144) != 255 || get16(148) != 0 || get16(572) != 0) { std::puts("fail 16 stores"); bad = 1; }
    reset(2, 7); put32(964, 7); setState__5Mummyi(obj, 16); if (get32(964) != 7) { std::puts("fail 16 keeps +964"); bad = 1; }
    for (unsigned k = 0; k < 6; ++k) { reset(2, k); setState__5Mummyi(obj, 16); const unsigned want[6] = {44, 48, 44, 48, 46, 50}; if (get32(964) != want[k]) { std::puts("fail 16 table"); bad = 1; } }
    reset(0, 4); setState__5Mummyi(obj, 1);
    bad |= check("1", prefix(1) + "reset;smart200;trans(252,0.1,2051);sync;");
    if (get32(272) == 0 || get16(572) != 1) { std::puts("fail 1 stores"); bad = 1; }
    reset(0, 0); setState__5Mummyi(obj, 17); bad |= check("17", prefix(17) + "reset;trans(92,0.1,2051);sync;");
    reset(0, 9); setState__5Mummyi(obj, 17); bad |= check("17 other", prefix(17) + "reset;trans(332,0.1,2051);sync;");
    reset(2, 2); setState__5Mummyi(obj, 3); bad |= check("3 kind 2", prefix(3) + "reset;trans(100,0.1,2051);sync;");
    reset(6, 0); setState__5Mummyi(obj, 3); bad |= check("3 from 6", prefix(3) + "reset;"); if (get16(572) != 3) bad = 1;
    reset(14, 0); setState__5Mummyi(obj, 3); bad |= check("3 from 14", prefix(3) + "reset;"); if (get16(572) != 14) { std::puts("fail 3 keeps 14"); bad = 1; }
    reset(0, 4); setState__5Mummyi(obj, 5); bad |= check("5", prefix(5) + "trans(164,0.1,3);sync;rand;"); if (get32(256) != 5000 + 1000 % 120 + 60) bad = 1;
    reset(0, 0); setState__5Mummyi(obj, 6); bad |= check("6 a", prefix(6) + "rand;shot(28,0.1,0.1,2056);");
    reset(0, 5); rollValue = 5; setState__5Mummyi(obj, 6); bad |= check("6 b", prefix(6) + "rand;shot(132,0.1,0.1,2056);");
    reset(0, 0); rollValue = 3; setState__5Mummyi(obj, 6); bad |= check("6 odd", prefix(6) + "rand;shot(36,0.1,0.1,2056);");
    reset(6, 0); { short b = 8; std::memcpy(obj + 294, &b, 2); } setState__5Mummyi(obj, 8); bad |= check("8 hold", prefix(8)); if (get16(572) != 6) bad = 1;
    reset(14, 0); { short b = 8; std::memcpy(obj + 294, &b, 2); } setState__5Mummyi(obj, 8); bad |= check("8 hold 14", prefix(8)); if (get16(572) != 14) bad = 1;
    reset(9, 0); setState__5Mummyi(obj, 8); bad |= check("8 hold 9", prefix(8)); if (get16(572) != 9) bad = 1;
    reset(6, 4); setState__5Mummyi(obj, 8); bad |= check("8 shot", prefix(8) + "shot(196,0.1,0.1,16);");
    reset(0, 0); setState__5Mummyi(obj, 9); bad |= check("9", prefix(9) + "trans(84,0.3,3);");
    reset(0, 0); rollValue = 2; setState__5Mummyi(obj, 14); bad |= check("14", prefix(14) + "rand;shot(156,0.1,0.1,2056);");
    reset(0, 0); rollValue = 4; setState__5Mummyi(obj, 13);
    bad |= check("13", prefix(13) + "trans(84,0.1,3);rand;shot(268,0.1,0.1,2056);rand;");
    if (get32(256) != static_cast<unsigned>(static_cast<int>(0x8000 * (1.0f / 65536.0f) * 120.0f) + 240)) { std::puts("fail 13 store"); bad = 1; }
    reset(0, 0); setState__5Mummyi(obj, 15); bad |= check("15", prefix(15) + "trans(84,0.1,3);shot(348,0.1,0.1,2056);"); if (get32(1112) != 5600) bad = 1;
    // State 10: dust branch, drop branch (bit 32 set skips the drop), then transition by kind.
    reset(0, 0); put32(160, 0x01000000u); put32(12, 0x10); float f224 = 1.5f, f228 = 2.5f; std::memcpy(obj + 224, &f224, 4); std::memcpy(obj + 228, &f228, 4);
    setState__5Mummyi(obj, 10);
    bad |= check("10 dust", prefix(10) + "dust(12,4:5:6,9,1.5,2.5);dellist;dropw;v119(" + num(0x1008) + ",0);");
    if (get32(12) != (0x10u | 0x4002u) || get64(160) != (0x01000000ull | 0x20000000000ull)) { std::puts("fail 10 dust stores"); bad = 1; }
    reset(0, 4); rollValue = 1; setState__5Mummyi(obj, 10);
    bad |= check("10 drop", prefix(10) + "drop(4:5:6,9,36,2,1.6,0.6,1.4,0.25,mummytongue);rand;trans(188,0.3,2054);dropw;v119(" + num(0x1008) + ",0);");
    reset(0, 1); put32(164, 1); rollValue = 0; setState__5Mummyi(obj, 10);
    bad |= check("10 no drop", prefix(10) + "rand;trans(300,0.3,2054);dropw;v119(" + num(0x1008) + ",0);");
    for (int n : {2, 4, 7, 11, 12, 18, -1}) { reset(0, 0); setState__5Mummyi(obj, n); bad |= check("idle state", prefix(n)); if (get16(572) != static_cast<short>(n)) bad = 1; }
    reset(3, 0); procResult = 1; setState__5Mummyi(obj, 0); bad |= check("handled", "proc0;"); if (get16(572) != 3 || get32(1104) != 0) bad = 1;
    return bad;
}
