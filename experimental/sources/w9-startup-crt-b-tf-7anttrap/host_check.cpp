// Host check only; not an EE build input.
#include "typeinfo.cpp"
#include <cstdio>
#include <cstring>
#include <string>

unsigned char __ti8AutoProp[12];
static std::string calls;
extern "C" void* __tf8AutoProp() { calls += "__tf8AutoProp;"; return nullptr; }
static const char* node_name(const void* p) {
    if (p == __ti7AntTrap) return "7AntTrap";
    if (p == __ti8AutoProp) return "8AutoProp";
    return "?";
}
extern "C" void __rtti_si(void* t, const char* name, const void* b) {
    calls += std::string("si(") + node_name(t) + "," + name + "," + node_name(b) + ");";
    static_cast<unsigned char*>(t)[0] = 1;  // the real call stores a name pointer
}
static int failures;
static void expect(bool ok, const char* what) { if (!ok) { std::fprintf(stderr, "fail: %s (calls %s)\n", what, calls.c_str()); ++failures; } }
static void clear_nodes() {
    std::memset(__ti7AntTrap, 0, 12);
    calls.clear();
}
int main() {
    clear_nodes();
    void* r = __tf7AntTrap();
    expect(r == __ti7AntTrap, "returns the outer node");
    expect(calls == "__tf8AutoProp;si(7AntTrap,7AntTrap,8AutoProp);", "all levels empty: base call, then each level from the innermost out");
    calls.clear();
    expect(__tf7AntTrap() == __ti7AntTrap && calls.empty(), "set node makes no calls");
    for (int byte = 0; byte < 4; ++byte) {
        clear_nodes();
        __ti7AntTrap[byte] = 1;
        expect(__tf7AntTrap() == __ti7AntTrap && calls.empty(), "any nonzero byte of the first word skips");
    }
    clear_nodes();
    __ti7AntTrap[4] = 9; __ti7AntTrap[11] = 4;
    __tf7AntTrap();
    expect(!calls.empty(), "only the first word gates initialization");
    if (!failures) std::puts("tf7AntTrap ok");
    return failures ? 1 : 0;
}
