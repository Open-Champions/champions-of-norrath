// Host check only; not an EE build input.
#include "typeinfo.cpp"
#include <cstdio>
#include <cstring>
#include <string>

unsigned char __ti10AutoObject[12];
unsigned char __ti10GameObject[12];
unsigned char __ti9AutoChest[12];
static std::string calls;
extern "C" void* __tf10GameObject() { calls += "__tf10GameObject;"; return nullptr; }
static const char* node_name(const void* p) {
    if (p == __ti11AutoChestLP) return "11AutoChestLP";
    if (p == __ti9AutoChest) return "9AutoChest";
    if (p == __ti10AutoObject) return "10AutoObject";
    if (p == __ti10GameObject) return "10GameObject";
    return "?";
}
extern "C" void __rtti_si(void* t, const char* name, const void* b) {
    calls += std::string("si(") + node_name(t) + "," + name + "," + node_name(b) + ");";
    static_cast<unsigned char*>(t)[0] = 1;  // the real call stores a name pointer
}
static int failures;
static void expect(bool ok, const char* what) { if (!ok) { std::fprintf(stderr, "fail: %s (calls %s)\n", what, calls.c_str()); ++failures; } }
static void clear_nodes() {
    std::memset(__ti11AutoChestLP, 0, 12);
    std::memset(__ti9AutoChest, 0, 12);
    std::memset(__ti10AutoObject, 0, 12);
    calls.clear();
}
int main() {
    clear_nodes();
    void* r = __tf11AutoChestLP();
    expect(r == __ti11AutoChestLP, "returns the outer node");
    expect(calls == "__tf10GameObject;si(10AutoObject,10AutoObject,10GameObject);si(9AutoChest,9AutoChest,10AutoObject);si(11AutoChestLP,11AutoChestLP,9AutoChest);", "all levels empty: base call, then each level from the innermost out");
    calls.clear();
    expect(__tf11AutoChestLP() == __ti11AutoChestLP && calls.empty(), "set node makes no calls");
    for (int byte = 0; byte < 4; ++byte) {
        clear_nodes();
        __ti11AutoChestLP[byte] = 1;
        expect(__tf11AutoChestLP() == __ti11AutoChestLP && calls.empty(), "any nonzero byte of the first word skips");
    }
    clear_nodes();
    __ti11AutoChestLP[4] = 9; __ti11AutoChestLP[11] = 4;
    __tf11AutoChestLP();
    expect(!calls.empty(), "only the first word gates initialization");
    clear_nodes();
    __ti9AutoChest[0] = 1;
    __tf11AutoChestLP();
    expect(calls == "si(11AutoChestLP,11AutoChestLP,9AutoChest);", "inner level 9AutoChest already set");
    clear_nodes();
    __ti10AutoObject[0] = 1;
    __tf11AutoChestLP();
    expect(calls == "si(9AutoChest,9AutoChest,10AutoObject);si(11AutoChestLP,11AutoChestLP,9AutoChest);", "inner level 10AutoObject already set");
    if (!failures) std::puts("tf11AutoChestLP ok");
    return failures ? 1 : 0;
}
