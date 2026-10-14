// Host check for newFlameSwordParticle2. Not an EE plan input; it includes the definition file.
#include <cstdint>
#include "newFlameSwordParticle2.cpp"
int main() {
    if (sizeof newFlameSwordParticle2 != 80u) return 1;
    if (reinterpret_cast<std::uintptr_t>(newFlameSwordParticle2) % 16u != 0u) return 2;
    for (unsigned i = 0; i < sizeof newFlameSwordParticle2; ++i)
        if (newFlameSwordParticle2[i] != 0) return 3;
    return 0;
}
