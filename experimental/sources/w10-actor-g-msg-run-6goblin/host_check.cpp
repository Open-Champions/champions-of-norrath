// Host contract for msg_run__6Goblin; not an EE input. Words are offsets from a host base
// (W10_ACTOR_G_HOST); the callee sequence and arguments are checked per state.
#define W10_ACTOR_G_HOST 1
#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <string>
unsigned char* w10g_host_base;
static std::string calls;
static unsigned char arena[0x8000] __attribute__((aligned(16)));
static unsigned char* const obj = arena + 0x1000;
static unsigned char* const tgt = arena + 0x2800;
static unsigned char* const deadObj = arena + 0x3000;
static std::string num(double v) { char b[40]; std::snprintf(b, sizeof b, "%g", v); return b; }
static long off(const void* p) { return static_cast<long>(static_cast<const unsigned char*>(p) - arena); }
static long rel(const void* p) { return off(p) - 0x1000; }
struct P3 { float x, y, z; };
static std::string pt(float x, float y, float z) { return "(" + num(x) + "," + num(y) + "," + num(z) + ")"; }
static void v168(void* o) { calls += "v168(" + num(rel(o)) + ");"; }
struct Pm { float x, y, z; };
static int pmod = -1;
static void v184(void* o, int a) { calls += "v184(" + num(rel(o)) + "," + num(a) + ");"; if (pmod >= 0) std::memcpy(obj + 972, &pmod, 4); }
static void v184dead(void* o, int a) { calls += "dead184(" + num(off(o)) + "," + num(a) + ");"; }
static int skillValues[4] = {7, 5, 3, 2};
static int skillFn(void* o, int which, int level, void* owner) {
    calls += "skill(" + num(off(o)) + "," + num(which) + "," + num(level) + "," + num(rel(owner)) + ");"; return skillValues[which & 3];
}
static void v176(void* o, int a, void* byValuePoint);
void* w10g_host_fn(unsigned w);
#include "msg_run.cpp"
static void v176typed(void* o, int a, Point3 p) { calls += "v176(" + num(rel(o)) + "," + num(a) + "," + pt(p.x, p.y, p.z) + ");"; }
void* w10g_host_fn(unsigned w) {
    switch (w) {
    case 1: return reinterpret_cast<void*>(v168);
    case 2: return reinterpret_cast<void*>(v176typed);
    case 3: return reinterpret_cast<void*>(v184);
    case 4: return reinterpret_cast<void*>(v184dead);
    default: return reinterpret_cast<void*>(skillFn);
    }
}
float worldDifficulty = 2.5f;
unsigned skills[64];
unsigned char globalItem[64];
static int everyResult, activationResult, advanceResult, exitResult, crowdedResult, wanderResult, numResult, atanResult, yawResult;
static int zoneResults[8], zoneIndex, zone2Results[4], zone2Index; static unsigned rolls[8]; static int rollIndex; static float trig[4]; static int trigIndex;
static void* deadResult; static int allocNull;
extern "C" {
void newDamageHandler__8CreatureP12_modelHeader(void*, void* m) { calls += "dmg(" + num(off(m)) + ");"; }
int everyFrame__8CreatureP12_modelHeader(void*, void* m) { calls += "every(" + num(off(m)) + ");"; return everyResult; }
int objectActivationCheck__8Creaturei(void*, int r) { calls += "act" + num(r) + ";"; return activationResult; }
int creatureAdvanceAnimation__8Creaturef(void*, float f) { calls += "adv" + num(f) + ";"; return advanceResult; }
void slideBack__8Creaturef(void*, float f) { calls += "slide" + num(f) + ";"; }
int enemyInZone__8Creaturefs(void*, float f, short s) { calls += "zone(" + num(f) + "," + num(s) + ");"; return zoneResults[zoneIndex++ & 7]; }
int enemyInZone2__8Creatureffssfi(void*, float a, float b, short c, short d, float e, int f) {
    calls += "zone2(" + num(a) + "," + num(b) + "," + num(c) + "," + num(d) + "," + num(e) + "," + num(f) + ");"; return zone2Results[zone2Index++ & 3];
}
int smartWander__8Creature(void*) { calls += "wander;"; return wanderResult; }
void turnToFaceEnemy__8Creature(void*) { calls += "turn;"; }
void steerAndRouteToEnemy__8Creature(void*) { calls += "steer;"; }
int bExitStatePersueIdle__8Creature(void*) { calls += "exit;"; return exitResult; }
void flee2__8CreatureP10GameObject(void*, void* t) { calls += "flee(" + num(off(t)) + ");"; }
int objectIsCrowded__8CreatureP10GameObjectffi(void*, void* other, float a, float b, int c) {
    calls += "crowd(" + num(off(other)) + "," + num(a) + "," + num(b) + "," + num(c) + ");"; return crowdedResult;
}
int numInRegion__8creatureG6Point3f(Point3 p, float r) { calls += "num(" + pt(p.x, p.y, p.z) + "," + num(r) + ");"; return numResult; }
int iatan2__Fff(float y, float x) { calls += "atan(" + num(y) + "," + num(x) + ");"; return atanResult; }
float icos__Fi(int a) { calls += "icos(" + num(a) + ");"; return trig[trigIndex++ & 3]; }
float isin__Fi(int a) { calls += "isin(" + num(a) + ");"; return trig[trigIndex++ & 3]; }
unsigned eRand__10GameObject(void*) { calls += "rand;"; return rolls[rollIndex++ & 7]; }
void produceMeleeDamage__8CreaturefiiiiPQ24item11description(void*, float a, int b, int c, int d, int e, void* p) {
    calls += "melee(" + num(a) + "," + num(b) + "," + num(c) + "," + num(d) + "," + num(e) + "," + num(p ? rel(p) : -1) + ");";
}
void findEnemyLeadPosition__8CreaturefP6Point3(void*, float r, Point3* p) { calls += "lead(" + num(r) + "," + pt(p->x, p->y, p->z) + ");"; p->x += 1; }
int creatureFindLeadYaw__8CreatureP10GameObjectf(void*, void* t, float r) { calls += "yaw(" + num(off(t)) + "," + num(r) + ");"; return yawResult; }
void* ISeeDeadPeople__8Creaturef(void*, float r) { calls += "dead(" + num(r) + ");"; return deadResult; }
void modelGetCharAttachmentPos__FP12_modelHeaderG6Point3sP14AnimationStateiP6Point3T5f(void* m, Point3 p, short a, void* an, int i, Point3* out, Point3* out2, float f) {
    calls += "attach(" + num(off(m)) + "," + pt(p.x, p.y, p.z) + "," + num(a) + "," + num(rel(an)) + "," + num(i) + "," + pt(out->x, out->y, out->z) + "," + (out2 ? "buf" : "null") + "," + num(f) + ");";
    out->y += 0.5f;
}
int sprintf(char* d, const char* f, ...) {
    va_list ap; va_start(ap, f); int v = va_arg(ap, int); va_end(ap);
    char b[64]; std::snprintf(b, sizeof b, f, v); std::strcpy(d, b); calls += std::string("sprintf(") + d + ");"; return 5;
}
void* lumpFindResource__FPcT0(char* l, char* n) { calls += std::string("lump(") + l + "," + n + ");"; return arena + 0x7000; }
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* h, void* o, float v) { calls += "sound(" + num(off(h)) + "," + num(off(o)) + "," + num(v) + ");"; }
void* blockAlloc__Fi(int s) { calls += "alloc" + num(s) + ";"; return allocNull ? nullptr : arena + 0x7800; }
void ColdArrowProjectile_construct(void*, Point3 p, short y, void* o, int a, void* sk) {
    calls += "cold(" + pt(p.x, p.y, p.z) + "," + num(y) + "," + num(rel(o)) + "," + num(a) + "," + num(off(sk)) + ");";
}
void ExplodeArrowProjectile_construct(void*, Point3 p, short y, void* o, int a, void* sk) {
    calls += "explode(" + pt(p.x, p.y, p.z) + "," + num(y) + "," + num(rel(o)) + "," + num(a) + "," + num(off(sk)) + ");";
}
void FireArrowProjectile_construct(void*, Point3 p, short y, void* o, int a, void* sk) {
    calls += "fire(" + pt(p.x, p.y, p.z) + "," + num(y) + "," + num(rel(o)) + "," + num(a) + "," + num(off(sk)) + ");";
}
void MissileWeapon_construct(void*, int a, void* model, void* texture, Point3 p, Point3 v, int yaw, void* owner, void* s1, void* s2, int b, int c, int d, int e, int f, int g, void* desc) {
    calls += "missile(" + num(a) + "," + num(off(model)) + "," + num(off(texture)) + "," + pt(p.x, p.y, p.z) + "," + pt(v.x, v.y, v.z) + "," + num(yaw) + "," + num(rel(owner)) + "," +
             (s1 || s2 ? "snd" : "nosnd") + "," + num(b) + "," + num(c) + "," + num(d) + "," + num(e) + "," + num(f) + "," + num(g) + "," + (desc ? "desc" : "null") + ");";
}
void FrostStormSpell_construct(void*, Point3 p, int a, float b, int c, void* owner, Point3 v) {
    calls += "frost(" + pt(p.x, p.y, p.z) + "," + num(a) + "," + num(b) + "," + num(c) + "," + num(rel(owner)) + "," + pt(v.x, v.y, v.z) + ");";
}
void ShockOfFrost_construct(void*, Point3 p, short y, int a, void* owner, void* sk) {
    calls += "sof(" + pt(p.x, p.y, p.z) + "," + num(y) + "," + num(a) + "," + num(rel(owner)) + "," + num(off(sk)) + ");";
}
void ShockProjectile_construct(void*, Point3 p, short y, void* owner, void* sk, int a) {
    calls += "shockp(" + pt(p.x, p.y, p.z) + "," + num(y) + "," + num(rel(owner)) + "," + num(off(sk)) + "," + num(a) + ");";
}
void floorAdd__FG6Point3sfii(Point3 p, short h, float r, int a, int b) { calls += "floor(" + pt(p.x, p.y, p.z) + "," + num(h) + "," + num(r) + "," + num(a) + "," + num(b) + ");"; }
void objectAddToSlowRunList__FP10GameObject(void*) { calls += "slowrun;"; }
void objectAddToDeleteList__FP10GameObject(void*) { calls += "delete;"; }
}
static int bad;
static void w32(int o, unsigned v) { std::memcpy(obj + o, &v, 4); }
static void wa(int o, unsigned v) { std::memcpy(arena + o, &v, 4); }
static void h16(int o, unsigned short v) { std::memcpy(obj + o, &v, 2); }
static void f32(unsigned char* base, int o, float v) { std::memcpy(base + o, &v, 4); }
static unsigned r32(int o) { unsigned v; std::memcpy(&v, obj + o, 4); return v; }
static float rf32(int o) { float v; std::memcpy(&v, obj + o, 4); return v; }
static short rh16(int o) { short v; std::memcpy(&v, obj + o, 2); return v; }
static void reset(short state) {
    std::memset(arena, 0, sizeof arena); w10g_host_base = arena; calls.clear();
    for (unsigned i = 0; i < 0x200; i += 4) wa(0x200 + i, 0x2000 + i);
    w32(36, 0x200); wa(0x200 + 12, 0x777); w32(72, 0x800);
    for (int e : {168, 176, 184}) { wa(0x800 + e + 4, e == 168 ? 1 : e == 176 ? 2 : 3); short adj = 16; std::memcpy(arena + 0x800 + e, &adj, 2); }
    h16(572, static_cast<unsigned short>(state));
    w32(576, 0); wa(0x300 + 12, 0x2800);
    f32(obj, 16, 100.0f); f32(obj, 20, 200.0f); f32(obj, 24, 7.0f); f32(obj, 28, 10.0f); f32(obj, 32, 4.0f); f32(obj, 224, 0.25f);
    h16(56, 100); h16(200, 3); h16(202, 0xfffe); h16(204, 7);
    w32(808, 1000);
    f32(tgt, 16, 160.0f); f32(tgt, 20, 260.0f); f32(tgt, 24, 17.0f); f32(tgt, 28, 12.0f);
    wa(0x3000 + 72, 0x900); wa(0x900 + 188, 4); { short adj = 8; std::memcpy(arena + 0x900 + 184, &adj, 2); }
    for (int idx : {23, 37, 43, 44, 47, 58}) { skills[idx] = 0x4000 + 0x100 * idx; wa(0x4000 + 0x100 * idx + 96, 0x6000); }
    wa(0x6000 + 104 + 4, 5); { short adj = 4; std::memcpy(arena + 0x6000 + 104, &adj, 2); }
    wa(0x5000, 0); std::memcpy(globalItem + 8, "\x10\x11\0\0\x20\x21\0\0", 8);
    everyResult = 0; activationResult = 1; advanceResult = 0; exitResult = 0; crowdedResult = 0; wanderResult = 0; numResult = 0; atanResult = 0; yawResult = 0x1234;
    zoneIndex = zone2Index = rollIndex = trigIndex = 0; for (int& z : zoneResults) z = 0; for (int& z : zone2Results) z = 0;
    for (int i = 0; i < 8; ++i) rolls[i] = 100 + i;
    trig[0] = 0.5f; trig[1] = -2.0f; trig[2] = 1.0f; trig[3] = 3.0f;
    deadResult = nullptr; allocNull = 0; pmod = -1; worldDifficulty = 2.5f;
}
static const std::string START = "dmg(1911);every(1911);act600;adv1;slide0.5;v168(16);";
static void expectCalls(const char* name, const std::string& want) {
    if (calls != want) { std::printf("fail %s:\n  got  %s\n  want %s\n", name, calls.c_str(), want.c_str()); bad = 1; }
}
static void ok(bool c, const char* what) { if (!c) { std::printf("fail: %s\n", what); bad = 1; } }
static void run() { msg_run__6Goblin(obj); }
int main() {
    reset(0); everyResult = 1; run(); expectCalls("everyFrame stops", "dmg(1911);every(1911);");
    reset(0); activationResult = 0; run(); expectCalls("activation stops", "dmg(1911);every(1911);act600;");
    reset(8); run(); expectCalls("state 8 skips slideBack", "dmg(1911);every(1911);act600;adv1;v168(16);v184(16,3);");
    for (int s : {7, 11, 12, 16, 17, 0x400d, -1}) { reset(s); run(); expectCalls("quiet", START); }
    // state 0, 1, 2
    reset(0); w32(256, 5); run(); expectCalls("0 idle", START + "zone(300,32767);zone(400,16000);v184(16,1);");
    reset(0); w32(256, 5000); run(); expectCalls("0 timer late", START + "zone(300,32767);zone(400,16000);");
    reset(0); zoneResults[1] = 1; run(); expectCalls("0 far", START + "zone(300,32767);zone(400,16000);v184(16,3);");
    reset(0); w32(972, 4); w32(256, 5); run(); expectCalls("0 archer", START + "zone2(0,300,100,32767,0,1);zone2(0,400,100,16000,0,1);zone(300,32767);zone(400,16000);v184(16,1);");
    reset(0); w32(972, 4); zone2Results[0] = 1; run(); expectCalls("0 archer near", START + "zone2(0,300,100,32767,0,1);v184(16,3);");
    reset(0); w32(972, 4); zone2Results[1] = 1; run(); expectCalls("0 archer far", START + "zone2(0,300,100,32767,0,1);zone2(0,400,100,16000,0,1);v184(16,3);");
    reset(1); run(); expectCalls("1", START + "zone(300,32767);zone(400,16000);wander;");
    reset(1); zoneResults[0] = 1; run(); expectCalls("1 near", START + "zone(300,32767);v184(16,0);");
    reset(1); w32(972, 4); zone2Results[1] = 1; run(); expectCalls("1 archer far", START + "zone2(0,300,100,32767,0,1);zone2(0,400,100,16000,0,1);v184(16,0);");
    reset(2); wanderResult = 1; run(); expectCalls("2 wander ok", START + "zone(300,32767);zone(400,16000);wander;");
    reset(2); wanderResult = 0; run(); expectCalls("2 wander zero", START + "zone(300,32767);zone(400,16000);wander;v184(16,0);");
    // state 14, 15, 9, 8
    reset(14); w32(576, 0x300); w32(256, 2000); run(); expectCalls("14 flee", START + "flee(10240);");
    reset(14); w32(256, 2000); run(); expectCalls("14 no link", START + "v184(16,0);");
    reset(14); w32(576, 0x300); w32(256, 500); run(); expectCalls("14 timer ok", START + "v184(16,0);");
    reset(15); w32(256, 5); run(); expectCalls("15", START + "v184(16,3);");
    reset(15); w32(256, 5000); run(); expectCalls("15 late", START);
    reset(9); w32(804, 5); run(); expectCalls("9", START + "v184(16,3);");
    reset(9); w32(804, 5000); run(); expectCalls("9 late", START);
    reset(8); { unsigned long long q = 0; std::memcpy(obj + 288, &q, 8); } run(); expectCalls("8", "dmg(1911);every(1911);act600;adv1;v168(16);v184(16,3);");
    reset(8); { unsigned long long q = 0x0010000000000000ull; std::memcpy(obj + 288, &q, 8); } run(); expectCalls("8 bit 52", "dmg(1911);every(1911);act600;adv1;v168(16);");
    reset(8); { unsigned long long q = 0x0020000000000000ull; std::memcpy(obj + 288, &q, 8); } run(); expectCalls("8 bit 53", "dmg(1911);every(1911);act600;adv1;v168(16);");
    reset(8); { unsigned long long q = 0x0040000000000000ull; std::memcpy(obj + 288, &q, 8); } run(); expectCalls("8 bit 54 does not count", "dmg(1911);every(1911);act600;adv1;v168(16);v184(16,3);");
    // state 3
    reset(3); run(); expectCalls("3 no link", START + "v184(16,0);");
    reset(3); w32(576, 0x300); w32(972, 4); zoneResults[0] = 1; run(); expectCalls("3 archer close", START + "zone(90,32767);v184(16,14);");
    reset(3); w32(576, 0x300); w32(972, 5); zone2Results[0] = 1; run(); expectCalls("3 caster ranged", START + "zone(90,32767);zone2(0,400,100,6000,0,1);v184(16,6);");
    reset(3); w32(576, 0x300); w32(972, 4); run(); expectCalls("3 archer steer", START + "zone(90,32767);zone2(0,400,100,6000,0,1);steer;");
    reset(3); w32(576, 0x300); zoneResults[0] = 1; run();
    expectCalls("3 melee zone", START + "zone(40,6000);v184(16,6);");
    ok(rf32(960) == 160.0f && rf32(964) == 260.0f && rf32(968) == 17.0f, "3 remembers the target position");
    reset(3); w32(576, 0x300); zoneResults[1] = 1; run(); expectCalls("3 turn", START + "zone(40,6000);zone(40,32767);turn;");
    reset(3); w32(576, 0x300); w32(972, 2); crowdedResult = 1; run(); expectCalls("3 crowded", START + "zone(40,6000);zone(40,32767);crowd(10240,400,30,0);v184(16,13);");
    reset(3); w32(576, 0x300); w32(972, 0); rolls[0] = 31; run(); expectCalls("3 steer roll", START + "zone(40,6000);zone(40,32767);rand;steer;");
    reset(3); w32(576, 0x300); w32(972, 0); rolls[0] = 30; numResult = 3; run(); expectCalls("3 crowd of three", START + "zone(40,6000);zone(40,32767);rand;num(" + pt(160, 260, 17) + ",21);steer;");
    reset(3); w32(576, 0x300); w32(972, 0); rolls[0] = 30; numResult = 2; atanResult = 100; f32(tgt, 56 - 56, 0); { short h = 100; std::memcpy(tgt + 56, &h, 2); } run();
    expectCalls("3 angle ok", START + "zone(40,6000);zone(40,32767);rand;num(" + pt(160, 260, 17) + ",21);atan(60,60);v184(16,4);");
    reset(3); w32(576, 0x300); w32(972, 0); rolls[0] = 30; numResult = 2; atanResult = 100 + 16383 + 0; { short h = 100; std::memcpy(tgt + 56, &h, 2); } run();
    expectCalls("3 angle edge in", START + "zone(40,6000);zone(40,32767);rand;num(" + pt(160, 260, 17) + ",21);atan(60,60);v184(16,4);");
    reset(3); w32(576, 0x300); w32(972, 0); rolls[0] = 30; numResult = 2; atanResult = 100 + 16383 + 16383 + 1; { short h = 100; std::memcpy(tgt + 56, &h, 2); } run();
    expectCalls("3 angle out", START + "zone(40,6000);zone(40,32767);rand;num(" + pt(160, 260, 17) + ",21);atan(60,60);steer;");
    reset(3); w32(576, 0x300); w32(972, 0); rolls[0] = 30; numResult = 2; atanResult = 100 - 16383; { short h = 100; std::memcpy(tgt + 56, &h, 2); } run();
    expectCalls("3 angle low edge", START + "zone(40,6000);zone(40,32767);rand;num(" + pt(160, 260, 17) + ",21);atan(60,60);v184(16,4);");
    reset(3); w32(576, 0x300); w32(972, 0); rolls[0] = 30; numResult = 2; atanResult = 100 - 16384; { short h = 100; std::memcpy(tgt + 56, &h, 2); } run();
    expectCalls("3 angle below", START + "zone(40,6000);zone(40,32767);rand;num(" + pt(160, 260, 17) + ",21);atan(60,60);steer;");
    // state 4
    reset(4); run(); expectCalls("4 no link", START + "v184(16,0);");
    reset(4); w32(576, 0x300); zoneResults[0] = 1; run(); expectCalls("4 close", START + "zone(40,6000);v184(16,6);");
    reset(4); w32(576, 0x300); rolls[0] = 31; run(); expectCalls("4 steer", START + "zone(40,6000);rand;steer;");
    reset(4); w32(576, 0x300); rolls[0] = 30; f32(obj, 960, 160.0f); f32(obj, 964, 260.0f); run(); expectCalls("4 still target", START + "zone(40,6000);rand;v184(16,3);");
    reset(4); w32(576, 0x300); rolls[0] = 30; f32(obj, 960, 159.0f); f32(obj, 964, 260.0f); run(); expectCalls("4 moved target", START + "zone(40,6000);rand;steer;");
    reset(4); w32(576, 0x300); rolls[0] = 30; f32(obj, 960, 160.0f); f32(obj, 964, 259.5f); run(); expectCalls("4 moved half", START + "zone(40,6000);rand;v184(16,3);");
    ok(rf32(960) == 160.0f && rf32(964) == 260.0f && rf32(968) == 17.0f, "4 remembers the target position");
    // state 13
    reset(13); run(); expectCalls("13 no link", START + "v184(16,0);");
    reset(13); w32(576, 0x300); w32(972, 5); zoneResults[0] = 1; run(); expectCalls("13 caster near", START + "zone(90,32767);v184(16,14);turn;");
    reset(13); w32(576, 0x300); w32(972, 5); w32(976, 5); run(); expectCalls("13 caster timer", START + "zone(90,32767);v184(16,3);turn;");
    reset(13); w32(576, 0x300); w32(972, 5); w32(976, 5000); run(); expectCalls("13 caster waits", START + "zone(90,32767);turn;");
    reset(13); w32(576, 0x300); w32(972, 0); w32(256, 5); run(); expectCalls("13 timer", START + "v184(16,3);turn;");
    reset(13); w32(576, 0x300); w32(972, 0); w32(256, 5000); run(); expectCalls("13 free", START + "crowd(10240,400,30,1);v184(16,3);turn;");
    reset(13); w32(576, 0x300); w32(972, 0); w32(256, 5000); crowdedResult = 1; run(); expectCalls("13 crowded", START + "crowd(10240,400,30,1);turn;");
    // state 5
    reset(5); run(); expectCalls("5 no link", START + "v184(16,3);");
    reset(5); w32(576, 0x300); w32(972, 4); zone2Results[0] = 1; run(); expectCalls("5 archer", START + "zone2(0,400,100,6000,0,1);v184(16,6);exit;");
    reset(5); w32(576, 0x300); w32(972, 4); zone2Results[0] = 1; pmod = 0; exitResult = 1; run();
    expectCalls("5 P changes", START + "zone2(0,400,100,6000,0,1);v184(16,6);zone(40,6000);exit;v184(16,3);");
    reset(5); w32(576, 0x300); w32(972, 0); zoneResults[0] = 1; run(); expectCalls("5 melee close", START + "zone(40,6000);v184(16,6);");
    reset(5); w32(576, 0x300); w32(972, 0); exitResult = 1; run(); expectCalls("5 exit", START + "zone(40,6000);exit;v184(16,3);");
    // state 10
    reset(10); w32(1136, 0); run(); expectCalls("10 quiet", START);
    reset(10); h16(294, 0x80); w32(1136, 0); run(); expectCalls("10 slow run list", START + "slowrun;");
    reset(10); advanceResult = 1; w32(1136, 0); rolls[0] = 0x10005; rolls[1] = 17; run(); expectCalls("10 floor", "dmg(1911);every(1911);act600;adv1;slide0.5;v168(16);rand;rand;floor(" + pt(100, 200, 7) + ",5,27,0,1);");
    reset(10); advanceResult = 1; w32(12, 2); w32(1136, 0); run(); expectCalls("10 floor blocked by bit 1", START);
    reset(10); advanceResult = 2; w32(1136, 0); run(); ok((r32(12) & 1) == 1, "10 sets bit 0 for F bit 1");
    reset(10); advanceResult = 2; w32(12, 2); w32(1136, 0); run(); ok(r32(12) == 2, "10 keeps bit 0 clear when bit 1 is set");
    reset(10); w32(1136, 1); h16(294, 0x80); f32(obj, 1140, 1.0f); f32(obj, 24, 7.0f); run();
    expectCalls("10 sinking", START + "slowrun;"); ok(rf32(1140) > 1.34f && rf32(1140) < 1.36f && rf32(60) < 5.7f && (r32(12) & 0x4002) == 0, "10 sink step");
    reset(10); w32(1136, 1); h16(294, 0x80); f32(obj, 1140, 23.7f); run();
    expectCalls("10 sunk", START + "slowrun;delete;"); ok((r32(12) & 0x4002) == 0x4002, "10 flags");
    reset(10); w32(1136, 1); h16(294, 0); f32(obj, 1140, 1.0f); run(); expectCalls("10 not sinking without bit 7", START); ok(rf32(1140) == 1.0f, "10 untouched");
    // state 6: shared timer logic and melee
    reset(6); w32(256, 1); run(); expectCalls("6 no link", START + "v184(16,3);"); ok(r32(256) == 0, "6 no link decrements");
    reset(6); w32(256, 5000); run(); expectCalls("6 timer running", START); ok(r32(256) == 4999, "6 decrement");
    reset(6); h16(294, 8); w32(256, 0); run(); expectCalls("6 bit 3 keeps the timer", START + "v184(16,3);"); ok(r32(256) == 0, "6 timer kept");
    reset(6); w32(256, 1); w32(972, 5); rolls[0] = 7; run(); expectCalls("6 caster timer", START + "rand;v184(16,13);"); ok(r32(976) == 1000 + 7 % 180 + 180, "6 caster w976");
    reset(6); w32(576, 0x300); w32(972, 0); advanceResult = 2; w32(256, 5000); w32(12, 0); run();
    expectCalls("6 melee hit", "dmg(1911);every(1911);act600;adv1;slide0.5;v168(16);melee(36,3,-2,7,262152,1064);");
    reset(6); w32(576, 0x300); w32(972, 0); advanceResult = 3; w32(256, 5000); f32(tgt, 12 - 12, 0); wa(0x2800 + 12, 0x40); rolls[0] = 10; run();
    expectCalls("6 melee on armed target", START + "melee(36,3,-2,7,8,1064);rand;v184(16,3);");
    reset(6); w32(576, 0x300); w32(972, 0); advanceResult = 1; w32(256, 5000); wa(0x2800 + 12, 0x40); rolls[0] = 11; run();
    expectCalls("6 melee no roll hit", START + "melee(36,3,-2,7,8,1064);rand;");
    reset(6); w32(576, 0x300); w32(972, 0); advanceResult = 0; w32(256, 5000); obj[980] = 1; run();
    { unsigned long long q; std::memcpy(&q, obj + 160, 8); ok(q == 0x200ull, "6 melee sets bit 9 with byte 980"); }
    reset(6); w32(576, 0x300); w32(972, 0); advanceResult = 0; w32(256, 5000); w32(992, 1); run(); expectCalls("6 melee Q turns", START + "turn;");
    reset(6); w32(576, 0x300); w32(972, 0); advanceResult = 0; w32(256, 5000); h16(294, 8); run(); expectCalls("6 melee bit 3 turns", START + "turn;");
    // state 6 archer
    for (int kind : {0, 1, 2, 3}) {
        reset(6); w32(576, 0x300); w32(972, 4); advanceResult = 1; w32(256, 5000); w32(208, kind); yawResult = 0x1234;
        run();
        const std::string head = START + "icos(100);isin(100);lead(400," + pt(250, -400, 7) + ");attach(1911," + pt(100, 200, 7) + ",100,288,0," + pt(100, 200, 21) +
                                 ",buf,0.25);yaw(10240,400);icos(4660);isin(4660);rand;rand;sprintf(swsh_arrow2.anm);lump(fx.lmp,swsh_arrow2.anm);sound(28672,4096,1);";
        const std::string pos = pt(100, 200.5f, 21);
        std::string tail;
        if (kind == 2) tail = "alloc116;explode(" + pos + ",4660,0,2," + num(0x4000 + 0x100 * 47) + ");";
        else if (kind == 1) tail = "alloc176;cold(" + pos + ",4660,0,2," + num(0x4000 + 0x100 * 44) + ");";
        else if (kind == 3) tail = "alloc176;fire(" + pos + ",4660,0,2," + num(0x4000 + 0x100 * 43) + ");";
        else tail = "alloc452;missile(0,4368,8480," + pos + "," + pt(400, 1200, 10) + ",-11724,0,nosnd,3,-2,0,0,7,4,null);";
        expectCalls("archer", head + tail);
        ok(r32(996) == 0, "archer clears word 996 before the spawn");
    }
    reset(6); w32(576, 0x300); w32(972, 4); advanceResult = 2; w32(256, 5000); run(); ok(r32(996) == 1, "archer sets word 996 for F bit 1");
    reset(6); w32(576, 0x300); w32(972, 4); advanceResult = 1; w32(256, 5000); allocNull = 1; w32(208, 1); run();
    ok(calls.find("cold(") == std::string::npos && calls.find("alloc176;") != std::string::npos, "archer null allocation constructs nothing");
    reset(6); w32(576, 0x300); w32(972, 4); advanceResult = 0; w32(256, 5000); h16(294, 8); run();
    expectCalls("archer without F bit 0 calls 176", START + "icos(100);isin(100);lead(400," + pt(100 + 0.5f * 300, 200 + -2.0f * 300, 7) + ");v176(16,0," + pt(100 + 0.5f * 300 + 1, 200 + -2.0f * 300, 7) + ");");
    // state 6 caster
    for (int pick : {0, 1, 2, 3}) {
        reset(6); w32(576, 0x300); w32(972, 5); advanceResult = 1; w32(256, 5000); rolls[0] = pick; deadResult = pick == 3 ? deadObj : nullptr; yawResult = 77;
        if (pick == 3) { rolls[0] = 0; rolls[1] = 0; }   // sets pick through the dead-person roll
        if (pick == 3) deadResult = deadObj;
        run();
        const size_t at = calls.find("dead(300);");
        if (pick == 3) {
            ok(at != std::string::npos && calls.find("dead184(12296,16);") != std::string::npos, "caster dead person heal");
            ok(calls.find("lump(fx.lmp,spel_he_heal.anm);sound(28672,12288,1);") != std::string::npos, "caster heal sound");
            ok(rh16(146) == 50 && rh16(150) == 128 && rh16(152) == 255 && rh16(154) == 150 && rh16(140) == 0 && rh16(142) == 128 && rh16(144) == 255 && rh16(148) == 0, "caster heal lights");
        } else if (pick == 0) {
            ok(calls.find("skill(") != std::string::npos && calls.find("alloc172;frost(") != std::string::npos, "caster frost storm");
        } else if (pick == 1) {
            ok(calls.find("yaw(10240,350);alloc208;sof(" + pt(100, 200, 7 + 2) + ",77,2,0," + num(0x4000 + 0x100 * 23) + ");") != std::string::npos, "caster shock of frost");
            ok(rh16(140) == 0 && rh16(142) == 128 && rh16(144) == 255 && rh16(146) == 50 && rh16(148) == 0 && rh16(150) == 128 && rh16(152) == 255 && rh16(154) == 150, "shock of frost lights");
        } else {
            ok(calls.find("yaw(10240,340);alloc408;shockp(" + pt(100, 200, 7 + 2) + ",77,0," + num(0x4000 + 0x100 * 37) + ",2);") != std::string::npos, "caster shock projectile");
            ok(rh16(140) == 80 && rh16(142) == 128 && rh16(144) == 255 && rh16(146) == 50 && rh16(148) == 80 && rh16(150) == 128 && rh16(152) == 255 && rh16(154) == 150, "shock projectile lights");
        }
    }
    reset(6); w32(576, 0x300); w32(972, 5); advanceResult = 1; w32(256, 5000); rolls[0] = 0; allocNull = 1; run(); ok(calls.find("frost(") == std::string::npos, "caster null allocation");
    reset(6); w32(576, 0x300); w32(972, 5); advanceResult = 0; w32(256, 5000); h16(294, 8); run();
    ok(calls.find("v176(16,0,") != std::string::npos && calls.find("alloc") == std::string::npos, "caster without F bit 0 only calls 176");
    if (!bad) std::puts("goblin msg_run ok");
    return bad;
}
