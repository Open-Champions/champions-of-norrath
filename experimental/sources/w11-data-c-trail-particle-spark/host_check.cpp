// Host contract for trailParticleSpark. Not an EE plan input.
extern "C" unsigned char trailParticleSpark[80];
int main() {
    if (sizeof trailParticleSpark != 80u) return 1;
    for (unsigned i = 0; i < sizeof trailParticleSpark; ++i)
        if (trailParticleSpark[i] != 0) return 2;
    return 0;
}
