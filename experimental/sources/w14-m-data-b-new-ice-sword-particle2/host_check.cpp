// Host check for newIceSwordParticle2. Not an EE plan input; it includes the definition file.
#include <cstdint>
#include "newIceSwordParticle2.cpp"
int main() {
    if (sizeof newIceSwordParticle2 != 80u) return 1;
    if (reinterpret_cast<std::uintptr_t>(newIceSwordParticle2) % 16u != 0u) return 2;
    for (unsigned i = 0; i < sizeof newIceSwordParticle2; ++i)
        if (newIceSwordParticle2[i] != 0) return 3;
    return 0;
}
