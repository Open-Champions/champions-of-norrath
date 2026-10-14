// Host contract for msg_run__17UndeadKnightClone; not an EE input (build with -fno-builtin so that strcmp is a real call). 32-bit words are offsets from a host
// base; callees log and answer from small queues; eRand returns 32768 (every ball point is then the
// origin) unless a test says otherwise. The authored function was also compared with the original in a
// private R5900 run (several hundred randomized objects over all states, state 3 and state 10 sampled
// again with varied shadow weights; every reachable original word reached).
#define W10_ACTOR_O_HOST 1
#include <cmath>
#include <cstdio>
#include <cstring>
#include <deque>
#include <string>
#include <initializer_list>
unsigned char* w10_actor_o_host_base;
#include "msg_run.cpp"

static unsigned char arena[0x10000] __attribute__((aligned(16)));
static unsigned char* const obj = arena + 0x1000;
int frameNm;
char levelName[400];
unsigned udkResourceMap[54];
unsigned char undead_knight_clone_msg_run_particle_7d7e80[8], undead_knight_clone_msg_run_particle_7d7ed0[8];
static std::string calls;
static std::deque<int> zone;
static int everyAnswer, advanceAnswer, exitAnswer, pointsAnswer, strcmpAnswer;
static std::deque<unsigned> rolls;
static unsigned defaultRoll = 32768u;
static void* allocAnswer;
static std::deque<float> weights;
static std::string num(double v) { char b[40]; std::snprintf(b, sizeof b, "%g", v); return b; }
static unsigned off(const void* p) { return static_cast<unsigned>(static_cast<const unsigned char*>(p) - arena); }
static std::string pt(const Point3& p) { return num(p.x) + ":" + num(p.y) + ":" + num(p.z); }
static std::string which(const void* d) { return d == undead_knight_clone_msg_run_particle_7d7e80 ? "p80" : d == undead_knight_clone_msg_run_particle_7d7ed0 ? "pd0" : "p??"; }
extern "C" {
void fxRunWeapon__FPQ24item11descriptionP12_modelHeaderP13fxWeaponStateG6Point3sT1P14AnimationStatefiifP8Matrix34(
    void* d, void* m, void* s, Point3 p, short a, void* m2, void* an, float f, int g, int h, float i, void* mat) {
    calls += "fx(" + num(off(d) - 0x1000) + "," + num(off(m)) + "," + num(off(s) - 0x1000) + "," + pt(p) + "," + num(a) + "," + num(off(m2)) + "," + num(off(an) - 0x1000) + "," + num(f) + "," + num(g) + "," + num(h) + "," + num(i) + "," + (mat ? "mat" : "null") + ");";
}
void newDamageHandler__8CreatureP12_modelHeader(void*, void* m) { calls += "dmg" + num(off(m)) + ";"; }
int everyFrame__8CreatureP12_modelHeader(void*, void* m) { calls += "every" + num(off(m)) + ";"; return everyAnswer; }
int objectActivationCheck__8Creaturei(void*, int v) { calls += "act" + num(v) + ";"; return 0; }
int creatureAdvanceAnimation__8Creaturef(void*, float f) { calls += "adv" + num(f) + ";"; return advanceAnswer; }
void slideBack__8Creaturef(void*, float f) { calls += "slide" + num(f) + ";"; }
int move2__8CreatureG6Point3(void*, Point3 p) { calls += "move2(" + pt(p) + ");"; return 0; }
void lightUpdatePosition__FP5Light(void* l) { calls += "light" + num(off(l) - 0x1000) + ";"; }
int enemyInZone__8Creaturefs(void*, float r, short a) {
    calls += "zone(" + num(r) + "," + num(a) + ");";
    if (zone.empty()) return 0;
    const int v = zone.front(); zone.pop_front(); return v;
}
void smartWander__8Creature(void*) { calls += "wander;"; }
unsigned eRand__10GameObject(void*) {
    calls += "rand;";
    if (rolls.empty()) return defaultRoll;
    const unsigned v = rolls.front(); rolls.pop_front(); return v;
}
void steerAndRouteToEnemy__8Creature(void*) { calls += "steer;"; }
int bExitStatePersueIdle__8Creature(void*) { calls += "exit;"; return exitAnswer; }
void turnToFaceEnemy__8Creature(void*) { calls += "turn;"; }
void produceMeleeDamage__8CreaturefiiiiPQ24item11description(void*, float amount, int a, int b, int c, int d, void* desc) {
    calls += "melee(" + num(amount) + "," + num(a) + "," + num(b) + "," + num(c) + "," + num(d) + "," + (desc ? "desc" : "null") + ");";
}
int modelGetNmShadowPoints__FP12_modelHeader(void* m) { calls += "points" + num(off(m)) + ";"; return pointsAnswer; }
void modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f(
    void* m, void* an, Point3 p, short yaw, int index, Point3* out, float* weight, void* matrix, float scale) {
    calls += "shadow(" + num(off(m)) + "," + num(off(an) - 0x1000) + "," + pt(p) + "," + num(yaw) + "," + num(index) + "," + (weight ? "w" : "null") + "," + (matrix ? "matrix" : "null") + "," + num(scale) + ");";
    *out = Point3{1, 2, 3};
    if (weight) { *weight = weights.empty() ? 0.0f : weights.front(); if (!weights.empty()) weights.pop_front(); }
}
void modelGetCharAttachmentMatrix__FP12_modelHeaderG6Point3sP14AnimationStateiP8Matrix34f(void* m, Point3 p, short yaw, void* an, int index, void* matrix, float scale) {
    calls += "matrix(" + num(off(m)) + "," + pt(p) + "," + num(yaw) + "," + num(off(an) - 0x1000) + "," + num(index) + "," + num(scale) + ");";
}
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* d, Point3* p, Point3* v, float age) {
    calls += "particle(" + which(d) + "," + pt(*p) + "," + pt(*v) + "," + num(age) + ");";
}
void* blockAlloc__Fi(int n) { calls += "alloc" + num(n) + ";"; return allocAnswer; }
void __6PhPropG6Point3P8Matrix34PcN23liPQ24item11descriptionf(void* self, Point3 p, void* matrix, char* a, char* b, char* c, long d, int e, void* desc, float scale) {
    calls += "prop(" + num(self == allocAnswer) + "," + pt(p) + "," + (matrix ? "matrix" : "null") + "," + a + "," + b + "," + c + "," + num(d) + "," + num(e) + "," + (desc ? "desc" : "null") + "," + num(scale) + ");";
}
void objectAddToDeleteList__FP10GameObject(void*) { calls += "delete;"; }
void objectRemoveFromRunList__FP10GameObject(void*) { calls += "remove;"; }
float icos__Fi(int a) { calls += "cos" + num(a) + ";"; return 0.5f; }
float isin__Fi(int a) { calls += "sin" + num(a) + ";"; return -0.25f; }
int strcmp(const char* a, const char* b) {
    calls += std::string("strcmp(") + a + "," + b + ");";
    while (*a != 0 && *a == *b) { ++a; ++b; }
    return static_cast<unsigned char>(*a) == static_cast<unsigned char>(*b) ? 0 : (static_cast<unsigned char>(*a) < static_cast<unsigned char>(*b) ? -1 : 1);
}
}
unsigned w10_actor_o_host_call(unsigned fw, void* adjusted, int n, unsigned long a1, unsigned long) {
    calls += "v" + num(fw) + "(A," + num(n) + "," + num(static_cast<long>(a1)) + ");"; (void)adjusted; return 0x2468;
}
static void put32(unsigned o, unsigned v) { std::memcpy(obj + o, &v, 4); }
static void putA(unsigned a, unsigned v) { std::memcpy(arena + a, &v, 4); }
static unsigned get32(unsigned o) { unsigned v; std::memcpy(&v, obj + o, 4); return v; }
static void put16(unsigned o, unsigned short v) { std::memcpy(obj + o, &v, 2); }
static void reset(short state) {
    std::memset(arena, 0, sizeof arena); w10_actor_o_host_base = arena; calls.clear(); zone.clear(); rolls.clear(); weights.clear();
    everyAnswer = 0; advanceAnswer = 0; exitAnswer = 0; pointsAnswer = 0; strcmpAnswer = 0; defaultRoll = 32768u; allocAnswer = arena + 0x3000;
    std::memset(levelName, 0, sizeof levelName); std::memset(udkResourceMap, 0, sizeof udkResourceMap); udkResourceMap[3] = 0x2468;
    frameNm = 100;
    put32(36, 0x300); putA(0x300 + 12, 0x400); putA(0x300 + 180, 0x480); put32(72, 0x500);
    putA(0x500 + 128, 0); putA(0x500 + 132, 0x80); putA(0x500 + 168, 0); putA(0x500 + 172, 0x68); putA(0x500 + 184, 0); putA(0x500 + 188, 0xb8);
    put16(572, static_cast<unsigned short>(state));
    put32(28, 0x41200000u);  // 10.0
    float pos[3] = {4, 5, 6}; std::memcpy(obj + 16, pos, 12);
    short h = 9; std::memcpy(obj + 56, &h, 2);
    put16(200, 11); put16(202, 12); put16(204, 13);
    put32(808, 100);
    float s224 = 2.0f; std::memcpy(obj + 224, &s224, 4);
}
static const std::string head = "dmg1024;every1024;act600;adv1;slide0.5;v104(A,0,0);move2(0:0:0);light2496;";
static std::string scatterRound(int index, const std::string& def, double speed) {
    // one scatter round with the stub values: P = {1, 2, 3} (ball points are the origin)
    (void)speed;
    return "v128(A,0,0);rand;shadow(9320,288,4:5:6,9," + num(index) + ",w,null,2);rand;rand;rand;rand;rand;rand;particle(" + def + ",1:2:3,0:0:0,0);";
}
static std::string tail() {
    std::string t = "v128(A,0,0);points9320;";
    for (int i = 0; i < 100; ++i) t += scatterRound(32768, "p80", 12);   // the divisor is zero here: the remainder is the dividend
    t += "v128(A,0,0);points9320;";
    return t;
}
static int check(const char* label, const std::string& want) {
    if (calls == want) return 0;
    std::printf("fail %s\n  want %s\n  got  %s\n", label, want.substr(0, 400).c_str(), calls.substr(0, 400).c_str());
    for (size_t i = 0; i < want.size() && i < calls.size(); ++i) if (want[i] != calls[i]) { std::printf("  first difference at %zu\n", i); break; }
    return 1;
}
static std::string set(int n) { return "v184(A,1," + num(n) + ");"; }
int main() {
    int bad = 0;
    const std::string z400 = "zone(400,32767);", z500 = "zone(500,16000);";
    const std::string melee = "melee(36,11,12,13,262152,null);turn;";
    // part 1 and 2
    reset(2); put32(2556, 5); put16(572, 6); everyAnswer = 1;
    msg_run__17UndeadKnightClone(obj);
    bad |= check("weapon", "fx(2624,1152,2560,4:5:6,9,1024,288,0.5,1,0,1,null);dmg1024;every1024;");
    reset(13); put32(2556, 1); everyAnswer = 1; msg_run__17UndeadKnightClone(obj);
    bad |= check("weapon 13", "fx(2624,1152,2560,4:5:6,9,1024,288,0.5,1,0,1,null);dmg1024;every1024;");
    reset(5); put32(2556, 1); everyAnswer = 1; msg_run__17UndeadKnightClone(obj);
    bad |= check("weapon 5", "fx(2624,1152,2560,4:5:6,9,1024,288,0.5,0,0,1,null);dmg1024;every1024;");
    reset(7); everyAnswer = 1; msg_run__17UndeadKnightClone(obj); bad |= check("every", "dmg1024;every1024;");
    // the light position and the common tail
    reset(7); msg_run__17UndeadKnightClone(obj);
    bad |= check("idle", head + set(3) + tail());
    { float lp[3]; std::memcpy(lp, obj + 2544, 12); if (lp[0] != 4.0f || lp[1] != 5.0f || lp[2] != 90.0f) { std::puts("fail light point"); bad = 1; } }
    for (int s : {2, 4, 6, 8, 9, 11, 12, 18, 40, -1}) { reset(static_cast<short>(s)); msg_run__17UndeadKnightClone(obj); bad |= check("default", head + set(3) + tail()); }
    // 0 and 1
    reset(0); zone = {1}; msg_run__17UndeadKnightClone(obj); bad |= check("0 near", head + z400 + set(3) + tail());
    reset(0); zone = {0, 1}; msg_run__17UndeadKnightClone(obj); bad |= check("0 far", head + z400 + z500 + set(3) + tail());
    reset(0); put32(256, 5); msg_run__17UndeadKnightClone(obj); bad |= check("0 wait", head + z400 + z500 + set(1) + tail());
    reset(0); put32(256, 100); msg_run__17UndeadKnightClone(obj); bad |= check("0 hold", head + z400 + z500 + tail());
    reset(1); zone = {1}; msg_run__17UndeadKnightClone(obj); bad |= check("1 near", head + z400 + set(0) + tail());
    reset(1); zone = {0, 1}; msg_run__17UndeadKnightClone(obj); bad |= check("1 far", head + z400 + z500 + set(0) + tail());
    reset(1); msg_run__17UndeadKnightClone(obj); bad |= check("1 wander", head + z400 + z500 + "wander;" + tail());
    // 3
    reset(3); msg_run__17UndeadKnightClone(obj); bad |= check("3 none", head + set(0) + tail());
    for (unsigned r : {10u, 64u, 65u, 164u}) {
        const bool low = r % 100 < 65;
        reset(3); put32(576, 0x600); zone = {1}; rolls = {r}; msg_run__17UndeadKnightClone(obj);
        bad |= check("3 a", head + "zone(70,23300);rand;" + set(low ? 16 : 14) + tail());
        reset(3); put32(576, 0x600); zone = {0, 1}; rolls = {r}; msg_run__17UndeadKnightClone(obj);
        bad |= check("3 b", head + "zone(70,23300);zone(90,11000);rand;" + set(low ? 14 : 15) + tail());
        reset(3); put32(576, 0x600); zone = {0, 0, 1}; rolls = {r}; msg_run__17UndeadKnightClone(obj);
        bad |= check("3 c", head + "zone(70,23300);zone(90,11000);zone(90,6000);rand;" + set(low ? 13 : 15) + tail());
    }
    for (int wait : {50, 150}) {
        reset(3); put32(576, 0x600); put32(256, static_cast<unsigned>(wait)); zone = {0, 0, 0, 1}; msg_run__17UndeadKnightClone(obj);
        bad |= check("3 d", head + "zone(70,23300);zone(90,11000);zone(90,6000);zone(130,3000);" + (100 < wait ? "steer;" : set(17)) + tail());
    }
    const std::string z3 = "zone(70,23300);zone(90,11000);zone(90,6000);zone(130,3000);";
    reset(3); put32(576, 0x600); zone = {0, 0, 0, 0, 1}; msg_run__17UndeadKnightClone(obj);
    bad |= check("3 e", head + z3 + "zone(300,32767);steer;" + tail());
    reset(3); put32(576, 0x600); std::strcpy(levelName + 81, "battle3"); msg_run__17UndeadKnightClone(obj);
    bad |= check("3 f", head + z3 + "zone(300,32767);strcmp(battle3,battle3);" + set(10) + tail());
    reset(3); put32(576, 0x600); std::strcpy(levelName + 81, "battle2"); msg_run__17UndeadKnightClone(obj);
    bad |= check("3 g", head + z3 + "zone(300,32767);strcmp(battle2,battle3);steer;" + tail());
    // 5
    reset(5); zone = {1}; msg_run__17UndeadKnightClone(obj); bad |= check("5 near", head + "zone(50,6000);" + set(6) + tail());
    reset(5); msg_run__17UndeadKnightClone(obj); bad |= check("5 stay", head + "zone(50,6000);exit;" + tail());
    reset(5); exitAnswer = 1; msg_run__17UndeadKnightClone(obj); bad |= check("5 leave", head + "zone(50,6000);exit;" + set(3) + tail());
    // 13, 14..17
    for (int s : {13, 14, 15, 16, 17}) {
        for (unsigned adv : {0u, 3u, 8u}) {
            for (unsigned bits : {0u, 8u}) {
                for (int enemy : {0, 1}) {
                    for (int wait : {0, 1, 2, 5, -3}) {
                        reset(static_cast<short>(s)); advanceAnswer = static_cast<int>(adv); put16(294, static_cast<unsigned short>(bits)); put32(256, static_cast<unsigned>(wait));
                        if (enemy) put32(576, 0x600);
                        msg_run__17UndeadKnightClone(obj);
                        std::string want = head;
                        if (enemy) want += ((adv & 7) ? melee : "turn;");
                        if (s == 13) { if (!bits) want += set(3); }
                        else {
                            const int after = bits ? wait : wait - 1;
                            if (after <= 0) want += set(3);
                            if (static_cast<int>(get32(256)) != after) { std::puts("fail wait"); bad = 1; }
                        }
                        bad |= check("13..17", want + tail());
                    }
                }
            }
        }
    }
    // the burst: one shadow point whose weight cubed is 1.728e12 against a cut of 1.6e12 gives two particles
    for (float wind : {0.0f, 2.0f}) {
        reset(7); pointsAnswer = 1; put32(228, 0); std::memcpy(obj + 228, &wind, 4);
        weights.assign(100, 0.0f); weights.push_back(12000.0f);
        msg_run__17UndeadKnightClone(obj);
        std::string want = head + set(3) + "v128(A,0,0);points9320;";
        for (int i = 0; i < 100; ++i) want += scatterRound(0, "p80", 12);
        want += "v128(A,0,0);points9320;v128(A,0,0);shadow(9320,288,4:5:6,9,0,w,null,2);";
        for (int i = 0; i < 2; ++i) {
            want += "rand;rand;rand;rand;rand;rand;";
            if (wind != 0.0f) want += "cos9;sin9;";
            want += wind != 0.0f ? "particle(p80,2:1.5:3,0:0:0,0);" : "particle(p80,1:2:3,0:0:0,0);";
        }
        bad |= check("burst", want);
    }
    // state 10: five hundred scatter rounds (zero divisor), a burst, and the death sequence when the timer is over
    for (int late : {0, 1}) {
        for (void* alloc : {static_cast<void*>(nullptr), static_cast<void*>(arena + 0x3000)}) {
            reset(10); put32(256, late ? 50u : 200u); frameNm = 100; allocAnswer = alloc; put32(2556, 1); put32(2696, 1);
            msg_run__17UndeadKnightClone(obj);
            std::string want = "fx(2624,1152,2560,4:5:6,9,1024,288,0.5,0,0,1,null);" + head;
            for (int i = 0; i < 500; ++i) want += "v128(A,0,0);rand;shadow(9320,288,4:5:6,9,32768,w,null,2);rand;rand;rand;rand;rand;rand;particle(pd0,1:2:3,0:0:0,0);";
            want += "v128(A,0,0);points9320;";
            if (late) {
                want += "matrix(9320,4:5:6,9,288,0,2);alloc584;";
                if (alloc) want += "prop(1,0:0:0,matrix,udk.lmp,sword.vif,sword.tex,1,16641,null,2);";
                want += "matrix(9320,4:5:6,9,288,1,2);alloc584;";
                if (alloc) want += "prop(1,0:0:0,matrix,udk.lmp,shield.vif,shield.tex,1,16641,null,2);";
                want += "delete;remove;";
            }
            want += tail();
            bad |= check("10", want);
            if (late && (get32(2556) != 0 || get32(2696) != 0)) { std::puts("fail 10 words"); bad = 1; }
            if (!late && (get32(2556) != 1 || get32(2696) != 1)) { std::puts("fail 10 words kept"); bad = 1; }
        }
    }
    return bad;
}
