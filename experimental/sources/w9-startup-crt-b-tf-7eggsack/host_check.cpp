// Host check only; not an EE build input.
#include "typeinfo.cpp"
#include <cstdio>
#include <cstring>
#include <string>

unsigned char __ti10AutoPropLP[12];
static std::string calls;
extern "C" void* __tf10AutoPropLP() { calls += "__tf10AutoPropLP;"; return nullptr; }
static const char* node_name(const void* p) {
    if (p == __ti7EggSack) return "7EggSack";
    if (p == __ti10AutoPropLP) return "10AutoPropLP";
    return "?";
}
extern "C" void __rtti_si(void* t, const char* name, const void* b) {
    calls += std::string("si(") + node_name(t) + "," + name + "," + node_name(b) + ");";
    static_cast<unsigned char*>(t)[0] = 1;  // the real call stores a name pointer
}
static int failures;
static void expect(bool ok, const char* what) { if (!ok) { std::fprintf(stderr, "fail: %s (calls %s)\n", what, calls.c_str()); ++failures; } }
static void clear_nodes() {
    std::memset(__ti7EggSack, 0, 12);
    calls.clear();
}
int main() {
    clear_nodes();
    void* r = __tf7EggSack();
    expect(r == __ti7EggSack, "returns the outer node");
    expect(calls == "__tf10AutoPropLP;si(7EggSack,7EggSack,10AutoPropLP);", "all levels empty: base call, then each level from the innermost out");
    calls.clear();
    expect(__tf7EggSack() == __ti7EggSack && calls.empty(), "set node makes no calls");
    for (int byte = 0; byte < 4; ++byte) {
        clear_nodes();
        __ti7EggSack[byte] = 1;
        expect(__tf7EggSack() == __ti7EggSack && calls.empty(), "any nonzero byte of the first word skips");
    }
    clear_nodes();
    __ti7EggSack[4] = 9; __ti7EggSack[11] = 4;
    __tf7EggSack();
    expect(!calls.empty(), "only the first word gates initialization");
    if (!failures) std::puts("tf7EggSack ok");
    return failures ? 1 : 0;
}
