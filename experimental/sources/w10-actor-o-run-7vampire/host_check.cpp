// Host contract for msg_run__7Vampire; not an EE input. 32-bit words are offsets from a host base;
// callees log and answer from small queues. The authored function was also compared with the original
// in a private R5900 run (600 random objects over all states, 1500 on the busier states, 300 on
// state 10 around its thresholds; every original word reached except padding).
#define W10_ACTOR_O_HOST 1
#include <cstdio>
#include <cstring>
#include <deque>
#include <string>
#include <initializer_list>
unsigned char* w10_actor_o_host_base;
#include "msg_run.cpp"

static unsigned char arena[0x8000] __attribute__((aligned(16)));
static unsigned char* const obj = arena + 0x1000;
static std::string calls;
static std::deque<int> zone;
static int everyAnswer, activeAnswer = 1, advanceAnswer, crowdAnswer, exitAnswer;
static unsigned roll;
static std::string num(double v) { char b[40]; std::snprintf(b, sizeof b, "%g", v); return b; }
static unsigned off(const void* p) { return static_cast<unsigned>(static_cast<const unsigned char*>(p) - arena); }
extern "C" {
void newDamageHandler__8CreatureP12_modelHeader(void*, void* m) { calls += "dmg" + num(off(m)) + ";"; }
int everyFrame__8CreatureP12_modelHeader(void*, void* m) { calls += "every" + num(off(m)) + ";"; return everyAnswer; }
int objectActivationCheck__8Creaturei(void*, int v) { calls += "act" + num(v) + ";"; return activeAnswer; }
int creatureAdvanceAnimation__8Creaturef(void*, float f) { calls += "adv" + num(f) + ";"; return advanceAnswer; }
void slideBack__8Creaturef(void*, float f) { calls += "slide" + num(f) + ";"; }
int move2__8CreatureG6Point3(void*, Point3 p) { calls += "move2(" + num(p.x) + ":" + num(p.y) + ":" + num(p.z) + ");"; return 0; }
int enemyInZone__8Creaturefs(void*, float r, short a) {
    calls += "zone(" + num(r) + "," + num(a) + ");";
    if (zone.empty()) return 0;
    const int v = zone.front(); zone.pop_front(); return v;
}
void smartWander__8Creature(void*) { calls += "wander;"; }
unsigned eRand__10GameObject(void*) { calls += "rand;"; return roll; }
int objectIsCrowded__8CreatureP10GameObjectffi(void*, void* other, float a, float b, int c) {
    calls += "crowd(" + num(off(other)) + "," + num(a) + "," + num(b) + "," + num(c) + ");"; return crowdAnswer;
}
void steerAndRouteToEnemy__8Creature(void*) { calls += "steer;"; }
int bExitStatePersueIdle__8Creature(void*) { calls += "exit;"; return exitAnswer; }
void turnToFaceEnemy__8Creature(void*) { calls += "turn;"; }
void produceMeleeDamage__8CreaturefiiiiPQ24item11description(void*, float amount, int a, int b, int c, int d, void* desc) {
    calls += "melee(" + num(amount) + "," + num(a) + "," + num(b) + "," + num(c) + "," + num(d) + "," + (desc ? "desc" : "null") + ");";
}
void fxRunCreatureFire__FP12_modelHeaderP14AnimationStateG6Point3sP15FxCreatureStateffff(
    void* m, void* an, Point3 p, short s, void* st, float a, float b, float c, float d) {
    calls += "fire(" + num(off(m)) + "," + num(off(an) - 0x1000) + "," + num(p.x) + ":" + num(p.y) + ":" + num(p.z) + "," + num(s) + "," + num(off(st) - 0x1000) + "," + num(a) + "," + num(b) + "," + num(c) + "," + num(d) + ");";
}
void objectRemoveFromRunList__FP10GameObject(void*) { calls += "remove;"; }
}
unsigned w10_actor_o_host_call(unsigned fw, void* adjusted, int n, unsigned long a1, unsigned long) {
    calls += "v" + num(fw) + "(" + num(off(adjusted) - 0x1000) + "," + num(n) + "," + num(static_cast<long>(a1)) + ");"; return 0;
}
static void put32(unsigned o, unsigned v) { std::memcpy(obj + o, &v, 4); }
static void putA(unsigned a, unsigned v) { std::memcpy(arena + a, &v, 4); }
static unsigned get32(unsigned o) { unsigned v; std::memcpy(&v, obj + o, 4); return v; }
static void put16(unsigned o, unsigned short v) { std::memcpy(obj + o, &v, 2); }
static void reset(short state) {
    std::memset(arena, 0, sizeof arena); w10_actor_o_host_base = arena; calls.clear(); zone.clear();
    everyAnswer = 0; activeAnswer = 1; advanceAnswer = 0; crowdAnswer = 0; exitAnswer = 0; roll = 0;
    put32(36, 0x300); putA(0x300 + 12, 0x400); put32(72, 0x500);
    putA(0x500 + 168, 0x10000 | 0); putA(0x500 + 172, 0x68);   // entry at +168: adjustment 0, function word 0x68
    putA(0x500 + 184, 0); putA(0x500 + 188, 0xb8);              // entry at +184: adjustment 0, function word 0xb8
    put16(572, static_cast<unsigned short>(state));
    put32(28, 0x41200000u);  // 10.0
    put32(224, 0x40200000u); put32(228, 0x40800000u); obj[40] = 7;
    put32(808, 100);
    short h = 9; std::memcpy(obj + 56, &h, 2);
    float pt[3] = {4, 5, 6}; std::memcpy(obj + 16, pt, 12);
    put16(200, 11); put16(202, 12); put16(204, 13);
}
static const std::string head = "dmg1024;every1024;act600;adv1;slide0.5;v104(0,0,0);move2(0:0:0);";
static int check(const char* label, const std::string& want) {
    if (calls == want) return 0;
    std::printf("fail %s\n  want %s\n  got  %s\n", label, want.c_str(), calls.c_str()); return 1;
}
static std::string set(int n) { return "v184(0,1," + num(n) + ");"; }
int main() {
    int bad = 0;
    // the preamble can end the call
    reset(0); everyAnswer = 1; msg_run__7Vampire(obj); bad |= check("every", "dmg1024;every1024;");
    reset(0); activeAnswer = 0; msg_run__7Vampire(obj); bad |= check("active", "dmg1024;every1024;act600;");
    for (int s : {2, 4, 7, 9, 11, 12, 17, 40, -1, 32767}) { reset(static_cast<short>(s)); msg_run__7Vampire(obj); bad |= check("idle", head); }
    const std::string z1 = "zone(400,32767);", z2 = "zone(500,18000);";
    // 0
    reset(0); zone = {1}; msg_run__7Vampire(obj); bad |= check("0 near", head + z1 + set(3));
    reset(0); zone = {0, 1}; msg_run__7Vampire(obj); bad |= check("0 far zone", head + z1 + z2 + set(3));
    reset(0); put32(256, 5); msg_run__7Vampire(obj); bad |= check("0 wait", head + z1 + z2 + set(1));
    reset(0); put32(256, 100); msg_run__7Vampire(obj); bad |= check("0 no wait", head + z1 + z2);
    reset(0); put32(256, 0xffffffffu); msg_run__7Vampire(obj); bad |= check("0 signed", head + z1 + z2 + set(1));
    // 1 and 13
    for (int s : {1, 13}) {
        reset(static_cast<short>(s)); msg_run__7Vampire(obj); bad |= check("1 wander", head + z1 + z2 + "wander;");
        reset(static_cast<short>(s)); zone = {0, 1}; msg_run__7Vampire(obj); bad |= check("1 zone", head + z1 + z2 + set(3));
    }
    // 3
    reset(3); msg_run__7Vampire(obj); bad |= check("3 none", head + set(0));
    reset(3); put32(576, 0x600); putA(0x600 + 12, 0x700); zone = {1}; msg_run__7Vampire(obj);
    bad |= check("3 close", head + "zone(30,5000);" + set(6));
    reset(3); put32(576, 0x600); putA(0x600 + 12, 0x700); zone = {0, 1}; roll = 4; msg_run__7Vampire(obj);
    bad |= check("3 melee near", head + "zone(30,5000);zone(90,3000);rand;" + set(15));
    reset(3); put32(576, 0x600); putA(0x600 + 12, 0x700); zone = {0, 1}; roll = 5; msg_run__7Vampire(obj);
    bad |= check("3 melee other", head + "zone(30,5000);zone(90,3000);rand;" + set(16));
    reset(3); put32(576, 0x600); putA(0x600 + 12, 0x700); crowdAnswer = 1; msg_run__7Vampire(obj);
    bad |= check("3 crowded", head + "zone(30,5000);zone(90,3000);crowd(1792,60,30,0);" + set(14));
    reset(3); put32(576, 0x600); putA(0x600 + 12, 0x700); msg_run__7Vampire(obj);
    bad |= check("3 steer", head + "zone(30,5000);zone(90,3000);crowd(1792,60,30,0);steer;");
    // 5
    reset(5); zone = {1}; msg_run__7Vampire(obj); bad |= check("5 close", head + "zone(30,5000);" + set(6));
    reset(5); msg_run__7Vampire(obj); bad |= check("5 stay", head + "zone(30,5000);exit;");
    reset(5); exitAnswer = 1; msg_run__7Vampire(obj); bad |= check("5 leave", head + "zone(30,5000);exit;" + set(3));
    // 6 and 16 (melee on bit 0 of the advance answer, turning on bit 3 of the flags)
    for (int s : {6, 16}) {
        for (int adv : {0, 1}) {
            for (int bits : {0, 8}) {
                for (int enemy : {0, 1}) {
                    for (int wait : {0, 1, 2, 5, -3}) {
                        reset(static_cast<short>(s)); advanceAnswer = adv; put16(294, static_cast<unsigned short>(bits));
                        put32(256, static_cast<unsigned>(wait));
                        if (enemy) put32(576, 0x600);
                        msg_run__7Vampire(obj);
                        const std::string pre = "dmg1024;every1024;act600;adv1;slide0.5;v104(0,0,0);move2(0:0:0);";
                        std::string tail;
                        int after = wait;
                        if (s == 6) {
                            bool ended = false;
                            if (enemy) {
                                if (adv) tail += "melee(36,11,12,13,8,null);";
                                if (!bits) { tail += set(15); ended = true; } else tail += "turn;";
                            }
                            if (!ended && !bits) tail += set(15);
                        } else {
                            if (enemy) {
                                if (adv) tail += "melee(80,11,12,13,8,null);";
                                if (bits) tail += "turn;";
                            }
                            after = bits ? wait : wait - 1;
                            if (after <= 0) tail += set(3);
                        }
                        if (calls != pre + tail) {
                            std::printf("fail %d adv=%d bits=%d enemy=%d wait=%d\n  want %s\n  got  %s\n", s, adv, bits, enemy, wait, (pre + tail).c_str(), calls.c_str());
                            bad = 1;
                        }
                        if (s == 16 && static_cast<int>(get32(256)) != after) { std::puts("fail 16 counter"); bad = 1; }
                    }
                }
            }
        }
    }
    // 8
    reset(8); put16(294, 0x10); msg_run__7Vampire(obj); bad |= check("8 hold", head);
    reset(8); put16(294, 0xffef); msg_run__7Vampire(obj); bad |= check("8 leave", head + set(3));
    // 15
    reset(15); put16(294, 8); msg_run__7Vampire(obj); bad |= check("15 hold", head);
    reset(15); put16(294, 0xfff7); msg_run__7Vampire(obj); bad |= check("15 leave", head + set(3));
    // 14
    reset(14); msg_run__7Vampire(obj); bad |= check("14 none", head + set(0));
    reset(14); put32(576, 0x600); putA(0x600 + 12, 0x700); put32(256, 5); msg_run__7Vampire(obj); bad |= check("14 wait", head + set(3) + "turn;");
    reset(14); put32(576, 0x600); putA(0x600 + 12, 0x700); put32(256, 100); msg_run__7Vampire(obj); bad |= check("14 free", head + "crowd(1792,60,30,1);" + set(3) + "turn;");
    reset(14); put32(576, 0x600); putA(0x600 + 12, 0x700); put32(256, 100); crowdAnswer = 1; msg_run__7Vampire(obj); bad |= check("14 crowded", head + "crowd(1792,60,30,1);turn;");
    // 10
    for (int wait : {0, 39, 40, 41}) {
        reset(10); put32(256, static_cast<unsigned>(wait)); put32(12, 0x10); put32(160, 1); put32(164, 0);
        msg_run__7Vampire(obj);
        const bool late = wait + 60 < 100, drop = static_cast<float>(wait) + 75.0f < 100.0f;
        std::string want = head + "fire(1024,288,4:5:6,9,824,1,2.5,4,7);" + (drop ? "remove;" : "");
        bad |= check("10", want);
        if (((get32(12) & 2) != 0) != late || ((get32(164) & 0x200) != 0) != late) { std::puts("fail 10 flags"); bad = 1; }
    }
    return bad;
}
