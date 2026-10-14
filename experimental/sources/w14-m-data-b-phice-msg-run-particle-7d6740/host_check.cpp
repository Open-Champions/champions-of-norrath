// Host check for phice_msg_run_particle_7d6740. Not an EE plan input; it includes the definition file.
#include <cstdint>
#include "phice_msg_run_particle_7d6740.cpp"
int main() {
    if (sizeof phice_msg_run_particle_7d6740 != 80u) return 1;
    if (reinterpret_cast<std::uintptr_t>(phice_msg_run_particle_7d6740) % 16u != 0u) return 2;
    for (unsigned i = 0; i < sizeof phice_msg_run_particle_7d6740; ++i)
        if (phice_msg_run_particle_7d6740[i] != 0) return 3;
    return 0;
}
