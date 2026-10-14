// Host check only; not an EE build input.
#include "typeinfo.cpp"
#include <cstdio>
#include <cstring>

unsigned char __ti10GameObject[8];
static int calls;
static const void* seen_node;
static const char* seen_name;
extern "C" void __rtti_user(void* t, const char* name) {
    ++calls; seen_node = t; seen_name = name;
    static_cast<unsigned char*>(t)[0] = 1;  // the real call stores a name pointer
}
static int failures;
static void expect(bool ok, const char* what) { if (!ok) { std::fprintf(stderr, "fail: %s\n", what); ++failures; } }
int main() {
    expect(__tf10GameObject() == __ti10GameObject, "returns the node");
    expect(calls == 1 && seen_node == __ti10GameObject && std::strcmp(seen_name, "10GameObject") == 0, "empty node runs __rtti_user once");
    expect(__tf10GameObject() == __ti10GameObject && calls == 1, "set node skips");
    for (int byte = 0; byte < 4; ++byte) {
        calls = 0;
        std::memset(__ti10GameObject, 0, 8);
        __ti10GameObject[byte] = 1;
        expect(__tf10GameObject() == __ti10GameObject && calls == 0, "any nonzero byte of the first word skips");
    }
    calls = 0;
    std::memset(__ti10GameObject, 0, 8);
    __ti10GameObject[4] = 7; __ti10GameObject[7] = 7;
    __tf10GameObject();
    expect(calls == 1, "only the first word gates");
    if (!failures) std::puts("tf10GameObject ok");
    return failures ? 1 : 0;
}
