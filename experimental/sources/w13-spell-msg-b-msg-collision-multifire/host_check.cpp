// Host contract for msg_collision__19MultiFireProjectileP10GameObject; not an EE input. 32-bit words are offsets from
// a host base and the callees log. The authored function was also compared with the original in a private R5900 run
// (75 scenarios over owner kind, target flags, the 88 entry result and both rounding signs; all reachable words covered).
#define W13_SPELL_MSG_B_HOST 1
#include "../w13-spell-msg-b-common/access.hpp"
#include <cstdio>
#include <cstring>
#include <string>
unsigned char* w13_spell_msg_b_host_base;
static std::string calls;
static int skill_result = 50, w160_result = 1, s88_result = 9, roll = 0;
unsigned w13_spell_msg_b_host_call(unsigned fn, void*, int, unsigned long a1, unsigned long a2) {
    if (fn == 0x700) { calls += "skill(" + std::to_string(a1) + "," + std::to_string(a2) + ");"; return skill_result; }
    if (fn == 0x710) { calls += "s88(" + std::to_string(a1) + "," + std::to_string(a2) + ");"; return s88_result; }
    calls += "w160;";
    return w160_result;
}
extern "C" {
unsigned char player[23984 * 3 + 300];
unsigned eRand__10GameObject(void*) { calls += "rand;"; return roll; }
static int attack_flags, attack_hits;
static float attack_a, attack_b;
static short slot_words[4];
static void* attack_description;
int gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(void*, void*, float x, float y, int, int, int, int s, void* description, bool) {
    calls += "attack;"; ++attack_hits; attack_flags = s; attack_a = x; attack_b = y; attack_description = description;
    if (description) std::memcpy(slot_words, static_cast<unsigned char*>(description) + 36, 8);
    return 0;
}
void terminate__19MultiFireProjectilei(void*, int hit) { calls += "term" + std::to_string(hit) + ";"; }
}
#include "msg_collision.cpp"

static unsigned char arena[0x2000] __attribute__((aligned(16)));
static void put32(unsigned off, unsigned v) { std::memcpy(arena + off, &v, 4); }
static void putf(unsigned off, float v) { std::memcpy(arena + off, &v, 4); }
static float getf(unsigned off) { float v; std::memcpy(&v, arena + off, 4); return v; }
enum { O = 0x100, X = 0x500, Q = 0x900, W = 0xa00, S = 0xe00, TS = 0x1000, TW = 0x1100 };
static void reset(unsigned wflags, unsigned xflags, int n) {
    w13_spell_msg_b_host_base = arena;
    std::memset(arena, 0, sizeof arena); std::memset(player, 0, sizeof player);
    calls.clear(); attack_hits = 0; skill_result = 50; w160_result = 1; s88_result = 9; roll = 0; attack_description = nullptr;
    put32(O + 76, Q); put32(Q + 12, W); put32(O + 112, S); put32(S + 96, TS); put32(TS + 112 + 4, 0x700); put32(TS + 88 + 4, 0x710); put32(O + 104, n);
    put32(W + 12, wflags); put32(W + 960, 1); put32(W + 72, TW);
    put32(X + 12, xflags);
    
    std::memcpy(player + 208 + 23984 + 4, "\x07\0\0\0\x09\0\0\0", 8);
}
int main() {
    unsigned char* const o = arena + O;
    unsigned char* const x = arena + X;
    // player owner: s = 100, v = 4 -> k = 100 * 0.01 / 4 = 0.25; record words 7 and 9 -> round(1.75) = 2, round(2.25) = 2
    reset(0x40, 0x20, 3); s88_result = 4; skill_result = 100;
    msg_collision__19MultiFireProjectileP10GameObject(o, x);
    if (calls != "skill(0,3);s88(2,3);attack;term2;") { std::puts(calls.c_str()); return 1; }
    if (attack_flags != 0x40000 || attack_a != 2.0f || attack_b != 2.0f || attack_description != nullptr) return 2;
    // negative record word: -7 * 0.25 = -1.75 -> -2
    reset(0x40, 0, 3); s88_result = 4; skill_result = 100;
    { int neg = -7; std::memcpy(player + 208 + 23984 + 4, &neg, 4); }
    msg_collision__19MultiFireProjectileP10GameObject(o, x);
    if (attack_a != -2.0f) return 3;
    reset(0x40, 0x40, 3);
    msg_collision__19MultiFireProjectileP10GameObject(o, x);
    if (calls != "") return 4;
    reset(0x40, 0x20, 3); std::memcpy(x + 160, "\0\0\0\x04\0\0\0\0", 8);
    msg_collision__19MultiFireProjectileP10GameObject(o, x);
    if (attack_hits != 0) return 5;
    // non-player owner: s = 50, k = 10.0; first roll 0 -> k * 0 >= 0 -> bonus (int)(10 * 0.5 + 0.5) = 5 for r2 = 32768
    reset(0x20, 0, 3); roll = 32768;
    msg_collision__19MultiFireProjectileP10GameObject(o, x);
    if (calls != "w160;skill(0,3);rand;rand;attack;term2;") { std::puts(calls.c_str()); return 10; }
    if (attack_a != 50.0f || attack_b != 55.0f || attack_flags != 0x40000) return 11;
    reset(0x20, 0, 3); roll = 32768; skill_result = -50;  // k = -10: k * (0.5) < 0 -> (int)(-10 * 0.5 - 0.5) = -5
    msg_collision__19MultiFireProjectileP10GameObject(o, x);
    if (attack_a != -50.0f || attack_b != -55.0f) return 12;
    reset(0x20, 0, 3); w160_result = 0;
    msg_collision__19MultiFireProjectileP10GameObject(o, x);
    if (calls != "w160;") return 13;
    reset(0x20, 0, 3); put32(O + 76, 0);
    msg_collision__19MultiFireProjectileP10GameObject(o, x);
    if (!calls.empty()) return 14;
    return 0;
}
