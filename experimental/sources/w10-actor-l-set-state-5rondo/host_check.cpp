// Host contract for setState__5Rondoi; not an EE input. All callees are logging stubs; words
// holding addresses are offsets from a host base (a group header word is 0x3000 plus its offset).
#define W10_ACTOR_L_HOST 1
#include <cstring>
#include <string>
#include <vector>
unsigned char* w10_actor_l_host_base;
static std::string calls; static std::vector<unsigned> rolls; static int proc_result, proc_new = -1000; static void* enemy_to_return;
unsigned w10_actor_l_host_call(unsigned fn, void* adjusted, int, unsigned long, unsigned long) { calls += "entry" + std::to_string(fn) + "@" + std::to_string(static_cast<unsigned char*>(adjusted) - w10_actor_l_host_base) + ";"; return 0; }
static unsigned off(const void* p) { return static_cast<unsigned>(static_cast<const unsigned char*>(p) - w10_actor_l_host_base); }
extern "C" {
int setStateProcessing__8CreaturePi(void*, int* s) { calls += "proc(" + std::to_string(*s) + ");"; if (proc_new != -1000) *s = proc_new; return proc_result; }
unsigned eRand__10GameObject(void*) { unsigned v = rolls.empty() ? 0 : rolls.front(); if (!rolls.empty()) rolls.erase(rolls.begin()); calls += "rand;"; return v; }
void resetRouting__8Creature(void*) { calls += "reset;"; }
void getSmartRandomDestination__8Creaturei(void* r, void*, int n) { calls += "smart(" + std::to_string(n) + ");"; float v[3] = {1.5f, 2.5f, 3.5f}; std::memcpy(r, v, 12); }
void animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(void* s, void* h, float r, int f) { calls += "tr(" + std::to_string(off(s)) + "," + std::to_string(off(h)) + "," + std::to_string(r) + "," + std::to_string(f) + ");"; }
void animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(void* s, void* h, float a, float b, int f) { calls += "os(" + std::to_string(off(s)) + "," + std::to_string(off(h)) + "," + std::to_string(a) + "," + std::to_string(b) + "," + std::to_string(f) + ");"; }
void animGlobalSync__FP14AnimationState(void* s) { calls += "sync(" + std::to_string(off(s)) + ");"; }
void* alternateEnemyInZone__8Creatureffss(void*, float a, float b, short c, short d) { calls += "enemy(" + std::to_string(a) + "," + std::to_string(b) + "," + std::to_string(c) + "," + std::to_string(d) + ");"; return enemy_to_return; }
void listDestroy__FP8ListHead(void* l) { calls += "ldestroy(" + std::to_string(off(l)) + ");"; }
void listOnewayLink__FP8ListHeadT0Pv(void* l, void* h, void* i) { calls += "link(" + std::to_string(off(l)) + "," + std::to_string(off(h)) + "," + std::to_string(off(i)) + ");"; }
}
#include "set_state.cpp"
alignas(16) static unsigned char arena[0x4000];
static void put32(unsigned at, unsigned v) { std::memcpy(arena + at, &v, 4); }
static short get16(unsigned at) { short v; std::memcpy(&v, arena + at, 2); return v; }
static unsigned get32(unsigned at) { unsigned v; std::memcpy(&v, arena + at, 4); return v; }
static void reset(std::vector<unsigned> r) {
    std::memset(arena, 0, sizeof arena); calls.clear(); rolls = r; proc_result = 0; proc_new = -1000; enemy_to_return = nullptr;
    put32(0x100 + 36, 0x1000); put32(0x100 + 72, 0x2000);
    for (unsigned k = 0; k < 40; ++k) put32(0x1000 + k * 4, 0x3000 + k * 4);
    put32(0x2000 + 192 + 4, 55);
}
static std::string os(int hdr, int f) { return "os(" + std::to_string(0x100 + 288) + "," + std::to_string(0x3000 + hdr) + ",0.100000,0.100000," + std::to_string(f) + ");"; }
static std::string tr(int hdr, int f) { return "tr(" + std::to_string(0x100 + 288) + "," + std::to_string(0x3000 + hdr) + ",0.100000," + std::to_string(f) + ");"; }
int main() {
    int bad = 0; unsigned char* const o = arena + 0x100; w10_actor_l_host_base = arena;
    reset({}); proc_result = 1; setState__5Rondoi(o, 5); if (calls != "proc(5);" || get16(0x100 + 572) != 0) bad = 1;
    for (int s : {2, 3, 4, 11, 12, 100, 0x10005}) { reset({}); setState__5Rondoi(o, s); if (calls != "proc(" + std::to_string(s) + ");" || get16(0x100 + 572) != static_cast<short>(s)) bad = 1; }
    reset({}); proc_new = 6; setState__5Rondoi(o, 2); if (calls != "proc(2);" + os(28, 8) || get16(0x100 + 572) != 6) bad = 1;
    // case 0, both branches
    reset({250}); put32(0x100 + 808, 40); put32(0x100 + 964, 0); setState__5Rondoi(o, 0);
    if (calls != "proc(0);rand;" + tr(44, 3) || get32(0x100 + 256) != 40 + 10 + 120) bad = 1;
    reset({119}); put32(0x100 + 964, 9); setState__5Rondoi(o, 0);
    if (calls != "proc(0);rand;" + tr(52, 3) || get32(0x100 + 256) != 119 + 120) bad = 1;
    // case 1, both branches
    for (int hit : {0, 1}) {
        reset({121}); put32(0x100 + 808, 5); if (hit) put32(0x100 + 160, 0x00800000);
        setState__5Rondoi(o, 1);
        float d[3]; std::memcpy(d, o + 272, 12);
        if (calls != "proc(1);reset;smart(200);rand;" + tr(hit ? 76 : 108, 2051) + "sync(544);" || get32(0x100 + 256) != 5 + 1 + 300 || d[0] != 1.5f || d[2] != 3.5f) bad = 1;
    }
    reset({}); setState__5Rondoi(o, 5); if (calls != "proc(5);" + tr(52, 3)) bad = 1;
    reset({}); setState__5Rondoi(o, 9); if (calls != "proc(9);" + tr(52, 3)) bad = 1;
    reset({}); setState__5Rondoi(o, 6); if (calls != "proc(6);" + os(28, 8)) bad = 1;
    // case 7
    reset({}); std::memcpy(arena + 0x100 + 56, "\xfe\xff", 2); std::memcpy(arena + 0x100 + 572, "\x07\x00", 2);
    setState__5Rondoi(o, 7); if (calls != "proc(7);enemy(0.000000,100.000000,-2,32767);" || get16(0x100 + 572) != 7) bad = 1;
    reset({}); enemy_to_return = arena + 0x2800; put32(0x100 + 808, 70); setState__5Rondoi(o, 7);
    if (calls != "proc(7);enemy(0.000000,100.000000,0,32767);ldestroy(" + std::to_string(0x100 + 960) + ");link(" + std::to_string(0x100 + 960) + ",10240,10240);" + tr(76, 2051) || get32(0x100 + 256) != 190 || get16(0x100 + 572) != 7) bad = 1;
    // case 8, 10
    reset({}); std::memcpy(arena + 0x100 + 572, "\x09\x00", 2); setState__5Rondoi(o, 8); if (calls != "proc(8);" || get16(0x100 + 572) != 9) bad = 1;
    reset({}); std::memcpy(arena + 0x100 + 572, "\x03\x00", 2); setState__5Rondoi(o, 8); if (calls != "proc(8);" + os(36, 16) || get16(0x100 + 572) != 8) bad = 1;
    reset({}); setState__5Rondoi(o, 10); if (calls != "proc(10);entry55@256;" + tr(44, 6) || get16(0x100 + 572) != 10) bad = 1;
    return bad;
}
