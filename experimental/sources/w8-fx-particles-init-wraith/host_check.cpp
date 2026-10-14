// Host contract for _GLOBAL_$I$wraithResourceMap and _GLOBAL_$D$wraithResourceMap. Not an EE plan input.
// The expected tuples below come from running the executable's own initializer on a small interpreter, not
// from the authored source; the object identities are checked by docs/agents/w8-fx-particles-compare.py.
#include <cstdio>
#include <cstring>

extern "C" {
void _GLOBAL_$I$wraithResourceMap();
void _GLOBAL_$D$wraithResourceMap();
unsigned _6Wraith$s_rWraithList[60];
}

struct Call { int ints[8]; unsigned floats[3]; int mode, tier, grid; };
static const Call expected[] = {
};
static const int count = 0;
static int calls = 0;
static void* objects[1];
static int failures = 0;
static void expect(bool ok, const char* what) {
    if (!ok) { std::fprintf(stderr, "fail: %s\n", what); ++failures; }
}
static unsigned float_bits(float v) { unsigned u; std::memcpy(&u, &v, 4); return u; }

static int destroyed = 0;
static void* destroyed_at[64];
extern "C" void listDestroy__FP8ListHead(void* head) { if (destroyed < 64) destroyed_at[destroyed] = head; ++destroyed; }

int main() {
    std::memset(_6Wraith$s_rWraithList, 0xa5, sizeof _6Wraith$s_rWraithList);
    _GLOBAL_$I$wraithResourceMap();
    expect(calls == count, "constructor call count");
    for (int n = 0; n < 60; ++n) expect(_6Wraith$s_rWraithList[n] == 0u, "array word cleared");
    expect(destroyed == 0, "no destruction at startup");
    _GLOBAL_$D$wraithResourceMap();
    expect(destroyed == 60, "one destroy per element");
    for (int n = 0; n < 60 && n < 64; ++n) expect(destroyed_at[n] == &_6Wraith$s_rWraithList[60 - 1 - n], "destroyed from the last element down");
    if (failures) return 1;
    std::printf("wraithResourceMap contract passed\n");
    return 0;
}
