// Host check only. Not an EE build input.
#include "typeinfo.cpp"
#include <cstdio>
#include <cstring>
#include <string>

unsigned char __ti5VIWnd[12];
static std::string calls;
extern "C" void* __tf5VIWnd() { calls += "tf5VIWnd;"; return nullptr; }
static const char* node_name(const void* p) {
    if (p == __ti9VIWndEula) return "9VIWndEula";
    if (p == __ti5VIWnd) return "5VIWnd";
    return "?";
}
extern "C" void __rtti_si(void* t, const char* name, const void* b) {
    calls += std::string("si(") + node_name(t) + "," + name + "," + node_name(b) + ");";
    static_cast<unsigned char*>(t)[0] = 1;  // the real call stores a name pointer
}
static int failures;
static void expect(bool ok, const char* what) { if (!ok) { std::fprintf(stderr, "fail: %s (calls %s)\n", what, calls.c_str()); ++failures; } }
static void clear_node() {
    std::memset(__ti9VIWndEula, 0, 12);
    calls.clear();
}
int main() {
    clear_node();
    expect(__tf9VIWndEula() == __ti9VIWndEula, "returns the node");
    expect(calls == "tf5VIWnd;si(9VIWndEula,9VIWndEula,5VIWnd);", "empty node: base call, then registration");
    calls.clear();
    expect(__tf9VIWndEula() == __ti9VIWndEula && calls.empty(), "set node makes no calls");
    for (int byte = 0; byte < 4; ++byte) {
        clear_node();
        __ti9VIWndEula[byte] = 1;
        expect(__tf9VIWndEula() == __ti9VIWndEula && calls.empty(), "any nonzero byte of the first word skips");
    }
    clear_node();
    __ti9VIWndEula[4] = 9; __ti9VIWndEula[11] = 4;
    __tf9VIWndEula();
    expect(!calls.empty(), "only the first word gates initialization");
    return failures ? 1 : 0;
}
