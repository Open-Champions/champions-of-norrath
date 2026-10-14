// Host contract for buildShockChainList__15ShockProjectileP6Point3; not an EE input. 32-bit words are offsets from a host base; the two
// helpers are logging stubs. The authored function was also compared with the original in a private R5900 run (150 scenarios
// with random lists, flags, limits and ranges; every original word covered).
#define W12_SPELL_C_HOST 1
#include "build.cpp"
#include <cstring>
#include <string>
unsigned char* w12_spell_c_host_base;
static std::string trace;
static bool chained_answer;
extern "C" {
int alreadyChained__15ShockProjectileP10GameObject(void*, void* object) {
    trace += "c" + std::to_string(static_cast<unsigned char*>(object) - w12_spell_c_host_base) + ";";
    return chained_answer ? 1 : 0;
}
void listOnewayLink__FP8ListHeadT0Pv(void* head, void* node, void* data) {
    trace += "l" + std::to_string(static_cast<unsigned char*>(head) - w12_spell_c_host_base) + "," +
             std::to_string(static_cast<unsigned char*>(node) - w12_spell_c_host_base) + (node == data ? "=" : "!") + ";";
}
}
static unsigned char arena[0x1000] __attribute__((aligned(16)));
static void put32(unsigned off, unsigned v) { std::memcpy(arena + off, &v, 4); }
static void putf(unsigned off, float v) { std::memcpy(arena + off, &v, 4); }
enum { O = 0x100, N1 = 0x400, N2 = 0x420, A = 0x500, B = 0x540 };
static void reset(int limit, int count) {
    std::memset(arena, 0, sizeof arena);
    trace.clear();
    arena[O + 394] = static_cast<unsigned char>(limit);
    arena[O + 395] = static_cast<unsigned char>(count);
    putf(O + 396, 100.0f);
    put32(O + 80, N1); put32(N1, N2); put32(N1 + 12, A); put32(N2 + 12, B);
    putf(A + 16, 3.0f); putf(A + 20, 4.0f);   // squared distance 25 from the origin
    putf(B + 16, 6.0f); putf(B + 20, 0.0f);   // squared distance 36
}
int main() {
    w12_spell_c_host_base = arena;
    Point3 origin = {0, 0, 0};
    chained_answer = false;
    // the nearer object is chosen (the farther one is not even tested for chaining) and nothing recurses
    reset(0, 1);
    buildShockChainList__15ShockProjectileP6Point3(arena + O, &origin);
    if (trace != "c1280;l340,1280=;") return 1;
    // range too small for both: nothing is linked
    reset(0, 1);
    putf(O + 396, 20.0f);
    buildShockChainList__15ShockProjectileP6Point3(arena + O, &origin);
    if (!trace.empty()) return 2;
    // an object whose flag word has bit 3 set is skipped
    reset(0, 1);
    put32(A + 12, 8);
    buildShockChainList__15ShockProjectileP6Point3(arena + O, &origin);
    if (trace != "c1344;l340,1344=;") return 3;
    // every candidate already chained
    reset(0, 1);
    chained_answer = true;
    buildShockChainList__15ShockProjectileP6Point3(arena + O, &origin);
    if (trace != "c1280;c1344;") return 4;
    chained_answer = false;
    // a null object is skipped
    reset(0, 1);
    put32(N1 + 12, 0);
    buildShockChainList__15ShockProjectileP6Point3(arena + O, &origin);
    if (trace != "c1344;l340,1344=;") return 5;
    // with a limit of 1 the count rises to 1 and the walk repeats from the chosen object, then from the origin; the second
    // level sees the count equal to the limit and stops
    reset(1, 0);
    buildShockChainList__15ShockProjectileP6Point3(arena + O, &origin);
    if (arena[O + 395] != 1) return 6;
    if (trace.compare(0, 17, "c1280;l340,1280=;") != 0) return 7;
    // already at the limit: returns at once
    reset(2, 2);
    buildShockChainList__15ShockProjectileP6Point3(arena + O, &origin);
    return trace.empty() ? 0 : 8;
}
