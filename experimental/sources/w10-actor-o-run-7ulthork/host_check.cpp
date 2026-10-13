// Host contract for msg_run__7Ulthork; not an EE input. 32-bit words are offsets from a host base;
// callees log and answer from small queues. The authored function was also compared with the original
// in a private R5900 run (600 random objects over all states, 1200 on the busier states, mutations of
// every constant in the spell branch detected; every reachable original word reached).
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
static unsigned char* const deadObj = arena + 0x6000;
float worldDifficulty;
unsigned skills[64];
static std::string calls;
static std::deque<int> zone;
static int everyAnswer, activeAnswer = 1, advanceAnswer, exitAnswer, zone2Answer;
static unsigned roll, rollSecond;
static int rollCount;
static void* deadAnswer;
static short yawAnswer;
static void* allocAnswer;
static unsigned skillAnswers[4];
static std::string num(double v) { char b[40]; std::snprintf(b, sizeof b, "%g", v); return b; }
static unsigned off(const void* p) { return static_cast<unsigned>(static_cast<const unsigned char*>(p) - arena); }
static std::string pt(const Point3& p) { return num(p.x) + ":" + num(p.y) + ":" + num(p.z); }
extern "C" {
void newDamageHandler__8CreatureP12_modelHeader(void*, void* m) { calls += "dmg" + num(off(m)) + ";"; }
int everyFrame__8CreatureP12_modelHeader(void*, void* m) { calls += "every" + num(off(m)) + ";"; return everyAnswer; }
int objectActivationCheck__8Creaturei(void*, int v) { calls += "act" + num(v) + ";"; return activeAnswer; }
int creatureAdvanceAnimation__8Creaturef(void*, float f) { calls += "adv" + num(f) + ";"; return advanceAnswer; }
void slideBack__8Creaturef(void*, float f) { unsigned b; std::memcpy(&b, &f, 4); char t[16]; std::snprintf(t, sizeof t, "%x", b); calls += std::string("slide") + t + ";"; }
int enemyInZone__8Creaturefs(void*, float r, short a) {
    calls += "zone(" + num(r) + "," + num(a) + ");";
    if (zone.empty()) return 0;
    const int v = zone.front(); zone.pop_front(); return v;
}
int enemyInZone2__8Creatureffssfi(void*, float a, float b, short c, short d, float e, int f) {
    calls += "zone2(" + num(a) + "," + num(b) + "," + num(c) + "," + num(d) + "," + num(e) + "," + num(f) + ");"; return zone2Answer;
}
void smartWander__8Creature(void*) { calls += "wander;"; }
unsigned eRand__10GameObject(void*) { calls += "rand;"; return rollCount++ == 0 ? roll : rollSecond; }
void steerAndRouteToEnemy__8Creature(void*) { calls += "steer;"; }
int bExitStatePersueIdle__8Creature(void*) { calls += "exit;"; return exitAnswer; }
void turnToFaceEnemy__8Creature(void*) { calls += "turn;"; }
void produceMeleeDamage__8CreaturefiiiiPQ24item11description(void*, float amount, int a, int b, int c, int d, void* desc) {
    calls += "melee(" + num(amount) + "," + num(a) + "," + num(b) + "," + num(c) + "," + num(d) + "," + (desc ? "desc" : "null") + ");";
}
float icos__Fi(int a) { calls += "cos" + num(a) + ";"; return 0.5f; }
float isin__Fi(int a) { calls += "sin" + num(a) + ";"; return -0.25f; }
void findEnemyLeadPosition__8CreaturefP6Point3(void*, float r, Point3* p) { calls += "lead(" + num(r) + "," + pt(*p) + ");"; }
void* ISeeDeadPeople__8Creaturef(void*, float r) { calls += "dead" + num(r) + ";"; return deadAnswer; }
short creatureFindLeadYaw__8CreatureP10GameObjectf(void*, void* o, float r) { calls += "yaw(" + num(off(o)) + "," + num(r) + ");"; return yawAnswer; }
void* blockAlloc__Fi(int n) { calls += "alloc" + num(n) + ";"; return allocAnswer; }
void __12ShockOfFrostG6Point3siP10GameObjectP5Skill(void* self, Point3 p, short yaw, int level, void* owner, void* skill) {
    calls += "shock(" + num(self == allocAnswer) + "," + pt(p) + "," + num(yaw) + "," + num(level) + "," + num(off(owner) - 0x1000) + "," + num(reinterpret_cast<__UINTPTR_TYPE__>(skill) & 0xffffffff) + ");";
}
void __15FrostStormSpellG6Point3ifiP10GameObjectT1(void* self, Point3 p, int a, float b, int c, void* owner, Point3 v) {
    calls += "storm(" + num(self == allocAnswer) + "," + pt(p) + "," + num(a) + "," + num(b) + "," + num(c) + "," + num(off(owner) - 0x1000) + "," + pt(v) + ");";
}
char* lumpFindResource__FPcT0(char* l, char* n) { calls += std::string("lump(") + l + "," + n + ");"; return reinterpret_cast<char*>(arena + 0x7000); }
int soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* h, void* o, float v) { calls += "seq(" + num(off(h)) + "," + num(off(o)) + "," + num(v) + ");"; return 0; }
void objectRemoveFromRunList__FP10GameObject(void*) { calls += "remove;"; }
}
unsigned w10_actor_o_host_call(unsigned fw, void* adjusted, int n, unsigned long a1, unsigned long) {
    std::string who = off(adjusted) == 0x1000 ? "A" : off(adjusted) == 0x6000 ? "D" : num(off(adjusted));
    calls += "v" + num(fw) + "(" + who + "," + num(n) + "," + num(static_cast<long>(a1)) + ");"; return 0;
}
unsigned w10_actor_o_host_call3(unsigned fw, void* adjusted, unsigned a1, int a2, unsigned a3) {
    calls += "skill" + num(fw) + "(" + num(off(adjusted)) + "," + num(a1) + "," + num(a2) + "," + num(a3 - 0) + ");";
    return skillAnswers[a1];
}
static void put32(unsigned char* base, unsigned o, unsigned v) { std::memcpy(base + o, &v, 4); }
static void putA(unsigned a, unsigned v) { std::memcpy(arena + a, &v, 4); }
static unsigned get32(unsigned o) { unsigned v; std::memcpy(&v, obj + o, 4); return v; }
static void put16(unsigned o, unsigned short v) { std::memcpy(obj + o, &v, 2); }
static short get16(unsigned o) { short v; std::memcpy(&v, obj + o, 2); return v; }
static void reset(short state) {
    std::memset(arena, 0, sizeof arena); w10_actor_o_host_base = arena; calls.clear(); zone.clear();
    everyAnswer = 0; activeAnswer = 1; advanceAnswer = 0; exitAnswer = 0; zone2Answer = 0; roll = 0; rollSecond = 0; rollCount = 0;
    deadAnswer = nullptr; yawAnswer = 77; allocAnswer = arena + 0x3000; worldDifficulty = 2.5f;
    for (int i = 0; i < 4; ++i) skillAnswers[i] = static_cast<unsigned>(10 + i);
    std::memset(skills, 0, sizeof skills);
    put32(obj, 36, 0x300); putA(0x300 + 12, 0x400); put32(obj, 72, 0x500);
    putA(0x500 + 168, 0); putA(0x500 + 172, 0x68); putA(0x500 + 184, 0); putA(0x500 + 188, 0xb8);
    put16(572, static_cast<unsigned short>(state));
    put32(obj, 28, 0x41200000u);  // 10.0
    float pos[3] = {4, 5, 6}; std::memcpy(obj + 16, pos, 12);
    float s32 = 8.0f; std::memcpy(obj + 32, &s32, 4);
    short h = 9; std::memcpy(obj + 56, &h, 2);
    put16(200, 11); put16(202, 12); put16(204, 13);
    put32(obj, 808, 100);
}
static const std::string head = "dmg1024;every1024;act600;adv1;slide3e999999;v104(A,0,0);";
static int check(const char* label, const std::string& want) {
    if (calls == want) return 0;
    std::printf("fail %s\n  want %s\n  got  %s\n", label, want.c_str(), calls.c_str()); return 1;
}
static std::string set(int n) { return "v184(A,1," + num(n) + ");"; }
int main() {
    int bad = 0;
    const std::string z400 = "zone(400,32767);";
    reset(0); everyAnswer = 1; msg_run__7Ulthork(obj); bad |= check("every", "dmg1024;every1024;");
    reset(0); activeAnswer = 0; msg_run__7Ulthork(obj); bad |= check("active", "dmg1024;every1024;act600;");
    for (int s : {2, 4, 7, 11, 12, 14, 40, -1}) { reset(static_cast<short>(s)); msg_run__7Ulthork(obj); bad |= check("idle", head); }
    // 0 and 1
    reset(0); zone = {1}; msg_run__7Ulthork(obj); bad |= check("0 near", head + z400 + set(3));
    reset(0); put32(obj, 256, 5); msg_run__7Ulthork(obj); bad |= check("0 wait", head + z400 + set(1));
    reset(0); put32(obj, 256, 100); msg_run__7Ulthork(obj); bad |= check("0 hold", head + z400);
    reset(1); zone = {1}; msg_run__7Ulthork(obj); bad |= check("1 near", head + z400 + set(3));
    reset(1); put32(obj, 256, 5); msg_run__7Ulthork(obj); bad |= check("1 wait", head + z400 + set(0));
    reset(1); put32(obj, 256, 100); msg_run__7Ulthork(obj); bad |= check("1 wander", head + z400 + "wander;");
    // 3
    reset(3); msg_run__7Ulthork(obj); bad |= check("3 none", head + set(0));
    reset(3); put32(obj, 576, 0x600); zone = {1}; msg_run__7Ulthork(obj); bad |= check("3 close", head + "zone(30,5000);" + set(6));
    reset(3); put32(obj, 576, 0x600); zone = {0, 1}; msg_run__7Ulthork(obj); bad |= check("3 face", head + "zone(30,5000);zone(30,32767);turn;");
    for (int b : {0, 2, 3}) {
        for (int z2 : {0, 1}) {
            reset(3); put32(obj, 576, 0x600); obj[1104] = static_cast<unsigned char>(b); zone2Answer = z2; msg_run__7Ulthork(obj);
            std::string want = head + "zone(30,5000);zone(30,32767);";
            if (b == 2) want += "zone2(0,400,9,5000,0,1);";
            want += (b == 2 && z2) ? set(13) : "steer;";
            bad |= check("3 spell", want);
        }
    }
    // 5
    reset(5); zone = {1}; msg_run__7Ulthork(obj); bad |= check("5 close", head + "zone(30,5000);" + set(6));
    reset(5); obj[1104] = 2; zone = {0, 1}; msg_run__7Ulthork(obj); bad |= check("5 spell", head + "zone(30,5000);zone(400,5000);" + set(13));
    reset(5); obj[1104] = 2; msg_run__7Ulthork(obj); bad |= check("5 spell stay", head + "zone(30,5000);zone(400,5000);exit;");
    reset(5); obj[1104] = 1; exitAnswer = 1; msg_run__7Ulthork(obj); bad |= check("5 leave", head + "zone(30,5000);exit;" + set(3));
    // 6
    for (unsigned adv : {0u, 1u, 2u, 3u}) {
        for (unsigned bits : {0u, 8u}) {
            for (int enemy : {0, 1}) {
                for (unsigned r1 : {0u, 8u, 1u}) {
                    for (unsigned r2 : {0u, 4u, 3u, 5u, 10u}) {
                        reset(6); advanceAnswer = static_cast<int>(adv); put16(294, static_cast<unsigned short>(bits)); if (enemy) put32(obj, 576, 0x600);
                        roll = r1; rollSecond = r2; msg_run__7Ulthork(obj);
                        std::string want = head;
                        int draws = 0;
                        auto next = [&]() { return draws++ == 0 ? r1 : r2; };
                        if (enemy) {
                            if (adv & 1) {
                                want += "rand;";
                                int k;
                                if ((next() & 7) == 0) k = 0x80008;
                                else { want += "rand;"; k = ((next() & 3) == 0) ? 0x40008 : 8; }
                                want += "melee(36,11,12,13," + num(k) + ",null);";
                            }
                            if (adv & 2) { want += "rand;"; if (next() % 5 == 0) want += set(3); }
                            if (!bits) { want += set(3); bad |= check("6", want); continue; }
                            want += "turn;";
                        }
                        if (!bits) want += set(3);
                        bad |= check("6", want);
                    }
                }
            }
        }
    }
    // 8, 9, 10
    reset(8); put16(294, 0x10); msg_run__7Ulthork(obj); bad |= check("8 hold", head);
    reset(8); put16(294, 0xffef); msg_run__7Ulthork(obj); bad |= check("8 leave", head + set(0));
    reset(9); put32(obj, 804, 5); msg_run__7Ulthork(obj); bad |= check("9 less", head + set(3));
    reset(9); put32(obj, 804, 100); msg_run__7Ulthork(obj); bad |= check("9 equal", head);
    reset(10); put16(294, 0x80); msg_run__7Ulthork(obj); bad |= check("10 remove", head + "remove;");
    reset(10); put16(294, 0x7f); msg_run__7Ulthork(obj); bad |= check("10 keep", head);
    // 13
    const std::string tail_zone = "zone(30,5000);";
    reset(13); put32(obj, 256, 5); msg_run__7Ulthork(obj); bad |= check("13 no enemy, wait", head + set(3));
    reset(13); put32(obj, 256, 100); msg_run__7Ulthork(obj); bad |= check("13 no enemy, hold", head + tail_zone);
    reset(13); put32(obj, 256, 100); zone = {1}; msg_run__7Ulthork(obj); bad |= check("13 no enemy, close", head + tail_zone + set(6));
    for (unsigned adv : {0u, 1u}) {
        for (int kind : {0, 1, 2}) {
            for (void* alloc : {static_cast<void*>(nullptr), static_cast<void*>(arena + 0x3000)}) {
                reset(13); put32(obj, 576, 0x600); putA(0x600 + 12, 0x700); advanceAnswer = static_cast<int>(adv); allocAnswer = alloc;
                put32(obj, 256, 100); roll = kind == 1 ? 1u : 0u; deadAnswer = kind == 2 ? static_cast<void*>(deadObj) : nullptr;
                put32(deadObj, 72, 0x900); putA(0x900 + 184, 0); putA(0x900 + 188, 200);
                skills[58] = 0x2000; skills[23] = 0x4444; putA(0x2000 + 96, 0x2800); putA(0x2800 + 104, 0x10); putA(0x2800 + 108, 153);
                msg_run__7Ulthork(obj);
                // t = {4 + 300 * 0.5, 5 + 300 * -0.25, 6}
                std::string want = head + "cos9;sin9;lead(400,154:-70:6);";
                if (adv & 1) {
                    want += "rand;dead300;";
                    if (kind == 0) {
                        want += "yaw(1792,350);alloc208;";
                        if (alloc) want += "shock(1,4:5:10,77,2,0,17476);";
                    } else if (kind == 1) {
                        for (int i = 0; i < 4; ++i) want += "skill153(8208," + num(i) + ",2,4096);";
                        want += "cos9;sin9;alloc172;";
                        if (alloc) want += "storm(1,154:-70:6,720,132,10,0,6.5:-3.25:0);";
                    } else {
                        want += "v200(D,1,14);lump(fx.lmp,spel_he_heal.anm);seq(28672,24576,1);";
                    }
                }
                want += "turn;zone(30,5000);";
                bad |= check("13", want);
                const short expect[] = {0, 128, 255, 50, 0, 128, 255, 150};
                for (int i = 0; i < 8; ++i)
                    if ((adv & 1) && get16(1048 + 2 * i) != expect[i]) { std::puts("fail 13 description"); bad = 1; }
                for (int i = 0; i < 8; ++i)
                    if (!(adv & 1) && get16(1048 + 2 * i) != 0) { std::puts("fail 13 description written"); bad = 1; }
            }
        }
    }
    return bad;
}
