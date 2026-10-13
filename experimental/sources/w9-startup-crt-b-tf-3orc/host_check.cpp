// Host check only; not an EE build input.
#include "typeinfo.cpp"
#include <cstdio>
#include <cstring>
#include <string>

unsigned char __ti8Creature[12];
static std::string calls;
extern "C" void* __tf8Creature() { calls += "__tf8Creature;"; return nullptr; }
static const char* node_name(const void* p) {
    if (p == __ti3Orc) return "3Orc";
    if (p == __ti8Creature) return "8Creature";
    return "?";
}
extern "C" void __rtti_si(void* t, const char* name, const void* b) {
    calls += std::string("si(") + node_name(t) + "," + name + "," + node_name(b) + ");";
    static_cast<unsigned char*>(t)[0] = 1;  // the real call stores a name pointer
}
static int failures;
static void expect(bool ok, const char* what) { if (!ok) { std::fprintf(stderr, "fail: %s (calls %s)\n", what, calls.c_str()); ++failures; } }
static void clear_nodes() {
    std::memset(__ti3Orc, 0, 12);
    calls.clear();
}
int main() {
    clear_nodes();
    void* r = __tf3Orc();
    expect(r == __ti3Orc, "returns the outer node");
    expect(calls == "__tf8Creature;si(3Orc,3Orc,8Creature);", "all levels empty: base call, then each level from the innermost out");
    calls.clear();
    expect(__tf3Orc() == __ti3Orc && calls.empty(), "set node makes no calls");
    for (int byte = 0; byte < 4; ++byte) {
        clear_nodes();
        __ti3Orc[byte] = 1;
        expect(__tf3Orc() == __ti3Orc && calls.empty(), "any nonzero byte of the first word skips");
    }
    clear_nodes();
    __ti3Orc[4] = 9; __ti3Orc[11] = 4;
    __tf3Orc();
    expect(!calls.empty(), "only the first word gates initialization");
    if (!failures) std::puts("tf3Orc ok");
    return failures ? 1 : 0;
}
