#include <climits>
#include <cstdio>
struct Skill;
extern "C" float get_energy_cost(const Skill*, int) asm("getEnergyCost__5Skilli");
int main() {
    const Skill* receiver = reinterpret_cast<const Skill*>(static_cast<unsigned long>(0x1234));
    const int values[] = {INT_MIN, -1, 0, 1, INT_MAX};
    for (int value : values) {
        if (get_energy_cost(receiver, value) != 1.0f) {
            std::fprintf(stderr, "energy cost differs for %d\n", value);
            return 1;
        }
    }
    return 0;
}
