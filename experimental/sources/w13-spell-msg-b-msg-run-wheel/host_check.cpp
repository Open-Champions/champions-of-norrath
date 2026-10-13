// Host contract for msg_run__5Wheel; not an EE input. The callees log. 32-bit words are offsets from a host base. The
// authored function was also compared with the original in a private R5900 run (150 scenarios over water result, the
// block vector length, roll values; every original word but one delay-slot nop covered).
#define W13_SPELL_MSG_B_HOST 1
#include "../w13-spell-msg-b-common/access.hpp"
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
unsigned char* w13_spell_msg_b_host_base;
unsigned w13_spell_msg_b_host_call(unsigned, void*, int, unsigned long, unsigned long) { return 0; }
static std::string calls;
static std::vector<int> rolls;
static unsigned roll_index;
static int water_result;
static Point3 water_point, perturb_point;
static float water_radius, perturb_a, perturb_b;
extern "C" {
void* world = reinterpret_cast<void*>(0x1234);
int eRand__10GameObject(void*) { calls += "rand;"; return rolls[roll_index++ % rolls.size()]; }
int worldCheckForWater__FP12_worldHeaderG6Point3fPf(void*, Point3 p, float radius, float* out) {
    calls += out == nullptr ? "water;" : "waterout;"; water_point = p; water_radius = radius; return water_result;
}
void worldPerturbWater__FP12_worldHeaderG6Point3ff(void*, Point3 p, float a, float b) { calls += "perturb;"; perturb_point = p; perturb_a = a; perturb_b = b; }
}
#include "msg_run.cpp"

static unsigned char arena[0x2000] __attribute__((aligned(16)));
static void put32(unsigned off, unsigned v) { std::memcpy(arena + off, &v, 4); }
static void putf(unsigned off, float v) { std::memcpy(arena + off, &v, 4); }
static unsigned get32(unsigned off) { unsigned v; std::memcpy(&v, arena + off, 4); return v; }
enum { O = 0x100, B = 0x800 };
static void reset(float x, float y, float z, int water) {
    w13_spell_msg_b_host_base = arena;
    std::memset(arena, 0, sizeof arena);
    calls.clear(); roll_index = 0; water_result = water; rolls = {0};
    put32(O + 396, B); put32(O + 384, 2); put32(B + 384, 77);
    putf(O + 16, 1); putf(O + 20, 2); putf(O + 24, 100); putf(O + 376, 2);
    putf(B + 2 * 12 + 1836, x); putf(B + 2 * 12 + 1840, y); putf(B + 2 * 12 + 1844, z);
}
int main() {
    unsigned char* const o = arena + O;
    reset(0, 0, 0, 0);
    msg_run__5Wheel(o);
    if (calls != "water;" || get32(O + 348) != 77 || get32(O + 392) != 0 || water_point.z != 100.0f - 78.0f || water_radius != 24.0f) return 1;
    // vector (3, 4, 0): m = 5. An odd first roll and a non-zero remainder perturb with 500.
    reset(3, 4, 0, 1); rolls = {1, 3};
    msg_run__5Wheel(o);
    if (calls != "water;rand;rand;perturb;" || perturb_a != 500.0f || perturb_b != 24.0f || get32(O + 392) != 1) return 2;
    reset(3, 4, 0, 1); rolls = {2, 3};
    msg_run__5Wheel(o);
    if (calls != "water;rand;") return 3;
    reset(3, 4, 0, 1); rolls = {1, 10};
    msg_run__5Wheel(o);
    if (calls != "water;rand;rand;") return 4;
    reset(3, 4, 0, 1); rolls = {1, -3};
    msg_run__5Wheel(o);
    if (calls != "water;rand;rand;perturb;") return 5;
    // vector shorter than 1: m = 0, a roll that is a multiple of 20 perturbs with 400
    reset(0.5f, 0, 0, 1); rolls = {40};
    msg_run__5Wheel(o);
    if (calls != "water;rand;perturb;" || perturb_a != 400.0f || perturb_b != 24.0f) return 6;
    reset(0.5f, 0, 0, 1); rolls = {41};
    msg_run__5Wheel(o);
    if (calls != "water;rand;") return 7;
    return 0;
}
