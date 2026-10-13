// Host check only. Not an EE build input. Build with the unit beside it:
//   c++ -std=c++17 -Wall -Wextra -Werror -DW13_HOST host_check.cpp get_skill_point_cost.cpp
#include <climits>
struct SkillDarkness;
extern "C" int get_skill_point_cost(const SkillDarkness*, int) asm("getSkillPointCost__13SkillDarknessi");
int main() {
    const SkillDarkness* receiver = reinterpret_cast<const SkillDarkness*>(static_cast<unsigned long>(0x1234));
    const int values[] = {INT_MIN, -1000, -2, -1, 0, 1, 2, 3, 7, 25, 1000, INT_MAX};
    for (int n : values)
        if (get_skill_point_cost(receiver, n) != static_cast<int>(static_cast<unsigned>(n) * 3u)) return 1;
    return 0;
}
