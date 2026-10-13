#include "render_by_frustum.cpp"

#include <cstring>
#include <vector>

struct Ev { int kind; unsigned a, b; unsigned long c, d, e; };
static std::vector<Ev> ev;
static int push_result, volume_result, pop_result;

extern "C" int PushFrustum__11VIClipStackR8VICamera(unsigned stack, unsigned camera) { ev.push_back({1, stack, camera, 0, 0, 0}); return push_result; }
extern "C" void GetTop__11VIClipStackRiPP7VIVect3T1PP7VIPlane(unsigned stack, void* a, void* b, void* c, void* d) {
    ev.push_back({2, stack, 0, (unsigned long)a, (unsigned long)b, (unsigned long)c});
    ev.back().b = (unsigned)(unsigned long)d;
}
extern "C" int RenderByVolume__7VIWorldR15VISceneRendVars(VIWorld* w, VISceneRendVars* v) { ev.push_back({3, 0, 0, (unsigned long)w, (unsigned long)v, 0}); return volume_result; }
extern "C" int Pop__11VIClipStack(unsigned stack) { ev.push_back({4, stack, 0, 0, 0, 0}); return pop_result; }

alignas(8) static unsigned char world[128];
alignas(8) static unsigned char vars[128];
static void put(unsigned char* base, int off, unsigned v) { std::memcpy(base + off, &v, 4); }
static unsigned get(unsigned char* base, int off) { unsigned v; std::memcpy(&v, base + off, 4); return v; }

int main() {
    VIWorld* w = reinterpret_cast<VIWorld*>(world);
    VISceneRendVars* v = reinterpret_cast<VISceneRendVars*>(vars);
    auto reset = [&](unsigned w52, unsigned w60) {
        std::memset(world, 0, sizeof world); std::memset(vars, 0, sizeof vars); ev.clear();
        put(world, 52, w52); put(world, 60, w60);
        put(vars, 20, 0x2000); put(vars, 12, 0x3000); put(vars, 40, 99);
    };
    // empty tree: only the two stores happen
    reset(0, 0);
    if (RenderByFrustum__7VIWorldR15VISceneRendVars(w, v) != 0 || !ev.empty()) return 1;
    if (get(vars, 40) != 1 || get(vars, 8) != (unsigned)(unsigned long)w) return 2;
    // either word nonzero runs the body
    int ok_cases[][2] = {{1, 0}, {0, 1}, {5, 6}};
    for (auto& c : ok_cases) {
        reset(c[0], c[1]); push_result = 0; volume_result = 3; pop_result = 4;
        if (RenderByFrustum__7VIWorldR15VISceneRendVars(w, v) != 0) return 3;
        if (ev.size() != 4) return 4;
        if (ev[0].kind != 1 || ev[0].a != 0x2000 || ev[0].b != 0x3000) return 5;
        if (ev[1].kind != 2 || ev[1].a != 0x2000 || ev[1].c != (unsigned long)(vars + 24) || ev[1].d != (unsigned long)(vars + 28) ||
            ev[1].e != (unsigned long)(vars + 32) || ev[1].b != (unsigned)(unsigned long)(vars + 36)) return 6;
        if (ev[2].kind != 3 || ev[2].c != (unsigned long)w || ev[2].d != (unsigned long)v) return 7;
        if (ev[3].kind != 4 || ev[3].a != 0x2000) return 8;
    }
    reset(1, 1); push_result = -2; volume_result = 0; pop_result = 0;
    if (RenderByFrustum__7VIWorldR15VISceneRendVars(w, v) != -1 || ev.size() != 1) return 9;
    reset(1, 1); push_result = 0; volume_result = -1; pop_result = 0;
    if (RenderByFrustum__7VIWorldR15VISceneRendVars(w, v) != -1 || ev.size() != 3) return 10;
    reset(1, 1); push_result = 0; volume_result = 0; pop_result = -7;
    if (RenderByFrustum__7VIWorldR15VISceneRendVars(w, v) != -1 || ev.size() != 4) return 11;
    return 0;
}
