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
    if (p == __ti8RollDoor) return "8RollDoor";
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
    std::memset(__ti8RollDoor, 0, 12);
    std::memset(__ti9WorldPart, 0, 12);
    calls.clear();
}
int main() {
    clear_nodes();
    void* r = __tf8RollDoor();
    expect(r == __ti8RollDoor, "returns the outer node");
    expect(calls == "__tf10GameObject;si(9WorldPart,9WorldPart,10GameObject);si(8RollDoor,8RollDoor,9WorldPart);", "all levels empty: base call, then each level from the innermost out");
    calls.clear();
    expect(__tf8RollDoor() == __ti8RollDoor && calls.empty(), "set node makes no calls");
    for (int byte = 0; byte < 4; ++byte) {
        clear_nodes();
        __ti8RollDoor[byte] = 1;
        expect(__tf8RollDoor() == __ti8RollDoor && calls.empty(), "any nonzero byte of the first word skips");
    }
    clear_nodes();
    __ti8RollDoor[4] = 9; __ti8RollDoor[11] = 4;
    __tf8RollDoor();
    expect(!calls.empty(), "only the first word gates initialization");
    clear_nodes();
    __ti9WorldPart[0] = 1;
    __tf8RollDoor();
    expect(calls == "si(8RollDoor,8RollDoor,9WorldPart);", "inner level 9WorldPart already set");
    if (!failures) std::puts("tf8RollDoor ok");
    return failures ? 1 : 0;
}
