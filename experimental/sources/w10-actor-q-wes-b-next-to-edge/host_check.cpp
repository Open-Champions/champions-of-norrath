// Host check only; not an EE build input.
#define W10_ACTOR_Q_HOST 1
#include "b_next_to_edge.cpp"
#include <cstdio>
#include <cstring>
#include <vector>
unsigned char* w10_actor_q_host_base;
void* world = reinterpret_cast<void*>(0x1234);
static std::vector<float> heights; static std::vector<float> seen_x, seen_y, seen_z; static int failures;
extern "C" float icos__Fi(int a) { return a == 0 ? 1.0f : a == 16384 ? 0.0f : a == 32767 ? -1.0f : 0.0f; }
extern "C" float isin__Fi(int a) { return a == 0 ? 0.0f : a == 16384 ? 1.0f : a == 32767 ? 0.0f : -1.0f; }
extern "C" float worldFindStandHeight__FP12_worldHeaderG6Point3f(void* w, const float* p, float r) {
    if (w != world || r != 10.0f) ++failures;
    seen_x.push_back(p[0]); seen_y.push_back(p[1]); seen_z.push_back(p[2]);
    return heights[seen_x.size() - 1];
}
static void expect(bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } }
int main() {
    unsigned char object[600];
    std::memset(object, 0, sizeof object);
    float pos[3] = {100.0f, 200.0f, 50.0f};
    std::memcpy(object + 16, pos, 12);
    // all probes at floor level: no edge, four probes at the documented points
    heights = {50.f, 50.f, 50.f, 50.f};
    expect(bNextToEdge__14WoodElfSoldier(object) == 0, "no edge");
    expect(seen_x.size() == 4, "four probes");
    expect(seen_x[0] == 130 && seen_y[0] == 200, "probe 1: +30 x");
    expect(seen_x[1] == 100 && seen_y[1] == 230, "probe 2: +30 y from the original point");
    expect(seen_x[2] == 70 && seen_y[2] == 230, "probe 3: -30 x added to probe 2");
    expect(seen_x[3] == 70 && seen_y[3] == 200, "probe 4: -30 y added to probe 3");
    for (float z : seen_z) expect(z == 50.0f, "z never changes");
    // a drop of exactly 100 is not an edge; below it is
    seen_x.clear(); seen_y.clear(); seen_z.clear(); heights = {-50.f, -50.f, -50.f, -50.f};
    expect(bNextToEdge__14WoodElfSoldier(object) == 0, "exactly 100 below is not low");
    seen_x.clear(); seen_y.clear(); seen_z.clear(); heights = {50.f, 50.f, -50.5f, 50.f};
    expect(bNextToEdge__14WoodElfSoldier(object) == 1 && seen_x.size() == 3, "stops at the first low probe");
    object[570] = 1; seen_x.clear();
    expect(bNextToEdge__14WoodElfSoldier(object) == 0 && seen_x.empty(), "nonzero +570 returns 0 without probing");
    if (!failures) std::puts("bNextToEdge ok");
    return failures ? 1 : 0;
}
