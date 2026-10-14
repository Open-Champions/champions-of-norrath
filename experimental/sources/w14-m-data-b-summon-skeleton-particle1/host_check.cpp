// Host check for summonskeletonParticle1. Not an EE plan input; it includes the definition file.
#include <cstdint>
#include "summonskeletonParticle1.cpp"
int main() {
    if (sizeof summonskeletonParticle1 != 80u) return 1;
    if (reinterpret_cast<std::uintptr_t>(summonskeletonParticle1) % 16u != 0u) return 2;
    for (unsigned i = 0; i < sizeof summonskeletonParticle1; ++i)
        if (summonskeletonParticle1[i] != 0) return 3;
    return 0;
}
