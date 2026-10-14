// Host contract for msg_run__11BoltGoRound; not an EE input. 32-bit words are offsets from a host base, the callees log, and
// rsqrt.s is replaced by a division. The authored function was also compared with the original in a private R5900 run (500
// scenarios over the end frame, reach, bolt count, hit results, flags and difficulty scale; every original word covered).
#define W12_SPELL_C_HOST 1
#include "msg_run.cpp"
#include <cmath>
#include <cstdio>
#include <cstring>
#include <string>
unsigned char* w12_spell_c_host_base;
static std::string calls;
static void* entry_object;
unsigned w12_spell_c_host_call(unsigned fn, void* object, int count, unsigned long a1, unsigned long) {
    calls += "entry" + std::to_string(fn) + "(" + std::to_string(count) + "," + std::to_string(a1) + ");";
    entry_object = object;
    return 0;
}
static unsigned char arena[0x2000] __attribute__((aligned(16)));
static unsigned char hit_object[64] __attribute__((aligned(16)));
static void* hit_result;
static float hit_r, difficulty;
struct Move { int handle; Point3 from, to; int a, b; };
static Move moves[8];
static int move_count, stop_channel, attack_count;
static float scan_range, scan_dx, scan_dy;
static short attack_slot[4];
static void* attack_target;
extern "C" {
int frameNm;
void* world;
float worldHitscanWithObjects__FP12_worldHeaderG6Point3T1fP10GameObjectPP10GameObjectii(void*, const Point3*, const Point3* delta, float range, void*, void** hit, int, int) {
    calls += "scan;"; scan_range = range; scan_dx = delta->x; scan_dy = delta->y; *hit = hit_result; return hit_r;
}
void P_MoveLightning__FiG6Point3T1ii(int handle, const Point3* from, const Point3* to, int a, int b) {
    calls += "move;"; moves[move_count++] = {handle, *from, *to, a, b};
}
float gameDifficultyScale__Fv() { calls += "scale;"; return difficulty; }
void gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(void* attacker, void* target, float, float, int, int, int, int, void* description, bool) {
    calls += "attack;"; ++attack_count; attack_target = attacker; std::memcpy(attack_slot, static_cast<unsigned char*>(description) + 36, 8); (void)target;
}
void soundStopSound__Fi(int channel) { calls += "stop;"; stop_channel = channel; }
}
static void put32(unsigned off, unsigned v) { std::memcpy(arena + off, &v, 4); }
static void putf(unsigned off, float v) { std::memcpy(arena + off, &v, 4); }
static float getf(unsigned off) { float v; std::memcpy(&v, arena + off, 4); return v; }
enum { O = 0x100, T = 0x800 };
static void reset(int frame, int end, float reach, short n) {
    w12_spell_c_host_base = arena;
    std::memset(arena, 0, sizeof arena);
    calls.clear(); move_count = 0; attack_count = 0; entry_object = nullptr; hit_result = nullptr; hit_r = 2.0f; difficulty = 10.5f;
    frameNm = frame;
    put32(O + 80, static_cast<unsigned>(end)); putf(O + 84, reach); std::memcpy(arena + O + 76, &n, 2);
    put32(O + 72, T); put32(T + 8 + 4, 0x99);
    const short adjust = 4;
    std::memcpy(arena + T + 8, &adjust, 2);
    put32(O + 88, 77); put32(O + 92, 78); put32(O + 152, 321);
    putf(O + 104, 3.0f); putf(O + 108, 4.0f); putf(O + 112, 0.0f);
    putf(O + 116, 0.0f); putf(O + 120, 6.0f); putf(O + 124, 8.0f);
}
int main() {
    // active, even frame: bolt 0 only; reach grows by 15; B = unit * 15
    reset(10, 20, 0.0f, 2);
    msg_run__11BoltGoRound(arena + O);
    if (calls != "scan;move;" || getf(O + 84) != 15.0f || move_count != 1) return 1;
    if (moves[0].handle != 77 || moves[0].a != 94 || moves[0].b != -1 || std::fabs(moves[0].to.x - 9.0f) > 1e-4 || std::fabs(moves[0].to.y - 12.0f) > 1e-4) return 2;
    if (scan_range != 1.1f && std::fabs(scan_range - 1.1f) > 1e-6) return 3;
    // odd frame starts at bolt 1 and the second target is used
    reset(11, 20, 480.0f, 2);
    msg_run__11BoltGoRound(arena + O);
    if (move_count != 1 || moves[0].handle != 78 || std::fabs(moves[0].to.y - 0.6f * 495.0f) > 1e-2 || std::fabs(moves[0].to.z - 0.8f * 495.0f) > 1e-2) return 4;
    // reach is capped for the next frame once it passes 500 and the line length is capped at 500
    reset(10, 20, 510.0f, 1);
    msg_run__11BoltGoRound(arena + O);
    if (getf(O + 84) != 510.0f || std::fabs(moves[0].to.x - 300.0f) > 1e-2) return 5;
    // a hit on a flagged object nearer than 1.0 pulls the end point to it and hurts it with the difficulty-scaled amount
    reset(10, 20, 0.0f, 1);
    hit_result = hit_object; hit_r = 0.5f;
    std::memset(hit_object, 0, sizeof hit_object);
    const unsigned flags = 0x40;
    std::memcpy(hit_object + 12, &flags, 4);
    const float hp[3] = {5, 6, 7};
    std::memcpy(hit_object + 16, hp, 12);
    msg_run__11BoltGoRound(arena + O);
    if (calls != "scan;scale;attack;move;" || attack_target != hit_object || attack_slot[0] != 24 || attack_slot[1] != 3 || attack_slot[2] != 3 || attack_slot[3] != 0) { std::puts(calls.c_str()); std::printf("%d %d %d\n", attack_slot[0], attack_slot[1], attack_slot[2]); return 6; }
    if (moves[0].to.x != 5.0f || moves[0].to.y != 6.0f || moves[0].to.z != 7.0f) return 7;
    // a hit that is too far, or on an unflagged object, changes nothing
    reset(10, 20, 0.0f, 1);
    hit_result = hit_object; hit_r = 1.0f;
    msg_run__11BoltGoRound(arena + O);
    if (calls != "scan;move;") return 8;
    reset(10, 20, 0.0f, 1);
    hit_result = hit_object; hit_r = 0.5f;
    std::memset(hit_object, 0, 16);
    msg_run__11BoltGoRound(arena + O);
    if (calls != "scan;move;") return 9;
    // inactive (past the end frame): no scan, reach falls by 15 and stops at zero; at zero the sound stops and the entry runs
    reset(30, 20, 20.0f, 1);
    msg_run__11BoltGoRound(arena + O);
    if (calls != "move;" || getf(O + 84) != 5.0f) return 10;
    reset(30, 20, 10.0f, 1);
    msg_run__11BoltGoRound(arena + O);
    if (calls != "move;stop;entry153(1,3);" || getf(O + 84) != 0.0f || stop_channel != 321 || entry_object != arena + O + 4) return 11;
    return 0;
}
