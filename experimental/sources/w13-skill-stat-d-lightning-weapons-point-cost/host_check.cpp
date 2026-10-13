#include <climits>
struct SkillLightningWeapons;
extern "C" int get_skill_point_cost(const SkillLightningWeapons*, int) asm("getSkillPointCost__21SkillLightningWeaponsi");
int main() {
    const SkillLightningWeapons* p = reinterpret_cast<const SkillLightningWeapons*>(static_cast<unsigned long>(0x1234));
    const int v[] = {INT_MIN, -1, 0, 1, 2, 50, INT_MAX};
    for (int n : v) if (get_skill_point_cost(p, n) != 1) return 1;
    return 0;
}
