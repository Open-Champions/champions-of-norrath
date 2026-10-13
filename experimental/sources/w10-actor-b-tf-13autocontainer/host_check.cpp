// Host check only. Not an EE build input.
#include "typeinfo.cpp"
#include <cstdio>
#include <cstring>
#include <string>

unsigned char __ti13AutoContainer[12];
unsigned char __ti10AutoObject[12];
unsigned char __ti10GameObject[12];
static std::string calls;
extern "C" void* __tf10GameObject() { calls += "tf10GameObject;"; return nullptr; }
static const char* node_name(const void* p) {
    if (p == __ti13AutoContainer) return "13AutoContainer";
    if (p == __ti10AutoObject) return "10AutoObject";
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
    std::memset(__ti13AutoContainer, 0, 12);
    std::memset(__ti10AutoObject, 0, 12);
    calls.clear();
}
int main() {
    clear_nodes();
    expect(__tf13AutoContainer() == __ti13AutoContainer, "returns the outer node");
    expect(calls == "tf10GameObject;si(10AutoObject,10AutoObject,10GameObject);si(13AutoContainer,13AutoContainer,10AutoObject);", "all levels empty: root call, then each level innermost first");
    calls.clear();
    expect(__tf13AutoContainer() == __ti13AutoContainer && calls.empty(), "set node makes no calls");
    for (int byte = 0; byte < 4; ++byte) {
        clear_nodes();
        __ti13AutoContainer[byte] = 1;
        expect(__tf13AutoContainer() == __ti13AutoContainer && calls.empty(), "any nonzero byte of the first word skips");
    }
    clear_nodes();
    __ti13AutoContainer[4] = 9; __ti13AutoContainer[11] = 4;
    __tf13AutoContainer();
    expect(!calls.empty(), "only the first word gates initialization");
    clear_nodes();
    __ti10AutoObject[0] = 1;
    __tf13AutoContainer();
    expect(calls == "si(13AutoContainer,13AutoContainer,10AutoObject);", "set inner level skips its work and the root call");
    return failures ? 1 : 0;
}
