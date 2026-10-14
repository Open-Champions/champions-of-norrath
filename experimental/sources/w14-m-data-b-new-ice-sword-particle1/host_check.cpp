// Host check for newIceSwordParticle1. Not an EE plan input; it includes the definition file.
#include <cstdint>
#include "newIceSwordParticle1.cpp"
int main() {
    if (sizeof newIceSwordParticle1 != 80u) return 1;
    if (reinterpret_cast<std::uintptr_t>(newIceSwordParticle1) % 16u != 0u) return 2;
    for (unsigned i = 0; i < sizeof newIceSwordParticle1; ++i)
        if (newIceSwordParticle1[i] != 0) return 3;
    return 0;
}
