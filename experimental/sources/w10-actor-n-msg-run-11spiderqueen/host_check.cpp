// Host contract for msg_run__11SpiderQueen; not an EE input. 32-bit words are offsets from a host base;
// callees log. The authored function was also compared with the original in a private R5900 run (see the
// notes file for the scenario counts and coverage). The calls made by the common start are cut from the
// log; the common finish (slot loop) is checked separately.
#define W10_ACTOR_N_HOST 1
#include <cstdio>
#include <cstring>
#include <string>
unsigned char* w10_actor_n_host_base;
#include "msg_run.cpp"

int world;
unsigned skills[64];
static unsigned char arena[0x8000] __attribute__((aligned(16)));
static unsigned char* const obj = arena + 0x1000;
static std::string calls;
static int everyResult, advanceResult, bexResult, grabResult, activeResult, atanResult;
static unsigned rollValue;
static unsigned rollQueue[16];
static int rollCount, rollPos;
static bool zone[16];
static int zoneIndex, clearFaceBit;
static void* allocResult;
static std::string num(double v) { char b[40]; std::snprintf(b, sizeof b, "%g", v); return b; }
static unsigned off(const void* p) { return static_cast<unsigned>(static_cast<const unsigned char*>(p) - arena); }
static std::string pt(const Point3* p) { return num(p->x) + ":" + num(p->y) + ":" + num(p->z); }
extern "C" {
void newDamageHandler__8CreatureP12_modelHeader(void*, void* m) { calls += "dmg(" + num(off(m)) + ");"; }
int everyFrame__8CreatureP12_modelHeader(void*, void* m) { calls += "every;"; return everyResult; }
void objectActivationCheck__8Creaturei(void*, int r) { calls += "act" + num(r) + ";"; }
int creatureAdvanceAnimation__8Creaturef(void*, float f) { calls += "adv" + num(f) + ";"; return advanceResult; }
int slideBack__8Creaturef(void*, float f) { calls += "slide" + num(f) + ";"; return 7; }
void run__3WebG6Point3i(void* w, Point3* p, int t) { calls += "webrun(" + num(static_cast<unsigned char*>(w) - obj) + "," + pt(p) + "," + num(t) + ");"; }
void draw__3WebG6Point3(void* w, Point3* p) { calls += "webdraw(" + pt(p) + ");"; calls.clear(); }   // end of the common start
void start__3WebG6Point3P8Creaturefi(void* w, Point3* p, void* c, float f, int t) { calls += "webstart(" + pt(p) + "," + num(off(c)) + "," + num(f) + "," + num(t) + ");"; }
int bActive__3Web(void*) { calls += "active;"; return activeResult; }
void breakWeb__3Web(void*) { calls += "webbreak;"; }
int enemyInZone__8Creaturefs(void*, float f, short a) { calls += "zone(" + num(f) + "," + num(a) + ");"; return zone[zoneIndex++ & 15] ? 1 : 0; }
int canGrab__11SpiderQueen(void*) { calls += "grab;"; return grabResult; }
void smartWander__8Creature(void*) { calls += "wander;"; }
void steerAndRouteToEnemy__8Creature(void*) { calls += "steer;"; }
int bExitStatePersueIdle__8Creature(void*) { calls += "bexit;"; return bexResult; }
void turnToFaceEnemy__8Creature(void*) {
    calls += "turn;";
    if (clearFaceBit) { short f; std::memcpy(&f, obj + 294, 2); f = static_cast<short>(f & ~8); std::memcpy(obj + 294, &f, 2); }
}
void produceMeleeDamage__8CreaturefiiiiPQ24item11description(void*, float r, int a, int b, int c, int flags, void* d) {
    const unsigned char* q = static_cast<const unsigned char*>(d);
    short e0, e1, e2, e3; std::memcpy(&e0, q + 36, 2); std::memcpy(&e1, q + 38, 2); std::memcpy(&e2, q + 40, 2); std::memcpy(&e3, q + 42, 2);
    calls += "melee(" + num(r) + "," + num(a) + "," + num(b) + "," + num(c) + "," + std::to_string(flags) + ",desc" + num(e0) + "/" + num(e1) + "/" + num(e2) + "/" + num(e3) + "/" + num(q[32]) + ");";
}
void gameSetCameraShake__Ffb(float f, bool b) { calls += "shake(" + num(f) + "," + num(b) + ");"; }
void floorAdd__FG6Point3sfii(Point3 p, short a, float f, int b, int c) { calls += "floor(" + pt(&p) + "," + num(a) + "," + num(f) + "," + num(b) + "," + num(c) + ");"; }
void soundFadeStream__Fi(int s) { calls += "fade" + num(s) + ";"; }
void objectRemoveFromRunList__FP10GameObject(void*) { calls += "rmrun;"; }
void modelGetCharAttachmentPos__FP12_modelHeaderG6Point3sP14AnimationStateiP6Point3T5f(void* m, Point3 p, short a, void* an, int i, Point3* o1, Point3* o2, float f) {
    calls += "pos(" + num(off(m)) + "," + pt(&p) + "," + num(a) + "," + num(i) + "," + (o2 ? "o2" : "null") + "," + num(f) + ");";
    *o1 = Point3{p.x + 1, p.y + 2, p.z + 3};
    if (o2) *o2 = Point3{1, 2, 3};
}
void findEnemyLeadPosition__8CreaturefP6Point3(void*, float f, Point3* p) { calls += "lead(" + num(f) + "," + pt(p) + ");"; }
int iatan2__Fff(float y, float x) { calls += "atan(" + num(y) + "," + num(x) + ");"; return atanResult; }
unsigned eRand__10GameObject(void*) { calls += "rand;"; return rollPos < rollCount ? rollQueue[rollPos++] : rollValue; }
float icos__Fi(int a) { calls += "cos" + num(a) + ";"; return 0.5f; }
float isin__Fi(int a) { calls += "sin" + num(a) + ";"; return 0.25f; }
void* blockAlloc__Fi(int n) { calls += "alloc" + num(n) + ";"; return allocResult; }
void __16PoisonProjectilePQ210GameObject11resourceMapiiiiG6Point3T6ifP8Creature(void*, void* map, int a, int b, int c, int d, Point3 p, Point3 v, int e, float f, void* cr) {
    calls += "proj(" + num(off(map)) + "," + num(a) + "," + num(b) + "," + num(c) + "," + num(d) + "," + pt(&p) + "," + pt(&v) + "," + num(e) + "," + num(f) + "," + (cr == obj ? "self" : "?") + ");";
}
float worldFindPointHeight__FP12_worldHeaderG6Point3(void*, Point3 p) { calls += "height(" + pt(&p) + ");"; return 12.5f; }
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* d, Point3* p, Point3* v, float f) {
    calls += std::string(d == BloodDropParticle ? "blood" : "?") + "(" + pt(p) + "," + pt(v) + "," + num(f) + ");";
}
void __11ParticleDefiiiiiiiifffiiiPFPv_v(void* s, int a, int b, int c, int d, int e, int f, int g, int h, float x, float y, float z, int i, int j, int k, void (*cb)(void*)) {
    calls += std::string("pdef(") + (s == BloodDropParticle ? "blood" : "?") + "," + num(a) + "," + num(b) + "," + num(c) + "," + num(d) + "," + num(e) + "," + num(f) + "," + num(g) + "," + num(h) + "," +
             num(x) + "," + num(y) + "," + num(z) + "," + num(i) + "," + num(j) + "," + num(k) + "," + (cb ? "cb" : "null") + ");";
}
}
unsigned w10_actor_n_host_call(unsigned fw, void* adjusted, int n, unsigned long a1, unsigned long a2) {
    const unsigned at = off(adjusted);
    calls += "v" + num(fw) + "(" + num(at >= 0x1000 && at < 0x1600 ? at - 0x1000 : at) + "," + num(n) + "," + num(a1) + (n > 1 ? "," + num(a2) : std::string()) + ");";
    return fw == 0x200 ? 100u : 0u;      // the skill entry returns 100
}
static void put32(unsigned a, unsigned v) { std::memcpy(obj + a, &v, 4); }
static void putA(unsigned a, unsigned v) { std::memcpy(arena + a, &v, 4); }
static void putf(unsigned a, float v) { std::memcpy(obj + a, &v, 4); }
static void put16(unsigned a, int v) { short s = static_cast<short>(v); std::memcpy(obj + a, &s, 2); }
static float getf(unsigned a) { float v; std::memcpy(&v, obj + a, 4); return v; }
static unsigned get32(unsigned a) { unsigned v; std::memcpy(&v, obj + a, 4); return v; }
static short get16(unsigned a) { short v; std::memcpy(&v, obj + a, 2); return v; }
static void reset(int state, bool target) {
    std::memset(arena, 0, sizeof arena); w10_actor_n_host_base = arena; calls.clear();
    everyResult = 0; advanceResult = 0; bexResult = 0; grabResult = 0; activeResult = 1; atanResult = 0; rollValue = 32768; rollCount = 0; rollPos = 0; zoneIndex = 0; clearFaceBit = 0; allocResult = nullptr;
    for (bool& z : zone) z = false;
    bloodDropReady = 1;
    put32(36, 0x300); putA(0x300 + 12, 0x400); put32(72, 0x800);
    putA(0x800 + 168, 8); putA(0x800 + 172, 0xa8); putA(0x800 + 184, 0); putA(0x800 + 188, 0xb8);
    put16(572, state);
    if (target) { put32(576, 0x500); putA(0x500 + 12, 0x600); }
    putf(28, 30.0f); putf(16, 100); putf(20, 200); putf(24, 300);
    put16(200, 11); put16(202, 12); put16(204, 13); put16(56, 1000);
    std::memset(skills, 0, sizeof skills); skills[26] = 0x700; putA(0x700 + 96, 0x720); putA(0x720 + 96, 0); putA(0x720 + 100, 0x200);
}
static int bad;
static void check(const char* label, const std::string& want) {
    if (calls != want) { std::printf("fail %s\n  want %s\n  got  %s\n", label, want.c_str(), calls.c_str()); bad = 1; }
}
static void go() { msg_run__11SpiderQueen(obj); }
int main() {
    auto st = [](int n) { return "v184(0,1," + num(n) + ");"; };
    // everyFrame ends the call before anything is drawn
    reset(0, true); everyResult = 1; go(); check("every", "dmg(1024);every;");
    // idle states do nothing before the finish
    for (int s : {2, 4, 7, 11, 12, 21, 40}) { reset(s, true); go(); check("idle", ""); }
    // The finish builds the particle record once (guard) and does not repeat it.
    reset(2, true); bloodDropReady = 0; go();
    check("pdef", "pdef(blood,39,108,40,53,40,87,19,0,10.5,1.2,-4.65,3,0,6,null);");
    if (bloodDropReady != 1) { std::puts("fail guard"); bad = 1; }
    // 0, 1
    reset(0, true); put32(256, 0); put32(808, 5); go(); check("0 timer", "zone(600,32767);zone(600,18000);" + st(1));
    reset(0, true); put32(256, 5); put32(808, 5); go(); check("0 wait", "zone(600,32767);zone(600,18000);");
    reset(0, true); zone[0] = true; go(); check("0 near", "zone(600,32767);" + st(3));
    reset(0, true); zone[1] = true; go(); check("0 near 2", "zone(600,32767);zone(600,18000);" + st(3));
    reset(1, true); go(); check("1 wander", "zone(600,32767);zone(600,18000);wander;");
    reset(1, true); zone[1] = true; go(); check("1 near", "zone(600,32767);zone(600,18000);" + st(3));
    // 3 and 5
    reset(3, false); go(); check("3 no target", st(0));
    reset(3, true); go(); check("3 none", "zone(600,32767);zone(60,2000);grab;grab;grab;grab;zone(450,3000);steer;");
    reset(3, true); zone[0] = true; go(); check("3 far", "zone(600,32767);zone(600,16384);" + st(20));
    reset(3, true); zone[0] = true; zone[1] = true; zone[2] = true; go(); check("3 close", "zone(600,32767);zone(600,16384);zone(60,2000);" + st(6));
    reset(3, true); grabResult = 1; zone[2] = true; go(); check("3 grab 13", "zone(600,32767);zone(60,2000);grab;zone(450,3000);" + st(13));
    reset(3, true); grabResult = 1; zone[3] = true; go(); check("3 grab 15", "zone(600,32767);zone(60,2000);grab;zone(450,3000);grab;zone(225,5000);" + st(15));
    reset(3, true); grabResult = 1; zone[4] = true; go(); check("3 grab 16", "zone(600,32767);zone(60,2000);grab;zone(450,3000);grab;zone(225,5000);grab;zone(250,5000);" + st(16));
    reset(3, true); grabResult = 1; zone[5] = true; go(); check("3 grab 17", "zone(600,32767);zone(60,2000);grab;zone(450,3000);grab;zone(225,5000);grab;zone(250,5000);grab;zone(250,6000);" + st(17));
    reset(3, true); zone[2] = true; go(); check("3 plain 18", "zone(600,32767);zone(60,2000);grab;grab;grab;grab;zone(450,3000);" + st(18));
    reset(5, false); go(); check("5 none", "zone(600,32767);zone(60,2000);grab;grab;grab;grab;zone(450,3000);bexit;");
    reset(5, false); bexResult = 1; go(); check("5 exit", "zone(600,32767);zone(60,2000);grab;grab;grab;grab;zone(450,3000);bexit;" + st(3));
    // Skill entry 26 (through its table at +96) is called with (0, 1); it returns 100 here.
    const std::string skill = "v512(1792,2,0,1);";
    // 6: one attack when bit 0 of the advance result is set; description 23 / 100 / 100 + (int)(100 * 0.2 * roll * 2^-16)
    reset(6, true); advanceResult = 1; rollValue = 32768; put16(294, 8); go();
    check("6 attack", skill + "rand;melee(54,11,12,13,589828,desc23/100/109/0/255);");
    reset(6, true); advanceResult = 2; put16(294, 8); go(); check("6 no attack", "");
    reset(6, true); advanceResult = 1; rollValue = 0; put16(294, 0); go(); check("6 leaves", skill + "rand;melee(54,11,12,13,589828,desc23/100/100/0/255);" + st(3));
    reset(6, false); advanceResult = 1; put16(294, 0); go(); check("6 no target", st(3));
    // 8, 9
    reset(8, true); put16(294, 0x10); go(); check("8 set", ""); reset(8, true); put16(294, 0); go(); check("8 clear", st(3));
    reset(9, true); put32(804, 4); put32(808, 5); go(); check("9 <", "webrun"[0] ? st(0) : "");
    reset(9, true); put32(804, 5); put32(808, 5); go(); check("9 =", "");
    // 10: bit 3 of the advance result and bit 1 of the word at +12 clear
    reset(10, true); advanceResult = 8; rollValue = 0x12345; put16(294, 0x80); go();
    check("10", "shake(8,0);rand;floor(100:200:300," + num(static_cast<short>(0x12345)) + ",100,6,1);fade0;rmrun;");
    reset(10, true); advanceResult = 8; put32(12, 2); put16(294, 0); go(); check("10 skipped", "");
    reset(10, true); advanceResult = 0; put16(294, 0x80); go(); check("10 remove only", "rmrun;");
    // 13
    reset(13, true); putA(0x600 + 12, 0x20); advanceResult = 1; activeResult = 0; go();
    check("13 start", "webstart(100:200:310,1536,300,0);active;" + st(3));
    reset(13, true); putA(0x600 + 12, 0); advanceResult = 1; activeResult = 1; put32(980, 4); go(); check("13 no start", "active;" + st(14));
    reset(13, true); activeResult = 1; go(); check("13 idle", "active;");
    // 14
    reset(14, true); activeResult = 0; go(); check("14 inactive", "active;" + st(3));
    reset(14, false); go(); check("14 no target", "active;");
    reset(14, true); zone[0] = true; go(); check("14 break", "active;zone(80,2000);webbreak;" + st(15) + "turn;");
    reset(14, true); go(); check("14 turn", "active;zone(80,2000);turn;");
    // 15: sets the word at +960 from bit 0, clears it from bit 2; moves the approach point; attack from bit 1
    reset(15, false); put16(294, 0); go(); check("15 no target", st(3));
    reset(15, true); advanceResult = 4; put32(960, 1); putA(0x600 + 12, 0x20); putA(0x600 + 756, 0x55); put16(294, 8); go();
    check("15 clear", ""); if (get32(960) != 0 || *reinterpret_cast<unsigned*>(arena + 0x600 + 756) != 0) { std::puts("fail 15 clear stores"); bad = 1; }
    reset(15, true); advanceResult = 1; put16(294, 8); putA(0x600 + 12, 0); go();
    check("15 approach", "cos1000;sin1000;pos(1024,100:200:300,1000,0,null,1);");
    if (get32(960) != 1) { std::puts("fail 15 word"); bad = 1; }
    reset(15, true); advanceResult = 2; put16(294, 8); go(); check("15 attack", skill + "rand;melee(54,11,12,13,65540,desc23/100/109/0/255);");
    // 16 and 17 share one body
    for (int s : {16, 17}) {
        reset(s, true); putA(0x600 + 12, 0x20); advanceResult = 1; put16(294, 8); go();
        if (get32(960) != 1) { std::puts("fail 16 word"); bad = 1; }
        check("16 zone", "zone(250,5000);");
        reset(s, true); put32(960, 1); zone[0] = true; put16(294, 8); go(); check("16 approach", "zone(250,5000);cos1000;sin1000;pos(1024,100:200:300,1000,0,null,1);");
        reset(s, true); advanceResult = 2; zone[0] = false; put16(294, 8); go(); check("16 near attack", "zone(250,5000);zone(60,2000);");
        reset(s, true); advanceResult = 2; zone[1] = true; put16(294, 8); go(); check("16 attack", "zone(250,5000);zone(60,2000);" + skill + "rand;melee(54,11,12,13,65540,desc23/100/109/0/255);");
        reset(s, true); putA(0x600 + 12, 0x20); putA(0x600 + 756, 0x55); put16(294, 0); go(); check("16 leave", "zone(250,5000);" + st(3));
        if (*reinterpret_cast<unsigned*>(arena + 0x600 + 756) != 0) { std::puts("fail 16 clears 756"); bad = 1; }
    }
    // 18: the spray of two poison projectiles
    reset(18, true); advanceResult = 0; put16(294, 8); go(); check("18 none", "");
    reset(18, true); advanceResult = 1; put16(294, 0); allocResult = arena + 0x2000; rollValue = 32768; atanResult = 0; go();
    {
        const std::string head = st(3) + "cos1000;sin1000;lead(300,250:275:300);cos1000;sin1000;pos(1024,100:200:300,1000,0,null,1);";
        if (calls.compare(0, head.size(), head) != 0) { std::printf("fail 18 head\n  want %s\n  got  %s\n", head.c_str(), calls.c_str()); bad = 1; }
        size_t a = calls.find("proj("), b = calls.find("proj(", a + 1);
        if (a == std::string::npos || b == std::string::npos || calls.find("proj(", b + 1) != std::string::npos || calls.find("alloc168;") == std::string::npos) { std::printf("fail 18 projectiles\n  %s\n", calls.c_str()); bad = 1; }
        if (calls.find(",1000-16384") != std::string::npos) {}
    }
    reset(18, true); advanceResult = 1; put16(294, 8); allocResult = nullptr; go();
    if (calls.find("proj(") != std::string::npos || calls.find("alloc168;alloc168;") != std::string::npos && false) { std::puts("fail 18 null alloc"); bad = 1; }
    // 19, 20
    reset(19, true); advanceResult = 1; put16(294, 8); go(); check("19", "shake(4,0);");
    reset(19, true); advanceResult = 0; put16(294, 0); go(); check("19 leave", st(3));
    reset(20, true); advanceResult = 3; put16(294, 8); put32(960, 0); go(); check("20 flip + shake + turn", "shake(5,0);turn;"); if (get32(960) != 1) { std::puts("fail 20 flip"); bad = 1; }
    reset(20, true); advanceResult = 1; put16(294, 8); put32(960, 1); go(); check("20 flip off", ""); if (get32(960) != 0) { std::puts("fail 20 flip off"); bad = 1; }
    // Finish: a spray particle per live slot, a splash one time in twenty.
    reset(2, true); put32(808, 100); put32(1028 + 4 * 2, 160); go();
    {
        const std::string pre = "pos(1024,100:200:300,1000,2,o2,0);";
        if (calls.compare(0, pre.size(), pre) != 0) { std::printf("fail slot start\n  %s\n", calls.c_str()); bad = 1; }
        if (calls.find("blood(101:202:303,") == std::string::npos) { std::printf("fail slot particle\n  %s\n", calls.c_str()); bad = 1; }
    }
    reset(2, true); put32(808, 100); put32(1028 + 4 * 2, 160); rollQueue[0] = 32768; rollQueue[1] = 32768; rollQueue[2] = 32768; rollQueue[3] = 40; rollCount = 4; go();
    if (calls.find("height(101:202:303);rand;rand;rand;floor(101:202:14.5,") == std::string::npos) { std::printf("fail splash\n  %s\n", calls.c_str()); bad = 1; }
    reset(2, true); put32(808, 100); put32(1028 + 4 * 2, 50); rollQueue[0] = 7; rollCount = 1; go(); if (calls != "rand;") { std::printf("fail expired no roll hit\n  %s\n", calls.c_str()); bad = 1; }
    reset(2, true); put32(808, 100); put32(1028 + 4 * 2, 50); rollQueue[0] = 70; rollCount = 1; go(); if (calls.find("blood(") == std::string::npos) { std::printf("fail expired roll hit\n  %s\n", calls.c_str()); bad = 1; }
    return bad;
}
