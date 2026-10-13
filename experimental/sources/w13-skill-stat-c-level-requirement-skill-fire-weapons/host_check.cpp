// Host check only. Not an EE build input. Build with the unit beside it:
//   c++ -std=c++17 -Wall -Wextra -Werror -DW13_HOST host_check.cpp get_level_requirement.cpp
#include <climits>
struct SkillFireWeapons;
extern "C" int get_level_requirement(const SkillFireWeapons*, int) asm("getLevelRequirement__16SkillFireWeaponsi");
int main() {
    const SkillFireWeapons* receiver = reinterpret_cast<const SkillFireWeapons*>(static_cast<unsigned long>(0x1234));
    const int values[] = {INT_MIN, -1000, -2, -1, 0, 1, 2, 3, 7, 25, 1000, INT_MAX};
    for (int n : values)
        if (get_level_requirement(receiver, n) != 20) return 1;
    return 0;
}
