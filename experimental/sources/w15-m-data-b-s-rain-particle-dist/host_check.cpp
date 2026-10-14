// Host check for sRainParticleDist. Not an EE plan input; it includes the definition file.
#include <cstdint>
#include "sRainParticleDist.cpp"
int main() {
    if (sizeof sRainParticleDist != 600u) return 1;
    if (reinterpret_cast<std::uintptr_t>(&sRainParticleDist) % 1u != 0u) return 2;
    const unsigned char* bytes = reinterpret_cast<const unsigned char*>(&sRainParticleDist);
    for (unsigned i = 0; i < sizeof sRainParticleDist; ++i)
        if (bytes[i] != 0) return 3;
    return 0;
}
