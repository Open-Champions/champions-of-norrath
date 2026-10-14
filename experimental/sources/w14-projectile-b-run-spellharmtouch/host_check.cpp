// Host contract for run__14SpellHarmTouchG6Point3s; not an EE input. 32-bit words are offsets from a host base and the table call
// is routed through a logging function. The authored function (without the host call route, which is compiled out for the EE) was
// also compared with the original in a private R5900 run (120 scenarios around the 1/60 step, zero, one and the table entry's
// adjustment; every original word covered).
#define W12_SPELL_C_HOST 1
#include "run.cpp"
#include <cmath>
#include <cstring>
#include <string>
unsigned char* w12_spell_c_host_base;
static std::string calls;
static void* call_object;
unsigned w12_spell_c_host_call(unsigned fn, void* object, int count, unsigned long a1, unsigned long) {
    calls += "call" + std::to_string(fn) + "," + std::to_string(count) + "," + std::to_string(a1) + ";";
    call_object = object;
    return 0;
}
static unsigned char arena[0x1000] __attribute__((aligned(16)));
static void put32(unsigned off, unsigned v) { std::memcpy(arena + off, &v, 4); }
static void putf(unsigned off, float v) { std::memcpy(arena + off, &v, 4); }
static float getf(unsigned off) { float v; std::memcpy(&v, arena + off, 4); return v; }
static short geth(unsigned off) { short v; std::memcpy(&v, arena + off, 2); return v; }
enum { O = 0x100, P = 0x400, T = 0x800 };
static void reset(float t) {
    std::memset(arena, 0, sizeof arena);
    calls.clear(); call_object = nullptr;
    put32(O + 12, P); put32(O + 20, T);
    const short adjust = 16;
    std::memcpy(arena + T + 8, &adjust, 2);
    put32(T + 8 + 4, 0x4242);
    putf(O + 24, t);
}
int main() {
    w12_spell_c_host_base = arena;
    Point3 point = {1, 2, 3};
    // time left above one second: the fade value is capped at 128
    reset(2.0f);
    run__14SpellHarmTouchG6Point3s(arena + O, point, 5);
    if (!calls.empty() || geth(P + 146) != 113 || geth(P + 142) != 128 || geth(P + 150) != 128) return 1;
    if (std::fabs(getf(O + 24) - (2.0f - 1.0f / 60.0f)) > 1e-6) return 2;
    // half a second left: 0.5 * 128 truncated
    reset(0.5f + 1.0f / 60.0f);
    run__14SpellHarmTouchG6Point3s(arena + O, point, 5);
    if (!calls.empty() || geth(P + 142) != 64 || geth(P + 150) != 64) return 3;
    // running out: the table entry at offset 8 is called with 3 on the adjusted object, and P is left alone
    reset(0.0f);
    run__14SpellHarmTouchG6Point3s(arena + O, point, 5);
    if (calls != "call16962,1,3;" || call_object != arena + O + 16 || geth(P + 146) != 0 || getf(O + 24) >= 0.0f) return 4;
    // just over 1/60 left stays on the fade branch with a fade of 0
    reset(0.0166667f);
    run__14SpellHarmTouchG6Point3s(arena + O, point, 5);
    if (!calls.empty() || geth(P + 146) != 113 || geth(P + 142) != 0) return 5;
    return 0;
}
