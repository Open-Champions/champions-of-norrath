// Host check only; not an EE build input.
#include "typeinfo.cpp"
#include <cstdio>
#include <cstring>
#include <string>

unsigned char __ti10GameObject[12];
static std::string calls;
extern "C" void* __tf10GameObject() { calls += "__tf10GameObject;"; return nullptr; }
static const char* node_name(const void* p) {
    if (p == __ti7Candle2) return "7Candle2";
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
    std::memset(__ti7Candle2, 0, 12);
    calls.clear();
}
int main() {
    clear_nodes();
    void* r = __tf7Candle2();
    expect(r == __ti7Candle2, "returns the outer node");
    expect(calls == "__tf10GameObject;si(7Candle2,7Candle2,10GameObject);", "all levels empty: base call, then each level from the innermost out");
    calls.clear();
    expect(__tf7Candle2() == __ti7Candle2 && calls.empty(), "set node makes no calls");
    for (int byte = 0; byte < 4; ++byte) {
        clear_nodes();
        __ti7Candle2[byte] = 1;
        expect(__tf7Candle2() == __ti7Candle2 && calls.empty(), "any nonzero byte of the first word skips");
    }
    clear_nodes();
    __ti7Candle2[4] = 9; __ti7Candle2[11] = 4;
    __tf7Candle2();
    expect(!calls.empty(), "only the first word gates initialization");
    if (!failures) std::puts("tf7Candle2 ok");
    return failures ? 1 : 0;
}
