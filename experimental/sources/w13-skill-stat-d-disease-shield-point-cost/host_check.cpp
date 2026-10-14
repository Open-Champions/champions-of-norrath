#include <climits>
struct SkillDiseaseShield;
extern "C" int get_skill_point_cost(const SkillDiseaseShield*, int) asm("getSkillPointCost__18SkillDiseaseShieldi");
int main() {
    const SkillDiseaseShield* p = reinterpret_cast<const SkillDiseaseShield*>(static_cast<unsigned long>(0x1234));
    const int v[] = {INT_MIN, -1, 0, 1, 2, 50, INT_MAX};
    for (int n : v) if (get_skill_point_cost(p, n) != 1) return 1;
    return 0;
}
