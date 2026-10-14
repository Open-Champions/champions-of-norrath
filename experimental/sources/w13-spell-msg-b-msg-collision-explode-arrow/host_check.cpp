// Host contract for msg_collision__22ExplodeArrowProjectileP10GameObject; not an EE input. 32-bit words are offsets from
// a host base and the callees log. The authored function was also compared with the original in a private R5900 run
// (221 scenarios over owner flags, n thresholds, list lengths, the 160 entry result, radius and roll values; every
// original word covered).
#define W13_SPELL_MSG_B_HOST 1
#include "../w13-spell-msg-b-common/access.hpp"
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
unsigned char* w13_spell_msg_b_host_base;
static std::string calls;
static int radius_result = 10, base_result = 50, roll = 0, blocked_offset = -1;
unsigned w13_spell_msg_b_host_call(unsigned fn, void* adjusted, int, unsigned long a1, unsigned long a2) {
    if (fn == 0x700) {
        calls += "e104(" + std::to_string(a1) + "," + std::to_string(a2) + ");";
        return a1 == 2 ? radius_result : base_result;
    }
    calls += "w160(" + std::to_string(a1) + ");";
    (void)adjusted;
    return static_cast<int>(a1) == blocked_offset ? 0 : 1;
}
extern "C" {
unsigned eRand__10GameObject(void*) { calls += "rand;"; return roll; }
static Point3 box_low, box_high;
static int box_capacity;
static std::vector<unsigned> listed;
void objectFindInBox__FG6Point3T0PP10GameObjecti(Point3 low, Point3 high, void** found, int capacity) {
    calls += "box;"; box_low = low; box_high = high; box_capacity = capacity;
    unsigned* out = reinterpret_cast<unsigned*>(found);
    // the host stores objects as offsets from the base, like every other word
    void** slots = found;
    for (size_t i = 0; i < listed.size(); ++i) slots[i] = w13_spell_msg_b_host_base + listed[i];
    slots[listed.size()] = nullptr;
    (void)out;
}
static int attack_flags, attack_hits;
static float attack_a, attack_b;
static std::vector<unsigned> targets;
int gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(void* a, void*, float x, float y, int, int, int, int s, void* description, bool) {
    calls += "attack;"; ++attack_hits; attack_flags = s; attack_a = x; attack_b = y;
    targets.push_back(static_cast<unsigned>(static_cast<unsigned char*>(a) - w13_spell_msg_b_host_base));
    return description == nullptr ? 0 : 1;
}
void terminate__22ExplodeArrowProjectilei(void*, int hit) { calls += "term" + std::to_string(hit) + ";"; }
}
#include "msg_collision.cpp"

static unsigned char arena[0x2000] __attribute__((aligned(16)));
static void put32(unsigned off, unsigned v) { std::memcpy(arena + off, &v, 4); }
static void putf(unsigned off, float v) { std::memcpy(arena + off, &v, 4); }
enum { O = 0x100, X = 0x500, Q = 0x900, W = 0xa00, S = 0xe00, TS = 0x1000, TW = 0x1100 };
static void reset(unsigned wflags, int n) {
    w13_spell_msg_b_host_base = arena;
    std::memset(arena, 0, sizeof arena);
    calls.clear(); attack_hits = 0; targets.clear(); listed.clear(); radius_result = 10; base_result = 50; roll = 0; blocked_offset = -1;
    put32(O + 76, Q); put32(Q + 12, W); put32(O + 112, S); put32(S + 96, TS); put32(TS + 104 + 4, 0x700); put32(O + 104, n);
    put32(W + 12, wflags); put32(W + 72, TW);
    putf(O + 16, 1); putf(O + 20, 2); putf(O + 24, 3);
}
int main() {
    unsigned char* const o = arena + O;
    unsigned char* const x = arena + X;
    reset(0x20, 3); listed = {0x1200, 0x1300};
    msg_collision__22ExplodeArrowProjectileP10GameObject(o, x);
    if (calls != "w160(1280);e104(2,3);box;w160(4608);e104(0,3);rand;attack;w160(4864);e104(0,3);rand;attack;term2;") { std::puts(calls.c_str()); return 1; }
    if (box_low.x != -9.0f || box_low.y != -8.0f || box_low.z != -7.0f || box_high.x != 11.0f || box_high.z != 13.0f || box_capacity != 512) return 2;
    if (attack_flags != 0 || attack_a != 50.0f || attack_b != 50.0f || targets.size() != 2 || targets[0] != 0x1200 || targets[1] != 0x1300) return 3;
    const int ns[] = {5, 6, 11, 12, 40};
    const int expect[] = {0, 0x40000, 0x40000, 0x80000, 0x80000};
    for (int i = 0; i < 5; ++i) {
        reset(0x20, ns[i]); listed = {0x1200};
        msg_collision__22ExplodeArrowProjectileP10GameObject(o, x);
        if (attack_flags != expect[i]) return 10 + i;
    }
    // roll 65536 -> fraction 1.0: 50 + 50 * 0.2f * 1.0 rounds to 60 (the sum is added before the cast)
    reset(0x20, 3); listed = {0x1200}; roll = 65536;
    msg_collision__22ExplodeArrowProjectileP10GameObject(o, x);
    if (attack_a != 50.0f || attack_b != 60.0f) return 20;
    // a listed object the 160 entry refuses is skipped
    reset(0x20, 3); listed = {0x1200, 0x1300}; blocked_offset = 0x1200;
    msg_collision__22ExplodeArrowProjectileP10GameObject(o, x);
    if (attack_hits != 1 || targets[0] != 0x1300) return 21;
    // an empty list still ends the projectile
    reset(0x20, 3);
    msg_collision__22ExplodeArrowProjectileP10GameObject(o, x);
    if (calls != "w160(1280);e104(2,3);box;term2;") { std::puts(calls.c_str()); return 22; }
    // the first 160 entry refuses X: nothing else happens
    reset(0x20, 3); blocked_offset = X; listed = {0x1200};
    msg_collision__22ExplodeArrowProjectileP10GameObject(o, x);
    if (calls != "w160(1280);") return 23;
    reset(0x00, 3);
    msg_collision__22ExplodeArrowProjectileP10GameObject(o, x);
    if (!calls.empty()) return 24;
    reset(0x20, 3); put32(O + 76, 0);
    msg_collision__22ExplodeArrowProjectileP10GameObject(o, x);
    if (!calls.empty()) return 25;
    return 0;
}
