// Host check only. Not an EE build input.
#include "typeinfo.cpp"
#include <cstdio>
#include <cstring>
#include <string>

unsigned char __ti5VIWnd[12];
static std::string calls;
extern "C" void* __tf5VIWnd() { calls += "tf5VIWnd;"; return nullptr; }
static const char* node_name(const void* p) {
    if (p == __ti12VIWndOptions) return "12VIWndOptions";
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
    std::memset(__ti12VIWndOptions, 0, 12);
    calls.clear();
}
int main() {
    clear_node();
    expect(__tf12VIWndOptions() == __ti12VIWndOptions, "returns the node");
    expect(calls == "tf5VIWnd;si(12VIWndOptions,12VIWndOptions,5VIWnd);", "empty node: base call, then registration");
    calls.clear();
    expect(__tf12VIWndOptions() == __ti12VIWndOptions && calls.empty(), "set node makes no calls");
    for (int byte = 0; byte < 4; ++byte) {
        clear_node();
        __ti12VIWndOptions[byte] = 1;
        expect(__tf12VIWndOptions() == __ti12VIWndOptions && calls.empty(), "any nonzero byte of the first word skips");
    }
    clear_node();
    __ti12VIWndOptions[4] = 9; __ti12VIWndOptions[11] = 4;
    __tf12VIWndOptions();
    expect(!calls.empty(), "only the first word gates initialization");
    return failures ? 1 : 0;
}
