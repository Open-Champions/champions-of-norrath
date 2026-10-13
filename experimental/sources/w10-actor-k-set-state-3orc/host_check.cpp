// Host contract for setState__3Orci; not an EE input. Words that hold addresses are offsets from a
// host base. Every callee is a stub that appends to a log; the header words in the resource block
// t are their own offsets, so the log shows which record each animation call used. Random values
// come from a fixed list. The authored function is compared against the original in a private
// R5900 run (2,200 mixed scenarios, 794 of 811 original words executed; the others are padding).
#define W10_ACTOR_K_HOST 1
#include <cstdio>
#include <cstring>
#include <cmath>
#include <string>
unsigned char* w10_actor_k_host_base;
extern "C" {
int g_BloodLevel;
signed char netNetworkMode;
}
#include "set_state.cpp"
alignas(16) static unsigned char arena[0x8000];
static unsigned char* const obj = arena + 0x1000;
static std::string calls;
static unsigned rolls[8]; static int rollIndex;
static int processing_result, new_state;
static bool have_commander;
static unsigned commander_at = 0x5000, enemy_obj_at = 0x5400, enemy_at = 0x5800;
static std::string num(double v) { char b[40]; std::snprintf(b, sizeof b, "%g", v); return b; }
static unsigned off(const void* p) { return static_cast<unsigned>(static_cast<const unsigned char*>(p) - arena); }
unsigned w10_actor_k_host_call(unsigned, void* adjusted, int, unsigned long, unsigned long) { calls += "entry192(" + num(off(adjusted) - 0x1000) + ");"; return 0; }
extern "C" {
int setStateProcessing__8CreaturePi(void*, int* s) { calls += "proc(" + num(*s) + ");"; if (new_state >= 0) *s = new_state; return processing_result; }
void setWeapon__3Orc(void*) { calls += "setweapon;"; }
void initType__3Orcb(void*, int f) { calls += "inittype" + num(f) + ";"; }
void objectAddToRunList__FP10GameObject(void*) { calls += "addrun;"; }
unsigned eRand__10GameObject(void*) { calls += "rand;"; return rolls[rollIndex++ & 7]; }
void resetRouting__8Creature(void*) { calls += "reset;"; }
void getSmartRandomDestination__8Creaturei(Point3* out, void*, int range) { calls += "smart" + num(range) + ";"; out->x = 1; out->y = 2; out->z = 3; }
void animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(void* s, void* h, float f, int m) { calls += "att(" + num(off(s) - 0x1000) + "," + num(off(h)) + "," + num(f) + "," + num(m) + ");"; }
void animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(void*, void* h, float a, float b, int m) { calls += "one(" + num(off(h)) + "," + num(a) + "," + num(b) + "," + num(m) + ");"; }
void animGlobalSync__FP14AnimationState(void*) { calls += "sync;"; }
unsigned char* findCommander__3Orc(void*) { calls += "findcmd;"; return have_commander ? arena + commander_at : nullptr; }
void deleteCommander__3Orc(void*) { calls += "delcmd;"; }
void leaveCommander__3Orc(void*) { calls += "leavecmd;"; }
void dropItem__3Orci(void*, int k) { calls += "drop" + num(k) + ";"; }
void dropTreasure__8CreatureG6Point3iffffffPc(void*, Point3 p, int a, float f1, float f2, float f3, float f4, float f5, float f6, char* n) {
    calls += "treasure(" + num(p.x) + ":" + num(p.y) + ":" + num(p.z) + "," + num(a) + "," + num(f1) + "," + num(f2) + "," + num(f3) + "," + num(f4) + "," + num(f5) + "," + num(f6) + "," + (n ? "name" : "null") + ");";
}
void dropItem__4itemG6Point3Pc(Point3 p, char* n) { calls += "arrows(" + num(p.x) + "," + std::string(n) + ");"; }
}
static void put32(unsigned at, unsigned v) { std::memcpy(arena + at, &v, 4); }
static void put16(unsigned at, short v) { std::memcpy(arena + at, &v, 2); }
static void putf(unsigned at, float v) { std::memcpy(arena + at, &v, 4); }
static unsigned get32(unsigned at) { unsigned v; std::memcpy(&v, arena + at, 4); return v; }
static short get16(unsigned at) { short v; std::memcpy(&v, arena + at, 2); return v; }
static float getf(unsigned at) { float v; std::memcpy(&v, arena + at, 4); return v; }
static int bad;
static void run(int state, int type, const char* want, short want_state, const char* what) {
    std::memset(arena, 0, sizeof arena);
    calls.clear(); rollIndex = 0;
    w10_actor_k_host_base = arena;
    for (int i = 0; i < 128; ++i) put32(0x2000 + 4 * i, 0x2000 + 4 * i);   // header word = its own offset
    put32(0x1000 + 36, 0x2000 - 0);
    put32(0x1000 + 72, 0x3000); put32(0x3000 + 192, 0); put32(0x3000 + 196, 0x77);
    put32(0x1000 + 1116, type); put32(0x1000 + 808, 5000);
    put32(0x1000 + 576, 0);
    const unsigned saved_state_at = 0x1000 + 572; put16(saved_state_at, 7);
    setState__3Orci(obj, state);
    if (calls != want) { std::printf("%s: calls\n got  %s\n want %s\n", what, calls.c_str(), want); bad = 1; }
    if (get16(0x1000 + 572) != want_state) { std::printf("%s: state %d want %d\n", what, get16(0x1000 + 572), want_state); bad = 1; }
}
int main() {
    rolls[0] = 7; rolls[1] = 4; rolls[2] = 100; rolls[3] = 9; rolls[4] = 5; rolls[5] = 2; rolls[6] = 1; rolls[7] = 3;
    new_state = -1; processing_result = 0;
    // header words: t = 0x2000 so t + k holds the value 0x2000 + k.
    run(0, 4, "proc(0);rand;att(288,8508,0.3,3);", 0, "state 0 type 4");
    run(0, 0, "proc(0);rand;rand;att(288,8460,0.3,3);", 0, "state 0 other type");
    run(18, 0, "proc(18);", 18, "state 18");
    run(7, 0, "proc(7);", 7, "state 7");
    run(13, 0, "proc(13);rand;att(288,8460,0.1,3);rand;", 13, "state 13");
    run(16, 0, "proc(16);att(288,8420,0.1,3);", 16, "state 16");
    run(5, 4, "proc(5);att(288,8500,0.1,3);", 5, "state 5 type 4");
    run(5, 5, "proc(5);att(288,8516,0.1,3);", 5, "state 5 type 5");
    run(2, 3, "proc(2);reset;rand;att(288,8532,0.3,3);sync;", 2, "state 2 type 3 (block A)");
    run(17, 0, "proc(17);setweapon;inittype0;addrun;rand;rand;att(288,8460,0.3,3);", 0, "state 17");
    processing_result = 1; new_state = 9;
    run(5, 0, "proc(5);", 7, "processing result stops the change");
    processing_result = 0; new_state = 7;
    run(10, 0, "proc(10);", 7, "processing may rewrite the state (7 has no case)");
    new_state = -1;
    // field checks for state 17 and state 16
    run(17, 0, "proc(17);setweapon;inittype0;addrun;rand;rand;att(288,8460,0.3,3);", 0, "state 17 fields");
    if (get32(0x1000 + 12) != 0 || get16(0x1000 + 146) != 50 || get16(0x1000 + 150) != 128 || get16(0x1000 + 152) != 255 || get16(0x1000 + 154) != 150 ||
        get16(0x1000 + 142) != 128 || get16(0x1000 + 144) != 255 || get16(0x1000 + 140) != 0 || get16(0x1000 + 148) != 0 || get32(0x1000 + 164) != 1) { std::puts("state 17 fields"); bad = 1; }
    if (get32(0x1000 + 256) != 5000 + 7 % 60 + 120) { std::puts("state 17 nextAt"); bad = 1; }
    // state 10, type 2: no random drop; animation index 3; treasure; word +12 loses bit 0.
    std::memset(arena, 0, sizeof arena); calls.clear(); rollIndex = 0; g_BloodLevel = 0; netNetworkMode = 0;
    for (int i = 0; i < 128; ++i) put32(0x2000 + 4 * i, 0x2000 + 4 * i);
    put32(0x1000 + 36, 0x2000); put32(0x1000 + 72, 0x3000); put32(0x3000 + 196, 0x77); put32(0x1000 + 1116, 2); put32(0x1000 + 12, 0xfffffff1u);
    put16(0x1000 + 56, 99); putf(0x1000 + 16, 1.0f); putf(0x1000 + 20, 2.0f); putf(0x1000 + 24, 3.0f);
    setState__3Orci(obj, 10);
    if (calls != "proc(10);delcmd;drop6;drop7;att(288,8220,0.3,6);treasure(1:2:3,99,36,2,2.8,0.6,2.4,0.25,null);entry192(0);") { std::printf("state 10: %s\n", calls.c_str()); bad = 1; }
    if (get32(0x1000 + 12) != 0xfffffff0u || arena[0x1000 + 40] != 70 || get16(0x1000 + 572) != 10) { std::puts("state 10 fields"); bad = 1; }
    // state 14 with a commander: expected position
    std::memset(arena, 0, sizeof arena); calls.clear(); rollIndex = 0; have_commander = true;
    put32(0x1000 + 36, 0x2000); put32(0x1000 + 1116, 5); put32(0x1000 + 808, 5000); put32(0x1000 + 576, enemy_obj_at);
    put32(enemy_obj_at + 12, enemy_at);
    putf(commander_at + 16, 10.0f); putf(commander_at + 20, 20.0f); putf(commander_at + 24, 30.0f);
    putf(enemy_at + 16, 10.0f); putf(enemy_at + 20, 20.0f + 3.0f); putf(enemy_at + 24, 30.0f + 4.0f);
    for (int i = 0; i < 128; ++i) put32(0x2000 + 4 * i, 0x2000 + 4 * i);
    setState__3Orci(obj, 14);
    if (std::fabs(getf(0x1000 + 272) - 10.0f) > 1e-3 || std::fabs(getf(0x1000 + 276) - (20.0f - 0.6f * 150.0f)) > 1e-2 ||
        std::fabs(getf(0x1000 + 280) - (30.0f - 0.8f * 150.0f)) > 1e-2) { std::printf("state 14 position %g %g %g\n", getf(0x1000 + 272), getf(0x1000 + 276), getf(0x1000 + 280)); bad = 1; }
    if (get32(0x1000 + 256) != 5060 || getf(0x1000 + 176) != 130.0f) { std::puts("state 14 nextAt/float"); bad = 1; }
    // state 14 without a commander
    std::memset(arena, 0, sizeof arena); calls.clear(); rollIndex = 0; have_commander = false;
    put32(0x1000 + 36, 0x2000); put32(0x1000 + 1116, 0); put32(0x1000 + 576, enemy_obj_at); putf(0x1000 + 272, 5.0f); putf(0x1000 + 276, 6.0f); putf(0x1000 + 280, 7.0f);
    setState__3Orci(obj, 14);
    if (getf(0x1000 + 272) != 0.0f || getf(0x1000 + 276) != 0.0f || getf(0x1000 + 280) != 7.0f || getf(0x1000 + 176) != 170.0f) { std::puts("state 14 no commander"); bad = 1; }
    return bad;
}
