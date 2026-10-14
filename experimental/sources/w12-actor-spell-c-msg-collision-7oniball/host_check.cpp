// Host contract for msg_collision__7OniBallP10GameObject; not an EE input. 32-bit words are offsets from a
// host base and the callees log. The authored function was also compared with the original in a private R5900
// run (116 scenarios: flag, rank, button, kind and clamp cases; all 91 reachable original words covered).
#define W12_SPELL_C_HOST 1
#include <cstdio>
#include <cstring>
#include <string>
unsigned char* w12_spell_c_host_base;
unsigned w12_spell_c_host_call(unsigned, void*, int, unsigned long, unsigned long) { return 0; }
extern "C" {
int frameNm = 77;
unsigned skills[64];
unsigned blockButton[4];
unsigned char pad[88 * 4 + 16];
unsigned char player[23984 * 4];
}
#include "msg_collision.cpp"

static std::string calls;
static int ranks[2], rankIndex;
static int amount;
extern "C" {
int playerGetSkillRank__Fii(int p, int s) { calls += "rank(" + std::to_string(p) + "," + std::to_string(s) + ");"; return ranks[rankIndex++ & 1]; }
int getAmount__12SkillLifeTapi(void* self, int rank) {
    calls += "amount(" + std::to_string(static_cast<unsigned char*>(self) - w12_spell_c_host_base) + "," + std::to_string(rank) + ");";
    return amount;
}
}

static unsigned char arena[0x2000] __attribute__((aligned(16)));
static unsigned char* const ball = arena + 0x100;
static unsigned char* const other = arena + 0x600;
static void put32(unsigned char* p, unsigned v) { std::memcpy(p, &v, 4); }
static void putf(unsigned char* p, float v) { std::memcpy(p, &v, 4); }
static float getf(unsigned char* p) { float v; std::memcpy(&v, p, 4); return v; }
static unsigned get32(unsigned char* p) { unsigned v; std::memcpy(&v, p, 4); return v; }
static void reset(unsigned flags, int kind) {
    w12_spell_c_host_base = arena;
    std::memset(arena, 0, sizeof arena);
    std::memset(player, 0, sizeof player);
    calls.clear(); rankIndex = 0; ranks[0] = ranks[1] = 3; amount = 40;
    put32(other + 12, flags); put32(other + 960, 1);
    ball[116] = static_cast<unsigned char>(kind);
    blockButton[1] = 4; put32(pad + 88 + 8, 6);
    skills[59] = static_cast<unsigned>(arena + 0x1000 - arena);
    unsigned char* rec = player + 23984;
    putf(rec + 172, 10); putf(rec + 176, 100); putf(rec + 180, 20);
    putf(rec + 196, 5); putf(rec + 200, 50); putf(rec + 204, 30);
}

int main() {
    int bad = 0;
    unsigned char* rec = player + 23984;
    reset(0x40, 0);
    msg_collision__7OniBallP10GameObject(ball, other);
    if (calls != "rank(1,59);rank(1,59);amount(4096,3);") { std::puts(calls.c_str()); std::puts("fail: calls"); bad = 1; }
    if (getf(rec + 180) != 60 || get32(other + 928) != 30 || get32(other + 936) != 0 || get32(other + 252) != 77 || get32(ball + 84) != 77) { std::puts("fail: kind 0"); bad = 1; }
    reset(0x40, 1);
    msg_collision__7OniBallP10GameObject(ball, other);
    if (getf(rec + 204) != 45 /* 30 + 40 clamps to 50 - 5 */ || getf(rec + 180) != 20 || get32(other + 936) != 30 || get32(other + 928) != 0) { std::puts("fail: kind 1"); bad = 1; }
    reset(0x40, 5);
    msg_collision__7OniBallP10GameObject(ball, other);
    if (getf(rec + 180) != 20 || getf(rec + 204) != 30 || get32(ball + 84) != 77 || get32(other + 252) != 77) { std::puts("fail: other kind"); bad = 1; }
    reset(0x20040, 0);
    msg_collision__7OniBallP10GameObject(ball, other);
    if (!calls.empty() || get32(ball + 84) != 0) { std::puts("fail: excluded flag"); bad = 1; }
    reset(0x00, 0);
    msg_collision__7OniBallP10GameObject(ball, other);
    if (!calls.empty()) { std::puts("fail: missing flag"); bad = 1; }
    reset(0x40, 0); ranks[0] = 0;
    msg_collision__7OniBallP10GameObject(ball, other);
    if (calls != "rank(1,59);" || get32(other + 252) != 0) { std::puts("fail: no rank"); bad = 1; }
    reset(0x40, 0); blockButton[1] = 1;
    msg_collision__7OniBallP10GameObject(ball, other);
    if (calls != "rank(1,59);" || get32(other + 252) != 0) { std::puts("fail: button not held"); bad = 1; }
    return bad;
}
