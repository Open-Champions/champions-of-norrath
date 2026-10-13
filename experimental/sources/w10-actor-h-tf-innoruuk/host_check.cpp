// Host check only; not an EE build input.
#include "typeinfo.cpp"
#include <cstdio>
#include <cstring>
#include <string>

unsigned char __ti8Creature[12];
static std::string calls;
static const char* node_name(const void* p) {
    if (p == __ti8Innoruuk) return "8Innoruuk";
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
    std::memset(__ti8Innoruuk, 0, 12); calls.clear();
    expect(__tf8Innoruuk() == __ti8Innoruuk, "returns the node");
    expect(calls == "__tf8Creature;si(8Innoruuk,8Innoruuk,8Creature);", "empty node: base function, then si");
    calls.clear();
    expect(__tf8Innoruuk() == __ti8Innoruuk && calls.empty(), "set node makes no calls");
    for (int byte = 0; byte < 4; ++byte) {
        std::memset(__ti8Innoruuk, 0, 12); __ti8Innoruuk[byte] = 1; calls.clear();
        expect(__tf8Innoruuk() == __ti8Innoruuk && calls.empty(), "any nonzero byte of the first word skips");
    }
    std::memset(__ti8Innoruuk, 0, 12); __ti8Innoruuk[4] = 9; __ti8Innoruuk[11] = 4; calls.clear();
    __tf8Innoruuk();
    expect(!calls.empty(), "only the first word gates initialization");
    if (!failures) std::puts("tf8Innoruuk ok");
    return failures ? 1 : 0;
}
