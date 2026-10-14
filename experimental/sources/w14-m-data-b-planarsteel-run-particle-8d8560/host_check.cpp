// Host check for planarsteel_run_particle_8d8560. Not an EE plan input; it includes the definition file.
#include <cstdint>
#include "planarsteel_run_particle_8d8560.cpp"
int main() {
    if (sizeof planarsteel_run_particle_8d8560 != 80u) return 1;
    if (reinterpret_cast<std::uintptr_t>(planarsteel_run_particle_8d8560) % 16u != 0u) return 2;
    for (unsigned i = 0; i < sizeof planarsteel_run_particle_8d8560; ++i)
        if (planarsteel_run_particle_8d8560[i] != 0) return 3;
    return 0;
}
