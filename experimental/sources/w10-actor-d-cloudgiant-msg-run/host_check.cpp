// Host contract for msg_run__10CloudGiant; not an EE input. Pointer words are offsets from a host
// base (W10_ACTOR_D_HOST); every callee is a logging stub with scripted results.
#define W10_ACTOR_D_HOST 1
#include <cmath>
#include <cstdio>
#include <cstring>
#include <string>
unsigned char* w10_actor_d_host_base;
typedef void (*HostFn)(void*, int);
HostFn w10_actor_d_host_functions[8];
static std::string calls;
static const void* currentType;
const void* w10_actor_d_type_slot(unsigned) { return currentType; }
int w10_actor_d_skill_function(unsigned, void*, int a, int b) { calls += "skill(" + std::to_string(a) + "," + std::to_string(b) + ");"; return 100; }
static int enemyAnswer;
int w10_actor_d_slot_call(unsigned, void*, int arg) { calls += "isEnemy(" + std::to_string(arg) + ");"; return enemyAnswer; }
extern "C" {
signed char cutMode; float worldDifficulty; unsigned world; unsigned char skills[256]; unsigned char norrath_w10_actor_d_view_7d6c40[80];
}
#include "msg_run.cpp"
static unsigned char* objectUnderTest;
static int everyResult, activationResult, advanceResult, persueResult, rollIndex, iatanResult, allocFails, zone2Result, hitWord;
static int zoneResults[12], zoneIndex, rolls[32], pointZoneResult;
static float groundHeight;
static std::string f(float v) { char b[32]; std::snprintf(b, sizeof b, "%g", v); return b; }
static std::string off(const void* p) { return std::to_string(static_cast<const unsigned char*>(p) - w10_actor_d_host_base); }
static std::string p3(const Point3* p) { return "(" + f(p->x) + "," + f(p->y) + "," + f(p->z) + ")"; }
extern "C" {
void fxRunWeapon(void* d, void* m, void* ws, const Point3* p, short h, void* m2, void* s, float a, int b, int c, float dd, void* mat) {
    calls += "fx(" + std::to_string(static_cast<unsigned char*>(d) - objectUnderTest) + "," + off(m) + "," + std::to_string(static_cast<unsigned char*>(ws) - objectUnderTest) + "," + p3(p) + "," + std::to_string(h) + "," + off(m2) + "," + f(a) + "," + std::to_string(b) + "," + std::to_string(c) + "," + f(dd) + "," + std::to_string(mat == nullptr) + ");";
    (void)s;
}
void newDamageHandler__8CreatureP12_modelHeader(void*, void* m) { calls += "dmg(" + off(m) + ");"; }
int everyFrame__8CreatureP12_modelHeader(void*, void* m) { calls += "every(" + off(m) + ");"; return everyResult; }
int objectActivationCheck__8Creaturei(void*, int n) { calls += "act(" + std::to_string(n) + ");"; return activationResult; }
int creatureAdvanceAnimation__8Creaturef(void*, float v) { calls += "adv(" + f(v) + ");"; return advanceResult; }
int slideBack__8Creaturef(void*, float v) { calls += "slide(" + f(v) + ");"; return 0x5a5a5a5a; }
int enemyInZone__8Creaturefs(void*, float h, short d) { calls += "zone(" + f(h) + "," + std::to_string(d) + ");"; return zoneResults[zoneIndex++ % 12]; }
int enemyInZone2__8Creatureffssfi(void*, float a, float b, short c, short d, float e, int g) { calls += "zone2(" + f(a) + "," + f(b) + "," + std::to_string(c) + "," + std::to_string(d) + "," + f(e) + "," + std::to_string(g) + ");"; return zone2Result; }
void smartWander__8Creature(void*) { calls += "wander;"; }
void turnToFaceEnemy__8Creature(void*) { calls += "turn;"; }
void steerAndRouteToEnemy__8Creature(void*) { calls += "steer;"; }
int bExitStatePersueIdle__8Creature(void*) { calls += "persue;"; return persueResult; }
void move2__8CreatureG6Point3(void*, const Point3* p) { calls += "move2" + p3(p) + ";"; }
int eRand__10GameObject(void*) { calls += "rand;"; return rolls[rollIndex++ % 32]; }
void produceMeleeDamage__8CreaturefiiiiPQ24item11description(void*, float a, int b, int c, int e, int g, void* d) {
    std::string desc = "null";
    if (d) { const unsigned char* q = static_cast<const unsigned char*>(d); short s[4]; for (int i = 0; i < 4; ++i) std::memcpy(&s[i], q + 36 + 8 * i, 2); desc = "D" + std::to_string(s[0]);
             short v1, v2; std::memcpy(&v1, q + 38, 2); std::memcpy(&v2, q + 40, 2); if (s[0] != -1) desc += ":" + std::to_string(v1) + ":" + std::to_string(v2); }
    calls += "melee(" + f(a) + "," + std::to_string(b) + "," + std::to_string(c) + "," + std::to_string(e) + "," + std::to_string(g) + "," + desc + ");";
}
float worldFindHeight__FP12_worldHeaderG6Point3f(void* w, const Point3* p, float v) { calls += "height(" + off(w) + "," + p3(p) + "," + f(v) + ");"; return groundHeight; }
float icos__Fi(int a) { calls += "cos(" + std::to_string(a) + ");"; return 0.5f; }
float isin__Fi(int a) { calls += "sin(" + std::to_string(a) + ");"; return -0.25f; }
int iatan2__Fff(float a, float b) { calls += "atan(" + f(a) + "," + f(b) + ");"; return iatanResult; }
static unsigned char blocks[3][600]; static int blockIndex;
void* blockAlloc__Fi(int n) { calls += "alloc(" + std::to_string(n) + ");"; return allocFails ? nullptr : blocks[blockIndex++ % 3]; }
void boltGoRound_construct(void*, const Point3* p, short h, int a, void* hd) { calls += "bolt(" + p3(p) + "," + std::to_string(h) + "," + std::to_string(a) + "," + off(hd) + ");"; }
int soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* h, void*, float v) { calls += "sound(" + off(h) + "," + f(v) + ");"; return 0x33; }
void soundStopSound__Fi(int v) { calls += "stopsound(" + std::to_string(v) + ");"; }
void soundFadeStream__Fi(int v) { calls += "fade(" + std::to_string(v) + ");"; }
void modelGetCharAttachmentPos__FP12_modelHeaderG6Point3sP14AnimationStateiP6Point3T5f(void* m, const Point3* p, short t, void*, int a, Point3* out, Point3* second, float s) {
    calls += "attach(" + off(m) + "," + p3(p) + "," + std::to_string(t) + "," + std::to_string(a) + "," + std::to_string(second == nullptr) + "," + f(s) + ");";
    *out = Point3{1.0f, 2.0f, 3.0f}; if (second) *second = Point3{0.5f, 0.25f, 0.125f};
}
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* d, Point3* p, Point3* v, float x) { calls += std::string("particle(") + (d == norrath_w10_actor_d_view_7d6c40 ? "breath" : "?") + "," + p3(p) + "," + p3(v) + "," + f(x) + ");"; }
static unsigned listWords[4];
void objectFindInBox__FG6Point3T0PP10GameObjecti(const Point3* lo, const Point3* hi, unsigned* list, int n) {
    calls += "box(" + p3(lo) + "," + p3(hi) + "," + std::to_string(n) + ");"; for (int i = 0; i < 4; ++i) list[i] = listWords[i];
}
int pointInZone__8CreatureG6Point3ffss(void*, const Point3* p, float a, float b, short c, short d) { calls += "pz(" + p3(p) + "," + f(a) + "," + f(b) + "," + std::to_string(c) + "," + std::to_string(d) + ");"; return pointZoneResult; }
void gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(void* t, void* a, float x, float y, int c, int d, int e, int g, void* desc, bool h) {
    std::string ds = "null";
    if (desc) { const unsigned char* q = static_cast<const unsigned char*>(desc); short s0, v1, v2; std::memcpy(&s0, q + 36, 2); std::memcpy(&v1, q + 38, 2); std::memcpy(&v2, q + 40, 2); ds = "D" + std::to_string(s0) + ":" + std::to_string(v1) + ":" + std::to_string(v2); }
    calls += "attack(" + off(t) + "," + std::to_string(a == objectUnderTest) + "," + f(x) + "," + f(y) + "," + std::to_string(c) + "," + std::to_string(d) + "," + std::to_string(e) + "," + std::to_string(g) + "," + ds + "," + std::to_string(h) + ");";
}
void gameSetCameraShake__Ffb(float a, bool b) { calls += "shake(" + f(a) + "," + std::to_string(b) + ");"; }
void findEnemyLeadPosition__8CreaturefP6Point3(void*, float v, Point3* p) { calls += "lead(" + f(v) + "," + p3(p) + ");"; }
int P_StartLightning__FG6Point3T0iiffUi(const Point3* a, const Point3* b, int c, int d, float x, float y, unsigned col) {
    calls += "lstart(" + p3(a) + "," + p3(b) + "," + std::to_string(c) + "," + std::to_string(d) + "," + f(x) + "," + f(y) + "," + std::to_string(col) + ");"; return 77;
}
void P_MoveLightning__FiG6Point3T1ii(int h, const Point3* a, const Point3* b, int c, int d) { calls += "lmove(" + std::to_string(h) + "," + p3(a) + "," + p3(b) + "," + std::to_string(c) + "," + std::to_string(d) + ");"; }
void missileWeapon_construct(void*, int a, void* model, void* texture, const Point3* pos, const Point3* vel, int angle, void* creature, void* v1, void* v2, int i1, int i2, int i3, int i4, int i5, int i6, void* d) {
    calls += "missile(" + std::to_string(a) + "," + off(model) + "," + off(texture) + "," + p3(pos) + "," + p3(vel) + "," + std::to_string(angle) + "," + std::to_string(creature == objectUnderTest) + "," + std::to_string(v1 == nullptr && v2 == nullptr && d == nullptr) + "," +
             std::to_string(i1) + "," + std::to_string(i2) + "," + std::to_string(i3) + "," + std::to_string(i4) + "," + std::to_string(i5) + "," + std::to_string(i6) + ");";
}
void objectRemoveFromRunList__FP10GameObject(void*) { calls += "rmrun;"; }
void floorAdd__FG6Point3sfii(const Point3* p, short a, float b, int c, int d) { calls += "floor(" + p3(p) + "," + std::to_string(a) + "," + f(b) + "," + std::to_string(c) + "," + std::to_string(d) + ");"; }
void modelGetAttachmentMatrix__FP12_modelHeaderP14AnimationStateiP8Matrix34f(void* m, void*, int a, Matrix34* mat, float s) { calls += "attmat(" + off(m) + "," + std::to_string(a) + "," + f(s) + ");"; for (int i = 0; i < 12; ++i) mat->m[i] = static_cast<float>(i); }
void matMakeTransYaw__FP8Matrix34G6Point3s(Matrix34* mat, const Point3* p, short y) { calls += "yaw(" + p3(p) + "," + std::to_string(y) + ");"; for (int i = 0; i < 12; ++i) mat->m[i] = 100.0f + i; }
void matMul__FP8Matrix34N20(Matrix34* d, Matrix34* a, Matrix34* b) { calls += "matmul(" + std::to_string(d == a) + "," + f(a->m[1]) + "," + f(b->m[1]) + ");"; d->m[0] = 9.0f; }
void modelGetShadowPos__FP12_modelHeaderP8Matrix34iP6Point3Pf(void* m, Matrix34* mat, int a, Point3* out, float*) {
    calls += "shadow(" + off(m) + "," + f(mat->m[0]) + "," + std::to_string(a) + ");"; *out = Point3{static_cast<float>(10 + a), 20.0f, 30.0f};
}
int worldHitscanWithObjects__FP12_worldHeaderG6Point3T1fP10GameObjectPP10GameObjectii(void* w, const Point3* from, const Point3* dir, float len, void* ig, unsigned* hit, int a, int b) {
    calls += "hitscan(" + off(w) + "," + p3(from) + "," + p3(dir) + "," + f(len) + "," + std::to_string(ig == objectUnderTest) + "," + std::to_string(a) + std::to_string(b) + ");"; *hit = static_cast<unsigned>(hitWord); return 0;
}
int __eq__C9type_infoRC9type_info(const void* a, const void* b) { return a == b; }
void __throw_bad_typeid() { calls += "throw;"; }
static char giantNode, otherNode;
const void* __tf10CloudGiant() { return &giantNode; }
}
static void setState(void* s, int v) { calls += "state(" + std::to_string(static_cast<unsigned char*>(s) - objectUnderTest) + "," + std::to_string(v) + ");"; }
static void slot168(void*, int) { calls += "slot168;"; }
static void stun(void*, int v) { calls += "stun(" + std::to_string(v) + ");"; }
static int failures;
static void expect(bool ok, const char* what) { if (!ok) { std::printf("fail: %s\n  calls: %s\n", what, calls.c_str()); ++failures; } }
template <typename T> static T get(unsigned o) { T v; std::memcpy(&v, objectUnderTest + o, sizeof v); return v; }
template <typename T> static void put(unsigned o, T v) { std::memcpy(objectUnderTest + o, &v, sizeof v); }
int main() {
    alignas(16) static unsigned char arena[0x40000];
    w10_actor_d_host_base = arena; w10_actor_d_host_functions[1] = setState; w10_actor_d_host_functions[2] = slot168; w10_actor_d_host_functions[3] = stun;
    unsigned char* object = arena + 0x10000; objectUnderTest = object;
    auto setup = [&](short state) {
        std::memset(arena, 0, sizeof arena); put<unsigned>(36, 300u); { unsigned h = 900; std::memcpy(arena + 300 + 12, &h, 4); for (unsigned k = 0; k < 100; ++k) { unsigned v = 1000 + 4 * k; if (k != 3) std::memcpy(arena + 300 + 4 * k, &v, 4); } unsigned h2 = 900; std::memcpy(arena + 300 + 12, &h2, 4); }
        put<short>(572, state); unsigned table = 4096; put<unsigned>(72, table);
        short adj = 0; unsigned fn = 1; std::memcpy(arena + table + 184, &adj, 2); std::memcpy(arena + table + 188, &fn, 4);
        fn = 2; std::memcpy(arena + table + 168, &adj, 2); std::memcpy(arena + table + 172, &fn, 4);
        put<float>(16, 10.0f); put<float>(20, 20.0f); put<float>(24, 30.0f); put<float>(28, 5.0f); put<float>(224, 2.0f); put<short>(56, 100);
        put<int>(808, 100); put<int>(256, 500); put<int>(960, 500); put<int>(968, 500); put<int>(804, 500);
        unsigned skill = 6000, skillTable = 7000, skill2 = 6100, skillTable2 = 7100; std::memcpy(skills + 48, &skill, 4); std::memcpy(arena + skill + 96, &skillTable, 4);
        std::memcpy(skills + 56, &skill2, 4); std::memcpy(arena + skill2 + 96, &skillTable2, 4); unsigned sfn = 77; std::memcpy(arena + skillTable + 96 + 4, &sfn, 4); std::memcpy(arena + skillTable2 + 96 + 4, &sfn, 4);
        worldDifficulty = 3.0f; world = 20000u; cutMode = 0; put<short>(200, 7); put<short>(202, -8); put<short>(204, 9);
        calls.clear(); everyResult = 0; activationResult = 1; advanceResult = 0; persueResult = 0; rollIndex = 0; iatanResult = 0; allocFails = 0; zoneIndex = 0; zone2Result = 0;
        for (int& z : zoneResults) z = 0;
        for (int i = 0; i < 32; ++i) rolls[i] = 32768;
        groundHeight = 0; hitWord = 0; enemyAnswer = 0; pointZoneResult = 0; blockIndex = 0; currentType = &otherNode;
        for (unsigned& w : listWords) w = 0;
    };
    const std::string head = "dmg(900);every(900);act(600);adv(1);slide(0);slot168;";
    // entry
    setup(0); everyResult = 1; msg_run__10CloudGiant(object); expect(calls == "dmg(900);every(900);", "everyFrame ends");
    setup(0); put<int>(976, 1); activationResult = 0; msg_run__10CloudGiant(object); expect(calls == "dmg(900);every(900);act(1200);", "boss activation distance");
    setup(0); activationResult = 0; msg_run__10CloudGiant(object); expect(calls == "dmg(900);every(900);act(600);", "plain activation distance");
    setup(0); put<int>(984, 1); msg_run__10CloudGiant(object); expect(calls.rfind("fx(1056,1196,988,(10,20,30),100,900,0.5,0,1,2,1);", 0) == 0, "first weapon effect");
    setup(6); put<int>(984, 1); put<int>(980, 1); msg_run__10CloudGiant(object);
    expect(calls.find("fx(1056,1196,988,(10,20,30),100,900,0.5,1,1,2,1);fx(1192,1180,1128,(10,20,30),100,900,1,0,0,2,1);") == 0, "both weapon effects in state 6");
    setup(15); put<int>(980, 1); cutMode = 1; msg_run__10CloudGiant(object); expect(calls.find("fx(") == std::string::npos, "cutMode stops weapon effects");
    setup(17); put<int>(980, 1); msg_run__10CloudGiant(object); expect(calls.find("fx(1192,1180,1128,(10,20,30),100,900,1,1,0,2,1);") == 0, "second weapon effect flag for states 15 and 17");
    for (short s : {2, 4, 7, 11, 12, 19, 100, -1}) { setup(s); msg_run__10CloudGiant(object); expect(calls == head, "states without a case"); }
    // 0, 1
    setup(0); zoneResults[0] = 1; msg_run__10CloudGiant(object); expect(calls == head + "zone(400,32767);state(0,3);", "state 0 enemy");
    setup(0); put<int>(256, 50); msg_run__10CloudGiant(object); expect(calls == head + "zone(400,32767);state(0,1);", "state 0 timer");
    setup(1); put<int>(256, 50); msg_run__10CloudGiant(object); expect(calls == head + "zone(400,32767);state(0,0);", "state 1 timer");
    setup(1); msg_run__10CloudGiant(object); expect(calls == head + "zone(400,32767);wander;", "state 1 wander");
    // 3
    setup(3); msg_run__10CloudGiant(object); expect(calls == head + "state(0,0);", "state 3 empty list");
    setup(3); put<int>(576, 1); put<int>(976, 1); msg_run__10CloudGiant(object); expect(calls == "dmg(900);every(900);act(1200);act(600);adv(1);slide(0);slot168;state(0,18);", "state 3 boss start");
    setup(3); put<int>(576, 1); put<int>(976, 1); put<int>(980, 1); zoneResults[0] = 1; rolls[0] = 5; msg_run__10CloudGiant(object); expect(calls.find("act(1200);act(600);adv(1);slide(0);slot168;zone(35,3000);rand;state(0,6);") != std::string::npos, "state 3 melee range, roll & 3 != 0");
    setup(3); put<int>(576, 1); zoneResults[0] = 1; rolls[0] = 8; msg_run__10CloudGiant(object); expect(calls == head + "zone(35,3000);rand;state(0,16);", "state 3 melee range, roll & 3 == 0");
    setup(3); put<int>(576, 1); put<int>(960, 50); rolls[0] = 3; zoneResults[1] = 1; msg_run__10CloudGiant(object); expect(calls == head + "zone(35,3000);rand;zone(250,3000);state(0,13);" && get<int>(960) == 460, "state 3 pick 0 -> 13");
    setup(3); put<int>(576, 1); put<int>(960, 50); rolls[0] = 4; zoneResults[1] = 0; zone2Result = 1; msg_run__10CloudGiant(object); expect(calls == head + "zone(35,3000);rand;zone(200,3000);zone2(0,500,100,3000,0,1);state(0,15);" && get<int>(960) == 460, "state 3 pick 1 -> 15");
    setup(3); put<int>(576, 1); put<int>(960, 50); rolls[0] = 5; put<int>(980, 1); msg_run__10CloudGiant(object); expect(calls.find(head + "zone(35,3000);rand;state(0,17);") != std::string::npos && get<int>(960) == 460, "state 3 pick 2 -> 17");
    setup(3); put<int>(576, 1); put<int>(960, 50); rolls[0] = 5; msg_run__10CloudGiant(object); expect(calls.find("state(0,14);") != std::string::npos, "state 3 pick 2 -> 14");
    setup(3); put<int>(576, 1); put<int>(960, 50); rolls[0] = 3; msg_run__10CloudGiant(object); expect(calls == head + "zone(35,3000);rand;zone(250,3000);" && get<int>(960) == 50, "state 3 pick 0 with nothing near");
    setup(3); put<int>(576, 1); zoneResults[1] = 1; msg_run__10CloudGiant(object); expect(calls == head + "zone(35,3000);zone(35,32767);turn;", "state 3 turns");
    setup(3); put<int>(576, 1); msg_run__10CloudGiant(object); expect(calls == head + "zone(35,3000);zone(35,32767);steer;", "state 3 steers");
    // 5
    setup(5); zoneResults[0] = 1; rolls[0] = 10; msg_run__10CloudGiant(object); expect(calls == head + "zone(35,3000);rand;state(0,16);", "state 5 melee range");
    setup(5); zoneResults[0] = 1; rolls[0] = 11; msg_run__10CloudGiant(object); expect(calls.find("state(0,6);") != std::string::npos, "state 5 melee range, roll % 5 != 0");
    setup(5); put<int>(960, 50); rolls[0] = 4; zoneResults[1] = 0; zoneResults[2] = 1; msg_run__10CloudGiant(object); expect(calls == head + "zone(35,3000);rand;zone(200,3000);zone(500,3000);state(0,15);" && get<int>(960) == 580, "state 5 pick 1 -> 15");
    setup(5); persueResult = 1; msg_run__10CloudGiant(object); expect(calls == head + "zone(35,3000);persue;state(0,3);", "state 5 leaves");
    // 6
    setup(6); put<int>(576, 1); advanceResult = 1; put<int>(208, 1); rolls[0] = 0; msg_run__10CloudGiant(object);
    expect(calls == head + "move2(0,0,0);skill(0,3);rand;melee(36,7,-8,9,8,D24:100:100);state(0,3);", "state 6 melee with the skill value");
    setup(6); put<int>(576, 1); advanceResult = 1; put<int>(208, 1); rolls[0] = 65536; msg_run__10CloudGiant(object);
    expect(calls.find("melee(36,7,-8,9,8,D24:100:120);") != std::string::npos, "state 6 second value: 100 + (int)(100 * 0.2 * 1.0)");
    setup(6); put<int>(576, 1); advanceResult = 1; msg_run__10CloudGiant(object); expect(calls == head + "move2(0,0,0);melee(36,7,-8,9,8,D-1);state(0,3);", "state 6 plain melee");
    setup(6); put<int>(576, 1); put<unsigned short>(294, 8); msg_run__10CloudGiant(object); expect(calls == head + "move2(0,0,0);turn;", "state 6 turning");
    setup(6); msg_run__10CloudGiant(object); expect(calls == head + "move2(0,0,0);state(0,3);", "state 6 plain");
    // 8, 9, 10
    setup(8); msg_run__10CloudGiant(object); expect(calls == head + "move2(0,0,0);state(0,0);", "state 8");
    setup(8); put<unsigned short>(294, 0x10); msg_run__10CloudGiant(object); expect(calls == head + "move2(0,0,0);", "state 8 with bit 4");
    setup(9); put<int>(804, 5); msg_run__10CloudGiant(object); expect(calls == head + "state(0,3);", "state 9");
    setup(10); put<unsigned short>(294, 0x80); advanceResult = 3; rolls[0] = 65537; rolls[1] = 21; msg_run__10CloudGiant(object);
    expect(calls == head + "fade(0);rmrun;cos(100);sin(100);rand;rand;floor((35,7.5,30),1,51,0,1);" && (get<unsigned>(12) & 1) != 0, "state 10 flow");
    setup(10); advanceResult = 2; msg_run__10CloudGiant(object); expect(calls == head && (get<unsigned>(12) & 1) != 0, "state 10 S bit 1 sets bit 0 of +12");
    setup(10); advanceResult = 3; put<unsigned>(12, 2); msg_run__10CloudGiant(object); expect(calls == head && get<unsigned>(12) == 2, "state 10 with bit 1 of +12");
    // 13
    setup(13); advanceResult = 1; put<unsigned char>(964, 0); msg_run__10CloudGiant(object); expect(calls.find("sound(1260,1);") != std::string::npos && get<int>(1308) == 0x33 && get<unsigned char>(964) == 1, "state 13 starts the roar");
    setup(13); put<unsigned char>(964, 1); put<int>(808, 101); put<int>(968, 50); put<int>(1308, 8); msg_run__10CloudGiant(object);
    expect(calls.find("attach(900,(10,20,30),100,2,0,2);") != std::string::npos && calls.find("particle(breath,") != std::string::npos && calls.find("box(") == std::string::npos && calls.rfind("stopsound(8);state(0,3);") != std::string::npos && get<int>(1308) == -1, "state 13 breath and timer");
    { size_t n = 0, pos = 0; while ((pos = calls.find("particle(", pos)) != std::string::npos) { ++n; ++pos; } expect(n == 16, "sixteen breath particles"); }
    setup(13); put<unsigned char>(964, 1); put<int>(808, 64); put<int>(968, 500); put<int>(808, 64); put<float>(32, 40.0f); listWords[0] = 20000; listWords[1] = 0;
    enemyAnswer = 1; pointZoneResult = 1; put<unsigned>(36, 300u); { unsigned flags = 0; std::memcpy(arena + 20000 + 12, &flags, 4); float p[3] = {1.0f, 2.0f, 3.0f}; std::memcpy(arena + 20000 + 16, p, 12); }
    msg_run__10CloudGiant(object);
    expect(calls.find("box((-240,-230,-10),(260,270,70),128);isEnemy(20000);pz((1,2,3),0,250,100,3000);skill(0,3);attack(20000,1,0,0,100000,0,0,263232,D22:20:20,0);") != std::string::npos && get<int>(968) == 500, "state 13 area attack");
    setup(13); put<unsigned char>(964, 1); put<int>(808, 64); put<int>(968, 500); put<float>(32, 40.0f); listWords[0] = 20000; enemyAnswer = 0; msg_run__10CloudGiant(object);
    expect(calls.find("isEnemy(20000);") != std::string::npos && calls.find("pz(") == std::string::npos && get<int>(968) == 0 && calls.find("stopsound") != std::string::npos, "state 13 nobody in range");
    // 14
    setup(14); advanceResult = 1; listWords[0] = 20000; { unsigned flags = 0x20; std::memcpy(arena + 20000 + 12, &flags, 4); unsigned table = 21000; std::memcpy(arena + 20000 + 72, &table, 4); short adj = 0; unsigned fn = 3; std::memcpy(arena + table + 208, &adj, 2); std::memcpy(arena + table + 212, &fn, 4); }
    currentType = &otherNode; msg_run__10CloudGiant(object);
    expect(calls == head + "move2(0,0,0);shake(6,0);box((-246,-236,-226),(266,276,286),128);attack(20000,1,3,-4,100000,255,0,67108864,null,0);stun(60);state(0,3);", "state 14 stuns others");
    setup(14); advanceResult = 1; listWords[0] = 20000; { unsigned flags = 0x20; std::memcpy(arena + 20000 + 12, &flags, 4); unsigned table = 21000; std::memcpy(arena + 20000 + 72, &table, 4); } currentType = &giantNode; msg_run__10CloudGiant(object);
    expect(calls.find("attack(") == std::string::npos, "state 14 spares other CloudGiants");
    setup(14); advanceResult = 1; listWords[0] = 20000; { unsigned flags = 0x28; std::memcpy(arena + 20000 + 12, &flags, 4); } msg_run__10CloudGiant(object); expect(calls.find("attack(") == std::string::npos, "state 14 skips flagged objects");
    // 16
    setup(16); put<int>(576, 1); advanceResult = 1; put<unsigned short>(294, 8); msg_run__10CloudGiant(object); expect(calls == head + "move2(0,0,0);melee(36,7,-8,9,1048641,null);turn;", "state 16 melee and turn");
    // 17, 18
    setup(17); advanceResult = 1; groundHeight = 12.5f; msg_run__10CloudGiant(object);
    expect(calls == head + "height(20000,(10,20,30),6);cos(-4900);sin(-4900);alloc(156);bolt((" + f(10.0f + 0.5f * 50.0f) + "," + f(20.0f - 0.25f * 50.0f) + ",12.5),100,240,1244);state(0,3);" && get<int>(980) == 0 && get<int>(972) == 340, "state 17 bolt");
    setup(18); advanceResult = 1; msg_run__10CloudGiant(object); expect(calls.find(head + "sound(1228,1);state(0,3);") != std::string::npos && get<int>(980) == 1 && get<short>(1208) == 150 && get<short>(1210) == 150 && get<short>(1212) == 200 && get<short>(1214) == 30, "state 18");
    // 15
    setup(15); put<int>(576, 1); advanceResult = 1; put<int>(980, 1); put<float>(224, 2.0f); msg_run__10CloudGiant(object);
    expect(calls.find("cos(100);sin(100);lead(300,(" + f(10.0f + 0.5f * 300.0f) + "," + f(20.0f - 0.25f * 300.0f) + ",30));attach(900,(10,20,30),100,0,1,2);lstart(") != std::string::npos && get<int>(1264) == 1 && get<int>(980) == 0, "state 15 beam");
    setup(15); put<int>(576, 1); advanceResult = 1; iatanResult = 16384 - 100 - 3000; msg_run__10CloudGiant(object);
    expect(calls.find("lead(400,(") != std::string::npos && calls.find("alloc(452);missile(0,1212,1220,(1,2,3),") != std::string::npos && calls.find("-16284") == std::string::npos, "state 15 missile");
    setup(15); put<int>(576, 1); advanceResult = 1; allocFails = 1; msg_run__10CloudGiant(object); expect(calls.find("alloc(452);turn;") != std::string::npos && calls.find("missile(") == std::string::npos, "state 15 failed allocation");
    setup(15); put<int>(576, 1); put<unsigned short>(294, 8); msg_run__10CloudGiant(object); expect(calls == head + "move2(0,0,0);turn;", "state 15 turning without firing");
    // tail
    setup(2); put<int>(980, 1); msg_run__10CloudGiant(object);
    expect(calls == "fx(1192,1180,1128,(10,20,30),100,900,1,0,0,2,1);" + head + "attmat(900,0,2);yaw((10,20,30),16484);matmul(1,1,101);shadow(1180,9,0);shadow(1180,9,80);lstart((10,20,30),(90,20,30),200,10,4,4.5,1585479808);lmove(77,(10,20,30),(90,20,30),94,-1);" && get<int>(1304) == 77, "tail with the weapon bolt");
    setup(2); put<int>(980, 1); put<int>(1304, 55); msg_run__10CloudGiant(object); expect(calls.find("lstart") == std::string::npos && calls.find("lmove(55,") != std::string::npos, "tail keeps an existing bolt");
    setup(2); msg_run__10CloudGiant(object); expect(calls == head, "tail idle");
    setup(2); put<int>(1264, 1); put<int>(1304, 55); { float a[3] = {0, 0, 0}, b[3] = {200, 0, 0}, v[3] = {1, 2, 3}; std::memcpy(object + 1268, a, 12); std::memcpy(object + 1280, b, 12); std::memcpy(object + 1292, v, 12); } groundHeight = -100;
    msg_run__10CloudGiant(object);
    expect(calls == head + "hitscan(20000,(0,0,0),(200,0,0),96,1,00);lmove(55,(1,2,3),(201,2,3),94,-1);height(20000,(201,2,3),6);" && get<float>(1268) == 1.0f && get<float>(1280) == 201.0f && get<int>(1264) == 1, "tail flies the bolt");
    setup(2); put<int>(1264, 1); put<int>(1304, 55); { float a[3] = {0, 0, 0}, b[3] = {50, 0, 0}, v[3] = {1, 2, 3}; std::memcpy(object + 1268, a, 12); std::memcpy(object + 1280, b, 12); std::memcpy(object + 1292, v, 12); } groundHeight = 5; put<float>(16, 5000.0f);
    msg_run__10CloudGiant(object);
    expect(get<float>(1280) == 50.0f && get<int>(1264) == 0 && get<int>(1304) == 0 && calls.find("sound(1228") == std::string::npos, "tail ends the bolt when too far from the creature");
    setup(2); put<int>(1264, 1); put<int>(1304, 55); { float a[3] = {0, 0, 0}, b[3] = {50, 0, 0}, v[3] = {1, 2, 3}; std::memcpy(object + 1268, a, 12); std::memcpy(object + 1280, b, 12); std::memcpy(object + 1292, v, 12); } groundHeight = 5; put<float>(16, 0.0f); put<float>(20, 0.0f); put<float>(24, 0.0f);
    msg_run__10CloudGiant(object);
    expect(calls.find("sound(1236,1);") != std::string::npos && get<int>(1264) == 0 && get<int>(1304) == 0, "tail ends the bolt on the ground");
    setup(2); put<int>(1264, 1); put<int>(1304, 55); hitWord = 20000; { unsigned flags = 0x40; std::memcpy(arena + 20000 + 12, &flags, 4); float a[3] = {0, 0, 0}, b[3] = {50, 0, 0}, v[3] = {1, 2, 3}; std::memcpy(object + 1268, a, 12); std::memcpy(object + 1280, b, 12); std::memcpy(object + 1292, v, 12); } groundHeight = -50; put<float>(16, 0.0f); put<float>(20, 0.0f);
    msg_run__10CloudGiant(object);
    expect(calls.find("attack(20000,0,0,0,9,0,0,64,D24:7:-8,0);sound(1236,1);") != std::string::npos, "tail hits an object with flag 0x40");
    return failures ? 1 : 0;
}
