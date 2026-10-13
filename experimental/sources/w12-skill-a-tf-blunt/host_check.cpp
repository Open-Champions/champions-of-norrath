// Host check only; not an EE build input.
#include "typeinfo.cpp"
#include <cstdio>
#include <cstring>
#include <string>

unsigned char __ti5Skill[8];
static std::string calls;
static const char* node_name(const void* p) {
    if (p == __ti10SkillBlunt) return "10SkillBlunt";
    if (p == __ti5Skill) return "5Skill";
    return "?";
}
extern "C" void __rtti_user(void* t, const char* name) {
    calls += std::string("user(") + node_name(t) + "," + name + ");";
    static_cast<unsigned char*>(t)[0] = 1;
}
extern "C" void __rtti_si(void* t, const char* name, const void* b) {
    calls += std::string("si(") + node_name(t) + "," + name + "," + node_name(b) + ");";
    static_cast<unsigned char*>(t)[0] = 1;
}
static int failures;
static void expect(bool ok, const char* what) { if (!ok) { std::fprintf(stderr, "fail: %s (calls %s)\n", what, calls.c_str()); ++failures; } }
int main() {
    std::memset(__ti10SkillBlunt, 0, 12); std::memset(__ti5Skill, 0, 8); calls.clear();
    expect(__tf10SkillBlunt() == __ti10SkillBlunt, "returns the node");
    expect(calls == "user(5Skill,5Skill);si(10SkillBlunt,10SkillBlunt,5Skill);", "both empty: base user init, then derived si");
    calls.clear();
    expect(__tf10SkillBlunt() == __ti10SkillBlunt && calls.empty(), "set node makes no calls");
    std::memset(__ti10SkillBlunt, 0, 12); calls.clear();
    __ti5Skill[0] = 1;
    __tf10SkillBlunt();
    expect(calls == "si(10SkillBlunt,10SkillBlunt,5Skill);", "base node state: set base node skips the user init");
    for (int byte = 0; byte < 4; ++byte) {
        std::memset(__ti10SkillBlunt, 0, 12); __ti10SkillBlunt[byte] = 1; calls.clear();
        expect(__tf10SkillBlunt() == __ti10SkillBlunt && calls.empty(), "any nonzero byte of the first word skips");
    }
    std::memset(__ti10SkillBlunt, 0, 12); __ti10SkillBlunt[4] = 1; __ti10SkillBlunt[11] = 1; calls.clear();
    __tf10SkillBlunt();
    expect(!calls.empty(), "nonzero bytes outside the first word do not skip");
    if (!failures) std::puts("tf10SkillBlunt ok");
    return failures ? 1 : 0;
}
