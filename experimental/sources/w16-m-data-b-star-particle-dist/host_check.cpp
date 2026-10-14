// Host check for mStarParticleDist. Not an EE plan input; it includes the definition file.
#include "mStarParticleDist.cpp"
int main() {
    if (sizeof mStarParticleDist != 384u) return 1;
    for (unsigned char byte : mStarParticleDist) if (byte != 0) return 2;
    return 0;
}
