#include <climits>
struct SkillSummonSkeleton;
extern "C" int get_level_requirement(const SkillSummonSkeleton*, int) asm("getLevelRequirement__19SkillSummonSkeletoni");
int main() {
    const SkillSummonSkeleton* p = reinterpret_cast<const SkillSummonSkeleton*>(static_cast<unsigned long>(0x1234));
    const int v[] = {INT_MIN, -1, 0, 1, 2, 50, INT_MAX};
    for (int n : v) if (get_level_requirement(p, n) != 15) return 1;
    return 0;
}
