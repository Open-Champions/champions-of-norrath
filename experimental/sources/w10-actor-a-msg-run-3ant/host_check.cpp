// Host contract for msg_run__3Ant; not an EE input. 32-bit words in the object are offsets from a
// host base (W10_ACTOR_A_HOST), so the real field offsets are used. Callees are stubs that append
// to a log; each case compares the log with the sequence read from the original's control flow.
// Random values come from a fixed list. The authored object is compared against the original in
// a private R5900 run (1200 mixed scenarios and focused state runs).
#define W10_ACTOR_A_HOST 1
#include <cstdio>
#include <cstring>
#include <string>
unsigned char* w10_actor_a_host_base;
#include "msg_run.cpp"

static unsigned char arena[0x8000] __attribute__((aligned(16)));
static unsigned char* const obj = arena + 0x1000;
static std::string calls;
static unsigned rolls[16]; static int rollIndex;
static bool zones[8]; static int zoneIndex;
static int everyFrameResult, activationResult, exitResult, skillResult;
static unsigned advanceResult;
static std::string num(double v) { char b[40]; std::snprintf(b, sizeof b, "%g", v); return b; }
static unsigned off(const void* p) { return static_cast<unsigned>(static_cast<const unsigned char*>(p) - arena); }

float worldDifficulty = 1.0f;
unsigned skills[64];
unsigned char ant_msg_run_particle_7d7170[80];

void newDamageHandler__8CreatureP12_modelHeader(void*, void* h) { calls += "ndh(" + num(off(h)) + ");"; }
int everyFrame__8CreatureP12_modelHeader(void*, void*) { calls += "ef;"; return everyFrameResult; }
int objectActivationCheck__8Creaturei(void*, int r) { calls += "oac" + num(r) + ";"; return activationResult; }
unsigned creatureAdvanceAnimation__8Creaturef(void*, float a) { calls += "caa" + num(a) + ";"; return advanceResult; }
void slideBack__8Creaturef(void*, float a) { calls += "sb" + num(a) + ";"; }
int enemyInZone__8Creaturefs(void*, float r, short s) { calls += "zone(" + num(r) + "," + num(s) + ");"; return zones[zoneIndex++ & 7]; }
void turnToFaceEnemy__8Creature(void*) { calls += "turn;"; }
void steerAndRouteToEnemy__8Creature(void*) { calls += "steer;"; }
void smartWander__8Creature(void*) { calls += "wander;"; }
int bExitStatePersueIdle__8Creature(void*) { calls += "exit?;"; return exitResult; }
void produceMeleeDamage__8CreaturefiiiiPQ24item11description(void*, float f, int a, int b, int c, int d, void* desc) {
    calls += "melee(" + num(f) + "," + num(a) + "," + num(b) + "," + num(c) + "," + num(d) + ",";
    if (!desc) { calls += "none);"; return; }
    const unsigned char* p = static_cast<const unsigned char*>(desc);
    short code, v1, v2, z; std::memcpy(&code, p + 36, 2); std::memcpy(&v1, p + 38, 2); std::memcpy(&v2, p + 40, 2); std::memcpy(&z, p + 42, 2);
    short second; std::memcpy(&second, p + 44, 2);
    calls += "code" + num(code) + ",v" + num(v1) + "," + num(v2) + ",z" + num(z) + ",next" + num(second) + ",b" + num(p[32]) + num(p[33]) + ");";
}
void move2__8CreatureG6Point3(void*, Point3 p) { calls += "move2(" + num(p.x) + "," + num(p.y) + "," + num(p.z) + ");"; }
unsigned eRand__10GameObject(void*) { calls += "rand;"; return rolls[rollIndex++ & 15]; }
float icos__Fi(int a) { calls += "icos" + num(a) + ";"; return 1.0f; }
float isin__Fi(int a) { calls += "isin" + num(a) + ";"; return 0.5f; }
int iatan2__Fff(float y, float x) { calls += "atan(" + num(y) + "," + num(x) + ");"; return 0; }
void* lumpFindResource__FPcT0(char* a, char* b) { calls += std::string("lump(") + a + "," + b + ");"; return arena + 0x7000; }
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* h, void*, float v) { calls += "sound(" + num(off(h)) + "," + num(v) + ");"; }
void* blockAlloc__Fi(int n) { calls += "alloc" + num(n) + ";"; return arena + 0x6000; }
void poison_projectile_construct(void*, void* map, int a, int b, int c, int d, Point3 p, Point3 v, int heading, float f, void*) {
    calls += "poison(" + num(off(map)) + "," + num(a) + "," + num(b) + "," + num(c) + "," + num(d) + ",p" + num(p.x) + ":" + num(p.y) + ":" + num(p.z) + ",v" + num(v.x) + ":" + num(v.y) + ":" + num(v.z) + ",h" + num(heading) + ",f" + num(f) + ");";
}
void P_AddParticle__FP11ParticleDefP6Point3T1f(void*, Point3* p, Point3* v, float f) {
    calls += "particle(" + num(p->x) + "," + num(p->y) + "," + num(p->z) + ";" + num(v->x) + "," + num(v->y) + "," + num(v->z) + ";" + num(f) + ");";
}
void objectAddToDeleteList__FP10GameObject(void*) { calls += "delete;"; }
void objectRemoveFromRunList__FP10GameObject(void*) { calls += "remove;"; }
void floorAdd__FG6Point3sfii(Point3 p, short a, float b, int c, int d) {
    calls += "floor(" + num(p.x) + "," + num(p.y) + "," + num(p.z) + "," + num(a) + "," + num(b) + "," + num(c) + "," + num(d) + ");";
}
void w10_actor_a_host_call1(unsigned fw, void* object) { calls += "v168(" + num(fw) + "," + num(off(object) - 0x1000) + ");"; }
void w10_actor_a_host_call2(unsigned fw, void* object, int n) { calls += "setState" + num(n) + "(" + num(fw) + "," + num(off(object) - 0x1000) + ");"; }
void w10_actor_a_host_call3(unsigned fw, void* object, int a, int b, int* result) {
    calls += "skill(" + num(fw) + "," + num(off(object)) + "," + num(a) + "," + num(b) + ");"; *result = skillResult;
}

static void put32(unsigned o, unsigned v) { std::memcpy(obj + o, &v, 4); }
static void putA(unsigned a, unsigned v) { std::memcpy(arena + a, &v, 4); }
static void putAf(unsigned a, float v) { std::memcpy(arena + a, &v, 4); }
static void putf(unsigned o, float v) { std::memcpy(obj + o, &v, 4); }
static void put16(unsigned o, unsigned short v) { std::memcpy(obj + o, &v, 2); }
static unsigned get32(unsigned o) { unsigned v; std::memcpy(&v, obj + o, 4); return v; }
static float getf(unsigned o) { float v; std::memcpy(&v, obj + o, 4); return v; }
static void reset(int state) {
    std::memset(arena, 0, sizeof arena);
    w10_actor_a_host_base = arena;
    put32(36, 0x200); putA(0x200 + 12, 0x900);          // resource block and its model header word
    put32(72, 0x800);                                    // table
    { short adj = 4; std::memcpy(arena + 0x800 + 168, &adj, 2); put32(0, 0); }
    { unsigned fw = 111; std::memcpy(arena + 0x800 + 172, &fw, 4); }
    { short adj = 8; std::memcpy(arena + 0x800 + 184, &adj, 2); unsigned fw = 222; std::memcpy(arena + 0x800 + 188, &fw, 4); }
    put16(572, static_cast<unsigned short>(state));
    put32(808, 100); put32(256, 50); put32(804, 10); put32(980, 10);
    putf(28, 5.0f); putf(16, 1.0f); putf(20, 2.0f); putf(24, 3.0f);
    calls.clear(); rollIndex = 0; zoneIndex = 0;
    for (int i = 0; i < 16; ++i) rolls[i] = 32768 + 100 * i;
    for (int i = 0; i < 8; ++i) zones[i] = false;
    everyFrameResult = 0; activationResult = 1; exitResult = 0; advanceResult = 0; skillResult = 10;
}
static int bad;
static void expect(const char* name, const std::string& want) { if (calls != want) { std::printf("fail %s:\n  got  %s\n  want %s\n", name, calls.c_str(), want.c_str()); bad = 1; } }
static const std::string pre = "ndh(2304);ef;oac600;caa1;sb0.5;v168(111,4);";
static std::string st(int n) { return "setState" + num(n) + "(222,8);"; }
int main() {
    reset(0); { zones[0] = true; } put16(572, 0);

    msg_run__3Ant(obj);
    expect("0 near", pre + "zone(400,32767);" + st(3));
    reset(0); msg_run__3Ant(obj);
    expect("0 far, counter below", pre + "zone(400,32767);zone(500,16000);" + st(1));
    reset(0); put32(256, 100); msg_run__3Ant(obj);
    expect("0 far, counter not below", pre + "zone(400,32767);zone(500,16000);");
    reset(1); zones[1] = true; msg_run__3Ant(obj);
    expect("1 second zone", pre + "zone(400,32767);zone(500,16000);" + st(0));
    reset(1); msg_run__3Ant(obj);
    expect("1 wander", pre + "zone(400,32767);zone(500,16000);wander;");
    reset(3); msg_run__3Ant(obj);
    expect("3 no enemy", pre + st(0));
    reset(3); put32(576, 0x300); zones[0] = true; msg_run__3Ant(obj);
    expect("3 close", pre + "zone(25,6000);" + st(6));
    reset(3); put32(576, 0x300); obj[968] = 1; zones[1] = true; msg_run__3Ant(obj);
    expect("3 to 13", pre + "zone(25,6000);zone(270,6000);" + st(13));
    reset(3); put32(576, 0x300); obj[968] = 1; zones[2] = true; msg_run__3Ant(obj);
    expect("3 byte1 face", pre + "zone(25,6000);zone(270,6000);zone(25,32767);turn;");
    reset(3); put32(576, 0x300); msg_run__3Ant(obj);
    expect("3 steer", pre + "zone(25,6000);zone(25,32767);steer;");
    reset(5); zones[0] = true; msg_run__3Ant(obj);
    expect("5 close", pre + "zone(25,6000);" + st(6));
    reset(5); exitResult = 1; msg_run__3Ant(obj);
    expect("5 exit", pre + "zone(25,6000);exit?;" + st(3));
    reset(5); obj[968] = 2; msg_run__3Ant(obj);
    expect("5 byte2", "ndh(2304);ef;oac600;caa1;sb0;v168(111,4);zone(25,6000);");
    reset(9); msg_run__3Ant(obj);
    expect("9 below", pre + st(0));
    reset(9); put32(804, 200); msg_run__3Ant(obj);
    expect("9 not below", pre);
    reset(8); msg_run__3Ant(obj);
    expect("8 flag clear", pre + st(3));
    reset(8); put16(294, 16); msg_run__3Ant(obj);
    expect("8 flag set", pre);
    reset(2); msg_run__3Ant(obj); expect("2", pre);
    reset(14); msg_run__3Ant(obj); expect("14", pre);
    reset(-1); msg_run__3Ant(obj); expect("-1", pre);
    // early exits
    reset(0); everyFrameResult = 1; msg_run__3Ant(obj);
    expect("everyFrame stops", "ndh(2304);ef;");
    reset(0); activationResult = 0; msg_run__3Ant(obj);
    expect("activation stops", "ndh(2304);ef;oac600;");
    // flag F: float +24 up before and down after
    reset(2); put32(160, 0x01000000); putf(972, 0.5f); msg_run__3Ant(obj);
    if (getf(24) != 3.0f) { std::puts("fail: flag F add/subtract is balanced"); bad = 1; }
    reset(2); put32(160, 0x01000000); putf(972, 0.5f); everyFrameResult = 1; msg_run__3Ant(obj);
    if (getf(24) != 3.5f) { std::puts("fail: early exit keeps the added float"); bad = 1; }
    // state 6
    reset(6); msg_run__3Ant(obj);
    expect("6 no enemy", pre + "");
    if (get32(256) != 49) { std::puts("fail 6 decrement"); bad = 1; }
    reset(6); put32(256, 1); msg_run__3Ant(obj);
    expect("6 expires", pre + st(3));
    reset(6); put32(256, 1); put16(294, 8); msg_run__3Ant(obj);
    expect("6 flag keeps the counter", pre);
    if (get32(256) != 1) { std::puts("fail 6 flag"); bad = 1; }
    reset(6); put32(576, 0x300); put32(964, 0); msg_run__3Ant(obj);
    expect("6 enemy, no advance bit", pre + "turn;");
    reset(6); put32(576, 0x300); put32(964, 1); msg_run__3Ant(obj);
    expect("6 enemy, already attacked", pre);
    reset(6); put32(576, 0x300); advanceResult = 1; put16(200, 7); put16(202, 8); put16(204, 9); msg_run__3Ant(obj);
    expect("6 plain attack", pre + "melee(36,7,8,9,8,none);");
    if (get32(964) != 1) { std::puts("fail 6 attack flag"); bad = 1; }
    reset(6); put32(576, 0x300); advanceResult = 1; put32(208, 1); put16(200, 7); put16(202, 8); put16(204, 9);
    skills[13] = 0x1000;
    { unsigned vt = 0x1200; std::memcpy(arena + 0x1000 + 96, &vt, 4); short adj = 16; std::memcpy(arena + 0x1200 + 96, &adj, 2); unsigned fw = 333; std::memcpy(arena + 0x1200 + 100, &fw, 4); }
    skillResult = 10; rolls[0] = 32768;   // unit factor 0.5
    // skills[13] points inside the arena at the object itself; its table entry is at 0x1200 + 96.
    msg_run__3Ant(obj);
    // 10 * 0.19999999 = 1.9999999 times 0.5 truncates to 0, so the rolled value is not raised.
    expect("6 skill attack", pre + "skill(333,4112,0,1);rand;melee(36,7,8,9,8,code21,v10,10,z0,next-1,b255255);");
    reset(6); put32(576, 0x300); advanceResult = 1; put32(208, 1); skills[13] = 0x1000; skillResult = 1000; rolls[0] = 65535;
    { unsigned vt = 0x1200; std::memcpy(arena + 0x1000 + 96, &vt, 4); short adj = 0; std::memcpy(arena + 0x1200 + 96, &adj, 2); unsigned fw = 333; std::memcpy(arena + 0x1200 + 100, &fw, 4); }
    msg_run__3Ant(obj);
    // 1000 * 0.19999999 = 199.99999 times 0.99998474 truncates to 199 -> 1199.
    expect("6 skill attack, big roll", pre + "skill(333,4096,0,1);rand;melee(36,0,0,0,8,code21,v1000,1199,z0,next-1,b255255);");
    // state 10 (not F)
    reset(10); put16(294, 0x80); msg_run__3Ant(obj);
    expect("10 remove", pre + "remove;");
    reset(10); msg_run__3Ant(obj);
    expect("10 nothing", pre);
    reset(10); put32(160, 0x01000000); put16(294, 0x80); msg_run__3Ant(obj);
    expect("10 start countdown", pre);
    if (get32(976) != 1 || get32(256) != 100) { std::puts("fail 10 start fields"); bad = 1; }
    reset(10); put32(160, 0x01000000); put32(976, 1); put32(256, 100 - 241); msg_run__3Ant(obj);
    if (calls.find("delete;") == std::string::npos || calls.find("particle(") == std::string::npos) { std::printf("fail 10 delete+particles: %s\n", calls.c_str()); bad = 1; }
    reset(10); put32(160, 0x01000000); put32(976, 1); put32(256, 100 - 80); msg_run__3Ant(obj);
    expect("10 80 is not enough", pre);
    reset(10); put32(160, 0x01000000); put32(976, 1); put32(256, 100 - 81); advanceResult = 0; msg_run__3Ant(obj);
    {
        int count = 0; for (size_t p = calls.find("particle("); p != std::string::npos; p = calls.find("particle(", p + 1)) ++count;
        if (count != 3 || calls.find("delete;") != std::string::npos) { std::printf("fail 10 three particles: %s\n", calls.c_str()); bad = 1; }
        float expected972 = 0.0f + bits(0x3e199999u);
        if (getf(972) != expected972) { std::puts("fail 10 +972"); bad = 1; }
    }
    reset(10); advanceResult = 1; msg_run__3Ant(obj);
    expect("10 floor splash", pre + "icos0;isin0;rand;rand;floor(2,2.5,3,-32768,15,6,1);");
    reset(10); advanceResult = 1; put32(12, 2); msg_run__3Ant(obj);
    expect("10 splash blocked", pre);
    // state 13
    reset(13); msg_run__3Ant(obj);
    expect("13 no enemy", pre + "move2(0,0,0);" + st(3));
    reset(13); put16(294, 8); msg_run__3Ant(obj);
    expect("13 no enemy, flag", pre + "move2(0,0,0);");
    reset(13); put32(576, 0x300); put16(294, 8); msg_run__3Ant(obj);
    expect("13 enemy, flag, no shot", pre + "move2(0,0,0);turn;");
    reset(13); put32(576, 0x300); msg_run__3Ant(obj);
    expect("13 enemy, no flag", pre + "move2(0,0,0);" + st(3));
    reset(13); put32(576, 0x300); putA(0x300 + 12, 0xa00); putAf(0xa00 + 16, 11.0f); putAf(0xa00 + 20, 2.0f); putAf(0xa00 + 24, 5.0f);
    advanceResult = 1; put16(294, 8); put16(56, 100); msg_run__3Ant(obj);
    expect("13 shot", pre + "move2(0,0,0);icos100;isin100;atan(10,0);rand;icos3868;isin3868;rand;lump(ant.lmp,spel_ant_spit.anm);sound(28672,1);alloc168;rand;"
        "poison(512,15,16,17,19,p11:7:15,v300:150:83,h-16284,f0.751526);turn;");
    return bad;
}
