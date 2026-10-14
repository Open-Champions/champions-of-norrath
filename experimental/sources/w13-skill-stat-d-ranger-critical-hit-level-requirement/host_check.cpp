#include <climits>
struct SkillRangerCriticalHit;
extern "C" int get_level_requirement(const SkillRangerCriticalHit*, int) asm("getLevelRequirement__22SkillRangerCriticalHiti");
int main() {
    const SkillRangerCriticalHit* p = reinterpret_cast<const SkillRangerCriticalHit*>(static_cast<unsigned long>(0x1234));
    const int v[] = {INT_MIN, -1, 0, 1, 2, 50, INT_MAX};
    for (int n : v) if (get_level_requirement(p, n) != 10) return 1;
    return 0;
}
