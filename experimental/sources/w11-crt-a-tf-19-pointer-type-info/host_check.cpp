// Host check only; not an EE build input.
#include "typeinfo.cpp"
#include <cstdio>
#include <cstring>
#include <string>

unsigned char __ti9type_info[12];
static std::string calls;
extern "C" void* __tf9type_info() { calls += "__tf9type_info;"; return nullptr; }
static const char* node_name(const void* p) {
    if (p == __ti19__pointer_type_info) return "19__pointer_type_info";
    if (p == __ti9type_info) return "9type_info";
    return "?";
}
extern "C" void __rtti_si(void* t, const char* name, const void* b) {
    calls += std::string("si(") + node_name(t) + "," + name + "," + node_name(b) + ");";
    static_cast<unsigned char*>(t)[0] = 1;  // the real call stores a name pointer
}
static int failures;
static void expect(bool ok, const char* what) { if (!ok) { std::fprintf(stderr, "fail: %s (calls %s)\n", what, calls.c_str()); ++failures; } }
int main() {
    std::memset(__ti19__pointer_type_info, 0, 12);
    void* r = __tf19__pointer_type_info();
    expect(r == __ti19__pointer_type_info, "returns the node");
    expect(calls == "__tf9type_info;si(19__pointer_type_info,19__pointer_type_info,9type_info);", "empty node: base call, then registration");
    calls.clear();
    expect(__tf19__pointer_type_info() == __ti19__pointer_type_info && calls.empty(), "set node makes no calls");
    for (int byte = 0; byte < 4; ++byte) {
        std::memset(__ti19__pointer_type_info, 0, 12);
        calls.clear();
        __ti19__pointer_type_info[byte] = 1;
        expect(__tf19__pointer_type_info() == __ti19__pointer_type_info && calls.empty(), "any nonzero byte of the first word skips");
    }
    std::memset(__ti19__pointer_type_info, 0, 12);
    __ti19__pointer_type_info[4] = 9; __ti19__pointer_type_info[11] = 4; calls.clear();
    __tf19__pointer_type_info();
    expect(!calls.empty(), "only the first word gates initialization");
    if (!failures) std::puts("tf19__pointer_type_info ok");
    return failures ? 1 : 0;
}
