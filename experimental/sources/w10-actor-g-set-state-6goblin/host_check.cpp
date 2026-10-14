// Host contract for setState__6Goblini; not an EE input. 32-bit words are offsets from a host base
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
static int procReturn, procNew = -1; static unsigned rolls[4]; static int rollIndex; static int tagResults[3], tagIndex;
static void* foundList[8]; static int foundCount;
static std::string num(double v) { char b[40]; std::snprintf(b, sizeof b, "%g", v); return b; }
static long off(const void* p) { return static_cast<long>(static_cast<const unsigned char*>(p) - arena); }
static std::string bits(float f) { unsigned b; std::memcpy(&b, &f, 4); char s[16]; std::snprintf(s, sizeof s, "%08x", b); return s; }
extern "C" {
int setStateProcessing__8CreaturePi(void*, int* state) { calls += "proc" + num(*state) + ";"; if (procNew >= 0) *state = procNew; return procReturn; }
unsigned eRand__10GameObject(void*) { calls += "rand;"; return rolls[rollIndex++ & 3]; }
void resetRouting__8Creature(void*) { calls += "reset;"; }
void getSmartRandomDestination__8Creaturei(Point3* r, void*, int range) { calls += "smart" + num(range) + ";"; r->x = 1; r->y = 2; r->z = 3; }
void animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(void* s, void* h, float b, int f) {
    calls += "trans(" + num(off(s) - 0x1000) + "," + num(off(h)) + "," + bits(b) + "," + num(f) + ");";
}
void animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(void* s, void* h, float a, float b, int f) {
    calls += "shot(" + num(off(s) - 0x1000) + "," + num(off(h)) + "," + bits(a) + "," + bits(b) + "," + num(f) + ");";
}
void animGlobalSync__FP14AnimationState(void* s) { calls += "sync" + num(off(s) - 0x1000) + ";"; }
void setWeapons__6Goblin(void*) { calls += "weapons;"; }
void initType__6Goblinb(void*, bool f) { calls += std::string("initType(") + (f ? "1" : "0") + ");"; }
void objectAddToRunList__FP10GameObject(void*) { calls += "runlist;"; }
int findTagInt__10GameObjectPci(void*, char* tag, int n) { calls += std::string("tag(") + tag + "," + num(n) + ");"; return tagResults[tagIndex++ % 3]; }
void objectFindInBox__FG6Point3T0PP10GameObjecti(Point3 lo, Point3 hi, void** found, int cap) {
    calls += "box(" + num(lo.x) + "," + num(lo.y) + "," + num(lo.z) + "," + num(hi.x) + "," + num(hi.y) + "," + num(hi.z) + "," + num(cap) + ");";
    for (int i = 0; i < foundCount; ++i) found[i] = foundList[i];
    found[foundCount] = nullptr;
}
void dropItem__6Goblini(void*, int w) { calls += "dropItem(" + num(w) + ");"; }
void dropTreasure__8CreatureG6Point3iffffffPc(void*, Point3 p, int a, float f0, float f1, float f2, float f3, float f4, float f5, char* name) {
    calls += "treasure(" + num(p.x) + "," + num(p.y) + "," + num(p.z) + "," + num(a) + "," + num(f0) + "," + num(f1) + "," + bits(f2) + "," + bits(f3) + "," + num(f4) + "," + num(f5) + "," + (name ? name : "null") + ");";
}
void dropItem__4itemG6Point3Pc(Point3 p, char* name) { calls += "itemDrop(" + num(p.x) + "," + num(p.y) + "," + num(p.z) + "," + name + ");"; }
}
static int bad;
static unsigned char box[8][64];
static void reset(short state572 = 0) {
    std::memset(arena, 0, sizeof arena); std::memset(box, 0, sizeof box); w10g_host_base = arena;
    for (unsigned i = 0; i < 0x400; i += 4) { unsigned v = 0x2000 + i; std::memcpy(arena + 0x200 + i, &v, 4); }
    unsigned t = 0x200; std::memcpy(obj + 36, &t, 4);
    unsigned vt = 0x800; std::memcpy(obj + 72, &vt, 4);
    unsigned fn = 777; std::memcpy(arena + 0x800 + 192 + 4, &fn, 4);
    short adj = 16; std::memcpy(arena + 0x800 + 192, &adj, 2);
    unsigned w808 = 1000; std::memcpy(obj + 808, &w808, 4);
    float p[3] = {200, 300, 400}; std::memcpy(obj + 16, p, 12);
    short h56 = 100; std::memcpy(obj + 56, &h56, 2);
    std::memcpy(obj + 572, &state572, 2);
    calls.clear(); procReturn = 0; procNew = -1; rollIndex = 0; tagIndex = 0; foundCount = 0;
    tagResults[0] = tagResults[1] = tagResults[2] = 0;
    rolls[0] = 1000007; rolls[1] = 13; rolls[2] = 4; rolls[3] = 9;
}
static short half(int o) { short v; std::memcpy(&v, obj + o, 2); return v; }
static unsigned wordv(int o) { unsigned v; std::memcpy(&v, obj + o, 4); return v; }
static unsigned long long wide(int o) { unsigned long long v; std::memcpy(&v, obj + o, 8); return v; }
static float floatv(int o) { float v; std::memcpy(&v, obj + o, 4); return v; }
static void setw(int o, unsigned v) { std::memcpy(obj + o, &v, 4); }
static void seth(int o, short v) { std::memcpy(obj + o, &v, 2); }
static void expectCalls(const char* name, const std::string& want) { if (calls != want) { std::printf("fail %s:\n  got  %s\n  want %s\n", name, calls.c_str(), want.c_str()); bad = 1; } }
static void ok(bool c, const char* what) { if (!c) { std::printf("fail: %s\n", what); bad = 1; } }
#define H(x) num(0x2000 + (x))
#define N "3e999999"
#define M "3dcccccc"
int main() {
    // state 0 and 16
    reset(); setState__6Goblini(obj, 0);
    expectCalls("0", "proc0;rand;trans(288," + H(108) + "," N ",3);"); ok(wordv(256) == 1000 + 1000007 % 60 + 120, "0 timer");
    reset(); setw(972, 4); setState__6Goblini(obj, 0); expectCalls("0 with P 4", "proc0;rand;trans(288," + H(60) + "," N ",3);");
    reset(5); setw(12, 0xffffffffu); setState__6Goblini(obj, 16);
    expectCalls("16", "proc16;weapons;initType(0);runlist;rand;trans(288," + H(108) + "," N ",3);");
    ok(wordv(12) == 0xfffffff6u && wide(160) == 0x100000000ull && half(146) == 50 && half(150) == 128 && half(152) == 255 && half(154) == 150 &&
       half(140) == 0 && half(142) == 128 && half(144) == 255 && half(148) == 0 && half(572) == 0, "16 fields");
    reset(); std::memset(obj + 160, 0xff, 8); setState__6Goblini(obj, 16); ok(wide(160) == 0xfffffffffffffdffull, "bit 9 cleared and bit 32 set on 64 bits");
    // state 1, 2, 3, 14, 4
    reset(); setState__6Goblini(obj, 1);
    expectCalls("1", "proc1;reset;smart300;trans(288," + H(140) + "," N ",3);sync288;");
    { float d[3]; std::memcpy(d, obj + 272, 12); ok(d[0] == 1 && d[1] == 2 && d[2] == 3 && floatv(176) == 60.0f && half(572) == 1, "1 fields"); }
    reset(); setState__6Goblini(obj, 2); expectCalls("2", "proc2;reset;trans(288," + H(116) + "," N ",3);sync288;"); ok(floatv(176) == 150.0f, "2 speed");
    reset(); setState__6Goblini(obj, 3); expectCalls("3", "proc3;reset;trans(288," + H(140) + "," N ",3);sync288;"); ok(floatv(176) == 60.0f, "3 speed");
    reset(); setState__6Goblini(obj, 14); expectCalls("14", "proc14;reset;trans(288," + H(116) + "," N ",3);sync288;"); ok(floatv(176) == 150.0f && wordv(256) == 1060, "14 fields");
    reset(); setState__6Goblini(obj, 4); expectCalls("4", "proc4;trans(288," + H(116) + "," N ",3);sync288;");
    reset(3); setState__6Goblini(obj, 4); expectCalls("4 from 3", "proc4;trans(288," + H(116) + "," N ",3);rand;sync288;"); ok(wordv(256) == 1000 + 1000007 % 120 + 40, "4 timer");
    // state 5, 13, 9, 15
    reset(); setState__6Goblini(obj, 5); expectCalls("5", "proc5;trans(288," + H(100) + "," M ",3);");
    reset(); setw(972, 5); setState__6Goblini(obj, 5); expectCalls("5 P 5", "proc5;trans(288," + H(124) + "," M ",3);");
    reset(); setState__6Goblini(obj, 13); expectCalls("13", "proc13;trans(288," + H(100) + "," M ",3);rand;"); ok(wordv(256) == 1000 + 1000007 % 240 + 240, "13 timer");
    reset(); setw(972, 5); setState__6Goblini(obj, 13); expectCalls("13 P 5", "proc13;trans(288," + H(124) + "," M ",3);rand;");
    reset(); setState__6Goblini(obj, 9); expectCalls("9", "proc9;trans(288," + H(108) + "," N ",3);");
    reset(); setw(972, 4); setState__6Goblini(obj, 9); expectCalls("9 P 4", "proc9;trans(288," + H(60) + "," N ",3);");
    reset(); setState__6Goblini(obj, 15); expectCalls("15", "proc15;trans(288," + H(148) + "," M ",3);"); ok(wordv(256) == 1120 && (wide(160) & 0x200) != 0, "15 fields");
    // state 6
    reset(); setw(972, 4); setState__6Goblini(obj, 6); expectCalls("6 P 4", "proc6;trans(288," + H(60) + "," M ",3);shot(288," + H(52) + "," M "," M ",8);rand;"); ok(wordv(256) == 1000007 % 20, "6 P4 timer");
    reset(); setw(972, 5); setw(992, 1); setState__6Goblini(obj, 6); expectCalls("6 P 5", "proc6;trans(288," + H(124) + "," M ",3);shot(288," + H(132) + "," M "," M ",8);rand;"); ok(wordv(256) == 1000007 % 60 + 20, "6 P5 timer with Q");
    reset(); setw(972, 0); setState__6Goblini(obj, 6);
    expectCalls("6 P 0 Q 0", "proc6;rand;trans(288," + H(100) + "," M ",3);shot(288," + H(28) + "," M "," M ",8);rand;"); ok(obj[980] == 0, "6 byte 980 cleared by roll & 3 == 0 or untouched");
    reset(); setw(972, 2); setw(992, 1); rolls[0] = 1; setState__6Goblini(obj, 6);
    expectCalls("6 P 2 Q 1", "proc6;rand;trans(288," + H(148) + "," M ",3);rand;shot(288," + H(28 + 8 * (13 & 1)) + "," M "," M ",8);rand;"); ok(obj[980] == 1, "6 byte 980 set");
    reset(); setw(972, 2); setw(992, 1); rolls[0] = 4; setState__6Goblini(obj, 6);
    expectCalls("6 roll & 3 == 0 clears", "proc6;rand;trans(288," + H(100) + "," M ",3);rand;shot(288," + H(28 + 8 * (13 & 1)) + "," M "," M ",8);rand;");
    reset(); setw(972, 6); setState__6Goblini(obj, 6); expectCalls("6 P 6", "proc6;rand;trans(288," + H(100) + "," M ",3);rand;");
    reset(); setw(972, 0xffffffffu); setState__6Goblini(obj, 6); expectCalls("6 P -1", "proc6;rand;trans(288," + H(100) + "," M ",3);rand;");
    // state 8
    reset(9); setState__6Goblini(obj, 8); expectCalls("8 from 9", "proc8;"); ok(half(572) == 9, "8 keeps 9");
    reset(6); { unsigned short f = 8; std::memcpy(obj + 294, &f, 2); } setState__6Goblini(obj, 8); expectCalls("8 from 6 with bit", "proc8;"); ok(half(572) == 6, "8 keeps 6");
    reset(6); setw(744, 0x40000); seth(748, 30000); setState__6Goblini(obj, 8); expectCalls("8 from 6 without bit, far", "proc8;shot(288," + H(164) + "," M "," M ",2064);");
    reset(0); setw(744, 0x40000); seth(748, 100 + 26767); setState__6Goblini(obj, 8); expectCalls("8 window edge in", "proc8;");
    reset(0); setw(744, 0x40000); seth(748, 100 + 26768); setState__6Goblini(obj, 8); expectCalls("8 window edge out", "proc8;shot(288," + H(164) + "," M "," M ",2064);");
    reset(0); setw(744, 0x40000); seth(748, static_cast<short>(100 - 26767)); setState__6Goblini(obj, 8); expectCalls("8 window low edge in", "proc8;");
    reset(0); setw(744, 0x40000); seth(748, static_cast<short>(100 - 26768)); setState__6Goblini(obj, 8); expectCalls("8 window low edge out", "proc8;shot(288," + H(164) + "," M "," M ",2064);");
    reset(0); setState__6Goblini(obj, 8); expectCalls("8 default", "proc8;rand;shot(288," + H(76 + 8 * (1000007 % 3)) + "," M "," M ",16);");
    // state 10
    reset(); tagResults[0] = 5; tagResults[1] = 0; setw(12, 3); setw(744, 0); setState__6Goblini(obj, 10);
    expectCalls("10 tag present", "proc10;tag(dead,0);tag(dead,0);trans(288," + H(68) + "," N ",6);treasure(200,300,400,100,36,2,3fe66666,3f199999,2,0.25,null);vcall(16);");
    ok((wordv(12) & 1) == 1, "10 sets bit 0 when the tag is present");
    reset(); setw(12, 2); setState__6Goblini(obj, 10);
    expectCalls("10 empty box", "proc10;tag(dead,0);box(104,204,304,296,396,496,128);tag(dead,0);trans(288," + H(68) + "," N ",6);treasure(200,300,400,100,36,2,3fe66666,3f199999,2,0.25,null);vcall(16);");
    ok(wordv(1136) == 0, "10 no crowd");
    for (int i = 0; i < 4; ++i) { std::memset(box[i], 0, 64); unsigned w = 0x28; std::memcpy(box[i] + 12, &w, 4); foundList[i] = box[i]; }
    reset(); for (int i = 0; i < 4; ++i) { unsigned w = 0x28; std::memcpy(box[i] + 12, &w, 4); foundList[i] = box[i]; } foundCount = 2; setw(12, 2); setState__6Goblini(obj, 10); ok(wordv(1136) == 0, "10 two matches do not count enough");
    reset(); for (int i = 0; i < 4; ++i) { unsigned w = 0x28; std::memcpy(box[i] + 12, &w, 4); foundList[i] = box[i]; } foundCount = 3; setw(12, 2); setState__6Goblini(obj, 10); ok(wordv(1136) == 1, "10 three matches set the word");
    reset(); for (int i = 0; i < 4; ++i) { unsigned w = i == 1 ? 0x29 : 0x28; std::memcpy(box[i] + 12, &w, 4); foundList[i] = box[i]; } foundCount = 3; setw(12, 2); setState__6Goblini(obj, 10); ok(wordv(1136) == 1, "10 bit 0 is outside the 0x68 mask");
    reset(); for (int i = 0; i < 4; ++i) { unsigned w = i == 1 ? 0x68 : 0x28; std::memcpy(box[i] + 12, &w, 4); foundList[i] = box[i]; } foundCount = 3; setw(12, 2); setState__6Goblini(obj, 10); ok(wordv(1136) == 0, "10 bit 6 set differs");
    reset(); setw(12, 1); setState__6Goblini(obj, 10);
    expectCalls("10 flag bit 1 clear drops items", "proc10;tag(dead,0);box(104,204,304,296,396,496,128);dropItem(1);dropItem(2);tag(dead,0);trans(288," + H(68) + "," N ",6);treasure(200,300,400,100,36,2,3fe66666,3f199999,2,0.25,null);vcall(16);");
    ok((wordv(12) & 1) == 0, "10 clears bit 0 before dropping");
    reset(); setw(12, 2); { unsigned long long q = 0x100000000ull; std::memcpy(obj + 160, &q, 8); } setState__6Goblini(obj, 10);
    expectCalls("10 bit 32 blocks treasure", "proc10;tag(dead,0);box(104,204,304,296,396,496,128);tag(dead,0);trans(288," + H(68) + "," N ",6);vcall(16);");
    reset(); setw(12, 2); setw(972, 4); rolls[0] = 5999; setState__6Goblini(obj, 10);
    expectCalls("10 arrows", "proc10;tag(dead,0);box(104,204,304,296,396,496,128);tag(dead,0);trans(288," + H(68) + "," N ",6);treasure(200,300,400,100,36,2,3fe66666,3f199999,2,0.25,null);rand;itemDrop(200,300,400,arrows);vcall(16);");
    reset(); setw(12, 2); setw(972, 4); rolls[0] = 6000; setState__6Goblini(obj, 10);
    expectCalls("10 arrows roll too high", "proc10;tag(dead,0);box(104,204,304,296,396,496,128);tag(dead,0);trans(288," + H(68) + "," N ",6);treasure(200,300,400,100,36,2,3fe66666,3f199999,2,0.25,null);rand;vcall(16);");
    // quiet states and final store
    for (int s : {7, 11, 12, 17, 18, 99, -1}) { reset(); setState__6Goblini(obj, s); expectCalls("quiet", "proc" + num(s) + ";"); ok(half(572) == static_cast<short>(s), "quiet store"); }
    reset(); std::memset(obj + 160, 0xff, 8); setState__6Goblini(obj, 7); ok(wide(160) == 0xfffffffffffffdffull, "bit 9 of +160 cleared for every accepted state");
    reset(); std::memset(obj + 160, 0xff, 8); procReturn = 1; setState__6Goblini(obj, 7); ok(wide(160) == 0xffffffffffffffffull, "nonzero processing result touches nothing");
    reset(); procReturn = 1; setState__6Goblini(obj, 5); expectCalls("returns", "proc5;"); ok(half(572) == 0, "no store after nonzero processing");
    reset(); procNew = 9; setState__6Goblini(obj, 7); expectCalls("rewritten", "proc7;trans(288," + H(108) + "," N ",3);"); ok(half(572) == 9, "rewritten state stored");
    if (!bad) std::puts("goblin setState ok");
    return bad;
}
