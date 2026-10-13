// Host check only. Not an EE build input. Build with the unit beside it:
//   c++ -std=c++17 -Wall -Wextra -Werror -DW13_HOST host_check.cpp get_energy_cost.cpp
#include <climits>
struct SkillFireStorm;
extern "C" float get_energy_cost(const SkillFireStorm*, int) asm("getEnergyCost__14SkillFireStormi");
int main() {
    const SkillFireStorm* receiver = reinterpret_cast<const SkillFireStorm*>(static_cast<unsigned long>(0x1234));
    const int ranks[] = {INT_MIN, -1000, -1, 0, 1, 2, 7, 1000, INT_MAX};
    const float step = 6.000004768371582f;
    for (int rank : ranks) {
        const float expected = static_cast<float>(static_cast<int>(static_cast<unsigned>(rank) - 1u)) * step + 120.0f;
        if (get_energy_cost(receiver, rank) != expected) return 1;
    }
    if (get_energy_cost(receiver, 1) != 120.0f) return 2;
    if (get_energy_cost(receiver, 6) != 150.000031f) return 3;
    return 0;
}
