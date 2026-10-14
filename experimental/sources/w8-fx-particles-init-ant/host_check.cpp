// Host contract for _GLOBAL_$I$antResourceMap. Not an EE plan input.
// The expected tuples below come from running the executable's own initializer on a small interpreter, not
// from the authored source; the object identities are checked by docs/agents/w8-fx-particles-compare.py.
#include <cstdio>
#include <cstring>

extern "C" {
void _GLOBAL_$I$antResourceMap();
}

struct Call { int ints[8]; unsigned floats[3]; int mode, tier, grid; };
static const Call expected[] = {
    {{50, 50, 20, 150, 40, 40, 10, 0}, {0x4203b6bau, 0x423954a5u, 0xbf9accf6u}, 2, 0, 4},
    {{2, 83, 74, 224, 31, 43, 31, 214}, {0x42a4570au, 0x42956d91u, 0x00000000u}, 1, 3, 2},
};
static const int count = 2;
static int calls = 0;
static void* objects[2];
static int failures = 0;
static void expect(bool ok, const char* what) {
    if (!ok) { std::fprintf(stderr, "fail: %s\n", what); ++failures; }
}
static unsigned float_bits(float v) { unsigned u; std::memcpy(&u, &v, 4); return u; }

extern "C" void __11ParticleDefiiiiiiiifffiiiPFPv_v(void* self, int a, int b, int c, int d, int e, int f, int g, int h, float x, float y,
                                         float z, int i, int j, int k, void (*callback)(void*)) {
    if (calls >= count) { ++calls; return; }
    const Call& want = expected[calls];
    const int got[8] = {a, b, c, d, e, f, g, h};
    for (int n = 0; n < 8; ++n) expect(got[n] == want.ints[n], "integer argument");
    expect(float_bits(x) == want.floats[0] && float_bits(y) == want.floats[1] && float_bits(z) == want.floats[2], "float argument");
    expect(i == want.mode && j == want.tier && k == want.grid && callback == nullptr, "trailing arguments");
    objects[calls] = self;
    ++calls;
}

int main() {
    _GLOBAL_$I$antResourceMap();
    expect(calls == count, "constructor call count");
    for (int n = 0; n < calls && n < count; ++n) {
        for (int m = 0; m < n; ++m) {
            expect(objects[m] != objects[n], "each call has its own object");
        }
    }
    if (failures) return 1;
    std::printf("antResourceMap contract passed\n");
    return 0;
}
