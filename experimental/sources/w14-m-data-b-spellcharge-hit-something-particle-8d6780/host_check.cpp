// Host check for spellcharge_hit_something_particle_8d6780. Not an EE plan input; it includes the definition file.
#include <cstdint>
#include "spellcharge_hit_something_particle_8d6780.cpp"
int main() {
    if (sizeof spellcharge_hit_something_particle_8d6780 != 80u) return 1;
    if (reinterpret_cast<std::uintptr_t>(spellcharge_hit_something_particle_8d6780) % 16u != 0u) return 2;
    for (unsigned i = 0; i < sizeof spellcharge_hit_something_particle_8d6780; ++i)
        if (spellcharge_hit_something_particle_8d6780[i] != 0) return 3;
    return 0;
}
