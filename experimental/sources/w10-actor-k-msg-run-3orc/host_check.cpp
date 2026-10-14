// Host contract for msg_run__3Orc; not an EE input. Words that hold addresses are offsets from a host
// base; table calls and every callee are stubs that append to a log. Each case sets the fields that
// drive one path and compares the log with the sequence stated in msg_run.cpp. The authored function
// is compared against the original in a private R5900 run (3,000 mixed scenarios, 1,246 of 1,267
// original words executed; the rest are padding nops).
#define W10_ACTOR_K_HOST 1
#include <cstdio>
#include <cstring>
#include <string>
unsigned char* w10_actor_k_host_base;
extern "C" {
unsigned skills[64];
unsigned char global_item[32];
}
#include "msg_run.cpp"
alignas(16) static unsigned char arena[0x8000];
static unsigned char* const obj = arena + 0x1000;
static std::string calls;
static unsigned rolls[8]; static int rollIndex;
static int ef_result, oac_result, wander_result, bexit_result, alloc_ok;
static bool zone_true[16]; static int zone_index; static bool zone2_true[16]; static int zone2_index;
static unsigned advance_result;
static std::string num(double v) { char b[40]; std::snprintf(b, sizeof b, "%g", v); return b; }
static unsigned off(const void* p) { return static_cast<unsigned>(static_cast<const unsigned char*>(p) - arena); }
unsigned w10_actor_k_host_call(unsigned fn, void* adjusted, int, unsigned long a1, unsigned long) {
    calls += "e" + num(fn) + "(" + num(off(adjusted) - 0x1000) + "," + num(a1) + ");";
    return 0;
}
extern "C" {
void newDamageHandler__8CreatureP12_modelHeader(void*, void* h) { calls += "ndh(" + num(off(h)) + ");"; }
int everyFrame__8CreatureP12_modelHeader(void*, void*) { calls += "ef;"; return ef_result; }
int objectActivationCheck__8Creaturei(void*, int r) { calls += "oac" + num(r) + ";"; return oac_result; }
unsigned creatureAdvanceAnimation__8Creaturef(void*, float a) { calls += "adv" + num(a) + ";"; return advance_result; }
void slideBack__8Creaturef(void*, float a) { calls += "slide" + num(a) + ";"; }
int commanderValid__3Orc(void*) { calls += "cv;"; return 0; }
void joinCommander__3Orc(void*) { calls += "join;"; }
int enemyInZone__8Creaturefs(void*, float r, short s) { calls += "zone(" + num(r) + "," + num(s) + ");"; return zone_true[zone_index++ & 15]; }
int enemyInZone2__8Creatureffssfi(void*, float a, float b, short c, short d, float e, int f) {
    calls += "zone2(" + num(a) + "," + num(b) + "," + num(c) + "," + num(d) + "," + num(e) + "," + num(f) + ");"; return zone2_true[zone2_index++ & 15];
}
int smartWander__8Creature(void*) { calls += "wander;"; return wander_result; }
void turnToFaceEnemy__8Creature(void*) { calls += "turn;"; }
void steerAndRouteToEnemy__8Creature(void*) { calls += "steer;"; }
void steerAndRouteToPosition__8CreatureG6Point3(void*, Point3 p) { calls += "steerpos(" + num(p.x) + "," + num(p.y) + "," + num(p.z) + ");"; }
void flee2__8CreatureP10GameObject(void*, void* e) { calls += "flee(" + num(off(e)) + ");"; }
int numInRegion__8creatureG6Point3f(Point3, float) { calls += "region;"; return 0; }
int objectIsCrowded__8CreatureP10GameObjectffi(void*, void*, float, float, int) { calls += "crowded;"; return 0; }
int iatan2__Fff(float y, float x) { calls += "atan(" + num(y) + "," + num(x) + ");"; return 4000; }
unsigned eRand__10GameObject(void*) { calls += "rand;"; return rolls[rollIndex++ & 7]; }
float icos__Fi(int a) { calls += "icos" + num(a) + ";"; return 1.0f; }
float isin__Fi(int a) { calls += "isin" + num(a) + ";"; return 0.5f; }
void findEnemyLeadPosition__8CreaturefP6Point3(void*, float r, Point3* p) { calls += "lead(" + num(r) + "," + num(p->x) + "," + num(p->y) + "," + num(p->z) + ");"; }
int bExitStatePersueIdle__8Creature(void*) { calls += "bexit;"; return bexit_result; }
void objectAddToSlowRunList__FP10GameObject(void*) { calls += "slowrun;"; }
void floorAdd__FG6Point3sfii(Point3 p, short a, float b, int c, int d) { calls += "floor(" + num(p.x) + "," + num(p.y) + "," + num(p.z) + "," + num(a) + "," + num(b) + "," + num(c) + "," + num(d) + ");"; }
int sprintf(char* out, const char* f, ...) { calls += std::string("sprintf(") + f + ");"; std::strcpy(out, "swsh"); return 4; }
void* lumpFindResource__FPcT0(char* a, char* b) { calls += std::string("lump(") + a + "," + b + ");"; return arena + 0x7000; }
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void*, void*, float v) { calls += "sound" + num(v) + ";"; }
void* blockAlloc__Fi(int n) { calls += "alloc" + num(n) + ";"; return alloc_ok ? arena + 0x6000 : nullptr; }
void* ISeeDeadPeople__8Creaturef(void*, float) { calls += "dead;"; return nullptr; }
void produceMeleeDamage__8CreaturefiiiiPQ24item11description(void*, float f, int a, int b, int c, int d, void*) {
    calls += "melee(" + num(f) + "," + num(a) + "," + num(b) + "," + num(c) + "," + num(d) + ");";
}
void cold_arrow_construct(void*, Point3 p, short h, void*, int a, void*) { calls += "cold(" + num(p.z) + "," + num(h) + "," + num(a) + ");"; }
void explode_arrow_construct(void*, Point3 p, short h, void*, int a, void*) { calls += "explode(" + num(p.z) + "," + num(h) + "," + num(a) + ");"; }
void fire_arrow_construct(void*, Point3 p, short h, void*, int a, void*) { calls += "fire(" + num(p.z) + "," + num(h) + "," + num(a) + ");"; }
void missile_weapon_construct(void*, int, void*, void*, Point3, Point3, int h, void*, void*, void*, int i1, int i2, int, int, int i5, int i6, void*) {
    calls += "missile(" + num(h) + "," + num(i1) + "," + num(i2) + "," + num(i5) + "," + num(i6) + ");";
}
void poison_rain_construct(void*, int, void*, Point3, void*, void*) { calls += "rain;"; }
}
static void put32(unsigned at, unsigned v) { std::memcpy(arena + at, &v, 4); }
static void put16(unsigned at, short v) { std::memcpy(arena + at, &v, 2); }
static void putf(unsigned at, float v) { std::memcpy(arena + at, &v, 4); }
static unsigned get32(unsigned at) { unsigned v; std::memcpy(&v, arena + at, 4); return v; }
static int bad;
static void setup(int state, unsigned type) {
    std::memset(arena, 0, sizeof arena); calls.clear(); rollIndex = 0; zone_index = zone2_index = 0;
    std::memset(zone_true, 0, sizeof zone_true); std::memset(zone2_true, 0, sizeof zone2_true);
    ef_result = 0; oac_result = 1; wander_result = 1; bexit_result = 0; alloc_ok = 1; advance_result = 0;
    w10_actor_k_host_base = arena;
    put32(0x1000 + 36, 0x2000); put32(0x2000 + 12, 0x2100);
    put32(0x1000 + 72, 0x3000);
    for (unsigned entry : {168u, 176u, 184u}) put32(0x3000 + entry + 4, entry);
    put16(0x1000 + 572, static_cast<short>(state)); put32(0x1000 + 1116, type);
    put32(0x1000 + 576, 0x4000); put32(0x4000 + 12, 0x4400);
    put16(0x1000 + 56, 100);
    putf(0x1000 + 16, 10.0f); putf(0x1000 + 20, 20.0f); putf(0x1000 + 24, 30.0f); putf(0x1000 + 28, 5.0f);
    putf(0x4400 + 16, 110.0f); putf(0x4400 + 20, 120.0f); putf(0x4400 + 24, 130.0f); putf(0x4400 + 28, 8.0f);
}
static void expect(const char* what, const char* want_tail) {
    const std::string want = std::string("ndh(8448);ef;oac600;adv1;slide0.4;e168(0,0);") + want_tail;
    if (calls != want) { std::printf("%s:\n got  %s\n want %s\n", what, calls.c_str(), want.c_str()); bad = 1; }
}
int main() {
    rolls[0] = 30; rolls[1] = 7; rolls[2] = 2; rolls[3] = 5; rolls[4] = 1; rolls[5] = 0; rolls[6] = 9; rolls[7] = 3;
    setup(0, 0); ef_result = 1;
    msg_run__3Orc(obj);
    if (calls != "ndh(8448);ef;") { std::printf("ef: %s\n", calls.c_str()); bad = 1; }
    setup(0, 0); oac_result = 0; msg_run__3Orc(obj);
    if (calls != "ndh(8448);ef;oac600;") { std::printf("oac: %s\n", calls.c_str()); bad = 1; }
    setup(17, 0); msg_run__3Orc(obj); expect("state 17 does nothing", "");
    setup(0, 4); zone2_true[0] = true; msg_run__3Orc(obj);
    expect("state 0 type 4 zone2", "zone2(0,400,100,32767,0,1);e184(0,3);");
    setup(0, 0); put32(0x1000 + 256, 1); put32(0x1000 + 808, 5); msg_run__3Orc(obj);
    expect("state 0 timer", "cv;join;zone(400,32767);zone(500,16000);e184(0,1);");
    setup(0, 1); put32(0x1000 + 256, 9); put32(0x1000 + 808, 5); msg_run__3Orc(obj);
    expect("state 0 type 1 joins the commander", "cv;join;zone(400,32767);zone(500,16000);");
    setup(1, 0); msg_run__3Orc(obj); expect("state 1 wanders", "zone(400,32767);zone(500,16000);wander;");
    setup(2, 0); wander_result = 0; msg_run__3Orc(obj); expect("state 2 stuck", "zone(400,32767);zone(500,16000);wander;e184(0,0);");
    setup(8, 0); msg_run__3Orc(obj); expect("state 8", "e184(0,3);");
    setup(8, 0); put16(0x1000 + 294, 0x10); msg_run__3Orc(obj); expect("state 8 held", "");
    setup(9, 0); put32(0x1000 + 804, 1); put32(0x1000 + 808, 2); msg_run__3Orc(obj); expect("state 9", "e184(0,0);");
    setup(16, 0); put32(0x1000 + 256, 1); put32(0x1000 + 808, 2); msg_run__3Orc(obj); expect("state 16", "e184(0,3);");
    setup(14, 0); put32(0x1000 + 808, 1); put32(0x1000 + 256, 5); msg_run__3Orc(obj); expect("state 14 flee", "flee(17408);");
    setup(14, 0); put32(0x1000 + 808, 1); put32(0x1000 + 256, 5); putf(0x1000 + 272, 3.0f); putf(0x1000 + 276, 4.0f); putf(0x1000 + 280, 5.0f);
    msg_run__3Orc(obj); expect("state 14 steer", "steerpos(3,4,5);");
    setup(14, 0); put32(0x1000 + 808, 9); put32(0x1000 + 256, 5); msg_run__3Orc(obj); expect("state 14 timer done", "e184(0,0);");
    setup(3, 0); put32(0x1000 + 576, 0); msg_run__3Orc(obj); expect("state 3 without an enemy", "e184(0,0);");
    setup(3, 4); msg_run__3Orc(obj); expect("state 3 type 4 steer", "zone(90,32767);zone2(0,400,100,6000,0,1);steer;");
    setup(5, 0); bexit_result = 1; msg_run__3Orc(obj); expect("state 5", "zone(35,6000);bexit;e184(0,3);");
    setup(10, 0); advance_result = 3; put16(0x1000 + 294, 0x80); msg_run__3Orc(obj);
    expect("state 10", "slowrun;icos100;isin100;rand;rand;floor(-20,5,30,30,37,0,1);");
    if (get32(0x1000 + 12) != 1) { std::puts("state 10 flag"); bad = 1; }
    // state 6 melee: damage, then the countdown.
    setup(6, 0); advance_result = 1; put32(0x1000 + 256, 5); put32(0x4400 + 12, 0x40); msg_run__3Orc(obj);
    expect("state 6 melee", "melee(36,0,0,0,8);rand;e184(0,3);"); if (get32(0x1000 + 256) != 4) { std::puts("state 6 countdown"); bad = 1; }
    setup(6, 1); advance_result = 2; put32(0x1000 + 256, 5); put32(0x1000 + 1112, 1); msg_run__3Orc(obj);
    expect("state 6 melee other flag", "melee(36,0,0,0,262152);");
    if (get32(0x1000 + 160) != 0x200) { std::puts("state 6 bit 9"); bad = 1; }
    // state 6 type 4: projectile by kind.
    for (int kind = 0; kind < 4; ++kind) {
        setup(6, 4); advance_result = 1; put32(0x1000 + 208, kind); put32(0x1000 + 256, 5); put16(0x1000 + 200, 11); put16(0x1000 + 202, 12); put16(0x1000 + 204, 13);
        for (int i = 0; i < 64; ++i) skills[i] = 0x100 + i;
        msg_run__3Orc(obj);
        const char* alloc = kind == 2 ? "alloc116;" : (kind == 0 || kind == 4) ? "alloc452;" : "alloc176;";
        if (calls.find(alloc) == std::string::npos) { std::printf("kind %d alloc: %s\n", kind, calls.c_str()); bad = 1; }
        const char* name = kind == 1 ? "cold(" : kind == 2 ? "explode(" : kind == 3 ? "fire(" : "missile(";
        if (calls.find(name) == std::string::npos) { std::printf("kind %d projectile: %s\n", kind, calls.c_str()); bad = 1; }
        if (calls.find("sprintf(swsh_arrow%d.anm);lump(fx.lmp,swsh);sound1;") == std::string::npos) { std::printf("sound: %s\n", calls.c_str()); bad = 1; }
    }
    return bad;
}
