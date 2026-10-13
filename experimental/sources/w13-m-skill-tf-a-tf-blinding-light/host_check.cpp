// Host check only; not an EE build input.
#include "typeinfo.cpp"
#include <cstdio>
#include <cstring>
#include <string>

unsigned char __ti5Skill[8];
static std::string calls;
static const char* node_name(const void* p) {
    if (p == __ti18SkillBlindingLight) return "18SkillBlindingLight";
    if (p == __ti5Skill) return "5Skill";
    return "?";
}
extern "C" void __rtti_si(void* t, const char* name, const void* b) {
    calls += std::string("si(") + node_name(t) + "," + name + "," + node_name(b) + ");";
    static_cast<unsigned char*>(t)[0] = 1;
}
extern "C" void* __tf5Skill() {
    calls += "tf5Skill();";
    return __ti5Skill;
}
static int failures;
static void expect(bool ok, const char* what) { if (!ok) { std::fprintf(stderr, "fail: %s (calls %s)\n", what, calls.c_str()); ++failures; } }
int main() {
    std::memset(__ti18SkillBlindingLight, 0, 12); std::memset(__ti5Skill, 0, 8); calls.clear();
    expect(__tf18SkillBlindingLight() == __ti18SkillBlindingLight, "returns the node");
    expect(calls == "tf5Skill();si(18SkillBlindingLight,18SkillBlindingLight,5Skill);", "empty node: __tf5Skill is called, then derived si");
    calls.clear();
    expect(__tf18SkillBlindingLight() == __ti18SkillBlindingLight && calls.empty(), "set node makes no calls");
    std::memset(__ti18SkillBlindingLight, 0, 12); calls.clear();
    __ti5Skill[0] = 1;
    __tf18SkillBlindingLight();
    expect(calls == "tf5Skill();si(18SkillBlindingLight,18SkillBlindingLight,5Skill);", "base node state: __tf5Skill still runs");
    for (int byte = 0; byte < 4; ++byte) {
        std::memset(__ti18SkillBlindingLight, 0, 12); __ti18SkillBlindingLight[byte] = 1; calls.clear();
        expect(__tf18SkillBlindingLight() == __ti18SkillBlindingLight && calls.empty(), "any nonzero byte of the first word skips");
    }
    std::memset(__ti18SkillBlindingLight, 0, 12); __ti18SkillBlindingLight[4] = 1; __ti18SkillBlindingLight[11] = 1; calls.clear();
    __tf18SkillBlindingLight();
    expect(!calls.empty(), "nonzero bytes outside the first word do not skip");
    if (!failures) std::puts("tf18SkillBlindingLight ok");
    return failures ? 1 : 0;
}
