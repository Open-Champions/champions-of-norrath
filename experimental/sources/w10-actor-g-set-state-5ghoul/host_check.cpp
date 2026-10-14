// Host contract for setState__5Ghouli; not an EE input. 32-bit words are offsets from a host base
// (W10_ACTOR_G_HOST). Each case checks the callee sequence with its arguments and the fields.
#define W10_ACTOR_G_HOST 1
#include <cstdio>
#include <cstring>
#include <string>
unsigned char* w10g_host_base;
static std::string calls;
static void vcallFn(void* o) { char b[48]; std::snprintf(b, sizeof b, "vcall(%ld);", static_cast<long>(static_cast<unsigned char*>(o) - w10g_host_base - 0x1000)); calls += b; }
void* w10g_host_fn(unsigned) { return reinterpret_cast<void*>(vcallFn); }
#include "set_state.cpp"
static unsigned char arena[0x4000] __attribute__((aligned(16)));
static unsigned char* const obj = arena + 0x1000;
static int procReturn, procNew = -1; static unsigned rolls[4]; static int rollIndex; static short pathResult;
static std::string num(double v) { char b[40]; std::snprintf(b, sizeof b, "%g", v); return b; }
static long off(const void* p) { return static_cast<long>(static_cast<const unsigned char*>(p) - arena); }
static std::string blend(float f) { unsigned b; std::memcpy(&b, &f, 4); char s[16]; std::snprintf(s, sizeof s, "%08x", b); return s; }
extern "C" {
int setStateProcessing__8CreaturePi(void*, int* state) { calls += "proc" + num(*state) + ";"; if (procNew >= 0) *state = procNew; return procReturn; }
unsigned eRand__10GameObject(void*) { calls += "rand;"; return rolls[rollIndex++ & 3]; }
void resetRouting__8Creature(void*) { calls += "reset;"; }
void getSmartRandomDestination__8Creaturei(Point3* r, void*, int range) { calls += "smart" + num(range) + ";"; r->x = 1; r->y = 2; r->z = 3; }
short findClearPath__8CreatureG6Point3scf(void*, Point3 p, short heading, char flag, float range) {
    calls += "path(" + num(p.x) + "," + num(p.y) + "," + num(p.z) + "," + num(heading) + "," + num(flag) + "," + num(range) + ");"; return pathResult;
}
void animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(void* s, void* h, float b, int f) {
    calls += "trans(" + num(off(s) - 0x1000) + "," + num(off(h)) + "," + blend(b) + "," + num(f) + ");";
}
void animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(void* s, void* h, float a, float b, int f) {
    calls += "shot(" + num(off(s) - 0x1000) + "," + num(off(h)) + "," + blend(a) + "," + blend(b) + "," + num(f) + ");";
}
void animGlobalSync__FP14AnimationState(void* s) { calls += "sync" + num(off(s) - 0x1000) + ";"; }
void dropTreasure__8CreatureG6Point3iffffffPc(void*, Point3 p, int a, float f0, float f1, float f2, float f3, float f4, float f5, char* name) {
    calls += "drop(" + num(p.x) + "," + num(p.y) + "," + num(p.z) + "," + num(a) + "," + num(f0) + "," + num(f1) + "," + blend(f2) + "," + blend(f3) + "," + blend(f4) + "," + num(f5) + "," + name + ");";
}
}
static int bad;
static void reset(short state572 = 0) {
    std::memset(arena, 0, sizeof arena); w10g_host_base = arena;
    for (unsigned i = 0; i < 0x400; i += 4) { unsigned v = 0x2000 + i; std::memcpy(arena + 0x200 + i, &v, 4); }
    unsigned t = 0x200; std::memcpy(obj + 36, &t, 4);
    unsigned vt = 0x800; std::memcpy(obj + 72, &vt, 4);
    unsigned fn = 777; std::memcpy(arena + 0x800 + 192 + 4, &fn, 4);
    short adj = 16; std::memcpy(arena + 0x800 + 192, &adj, 2);
    unsigned w808 = 1000; std::memcpy(obj + 808, &w808, 4);
    float p[3] = {5, 6, 7}; std::memcpy(obj + 16, p, 12);
    short h56 = 100; std::memcpy(obj + 56, &h56, 2);
    std::memcpy(obj + 572, &state572, 2);
    calls.clear(); procReturn = 0; procNew = -1; rollIndex = 0; pathResult = 0;
    rolls[0] = 1000007; rolls[1] = 13; rolls[2] = 4; rolls[3] = 9;
}
static short half(int o) { short v; std::memcpy(&v, obj + o, 2); return v; }
static unsigned wordv(int o) { unsigned v; std::memcpy(&v, obj + o, 4); return v; }
static float floatv(int o) { float v; std::memcpy(&v, obj + o, 4); return v; }
static void expectCalls(const char* name, const std::string& want) { if (calls != want) { std::printf("fail %s:\n  got  %s\n  want %s\n", name, calls.c_str(), want.c_str()); bad = 1; } }
static void ok(bool c, const char* what) { if (!c) { std::printf("fail: %s\n", what); bad = 1; } }
#define H(x) num(0x2000 + (x))
#define B3 "3e999999"
#define B1 "3dcccccc"
int main() {
    reset(); setState__5Ghouli(obj, 0);
    expectCalls("0", "proc0;rand;trans(288," + H(180) + "," B3 ",3);"); ok(wordv(256) == 1000 + 1000007 % 90 + 90 && half(572) == 0, "0 fields");
    reset(); setState__5Ghouli(obj, 1);
    expectCalls("1", "proc1;reset;smart300;trans(288," + H(52) + "," B3 ",3);sync288;");
    { float d[3]; std::memcpy(d, obj + 272, 12); ok(d[0] == 1 && d[1] == 2 && d[2] == 3 && floatv(176) == 170.0f && half(572) == 1, "1 fields"); }
    reset(); setState__5Ghouli(obj, 13); expectCalls("13", "proc13;reset;trans(288," + H(52) + "," B3 ",3);sync288;"); ok(floatv(176) == 170.0f, "13 float");
    reset(3); setState__5Ghouli(obj, 3); expectCalls("3 already 3", "proc3;sync288;"); ok(floatv(176) == 170.0f, "3 float");
    reset(2); setState__5Ghouli(obj, 3); expectCalls("3 from other", "proc3;trans(288," + H(52) + "," B3 ",3);sync288;");
    reset(); setState__5Ghouli(obj, 5); expectCalls("5", "proc5;trans(288," + H(60) + "," B1 ",3);");
    reset(); setState__5Ghouli(obj, 6); expectCalls("6", "proc6;trans(288," + H(60) + "," B1 ",3);rand;shot(288," + H(28 + 8 * (1000007 & 1)) + "," B1 "," B1 ",8);");
    reset(); setState__5Ghouli(obj, 9); expectCalls("9", "proc9;trans(288," + H(180) + "," B3 ",3);");
    reset(); setState__5Ghouli(obj, 14); expectCalls("14", "proc14;trans(288," + H(196) + "," B1 ",3);rand;"); ok(wordv(256) == 1000 + 1000007 % 240 + 240, "14 field");
    reset(); setState__5Ghouli(obj, 17);
    expectCalls("17", "proc17;trans(288," + H(60) + "," B1 ",3);shot(288," + H(44) + "," B1 "," B1 ",8);"); ok(wordv(256) == 20, "17 field");
    // state 16: quadrants
    struct { int diff; int index; } quad[] = {{0, 18}, {-8192, 18}, {8191, 18}, {8192, 19}, {24575, 19}, {24576, 17}, {32767, 17}, {-8193, 20}, {-24575, 20}, {-24576, 18}, {-32768, 18}};
    for (auto q : quad) {
        reset(); pathResult = static_cast<short>(100 + q.diff);
        setState__5Ghouli(obj, 16);
        expectCalls("16", "proc16;trans(288," + H(52) + "," B1 ",3);rand;path(5,6,7,100,1,3);shot(288," + H(4 + 8 * q.index) + ",3c23d70a," B1 ",8);");
        ok(half(964) == static_cast<short>(100 + q.diff) && wordv(968) == 0, "16 stores");
        ok(floatv(176) == (q.index == 17 ? 250.0f : 200.0f), "16 speed");
    }
    // state 8
    reset(9); setState__5Ghouli(obj, 8); expectCalls("8 from 9", "proc8;"); ok(half(572) == 9, "8 keeps 9");
    reset(6); { unsigned short f = 8; std::memcpy(obj + 294, &f, 2); } setState__5Ghouli(obj, 8); expectCalls("8 from 6 with bit", "proc8;"); ok(half(572) == 6, "8 keeps 6");
    reset(17); { unsigned short f = 8; std::memcpy(obj + 294, &f, 2); } setState__5Ghouli(obj, 8); expectCalls("8 from 17 with bit", "proc8;"); ok(half(572) == 17, "8 keeps 17");
    reset(15); setState__5Ghouli(obj, 8); expectCalls("8 from 15", "proc8;"); ok(half(572) == 15 && (wordv(12) & 1) == 1, "8 from 15: state 15 sets bit 0");
    reset(6); setState__5Ghouli(obj, 8); expectCalls("8 from 6 without bit (roll % 3 == 1)", "proc8;rand;shot(288," + H(132) + "," B1 "," B1 ",16);"); ok(half(572) == 8, "8 stores 8");
    reset(0); rolls[0] = 1000008; setState__5Ghouli(obj, 8);
    expectCalls("8 roll % 3 == 0", "proc8;rand;trans(288," + H(220) + ",3f800000,3);shot(288," + H(124) + "," B1 "," B1 ",20);rand;");
    ok(half(572) == 15 && (wordv(12) & 1) == 1 && wordv(960) == 1000 + 13 % 180 + 120, "8 to 15 with second roll");
    // state 10
    reset(); setState__5Ghouli(obj, 10);
    expectCalls("10 clear", "proc10;drop(5,6,7,100,36,2,3fcccccc,3f199999,3fb33333,0.25,ghoulflesh);trans(288," + H(124) + "," B3 ",14);vcall(16);");
    ok(obj[40] == 60 && (wordv(12) & 1) == 0, "10 fields");
    reset(); { unsigned w = 3; std::memcpy(obj + 12, &w, 4); } setState__5Ghouli(obj, 10);
    expectCalls("10 with bit 1", "proc10;drop(5,6,7,100,36,2,3fcccccc,3f199999,3fb33333,0.25,ghoulflesh);trans(288," + H(68) + "," B3 ",6);vcall(16);");
    ok(wordv(12) == 3, "10 keeps both bits when bit 1 is set");
    reset(); { unsigned w = 5; std::memcpy(obj + 12, &w, 4); } setState__5Ghouli(obj, 10); ok(wordv(12) == 4, "10 clears bit 0 when bit 1 is clear");
    // final bit handling
    reset(); setState__5Ghouli(obj, 15); expectCalls("15", "proc15;"); ok((wordv(12) & 1) == 1 && half(572) == 15, "15 sets bit 0");
    reset(15); { unsigned w = 1; std::memcpy(obj + 12, &w, 4); } setState__5Ghouli(obj, 2); ok((wordv(12) & 1) == 0 && half(572) == 2, "leaving 15 clears bit 0");
    reset(15); { unsigned w = 1; std::memcpy(obj + 12, &w, 4); } setState__5Ghouli(obj, 15); ok((wordv(12) & 1) == 1, "15 to 15 keeps bit 0");
    reset(3); { unsigned w = 1; std::memcpy(obj + 12, &w, 4); } setState__5Ghouli(obj, 2); ok((wordv(12) & 1) == 1, "other states leave bit 0");
    for (int s : {2, 4, 7, 11, 12, 18, 99, -1}) { reset(); setState__5Ghouli(obj, s); expectCalls("quiet", "proc" + num(s) + ";"); ok(half(572) == static_cast<short>(s), "quiet store"); }
    reset(); procReturn = 1; setState__5Ghouli(obj, 5); expectCalls("returns", "proc5;"); ok(half(572) == 0, "no store after nonzero processing");
    reset(); procNew = 9; setState__5Ghouli(obj, 2); expectCalls("rewritten", "proc2;trans(288," + H(180) + "," B3 ",3);"); ok(half(572) == 9, "rewritten state stored");
    if (!bad) std::puts("ghoul setState ok");
    return bad;
}
