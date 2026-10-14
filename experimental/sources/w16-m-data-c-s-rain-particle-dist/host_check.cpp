// Host check for sRainParticleDist. Not an EE plan input; it includes the definition file.
#include "sRainParticleDist.cpp"
int main() {
    if (sizeof sRainParticleDist != 600u) return 1;
    for (unsigned i = 0; i < sizeof sRainParticleDist; ++i)
        if (sRainParticleDist[i] != 0) return 2;
    return 0;
}
