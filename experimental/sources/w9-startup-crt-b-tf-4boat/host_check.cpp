// Host check only; not an EE build input.
#include "typeinfo.cpp"
#include <cstdio>
#include <cstring>
#include <string>

unsigned char __ti7Vehicle[12];
static std::string calls;
extern "C" void* __tf7Vehicle() { calls += "__tf7Vehicle;"; return nullptr; }
static const char* node_name(const void* p) {
    if (p == __ti4Boat) return "4Boat";
    if (p == __ti7Vehicle) return "7Vehicle";
    return "?";
}
extern "C" void __rtti_si(void* t, const char* name, const void* b) {
    calls += std::string("si(") + node_name(t) + "," + name + "," + node_name(b) + ");";
    static_cast<unsigned char*>(t)[0] = 1;  // the real call stores a name pointer
}
static int failures;
static void expect(bool ok, const char* what) { if (!ok) { std::fprintf(stderr, "fail: %s (calls %s)\n", what, calls.c_str()); ++failures; } }
static void clear_nodes() {
    std::memset(__ti4Boat, 0, 12);
    calls.clear();
}
int main() {
    clear_nodes();
    void* r = __tf4Boat();
    expect(r == __ti4Boat, "returns the outer node");
    expect(calls == "__tf7Vehicle;si(4Boat,4Boat,7Vehicle);", "all levels empty: base call, then each level from the innermost out");
    calls.clear();
    expect(__tf4Boat() == __ti4Boat && calls.empty(), "set node makes no calls");
    for (int byte = 0; byte < 4; ++byte) {
        clear_nodes();
        __ti4Boat[byte] = 1;
        expect(__tf4Boat() == __ti4Boat && calls.empty(), "any nonzero byte of the first word skips");
    }
    clear_nodes();
    __ti4Boat[4] = 9; __ti4Boat[11] = 4;
    __tf4Boat();
    expect(!calls.empty(), "only the first word gates initialization");
    if (!failures) std::puts("tf4Boat ok");
    return failures ? 1 : 0;
}
