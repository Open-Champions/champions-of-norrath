// Host check only; not an EE build input.
#include "typeinfo.cpp"
#include <cstdio>
#include <cstring>
#include <string>

unsigned char __ti10GameObject[12];
static std::string calls;
extern "C" void* __tf10GameObject() { calls += "__tf10GameObject;"; return nullptr; }
static const char* node_name(const void* p) {
    if (p == __ti6Candle) return "6Candle";
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
    std::memset(__ti6Candle, 0, 12);
    calls.clear();
}
int main() {
    clear_nodes();
    void* r = __tf6Candle();
    expect(r == __ti6Candle, "returns the outer node");
    expect(calls == "__tf10GameObject;si(6Candle,6Candle,10GameObject);", "all levels empty: base call, then each level from the innermost out");
    calls.clear();
    expect(__tf6Candle() == __ti6Candle && calls.empty(), "set node makes no calls");
    for (int byte = 0; byte < 4; ++byte) {
        clear_nodes();
        __ti6Candle[byte] = 1;
        expect(__tf6Candle() == __ti6Candle && calls.empty(), "any nonzero byte of the first word skips");
    }
    clear_nodes();
    __ti6Candle[4] = 9; __ti6Candle[11] = 4;
    __tf6Candle();
    expect(!calls.empty(), "only the first word gates initialization");
    if (!failures) std::puts("tf6Candle ok");
    return failures ? 1 : 0;
}
