// Host contract for soulParticle. Not an EE plan input.
extern "C" unsigned char soulParticle[80];
int main() {
    if (sizeof soulParticle != 80u) return 1;
    for (unsigned i = 0; i < sizeof soulParticle; ++i)
        if (soulParticle[i] != 0) return 2;
    return 0;
}
