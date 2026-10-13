#include <climits>
struct SkillHammerOfWrath;
extern "C" int get_skill_point_cost(const SkillHammerOfWrath*, int) asm("getSkillPointCost__18SkillHammerOfWrathi");
int main() {
    const SkillHammerOfWrath* p = reinterpret_cast<const SkillHammerOfWrath*>(static_cast<unsigned long>(0x1234));
    const int v[] = {INT_MIN, -1, 0, 1, 2, 50, INT_MAX};
    for (int n : v) if (get_skill_point_cost(p, n) != 1) return 1;
    return 0;
}
