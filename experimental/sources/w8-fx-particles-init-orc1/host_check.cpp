// Host contract for _GLOBAL_$I$orc1ResourceMap and _GLOBAL_$D$orc1ResourceMap. Not an EE plan input.
// The expected tuples below come from running the executable's own initializer on a small interpreter, not
// from the authored source; the object identities are checked by docs/agents/w8-fx-particles-compare.py.
#include <cstdio>
#include <cstring>

extern "C" {
void _GLOBAL_$I$orc1ResourceMap();
void _GLOBAL_$D$orc1ResourceMap();
unsigned _3Orc$m_commanderList[20];
}

struct Call { int ints[8]; unsigned floats[3]; int mode, tier, grid; };
static const Call expected[] = {
    {{0, 15, 225, 255, 0, 5, 150, 0}, {0xc1000000u, 0x41700000u, 0x80000000u}, 2, 2, 6},
};
static const int count = 1;
static int calls = 0;
static void* objects[1];
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
static int destroyed = 0;
static void* destroyed_at[64];
extern "C" void listDestroy__FP8ListHead(void* head) { if (destroyed < 64) destroyed_at[destroyed] = head; ++destroyed; }

int main() {
    std::memset(_3Orc$m_commanderList, 0xa5, sizeof _3Orc$m_commanderList);
    _GLOBAL_$I$orc1ResourceMap();
    expect(calls == count, "constructor call count");
    for (int n = 0; n < calls && n < count; ++n) {
        for (int m = 0; m < n; ++m) {
            expect(objects[m] != objects[n], "each call has its own object");
        }
    }
    for (int n = 0; n < 20; ++n) expect(_3Orc$m_commanderList[n] == 0u, "array word cleared");
    expect(destroyed == 0, "no destruction at startup");
    _GLOBAL_$D$orc1ResourceMap();
    expect(destroyed == 20, "one destroy per element");
    for (int n = 0; n < 20 && n < 64; ++n) expect(destroyed_at[n] == &_3Orc$m_commanderList[20 - 1 - n], "destroyed from the last element down");
    if (failures) return 1;
    std::printf("orc1ResourceMap contract passed\n");
    return 0;
}
