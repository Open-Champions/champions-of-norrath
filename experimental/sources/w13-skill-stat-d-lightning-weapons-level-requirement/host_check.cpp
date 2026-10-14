#include <climits>
struct SkillLightningWeapons;
extern "C" int get_level_requirement(const SkillLightningWeapons*, int) asm("getLevelRequirement__21SkillLightningWeaponsi");
int main() {
    const SkillLightningWeapons* p = reinterpret_cast<const SkillLightningWeapons*>(static_cast<unsigned long>(0x1234));
    const int v[] = {INT_MIN, -1, 0, 1, 2, 50, INT_MAX};
    for (int n : v) if (get_level_requirement(p, n) != 12) return 1;
    return 0;
}
