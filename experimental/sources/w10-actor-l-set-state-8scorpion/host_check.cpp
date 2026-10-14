// Host contract for setState__8Scorpioni; not an EE input. All callees are stubs that log; words
// holding addresses are offsets from a host base. Spot-checks each case for call order, the
// arguments (header words by offset, rates, flags), the stored words and the final halfword.
#define W10_ACTOR_L_HOST 1
#include <cstring>
#include <string>
#include <vector>
unsigned char* w10_actor_l_host_base;
static std::string calls; static unsigned char* g_o; static std::vector<unsigned> rolls; static int proc_result, proc_new = -1000;
unsigned w10_actor_l_host_call(unsigned fn, void* adjusted, int, unsigned long, unsigned long) { calls += "entry" + std::to_string(fn) + "@" + std::to_string(static_cast<unsigned char*>(adjusted) - w10_actor_l_host_base) + ";"; return 0; }
static unsigned off(const void* p) { return static_cast<unsigned>(static_cast<const unsigned char*>(p) - w10_actor_l_host_base); }
extern "C" {
int setStateProcessing__8CreaturePi(void*, int* s) { calls += "proc(" + std::to_string(*s) + ");"; if (proc_new != -1000) *s = proc_new; return proc_result; }
unsigned eRand__10GameObject(void*) { unsigned v = rolls.empty() ? 0 : rolls.front(); if (!rolls.empty()) rolls.erase(rolls.begin()); calls += "rand;"; return v; }
void resetRouting__8Creature(void*) { calls += "reset;"; }
void getSmartRandomDestination__8Creaturei(void* r, void*, int n) { calls += "smart(" + std::to_string(n) + ");"; float v[3] = {1.5f, 2.5f, 3.5f}; std::memcpy(r, v, 12); }
void animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(void* s, void* h, float r, int f) { calls += "tr(" + std::to_string(off(s)) + "," + std::to_string(off(h)) + "," + std::to_string(r) + "," + std::to_string(f) + ");"; }
void animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(void* s, void* h, float a, float b, int f) { calls += "os(" + std::to_string(off(s)) + "," + std::to_string(off(h)) + "," + std::to_string(a) + "," + std::to_string(b) + "," + std::to_string(f) + ");"; }
void loseBodyPart__8Scorpioni(void*, int p) { calls += "lose(" + std::to_string(p) + ");"; }
void dropTreasure__8CreatureG6Point3iffffffPc(void*, const void* p, int a, float, float, float, float, float, float, char*) { float v[3]; std::memcpy(v, p, 12); calls += "drop(" + std::to_string(v[0]) + "," + std::to_string(v[2]) + "," + std::to_string(a) + ");"; }
}
#include "set_state.cpp"
alignas(16) static unsigned char arena[0x4000];
static void put32(unsigned at, unsigned v) { std::memcpy(arena + at, &v, 4); }
static short get16(unsigned at) { short v; std::memcpy(&v, arena + at, 2); return v; }
static unsigned get32(unsigned at) { unsigned v; std::memcpy(&v, arena + at, 4); return v; }
static void reset(std::vector<unsigned> r) {
    std::memset(arena, 0, sizeof arena); calls.clear(); rolls = r; proc_result = 0; proc_new = -1000;
    put32(0x100 + 36, 0x1000); put32(0x100 + 72, 0x2000);
    for (unsigned k = 0; k < 40; ++k) put32(0x1000 + k * 4, 0x3000 + k * 4);  // header word at group+k*4 is its own offset+0x3000
    put32(0x2000 + 192 + 4, 55); put32(0x1000 + 808 - 808, 0x3000);
}
static std::string os(int hdr, int f) { return "os(" + std::to_string(0x100 + 288) + "," + std::to_string(0x3000 + hdr) + ",0.100000,0.100000," + std::to_string(f) + ");"; }
static std::string tr(int hdr, const char* rate, int f) { return "tr(" + std::to_string(0x100 + 288) + "," + std::to_string(0x3000 + hdr) + "," + rate + "," + std::to_string(f) + ");"; }
int main() {
    int bad = 0; unsigned char* const o = arena + 0x100; g_o = o; w10_actor_l_host_base = arena;
    // processing nonzero: nothing else, halfword not stored
    reset({}); proc_result = 1; put32(0x100 + 572 - 0, 0);
    setState__8Scorpioni(o, 5);
    if (calls != "proc(5);" || get16(0x100 + 572) != 0) bad = 1;
    // default / no-op states store the low 16 bits
    for (int s : {2, 4, 7, 11, 12, 13, 16, 100, 0x10005}) { reset({}); setState__8Scorpioni(o, s); if (calls != "proc(" + std::to_string(s) + ");" || get16(0x100 + 572) != static_cast<short>(s)) bad = 1; }
    // processing may rewrite the state
    reset({}); proc_new = 9; setState__8Scorpioni(o, 2);
    if (calls != "proc(2);" + tr(132, "0.300000", 3) || get16(0x100 + 572) != 9) bad = 1;
    // case 0: roll 75 -> 15, word808 = 40 -> 40 + 15 + 120
    reset({75}); put32(0x100 + 808, 40); setState__8Scorpioni(o, 0);
    if (calls != "proc(0);rand;" + tr(132, "0.300000", 3) || get32(0x100 + 256) != 175 || get16(0x100 + 572) != 0) bad = 1;
    // case 1: destination copied to +272
    reset({}); setState__8Scorpioni(o, 1);
    float d[3]; std::memcpy(d, o + 272, 12);
    if (calls != "proc(1);reset;smart(200);" + tr(148, "0.300000", 2051) || d[0] != 1.5f || d[2] != 3.5f) bad = 1;
    // case 3, 5, 9, 14, 15
    reset({}); setState__8Scorpioni(o, 3); if (calls != "proc(3);reset;" + tr(156, "0.300000", 2051)) bad = 1;
    reset({}); setState__8Scorpioni(o, 5); if (calls != "proc(5);" + tr(140, "0.100000", 3)) bad = 1;
    reset({}); setState__8Scorpioni(o, 14); if (calls != "proc(14);" + tr(140, "0.100000", 3) + os(108, 2056)) bad = 1;
    reset({}); setState__8Scorpioni(o, 15); if (calls != "proc(15);" + tr(140, "0.100000", 3) + os(116, 2056)) bad = 1;
    // case 6: roll%6==0 -> header 68, state 13; else pick%3
    reset({6, 20}); setState__8Scorpioni(o, 6);
    if (calls != "proc(6);" + tr(140, "0.100000", 3) + "rand;" + os(68, 8) + "rand;" || get32(0x100 + 256) != 20 % 20u || get16(0x100 + 572) != 13) bad = 1;
    reset({7, 5, 41}); setState__8Scorpioni(o, 6);
    if (get16(0x100 + 572) != 6 || get32(0x100 + 256) != 1) bad = 1;
    // case 8
    reset({}); std::memcpy(arena + 0x100 + 572, "\x09\x00", 2); setState__8Scorpioni(o, 8); if (get16(0x100 + 572) != 9 || calls != "proc(8);") bad = 1;
    reset({}); std::memcpy(arena + 0x100 + 572, "\x06\x00", 2); std::memcpy(arena + 0x100 + 294, "\x08\x00", 2); setState__8Scorpioni(o, 8); if (get16(0x100 + 572) != 6 || calls != "proc(8);") bad = 1;
    reset({}); std::memcpy(arena + 0x100 + 572, "\x06\x00", 2); setState__8Scorpioni(o, 8); if (get16(0x100 + 572) != 8 || calls != "proc(8);" + os(124, 16)) bad = 1;
    // case 10
    reset({1, 1}); std::memcpy(arena + 0x100 + 16, "\x00\x00\xc0\x3f", 4); std::memcpy(arena + 0x100 + 24, "\x00\x00\x20\x40", 4); std::memcpy(arena + 0x100 + 56, "\xfe\xff", 2);
    setState__8Scorpioni(o, 10);
    if (calls != "proc(10);rand;drop(1.500000,2.500000,-2);rand;" + tr(100, "0.300000", 2054) + "entry55@256;" || get16(0x100 + 572) != 10) bad = 1;
    reset({0, 8, 0}); setState__8Scorpioni(o, 10);
    if (calls.find("rand;lose(2);drop") == std::string::npos) bad = 1;
    return bad;
}
