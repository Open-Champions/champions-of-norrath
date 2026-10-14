// Host contract for lavaGoopParticle. Not an EE plan input.
extern "C" unsigned char lavaGoopParticle[80];
int main() {
    if (sizeof lavaGoopParticle != 80u) return 1;
    for (unsigned i = 0; i < sizeof lavaGoopParticle; ++i)
        if (lavaGoopParticle[i] != 0) return 2;
    return 0;
}
