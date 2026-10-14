// Host check for norrath_container_msg_run_smokeParticle_bytes. Not an EE plan input; it includes the definition file.
#include <cstdint>
#include "norrath_container_msg_run_smokeParticle_bytes.cpp"
int main() {
    if (sizeof norrath_container_msg_run_smokeParticle_bytes != 80u) return 1;
    if (reinterpret_cast<std::uintptr_t>(norrath_container_msg_run_smokeParticle_bytes) % 16u != 0u) return 2;
    for (unsigned i = 0; i < sizeof norrath_container_msg_run_smokeParticle_bytes; ++i)
        if (norrath_container_msg_run_smokeParticle_bytes[i] != 0) return 3;
    return 0;
}
