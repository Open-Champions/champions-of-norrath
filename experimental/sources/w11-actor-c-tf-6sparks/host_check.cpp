// Host contract for __tf6Sparks; not an EE input. Includes the source.
#include "typeinfo.cpp"
#include <cstdio>
#include <cstring>
#include <string>

unsigned char __ti10Projectile[12];
unsigned char __ti10GameObject[8];
static std::string calls;
static const char* node_name(const void* p) {
    if (p == __ti6Sparks) return "6Sparks";
    if (p == __ti10Projectile) return "10Projectile";
    if (p == __ti10GameObject) return "10GameObject";
    return "?";
}
void* __tf10GameObject() { calls += "__tf10GameObject;"; return nullptr; }
void __rtti_si(void* t, const char* name, const void* b) {
    calls += std::string("si(") + node_name(t) + "," + name + "," + node_name(b) + ");";
    static_cast<unsigned char*>(t)[0] = 1;
}
static int failures;
static void expect(bool ok, const char* what) { if (!ok) { std::fprintf(stderr, "fail: %s (calls %s)\n", what, calls.c_str()); ++failures; } }
int main() {
    std::memset(__ti6Sparks, 0, 12); std::memset(__ti10Projectile, 0, 12); calls.clear();
    expect(__tf6Sparks() == __ti6Sparks, "returns the node");
    expect(calls == "__tf10GameObject;si(10Projectile,10Projectile,10GameObject);si(6Sparks,6Sparks,10Projectile);", "both empty");
    calls.clear();
    expect(__tf6Sparks() == __ti6Sparks && calls.empty(), "set node makes no calls");
    std::memset(__ti6Sparks, 0, 12); calls.clear();   // Projectile node already set up
    __tf6Sparks();
    expect(calls == "si(6Sparks,6Sparks,10Projectile);", "base already set: only the Sparks call");
    for (int byte = 0; byte < 4; ++byte) {
        std::memset(__ti6Sparks, 0, 12); __ti6Sparks[byte] = 1; calls.clear();
        expect(__tf6Sparks() == __ti6Sparks && calls.empty(), "any nonzero byte of the first word skips");
    }
    std::memset(__ti6Sparks, 0, 12); std::memset(__ti10Projectile, 0, 12);
    __ti6Sparks[4] = 9; __ti10Projectile[8] = 7; calls.clear();
    __tf6Sparks();
    expect(calls.find("__tf10GameObject") != std::string::npos, "only the first word of each node gates");
    if (!failures) std::puts("tf6Sparks ok");
    return failures ? 1 : 0;
}
