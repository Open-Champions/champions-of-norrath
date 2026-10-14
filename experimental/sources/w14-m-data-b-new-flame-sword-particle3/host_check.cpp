// Host check for newFlameSwordParticle3. Not an EE plan input; it includes the definition file.
#include <cstdint>
#include "newFlameSwordParticle3.cpp"
int main() {
    if (sizeof newFlameSwordParticle3 != 80u) return 1;
    if (reinterpret_cast<std::uintptr_t>(newFlameSwordParticle3) % 16u != 0u) return 2;
    for (unsigned i = 0; i < sizeof newFlameSwordParticle3; ++i)
        if (newFlameSwordParticle3[i] != 0) return 3;
    return 0;
}
