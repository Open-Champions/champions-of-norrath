// Host check only; not an EE build input.
extern "C" { extern unsigned char __ti11AddParticle[12]; void* __tf11AddParticle(); }
#include <cstdio>
#include <cstring>
#include <string>

unsigned char __ti10GameObject[12];
static std::string calls;
extern "C" void* __tf10GameObject() { calls += "__tf10GameObject;"; return nullptr; }
static const char* node_name(const void* p) {
    if (p == __ti11AddParticle) return "11AddParticle";
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
    std::memset(__ti11AddParticle, 0, 12);
    calls.clear();
}
int main() {
    clear_nodes();
    void* r = __tf11AddParticle();
    expect(r == __ti11AddParticle, "returns the outer node");
    expect(calls == "__tf10GameObject;si(11AddParticle,11AddParticle,10GameObject);", "all levels empty: base call, then each level from the innermost out");
    calls.clear();
    expect(__tf11AddParticle() == __ti11AddParticle && calls.empty(), "set node makes no calls");
    for (int byte = 0; byte < 4; ++byte) {
        clear_nodes();
        __ti11AddParticle[byte] = 1;
        expect(__tf11AddParticle() == __ti11AddParticle && calls.empty(), "any nonzero byte of the first word skips");
    }
    clear_nodes();
    __ti11AddParticle[4] = 9; __ti11AddParticle[11] = 4;
    __tf11AddParticle();
    expect(!calls.empty(), "only the first word gates initialization");
    if (!failures) std::puts("tf11AddParticle ok");
    return failures ? 1 : 0;
}
