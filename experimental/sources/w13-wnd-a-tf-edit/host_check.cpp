// Host check only. Not an EE build input.
#include "typeinfo.cpp"
#include <cstdio>
#include <cstring>
#include <string>

unsigned char __ti5VIWnd[12];
static std::string calls;
extern "C" void* __tf5VIWnd() { calls += "tf5VIWnd;"; return nullptr; }
static const char* node_name(const void* p) {
    if (p == __ti9VIWndEdit) return "9VIWndEdit";
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
    std::memset(__ti9VIWndEdit, 0, 12);
    calls.clear();
}
int main() {
    clear_node();
    expect(__tf9VIWndEdit() == __ti9VIWndEdit, "returns the node");
    expect(calls == "tf5VIWnd;si(9VIWndEdit,9VIWndEdit,5VIWnd);", "empty node: base call, then registration");
    calls.clear();
    expect(__tf9VIWndEdit() == __ti9VIWndEdit && calls.empty(), "set node makes no calls");
    for (int byte = 0; byte < 4; ++byte) {
        clear_node();
        __ti9VIWndEdit[byte] = 1;
        expect(__tf9VIWndEdit() == __ti9VIWndEdit && calls.empty(), "any nonzero byte of the first word skips");
    }
    clear_node();
    __ti9VIWndEdit[4] = 9; __ti9VIWndEdit[11] = 4;
    __tf9VIWndEdit();
    expect(!calls.empty(), "only the first word gates initialization");
    return failures ? 1 : 0;
}
