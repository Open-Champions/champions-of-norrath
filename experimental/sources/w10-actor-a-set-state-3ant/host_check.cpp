// Host contract for setState__3Anti; not an EE input. 32-bit words in the object are offsets
// from a host base (W10_ACTOR_A_HOST), so the real field offsets are used. Each case checks
// the callee sequence with its arguments and the final halfword at +572.
#define W10_ACTOR_A_HOST 1
#include <cstdio>
#include <cstring>
#include <string>
unsigned char* w10_actor_a_host_base;
#include "set_state.cpp"

static unsigned char arena[0x4000] __attribute__((aligned(16)));
static unsigned char* const obj = arena + 0x1000;   // the object (offsets used as words are relative to arena)
static std::string calls;
static int procReturn, procNew = -1;
static unsigned rolls[4]; static int rollIndex;
static std::string num(double v) { char b[32]; std::snprintf(b, sizeof b, "%g", v); return b; }
static unsigned off(const void* p) { return static_cast<unsigned>(static_cast<const unsigned char*>(p) - arena); }
int setStateProcessing__8CreaturePi(void*, int* state) { calls += "proc" + num(*state) + ";"; if (procNew >= 0) *state = procNew; return procReturn; }
unsigned eRand__10GameObject(void*) { calls += "rand;"; return rolls[rollIndex++ & 3]; }
void resetRouting__8Creature(void*) { calls += "reset;"; }
void getSmartRandomDestination__8Creaturei(Point3* r, void*, int range) { calls += "smart" + num(range) + ";"; r->x = 1; r->y = 2; r->z = 3; }
void animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(void* s, void* h, float b, int f) {
    calls += "trans(" + num(off(s) - 0x1000) + "," + num(off(h)) + "," + num(b) + "," + num(f) + ");";
}
void animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(void* s, void* h, float a, float b, int f) {
    calls += "shot(" + num(off(s) - 0x1000) + "," + num(off(h)) + "," + num(a) + "," + num(b) + "," + num(f) + ");";
}
void animGlobalSync__FP14AnimationState(void* s) { calls += "sync" + num(off(s) - 0x1000) + ";"; }
void loseBodyPart__3Anti(void*, int part) { calls += "lose" + num(part) + ";"; }
void dropTreasure__8CreatureG6Point3iffffffPc(void*, Point3 p, int a, float f0, float f1, float f2, float f3, float f4, float f5, char* name) {
    calls += "drop(" + num(p.x) + "," + num(p.y) + "," + num(p.z) + "," + num(a) + "," + num(f0) + "," + num(f1) + "," + num(f2) + "," + num(f3) + "," + num(f4) + "," + num(f5) + "," + (name ? "name" : "null") + ");";
}
void w10_actor_a_host_call(unsigned fw, void* object) { calls += "vcall(" + num(fw) + "," + num(off(object) - 0x1000) + ");"; }

static int bad;
static void reset() {
    std::memset(arena, 0, sizeof arena);
    w10_actor_a_host_base = arena;
    for (unsigned i = 0; i < 0x100; i += 4) { unsigned v = 0x2000 + i; std::memcpy(arena + 0x200 + i, &v, 4); }  // header table: entry at +i holds 0x2000+i
    unsigned t = 0x200; std::memcpy(obj + 36, &t, 4);
    unsigned vt = 0x800; std::memcpy(obj + 72, &vt, 4);
    unsigned fn = 777; std::memcpy(arena + 0x800 + 192 + 4, &fn, 4);
    short adj = 16; std::memcpy(arena + 0x800 + 192, &adj, 2);
    float p[3] = {5, 6, 7}; std::memcpy(obj + 16, p, 12);
    short sixteen = -3; std::memcpy(obj + 56, &sixteen, 2);
    unsigned w808 = 1000; std::memcpy(obj + 808, &w808, 4);
    calls.clear(); procReturn = 0; procNew = -1; rollIndex = 0;
    rolls[0] = 1000007; rolls[1] = 13; rolls[2] = 4; rolls[3] = 9;
}
static short half(int o) { short v; std::memcpy(&v, obj + o, 2); return v; }
static unsigned wordv(int o) { unsigned v; std::memcpy(&v, obj + o, 4); return v; }
static void expectCalls(const char* name, const std::string& want) { if (calls != want) { std::printf("fail %s:\n  got  %s\n  want %s\n", name, calls.c_str(), want.c_str()); bad = 1; } }
#define H(x) num(0x2000 + (x))
int main() {
    reset(); setState__3Anti(obj, 0);
    expectCalls("0", "proc0;rand;trans(288," + H(0x200 + 84 - 0x200) + ",0.3,3);");
    if (wordv(256) != 1000 + 1000007 % 120 || half(572) != 0) { std::puts("fail 0 fields"); bad = 1; }
    reset(); setState__3Anti(obj, 1);
    expectCalls("1", "proc1;reset;smart200;trans(288," + H(108) + ",0.3,3);sync288;");
    float f176, d[3]; std::memcpy(&f176, obj + 176, 4); std::memcpy(d, obj + 272, 12);
    if (f176 != 140.0f || d[0] != 1 || d[1] != 2 || d[2] != 3 || half(572) != 1) { std::puts("fail 1 fields"); bad = 1; }
    reset(); setState__3Anti(obj, 3);
    expectCalls("3", "proc3;reset;trans(288," + H(108) + ",0.3,3);sync288;");
    reset(); setState__3Anti(obj, 5); expectCalls("5", "proc5;trans(288," + H(100) + ",0.1,3);");
    reset(); setState__3Anti(obj, 9); expectCalls("9", "proc9;trans(288," + H(84) + ",0.3,3);");
    reset(); setState__3Anti(obj, 6);
    expectCalls("6", "proc6;trans(288," + H(100) + ",0.1,3);shot(288," + H(36) + ",0.1,0.1,8);rand;");
    if (wordv(964) != 0 || wordv(256) != 1000007 % 15) { std::puts("fail 6 fields"); bad = 1; }
    reset(); obj[968] = 1; setState__3Anti(obj, 6);
    expectCalls("6/1", "proc6;trans(288," + H(100) + ",0.1,3);rand;shot(288," + H(36 + 8) + ",0.1,0.1,8);rand;");
    reset(); setState__3Anti(obj, 13);
    expectCalls("13", "proc13;shot(288," + H(116) + ",0.1,0.1,8);rand;");
    if (wordv(980) != 1000 + 1000007 % 60 + 180) { std::puts("fail 13 field"); bad = 1; }
    reset(); setState__3Anti(obj, 8);
    expectCalls("8 plain", "proc8;shot(288," + H(76) + ",0.1,0.1,16);");
    reset(); { short c = 6; std::memcpy(obj + 572, &c, 2); unsigned short fl = 8; std::memcpy(obj + 294, &fl, 2); } setState__3Anti(obj, 8);
    expectCalls("8 keep 6", "proc8;"); if (half(572) != 6) { std::puts("fail 8 keeps state 6"); bad = 1; }
    reset(); { short c = 9; std::memcpy(obj + 572, &c, 2); } setState__3Anti(obj, 8);
    expectCalls("8 keep 9", "proc8;"); if (half(572) != 9) { std::puts("fail 8 keeps state 9"); bad = 1; }
    reset(); { short c = 6; std::memcpy(obj + 572, &c, 2); } setState__3Anti(obj, 8);
    expectCalls("8 six without bit", "proc8;shot(288," + H(76) + ",0.1,0.1,16);");
    reset(); rolls[0] = 3; rolls[1] = 5; rolls[2] = 2; setState__3Anti(obj, 10);
    expectCalls("10 lose", "proc10;rand;rand;lose5;drop(5,6,7,-3,36,2,1,0.1,1.2,0.25,null);rand;trans(288," + H(52 + 16) + ",0.3,6);vcall(777,16);");
    reset(); rolls[0] = 4; rolls[1] = 1; obj[968] = 2; unsigned w12 = 0xff; std::memcpy(obj + 12, &w12, 4); setState__3Anti(obj, 10);
    expectCalls("10 no lose", "proc10;rand;drop(5,6,7,-3,36,2,1,0.1,1.2,0.25,null);rand;trans(288," + H(52 + 8) + ",0.3,6);vcall(777,16);");
    if (wordv(12) != 0xfe || wordv(976) != 0) { std::puts("fail 10 fields"); bad = 1; }
    reset(); setState__3Anti(obj, 2); expectCalls("2", "proc2;"); if (half(572) != 2) { std::puts("fail 2"); bad = 1; }
    reset(); setState__3Anti(obj, 14); expectCalls("14", "proc14;"); if (half(572) != 14) { std::puts("fail 14"); bad = 1; }
    reset(); setState__3Anti(obj, -1); expectCalls("-1", "proc-1;"); if (half(572) != -1) { std::puts("fail -1"); bad = 1; }
    reset(); obj[968] = 2; setState__3Anti(obj, 3); expectCalls("byte2 -> 5", "proc3;trans(288," + H(100) + ",0.1,3);"); if (half(572) != 5) { std::puts("fail forced 5"); bad = 1; }
    reset(); obj[968] = 2; setState__3Anti(obj, 5); expectCalls("byte2 keeps 5", "proc5;trans(288," + H(100) + ",0.1,3);");
    reset(); obj[968] = 2; setState__3Anti(obj, 13); expectCalls("byte2 13->5", "proc13;trans(288," + H(100) + ",0.1,3);");
    reset(); procReturn = 1; procNew = 5; setState__3Anti(obj, 3); expectCalls("early", "proc3;"); if (half(572) != 0) { std::puts("fail early return stores"); bad = 1; }
    reset(); procNew = 5; setState__3Anti(obj, 3); expectCalls("changed", "proc3;trans(288," + H(100) + ",0.1,3);"); if (half(572) != 5) { std::puts("fail changed state"); bad = 1; }
    return bad;
}
