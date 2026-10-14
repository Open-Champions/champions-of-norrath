#include <climits>
struct SkillHammerOfWrath;
extern "C" int get_level_requirement(const SkillHammerOfWrath*, int) asm("getLevelRequirement__18SkillHammerOfWrathi");
int main() {
    const SkillHammerOfWrath* p = reinterpret_cast<const SkillHammerOfWrath*>(static_cast<unsigned long>(0x1234));
    const int v[] = {INT_MIN, -1, 0, 1, 2, 50, INT_MAX};
    for (int n : v) if (get_level_requirement(p, n) != 10) return 1;
    return 0;
}
