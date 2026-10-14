// Host check only; not an EE build input.
#include "typeinfo.cpp"
#include <cstdio>
#include <cstring>
#include <string>

unsigned char __ti10GameObject[12];
unsigned char __ti9WorldPart[12];
static std::string calls;
extern "C" void* __tf10GameObject() { calls += "__tf10GameObject;"; return nullptr; }
static const char* node_name(const void* p) {
    if (p == __ti10DoorSecret) return "10DoorSecret";
    if (p == __ti9WorldPart) return "9WorldPart";
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
    std::memset(__ti10DoorSecret, 0, 12);
    std::memset(__ti9WorldPart, 0, 12);
    calls.clear();
}
int main() {
    clear_nodes();
    void* r = __tf10DoorSecret();
    expect(r == __ti10DoorSecret, "returns the outer node");
    expect(calls == "__tf10GameObject;si(9WorldPart,9WorldPart,10GameObject);si(10DoorSecret,10DoorSecret,9WorldPart);", "all levels empty: base call, then each level from the innermost out");
    calls.clear();
    expect(__tf10DoorSecret() == __ti10DoorSecret && calls.empty(), "set node makes no calls");
    for (int byte = 0; byte < 4; ++byte) {
        clear_nodes();
        __ti10DoorSecret[byte] = 1;
        expect(__tf10DoorSecret() == __ti10DoorSecret && calls.empty(), "any nonzero byte of the first word skips");
    }
    clear_nodes();
    __ti10DoorSecret[4] = 9; __ti10DoorSecret[11] = 4;
    __tf10DoorSecret();
    expect(!calls.empty(), "only the first word gates initialization");
    clear_nodes();
    __ti9WorldPart[0] = 1;
    __tf10DoorSecret();
    expect(calls == "si(10DoorSecret,10DoorSecret,9WorldPart);", "inner level 9WorldPart already set");
    if (!failures) std::puts("tf10DoorSecret ok");
    return failures ? 1 : 0;
}
