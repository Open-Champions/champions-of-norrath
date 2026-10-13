#include <climits>
#include <cstring>
struct SkillDiseaseShield;
extern "C" float get_energy_cost(const SkillDiseaseShield*, int) asm("getEnergyCost__18SkillDiseaseShieldi");
int main() {
    const SkillDiseaseShield* p = reinterpret_cast<const SkillDiseaseShield*>(static_cast<unsigned long>(0x1234));
    volatile float scale = 0x1.000046p+2f; volatile float base = 43.0f;
    const int v[] = {INT_MIN, -100, -1, 0, 1, 2, 3, 10, 40, 1000, INT_MAX};
    for (int n : v) {
        float want = static_cast<float>(static_cast<int>(static_cast<unsigned>(n) - 1u)) * scale + base;
        float got = get_energy_cost(p, n);
        if (std::memcmp(&want, &got, 4) != 0) return 1;
    }
    float one = get_energy_cost(p, 1);
    float b = base;
    if (std::memcmp(&one, &b, 4) != 0) return 2;
    return 0;
}
