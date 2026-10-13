// Host contract for the five skill tree tables. Not an EE plan input.
#include "skill_trees.cpp"
#include <cstring>
static int endIndex(const SkillTreeEntry* t) {
    int i = 0;
    while (std::strcmp(t[i].name, "END") != 0) ++i;
    return i;
}
int main() {
    if (sizeof(WarriorSkillTree) != 728 || endIndex(WarriorSkillTree) != 12) return 1;
    if (sizeof(ClericSkillTree) != 504 || endIndex(ClericSkillTree) != 8) return 2;
    if (sizeof(WizardSkillTree) != 504 || endIndex(WizardSkillTree) != 8) return 3;
    if (sizeof(RangerSkillTree) != 336 || endIndex(RangerSkillTree) != 5) return 4;
    if (sizeof(ShadowKnightSkillTree) != 448 || endIndex(ShadowKnightSkillTree) != 7) return 5;
    if (std::strcmp(WarriorSkillTree[7].name, "bullrush") != 0 || WarriorSkillTree[7].f36 != 5.0f ||
        WarriorSkillTree[7].b34 != 3 || WarriorSkillTree[7].q40 != 8) return 6;
    if (WizardSkillTree[1].b34 != 16 || WizardSkillTree[1].q40 != 2048) return 7;
    for (int i = 0; i < 13; ++i) {
        const long long v = WarriorSkillTree[i].q40;
        if ((v & (v - 1)) != 0) return 8;       // zero or a single bit
    }
    return 0;
}
