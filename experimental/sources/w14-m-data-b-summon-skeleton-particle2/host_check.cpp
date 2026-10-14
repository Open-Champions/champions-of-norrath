// Host check for summonskeletonParticle2. Not an EE plan input; it includes the definition file.
#include <cstdint>
#include "summonskeletonParticle2.cpp"
int main() {
    if (sizeof summonskeletonParticle2 != 80u) return 1;
    if (reinterpret_cast<std::uintptr_t>(summonskeletonParticle2) % 16u != 0u) return 2;
    for (unsigned i = 0; i < sizeof summonskeletonParticle2; ++i)
        if (summonskeletonParticle2[i] != 0) return 3;
    return 0;
}
