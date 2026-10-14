// Host contract for lampParticle. Not an EE plan input.
extern "C" unsigned char lampParticle[80];
int main() {
    if (sizeof lampParticle != 80u) return 1;
    for (unsigned i = 0; i < sizeof lampParticle; ++i)
        if (lampParticle[i] != 0) return 2;
    return 0;
}
