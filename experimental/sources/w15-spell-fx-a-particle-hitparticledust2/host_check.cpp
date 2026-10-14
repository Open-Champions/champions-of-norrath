// Host contract for hitParticleDust2. Not an EE plan input.
extern "C" unsigned char hitParticleDust2[80];
int main() {
    if (sizeof hitParticleDust2 != 80u) return 1;
    for (unsigned i = 0; i < sizeof hitParticleDust2; ++i)
        if (hitParticleDust2[i] != 0) return 2;
    return 0;
}
