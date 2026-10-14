// Host contract for msg_run__12UndeadKnight; not an EE input. 32-bit words are offsets from a host
// base; callees log and answer from small queues. The authored function was also compared with the
// original in a private R5900 run (600 random objects over all states plus 900 on the busier states;
// every original word reached except padding).
#define W10_ACTOR_O_HOST 1
#include <cstdio>
#include <cstring>
#include <deque>
#include <string>
#include <initializer_list>
unsigned char* w10_actor_o_host_base;
#include "msg_run.cpp"

static unsigned char arena[0x10000] __attribute__((aligned(16)));
static unsigned char* const obj = arena + 0x1000;
static unsigned char* const other = arena + 0x6000;
unsigned char undead_knight_msg_run_particle_7d7e30[8];
int frameNm;
static std::string calls;
static std::deque<int> zone;
static int everyAnswer, advanceAnswer, exitAnswer, pointsAnswer;
static unsigned roll;
static std::string num(double v) { char b[40]; std::snprintf(b, sizeof b, "%g", v); return b; }
static unsigned off(const void* p) { return static_cast<unsigned>(static_cast<const unsigned char*>(p) - arena); }
static std::string pt(const Point3& p) { return num(p.x) + ":" + num(p.y) + ":" + num(p.z); }
extern "C" {
void fxRunWeapon__FPQ24item11descriptionP12_modelHeaderP13fxWeaponStateG6Point3sT1P14AnimationStatefiifP8Matrix34(
    void* d, void* m, void* s, Point3 p, short a, void* m2, void* an, float f, int g, int h, float i, void* mat) {
    calls += "fx(" + num(off(d) - 0x1000) + "," + num(off(m)) + "," + num(off(s) - 0x1000) + "," + pt(p) + "," + num(a) + "," + num(off(m2)) + "," + num(off(an) - 0x1000) + "," + num(f) + "," + num(g) + "," + num(h) + "," + num(i) + "," + (mat ? "mat" : "null") + ");";
}
void objectUpdateInGrid__FP10GameObject(void*) { calls += "grid;"; }
void newDamageHandler__8CreatureP12_modelHeader(void*, void* m) { calls += "dmg" + num(off(m)) + ";"; }
int everyFrame__8CreatureP12_modelHeader(void*, void* m) { calls += "every" + num(off(m)) + ";"; return everyAnswer; }
int objectActivationCheck__8Creaturei(void*, int v) { calls += "act" + num(v) + ";"; return 0; }
int creatureAdvanceAnimation__8Creaturef(void*, float f) { calls += "adv" + num(f) + ";"; return advanceAnswer; }
void slideBack__8Creaturef(void*, float f) { calls += "slide" + num(f) + ";"; }
int move2__8CreatureG6Point3(void*, Point3 p) { calls += "move2(" + pt(p) + ");"; return 0; }
int enemyInZone__8Creaturefs(void*, float r, short a) {
    calls += "zone(" + num(r) + "," + num(a) + ");";
    if (zone.empty()) return 0;
    const int v = zone.front(); zone.pop_front(); return v;
}
void smartWander__8Creature(void*) { calls += "wander;"; }
unsigned eRand__10GameObject(void*) { calls += "rand;"; return roll; }
void steerAndRouteToEnemy__8Creature(void*) { calls += "steer;"; }
int bExitStatePersueIdle__8Creature(void*) { calls += "exit;"; return exitAnswer; }
void turnToFaceEnemy__8Creature(void*) { calls += "turn;"; }
void produceMeleeDamage__8CreaturefiiiiPQ24item11description(void*, float amount, int a, int b, int c, int d, void* desc) {
    calls += "melee(" + num(amount) + "," + num(a) + "," + num(b) + "," + num(c) + "," + num(d) + "," + (desc ? "desc" : "null") + ");";
}
int modelGetNmShadowPoints__FP12_modelHeader(void* m) { calls += "points" + num(off(m)) + ";"; return pointsAnswer; }
void modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f(
    void* m, void* an, Point3 p, short yaw, int index, Point3* out, float* extra, void* matrix, float scale) {
    const bool mine = off(an) == 0x1000 + 288;
    calls += "shadow(" + num(off(m)) + "," + (mine ? "mine" : "other") + "," + pt(p) + "," + num(yaw) + "," + num(index) + "," + (extra ? "extra" : "null") + "," + (matrix ? "matrix" : "null") + "," + num(scale) + ");";
    *out = mine ? Point3{1, 2, 3} : Point3{11, 22, 33};
}
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* d, Point3* p, Point3* v, float age) {
    calls += "particle(" + num(d == undead_knight_msg_run_particle_7d7e30) + "," + pt(*p) + "," + pt(*v) + "," + num(age) + ");";
}
void soundFadeStream__Fi(int s) { calls += "fade" + num(s) + ";"; }
void objectRemoveFromRunList__FP10GameObject(void*) { calls += "remove;"; }
}
unsigned w10_actor_o_host_call(unsigned fw, void* adjusted, int n, unsigned long a1, unsigned long) {
    std::string who = off(adjusted) == 0x1000 ? "A" : off(adjusted) == 0x6000 ? "T" : num(off(adjusted));
    calls += "v" + num(fw) + "(" + who + "," + num(n) + "," + num(static_cast<long>(a1)) + ");"; return 0x2468;
}
static void put32(unsigned char* base, unsigned o, unsigned v) { std::memcpy(base + o, &v, 4); }
static void putA(unsigned a, unsigned v) { std::memcpy(arena + a, &v, 4); }
static unsigned get32(unsigned o) { unsigned v; std::memcpy(&v, obj + o, 4); return v; }
static void put16(unsigned o, unsigned short v) { std::memcpy(obj + o, &v, 2); }
static void reset(short state) {
    std::memset(arena, 0, sizeof arena); w10_actor_o_host_base = arena; calls.clear(); zone.clear();
    everyAnswer = 0; advanceAnswer = 0; exitAnswer = 0; pointsAnswer = 4; roll = 0; frameNm = 100;
    put32(obj, 36, 0x300); putA(0x300 + 12, 0x400); putA(0x300 + 180, 0x480); put32(obj, 72, 0x500);
    putA(0x500 + 128, 0); putA(0x500 + 132, 0x80); putA(0x500 + 168, 0); putA(0x500 + 172, 0x68); putA(0x500 + 184, 0); putA(0x500 + 188, 0xb8);
    put16(572, static_cast<unsigned short>(state));
    put32(obj, 28, 0x41200000u);  // 10.0
    put32(obj, 960, 0);
    float pos[3] = {4, 5, 6}; std::memcpy(obj + 16, pos, 12);
    short h = 9; std::memcpy(obj + 56, &h, 2);
    put16(200, 11); put16(202, 12); put16(204, 13);
    put32(obj, 808, 100);
}
static const std::string head = "dmg1024;every1024;act1200;adv1;slide0.5;v104(A,0,0);move2(0:0:0);";
static int check(const char* label, const std::string& want) {
    if (calls == want) return 0;
    std::printf("fail %s\n  want %s\n  got  %s\n", label, want.c_str(), calls.c_str()); return 1;
}
static std::string set(int n) { return "v184(A,1," + num(n) + ");"; }
int main() {
    int bad = 0;
    const std::string z400 = "zone(400,32767);", z500 = "zone(500,16000);";
    const std::string melee = "melee(36,11,12,13,262152,null);turn;";
    // preamble
    reset(2); put32(obj, 968, 5); put32(obj, 960, 1); put16(572, 6); everyAnswer = 1; put32(obj, 36, 0x300);
    msg_run__12UndeadKnight(obj);
    bad |= check("weapon", "fx(1040,1152,972,4:5:6,9,1024,288,0.5,1,0,1,null);grid;dmg1024;every1024;");
    for (int s : {13, 5}) {
        reset(static_cast<short>(s)); put32(obj, 968, 1); everyAnswer = 1; msg_run__12UndeadKnight(obj);
        bad |= check("weapon flag", std::string("fx(1040,1152,972,4:5:6,9,1024,288,0.5,") + (s == 13 ? "1" : "0") + ",0,1,null);dmg1024;every1024;");
    }
    reset(7); msg_run__12UndeadKnight(obj); bad |= check("idle", head + set(3));   // grid word zero: move2 runs
    reset(7); put32(obj, 960, 1); msg_run__12UndeadKnight(obj); bad |= check("grid", "grid;dmg1024;every1024;act1200;adv1;slide0.5;v104(A,0,0);" + set(3));
    for (int s : {2, 4, 6, 11, 12, 21, 40, -1}) { reset(static_cast<short>(s)); msg_run__12UndeadKnight(obj); bad |= check("default", head + set(3)); }
    // 0
    reset(0); zone = {1}; msg_run__12UndeadKnight(obj); bad |= check("0 near", head + z400 + set(3));
    reset(0); zone = {0, 1}; msg_run__12UndeadKnight(obj); bad |= check("0 far", head + z400 + z500 + set(3));
    reset(0); put32(obj, 256, 5); msg_run__12UndeadKnight(obj); bad |= check("0 wait", head + z400 + z500 + set(1));
    reset(0); put32(obj, 256, 100); msg_run__12UndeadKnight(obj); bad |= check("0 hold", head + z400 + z500);
    // 1
    reset(1); zone = {1}; msg_run__12UndeadKnight(obj); bad |= check("1 near", head + z400 + set(0));
    reset(1); zone = {0, 1}; msg_run__12UndeadKnight(obj); bad |= check("1 far", head + z400 + z500 + set(0));
    reset(1); msg_run__12UndeadKnight(obj); bad |= check("1 wander", head + z400 + z500 + "wander;");
    // 3
    reset(3); msg_run__12UndeadKnight(obj); bad |= check("3 none", head + set(0));
    for (unsigned r : {10u, 64u, 65u, 164u}) {
        const bool low = r % 100 < 65;
        reset(3); put32(obj, 576, 0x600); zone = {1}; roll = r; msg_run__12UndeadKnight(obj);
        bad |= check("3 a", head + "zone(70,23300);rand;" + set(low ? 16 : 14));
        reset(3); put32(obj, 576, 0x600); zone = {0, 1}; roll = r; msg_run__12UndeadKnight(obj);
        bad |= check("3 b", head + "zone(70,23300);zone(90,11000);rand;" + set(low ? 14 : 15));
        reset(3); put32(obj, 576, 0x600); zone = {0, 0, 1}; roll = r; msg_run__12UndeadKnight(obj);
        bad |= check("3 c", head + "zone(70,23300);zone(90,11000);zone(90,6000);rand;" + set(low ? 13 : 15));
    }
    for (int wait : {50, 150}) {
        reset(3); put32(obj, 576, 0x600); put32(obj, 256, static_cast<unsigned>(wait)); zone = {0, 0, 0, 1}; msg_run__12UndeadKnight(obj);
        bad |= check("3 d", head + "zone(70,23300);zone(90,11000);zone(90,6000);zone(130,3000);" + (100 < wait ? "steer;" : set(17)));
    }
    for (unsigned w964 : {0u, 0x700u}) {
        reset(3); put32(obj, 576, 0x600); put32(obj, 964, w964); msg_run__12UndeadKnight(obj);
        bad |= check("3 e", head + "zone(70,23300);zone(90,11000);zone(90,6000);zone(130,3000);" + (w964 ? "steer;" : set(19)));
    }
    // 5
    reset(5); zone = {1}; msg_run__12UndeadKnight(obj); bad |= check("5 near", head + "zone(50,6000);" + set(6));
    reset(5); msg_run__12UndeadKnight(obj); bad |= check("5 stay", head + "zone(50,6000);exit;");
    reset(5); exitAnswer = 1; msg_run__12UndeadKnight(obj); bad |= check("5 leave", head + "zone(50,6000);exit;" + set(3));
    // 8, 9
    reset(8); put16(294, 0x10); msg_run__12UndeadKnight(obj); bad |= check("8 hold", head);
    reset(8); put16(294, 0xffef); msg_run__12UndeadKnight(obj); bad |= check("8 leave", head + set(3));
    reset(9); put32(obj, 804, 5); put32(obj, 808, 100); msg_run__12UndeadKnight(obj); bad |= check("9 less", head + set(3));
    reset(9); put32(obj, 804, 100); put32(obj, 808, 100); msg_run__12UndeadKnight(obj); bad |= check("9 equal", head);
    reset(9); put32(obj, 804, 0xffffffffu); msg_run__12UndeadKnight(obj); bad |= check("9 signed", head + set(3));
    // 10
    for (unsigned w964 : {0u, 0x700u}) {
        for (unsigned b : {0u, 0x80u}) {
            reset(10); put32(obj, 964, w964); putA(0x700 + 12, 0x6000); put32(other, 12, 0xffffffffu); put16(294, static_cast<unsigned short>(b));
            msg_run__12UndeadKnight(obj);
            bad |= check("10", head + (b ? "fade0;remove;" : ""));
            unsigned w; std::memcpy(&w, other + 12, 4);
            if (w != (w964 ? 0xfffffffdu : 0xffffffffu)) { std::puts("fail 10 bit"); bad = 1; }
        }
    }
    // 13
    for (unsigned adv : {0u, 1u, 8u}) {
        for (unsigned bits : {0u, 8u}) {
            for (int enemy : {0, 1}) {
                reset(13); advanceAnswer = static_cast<int>(adv); put16(294, static_cast<unsigned short>(bits)); if (enemy) put32(obj, 576, 0x600);
                msg_run__12UndeadKnight(obj);
                std::string want = head;
                if (enemy) want += ((adv & 7) ? melee : "turn;");
                if (!bits) want += set(3);
                bad |= check("13", want);
            }
        }
    }
    // 14..17 and 18, 20
    for (int s : {14, 15, 16, 17, 18, 20}) {
        for (unsigned adv : {0u, 3u}) {
            for (unsigned bits : {0u, 8u}) {
                for (int enemy : {0, 1}) {
                    for (int wait : {0, 1, 2, 5, -3}) {
                        reset(static_cast<short>(s)); advanceAnswer = static_cast<int>(adv); put16(294, static_cast<unsigned short>(bits)); put32(obj, 256, static_cast<unsigned>(wait));
                        if (enemy) put32(obj, 576, 0x600);
                        msg_run__12UndeadKnight(obj);
                        std::string want = head;
                        if (s <= 17 && enemy) want += ((adv & 7) ? melee : "turn;");
                        const int after = bits ? wait : wait - 1;
                        if (after <= 0) want += set(3);
                        bad |= check("14..20", want);
                        if (static_cast<int>(get32(256)) != after) { std::puts("fail wait"); bad = 1; }
                    }
                }
            }
        }
    }
    // 19: 75 rounds between the setup and the tail
    const unsigned kbits = 0x3e991622u;
    float k; std::memcpy(&k, &kbits, 4);
    const std::string velocity = pt(Point3{(11.0f - 1.0f) * k, (22.0f - 2.0f) * k, (33.0f - 3.0f) * k});
    for (int w964 : {0, 1}) {
        for (unsigned adv : {0u, 1u}) {
            for (unsigned bits : {0u, 8u}) {
                reset(19); advanceAnswer = static_cast<int>(adv); put16(294, static_cast<unsigned short>(bits));
                if (w964) { put32(obj, 964, 0x700); putA(0x700 + 12, 0x6000); put32(other, 72, 0x900); putA(0x900 + 128, 0); putA(0x900 + 132, 0x81); put32(other, 12, 0xffffffffu); }
                float opos[3] = {40, 50, 60}; std::memcpy(other + 16, opos, 12); short oh = 5; std::memcpy(other + 56, &oh, 2);
                float scale = 2.0f; std::memcpy(other + 224, &scale, 4);
                roll = 7; msg_run__12UndeadKnight(obj);
                std::string want = head;
                if (w964) {
                    want += "v128(A,0,0);points9320;v129(T,0,0);points9320;";
                    for (int i = 0; i < 75; ++i)
                        want += "v128(A,0,0);rand;shadow(9320,mine,4:5:6,9,3,null,null,0);v129(T,0,0);rand;shadow(9320,other,40:50:60,5,3,null,null,2);particle(1,1:2:3," + velocity + ",0);";
                }
                if (!bits) want += set(3);
                bad |= check("19", want);
                unsigned w; std::memcpy(&w, other + 12, 4);
                if (w964 && w != ((adv & 1) ? 0xfffffffdu : 0xffffffffu)) { std::puts("fail 19 bit"); bad = 1; }
                short h148, h150, h152, h154; std::memcpy(&h148, obj + 148, 2); std::memcpy(&h150, obj + 150, 2); std::memcpy(&h152, obj + 152, 2); std::memcpy(&h154, obj + 154, 2);
                if (h148 != -150 || h150 != -150 || h152 != -150 || h154 != 0) { std::puts("fail 19 halfwords"); bad = 1; }
            }
        }
    }
    return bad;
}
