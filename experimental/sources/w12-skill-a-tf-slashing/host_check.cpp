// Host check only; not an EE build input.
#include "typeinfo.cpp"
#include <cstdio>
#include <cstring>
#include <string>

unsigned char __ti5Skill[8];
static std::string calls;
static const char* node_name(const void* p) {
    if (p == __ti13SkillSlashing) return "13SkillSlashing";
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
    std::memset(__ti13SkillSlashing, 0, 12); std::memset(__ti5Skill, 0, 8); calls.clear();
    expect(__tf13SkillSlashing() == __ti13SkillSlashing, "returns the node");
    expect(calls == "user(5Skill,5Skill);si(13SkillSlashing,13SkillSlashing,5Skill);", "both empty: base user init, then derived si");
    calls.clear();
    expect(__tf13SkillSlashing() == __ti13SkillSlashing && calls.empty(), "set node makes no calls");
    std::memset(__ti13SkillSlashing, 0, 12); calls.clear();
    __ti5Skill[0] = 1;
    __tf13SkillSlashing();
    expect(calls == "si(13SkillSlashing,13SkillSlashing,5Skill);", "base node state: set base node skips the user init");
    for (int byte = 0; byte < 4; ++byte) {
        std::memset(__ti13SkillSlashing, 0, 12); __ti13SkillSlashing[byte] = 1; calls.clear();
        expect(__tf13SkillSlashing() == __ti13SkillSlashing && calls.empty(), "any nonzero byte of the first word skips");
    }
    std::memset(__ti13SkillSlashing, 0, 12); __ti13SkillSlashing[4] = 1; __ti13SkillSlashing[11] = 1; calls.clear();
    __tf13SkillSlashing();
    expect(!calls.empty(), "nonzero bytes outside the first word do not skip");
    if (!failures) std::puts("tf13SkillSlashing ok");
    return failures ? 1 : 0;
}
