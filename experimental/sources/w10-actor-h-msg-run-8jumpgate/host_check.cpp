// Host contract for JumpGate msg_run; not an EE input. Words are offsets from a host base. Calls are
// recorded as short strings; the random draws come from a scripted list.
#define W10_ACTOR_H_HOST 1
#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
unsigned char* w10h_host_base;
void* w10h_host_fn(unsigned) { return nullptr; }
extern "C" int frameNm = 0;
extern "C" int jumping = 0;
extern "C" float jumpGatePos[3] = {};
extern "C" unsigned char jumpParticle2[16];
extern "C" unsigned char jumpParticleOn[16];
extern "C" int playersAlive[4] = {};
extern "C" int nmPlayersAlive = 0;
extern "C" unsigned char player[4 * 23984];
unsigned char jumpParticle2[16], jumpParticleOn[16], player[4 * 23984];
#include "msg_run.cpp"

static unsigned char arena[0x8000] __attribute__((aligned(16)));
static unsigned char* const O = arena + 0x1000;
static std::string calls; static std::vector<int> rolls; static size_t ri;
static int oapRet; static char* tagRet; static int scriptRet, taskRet; static int localFlags[4];
struct P { void* def; float pos[3]; float vel[3]; float extra; }; static std::vector<P> parts;
static float attach[3]; static std::vector<std::string> events;
static std::vector<float> shockArgs; static int initPlayer; static char initFlag; static char* initTag; static int hudB;
static std::string lumpName; static int animMode;
static std::string num(long v) { return std::to_string(v); }
extern "C" {
int objectActiveProcessing__FP10GameObjectii(void*, int a, int b) { calls += "A" + num(a) + "," + num(b) + ";"; return oapRet; }
char* findTagString__10GameObjectPci(void*, const char* t, int i) { calls += std::string("F") + t + num(i) + ";"; return tagRet; }
int scriptStart__FPc(char*) { calls += "S;"; return scriptRet; }
void lightActivate__FP5Light(void* l) { calls += "la" + num(static_cast<unsigned char*>(l) - O) + ";"; }
void lightDeactivate__FP5Light(void* l) { calls += "ld" + num(static_cast<unsigned char*>(l) - O) + ";"; }
void lightUpdatePosition__FP5Light(void* l) { calls += "lu" + num(static_cast<unsigned char*>(l) - O) + ";"; }
void modelGetAttachmentPos__FP12_modelHeaderP14AnimationStateiP6Point3T3(void* m, void* a, int i, void* out, void* second) {
    calls += "M" + (a ? num(static_cast<unsigned char*>(a) - O) : std::string("n")) + (out == jumpGatePos ? "" : "!") + (second ? "!" : "") + ";"; std::memcpy(out, attach, 12); }
void animAdvanceAnimation__FP14AnimationStateP6Point3P6Point2Ps(void* a, void* p, void*, void*) { calls += "V" + num(static_cast<unsigned char*>(a) - O) + (p == O + 16 ? "" : "!") + ";"; }
void fxRunCreatureShock__FP12_modelHeaderP14AnimationStateG6Point3sP15FxCreatureStatefffUi(void*, void* a, const void* p, short yaw, void* st, float f0, float f1, float f2, unsigned c) {
    calls += "K;"; shockArgs = {static_cast<float>(yaw), f0, f1, f2, static_cast<float>(c == 0x5e808080u), static_cast<float>(static_cast<unsigned char*>(st) - O), static_cast<float>(static_cast<unsigned char*>(a) - O), std::memcmp(p, O + 16, 12) == 0 ? 1.0f : 0.0f}; }
int eRand__10GameObject(void*) { calls += "r"; return rolls.at(ri++ % rolls.size()); }
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* d, void* pos, void* vel, float e) { calls += "P;"; P p; p.def = d; std::memcpy(p.pos, pos, 12); std::memcpy(p.vel, vel, 12); p.extra = e; parts.push_back(p); }
int sprintf(char* d, const char* f, ...) { va_list ap; va_start(ap, f); const char* a = va_arg(ap, const char*); va_end(ap); std::string fs(f); size_t at = fs.find("%s"); std::string out = fs.substr(0, at) + a + fs.substr(at + 2); std::strcpy(d, out.c_str()); int n = static_cast<int>(out.size()); calls += std::string("p") + d + ";"; return n; }
void* lumpFindResource__FPcT0(char* l, char* n) { calls += "L" + num(reinterpret_cast<unsigned char*>(l) - O) + ";"; lumpName = n; return arena + 0x7000; }
void animAdd__FP14AnimationStateP15AnimationHeaderi(void* a, void* h, int m) { calls += "Z" + num(static_cast<unsigned char*>(a) - O) + (h == arena + 0x7000 ? "" : "!") + num(m) + ";"; }
int engineTaskActive__FPc(char*) { calls += "T;"; return taskRet; }
int netPlayerIsLocal__Fi(int p) { calls += "n" + num(p) + ";"; return localFlags[p & 3]; }
void jumpGateInit__FiPcc(int p, char* t, char f) { calls += "I;"; initPlayer = p; initTag = t; initFlag = f; }
unsigned short* textGetString__FPc(char*) { calls += "G;"; return nullptr; }
void hudSetInfoMessage__FPUsiic(unsigned short*, int a, int b, char) { calls += "H" + num(a) + ";"; hudB = b; }
}
static void putf(unsigned char* w, float v) { std::memcpy(w, &v, 4); }
static float getf(unsigned char* w) { float v; std::memcpy(&v, w, 4); return v; }
static unsigned char* objectOf(int slot) { return arena + 0x4000 + 0x100 * slot; }
static void reset() {
    std::memset(arena, 0, sizeof arena); calls.clear(); parts.clear(); ri = 0; oapRet = 1; tagRet = nullptr; scriptRet = 1; taskRet = 0;
    for (int i = 0; i < 4; ++i) localFlags[i] = 0;
    frameNm = 1; jumping = 0; nmPlayersAlive = 0; for (int i = 0; i < 4; ++i) { playersAlive[i] = i; unsigned w = 0x4000 + 0x100 * i; std::memcpy(player + i * 23984 + 21872, &w, 4); }
    jumpGatePos[0] = 1; jumpGatePos[1] = 2; jumpGatePos[2] = 3; attach[0] = 10; attach[1] = 20; attach[2] = 30;
    putf(O + 16, 100); putf(O + 20, 200); putf(O + 24, 300);
    unsigned m = 0x2000; std::memcpy(O + 348, &m, 4);
    O[76] = 1;
}
int main() {
    int bad = 0;
    w10h_host_base = arena;
    auto expect = [&](const std::string& want, const char* what) { if (calls != want) { std::printf("fail: %s\n   got  %s\n   want %s\n", what, calls.c_str(), want.c_str()); bad = 1; } };
    // an object whose byte 76 comes back zero ends at once with the flag cleared
    reset(); oapRet = 0; O[208] = 1; msg_run__8JumpGate(O); expect("A1,480;", "inactive object"); if (O[208] != 0 || O[76] != 0) { std::puts("fail: inactive stores"); bad = 1; }
    // plain case: nothing set; both flags zero
    reset(); O[79] = 0; O[77] = 0; O[208] = 1; msg_run__8JumpGate(O); expect("A1,480;M" "n;", "second branch: attachment only"); if (O[208] != 0) { std::puts("fail: flag cleared"); bad = 1; }
    if (jumpGatePos[0] != 110.0f || jumpGatePos[1] != 220.0f || jumpGatePos[2] != 330.0f) { std::puts("fail: gate = attachment + pos"); bad = 1; }
    // activate animation (mode 0), jumping set
    reset(); O[79] = 0; O[77] = 0; O[78] = 0; O[220] = 0; jumping = 1; std::memcpy(O + 80, "gate", 5);
    rolls = {40000, 30000, 36000, 20000, 50000, 33000};
    msg_run__8JumpGate(O);
    if (O[78] != 1 || lumpName != "gate_activate.anm") { std::puts("fail: activate state or name"); bad = 1; }
    expect("A1,480;Mn;la256;pgate_activate.anm;L144;Z2164;", "mode 0 activation (byte 220 is zero, so the rest is skipped)");
    // mode 1 with bit 7 set
    reset(); O[79] = 0; O[77] = 1; O[78] = 1; O[220] = 1; O[222] = 0x80; std::memcpy(O + 80, "gate", 5);
    msg_run__8JumpGate(O);
    if (O[78] != 2 || O[220] != 0 || lumpName != "gate_idle.anm") { std::puts("fail: idle state"); bad = 1; }
    // particles and light update: byte 220 set, jumping set
    reset(); O[79] = 0; O[77] = 0; O[78] = 2; O[220] = 1; jumping = 1;
    rolls = {60000, 60000, 60000, 40000, 30000, 36000, 20000, 50000, 33000};   // one rejected draw, then two accepted draws
    msg_run__8JumpGate(O);
    expect("A1,480;Mn;rrrrrrrrrP;lu256;", "particle with two unit draws, then the light update");
    if (parts.size() == 1) {
        auto comp = [](int r) { float v = static_cast<float>(r) * 0x1.0p-16f; v = v + v; return v - 1.0f; };
        if (parts[0].def != jumpParticle2 || parts[0].extra != 0.0f) { std::puts("fail: particle def"); bad = 1; }
        if (parts[0].vel[0] != comp(40000) * 0x1.3634dep+3f || parts[0].vel[2] != comp(36000) * 0x1.3634dep+3f) { std::puts("fail: velocity"); bad = 1; }
        if (parts[0].pos[0] != 110.0f + comp(20000) * 16.0f || parts[0].pos[2] != 330.0f + comp(33000) * 16.0f) { std::puts("fail: particle position"); bad = 1; }
    } else { std::puts("fail: one particle"); bad = 1; }
    // light position: gate (110, 220, 330) + (0, 0, 36)
    if (getf(O + 304) != 110.0f || getf(O + 308) != 220.0f || getf(O + 312) != 366.0f) { std::puts("fail: light position"); bad = 1; }
    // first branch with ok: shock, size decay and clamps
    reset(); O[79] = 1; O[208] = 1; jumping = 0; putf(O + 284, 100.0f); rolls = {19};
    msg_run__8JumpGate(O);
    expect("A1,480;Fscript0;Mn;V216;M216;K;rFdestination0;I;", "first branch: attachments, shock, one chance draw that misses; the second branch then initialises");
    if (std::fabs(getf(O + 212) - (100.0f - 0x1.555554p-3f)) > 1e-4f || getf(O + 284) != getf(O + 212) || getf(O + 292) != getf(O + 212)) { std::puts("fail: size"); bad = 1; }
    if (getf(O + 296) != 1.0f / (getf(O + 212) * getf(O + 212)) || getf(O + 288) != getf(O + 296)) { std::puts("fail: inverse"); bad = 1; }
    reset(); O[79] = 1; O[208] = 1; jumping = 1; putf(O + 284, 30.0f); rolls = {40, 65536};
    msg_run__8JumpGate(O); if (getf(O + 212) != 60.0f + 65536.0f * 0x1.0p-16f * 120.0f) { std::printf("fail: floor then growth %f\n", getf(O + 212)); bad = 1; }
    reset(); O[79] = 1; O[208] = 1; jumping = 1; putf(O + 284, 500.0f); rolls = {1};
    msg_run__8JumpGate(O); if (getf(O + 212) != 300.0f || getf(O + 284) != 300.0f) { std::puts("fail: upper clamp"); bad = 1; }
    // players: all near gives the init call with the slot after the last
    reset(); O[79] = 1; O[77] = 1; O[208] = 1; nmPlayersAlive = 2; tagRet = reinterpret_cast<char*>(O); playersAlive[2] = 3;
    putf(objectOf(0) + 16, 100); putf(objectOf(1) + 16, 101);
    for (int s = 0; s < 2; ++s) { putf(objectOf(s) + 20, 200); putf(objectOf(s) + 24, 300); }
    rolls = {40000, 30000, 36000};
    msg_run__8JumpGate(O);
    if (calls.find("Fdestination0;I;") == std::string::npos || initPlayer != 3 || initFlag != 0) { std::printf("fail: init all near: %s\n", calls.c_str()); bad = 1; }
    reset(); O[79] = 1; O[77] = 1; O[208] = 1; nmPlayersAlive = 2; rolls = {40000, 30000, 36000}; putf(objectOf(1) + 16, 400);
    msg_run__8JumpGate(O); if (calls.find("I;") != std::string::npos) { std::puts("fail: a far player blocks init"); bad = 1; }
    // second branch (byte 77 zero): initialisation sets byte 77, a local player in range raises the warning
    reset(); O[79] = 1; O[77] = 0; O[208] = 1; nmPlayersAlive = 1; putf(objectOf(0) + 16, 100); putf(objectOf(0) + 20, 200); putf(objectOf(0) + 24, 300);
    msg_run__8JumpGate(O); if (O[77] != 1 || O[208] != 0 || calls.find("I;") == std::string::npos) { std::puts("fail: init sets byte 77"); bad = 1; }
    reset(); O[79] = 1; O[77] = 0; O[208] = 1; nmPlayersAlive = 2; scriptRet = 0; putf(O + 16, 0); putf(O + 20, 0); putf(O + 24, 0);
    localFlags[0] = 1; localFlags[1] = 1; putf(objectOf(0) + 16, 100); putf(objectOf(1) + 16, 300);
    msg_run__8JumpGate(O); if (calls.find("H180;") == std::string::npos || hudB != 0) { std::printf("fail: warning for the first local player in range: %s\n", calls.c_str()); bad = 1; }
    if (!bad) std::puts("jumpgate msg_run ok");
    return bad;
}
