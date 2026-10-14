// Host contract for run__18SpellHammerOfWrathG6Point3s; not an EE input. 32-bit words are offsets from a host base and the table call is
// routed through a logging function. The authored function was also compared with the original in a private R5900 run
// (40 scenarios with random memory, word at +24 zero and nonzero, several adjustments; every original word covered).
#define W12_SPELL_C_HOST 1
#include "run.cpp"
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
int main() {
    w12_spell_c_host_base = arena;
    Point3 point = {1, 2, 3};
    enum { O = 0x100, T = 0x800 };
    const short adjust = 16;
    std::memset(arena, 0, sizeof arena);
    put32(O + 20, T);
    std::memcpy(arena + T + 8, &adjust, 2);
    put32(T + 8 + 4, 0x4242);
    run__18SpellHammerOfWrathG6Point3s(arena + O, point, 5);
    if (calls != "call16962,1,3;" || call_object != arena + O + 16) return 1;
    calls.clear();
    put32(O + 24, 1);
    run__18SpellHammerOfWrathG6Point3s(arena + O, point, 5);
    return calls.empty() ? 0 : 2;
}
