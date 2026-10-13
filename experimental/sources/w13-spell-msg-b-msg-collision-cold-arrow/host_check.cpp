// Host contract for msg_collision__19ColdArrowProjectileP10GameObject; not an EE input. 32-bit words are offsets from
// a host base and the callees log. The authored function was also compared with the original in a private R5900 run
// (75 scenarios over owner kind, target flags, n thresholds and the 88 entry result; all reachable words covered).
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
void addEffect__FiG6Point3(int effect, Point3 position) { calls += "effect" + std::to_string(effect) + "@" + std::to_string(static_cast<int>(position.x)) + ";"; }
int gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(void*, void*, float x, float y, int, int, int, int s, void* description, bool) {
    calls += "attack;"; ++attack_hits; attack_flags = s; attack_a = x; attack_b = y; attack_description = description;
    if (description) std::memcpy(slot_words, static_cast<unsigned char*>(description) + 36, 8);
    return 0;
}
void terminate__19ColdArrowProjectilei(void*, int hit) { calls += "term" + std::to_string(hit) + ";"; }
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
    put32(O + 76, Q); put32(Q + 12, W); put32(O + 172, S); put32(S + 96, TS); put32(TS + 112 + 4, 0x700); put32(TS + 88 + 4, 0x710); put32(O + 104, n);
    put32(W + 12, wflags); put32(W + 960, 1); put32(W + 72, TW);
    put32(X + 12, xflags);
    putf(O + 16, 1); putf(X + 860, 100.0f);
    std::memcpy(player + 208 + 23984 + 4, "\x07\0\0\0\x09\0\0\0", 8);
}
int main() {
    unsigned char* const o = arena + O;
    unsigned char* const x = arena + X;
    reset(0x40, 0x20, 3);
    msg_collision__19ColdArrowProjectileP10GameObject(o, x);
    if (calls != "effect10@1;skill(2,3);skill(2,3);attack;s88(3,3);term2;") { std::puts(calls.c_str()); return 1; }
    if (attack_flags != 0x40 || attack_a != 7.0f || attack_b != 9.0f || slot_words[0] != 32 || slot_words[1] != 100 || slot_words[2] != 50 || slot_words[3] != 50) return 2;
    if (getf(X + 860) != 104.0f) return 3;  // 9 / 2 = 4
    const int ns[] = {10, 11, 15, 16};
    const int expect[] = {0x40, 0x40040, 0x40040, 0x80040};
    for (int i = 0; i < 4; ++i) {
        reset(0x40, 0, ns[i]);
        msg_collision__19ColdArrowProjectileP10GameObject(o, x);
        if (attack_flags != expect[i] || calls.find("s88") != std::string::npos) return 10 + i;
    }
    reset(0x40, 0x40, 3);
    msg_collision__19ColdArrowProjectileP10GameObject(o, x);
    if (!calls.empty()) return 20;
    reset(0x40, 0x20, 3); std::memcpy(x + 160, "\0\0\0\x04\0\0\0\0", 8);
    msg_collision__19ColdArrowProjectileP10GameObject(o, x);
    if (!calls.empty()) return 21;
    reset(0x20, 0x20, 3); s88_result = -7;
    msg_collision__19ColdArrowProjectileP10GameObject(o, x);
    if (calls != "w160;effect10@1;skill(0,3);rand;attack;s88(3,3);term2;") { std::puts(calls.c_str()); return 30; }
    if (attack_a != 50.0f || attack_b != 50.0f || attack_flags != 64 || attack_description != nullptr || getf(X + 860) != 97.0f) return 31;  // -7 / 2 = -3
    reset(0x20, 0, 3); roll = 65536;  // r * 2^-16 = 1.0: upper = 50 + (int)(50 * 0.2f) = 59 as 0.2f is just below 0.2
    msg_collision__19ColdArrowProjectileP10GameObject(o, x);
    if (attack_b != 59.0f || getf(X + 860) != 100.0f || calls.find("s88") != std::string::npos) return 32;
    reset(0x20, 0, 3); w160_result = 0;
    msg_collision__19ColdArrowProjectileP10GameObject(o, x);
    if (calls != "w160;") return 33;
    reset(0x20, 0, 3); put32(O + 76, 0);
    msg_collision__19ColdArrowProjectileP10GameObject(o, x);
    if (!calls.empty()) return 34;
    return 0;
}
