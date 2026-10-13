// Host check only; not an EE build input.
#include "typeinfo.cpp"
#include <cstdio>
#include <cstring>
#include <string>

unsigned char __ti10GameObject[12];
static std::string calls;
static const char* node_name(const void* p) {
    if (p == __ti4Lamp) return "4Lamp";
    if (p == __ti10GameObject) return "10GameObject";
    return "?";
}
void* __tf10GameObject() { calls += "__tf10GameObject;"; return nullptr; }
void __rtti_si(void* t, const char* name, const void* b) {
    calls += std::string("si(") + node_name(t) + "," + name + "," + node_name(b) + ");";
    static_cast<unsigned char*>(t)[0] = 1;
}
static int failures;
static void expect(bool ok, const char* what) { if (!ok) { std::fprintf(stderr, "fail: %s (calls %s)\n", what, calls.c_str()); ++failures; } }
int main() {
    std::memset(__ti4Lamp, 0, 12); calls.clear();
    expect(__tf4Lamp() == __ti4Lamp, "returns the node");
    expect(calls == "__tf10GameObject;si(4Lamp,4Lamp,10GameObject);", "empty node: base function, then si");
    calls.clear();
    expect(__tf4Lamp() == __ti4Lamp && calls.empty(), "set node makes no calls");
    for (int byte = 0; byte < 4; ++byte) {
        std::memset(__ti4Lamp, 0, 12); __ti4Lamp[byte] = 1; calls.clear();
        expect(__tf4Lamp() == __ti4Lamp && calls.empty(), "any nonzero byte of the first word skips");
    }
    std::memset(__ti4Lamp, 0, 12); __ti4Lamp[4] = 9; __ti4Lamp[11] = 4; calls.clear();
    __tf4Lamp();
    expect(!calls.empty(), "only the first word gates initialization");
    if (!failures) std::puts("tf4Lamp ok");
    return failures ? 1 : 0;
}
