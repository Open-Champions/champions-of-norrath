// Host check only. Not an EE build input. Build with the unit beside it:
//   c++ -std=c++17 -Wall -Wextra -Werror -DW13_HOST host_check.cpp get_energy_cost.cpp
#include <climits>
#include <cstring>
struct SkillUndeadShield;
extern "C" float get_energy_cost(const SkillUndeadShield*, int) asm("getEnergyCost__17SkillUndeadShieldi");
int main() {
    const SkillUndeadShield* receiver = reinterpret_cast<const SkillUndeadShield*>(static_cast<unsigned long>(0x1234));
    const int ranks[] = {INT_MIN, -1000, -1, 0, 1, 2, 7, 1000, INT_MAX};
    const unsigned bits = 0x40800034u;
    float step;
    std::memcpy(&step, &bits, sizeof step);
    for (int rank : ranks) {
        const float expected = static_cast<float>(static_cast<int>(static_cast<unsigned>(rank) - 1u)) * step + 41.0f;
        if (get_energy_cost(receiver, rank) != expected) return 1;
    }
    if (get_energy_cost(receiver, 1) != 41.0f) return 2;
    return 0;
}
