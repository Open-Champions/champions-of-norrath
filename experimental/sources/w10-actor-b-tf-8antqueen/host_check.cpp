// Host check only. Not an EE build input.
#include "typeinfo.cpp"
#include <cstdio>
#include <cstring>
#include <string>

unsigned char __ti8Creature[12];
static std::string calls;
extern "C" void* __tf8Creature() { calls += "tf8Creature;"; return nullptr; }
static const char* node_name(const void* p) {
    if (p == __ti8AntQueen) return "8AntQueen";
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
    std::memset(__ti8AntQueen, 0, 12);
    calls.clear();
}
int main() {
    clear_nodes();
    expect(__tf8AntQueen() == __ti8AntQueen, "returns the outer node");
    expect(calls == "tf8Creature;si(8AntQueen,8AntQueen,8Creature);", "all levels empty: root call, then each level innermost first");
    calls.clear();
    expect(__tf8AntQueen() == __ti8AntQueen && calls.empty(), "set node makes no calls");
    for (int byte = 0; byte < 4; ++byte) {
        clear_nodes();
        __ti8AntQueen[byte] = 1;
        expect(__tf8AntQueen() == __ti8AntQueen && calls.empty(), "any nonzero byte of the first word skips");
    }
    clear_nodes();
    __ti8AntQueen[4] = 9; __ti8AntQueen[11] = 4;
    __tf8AntQueen();
    expect(!calls.empty(), "only the first word gates initialization");
    return failures ? 1 : 0;
}
