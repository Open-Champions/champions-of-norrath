// Host check only; not an EE build input.
#include "typeinfo.cpp"
#include <cstdio>
#include <cstring>
#include <string>

unsigned char __ti10GameObject[12];
static std::string calls;
extern "C" void* __tf10GameObject() { calls += "__tf10GameObject;"; return nullptr; }
static const char* node_name(const void* p) {
    if (p == __ti7Blocker) return "7Blocker";
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
    std::memset(__ti7Blocker, 0, 12);
    calls.clear();
}
int main() {
    clear_nodes();
    void* r = __tf7Blocker();
    expect(r == __ti7Blocker, "returns the outer node");
    expect(calls == "__tf10GameObject;si(7Blocker,7Blocker,10GameObject);", "all levels empty: base call, then each level from the innermost out");
    calls.clear();
    expect(__tf7Blocker() == __ti7Blocker && calls.empty(), "set node makes no calls");
    for (int byte = 0; byte < 4; ++byte) {
        clear_nodes();
        __ti7Blocker[byte] = 1;
        expect(__tf7Blocker() == __ti7Blocker && calls.empty(), "any nonzero byte of the first word skips");
    }
    clear_nodes();
    __ti7Blocker[4] = 9; __ti7Blocker[11] = 4;
    __tf7Blocker();
    expect(!calls.empty(), "only the first word gates initialization");
    if (!failures) std::puts("tf7Blocker ok");
    return failures ? 1 : 0;
}
