// Host contract for msg_run__5Ghoul; not an EE input. Words are offsets from a host base
// (W10_ACTOR_G_HOST); the callee sequence and arguments are checked per state.
#define W10_ACTOR_G_HOST 1
#include <cstdio>
#include <cstring>
#include <string>
unsigned char* w10g_host_base;
static std::string calls;
static unsigned char arena[0x4000] __attribute__((aligned(16)));
static unsigned char* const obj = arena + 0x1000;
static std::string num(double v) { char b[40]; std::snprintf(b, sizeof b, "%g", v); return b; }
static long off(const void* p) { return static_cast<long>(static_cast<const unsigned char*>(p) - arena); }
static void v168(void* o) { calls += "v168(" + num(off(o) - 0x1000) + ");"; }
static void v184(void* o, int a) { calls += "v184(" + num(off(o) - 0x1000) + "," + num(a) + ");"; }
void* w10g_host_fn(unsigned w) { return w == 1 ? reinterpret_cast<void*>(v168) : reinterpret_cast<void*>(v184); }
#include "msg_run.cpp"
static int everyResult, activationResult, advanceResult, exitResult, crowdedResult;
static int zoneResults[4], zoneIndex; static unsigned rolls[6]; static int rollIndex;
static float trig[2]; static int trigIndex;
static std::string bits(float f) { unsigned b; std::memcpy(&b, &f, 4); char s[16]; std::snprintf(s, sizeof s, "%08x", b); return s; }
extern "C" {
void newDamageHandler__8CreatureP12_modelHeader(void*, void* m) { calls += "dmg(" + num(off(m)) + ");"; }
int everyFrame__8CreatureP12_modelHeader(void*, void* m) { calls += "every(" + num(off(m)) + ");"; return everyResult; }
int objectActivationCheck__8Creaturei(void*, int r) { calls += "act" + num(r) + ";"; return activationResult; }
int creatureAdvanceAnimation__8Creaturef(void*, float f) { calls += "adv" + num(f) + ";"; return advanceResult; }
void slideBack__8Creaturef(void*, float f) { calls += "slide" + num(f) + ";"; }
int enemyInZone__8Creaturefs(void*, float f, short s) { calls += "zone(" + num(f) + "," + num(s) + ");"; return zoneResults[zoneIndex++ & 3]; }
void smartWander__8Creature(void*) { calls += "wander;"; }
void turnToFaceEnemy__8Creature(void*) { calls += "turn;"; }
void steerAndRouteToEnemy__8Creature(void*) { calls += "steer;"; }
int bExitStatePersueIdle__8Creature(void*) { calls += "exit;"; return exitResult; }
int objectIsCrowded__8CreatureP10GameObjectffi(void*, void* other, float a, float b, int c) {
    calls += "crowd(" + num(off(other)) + "," + num(a) + "," + num(b) + "," + num(c) + ");"; return crowdedResult;
}
void produceMeleeDamage__8CreaturefiiiiPQ24item11description(void*, float a, int b, int c, int d, int e, void* p) {
    calls += "melee(" + num(a) + "," + num(b) + "," + num(c) + "," + num(d) + "," + num(e) + "," + (p ? "desc" : "null") + ");";
}
void move2__8CreatureG6Point3(void*, Point3 p) { calls += "move2(" + num(p.x) + "," + num(p.y) + "," + num(p.z) + ");"; }
float icos__Fi(int a) { calls += "icos(" + num(a) + ");"; return trig[trigIndex++ & 1]; }
float isin__Fi(int a) { calls += "isin(" + num(a) + ");"; return trig[trigIndex++ & 1]; }
unsigned eRand__10GameObject(void*) { calls += "rand;"; return rolls[rollIndex++ % 6]; }
void animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(void* s, void* h, float b, int f) {
    calls += "trans(" + num(off(s) - 0x1000) + "," + num(off(h)) + "," + bits(b) + "," + num(f) + ");";
}
void animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(void* s, void* h, float a, float b, int f) {
    calls += "shot(" + num(off(s) - 0x1000) + "," + num(off(h)) + "," + bits(a) + "," + bits(b) + "," + num(f) + ");";
}
void floorAdd__FG6Point3sfii(Point3 p, short heading, float radius, int a, int b) {
    calls += "floor(" + num(p.x) + "," + num(p.y) + "," + num(p.z) + "," + num(heading) + "," + num(radius) + "," + num(a) + "," + num(b) + ");";
}
void objectRemoveFromRunList__FP10GameObject(void*) { calls += "remove;"; }
}
static int bad;
static void w32(int o, unsigned v) { std::memcpy(obj + o, &v, 4); }
static void wa(int o, unsigned v) { std::memcpy(arena + o, &v, 4); }
static void h16(int o, unsigned short v) { std::memcpy(obj + o, &v, 2); }
static void f32(int o, float v) { std::memcpy(obj + o, &v, 4); }
static unsigned r32(int o) { unsigned v; std::memcpy(&v, obj + o, 4); return v; }
static void reset(short state) {
    std::memset(arena, 0, sizeof arena); w10g_host_base = arena; calls.clear();
    for (unsigned i = 0; i < 0x400; i += 4) wa(0x200 + i, 0x2000 + i);
    w32(36, 0x200); wa(0x200 + 12, 0x777); w32(72, 0x800);
    for (int e : {168, 184}) { unsigned fn = e == 168 ? 1 : 3; wa(0x800 + e + 4, fn); short adj = 16; std::memcpy(arena + 0x800 + e, &adj, 2); }
    h16(572, static_cast<unsigned short>(state));
    everyResult = 0; activationResult = 1; advanceResult = 0; exitResult = 0; crowdedResult = 0; zoneIndex = 0; rollIndex = 0; trigIndex = 0;
    for (int& z : zoneResults) z = 0;
    rolls[0] = 1000007; rolls[1] = 13; rolls[2] = 4; rolls[3] = 9; rolls[4] = 6; rolls[5] = 3;
    trig[0] = 0.5f; trig[1] = -2.0f;
    f32(28, 10.0f); w32(576, 0); w32(808, 100);
}
static const std::string START = "dmg(1911);every(1911);act600;adv1;slide0.5;v168(16);";
static void expectCalls(const char* name, const std::string& want) {
    if (calls != want) { std::printf("fail %s:\n  got  %s\n  want %s\n", name, calls.c_str(), want.c_str()); bad = 1; }
}
static void ok(bool c, const char* what) { if (!c) { std::printf("fail: %s\n", what); bad = 1; } }
#define H(x) num(0x2000 + (x))
#define B1 "3dcccccc"
int main() {
    reset(0); everyResult = 1; msg_run__5Ghoul(obj); expectCalls("everyFrame stops", "dmg(1911);every(1911);");
    reset(0); activationResult = 0; msg_run__5Ghoul(obj); expectCalls("activation stops", "dmg(1911);every(1911);act600;");
    for (int s : {2, 4, 7, 11, 12, 18, 99, -1, 0x400d}) { reset(static_cast<short>(s)); msg_run__5Ghoul(obj); expectCalls("quiet", START); }
    // state 0
    reset(0); w32(256, 5); msg_run__5Ghoul(obj); expectCalls("0 idle", START + "zone(400,32767);zone(500,18000);v184(16,1);");
    reset(0); w32(256, 500); msg_run__5Ghoul(obj); expectCalls("0 timer not below", START + "zone(400,32767);zone(500,18000);");
    reset(0); zoneResults[0] = 1; msg_run__5Ghoul(obj); expectCalls("0 near", START + "zone(400,32767);v184(16,3);");
    reset(0); zoneResults[1] = 1; msg_run__5Ghoul(obj); expectCalls("0 far", START + "zone(400,32767);zone(500,18000);v184(16,3);");
    // states 1 and 13
    for (int s : {1, 13}) {
        reset(s); msg_run__5Ghoul(obj); expectCalls("wander", START + "zone(400,32767);zone(500,18000);wander;");
        reset(s); zoneResults[1] = 1; msg_run__5Ghoul(obj); expectCalls("wander far", START + "zone(400,32767);zone(500,18000);v184(16,3);");
    }
    // state 3
    reset(3); msg_run__5Ghoul(obj); expectCalls("3 no link", START + "v184(16,0);");
    reset(3); w32(576, 0x300); wa(0x300 + 12, 0x2800); zoneResults[0] = 1; msg_run__5Ghoul(obj); expectCalls("3 close", START + "zone(30,5000);v184(16,6);");
    reset(3); w32(576, 0x300); wa(0x300 + 12, 0x2800); zoneResults[1] = 1; msg_run__5Ghoul(obj); expectCalls("3 melee range odd roll", START + "zone(30,5000);zone(90,3000);rand;v184(16,16);");
    reset(3); w32(576, 0x300); wa(0x300 + 12, 0x2800); zoneResults[1] = 1; rolls[0] = 1000008; msg_run__5Ghoul(obj);
    expectCalls("3 melee range even roll", START + "zone(30,5000);zone(90,3000);rand;v184(16,17);");
    reset(3); w32(576, 0x300); wa(0x300 + 12, 0x2800); crowdedResult = 1; msg_run__5Ghoul(obj); expectCalls("3 crowded", START + "zone(30,5000);zone(90,3000);crowd(10240,60,30,0);v184(16,14);");
    reset(3); w32(576, 0x300); wa(0x300 + 12, 0x2800); zoneResults[2] = 1; msg_run__5Ghoul(obj); expectCalls("3 face", START + "zone(30,5000);zone(90,3000);crowd(10240,60,30,0);zone(30,32767);turn;");
    reset(3); w32(576, 0x300); wa(0x300 + 12, 0x2800); msg_run__5Ghoul(obj); expectCalls("3 steer", START + "zone(30,5000);zone(90,3000);crowd(10240,60,30,0);zone(30,32767);steer;");
    // state 5
    reset(5); zoneResults[0] = 1; msg_run__5Ghoul(obj); expectCalls("5 close", START + "zone(30,5000);v184(16,6);");
    reset(5); exitResult = 1; msg_run__5Ghoul(obj); expectCalls("5 exit", START + "zone(30,5000);exit;v184(16,3);");
    reset(5); msg_run__5Ghoul(obj); expectCalls("5 stay", START + "zone(30,5000);exit;");
    // state 6 and 17
    reset(6); w32(576, 0x300); advanceResult = 2; w32(256, 5); h16(200, 3); h16(202, 0xfffe); h16(204, 7); msg_run__5Ghoul(obj);
    expectCalls("6 hit", START + "melee(36,3,-2,7,1,null);v184(16,16);");
    reset(6); w32(576, 0x300); advanceResult = 1; msg_run__5Ghoul(obj); expectCalls("6 no hit bit", START + "v184(16,16);");
    reset(6); w32(576, 0x300); advanceResult = 2; h16(294, 8); msg_run__5Ghoul(obj); expectCalls("6 hit with bit 3", START + "melee(36,0,0,0,1,null);turn;");
    reset(6); msg_run__5Ghoul(obj); expectCalls("6 no link", START + "v184(16,16);");
    reset(17); w32(576, 0x300); advanceResult = 2; w32(256, 5); msg_run__5Ghoul(obj); expectCalls("17 hit", START + "melee(80,0,0,0,1,null);"); ok(r32(256) == 4, "17 timer decremented");
    reset(17); w32(256, 1); msg_run__5Ghoul(obj); expectCalls("17 timer expires", START + "v184(16,3);"); ok(r32(256) == 0, "17 timer 0");
    reset(17); w32(256, 1); h16(294, 8); msg_run__5Ghoul(obj); expectCalls("17 bit 3 keeps the timer", START + ""); ok(r32(256) == 1, "17 timer kept with bit 3");
    // states 8, 9, 15
    reset(8); msg_run__5Ghoul(obj); expectCalls("8", START + "v184(16,3);");
    reset(8); h16(294, 0x10); msg_run__5Ghoul(obj); expectCalls("8 bit 4", START);
    reset(9); w32(804, 1); w32(808, 2); msg_run__5Ghoul(obj); expectCalls("9 below", START + "v184(16,0);");
    reset(9); w32(804, 2); w32(808, 2); msg_run__5Ghoul(obj); expectCalls("9 equal", START);
    reset(15); w32(960, 1); w32(808, 2); msg_run__5Ghoul(obj); expectCalls("15 below", START + "shot(288," + H(140) + "," B1 ",40000000,0);v184(16,3);");
    reset(15); w32(960, 2); w32(808, 2); msg_run__5Ghoul(obj); expectCalls("15 equal", START);
    // state 14
    reset(14); msg_run__5Ghoul(obj); expectCalls("14 no link", START + "v184(16,0);");
    reset(14); w32(576, 0x300); wa(0x300 + 12, 0x2800); w32(256, 5); msg_run__5Ghoul(obj); expectCalls("14 timer below", START + "v184(16,3);turn;");
    reset(14); w32(576, 0x300); wa(0x300 + 12, 0x2800); w32(256, 500); msg_run__5Ghoul(obj); expectCalls("14 free", START + "crowd(10240,60,30,1);v184(16,3);turn;");
    reset(14); w32(576, 0x300); wa(0x300 + 12, 0x2800); w32(256, 500); crowdedResult = 1; msg_run__5Ghoul(obj); expectCalls("14 crowded", START + "crowd(10240,60,30,1);turn;");
    // state 16
    reset(16); h16(294, 8); msg_run__5Ghoul(obj); expectCalls("16 idle bit 3", START); ok(r32(968) == 0, "16 w968");
    reset(16); msg_run__5Ghoul(obj); expectCalls("16 idle", START + "v184(16,3);");
    reset(16); advanceResult = 1; h16(294, 8); f32(176, 100.0f); h16(964, 0x1234); msg_run__5Ghoul(obj);
    expectCalls("16 flag 1", START + "icos(4660);isin(4660);move2(50,-200,0);"); ok(r32(968) == 1, "16 sets w968");
    reset(16); advanceResult = 3; w32(968, 7); h16(294, 8); msg_run__5Ghoul(obj); expectCalls("16 flags 1 and 2", START); ok(r32(968) == 0, "16 flag 2 wins");
    reset(16); advanceResult = 5; h16(294, 8); f32(176, 100.0f); obj[800] = 30; msg_run__5Ghoul(obj);
    { float want = 100.0f * (30.0f * 0x1.111110p-6f); std::string w = START + "v184(16,3);icos(0);isin(0);move2(" + num(0.5f * want) + "," + num(-2.0f * want) + ",0);";
      expectCalls("16 flags 1 and 4, byte 30", w); }
    reset(16); advanceResult = 1; h16(294, 8); f32(176, 100.0f); obj[800] = 61; msg_run__5Ghoul(obj); expectCalls("16 byte 61 not scaled", START + "icos(0);isin(0);move2(50,-200,0);");
    reset(16); advanceResult = 1; h16(294, 8); f32(176, 100.0f); obj[800] = 0; msg_run__5Ghoul(obj); expectCalls("16 byte 0 not scaled", START + "icos(0);isin(0);move2(50,-200,0);");
    // state 10
    reset(10); w32(292, 0x00880000u); msg_run__5Ghoul(obj); expectCalls("10 both bits", START + "rand;trans(288," + H(220) + "," B1 ",19);"); ok((r32(12) & 1) == 1 && r32(960) == 100 + 1000007 % 120, "10 fields");
    reset(10); w32(292, 0x00080000u); h16(294, 0); msg_run__5Ghoul(obj); expectCalls("10 one bit", START + "remove;");
    reset(10); h16(294, 0x08); msg_run__5Ghoul(obj); expectCalls("10 bit 3 blocks removal", START);
    reset(10); h16(294, 0x10); w32(960, 5); msg_run__5Ghoul(obj); expectCalls("10 bit 4", START + "rand;trans(288," + H(212) + "," B1 ",35);"); ok(r32(960) == 100 + 1000007 % 120, "10 bit 4 timer");
    reset(10); h16(294, 0x10); w32(960, 500); msg_run__5Ghoul(obj); expectCalls("10 bit 4 timer late", START + "");
    reset(10); h16(294, 0x20); w32(960, 5); h16(56, 0x40); f32(16, 100.0f); f32(20, 200.0f); f32(24, 7.0f); msg_run__5Ghoul(obj);
    expectCalls("10 bit 5", START + "rand;trans(288," + H(68 + 8 * (1000007 % 7)) + ",3f800000,3);icos(64);isin(64);rand;rand;floor(90,240,7,13,29,0,1);");
    ok((r32(12) & 1) == 1 && r32(960) == 190, "10 bit 5 fields");
    reset(10); h16(294, 0x20); w32(960, 500); msg_run__5Ghoul(obj); expectCalls("10 bit 5 timer late, bit 5 blocks removal", START);
    reset(10); h16(294, 0x04); w32(960, 500); msg_run__5Ghoul(obj); expectCalls("10 other bits, timer late", START);
    reset(10); h16(294, 0x04); w32(960, 5); msg_run__5Ghoul(obj); expectCalls("10 other bits, timer early", START + "remove;");
    if (!bad) std::puts("ghoul msg_run ok");
    return bad;
}
