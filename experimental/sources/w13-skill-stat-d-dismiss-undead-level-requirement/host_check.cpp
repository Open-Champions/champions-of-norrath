#include <climits>
struct SkillDismissUndead;
extern "C" int get_level_requirement(const SkillDismissUndead*, int) asm("getLevelRequirement__18SkillDismissUndeadi");
int main() {
    const SkillDismissUndead* p = reinterpret_cast<const SkillDismissUndead*>(static_cast<unsigned long>(0x1234));
    const int v[] = {INT_MIN, -1, 0, 1, 2, 50, INT_MAX};
    for (int n : v) if (get_level_requirement(p, n) != 25) return 1;
    return 0;
}
