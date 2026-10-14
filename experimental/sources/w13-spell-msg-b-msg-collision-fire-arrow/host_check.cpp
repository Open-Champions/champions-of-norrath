// Host contract for msg_collision__19FireArrowProjectileP10GameObject; not an EE input. 32-bit words are offsets from a
// host base and the callees log. The authored function was also compared with the original in a private R5900 run
// (75 scenarios over owner kind, target flags, n thresholds, rolls and allocation failure; all reachable words covered).
#define W13_SPELL_MSG_B_HOST 1
#include "../w13-spell-msg-b-common/access.hpp"
#include <cstdio>
#include <cstring>
#include <string>
unsigned char* w13_spell_msg_b_host_base;
static std::string calls;
static int skill_result = 50, w160_result = 1, roll = 0;
unsigned w13_spell_msg_b_host_call(unsigned fn, void*, int, unsigned long a1, unsigned long a2) {
    if (fn == 0x700) { calls += "skill(" + std::to_string(a1) + "," + std::to_string(a2) + ");"; return skill_result; }
    calls += "w160;";
    return w160_result;
}
extern "C" {
unsigned char player[23984 * 3 + 300];
unsigned eRand__10GameObject(void*) { calls += "rand;"; return roll; }
static unsigned char heap[1192];
static bool alloc_ok = true;
void* blockAlloc__Fi(int size) { calls += "alloc" + std::to_string(size) + ";"; return alloc_ok ? heap : nullptr; }
static int attack_flags, attack_hits;
static float attack_a, attack_b;
static short slot_words[4];
static void* attack_target;
int gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(void* a, void* b, float x, float y, int p, int q, int r, int s, void* description, bool) {
    calls += "attack;"; ++attack_hits; attack_flags = s; attack_a = x; attack_b = y; attack_target = a; (void)b; (void)p; (void)q; (void)r;
    std::memcpy(slot_words, static_cast<unsigned char*>(description) + 36, 8);
    return 0;
}
static Point3 sf_where; static float sf_a, sf_c; static int sf_b;
void StaticFire_construct(void*, Point3 where, float a, int b, float c) __asm__("__10StaticFireG6Point3fif");
void StaticFire_construct(void*, Point3 where, float a, int b, float c) { calls += "sf;"; sf_where = where; sf_a = a; sf_b = b; sf_c = c; }
void terminate__19FireArrowProjectilei(void*, int hit) { calls += "term" + std::to_string(hit) + ";"; }
}
#include "msg_collision.cpp"

static unsigned char arena[0x2000] __attribute__((aligned(16)));
static void put32(unsigned off, unsigned v) { std::memcpy(arena + off, &v, 4); }
static void putf(unsigned off, float v) { std::memcpy(arena + off, &v, 4); }
enum { O = 0x100, X = 0x500, Q = 0x900, W = 0xa00, S = 0xe00, TS = 0x1000, TW = 0x1100 };
static void reset(unsigned wflags, unsigned xflags, int n) {
    w13_spell_msg_b_host_base = arena;
    std::memset(arena, 0, sizeof arena); std::memset(player, 0, sizeof player);
    calls.clear(); attack_hits = 0; skill_result = 50; w160_result = 1; roll = 0; alloc_ok = true;
    put32(O + 76, Q); put32(Q + 12, W); put32(O + 172, S); put32(S + 96, TS); put32(TS + 112 + 4, 0x700); put32(O + 108, n);
    put32(W + 12, wflags); put32(W + 960, 1); put32(W + 72, TW);
    put32(X + 12, xflags);
    putf(O + 16, 1); putf(O + 20, 2); putf(O + 24, 3); putf(X + 16, 10); putf(X + 20, 20); putf(X + 24, 30);
    std::memcpy(player + 208 + 23984 + 4, "\x07\0\0\0\x09\0\0\0", 8);
}
int main() {
    unsigned char* const o = arena + O;
    unsigned char* const x = arena + X;
    reset(0x40, 0x20, 3);
    msg_collision__19FireArrowProjectileP10GameObject(o, x);
    if (calls != "skill(0,3);rand;attack;alloc1192;sf;term2;") { std::puts(calls.c_str()); return 1; }
    if (attack_flags != 0x60 || attack_a != 7.0f || attack_b != 9.0f || slot_words[0] != 31 || slot_words[1] != 100 || slot_words[2] != 50 || slot_words[3] != 50) return 2;
    if (sf_where.x != 10.0f || sf_a != 300.0f || sf_b != 3 * 60 + 120 || sf_c != 0.5f) return 3;
    const int ns[] = {5, 6, 10, 11, 15, 16, 99};
    const int expect[] = {0x60, 0x40060, 0x40060, 0x80060, 0x80060, 0x200060, 0x200060};
    for (int i = 0; i < 7; ++i) {
        reset(0x40, 0x20, ns[i]);
        msg_collision__19FireArrowProjectileP10GameObject(o, x);
        if (attack_flags != expect[i]) return 10 + i;
    }
    reset(0x40, 0x40, 3);
    msg_collision__19FireArrowProjectileP10GameObject(o, x);
    if (calls != "skill(0,3);rand;") return 20;
    reset(0x40, 0x01000000, 3);
    msg_collision__19FireArrowProjectileP10GameObject(o, x);
    if (attack_hits != 0) return 21;
    reset(0x40, 0x20, 3); std::memcpy(x + 160, "\0\0\0\x04\0\0\0\0", 8);
    msg_collision__19FireArrowProjectileP10GameObject(o, x);
    if (attack_hits != 0) return 22;
    // owner that is not a player
    reset(0x20, 0x0, 3);
    msg_collision__19FireArrowProjectileP10GameObject(o, x);
    if (attack_hits != 1 || attack_flags != 0x60 || attack_a != 50.0f || attack_b != 50.0f || slot_words[0] != 21 || slot_words[1] != 0) return 30;
    if (sf_where.x != 10.0f || sf_b != 1020) return 31;
    reset(0x20, 0x200, 3);
    msg_collision__19FireArrowProjectileP10GameObject(o, x);
    if (sf_where.x != 1.0f || sf_b != 1020) return 32;
    reset(0x20, 0x0, 3); w160_result = 0;
    msg_collision__19FireArrowProjectileP10GameObject(o, x);
    if (attack_hits != 0 || calls.find("term") != std::string::npos) return 33;
    reset(0x20, 0x0, 3); alloc_ok = false;
    msg_collision__19FireArrowProjectileP10GameObject(o, x);
    if (calls.find("sf;") != std::string::npos || calls.find("term2;") == std::string::npos) return 34;
    reset(0x20, 0x0, 3); put32(O + 76, 0);
    msg_collision__19FireArrowProjectileP10GameObject(o, x);
    if (!calls.empty()) return 35;
    return 0;
}
