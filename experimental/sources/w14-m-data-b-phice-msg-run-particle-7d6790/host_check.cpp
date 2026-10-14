// Host check for phice_msg_run_particle_7d6790. Not an EE plan input; it includes the definition file.
#include <cstdint>
#include "phice_msg_run_particle_7d6790.cpp"
int main() {
    if (sizeof phice_msg_run_particle_7d6790 != 80u) return 1;
    if (reinterpret_cast<std::uintptr_t>(phice_msg_run_particle_7d6790) % 16u != 0u) return 2;
    for (unsigned i = 0; i < sizeof phice_msg_run_particle_7d6790; ++i)
        if (phice_msg_run_particle_7d6790[i] != 0) return 3;
    return 0;
}
