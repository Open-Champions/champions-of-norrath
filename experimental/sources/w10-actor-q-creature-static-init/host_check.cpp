// Host check only; not an EE build input.
#include "static_init.cpp"
#include <cstdio>
#include <cstring>
struct Call { int ints[8]; unsigned floats[3]; int mode, tier, grid; };
static const Call expected[] = {
    {{200, 200, 150, 300, 200, 175, 100, 0}, {0x40000000u, 0x40a00000u, 0xc1a00000u}, 1, 2, 2},
    {{228, 150, 150, 150, 255, 0, 0, 0}, {0xc1000000u, 0x41700000u, 0x80000000u}, 2, 2, 6},
};
static int calls, failures;
static void* objects[3];
static void expect(bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } }
static unsigned fb(float v) { unsigned u; std::memcpy(&u, &v, 4); return u; }
extern "C" void __11ParticleDefiiiiiiiifffiiiPFPv_v(void* self, int a, int b, int c, int d, int e, int f, int g, int h,
                                                     float x, float y, float z, int i, int j, int k, void (*cb)(void*)) {
    if (calls < 2) {
        const Call& w = expected[calls];
        const int got[8] = {a, b, c, d, e, f, g, h};
        for (int n = 0; n < 8; ++n) expect(got[n] == w.ints[n], "integer argument");
        expect(fb(x) == w.floats[0] && fb(y) == w.floats[1] && fb(z) == w.floats[2], "float argument");
        expect(i == w.mode && j == w.tier && k == w.grid && cb == nullptr, "trailing arguments");
        objects[calls] = self;
    }
    ++calls;
}
int main() {
    _GLOBAL_$I$numInRegion__8creatureG6Point3f();
    expect(calls == 2, "two constructions");
    expect(objects[0] != objects[1] && objects[0] && objects[1], "distinct objects");
    if (!failures) std::puts("creature static init ok");
    return failures ? 1 : 0;
}
