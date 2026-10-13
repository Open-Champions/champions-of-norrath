// Host check only; not an EE build input.
//   c++ -std=c++17 -o check host_check.cpp && ./check
#include "typeinfo.cpp"
#include <cstdio>
#include <cstring>
#include <string>
static std::string calls;
extern "C" void __rtti_user(void* t, const char* name) {
    calls += std::string("user(") + (t == __ti5Skill ? "node" : "?") + "," + name + ");";
    static_cast<unsigned char*>(t)[0] = 1;  // the real call stores a name pointer
}
static int failures;
static void expect(bool ok, const char* what) { if (!ok) { std::fprintf(stderr, "fail: %s (calls %s)\n", what, calls.c_str()); ++failures; } }
int main() {
    std::memset(__ti5Skill, 0, 8);
    expect(__tf5Skill() == __ti5Skill, "returns the node");
    expect(calls == "user(node,5Skill);", "empty node registers the name");
    calls.clear();
    expect(__tf5Skill() == __ti5Skill && calls.empty(), "set node makes no calls");
    for (int byte = 0; byte < 4; ++byte) {
        std::memset(__ti5Skill, 0, 8); calls.clear();
        __ti5Skill[byte] = 1;
        expect(__tf5Skill() == __ti5Skill && calls.empty(), "any nonzero byte of the first word skips");
    }
    std::memset(__ti5Skill, 0, 8); __ti5Skill[4] = 9; calls.clear();
    __tf5Skill();
    expect(!calls.empty(), "only the first word gates initialization");
    if (!failures) std::puts("tf5Skill ok");
    return failures ? 1 : 0;
}
