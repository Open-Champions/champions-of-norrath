#include <climits>
struct SkillRoot;
extern "C" int get_level_requirement(const SkillRoot*, int) asm("getLevelRequirement__9SkillRooti");
int main() {
    const SkillRoot* p = reinterpret_cast<const SkillRoot*>(static_cast<unsigned long>(0x1234));
    const int v[] = {INT_MIN, -1, 0, 1, 2, 50, INT_MAX};
    for (int n : v) if (get_level_requirement(p, n) != 25) return 1;
    return 0;
}
