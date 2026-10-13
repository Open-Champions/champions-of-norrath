// Host contract for msg_run__6Cthulu; not an EE input. Pointer words are offsets from a host base
// (W10_ACTOR_D_HOST); callees are logging stubs with scripted results.
#define W10_ACTOR_D_HOST 1
#include <cstdio>
#include <cstring>
#include <string>
unsigned char* w10_actor_d_host_base;
typedef void (*HostFn)(void*, int);
HostFn w10_actor_d_host_functions[4];
extern "C" { unsigned char skills[512]; float worldDifficulty; }
static std::string calls;
int w10_actor_d_skill_function(unsigned function, void* skill, int a, int b) {
    calls += "skill(" + std::to_string(function) + "," + std::to_string(static_cast<unsigned char*>(skill) - w10_actor_d_host_base) + "," + std::to_string(a) + "," + std::to_string(b) + ");";
    return 0x12345;
}
#include "msg_run.cpp"
static unsigned char* objectUnderTest;
static int everyResult, activationResult, advanceResult, persueResult, rollIndex, iatanResult, allocFails;
static int zoneResults[8], zoneIndex, rolls[16];
static std::string f(float v) { char b[32]; std::snprintf(b, sizeof b, "%g", v); return b; }
void newDamageHandler__8CreatureP12_modelHeader(void*, void* m) { calls += "dmg(" + std::to_string(static_cast<unsigned char*>(m) - w10_actor_d_host_base) + ");"; }
int everyFrame__8CreatureP12_modelHeader(void*, void* m) { calls += "every(" + std::to_string(static_cast<unsigned char*>(m) - w10_actor_d_host_base) + ");"; return everyResult; }
int objectActivationCheck__8Creaturei(void*, int n) { calls += "act(" + std::to_string(n) + ");"; return activationResult; }
int creatureAdvanceAnimation__8Creaturef(void*, float v) { calls += "adv(" + f(v) + ");"; return advanceResult; }
int slideBack__8Creaturef(void*, float v) { calls += "slide(" + f(v) + ");"; return 0x5a5a5a5a; }
int enemyInZone__8Creaturefs(void*, float h, short d) { calls += "zone(" + f(h) + "," + std::to_string(d) + ");"; return zoneResults[zoneIndex++ % 8]; }
void smartWander__8Creature(void*) { calls += "wander;"; }
void turnToFaceEnemy__8Creature(void*) { calls += "turn;"; }
void steerAndRouteToEnemy__8Creature(void*) { calls += "steer;"; }
int bExitStatePersueIdle__8Creature(void*) { calls += "persue;"; return persueResult; }
void move2__8CreatureG6Point3(void*, const Point3* p) { calls += "move2(" + f(p->x) + f(p->y) + f(p->z) + ");"; }
int eRand__10GameObject(void*) { calls += "rand;"; return rolls[rollIndex++ % 16]; }
float icos__Fi(int a) { calls += "cos(" + std::to_string(a) + ");"; return 0.5f; }
float isin__Fi(int a) { calls += "sin(" + std::to_string(a) + ");"; return -0.25f; }
int iatan2__Fff(float a, float b) { calls += "atan(" + f(a) + "," + f(b) + ");"; return iatanResult; }
void findEnemyLeadPosition__8CreaturefP6Point3(void*, float v, Point3* p) { calls += "lead(" + f(v) + "," + f(p->x) + "," + f(p->y) + "," + f(p->z) + ");"; }
static std::string d(void* description) {
    const unsigned char* p = static_cast<const unsigned char*>(description);
    std::string s = "D[";
    for (int i = 0; i < 4; ++i) { short a, b, c, e; std::memcpy(&a, p + 36 + 8 * i, 2); std::memcpy(&b, p + 38 + 8 * i, 2); std::memcpy(&c, p + 40 + 8 * i, 2); std::memcpy(&e, p + 42 + 8 * i, 2); s += std::to_string(a) + (a == -1 ? "" : ":" + std::to_string(b) + ":" + std::to_string(c) + ":" + std::to_string(e)) + ","; }
    return s + std::to_string(p[32]) + std::to_string(p[33]) + "]";
}
void produceMeleeDamage__8CreaturefiiiiPQ24item11description(void*, float a, int b, int c, int e, int g, void* desc) {
    calls += "melee(" + f(a) + "," + std::to_string(b) + "," + std::to_string(c) + "," + std::to_string(e) + "," + std::to_string(g) + "," + d(desc) + ");";
}
void objectRemoveFromRunList__FP10GameObject(void*) { calls += "rmrun;"; }
void floorAdd__FG6Point3sfii(const Point3* p, short a, float b, int c, int e) { calls += "floor(" + f(p->x) + "," + std::to_string(a) + "," + f(b) + "," + std::to_string(c) + "," + std::to_string(e) + ");"; }
static unsigned char block[200];
void* blockAlloc__Fi(int n) { calls += "alloc(" + std::to_string(n) + ");"; return allocFails ? nullptr : block; }
void poisonProjectile_construct(void*, void* map, int a, int b, int c, int e, const Point3* at, const Point3* v, int angle, float scale, void* creature) {
    calls += "poison(" + std::to_string(static_cast<unsigned char*>(map) - w10_actor_d_host_base) + "," + std::to_string(a) + "," + std::to_string(b) + "," + std::to_string(c) + "," + std::to_string(e) + ",(" + f(at->x) + "," + f(at->y) + "," + f(at->z) + "),(" + f(v->x) + "," + f(v->y) + "," + f(v->z) + ")," + std::to_string(angle) + "," + f(scale) + "," + std::to_string(creature == objectUnderTest) + ");";
}
static void setState(void* s, int v) { calls += "state(" + std::to_string(static_cast<unsigned char*>(s) - objectUnderTest) + "," + std::to_string(v) + ");"; }
static void slot168(void*, int) { calls += "slot168;"; }
static int failures;
static void expect(bool ok, const char* what) { if (!ok) { std::printf("fail: %s\n  calls: %s\n", what, calls.c_str()); ++failures; } }
template <typename T> static T get(unsigned o) { T v; std::memcpy(&v, objectUnderTest + o, sizeof v); return v; }
template <typename T> static void put(unsigned o, T v) { std::memcpy(objectUnderTest + o, &v, sizeof v); }
int main() {
    alignas(16) static unsigned char arena[0x20000];
    w10_actor_d_host_base = arena; w10_actor_d_host_functions[1] = setState; w10_actor_d_host_functions[2] = slot168;
    unsigned char* object = arena + 0x8000; objectUnderTest = object;
    auto setup = [&](short state) {
        std::memset(arena, 0, sizeof arena); put<unsigned>(36, 300u); { unsigned h = 900; std::memcpy(arena + 300 + 12, &h, 4); }
        put<short>(572, state); unsigned table = 4096; put<unsigned>(72, table);
        short adj = 0; unsigned fn = 1; std::memcpy(arena + table + 184, &adj, 2); std::memcpy(arena + table + 188, &fn, 4);
        fn = 2; std::memcpy(arena + table + 168, &adj, 2); std::memcpy(arena + table + 172, &fn, 4);
        put<float>(16, 10.0f); put<float>(20, 20.0f); put<float>(24, 30.0f); put<float>(28, 5.0f); put<short>(56, 100);
        put<int>(808, 100); put<int>(256, 500); put<int>(968, 500); put<int>(804, 500);
        unsigned skill = 6000, skillTable = 7000; std::memcpy(skills + 104, &skill, 4); std::memcpy(arena + skill + 96, &skillTable, 4);
        short skillAdj = 4; unsigned skillFn = 77; std::memcpy(arena + skillTable + 96, &skillAdj, 2); std::memcpy(arena + skillTable + 100, &skillFn, 4);
        worldDifficulty = 3.0f; put<short>(200, 7); put<short>(202, -8); put<short>(204, 9);
        calls.clear(); everyResult = 0; activationResult = 1; advanceResult = 0; persueResult = 0; rollIndex = 0; iatanResult = 0; allocFails = 0; zoneIndex = 0;
        for (int& z : zoneResults) z = 0;
        for (int i = 0; i < 16; ++i) rolls[i] = 1;
    };
    const std::string head = "dmg(900);every(900);act(600);adv(1);slide(0.5);slot168;";
    setup(0); everyResult = 1; msg_run__6Cthulu(object); expect(calls == "dmg(900);every(900);", "everyFrame stops the run");
    setup(0); activationResult = 0; msg_run__6Cthulu(object); expect(calls == "dmg(900);every(900);act(600);", "activation check stops the run");
    for (short s : {2, 4, 7, 11, 12, 17, 100, -1}) { setup(s); msg_run__6Cthulu(object); expect(calls == head, "states without a case"); }
    // 0 and 1
    setup(0); zoneResults[0] = 1; msg_run__6Cthulu(object); expect(calls == head + "zone(400,32767);state(0,3);", "state 0 enemy in zone");
    setup(0); put<int>(256, 50); msg_run__6Cthulu(object); expect(calls == head + "zone(400,32767);state(0,1);", "state 0 timed out");
    setup(0); msg_run__6Cthulu(object); expect(calls == head + "zone(400,32767);", "state 0 waits");
    setup(1); zoneResults[0] = 1; msg_run__6Cthulu(object); expect(calls == head + "zone(400,32767);state(0,3);", "state 1 enemy");
    setup(1); put<int>(256, 50); msg_run__6Cthulu(object); expect(calls == head + "zone(400,32767);state(0,0);", "state 1 timed out");
    setup(1); msg_run__6Cthulu(object); expect(calls == head + "zone(400,32767);wander;", "state 1 wanders");
    // 3
    setup(3); msg_run__6Cthulu(object); expect(calls == head + "state(0,0);", "state 3 with an empty list");
    setup(3); put<int>(576, 1); put<int>(964, 1); zoneResults[0] = 1; msg_run__6Cthulu(object); expect(calls == head + "zone(45,5000);state(0,6);", "state 3 melee range with +964");
    setup(3); put<int>(576, 1); put<int>(964, 1); put<int>(968, 50); zoneResults[0] = 0; zoneResults[1] = 1; msg_run__6Cthulu(object);
    expect(calls == head + "zone(45,5000);zone(305,5000);state(0,16);", "state 3 with +964 and the far zone");
    setup(3); put<int>(576, 1); zoneResults[0] = 1; zoneResults[1] = 1; rolls[0] = 4; msg_run__6Cthulu(object); expect(calls == head + "zone(45,5000);zone(45,5000);rand;state(0,13);", "state 3 roll 4 % 3 = 1");
    setup(3); put<int>(576, 1); zoneResults[0] = 1; zoneResults[1] = 1; rolls[0] = 3; msg_run__6Cthulu(object); expect(calls.find("rand;state(0,6);") != std::string::npos, "roll % 3 == 0");
    setup(3); put<int>(576, 1); zoneResults[0] = 1; zoneResults[1] = 1; rolls[0] = 5; msg_run__6Cthulu(object); expect(calls.find("rand;state(0,14);") != std::string::npos, "roll % 3 == 2");
    setup(3); put<int>(576, 1); zoneResults[0] = 0; zoneResults[1] = 0; zoneResults[2] = 1; msg_run__6Cthulu(object);
    expect(calls == head + "zone(45,5000);zone(45,5000);zone(105,5000);state(0,15);", "state 3 mid range");
    setup(3); put<int>(576, 1); put<int>(972, 1); zoneResults[0] = 0; zoneResults[1] = 0; put<int>(968, 50); zoneResults[2] = 1; msg_run__6Cthulu(object);
    expect(calls == head + "zone(45,5000);zone(45,5000);zone(305,5000);state(0,16);", "state 3 skips the mid range test with +972");
    setup(3); put<int>(576, 1); msg_run__6Cthulu(object); expect(calls == head + "zone(45,5000);zone(45,5000);zone(105,5000);zone(45,32767);steer;", "state 3 steers");
    setup(3); put<int>(576, 1); zoneResults[3] = 1; msg_run__6Cthulu(object); expect(calls == head + "zone(45,5000);zone(45,5000);zone(105,5000);zone(45,32767);turn;", "state 3 turns");
    // 5
    setup(5); zoneResults[0] = 1; msg_run__6Cthulu(object); expect(calls == head + "zone(45,5000);state(0,6);", "state 5 near");
    setup(5); zoneResults[1] = 1; msg_run__6Cthulu(object); expect(calls == head + "zone(45,5000);zone(305,5000);state(0,16);", "state 5 far");
    setup(5); persueResult = 1; msg_run__6Cthulu(object); expect(calls == head + "zone(45,5000);zone(305,5000);persue;state(0,3);", "state 5 exits");
    setup(5); persueResult = 0; msg_run__6Cthulu(object); expect(calls == head + "zone(45,5000);zone(305,5000);persue;", "state 5 stays");
    // 6, 13, 14, 15
    setup(6); put<int>(576, 1); put<int>(964, 1); advanceResult = 0; msg_run__6Cthulu(object); expect(calls == head + "move2(000);rand;state(0,3);" && get<int>(960) == 0, "state 6 without S bit 0");
    setup(6); put<int>(576, 1); advanceResult = 1; rolls[0] = 10; rolls[1] = 12; msg_run__6Cthulu(object);
    expect(calls == head + "move2(000);rand;skill(77,6004,0,3);melee(36,7,-8,9,65544,D[23:9029:9029:0,-1,-1,-1,255255]);state(0,3);", "state 6 melee with the special roll");
    setup(6); put<int>(576, 1); advanceResult = 1; rolls[0] = 11; msg_run__6Cthulu(object); expect(calls == head + "move2(000);rand;melee(36,7,-8,9,65544,D[-1,-1,-1,-1,255255]);state(0,3);", "state 6 melee without the special roll");
    setup(6); put<int>(576, 1); put<unsigned short>(294, 8); msg_run__6Cthulu(object); expect(calls == head + "move2(000);turn;", "state 6 turning keeps the state");
    setup(6); put<unsigned short>(294, 8); msg_run__6Cthulu(object); expect(calls == head + "move2(000);", "state 6, empty list, bit 8");
    setup(6); msg_run__6Cthulu(object); expect(calls == head + "move2(000);state(0,3);", "state 6 plain");
    setup(13); put<int>(576, 1); advanceResult = 1; rolls[0] = 10; msg_run__6Cthulu(object); expect(calls == head + "rand;skill(77,6004,0,3);melee(36,7,-8,9,65544,D[23:9029:9029:0,-1,-1,-1,255255]);state(0,3);", "state 13 melee, no move2");
    setup(14); put<int>(576, 1); put<int>(964, 1); msg_run__6Cthulu(object); expect(calls == head + "state(0,3);" && get<int>(960) == 0, "state 14 sets state 3 without touching +960");
    setup(15); put<int>(576, 1); advanceResult = 1; rolls[0] = 11; msg_run__6Cthulu(object); expect(calls == head + "move2(000);rand;melee(36,7,-8,9,65544,D[-1,-1,-1,-1,255255]);state(0,3);", "state 15 melee");
    // 16
    setup(16); put<int>(576, 1); advanceResult = 0; iatanResult = 0; msg_run__6Cthulu(object);
    expect(calls == head + "move2(000);cos(100);sin(100);lead(400,160,-55,30);state(0,3);", "state 16 without the S bit: only the lead position");
    setup(16); put<int>(576, 1); advanceResult = 1; iatanResult = 16384 - 100 - 3000; rolls[0] = 1000; rolls[1] = 7; rolls[2] = 10000;
    msg_run__6Cthulu(object);
    { const float speed = __builtin_sqrtf(150.0f * 150.0f + 75.0f * 75.0f) * 0.0025f; const float vz = (7 % 30 + 80.0f) * speed;
      std::string want = head + "move2(000);cos(100);sin(100);lead(400,160,-55,30);cos(100);sin(100);atan(150,-75);rand;cos(3100);sin(3100);rand;alloc(168);rand;poison(300,21,22,23,25,(15,17.5,54),(200,-100," + f(vz) + ")," + std::to_string(100 - 16384) + ",0.576294,1);state(0,3);";
      expect(calls == want, "state 16 fires"); }
    setup(16); put<int>(576, 1); advanceResult = 1; iatanResult = 16384 - 100 - 9000; rolls[0] = 1000; msg_run__6Cthulu(object);
    expect(calls.find("cos(4100);") != std::string::npos, "state 16 clamps a large positive turn to +4000");
    setup(16); put<int>(576, 1); advanceResult = 1; iatanResult = 16384 - 100 + 9000; rolls[0] = 1000; msg_run__6Cthulu(object);
    expect(calls.find("cos(-3900);") != std::string::npos, "state 16 clamps a large negative turn to -4000");
    setup(16); put<int>(576, 1); advanceResult = 1; allocFails = 1; msg_run__6Cthulu(object); expect(calls.find("alloc(168);state(0,3);") != std::string::npos && calls.find("poison") == std::string::npos, "state 16 failed allocation");
    setup(16); put<unsigned short>(294, 8); msg_run__6Cthulu(object); expect(calls == head + "move2(000);", "state 16 with an empty list and bit 8");
    // 8, 9, 10
    setup(8); msg_run__6Cthulu(object); expect(calls == head + "state(0,0);", "state 8");
    setup(8); put<unsigned short>(294, 0x10); msg_run__6Cthulu(object); expect(calls == head, "state 8 with bit 4 set");
    setup(9); put<int>(804, 50); msg_run__6Cthulu(object); expect(calls == head + "state(0,3);", "state 9");
    setup(9); msg_run__6Cthulu(object); expect(calls == head, "state 9 waiting");
    setup(10); advanceResult = 4; rolls[0] = 65537; rolls[1] = 17; msg_run__6Cthulu(object); expect(calls == head + "rand;rand;floor(10,1,37,3,1);", "state 10 splash");
    setup(10); advanceResult = 4; put<unsigned>(12, 2); msg_run__6Cthulu(object); expect(calls == head, "state 10 with bit 2 of +12");
    setup(10); advanceResult = 3; put<unsigned short>(294, 0x80); msg_run__6Cthulu(object); expect(calls == head + "rmrun;", "state 10 leaves the run list");
    return failures ? 1 : 0;
}
