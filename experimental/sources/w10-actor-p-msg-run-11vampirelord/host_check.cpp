// Host contract for msg_run__11VampireLord; not an EE input. 32-bit words are offsets from a host base,
// table calls go through w10_actor_p_host_call, and callees log. The authored function was also
// compared with the original in a private R5900 run (over 23000 randomized scenarios, every state,
// 1970 of 2016 original words covered; the rest are padding, annulled delay slots and a dead search loop).
#define W10_ACTOR_P_HOST 1
#include <cstdio>
#include <cstring>
#include <string>
unsigned char* w10_actor_p_host_base;
#include "msg_run.cpp"

static unsigned char arena[0x8000] __attribute__((aligned(16)));
static unsigned char* const obj = arena + 0x1000;
static const unsigned BLOCK = 0x2000, TABLE = 0x2800, NODE = 0x3000, TARGET = 0x3400, LISTNODE = 0x3800;
signed char cutMode;
float worldDifficulty;
unsigned char skills[256];
char bossMeterName[128];
int bossMeterCurrent;
unsigned char smokeParticle[80];
static char batNames[2][4] = {};
char* batParams[2] = {batNames[0], nullptr};
static std::string calls;
static int zoneResult = 0, advance = 0, rollValue = 1, allocFail = 0, shadowPoints = 3, everyFrame = 0;
static std::string num(double v) { char b[40]; std::snprintf(b, sizeof b, "%g", v); return b; }
static unsigned off(const void* p) { return static_cast<unsigned>(static_cast<const unsigned char*>(p) - arena); }
unsigned w10_actor_p_host_call(unsigned fn, void* adjusted, int n, unsigned long a1, unsigned long) {
    calls += "slot(" + num(fn) + "," + num(off(adjusted) - 0x1000) + "," + num(n) + "," + num(a1) + ");";
    return fn == 100 ? 0x2200u : 0u;
}
extern "C" {
void fxRunWeapon__FPQ24item11descriptionP12_modelHeaderP13fxWeaponStateG6Point3sT1P14AnimationStatefiifP8Matrix34(
    void*, void*, void*, Point3, short, void*, void*, float, int g, int, float, void*) { calls += "fx(" + num(g) + ");"; }
void newDamageHandler__8CreatureP12_modelHeader(void*, void*) { calls += "ndh;"; }
int everyFrame__8CreatureP12_modelHeader(void*, void*) { calls += "ef;"; return everyFrame; }
int objectActivationCheck__8Creaturei(void*, int d) { calls += "oac(" + num(d) + ");"; return 0; }
int creatureAdvanceAnimation__8Creaturef(void*, float r) { calls += "caa(" + num(r) + ");"; return advance; }
void slideBack__8Creaturef(void*, float r) { calls += "slide(" + num(r) + ");"; }
void* lumpFindResource__FPcT0(const char* a, const char* b) { calls += std::string("lump(") + a + "," + b + ");"; return nullptr; }
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void*, void*, float v) { calls += "sound(" + num(v) + ");"; }
void floorAdd__FG6Point3sfii(Point3 p, short, float e, int a, int b) { calls += "floor(" + num(p.z) + "," + num(e) + "," + num(a) + "," + num(b) + ");"; }
void objectRemoveFromRunList__FP10GameObject(void*) { calls += "orl;"; }
unsigned eRand__10GameObject(void*) { calls += "rand;"; return static_cast<unsigned>(rollValue); }
int modelGetNmShadowPoints__FP12_modelHeader(void*) { calls += "nsp;"; return shadowPoints; }
void modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f(void*, void*, Point3, short, int index, Point3* out, float* of, void*, float) {
    calls += "msp(" + num(index) + ");"; *out = Point3{1, 2, 3}; *of = 0;
}
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* d, Point3* p, Point3* v, float t) {
    calls += std::string("add(") + (d == smokeParticle ? "smoke" : "bad") + "," + num(p->x) + ":" + num(p->y) + ":" + num(p->z) + "," + num(v->x) + "," + num(t) + ");";
}
int enemyInZone__8Creaturefs(void*, float r, short a) { calls += "zone(" + num(r) + "," + num(a) + ");"; return zoneResult; }
void smartWander__8Creature(void*) { calls += "wander;"; }
void turnToFaceEnemy__8Creature(void*) { calls += "turn;"; }
void steerAndRouteToEnemy__8Creature(void*) { calls += "steer;"; }
int bExitStatePersueIdle__8Creature(void*) { calls += "bexit;"; return zoneResult; }
void convertToBat__11VampireLord(void*) { calls += "bat;"; }
void convertToLord__11VampireLord(void*) { calls += "lord;"; }
void flee2__8CreatureP10GameObject(void*, void* t) { calls += std::string("flee(") + (t == arena + TARGET ? "target" : "bad") + ");"; }
void move2__8CreatureG6Point3(void*, Point3) { calls += "move2;"; }
int animAdvanceAnimationTime__FP14AnimationStatefP6Point3P6Point2Ps(void*, float t, Point3*, Point2*, short*) { calls += "aat(" + num(t) + ");"; return advance; }
int produceMeleeDamage__8CreaturefiiiiPQ24item11description(void*, float, int, int, int, int flags, void* d) { calls += "pmd(" + num(flags) + (d ? ",desc" : ",null") + ");"; return 0; }
float icos__Fi(int) { calls += "cos;"; return 1.0f; }
float isin__Fi(int) { calls += "sin;"; return 0.0f; }
int iatan2__Fff(float, float) { calls += "atan;"; return 0; }
void* blockAlloc__Fi(int n) { calls += "alloc(" + num(n) + ");"; static unsigned char block[8][64]; static int i; return allocFail ? nullptr : block[i++ & 7]; }
void critter_construct(void*, char* n, Point3, short, char** a) { calls += std::string("critter(") + n + "," + (a == batParams ? "params" : "other") + ");"; }
void soundFadeStream__Fi(int s) { calls += "fade(" + num(s) + ");"; }
void objectAddToDeleteList__FP10GameObject(void*) { calls += "del;"; }
void objectUpdateInGrid__FP10GameObject(void*) { calls += "grid;"; }
void makeExplosion__FG6Point3f(Point3, float s) { calls += "explosion(" + num(s) + ");"; }
unsigned long long modelGetActiveChunks__FP12_modelHeaderi(void*, int w) { calls += "chunks(" + num(w) + ");"; return 0x1122334455667788ull; }
void animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(void*, void*, float r, int f) { calls += "trans(" + num(r) + "," + num(f) + ");"; }
}
static void put32(unsigned at, unsigned v) { std::memcpy(arena + at, &v, 4); }
static void put16(unsigned at, unsigned short v) { std::memcpy(arena + at, &v, 2); }
static unsigned get32(unsigned at) { unsigned v; std::memcpy(&v, arena + at, 4); return v; }
static void reset(int state) {
    std::memset(arena, 0, sizeof arena); calls.clear(); w10_actor_p_host_base = arena;
    zoneResult = advance = allocFail = everyFrame = 0; rollValue = 32768; shadowPoints = 3; cutMode = 0;
    std::memcpy(batNames[0], "fx", 3);
    put32(0x1000 + 36, BLOCK); put32(0x1000 + 72, TABLE); put32(0x1000 + 808, 1000);
    for (unsigned i = 0; i < 0x400; i += 4) put32(BLOCK + i, BLOCK + i);
    put32(TABLE + 128 + 4, 100); put32(TABLE + 168 + 4, 168); put32(TABLE + 184 + 4, 184);
    put16(0x1000 + 572, static_cast<unsigned short>(state));
    put32(0x1000 + 576, NODE); put32(NODE + 12, TARGET);
}
static int bad;
static void expect(const char* name, bool ok) { if (!ok) { std::printf("fail: %s\n  calls %s\n", name, calls.c_str()); bad = 1; } }
static bool has(const char* s) { return calls.find(s) != std::string::npos; }
int main() {
    const std::string pre = "ndh;ef;oac(600);caa(1);slide(0.5);slot(168,0,0,0);";
    reset(2); msg_run__11VampireLord(obj);
    expect("state 2", calls == pre);
    reset(2); everyFrame = 1; msg_run__11VampireLord(obj);
    expect("frame handled", calls == "ndh;ef;");
    reset(23); msg_run__11VampireLord(obj);
    expect("state 23", calls == pre);
    reset(3); advance = 0; msg_run__11VampireLord(obj);
    expect("rate for state 3", calls.find("caa(2);") != std::string::npos);
    reset(2); put32(0x1000 + 980, 1); msg_run__11VampireLord(obj);
    { unsigned short a; std::memcpy(&a, arena + 0x1000 + 1120, 2); expect("rate with 980", has("caa(1.5);") && a == 190); }
    reset(2); put32(0x1000 + 1020, 1); put32(0x1000 + 1016, 2000); msg_run__11VampireLord(obj);
    expect("timer not due", calls == "ndh;ef;oac(600);");
    reset(2); put32(0x1000 + 1020, 1); put32(0x1000 + 1016, 500); msg_run__11VampireLord(obj);
    expect("timer due", calls == "ndh;ef;oac(600);slot(184,0,1,21);caa(1);slide(0.5);slot(168,0,0,0);" && get32(0x1000 + 1020) == 0);
    reset(2); put32(0x1000 + 960, 1); put16(0x1000 + 572, 15); msg_run__11VampireLord(obj);
    expect("weapon effect in the 14..17 band", calls.compare(0, 5, "fx(1)") == 0);
    reset(2); cutMode = 1; advance = 0x80; msg_run__11VampireLord(obj);
    expect("cut sparks", has("lump(fx.lmp,spel_fire_hit.anm);sound(1);rand;floor(1,48,9,0);orl;") && (get32(0x1000 + 12) & 0xa) == 0xa);
    reset(2); put32(0x1000 + 968, 1); shadowPoints = 2; msg_run__11VampireLord(obj);
    expect("smoke", calls.find("nsp;rand;slot(100,0,0,0);msp(0);") != std::string::npos && calls.size() > 400);
    int smoke = 0; for (size_t p = calls.find("add(smoke"); p != std::string::npos; p = calls.find("add(smoke", p + 1)) ++smoke;
    expect("sixteen smoke particles", smoke == 16);
    reset(0); zoneResult = 1; msg_run__11VampireLord(obj);
    expect("state 0 near", calls == pre + "zone(400,32767);slot(184,0,1,3);");
    reset(0); put32(0x1000 + 256, 5000); msg_run__11VampireLord(obj);
    expect("state 0 waits", calls == pre + "zone(400,32767);zone(500,18000);");
    reset(0); put32(0x1000 + 256, 5); msg_run__11VampireLord(obj);
    expect("state 0 leaves", has("zone(500,18000);slot(184,0,1,1);"));
    reset(1); msg_run__11VampireLord(obj);
    expect("state 1", calls == pre + "zone(400,32767);zone(500,18000);wander;");
    reset(8); put16(0x1000 + 294, 0x10); msg_run__11VampireLord(obj);
    expect("state 8 busy", calls == pre);
    reset(8); msg_run__11VampireLord(obj);
    expect("state 8 done", calls == pre + "slot(184,0,1,3);");
    reset(5); zoneResult = 1; msg_run__11VampireLord(obj);
    expect("state 5", calls == pre + "bexit;slot(184,0,1,3);");
    reset(3); put32(0x1000 + 576, 0); msg_run__11VampireLord(obj);
    expect("state 3 no target", calls == "ndh;ef;oac(600);caa(2);slide(0.5);slot(168,0,0,0);slot(184,0,1,0);");
    reset(21); put32(0x1000 + 256, 5); msg_run__11VampireLord(obj);
    { unsigned long long q; std::memcpy(&q, arena + 0x1000 + 552, 8);
      expect("state 21", has("chunks(3);slot(184,0,1,3);") && get32(0x1000 + 960) == 1 && get32(0x1000 + 244) == 3 && q == 0x1122334455667788ull); }
    reset(7); advance = 2; msg_run__11VampireLord(obj);
    { int crits = 0; for (size_t p = calls.find("critter("); p != std::string::npos; p = calls.find("critter(", p + 1)) ++crits;
      expect("state 7 bats", crits == 6 && has("bat;") && has("lord;slot(184,0,1,3);") && !has("flee")); }
    reset(7); advance = 2; allocFail = 1; msg_run__11VampireLord(obj);
    expect("state 7 no memory", !has("critter(") && has("alloc(1088);"));
    reset(7); put32(0x1000 + 256, 5000); msg_run__11VampireLord(obj);
    expect("state 7 flees", has("flee(target);") && !has("lord;"));
    reset(10); advance = 2; put32(0x1000 + 256, 5); msg_run__11VampireLord(obj);
    expect("state 10 ends", has("fade(0);del;") && !has("flee"));
    reset(22); put32(0x1000 + 1028, LISTNODE); put32(LISTNODE + 0, 0);
    msg_run__11VampireLord(obj);
    expect("state 22", has("grid;explosion(4);lump(fx.lmp,spel_we_explarrow_hit.anm);sound(1);rand;trans(0.1,3);") && has("del;") && get32(0x1000 + 256) == 1000 + 32768 % 90 + 90);
    return bad;
}
