// Host check for oniball_msg_run_particle_7d7810. Not an EE plan input; it includes the definition file.
#include <cstdint>
#include "oniball_msg_run_particle_7d7810.cpp"
int main() {
    if (sizeof oniball_msg_run_particle_7d7810 != 80u) return 1;
    if (reinterpret_cast<std::uintptr_t>(oniball_msg_run_particle_7d7810) % 16u != 0u) return 2;
    for (unsigned i = 0; i < sizeof oniball_msg_run_particle_7d7810; ++i)
        if (oniball_msg_run_particle_7d7810[i] != 0) return 3;
    return 0;
}
