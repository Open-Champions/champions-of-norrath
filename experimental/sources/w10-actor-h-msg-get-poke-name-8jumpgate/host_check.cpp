// Host contract for JumpGate msg_getPokeName; not an EE input. Words are offsets from a host base.
#define W10_ACTOR_H_HOST 1
#include <cstdio>
#include <cstring>
#include <string>
unsigned char* w10h_host_base;
void* w10h_host_fn(unsigned) { return nullptr; }
extern "C" signed char jumpGateDestCount = 0;
extern "C" int playersAlive[4] = {};
extern "C" int nmPlayersAlive = 0;
extern "C" unsigned char player[4 * 23984];
unsigned char player[4 * 23984];
#include "msg_get_poke_name.cpp"

static unsigned char arena[0x4000] __attribute__((aligned(16)));
static std::string calls; static int taskResult, scriptResult, tagCalls, scriptCalls, addCalls; static char* tagResult = nullptr;
static unsigned short textStore[8]; static int hudA, hudB; static char hudC; static unsigned short* hudText; static unsigned short* copied; static unsigned short* copyTo;
extern "C" {
int engineTaskActive__FPc(char* n) { calls += std::string("T") + n + ";"; return taskResult; }
char* findTagString__10GameObjectPci(void*, const char* t, int i) { ++tagCalls; calls += std::string("F") + t + std::to_string(i) + ";"; return tagResult; }
int scriptStart__FPc(char*) { ++scriptCalls; calls += "S;"; return scriptResult; }
void jumpGateAddDestination__Fv() { ++addCalls; calls += "A;"; }
unsigned short* textGetString__FPc(char* k) { calls += std::string("G") + k + ";"; return textStore; }
void hudSetInfoMessage__FPUsiic(unsigned short* t, int a, int b, char c) { calls += "H;"; hudText = t; hudA = a; hudB = b; hudC = c; }
void ustrcpy__FPUsT0(unsigned short* to, unsigned short* from) { calls += "U;"; copyTo = to; copied = from; }
}
static void putf(unsigned char* w, float v) { std::memcpy(w, &v, 4); }
int main() {
    int bad = 0;
    w10h_host_base = arena;
    auto expect = [&](bool ok, const char* what) { if (!ok) { std::printf("fail: %s (calls %s)\n", what, calls.c_str()); bad = 1; } };
    unsigned short out[16];
    auto setup = [&]() {
        std::memset(arena, 0, sizeof arena); std::memset(player, 0, sizeof player); calls.clear(); taskResult = 0; scriptResult = 1; tagResult = nullptr; tagCalls = scriptCalls = addCalls = 0;
        jumpGateDestCount = 2; nmPlayersAlive = 2; playersAlive[0] = 0; playersAlive[1] = 1; playersAlive[2] = 2; playersAlive[3] = 3;
        for (int p = 0; p < 4; ++p) { unsigned w = 0x2000 + 0x100 * p; std::memcpy(player + p * 23984 + 21872, &w, 4); }
    };
    unsigned char* o = arena + 0x1000;
    setup(); o[79] = 1; expect(msg_getPokeName__8JumpGatePUsi(o, out, 0) == 0 && calls.empty(), "byte 79 returns 0 at once");
    setup(); taskResult = 1; expect(msg_getPokeName__8JumpGatePUsi(o, out, 0) == 0 && calls == "Tjumpgate;", "task active returns 0");
    setup(); tagResult = reinterpret_cast<char*>(o); scriptResult = 0;
    expect(msg_getPokeName__8JumpGatePUsi(o, out, 0) == 0 && tagCalls == 2 && scriptCalls == 1 && addCalls == 0, "script start failing returns 0");
    setup(); tagResult = reinterpret_cast<char*>(o); scriptResult = 1; o[77] = 0xff; jumpGateDestCount = 1;
    expect(msg_getPokeName__8JumpGatePUsi(o, out, 2) == 0 && addCalls == 0 && copyTo == out && calls.find("GJumpGateVirgin") != std::string::npos, "script ok, byte 77 set: one destination gives the virgin text and 0");
    setup(); o[77] = 0; playersAlive[1] = 3; jumpGateDestCount = 1;
    expect(msg_getPokeName__8JumpGatePUsi(o, out, 1) == 0 && o[77] == 1 && addCalls == 1 && hudA == 180 && hudB == 3 && hudC == 0 && hudText == textStore, "activation message uses playersAlive[index]");
    expect(calls == "Tjumpgate;Fscript0;A;GJumpGateActivate;H;GJumpGateVirgin;U;", "call order of the first-activation path");
    // far and near players, positions: o at (0,0,0), others at x = 96 (sum 9216: not greater) and 97 (greater)
    setup(); o[77] = 1; for (int i = 0; i < 3; ++i) putf(o + 16 + 4 * i, 0.0f);
    putf(arena + 0x2000 + 16, 96.0f); putf(arena + 0x2100 + 16, 96.0f);
    expect(msg_getPokeName__8JumpGatePUsi(o, out, 7) == 1 && calls.find("GJumpGate;") != std::string::npos, "exactly 9216.0 is not far: plain name, 1");
    setup(); o[77] = 1; putf(arena + 0x2000 + 16, 0.0f); putf(arena + 0x2100 + 16, 97.0f);
    expect(msg_getPokeName__8JumpGatePUsi(o, out, 7) == 0 && calls.find("GJumpGateNeedPlayers") != std::string::npos, "a far second player: need-players text and 0");
    setup(); o[77] = 1; putf(arena + 0x2000 + 16, 0.0f); putf(arena + 0x2100 + 16, 500.0f);
    expect(msg_getPokeName__8JumpGatePUsi(o, out, 1) == 1, "the far player is the index slot and is skipped");
    setup(); o[77] = 1; nmPlayersAlive = 0; expect(msg_getPokeName__8JumpGatePUsi(o, out, 0) == 1, "no players: plain name");
    if (!bad) std::puts("jumpgate msg_getPokeName ok");
    return bad;
}
