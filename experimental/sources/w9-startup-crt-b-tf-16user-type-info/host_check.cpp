// Host check only; not an EE build input.
#include "typeinfo.cpp"
#include <cstdio>
#include <cstring>
#include <string>

unsigned char __ti9type_info[12];
static std::string calls;
extern "C" void* __tf9type_info() { calls += "__tf9type_info;"; return nullptr; }
static const char* node_name(const void* p) {
    if (p == __ti16__user_type_info) return "16__user_type_info";
    if (p == __ti9type_info) return "9type_info";
    return "?";
}
extern "C" void __rtti_si(void* t, const char* name, const void* b) {
    calls += std::string("si(") + node_name(t) + "," + name + "," + node_name(b) + ");";
    static_cast<unsigned char*>(t)[0] = 1;  // the real call stores a name pointer
}
static int failures;
static void expect(bool ok, const char* what) { if (!ok) { std::fprintf(stderr, "fail: %s (calls %s)\n", what, calls.c_str()); ++failures; } }
static void clear_nodes() {
    std::memset(__ti16__user_type_info, 0, 12);
    calls.clear();
}
int main() {
    clear_nodes();
    void* r = __tf16__user_type_info();
    expect(r == __ti16__user_type_info, "returns the outer node");
    expect(calls == "__tf9type_info;si(16__user_type_info,16__user_type_info,9type_info);", "all levels empty: base call, then each level from the innermost out");
    calls.clear();
    expect(__tf16__user_type_info() == __ti16__user_type_info && calls.empty(), "set node makes no calls");
    for (int byte = 0; byte < 4; ++byte) {
        clear_nodes();
        __ti16__user_type_info[byte] = 1;
        expect(__tf16__user_type_info() == __ti16__user_type_info && calls.empty(), "any nonzero byte of the first word skips");
    }
    clear_nodes();
    __ti16__user_type_info[4] = 9; __ti16__user_type_info[11] = 4;
    __tf16__user_type_info();
    expect(!calls.empty(), "only the first word gates initialization");
    if (!failures) std::puts("tf16__user_type_info ok");
    return failures ? 1 : 0;
}
