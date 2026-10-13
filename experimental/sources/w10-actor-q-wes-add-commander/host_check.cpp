// Host check only; not an EE build input.
#include "add_commander.cpp"
#include <cstdio>
unsigned WoodElfSoldier_commanderList[10] __asm__("_14WoodElfSoldier$m_commanderList");
static int failures, calls; static void* a; static void* b; static void* c;
extern "C" void* listOnewayLink__FP8ListHeadT0Pv(void* h, void* s, void* v) { ++calls; a = h; b = s; c = v; return nullptr; }
static void expect(bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } }
int main() {
    int object;
    for (int first = 0; first < 10; ++first) {
        for (int n = 0; n < 10; ++n) WoodElfSoldier_commanderList[n] = n < first ? 7u : 0u;
        if (first < 9) WoodElfSoldier_commanderList[first + 1] = 5u;  // later heads are irrelevant
        calls = 0;
        addCommander__14WoodElfSoldier(&object);
        expect(calls == 1 && a == &WoodElfSoldier_commanderList[first] && b == &object && c == &object, "links the first empty head");
    }
    for (int n = 0; n < 10; ++n) WoodElfSoldier_commanderList[n] = 1u;
    calls = 0;
    addCommander__14WoodElfSoldier(&object);
    expect(calls == 0, "full list links nothing");
    for (int n = 0; n < 10; ++n) WoodElfSoldier_commanderList[n] = 0u;
    WoodElfSoldier_commanderList[9] = 0u; WoodElfSoldier_commanderList[0] = 1u; calls = 0;
    addCommander__14WoodElfSoldier(&object);
    expect(calls == 1 && a == &WoodElfSoldier_commanderList[1], "skips only nonzero heads");
    if (!failures) std::puts("addCommander ok");
    return failures ? 1 : 0;
}
