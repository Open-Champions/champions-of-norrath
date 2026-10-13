// Host check only. Not an EE build input. Build with the unit beside it:
//   c++ -std=c++17 -Wall -Wextra -Werror -DW13_HOST host_check.cpp get_energy_cost.cpp
#include <climits>
struct SkillWizardBeam;
extern "C" float get_energy_cost(const SkillWizardBeam*, int) asm("getEnergyCost__15SkillWizardBeami");
int main() {
    const SkillWizardBeam* receiver = reinterpret_cast<const SkillWizardBeam*>(static_cast<unsigned long>(0x1234));
    const int ranks[] = {INT_MIN, -1000, -1, 0, 1, 2, 7, 1000, INT_MAX};
    const float step = __builtin_bit_cast(float, 0x40000021u);
    for (int rank : ranks) {
        const float expected = static_cast<float>(static_cast<int>(static_cast<unsigned>(rank) - 1u)) * step + 22.0f;
        if (get_energy_cost(receiver, rank) != expected) return 1;
    }
    if (get_energy_cost(receiver, 1) != 22.0f) return 2;
    if (get_energy_cost(receiver, 2) != 22.0f + step) return 3;
    return 0;
}
