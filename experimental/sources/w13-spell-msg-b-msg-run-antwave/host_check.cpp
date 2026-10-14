// Host contract for msg_run__7AntWave; not an EE input. 32-bit words are offsets from a host base and the callees log.
// The authored function was also compared with the original in a private R5900 run (180 scenarios over target presence,
// the frame counter, its limit word, the radius and the target offset; every original word covered).
#define W13_SPELL_MSG_B_HOST 1
#include "../w13-spell-msg-b-common/access.hpp"
#include <cstdio>
#include <cstring>
#include <string>
unsigned char* w13_spell_msg_b_host_base;
static std::string calls;
unsigned w13_spell_msg_b_host_call(unsigned fn, void*, int, unsigned long a1, unsigned long) {
    calls += "entry" + std::to_string(fn) + "(" + std::to_string(a1) + ");";
    return 0;
}
void w13_spell_msg_b_host_hurt(unsigned fn, void* adjusted, float amount, void* source, int flags) {
    calls += "hurt" + std::to_string(fn) + "@" + std::to_string(static_cast<unsigned char*>(adjusted) - w13_spell_msg_b_host_base) + "(" +
             std::to_string(static_cast<int>(amount)) + "," + std::to_string(static_cast<unsigned char*>(source) - w13_spell_msg_b_host_base) + "," + std::to_string(flags) + ");";
}
extern "C" {
int everyFrame__8CreatureP12_modelHeader(void*, void* model) { calls += model == nullptr ? "every;" : "every?;"; return 0; }
static float persue_a, persue_b, persue_c;
static void* persue_target;
void persue2__8CreatureP10GameObjectfff(void*, void* target, float a, float b, float c) { calls += "persue;"; persue_target = target; persue_a = a; persue_b = b; persue_c = c; }
}
#include "msg_run.cpp"

static unsigned char arena[0x2000] __attribute__((aligned(16)));
static void put32(unsigned off, unsigned v) { std::memcpy(arena + off, &v, 4); }
static void putf(unsigned off, float v) { std::memcpy(arena + off, &v, 4); }
enum { O = 0x100, T = 0x600, TG = 0x700, OT = 0xc00, TT = 0xd00 };
static void reset(int n, int limit, float radius, float dist) {
    w13_spell_msg_b_host_base = arena;
    std::memset(arena, 0, sizeof arena); calls.clear();
    put32(O + 968, T); put32(T + 12, TG); put32(O + 808, n); put32(O + 964, limit);
    put32(O + 72, OT); put32(OT + 8 + 4, 0x800); put32(TG + 72, TT); put32(TT + 16 + 4, 0x900);
    putf(O + 16, 0); putf(O + 20, 0); putf(O + 24, 0);
    putf(TG + 16, dist); putf(TG + 20, 0); putf(TG + 24, 0);
    putf(O + 28, radius);
}
int main() {
    unsigned char* const o = arena + O;
    reset(5, 100, 10.0f, 3.0f); put32(O + 968, 0);
    msg_run__7AntWave(o);
    if (calls != "every;entry2048(3);") { std::puts(calls.c_str()); return 1; }
    // a target, counter not a multiple of 60, limit not reached
    reset(5, 100, 10.0f, 3.0f);
    msg_run__7AntWave(o);
    if (calls != "every;persue;") return 2;
    if (persue_target != arena + TG || persue_a != 500.0f || persue_b != 1.0f || persue_c != 0.0f) return 3;
    // counter 60: inside the radius: hurt, and the limit passed: entry 8
    reset(60, 59, 10.0f, 3.0f);
    msg_run__7AntWave(o);
    if (calls != "every;persue;hurt2304@1792(1,256,256);entry2048(3);") { std::puts(calls.c_str()); return 4; }
    // counter 60 outside the radius: no hurt
    reset(60, 60, 2.0f, 3.0f);
    msg_run__7AntWave(o);
    if (calls != "every;persue;") return 5;
    // exactly on the radius is not inside
    reset(60, 100, 3.0f, 3.0f);
    msg_run__7AntWave(o);
    if (calls != "every;persue;") return 6;
    // negative counter that is a multiple of 60
    reset(-60, -61, 10.0f, 3.0f);
    msg_run__7AntWave(o);
    if (calls.find("hurt") == std::string::npos || calls.find("entry") == std::string::npos) return 7;
    return 0;
}
