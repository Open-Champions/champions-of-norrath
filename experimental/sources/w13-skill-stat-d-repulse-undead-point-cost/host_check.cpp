#include <climits>
struct SkillRepulseUndead;
extern "C" int get_skill_point_cost(const SkillRepulseUndead*, int) asm("getSkillPointCost__18SkillRepulseUndeadi");
int main() {
    const SkillRepulseUndead* p = reinterpret_cast<const SkillRepulseUndead*>(static_cast<unsigned long>(0x1234));
    const int v[] = {INT_MIN, -1, 0, 1, 2, 50, INT_MAX};
    for (int n : v) if (get_skill_point_cost(p, n) != 1) return 1;
    return 0;
}
