// Host contract for Innoruuk setState; not an EE input. Words are offsets from a host base; the table's
// function word 1 selects the slot-192 stub. Calls are recorded as short strings.
#define W10_ACTOR_H_HOST 1
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
unsigned char* w10h_host_base;
static void slot(void*);
void* w10h_host_fn(unsigned w) { return w == 1 ? reinterpret_cast<void*>(&slot) : nullptr; }
extern "C" void* gameCam = reinterpret_cast<void*>(0x77);
#include "set_state.cpp"

static unsigned char arena[0x4000] __attribute__((aligned(16)));
static unsigned char* const O = arena + 0x1000;
static std::string calls; static std::vector<unsigned> rolls; static size_t ri; static int procRet, procNew; static bool procSet;
static void* slotSelf; static unsigned char dest[12]; static void* dropped[1];
static std::string num(long v) { return std::to_string(v); }
static std::string hdr(void* h) { return num(reinterpret_cast<unsigned char*>(h) - arena); }
static void slot(void* s) { calls += "V;"; slotSelf = s; }
extern "C" {
int setStateProcessing__8CreaturePi(void*, int* st) { calls += "P;"; if (procSet) *st = procNew; return procRet; }
unsigned eRand__10GameObject(void*) { calls += "r;"; return rolls.at(ri++ % rolls.size()); }
void animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(void* s, void* h, float r, int f) {
    calls += "T" + num(static_cast<unsigned char*>(s) - O) + "," + hdr(h) + "," + (r == 0x1.999998p-4f ? "K" : r == 0x1.333332p-2f ? "N" : "?") + "," + num(f) + ";"; }
void animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(void* s, void* h, float a, float b, int f) {
    calls += "O" + num(static_cast<unsigned char*>(s) - O) + "," + hdr(h) + "," + (a == 0x1.999998p-4f && b == a ? "K" : "?") + "," + num(f) + ";"; }
void animGlobalSync__FP14AnimationState(void* s) { calls += "Y" + num(static_cast<unsigned char*>(s) - O) + ";"; }
void resetRouting__8Creature(void*) { calls += "Z;"; }
void getSmartRandomDestination__8Creaturei(void* r, void*, int range) { calls += "D" + num(range) + ";"; std::memcpy(r, dest, 12); }
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* h, void*, float v) { calls += "S" + hdr(h) + "," + (v == 1.0f ? "1" : "?") + ";"; }
void soundStopSound__Fi(int id) { calls += "X" + num(id) + ";"; }
void* lumpFindResource__FPcT0(char* a, char* b) { calls += std::string("L") + a + "," + b + ";"; return arena + 0x3000; }
void coreStartSwitchScene__Fi(int s) { calls += "C" + num(s) + ";"; }
void Push__6Camera(void* c) { calls += c == gameCam ? "G;" : "g;"; }
void SetMode__6Camerai(void* c, int m) { calls += (c == gameCam ? "M" : "m") + num(m) + ";"; }
void dropTreasure__8CreatureG6Point3iffffffPc(void*, const void* p, int a, float f0, float f1, float f2, float f3, float f4, float f5, char* t) {
    calls += "R" + num(a) + "," + num(static_cast<int>(f0)) + num(static_cast<int>(f1)) + num(static_cast<int>(f2)) + num(static_cast<int>(f3)) + num(static_cast<int>(f4)) + num(static_cast<int>(f5)) + (t ? "t" : "n") +
             (std::memcmp(p, O + 16, 12) == 0 ? "p" : "q") + ";"; }
}
static void wset(unsigned off, unsigned v) { std::memcpy(O + off, &v, 4); }
static unsigned wget(unsigned off) { unsigned v; std::memcpy(&v, O + off, 4); return v; }
static void hset(unsigned off, short v) { std::memcpy(O + off, &v, 2); }
static short hget(unsigned off) { short v; std::memcpy(&v, O + off, 2); return v; }
static void reset() {
    std::memset(arena, 0, sizeof arena); calls.clear(); ri = 0; procRet = 0; procSet = false; slotSelf = nullptr;
    wset(36, 0x2000); wset(72, 0x100); short adj = 24; std::memcpy(arena + 0x100 + 192, &adj, 2); unsigned fw = 1; std::memcpy(arena + 0x100 + 196, &fw, 4);
    for (unsigned i = 0; i < 0x400; i += 4) { unsigned v = 0x2000 + 0x40 + i; std::memcpy(arena + 0x2000 + i, &v, 4); }  // block word at R+i = (0x2040 + i)
    wset(808, 1000); for (int i = 0; i < 12; ++i) dest[i] = static_cast<unsigned char>(0x80 + i);
}
int main() {
    int bad = 0;
    w10h_host_base = arena;
    auto expect = [&](const std::string& want, const char* what) { if (calls != want) { std::printf("fail: %s\n   got  %s\n   want %s\n", what, calls.c_str(), want.c_str()); bad = 1; } };
    // headers are the words at R+off, which these tests set to 0x2040+off
    auto H = [](unsigned off) { return num(0x2040 + off); };
    reset(); procRet = 1; setState__8Innoruuki(O, 0); expect("P;", "a nonzero processing result changes nothing"); if (hget(572) != 0) { std::puts("fail: state kept"); bad = 1; }
    reset(); rolls = {250}; setState__8Innoruuki(O, 0);
    expect("P;r;T288," + H(124) + ",K,3;", "state 0 without the word at +960"); if (wget(256) != 1000u + 250u % 120u || hget(572) != 0) { std::puts("fail: state 0 stores"); bad = 1; }
    reset(); rolls = {250}; wset(960, 1); setState__8Innoruuki(O, 0); expect("P;r;T288," + H(116) + ",K,3;", "state 0 with +960");
    reset(); rolls = {7}; wset(960, 1); setState__8Innoruuki(O, 1);
    expect("P;Z;D200;r;T288," + H(164) + ",N,2051;Y288;", "state 1"); if (std::memcmp(O + 272, dest, 12) != 0 || wget(256) != 1000u + 300u + 7u) { std::puts("fail: state 1 stores"); bad = 1; }
    reset(); setState__8Innoruuki(O, 6); expect("P;T288," + H(116) + ",K,3;O288," + H(84) + ",K,2056;", "state 6");
    reset(); setState__8Innoruuki(O, 17); expect("P;T288," + H(116) + ",K,3;O288," + H(76) + ",K,16392;", "state 17");
    reset(); rolls = {5}; setState__8Innoruuki(O, 14); expect("P;r;T288," + H(52 + 8 * 2) + ",K,3;", "state 14 picks header 52 + 8 * (5 % 3)"); if (wget(256) != 1300u) { std::puts("fail: state 14 store"); bad = 1; }
    reset(); setState__8Innoruuki(O, 20); expect("P;S" + H(220) + ",1;T288," + H(124) + ",K,3;O288," + H(100) + ",K,8;", "state 20");
    reset(); setState__8Innoruuki(O, 2); expect("P;", "an unlisted state only runs the final step"); if (hget(572) != 2) { std::puts("fail: state stored"); bad = 1; }
    reset(); setState__8Innoruuki(O, 99); expect("P;", "state 99"); if (hget(572) != 99) { std::puts("fail: state 99 stored"); bad = 1; }
    reset(); procSet = true; procNew = 6; setState__8Innoruuki(O, 2); expect("P;T288," + H(116) + ",K,3;O288," + H(84) + ",K,2056;", "processing may replace the state");
    // state 8 variants
    reset(); rolls = {31}; wset(960, 1); hset(572, 5); setState__8Innoruuki(O, 8); expect("P;r;", "state 8 draw >= 31 keeps the state"); if (hget(572) != 5 || wget(972) || wget(976) || wget(968)) { std::puts("fail: state 8 keep"); bad = 1; }
    reset(); rolls = {30}; wset(960, 0); hset(572, 5); setState__8Innoruuki(O, 8); expect("P;r;", "state 8 without +960 keeps the state");
    reset(); rolls = {30, 49}; wset(960, 1); hset(572, 19); wset(972, 5); setState__8Innoruuki(O, 8);
    expect("P;r;r;O288," + H(44) + ",K,16;", "state 8 from 19: attack sweep"); if (wget(964) != 1 || wget(972) != 0 || hget(572) != 8) { std::puts("fail: state 8 sweep stores"); bad = 1; }
    reset(); rolls = {30, 50}; wset(960, 1); hset(572, 19); setState__8Innoruuki(O, 8); expect("P;r;r;r;O288," + H(108) + ",K,16;", "state 8 from 19, second draw fails then the third draw also fails");
    reset(); rolls = {30}; wset(960, 1); hset(572, 20); setState__8Innoruuki(O, 8); expect("P;r;", "state 8 from 20 keeps"); if (hget(572) != 20) { std::puts("fail: kept 20"); bad = 1; }
    reset(); rolls = {30, 19}; wset(960, 1); hset(572, 7); setState__8Innoruuki(O, 8);
    expect("P;r;r;O288," + H(44) + ",K,16;", "state 8 other: third draw below 20"); if (wget(964) != 1 || wget(996) != 4600u) { std::puts("fail: state 8 stores"); bad = 1; }
    reset(); rolls = {30, 20}; wset(960, 1); hset(572, 7); setState__8Innoruuki(O, 8); expect("P;r;r;O288," + H(108) + ",K,16;", "state 8 other: default animation");
    // state 10
    reset(); setState__8Innoruuki(O, 10);
    expect("P;Linnoruuk.lmp,hitreact1.anm;S" + num(0x3000) + ",1;C10;G;M2;R0,36110161np;T288," + H(36) + ",N,7;V;", "state 10");
    if (slotSelf != O + 24) { std::puts("fail: slot call target"); bad = 1; }
    { unsigned long long v; std::memcpy(&v, O + 160, 8); if ((v & 0x10000000ull) == 0 || wget(1000) != 1000 || wget(972) || wget(976) || wget(968)) { std::puts("fail: state 10 stores"); bad = 1; } }
    // final step
    reset(); hset(572, 15); wset(1216, 77); setState__8Innoruuki(O, 3); expect("P;T288," + H(156) + ",K,2051;X77;", "leaving state 15 stops its sound"); if (wget(1216) != 0xffffffffu) { std::puts("fail: sound id cleared"); bad = 1; }
    reset(); hset(572, 15); wset(1216, 77); setState__8Innoruuki(O, 15); if (calls.find('X') != std::string::npos || wget(1216) != 77) { std::puts("fail: staying in 15 keeps the sound"); bad = 1; }
    if (!bad) std::puts("innoruuk setState ok");
    return bad;
}
