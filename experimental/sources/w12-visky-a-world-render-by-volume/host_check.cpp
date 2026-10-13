// Host check only; not a build input. Native build: c++ -std=c++17 -ffp-contract=off host_check.cpp
#include "render_by_volume.cpp"
#include <cstring>
#include <string>

static std::string trace;
static int result;
static unsigned char *seen_self, *seen_vars;
static unsigned vars_word_at_call, counter_at_call;
extern "C" int RenderByVolume__7VIWorldiR15VISceneRendVars(unsigned char* self, int mode, unsigned char* vars) {
    trace += "call:" + std::to_string(mode) + ";";
    seen_self = self; seen_vars = vars;
    std::memcpy(&vars_word_at_call, vars, 4);
    std::memcpy(&counter_at_call, self + 68, 4);
    return result;
}

static void put(unsigned char* p, unsigned off, int v) { std::memcpy(p + off, &v, 4); }
static int get(const unsigned char* p, unsigned off) { int v; std::memcpy(&v, p + off, 4); return v; }

int main() {
    alignas(8) unsigned char world[128], vars[32];
    // Word at +52 nonzero: mode 0.
    std::memset(world, 0x3c, sizeof world); std::memset(vars, 0x5d, sizeof vars);
    put(world, 52, 1); put(world, 68, 41); trace.clear(); result = 0;
    if (RenderByVolume__7VIWorldR15VISceneRendVars(world, vars) != 0) return 1;
    if (trace != "call:0;" || seen_self != world || seen_vars != vars || vars_word_at_call != 41 || counter_at_call != 41) return 2;
    if (get(world, 68) != 42 || get(vars, 0) != 41 || vars[4] != 0x5d || vars[31] != 0x5d) return 3;
    // Word at +52 zero: mode 0x40000000.
    put(world, 52, 0); trace.clear(); result = 7;
    if (RenderByVolume__7VIWorldR15VISceneRendVars(world, vars) != 0 || trace != "call:1073741824;" || get(world, 68) != 43) return 4;
    // Negative result: -1, the counter stays, but the published copy was already stored.
    put(world, 68, 10); trace.clear(); result = -5;
    if (RenderByVolume__7VIWorldR15VISceneRendVars(world, vars) != -1 || get(world, 68) != 10 || get(vars, 0) != 10) return 5;
    return 0;
}
