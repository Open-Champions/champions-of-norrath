// Host check for oniball_msg_run_particle_7d78b0. Not an EE plan input; it includes the definition file.
#include <cstdint>
#include "oniball_msg_run_particle_7d78b0.cpp"
int main() {
    if (sizeof oniball_msg_run_particle_7d78b0 != 80u) return 1;
    if (reinterpret_cast<std::uintptr_t>(oniball_msg_run_particle_7d78b0) % 16u != 0u) return 2;
    for (unsigned i = 0; i < sizeof oniball_msg_run_particle_7d78b0; ++i)
        if (oniball_msg_run_particle_7d78b0[i] != 0) return 3;
    return 0;
}
