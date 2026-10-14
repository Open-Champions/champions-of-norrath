// Host check only; not an EE build input.
#include "typeinfo.cpp"
#include <cstdio>
#include <cstring>
#include <string>

unsigned char __ti8Creature[12];
static std::string calls;
static const char* node_name(const void* p) {
    if (p == __ti6Wraith) return "6Wraith";
    if (p == __ti8Creature) return "8Creature";
    return "?";
}
void* __tf8Creature() { calls += "__tf8Creature;"; return nullptr; }
void __rtti_si(void* t, const char* name, const void* b) {
    calls += std::string("si(") + node_name(t) + "," + name + "," + node_name(b) + ");";
    static_cast<unsigned char*>(t)[0] = 1;
}
static int failures;
static void expect(bool ok, const char* what) { if (!ok) { std::fprintf(stderr, "fail: %s (calls %s)\n", what, calls.c_str()); ++failures; } }
int main() {
    std::memset(__ti6Wraith, 0, 12); calls.clear();
    expect(__tf6Wraith() == __ti6Wraith, "returns the node");
    expect(calls == "__tf8Creature;si(6Wraith,6Wraith,8Creature);", "empty node: base function, then si");
    calls.clear();
    expect(__tf6Wraith() == __ti6Wraith && calls.empty(), "set node makes no calls");
    for (int byte = 0; byte < 4; ++byte) {
        std::memset(__ti6Wraith, 0, 12); __ti6Wraith[byte] = 1; calls.clear();
        expect(__tf6Wraith() == __ti6Wraith && calls.empty(), "any nonzero byte of the first word skips");
    }
    std::memset(__ti6Wraith, 0, 12); __ti6Wraith[4] = 9; __ti6Wraith[11] = 4; calls.clear();
    __tf6Wraith();
    expect(!calls.empty(), "only the first word gates initialization");
    if (!failures) std::puts("tf6Wraith ok");
    return failures ? 1 : 0;
}
