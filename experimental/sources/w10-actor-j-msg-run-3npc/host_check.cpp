// Host contract for msg_run__3NPC; not an EE input. 32-bit words in the object are offsets from a
// host base (W10_ACTOR_J_HOST). Callees are stubs that append to a log; each case compares the log
// with the sequence read from the original's control flow. The authored function was also compared
// with the original in a private R5900 run (250 mixed scenarios, 302 of 303 words covered).
#define W10_ACTOR_J_HOST 1
#include <cstdio>
#include <cstring>
#include <string>
unsigned char* w10_actor_j_host_base;
#include "msg_run.cpp"

static unsigned char arena[0x8000] __attribute__((aligned(16)));
static unsigned char* const obj = arena + 0x1000;
static std::string calls;
static int activation;
static unsigned rolls[32]; static int rollIndex;
static std::string num(double v) { char b[40]; std::snprintf(b, sizeof b, "%g", v); return b; }
static unsigned off(const void* p) { return static_cast<unsigned>(static_cast<const unsigned char*>(p) - arena); }

extern "C" {
char levelName[400];
unsigned char npc_msg_run_particle_7d7dd0[16];
int everyFrame__8CreatureP12_modelHeader(void*, void* h) { calls += "ef(" + num(off(h)) + ");"; return 7; }
int objectActivationCheck__8Creaturei(void*, int r) { calls += "act" + num(r) + ";"; return activation; }
unsigned creatureAdvanceAnimation__8Creaturef(void*, float a) { calls += "adv" + num(a) + ";"; return 0; }
void objectUpdateInGrid__FP10GameObject(void*) { calls += "grid;"; }
void smartWander__8Creature(void* self) { float f; std::memcpy(&f, static_cast<unsigned char*>(self) + 176, 4); calls += "wander(" + num(f) + ");"; }
unsigned eRand__10GameObject(void*) { calls += "rand;"; return rolls[rollIndex++ & 31]; }
double fptodp(float v) { return v; }
int dpcmp(double a, double b) { return a < b ? -1 : a > b ? 1 : 0; }
double dpsub(double a, double b) { return a - b; }
void modelGetCharAttachmentPos__FP12_modelHeaderG6Point3sP14AnimationStateiP6Point3T5f(void* m, Point3 p, short a, void* s, int b, Point3* first, Point3* second, float f) {
    calls += "attach(" + num(off(m)) + "," + num(p.x) + ":" + num(p.y) + ":" + num(p.z) + "," + num(a) + "," + num(off(s) - 0x1000) + "," + num(b) + "," + num(f) + ");";
    *first = Point3{10, 20, 30}; *second = Point3{1, 2, 3};
}
void P_AddParticle__FP11ParticleDefP6Point3T1f(void*, Point3* p, Point3* v, float f) {
    calls += "particle(" + num(p->x) + ":" + num(p->y) + ":" + num(p->z) + "," + num(v->x) + ":" + num(v->y) + ":" + num(v->z) + "," + num(f) + ");";
}
}
unsigned w10_actor_j_host_call(unsigned fw, void* adjusted, int n, unsigned long a1, unsigned long a2) {
    calls += "v" + num(fw) + "(" + num(off(adjusted)) + "," + num(n) + "," + num(a1) + "," + num(a2) + ");";
    return 0;
}

static void put32(unsigned o, unsigned v) { std::memcpy(obj + o, &v, 4); }
static void putA(unsigned a, unsigned v) { std::memcpy(arena + a, &v, 4); }
static void putf(unsigned o, float v) { std::memcpy(obj + o, &v, 4); }
static void put16(unsigned o, unsigned short v) { std::memcpy(obj + o, &v, 2); }
static unsigned get32(unsigned o) { unsigned v; std::memcpy(&v, obj + o, 4); return v; }
static float getf(unsigned o) { float v; std::memcpy(&v, obj + o, 4); return v; }
static unsigned short get16(unsigned o) { unsigned short v; std::memcpy(&v, obj + o, 2); return v; }

static void reset(const char* name, const char* level, const char* lump, int state) {
    std::memset(arena, 0, sizeof arena); std::memset(levelName, 0, sizeof levelName);
    w10_actor_j_host_base = arena;
    put32(72, 0x200); putA(0x200 + 184, 8); putA(0x200 + 188, 0xb8);   // table: adjustment 8, function word 0xb8
    put32(1056, 0x300);
    std::strcpy(reinterpret_cast<char*>(obj + 1068), name); std::strcpy(levelName + 81, level); std::strcpy(reinterpret_cast<char*>(obj + 960), lump);
    put16(572, static_cast<unsigned short>(state));
    calls.clear(); activation = 1; rollIndex = 0;
    for (int i = 0; i < 32; ++i) rolls[i] = 0x8000;
}
static int expect(const char* label, const std::string& want) {
    if (calls == want) return 0;
    std::printf("fail %s\n  want %s\n  got  %s\n", label, want.c_str(), calls.c_str());
    return 1;
}

int main() {
    int bad = 0;
    // Name rewrite: "firiona" outside "airhub" changes the four halfwords; inactive and no parent ends early.
    reset("FiRiOnA", "x", "", 5); activation = 0; msg_run__3NPC(obj);
    bad |= expect("early", "ef(768);act600;");
    if (get16(142) != 50 || get16(144) != 154 || get16(146) != 83 || get16(140) != 50 || get16(138) != 0) { std::puts("fail halfwords"); bad = 1; }
    reset("FiRiOnA", "AIRHUB", "", 5); msg_run__3NPC(obj);
    bad |= expect("airhub", "ef(768);act600;adv1;");
    if (get16(142) != 0) { std::puts("fail airhub halfwords"); bad = 1; }
    // Parent: its table entry at +144 is called with the two field addresses, then the grid update.
    reset("n", "", "", 0); put32(1132, 0x2000); putA(0x2000 + 72, 0x2100); putA(0x2100 + 144, 0xfff0); putA(0x2100 + 148, 0x77);
    msg_run__3NPC(obj);
    bad |= expect("parent", "ef(768);adv1;v119(" + num(0x2000 - 16) + ",2," + num(0x1000 + 16) + "," + num(0x1000 + 56) + ");grid;");
    // State 0 with bit 27 of the 64-bit flags: table entry +184 with argument 1.
    reset("n", "", "", 0); put32(160, 0x08000000u); msg_run__3NPC(obj);
    bad |= expect("state0", "ef(768);act600;adv1;v184(" + num(0x1008) + ",1,1,0);");
    reset("n", "", "", 0); put32(164, 0xffffffffu); put32(160, 0xf7ffffffu); msg_run__3NPC(obj);
    bad |= expect("state0 no flag", "ef(768);act600;adv1;");
    // State 1: |+180| above 0.5 clears +176 for the wander call; the saved integer comes back as a float.
    reset("n", "", "", 1); putf(176, 7.9f); putf(180, -0.75f); putf(184, 0.0f); msg_run__3NPC(obj);
    bad |= expect("state1 a", "ef(768);act600;adv1;wander(0);");
    if (getf(176) != 7.0f) { std::puts("fail restore"); bad = 1; }
    reset("n", "", "", 1); putf(176, -3.5f); putf(180, 0.5f); putf(184, 0.5001f); msg_run__3NPC(obj);
    bad |= expect("state1 b", "ef(768);act600;adv1;wander(0);");
    if (getf(176) != -3.0f) { std::puts("fail restore b"); bad = 1; }
    reset("n", "", "", 1); putf(176, 2.25f); putf(180, 0.5f); putf(184, -0.5f); msg_run__3NPC(obj);
    bad |= expect("state1 c", "ef(768);act600;adv1;wander(2.25);");
    // King lump: attachment points, particle only when the first roll is a multiple of 4.
    reset("n", "", "we_king.lmp", 3); put16(56, 5); putf(224, 2.5f); putf(16, 4); putf(20, 5); putf(24, 6);
    rolls[0] = 1; msg_run__3NPC(obj);
    bad |= expect("king no", "ef(768);act600;adv1;attach(768,4:5:6,5,288,0,2.5);rand;");
    reset("n", "", "we_king.lmp", 3); put16(56, 5); putf(224, 2.5f);
    rolls[0] = 4; rolls[1] = 0x8000 + 0x4000; rolls[2] = 0x8000; rolls[3] = 0x8000 - 0x2000;      // first ball: (0.5,0,-0.25)
    rolls[4] = 0xffff; rolls[5] = 0xffff; rolls[6] = 0xffff;                                       // rejected: length above 1
    rolls[7] = 0x8000 + 0x2000; rolls[8] = 0x8000 - 0x4000; rolls[9] = 0x8000;                      // second ball: (0.25,-0.5,0)
    msg_run__3NPC(obj);
    bad |= expect("king particle", "ef(768);act600;adv1;attach(768,0:0:0,5,288,0,2.5);rand;rand;rand;rand;rand;rand;rand;rand;rand;rand;particle(10.25:19.5:30,1:2:3,0);");
    // Other lump names do nothing more.
    reset("n", "", "x.lmp", 7); msg_run__3NPC(obj);
    bad |= expect("other lump", "ef(768);act600;adv1;");
    return bad;
}
