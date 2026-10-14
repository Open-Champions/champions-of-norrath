// Host contract for _GLOBAL_$I$wesResourceMap and _GLOBAL_$D$wesResourceMap. Not an EE plan input.
// The expected tuples below come from running the executable's own initializer on a small interpreter, not
// from the authored source; the object identities are checked by docs/agents/w8-fx-particles-compare.py.
#include <cstdio>
#include <cstring>

extern "C" {
void _GLOBAL_$I$wesResourceMap();
void _GLOBAL_$D$wesResourceMap();
unsigned _14WoodElfSoldier$m_commanderList[10];
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
    std::memset(_14WoodElfSoldier$m_commanderList, 0xa5, sizeof _14WoodElfSoldier$m_commanderList);
    _GLOBAL_$I$wesResourceMap();
    expect(calls == count, "constructor call count");
    for (int n = 0; n < 10; ++n) expect(_14WoodElfSoldier$m_commanderList[n] == 0u, "array word cleared");
    expect(destroyed == 0, "no destruction at startup");
    _GLOBAL_$D$wesResourceMap();
    expect(destroyed == 10, "one destroy per element");
    for (int n = 0; n < 10 && n < 64; ++n) expect(destroyed_at[n] == &_14WoodElfSoldier$m_commanderList[10 - 1 - n], "destroyed from the last element down");
    if (failures) return 1;
    std::printf("wesResourceMap contract passed\n");
    return 0;
}
